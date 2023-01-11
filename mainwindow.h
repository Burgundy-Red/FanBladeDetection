#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QString>
#include <QMainWindow>
#include <QObject>
#include <QWidget>
#include <QMessageBox>

#include "manageplan.h"
#include <QPushButton>
#include <QTableWidget>
#include <QMessageBox>
#include <QToolTip>
#include <QCursor>

#include "fillinformation.h"
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
    void toFillInformation(QString, QString);
    void toManageplanwindowInfo(QString, QString);

public slots:
    void curLogin(QString, QString);

private slots:
    void OnBtnClickedFillinformation();
    void OnBtnClickedManageplan();
    void OnBtnClickedQuerylog();
    void OnBtnClickedGenReport();
    void OnBtnClickedActionend();
    void OnBtnClickedInfo();

private:
    Ui::MainWindow* ui;
    QString curSurveyorName, curFarmId, curFarmName;

    FillInformation* fillinformation_window;
    Manageplan* manageplan_window;
};

#endif // MAINWINDOW_H
