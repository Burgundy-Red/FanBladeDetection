#include "fillinformation.h"
#include "ui_FillInformation.h"

FillInformation::FillInformation(QWidget *parent) : QWidget(parent),
    ui(new Ui::FillInformationWindow)
{
    ui->setupUi(this);
    mysetupUi();

    connect(ui->addTurbine_btn, &QPushButton::clicked, this, &FillInformation::onAddTurbineButtonClicked);
    connect(ui->turbineType_combo, SIGNAL(currentIndexChanged(int)), this, SLOT(onWindTurbineTypeComboIndexChanged(int)));
    connect(ui->searchTurbine_btn, &QPushButton::clicked, this, &FillInformation::onSearchTurbineButtonClicked);
    connect(ui->deleteTurbine_btn, &QPushButton::clicked, this, &FillInformation::onDeleteTurbineButtonClicked);
    // connect(ui->turbineList, &QListWidget::itemChanged, this, &FillInformation::onTurbineListItemChanged);
    connect(ui->changeTurbineInfo_btn, &QPushButton::clicked, this, &FillInformation::onChangeTurbineInfoButtonClicked);
}

FillInformation::~FillInformation() {
    delete ui;
}

void FillInformation::onAddTurbineButtonClicked()
{
    //获取文本框的内容
    QString windFarm = ui->windfarm_text->toPlainText();
    QString machineNum = ui->machine_le->text();
    QString machineType = ui->turbineType_combo->currentText();
    QString bladeLength = ui->bladeLength->text();
    QString wheelsHeight = ui->wheelsHeight->text();

    //检查是否填写完整
    if (machineNum.isEmpty() || machineType.isEmpty() || bladeLength.isEmpty() || wheelsHeight.isEmpty()) {
        QMessageBox::warning(this, "提示", "信息填写不完整");
        return;
    }

    //将文本转换为数字
    int bladeLengthInt = bladeLength.toInt();
    int wheelsHeightInt = wheelsHeight.toInt();

    //检查转换是否成功
    if (bladeLengthInt == 0 || wheelsHeightInt == 0) {
        QMessageBox::warning(this, "错误", "叶片长度或轮毂高度填写不正确");
        return;
    }

    //构建数据库查询语句
    QSqlQuery query;
    QString Sqlstr, fanID, farmID;
    Sqlstr = QString("SELECT FarmID FROM windfarm WHERE FarmName= '%1'").arg(windFarm);
    if (!query.exec(Sqlstr)) { qDebug() << "line 53: " << query.lastError(); return; }
    if (query.next()) farmID = query.value(0).toString();
    Sqlstr = QString("SELECT FanID FROM Fan WHERE FanNum= '%1' and FarmID= '%2'").arg(machineNum, farmID);
    if (!query.exec(Sqlstr)) { qDebug() << "line 56: " << query.lastError(); return; }
    if (query.next()) {
        QMessageBox::warning(this, "提示", "此风机已添加完成");
        return;
    }

    //添加风机
    Sqlstr = QString("INSERT Fan (FanNum,FanType,BladeLength,WheelHubHeight,FarmID) "
                     "VALUES ('%1','%2','%3','%4','%5')").arg(machineNum, machineType, bladeLength, wheelsHeight, farmID);
    if (!query.exec(Sqlstr))
    {
        { qDebug() << "line 67: " << query.lastError(); return; }
        QMessageBox::information(this, "错误", "插入风机失败");
        return;
    } else {
        /*QString numOfMachines;
        int rowCount = ui->turbine_table->rowCount();
        ui->turbine_table->insertRow(rowCount);
        ui->turbine_table->setItem(rowCount, 0, new QTableWidgetItem(machineNum));
        ui->turbine_table->setItem(rowCount, 1, new QTableWidgetItem(machineType));
        ui->turbine_table->setItem(rowCount, 2, new QTableWidgetItem(bladeLength));
        ui->turbine_table->setItem(rowCount, 3, new QTableWidgetItem(wheelsHeight));*/
        RefreshWindTurbineInformation();
        return;
    }
}

