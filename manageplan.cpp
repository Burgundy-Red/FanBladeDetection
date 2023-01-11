#include "manageplan.h"

#include "ui_Manageplan.h"

Manageplan::Manageplan(QWidget *parent) : QMainWindow(parent),
    ui(new Ui::ManageplanWindow())
{
    ui->setupUi(this);
    mysetupUi();
    model=new QSqlTableModel(this);
    model->setEditStrategy(QSqlTableModel::OnManualSubmit);

//    UpdatePlanList();
    connect(ui->newplan_btn,SIGNAL(clicked()),this,SLOT(OnBtnClickedNewplan()));
    connect(ui->startplan_btn,SIGNAL(clicked()),this,SLOT(OnBtnClickedStartplan()));
    connect(ui->changeplan_btn,SIGNAL(clicked()),this,SLOT(OnBtnClickedChangeplan()));
    connect(ui->delateplan_btn,SIGNAL(clicked()),this,SLOT(OnBtnClickedDelateplan()));

    connect(ui->PlanList, &QTableView::clicked, this, &Manageplan::show_data);

}

Manageplan::~Manageplan()
{
    delete ui;
}

void Manageplan::fromMainwindow(QString v1, QString v2) {
    this->surveyorName = v1;
    this->farmId = v2;
    UpdatePlanList();
    QString strSql = "SELECT FarmName FROM windfarm WHERE FarmID= '" + this->farmId + "'"; // Query for power plant Name
    QSqlQuery query;
    query.exec(strSql);
    if (query.next()) {
        this->farmName = query.value(0).toString();
    }

    strSql = "SELECT SurveyorID FROM surveyors WHERE FarmID= '" + farmId + "' and SurveyorName='" + surveyorName + "'";
    query.exec(strSql);
    if (query.next()) {
        this->surveyorId = query.value(0).toString();
    }
    qDebug()<<farmId<<farmName<<this->surveyorId<<strSql;

}

void Manageplan::UpdatePlanList()
{
    QSqlQuery query;
    ui->PlanList->setRowCount(0);  //把行数设为零
    ui->PlanList->clearContents(); //清空列表内容
    QString NumOfPlans, Sqlstr;
    Sqlstr = "SELECT COUNT(*) FROM (SELECT PlanName FROM testplan WHERE FarmID= '" + this->farmId + "')as fannum";  //查询已有计划数量
    //theApp.LocalMySql.SelectMysql(Sqlstr, &NumOfPlans);
    //int PlansNum = NumOfPlans.toInt();
    query.exec(Sqlstr);
    int PlansNum=0;
    if(query.next()) PlansNum = query.value(0).toInt();
    if (PlansNum != 0)
    {
        //CString* plansNum = new CString[PlansNum]();
        Sqlstr = "SELECT PlanName FROM testplan WHERE FarmID = '" + this->farmId + "'";  //保存计划信息
        query.exec(Sqlstr);
        QStringList plansName;
        while (query.next()) {
            plansName << query.value(0).toString();
        }
        //CString* plandate = new CString[PlansNum]();
        Sqlstr = "SELECT PlanDate FROM testplan WHERE FarmID = '" + this->farmId + "'";  //创建计划日期
        query.exec(Sqlstr);
        QStringList plandate;
        while (query.next()) {
            plandate << query.value(0).toString();
        }
        for (int i = 0; i < PlansNum; i++)
        {
            // m_PlanList.InsertItem(i, "");  //清空当前行信息
            ui->PlanList->insertRow(i);

            QString testplanID;
            Sqlstr = "SELECT TestPlanID FROM testplan WHERE PlanName = '" + plansName[i] + "'LIMIT 1";  //查找当前计划对应的计划ID
            query.exec(Sqlstr);
            if(query.next()) testplanID = query.value(0).toString();

            QString finishedNum;  //已检测完毕机组数量
            Sqlstr = "SELECT COUNT(*) FROM(SELECT TestState FROM test WHERE test.TestState='已检测' AND TestPlanID = '" + testplanID + "')as teststate";
            query.exec(Sqlstr);
            if(query.next()) finishedNum = query.value(0).toString();

            QString MachineNums;  //当前计划总机组数量
            Sqlstr = "SELECT COUNT(*) FROM(SELECT TestID FROM test WHERE TestPlanID = '" + testplanID + "')as PlanName";
            query.exec(Sqlstr);
            if(query.next()) MachineNums = query.value(0).toString();

            finishedNum += '/' + MachineNums;  //检测进度计算
            QTableWidgetItem* newItem = new QTableWidgetItem(plansName[i]);
            ui->PlanList->setItem(i, 0, newItem);
            newItem = new QTableWidgetItem(finishedNum);
            ui->PlanList->setItem(i, 1, newItem);
            newItem = new QTableWidgetItem(plandate[i]);
            ui->PlanList->setItem(i, 2, newItem);
        }
    }
}

