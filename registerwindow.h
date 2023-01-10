#ifndef REGISTERWINDOW_H
#define REGISTERWINDOW_H

#include <QMainWindow>
#include <QObject>
#include <QWidget>
#include <QSqlTableModel>
#include <QMessageBox>

namespace Ui {
    class RegisterWindow;
}

class Registerwindow : public QMainWindow
{
    Q_OBJECT

public:
    Registerwindow();
    ~Registerwindow();

signals:

public slots:
    void OnBtnClickedOk();
    void OnBtnClickedCancel();

private:
    Ui::RegisterWindow* ui;
    QSqlTableModel* model;
};

#endif // REGISTERWINDOW_H
