#include "LoaderController.h"

#include <QQmlApplicationEngine>
#include <QQmlContext>

namespace client::gui
{
LoaderController::LoaderController()
{
    view.engine()->rootContext()->setContextProperty(componentName, this);

    view.setSource(qUrl);

    view.setResizeMode(QQuickView::SizeRootObjectToView);

    view.show();
}

void LoaderController::callLoadView(const QUrl& qUrl)
{
    emit loadView(qUrl);
}

QQmlEngine* LoaderController::getEngine() const
{
    return view.engine();
}
}
