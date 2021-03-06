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
/*!
    \file pCommand.h
    \date 2008-01-14T00:36:50
    \author Filipe AZEVEDO aka Nox PasNox <pasnox@gmail.com>
    \brief Header for pCommand
*/
#ifndef PCOMMAND_H
#define PCOMMAND_H

#include <MonkeyExport.h>

#include "xupmanager/core/XUPProjectItem.h"

#include <QStringList>
#include <QVariant>
#include <QPointer>

/*!
    Container for storing console command
    
    pCommand can store command line for executing command, working dirrectory,
    options of execution of command, can define parsers, which could be used 
    for executing, and also can remember project, for which command is 
    executing
*/
class Q_MONKEY_EXPORT pCommand
{
public:
    typedef QList<pCommand> List;
    
    pCommand( const QString& t = QString::null, const QString& p = QString::null )
    {
        mText = t;
        mSkipOnError = false;
        mWorkingDirectory = p;
        mTryAllParsers = false;
        mProject = 0;
        mExecutableCheckingType = XUPProjectItem::NoTarget;
    }
    
    pCommand( const QString& t, const QString& c, bool b, const QStringList& p = QStringList(), const QString& d = QString::null, bool bb = false )
    {
        mText = t;
        mCommand = c;
        mSkipOnError = b;
        mParsers = p;
        mWorkingDirectory = d;
        mTryAllParsers = bb;
        mProject = 0;
        mExecutableCheckingType = XUPProjectItem::NoTarget;
    }
    ~pCommand() {}
    
    bool isValid() const
    {
        bool valid = !name().isEmpty() && !text().isEmpty() && !command().isEmpty();
        
        if ( !valid ) {
            foreach ( const pCommand& cmd, mChildCommands ) {
                valid = cmd.isValid();
                
                if ( valid ) {
                    break;
                }
            }
        }
        
        return valid;
    }
    
    bool operator==( const pCommand& t ) const
    { return mName == t.mName && mText == t.mText && mCommand == t.mCommand &&
            mWorkingDirectory == t.mWorkingDirectory && mParsers == t.mParsers && mSkipOnError == t.mSkipOnError &&
            mTryAllParsers == t.mTryAllParsers && mUserData == t.mUserData && mProject == t.mProject &&
            mExecutableCheckingType == t.mExecutableCheckingType;
        ;
    }

    QString name() const { return mName; }
    QString text() const { return mText; }
    QString command() const { return mCommand; }
    QString workingDirectory() const { return mWorkingDirectory; }
    QStringList parsers() const { return mParsers; }
    bool skipOnError() const { return mSkipOnError; }
    bool tryAllParsers() const { return mTryAllParsers; }
    QVariant userData() const { return mUserData; }
    XUPProjectItem* project() const { return mProject; }
    pCommand::List childCommands() const { return mChildCommands; }
    int executableCheckingType() { return mExecutableCheckingType; }

    void setName( const QString& s ) { mName = s; }
    void setText( const QString& s ) { mText = s.trimmed(); }
    void setCommand( const QString& s ) { mCommand = s.trimmed(); }
    void setWorkingDirectory( const QString& s ) { mWorkingDirectory = s.trimmed(); }
    void addParser( const QString& p ) { if ( !mParsers.contains( p ) ) mParsers << p.trimmed(); }
    void setParsers( const QStringList& p ) { mParsers.clear(); addParsers( p ); }
    void addParsers( const QStringList& p ) { foreach ( const QString& s, p ) addParser( s ); }
    void setSkipOnError( bool b ) { mSkipOnError = b; }
    void setTryAllParsers( bool b ) { mTryAllParsers = b; }
    void setUserData( const QVariant& data ) { mUserData = data; }
    void setProject( XUPProjectItem* project ) { mProject = project; }
    void setChildCommands( const pCommand::List& commands ) { mChildCommands = commands; }
    void addChildCommand( const pCommand& command ) { mChildCommands << command; }
    void setExecutableCheckingType( int type ) { mExecutableCheckingType = type; }
    
    QString toString() const
    {
        QString s;
        s += QString( "mName: %1\n" ).arg( mName );
        s += QString( "mText: %1\n" ).arg( mText );
        s += QString( "mCommand: %1\n" ).arg( mCommand );
        s += QString( "mWorkingDirectory: %1\n" ).arg( mWorkingDirectory );
        s += QString( "mSkipOnError: %1\n" ).arg( mSkipOnError );
        s += QString( "mParsers: %1\n" ).arg( mParsers.join( " " ) );
        s += QString( "mTryAllParsers: %1\n" ).arg( mTryAllParsers );
        s += QString( "mUserData: %1\n" ).arg( mUserData.toString() );
        s += QString( "mProject: %1\n" ).arg( (quintptr)mProject.data() );
        s += QString( "mExecutableCheckingType: %1" ).arg( mExecutableCheckingType );
        return s;
    }
    
    void debug() const
    { qWarning( "%s", toString().toLocal8Bit().constData() ); }

protected:
    QString mName;                              /**< Command name/id */
    QString mText;                              /**< Command text */
    QString mCommand;                           /**< Console command line */
    QString mWorkingDirectory;                  /**< Working dirrectory of process */
    bool mSkipOnError;                          /**< Skip command, if error ocurred */
    QStringList mParsers;                       /**< List of parsers, which should be used for command. Position in the list is not ignored */
    bool mTryAllParsers;                        /**< Try to use all availible parsers after parsers from list */
    QPointer<XUPProjectItem> mProject;          /**< Project, for which command is executing */
    pCommand::List mChildCommands;              /**< This command is a fake command executing the child commands */
    QVariant mUserData;                         /**< User custom placeholder to stock customdata, currently it's internally used to store commands map */
    int mExecutableCheckingType;                /**< Warn user, if executable file does not exist and propose to select a file */
};

Q_DECLARE_METATYPE( pCommand );
Q_DECLARE_METATYPE( pCommand::List );

#endif // PCOMMAND_H
