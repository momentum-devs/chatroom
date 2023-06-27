#pragma once

#include <QObject>
#include <QQuickView>
#include <QString>

namespace client::gui
{
class LoaderController : public QObject
{
    Q_OBJECT

public:
    LoaderController();

    void callLoadView(const QUrl& qUrl);

    QQmlEngine* getEngine() const;

signals:
    void loadView(const QUrl& qUrl);

private:
    inline static const QString componentName{"loaderController"};
    inline static const QUrl qUrl{QUrl::fromLocalFile("chatroom/gui/qml/MainView.qml")};
    QQuickView view;
};
}