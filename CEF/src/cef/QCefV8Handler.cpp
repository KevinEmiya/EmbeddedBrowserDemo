/*
 * QCefV8Handler
* @brief: V8处理类
* @author: kevin_yang
* @note: 处理在RenderHandler中注册的C++方法
*/

#include "QCefV8Handler.h"

QCefV8Handler::QCefV8Handler(CefRefPtr<CefBrowser> browser)
{
    m_browser = browser;
}

QCefV8Handler::~QCefV8Handler()
{
}

bool QCefV8Handler::Execute(const CefString& name,
           CefRefPtr<CefV8Value> object,
           const CefV8ValueList& arguments,
           CefRefPtr<CefV8Value>& retval,
           CefString& exception)
{
    if (name == "getWebMessage")
    {
        if (arguments.size() == 1)
        {
            CefString msgStr = arguments.at(0)->GetStringValue();
            //Msg will be passed to OnProcessMessageReceived interface of CefClient
            m_browser->SendProcessMessage(PID_BROWSER, CefProcessMessage::Create(msgStr));
            retval = CefV8Value::CreateInt(0);
        }
        return true;
    }
    return false;
}
