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
    void toQuerylog(QString, QString);

public slots:
    void curLogin(QString, QString);
    void fromQuerylogTestplanId(QString, QString);
    void fromManageplanwindow(QString, QString);

    void OnBtnClickedFillinformation();
    void OnBtnClickedManageplan();
    void OnBtnClickedQuerylog();
    void OnBtnClickedFanStitching();
    void OnBtnClickedGenReport();
    void OnBtnClickedActionend();
    void OnBtnClickedInfo();

private:
    Ui::MainWindow* ui;

    QString testplanId, machineNum;
    QString curSurveyorName, curFarmId, curFarmName, curPlanName;

    FillInformation* fillinformation_window;
    Manageplan* manageplan_window;
    Querylog* querylog_window;
    void showturbine_table();
    void mysetupUi();
};

#endif // MAINWINDOW_H
