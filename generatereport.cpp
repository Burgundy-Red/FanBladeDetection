#include "generatereport.h"

Generatereport::Generatereport(QWidget *parent) : QWidget(parent)
{

}

Generatereport::~Generatereport() {

}

//void Generatereport::onInitDialog()
//{
//    QString sqlstr;
//    QSqlQuery query;

//    // 初始化下拉框
      // planname

//    sqlstr = "SELECT COUNT(*) FROM (SELECT FanID FROM test WHERE TestPlanID = '" + m_pMainDlg->currentPlanID + "' AND TestState = '已检测') AS FanID";
//    if (!query.exec(sqlstr)) {
//        qDebug() << "Failed to execute the SQL statement: " << query.lastError();
//        return;
//    }
//    if (query.next()) {
//        int machineNum = query.value(0).toInt();

//        sqlstr = "SELECT FanID FROM test WHERE TestPlanID = '" + m_pMainDlg->currentPlanID + "' AND TestState = '已检测'";
//        if (!query.exec(sqlstr)) {
//            qDebug() << "Failed to execute the SQL statement: " << query.lastError();
//            return;
//        }
//        while (query.next()) {
//            QString machineID = query.value(0).toString();
//            sqlstr = "SELECT FanNum FROM fan WHERE FanID = '" + machineID + "'";
//            if (!query.exec(sqlstr)) {
//                qDebug() << "Failed to execute the SQL statement: " << query.lastError();
//                return;
//            }
//            if (query.next()) {
//                QString machineName = query.value(0).toString();
//                ui->m_SelectMachine->addItem(machineName);
//            }
//        }
//    }

//    // 初始化文本框
//    ui->m_SavePath->setText(m_pMainDlg->currentPath);

//    sqlstr = "SELECT FarmName from windfarm where FarmID ='" + m_pMainDlg->farmID + "'";
//    if (!query.exec(sqlstr)) {
//        qDebug() << "Failed to execute the SQL statement: " << query.lastError();
//        return;
//    }
//    if (query.next()) {
//        QString farmName = query.value(0).toString();
//        ui->m_FarmName->setText(farmName);
//    }

//    ui->m_Visible->addItem("是");
//    ui->m_Visible->addItem("否");
//    ui->m_Visible->setCurrentIndex(1);
//    m_ifAddName = false;
//}

//void Generatereport::getInfo() {
//    // TODO:  在此添加控件通知处理程序代码
//	if (FanName == "")
//	{
//		MessageBox("请选择相应机组...", "提示", MB_OK);
//		return;
//	}
//	else
//	{
//		CString FanID,Sqlstr;
//		Sqlstr = "SELECT FanID from fan where FanNum ='" + FanName + "'AND FarmID = '" + m_pMainDlg->FarmID + "'";				//查询对应电厂ID的电厂名
//		theApp.LocalMySql.SelectMysql(Sqlstr, &FanID);
//		Sqlstr = "SELECT SurveyDate from test where FanID ='" + FanID + "'AND TestPlanID = '" + m_pMainDlg->CurrentPlanID + "'";				//查询对应电厂ID的电厂名
//		theApp.LocalMySql.SelectMysql(Sqlstr, &TestDate);
//		Sqlstr = "SELECT TestID from test where FanID ='" + FanID + "'AND TestPlanID = '" + m_pMainDlg->CurrentPlanID + "'";				//查询对应电厂ID的电厂名
//		theApp.LocalMySql.SelectMysql(Sqlstr, &TestID);
//		Sqlstr = "SELECT FanType from fan where FanID ='" + FanID + "'";
//		theApp.LocalMySql.SelectMysql(Sqlstr, &FanType);
//		CString BigImageNum;																							//定义相应字符串
//		Sqlstr = "SELECT COUNT(*) FROM (SELECT BigImageID FROM bigimages WHERE TestID = '" + TestID + "')AS TestID";			//查询当前数据库内已注册用户数
//		theApp.LocalMySql.SelectMysql(Sqlstr, &BigImageNum);
//		int BigImagesNum = _ttoi(BigImageNum);																					//字符串类型转为整型
//		CString* BigImageID = new CString[BigImagesNum];																			//定义相应大小字符串数组
//		Sqlstr = "SELECT BigImageID FROM bigimages WHERE TestID = '" + TestID + "'";
//		theApp.LocalMySql.SelectMysql(Sqlstr, BigImageID);																	//全部存进UserName
//		for (int i = 0; i < BigImagesNum; i++)
//		{
//			CString IfBroken;
//			Sqlstr = "SELECT BreakPoint FROM bigimages WHERE BigImageID = '" + BigImageID[i] + "'";
//			theApp.LocalMySql.SelectMysql(Sqlstr, &IfBroken);
//			if (IfBroken == 1)
//			{
//				IfDamage = "是";
//				break;
//			}
//		}
//		if (IfDamage != "是")
//		{
//			IfDamage = "否";
//		}
//		GenerateReport(IfVisible, CurrentPath, BigImageID);
//		delete[] BigImageID;
//		//CDialogEx::OnOK();
//	}
//}

