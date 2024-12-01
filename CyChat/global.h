#ifndef GLOBAL_H
#define GLOBAL_H
#include <QWidget>
#include <functional>
#include "QStyle"
#include <QRegularExpression>
#include <memory>
#include <mutex>
#include <iostream>
#include <QByteArray>
#include <QDir>
#include <QSettings>
#include <QJsonObject>

//刷新qss的函数
extern QString gate_url_prefix;
extern std::function<void(QWidget*)>repolish;
enum ReqId{
    ID_GET_VARIFY_CODE = 1001,
    ID_REG_USER = 1002,

};

enum Modules{
    REGISTERMOD = 0,

};

enum ErrorCodes{
    SUCCESS = 0,
    ERR_JSON = 1001,//json解析错误
    ERR_NETWORK = 2,//网络错误

    RPCFAILED = 1002,
    VarifyExpired = 1003,//验证码过期
    VarifyCodeErr = 1004,//验证码错误
    UserExist = 1005,//用户已存在
    PasswdErr = 1006,//密码错误
    EmailNotMatch = 1007,//邮箱不匹配
    PasswdUpFailed = 1008, //更新密码失败
    PasswdInvaild = 1009, // 密码更新失败
};



#endif // GLOBAL_H
