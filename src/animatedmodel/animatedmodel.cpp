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
#include <QSet>
#include <QList>
#include <algorithm>

#include "vertex.h"
#include "bone.h"
#include "bonetransform.h"
#include "keyframe.h"
#include "animation.h"


AnimatedModel:: AnimatedModel(QString fileName)
{

    Assimp::Importer importer;




    scene = importer.ReadFile(fileName.toStdString(),
    aiProcess_CalcTangentSpace |
    aiProcess_Triangulate |
    aiProcess_JoinIdenticalVertices |
    aiProcess_SortByPType);

    scene = importer.GetOrphanedScene();

    if( !scene)
    {
        std::cout << importer.GetErrorString() << std::endl;
    }


    loadModelFromFile(fileName);


}

AnimatedModel::~AnimatedModel(){

}


//Initialize model from file
void AnimatedModel::loadModelFromFile(QString fileName){


int meshID = 0;
    // If the import failed, report it

    //Get model informations from the scene
    aiMesh **meshes = scene->mMeshes;





    //Get list of vertices
    QVector<Vertex*> verticesList;
    QVector<unsigned int> indicesList;

    for(unsigned int i = 0; i<meshes[meshID]->mNumVertices; ++i){

        //Initialize list of bones and weight for this vertex
        QVector<Bone*> vertexBones;
        QVector<float> weight;

        Vertex* newVertex = new Vertex(QVector3D(meshes[meshID]->mVertices[i].x,meshes[meshID]->mVertices[i].y, meshes[meshID]->mVertices[i].z),
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
                for(int m = 0;m<bonesList.size(); ++m){ // Get the bone from the list of bones
                    if(bonesList[m]->getName() == QString(scene->mAnimations[i]->mChannels[k]->mNodeName.data)){
                        transformedBone = bonesList[m];
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
        std::sort(keyFramesList.begin(), keyFramesList.end(), []( KeyFrame* a,  KeyFrame* b){
            return a->getTimeStamp()<b->getTimeStamp();
        });

        animationsList.append(new Animation(QString (scene->mAnimations[i]->mName.data), scene->mAnimations[i]->mDuration, scene->mAnimations[i]->mTicksPerSecond, keyFramesList));

    }





    // Generate childIndex list for each bones
    for(int i = 0; i <bonesList.size(); ++i){
        bonesList[i]->generateChildsIndex(bonesList);
    }

    vertices = verticesList;
    indices = indicesList;
    bones = bonesList;
    textureFileName = textureName;
    animations = animationsList;










    /*
    std::cout<< "Nombre de frame : " << animationsList[0]->getFrameNumber()<< std::endl;
    std::cout<< "Nombre de frame par seconde : " << animationsList[0]->getFramePerSecond()<< std::endl;
    std::cout<< "Nombre de keyFrame : " << animationsList[0]->getKeyFramesList().size()<< std::endl;
    for(int i =0 ; i< animationsList[0]->getKeyFramesList().size(); ++i){
         std::cout<< "KeyFrame n° " << i << std::endl << animationsList[0]->getKeyFramesList()[i]->getTimeStamp() << std::endl;
         std::cout<<"Nombre de bonetransform :" << animationsList[0]->getKeyFramesList()[i]->getBoneTransforms().size() <<std::endl;
         for(int j=0; j<animationsList[0]->getKeyFramesList()[i]->getBoneTransforms().size();++j){
             std::cout<< "BoneTransform n° " << j << std::endl << "os : " << animationsList[0]->getKeyFramesList()[i]->getBoneTransforms()[j]->getBone()->getName().toStdString() << std::endl <<
                         "position : (" << animationsList[0]->getKeyFramesList()[i]->getBoneTransforms()[j]->getPosition().x()<<
                         " ; "<< animationsList[0]->getKeyFramesList()[i]->getBoneTransforms()[j]->getPosition().y()<<" ; "<<
                         animationsList[0]->getKeyFramesList()[i]->getBoneTransforms()[j]->getPosition().z()<<") "<<std::endl <<
                         "rotation : (" << animationsList[0]->getKeyFramesList()[i]->getBoneTransforms()[j]->getRotation().scalar()<<
                         " ; " << animationsList[0]->getKeyFramesList()[i]->getBoneTransforms()[j]->getRotation().x()<<
                         " ; " << animationsList[0]->getKeyFramesList()[i]->getBoneTransforms()[j]->getRotation().y()<<
                         " ; " << animationsList[0]->getKeyFramesList()[i]->getBoneTransforms()[j]->getRotation().z()
                      <<std::endl;
         }
    }
    */

}


QVector<QMatrix4x4> AnimatedModel::getTransformationsAtTime(double time){
    QVector<QMatrix4x4> transformationList;
    transformationList.resize(bones.size());

    double framePerSeconde = animations[0]->getFramePerSecond() != 0 ?  animations[0]->getFramePerSecond() : 30.0f;
    double animationTime = fmod(time * framePerSeconde, animations[0]->getFrameNumber());

    QMatrix4x4 identity;
    identity.setToIdentity();


    calculateBonesTransformations(animationTime, transformationList, identity, scene->mRootNode );

std::cout<<"---------------------------"<<std::endl;

    return transformationList;
}

void AnimatedModel::calculateBonesTransformations(double time, QVector<QMatrix4x4> &transformationList, QMatrix4x4 parentTransformation, aiNode* node){

    // TODO : INTERPOLATION



    QMatrix4x4 nodeTransform = QMatrix4x4(node->mTransformation.a1, node->mTransformation.a2, node->mTransformation.a3 ,node->mTransformation.a4,
                                          node->mTransformation.b1, node->mTransformation.b2, node->mTransformation.b3 ,node->mTransformation.b4,
                                          node->mTransformation.c1, node->mTransformation.c2, node->mTransformation.c3 ,node->mTransformation.c4,
                                          node->mTransformation.d1, node->mTransformation.d2, node->mTransformation.d3 ,node->mTransformation.d4);


    for(int i = 0; i<animations[0]->getKeyFramesList()[0]->getBoneTransforms().size(); ++i){
        if(animations[0]->getKeyFramesList()[0]->getBoneTransforms()[i]->getBone()->getName() == QString(node->mName.data)){

            QMatrix4x4 T = QMatrix4x4(1,0,0,animations[0]->getKeyFramesList()[0]->getBoneTransforms()[i]->getPosition().x(),
                                      0,1,0,animations[0]->getKeyFramesList()[0]->getBoneTransforms()[i]->getPosition().y(),
                                      0,0,1,animations[0]->getKeyFramesList()[0]->getBoneTransforms()[i]->getPosition().z(),
                                      0,0,0,1) ;

            QMatrix4x4 R = QMatrix4x4(animations[0]->getKeyFramesList()[0]->getBoneTransforms()[i]->getRotation().toRotationMatrix()) ;

            QMatrix4x4 S = QMatrix4x4(animations[0]->getKeyFramesList()[0]->getBoneTransforms()[i]->getScaling().x(), 0 ,0, 0,
                                      0, animations[0]->getKeyFramesList()[0]->getBoneTransforms()[i]->getScaling().y(), 0, 0,
                                      0, 0, animations[0]->getKeyFramesList()[0]->getBoneTransforms()[i]->getScaling().z(), 0,
                                      0, 0, 0, 1);

            //nodeTransform = T * R * S;
        }
    }


    QMatrix4x4 transformation =  parentTransformation * nodeTransform;

    for(unsigned int i = 0; i<bones.size(); ++i){
        if(bones[i]->getName() == QString(node->mName.data)){
            transformationList[i] = transformation * bones[i]->getOffset() ;
            std::cout<< "OOOOOOOOOOOOOOOOOOOOOOOOOOOOOOO"<< std::endl;

            std::cout<< "transformation ["<< node->mName.data <<"]"<< " : index["<<i<<"]" << std::endl;
            std::cout<< "transformation ["<< i <<"]"<< " :"<< std::endl;
            std::cout<< "("<<transformationList[i].row(0).x() << " ; " <<transformationList[i].row(0).y()<< " ; " << transformationList[i].row(0).z()<< " ; "<< transformationList[i].row(0).w()<< ")"<< std::endl;
            std::cout<< "("<<transformationList[i].row(1).x() << " ; " <<transformationList[i].row(1).y()<< " ; " <<transformationList[i].row(1).z()<< " ; "<< transformationList[i].row(1).w()<< ")"<< std::endl;
            std::cout<< "("<<transformationList[i].row(2).x() << " ; " << transformationList[i].row(2).y()<< " ; " << transformationList[i].row(2).z()<< " ; "<< transformationList[i].row(2).w()<< ")"<< std::endl;
            std::cout<< "("<<transformationList[i].row(3).x() << " ; " << transformationList[i].row(3).y()<< " ; " << transformationList[i].row(3).z()<< " ; "<< transformationList[i].row(3).w()<< ")"<< std::endl;
            std::cout<<"YYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYY"<< std::endl;
        }
    }


    std::cout<< "transformation ["<< node->mName.data <<"]"<< " :"<< std::endl;
    std::cout<< "("<<transformation.row(0).x() << " ; " <<transformation.row(0).y()<< " ; " << transformation.row(0).z()<< " ; "<< transformation.row(0).w()<< ")"<< std::endl;
    std::cout<< "("<<transformation.row(1).x() << " ; " <<transformation.row(1).y()<< " ; " <<transformation.row(1).z()<< " ; "<< transformation.row(1).w()<< ")"<< std::endl;
    std::cout<< "("<<transformation.row(2).x() << " ; " << transformation.row(2).y()<< " ; " << transformation.row(2).z()<< " ; "<< transformation.row(2).w()<< ")"<< std::endl;
    std::cout<< "("<<transformation.row(3).x() << " ; " << transformation.row(3).y()<< " ; " << transformation.row(3).z()<< " ; "<< transformation.row(3).w()<< ")"<< std::endl;
/*
    for(int i = 0; i< bones[currentBoneIndex]->getChildsIndex().size(); ++i){
        calculateBonesTransformations(time, transformationList, transformation, bones[currentBoneIndex]->getChildsIndex()[i]);
    }
*/

    for (unsigned int i = 0 ; i < node->mNumChildren ; i++) {

        calculateBonesTransformations(time, transformationList, transformation, node->mChildren[i]);

    }

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

QVector<Animation*> AnimatedModel::getAnimations(){
    return animations;
}
