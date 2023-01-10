#include "mainwindow.h"

#include "ui_Main_window.h"

Mainwindow::Mainwindow()
{
    ui = new Ui::MainWindow();
    ui->setupUi(this);

    // 创建菜单栏
//    actionfillinformation = ui->action1;
//    actionmanageplan = new QAction("管理计划", this);
//    actioninquire = new QAction("查询", this);
//    actiongeneratereports = new QAction("生成报告", this);
//    actionstop = new QAction("停止", this);
//    actionstatus = new QAction("状态", this);



    // 连接信号和槽
//    connect(actionfillinformation, &QAction::triggered, this, &Mainwindow::OnBtnClickedFillinformation);
//    connect(actionmanageplan, &QAction::triggered, this, &Mainwindow::OnBtnClickedManageplan);
//    connect(actioninquire, &QAction::triggered, this, &Mainwindow::OnBtnClickedStatus);
//    connect(actiongeneratereports, &QAction::triggered, this, &Mainwindow::OnBtnClickedStatus);
//    connect(actionstop, &QAction::triggered, this, &Mainwindow::OnBtnClickedStatus);
//    connect(actionstatus, &QAction::triggered, this, &Mainwindow::OnBtnClickedStatus);
//    connect(gather_btn, &QPushButton::clicked, this, &Mainwindow::OnBtnClickedGather);
//    connect(stop_btn, &QPushButton::clicked, this, &Mainwindow::OnBtnClickedStop);
//    connect(table1, &QTableWidget::itemClicked, this, &Mainwindow::show_data1);
}

Mainwindow::~Mainwindow() {
    delete ui;
}

void Mainwindow::curLogin(QString v1, QString v2) {
    // 信息传过来了
    this->curSurveyorName = v1;
    this->curFarmId = v2;

    // 就可以在ui对应地方设置姓名等
    // ui->info->setText(v1);
}

void Mainwindow::OnBtnClickedFillinformation()
{
    //...
}

void Mainwindow::OnBtnClickedManageplan()
{
    //...
}

void Mainwindow::OnBtnClickedStatus()
{
    //...
}

void Mainwindow::OnBtnClickedGather()
{
    //...
}

void Mainwindow::OnBtnClickedStop()
{
    //...
}

void Mainwindow::show_data1()
{
    //...
}
