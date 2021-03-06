// This is the SIP interface definition for QsciScintilla.
//
// Copyright (c) 2008 Riverbank Computing Limited <info@riverbankcomputing.com>
// 
// This file is part of QScintilla.
// 
// This file may be used under the terms of the GNU General Public
// License versions 2.0 or 3.0 as published by the Free Software
// Foundation and appearing in the files LICENSE.GPL2 and LICENSE.GPL3
// included in the packaging of this file.  Alternatively you may (at
// your option) use any later version of the GNU General Public
// License if such license has been publicly approved by Riverbank
// Computing Limited (or its successors, if any) and the KDE Free Qt
// Foundation. In addition, as a special exception, Riverbank gives you
// certain additional rights. These rights are described in the Riverbank
// GPL Exception version 1.1, which can be found in the file
// GPL_EXCEPTION.txt in this package.
// 
// Please review the following information to ensure GNU General
// Public Licensing requirements will be met:
// http://trolltech.com/products/qt/licenses/licensing/opensource/. If
// you are unsure which license is appropriate for your use, please
// review the following information:
// http://trolltech.com/products/qt/licenses/licensing/licensingoverview
// or contact the sales department at sales@riverbankcomputing.com.
// 
// This file is provided "AS IS" with NO WARRANTY OF ANY KIND,
// INCLUDING THE WARRANTIES OF DESIGN, MERCHANTABILITY AND FITNESS FOR
// A PARTICULAR PURPOSE. Trolltech reserves all rights not expressly
// granted herein.
// 
// This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
// WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.


class QsciScintilla : QsciScintillaBase
{
%TypeHeaderCode
#include <Qsci/qsciscintilla.h>
%End

public:
    enum {
        AiMaintain,
        AiOpening,
        AiClosing
    };

    enum AutoCompletionSource {
        AcsNone,
        AcsAll,
        AcsDocument,
        AcsAPIs,
    };

    enum BraceMatch {
        NoBraceMatch,
        StrictBraceMatch,
        SloppyBraceMatch
    };

    enum CallTipsStyle {
        CallTipsNone,
        CallTipsNoContext,
        CallTipsNoAutoCompletionContext,
        CallTipsContext
    };

    enum EdgeMode {
        EdgeNone,
        EdgeLine,
        EdgeBackground
    };

    enum EolMode {
        EolWindows,
        EolUnix,
        EolMac
    };

    enum FoldStyle {
        NoFoldStyle,
        PlainFoldStyle,
        CircledFoldStyle,
        BoxedFoldStyle,
        CircledTreeFoldStyle,
        BoxedTreeFoldStyle
    };

    enum MarkerSymbol {
        Circle,
        Rectangle,
        RightTriangle,
        SmallRectangle,
        RightArrow,
        Invisible,
        DownTriangle,
        Minus,
        Plus,
        VerticalLine,
        BottomLeftCorner,
        LeftSideSplitter,
        BoxedPlus,
        BoxedPlusConnected,
        BoxedMinus,
        BoxedMinusConnected,
        RoundedBottomLeftCorner,
        LeftSideRoundedSplitter,
        CircledPlus,
        CircledPlusConnected,
        CircledMinus,
        CircledMinusConnected,
        Background,
        ThreeDots,
        ThreeRightArrows
    };

    enum WhitespaceVisibility {
        WsInvisible,
        WsVisible,
        WsVisibleAfterIndent
    };

    enum WrapMode {
        WrapNone,
        WrapWord,
        WrapCharacter,
    };

    enum WrapVisualFlag {
        WrapFlagNone,
        WrapFlagByText,
        WrapFlagByBorder,
    };

%If (Qsci_Qt4)
    QsciScintilla(QWidget *parent /TransferThis/ = 0);
%End
%If (!Qsci_Qt4)
    QsciScintilla(QWidget *parent /TransferThis/ = 0, const char *name = 0,
            WFlags f = 0);
%End

    virtual ~QsciScintilla();

    bool autoCompletionCaseSensitivity() const;
    bool autoCompletionFillupsEnabled() const;
    bool autoCompletionReplaceWord() const;
    bool autoCompletionShowSingle() const;
    AutoCompletionSource autoCompletionSource() const;
    int autoCompletionThreshold() const;
    bool autoIndent() const;

