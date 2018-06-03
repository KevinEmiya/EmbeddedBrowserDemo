#ifndef QCEFVIEW_H
#define QCEFVIEW_H

#include <QWidget>
#include <QTimer>
#include <QStackedLayout>
#include <QWindow>
#include <QUrl>
#include <QString>

#include "QCefApp.h"

class QCefView : public QWidget
{
    Q_OBJECT
public:
    explicit QCefView(CefRefPtr<QCefClient> cefClient, QWidget *parent = 0);
    virtual ~QCefView();

signals:
    void cefEmbedded();
    void loadStarted(bool isMainFrame);
    void loadFinished(bool ok, bool isMainFrame);
    void loadError(QString errorStr);
    void webMsgReceived(QString msg);
    void keyEvent(Qt::Key key);

protected slots:
    virtual void onCefTimer();

public:
    void load(QUrl url);
    void runJavaScript(QString script);
    void sendToWeb(QString msg);
    void checkPage();
    void reload();

protected:
    CefRefPtr<QCefClient> m_cefClient;
    bool m_cefEmbedded;
    QTimer* m_cefTimer;
};

#endif // QCEFVIEW_H
