TEMPLATE = lib
CONFIG *= staticlib

TARGET = Core
DEFINES += CORE_LIBRARY
QT += xml \
    script \
    svg

include(../../qworkbenchplugin.pri)
include(../../libs/utils/utils.pri)
include(../../shared/scriptwrapper/scriptwrapper.pri)
include(coreplugin_dependencies.pri)
INCLUDEPATH += dialogs \
    actionmanager \
    editormanager \
    progressmanager \
    scriptmanager

DEPENDPATH += dialogs \
    actionmanager \
    editormanager
SOURCES += \
    messagemanager.cpp \
    messageoutputwindow.cpp \
    basefilewizard.cpp \
    fancytabwidget.cpp \
    variablemanager.cpp \
    stylehelper.cpp \
    minisplitter.cpp \
    baseview.cpp \
    filemanager.cpp \
    tabpositionindicator.cpp \
    vcsmanager.cpp \
    inavigationwidgetfactory.cpp \
    mainwindow.cpp \
    coreimpl.cpp \
    basemode.cpp \
    uniqueidmanager.cpp \
    modemanager.cpp \
    findplaceholder.cpp \
    rightpane.cpp \
    outputpane.cpp \
    navigationwidget.cpp \
    mimedatabase.cpp \
    editormanager/editorgroup.cpp \
    editormanager/editormanager.cpp \
    editormanager/stackededitorgroup.cpp \
    editormanager/editorsplitter.cpp \
    editormanager/openeditorsview.cpp \
    editormanager/openeditorswindow.cpp \
    dialogs/saveitemsdialog.cpp \
    dialogs/newdialog.cpp \
    dialogs/settingsdialog.cpp \
    dialogs/shortcutsettings.cpp \
    dialogs/openwithdialog.cpp \ 
    progressmanager/progressmanager.cpp \
    progressmanager/progressview.cpp \
    progressmanager/progresspie.cpp \
    progressmanager/futureprogress.cpp \
    scriptmanager/scriptmanager.cpp \
    scriptmanager/qworkbench_wrapper.cpp \
    actionmanager/actionmanager.cpp \
    actionmanager/command.cpp \
    actionmanager/actioncontainer.cpp \
    actionmanager/commandsfile.cpp \

HEADERS += \
    core_global.h \
    ioutputpane.h \
    messagemanager.h \
    messageoutputwindow.h \
    basefilewizard.h \
    ifilewizardextension.h \
    icorelistener.h \
    fancytabwidget.h \
    variablemanager.h \
    stylehelper.h \
    iview.h \
    baseview.h \
    filemanager.h \
    iversioncontrol.h \
    vcsmanager.h \
    tabpositionindicator.h \
    inavigationwidgetfactory.h \
    mainwindow.h \
    coreimpl.h \
    ifilefactory.h \
    uniqueidmanager.h \
    mimedatabase.h \
    outputpane.h \
    editormanager/editorgroup.h \
    editormanager/editormanager.h \
    editormanager/stackededitorgroup.h \
    editormanager/editorsplitter.h \
    editormanager/openeditorsview.h \
    editormanager/openeditorswindow.h \
    editormanager/ieditor.h \
    editormanager/ieditorfactory.h \
    dialogs/saveitemsdialog.h \
    dialogs/newdialog.h \
    dialogs/settingsdialog.h \
    dialogs/shortcutsettings.h \
    dialogs/openwithdialog.h \
    dialogs/iwizard.h \
    dialogs/ioptionspage.h \
    progressmanager/progressmanager_p.h \
    progressmanager/progressview.h \
    progressmanager/progresspie.h \
    progressmanager/futureprogress.h \
    progressmanager/progressmanager.h \
    actionmanager/actioncontainer.h \
    actionmanager/actionmanager.h \
    actionmanager/command.h \
    actionmanager/actionmanager_p.h \
    actionmanager/command_p.h \
    actionmanager/actioncontainer_p.h \
    actionmanager/commandsfile.h \
    scriptmanager/metatypedeclarations.h \
    scriptmanager/qworkbench_wrapper.h \
    scriptmanager/scriptmanager.h \
    scriptmanager/scriptmanager_p.h \
    icontext.h \
    icore.h \
    ifile.h \
    imode.h \
    coreconstants.h \
    basemode.h \
    modemanager.h \
    plugindialog.h \
    navigationwidget.h \
    findplaceholder.h \
    rightpane.h

FORMS += dialogs/newdialog.ui \
    dialogs/settingsdialog.ui \
    dialogs/shortcutsettings.ui \
    dialogs/saveitemsdialog.ui \
    dialogs/openwithdialog.ui \
    editormanager/openeditorsview.ui


