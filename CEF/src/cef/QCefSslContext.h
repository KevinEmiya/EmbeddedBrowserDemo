#ifndef QCEFSSLCONTEXT_H
#define QCEFSSLCONTEXT_H

#include <QObject>
#include <QSslCertificate>
#include <QSslKey>

class QCefSslContext : public QObject
{
    Q_OBJECT
public:
    explicit QCefSslContext(QString certPath, QObject *parent = 0);
    explicit QCefSslContext(QObject *parent = 0);
    virtual ~QCefSslContext();

public:
    QSslCertificate certificate() const;
    QSslKey privateKey() const;
    QList<QSslCertificate> caCertificates() const;
    bool isValid();

private:
    void init();

private:
    QSslCertificate m_certificate; //local certificate
    QSslKey m_privateKey; //local private key
    QList<QSslCertificate> m_caCertificates;
    bool m_valid;
    QString m_certPath;
};

#endif // QCEFSSLCONTEXT_H
