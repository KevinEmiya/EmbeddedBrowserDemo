#ifndef QSSLPAGE_H
#define QSSLPAGE_H

#include <QObject>
#include <QWebEnginePage>

#include <QList>

class QSslPage : public QWebEnginePage
{
    Q_OBJECT
  public:
    explicit QSslPage(QObject* parent = nullptr);

  protected:
    bool certificateError(const QWebEngineCertificateError& certificateError);

  private:
    static QList<QWebEngineCertificateError::Error> m_allowedError;
};
#endif // QSSLPAGE_H
