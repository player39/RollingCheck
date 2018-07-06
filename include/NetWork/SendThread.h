#ifndef _SENDTHREAD_H
#define _SENDTHREAD_H

#include <QtCore/QThread>
#include <QtNetWork/QtNetwork>
#include "NetWork/TcpLink.h"

class jySendThread : public QThread
{
  Q_OBJECT
public:

  jySendThread(QObject *parent = 0);
private:
  jyTcpLink *m_pTcpLink;

signals:
  //每隔两秒发送一个信号
  void sigCircle();

public slots:

protected:
  //重载run函数
  void run() override;
};

#endif // !_M_SENDTHREAD_H
