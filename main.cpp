#include "mainwindow.h"
#include "settings/Settings.h"
#include "log/Log.h"

#include <QtCore>
#include <QCoreApplication>
#include <QApplication>
#include <QDebug>

// Get the default Qt message handler.
static const QtMessageHandler QT_DEFAULT_MESSAGE_HANDLER = qInstallMessageHandler(0);

void customMessageHandler(QtMsgType type, const QMessageLogContext &context, const QString &msg)
{
    // Handle the messages!
    QString dt = QDateTime::currentDateTime().toString("yyyy.MM.dd hh:mm:ss");
    QString stype;
    switch (type)
    {
    case QtMsgType::QtInfoMsg:
        stype = QString("Info");
        break;
    case QtMsgType::QtDebugMsg:
        stype = QString("Debug");
        break;
    case QtMsgType::QtWarningMsg:
        stype = QString("Warning");
        break;
    case QtMsgType::QtCriticalMsg:
        stype = QString("Critical");
        break;
    case QtMsgType::QtFatalMsg:
        stype = QString("Fatal");
        break;
    }

    //to file
    QFile file(Settings::logFilePath());
    file.open(QIODevice::WriteOnly | QIODevice::Append);
    QTextStream ts(&file);

    QString logMsg = QString("%1 [%2] %3").arg(dt).arg(stype).arg(msg);
    ts << logMsg << Qt::endl;
    file.close();

    //to application
    Log::instance().message(logMsg);

    //check file size
    QFileInfo fileInfo(Settings::logFilePath());
    if(fileInfo.size()>1024*1024)
    {
        QStringList pathList = Settings::logFilePath().split(".");

        QString suffix = pathList.last();
        pathList.removeLast();

        QFileInfo newFileInfo;

        int index = 0;
        do
        {
            newFileInfo.setFile(pathList.join(".") + "_" + QString::number(index,10) + "." + suffix);
            index++;
        }while(newFileInfo.exists());

        file.rename(newFileInfo.absoluteFilePath());
    }

    // Call the default handler
    (*QT_DEFAULT_MESSAGE_HANDLER)(type, context, msg);
}

int main(int argc, char *argv[])
{
    int ret = 0;

    QCoreApplication::setAttribute(Qt::AA_UseStyleSheetPropagationInWidgetStyles, true);
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling, true);

    QApplication app(argc, argv);

    qInstallMessageHandler(customMessageHandler);

    QCoreApplication::setOrganizationName(Settings::cOrganizationName);
    QCoreApplication::setApplicationName(Settings::cApplicationName);
    QCoreApplication::setApplicationVersion(Settings::cVersion);

    qInfo() << "Application start, ver: " << Settings::cVersion;

    try
    {
        MainWindow mainWindow;
        mainWindow.setWindowTitle(QString("%1 (%2)").arg(QCoreApplication::applicationName()).arg(QCoreApplication::applicationVersion()));
        mainWindow.show();

        app.exec();
    }
    catch (QException& e)
    {
        qCritical() << "Exception: " << e.what();

        return EXIT_FAILURE;
    }
    catch(...)
    {
        qCritical() << "Unknown exception";

        return EXIT_FAILURE;
    }

    qInfo() << "Application close, return code: " << ret;

    return ret;
}
