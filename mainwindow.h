#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QObject>
#include <QWidget>

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

private:
    Ui::MainWindow* ui;
    QString curSurveyorName, curFarmId;
};

#endif // MAINWINDOW_H
