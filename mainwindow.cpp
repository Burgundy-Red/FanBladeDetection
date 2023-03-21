#include "mainwindow.h"

#include "ui_Main_window.h"

Mainwindow::Mainwindow()
{
    ui = new Ui::MainWindow();
    ui->setupUi(this);
    mysetupUi();
    this->fillinformation_window = new FillInformation();
    this->manageplan_window = new Manageplan();
    this->querylog_window = new Querylog();

    // 连接信号和槽
    connect(this, &Mainwindow::toFillInformation, fillinformation_window, &FillInformation::fromMainwindow);
    connect(this, &Mainwindow::toManageplanwindowInfo, manageplan_window, &Manageplan::fromMainwindow);
    connect(this, &Mainwindow::toQuerylog, querylog_window, &Querylog::fromMainwindow);
    connect(manageplan_window, &Manageplan::toMainwindow, this, &Mainwindow::fromManageplanwindow);

    connect(ui->action_fillinfo, &QAction::triggered, this, &Mainwindow::OnBtnClickedFillinformation);
    connect(ui->action_manage, &QAction::triggered, this, &Mainwindow::OnBtnClickedManageplan);
    connect(ui->action_querylog, &QAction::triggered, this, &Mainwindow::OnBtnClickedQuerylog);
    connect(ui->action_fanstitching, &QAction::triggered, this, &Mainwindow::OnBtnClickedFanStitching);
    connect(ui->action_genreport, &QAction::triggered, this, &Mainwindow::OnBtnClickedGenReport);
    connect(ui->action_curactionend, &QAction::triggered, this, &Mainwindow::OnBtnClickedActionend);
    connect(ui->action_curinfo, &QAction::triggered, this, &Mainwindow::OnBtnClickedInfo);

	connect(ui->turbine_table, &QTableView::clicked, this, &Mainwindow::show_data);

//    connect(gather_btn, &QPushButton::clicked, this, &Mainwindow::OnBtnClickedGather);
//    connect(stop_btn, &QPushButton::clicked, this, &Mainwindow::OnBtnClickedStop);
//    connect(turbine_table, &QTableWidget::itemClicked, this, &Mainwindow::show_data1);

    // 子窗口和父窗口连接
    // 查询检测记录
    connect(querylog_window, SIGNAL(toMainwindowTestplanId(QString, QString)), this, SLOT(fromQuerylogTestplanId(QString, QString)));
}

Mainwindow::~Mainwindow() {
    delete fillinformation_window;
    delete manageplan_window;

    delete ui;
}

void Mainwindow::fromManageplanwindow(QString planName, QString planID){
    this->curPlanName = planName;
    this->testplanId = planID;
    this->showturbine_table();
}

void Mainwindow::OnBtnClickedFillinformation()
{
    // 阻塞主窗口，实现子窗口不关闭无法操作父窗口
    this->fillinformation_window->setWindowModality(Qt::ApplicationModal);
    emit toFillInformation(this->curSurveyorName, this->curFarmId);
    this->fillinformation_window->show();
}

void Mainwindow::OnBtnClickedManageplan()
{
    // 阻塞主窗口，实现子窗口不关闭无法操作父窗口
    this->manageplan_window->setWindowModality(Qt::ApplicationModal);
    emit toManageplanwindowInfo(this->curSurveyorName, this->curFarmId);
    this->manageplan_window->show();
}

void Mainwindow::OnBtnClickedQuerylog()
{
    this->querylog_window->setWindowModality(Qt::ApplicationModal);
    emit toQuerylog(this->curSurveyorName, this->curFarmId);
    this->querylog_window->show();
}

void Mainwindow::OnBtnClickedFanStitching()
{

}

void Mainwindow::OnBtnClickedGenReport()
{
    //...
}

void Mainwindow::OnBtnClickedActionend()
{
    //...
}

void Mainwindow::OnBtnClickedInfo()
{
    QToolTip::showText(QCursor::pos(),
                       "操  作  员: " + this->curSurveyorName + "\n"
                       "电       厂: " + this->curFarmName + "\n"
                       "当前计划: " + this->curPlanName
                       );
}


void Mainwindow::curLogin(QString v1, QString v2)
{
    // 信息传过来了
    this->curSurveyorName = v1;
    this->curFarmId = v2;
    this->curFarmName = "";

    QString strSql = QString("SELECT FarmName FROM windfarm WHERE FarmID='%1';").arg(this->curFarmId);
    QSqlQuery query;
    if (!query.exec(strSql)) { qDebug() << "line 80: " << query.lastError(); return; };
    if (query.next()) { this->curFarmName = query.value(0).toString(); }

//    strSql = QString("SELECT DISTINCT FanNum FROM fan WHERE FarmID='%1';").arg(this->curFarmId);
//    if (!query.exec(strSql)) { qDebug() << "line 85: " << query.lastError(); return; };
//    while (query.next()) {
//        ui->fanNum_combo->addItem(query.value(0).toString());
//    }
}

