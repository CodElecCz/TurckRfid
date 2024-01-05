#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QButtonGroup>
#include <QHttpServer>
#include <QHttpServerResponse>

#include "centralview.h"
#include "twincatads.h"
#include "support/MainWindowExt.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class TwinCatAdsView;
class SettingsView;
class LogView;
class CentralView;

class MainWindow : public MainWindowExt
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    int AdsOpen();
    int AdsClose();

    int HttpServerOpen();

    void initializeMenuBar();
    void initializeToolBar();
    void initializeStatusBar();

    void setView(CentralView::EView view);

    void createLogWindow(const QString& name, bool showWindow);

    const QString handleRequest (const QHttpServerRequest &request);

private slots:
    void toolBarButtons_buttonClicked(int);

private:
    Ui::MainWindow *ui;

    TwinCatAdsView* m_adsView;
    HttpView*       m_httpView;
    SettingsView*   m_settingsView;

    CentralView*    m_centralView;

    LogView*        m_logView;

    QButtonGroup*       m_toolBarButtons;
    CentralView::EView  m_lastView;

    TwinCatAds*     m_ads;
    QHttpServer*    m_httpServer;
};
#endif // MAINWINDOW_H
