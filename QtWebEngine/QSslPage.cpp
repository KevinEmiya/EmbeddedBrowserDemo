#include "QSslPage.h"

QList<QWebEngineCertificateError::Error> QSslPage::m_allowedError =
    QList<QWebEngineCertificateError::Error>()
    << QWebEngineCertificateError::Error::CertificateAuthorityInvalid;

QSslPage::QSslPage(QObject* parent) : QWebEnginePage(parent)
{
}

bool QSslPage::certificateError(const QWebEngineCertificateError& certificateError)
{
    if (m_allowedError.contains(certificateError.error()))
    {
        return true;
    }
    else
    {
        qDebug() << "[cert error]" << certificateError.errorDescription();
        return QWebEnginePage::certificateError(certificateError);
    }
}
