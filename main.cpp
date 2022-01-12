#include "View/MainWindow.h"

#include <QApplication>
#include <QLocale>
#include <QTranslator>

int main(int argc, char *argv[])
{
    auto sslConf = QSslConfiguration::defaultConfiguration();
    qDebug() << QSslSocket::sslLibraryVersionString();
    sslConf.addCaCertificates(":/Assets/Certificates/isrgrootx1.pem");
    sslConf.setProtocol(QSsl::TlsV1_3);
    QSslConfiguration::setDefaultConfiguration(sslConf);

    QApplication a(argc, argv);

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