void FillInformation::onWindTurbineTypeComboIndexChanged(int index)
{
    QString machineType, sqlStr, bladeLength, hubHeight;
    int nIndex = index;
    int nCount = ui->turbineType_combo->count();
    if ((nIndex != -1) && (nCount >= 1))
    {
        machineType = ui->turbineType_combo->currentText();
    }
    QSqlQuery query;
    sqlStr = QString("SELECT BladeLength, WheelHubHeight from fan where FanType='%1' LIMIT 1;").arg(machineType);
    if (!query.exec(sqlStr)) { qDebug() << "line 94: " << query.lastError(); return; }
    if (query.next()) {
        bladeLength = query.value(0).toString();
        hubHeight = query.value(1).toString();
    }
    if ((bladeLength[0] != "") && (hubHeight != ""))
    {

        ui->bladeLength->setText(bladeLength);
        ui->wheelsHeight->setText(hubHeight);
    }
    else return;
}

void FillInformation::onSearchTurbineButtonClicked()
{
    // TODO:  在此添加控件通知处理程序代码
    QString Sqlstr, select;
    QString* Machines = new QString[4];
    select = ui->lineEdit->text();
    if (select.isEmpty()) {
        QMessageBox::warning(this, "Error", "请选择查询的机组");
        return;
    }

    int selectrecord;
    selectrecord = select.toInt();
    if (selectrecord == 0)													//未填写查找机组编号
    {
        RefreshWindTurbineInformation();							//显示全部机组
        return;
    }
    QSqlQuery query;
    QString farmID;
    Sqlstr = QString("SELECT FarmID FROM WindFarm WHERE FarmName = '%1'").arg(this->farmName);
    if (!query.exec(Sqlstr)) { qDebug() << "line 124: " << query.lastError(); return; }
    if (query.next()) farmID = query.value(0).toString();
    Sqlstr = QString("SELECT FanNum, FanType, BladeLength, WheelHubHeight FROM fan WHERE FanNum = '%1' AND FarmID = '%2'").arg(select, farmID);
    if (!query.exec(Sqlstr)) { qDebug() << "line 127: " << query.lastError(); return; }
    if (query.next()) {
        for (int i = 0; i < 4; i++) {
            Machines[i] = query.value(i).toString();
        }
    }
    if (Machines[0] == "")													//未查到相应机组
    {
        QMessageBox::warning(this, "提示", "无此机组相关信息");
        return;
    }
    ui->turbine_table->setRowCount(0);
    ui->turbine_table->clearContents();										//清空列表
    ui->turbine_table->insertRow(0);
    int count = 0;
    for (int j = 0; j < 4; j++)
    {
        ui->turbine_table->setItem(0, j, new QTableWidgetItem(Machines[j]));				//显示单条机组信息
    }
    // TODO: SetDlgItemText(IDC_TURBINENUMBER_EDIT, "");
    delete[] Machines;
}

