# coding=utf8

import parsing
from string import Template

file_name = r"[\w\d\./\\]+"
number = "\d+"
compiler = "(gcc|g\+\+|mingw32-gcc|mingw32-g\+\+)"
source_file = file_name + "\.c(pp)?"

## Error
regEx = Template("^($file_name):($number): (error:|undefined reference) [^\n]+")
error =parsing.Pattern(regEx.substitute(file_name = file_name, number = number),
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

error.test("/home/a/code/monkeyrepos/v2/branches/dev/monkey/src/main.cpp:30: undefined reference to `fake()'",
				type = 'error',
				file = '/home/a/code/monkeyrepos/v2/branches/dev/monkey/src/main.cpp',
				line = '30',
				text = "/home/a/code/monkeyrepos/v2/branches/dev/monkey/src/main.cpp:30: undefined reference to `fake()'",
				hint = "/home/a/code/monkeyrepos/v2/branches/dev/monkey/src/main.cpp:30: undefined reference to `fake()'")

error.test("src/ui/UIMonkeyProjectSettings.cpp:26: error: expected `;' before 'setWindowTitle'",
				type = 'error',
				file = 'src/ui/UIMonkeyProjectSettings.cpp',
				line = '26',
				text = "src/ui/UIMonkeyProjectSettings.cpp:26: error: expected `;' before 'setWindowTitle'",
				hint = "src/ui/UIMonkeyProjectSettings.cpp:26: error: expected `;' before 'setWindowTitle'")

text = "/home/a/code/monkeyrepos/v2/branches/dev/monkey/src/xupmanager/ui/UIXUPManager.cpp:313: undefined reference to `pRecentsManager::recentProjectOpenPath() const'"
error.test(	text,
					type = 'error',
					file = '/home/a/code/monkeyrepos/v2/branches/dev/monkey/src/xupmanager/ui/UIXUPManager.cpp',
					line = '313',
					text = text,
					hint = text)

text = "src/views/TreeView.cpp:20: error: cannot allocate an object of abstract type 'TreeViewModel'"
error.test(	text,
					type = 'error',
					file = 'src/views/TreeView.cpp',
					line = '20',
					text = text,
					hint = text)

## Warning
regEx = Template("^($file_name):($number): warning: (.+)")
warning =parsing.Pattern(regEx.substitute(file_name = file_name, number = number),
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

# windows specific
warning.test("src\CmlLineManager.cpp:11: warning: unused variable 'z'",
					 type = 'warning',
					 file = 'src\CmlLineManager.cpp',
					 line = '11',
					 text = "src\CmlLineManager.cpp:11: warning: unused variable 'z'",
					 hint = "src\CmlLineManager.cpp:11: warning: unused variable 'z'")

## Link failed
regEx = "collect2: ld returned 1 exit status"
link_failed = parsing.Pattern(regEx,
											type = 'error',
											text = 'Linking failed')
link_failed.setComment('Link failed, ld returned 1 exit status')
link_failed.test(regEx,
						type = 'error',
						text = 'Linking failed',
						hint = regEx)


## Building file
regEx = Template("^$compiler[^\n]+ ($source_file)")
compiling =parsing.Pattern(regEx.substitute(compiler=compiler, source_file=source_file),
										   type = 'compiling',
										   text = 'Compiling %2...',
										   file = '%2')

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
					type = 'compiling',
					text = 'Compiling src/MSVCMake.cpp...',
					hint = text,
					file = 'src/MSVCMake.cpp')

# it is windows specific
text = 'g++ -c -g -Wall -frtti -fexceptions -mthreads -DPACKAGE_NAME="\"Mirrorad\"" -DQT_DLL -DQT_CORE_LIB -DQT_THREAD_SUPPORT -I"..\..\Qt\4.4.3\include\QtCore" -I"..\..\Qt\4.4.3\include\QtCore" -I"..\..\Qt\4.4.3\include" -I"src" -I"src\ie" -I"src\ff" -I"src\ui" -I"src\opera" -I"c:\Development\Qt\4.4.3\include\ActiveQt" -I"build\debug\.moc" -I"build\debug\.ui" -I"..\..\Qt\4.4.3\mkspecs\win32-g++" -o build\debug\.obj\win32\main.o src\main.cpp'
compiling.test(	text,
						type = 'compiling',
						text = 'Compiling src\main.cpp...',
						file = 'src\main.cpp',
						hint = text)

# Cannot find library
regEx = "^/usr/bin/ld: cannot find -l([\w]+)"
no_lib =parsing.Pattern(regEx,
									type = 'error',
									text = 'Cannot find library "%1"')

no_lib.setComment('Linker cannot find library')
no_lib.test('/usr/bin/ld: cannot find -lqscintilla2d',
				 type = 'error',
				 text = 'Cannot find library "qscintilla2d"',
				 hint = '/usr/bin/ld: cannot find -lqscintilla2d')

print '# It is a machine generated file. Do not edit it manualy!\n'

print error.generateMkSScript('GCC')
print warning.generateMkSScript('GCC')
print compiling.generateMkSScript('GCC')
print no_lib.generateMkSScript('GCC')
print link_failed.generateMkSScript('GCC')
