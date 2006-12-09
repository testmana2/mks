// This module implements the "official" low-level API.
//
// Copyright (c) 2006
// 	Riverbank Computing Limited <info@riverbankcomputing.co.uk>
// 
// This file is part of QScintilla.
// 
// This copy of QScintilla is free software; you can redistribute it and/or
// modify it under the terms of the GNU General Public License as published by
// the Free Software Foundation; either version 2, or (at your option) any
// later version.
// 
// QScintilla is supplied in the hope that it will be useful, but WITHOUT ANY
// WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
// FOR A PARTICULAR PURPOSE.  See the GNU General Public License for more
// details.
// 
// You should have received a copy of the GNU General Public License along with
// QScintilla; see the file LICENSE.  If not, write to the Free Software
// Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.


#include "qsciscintillabase.h"

#include <qapplication.h>
#include <qclipboard.h>
#include <qcolor.h>
#include <qscrollbar.h>

#include <QContextMenuEvent>
#include <QDragEnterEvent>
#include <QDragMoveEvent>
#include <QDropEvent>
#include <QDragLeaveEvent>
#include <QFocusEvent>
#include <QKeyEvent>
#include <QList>
#include <QMouseEvent>
#include <QPaintEvent>

#include "ScintillaQt.h"


// The #defines in Scintilla.h and the enums in qsciscintillabase.h conflict
// (because we want to use the same names) so we have to undefine those we use
// in this file.
#undef  SCI_SETCARETPERIOD
#undef  SCK_DOWN
#undef  SCK_UP
#undef  SCK_LEFT
#undef  SCK_RIGHT
#undef  SCK_HOME
#undef  SCK_END
#undef  SCK_PRIOR
#undef  SCK_NEXT
#undef  SCK_DELETE
#undef  SCK_INSERT
#undef  SCK_ESCAPE
#undef  SCK_BACK
#undef  SCK_TAB
#undef  SCK_RETURN
#undef  SCK_ADD
#undef  SCK_SUBTRACT
#undef  SCK_DIVIDE


// Remember if we have linked the lexers.
static bool lexersLinked = false;

// The list of instances.
static QList<QsciScintillaBase *> poolList;


// The ctor.
QsciScintillaBase::QsciScintillaBase(QWidget *parent)
    : QAbstractScrollArea(parent)
{
    connect(verticalScrollBar(), SIGNAL(valueChanged(int)),
            SLOT(handleVSb(int)));

    connect(horizontalScrollBar(), SIGNAL(valueChanged(int)),
            SLOT(handleHSb(int)));

    setAcceptDrops(true);
    setFocusPolicy(Qt::WheelFocus);
    setAttribute(Qt::WA_KeyCompression);

    viewport()->setBackgroundRole(QPalette::Base);
    viewport()->setMouseTracking(true);

    triple_click.setSingleShot(true);

    sci = new ScintillaQt(this);

    SendScintilla(SCI_SETCARETPERIOD, QApplication::cursorFlashTime() / 2);

    // Make sure the lexers are linked in.
    if (!lexersLinked)
    {
        Scintilla_LinkLexers();
        lexersLinked = true;
    }

    QClipboard *cb = QApplication::clipboard();

    if (cb->supportsSelection())
        connect(cb, SIGNAL(selectionChanged()), SLOT(handleSelection()));

    // Add it to the pool.
    poolList.append(this);
}


// The dtor.
QsciScintillaBase::~QsciScintillaBase()
{
    // Remove it from the pool.
    poolList.removeAt(poolList.indexOf(this));

    delete sci;
}


// Return an instance from the pool.
QsciScintillaBase *QsciScintillaBase::pool()
{
    return poolList.first();
}


// Send a message to the real Scintilla widget using the low level Scintilla
// API.
long QsciScintillaBase::SendScintilla(unsigned int msg, unsigned long wParam,
        long lParam)
{
    return sci->WndProc(msg, wParam, lParam);
}


// Send a message to the real Scintilla widget that needs a TextRange
// structure.
long QsciScintillaBase::SendScintilla(unsigned int msg, long cpMin, long cpMax,
        char *lpstrText)
{
    TextRange tr;

    tr.chrg.cpMin = cpMin;
    tr.chrg.cpMax = cpMax;
    tr.lpstrText = lpstrText;

    return sci->WndProc(msg, 0, reinterpret_cast<long>(&tr));
}


// Send a message to the real Scintilla widget that needs a RangeToFormat
// structure.
long QsciScintillaBase::SendScintilla(unsigned int msg, unsigned long wParam,
        QPainter *hdc, const QRect &rc, long cpMin, long cpMax)
{
    RangeToFormat rf;

    rf.hdc = rf.hdcTarget = reinterpret_cast<SurfaceID>(hdc);

    rf.rc.left = rc.left();
    rf.rc.top = rc.top();
    rf.rc.right = rc.right() + 1;
    rf.rc.bottom = rc.bottom() + 1;

    rf.chrg.cpMin = cpMin;
    rf.chrg.cpMax = cpMax;

    return sci->WndProc(msg, wParam, reinterpret_cast<long>(&rf));
}


