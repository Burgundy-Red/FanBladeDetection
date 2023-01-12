#ifndef MANAGEPLAN_H
#define MANAGEPLAN_H

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

#include "modifyplan.h"

namespace Ui {
class ManageplanWindow;
}

class Modifyplan;
class Manageplan : public QMainWindow
{
    Q_OBJECT

public:
    explicit Manageplan(QWidget *parent = nullptr);
     ~Manageplan();

signals:
    void toModifyplanwindow(QString, QString, QString, QString);
    void toMainwindow(QString);

private slots:
    void OnBtnClickedNewplan();
    void OnBtnClickedStartplan();
    void OnBtnClickedChangeplan();
    void OnBtnClickedDelateplan();

public slots:
    void fromMainwindow(QString, QString);
    void fromModifyplanwindow();

private:
    Ui::ManageplanWindow *ui;
    QSqlTableModel *model;
    QString surveyorId,surveyorName, farmId, farmName;

    Modifyplan* modifyplan_window;
    void UpdatePlanList();
    void mysetupUi();
    void show_data(QModelIndex Item);

};

#endif // MANAGEPLAN_H
