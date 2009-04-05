
#include <QTextBlock>
#include <QIcon>

namespace TextEditor {
class BaseTextMark: public QObject {
	
	public:
		BaseTextMark (const QString&, int);
protected:
	void updateMarker() {};
public:
	QString fileName()
	{
		return QString::null;
	};
	int lineNumber()
	{
		return 0;
	};
};

} // namespace TextEditor