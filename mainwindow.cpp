#include "mainwindow.h"

#include "ui_Main_window.h"

Mainwindow::Mainwindow()
{
    ui = new Ui::MainWindow();
    ui->setupUi(this);

    this->fillinformation_window = new FillInformation();
    this->manageplan_window = new Manageplan();
    this->querylog_window = new Querylog();

    // turbine_table 不显示行号
    QHeaderView* headerView = ui->turbine_table->verticalHeader();
    headerView->setHidden(true); //false 显示行号列  true Hide
    // defect_table 不显示行号
    QHeaderView* headerView1 = ui->defect_table->verticalHeader();
    headerView1->setHidden(true); //false 显示行号列  true Hide

    // 连接信号和槽
    connect(this, &Mainwindow::toFillInformation, fillinformation_window, &FillInformation::fromMainwindow);
    connect(this, &Mainwindow::toQuerylog, querylog_window, &Querylog::fromMainwindow);

    connect(ui->action_fillinfo, &QAction::triggered, this, &Mainwindow::OnBtnClickedFillinformation);
    connect(ui->action_manage, &QAction::triggered, this, &Mainwindow::OnBtnClickedManageplan);
    connect(this, SIGNAL(toManageplanwindowInfo(QString, QString)), manageplan_window, SLOT(curLogin(QString, QString)));
    //    emit toManageplanwindowInfo(this->curFarmId, this->curSurveyorName);  // 在这里释放信号

    connect(ui->action_querylog, &QAction::triggered, this, &Mainwindow::OnBtnClickedQuerylog);
    connect(ui->action_genreport, &QAction::triggered, this, &Mainwindow::OnBtnClickedGenReport);
    connect(ui->action_curactionend, &QAction::triggered, this, &Mainwindow::OnBtnClickedActionend);
    connect(ui->action_curinfo, &QAction::triggered, this, &Mainwindow::OnBtnClickedInfo);

//    connect(gather_btn, &QPushButton::clicked, this, &Mainwindow::OnBtnClickedGather);
//    connect(stop_btn, &QPushButton::clicked, this, &Mainwindow::OnBtnClickedStop);
//    connect(table1, &QTableWidget::itemClicked, this, &Mainwindow::show_data1);

    // 子窗口和父窗口连接
    // 查询检测记录
    connect(querylog_window, SIGNAL(toMainwindowTestplanId(QString, QString)), this, SLOT(fromQuerylogTestplanId(QString, QString)));

}

Mainwindow::~Mainwindow() {
    delete fillinformation_window;
    delete manageplan_window;

    delete ui;
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
    this->manageplan_window->show();
}

void Mainwindow::OnBtnClickedQuerylog()
{
    this->querylog_window->setWindowModality(Qt::ApplicationModal);
    emit toQuerylog(this->curSurveyorName, this->curFarmId);
    this->querylog_window->show();
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
                       "操作员: " + this->curSurveyorName + "\n"
                       "电厂:   " + this->curFarmName);
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
                ui->turbine_table->item(n,m)->setTextAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
        }
    }
}

