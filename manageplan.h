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

private:
    Ui::ManageplanWindow *ui;

};

#endif // MANAGEPLAN_H
