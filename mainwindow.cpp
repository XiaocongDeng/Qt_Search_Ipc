#include "mainwindow.h"
#include "ui_mainwindow.h"


#include <QDebug>
using namespace std;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    InitSearch();
}

MainWindow::~MainWindow()
{
    delete ui;
    UninitSearch();
}

//初始化搜索
void MainWindow::InitSearch()
{
    s_jsonmap = new CJsonMap;//json 初始化
    s_searchsocket = new QUdpSocket(this);//udp
    if(s_searchsocket->state()!=s_searchsocket->BoundState) {
        if( !s_searchsocket->bind(QHostAddress::AnyIPv4, SEARCH_PORT, QUdpSocket::ShareAddress) ) {
            cout<<"bind failed , UDP search initialization error !"<<endl;
        }
    } else {
        cout<<"socket state failed , UDP search initialization error !"<<endl;
    }

    if( s_searchsocket->joinMulticastGroup(QHostAddress(SEARCH_HOSTADDR)) ) {
        s_searchsocket->setSocketOption(QAbstractSocket::MulticastLoopbackOption, 0);
        //设置缓冲区
        s_searchsocket->setSocketOption(QAbstractSocket::ReceiveBufferSizeSocketOption, SOCKET_RECEIVE_BUFFER);
        s_searchsocket->setSocketOption(QAbstractSocket::MulticastTtlOption, 2);
        //连接接收信号槽
        connect(s_searchsocket,SIGNAL(readyRead()),this,SLOT(readResultMsg()));
    } else {
        cout<<"join multicast Group failed , UDP search initialization errro !"<<endl;
    }
}

//注销初始化
void MainWindow::UninitSearch()
{
    disconnect(s_searchsocket,SIGNAL(readyRead()),this,SLOT(readResultMsg()));
    s_searchsocket->deleteLater();
    delete s_searchsocket;s_searchsocket = NULL;
    delete s_jsonmap;s_jsonmap = NULL;
}

//读取搜索回复数据
void MainWindow::readResultMsg()
{
    char bodyData[1024]={0};
    char msg[2048]={0};
    memset(&bodyData,0,sizeof(bodyData));
    memset(&msg,0,sizeof(msg));

    //QByteArray tmpdata=s_searchsocket->readAll();
    s_searchsocket->readDatagram(msg,2048);
    cJSON *jsondata = cJSON_Parse(msg);
    JsonMsg_T msgdata;
    memset( &msgdata,0,sizeof(msgdata) );
    if(jsondata == NULL) {
        return;
    }

   s_jsonmap->JSonPraseMsg(jsondata, &msgdata);
   if( strcmp(msgdata.method,"request") == 0) {
       cJSON_Delete(jsondata);
       return;
   }
   SearchInfo_T searchResult;
   memset(&searchResult,0,sizeof(searchResult));
   if(msgdata.bodyData!=NULL) {
        s_jsonmap->JsonSearchResultprase(msgdata.bodyData,&searchResult);
        if( strlen(searchResult.ip) > 0 ) {
            //========================================================================//
            //搜索回复结果: searchResult
            qDebug()<<"respose msg: "<<searchResult.ip;
        }
   }

   cJSON_Delete(jsondata);jsondata = NULL;
}

void MainWindow::on_pushButton_clicked()
{
    cJSON*msg= s_jsonmap->JsonSearchInfoMake();//搜索组包
    if(msg == NULL) {
        cout<< "search json msg is NULL !"<<endl;
        return ;
    }
    char*str = cJSON_PrintUnformatted(msg);
    if( !s_searchsocket->writeDatagram(str,strlen(str), QHostAddress(SEARCH_HOSTADDR), SEARCH_PORT) ) {
        cout<<"write search msg error !";
    }
    delete msg;msg = NULL;
}
