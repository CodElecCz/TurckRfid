#include "mainwindow.h"
#include "centralview.h"
#include "settings/SettingsView.h"
#include "TcADS/twincatadsview.h"
#include "http/httpview.h"
#include "log/LogView.h"

#include "ui_mainwindow.h"

#include <QDebug>
#include <QToolButton>
#include <QByteArray>

using namespace Qt::StringLiterals;

MainWindow::MainWindow(QWidget *parent)
    : MainWindowExt(parent),
    ui(new Ui::MainWindow),

    m_adsView(new TwinCatAdsView(this)),
    m_httpView(new HttpView(this)),
    m_settingsView(new SettingsView(this)),

    m_centralView(new CentralView(m_httpView, m_settingsView, this)),

    m_logView(nullptr),

    m_toolBarButtons(new QButtonGroup(this)),
    m_lastView(CentralView::EView_none),

    m_ads(new TwinCatAds(this)),
    m_httpServer(new QHttpServer(this))
{
    ui->setupUi(this);

    this->setCentralWidget(m_centralView);

    createLogWindow("Log", true);

    initializeMenuBar();
    initializeToolBar();
    initializeStatusBar();

    initializeWindow();

    AdsOpen();
    HttpServerOpen();
}

MainWindow::~MainWindow()
{
    AdsClose();

    delete ui;
}

void MainWindow::initializeMenuBar()
{
    //style
    createStyleMenu(ui->menuStyle);

    //application close
    connect(ui->actionAbout, SIGNAL(triggered()), this, SLOT(aboutDialog()));
}

void MainWindow::initializeStatusBar()
{

}

void MainWindow::initializeToolBar()
{
    //toolBar
    QToolButton* httpButton = new QToolButton(ui->toolbar);
    httpButton->setAccessibleName("FileManager");
    httpButton->setText("HTTP\nServer");
    httpButton->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
    httpButton->setToolTip(tr("HTTP Server"));
    httpButton->setCheckable(true);
    httpButton->setChecked(true);
    httpButton->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);

    QToolButton* settingsButton = new QToolButton(ui->toolbar);
    settingsButton->setAccessibleName("Settings");
    settingsButton->setText("Settings");
    settingsButton->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
    settingsButton->setToolTip(tr("Application settings"));
    settingsButton->setCheckable(true);
    settingsButton->setChecked(false);
    settingsButton->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);

    QWidget* spacer = new QWidget();
    spacer->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Expanding);

    ui->toolbar->addWidget(httpButton);
    ui->toolbar->addWidget(spacer);
    ui->toolbar->addWidget(settingsButton);

    m_toolBarButtons->addButton(httpButton, CentralView::EView_Http);
    m_toolBarButtons->addButton(settingsButton, CentralView::EView_Settings);
    m_toolBarButtons->setExclusive(true);

    connect(m_toolBarButtons, &QButtonGroup::idClicked, this, &MainWindow::toolBarButtons_buttonClicked);

    setView(CentralView::EView_Http);
}

void MainWindow::toolBarButtons_buttonClicked(int eView)
{
    CentralView::EView view = static_cast<CentralView::EView>(eView);
    setView(view);
}

void MainWindow::setView(CentralView::EView view)
{
    //close
    if(m_lastView != view)
    {
        switch(m_lastView)
        {
        case CentralView::EView_Http:
            break;
        case CentralView::EView_Settings:
            break;
        case CentralView::EView_none:
            break;
        default:
            break;
        }

        m_lastView = view;
    }

    m_centralView->setView(view);

    //open
    switch(view)
    {
    case CentralView::EView_Http:
        break;
    case CentralView::EView_Settings:
        break;
    case CentralView::EView_none:
        break;
    default:
        break;
    }
}

