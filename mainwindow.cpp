#include "mainwindow.h"

#include "ui_Main_window.h"

Mainwindow::Mainwindow()
{
    ui = new Ui::MainWindow();
    ui->setupUi(this);

//    connect(ui->action1,SIGNAL(triggered()),this,SLOT(OnBtnClickedFillinformation()));
    connect(ui->actionguanli,SIGNAL(triggered()),this,SLOT(OnBtnClickedManageplan()));
    connect(ui->actionstatus,SIGNAL(triggered()),this,SLOT(OnBtnClickedStatus()));
}

Mainwindow::~Mainwindow() {
    delete ui;
}

void Mainwindow::OnBtnClickedManageplan(){
    manageplan_window = new Manageplan();
    manageplan_window->show();
}

void Mainwindow::OnBtnClickedStatus(){
    QString info = "电厂名：";
    info = info + this->curFarmId + "\n" + "用户名：" + this->curSurveyorName + "\n" + "当前权限：\n当前计划";
    QMessageBox::about(this,"about",info);
}

void Mainwindow::curLogin(QString v1, QString v2) {
    // 信息传过来了
    this->curSurveyorName = v1;
    this->curFarmId = v2;

    // 就可以在ui对应地方设置姓名等
    // ui->info->setText(v1);
}
