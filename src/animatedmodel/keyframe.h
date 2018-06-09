#ifndef KEYFRAME_H
#define KEYFRAME_H

#include <QVector>
#include "bonetransform.h"

class KeyFrame
{
public:
    KeyFrame(double time, QVector<BoneTransform*> transforms);
    ~KeyFrame();
private:
    double timeStamp;
    QVector<BoneTransform*> boneTransforms;

};

#endif // KEYFRAME_H
