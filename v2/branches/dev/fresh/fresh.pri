# fresh includes path to include in plugins/core project 

# for normal build, comment for build plugins
DEFINES	+= MONKEY_CORE_BUILD

# include path
INCLUDEPATH	+= .

# Objects
INCLUDEPATH	+= objects

# Widgets
INCLUDEPATH	+= widgets

# dependpath as includepath
DEPENDPATH	+= $$INCLUDEPATH
