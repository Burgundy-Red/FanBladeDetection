#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QString>
#include <QMainWindow>
#include <QObject>
#include <QWidget>
#include <QMessageBox>

#include "manageplan.h"

namespace  Ui {
    class MainWindow;
}

class Mainwindow : public QMainWindow
{
    Q_OBJECT

public:
    Mainwindow();
    ~Mainwindow();

signals:

public slots:
    void curLogin(QString, QString);
//    void OnBtnClickedFillinformation();
    void OnBtnClickedManageplan();
    void OnBtnClickedStatus();

private:
    Ui::MainWindow* ui;
    Manageplan* manageplan_window;
    QString curSurveyorName, curFarmId;
};

#endif // MAINWINDOW_H
