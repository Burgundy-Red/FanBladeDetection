#include "registerwindow.h"
#include "ui_RegisteredUser.h"

Registerwindow::Registerwindow()
{
    ui = new Ui::RegisterWindow();
    ui->setupUi(this);

    //创建QSqlTableModel
    model=new QSqlTableModel(this);
    model->setEditStrategy(QSqlTableModel::OnManualSubmit);

    // 连接信号和槽
    connect(ui->ok_btn,SIGNAL(clicked()),this,SLOT(OnBtnClickedOk()));
    connect(ui->cancel_btn,SIGNAL(clicked()),this,SLOT(OnBtnClickedCancel()));
}

Registerwindow::~Registerwindow() {
    delete ui;
}

void Registerwindow::OnBtnClickedOk() {
    QString wind_farm_name_input = ui->wind_farm_name_le->text();
    QString username_input = ui->username_le->text();
    QString password_input = ui->password_le->text();
    QString position_select = ui->permissions_cb->currentText();
    int position_index = ui->permissions_cb->currentIndex();

    if (wind_farm_name_input == "" || username_input == "" || password_input == "") {
        QMessageBox::information(this, "错误", "资料填写不完整", QMessageBox::Yes);
        return;
    }

    if (password_input.size() > 6) {
        QMessageBox::information(this, "错误", "密码长度不可高于6位", QMessageBox::Yes);
        return;
    }

    model->setTable("surveyors");
    model->setFilter(QString("SurveyorName = '%1'").arg(username_input));
    model->select();
    if (!model->rowCount()) {
        // 不存在同名注册用户
        model->setTable("windfarm");
        model->setFilter(QString("FarmName = '%1'").arg(wind_farm_name_input));
        model->select();
        int setFarmId = -1, id = -1;
        if (!model->rowCount()) {
            // 无当前发电厂 插入数据库
            model->select();
            int rowNum = model->rowCount(); //获得表的行数
            id = rowNum;
            model->insertRow(rowNum); //添加一行
            model->setData(model->index(rowNum, 0), id);
            // TODO insert farm
            model->setData(model->index(rowNum, 0), "Farm");
            model->submitAll();
        } else {
            // QSqlRecord record = model->record(0);
            // setFarmId = record->value(0);
        }
        // TODO Insert surveyors
    } else {
        QMessageBox::information(this, "提示", "此用户已注册", QMessageBox::Yes);
    }
}

void Registerwindow::OnBtnClickedCancel() {
    this->close();
}




