#ifndef BONETRANSFORM_H
#define BONETRANSFORM_H

#include <QVector3D>
#include <QQuaternion>
#include "bone.h"


class BoneTransform
{

    friend class AnimatedModel;

public:
    BoneTransform(Bone* transformedBone);
    ~BoneTransform();
    Bone* getBone();
    QVector3D getPosition();
    QQuaternion getRotation();
    QVector3D getScaling();
private:
    Bone* bone;
    QVector3D position; //Position transformation (translation)
    QQuaternion rotation;
    QVector3D scaling;
};

#endif // BONETRANSFORM_H
