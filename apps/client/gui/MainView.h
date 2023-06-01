#pragma once

#include <memory>

#include <QObject>
#include <QString>

#include "api/Session.h"

namespace client::gui
{
class MainView : public QObject
{
    Q_OBJECT

public:
    MainView(std::shared_ptr<api::Session> session);
    Q_INVOKABLE void handleRegisterRequest(const QString& username, const QString& password);

signals:
    void registerRequest(const QString& username, const QString& password);

private:
    std::shared_ptr<api::Session> session;
};
}