void MainWindow::createLogWindow(const QString& name, bool showWindow)
{
    QDockWidget *dock = new QDockWidget(name, this);
    dock->setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea | Qt::BottomDockWidgetArea);
    dock->setFeatures(dock->features() & ~QDockWidget::DockWidgetFloatable);

    m_logView = new LogView(dock);
    m_logView->setObjectName("LogView");

    dock->setWidget(m_logView);
    dock->setObjectName(name);

    ui->menuWindow->addAction(dock->toggleViewAction());

    MainWindowExt::tabifyDockWidget(dock, name, Qt::BottomDockWidgetArea);
    //addDockWidget(Qt::LeftDockWidgetArea, dock);

    if(showWindow)
        dock->show();
    else
        dock->hide();
}

static inline QString host(const QHttpServerRequest &request)
{
    return QString::fromLatin1(request.value("Host"));
}

const QString MainWindow::handleRequest (const QHttpServerRequest &request)
{
    if(request.query().hasQueryItem("epc"))
    {
        qDebug() << request.query().queryItemValue("epc");
    }

    if(request.query().hasQueryItem("time"))
        qDebug() << request.query().queryItemValue("time");

    return host(request) + u"/findOne/"_s;
}

int MainWindow::AdsOpen()
{
    int ret = m_ads->Open();
    if(ret != 0)
    {
        m_httpView->SetStatus(QString("ADS Open Error 0x%1").arg(QString::number(m_ads->GetLastError(), 16)));
        return -1;
    }

    m_httpView->SetStatus(QString("ADS Open Port %1").arg(QString::number(m_ads->GetPort(), 10)));

    ret = m_ads->InitVariables();
    if(ret != 0)
    {
        return -1;
    }

    m_httpView->SetCounter(m_ads->GetCounter());

    return 0;
}

int MainWindow::AdsClose()
{
    int ret = m_ads->Close();
    if(ret != 0)
        return -1;

    return 0;
}

int MainWindow::HttpServerOpen()
{
    m_httpServer->route("/findOne", [=](const QHttpServerRequest &request) {

        QString httpRequest = u"/findOne?%1"_s.arg(QUrl::fromPercentEncoding(request.query().toString().toUtf8()));
        qDebug() << httpRequest;
        m_httpView->SetRequest(httpRequest);

        bool hasEpc = request.query().hasQueryItem("epc");
        bool hasTime = request.query().hasQueryItem("timestamp");
        int ads_ret = 0;

        if(hasEpc)
        {
            QString epc = QUrl::fromPercentEncoding(request.query().queryItemValue("epc").toUtf8());
            //qDebug() << epc;
            ads_ret = m_ads->SetTag(epc);
            m_httpView->SetTag(epc);
        }

        if(hasTime)
        {
            QString time = QUrl::fromPercentEncoding(request.query().queryItemValue("timestamp").toUtf8());
            //qDebug() << time;
            if(ads_ret == 0)
                ads_ret = m_ads->SetLocalTime(time);

            m_httpView->SetTime(time);
        }

        if(ads_ret == 0)
            ads_ret = m_ads->AddCounter();

        m_httpView->SetCounter(m_ads->GetCounter());

        if(ads_ret == 0)
            return host(request) + u"/findOne?%1&%2"_s.arg(hasEpc? "epc=stored":"epc=missing").arg(hasTime? "timestamp=stored":"timestamp=missing");
        else
            return host(request) + u"/findOne?ads_error=%1"_s.arg(QString::number(ads_ret));
    });

    m_httpServer->afterRequest([](QHttpServerResponse &&resp) {
        resp.setHeader("Server", "HTTP Server");
        return std::move(resp);
    });

    const auto port = m_httpServer->listen(QHostAddress::Any, 18080);
    if (!port) {
        qWarning() << QCoreApplication::translate("HTTP Server",
                                                  "Server failed to listen on a port.");
        return -1;
    }

    qInfo().noquote() << QCoreApplication::translate("HTTP Server",
                                                     "Running on http://127.0.0.1:%1/").arg(port);

    return 0;
}
