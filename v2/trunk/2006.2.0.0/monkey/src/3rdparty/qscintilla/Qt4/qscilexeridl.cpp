// This module implements the QsciLexerIDL class.
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


#include "qscilexeridl.h"

#include <qcolor.h>
#include <qfont.h>


// The ctor.
QsciLexerIDL::QsciLexerIDL(QObject *parent)
    : QsciLexerCPP(parent)
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
QColor QsciLexerIDL::color(int style) const
{
    if (style == UUID)
        return QColor(0x80,0x40,0x80);

    return QsciLexerCPP::color(style);
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
