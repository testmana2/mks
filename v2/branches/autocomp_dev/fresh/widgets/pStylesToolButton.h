#ifndef PSTYLESTOOLBUTTON_H
#define PSTYLESTOOLBUTTON_H

#include "MonkeyExport.h"

#include <QToolButton>

class QMenu;
class pStylesActionGroup;

class Q_MONKEY_EXPORT pStylesToolButton : public QToolButton
{
    Q_OBJECT

public:
    pStylesToolButton( const QString& textFormat = QLatin1String( "%1" ), QWidget* parent = 0 );
    
    QString systemStyle() const;
    QAction* systemAction() const;
    
    bool isCheckableActions() const;
    QString currentStyle() const;

public slots:
    void refreshActions();
    
    void setCheckableActions( bool checkable );
    void setCurrentStyle( const QString& style );

protected:
    QMenu* mMenu;
    pStylesActionGroup* mActions;

signals:
    void styleSelected( const QString& style );
};

#endif // PSTYLESTOOLBUTTON_H
