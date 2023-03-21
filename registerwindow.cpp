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

    QSqlQuery query;
    QString sqlStr;
    sqlStr = QString("SELECT SurveyorID from surveyors where SurveyorName = '%1';").arg(username_input);
    if (!query.exec(sqlStr)) { qDebug() << "line 42: " << query.lastError(); return; }
    int farmid = 0;
    if (!query.size()) { // 新用户
        sqlStr = QString("SELECT FarmID FROM windfarm WHERE FarmName= '%1'").arg(wind_farm_name_input);
        if (query.exec(sqlStr) && !query.size()) { // 插入新电厂
            if (!query.exec("select Max(FarmID) from windfarm")) { qDebug() << "line 47: " << query.lastError(); return; }
            int id = 0;
            if (query.next()) { id = query.value(0).toInt() + 1; }
            sqlStr = QString("INSERT INTO windfarm (FarmName) VALUES (%1);").arg(wind_farm_name_input);
            if (!query.exec(sqlStr)) { qDebug() << "line 51: " << query.lastError(); return; }
            farmid = id;
        } else {
            if (!query.isActive()) { qDebug() << "line 54: " << query.lastError(); return; }
            if (query.next()) { farmid = query.value(0).toInt();}
        }
        query.prepare("INSERT INTO surveyors (SurveyorName, PassWord, FarmID, Position) VALUES (:SurveyorName, :PassWord, :FarmID, :Position)");
        query.bindValue(":SurveyorName", username_input);
        query.bindValue(":PassWord", password_input);
        query.bindValue(":FarmID", farmid);
        query.bindValue(":Position", position_select);
        if (!query.exec()) {
            qDebug() << "line 60: " << query.lastError();
            QMessageBox::information(this, "错误", "注册失败", QMessageBox::Yes);
            return;
        } else {
            QMessageBox::information(this, "提示", "注册成功", QMessageBox::Yes);
            return;
        }
    } else {
        QMessageBox::information(this, "提示", "此用户已注册", QMessageBox::Yes);
    }
    return;
}

void Registerwindow::OnBtnClickedCancel() {
    this->close();
}




