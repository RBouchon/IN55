#ifndef ANIMATION_H
#define ANIMATION_H

#include "keyframe.h"
#include <QVector>
#include <QString>

class Animation
{
public:
    Animation(QString animationName, double frameNumber, double framePerSecond);
    ~Animation();
private:
    QString name;
    double frameNum;
    double FPS;
    QVector<KeyFrame> keyframes;
};

#endif // ANIMATION_H