// Send a message to the real Scintilla widget that needs a colour.
long QsciScintillaBase::SendScintilla(unsigned int msg, unsigned long wParam,
        const QColor &col)
{
    long lParam = (col.blue() << 16) | (col.green() << 8) | col.red();

    return sci->WndProc(msg, wParam, lParam);
}


// Send a message to the real Scintilla widget that needs a colour.
long QsciScintillaBase::SendScintilla(unsigned int msg, const QColor &col)
{
    unsigned long wParam = (col.blue() << 16) | (col.green() << 8) | col.red();

    return sci->WndProc(msg, wParam, 0);
}


// Handle the timer on behalf of the ScintillaQt instance.
void QsciScintillaBase::handleTimer()
{
    sci->Tick();
}


// Re-implemented to handle the context menu.
void QsciScintillaBase::contextMenuEvent(QContextMenuEvent *e)
{
    sci->ContextMenu(Point(e->globalX(), e->globalY()));
}


// Re-implemented to tell the widget it has the focus.
void QsciScintillaBase::focusInEvent(QFocusEvent *)
{
    sci->SetFocusState(true);
}


// Re-implemented to tell the widget it has lost the focus.
void QsciScintillaBase::focusOutEvent(QFocusEvent *)
{
    sci->SetFocusState(false);
}


// Re-implemented to make sure tabs are passed to the editor.
bool QsciScintillaBase::focusNextPrevChild(bool next)
{
    if (!sci->pdoc->IsReadOnly())
        return false;

    return QAbstractScrollArea::focusNextPrevChild(next);
}


// Handle the selection changing.
void QsciScintillaBase::handleSelection()
{
    if (!QApplication::clipboard()->ownsSelection())
        sci->UnclaimSelection();
}


// Handle key presses.
void QsciScintillaBase::keyPressEvent(QKeyEvent *e)
{
    unsigned key;
    QByteArray utf8;

    bool shift = e->modifiers() & Qt::ShiftModifier;
    bool ctrl = e->modifiers() & Qt::ControlModifier;
    bool alt = e->modifiers() & Qt::AltModifier;

    switch (e->key())
    {
    case Qt::Key_Down:
        key = SCK_DOWN;
        break;

    case Qt::Key_Up:
        key = SCK_UP;
        break;

    case Qt::Key_Left:
        key = SCK_LEFT;
        break;

    case Qt::Key_Right:
        key = SCK_RIGHT;
        break;

    case Qt::Key_Home:
        key = SCK_HOME;
        break;

    case Qt::Key_End:
        key = SCK_END;
        break;

    case Qt::Key_PageUp:
        key = SCK_PRIOR;
        break;

    case Qt::Key_PageDown:
        key = SCK_NEXT;
        break;

    case Qt::Key_Delete:
        key = SCK_DELETE;
        break;

    case Qt::Key_Insert:
        key = SCK_INSERT;
        break;

    case Qt::Key_Escape:
        key = SCK_ESCAPE;
        break;

    case Qt::Key_Backspace:
        key = SCK_BACK;
        break;

    case Qt::Key_Tab:
        key = SCK_TAB;
        break;

    case Qt::Key_Return:
    case Qt::Key_Enter:
        key = SCK_RETURN;
        break;

    default:
        // See if the input was a single ASCII key.  If so it will be passed to
        // KeyDown to allow it to be filtered.  Correct the modifiers and key
        // for ASCII letters as Qt uses the ASCII code of uppercase letters for
        // Key_A etc.
        utf8 = e->text().toUtf8();

        if (utf8.length() > 1)
            key = 0;
        else if ((key = utf8[0]) >= 0x80)
            key = 0;
        else if (key >= 0x01 && key <= 0x1a)
            key += 0x40;
        else if (key >= 'A' && key <= 'Z')
            shift = true;
        else if (key >= 'a' && key <= 'z')
        {
            key -= 0x20;
            shift = false;
        }
    }

    if (key)
    {
        bool consumed = false;

        sci->KeyDown(key, shift, ctrl, alt, &consumed);

        if (consumed)
        {
            e->accept();
            return;
        }
    }

    // Add the text if it has a compatible size depending on what Unicode mode
    // we are in.
    if (utf8.length() > 0 && (sci->IsUnicodeMode() || utf8.length() == 1))
    {
        sci->AddCharUTF(utf8.data(), utf8.length());
        e->accept();
    }
    else
        QAbstractScrollArea::keyPressEvent(e);
}


