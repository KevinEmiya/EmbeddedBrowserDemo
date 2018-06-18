#ifndef QCEFCLIENT_H
#define QCEFCLIENT_H

#include "include/cef_client.h"
#include "QCefJSDialogHandler.h"

#include "QCefDefines.h"

#include <queue>
#include <QWidget>
#include <QSslCertificate>

class QCefClient: public QWidget,
        public CefClient,
        public CefDisplayHandler,
        public CefLifeSpanHandler,
        public CefFocusHandler,
        public CefKeyboardHandler,
        public CefLoadHandler,
        public CefRequestHandler,
        public CefContextMenuHandler,
        public QCefJSDialogHandler
{
    Q_OBJECT
public:
    QCefClient(QWidget* parent=0);
    virtual ~QCefClient();

    // CefClient methods:
    virtual CefRefPtr<CefDisplayHandler> GetDisplayHandler() OVERRIDE {
      return this;
    }
    virtual CefRefPtr<CefLifeSpanHandler> GetLifeSpanHandler() OVERRIDE {
      return this;
    }
    virtual CefRefPtr<CefLoadHandler> GetLoadHandler() OVERRIDE {
      return this;
    }
    virtual CefRefPtr<CefJSDialogHandler> GetJSDialogHandler() OVERRIDE {
        return this;
    }
    virtual CefRefPtr<CefFocusHandler> GetFocusHandler() OVERRIDE {
        return this;
    }
    virtual CefRefPtr<CefKeyboardHandler> GetKeyboardHandler() OVERRIDE {
        return this;
    }
    virtual CefRefPtr<CefRequestHandler> GetRequestHandler() OVERRIDE {
      return this;
    }
    virtual CefRefPtr<CefContextMenuHandler> GetContextMenuHandler() OVERRIDE {
      return this;
    }

    virtual bool OnProcessMessageReceived(CefRefPtr<CefBrowser> browser,
                                          CefProcessId source_process,
                                          CefRefPtr<CefProcessMessage> message) OVERRIDE;

    //CefLoadHandler methods:
    virtual void OnLoadStart(CefRefPtr<CefBrowser> browser,
                             CefRefPtr<CefFrame> frame,
                             TransitionType transition_type) OVERRIDE;
    virtual void OnLoadEnd(CefRefPtr<CefBrowser> browser,
                           CefRefPtr<CefFrame> frame,
                           int httpStatusCode) OVERRIDE;
    virtual void OnLoadError(CefRefPtr<CefBrowser> browser,
                             CefRefPtr<CefFrame> frame,
                             ErrorCode errorCode,
                             const CefString& errorText,
                             const CefString& failedUrl) OVERRIDE;

    // CefLifeSpanHandler methods:
    virtual void OnAfterCreated(CefRefPtr<CefBrowser> browser) OVERRIDE;
    virtual bool DoClose(CefRefPtr<CefBrowser> browser) OVERRIDE;
    virtual void OnBeforeClose(CefRefPtr<CefBrowser> browser) OVERRIDE;

    // CefFocusHandler methods:
    virtual bool OnSetFocus(CefRefPtr<CefBrowser> browser,
                            FocusSource source) OVERRIDE;

    virtual void OnGotFocus(CefRefPtr<CefBrowser> browser) OVERRIDE;

    // CefKeyBoardHandler methods:
    virtual bool OnKeyEvent(CefRefPtr<CefBrowser> browser,
                            const CefKeyEvent& event,
                            CefEventHandle os_event) OVERRIDE;

    virtual bool OnCertificateError(CefRefPtr<CefBrowser> browser,
                                    cef_errorcode_t cert_error,
                                    const CefString &request_url,
                                    CefRefPtr<CefSSLInfo> ssl_info,
                                    CefRefPtr<CefRequestCallback> callback) OVERRIDE;

    // CefContextMenuHandler methods:
    virtual void OnBeforeContextMenu(CefRefPtr<CefBrowser> browser,
                                     CefRefPtr<CefFrame> frame,
                                     CefRefPtr<CefContextMenuParams> params,
                                     CefRefPtr<CefMenuModel> model) OVERRIDE;

    virtual void OnRenderProcessTerminated(CefRefPtr<CefBrowser> browser,
                                           TerminationStatus status) OVERRIDE;

    //Getters and setters:
    CefRefPtr<CefBrowser> browser();
    CefWindowHandle browserWinId();

    void load(CefString url);
    void reload();

    void setCaCerts(QList<QSslCertificate> caCerts);

signals:
    void loadStarted(bool isMainFrame);
    void loadFinished(bool loadOk, bool isMainFrame);
    void loadError(QString errorStr);
    void loadResourceError(QString resource, QString errorStr);
    void webMsgReceived(QString msg);
    void inspectorRequested();

private:
    bool loadingCefErrorPage();
    bool loadingMainFrame(CefRefPtr<CefBrowser> browser,
                          CefRefPtr<CefFrame> frame);

private:
    CefRefPtr<CefBrowser> m_browser;
    bool m_created;

    CefString m_url;
    int m_errorCode;
    CefString m_errorStr;

    QList<QSslCertificate> m_caCerts;

    // Include the default reference counting implementation.
    IMPLEMENT_REFCOUNTING(QCefClient)
};

#endif // QCEFCLIENT_H
