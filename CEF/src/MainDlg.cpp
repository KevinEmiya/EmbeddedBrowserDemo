#include "MainDlg.h"
#include "ui_MainDlg.h"

#include <QKeyEvent>
#include <QMessageBox>
#include <QSplitter>
#include <QStackedLayout>
#include <QVBoxLayout>

MainDlg::MainDlg(CefRefPtr<QCefApp> cefApp, QWidget* parent) : QDialog(parent),
                                                               ui(new Ui::MainDlg)
{
    ui->setupUi(this);
    initWebview(cefApp);
}

MainDlg::~MainDlg()
{
    delete ui;
}

void MainDlg::initWebview(CefRefPtr<QCefApp> cefApp)
{
    QVBoxLayout* boxLayout = new QVBoxLayout(ui->webviewFrame);
    boxLayout->setContentsMargins(0, 0, 0, 0);
    boxLayout->setSpacing(16);

    QSplitter* splitter = new QSplitter(ui->webviewFrame);
    splitter->setOrientation(Qt::Vertical);
    boxLayout->addWidget(splitter);

    QFrame* upperFrame = new QFrame(splitter);
    splitter->addWidget(upperFrame);

    QStackedLayout* webLayout = new QStackedLayout(upperFrame);
    webLayout->setContentsMargins(0, 0, 0, 0);
    webLayout->setSpacing(0);
    webLayout->setStackingMode(QStackedLayout::StackAll);

    //TODO: for ssl tutorial, add ca certs into the main client
    m_webview = new QCefView(cefApp->addBrowser(), upperFrame);
    m_webview->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    webLayout->addWidget(m_webview);
    webLayout->setCurrentWidget(m_webview);

    connect(m_webview, SIGNAL(cefEmbedded()), this, SLOT(onCefEmbedded()));
    connect(m_webview, SIGNAL(webMsgReceived(QString)), this, SLOT(onRecvFromWeb(QString)));

    m_inspector = new QCefInspector(cefApp->addBrowser(), ui->webviewFrame);
    m_inspector->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    splitter->addWidget(m_inspector);
    m_inspector->hide();
    m_inspectorVisible = false;

    connect(ui->btnGo, &QPushButton::clicked, this, [this]() {
        m_webview->load(QUrl(ui->editAddress->text()));
    });

    connect(m_webview, &QCefView::loadFinished, this, [this](bool ok, bool isMainFrame) {
        if (ok && isMainFrame && m_inspector->isVisible())
        {
            m_inspector->inspect();
        }
    });

    connect(m_webview, &QCefView::inspectorRequested, this, [this]() {
        if (m_inspectorVisible)
        {
            m_inspector->hide();
            m_inspectorVisible = false;
        }
        else
        {
            m_inspector->inspect();
            m_inspector->show();
            m_inspectorVisible = true;
        }
    });
}

void MainDlg::onCefEmbedded()
{
    show();
}

void MainDlg::onRecvFromWeb(QString msg)
{
    QMessageBox::information(this, "Received Message from Web Page", msg);
}
