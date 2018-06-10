#include "bonetransform.h"
#include <QVector3D>
#include <QQuaternion>


BoneTransform::BoneTransform(Bone *transformedBone)
    : bone(transformedBone), position(QVector3D(0.0,0.0,0.0)), rotation(QQuaternion(1.0,0.0,0.0,0.0)), scaling(QVector3D(1.0,1.0,1.0))
{

}

BoneTransform::~BoneTransform(){

}

Bone* BoneTransform::getBone(){
    return bone;
}

QVector3D BoneTransform::getPosition(){
    return position;
}

QQuaternion BoneTransform::getRotation(){
    return rotation;
}
QVector3D BoneTransform::getScaling(){
    return scaling;
}
