#ifndef QCEFRENDERHANDLER_H
#define QCEFRENDERHANDLER_H

#include "include/cef_app.h"
#include "QCefV8Handler.h"

class QCefRenderHandler : public CefApp,
        public CefRenderProcessHandler
{
public:
    QCefRenderHandler();
    ~QCefRenderHandler();

    CefRefPtr<CefRenderProcessHandler> GetRenderProcessHandler() OVERRIDE
    {
        return this;
    }

    virtual void OnContextCreated(CefRefPtr<CefBrowser> browser,
                                 CefRefPtr<CefFrame> frame,
                                 CefRefPtr<CefV8Context> context) OVERRIDE;

private:
    CefRefPtr<QCefV8Handler> m_v8Handler;

    // Include the default reference counting implementation.
    IMPLEMENT_REFCOUNTING(QCefRenderHandler)
};

#endif // QCEFRENDERHANDLER_H

