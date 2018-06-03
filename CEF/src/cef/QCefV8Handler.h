#ifndef QCEFV8HANDLER_H
#define QCEFV8HANDLER_H

#include "include/cef_v8.h"
#include "include/cef_browser.h"

class QCefV8Handler : public CefV8Handler
{
public:
    QCefV8Handler(CefRefPtr<CefBrowser> browser);
    ~QCefV8Handler();

virtual bool Execute(const CefString& name,
                       CefRefPtr<CefV8Value> object,
                       const CefV8ValueList& arguments,
                       CefRefPtr<CefV8Value>& retval,
                       CefString& exception) OVERRIDE;

private:
    CefRefPtr<CefBrowser> m_browser;

  // Provide the reference counting implementation for this class.
  IMPLEMENT_REFCOUNTING(QCefV8Handler)
};

#endif // QCEFV8HANDLER_H
