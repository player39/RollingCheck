#include "NetWork/SendThread.h"
#include <QtCore/QDateTime>


jySendThread::jySendThread( QObject* parent)
{
  
}
//����run�������ڵ������е��߳��з������շ�����
void jySendThread::run()
{
  while (1)
  {
    qDebug() << "thread working---------";
    QDateTime nowtime = QDateTime::currentDateTime();
    //�����ź�
    emit sigCircle();
    qDebug() << nowtime.toString("hh:mm:ss.zzz");
    //˯�������Ժ����ִ��
    sleep(2);
  }
  
}