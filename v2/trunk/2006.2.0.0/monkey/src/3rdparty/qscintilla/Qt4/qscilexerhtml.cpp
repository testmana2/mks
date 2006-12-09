// This module implements the QsciLexerHTML class.
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


#include "qscilexerhtml.h"

#include <qcolor.h>
#include <qfont.h>
#include <qsettings.h>

#include "qscilexerjavascript.h"
#include "qscilexerpython.h"


// The ctor.
QsciLexerHTML::QsciLexerHTML(QObject *parent)
    : QsciLexer(parent),
      fold_compact(true), fold_preproc(true), case_sens_tags(false)
{
}


// The dtor.
QsciLexerHTML::~QsciLexerHTML()
{
}


// Returns the language name.
const char *QsciLexerHTML::language() const
{
    return "HTML";
}


// Returns the lexer name.
const char *QsciLexerHTML::lexer() const
{
    return "hypertext";
}


// Return the string of characters that comprise a word.
const char *QsciLexerHTML::wordCharacters() const
{
        return "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789_-";
}


// Returns the foreground colour of the text for a style.
QColor QsciLexerHTML::color(int style) const
{
    switch (style)
    {
    case Default:
    case JavaScriptDefault:
    case JavaScriptWord:
    case JavaScriptSymbol:
    case ASPJavaScriptDefault:
    case ASPJavaScriptWord:
    case ASPJavaScriptSymbol:
    case VBScriptDefault:
    case ASPVBScriptDefault:
    case PHPOperator:
        return QColor(0x00,0x00,0x00);

    case Tag:
    case XMLTagEnd:
    case Script:
    case SGMLDefault:
    case SGMLCommand:
    case VBScriptKeyword:
    case VBScriptIdentifier:
    case VBScriptUnclosedString:
    case ASPVBScriptKeyword:
    case ASPVBScriptIdentifier:
    case ASPVBScriptUnclosedString:
        return QColor(0x00,0x00,0x80);

    case UnknownTag:
    case UnknownAttribute:
        return QColor(0xff,0x00,0x00);

    case Attribute:
    case VBScriptNumber:
    case ASPVBScriptNumber:
        return QColor(0x00,0x80,0x80);

    case HTMLNumber:
    case JavaScriptNumber:
    case ASPJavaScriptNumber:
    case PythonNumber:
    case PythonFunctionMethodName:
    case ASPPythonNumber:
    case ASPPythonFunctionMethodName:
        return QColor(0x00,0x7f,0x7f);

    case HTMLDoubleQuotedString:
    case HTMLSingleQuotedString:
    case JavaScriptDoubleQuotedString:
    case JavaScriptSingleQuotedString:
    case ASPJavaScriptDoubleQuotedString:
    case ASPJavaScriptSingleQuotedString:
    case PythonDoubleQuotedString:
    case PythonSingleQuotedString:
    case ASPPythonDoubleQuotedString:
    case ASPPythonSingleQuotedString:
    case PHPKeyword:
        return QColor(0x7f,0x00,0x7f);

    case OtherInTag:
    case Entity:
    case VBScriptString:
    case ASPVBScriptString:
        return QColor(0x80,0x00,0x80);

    case HTMLComment:
    case SGMLComment:
        return QColor(0x80,0x80,0x00);

    case XMLStart:
    case XMLEnd:
    case PHPStart:
    case PythonClassName:
    case ASPPythonClassName:
        return QColor(0x00,0x00,0xff);

    case HTMLValue:
        return QColor(0xff,0x00,0xff);

    case SGMLParameter:
        return QColor(0x00,0x66,0x00);

    case SGMLDoubleQuotedString:
    case SGMLError:
        return QColor(0x80,0x00,0x00);

    case SGMLSingleQuotedString:
        return QColor(0x99,0x33,0x00);

    case SGMLSpecial:
        return QColor(0x33,0x66,0xff);

    case SGMLEntity:
        return QColor(0x33,0x33,0x33);

    case SGMLBlockDefault:
        return QColor(0x00,0x00,0x66);

    case JavaScriptStart:
    case ASPJavaScriptStart:
        return QColor(0x7f,0x7f,0x00);

    case JavaScriptComment:
    case JavaScriptCommentLine:
    case ASPJavaScriptComment:
    case ASPJavaScriptCommentLine:
    case PythonComment:
    case ASPPythonComment:
    case PHPDoubleQuotedString:
        return QColor(0x00,0x7f,0x00);

    case JavaScriptCommentDoc:
        return QColor(0x3f,0x70,0x3f);

    case JavaScriptKeyword:
    case ASPJavaScriptKeyword:
    case PythonKeyword:
    case ASPPythonKeyword:
    case PHPVariable:
    case PHPDoubleQuotedVariable:
        return QColor(0x00,0x00,0x7f);

    case ASPJavaScriptCommentDoc:
        return QColor(0x7f,0x7f,0x7f);

    case VBScriptComment:
    case ASPVBScriptComment:
        return QColor(0x00,0x80,0x00);

    case PythonStart:
    case PythonDefault:
    case ASPPythonStart:
    case ASPPythonDefault:
        return QColor(0x80,0x80,0x80);

    case PythonTripleSingleQuotedString:
    case PythonTripleDoubleQuotedString:
    case ASPPythonTripleSingleQuotedString:
    case ASPPythonTripleDoubleQuotedString:
        return QColor(0x7f,0x00,0x00);

    case PHPDefault:
        return QColor(0x00,0x00,0x33);

    case PHPSingleQuotedString:
        return QColor(0x00,0x9f,0x00);

    case PHPNumber:
        return QColor(0xcc,0x99,0x00);

    case PHPComment:
        return QColor(0x99,0x99,0x99);

    case PHPCommentLine:
        return QColor(0x66,0x66,0x66);
    }

    return QsciLexer::color(style);
}


