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

namespace Ui {
class ManageplanWindow;
}

class Manageplan : public QMainWindow
{
    Q_OBJECT

public:
    explicit Manageplan(QWidget *parent = nullptr);
     ~Manageplan();

private slots:
    void OnBtnClickedNewplan();
    void OnBtnClickedStartplan();
    void OnBtnClickedChangeplan();
    void OnBtnClickedDelateplan();

public slots:
    void fromMainwindow(QString, QString);

private:
    Ui::ManageplanWindow *ui;
    QSqlTableModel *model;
    QString surveyorId,surveyorName, farmId, farmName;

    void UpdatePlanList();
    void mysetupUi();
    void show_data(QModelIndex Item);

};

#endif // MANAGEPLAN_H
