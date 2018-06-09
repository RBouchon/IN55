#include "keyframe.h"

KeyFrame::KeyFrame(double time, QVector<BoneTransform *> transforms)
    : timeStamp(time), boneTransforms(transforms)
{

}

KeyFrame::~KeyFrame(){

}