void Manageplan::OnBtnClickedNewplan()
{
    // Create a new inspection plan
    QString PlanName = ui->planname_le->text();
    if (PlanName == "") {
        QMessageBox::information(this, "提示", "计划名不能为空", QMessageBox::Yes);
        return;
    }

    QString WindFarmName = this->farmName;
    QDate PlanDate = QDate::currentDate(); // Get the current date
    QString SelectionCriteria = "全部选择"; // Set to select all

    QString Sqlstr = "SELECT COUNT(*) FROM (SELECT FanNum FROM fan WHERE FarmID= '" + this->farmId + "')as fannum"; // Number of units created in the current power plant
    QSqlQuery query;
    query.exec(Sqlstr);
    query.next();
    int NumOfMachineNum = query.value(0).toInt();
    if (NumOfMachineNum == 0) {
        QMessageBox::information(this, "提示", "无可导入机组，请优先填写机组等信息", QMessageBox::Yes);
        return;
    }

    if (PlanName.toLocal8Bit().size() > 12) { // Limit the length of the plan name
        QMessageBox::critical(this, "错误", "计划名称不可超过12个字符（一个汉字相当于两个字符）", QMessageBox::Yes);
        return;
    }

    Sqlstr = "SELECT TestPlanID FROM testplan WHERE PlanName='" + PlanName + "'"; // Get the plan ID
    query.exec(Sqlstr);
    if (query.next()) {
        QMessageBox::critical(this, "错误", "已存在此检测计划名称", QMessageBox::Yes);
        return;
    }

    Sqlstr = "INSERT INTO testplan (PlanName,PlanDate,FanSelectionMethod,FarmID) VALUES(:planName, :planDate, :fanSelectionMethod, :farmID)";
    query.prepare(Sqlstr);
    query.bindValue(":planName", PlanName);
    query.bindValue(":planDate", PlanDate);
    query.bindValue(":fanSelectionMethod", SelectionCriteria);
    query.bindValue(":farmID", farmId);
    if (!query.exec())
    {
        QMessageBox::warning(NULL, "错误！", "计划添加失败");
        return;
    }

    QString maxTestPlanID;
    Sqlstr = "SELECT MAX(TestPlanID) FROM testplan WHERE FarmID = '" + this->farmId + "'";
    query.exec(Sqlstr);
    if(query.next())
        maxTestPlanID=query.value(0).toString();
    if (SelectionCriteria == "全部选择")
    {
        if (NumOfMachineNum != 0)
        {
            QStringList machineNum;
            Sqlstr = "SELECT FanNum FROM fan WHERE FarmID= '" + this->farmId + "'";
            query.exec(Sqlstr);
            while(query.next())
                machineNum<<query.value(0).toString();
            for (int i = 0; i < machineNum.size(); i++)
            {
                QString machineID;
                Sqlstr = "SELECT FanID FROM fan WHERE FanNum= '" + machineNum[i] + "'AND FarmID='"+ this->farmId +"'";
                query.exec(Sqlstr);
                if(query.next())
                    machineID=query.value(0).toString();
                Sqlstr = "INSERT INTO test (FanID,SurveyorID,TestPlanID,TestState) VALUES ('" + machineID + "','" + surveyorId + "','" + maxTestPlanID + "','未检测')";
                if (!query.exec(Sqlstr))
                {
                    qDebug()<<machineID<<surveyorId<<maxTestPlanID;
                    QMessageBox::warning(NULL, "提示！", "添加机组失败");
                    return;
                }
            }
        }
        else
        {
            QMessageBox::warning(NULL, "提示！", "没有符合条件的机组");
            return;
        }
        QMessageBox::information(NULL, "提示！", "检测计划保存成功");
        ui->planname_le->clear();
        UpdatePlanList();  //刷新计划列表
        return;
    }



}

