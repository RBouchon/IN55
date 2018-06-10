#include "bone.h"
#include <iostream>

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

void Bone::generateChildsIndex(QVector<Bone*> bonesList){



    for(int i = 0; i< bonesChilds.size(); ++i){
        for(int j = 0; j <bonesList.size(); ++j){

            if(bonesChilds[i] == bonesList[j]->getName()){
                childsIndex.append(j);
            }
        }

    }


}

QVector<unsigned int> Bone::getChildsIndex(){
    return childsIndex;
}