// Returns the end-of-line fill for a style.
bool QsciLexerHTML::eolFill(int style) const
{
    switch (style)
    {
    case JavaScriptDefault:
    case JavaScriptComment:
    case JavaScriptCommentDoc:
    case JavaScriptUnclosedString:
    case ASPJavaScriptDefault:
    case ASPJavaScriptComment:
    case ASPJavaScriptCommentDoc:
    case ASPJavaScriptUnclosedString:
    case VBScriptDefault:
    case VBScriptComment:
    case VBScriptNumber:
    case VBScriptKeyword:
    case VBScriptString:
    case VBScriptIdentifier:
    case VBScriptUnclosedString:
    case ASPVBScriptDefault:
    case ASPVBScriptComment:
    case ASPVBScriptNumber:
    case ASPVBScriptKeyword:
    case ASPVBScriptString:
    case ASPVBScriptIdentifier:
    case ASPVBScriptUnclosedString:
    case PythonDefault:
    case PythonComment:
    case PythonNumber:
    case PythonDoubleQuotedString:
    case PythonSingleQuotedString:
    case PythonKeyword:
    case PythonTripleSingleQuotedString:
    case PythonTripleDoubleQuotedString:
    case PythonClassName:
    case PythonFunctionMethodName:
    case PythonOperator:
    case PythonIdentifier:
    case ASPPythonDefault:
    case ASPPythonComment:
    case ASPPythonNumber:
    case ASPPythonDoubleQuotedString:
    case ASPPythonSingleQuotedString:
    case ASPPythonKeyword:
    case ASPPythonTripleSingleQuotedString:
    case ASPPythonTripleDoubleQuotedString:
    case ASPPythonClassName:
    case ASPPythonFunctionMethodName:
    case ASPPythonOperator:
    case ASPPythonIdentifier:
    case PHPDefault:
        return true;
    }

    return QsciLexer::eolFill(style);
}


