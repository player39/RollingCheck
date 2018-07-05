#ifndef _CHECK_ROLLING_H
#define _CHECK_ROLLING_H

#include <QtWidgets/QMainWindow>
#include <ui_Rolling_Check.h>
//#include"Main_Widget.h"
class Check_Rolling : public QMainWindow
{
	Q_OBJECT

public:
	Check_Rolling(QWidget *parent = Q_NULLPTR);

private:
	Ui::Check_RollingClass ui;
	//Main_Widget *m_widget;
};
#endif