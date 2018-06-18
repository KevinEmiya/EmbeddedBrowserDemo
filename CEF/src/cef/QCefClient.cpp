/*
 * QCefClient
* @brief: CefClient和各类Handler的实现
* @author: kevin_yang
* @note:
*/
#include "QCefClient.h"

#include "include/base/cef_bind.h"
#include "include/cef_app.h"
#include "include/wrapper/cef_helpers.h"

#include <QDebug>

#include <QDateTime>

QCefClient::QCefClient(QWidget* parent) : QWidget(parent)
{
    m_browser = NULL;
    m_created = false;
    m_errorCode = 0;

    setMouseTracking(true);
}

QCefClient::~QCefClient()
{
}

bool QCefClient::OnProcessMessageReceived(CefRefPtr<CefBrowser> browser,
                                      CefProcessId source_process,
                                      CefRefPtr<CefProcessMessage> message)
{
    emit webMsgReceived(QString(message->GetName().ToString().c_str()));
    return true;
}

void QCefClient::OnAfterCreated(CefRefPtr<CefBrowser> browser)
{
    CEF_REQUIRE_UI_THREAD();
    m_browser = browser;
    m_created = true;
}

bool QCefClient::DoClose(CefRefPtr<CefBrowser> browser)
{
    CEF_REQUIRE_UI_THREAD();
    return true;
}

void QCefClient::OnBeforeClose(CefRefPtr<CefBrowser> browser)
{
    CEF_REQUIRE_UI_THREAD();
    browser->GetHost()->CloseBrowser(true);
    m_browser = NULL;
}

bool QCefClient::OnSetFocus(CefRefPtr<CefBrowser> browser,
                        FocusSource source)
{
    if (source == FOCUS_SOURCE_NAVIGATION)
    {
        return true;
    }
    return false;
}

void QCefClient::OnGotFocus(CefRefPtr<CefBrowser> browser)
{
    OnSetFocus(browser, FOCUS_SOURCE_NAVIGATION);
}

void QCefClient::OnLoadStart(CefRefPtr<CefBrowser> browser,
                         CefRefPtr<CefFrame> frame,
                         TransitionType transition_type)
{
    CEF_REQUIRE_UI_THREAD();
    if(loadingCefErrorPage() == false)
    {
        m_errorCode = 0;
        m_errorStr.clear();
        emit loadStarted(loadingMainFrame(browser, frame));
    }
}

bool QCefClient::OnKeyEvent(CefRefPtr<CefBrowser> browser,
                             const CefKeyEvent& event,
                             CefEventHandle os_event)
{
    if(event.type == KEYEVENT_KEYUP)
    {
#ifdef CEF_LINUX
        CEF_KEY_CODE keyCode = (CEF_KEY_CODE)event.native_key_code;
#else
		CEF_KEY_CODE keyCode = (CEF_KEY_CODE)event.windows_key_code;
#endif
        switch (keyCode)
        {
        case CEF_F5:
            reload();
            return true;
        case CEF_F12:
            emit inspectorRequested();
            return true;
        default:
            break;
        }
    }
    return CefKeyboardHandler::OnKeyEvent(browser, event, os_event);
}

void QCefClient::OnLoadEnd(CefRefPtr<CefBrowser> browser,
                       CefRefPtr<CefFrame> frame,
                       int httpStatusCode)
{
    CEF_REQUIRE_UI_THREAD();
    if(loadingCefErrorPage() == false)
    {
        emit loadFinished(true, loadingMainFrame(browser, frame));
    }
}

bool QCefClient::OnCertificateError(CefRefPtr<CefBrowser> browser,
                                cef_errorcode_t cert_error,
                                const CefString &request_url,
                                CefRefPtr<CefSSLInfo> ssl_info,
                                CefRefPtr<CefRequestCallback> callback)
{
    CEF_REQUIRE_UI_THREAD();
    CefRefPtr<CefX509Certificate> cefCert = ssl_info->GetX509Certificate();
    size_t size = cefCert->GetDEREncoded()->GetSize();
    char* buffer;
    buffer = (char*)malloc(size + 1);
    cefCert->GetDEREncoded()->GetData(buffer, size, 0);
    QByteArray derByte(buffer, size);
    free(buffer);
    if(!m_caCerts.isEmpty())
    {
        foreach(QSslCertificate cert, m_caCerts)
        {
            QByteArray refCert = cert.toDer();
            if(refCert == derByte)
            {
                qDebug() << "服务器证书验证通过！";
                callback->Continue(true);
                return true;
            }
        }
    }
    return false;
}

void QCefClient::OnLoadError(CefRefPtr<CefBrowser> browser,
                         CefRefPtr<CefFrame> frame,
                         ErrorCode errorCode,
                         const CefString& errorText,
                         const CefString& failedUrl)
{
    CEF_REQUIRE_UI_THREAD();

    // Don't display an error for downloaded files.
    if (errorCode == ERR_ABORTED)
      return;

    // Don't display an error for external protocols that we allow the OS to
    // handle. See OnProtocolExecution().
    if (errorCode == ERR_UNKNOWN_URL_SCHEME)
    {
      if (m_url.ToString().find("spotify:") == 0)
        return;
    }

    m_errorCode = errorCode;
    m_errorStr = errorText;
    emit loadError(QString(m_errorStr.ToString().c_str()));
}

void QCefClient::OnBeforeContextMenu(CefRefPtr<CefBrowser> browser,
                                     CefRefPtr<CefFrame> frame,
                                     CefRefPtr<CefContextMenuParams> params,
                                     CefRefPtr<CefMenuModel> model)
{
    model->Clear();
}

void QCefClient::OnRenderProcessTerminated(CefRefPtr<CefBrowser> browser,
                                       TerminationStatus status)
{
    qDebug() << "[QCefClient]Render process terminated with status: " << status << ", reloading...";
    if(browser != NULL)
    {
        browser->Reload();
    }
}

CefRefPtr<CefBrowser> QCefClient::browser()
{
    return m_browser;
}

CefWindowHandle QCefClient::browserWinId()
{
    if(m_created)
    {
        return m_browser->GetHost()->GetWindowHandle();
    }
    return (CefWindowHandle)-1;
}

void QCefClient::load(CefString url)
{
    browser()->GetMainFrame()->LoadURL(url);
    m_url = url;
}

void QCefClient::reload()
{
    browser()->ReloadIgnoreCache();
}

bool QCefClient::loadingCefErrorPage()
{
    CefString cefErrorUrl = "data:text/html,chromewebdata";
    QString devToolUrl = "http://localhost:7777/";
    CefString currentUrl = browser()->GetMainFrame()->GetURL();
    if(currentUrl == cefErrorUrl || QString(currentUrl.ToString().c_str()).contains(devToolUrl))
    {
        return true;
    }
    return false;
}

bool QCefClient::loadingMainFrame(CefRefPtr<CefBrowser> browser, CefRefPtr<CefFrame> frame)
{
    int mainFrameId = browser->GetMainFrame()->GetIdentifier();
    int frameId = frame->GetIdentifier();
    return frameId == mainFrameId;
}

void QCefClient::setCaCerts(QList<QSslCertificate> caCerts)
{
    m_caCerts = caCerts;
}
