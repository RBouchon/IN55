#ifndef ANIMATEDMODEL_H
#define ANIMATEDMODEL_H

#include <QList>
#include "bone.h"
#include "vertex.h"
#include "animation.h"

class AnimatedModel
{
public:
    AnimatedModel();

private:
    QList<Bone> bones;
    QList<Vertex> mesh;
    QList<Animation> animations;

};

#endif // ANIMATEDMODEL_H
