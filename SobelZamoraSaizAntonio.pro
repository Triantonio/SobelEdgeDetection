QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

INCLUDEPATH += \
$$PWD/include/GUI/SobelZamoraSaizAntonio

SOURCES += \
    src/main.cpp \
    $$files($$PWD/src/GUI/SobelZamoraSaizAntonio\*.cpp)

HEADERS += \
    $$files($$PWD/include/GUI/SobelZamoraSaizAntonio\*.hpp)\

FORMS += \
    $$files($$PWD/resources/UI/*.ui)

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

win32:CONFIG(release, debug|release): LIBS += -L$$PWD/../../../../../../../../opencv/build/x64/vc15/lib/ -lopencv_world455
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/../../../../../../../../opencv/build/x64/vc15/lib/ -lopencv_world455d
else:unix: LIBS += -L$$PWD/../../../../../../../../opencv/build/x64/vc15/lib/ -lopencv_world455

INCLUDEPATH += $$PWD/../../../../../../../../opencv/build/include
DEPENDPATH += $$PWD/../../../../../../../../opencv/build/include
