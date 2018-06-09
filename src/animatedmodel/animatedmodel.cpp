#include "animatedmodel.h"
#include "iostream"

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>


#include <QMatrix4x4>
#include <QString>
#include <QOpenGLTexture>
#include <QFileInfo>
#include <QSet>
#include <QList>

#include "vertex.h"
#include "bone.h"
#include "bonetransform.h"
#include "keyframe.h"
#include "animation.h"


AnimatedModel:: AnimatedModel(QString fileName)
{

    loadModelFromFile(fileName);


}

AnimatedModel::~AnimatedModel(){

}


//Initialize model from file
void AnimatedModel::loadModelFromFile(QString fileName){

    Assimp::Importer importer;

    int meshID = 0;


    const aiScene* scene = importer.ReadFile(fileName.toStdString(),
    aiProcess_CalcTangentSpace |
    aiProcess_Triangulate |
    aiProcess_JoinIdenticalVertices |
    aiProcess_SortByPType);

    // If the import failed, report it
    if( !scene)
    {
        std::cout << importer.GetErrorString() << std::endl;
    }

    //Get model informations from the scene
    aiMesh **meshes = scene->mMeshes;


    float dezoom = 20;


    //Get list of vertices
    QVector<Vertex*> verticesList;
    QVector<unsigned int> indicesList;

    for(unsigned int i = 0; i<meshes[meshID]->mNumVertices; ++i){

        //Initialize list of bones and weight for this vertex
        QVector<Bone*> vertexBones;
        QVector<float> weight;

        Vertex* newVertex = new Vertex(QVector3D(meshes[meshID]->mVertices[i].x/dezoom,meshes[meshID]->mVertices[i].y/dezoom, meshes[meshID]->mVertices[i].z/dezoom),
                                       QVector2D(meshes[meshID]->mTextureCoords[0][i].x,meshes[meshID]->mTextureCoords[0][i].y),
                                       QVector3D(meshes[meshID]->mNormals[i].x, meshes[meshID]->mNormals[i].y, meshes[meshID]->mNormals[i].z),
                                       vertexBones, weight);
        verticesList.append(newVertex);


    }



    //Create list of bones
    QVector<Bone*> bonesList;

    for(unsigned int j = 0; j<meshes[meshID]->mNumBones; ++j){ //for each bone, create a list of boneschilds

        QVector<QString> bonesChilds;
        for(unsigned int k = 0; k<scene->mRootNode->FindNode(meshes[meshID]->mBones[j]->mName)->mNumChildren; ++k){ //Get the name of each childrens
            bonesChilds.append(QString(scene->mRootNode->FindNode(meshes[meshID]->mBones[j]->mName)->mChildren[k]->mName.data));

        }
        aiMatrix4x4 offsetMatrix = meshes[meshID]->mBones[j]->mOffsetMatrix;
        aiMatrix4x4 transformMatrix = scene->mRootNode->FindNode(meshes[meshID]->mBones[j]->mName)->mTransformation;

        Bone* newBone = new Bone(QString(meshes[meshID]->mBones[j]->mName.data), bonesChilds,
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
        for(unsigned int k = 0; k<meshes[meshID]->mBones[j]->mNumWeights; ++k){
            verticesList[meshes[meshID]->mBones[j]->mWeights[k].mVertexId]->getBones().append(newBone);
            verticesList[meshes[meshID]->mBones[j]->mWeights[k].mVertexId]->getBonesWeight().append(meshes[meshID]->mBones[j]->mWeights[k].mWeight);
        }
    }




    //Get list of indices (faces with triangles primitive)
    for(unsigned int i = 0; i<meshes[meshID]->mNumFaces; ++i){

        indicesList.append(meshes[meshID]->mFaces[i].mIndices[0]);
        indicesList.append(meshes[meshID]->mFaces[i].mIndices[1]);
        indicesList.append(meshes[meshID]->mFaces[i].mIndices[2]);

    }


    // Get texture filename

    QString textureName;
    aiString Path;
    if(scene->mMaterials[0]->GetTexture(aiTextureType_DIFFUSE, 0, &Path, NULL, NULL, NULL, NULL, NULL)== AI_SUCCESS){
        textureName = QFileInfo(fileName).path() + '/' + QString(Path.data);

    }else{
        std::cout << "Erreur lors de la récupération du nom du fichier de texture" << std::endl;
    }



    //Get Animations list
    QVector<Animation*> animationsList;

    for(unsigned int i = 0; i<scene->mNumAnimations; ++i){ // For each animation

        QSet<double> timeStampSet; //Set of timeStamp for this animation

        // For each bones transformation of the animation, update the timestamp set
        for(unsigned int j = 0; j <scene->mAnimations[i]->mNumChannels; ++j){

            //Update the timeStamp set

            for(unsigned int k = 0; k <scene->mAnimations[i]->mChannels[j]->mNumPositionKeys; ++k){
                timeStampSet.insert(scene->mAnimations[i]->mChannels[j]->mPositionKeys[k].mTime);
            }
            for(unsigned int k = 0; k <scene->mAnimations[i]->mChannels[j]->mNumRotationKeys; ++k){
                timeStampSet.insert(scene->mAnimations[i]->mChannels[j]->mRotationKeys[k].mTime);
            }

            for(unsigned int k = 0; k <scene->mAnimations[i]->mChannels[j]->mNumScalingKeys; ++k){
                timeStampSet.insert(scene->mAnimations[i]->mChannels[j]->mScalingKeys[k].mTime);
            }

        }

        QVector<KeyFrame*> keyFramesList;
        QList<double> timeStampList = timeStampSet.toList();
        //Get each keyFrame
        for(int j = 0; j<timeStampList.size(); ++j){

            QVector<BoneTransform*> bonesTransforms;

            //For each bones
            for(unsigned int k = 0; k<scene->mAnimations[i]->mNumChannels; ++k){
                Bone* transformedBone = NULL;
                for(int m = 0;m<bones.size(); ++m){ // Get the bone from the list of bones
                    if(bones[m]->getName() == QString(scene->mAnimations[i]->mChannels[k]->mNodeName.data)){
                        transformedBone = bones[m];
                    }
                }

                if(transformedBone != NULL){
                    BoneTransform* bt = new BoneTransform(transformedBone);

                    for(unsigned int m = 0; m <scene->mAnimations[i]->mChannels[k]->mNumPositionKeys; ++m){
                        if(scene->mAnimations[i]->mChannels[k]->mPositionKeys[m].mTime == timeStampList[j]){
                            bt->position = QVector3D(scene->mAnimations[i]->mChannels[k]->mPositionKeys[m].mValue.x,
                                                    scene->mAnimations[i]->mChannels[k]->mPositionKeys[m].mValue.y,
                                                    scene->mAnimations[i]->mChannels[k]->mPositionKeys[m].mValue.z);
                        }
                    }

                    for(unsigned int m = 0; m <scene->mAnimations[i]->mChannels[k]->mNumRotationKeys; ++m){
                        if(scene->mAnimations[i]->mChannels[k]->mRotationKeys[m].mTime == timeStampList[j]){
                            bt->rotation = QQuaternion(scene->mAnimations[i]->mChannels[k]->mRotationKeys[m].mValue.w,
                                                      scene->mAnimations[i]->mChannels[k]->mRotationKeys[m].mValue.x,
                                                      scene->mAnimations[i]->mChannels[k]->mRotationKeys[m].mValue.y,
                                                      scene->mAnimations[i]->mChannels[k]->mRotationKeys[m].mValue.z);
                        }
                    }

                    for(unsigned int m = 0; m <scene->mAnimations[i]->mChannels[k]->mNumScalingKeys; ++m){
                        if(scene->mAnimations[i]->mChannels[k]->mScalingKeys[m].mTime == timeStampList[j]){
                            bt->scaling = QVector3D(scene->mAnimations[i]->mChannels[k]->mRotationKeys[m].mValue.x,
                                                    scene->mAnimations[i]->mChannels[k]->mRotationKeys[m].mValue.y,
                                                    scene->mAnimations[i]->mChannels[k]->mRotationKeys[m].mValue.z);
                        }
                    }
                    bonesTransforms.append(bt);
                }
            }

            keyFramesList.append(new KeyFrame(timeStampList[j], bonesTransforms));

        }
        animationsList.append(new Animation(QString (scene->mAnimations[i]->mName.data), scene->mAnimations[i]->mDuration, scene->mAnimations[i]->mTicksPerSecond, keyFramesList));

    }



    vertices = verticesList;
    indices = indicesList;
    bones = bonesList;
    textureFileName = textureName;
    animations = animationsList;

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