// Returns the font of the text for a style.
QFont QsciLexerHTML::font(int style) const
{
    QFont f;

    switch (style)
    {
    case Default:
    case Entity:
#if defined(Q_OS_WIN)
        f = QFont("Times New Roman",11);
#else
        f = QFont("Bitstream Charter",10);
#endif
        break;

    case HTMLComment:
#if defined(Q_OS_WIN)
        f = QFont("Verdana",9);
#else
        f = QFont("Bitstream Vera Sans",8);
#endif
        break;

    case SGMLCommand:
    case PythonKeyword:
    case PythonClassName:
    case PythonFunctionMethodName:
    case PythonOperator:
    case ASPPythonKeyword:
    case ASPPythonClassName:
    case ASPPythonFunctionMethodName:
    case ASPPythonOperator:
        f = QsciLexer::font(style);
        f.setBold(true);
        break;

    case JavaScriptDefault:
    case JavaScriptCommentDoc:
    case JavaScriptKeyword:
    case JavaScriptSymbol:
    case ASPJavaScriptDefault:
    case ASPJavaScriptCommentDoc:
    case ASPJavaScriptKeyword:
    case ASPJavaScriptSymbol:
#if defined(Q_OS_WIN)
        f = QFont("Comic Sans MS",9);
#else
        f = QFont("Bitstream Vera Serif",9);
#endif
        f.setBold(true);
        break;

    case JavaScriptComment:
    case JavaScriptCommentLine:
    case JavaScriptNumber:
    case JavaScriptWord:
    case JavaScriptDoubleQuotedString:
    case JavaScriptSingleQuotedString:
    case ASPJavaScriptComment:
    case ASPJavaScriptCommentLine:
    case ASPJavaScriptNumber:
    case ASPJavaScriptWord:
    case ASPJavaScriptDoubleQuotedString:
    case ASPJavaScriptSingleQuotedString:
    case VBScriptComment:
    case ASPVBScriptComment:
    case PythonComment:
    case ASPPythonComment:
    case PHPComment:
#if defined(Q_OS_WIN)
        f = QFont("Comic Sans MS",9);
#else
        f = QFont("Bitstream Vera Serif",9);
#endif
        break;

    case VBScriptDefault:
    case VBScriptNumber:
    case VBScriptString:
    case VBScriptIdentifier:
    case VBScriptUnclosedString:
    case ASPVBScriptDefault:
    case ASPVBScriptNumber:
    case ASPVBScriptString:
    case ASPVBScriptIdentifier:
    case ASPVBScriptUnclosedString:
#if defined(Q_OS_WIN)
        f = QFont("Lucida Sans Unicode",9);
#else
        f = QFont("Bitstream Vera Serif",9);
#endif
        break;

    case VBScriptKeyword:
    case ASPVBScriptKeyword:
#if defined(Q_OS_WIN)
        f = QFont("Lucida Sans Unicode",9);
#else
        f = QFont("Bitstream Vera Serif",9);
#endif
        f.setBold(true);
        break;

    case PythonDoubleQuotedString:
    case PythonSingleQuotedString:
    case ASPPythonDoubleQuotedString:
    case ASPPythonSingleQuotedString:
#if defined(Q_OS_WIN)
        f = QFont("Courier New",10);
#else
        f = QFont("Bitstream Vera Sans Mono",9);
#endif
        break;

    case PHPKeyword:
    case PHPVariable:
    case PHPDoubleQuotedVariable:
        f = QsciLexer::font(style);
        f.setItalic(true);
        break;

    case PHPCommentLine:
#if defined(Q_OS_WIN)
        f = QFont("Comic Sans MS",9);
#else
        f = QFont("Bitstream Vera Serif",9);
#endif
        f.setItalic(true);
        break;

    default:
        f = QsciLexer::font(style);
    }

    return f;
}


