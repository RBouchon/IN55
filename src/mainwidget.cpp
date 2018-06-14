/****************************************************************************
**
** Copyright (C) 2016 The Qt Company Ltd.
** Contact: https://www.qt.io/licensing/
**
** This file is part of the QtCore module of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:BSD$
** Commercial License Usage
** Licensees holding valid commercial Qt licenses may use this file in
** accordance with the commercial license agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and The Qt Company. For licensing terms
** and conditions see https://www.qt.io/terms-conditions. For further
** information use the contact form at https://www.qt.io/contact-us.
**
** BSD License Usage
** Alternatively, you may use this file under the terms of the BSD license
** as follows:
**
** "Redistribution and use in source and binary forms, with or without
** modification, are permitted provided that the following conditions are
** met:
**   * Redistributions of source code must retain the above copyright
**     notice, this list of conditions and the following disclaimer.
**   * Redistributions in binary form must reproduce the above copyright
**     notice, this list of conditions and the following disclaimer in
**     the documentation and/or other materials provided with the
**     distribution.
**   * Neither the name of The Qt Company Ltd nor the names of its
**     contributors may be used to endorse or promote products derived
**     from this software without specific prior written permission.
**
**
** THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
** "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
** LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
** A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
** OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
** SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
** LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
** DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
** THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
** (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
** OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE."
**
** $QT_END_LICENSE$
**
****************************************************************************/

#include "mainwidget.h"
#include "camera.h"
#include "iostream"

#include "animatedmodel/animatedmodel.h"

#include <QMatrix4x4>
#include <QString>
#include <QOpenGLTexture>


#include <QFileDialog>
#include <QMouseEvent>
#include <math.h>
#include <chrono>

MainWidget::MainWidget(QWidget *parent) :
    QOpenGLWidget(parent),
    geometries(0),
    texture(0),
    angularSpeed(0),
    frameNumber(0)
{
}

MainWidget::~MainWidget()
{
    // Make sure the context is current when deleting the texture
    // and the buffers.
    makeCurrent();
    delete geometries;
    delete texture;
    doneCurrent();
}

//! [0]
void MainWidget::mousePressEvent(QMouseEvent *e)
{
    // Save mouse press position
    mousePressPosition = QVector2D(e->localPos());
}

void MainWidget::keyPressEvent(QKeyEvent *event){
    switch ( event->key() )
        {
            case Qt::Key_F :
                cam.changeFocusMireille();
                break;
            case Qt::Key_Left :
                //Démarrer annimation gauche
                break;
            case Qt::Key_Right:
                //Démarrer annimation droite

                break;
            case Qt::Key_Down:
                //Démarrer annimation bas
        cam.dezoom(1.2);
                break;
            case Qt::Key_Up:
                //Démarrer annimation saut
        cam.zoom(1.2);
                break;

            case Qt::Key_Z:
                //Bouger cam en avant
                //cam.avancer();
            cam.orienterHaut(2);
                break;
            case Qt::Key_S:
                //Bouger cam en arriere
                //cam.reculer();
        cam.orienterBas(2);
                break;
    case Qt::Key_D:
        //Bouger cam en arriere
        cam.orienterDroite(2);
        break;
    case Qt::Key_Q:
        //Bouger cam en arriere
        cam.orienterGauche(2);
        break;

                break;
            default:
                break;
        }
}

void MainWidget::wheelEvent(QWheelEvent *event){

}

void MainWidget::mouseMoveEvent(QMouseEvent *event){

    //qInfo("mouseMove");
    // Récupération des angles
    QVector2D newMousePosition = QVector2D(event->localPos());
    QVector2D deltaVector =newMousePosition - mousePressPosition;
    cam.orienter(deltaVector.x(),deltaVector.y());
    //update();

}

