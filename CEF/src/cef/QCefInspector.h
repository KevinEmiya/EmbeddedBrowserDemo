#ifndef QCEFINSPECTOR_H
#define QCEFINSPECTOR_H

#include <QNetworkAccessManager>
#include <QNetworkReply>

#include "QCefView.h"

class QCefInspector : public QCefView
{
    Q_OBJECT
public:
    explicit QCefInspector(CefRefPtr<QCefClient> cefClient, QWidget *parent = 0);
    ~QCefInspector();

    void inspect();

private slots:
    void onNamLoadFinished(QNetworkReply* reply);

private:
    QUrl m_inspectUrl;
    QNetworkAccessManager* m_nam;
};

#endif // QCEFINSPECTOR_H
