#include "login.h"

#include "ui_Login.h"

Login::Login(QWidget *parent) : QMainWindow(parent)
    ,ui(new Ui::Form())
{
    ui->setupUi(this);

    //创建QSqlTableModel
    model=new QSqlTableModel(this);
    model->setEditStrategy(QSqlTableModel::OnManualSubmit);

//    ui->username_le->setPlaceholderText("请输入用户名");
//    ui->password_le->setPlaceholderText("请输入密码");
//    //设置passlineedit显示为密码模式
//    ui->password_le->setEchoMode(QLineEdit::Password);

    //连接信号与槽
    connect(ui->login_btn,SIGNAL(clicked()),this,SLOT(OnBtnClickedLogin()));
    connect(ui->logon_btn,SIGNAL(clicked()),this,SLOT(OnBtnClickedLogon()));
    connect(ui->exit_btn,SIGNAL(clicked()),this,SLOT(OnBtnClickeExit()));
}


Login::~Login()
{
    delete ui;
}

void Login::OnBtnClickedLogin() {
    if(!this->judgeEmpty())
    {
        ui->password_le->clear();
        return;
    }

    model->setTable("surveyors");
    model->select();
    for(int i=0;i<model->rowCount();i++)
    {
        QSqlRecord record = model->record(i);

        if(record.value(1)==ui->username_le->text()&&
                record.value(2)==ui->password_le->text())
        {
            this->clearAll();

            //连接学生成绩管理窗口和登录对话框信号与槽
            //connect(student,SIGNAL(toLoginDialog()),this,SLOT(showNormal()));

            mainwindow = new Mainwindow();
            mainwindow->show();
            connect(this, SIGNAL(toMainwindowInfo(QString, QString)), mainwindow, SLOT(curLogin(QString, QString)));
            // qvariant -> qstring toString
            emit toMainwindowInfo(record.value(1).toString(), record.value(3).toString());
            this->hide();

            return;
        }
        else if(record.value(1)==ui->username_le->text()&&
                record.value(2)!=ui->password_le->text())
       {
            QMessageBox::information(this,"提示","密码输入有误",QMessageBox::Yes);
            this->clearAll();
            return;
        }
    }
    QMessageBox::warning(this,"提示","用户不存在,请注册",QMessageBox::Yes);
    this->clearAll();
    return;
}

void Login::OnBtnClickedLogon() {

}

void Login::OnBtnClickeExit() {
    this->close();
}

bool Login::judgeEmpty()
{
    if(ui->username_le->text().isEmpty())
    {
        QMessageBox::warning(this,"警告","用户名不能为空");
        return false;
    }
    if(ui->password_le->text().isEmpty())
    {
        QMessageBox::warning(this,"警告","密码不能为空");
        return false;
    }
    else
        return true;
}

void Login::clearAll()
{
    ui->username_le->clear();
    ui->password_le->clear();
}
