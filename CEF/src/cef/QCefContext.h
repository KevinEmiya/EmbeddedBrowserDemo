#ifndef QCEFINITIALIZER_H
#define QCEFINITIALIZER_H

#include "QCefApp.h"
#include "QCefRenderHandler.h"
#include "include/base/cef_logging.h"

class QCefContext
{
public:
    QCefContext(CefSettings* settings);
    ~QCefContext();

    //Init Cef
    int initCef(int argc, char *argv[]);

public:
    CefRefPtr<QCefApp> cefApp() const;
    CefRefPtr<QCefRenderHandler> cefRenderer() const;

private:
    int initCef(CefMainArgs& mainArgs);

private:
    CefSettings* m_settings;
    CefRefPtr<QCefApp> m_cefApp;
    CefRefPtr<QCefRenderHandler> m_cefRenderer;
    CefRefPtr<CefCommandLine> m_cmdLine;
};

#endif // QCEFINITIALIZER_H
