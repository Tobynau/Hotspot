#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include "HotspotClient.h"
#include <QQuickStyle>

int main(int argc, char **argv)
{
    QGuiApplication app(argc, argv);
    QQuickStyle::setStyle("Material");
    QQmlApplicationEngine engine;

    HotspotClient client;
    engine.rootContext()->setContextProperty("HotspotClient", &client);

    engine.load(QUrl::fromLocalFile("src/gui/qml/Main.qml"));
    if (engine.rootObjects().isEmpty())
        return -1;
    return app.exec();
}
