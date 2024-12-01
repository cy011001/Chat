#include "registerdialog.h"
#include "ui_registerdialog.h"
#include "global.h"
#include "httpmgr.h"
RegisterDialog::RegisterDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::RegisterDialog)
{
    ui->setupUi(this);
    ui->pass_Edit->setEchoMode(QLineEdit::Password);
    ui->confirm_Edit->setEchoMode(QLineEdit::Password);
    ui->err_tip->setProperty("state","normal");
    repolish(ui->err_tip);
    initHttpHandlers();
    connect(HttpMgr::GetInstance().get(), &HttpMgr::sig_reg_mod_finish,this, &RegisterDialog::slot_reg_mod_finish);

}

RegisterDialog::~RegisterDialog()
{
    delete ui;
}

void RegisterDialog::on_getCode_btn_clicked()
{
    auto email = ui->email_Edit->text();
    //正则表的式
    QRegularExpression regex(R"((\w+)(\.|_)?(\w*)@(\w+)(\.(\w+))+)");
    bool match = regex.match(email).hasMatch();
    if(match){
        QJsonObject json_obj;
                json_obj["email"] = email;
                HttpMgr::GetInstance()->PostHttpReg(QUrl("http://localhost:8080/get_varifycode"),
                             json_obj, ReqId::ID_GET_VARIFY_CODE,Modules::REGISTERMOD);
    }
    else{
        showTip(tr("邮箱地址不正确"), false);
    }
}

void RegisterDialog::slot_reg_mod_finish(ReqId id, QString res, ErrorCodes err)
{
    if(err != ErrorCodes::SUCCESS){
        showTip(tr("网络错误"),false);
        return;
    }
    //解析json字符串， res转换成byteArray
    QJsonDocument jsonDoc = QJsonDocument::fromJson(res.toUtf8());

    if(jsonDoc.isNull()){
        showTip(tr("json 解析失败"),false);
        return;
    }

    if(!jsonDoc.isObject()){
        showTip(tr("json 解析失败"),false);
        return;
    }
    QJsonObject jsonObj = jsonDoc.object();
    _handlers[id](jsonDoc.object());
    return;
}

void RegisterDialog::initHttpHandlers()
{
    //注册获取验证码回包逻辑
    _handlers.insert(ReqId::ID_GET_VARIFY_CODE, [this](QJsonObject jsonObj){
        int error = jsonObj["error"].toInt();
        if(error != ErrorCodes::SUCCESS){
            showTip(tr("参数错误"),false);
            return;
        }
        auto email = jsonObj["email"].toString();
        showTip(tr("验证码已发送到邮箱，注意查收"), true);
        qDebug()<< "email is " << email ;
    });
    //注册注册用户回包逻辑
        _handlers.insert(ReqId::ID_REG_USER, [this](QJsonObject jsonObj){
            int error = jsonObj["error"].toInt();
            if(error != ErrorCodes::SUCCESS){
                showTip(tr("参数错误"),false);
                return;
            }
            auto email = jsonObj["email"].toString();
            showTip(tr("用户注册成功"), true);
            qDebug()<< "email is " << email ;
        });
}
void RegisterDialog::showTip(QString str, bool b_ok)
{

    if(b_ok){
        ui->err_tip->setProperty("state", "normal");

    }
    else{
       ui->err_tip->setProperty("state", "err");
    }
    ui->err_tip->setText(str);

    repolish(ui->err_tip);
}

void RegisterDialog::on_confirm_btn_clicked()
{
    if(ui->user_Edit->text() == ""){
            showTip(tr("用户名不能为空"), false);
            return;
        }
        if(ui->email_Edit->text() == ""){
            showTip(tr("邮箱不能为空"), false);
            return;
        }
        if(ui->pass_Edit->text() == ""){
            showTip(tr("密码不能为空"), false);
            return;
        }
        if(ui->confirm_Edit->text() == ""){
            showTip(tr("确认密码不能为空"), false);
            return;
        }
        if(ui->confirm_Edit->text() != ui->pass_Edit->text()){
            showTip(tr("密码和确认密码不匹配"), false);
            return;
        }
        if(ui->variftEdit->text() == ""){
            showTip(tr("验证码不能为空"), false);
            return;
        }
        //day11 发送http请求注册用户
        QJsonObject json_obj;
        json_obj["user"] = ui->user_Edit->text();
        json_obj["email"] = ui->email_Edit->text();
        json_obj["passwd"] = ui->pass_Edit->text();
        json_obj["confirm"] = ui->confirm_Edit->text();
        json_obj["varifycode"] = ui->variftEdit->text();
        HttpMgr::GetInstance()->PostHttpReg(QUrl(gate_url_prefix+"/user_register"),
                     json_obj, ReqId::ID_REG_USER,Modules::REGISTERMOD);
}
