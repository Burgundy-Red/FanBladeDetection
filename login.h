#ifndef LOGIN_H
#define LOGIN_H

#include <QMainWindow>
#include <QObject>
#include <QSharedDataPointer>
#include <QWidget>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QSqlTableModel>
#include <QSqlRecord>
#include <QMessageBox>

#include "registerwindow.h"
#include "Mainwindow.h"


namespace Ui {
class Form;
}

class Login : public QMainWindow
{
    Q_OBJECT

public:
    explicit Login(QWidget *parent = nullptr);
    ~Login();

    bool judgeEmpty();
    void clearAll();

signals:
    void toMainwindowInfo(QString, QString);

private slots:
    void OnBtnClickedLogon();
    void OnBtnClickedLogin();
    void OnBtnClickeExit(); 

private:
    Ui::Form *ui;
    QSqlTableModel *model;

    Mainwindow* mainwindow;
    Registerwindow* registerwindow;
};

#endif // LOGIN_H
