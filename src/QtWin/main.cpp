#include "QtWin/RollingCheck.h"
#include <QtWidgets/QApplication>
#include "NetWork/ComOpen.h"
#include "NetWork/TcpLink.h"
#include "NetWork/SendThread.h"
int main(int argc,char *argv[])
{
//  TCP_Link *test = new TCP_Link();
  QApplication a(argc,argv);
  //thread t1;
//  Data_RS *test = new Data_RS();
 // test->start();
  //test *test1=new test();
  jyRollingCheck w;
  w.show();
  return a.exec();
}