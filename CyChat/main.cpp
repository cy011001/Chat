#include "mainwindow.h"
#include <QApplication>
#include <QFile>
#include <QDebug>
#include "global.h"
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    //加载qss设置样式
    QFile qss(":style/stylesheet.qss");
    if(qss.open(QFile::ReadOnly)){
        qDebug("open success");
        //qbytearray -> string
        QString style = QLatin1String(qss.readAll());
        a.setStyleSheet(style);
        qss.close();
    }
    else{
        qDebug("open failed");
    }

    QString app_path= QApplication::applicationDirPath();
    QString file_name = "config.ini";
    QString config_path = QDir::toNativeSeparators(app_path + QDir::separator() + file_name);

    QSettings settings(config_path, QSettings::IniFormat);
    QString gate_host = settings.value("GateServer/host").toString();
    QString gate_port = settings.value("GateServer/port").toString();
    gate_url_prefix = "http://" + gate_host + ":" + gate_port;

    MainWindow w;
    w.show();

    return a.exec();
}
