/****************************************************************************
    Copyright (C) 2005 - 2008  Filipe AZEVEDO & The Monkey Studio Team

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
    \file gdbPatternFile.h
    \date 14/08/08
    \author Xiantia
    \version 1.3.2
    \brief Load and store all patterns from gdbparing.txt file in list
*/


#ifndef GDBPATTERNFILE_H
#define GDBPATTERNFILE_H

#include <fresh.h>

#include <QObject>
#include <QRegExp>

/*!
    \brief Struct for strore all informations of pattern.
    \details A pattern is compose by one Id, one 
    comment, one key and one interger indicate if this pattern is use or not.
*/
/*typedef, fixe for 64 bits*/
struct GdbPattern
{
    /*!
        \details Comment of pattern
    */
    QString comment;
    /*!
        \details String from Gdb in QRegExp format
    */
    QRegExp key;
    /*!
        \details If of string. 0 = prompt event, 10000 to 19999 is generique information, 20000 and more is an error found
    */
    int id;
    /*!
        \details pattern is enable. enable = 0 the pattern is not use, enable = 1 the pattern is not use but the message is show, enable = 2 the parttern is use 
    */
    int enable;
};


/*!
    \brief Load file and store all informations in list, GdbPattern struct.
    GdbParser use one list for parse all datas from GdbProcess. It seek in this file the string from Gdb.
    This file is loaded in GdbPattern. this pattern store an Id, one key, one comment and if this pattern is use or not.
    \sa GdbPaser, GdbPattern
*/
class GdbPatternFile : public QObject, public QSingleton<GdbPatternFile>
{

    Q_OBJECT
    friend class QSingleton<GdbPatternFile>;

public:

    GdbPatternFile(QObject *parent = 0);
    ~GdbPatternFile();
    bool load(const QString & );
    GdbPattern find(const QString &);

    /*!
        \details Get the list
    */
    QList<GdbPattern> * get(){ return &GdbPatternList;}

    int getId(const GdbPattern & p);
    QString getPattern(const GdbPattern & p );
    QString getComment( const GdbPattern & p);

    /*!
        \details Get path and name of file
    */
    QString getFileName() { return mFullName; }

private :

    /*! 
        \details List of all patterns
        This list contains all datas from parse file. It stored in GdbPattern struct.
    */
    QList<GdbPattern> GdbPatternList;
    /*!
        \details Current parse file to use
    */
    QString mFullName;
};
#endif
