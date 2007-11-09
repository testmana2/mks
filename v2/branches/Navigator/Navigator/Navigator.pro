# include ctags framework
include( ../ctags/ctags.pri )

BUILD_PATH	= ../build

TEMPLATE	= app
LANGUAGE	= Qt4/C++
RESOURCES	= icons/icons.qrc
CONFIG	*= warn_on thread x11 windows debug_and_release
LIBS	= -L$${BUILD_PATH}
INCLUDEPATH	*= ../ctagswrapper
DESTDIR	= ../

CONFIG(debug, debug|release) {
	unix:LIBS	+= -lctags_debug
	else:LIBS	+= -lctagsd
} else {
	LIBS	+= -lctags
}

CONFIG(debug, debug|release) {
	#Debug
	CONFIG	+= console
	unix:TARGET	= $$join(TARGET,,,_debug)
	else:TARGET	= $$join(TARGET,,,d)
	unix:OBJECTS_DIR	= $${BUILD_PATH}/debug/.obj/unix
	win32:OBJECTS_DIR	= $${BUILD_PATH}/debug/.obj/win32
	mac:OBJECTS_DIR	= $${BUILD_PATH}/debug/.obj/mac
	UI_DIR	= $${BUILD_PATH}/debug/.ui
	MOC_DIR	= $${BUILD_PATH}/debug/.moc
	RCC_DIR	= $${BUILD_PATH}/debug/.rcc
} else {
	#Release
	unix:OBJECTS_DIR	= $${BUILD_PATH}/release/.obj/unix
	win32:OBJECTS_DIR	= $${BUILD_PATH}/release/.obj/win32
	mac:OBJECTS_DIR	= $${BUILD_PATH}/release/.obj/mac
	UI_DIR	= $${BUILD_PATH}/release/.ui
	MOC_DIR	= $${BUILD_PATH}/release/.moc
	RCC_DIR	= $${BUILD_PATH}/release/.rcc
}

HEADERS	= ../ctagswrapper/CtagsWrapper.h \
	src/Entity.h \
	src/EntityContainer.h \
	src/NavigatorSettings.h

SOURCES	= ../ctagswrapper/CtagsWrapper.cpp \
	src/Entity.cpp \
	src/EntityContainer.cpp \
	src/NavigatorSettings.cpp \
	src/main.cpp
