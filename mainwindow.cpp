#include "mainwindow.h"

#include "ui_Main_window.h"

Mainwindow::Mainwindow()
{
    ui = new Ui::MainWindow();
    ui->setupUi(this);

    this->fillinformation_window = new FillInformation();
    this->manageplan_window = new Manageplan();
    // 连接信号和槽
    connect(this, &Mainwindow::toFillInformation, fillinformation_window, &FillInformation::fromMainwindow);

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
    //...
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

    strSql = QString("SELECT DISTINCT FanNum FROM fan WHERE FarmID='%1';").arg(this->curFarmId);
    if (!query.exec(strSql)) { qDebug() << "line 85: " << query.lastError(); return; };
    while (query.next()) {
        ui->fanNum_combo->addItem(query.value(0).toString());
    }
}

