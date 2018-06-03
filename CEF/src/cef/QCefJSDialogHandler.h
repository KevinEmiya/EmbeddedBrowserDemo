#ifndef QCEFJSDIALOGHANDLER_H
#define QCEFJSDIALOGHANDLER_H

#include "include/cef_client.h"

class QCefJSDialogHandler : public CefJSDialogHandler
{
public:
    QCefJSDialogHandler();

    virtual bool OnJSDialog(CefRefPtr<CefBrowser> browser,
                            const CefString& origin_url,
                            const CefString& accept_lang,
                            JSDialogType dialog_type,
                            const CefString& message_text,
                            const CefString& default_prompt_text,
                            CefRefPtr<CefJSDialogCallback> callback,
                            bool& suppress_message);
};

#endif // QCEFJSDIALOGHANDLER_H
