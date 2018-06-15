#include "animatedmodel.h"
#include "iostream"
#include "math.h"

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>


#include <QMatrix4x4>
#include <QString>
#include <QOpenGLTexture>
#include <QFileInfo>

#include "vertex.h"
#include "bone.h"




AnimatedModel:: AnimatedModel(){

}

AnimatedModel:: AnimatedModel(QString fileName)
{

    Assimp::Importer importer;

    scene = importer.ReadFile(fileName.toStdString(),
    aiProcess_CalcTangentSpace |
    aiProcess_Triangulate |
    aiProcess_JoinIdenticalVertices |
    aiProcess_SortByPType );

    scene = importer.GetOrphanedScene();

    if( !scene)
    {
        std::cout << importer.GetErrorString() << std::endl;
    }


    loadModelFromFile(fileName);


}

AnimatedModel::~AnimatedModel(){


}


bool AnimatedModel::loadAnimationFromFile(QString fileName, QString animationName){
    Assimp::Importer importer;

    const aiScene* tmpScene = importer.ReadFile(fileName.toStdString(),
    aiProcess_CalcTangentSpace |
    aiProcess_Triangulate |
    aiProcess_JoinIdenticalVertices |
    aiProcess_SortByPType );


    if( !tmpScene)
    {
       std::cout << importer.GetErrorString() << std::endl;
       return false;
    }else{
       animations.insert(animationName, importer.GetOrphanedScene());
       return true;
    }


}


//Initialize model from file
void AnimatedModel::loadModelFromFile(QString fileName){


    // If the import failed, report it

    //Get model informations from the scene
    aiMesh **meshes = scene->mMeshes;




    //Get list of vertices
    QVector<Vertex*> verticesList;
    QVector<unsigned int> indicesList;

    for(unsigned int i = 0; i<meshes[0]->mNumVertices; ++i){

        //Initialize list of bones and weight for this vertex
        QVector<Bone*> vertexBones;
        QVector<float> weight;

        Vertex* newVertex = new Vertex(QVector3D(meshes[0]->mVertices[i].x,meshes[0]->mVertices[i].y, meshes[0]->mVertices[i].z),
                                       QVector2D(meshes[0]->mTextureCoords[0][i].x,meshes[0]->mTextureCoords[0][i].y),
                                       QVector3D(meshes[0]->mNormals[i].x, meshes[0]->mNormals[i].y, meshes[0]->mNormals[i].z),
                                       vertexBones, weight);
        verticesList.append(newVertex);


    }



    //Create list of bones
    QVector<Bone*> bonesList;

    for(unsigned int j = 0; j<meshes[0]->mNumBones; ++j){ //for each bone, create a list of boneschilds

        QVector<QString> bonesChilds;
        for(unsigned int k = 0; k<scene->mRootNode->FindNode(meshes[0]->mBones[j]->mName)->mNumChildren; ++k){ //Get the name of each childrens
            bonesChilds.append(QString(scene->mRootNode->FindNode(meshes[0]->mBones[j]->mName)->mChildren[k]->mName.data));

        }
        aiMatrix4x4 offsetMatrix = meshes[0]->mBones[j]->mOffsetMatrix;

        aiMatrix4x4 transformMatrix = scene->mRootNode->FindNode(meshes[0]->mBones[j]->mName)->mTransformation;

        Bone* newBone = new Bone(QString(meshes[0]->mBones[j]->mName.data), bonesChilds,
                                 QMatrix4x4(transformMatrix.a1, transformMatrix.a2, transformMatrix.a3, transformMatrix.a4,
                                            transformMatrix.b1, transformMatrix.b2, transformMatrix.b3, transformMatrix.b4,
                                            transformMatrix.c1, transformMatrix.c2, transformMatrix.c3, transformMatrix.c4,
                                            transformMatrix.d1, transformMatrix.d2, transformMatrix.d3, transformMatrix.d4),
                                 QMatrix4x4(offsetMatrix.a1, offsetMatrix.a2, offsetMatrix.a3, offsetMatrix.a4,
                                            offsetMatrix.b1, offsetMatrix.b2, offsetMatrix.b3, offsetMatrix.b4,
                                            offsetMatrix.c1, offsetMatrix.c2, offsetMatrix.c3, offsetMatrix.c4,
                                            offsetMatrix.d1, offsetMatrix.d2, offsetMatrix.d3, offsetMatrix.d4));
        bonesList.append(newBone);


        //Set the bones and weights on each vertices affected by this bone

        for(unsigned int k = 0; k<meshes[0]->mBones[j]->mNumWeights; ++k){

            verticesList[meshes[0]->mBones[j]->mWeights[k].mVertexId]->getBones().append(newBone);
            verticesList[meshes[0]->mBones[j]->mWeights[k].mVertexId]->getBonesWeight().append(meshes[0]->mBones[j]->mWeights[k].mWeight);

        }
    }





    //Get list of indices (faces with triangles primitive)
    for(unsigned int i = 0; i<meshes[0]->mNumFaces; ++i){

        indicesList.append(meshes[0]->mFaces[i].mIndices[0]);
        indicesList.append(meshes[0]->mFaces[i].mIndices[1]);
        indicesList.append(meshes[0]->mFaces[i].mIndices[2]);

    }


    // Get texture filename

    QString textureName;
    aiString Path;
    if(scene->mMaterials[0]->GetTexture(aiTextureType_DIFFUSE, 0, &Path, NULL, NULL, NULL, NULL, NULL)== AI_SUCCESS){
        textureName = QFileInfo(fileName).path() + '/' + QString(Path.data);

    }else{
        std::cout << "Erreur lors de la récupération du nom du fichier de texture" << std::endl;

    }




    // Generate childIndex list for each bones
    for(int i = 0; i <bonesList.size(); ++i){
        bonesList[i]->generateChildsIndex(bonesList);
    }

    vertices = verticesList;
    indices = indicesList;
    bones = bonesList;
    textureFileName = textureName;



    aiMatrix4x4 rootTransform = scene->mRootNode->mTransformation;
    globalTransform = QMatrix4x4(rootTransform.a1, rootTransform.a2, rootTransform.a3, rootTransform.a4,
                                    rootTransform.b1, rootTransform.b2, rootTransform.b3, rootTransform.b4,
                                    rootTransform.c1, rootTransform.c2, rootTransform.c3, rootTransform.c4,
                                    rootTransform.d1, rootTransform.d2, rootTransform.d3, rootTransform.d4);



}


