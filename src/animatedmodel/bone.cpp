#include "bone.h"

Bone::Bone(QString boneName, QVector<QString> bonesChildsName, QMatrix4x4 transformMatrix, QMatrix4x4 offsetMatrix)
    : name(boneName), bonesChilds(bonesChildsName), transform(transformMatrix), offset(offsetMatrix)
{

}

Bone::~Bone()
{

}

QString Bone::getName(){
    return name;
}