    bool backspaceUnindents() const;
    void beginUndoAction();
    BraceMatch braceMatching() const;

    CallTipsStyle callTipsStyle() const;
    int callTipsVisible() const;
    void cancelList();
    bool caseSensitive() const;
    void clearRegisteredImages();
    QColor color() const;
    void convertEols(EolMode mode);

    QsciDocument document() const;

    void endUndoAction();
    QColor edgeColor() const;
    int edgeColumn() const;
    EdgeMode edgeMode() const;
    EolMode eolMode() const;
    bool eolVisibility() const;

    virtual bool findFirst(const QString &expr, bool re, bool cs, bool wo,
            bool wrap, bool forward = true, int line = -1, int index = -1,
            bool show = true);
    virtual bool findNext();
    int firstVisibleLine() const;
    FoldStyle folding() const;

    void getCursorPosition(int *line, int *index) const;
    void getSelection(int *lineFrom, int *indexFrom, int *lineTo,
            int *indexTo) const;

    bool hasSelectedText() const;

    int indentation(int line) const;
    bool indentationGuides() const;
    bool indentationsUseTabs() const;
    int indentationWidth() const;
    bool isCallTipActive() const;
    bool isListActive() const;
    bool isModified() const;
    bool isReadOnly() const;
    bool isRedoAvailable() const;
    bool isUndoAvailable() const;
    bool isUtf8() const;
    bool isWordCharacter(char ch) const;

    int lineAt(const QPoint &pos) const;
    int lineLength(int line) const;
    int lines() const;
    int length() const;
    QsciLexer *lexer() const;

    bool marginLineNumbers(int margin) const;
    int marginMarkerMask(int margin) const;
    bool marginSensitivity(int margin) const;
    int marginWidth(int margin) const;

    int markerDefine(MarkerSymbol sym, int mnr = -1);
    int markerDefine(char ch, int mnr = -1);
    int markerDefine(const QPixmap &pm, int mnr = -1);
    int markerAdd(int linenr, int mnr);
    unsigned markersAtLine(int linenr) const;
    void markerDelete(int linenr, int mnr = -1);
    void markerDeleteAll(int mnr = -1);
    void markerDeleteHandle(int mhandle);
    int markerLine(int mhandle) const;
    int markerFindNext(int linenr, unsigned mask) const;
    int markerFindPrevious(int linenr, unsigned mask) const;

    QColor paper() const;

    virtual void recolor(int start = 0, int end = -1);
    void registerImage(int id, const QPixmap &pm);
    virtual void replace(const QString &replaceStr);
    void resetFoldMarginColors();
    void setFoldMarginColors(const QColor &fore, const QColor &back);

    void setAutoCompletionFillupsEnabled(bool enabled);
    void setAutoCompletionFillups(const char *fillups);
    void setAutoCompletionWordSeparators(const QStringList &separators);

    void setCallTipsBackgroundColor(const QColor &col);
    void setCallTipsForegroundColor(const QColor &col);
    void setCallTipsHighlightColor(const QColor &col);
    void setCallTipsStyle(CallTipsStyle style);
    void setCallTipsVisible(int nr);

    void setDocument(const QsciDocument &document);

    void setEdgeColor(const QColor &col);
    void setEdgeColumn(int colnr);
    void setEdgeMode(EdgeMode mode);

    void setFont(const QFont &f);

    void setMarkerBackgroundColor(const QColor &col, int mnr = -1);
    void setMarkerForegroundColor(const QColor &col, int mnr = -1);

    void setMatchedBraceBackgroundColor(const QColor &col);
    void setMatchedBraceForegroundColor(const QColor &col);
    void setUnmatchedBraceBackgroundColor(const QColor &col);
    void setUnmatchedBraceForegroundColor(const QColor &col);

    void setWrapVisualFlags(WrapVisualFlag eflag,
            WrapVisualFlag sflag = QsciScintilla::WrapFlagNone,
            int sindent = 0);

    QString selectedText() const;
    bool selectionToEol() const;
    void setSelectionToEol(bool filled);
    void showUserList(int id, const QStringList &list);
    QsciCommandSet *standardCommands() const;
    bool tabIndents() const;
    int tabWidth() const;
    QString text() const;
    QString text(int line) const;
    int textHeight(int linenr) const;
    WhitespaceVisibility whitespaceVisibility() const;
    const char *wordCharacters() const;
    WrapMode wrapMode() const;

public slots:
    virtual void append(const QString &text);
    virtual void autoCompleteFromAll();
    virtual void autoCompleteFromAPIs();
    virtual void autoCompleteFromDocument();
    virtual void callTip();
    virtual void clear();
    virtual void copy();
    virtual void cut();