// Returns the set of keywords.
const char *QsciLexerHTML::keywords(int set) const
{
    if (set == 1)
        return
            "a abbr acronym address applet area "
            "b base basefont bdo big blockquote body br button "
            "caption center cite code col colgroup "
            "dd del dfn dir div dl dt "
            "em "
            "fieldset font form frame frameset "
            "h1 h2 h3 h4 h5 h6 head hr html "
            "i iframe img input ins isindex "
            "kbd "
            "label legend li link "
            "map menu meta "
            "noframes noscript "
            "object ol optgroup option "
            "p param pre "
            "q "
            "s samp script select small span strike strong style "
            "sub sup "
            "table tbody td textarea tfoot th thead title tr tt "
            "u ul "
            "var "
            "xml xmlns "
            "abbr accept-charset accept accesskey action align "
            "alink alt archive axis "
            "background bgcolor border "
            "cellpadding cellspacing char charoff charset checked "
            "cite class classid clear codebase codetype color "
            "cols colspan compact content coords "
            "data datafld dataformatas datapagesize datasrc "
            "datetime declare defer dir disabled "
            "enctype event "
            "face for frame frameborder "
            "headers height href hreflang hspace http-equiv "
            "id ismap label lang language leftmargin link "
            "longdesc "
            "marginwidth marginheight maxlength media method "
            "multiple "
            "name nohref noresize noshade nowrap "
            "object onblur onchange onclick ondblclick onfocus "
            "onkeydown onkeypress onkeyup onload onmousedown "
            "onmousemove onmouseover onmouseout onmouseup onreset "
            "onselect onsubmit onunload "
            "profile prompt "
            "readonly rel rev rows rowspan rules "
            "scheme scope selected shape size span src standby "
            "start style summary "
            "tabindex target text title topmargin type "
            "usemap "
            "valign value valuetype version vlink vspace "
            "width "
            "text password checkbox radio submit reset file "
            "hidden image "
            "public !doctype";

    if (set == 2)
        return QsciLexerJavaScript::keywordClass;

    if (set == 3)
        return
            // Move these to QsciLexerVisualBasic when we
            // get round to implementing it.
            "and begin case call continue do each else elseif end "
            "erase error event exit false for function get gosub "
            "goto if implement in load loop lset me mid new next "
            "not nothing on or property raiseevent rem resume "
            "return rset select set stop sub then to true unload "
            "until wend while with withevents attribute alias as "
            "boolean byref byte byval const compare currency date "
            "declare dim double enum explicit friend global "
            "integer let lib long module object option optional "
            "preserve private property public redim single static "
            "string type variant";

    if (set == 4)
        return QsciLexerPython::keywordClass;

    if (set == 5)
        return
            "and argv as argc break case cfunction class continue "
            "declare default do die "
            "echo else elseif empty enddeclare endfor endforeach "
            "endif endswitch endwhile e_all e_parse e_error "
            "e_warning eval exit extends "
            "false for foreach function global "
            "http_cookie_vars http_get_vars http_post_vars "
            "http_post_files http_env_vars http_server_vars "
            "if include include_once list new not null "
            "old_function or "
            "parent php_os php_self php_version print "
            "require require_once return "
            "static switch stdclass this true var xor virtual "
            "while "
            "__file__ __line__ __sleep __wakeup";

    if (set == 6)
        return "ELEMENT DOCTYPE ATTLIST ENTITY NOTATION";

    return 0;
}


