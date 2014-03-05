QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = fg01
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
	scene.cpp \
	item.cpp \
	textitem.cpp \
	line.cpp \
	gridlines.cpp \
    libstructure.cpp \
    port.cpp \
    connection.cpp \
    mouseaction.cpp

HEADERS  += mainwindow.h \
	scene.h \
	item.h \
	textitem.h \
	line.h \
	gridlines.h \
    libstructure.h \
    port.h \
    connection.h \
    mouseaction.h

FORMS    += mainwindow.ui
OTHER_FILES += \
    images/underline.png \
    images/textpointer.png \
    images/sendtoback.png \
    images/pointer.png \
    images/linepointer.png \
    images/linecolor.png \
    images/italic.png \
    images/floodfill.png \
    images/delete.png \
    images/bringtofront.png \
    images/bold.png \
    images/background6.png \
    images/background5.png \
    images/background4.png \
    images/background3.png \
    images/background2.png \
    images/background1.png \
    qnodeseditor.pro

RESOURCES += \
    fg01.qrc