// Handle a mouse button double click.
void QsciScintillaBase::mouseDoubleClickEvent(QMouseEvent *e)
{
    if (e->button() != Qt::LeftButton)
    {
        e->ignore();
        return;
    }

    setFocus();

    // Make sure Scintilla will interpret this as a double-click.
    unsigned clickTime = sci->lastClickTime + Platform::DoubleClickTime() - 1;

    bool shift = e->modifiers() & Qt::ShiftModifier;
    bool ctrl = e->modifiers() & Qt::ControlModifier;
    bool alt = e->modifiers() & Qt::AltModifier;

    sci->ButtonDown(Point(e->x(), e->y()), clickTime, shift, ctrl, alt);

    // Remember the current position and time in case it turns into a triple
    // click.
    triple_click_at = e->globalPos();
    triple_click.start(QApplication::doubleClickInterval());
}


// Handle a mouse move.
void QsciScintillaBase::mouseMoveEvent(QMouseEvent *e)
{
    sci->ButtonMove(Point(e->x(), e->y()));
}


// Handle a mouse button press.
void QsciScintillaBase::mousePressEvent(QMouseEvent *e)
{
    setFocus();

    Point pt(e->x(), e->y());

    if (e->button() == Qt::LeftButton)
    {
        unsigned clickTime;

        // It is a triple click if the timer is running and the mouse hasn't
        // moved too much.
        if (triple_click.isActive() && (e->globalPos() - triple_click_at).manhattanLength() < QApplication::startDragDistance())
            clickTime = sci->lastClickTime + Platform::DoubleClickTime() - 1;
        else
            clickTime = sci->lastClickTime + Platform::DoubleClickTime() + 1;

        triple_click.stop();

        bool shift = e->modifiers() & Qt::ShiftModifier;
        bool ctrl = e->modifiers() & Qt::ControlModifier;
        bool alt = e->modifiers() & Qt::AltModifier;

        sci->ButtonDown(pt, clickTime, shift, ctrl, alt);
    }
    else if (e->button() == Qt::MidButton)
    {
        QClipboard *cb = QApplication::clipboard();

        if (cb->supportsSelection())
        {
            int pos = sci->PositionFromLocation(pt);

            sci->SetSelection(pos, pos);
            sci->pasteFromClipboard(QClipboard::Selection);
        }
    }
}


// Handle a mouse button releases.
void QsciScintillaBase::mouseReleaseEvent(QMouseEvent *e)
{
    if (sci->HaveMouseCapture() && e->button() == Qt::LeftButton)
    {
        bool ctrl = e->modifiers() & Qt::ControlModifier;

        sci->ButtonUp(Point(e->x(), e->y()), 0, ctrl);
    }
}


// Handle paint events.
void QsciScintillaBase::paintEvent(QPaintEvent *e)
{
    sci->paintEvent(e);
}


// Handle resize events.
void QsciScintillaBase::resizeEvent(QResizeEvent *)
{
    sci->ChangeSize();
}


// Re-implemented to suppress the default behaviour as Scintilla works at a
// more fundamental level.
void QsciScintillaBase::scrollContentsBy(int, int)
{
}


// Handle the vertical scrollbar.
void QsciScintillaBase::handleVSb(int value)
{
    sci->ScrollTo(value);
}


// Handle the horizontal scrollbar.
void QsciScintillaBase::handleHSb(int value)
{
    sci->HorizontalScrollTo(value);
}


// Handle drag enters.
void QsciScintillaBase::dragEnterEvent(QDragEnterEvent *e)
{
    QsciScintillaBase::dragMoveEvent(e);
}


// Handle drag leaves.
void QsciScintillaBase::dragLeaveEvent(QDragLeaveEvent *)
{
    sci->SetDragPosition(-1);
}


// Handle drag moves.
void QsciScintillaBase::dragMoveEvent(QDragMoveEvent *e)
{
    sci->SetDragPosition(sci->PositionFromLocation(Point(e->pos().x(),
                    e->pos().y())));

    if (sci->pdoc->IsReadOnly() || !e->mimeData()->hasText())
    {
        e->ignore();
        return;
    }

    e->acceptProposedAction();
}


// Handle drops.
void QsciScintillaBase::dropEvent(QDropEvent *e)
{
    bool moving;
    const char *s;

    if (sci->pdoc->IsReadOnly() || !e->mimeData()->hasText())
    {
        e->ignore();
        return;
    }

    e->acceptProposedAction();

    moving = (e->dropAction() == Qt::MoveAction &&
            (e->source() == this || e->source() == viewport()));

    QByteArray ba;

    if (sci->IsUnicodeMode())
        ba = e->mimeData()->text().toUtf8();
    else
        ba = e->mimeData()->text().toLatin1();

    s = ba.data();

    sci->DropAt(sci->posDrop, s, moving, false);
    sci->Redraw();
}
