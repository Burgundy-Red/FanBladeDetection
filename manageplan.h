#ifndef MANAGEPLAN_H
#define MANAGEPLAN_H

#include <QMainWindow>
#include <QObject>
#include <QSharedDataPointer>
#include <QWidget>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QSqlTableModel>
#include <QTableWidgetItem>
#include <QSqlRecord>
#include <QMessageBox>

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
    void curLogin(QString, QString);

private:
    Ui::ManageplanWindow *ui;
    QSqlTableModel *model;
    QString curSurveyorName, curFarmId;

    void UpdatePlanList();

};

#endif // MANAGEPLAN_H