void MainWidget::mouseReleaseEvent(QMouseEvent *e)
{
    /*
    qInfo("rota");
    // Mouse release position - mouse press position
    QVector2D diff = QVector2D(e->localPos()) - mousePressPosition;

    // Rotation axis is perpendicular to the mouse position difference
    // vector
    QVector3D n = QVector3D(diff.y(), diff.x(), 0.0).normalized();

    // Accelerate angular speed relative to the length of the mouse sweep
    qreal acc = diff.length() / 100.0;

    // Calculate new rotation axis as weighted sum
    rotationAxis = (rotationAxis * angularSpeed + n * acc).normalized();
    rotationAxis = QVector3D(0,1,0);
    // Increase angular speed
    angularSpeed += acc;*/
}
//! [0]

//! [1]
void MainWidget::timerEvent(QTimerEvent *)
{


    initBonesTransforms(bonesTransformationsList.at(frameNumber%FPS));
    ++frameNumber;
    update();


}
//! [1]

void MainWidget::initializeGL()
{


    initializeOpenGLFunctions();

    glClearColor(0, 0, 0, 1);

    initShaders();

//! [2]
    // Enable depth buffer
    glEnable(GL_DEPTH_TEST);

    // Enable back face culling
    glEnable(GL_CULL_FACE);
//! [2]


    QString fileName = QFileDialog::getOpenFileName(this,
        tr("Open file dae"), "./", tr("dae Files (*.dae)"));

    model = AnimatedModel(fileName);

    initTextures(model.getTextureFileName());


    for(unsigned int i = 0; i<FPS; ++i){
        bonesTransformationsList.append(model.getTransformationsAtTime((1.0/FPS)*i));
    }

    //initBonesTransforms(bonesTransformationsList.at(0));
    geometries = new GeometryEngine(model);

    // Use QBasicTimer because its faster than QTimer
    timer.start((int) 1000/FPS, this);


}

//! [3]
void MainWidget::initShaders()
{
    // Compile vertex shader
    if (!program.addShaderFromSourceFile(QOpenGLShader::Vertex, ":/vshader.glsl"))
        close();

    // Compile fragment shader
    if (!program.addShaderFromSourceFile(QOpenGLShader::Fragment, ":/fshader.glsl"))
        close();

    // Link shader pipeline
    if (!program.link())
        close();

    // Bind shader pipeline for use
    if (!program.bind())
        close();
}
//! [3]


void MainWidget::initTextures(QString textureFileName){

    texture = new QOpenGLTexture(QImage(textureFileName).mirrored());
    texture->setMinificationFilter(QOpenGLTexture::Nearest);
    texture->setMagnificationFilter(QOpenGLTexture::Linear);

}

void MainWidget::initBonesTransforms(QVector<QMatrix4x4> bonesTransforms){

    for(int i =0; i< bonesTransforms.size(); ++i){

        bonesTransformations[i] = bonesTransforms[i];

    }
}

//! [5]
void MainWidget::resizeGL(int w, int h)
{
    // Calculate aspect ratio
    qreal aspect = qreal(w) / qreal(h ? h : 1);

    // Set near plane to 3.0, far plane to 7.0, field of view 45 degrees
    const qreal zNear = 3.0, zFar = 1000.0, fov = 45.0;

    // Reset projection
    projection.setToIdentity();

    // Set perspective projection
    projection.perspective(fov, aspect, zNear, zFar);



}
//! [5]

void MainWidget::paintGL()
{
    // Clear color and depth buffer
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


    texture->bind();
    view.setToIdentity();
    view.lookAt(cam.getCam_position(),cam.getCam_pointcible(),cam.getCamUpVector());
//! [6]
    // Calculate model view transformation
    QMatrix4x4 matrix;
    matrix.translate(0.0, -100.0, 0.0);
    matrix.rotate(rotation);
    //view.rotate((rotation));


    // Set modelview-projection matrix
    program.setUniformValue("mvp",  projection* view * matrix);

    program.setUniformValue("texture", 0);

    program.setUniformValueArray("boneTransformations", bonesTransformations, 30);
//! [6]



    // Draw cube geometry
    geometries->drawGeometry(&program);
}
