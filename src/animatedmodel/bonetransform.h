#ifndef BONETRANSFORM_H
#define BONETRANSFORM_H

#include <QVector3D>
#include <QQuaternion>
#include "bone.h"

class BoneTransform
{
public:
    BoneTransform(Bone transformedBone, QVector3D positionTransform, QQuaternion rotationTransform, QVector3D scalingTransform);
    ~BoneTransform();
private:
    Bone bone;
    QVector3D position; //Position transformation (translation)
    QQuaternion rotation;
    QVector3D scaling;
};

#endif // BONETRANSFORM_H