void FillInformation::onDeleteTurbineButtonClicked()
{
    QString SelectNum = ui->lineEdit->text(); // get the turbine number
    if (SelectNum.isEmpty()) {
        QMessageBox::warning(this, "Error", "请选择删除机组");
        return;
    }
    int serMachineNum = SelectNum.toInt();

    QString Sqlstr, fanID, farmID;
    Sqlstr = QString("SELECT FarmID FROM windfarm WHERE FarmName = '%1'").arg(this->farmName);
    QSqlQuery query;
    if (!query.exec(Sqlstr)) {
        qDebug() << query.lastError();
        return;
    }
    while (query.next()) {
        farmID = query.value(0).toString();
    }

    Sqlstr = QString("SELECT FanID FROM fan WHERE FanNum= '%1' and FarmID= '%2'").arg(serMachineNum, farmID.toInt());
    if(!query.exec(Sqlstr)) {
        qDebug() << "Error: Failed to execute query." << query.lastError();
        return;
    }
    while (query.next()) {
        fanID = query.value(0).toString();
    }
    if (fanID.isEmpty()) {
        QMessageBox::warning(this, "Error", "This turbine does not exist");
        return;
    }

    QString serTestID;
    Sqlstr = QString("SELECT TestID FROM test WHERE FanID='%1' LIMIT 1").arg(fanID);
    if(!query.exec(Sqlstr)){
        qDebug() << "Error: Failed to execute query." << query.lastError();
        return;
    }
    while (query.next()) {
        serTestID = query.value(0).toString();
    }
    if (!serTestID.isEmpty()) {
        if (QMessageBox::warning(this, "警告", "此机组存在检测记录，删除后检测记录不可恢复!是否继续删除？", QMessageBox::Ok | QMessageBox::Cancel) != QMessageBox::Ok) {
            return;
        } else {
            bool isExistDir;
            QString Sqlstr = QString("SELECT COUNT(*) FROM (SELECT TestID FROM test WHERE FanID='%1') AS TESTNUM").arg(fanID);
            if (!query.exec(Sqlstr)) { qDebug() << "line 196: " << query.lastError(); return; }
            int testRecordNum;
            if (query.next()) { int testRecordNum = query.value(0).toInt(); }
            QVector<QString> testID;
            Sqlstr = QString("SELECT TestID FROM test WHERE FanID='%1'").arg(fanID);
            if(!query.exec(Sqlstr)){ qDebug() << "line 202: " << query.lastError(); return; }
            while(query.next()) {
                testID.append(query.value(0).toString());
            }
            for (int i = 0; i < testRecordNum; i++)
            {
                Sqlstr = QString("SELECT COUNT(*) FROM (SELECT BigImageID FROM bigimages WHERE TestID='%1') AS NUMOFBIG").arg(testID[i]);
                if(!query.exec(Sqlstr)){ qDebug() << "line 208: " << query.lastError(); return; }
                query.next();
                int bigNum = query.value(0).toInt();

                if (bigNum != 0) isExistDir = true;
                QVector<QString> BigImageID;
                Sqlstr = QString("SELECT BigImageID FROM bigimages WHERE TestID='%1'").arg(testID[i]);
                if(!query.exec(Sqlstr)){ qDebug() << "line 215: " << query.lastError();return; }
                while(query.next()) {
                    BigImageID.append(query.value(0).toString());
                }
                for (int j = 0; j < bigNum; j++)
                {
                   Sqlstr = QString("SELECT COUNT(*) FROM (SELECT SmallImageID FROM smallimages WHERE BigImageID='%1') AS SMALLNUM").arg(BigImageID[j]);
                   if(!query.exec(Sqlstr)){ qDebug() << "line 222: " << query.lastError(); return; }
                   query.next();
                   int smallIDNum = query.value(0).toInt();
                   QVector<QString> SmallImageID;
                   Sqlstr = QString("SELECT SmallImageID FROM smallimages WHERE BigImageID='%1'").arg(BigImageID[j]);
                   if(!query.exec(Sqlstr)){ qDebug() << "line 228: " << query.lastError(); return; }
                   while(query.next()) {
                       SmallImageID.append(query.value(0).toString());
                   }
                   for (int k = 0; k < smallIDNum; k++)
                   {
                       Sqlstr = QString("SELECT COUNT(*) FROM (SELECT DefectInfoID FROM defectinfo WHERE SmallImageID='%1') AS DEFECTNUM").arg(SmallImageID[k]);
                       if(!query.exec(Sqlstr)){ qDebug() << "line 235: " << query.lastError();return;}
                       query.next();
                       int defectIDnum = query.value(0).toInt();
                       QVector<QString> DefectInfoID;
                       Sqlstr = QString("SELECT DefectInfoID FROM defectinfo WHERE SmallImageID='%1'").arg(SmallImageID[k]);
                       if(!query.exec(Sqlstr)){ qDebug() << "line 240: " << query.lastError(); return; }
                       while(query.next()) {
                            DefectInfoID.append(query.value(0).toString());
                       }
                       for (int q = 0; q < defectIDnum; q++)
                       {
                           Sqlstr = QString("DELETE FROM defectinfo WHERE DefectInfoID='%1'").arg(DefectInfoID[q]);
                           if(!query.exec(Sqlstr)){ qDebug() << "line 247: " << query.lastError();
                               QMessageBox::critical(NULL,"Error","删除缺陷失败");
                               return;
                           }
                       }
                       Sqlstr = QString("DELETE FROM smallimages WHERE SmallImageID='%1'").arg(SmallImageID[k]);
                       if (!query.exec(Sqlstr)) {
                           qDebug() << "line 253: " << query.lastError();
                           QMessageBox::critical(NULL,"错误","删除小图失败");
                           return;
                       }
                   }
                   Sqlstr = QString("DELETE FROM bigimages WHERE BigImageID='%1'").arg(BigImageID[j]);
                   if (!query.exec(Sqlstr)) {
                       qDebug() << "line 260: " << query.lastError();
                       QMessageBox::critical(NULL,"错误","删除大图失败");
                       return;
                   }
               }
               Sqlstr = QString("DELETE FROM test WHERE TestID='%1'").arg(testID[i]);
               if (!query.exec(Sqlstr)) {
                   qDebug() << "line 267: " << query.lastError();
                   QMessageBox::critical(NULL,"错误","删除检测记录失败");
                   return;
               }
            }
            Sqlstr = QString("DELETE FROM fan WHERE FanID='%1'").arg(fanID);
            if (!query.exec(Sqlstr)) {
                qDebug() << "line 274: " << query.lastError();
                QMessageBox::critical(NULL,"错误","机组删除失败");
                return;
            } else {
                // DEBUG 删除文件夹
                if (isExistDir) {
                    QString path = QCoreApplication::applicationDirPath();
                    path = path.left(path.lastIndexOf("/"));
                    path = path + "/RECORD" + '/' + this->farmName + '/' + serMachineNum;
                    if (!QDir(path).exists()) {
                        QMessageBox::critical(NULL,"Error","File path delete error");
                        return;
                    } else {
                        QDir directory(path);
                        QStringList files = directory.entryList(QDir::NoDotAndDotDot | QDir::System | QDir::Hidden  | QDir::AllDirs | QDir::Files, QDir::DirsFirst);
                        for(int i = 0; i < files.count(); i++) {
                            QString filePath = path + "/" + files[i];
                            QFile::remove(filePath);
                        }
                        directory.rmdir(path);
                    }
                }
                serMachineNum = 0;
                QMessageBox::information(NULL,"提示","删除成功");
                ui->turbine_table->clearContents();
                RefreshWindTurbineInformation();
                return;
            }
        }
    }
    else {
        Sqlstr = QString("DELETE FROM fan WHERE FanID='%1'").arg(fanID);
        if (!query.exec(Sqlstr)) {
            qDebug() << "line 307: " << query.lastError();
            QMessageBox::critical(NULL,"错误","机组删除失败");
            return;
        }
        else {
            serMachineNum = 0;
            QMessageBox::information(NULL,"提示","删除成功");
            RefreshWindTurbineInformation();
            return;
        }

    }
}