QVector<QMatrix4x4> AnimatedModel::getTransformationsAtTime(double time, aiAnimation* animation){


    QVector<QMatrix4x4> transformationList;
    transformationList.resize(bones.size());


    double animationTime = time*animation->mDuration;

    calculateBonesTransformations(animationTime, animation, transformationList, globalTransform, scene->mRootNode );


    return transformationList;
}

void AnimatedModel::calculateBonesTransformations(double time, aiAnimation* animation, QVector<QMatrix4x4> &transformationList, QMatrix4x4 parentTransformation, aiNode* node){



    aiMatrix4x4 nodeTransformMatrix = node->mTransformation;


    QMatrix4x4 nodeTransform = QMatrix4x4(nodeTransformMatrix.a1, nodeTransformMatrix.a2, nodeTransformMatrix.a3 ,nodeTransformMatrix.a4,
                                          nodeTransformMatrix.b1, nodeTransformMatrix.b2, nodeTransformMatrix.b3 ,nodeTransformMatrix.b4,
                                          nodeTransformMatrix.c1, nodeTransformMatrix.c2, nodeTransformMatrix.c3 ,nodeTransformMatrix.c4,
                                          nodeTransformMatrix.d1, nodeTransformMatrix.d2, nodeTransformMatrix.d3 ,nodeTransformMatrix.d4);



    for(unsigned int i = 0; i<animation->mNumChannels; ++i){
        if(node->mName == animation->mChannels[i]->mNodeName){


            QMatrix4x4 T = interpolateTranslation(time, animation->mChannels[i]);
            QMatrix4x4 R = interpolateRotation(time, animation->mChannels[i]);
            QMatrix4x4 S = interpolateScaling(time, animation->mChannels[i]);

            nodeTransform =  T*R*S;

        }
    }


    QMatrix4x4 transformation = parentTransformation*nodeTransform;

    for(int i = 0; i<bones.size(); ++i){
        if(bones[i]->getName() == QString(node->mName.data)){
            transformationList[i] = globalTransform.inverted()*transformation*bones[i]->getOffset();

        }
    }


    for (unsigned int i = 0 ; i < node->mNumChildren ; i++) {

        calculateBonesTransformations(time, animation, transformationList, transformation, node->mChildren[i]);

    }

}


QMatrix4x4 AnimatedModel::interpolateTranslation(double time, aiNodeAnim* animationNode){
    QMatrix4x4 identity;

    identity.setToIdentity();

    if (animationNode->mNumPositionKeys == 1) {
        return QMatrix4x4(1,0,0, animationNode->mPositionKeys[0].mValue.x,
                          0,1,0, animationNode->mPositionKeys[0].mValue.y,
                          0,0,1, animationNode->mPositionKeys[0].mValue.z,
                          0,0,0,1);

    }
    if (animationNode->mNumPositionKeys == 0) {
        return identity;

    }

    unsigned int positionIndex=0;
    for (unsigned int i = 0 ; i < animationNode->mNumPositionKeys - 1 ; ++i) {
        if (time < animationNode->mPositionKeys[i + 1].mTime) {
            positionIndex = i;
            break;
        }
    }

    unsigned int nextPositionIndex = positionIndex +1;
    if(nextPositionIndex >= animationNode->mNumPositionKeys ){
        return identity;
    }

    float deltaTime = animationNode->mPositionKeys[nextPositionIndex].mTime - animationNode->mPositionKeys[positionIndex].mTime;
    float factor = (time - animationNode->mPositionKeys[positionIndex].mTime) / deltaTime;

    aiVector3D start = animationNode->mPositionKeys[positionIndex].mValue;
    aiVector3D end = animationNode->mPositionKeys[nextPositionIndex].mValue;
    aiVector3D delta = end - start;
    aiVector3D interpolatedVector = start + factor * delta;
    QMatrix4x4 T;

    T.translate(interpolatedVector.x, interpolatedVector.y, interpolatedVector.z);

    return T ;


}

