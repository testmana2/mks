IDE_BUILD_TREE = $$OUT_PWD/../../../
include(qworkbench.pri)
# include 'plugins' dirrectory
INCLUDEPATH	*= ${PWD}/..
# include 'core' dirrectory
INCLUDEPATH	*= ${PWD}/../../core
