#include "MainWindow.h"
#include "ui_MainWindow.h"

#include <QStackedLayout>
#include <QWebChannel>
#include <QWebEngineContextMenuData>
#include <QtWebEngineWidgets/QWebEngineView>
#include <QMenu>
#include <QSslConfiguration>
#include <QSslKey>
#include <QFile>

#include "JsContext.h"
#include "Inspector.h"
#include "QSslPage.h"

MainWindow::MainWindow(QWidget* parent) : QMainWindow(parent),
                                          ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    QSslConfiguration sslConf = initSecurityContext();

    qputenv("QTWEBENGINE_REMOTE_DEBUGGING", "7777");
    m_webView = new QWebEngineView(this);
    QStackedLayout* layout = new QStackedLayout(ui->frame);
    ui->frame->setLayout(layout);
    layout->addWidget(m_webView);

    m_webView->setPage(new QSslPage(m_webView));

    connect(ui->btnGo, &QPushButton::clicked, this, [this, sslConf]() {
        QString url = ui->addressEdit->text();
        if (!url.isEmpty())
        {
            m_webView->load(url);
        }
    });
    connect(ui->btnRefresh, &QPushButton::clicked, this, [this]() {
        m_webView->reload();
    });

    m_jsContext = new JsContext(this);
    m_webChannel = new QWebChannel(this);
    m_webChannel->registerObject("context", m_jsContext);
    m_webView->page()->setWebChannel(m_webChannel);
    connect(m_jsContext, &JsContext::recvdMsg, this, [this](const QString& msg) {
        ui->statusBar->showMessage(QString("Received message：%1").arg(msg), 3000);
    });

    connect(ui->btnSend, &QPushButton::clicked, this, [this]() {
        QString msg = ui->msgEdit->text();
        if (!msg.isEmpty())
        {
            m_jsContext->sendMsg(m_webView->page(), msg);
        }
    });

    m_webView->setContextMenuPolicy(Qt::CustomContextMenu);
    m_inspector = NULL;
    connect(m_webView, &QWidget::customContextMenuRequested, this, [this]() {
        QMenu* menu = new QMenu(this);
        QAction* action = menu->addAction("Inspect");
        connect(action, &QAction::triggered, this, [this](){
            if(m_inspector == NULL)
            {
                m_inspector = new Inspector(this);
            }
            else
            {
                m_inspector->show();
            }
        });
        menu->exec(QCursor::pos());
    });
}

MainWindow::~MainWindow()
{
    delete ui;
}

QSslConfiguration MainWindow::initSecurityContext()
{
    QSslConfiguration config = QSslConfiguration::defaultConfiguration();
    config.setProtocol(QSsl::AnyProtocol);

    QString certDir = "/home/kevinyang/testCert/demo";

    QFile keyFile(certDir + QString("/client/client.p12"));
    bool openOK = keyFile.open(QIODevice::ReadWrite);

    QSslKey key;
    QSslCertificate cert;

    QByteArray passPhrase = QString("123456").toLatin1();
    openOK = QSslCertificate::importPkcs12(&keyFile, &key, &cert, NULL, passPhrase);
    keyFile.close();

    qDebug()<<"cert valid = " << !cert.isNull();
    config.setLocalCertificate(cert);

    qDebug()<< "key valid = " << !key.isNull();
    config.setPrivateKey(key);

    QFile peerFile(certDir + QString("/server/serverCert.cer"));
    openOK = peerFile.open(QIODevice::ReadOnly);
    QSslCertificate peerCert(peerFile.readAll(), QSsl::Der);
    qDebug()<<"peer cert valid = " << !peerCert.isNull();
    QList<QSslCertificate> caCerts;
    caCerts << peerCert;
    config.setCaCertificates(caCerts);
    config.setPeerVerifyMode(QSslSocket::VerifyPeer);
    config.setPeerVerifyDepth(1);
    peerFile.close();

    //QSslConfiguration::setDefaultConfiguration(config);
    return config;
}
