#include "logindialog.h"
#include "ui_logindialog.h"

LoginDialog::LoginDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::LoginDialog)
{
    ui->setupUi(this);
    connect(ui->regist_Button, &QPushButton::clicked, this, &LoginDialog::switchRegister);
}

LoginDialog::~LoginDialog()
{
    delete ui;
}
