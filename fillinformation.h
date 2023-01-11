#ifndef FILLINFORMATION_H
#define FILLINFORMATION_H

#include <QDir>
#include <QMainWindow>
#include <QObject>
#include <QWidget>
#include <QDebug>
#include <QMessageBox>
#include <QSqlQuery>
#include <QSqlError>

namespace Ui {
    class FillInformationWindow;
};

class FillInformation : public QWidget
{
    Q_OBJECT

public:
    explicit FillInformation(QWidget *parent = nullptr);
    ~FillInformation();
    void InitWindTurbineInfo();
    void RefreshWindTurbineInformation();

signals:

public slots:
    void fromMainwindow(QString, QString);

    void onAddTurbineButtonClicked();
    void onWindTurbineTypeComboIndexChanged(int index);
    void onSearchTurbineButtonClicked();
    void onDeleteTurbineButtonClicked();
    // void onTurbineListItemChanged(QListWidgetItem* item);
    void onChangeTurbineInfoButtonClicked();

private:
    Ui::FillInformationWindow* ui;

    QString surveyorName, farmId, farmName;
};

#endif // FILLINFORMATION_H
