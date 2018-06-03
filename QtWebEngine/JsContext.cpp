#include "JsContext.h"

JsContext::JsContext(QObject *parent) : QObject(parent)
{
}

void JsContext::onMsg(const QString &msg)
{
    emit recvdMsg(msg);
}

void JsContext::sendMsg(QWebEnginePage* page, const QString& msg)
{
    page->runJavaScript(QString("recvMessage('%1');").arg(msg));
}
