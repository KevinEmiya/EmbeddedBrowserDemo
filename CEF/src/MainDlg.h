#ifndef MAINDLG_H
#define MAINDLG_H

#include <QDialog>

#include "src/cef/QCefView.h"
#include "src/cef/QCefInspector.h"

namespace Ui {
class MainDlg;
}

class MainDlg : public QDialog
{
    Q_OBJECT

public:
    explicit MainDlg(CefRefPtr<QCefApp> cefApp, QWidget *parent = 0);
    ~MainDlg();

private:
    void initWebview(CefRefPtr<QCefApp> cefApp);

private slots:
    void onCefEmbedded();
    void onRecvFromWeb(QString msg);
    void onKeyEvent(Qt::Key key);

private:
    Ui::MainDlg *ui;
    QCefView* m_webview;

    bool m_inspectorVisible;
    QCefInspector* m_inspector;
};

#endif // MAINDLG_H
