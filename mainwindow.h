#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QObject>
#include <QWidget>
#include <QPushButton>
#include <QTableWidget>

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
private slots:
    void OnBtnClickedFillinformation();
    void OnBtnClickedManageplan();
    void OnBtnClickedStatus();
    void OnBtnClickedGather();
    void OnBtnClickedStop();
    void show_data1();

private:
    Ui::MainWindow* ui;
    QString curSurveyorName, curFarmId;

    QAction *actionfillinformation;
    QAction *actionmanageplan;
    QAction *actioninquire;
    QAction *actiongeneratereports;
    QAction *actionstop;
    QAction *actionstatus;
    QPushButton *gather_btn;
    QPushButton *stop_btn;
    QTableWidget *table1;
};

#endif // MAINWINDOW_H
