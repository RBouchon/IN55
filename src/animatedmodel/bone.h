#ifndef BONE_H
#define BONE_H

#include<QList>
#include<QMatrix4x4>

class Bone
{
public:
    Bone();
private:
    QList<Bone> boneChilds;
    QMatrix4x4 transform;
};

#endif // BONE_H
