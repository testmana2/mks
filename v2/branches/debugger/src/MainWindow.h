#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "ui_MainWindow.h"

class MainWindow : public QMainWindow, public Ui::MainWindow
{
	Q_OBJECT

public:
	MainWindow( QWidget* parent = 0 );
	virtual ~MainWindow();
};

#endif // MAINWINDOW_H
