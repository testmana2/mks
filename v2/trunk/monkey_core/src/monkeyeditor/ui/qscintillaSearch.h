#ifndef QSCINTILLASEARCH_H
#define QSCINTILLASEARCH_H
//
#include "ui_qscintillaSearch.h"
#include "MonkeyExport.h"
//
#include <QPointer>
//
class QsciScintilla;
//
class Q_MONKEY_EXPORT qscintillaSearch : public QDockWidget, public Ui::qscintillaSearch
{
	Q_OBJECT
	//
public:
	static qscintillaSearch* self( QsciScintilla* = 0 );
	QsciScintilla* editor() const;
	void setEditor( QsciScintilla* );
	//
public slots:
	bool on_tbPrevious_clicked();
	bool on_tbNext_clicked();
	bool on_tbReplace_clicked();
	void on_tbReplaceAll_clicked();
private:
	qscintillaSearch( QsciScintilla* = 0 );
	bool checkEditor();
	static QPointer<qscintillaSearch> mSelf;
	QPointer<QsciScintilla> mEditor;
	//
};
//
#endif // QSCINTILLASEARCH_H
