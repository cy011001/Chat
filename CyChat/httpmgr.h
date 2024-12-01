#ifndef HTTPMGR_H
#define HTTPMGR_H
#include"singleton.h"
#include <QString>
#include<QUrl>
#include<QObject>
#include <QNetworkAccessManager>
#include <QJsonObject>
#include<QJsonDocument>
#include <QNetworkReply>
#include "global.h"

//CRTP 奇异递归模板，编译器编译时，定义以自己为基类，只有在运行的时候才动态实例
class HttpMgr:public QObject, public Singleton<HttpMgr>,
        public std::enable_shared_from_this<HttpMgr>
{
    Q_OBJECT
public:
    //析构公有，在实列析构得时候会调用智能指针得析构，但是智能指针以httpmgr为基类，所以要调用httpmgr得析构
    ~HttpMgr();
    void PostHttpReg(QUrl url, QJsonObject json, ReqId req_id, Modules mod);
private:
    //在模板类中使用了new就要调用构造函数，但是单例类把构造函数私有，因此就要加上友元
    friend class Singleton<HttpMgr>;

    HttpMgr();
    QNetworkAccessManager _manager;
    //发送函数，需要知道哪个模块哪个功能

private slots:
    void slot_http_finish(ReqId id, QString res, ErrorCodes err, Modules mod);
signals:
    //表示http发送完毕
    void sig_http_finish(ReqId id, QString res, ErrorCodes err, Modules mod);
    void sig_reg_mod_finish(ReqId id, QString res, ErrorCodes err);
};

#endif // HTTPMGR_H
