QT       += core gui widgets multimedia

TARGET = template
TEMPLATE = app

SOURCES += \
    src/main.cpp \
    src/mainwidget.cpp \
    src/geometryengine.cpp \
    src/animatedmodel/animatedmodel.cpp \
    src/animatedmodel/bone.cpp \
    src/animatedmodel/vertex.cpp \
    src/camera.cpp

HEADERS += \
    src/mainwidget.h \
    src/geometryengine.h \
    src/animatedmodel/animatedmodel.h \
    src/animatedmodel/bone.h \
    src/animatedmodel/vertex.h \
    src/camera.h

RESOURCES += \
    ressources/shaders.qrc \



win32:CONFIG(release, debug|release): LIBS += -L$$PWD/assimp/lib/x64/ -llibassimp.dll
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/assimp/lib/x64/ -llibassimp.dlld
else:unix: LIBS += -L$$PWD/assimp/lib/x64/ -llibassimp.dll

INCLUDEPATH += $$PWD/assimp/include
DEPENDPATH += $$PWD/assimp/include