// Returns the user name of a style.
QString QsciLexerHTML::description(int style) const
{
    switch (style)
    {
    case Default:
        return tr("HTML default");

    case Tag:
        return tr("Tag");

    case UnknownTag:
        return tr("Unknown tag");

    case Attribute:
        return tr("Attribute");

    case UnknownAttribute:
        return tr("Unknown attribute");

    case HTMLNumber:
        return tr("HTML number");

    case HTMLDoubleQuotedString:
        return tr("HTML double-quoted string");

    case HTMLSingleQuotedString:
        return tr("HTML single-quoted string");

    case OtherInTag:
        return tr("Other text in a tag");

    case HTMLComment:
        return tr("HTML comment");

    case Entity:
        return tr("Entity");

    case XMLTagEnd:
        return tr("End of a tag");

    case XMLStart:
        return tr("Start of an XML fragment");

    case XMLEnd:
        return tr("End of an XML fragment");

    case Script:
        return tr("Script tag");

    case ASPAtStart:
        return tr("Start of an ASP fragment with @");

    case ASPStart:
        return tr("Start of an ASP fragment");

    case CDATA:
        return tr("CDATA");

    case PHPStart:
        return tr("Start of a PHP fragment");

    case HTMLValue:
        return tr("Unquoted HTML value");

    case ASPXCComment:
        return tr("ASP X-Code comment");

    case SGMLDefault:
        return tr("SGML default");

    case SGMLCommand:
        return tr("SGML command");

    case SGMLParameter:
        return tr("First parameter of an SGML command");

    case SGMLDoubleQuotedString:
        return tr("SGML double-quoted string");

    case SGMLSingleQuotedString:
        return tr("SGML single-quoted string");

    case SGMLError:
        return tr("SGML error");

    case SGMLSpecial:
        return tr("SGML special entity");

    case SGMLComment:
        return tr("SGML comment");

    case SGMLParameterComment:
        return tr("First parameter comment of an SGML command");

    case SGMLBlockDefault:
        return tr("SGML block default");

    case JavaScriptStart:
        return tr("Start of a JavaScript fragment");

    case JavaScriptDefault:
        return tr("JavaScript default");

    case JavaScriptComment:
        return tr("JavaScript comment");

    case JavaScriptCommentLine:
        return tr("JavaScript line comment");

    case JavaScriptCommentDoc:
        return tr("JavaDoc style JavaScript comment");

    case JavaScriptNumber:
        return tr("JavaScript number");

    case JavaScriptWord:
        return tr("JavaScript word");

    case JavaScriptKeyword:
        return tr("JavaScript keyword");

    case JavaScriptDoubleQuotedString:
        return tr("JavaScript double-quoted string");

    case JavaScriptSingleQuotedString:
        return tr("JavaScript single-quoted string");

    case JavaScriptSymbol:
        return tr("JavaScript symbol");

    case JavaScriptUnclosedString:
        return tr("JavaScript unclosed string");

    case JavaScriptRegex:
        return tr("JavaScript regular expression");

    case ASPJavaScriptStart:
        return tr("Start of an ASP JavaScript fragment");

    case ASPJavaScriptDefault:
        return tr("ASP JavaScript default");

    case ASPJavaScriptComment:
        return tr("ASP JavaScript comment");

    case ASPJavaScriptCommentLine:
        return tr("ASP JavaScript line comment");

    case ASPJavaScriptCommentDoc:
        return tr("JavaDoc style ASP JavaScript comment");

    case ASPJavaScriptNumber:
        return tr("ASP JavaScript number");

    case ASPJavaScriptWord:
        return tr("ASP JavaScript word");

    case ASPJavaScriptKeyword:
        return tr("ASP JavaScript keyword");

    case ASPJavaScriptDoubleQuotedString:
        return tr("ASP JavaScript double-quoted string");

    case ASPJavaScriptSingleQuotedString:
        return tr("ASP JavaScript single-quoted string");

    case ASPJavaScriptSymbol:
        return tr("ASP JavaScript symbol");

    case ASPJavaScriptUnclosedString:
        return tr("ASP JavaScript unclosed string");

    case ASPJavaScriptRegex:
        return tr("ASP JavaScript regular expression");

    case VBScriptStart:
        return tr("Start of a VBScript fragment");

    case VBScriptDefault:
        return tr("VBScript default");

    case VBScriptComment:
        return tr("VBScript comment");

    case VBScriptNumber:
        return tr("VBScript number");

    case VBScriptKeyword:
        return tr("VBScript keyword");

    case VBScriptString:
        return tr("VBScript string");

    case VBScriptIdentifier:
        return tr("VBScript identifier");

    case VBScriptUnclosedString:
        return tr("VBScript unclosed string");

    case ASPVBScriptStart:
        return tr("Start of an ASP VBScript fragment");

    case ASPVBScriptDefault:
        return tr("ASP VBScript default");

    case ASPVBScriptComment:
        return tr("ASP VBScript comment");

    case ASPVBScriptNumber:
        return tr("ASP VBScript number");

    case ASPVBScriptKeyword:
        return tr("ASP VBScript keyword");

    case ASPVBScriptString:
        return tr("ASP VBScript string");

    case ASPVBScriptIdentifier:
        return tr("ASP VBScript identifier");

    case ASPVBScriptUnclosedString:
        return tr("ASP VBScript unclosed string");

    case PythonStart:
        return tr("Start of a Python fragment");

    case PythonDefault:
        return tr("Python default");

    case PythonComment:
        return tr("Python comment");

    case PythonNumber:
        return tr("Python number");

    case PythonDoubleQuotedString:
        return tr("Python double-quoted string");

    case PythonSingleQuotedString:
        return tr("Python single-quoted string");

    case PythonKeyword:
        return tr("Python keyword");

    case PythonTripleDoubleQuotedString:
        return tr("Python triple double-quoted string");

    case PythonTripleSingleQuotedString:
        return tr("Python triple single-quoted string");

    case PythonClassName:
        return tr("Python class name");

    case PythonFunctionMethodName:
        return tr("Python function or method name");

    case PythonOperator:
        return tr("Python operator");

    case PythonIdentifier:
        return tr("Python identifier");

    case ASPPythonStart:
        return tr("Start of an ASP Python fragment");

    case ASPPythonDefault:
        return tr("ASP Python default");

    case ASPPythonComment:
        return tr("ASP Python comment");

    case ASPPythonNumber:
        return tr("ASP Python number");

    case ASPPythonDoubleQuotedString:
        return tr("ASP Python double-quoted string");

    case ASPPythonSingleQuotedString:
        return tr("ASP Python single-quoted string");

    case ASPPythonKeyword:
        return tr("ASP Python keyword");

    case ASPPythonTripleDoubleQuotedString:
        return tr("ASP Python triple double-quoted string");

    case ASPPythonTripleSingleQuotedString:
        return tr("ASP Python triple single-quoted string");

    case ASPPythonClassName:
        return tr("ASP Python class name");

    case ASPPythonFunctionMethodName:
        return tr("ASP Python function or method name");

    case ASPPythonOperator:
        return tr("ASP Python operator");

    case ASPPythonIdentifier:
        return tr("ASP Python identifier");

    case PHPDefault:
        return tr("PHP default");

    case PHPDoubleQuotedString:
        return tr("PHP double-quoted string");

    case PHPSingleQuotedString:
        return tr("PHP single-quoted string");

    case PHPKeyword:
        return tr("PHP keyword");

    case PHPNumber:
        return tr("PHP number");

    case PHPVariable:
        return tr("PHP variable");

    case PHPComment:
        return tr("PHP comment");

    case PHPCommentLine:
        return tr("PHP line comment");

    case PHPDoubleQuotedVariable:
        return tr("PHP double-quoted variable");

    case PHPOperator:
        return tr("PHP operator");
    }

    return QString();
}


