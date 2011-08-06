#ifndef XUPITEMVARIABLEEDITORMODEL_H
#define XUPITEMVARIABLEEDITORMODEL_H

#include <QAbstractItemModel>
#include <QHash>
#include <QStringList>

class XUPItem;
class DocumentFilterMap;

struct XUPItemVariableEditorModelItem {
	typedef QList<XUPItemVariableEditorModelItem> List;
	
	XUPItemVariableEditorModelItem( const QString& string = QString::null, bool enabled = false, XUPItem* item = 0 ) {
		this->string = string;
		this->enabled = enabled;
		this->item = item;
	}
	
	bool operator<( const XUPItemVariableEditorModelItem& other ) const {
		return string < other.string;
	}
	
	bool operator==( const XUPItemVariableEditorModelItem& other ) const {
		return string == other.string
			&& enabled == other.enabled
			&& item == other.item
			&& children == other.children
			;
	}
	
	QString string;
	bool enabled;
	XUPItem* item;
	XUPItemVariableEditorModelItem::List children;
};

class XUPItemVariableEditorModel : public QAbstractItemModel {
	Q_OBJECT

public:
	XUPItemVariableEditorModel( QObject* parent = 0 );
	
	virtual int columnCount( const QModelIndex& parent = QModelIndex() ) const;
	virtual QVariant data( const QModelIndex& index, int role = Qt::DisplayRole ) const;
	virtual QModelIndex index( int row, int column, const QModelIndex& parent = QModelIndex() ) const;
	virtual QModelIndex parent( const QModelIndex& index ) const;
	virtual int	rowCount( const QModelIndex& parent = QModelIndex() ) const;
	virtual Qt::ItemFlags flags( const QModelIndex& index ) const;
	virtual bool setData( const QModelIndex& index, const QVariant& value, int role = Qt::EditRole );
	
	XUPItemVariableEditorModelItem* item( const QModelIndex& index ) const;
	QModelIndex childIndex( const QModelIndex& index, const QString& string ) const;
	
	void clear();
	void setRootItem( const XUPItemVariableEditorModelItem& item );
	void setRootItem( XUPItem* item );
	
	QModelIndex addVariable( const QString& variable );
	QModelIndex addValue( const QModelIndex& variable, const QString& value );
	
	QStringList fileVariables() const;
	QStringList pathVariables() const;
	QStringList knownVariables() const;
	
	void setFilteredVariables( const QStringList& filters );
	QStringList filteredVariables() const;
	
	QString quoteString() const;
	bool quoteValues() const;
	QString quotedValue( const QString& value ) const;

public slots:
	virtual void revert();
	virtual bool submit();
	
	void setQuoteString( const QString& string );
	void setQuoteValues( bool quote );

protected:
	XUPItem* mRootItem;
	const DocumentFilterMap* mDocumentFilterMap;
	mutable XUPItemVariableEditorModelItem mRoot;
	QHash<XUPItemVariableEditorModelItem*, XUPItemVariableEditorModelItem*> mMapping;
	QStringList mFilteredVariables;
	QString mQuoteString;
	bool mQuoteValues;
	
	void buildParentMapping( XUPItemVariableEditorModelItem& item );
	void setRootItemInternal( const XUPItemVariableEditorModelItem& item, bool clearModel );
	void revert( XUPItem* item );
};

#endif // XUPITEMVARIABLEEDITORMODEL_H
