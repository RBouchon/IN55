#include "bone.h"
#include <iostream>


/**
 * @brief Bone::Bone
 *
 * Constructor for a Bone.
 *
 * @param boneName
 * @param bonesChildsName
 * @param transformMatrix
 * @param offsetMatrix
 */
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

QMatrix4x4 Bone::getOffset(){
    return offset;
}

QMatrix4x4 Bone::getTransform(){
    return transform;
}

QVector<QString> Bone::getBonesChilds(){
    return bonesChilds;
}
