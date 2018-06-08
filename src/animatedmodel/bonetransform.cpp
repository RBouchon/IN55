#include "bonetransform.h"

BoneTransform::BoneTransform(Bone transformedBone, QVector3D positionTransform, QQuaternion rotationTransform, QVector3D scalingTransform)
    : bone(transformedBone), position(positionTransform), rotation(rotationTransform), scaling(scalingTransform)
{

}

BoneTransform::~BoneTransform(){

}
