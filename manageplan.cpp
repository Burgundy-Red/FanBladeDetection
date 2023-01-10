#include "manageplan.h"

#include "ui_Manageplan.h"

Manageplan::Manageplan(QWidget *parent) : QMainWindow(parent),
    ui(new Ui::ManageplanWindow())
{
    ui->setupUi(this);
}


Manageplan::~Manageplan()
{
    delete ui;
}
