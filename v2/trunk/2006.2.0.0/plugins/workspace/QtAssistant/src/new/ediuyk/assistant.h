/****************************************************************************
**
** 	Created using Edyuk IDE 0.5.1.0
**
** File : assistant.h
** Date : ven jui 21 13:43:01 2006
**
** This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
** WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
**
****************************************************************************/

#ifndef _ASSISTANT_H_
#define _ASSISTANT_H_

#include "main.h"

#include <QPair>
#include <QStringList>
#include <QStringListModel>

class Index;
class IndexKeyword;
class ContentItem;

class QListView;
class QModelIndex;
class QTreeWidget;
class QTreeWidgetItem;

class AssistantClient;
class AssistantIndexModel;

class qmdiMainWindow;

typedef QList<ContentItem> ContentList;

class Assistant : public QObject
{
	Q_OBJECT
	
	public:
		enum AssistantNodeType
		{
			Null,
			Chapter,
			Section,
			File,
			Extra
		};
		
		enum AssistantNodeRole
		{
			LinkRole = Qt::UserRole
		};
		
		Assistant(qmdiMainWindow *w, QObject *p = 0);
		~Assistant();
		
		QStringList profiles() const;
		
		QStringList search(QString str);
		QString title(const QString& doc);
		
		void searchIndex(const QString& s);
		
		void setupIndex(QListView *lv);
		void setupContent(QTreeWidget *tw);
		
		AssistantClient* createClient();
		void removeClient(AssistantClient *a);
		
		static QString docPath();
		
		inline qmdiMainWindow* mainWindow() const { return pMain; }
		inline void setMainWindow(qmdiMainWindow *m) { pMain = m; }
		
	public slots:
		void setProfile(const QString& pro);
		
		void openLinkToFile(const QString& f);
		void openLinkToItem(QTreeWidgetItem *i);
		void openLinkToIndex(const QModelIndex& i);
		
	signals:
		void linkSelectedCurTab(const QString& url);
		void linkSelectedNewTab(const QString& url);
		
	private slots:
		void contextMenu(const QPoint& p);
		
	private:
		void setupProfiles();
		QString databasePath();
		
		QMenu *pMenu;
		QAction *aOpenInCur, *aOpenInNew;
		
		//profiles data
		QHash<QString, QStringList> profilesTable;
		
		//pre-profile data
		Index *pIndex;
		QList<IndexKeyword> keywords;
		QList< QPair<QString, ContentList> > contentList;
		
		AssistantIndexModel *pIndexModel;
		
		//widgets data
		qmdiMainWindow *pMain;
		QList<AssistantClient*> clients;
};

class AssistantIndexModel : public QStringListModel
{
	Q_OBJECT
	
	public:
		AssistantIndexModel(QObject *p = 0) : QStringListModel(p) {}
		virtual ~AssistantIndexModel() {}
		
		void clear() { setStringList( QStringList() ); link.clear(); }
		
		inline QString description(int i) { return stringList().at(i); }
		inline QStringList links(int i) { return link.values( description(i) ); }
		
		void addLink(const QString& d, const QString& l) { link.insert(d, l); }
		
		QModelIndex filter(const QString& s, const QString& real);
		
		virtual Qt::ItemFlags flags(const QModelIndex &index) const
		{ return QStringListModel::flags(index) & ~Qt::ItemIsEditable; }
		
		void search(const QString& s, const QString& r)
		{ emit currentIndexChanged( filter(s, r) ); }
		
	signals:
		void currentIndexChanged(const QModelIndex& c);
		
	private:
		QMultiMap<QString, QString> link;
};

#endif // _ASSISTANT_H_
