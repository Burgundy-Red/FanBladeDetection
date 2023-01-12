#include "querylog.h"
#include "ui_Querylog.h"

Querylog::Querylog(QWidget *parent) : QWidget(parent),
    ui(new Ui::QuerylogWindow)
{
    ui->setupUi(this);

    connect(ui->planName_combo, SIGNAL(currentIndexChanged(const QString&)), this, SLOT(OnPlannameComboChange(const QString&)));
    connect(ui->ok_btn, SIGNAL(clicked()), this, SLOT(OnBtnClickedOk()));
    connect(ui->cancel_btn, SIGNAL(clicked()), this, SLOT(OnBtnClickedCancel()));
}

Querylog::~Querylog() {
    delete ui;
}

void Querylog::InitPlanNameCombo() {
    ui->planName_combo->clear();   // Clear the user combo box

    QString Sqlstr, PlanNameNum;
    QSqlQuery query;
    query.prepare("SELECT COUNT(*) FROM (SELECT PlanName FROM testplan WHERE FarmID = :FarmID) AS FarmID");
    query.bindValue(":FarmID", this->farmId);
    if(!query.exec()) {
        qDebug() << "Error: " << query.lastError().text();
        return;
    }
    while (query.next()) {
        PlanNameNum = query.value(0).toString();
    }

    int PlanNum = PlanNameNum.toInt();
    query.prepare("SELECT PlanName FROM testplan WHERE FarmID = :FarmID");
    query.bindValue(":FarmID", this->farmId);
    if(!query.exec()) {
        qDebug() << "Error: " << query.lastError().text();
        return;
    }
    while (query.next()) {
        ui->planName_combo->addItem(query.value(0).toString());
    }
    changeMachineCombo();
}

void Querylog::changeMachineCombo() {
    ui->machineName_combo->clear();   // Clear the user combo box
    QString PlanName = ui->planName_combo->currentText();
    qDebug() << PlanName;
    if (PlanName.isEmpty()) {
        return;
    }

    QString Sqlstr, TestPlanID, MachineNum;
    QSqlQuery query;
    query.prepare("SELECT TestPlanID FROM testplan WHERE PlanName = :PlanName");
    query.bindValue(":PlanName", PlanName);
    if(!query.exec()) {
        qDebug() << "Error: " << query.lastError().text();
        return;
    }
    while (query.next()) {
        TestPlanID = query.value("TestPlanID").toString();
    }

    query.prepare("SELECT COUNT(*) FROM (SELECT FanID FROM test WHERE TestPlanID = :TestPlanID) AS TestPlanID");
    query.bindValue(":TestPlanID", TestPlanID);
    if(!query.exec()) {
        qDebug() << "Error: " << query.lastError().text();
        return;
    }
    while (query.next()) {
        MachineNum = query.value(0).toString();
    }

    int MachineNums = MachineNum.toInt();
    QList<QString> FanID;
    query.prepare("SELECT FanID FROM test WHERE TestPlanID = :TestPlanID");
    query.bindValue(":TestPlanID", TestPlanID);
    if(!query.exec()) {
        qDebug() << "Error: " << query.lastError().text();
        return;
    }
    while (query.next()) {
        FanID.append(query.value("FanID").toString());
    }

    QString FanNum;
    for (int i = 0; i < MachineNums; i++) {
        query.prepare("SELECT FanNum FROM fan WHERE FanID = :FanID");
        query.bindValue(":FanID", FanID[i]);
        if(!query.exec()) {
            qDebug() << "Error: " << query.lastError().text();
            return;
        }
        while (query.next()) {
            FanNum = query.value("FanNum").toString();
        }
        ui->machineName_combo->addItem(FanNum);
    }
}

void Querylog::OnPlannameComboChange(const QString& str) {
    ui->machineName_combo->clear();
    QString PlanName;
    int nIndex = ui->planName_combo->currentIndex();
    PlanName = ui->planName_combo->itemText(nIndex);
    if (PlanName.isEmpty())
    {
        return;
    }

    QString Sqlstr, TestPlanID, MachineNum;
    QSqlQuery query;
    Sqlstr = "SELECT TestPlanID FROM testplan WHERE PlanName= '" + PlanName + "'";
    if (!query.exec(Sqlstr)) { qDebug() << "line 113: " << query.lastError().text(); }
    while (query.next()) {
        TestPlanID = query.value(0).toString();
    }

    Sqlstr = "SELECT COUNT(*) FROM (SELECT FanID FROM test WHERE TestPlanID = '" + TestPlanID + "')AS TestPlanID";
    if (!query.exec(Sqlstr)) { qDebug() << "line 119: " << query.lastError().text(); }
    while (query.next()) {
        MachineNum = query.value(0).toString();
    }
    int MachineNums = MachineNum.toInt();

    QString* FanID = new QString[MachineNums];
    Sqlstr = "SELECT FanID FROM test WHERE TestPlanID = '" + TestPlanID + "'";
    if (!query.exec(Sqlstr)) { qDebug() << "line 127: " << query.lastError().text(); }
    int i=0;
    while (query.next()) {
        FanID[i] = query.value(0).toString();
        i++;
    }

    QString FanNum;
    for (i = 0; i < MachineNums; i++)
    {
        Sqlstr = "SELECT FanNum FROM fan WHERE FanID = '" + FanID[i] + "'";
        if (!query.exec(Sqlstr)) { qDebug() << "line 138: " << query.lastError().text(); }
        while(query.next()){
            FanNum = query.value(0).toString();
            ui->machineName_combo->addItem(FanNum);
        }
    }
    delete[] FanID;
}

void Querylog::OnBtnClickedOk() {
    QString SelectPlanName = ui->planName_combo->currentText();
    QString SelectMachineName = ui->machineName_combo->currentText();
    if (SelectPlanName.isEmpty()) {
        QMessageBox::warning(this, "提示", "请填写查询计划...");
        return;
    }
    else {
        QString Sqlstr;
        QSqlQuery query;
        query.prepare("SELECT TestPlanID FROM testplan WHERE PlanName = :PlanName");
        query.bindValue(":PlanName", SelectPlanName);
        if(!query.exec()) {
            qDebug() << "Error: " << query.lastError().text();
            return;
        }
        while (query.next()) {
            this->selectedTestplanId = query.value(0).toString();
        }
    }
    emit toMainwindowTestplanId(this->selectedTestplanId);
    this->close();
}

void Querylog::OnBtnClickedCancel() {
    this->close();
}

void Querylog::fromMainwindow(QMainWindow* p, QString v1, QString v2) {
    this->mainwindow = p;
    this->surveyorName = v1;
    this->farmId = v2;
    qDebug() << v1 << v2;

    InitPlanNameCombo();
}

