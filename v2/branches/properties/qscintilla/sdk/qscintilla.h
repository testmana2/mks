#include "qsciapis.h"
#include "qscicommand.h"
#include "qscicommandset.h"
#include "qscidocument.h"
#include "qsciglobal.h"
#include "qscilexerbash.h"
#include "qscilexerbatch.h"
#include "qscilexercmake.h"
#include "qscilexercpp.h"
#include "qscilexercsharp.h"
#include "qscilexercss.h"
#include "qscilexerd.h"
#include "qscilexerdiff.h"
#include "qscilexer.h"
#include "qscilexerhtml.h"
#include "qscilexeridl.h"
#include "qscilexerjava.h"
#include "qscilexerjavascript.h"
#include "qscilexerlua.h"
#include "qscilexermakefile.h"
#include "qscilexerperl.h"
#include "qscilexerpov.h"
#include "qscilexerproperties.h"
#include "qscilexerpython.h"
#include "qscilexerruby.h"
#include "qscilexersql.h"
#include "qscilexertex.h"
#include "qscilexervhdl.h"
#include "qscimacro.h"
#include "qsciprinter.h"
#include "qsciscintillabase.h"
#include "qsciscintilla.h"

#if QSCINTILLA_VERSION >= 0x020300
#include "qscilexertcl.h"
#include "qscilexerfortran.h"
#include "qscilexerfortran77.h"
#include "qscilexerpascal.h"
#include "qscilexerpostscript.h"
#include "qscilexerxml.h"
#include "qscilexeryaml.h"
#endif

#if QSCINTILLA_VERSION > 0x020400
#include "qscilexerverilog.h"
#include "qscilexerspice.h"
#endif
