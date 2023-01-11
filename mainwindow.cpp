#include "mainwindow.h"

#include "ui_Main_window.h"

Mainwindow::Mainwindow()
{
    ui = new Ui::MainWindow();
    ui->setupUi(this);
    manageplan_window = new Manageplan();
    // 创建菜单栏
//    actionfillinformation = ui->action1;
//    actionmanageplan = new QAction("管理计划", this);
//    actioninquire = new QAction("查询", this);
//    actiongeneratereports = new QAction("生成报告", this);
//    actionstop = new QAction("停止", this);
//    actionstatus = new QAction("状态", this);



    // 连接信号和槽
//    connect(ui->action_fillinfo, &QAction::triggered, this, &Mainwindow::OnBtnClickedFillinformation);
    connect(ui->action_manage, &QAction::triggered, this, &Mainwindow::OnBtnClickedManageplan);
    connect(this, SIGNAL(toManageplanwindowInfo(QString, QString)), manageplan_window, SLOT(curLogin(QString, QString)));
//    emit toManageplanwindowInfo(this->curFarmId, this->curSurveyorName);  // 在这里释放信号
//    connect(ui->action_querylog, &QAction::triggered, this, &Mainwindow::OnBtnClickedStatus);
//    connect(ui->action_genreport, &QAction::triggered, this, &Mainwindow::OnBtnClickedStatus);
//    connect(ui->action_curactionend, &QAction::triggered, this, &Mainwindow::OnBtnClickedStatus);
    connect(ui->action_curinfo, &QAction::triggered, this, &Mainwindow::OnBtnClickedStatus);
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
//    manageplan_window = new Manageplan();
    emit toManageplanwindowInfo(this->curFarmId, this->curSurveyorName);
    manageplan_window->show();
//    connect(this, SIGNAL(toManageplanwindowInfo(QString, QString)), manageplan_window, SLOT(curLogin(QString, QString)));
    // qvariant -> qstring toString
}

void Mainwindow::OnBtnClickedStatus(){
    QString info = "电厂名：";
    info = info + this->curFarmId + "\n" + "用户名：" + this->curSurveyorName + "\n" + "当前权限：\n当前计划";
    QMessageBox::about(this,"about",info);
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
