#include "mainwindow.h"

#include "ui_Main_window.h"

Mainwindow::Mainwindow()
{
    ui = new Ui::MainWindow();
    ui->setupUi(this);
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
