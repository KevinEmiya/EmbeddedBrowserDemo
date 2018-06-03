#ifndef QCEFAPP_H
#define QCEFAPP_H

#include "include/cef_app.h"
#include "include/cef_browser.h"
#include "include/cef_command_line.h"
#include "include/wrapper/cef_helpers.h"

#include "QCefClient.h"
#include <QQueue>
#include <QSslCertificate>

class QCefApp: public CefApp,
        public CefBrowserProcessHandler
{
public:
    QCefApp();
    virtual ~QCefApp();

    // CefApp methods:
    virtual CefRefPtr<CefBrowserProcessHandler> GetBrowserProcessHandler()
        OVERRIDE { return this; }

    // CefBrowserProcessHandler methods:
    virtual void OnContextInitialized() OVERRIDE;

    //Factory method to create new browser
    CefRefPtr<QCefClient> addBrowser(QList<QSslCertificate> caCerts = QList<QSslCertificate>());
    void closeAllBrowser();

   private:
    bool m_contextReady;
    QQueue<CefRefPtr<QCefClient> > m_clients;
    // Include the default reference counting implementation.
    IMPLEMENT_REFCOUNTING(QCefApp)
};

#endif // QCEFAPP_H
