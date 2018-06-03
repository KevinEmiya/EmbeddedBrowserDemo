/*
 * QCefJSDialogHandler
* @brief: JS对话框处理类
* @author: kevin_yang
* @note: JS调用Qt对话框的实现
*/

#include "QCefJSDialogHandler.h"
#include <QMessageBox>

QCefJSDialogHandler::QCefJSDialogHandler()
{

}

bool QCefJSDialogHandler::OnJSDialog(CefRefPtr<CefBrowser> browser,
                        const CefString& origin_url,
                        const CefString& accept_lang,
                        JSDialogType dialog_type,
                        const CefString& message_text,
                        const CefString& default_prompt_text,
                        CefRefPtr<CefJSDialogCallback> callback,
                        bool& suppress_message)
{
       QMessageBox::warning(NULL, "JavaScript Alert", QString(message_text.ToString().c_str()));
       return true;
}
