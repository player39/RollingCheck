#ifndef _IPLINEEDIT_H
#define _IPLINEEDIT_H
#include<QtWidgets/QFrame>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QLabel>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QWidget>
class jyipLineEdit :public QWidget
{
  Q_OBJECT
public:
  jyipLineEdit(QObject *parent = 0);
private:
  QLineEdit *m_pLine[4];
  QLabel *m_pLabel[3];
  QHBoxLayout *m_pHLayout;
};
#endif