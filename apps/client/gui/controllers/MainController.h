#pragma once

#include <memory>
#include <QObject>
#include <QString>

#include "client/api/Session.h"

namespace client::gui
{
class MainController : public QObject
{
    Q_OBJECT

public:
    MainController(std::shared_ptr<api::Session> session);

    Q_INVOKABLE void handleRegisterRequest(const QString& email, const QString& password);

signals:
    void registerRequest(const QString& email, const QString& password);

private:
    std::shared_ptr<api::Session> session;
};
}