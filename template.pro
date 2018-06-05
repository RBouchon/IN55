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
    src/animatedmodel/vertex.cpp

HEADERS += \
    src/mainwidget.h \
    src/geometryengine.h \
    src/animatedmodel/animatedmodel.h \
    src/animatedmodel/animation.h \
    src/animatedmodel/bone.h \
    src/animatedmodel/bonetransform.h \
    src/animatedmodel/keyframe.h \
    src/animatedmodel/modelloader.h \
    src/animatedmodel/vertex.h

RESOURCES += \
    ressources/shaders.qrc \