QMatrix4x4 AnimatedModel::interpolateRotation(double time, aiNodeAnim* animationNode){
    QMatrix4x4 identity;
    aiMatrix3x3 rotationMatrix;
    identity.setToIdentity();

    if (animationNode->mNumRotationKeys == 1) {
        rotationMatrix = animationNode->mRotationKeys[0].mValue.GetMatrix();
        return QMatrix4x4(rotationMatrix.a1, rotationMatrix.a2, rotationMatrix.a3, 0,
                rotationMatrix.b1, rotationMatrix.b2, rotationMatrix.b3, 0,
                rotationMatrix.c1, rotationMatrix.c2, rotationMatrix.c3, 0,
                0,0,0,1) ;

    }

    if (animationNode->mNumRotationKeys == 0) {
        return identity;

    }

    unsigned int rotationIndex = 0;
    for (unsigned int i = 0 ; i < animationNode->mNumRotationKeys - 1 ; ++i) {
        if (time < animationNode->mRotationKeys[i + 1].mTime) {
            rotationIndex = i;
            break;
        }
    }

    unsigned int nextRotationIndex = rotationIndex +1;
    if(nextRotationIndex >= animationNode->mNumRotationKeys ){
        return identity;
    }

    float deltaTime = animationNode->mRotationKeys[nextRotationIndex].mTime - animationNode->mRotationKeys[rotationIndex].mTime;
    float factor = (time - animationNode->mRotationKeys[rotationIndex].mTime) / deltaTime;


    aiQuaternion start = animationNode->mRotationKeys[rotationIndex].mValue;
    aiQuaternion end = animationNode->mRotationKeys[nextRotationIndex].mValue;
    aiQuaternion interpolatedRotation;
    aiQuaternion::Interpolate(interpolatedRotation, start, end, factor);
    interpolatedRotation.Normalize();

    rotationMatrix = interpolatedRotation.GetMatrix();

    QQuaternion q = QQuaternion(interpolatedRotation.w,interpolatedRotation.x, interpolatedRotation.y, interpolatedRotation.z);
    QMatrix4x4 R;


    R.rotate(q);
    return R;


}

QMatrix4x4 AnimatedModel::interpolateScaling(double time, aiNodeAnim* animationNode){
    QMatrix4x4 identity;

    identity.setToIdentity();

    if (animationNode->mNumScalingKeys == 1) {
        return QMatrix4x4(animationNode->mScalingKeys[0].mValue.x,0,0,0,
                0,animationNode->mScalingKeys[0].mValue.y,0,0,
                0,0,animationNode->mScalingKeys[0].mValue.z,0,
                0,0,0,1);

    }
    if (animationNode->mNumScalingKeys == 0) {
        return identity;

    }

    unsigned int positionIndex = 0;
    for (unsigned int i = 0 ; i < animationNode->mNumScalingKeys - 1 ; ++i) {
        if (time < animationNode->mScalingKeys[i + 1].mTime) {
            positionIndex = i;
            break;
        }
    }

    unsigned int nextPositionIndex = positionIndex +1;
    if(nextPositionIndex >= animationNode->mNumScalingKeys ){
        return identity;
    }

    float deltaTime = animationNode->mScalingKeys[nextPositionIndex].mTime - animationNode->mScalingKeys[positionIndex].mTime;
    float factor = (time - animationNode->mScalingKeys[positionIndex].mTime) / deltaTime;

    aiVector3D start = animationNode->mScalingKeys[positionIndex].mValue;
    aiVector3D end = animationNode->mScalingKeys[nextPositionIndex].mValue;
    aiVector3D delta = end - start;
    aiVector3D interpolatedVector = start + factor * delta;

    QMatrix4x4 S;
    S.scale(interpolatedVector.x, interpolatedVector.y, interpolatedVector.z);
    return S;
}


QVector<Vertex*> AnimatedModel::getVertices(){
    return vertices;
}

QVector<unsigned int> AnimatedModel::getIndices(){
    return indices;
}

QString AnimatedModel::getTextureFileName(){
    return textureFileName;
}

QVector<Bone*> AnimatedModel::getBones(){
    return bones;
}

QMap<QString, aiScene *> AnimatedModel::getAnimations(){
    return animations;
}
