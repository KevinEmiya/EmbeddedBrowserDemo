#include "QCefInspector.h"

#include <QJsonArray>
#include <QJsonObject>
#include <QJsonDocument>


QCefInspector::QCefInspector(CefRefPtr<QCefClient> cefClient, QWidget *parent) : QCefView(cefClient, parent)
{
    m_nam = new QNetworkAccessManager(this);
    connect(m_nam, SIGNAL(finished(QNetworkReply*)), this, SLOT(onNamLoadFinished(QNetworkReply*)));

    m_inspectUrl = QUrl::fromUserInput("localhost:7777");
}

QCefInspector::~QCefInspector()
{

}

void QCefInspector::inspect()
{
    m_nam->get(QNetworkRequest(m_inspectUrl.resolved(QUrl("json/list"))));
}

void QCefInspector::onNamLoadFinished(QNetworkReply* reply)
{
    QJsonArray viewList = QJsonDocument::fromJson(reply->readAll()).array();
    if (viewList.size() > 1)
    {
        QJsonObject object = viewList.at(1).toObject();
        QUrl pageUrl = m_inspectUrl.resolved(QUrl(object.value(QStringLiteral("devtoolsFrontendUrl")).toString()));
        load(pageUrl);
    }
}
