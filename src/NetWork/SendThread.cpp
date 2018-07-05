#include "NetWork/SendThread.h"
#include <QtCore/QDateTime>


jySendThread::jySendThread( QObject* parent)
{
  
}
//重载run函数，在单独运行的线程中反复的收发数据
void jySendThread::run()
{
  while (1)
  {
    qDebug() << "thread working---------";
    QDateTime nowtime = QDateTime::currentDateTime();
    //发送信号
    emit sigCircle();
    qDebug() << nowtime.toString("hh:mm:ss.zzz");
    //睡眠两秒以后继续执行
    sleep(2);
  }
  
}