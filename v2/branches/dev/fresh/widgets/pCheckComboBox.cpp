#include "pCheckComboBox.h"
#include "pGenericTableModel.h"

#include <QEvent>
#include <QHelpEvent>
#include <QListView>
#include <QStylePainter>
#include <QToolTip>
#include <QWhatsThis>
#include <QDebug>

// pCheckComboBoxDelegate

pCheckComboBoxDelegate::pCheckComboBoxDelegate( QObject* parent, QComboBox* cmb )
    : QStyledItemDelegate( parent ), mCombo( cmb )
{
}

bool pCheckComboBoxDelegate::isSeparator( const QModelIndex& index )
{
    return index.data( Qt::AccessibleDescriptionRole ).toString() == QLatin1String( "separator" );
}

void pCheckComboBoxDelegate::setSeparator( QAbstractItemModel* model, const QModelIndex& index, bool set )
{
    model->setData( index, set ? QLatin1String( "separator" ) : QVariant(), Qt::AccessibleDescriptionRole );
    
    if ( model->inherits( "pGenericTableModel" ) ) {
        Qt::ItemFlags flags = model->data( index, pGenericTableModel::ItemFlagsRole ).value<Qt::ItemFlags>();
        
        if ( set ) {
            flags = flags & ~( Qt::ItemIsSelectable | Qt::ItemIsEnabled );
        }
        else {
            flags = flags | Qt::ItemIsSelectable | Qt::ItemIsEnabled;
        }
        
        model->setData( index, QVariant::fromValue( flags ), pGenericTableModel::ItemFlagsRole );
    }
}

void pCheckComboBoxDelegate::paint( QPainter* painter, const QStyleOptionViewItem& option, const QModelIndex& index ) const
{
    QStyleOptionViewItemV4 opt = *qstyleoption_cast<const QStyleOptionViewItemV4*>( &option );
    opt.state = opt.state & ~QStyle::State_HasFocus;
    
    if ( isSeparator( index ) ) {
        QRect rect = option.rect;
        
        if ( const QAbstractItemView* view = qobject_cast<const QAbstractItemView*>( opt.widget ) ) {
            rect.setWidth( view->viewport()->width() );
        }
        
        QStyleOption o;
        o.rect = rect;
        mCombo->style()->drawPrimitive( QStyle::PE_IndicatorToolBarSeparator, &o, painter, mCombo );
    }
    else {
        QStyledItemDelegate::paint( painter, opt, index );
    }
}

QSize pCheckComboBoxDelegate::sizeHint( const QStyleOptionViewItem& option, const QModelIndex& index ) const
{
    if ( isSeparator( index ) ) {
        int pm = mCombo->style()->pixelMetric( QStyle::PM_DefaultFrameWidth, 0, mCombo );
        return QSize( pm, pm );
    }
    
    return QStyledItemDelegate::sizeHint( option, index );
}

// pCheckComboBox

pCheckComboBox::pCheckComboBox( QWidget* parent )
    : QComboBox( parent )
{
    mModel = new pGenericTableModel( this );
    mModel->setColumnCount( 1 );
    
    mView = new QListView( this );
    mView->setUniformItemSizes( true );
    mView->setAlternatingRowColors( false );
    
    mDelegate = new pCheckComboBoxDelegate( this, this );
    
    setView( mView );
    setModel( mModel );
    setItemDelegate( mDelegate );
    
    // some styles force alternating rows color bypassing the QPelette colors...
    mView->setStyleSheet( QString(
        "QListView {"
            "background-color: %1;"
            "alternate-background-color: %1;"
        "}"
        "QListView::item:hover {"
            "background: qlineargradient(x1: 0, y1: 0, x2: 0, y2: 1, stop: 0 %2, stop: 1 %3);"
            "color: %4;"
        "}"
        )
        .arg( palette().color( QPalette::Window ).name() )
        .arg( palette().color( QPalette::Active, QPalette::Highlight ).name() )
        .arg( palette().color( QPalette::Active, QPalette::Highlight ).darker( 130 ).name() )
        .arg( palette().color( QPalette::Active, QPalette::HighlightedText ).name() )
    );
}