    virtual void ensureCursorVisible();
    virtual void ensureLineVisible(int line);
    virtual void foldAll(bool children = false);
    virtual void foldLine(int line);
    virtual void indent(int line);
    virtual void insert(const QString &text);
    virtual void insertAt(const QString &text, int line, int index);
    virtual void moveToMatchingBrace();
    virtual void paste();
    virtual void redo();
    virtual void removeSelectedText();
    virtual void resetSelectionBackgroundColor();
    virtual void resetSelectionForegroundColor();
    virtual void selectAll(bool select = true);
    virtual void selectToMatchingBrace();

    virtual void setAutoCompletionCaseSensitivity(bool cs);
    virtual void setAutoCompletionReplaceWord(bool replace);
    virtual void setAutoCompletionShowSingle(bool single);
    virtual void setAutoCompletionSource(AutoCompletionSource source);
    virtual void setAutoCompletionThreshold(int thresh);
    virtual void setAutoIndent(bool autoindent);
    virtual void setBraceMatching(BraceMatch bm);
    virtual void setBackspaceUnindents(bool unindent);
    virtual void setCaretForegroundColor(const QColor &col);
    virtual void setCaretLineBackgroundColor(const QColor &col);
    virtual void setCaretLineVisible(bool enable);
    virtual void setCaretWidth(int width);
    virtual void setColor(const QColor &col);
    virtual void setCursorPosition(int line, int index);
    virtual void setEolMode(EolMode mode);
    virtual void setEolVisibility(bool visible);
    virtual void setFolding(FoldStyle fold);

    virtual void setIndentation(int line, int indentation);
    virtual void setIndentationGuides(bool enable);
    virtual void setIndentationGuidesBackgroundColor(const QColor &col);
    virtual void setIndentationGuidesForegroundColor(const QColor &col);
    virtual void setIndentationsUseTabs(bool tabs);
    virtual void setIndentationWidth(int width);

    virtual void setLexer(QsciLexer *lexer = 0);

    virtual void setMarginsBackgroundColor(const QColor &col);
    virtual void setMarginsFont(const QFont &f);
    virtual void setMarginsForegroundColor(const QColor &col);

    virtual void setMarginLineNumbers(int margin, bool lnrs);
    virtual void setMarginMarkerMask(int margin, int mask);
    virtual void setMarginSensitivity(int margin, bool sens);
    virtual void setMarginWidth(int margin, int width);
    virtual void setMarginWidth(int margin, const QString &s);

    virtual void setModified(bool m);
    virtual void setPaper(const QColor &c);
    virtual void setReadOnly(bool ro);
    virtual void setSelection(int lineFrom, int indexFrom, int lineTo,
            int indexTo);
    virtual void setSelectionBackgroundColor(const QColor &col);
    virtual void setSelectionForegroundColor(const QColor &col);
    virtual void setTabIndents(bool indent);
    virtual void setTabWidth(int width);
    virtual void setText(const QString &text);
    virtual void setUtf8(bool cp);
    virtual void setWhitespaceVisibility(WhitespaceVisibility mode);
    virtual void setWrapMode(WrapMode mode);

    virtual void undo();
    virtual void unindent(int line);

    virtual void zoomIn(int range);
    virtual void zoomIn();
    virtual void zoomOut(int range);
    virtual void zoomOut();
    virtual void zoomTo(int size);

signals:
    void cursorPositionChanged(int line, int pos);
    void copyAvailable(bool yes);

    void linesChanged();

%If (Qsci_Qt4)
    void marginClicked(int margin, int line, Qt::KeyboardModifiers state);
%End
%If (!Qsci_Qt4)
    void marginClicked(int margin, int line, Qt::ButtonState state);
%End

    void modificationAttempted();
    void modificationChanged(bool m);
    void selectionChanged();
    void textChanged();
    void userListActivated(int id, const QString &string);

private:
    QsciScintilla(const QsciScintilla &);
};
