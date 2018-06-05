#ifndef KEYFRAME_H
#define KEYFRAME_H

#include <QList>
#include "bonetransform.h"

class KeyFrame
{
public:
    KeyFrame();
private:
    float timeStamp;
    QList<BoneTransform> boneTransforms;

};

#endif // KEYFRAME_H
