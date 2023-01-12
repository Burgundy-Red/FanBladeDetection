#include "mainwindow.h"

#include "ui_Main_window.h"

Mainwindow::Mainwindow()
{
    ui = new Ui::MainWindow();
    ui->setupUi(this);

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
//    connect(this, SIGNAL(toManageplanwindowInfo(QString, QString)), this->manageplan_window, SLOT(curLogin(QString, QString)));
//    emit toManageplanwindowInfo(this->curFarmId, this->curSurveyorName);  // 在这里释放信号

    connect(ui->action_querylog, &QAction::triggered, this, &Mainwindow::OnBtnClickedQuerylog);
    connect(ui->action_genreport, &QAction::triggered, this, &Mainwindow::OnBtnClickedGenReport);
    connect(ui->action_curactionend, &QAction::triggered, this, &Mainwindow::OnBtnClickedActionend);
    connect(ui->action_curinfo, &QAction::triggered, this, &Mainwindow::OnBtnClickedInfo);

//    connect(gather_btn, &QPushButton::clicked, this, &Mainwindow::OnBtnClickedGather);
//    connect(stop_btn, &QPushButton::clicked, this, &Mainwindow::OnBtnClickedStop);
//    connect(table1, &QTableWidget::itemClicked, this, &Mainwindow::show_data1);

    // 子窗口和父窗口连接
    connect(querylog_window, SIGNAL(toMainwindowTestplanId(QString)), this, SLOT(fromQuerylogTestplanId(QString)));

}

Mainwindow::~Mainwindow() {
    delete fillinformation_window;
    delete manageplan_window;

    delete ui;
}

void Mainwindow::fromManageplanwindow(QString planName){
    this->curPlanName = planName;
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
    emit toQuerylog(this, this->curSurveyorName, this->curFarmId);
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

void Mainwindow::fromQuerylogTestplanId(QString v) {
    this->testplanId = v;

    // testplanid -> fanid -> fannum
}

