#include "mainwindow.h"

#include <QApplication>
#include <QFile>
#include <QImageReader>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);


    QFile f(":/themes/default.qss");
    if (f.open(QFile::ReadOnly)) {
        QString style = QLatin1String(f.readAll());
        a.setStyleSheet(style);
    } else {
        qDebug() << "Failed to open stylesheet:" << f.errorString();
    }

    MainWindow w;
    w.show();
    return a.exec();
}
