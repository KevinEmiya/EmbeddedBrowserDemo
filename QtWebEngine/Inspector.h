#ifndef INSPECTOR_H
#define INSPECTOR_H

#include <QDialog>

class QWebEngineView;

namespace Ui {
class Inspector;
}

class Inspector : public QDialog
{
    Q_OBJECT

public:
    explicit Inspector(QWidget *parent = 0);
    ~Inspector();

    void show();

private:
    Ui::Inspector *ui;
    QWebEngineView* m_webView;
};

#endif // INSPECTOR_H
