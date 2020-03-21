#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

namespace Ui {
class MainWindow;
}

#include <QUdpSocket>
#include "include/jsonmap.hpp"

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    void InitSearch();
    void UninitSearch();
private slots:
    void on_pushButton_clicked();

    void readResultMsg();//udp数据回调
private:
    Ui::MainWindow *ui;

    QUdpSocket *s_searchsocket;//udp搜索socket
    CJsonMap *s_jsonmap;//cjson数据
};

#endif // MAINWINDOW_H
