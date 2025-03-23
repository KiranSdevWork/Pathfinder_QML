#include <QApplication>
#include <QQmlApplicationEngine>
#include "pathmodel.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    qmlRegisterType<PathModel>("PathModel", 1, 0, "PathModel");

    QQmlApplicationEngine engine;
    engine.load(QUrl(QStringLiteral("../../../../../Main.qml")));
    return a.exec();
}
