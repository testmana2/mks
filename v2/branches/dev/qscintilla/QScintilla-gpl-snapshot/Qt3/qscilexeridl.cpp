// This module implements the QsciLexerIDL class.
//
// Copyright (c) 2011 Riverbank Computing Limited <info@riverbankcomputing.com>
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
// This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
// WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.


#include "Qsci/qscilexeridl.h"

#include <qcolor.h>
#include <qfont.h>


// The ctor.
QsciLexerIDL::QsciLexerIDL(QObject *parent, const char *name)
    : QsciLexerCPP(parent, name)
{
}


// The dtor.
QsciLexerIDL::~QsciLexerIDL()
{
}


// Returns the language name.
const char *QsciLexerIDL::language() const
{
    return "IDL";
}


// Returns the foreground colour of the text for a style.
QColor QsciLexerIDL::defaultColor(int style) const
{
    if (style == UUID)
        return QColor(0x80,0x40,0x80);

    return QsciLexerCPP::defaultColor(style);
}


// Returns the set of keywords.
const char *QsciLexerIDL::keywords(int set) const
{
    if (set != 1)
        return 0;

    return "aggregatable allocate appobject arrays async async_uuid "
           "auto_handle bindable boolean broadcast byte byte_count "
           "call_as callback char coclass code comm_status const "
           "context_handle context_handle_noserialize "
           "context_handle_serialize control cpp_quote custom decode "
           "default defaultbind defaultcollelem defaultvalue "
           "defaultvtable dispinterface displaybind dllname double dual "
           "enable_allocate encode endpoint entry enum error_status_t "
           "explicit_handle fault_status first_is float handle_t heap "
           "helpcontext helpfile helpstring helpstringcontext "
           "helpstringdll hidden hyper id idempotent ignore iid_as iid_is "
           "immediatebind implicit_handle import importlib in include "
           "in_line int __int64 __int3264 interface last_is lcid "
           "length_is library licensed local long max_is maybe message "
           "methods midl_pragma midl_user_allocate midl_user_free min_is "
           "module ms_union ncacn_at_dsp ncacn_dnet_nsp ncacn_http "
           "ncacn_ip_tcp ncacn_nb_ipx ncacn_nb_nb ncacn_nb_tcp ncacn_np "
           "ncacn_spx ncacn_vns_spp ncadg_ip_udp ncadg_ipx ncadg_mq "
           "ncalrpc nocode nonbrowsable noncreatable nonextensible notify "
           "object odl oleautomation optimize optional out out_of_line "
           "pipe pointer_default pragma properties propget propput "
           "propputref ptr public range readonly ref represent_as "
           "requestedit restricted retval shape short signed size_is "
           "small source strict_context_handle string struct switch "
           "switch_is switch_type transmit_as typedef uidefault union "
           "unique unsigned user_marshal usesgetlasterror uuid v1_enum "
           "vararg version void wchar_t wire_marshal";
}


// Returns the user name of a style.
QString QsciLexerIDL::description(int style) const
{
    if (style == UUID)
        return tr("UUID");

    return QsciLexerCPP::description(style);
}
