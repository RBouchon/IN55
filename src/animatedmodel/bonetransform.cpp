#include "bonetransform.h"
#include <QVector3D>
#include <QQuaternion>


BoneTransform::BoneTransform(Bone *transformedBone)
    : bone(transformedBone), position(QVector3D(0.0,0.0,0.0)), rotation(QQuaternion(1.0,0.0,0.0,0.0)), scaling(QVector3D(1.0,1.0,1.0))
{

}

BoneTransform::~BoneTransform(){

}