pCheckComboBox::~pCheckComboBox()
{
}

void pCheckComboBox::showPopup()
{
    if ( !model() ) {
        return;
    }
    
    Q_ASSERT( model()->inherits( "pGenericTableModel" ) );
    
    const Qt::ItemFlags flags = Qt::ItemIsEnabled | Qt::ItemIsUserCheckable;
    
    for ( int i = 0; i < model()->rowCount( rootModelIndex() ); i++ ) {
        const QModelIndex index = modelIndex( i );
        
        if ( isSeparator( index.row() ) ) {
            continue;
        }
        
        model()->setData( index, QSize( 0, 21 ), Qt::SizeHintRole );
        
        if ( model()->inherits( "pGenericTableModel" ) ) {
            model()->setData( index, QVariant::fromValue( flags ),  pGenericTableModel::ItemFlagsRole );
        }
        
        if ( index.data( Qt::CheckStateRole ).isNull() ) {
            model()->setData( index, Qt::Unchecked, Qt::CheckStateRole );
        }
        
    }
    
    QComboBox::showPopup();
}

void pCheckComboBox::insertSeparator( int index )
{
    const int itemCount = count();
    
    index = qBound( 0, index, itemCount );
    
    if ( index >= maxCount() ) {
        return;
    }
    
    insertItem( index, QIcon(), QString::null );
    pCheckComboBoxDelegate::setSeparator( model(), modelIndex( index ), true );
}

void pCheckComboBox::addSeparator()
{
    insertSeparator( maxCount() );
}

bool pCheckComboBox::isSeparator( int index )
{
    return mDelegate->isSeparator( modelIndex( index ) );
}

void pCheckComboBox::setSeparator( int index, bool set )
{
    mDelegate->setSeparator( model(), modelIndex( index ), set );
}

QList<int> pCheckComboBox::checkedRows() const
{
    return mModel->checkedRows( modelColumn() );
}

QStringList pCheckComboBox::checkedStringList() const
{
    return mModel->checkedStringList( modelColumn() );
}

QModelIndexList pCheckComboBox::checkedIndexes() const
{
    return mModel->checkedIndexes( modelColumn() );
}

void pCheckComboBox::clearCheckStates()
{
    mModel->clearCheckStates( modelColumn() );
}

void pCheckComboBox::retranslateUi()
{
    update();
}

QModelIndex pCheckComboBox::modelIndex( int index ) const
{
    return model()->index( index, modelColumn(), rootModelIndex() );
}

QString pCheckComboBox::text() const
{
    return checkedStringList().join( ", " );
}

bool pCheckComboBox::event( QEvent* event )
{
    switch ( event->type() ) {
        case QEvent::ToolTip:
            QToolTip::showText( static_cast<QHelpEvent*>( event )->globalPos(), text(), this );
            return true;
        case QEvent::WhatsThis:
            QWhatsThis::showText( static_cast<QHelpEvent*>( event )->globalPos(), text(), this );
            return true;
        default:
            return QComboBox::event( event );
    }
}

void pCheckComboBox::changeEvent( QEvent* event )
{
    QComboBox::changeEvent( event );
    
    if ( event->type() == QEvent::LanguageChange ) {
        retranslateUi();
    }
}

void pCheckComboBox::paintEvent( QPaintEvent* event )
{
    Q_UNUSED( event );
    
    QStylePainter painter( this );
    QStyleOptionComboBox option;
    
    initStyleOption( &option );
    option.currentIcon = QIcon();
    option.currentText = text();
    
    painter.setPen( palette().color( QPalette::Text ) );
    painter.drawComplexControl( QStyle::CC_ComboBox, option );
    painter.drawControl( QStyle::CE_ComboBoxLabel, option );
}
