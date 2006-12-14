/****************************************************************************
**
** Copyright (C) 1992-2006 Trolltech ASA. All rights reserved.
**
** This file is part of the Qt Assistant of the Qt Toolkit.
**
** This file may be used under the terms of the GNU General Public
** License version 2.0 as published by the Free Software Foundation
** and appearing in the file LICENSE.GPL included in the packaging of
** this file.  Please review the following information to ensure GNU
** General Public Licensing requirements will be met:
** http://www.trolltech.com/products/qt/opensource.html
**
** If you are unsure which license is appropriate for your use, please
** review the following information:
** http://www.trolltech.com/products/qt/licensing.html or contact the
** sales department at sales@trolltech.com.
**
** This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
** WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
**
****************************************************************************/

#include "profile.h"
#include <QtXml>
#include <QTextCodec>
#include <QFileInfo>
#include <QRegExp>
#include <QDir>
#include <QList>
#include <QLibraryInfo>

#define QT_TITLE         QLatin1String("Qt Reference Documentation")
#define DESIGNER_TITLE   QLatin1String("Qt Designer Manual")
#define ASSISTANT_TITLE  QLatin1String("Qt Assistant Manual")
#define LINGUIST_TITLE   QLatin1String("Qt Linguist Manual")
#define QMAKE_TITLE      QLatin1String("qmake Manual")

Profile *Profile::createDefaultProfile(const QString &docPath)
{
    QString path = QLibraryInfo::location(QLibraryInfo::DocumentationPath);
    if (!docPath.isEmpty())
        path = docPath;
    path = QDir::cleanPath(path) + QLatin1String("/html/");

    Profile *profile = new Profile;
    profile->valid = true;
    profile->type = DefaultProfile;
    profile->props[QLatin1String("name")] = QLatin1String("default");
    profile->props[QLatin1String("applicationicon")] = QLatin1String("assistant.png");
    profile->props[QLatin1String("aboutmenutext")] = QLatin1String("About Qt");
    profile->props[QLatin1String("abouturl")] = QLatin1String("about_qt");
    profile->props[QLatin1String("basepath")] = path;
    profile->props[QLatin1String("startpage")] = path + QLatin1String("index.html");

    profile->addDCFTitle( path + QLatin1String("qt.dcf"), QT_TITLE );
    profile->addDCFTitle( path + QLatin1String("designer.dcf"), DESIGNER_TITLE );
    profile->addDCFTitle( path + QLatin1String("assistant.dcf"), ASSISTANT_TITLE );
    profile->addDCFTitle( path + QLatin1String("linguist.dcf"), LINGUIST_TITLE );
    profile->addDCFTitle( path + QLatin1String("qmake.dcf"), QMAKE_TITLE );

    profile->addDCFIcon( QT_TITLE, QLatin1String("qt.png") );
    profile->addDCFIcon( DESIGNER_TITLE, QLatin1String("designer.png") );
    profile->addDCFIcon( ASSISTANT_TITLE, QLatin1String("assistant.png") );
    profile->addDCFIcon( LINGUIST_TITLE, QLatin1String("linguist.png") );

    profile->addDCFIndexPage( QT_TITLE, path + QLatin1String("index.html") );
    profile->addDCFIndexPage( DESIGNER_TITLE, path + QLatin1String("designer-manual.html") );
    profile->addDCFIndexPage( ASSISTANT_TITLE, path + QLatin1String("assistant-manual.html") );
    profile->addDCFIndexPage( LINGUIST_TITLE, path + QLatin1String("linguist-manual.html") );
    profile->addDCFIndexPage( QMAKE_TITLE, path + QLatin1String("qmake-manual.html") );

    profile->addDCFImageDir( QT_TITLE, QLatin1String("../../gif/") );
    profile->addDCFImageDir( DESIGNER_TITLE, QLatin1String("../../gif/") );
    profile->addDCFImageDir( ASSISTANT_TITLE, QLatin1String("../../gif/") );
    profile->addDCFImageDir( LINGUIST_TITLE, QLatin1String("../../gif/") );
    profile->addDCFImageDir( QMAKE_TITLE, QLatin1String("../../gif/") );

    return profile;
}

Profile::Profile()
    : valid( true ), dparser( 0 )
{
    type = DefaultProfile;
}

bool Profile::isValid() const
{
    return valid;
}

void Profile::addDCFTitle(const QString &dcf, const QString &title)
{
    QString absdcf = QFileInfo(dcf).absoluteFilePath();
    dcfTitles[title] = absdcf;
    if (!docs.contains(absdcf))
        docs << absdcf;
}

void Profile::addDCF(const QString &docfile)
{
    if( !docs.contains( docfile ) == 0 )
        docs << docfile;
}

void Profile::addDCFIcon(const QString docfile, const QString &icon)
{
    icons[docfile] = icon;
}

void Profile::addDCFIndexPage(const QString title, const QString &indexPage)
{
    indexPages[title] = indexPage;
}

void Profile::addDCFImageDir(const QString docfile, const QString &imgDir)
{
    imageDirs[docfile] = imgDir;
}

void Profile::addProperty(const QString &name, const QString &value)
{
    props[name] = value;
}

bool Profile::hasDocFile(const QString &name)
{
    return docs.contains( name );
}

void Profile::removeDocFileEntry(const QString &docfile)
{
    docs.removeAll(docfile);
    QStringList titles;

    for( QMap<QString,QString>::Iterator it = dcfTitles.begin();
        it != dcfTitles.end(); ++it ) {
        if( (*it) == docfile ) {
            indexPages.remove( *it );
            icons.remove( *it );
            imageDirs.remove( *it );
            titles << it.key();
        }
    }

    for( QStringList::ConstIterator title = titles.constBegin();
        title != titles.constEnd(); ++title )
        dcfTitles.remove( *title );

#ifdef ASSISTANT_DEBUG
    qDebug() << "docs:\n  - " << docs.join("\n  - ");
    qDebug() << "titles:\n  - " << titles.join("\n  - ");
    qDebug() << "keys:\n  - " << ((QStringList*)&(dcfTitles.keys()))->join("\n  - ");
    qDebug() << "values:\n  - " << ((QStringList*)&(dcfTitles.values()))->join("\n  - ");
#endif
}

QString Profile::storableFilePath(const QString &fileName)
{
    QString path = QLibraryInfo::location(QLibraryInfo::DocumentationPath).replace("\\", "/");
    QString fName = fileName;
    if (fName.startsWith(path))
        fName.replace(0, path.length(), "$DOCPATH$");
    return fName;
}

QString Profile::loadableFilePath(const QString &fileName)
{
    QString path = QLibraryInfo::location(QLibraryInfo::DocumentationPath).replace("\\", "/");
    QString fName = fileName;
    if (fName.startsWith("$DOCPATH$"))
        fName.replace(0, 9, path);
    return fName;
}
