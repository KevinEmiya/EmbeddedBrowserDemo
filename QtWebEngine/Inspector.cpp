#include "Inspector.h"
#include "ui_Inspector.h"

#include <QWebEngineView>
#include <QStackedLayout>

Inspector::Inspector(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Inspector)
{
    ui->setupUi(this);

    connect(ui->btnClose, &QPushButton::clicked, this, [this](){
        hide();
    });

    m_webView = new QWebEngineView(this);
    QStackedLayout* layout = new QStackedLayout(ui->frame);
    ui->frame->setLayout(layout);
    layout->addWidget(m_webView);
    m_webView->load(QUrl("http://localhost:7777"));
    QDialog::show();
}

Inspector::~Inspector()
{
    delete ui;
}

void Inspector::show()
{
    m_webView->reload();
    QDialog::show();
}
