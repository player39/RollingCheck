#ifndef _IPLINEEDIT_H
#define _IPLINEEDIT_H

#include <QtWidgets/QFrame>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QLabel>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QWidget>
#include <QEvent>
#include <QPainter>

class jyipLineEdit :public QLineEdit
{
  Q_OBJECT
public:
  jyipLineEdit(QObject *parent = Q_NULLPTR);
  //返回ip地址栏内容
  QString returnIP();
  //检查ip地址格式
  bool CheckIPAddr(QString IPAddr);
  //初始化ip地址栏通过config.ini
  void initIPLine(QString &IPAddr);
private:
  QLineEdit *m_pLine[4];
  QHBoxLayout *m_pHLayout;

protected:
  virtual void paintEvent(QPaintEvent *event);
};
#endif //_IPLINEEDIT_H