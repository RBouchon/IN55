#include "animation.h"

Animation::Animation(QString animationName, double frameNumber, double framePerSecond)
    : name(animationName), frameNum(frameNumber), FPS(framePerSecond)
{

}

Animation::~Animation(){

}
