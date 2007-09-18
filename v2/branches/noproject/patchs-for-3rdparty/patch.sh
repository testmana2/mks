#! /bin/sh
cp ../qscintilla/QScintilla-gpl-2-snapshot-20070722/Qt4/qscilexer.cpp ../qscintilla/QScintilla-gpl-2-snapshot-20070722/Qt4/qscilexer.cpp.bak
patch -N ../qscintilla/QScintilla-gpl-2-snapshot-20070722/Qt4/qscilexer.cpp qscilexer.cpp.patch