// Returns the background colour of the text for a style.
QColor QsciLexerHTML::paper(int style) const
{
    switch (style)
    {
    case ASPAtStart:
        return QColor(0xff,0xff,0x00);

    case ASPStart:
    case CDATA:
        return QColor(0xff,0xdf,0x00);

    case PHPStart:
        return QColor(0xff,0xef,0xbf);

    case HTMLValue:
        return QColor(0xff,0xef,0xff);

    case SGMLDefault:
    case SGMLCommand:
    case SGMLParameter:
    case SGMLDoubleQuotedString:
    case SGMLSingleQuotedString:
    case SGMLSpecial:
    case SGMLEntity:
    case SGMLComment:
        return QColor(0xef,0xef,0xff);

    case SGMLError:
        return QColor(0xff,0x66,0x66);

    case SGMLBlockDefault:
        return QColor(0xcc,0xcc,0xe0);

    case JavaScriptDefault:
    case JavaScriptComment:
    case JavaScriptCommentLine:
    case JavaScriptCommentDoc:
    case JavaScriptNumber:
    case JavaScriptWord:
    case JavaScriptKeyword:
    case JavaScriptDoubleQuotedString:
    case JavaScriptSingleQuotedString:
    case JavaScriptSymbol:
        return QColor(0xf0,0xf0,0xff);

    case JavaScriptUnclosedString:
    case ASPJavaScriptUnclosedString:
        return QColor(0xbf,0xbb,0xb0);

    case JavaScriptRegex:
    case ASPJavaScriptRegex:
        return QColor(0xff,0xbb,0xb0);

    case ASPJavaScriptDefault:
    case ASPJavaScriptComment:
    case ASPJavaScriptCommentLine:
    case ASPJavaScriptCommentDoc:
    case ASPJavaScriptNumber:
    case ASPJavaScriptWord:
    case ASPJavaScriptKeyword:
    case ASPJavaScriptDoubleQuotedString:
    case ASPJavaScriptSingleQuotedString:
    case ASPJavaScriptSymbol:
        return QColor(0xdf,0xdf,0x7f);

    case VBScriptDefault:
    case VBScriptComment:
    case VBScriptNumber:
    case VBScriptKeyword:
    case VBScriptString:
    case VBScriptIdentifier:
        return QColor(0xef,0xef,0xff);

    case VBScriptUnclosedString:
    case ASPVBScriptUnclosedString:
        return QColor(0x7f,0x7f,0xff);

    case ASPVBScriptDefault:
    case ASPVBScriptComment:
    case ASPVBScriptNumber:
    case ASPVBScriptKeyword:
    case ASPVBScriptString:
    case ASPVBScriptIdentifier:
        return QColor(0xcf,0xcf,0xef);

    case PythonDefault:
    case PythonComment:
    case PythonNumber:
    case PythonDoubleQuotedString:
    case PythonSingleQuotedString:
    case PythonKeyword:
    case PythonTripleSingleQuotedString:
    case PythonTripleDoubleQuotedString:
    case PythonClassName:
    case PythonFunctionMethodName:
    case PythonOperator:
    case PythonIdentifier:
        return QColor(0xef,0xff,0xef);

    case ASPPythonDefault:
    case ASPPythonComment:
    case ASPPythonNumber:
    case ASPPythonDoubleQuotedString:
    case ASPPythonSingleQuotedString:
    case ASPPythonKeyword:
    case ASPPythonTripleSingleQuotedString:
    case ASPPythonTripleDoubleQuotedString:
    case ASPPythonClassName:
    case ASPPythonFunctionMethodName:
    case ASPPythonOperator:
    case ASPPythonIdentifier:
        return QColor(0xcf,0xef,0xcf);

    case PHPDefault:
    case PHPDoubleQuotedString:
    case PHPSingleQuotedString:
    case PHPKeyword:
    case PHPNumber:
    case PHPVariable:
    case PHPComment:
    case PHPCommentLine:
    case PHPDoubleQuotedVariable:
    case PHPOperator:
        return QColor(0xff,0xf8,0xf8);
    }

    return QsciLexer::paper(style);
}


