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
  //ÿ�����뷢��һ���ź�
  void sigCircle();

public slots:

protected:
  //����run����
  void run() override;
};

#endif // !_M_SENDTHREAD_H
