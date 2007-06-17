# fresh demo project file

TEMPLATE	= app

include( fresh.pro )

contains( TEMPLATE, app ):SOURCES	+= main.cpp