void Mainwindow::fromQuerylogTestplanId(QString testplanId, QString machineNum) {
    this->testplanId = testplanId;
    this->machineNum = machineNum;
//    qDebug() << testplanId << machineNum;
    ui->machineNum_le->setText(machineNum);

    // testplanid -> fanid -> fannum
    QString sqlstr, surveyDate, testState;
    QSqlQuery query;

    // clear the list widget
    ui->turbine_table->clearContents();

    // get the survey date and test state from the database
    sqlstr = "SELECT SurveyDate FROM test WHERE TestPlanID='" + testplanId + "' and FanID=(SELECT FanID FROM fan WHERE FanNum='" + machineNum + "')";
    if (!query.exec(sqlstr)) {
        qDebug() << "line 119: " << query.lastError();
        return;
    }
    if (query.next()) {
        surveyDate = query.value(0).toString();
    }

    sqlstr = "SELECT TestState FROM test WHERE TestPlanID='" + testplanId + "' and FanID=(SELECT FanID FROM fan WHERE FanNum='" + machineNum + "')";
    if (!query.exec(sqlstr)) {
        qDebug() << "Failed to execute the SQL statement: " << query.lastError();
        return;
    }
    if (query.next()) {
        testState = query.value(0).toString();
    }

    ui->turbine_table->insertRow(0);
    ui->turbine_table->setItem(0, 0, new QTableWidgetItem(machineNum));
    ui->turbine_table->setItem(0, 1, new QTableWidgetItem(testState));
    ui->turbine_table->setItem(0, 2, new QTableWidgetItem(surveyDate));
    int nCount = ui->turbine_table->rowCount();
    // 居中显示
    int nClumn = ui->turbine_table->columnCount();
    for (int n = 0; n < nCount;n++)
    {
        for (int m = 0; m < nClumn ;m++)
        {
            if (ui->turbine_table->item(n,m))
                ui->turbine_table->item(n,m)->setTextAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
        }
    }
}

void Mainwindow::showturbine_table() {
    qDebug() << "启动计划";
    ui->turbine_table->setRowCount(0); // 把行数设为零
    ui->turbine_table->clearContents(); // 清空列表内容

    QSqlQuery query;
    QString Sqlstr = "SELECT COUNT(*) FROM (SELECT test.FanID FROM test, fan WHERE fan.FanID IN(SELECT FanID FROM fan WHERE FarmID = '" + curFarmId + "') and test.FanID IN(SELECT FanID FROM test WHERE TestState = '未检测' and TestPlanID= '" + testplanId + "') and fan.FanID = test.FanID and TestPlanID= '" + testplanId + "') AS defectiveNum";
    query.exec(Sqlstr);
    query.next();
    int unTestNum = query.value(0).toInt();
    Sqlstr = "SELECT COUNT(*) FROM (SELECT fan.FanNum FROM fan WHERE fan.FanID IN(SELECT FanID FROM test WHERE TestPlanID= '" + testplanId + "' AND TestState= '正在检测')) AS numOfTesting";
    query.exec(Sqlstr);
    query.next();
    int NumOfTesting = query.value(0).toInt();
    Sqlstr = "SELECT COUNT(*) FROM (SELECT fan.FanNum FROM fan WHERE fan.FanID IN(SELECT FanID FROM test WHERE TestPlanID= '" + testplanId + "' AND TestState= '已检测')) AS numOfTested";
    query.exec(Sqlstr);
    query.next();
    int NumOfTested = query.value(0).toInt();

    qDebug() << unTestNum<<NumOfTesting<<NumOfTested<<curFarmId<<testplanId;
    Sqlstr = "SELECT fan.FanNum FROM fan WHERE fan.FanID IN(SELECT FanID FROM test WHERE TestPlanID= '" + testplanId + "' AND TestState= '正在检测')";
    query.exec(Sqlstr);
    QStringList TestingFanNum;
    while (query.next()) {
        TestingFanNum.append(query.value(0).toString());
    }

    for (int i = 0; i < NumOfTesting; i++) {
        int row = ui->turbine_table->rowCount();
        ui->turbine_table->insertRow(row); // 在最后插入行
        Sqlstr = "SELECT SurveyDate FROM test WHERE TestPlanID='" + testplanId + "' and FanID=(SELECT FanID FROM fan WHERE FanNum='" + TestingFanNum[i] + "')";
        query.exec(Sqlstr);
        query.next();
        QString serSurveyDate = query.value(0).toString();
        QTableWidgetItem* item1 = new QTableWidgetItem(TestingFanNum[i]);
        item1->setTextAlignment(Qt::AlignHCenter | Qt::AlignVCenter); // 居中
        QTableWidgetItem* item2 = new QTableWidgetItem("正在检测");
        item2->setTextAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
        QTableWidgetItem* item3 = new QTableWidgetItem(serSurveyDate);
        item3->setTextAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
        ui->turbine_table->setItem(row, 0, item1); // 显示
        ui->turbine_table->setItem(row, 1, item2);
        ui->turbine_table->setItem(row, 2, item3);
    }

    Sqlstr = "SELECT test.FanID FROM test, fan WHERE test.FanID IN(SELECT FanID FROM test WHERE TestState = '未检测'and TestPlanID= '" + testplanId + "') and fan.FanID = test.FanID and TestPlanID= '" + testplanId + "'";
    query.exec(Sqlstr);
    QStringList serFanID;
    while(query.next()){
        serFanID.append(query.value(0).toString());
    }
    for (int i = 0; i < unTestNum; i++) {
        int row = ui->turbine_table->rowCount();
        ui->turbine_table->insertRow(row); // 在最后插入行
        Sqlstr = "SELECT FanNum FROM fan WHERE FanID= '" + serFanID[i] + "'AND FarmID = '" + curFarmId + "'";
        query.exec(Sqlstr);
        query.next();
        QString serMachineNum = query.value(0).toString();
        QTableWidgetItem* item1 = new QTableWidgetItem(serMachineNum);
        item1->setTextAlignment(Qt::AlignHCenter | Qt::AlignVCenter); // 居中
        QTableWidgetItem* item2 = new QTableWidgetItem("未检测");
        item2->setTextAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
        ui->turbine_table->setItem(row, 0, item1); // 显示
        ui->turbine_table->setItem(row, 1, item2);
    }

    Sqlstr = "SELECT fan.FanNum FROM fan WHERE fan.FanID IN(SELECT FanID FROM test WHERE TestPlanID= '" + testplanId + "' AND FarmID = '" + curFarmId + "'AND TestState= '已检测')";
    query.exec(Sqlstr);
    QStringList TestedFanNum;
    while(query.next()){
        TestedFanNum.append(query.value(0).toString());
    }
    for (int i = 0; i < NumOfTested; i++) {
        int row = ui->turbine_table->rowCount();
        ui->turbine_table->insertRow(row); // 在最后插入行
        Sqlstr = "SELECT SurveyDate FROM test WHERE TestPlanID='" + testplanId + "' and FanID=(SELECT FanID FROM fan WHERE FanNum='" + TestedFanNum[i] + "'AND FarmID = '" + curFarmId + "')";
        query.exec(Sqlstr);
        query.next();
        QString serSurveyDate = query.value(0).toString();
        QTableWidgetItem* item1 = new QTableWidgetItem(TestedFanNum[i]);
        item1->setTextAlignment(Qt::AlignHCenter | Qt::AlignVCenter); // 居中
        QTableWidgetItem* item2 = new QTableWidgetItem("已检测");
        item2->setTextAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
        QTableWidgetItem* item3 = new QTableWidgetItem(serSurveyDate);
        item3->setTextAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
        ui->turbine_table->setItem(row, 0, item1); // 显示
        ui->turbine_table->setItem(row, 1, item2);
        ui->turbine_table->setItem(row, 2, item3);
    }
}

