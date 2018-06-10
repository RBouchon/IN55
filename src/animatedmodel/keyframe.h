#ifndef KEYFRAME_H
#define KEYFRAME_H

#include <QVector>
#include "bonetransform.h"

class KeyFrame
{
public:
    KeyFrame(double time, QVector<BoneTransform*> transforms);
    ~KeyFrame();
    double getTimeStamp();
    QVector<BoneTransform*> getBoneTransforms();

private:
    double timeStamp;
    QVector<BoneTransform*> boneTransforms;

};

#endif // KEYFRAME_H
