#include "QCefView.h"

QCefView::QCefView(CefRefPtr<QCefClient> cefClient, QWidget *parent) : QWidget(parent)
{
    m_cefEmbedded = false;
    m_cefClient = cefClient;

    m_cefTimer = new QTimer(this);
    m_cefTimer->start(10);
    connect(m_cefTimer, SIGNAL(timeout()), this, SLOT(onCefTimer()));

    QCefClient* cefClientPtr = m_cefClient.get();
    connect(cefClientPtr, SIGNAL(loadStarted(bool)), this, SIGNAL(loadStarted(bool)));
    connect(cefClientPtr, SIGNAL(loadFinished(bool, bool)), this, SIGNAL(loadFinished(bool, bool)));
    connect(cefClientPtr, SIGNAL(loadError(QString)), this, SIGNAL(loadError(QString)));
    connect(cefClientPtr, SIGNAL(webMsgReceived(QString)), this, SIGNAL(webMsgReceived(QString)));
    connect(cefClientPtr, SIGNAL(keyEvent(Qt::Key)), this, SIGNAL(keyEvent(Qt::Key)));
}

QCefView::~QCefView()
{
}

void QCefView::load(QUrl url)
{
    m_cefClient->load(url.toString().toStdString());
}

void QCefView::reload()
{
    m_cefClient->reload();
}

void QCefView::onCefTimer()
{
    CefDoMessageLoopWork();
    if(m_cefEmbedded == false)
    {
        CefWindowHandle browserHandle = m_cefClient->browserWinId();
        if(browserHandle != (CefWindowHandle)-1)
        {
                QWindow* subW = QWindow::fromWinId((WId)browserHandle);
                QWidget* container = QWidget::createWindowContainer(subW, this);
                QStackedLayout* cefLayout = new QStackedLayout(this);
                setLayout(cefLayout);
                container->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
                cefLayout->addWidget(container);
                m_cefEmbedded = true;
                emit cefEmbedded();
        }
    }
}

void QCefView::runJavaScript(QString script)
{
    CefRefPtr<CefFrame> frame = m_cefClient->browser()->GetMainFrame();
    frame->ExecuteJavaScript(script.toStdString(), frame->GetURL(), 0);
}

void QCefView::sendToWeb(QString msg)
{
    runJavaScript(QString("ckRecvMessage('%1');").arg(msg));
}

void QCefView::checkPage()
{
    runJavaScript(QString("checkPage();"));
}
