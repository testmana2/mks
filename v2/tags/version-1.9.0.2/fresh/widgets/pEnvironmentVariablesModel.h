/****************************************************************************
    Copyright (C) 2005 - 2011  Filipe AZEVEDO & The Monkey Studio Team
    http://monkeystudio.org licensing under the GNU GPL.

    This program is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 2 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program; if not, write to the Free Software
    Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
****************************************************************************/
#ifndef PENVIRONMENTVARIABLESMODEL_H
#define PENVIRONMENTVARIABLESMODEL_H

#include "MonkeyExport.h"

#include <QAbstractItemModel>

class Q_MONKEY_EXPORT pEnvironmentVariablesModel : public QAbstractItemModel
{
    Q_OBJECT

public:
    struct Variable
    {
        Variable( const QString& _name = QString::null, const QString& _value = QString::null, bool _enabled = false )
        {
            name = _name;
            value = _value;
            enabled = _enabled;
        }

        QString name;
        QString value;
        bool enabled;
    };

    typedef QHash<QString, pEnvironmentVariablesModel::Variable> Variables;
    typedef QList<pEnvironmentVariablesModel::Variable*> VariableList;

    pEnvironmentVariablesModel( QObject* parent = 0 );

    virtual int columnCount( const QModelIndex& parent = QModelIndex() ) const;
    virtual QVariant data( const QModelIndex& index, int role = Qt::DisplayRole ) const;
    virtual QModelIndex index( int row, int column, const QModelIndex& parent = QModelIndex() ) const;
    virtual QModelIndex parent( const QModelIndex& index ) const;
    virtual int rowCount( const QModelIndex& parent = QModelIndex() ) const;

    virtual QVariant headerData( int section, Qt::Orientation orientation, int role = Qt::DisplayRole ) const;
    virtual bool hasChildren( const QModelIndex& parent = QModelIndex() ) const;
    virtual Qt::ItemFlags flags( const QModelIndex& index ) const;
    virtual bool setData( const QModelIndex& index, const QVariant& value, int role = Qt::EditRole );

    QModelIndex index( const QString& name, int column = 0 ) const;
    pEnvironmentVariablesModel::Variable variable( const QModelIndex& index ) const;

    const pEnvironmentVariablesModel::Variables& variables() const;
    const pEnvironmentVariablesModel::Variables& defaultVariables() const;
    QStringList variables( bool keepDisabled ) const;
    pEnvironmentVariablesModel::Variable variable( const QString& name ) const;
    bool contains( const QString& name ) const;
    bool isEmpty() const;
    
    static pEnvironmentVariablesModel::Variables stringListToVariables( const QStringList& variables );
    static QStringList variablesToStringList( const pEnvironmentVariablesModel::Variables& variables, bool keepDisabled );

public slots:
    void setVariables( const pEnvironmentVariablesModel::Variables& variables, bool setDefault );
    void setDefaultVariables( const pEnvironmentVariablesModel::Variables& variables );
    void setVariables( const QStringList& variables, bool setDefault );
    void setVariable( const QString& name, const pEnvironmentVariablesModel::Variable& variable );
    void removeVariable( const QString& name );
    void clearVariables();
    void resetVariablesToDefault();
    void resetVariablesToSystem( bool setDefault );

protected:
    mutable int mRowCount;
    mutable pEnvironmentVariablesModel::Variables mDefaultVariables;
    mutable pEnvironmentVariablesModel::Variables mVariables;
    mutable pEnvironmentVariablesModel::VariableList mOrder;

signals:
    void defaultVariablesChanged();
};

Q_DECLARE_METATYPE( pEnvironmentVariablesModel::Variable );
Q_DECLARE_METATYPE( pEnvironmentVariablesModel::Variables )

#endif // PENVIRONMENTVARIABLESMODEL_H