//void FillInformation::onTurbineListItemChanged(QListWidgetItem* item)
//{
//    // Do something
//}

void FillInformation::onChangeTurbineInfoButtonClicked()
{
    //Qt code snippet
    QString blades, type, length, height;
    QString Sqlstr;
    blades = ui->machine_le->text();
    type = ui->turbineType_combo->currentText();
    length = ui->bladeLength->text();
    height = ui->wheelsHeight->text();
    int selectrecord, bladelength, wheelheighnt;
    bladelength = length.toInt();
    wheelheighnt = height.toInt();
    QString tmpFanID, tmpFanNUM;

    Sqlstr = QString("SELECT FanID FROM fan WHERE FanType= '%1'LIMIT 1").arg(type);
    QSqlQuery query;
    if(!query.exec(Sqlstr)){
        qDebug()<<"line 349: "<<query.lastError().text();
        return;
    }
    while (query.next()) {
        tmpFanID = query.value("FanID").toString();
    }
    Sqlstr = QString("SELECT FanNum FROM fan WHERE FanNum= '%1' AND FanType= '%2'LIMIT 1").arg(blades, type);
    if(!query.exec(Sqlstr)){
        qDebug()<<"line 357: "<<query.lastError().text();
        return;
    }
    while (query.next()) {
        tmpFanNUM = query.value(0).toString();
    }

    if (type == "" || length == "" || height == "")
    {
        QMessageBox::warning(this,"提示","填写信息不完整");
        return;
    }
    if (bladelength == 0 || wheelheighnt == 0)
    {
        QMessageBox::warning(this,"错误","叶片长度或轮毂高度填写不正确");
        return;
    }
    if (bladelength >= wheelheighnt)
    {
        QMessageBox::warning(this,"错误","叶片长度应小于轮毂高度");
        return;
    }

    if (tmpFanID == "")
    {
        int res;
        query.prepare("INSERT INTO fan (FanNum,BladeLength,WheelHubHeight,FanType) VALUES (:blades, :length, :height, :type)");
        query.bindValue(":blades", blades);
        query.bindValue(":length", length);
        query.bindValue(":height", height);
        query.bindValue(":type", type);
        if(!query.exec()) {
            qDebug() << "line 389: " << query.lastError().text();
            QMessageBox::warning(this,"提示","新增机型信息失败");
            return;
        }
        else
        {
            QMessageBox::warning(this,"提示","新增机型信息成功");
            RefreshWindTurbineInformation();
            ui->turbineType_combo->setCurrentText(type);
            ui->bladeLength->setText(length);
            ui->wheelsHeight->setText(height);
            return;
        }
    }
    else
    {
        if (tmpFanNUM != "")
        {
            query.prepare("UPDATE fan SET BladeLength= :length,WheelHubHeight= :height WHERE FanType= :type");
            query.bindValue(":length", length);
            query.bindValue(":height", height);
            query.bindValue(":type", type);
            if(!query.exec()) {
                qDebug() << "Error: " << query.lastError().text();
                QMessageBox::warning(this,"提示","机型信息更改失败");
                return;
            }
            else
            {
                QMessageBox::warning(this,"提示","机型信息更改成功");
                RefreshWindTurbineInformation();
                ui->turbineType_combo->setCurrentText(type);
                ui->bladeLength->setText(length);
                ui->wheelsHeight->setText(height);
                return;
            }
        }
    }
}

