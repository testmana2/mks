#ifndef PTEMPLATEPREVIEWER_H
#define PTEMPLATEPREVIEWER_H

#include <QWidget>

class QLabel;
class pEditor;

class pTemplatePreviewer : public QWidget
{
	Q_OBJECT

protected:
	QLabel* mLabel;
	pEditor* mEditor;

public:
	pTemplatePreviewer( QWidget* = 0 );
	virtual ~pTemplatePreviewer();

	void setText( const QString& );
	QString text() const;

	void setLanguage( const QString& );
	QString language() const;

	void setName( const QString& );
	QString name() const;

	void setOpen( bool );
	bool open() const;

	bool open( const QString& );

};

#endif // PTEMPLATEPREVIEWER_H
 
