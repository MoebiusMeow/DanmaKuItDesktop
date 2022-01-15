#include "View/MainWindow.h"

#include <QApplication>
#include <QLocale>
#include <QTranslator>

int main(int argc, char *argv[])
{
    auto sslConf = QSslConfiguration::defaultConfiguration();
    qDebug() << "SSL Library:" << QSslSocket::sslLibraryVersionString();
    // install Let's Encrypt ISRG Root X1 certificate to avoid related issues
    sslConf.addCaCertificates(":/Assets/Certificates/isrgrootx1.pem");
    QSslConfiguration::setDefaultConfiguration(sslConf);

    QApplication a(argc, argv);
    a.setOrganizationDomain("danmakuit.panda2134.site");
    a.setApplicationName("DanmakuIt");

    QTranslator translator;
    const QStringList uiLanguages = QLocale::system().uiLanguages();
    for (const QString &locale : uiLanguages)
    {
        const QString baseName = "DanmaKuItDesktop_" + QLocale(locale).name();
        if (translator.load(":/i18n/" + baseName))
        {
            a.installTranslator(&translator);
            break;
        }
    }

    MainWindow w;
    w.show();
    return a.exec();
}
