# coding=utf8

import parsing
from string import Template

unix_file = "[\w\d\./]+"
number = "\d+"
compiler = "(gcc|g\+\+|mingw32-gcc|mingw32-g\+\+)"
source_file = unix_file + "(\.c|\.cpp)"

# Error
regEx = Template("^($unix_file):($number): error: (.+)")
error =parsing.Pattern(regEx.substitute(unix_file = unix_file, number = number),
								   type = 'error',
								   file = "%1",
								   line = "%2")

error.setComment('Error (usualy syntax)')

error.test("src/main.cpp:20: error: expected initializer before 's'",
				type = 'error',
				file = 'src/main.cpp',
				line = '20',
				text = "src/main.cpp:20: error: expected initializer before 's'",
				hint = "src/main.cpp:20: error: expected initializer before 's'")

# Warning
regEx = Template("^($unix_file):($number): warning: (.+)")
warning =parsing.Pattern(regEx.substitute(unix_file = unix_file, number = number),
								   type = 'warning',
								   file = "%1",
								   line = "%2")

warning.setComment('Warning')

warning.test("src/MSVCMake.cpp:122: warning: unused parameter ‘s’",
				type = 'warning',
				file = 'src/MSVCMake.cpp',
				line = '122',
				text = "src/MSVCMake.cpp:122: warning: unused parameter ‘s’",
				hint = "src/MSVCMake.cpp:122: warning: unused parameter ‘s’")

# Building file
regEx = Template("^($compiler)[^\n]+ ($source_file)")
compiling =parsing.Pattern(regEx.substitute(compiler=compiler, source_file=source_file),
										   type = 'compiling')

compiling.setComment('Compiling')

text = \
'g++ -c -pipe -g -D_REENTRANT -Wall -W -fPIC -DQT_SHARED -DPLUGIN_NAME="\"MSVCMake\"" -DQT_PLUGIN -DQT_GUI_LIB -DQT_CORE_LIB' + \
' -I/usr/share/qt4/mkspecs/linux-g++ -I. -I/usr/include/qt4/QtCore -I/usr/include/qt4/QtCore -I/usr/include/qt4/QtGui' + \
' -I/usr/include/qt4/QtGui -I/usr/include/qt4 -I../../../qscintilla/QScintilla-gpl-2-snapshot-20071024/Qt4' + \
' -I../../../qscintilla/QScintilla-gpl-2-snapshot-20071024/Qt4/Qsci -I../../../qscintilla/QScintilla-gpl-2-snapshot-20071024/include' + \
' -I../../../qscintilla/QScintilla-gpl-2-snapshot-20071024/src -I../../../fresh -I../../../fresh/objects -I../../../fresh/widgets' + \
' -I../../../ctags/ctags -I../../../monkey -I../../../monkey -I../../../monkey/src -I. -Isrc -I../../../monkey/src/maininterface' + \
' -I../../../monkey/src/maininterface/ui -I../../../monkey/src/workspace -I../../../monkey/src/recentsmanager' + \
' -I../../../monkey/src/qscintillamanager -I../../../monkey/src/qscintillamanager/ui -I../../../monkey/src/ctags manager' + \
' -I../../../monkey/src/variablesmanager -I../../../monkey/src/templatesmanager -I../../../monkey/src/templatesmanager/ui' + \
' -I../../../monkey/src/abbreviationsmanager -I../../../monkey/src/abbreviationsmanager/ui -I../../../monkey/src/toolsmanager' + \
' -I../../../monkey/src/toolsmanager/ui -I../../../monkey/src/consolemanager -I/home/a/code/monkeyrepos/v2/trunk/monkey/src/consolemanager/ui' + \
' -I../../../monkey/src/pluginsmanager -I../../../monkey/src/pluginsmanager/ui -I../../../monkey/src/projectsmanager' + \
' -I../../../monkey/src/projectsmanager/ui -I/usr/X11R6/include -I../../../build/debug/.moc -I../../../build/debug/.ui' + \
' -o ../../../build/debug/.obj/unix/MSVCMake.o src/MSVCMake.cpp'
compiling.test(text,
					type = 'compiling')

print '# It is a machine generated file. Do not edit it manualy!\n'

print error.generateMkSScript('GCC')
print warning.generateMkSScript('GCC')
