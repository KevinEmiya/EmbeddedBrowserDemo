/*
 * QCefContext
* @brief: CEF上下文设置
* @author: kevin_yang
* @note:
*/

#include "QCefContext.h"


QCefContext::QCefContext(CefSettings* settings)
{
    m_cefApp = NULL;
    m_cefRenderer = NULL;
    m_cmdLine = NULL;
    m_settings = settings;
}

QCefContext::~QCefContext()
{
}


CefRefPtr<QCefApp> QCefContext::cefApp() const
{
    return m_cefApp;
}

CefRefPtr<QCefRenderHandler> QCefContext::cefRenderer() const
{
    return m_cefRenderer;
}

int QCefContext::initCef(int argc, char *argv[])
{
    // Provide CEF with command-line arguments.
    m_cmdLine = CefCommandLine::CreateCommandLine();
#ifdef CEF_LINUX
    CefMainArgs mainArgs(argc, argv);
    m_cmdLine->InitFromArgv(argc, argv);
#else
    HINSTANCE hInstance = (HINSTANCE)GetModuleHandle(NULL);
    CefMainArgs mainArgs(hInstance);
    m_cmdLine->InitFromString(::GetCommandLineW());
#endif
    return initCef(mainArgs);
}

int QCefContext::initCef(CefMainArgs& mainArgs)
{
    CefRefPtr<CefApp> app;

    // Create a ClientApp of the correct type.
    if (!m_cmdLine->HasSwitch("type"))
    {
      app = new QCefApp();
      m_cefApp = CefRefPtr<QCefApp>((QCefApp*)app.get());
    }
    else
    {
        CefString procType = m_cmdLine->GetSwitchValue("type");
        bool typeJudge = (procType == "renderer");
#ifdef CEF_LINUX
        typeJudge |= (procType == "zygote");
#endif
        if (typeJudge)
        {
            app = new QCefRenderHandler();
            m_cefRenderer = CefRefPtr<QCefRenderHandler>((QCefRenderHandler*)app.get());
        }
    }

    // CEF applications have multiple sub-processes (render, plugin, GPU, etc)
    // that share the same executable. This function checks the command-line and,
    // if this is a sub-process, executes the appropriate logic.
    int result = CefExecuteProcess(mainArgs, app, NULL);
    if (result >= 0)
    {
      // The sub-process has completed so return here.
      return result;
    }

    // Initialize CEF for the browser process.
    CefInitialize(mainArgs, *m_settings, app.get(), NULL);

    return -1;
}