void Mainwindow::show_data(QModelIndex Item)
{
	// Show the plan name in the plan name box when a row in the list is clicked
	int row = Item.row();  // Get the row number
	QString fanNum = ui->turbine_table->item(row, 0)->text();
	// Set the font and font size of the text
	ui->machineNum_le->setFont(QFont("SimSun", 13));
	ui->machineNum_le->setText(fanNum);
}

void Mainwindow::mysetupUi(){
    // turbine_table 不显示行号
    QHeaderView* headerView = ui->turbine_table->verticalHeader();
    headerView->setHidden(true); //false 显示行号列  true Hide
    // defect_table 不显示行号
    QHeaderView* headerView1 = ui->defect_table->verticalHeader();
    headerView1->setHidden(true); //false 显示行号列  true Hide

    ui->turbine_table->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch); // 自动调整列宽
    ui->turbine_table->horizontalHeader()->setSectionResizeMode(0, QHeaderView::Interactive); // 可以手动调整列宽
    ui->defect_table->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch); // 自动调整列宽
    ui->defect_table->horizontalHeader()->setSectionResizeMode(0, QHeaderView::Interactive); // 可以手动调整列宽

    ui->turbine_table->setSelectionMode(QAbstractItemView::SingleSelection); // 设置只可以单选，可以使用ExtendedSelection进行多选
    ui->turbine_table->setSelectionBehavior(QAbstractItemView::SelectRows); // 设置 不可选择单个单元格，只可选择一行。
    ui->turbine_table->setEditTriggers(QAbstractItemView::NoEditTriggers); // 设置表格不可更改
    ui->defect_table->setSelectionMode(QAbstractItemView::SingleSelection); // 设置只可以单选，可以使用ExtendedSelection进行多选
    ui->defect_table->setSelectionBehavior(QAbstractItemView::SelectRows); // 设置 不可选择单个单元格，只可选择一行。
    ui->defect_table->setEditTriggers(QAbstractItemView::NoEditTriggers); // 设置表格不可更改

    ui->machineNum_le->setFixedWidth(255);
}
