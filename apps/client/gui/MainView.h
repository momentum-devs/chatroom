#include <QObject>
#include <QString>

namespace client::gui
{
class MainView : public QObject
{
    Q_OBJECT

public slots:
    void handleRegisterRequest();

signals:
    void onRegisterRequest();
};
}