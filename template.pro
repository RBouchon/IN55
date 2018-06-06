QT       += core gui widgets

TARGET = template
TEMPLATE = app

SOURCES += \
    src/main.cpp \
    src/mainwidget.cpp \
    src/geometryengine.cpp \
    src/animatedmodel/animatedmodel.cpp \
    src/animatedmodel/animation.cpp \
    src/animatedmodel/bone.cpp \
    src/animatedmodel/bonetransform.cpp \
    src/animatedmodel/keyframe.cpp \
    src/animatedmodel/modelloader.cpp \
    src/animatedmodel/vertex.cpp \
    src/camera.cpp

HEADERS += \
    src/mainwidget.h \
    src/geometryengine.h \
    src/animatedmodel/animatedmodel.h \
    src/animatedmodel/animation.h \
    src/animatedmodel/bone.h \
    src/animatedmodel/bonetransform.h \
    src/animatedmodel/keyframe.h \
    src/animatedmodel/modelloader.h \
    src/animatedmodel/vertex.h \
    src/camera.h

RESOURCES += \
    ressources/shaders.qrc \



win32:CONFIG(release, debug|release): LIBS += -L$$PWD/assimp/lib/x64/ -llibassimp.dll
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/assimp/lib/x64/ -llibassimp.dlld
else:unix: LIBS += -L$$PWD/assimp/lib/x64/ -llibassimp.dll

INCLUDEPATH += $$PWD/assimp/include
DEPENDPATH += $$PWD/assimp/include
