#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QString>
#include <QMainWindow>
#include <QObject>
#include <QWidget>
#include <QMessageBox>
#include <QPushButton>
#include <QTableWidget>
#include <QMessageBox>
#include <QToolTip>
#include <QCursor>

#include "fillinformation.h"
#include "manageplan.h"
#include "querylog.h"

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
    void toFillInformation(QString, QString);
    void toManageplanwindowInfo(QString, QString);
    void toQuerylog(QMainWindow*, QString, QString);

public slots:
    void curLogin(QString, QString);
    void fromQuerylogTestplanId(QString);
    void fromManageplanwindow(QString, QString);

    void OnBtnClickedFillinformation();
    void OnBtnClickedManageplan();
    void OnBtnClickedQuerylog();
    void OnBtnClickedGenReport();
    void OnBtnClickedActionend();
    void OnBtnClickedInfo();

private:
    Ui::MainWindow* ui;
    QString curSurveyorName, curFarmId, curFarmName, curPlanName;
    QString testplanId;

    FillInformation* fillinformation_window;
    Manageplan* manageplan_window;
    Querylog* querylog_window;
    void showtable1();
    void mysetupUi();
};

#endif // MAINWINDOW_H
