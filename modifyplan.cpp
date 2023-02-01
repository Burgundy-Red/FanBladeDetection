#include "modifyplan.h"

#include "ui_Modifyplan.h"

Modifyplan::Modifyplan(QWidget *parent) : QMainWindow(parent),
    ui(new Ui::ModifyplanWindow)
{
    ui->setupUi(this);
    connect(ui->table1, &QTableWidget::itemClicked, this, &Modifyplan::show_data1);
    connect(ui->table2, &QTableWidget::itemClicked, this, &Modifyplan::show_data2);
    connect(ui->add_btn, &QPushButton::clicked, this, &Modifyplan::OnBnClickedAddmachineButton);
    connect(ui->delate_btn, &QPushButton::clicked, this, &Modifyplan::OnBnClickedDeletemachineButton);
    connect(ui->exit_btn, &QPushButton::clicked, this, &QMainWindow::close);
}

Modifyplan::~Modifyplan()
{
    delete ui;
}


void Modifyplan::fromManageplanwindow(QString planid, QString planname, QString farmid,  QString surveyorid){
    planId = planid;
    planName = planname;
    farmId = farmid;
    surveyorID = surveyorid;
    mysetupUi();
    UpdateMachineList();
}

//void Modifyplan::mysetupUi(){
//    //...
//}

void Modifyplan::mysetupUi()
{
    ui->currentplan_tb->setFont(QFont("SimSun", 10));
    ui->currentplan_tb->setText(planName);

    ui->table1->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->table1->horizontalHeader()->setSectionResizeMode(0, QHeaderView::Interactive);
    ui->table2->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->table2->horizontalHeader()->setSectionResizeMode(0, QHeaderView::Interactive);

    ui->table1->setSelectionMode(QAbstractItemView::SingleSelection);
    ui->table1->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->table1->setEditTriggers(QAbstractItemView::NoEditTriggers);

    ui->table2->setSelectionMode(QAbstractItemView::SingleSelection);
    ui->table2->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->table2->setEditTriggers(QAbstractItemView::NoEditTriggers);
}


void Modifyplan::UpdateMachineList(){
    //...
    ui->table1->setRowCount(0);
    ui->table1->clearContents();
    QSqlQuery query, subquery;
    QString Sqlstr;
//    QString Sqlstr = "SELECT COUNT(*) FROM (SELECT TestID FROM test WHERE TestPlanID= '" + planId + "')as planname";
//    query.exec(Sqlstr);
//    query.next();
//    int machinesnum = query.value(0).toInt();
    Sqlstr = "SELECT FanID FROM test WHERE TestPlanID = '" + planId + "'";
    query.exec(Sqlstr);

    /*---------刷新已包含机组列表---------*/
    int i = 0;
    while (query.next()) {
        QString machinesnum = query.value(0).toString();

        subquery.exec("SELECT FanNum FROM fan WHERE FanID = '" + machinesnum + "'");
        subquery.next();
        QString Numofmachines = subquery.value(0).toString();

        subquery.exec("SELECT TestState FROM test WHERE FanID ='" + machinesnum + "' AND TestPlanID = '" + planId + "'");
        subquery.next();
        QString DetectionState = subquery.value(0).toString();

        subquery.exec("SELECT SurveyDate FROM test WHERE FanID ='" + machinesnum + "' AND TestPlanID = '" + planId + "'");
        subquery.next();
        QString surveydate = subquery.value(0).toString();

        ui->table1->insertRow(i);
        ui->table1->setItem(i, 0, new QTableWidgetItem(Numofmachines));
        ui->table1->setItem(i, 1, new QTableWidgetItem(DetectionState));
        ui->table1->setItem(i, 2, new QTableWidgetItem(surveydate));
        i++;
    }

    /*---------刷新未包含机组列表----------*/
    ui->table2->setRowCount(0);
    ui->table2->clearContents();
    query.exec("SELECT FanID FROM test WHERE TestPlanID= '" + planId + "'");
    QStringList includedFans;
    while (query.next()) {
        includedFans << query.value(0).toString();
    }

    query.exec("SELECT FanID,FanNum FROM fan WHERE FarmID= '" + farmId + "'");
    int col = 0;
    while (query.next()) {
        QString fanID = query.value(0).toString();
        QString fanNum = query.value(1).toString();
        if (!includedFans.contains(fanID)) {
            ui->table2->insertRow(col);
            ui->table2->setItem(col, 0, new QTableWidgetItem(fanNum));
            ui->table2->setItem(col, 1, new QTableWidgetItem("未检测"));
            col++;
        }
    }
}

void Modifyplan::show_data1(QTableWidgetItem* Item)
{
    ui->table2->setCurrentItem(nullptr);
    int row = Item->row();
    QString fanid = ui->table1->item(row, 0)->text();
    ui->fanid_le->setFont(QFont("SimSun", 15));
    ui->fanid_le->setText(fanid);
}