// Refresh all properties.
void QsciLexerHTML::refreshProperties()
{
    setCompactProp();
    setPreprocProp();
    setCaseSensTagsProp();
}


// Read properties from the settings.
bool QsciLexerHTML::readProperties(QSettings &qs,const QString &prefix)
{
    int rc = true;

    fold_compact = qs.value(prefix + "foldcompact", true).toBool();
    fold_preproc = qs.value(prefix + "foldpreprocessor", false).toBool();
    case_sens_tags = qs.value(prefix + "casesensitivetags", false).toBool();

    return rc;
}


// Write properties to the settings.
bool QsciLexerHTML::writeProperties(QSettings &qs,const QString &prefix) const
{
    int rc = true;

    qs.setValue(prefix + "foldcompact", fold_compact);
    qs.setValue(prefix + "foldpreprocessor", fold_preproc);
    qs.setValue(prefix + "casesensitivetags", case_sens_tags);

    return rc;
}


// Return true if tags are case sensitive.
bool QsciLexerHTML::caseSensitiveTags() const
{
    return case_sens_tags;
}


// Set if tags are case sensitive.
void QsciLexerHTML::setCaseSensitiveTags(bool sens)
{
    case_sens_tags = sens;

    setCaseSensTagsProp();
}


// Set the "html.tags.case.sensitive" property.
void QsciLexerHTML::setCaseSensTagsProp()
{
    emit propertyChanged("html.tags.case.sensitive",(case_sens_tags ? "1" : "0"));
}


// Return true if folds are compact.
bool QsciLexerHTML::foldCompact() const
{
    return fold_compact;
}


// Set if folds are compact
void QsciLexerHTML::setFoldCompact(bool fold)
{
    fold_compact = fold;

    setCompactProp();
}


// Set the "fold.compact" property.
void QsciLexerHTML::setCompactProp()
{
    emit propertyChanged("fold.compact",(fold_compact ? "1" : "0"));
}


// Return true if preprocessor blocks can be folded.
bool QsciLexerHTML::foldPreprocessor() const
{
    return fold_preproc;
}


// Set if preprocessor blocks can be folded.
void QsciLexerHTML::setFoldPreprocessor(bool fold)
{
    fold_preproc = fold;

    setPreprocProp();
}


// Set the "fold.preprocessor" property.
void QsciLexerHTML::setPreprocProp()
{
    emit propertyChanged("fold.html.preprocessor",(fold_preproc ? "1" : "0"));
}
