#include "myqq.h"
#include "tinyxml/tinyxml.h"
#include "tinyxml/tinystr.h"

void Myqq::checkAndSetConfig()
{
    TiXmlDocument config_dom(config_file);
    if(config_dom.LoadFile())
    {
        cout<<"找到配置文件。。。\n";
    }
    else
    {
        init_file();
        return;
    }
    TiXmlElement* root = config_dom.RootElement();
    if(root == NULL)
        return;
    TiXmlElement* UsersElement = root->FirstChildElement();
    if(UsersElement==NULL)
        return;
    TiXmlElement* UserElement = UsersElement->FirstChildElement();
    if(UserElement==NULL)
        return;
    TiXmlElement *account = UserElement->FirstChildElement();
    TiXmlElement *password = account->NextSiblingElement();
    TiXmlElement *mode = password->NextSiblingElement();
    this->account = atoi(account->GetText());
    strcpy(this->password,password->GetText());
    if(atoi(mode->GetText())==0)
        qq->mode = QQ_ONLINE;
    else
        qq->mode = QQ_HIDDEN;
}
void Myqq::init_file()
{
    //创建一个XML的文档对象。
    TiXmlDocument *myDocument = new TiXmlDocument();
    TiXmlDeclaration *title = new TiXmlDeclaration("1.0","utf-8","yes");
    myDocument->LinkEndChild(title);

    TiXmlComment *comment = new TiXmlComment("update the information below");
    myDocument->LinkEndChild(comment);


    //创建一个根元素并连接。
    TiXmlElement *RootElement = new TiXmlElement("myqq_config"); myDocument->LinkEndChild(RootElement);
    //创建一个users元素并连接。
    TiXmlElement *UsersElement = new TiXmlElement("users");
    RootElement->LinkEndChild(UsersElement);
    //创建一个user元素并连接。
    TiXmlElement *userElement = new TiXmlElement("user");
    UsersElement->LinkEndChild(userElement);
    //设置user元素的属性。

    TiXmlElement *tempElement = new TiXmlElement("account");
    userElement->LinkEndChild(tempElement);
    TiXmlText *tempContent = new TiXmlText("0");
    tempElement->LinkEndChild(tempContent);

    tempElement = new TiXmlElement("password");
    userElement->LinkEndChild(tempElement);
    tempContent = new TiXmlText("0");
    tempElement->LinkEndChild(tempContent);

    tempElement = new TiXmlElement("mode");
    userElement->LinkEndChild(tempElement);
    tempContent = new TiXmlText("0");
    tempElement->LinkEndChild(tempContent);

    tempElement = new TiXmlElement("QQPacketLog");
    RootElement->LinkEndChild(tempElement);
    tempContent = new TiXmlText("false");
    tempElement->LinkEndChild(tempContent);

    tempElement = new TiXmlElement("QQTerminalLog");
    RootElement->LinkEndChild(tempElement);
    tempContent = new TiXmlText("false");
    tempElement->LinkEndChild(tempContent);

    tempElement = new TiXmlElement("QQLogDir");
    RootElement->LinkEndChild(tempElement);
    tempContent = new TiXmlText("./log");
    tempElement->LinkEndChild(tempContent);

    tempElement = new TiXmlElement("QQVerifyDir");
    RootElement->LinkEndChild(tempElement);
    tempContent = new TiXmlText("./verify");
    tempElement->LinkEndChild(tempContent);

    tempElement = new TiXmlElement("QQNetwork");
    RootElement->LinkEndChild(tempElement);
    tempContent = new TiXmlText("UDP");
    tempElement->LinkEndChild(tempContent);

    tempElement = new TiXmlElement("QQTcpServerList");
    RootElement->LinkEndChild(tempElement);

    TiXmlElement *itemElement = new TiXmlElement("item");
    tempElement->LinkEndChild(itemElement);
    tempContent = new TiXmlText("219.133.60.173:443");
    itemElement->LinkEndChild(tempContent);

    itemElement = new TiXmlElement("item");
    tempElement->LinkEndChild(itemElement);
    tempContent = new TiXmlText("219.133.49.125:443");
    itemElement->LinkEndChild(tempContent);

    itemElement = new TiXmlElement("item");
    tempElement->LinkEndChild(itemElement);
    tempContent = new TiXmlText("58.60.15.33:443");
    itemElement->LinkEndChild(tempContent);

    itemElement = new TiXmlElement("item");
    tempElement->LinkEndChild(itemElement);
    tempContent = new TiXmlText("219.133.60.173:443");
    itemElement->LinkEndChild(tempContent);

    itemElement = new TiXmlElement("item");
    tempElement->LinkEndChild(itemElement);
    tempContent = new TiXmlText("219.133.60.173:443");
    itemElement->LinkEndChild(tempContent);

    itemElement = new TiXmlElement("item");
    tempElement->LinkEndChild(itemElement);
    tempContent = new TiXmlText("219.133.60.173:443");
    itemElement->LinkEndChild(tempContent);

    tempElement = new TiXmlElement("QQUdpServerList");
    RootElement->LinkEndChild(tempElement);

    itemElement = new TiXmlElement("item");
    tempElement->LinkEndChild(itemElement);
    tempContent = new TiXmlText("219.133.49.171:8000");
    itemElement->LinkEndChild(tempContent);

    itemElement = new TiXmlElement("item");
    tempElement->LinkEndChild(itemElement);
    tempContent = new TiXmlText("58.60.14.37:8000");
    itemElement->LinkEndChild(tempContent);

    itemElement = new TiXmlElement("item");
    tempElement->LinkEndChild(itemElement);
    tempContent = new TiXmlText("219.133.60.36:8000");
    itemElement->LinkEndChild(tempContent);

    itemElement = new TiXmlElement("item");
    tempElement->LinkEndChild(itemElement);
    tempContent = new TiXmlText("sz6.tencent.com:8000");
    itemElement->LinkEndChild(tempContent);

    itemElement = new TiXmlElement("item");
    tempElement->LinkEndChild(itemElement);
    tempContent = new TiXmlText("sz7.tencent.com:8000");
    itemElement->LinkEndChild(tempContent);

    itemElement = new TiXmlElement("item");
    tempElement->LinkEndChild(itemElement);
    tempContent = new TiXmlText("sz8.tencent.com:8000");
    itemElement->LinkEndChild(tempContent);

    itemElement = new TiXmlElement("item");
    tempElement->LinkEndChild(itemElement);
    tempContent = new TiXmlText("sz9.tencent.com:8000");
    itemElement->LinkEndChild(tempContent);


    myDocument->SaveFile(config_file);
    cout<<"初始化配置文件到"<<config_file<<endl;
}
int Myqq::save_file()
{
    TiXmlDocument mystarconf(config_file);
    if(mystarconf.LoadFile())
    {

        TiXmlElement* mystarroot = mystarconf.RootElement();
        if(mystarroot == NULL)
            return 1;
        TiXmlElement* UsersElement = mystarroot->FirstChildElement();
        if(UsersElement==NULL)
            return 1;
        TiXmlElement* UserElement = UsersElement->FirstChildElement();
        if(UserElement==NULL)
            return 1;
        TiXmlElement *nickname = UserElement->FirstChildElement();
        TiXmlElement *username = nickname->NextSiblingElement();
        TiXmlElement *password = username->NextSiblingElement();
        TiXmlElement *fakeAddress = password->NextSiblingElement();

        TiXmlText *tempContent = new TiXmlText("QQ账号");
        nickname->ReplaceChild(nickname->FirstChild(), *tempContent);

        tempContent = new TiXmlText("密码");
        username->ReplaceChild(username->FirstChild(), *tempContent);

        TiXmlElement *config = UsersElement->NextSiblingElement();
        TiXmlElement *autologinElement = config->FirstChildElement();

        cout<<"save to "<<config_file<<endl;
        mystarconf.SaveFile();
        return 0;
    }
}
