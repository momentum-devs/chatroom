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

    void callLoadView(const QUrl& qUrl, std::function<void()> callbackOnFinished = nullptr);

    QQmlEngine* getEngine() const;

    Q_INVOKABLE void loaded();

signals:
    void loadView(const QUrl& qUrl);

private:
    inline static const QString componentName{"loaderController"};
    inline static const QUrl qUrl{QUrl::fromLocalFile("chatroom/gui/qml/LoaderView.qml")};
    QQuickView view;
    std::function<void()> callbackOnFinished;
};
}