void FillInformation::InitWindTurbineInfo() {
    QSqlQuery query;
    QString strSql, serFarmID, strFarmName;
    strSql = QString("SELECT FarmID FROM surveyors WHERE SurveyorName='%1';").arg(this->surveyorName);
    if(!query.exec(strSql)) {
        qDebug() << "line 435: " << query.lastError().text();
        return;
    }
    if (query.next()) { serFarmID = query.value(0).toString(); }
    strSql = QString("SELECT FarmName FROM windfarm WHERE FarmID= '%1';").arg(this->farmId);
    if(!query.exec(strSql)) {
        qDebug() << "line 442: " << query.lastError().text();
        return;
    }
    if (query.next()) { strFarmName = query.value(0).toString(); }
    ui->windfarm_text->setText(strFarmName);
    if (!strFarmName.isEmpty()) {
        this->farmName = strFarmName;
        RefreshWindTurbineInformation();
    }

    QString numOfFanModel, Sqlstr;
    Sqlstr = "SELECT COUNT(*) FROM (SELECT DISTINCT FanType from Fan) AS numOfFanType;";
    int FanModelNum;
    query.exec(Sqlstr);
    if (query.next()) { FanModelNum = query.value(0).toInt(); }
    if (FanModelNum != 0) {
        QString* MachineType = new QString[FanModelNum];
        Sqlstr = "SELECT DISTINCT FanType from Fan;";
        query.exec(Sqlstr);
        int i = 0;
        while (query.next()) {
            MachineType[i++] = query.value(0).toString();
        }
        for (int j = 0; j < FanModelNum; j++) {
            ui->turbineType_combo->addItem(MachineType[j]);
        }
        delete[] MachineType;
    }
}

