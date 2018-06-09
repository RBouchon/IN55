#include "animation.h"

Animation::Animation(QString animationName, double frameNumber, double framePerSecond, QVector<KeyFrame *> keyFramesList)
    : name(animationName), frameNum(frameNumber), FPS(framePerSecond), keyFrames(keyFramesList)
{

}

Animation::~Animation(){

}

QString Animation::getName(){
    return name;
}

double Animation::getFrameNumber(){
    return frameNum;
}

double Animation::getFramePerSecond(){
    return FPS;
}

QVector<KeyFrame*> Animation::getKeyFramesList(){
    return keyFrames;
}
