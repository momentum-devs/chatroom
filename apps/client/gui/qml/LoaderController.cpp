#include "LoaderController.h"

#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <QQuickItem>

namespace client::gui
{
LoaderController::LoaderController()
{
    view.engine()->rootContext()->setContextProperty(componentName, this);

    view.setSource(qUrl);

    view.setResizeMode(QQuickView::SizeRootObjectToView);
    
    view.setMinimumWidth(view.rootObject()->property("minWidth").toInt());

    view.setMinimumHeight(view.rootObject()->property("minHeight").toInt());

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