void Modifyplan::show_data2(QTableWidgetItem* Item)
{
    ui->table1->setCurrentItem(nullptr);
    int row = Item->row();
    QString fanid = ui->table2->item(row, 0)->text();
    ui->fanid_le->setFont(QFont("SimSun", 15));
    ui->fanid_le->setText(fanid);
}

void Modifyplan::OnBnClickedAddmachineButton()
{
    QString SelectNum = ui->fanid_le->text();
    if (SelectNum == "")
    {
        QMessageBox::information(this, "提示", "请选择添加机组", QMessageBox::Yes);
        return;
    }

    QSqlQuery query;
    query.exec("SELECT FanID FROM fan WHERE FanNum = '" + SelectNum + "'AND FarmID = '" + farmId + "'");
    if (!query.next())
    {
        QMessageBox::information(this, "提示", "添加机组失败", QMessageBox::Yes);
        return;
    }
    int fanID = query.value(0).toInt();
    query.exec("SELECT TestPlanID FROM testplan WHERE PlanName = '" + planName + "'AND FarmID = '" + farmId + "'");
    if (!query.next())
    {
        QMessageBox::information(this, "提示", "添加机组失败", QMessageBox::Yes);
        return;
    }
    int testplanID = query.value(0).toInt();
    query.exec("SELECT TestID FROM test WHERE FanID = '" + QString::number(fanID) + "'AND TestPlanID = '" + QString::number(testplanID) + "'");
    if (query.next())
    {
        QMessageBox::information(this, "提示", "当前计划已经包含此机组", QMessageBox::Yes);
        return;
    }
    QString teststate = "未检测";
    query.exec("INSERT INTO test(FanID, SurveyorID, TestState, TestPlanID) VALUES('" + QString::number(fanID) + "', '" + surveyorID + "', '" + teststate + "', '" + QString::number(testplanID) + "')");
    if (!query.isActive())
    {
        QMessageBox::critical(this, "错误", "添加机组失败", QMessageBox::Yes);
        return;
    }
    UpdateMachineList();
    QMessageBox::information(this, "提示", "添加机组成功", QMessageBox::Yes);
    emit toManageplanwindow();
}

void Modifyplan::OnBnClickedDeletemachineButton()
{
    QString selectNum = ui->fanid_le->text();
    if (selectNum.isEmpty()) {
        QMessageBox::information(this, "提示", "请选择删除机组", QMessageBox::Yes);
        return;
    }

    QSqlQuery query;
    QString sqlstr;

    // 查询 fanID
    sqlstr = "SELECT FanID FROM fan WHERE FanNum = '" + selectNum + "' AND FarmID = '" + farmId + "'";
    if (!query.exec(sqlstr)) {
        QMessageBox::critical(this, "错误", "执行查询失败", QMessageBox::Yes);
        return;
    }
    if (!query.first()) {
        QMessageBox::information(this, "提示", "无此机组相关信息", QMessageBox::Yes);
        return;
    }
    QString fanID = query.value(0).toString();

    // 查询 testID
    sqlstr = "SELECT TestID FROM test WHERE FanID = '" + fanID + "' AND TestPlanID = '" + planId + "'";
    if (!query.exec(sqlstr)) {
        QMessageBox::critical(this, "错误", "执行查询失败", QMessageBox::Yes);
        return;
    }
    if (!query.first()) {
        QMessageBox::information(this, "提示", "无此机组相关信息", QMessageBox::Yes);
        return;
    }

    QString testID = query.value(0).toString();

    // 查询 teststate
    sqlstr = "SELECT TestState FROM test WHERE TestID = '" + testID + "'";
    if (!query.exec(sqlstr)) {
        QMessageBox::critical(this, "错误", "执行查询失败", QMessageBox::Yes);
        return;
    }
    if (!query.first()) {
        QMessageBox::information(this, "提示", "无此机组相关信息", QMessageBox::Yes);
        return;
    }
    QString teststate = query.value(0).toString();
    if (teststate == "正在检测" || teststate == "已检测") {
        int reply = QMessageBox::information(this, "提示", "已存在此机组检测记录，删除机组后检测记录不可恢复，是否继续？", QMessageBox::Yes | QMessageBox::No, QMessageBox::No);
        if (reply == QMessageBox::No) {
            return;
        }
    }

    // 删除机组
    sqlstr = "DELETE FROM test WHERE TestID = '" + testID + "'";
    if (!query.exec(sqlstr)) {
        QMessageBox::critical(this, "错误", "删除机组失败", QMessageBox::Yes);
        return;
    }

    UpdateMachineList();
    QMessageBox::information(this, "提示", "机组删除成功", QMessageBox::Yes);
    emit toManageplanwindow();
}


