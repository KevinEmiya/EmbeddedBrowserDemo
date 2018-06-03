#include "MainDlg.h"
#include <QApplication>

#include "src/cef/QCefContext.h"

int main(int argc, char *argv[])
{
    int result = 0;

    CefSettings settings;
    // Completely disable logging.
    settings.log_severity = LOGSEVERITY_DISABLE;
    // The resources(cef.pak and/or devtools_resources.pak) directory.
    CefString(&settings.resources_dir_path) = CefString();
    // The locales directory.
    CefString(&settings.locales_dir_path) = CefString();
    settings.remote_debugging_port = 7777;

    QCefContext* cef = new QCefContext(&settings);
    result = cef->initCef(argc, argv);
    if (result >= 0)
        return result;

    QApplication a(argc, argv);
    QApplication::addLibraryPath(".");

    MainDlg* browser = new MainDlg(cef->cefApp());
    result = a.exec();

    delete browser;
    delete cef;

    CefShutdown();

    return result;
}
