
#include <QTextBlock>
#include <QIcon>

namespace TextEditor {
class BaseTextMark: public QObject {
	
	public:
		BaseTextMark (const QString&, int);
protected:
	void updateMarker();
public:
	QString fileName();
	int lineNumber();
};

} // namespace TextEditor