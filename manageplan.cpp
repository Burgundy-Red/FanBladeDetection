#include "manageplan.h"

#include "ui_Manageplan.h"

Manageplan::Manageplan(QWidget *parent) : QMainWindow(parent),
    ui(new Ui::ManageplanWindow())
{
    ui->setupUi(this);
    model=new QSqlTableModel(this);
    model->setEditStrategy(QSqlTableModel::OnManualSubmit);

    UpdatePlanList();
    connect(ui->newplan_btn,SIGNAL(clicked()),this,SLOT(OnBtnClickedNewplan()));
    connect(ui->startplan_btn,SIGNAL(clicked()),this,SLOT(OnBtnClickedStartplan()));
    connect(ui->changeplan_btn,SIGNAL(clicked()),this,SLOT(OnBtnClickedChangeplan()));
    connect(ui->delateplan_btn,SIGNAL(clicked()),this,SLOT(OnBtnClickedDelateplan()));
}

Manageplan::~Manageplan()
{
    delete ui;
}

void Manageplan::curLogin(QString v1, QString v2) {
    // 信息传过来了
    this->curSurveyorName = v1;
    this->curFarmId = v2;

    // 就可以在ui对应地方设置姓名等
    // ui->info->setText(v1);
}

void Manageplan::UpdatePlanList(){
//    QMessageBox::about(this,"about",this->curFarmId);
    model->setTable("testplan");
    model->setFilter(QString("name = '%1'").arg(this->curFarmId)); //根据curFarmId进行筛选
    model->select(); //显示结果
    for(int i=0; i<model->rowCount();i++){
        QSqlRecord record = model->record(i);
        ui->PlanList->setItem(i, 0, new QTableWidgetItem(record.value(0).toString()));
        ui->PlanList->setItem(i, 1, new QTableWidgetItem(record.value(1).toString()));
        ui->PlanList->setItem(i, 2, new QTableWidgetItem(record.value(2).toString()));
    }
//    ui->PlanList->setModel(model);
}

void Manageplan::OnBtnClickedNewplan(){
    QMessageBox::about(this,"about","点击新建计划");
    //..
}

void Manageplan::OnBtnClickedStartplan(){
    QMessageBox::about(this,"about","点击启动计划");
    //..
}

void Manageplan::OnBtnClickedChangeplan(){
    QMessageBox::about(this,"about","点击修改计划");
    //..
}

void Manageplan::OnBtnClickedDelateplan(){
    QMessageBox::about(this,"about","点击删除计划");
    //..
}
