#ifndef BONE_H
#define BONE_H

#include <QVector>
#include <QMatrix4x4>
#include <QString>

class Bone
{
public:
    Bone(QString boneName, QVector<QString> bonesChildsName, QMatrix4x4 transformMatrix, QMatrix4x4 offsetMatrix);
    ~Bone();
    QString getName();
    QMatrix4x4 getOffset();
    QMatrix4x4 getTransform();
    QVector<QString> getBonesChilds();
private:
    QString name; // name of this bone
    QVector<QString> bonesChilds; // List of childs names
    QMatrix4x4 transform; // The transformation relative to the bone's parent.
    QMatrix4x4 offset; // Matrix that transforms from mesh space to bone space in bind pose
};

#endif // BONE_H
