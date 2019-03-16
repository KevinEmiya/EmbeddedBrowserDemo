#include "QCefSslContext.h"

#include <QDir>
#include <QDebug>
#include <QSslConfiguration>

QCefSslContext::QCefSslContext(QString certPath, QObject *parent) : QObject(parent)
{
    m_valid = false;
    m_certPath = certPath;
    init();
}

QCefSslContext::QCefSslContext(QObject *parent) : QObject(parent)
{
    m_valid = false;
    m_certPath = QDir::currentPath();
    init();
}

QCefSslContext::~QCefSslContext()
{

}

void QCefSslContext::init()
{
    QSslConfiguration config = QSslConfiguration::defaultConfiguration();

    config.setProtocol(QSsl::AnyProtocol);

    QFile certFile(m_certPath + QString("/sslCert/client.p12"));
    m_valid = certFile.open(QIODevice::ReadOnly);
    m_valid &= QSslCertificate::importPkcs12(&certFile, &m_privateKey, &m_certificate, nullptr, QByteArray("password"));

    QFile peerFile(m_certPath + QString("/sslCert/server_cert.pem"));
    m_valid &= peerFile.open(QIODevice::ReadOnly);
    QSslCertificate peerCert(peerFile.readAll(), QSsl::Pem);

    bool peerCertValid = !peerCert.isNull();
    m_valid &= peerCertValid;
    qDebug()<<"[QCefSslContext]peer cert valid = " << peerCertValid;
    QList<QSslCertificate> caCerts;
    caCerts << peerCert;
    config.setCaCertificates(caCerts);
    config.setPeerVerifyMode(QSslSocket::VerifyPeer);
    config.setPeerVerifyDepth(1);
    m_caCertificates = caCerts;

    QSslConfiguration::setDefaultConfiguration(config);
}

QSslCertificate QCefSslContext::certificate() const
{
    return m_certificate;
}

QSslKey QCefSslContext::privateKey() const
{
    return m_privateKey;
}

QList<QSslCertificate> QCefSslContext::caCertificates() const
{
    return m_caCertificates;
}

bool QCefSslContext::isValid()
{
    return m_valid;
}
