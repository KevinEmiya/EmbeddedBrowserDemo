/*
 * QCefRenderHandler
* @brief: 渲染处理类
* @author: kevin_yang
* @note: 包含JS调用C++方法的处理
*/

#include "QCefRenderHandler.h"

#include <iostream>

#include <QDebug>

QCefRenderHandler::QCefRenderHandler()
{
    m_v8Handler = NULL;
}

QCefRenderHandler::~QCefRenderHandler()
{
}

void QCefRenderHandler::OnContextCreated(CefRefPtr<CefBrowser> browser,
                             CefRefPtr<CefFrame> frame,
                             CefRefPtr<CefV8Context> context)
{
    // Retrieve the context's window object.
    CefRefPtr<CefV8Value> object = context->GetGlobal();

    // Create the "getWebMessage" function.
    m_v8Handler = CefRefPtr<QCefV8Handler>(new QCefV8Handler(browser));
    CefRefPtr<CefV8Value> func = CefV8Value::CreateFunction("getWebMessage", m_v8Handler);

    // Add the "getWebMessage" function to the "window" object.
    object->SetValue("getWebMessage", func, V8_PROPERTY_ATTRIBUTE_NONE);
}

