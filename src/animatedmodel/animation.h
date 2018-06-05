#ifndef ANIMATION_H
#define ANIMATION_H

#include "keyframe.h"
#include <QList>

class Animation
{
public:
    Animation();
private:
    QList<KeyFrame> keyframes;
};

#endif // ANIMATION_H
