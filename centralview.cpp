#include "centralview.h"
#include "http/httpview.h"
#include "settings/SettingsView.h"

#include "ui_centralview.h"

CentralView::CentralView(HttpView* httpView,
                         SettingsView *settingsView,
                         QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::CentralView)
{
    ui->setupUi(this);

    ui->stackedWidget->addWidget(httpView);
    ui->stackedWidget->addWidget(settingsView);
}

CentralView::~CentralView()
{
    delete ui;
}

void CentralView::setView(EView view)
{
    switch(view)
    {
    case EView_Http:
        ui->stackedWidget->setCurrentIndex(0);
        break;
    case EView_Settings:
        ui->stackedWidget->setCurrentIndex(1);
        break;
    default:
        break;
    }
}
