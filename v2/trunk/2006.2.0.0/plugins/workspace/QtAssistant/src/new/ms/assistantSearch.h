#ifndef ASSISTANTSEARCH_H
#define ASSISTANTSEARCH_H
//
#include "ui_assistantSearch.h"
//
#include <QTextBrowser>
//
class assistantSearch : public QWidget, public Ui::assistantSearch
{
	Q_OBJECT
	//
public:
	assistantSearch( QTextBrowser* );
	//
private slots:
	void on_tbPrevious_clicked();
	void on_tbNext_clicked();
	void search( QTextDocument::FindFlags );
	//
private:
	QTextBrowser* tbSource;
	//
};
//
#endif // ASSISTANTSEARCH_H
