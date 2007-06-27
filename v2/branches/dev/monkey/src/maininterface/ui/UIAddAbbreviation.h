#ifndef UIADDABBREVIATION_H
#define UIADDABBREVIATION_H

#include "MonkeyExport.h"
#include "ui_UIAddAbbreviation.h"

class Q_MONKEY_EXPORT UIAddAbbreviation : public QDialog, public Ui::UIAddAbbreviation
{
	Q_OBJECT

public:
	UIAddAbbreviation( QWidget* = 0 );
	~UIAddAbbreviation();

};

#endif // UIADDABBREVIATION_H