void FillInformation::RefreshWindTurbineInformation()
{
    ui->turbine_table->setRowCount(0);  //把行数设为零
    ui->turbine_table->clearContents(); //清空列表内容
    QSqlQuery query;
    QString numOfMachines, Sqlstr;
    Sqlstr = QString("SELECT COUNT(*) FROM (SELECT FanNum,FanType,BladeLength AND WheelHubHeight FROM fan WHERE FarmID= (SELECT FarmID FROM windfarm WHERE FarmName= '%1'))AS numOfFFBW").arg(this->farmName);
    if(!query.exec(Sqlstr)) {
        qDebug() << "line 480: " << query.lastError().text();
        return;
    }
    int MachinesNum;
    if (query.next()) { MachinesNum = query.value(0).toInt(); }

    QString* Machines = new QString[MachinesNum * 4];
    Sqlstr = QString("SELECT FanNum, FanType, BladeLength, WheelHubHeight FROM fan WHERE FarmID= (SELECT FarmID FROM windfarm WHERE FarmName= '%1')").arg(this->farmName);
    if(!query.exec(Sqlstr)) {
        qDebug() << "line 488: " << query.lastError().text();
        return;
    }
    int i = 0;
    while (query.next()) {
        for (int j = 0; j < 4; j++) {
            Machines[i++] = query.value(j).toString();
        }
    }

    int count = 0;
    for (int i = 0; i < MachinesNum; i++) {
        ui->turbine_table->insertRow(i);
        for (int j = 0; j < 4; j++) {
            ui->turbine_table->setItem(i, j, new QTableWidgetItem(Machines[i*4 + j]));

        }
    }

    // 居中显示
    int nCount = ui->turbine_table->rowCount();
    int nClumn = ui->turbine_table->columnCount();
    for (int n = 0; n < nCount;n++) {
        for (int m = 0; m < nClumn;m++) {
            // 判断 null
            if (ui->turbine_table->item(n,m))
                ui->turbine_table->item(n,m)->setTextAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
        }
    }
    delete[] Machines;
}


void FillInformation::fromMainwindow(QString v1, QString v2) {
    this->surveyorName = v1;
    this->farmId = v2;

    InitWindTurbineInfo();
}

void FillInformation::mysetupUi()
{
    //显示行号列
    QHeaderView* headerView = ui->turbine_table->verticalHeader();
    headerView->setHidden(true); //false 显示行号列  true Hide

    // Set some properties of the table
    ui->turbine_table->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);  // Auto-adjust column width
    ui->turbine_table->horizontalHeader()->setSectionResizeMode(0, QHeaderView::Interactive);  // Manually adjust column width

    ui->turbine_table->setSelectionMode(QAbstractItemView::SingleSelection);  // Only single selection allowed
    ui->turbine_table->setSelectionBehavior(QAbstractItemView::SelectRows);  // Only entire rows can be selected
    ui->turbine_table->setEditTriggers(QAbstractItemView::NoEditTriggers);   // Table is not editable
    ui->turbineType_combo->setEditable(true);
}

