
#include "WDLogin.h"
#include "MTNetwork.h"
#include <QStyleFactory>

int main(int argc, char *argv[])
{
    QCoreApplication::setApplicationName( QString("Mailto") );
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);


    QApplication a(argc, argv);
    qDebug() << QStyleFactory::keys();
    a.setStyle(QStyleFactory::create("macintosh"));

    // 高分屏支持
    qDebug() << QCoreApplication::applicationDirPath();

    WDLogin w;
    w.show();
    return a.exec();
}
