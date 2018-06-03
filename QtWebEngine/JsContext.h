#ifndef JSCONTEXT_H
#define JSCONTEXT_H

#include <QObject>
#include <QWebEnginePage>

class JsContext : public QObject
{
    Q_OBJECT
public:
    explicit JsContext(QObject *parent = nullptr);

signals:
    void recvdMsg(const QString& msg);

public:
    void sendMsg(QWebEnginePage* page, const QString& msg);

public slots:
    void onMsg(const QString& msg);
};

#endif // JSCONTEXT_H
