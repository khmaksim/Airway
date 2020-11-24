#include "mainwindow.h"
#include <QApplication>
#include <QTranslator>
#include <QDebug>
#include <QSslSocket>
#include <QStyleFactory>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    QCoreApplication::setOrganizationName("Aviacominfo");
    QCoreApplication::setOrganizationDomain("aviacominfo.com");
    QCoreApplication::setApplicationName("Airway");
    QApplication::setStyle(QStyleFactory::create("windowsvista"));
//    qDebug() << QSslSocket::supportsSsl() << QSslSocket::sslLibraryBuildVersionString() << QSslSocket::sslLibraryVersionString();

    QTranslator translator;
    if (translator.load(QLocale(), QLatin1String("airway"), QLatin1String("_"), QLatin1String(":/translations/res/translations")))
        QApplication::installTranslator(&translator);

    MainWindow w;
    w.show();

    return QApplication::exec();
}
