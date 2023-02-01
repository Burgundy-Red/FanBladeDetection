#ifndef MODIFYPLAN_H
#define MODIFYPLAN_H

#include <QMainWindow>
#include <QObject>
#include <QSharedDataPointer>
#include <QWidget>
#include <QDebug>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QSqlTableModel>
#include <QTableWidgetItem>
#include <QSqlRecord>
#include <QMessageBox>
#include <QSqlQuery>
#include <QSqlError>
#include <QDate>

#include "manageplan.h"

namespace Ui {
class ModifyplanWindow;
}

class Modifyplan : public QMainWindow
{
    Q_OBJECT

public:
    explicit Modifyplan(QWidget *parent = nullptr);
    ~Modifyplan();

signals:
    void toManageplanwindow();

public slots:
    void fromManageplanwindow(QString, QString, QString, QString);

private:
    Ui::ModifyplanWindow *ui;

    void mysetupUi();
    void UpdateMachineList();
    void show_data1(QTableWidgetItem*);
    void show_data2(QTableWidgetItem*);
    void OnBnClickedAddmachineButton();
    void OnBnClickedDeletemachineButton();

    QString planId, planName, farmId, surveyorID;

};

#endif // MODIFYPLAN_H