void Manageplan::OnBtnClickedStartplan(){
    QMessageBox::about(this,"about","点击启动计划");
    //..
}

void Manageplan::OnBtnClickedChangeplan(){
    QMessageBox::about(this,"about","点击修改计划");
    //..
}

//void Manageplan::OnBtnClickedDelateplan(){
//    QMessageBox::about(this,"about","点击删除计划");
//    //..
//}

void Manageplan::OnBtnClickedDelateplan() {
    QString PlanName = ui->planname_le->text();
    if (PlanName == "") {
        QMessageBox::information(this, "提示", "请选择计划", QMessageBox::Yes);
        return;
    }
    if (QMessageBox::question(this, "提示", "是否删除此计划相关所有检测记录?", QMessageBox::Yes|QMessageBox::No, QMessageBox::No) == QMessageBox::No) {
        return;
    }
    QString Sqlstr = "SELECT TestPlanID FROM testplan WHERE PlanName= '" + PlanName + "'AND FarmID = '" + farmId + "'";
    QSqlQuery query;
    query.exec(Sqlstr);
    if (!query.next()) {
        QMessageBox::information(this, "提示", "没有此计划相关信息", QMessageBox::Yes);
        return;
    }
    QString TestPlanID = query.value(0).toString();
    Sqlstr = "SELECT FarmName FROM windfarm WHERE FarmID = '"+ farmId +"'";
    QString serFarmName = farmName;
    // 删除所有计划内容
    Sqlstr = "SELECT COUNT(*) FROM (SELECT TestID FROM test WHERE TestPlanID= '" + TestPlanID + "')AS NumOfTestID";
    query.exec(Sqlstr);
    int TestIDNum = 0;
    if(query.next()){
        TestIDNum = query.value(0).toInt();
    }

    if (TestIDNum != 0) {
        QString testID[TestIDNum];
        QSqlQuery query;
        query.exec("SELECT TestID FROM test WHERE TestPlanID='" + TestPlanID + "'");
        int i = 0;
        while (query.next()) {
            testID[i] = query.value(0).toString();
            QSqlQuery subquery;
            subquery.exec("SELECT SurveyDate FROM test WHERE TestID='" + testID[i] + "'");
            QString surveyDate;
            if (subquery.next()) {
                surveyDate = subquery.value(0).toString();
            }
            subquery.exec("SELECT FanNum FROM fan WHERE FanID IN (SELECT FanID FROM test WHERE TestID='" + testID[i] + "')");
            if (subquery.next()) {
                QString fanNum = subquery.value(0).toString();
            }
            else {
                QMessageBox::critical(this, "错误", "机组查找失败");
                return;
            }

            subquery.exec("DELETE FROM test WHERE TestID='" + testID[i] + "'");
            if(query.numRowsAffected()<=0){
                QMessageBox::critical(this, "提示", "删除检测记录失败");
                return;
            }

            i++;
        }
    }
    Sqlstr = "DELETE FROM testplan WHERE TestPlanID= '" + TestPlanID + "'";
    if (!query.exec(Sqlstr))
    {
        QMessageBox::warning(this, "提示", "删除计划失败");
        UpdatePlanList();
        return;
    }
    QMessageBox::information(this, "提示", "检测计划删除成功");
    ui->planname_le->clear();
    UpdatePlanList();
    return;

}


void Manageplan::mysetupUi()
{
    // Set some properties of the table
    ui->PlanList->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);  // Auto-adjust column width
    ui->PlanList->horizontalHeader()->setSectionResizeMode(0, QHeaderView::Interactive);  // Manually adjust column width

    ui->PlanList->setSelectionMode(QAbstractItemView::SingleSelection);  // Only single selection allowed
    ui->PlanList->setSelectionBehavior(QAbstractItemView::SelectRows);  // Only entire rows can be selected
    ui->PlanList->setEditTriggers(QAbstractItemView::NoEditTriggers);   // Table is not editable
}

void Manageplan::show_data(QModelIndex Item)
{
    // Show the plan name in the plan name box when a row in the list is clicked
    int row = Item.row();  // Get the row number
    QString planname = ui->PlanList->item(row, 0)->text();
    // Set the font and font size of the text
    ui->planname_le->setFont(QFont("SimSun", 15));
    ui->planname_le->setText(planname);
}


