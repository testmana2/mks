#ifndef QCTAGSSENSELANGUAGESMODEL_H
#define QCTAGSSENSELANGUAGESMODEL_H

#include <QAbstractItemModel>
#include <QStringList>

class qCtagsSenseSQL;

class qCtagsSenseLanguagesModel : public QAbstractItemModel
{
	Q_OBJECT
	
public:
	qCtagsSenseLanguagesModel( qCtagsSenseSQL* parent );
	virtual ~qCtagsSenseLanguagesModel();
	
	virtual int columnCount( const QModelIndex& parent = QModelIndex() ) const;
	virtual QVariant data( const QModelIndex& index, int role = Qt::DisplayRole ) const;
	virtual QModelIndex index( int row, int column, const QModelIndex& parent = QModelIndex() ) const;
	virtual QModelIndex parent( const QModelIndex& index ) const;
	virtual int rowCount( const QModelIndex& parent = QModelIndex() ) const;
	virtual bool hasChildren( const QModelIndex& parent = QModelIndex() ) const;
	
	QString language( int id ) const;
	int indexOf( const QString& language ) const;
	
public slots:
	void refresh();
	
protected:
	qCtagsSenseSQL* mSQL;
	QStringList mLanguages;
};

#endif // QCTAGSSENSELANGUAGESMODEL_H