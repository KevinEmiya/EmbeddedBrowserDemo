#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QSslConfiguration>

class QWebEngineView;
class QWebChannel;
class JsContext;
class Inspector;

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private:
    QSslConfiguration initSecurityContext();

private:
    Ui::MainWindow *ui;
    QWebEngineView* m_webView;
    QWebChannel* m_webChannel;
    JsContext* m_jsContext;
    Inspector* m_inspector;
};

#endif // MAINWINDOW_H
