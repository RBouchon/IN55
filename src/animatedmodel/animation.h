#ifndef ANIMATION_H
#define ANIMATION_H

#include "keyframe.h"
#include <QVector>
#include <QString>

class Animation
{
public:
    Animation(QString animationName, double frameNumber, double framePerSecond, QVector<KeyFrame*> keyFramesList);
    ~Animation();
    QString getName();
    double getFrameNumber();
    double getFramePerSecond();
    QVector<KeyFrame*> getKeyFramesList();
private:
    QString name;
    double frameNum;
    double FPS;
    QVector<KeyFrame*> keyFrames;
};

#endif // ANIMATION_H
