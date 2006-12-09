#ifndef MONKEY_H
#define MONKEY_H
//
#include <QApplication>
#include <QPointer>
//
class Settings;
class UIMain;
class StatusBar;
class MenuTools;
class MenuBar;
class Workspace;
class ProjectManager;
//
class Monkey : public QApplication
{
	Q_OBJECT
	//
public:
	static Monkey* self( int&, char** );
	~Monkey();
	//
	void initialize();
	//
	Settings* settings();
	UIMain* main();
	StatusBar* statusBar();
	MenuBar* menuBar();
	MenuTools* menuTools();
	Workspace* workspace();
	ProjectManager* projectManager();
	//
private:
	Monkey( int&, char** );
	//
	static QPointer<Monkey> mSelf;
	//
};
//
#endif // MONKEY_H
