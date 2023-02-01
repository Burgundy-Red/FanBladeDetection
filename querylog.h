#ifndef QUERYLOG_H
#define QUERYLOG_H

#include <QMainWindow>
#include <QObject>
#include <QWidget>
#include <QDebug>
#include <QSqlQuery>
#include <QSqlError>
#include <QMessageBox>



namespace Ui {
    class QuerylogWindow;
}

class Querylog : public QWidget
{
    Q_OBJECT

public:
    explicit Querylog(QWidget *parent = nullptr);
    ~Querylog();
    void InitPlanNameCombo();
    void changeMachineCombo();

signals:
    void toMainwindowTestplanId(QString, QString);

public slots:
    void fromMainwindow(QString, QString);

    void OnPlannameComboChange(const QString&);
    void OnBtnClickedOk();
    void OnBtnClickedCancel();

private:
    Ui::QuerylogWindow* ui;
    QMainWindow* mainwindow;
    QString surveyorName, farmId, selectedTestplanId, selectedMachineNum;

};

#endif // QUERYLOG_H
