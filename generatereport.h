#ifndef GENERATEREPORT_H
#define GENERATEREPORT_H

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
#include <QSqlQuery>
#include <QSqlError>

class Generatereport : public QWidget
{
    Q_OBJECT
public:
    explicit Generatereport(QWidget *parent = nullptr);
    ~Generatereport();



signals:

};

#endif // GENERATEREPORT_H
