/**
 * @file   CamtransCamera.cpp
 *
 * This is the perspective camera class you will need to fill in for the Camtrans lab.  See the
 * lab handout for more details.
 */

#include "CamtransCamera.h"
#include <Settings.h>

#include <iostream>

CamtransCamera::CamtransCamera(): // might need to set defaults for variables here
    m_near(1.0f),
    m_far(30.0f),
    m_thetaH(60.0f),
    m_aspectRatio(1.0f), // DEBUG: lab says aspect ration 1:1
    m_eye(glm::vec4(2.0f, 2.0f, 2.0f, 1.0f)), // DEBUG: we want (2,2,2) in world coords, is eye in world co0rds?
    m_look(glm::vec4(0.0f, 0.0f, 0.0f, 1.0f)), // camera is looking at origin
    m_up(glm::vec4(0.0f, 1.0f, 0.0f, 1.0f))
{

    orientLook(
                    glm::vec4(2.f, 2.f, 2.f, 1.f),
                    glm::vec4(-1.f, -1.f, -1.f, 0.f),
                    glm::vec4(0.f, 1.f, 0.f, 0.f)
    );
}

glm::mat4x4 CamtransCamera::getProjectionMatrix() const {
    // @TODO: [CAMTRANS] Fill this in...
    return m_perspectiveTransformation * m_scaleMatrix;
}

glm::mat4x4 CamtransCamera::getViewMatrix() const {
    // @TODO: [CAMTRANS] Fill this in...
    return m_rotationMatrix * m_translationMatrix; // from lecture slides
}

glm::mat4x4 CamtransCamera::getScaleMatrix() const {
    // @TODO: [CAMTRANS] Fill this in...
    return m_scaleMatrix;
}

glm::mat4x4 CamtransCamera::getPerspectiveMatrix() const {
    // @TODO: [CAMTRANS] Fill this in...
    return m_perspectiveTransformation;
}

glm::vec4 CamtransCamera::getPosition() const {
    return m_eye;
}

glm::vec4 CamtransCamera::getLook() const {
    return -1.0f * m_w; // this would be like a weird camera space type thing
    //gotta include rotation in this vector
    //from lecture slides: w = - look / distance(look)
}

glm::vec4 CamtransCamera::getU() const {
    return m_u;
}

glm::vec4 CamtransCamera::getV() const {
    return m_v;
}

glm::vec4 CamtransCamera::getW() const {
    return m_w;
}

glm::vec4 CamtransCamera::getUp() const {
    return m_up;
}

float CamtransCamera::getAspectRatio() const {
    return m_aspectRatio;
}

float CamtransCamera::getHeightAngle() const {
    return m_thetaH;
}





void CamtransCamera::orientLook(const glm::vec4 &eye, const glm::vec4 &look, const glm::vec4 &up) {
    m_eye = eye;
    m_w = -1.0f * glm::normalize(look);

    // getting the good components out of the messy up vector
    m_v = up - (glm::dot( up, m_w ) * m_w);
    m_v = glm::normalize(m_v);

    // perpendicular to w and v
    m_u = glm::vec4( glm::cross( glm::vec3(m_v.x, m_v.y, m_v.z), glm::vec3(m_w.x, m_w.y, m_w.z) ), 0.0f );


    //from lab: then call
    updateViewMatrix();
    updateProjectionMatrix();
}

void CamtransCamera::setHeightAngle(float h) { //h is in degrees!
//    m_thetaH = h;

//    float temp_h = m_far * tan(m_thetaH/ 2.0f);
//    float temp_w = m_aspectRatio * temp_h;

//    m_thetaW = glm::atan(temp_w / m_far) * 2.0f; // DEBUG: do I need to update aspect ratio?

//    updateProjectionMatrix();

    m_thetaH = glm::radians(h);
    m_thetaW = 2.0f * glm::atan(m_aspectRatio * glm::tan(m_thetaH / 2.0f));
    updateProjectionMatrix();
}



void CamtransCamera::setAspectRatio(float a)
{
    m_aspectRatio = a;
    updateProjectionMatrix();
}

void CamtransCamera::translate(const glm::vec4 &v) {
    m_eye = v + m_eye;
    this->updateViewMatrix();
}

void CamtransCamera::rotateU(float degrees) { // in degrees!
    float d = glm::radians(degrees);
    glm::vec4 temp_v = m_v;
    m_v = m_w * sin(d) + m_v * cos(d);
    m_w = temp_v * -1.0f * sin(d) + m_w * cos(d);
    this->updateViewMatrix();
}

void CamtransCamera::rotateV(float degrees) { // issue here

    float d = glm::radians(degrees);
    glm::vec4 temp_u = m_u;
    m_u = - m_w * sin(d) + m_u * cos(d);
    m_w = temp_u * 1.0f * sin(d) + m_w * cos(d);
    this->updateViewMatrix();
}

void CamtransCamera::rotateW(float degrees) {
    float d = glm::radians(degrees);
    glm::vec4 v = m_v * glm::cos(d) - m_u * glm::sin(d);
    glm::vec4 u = m_v * glm::sin(d) + m_u * glm::cos(d);
    m_v = v;
    m_u = u;
    this->updateViewMatrix();
}

void CamtransCamera::setClip(float nearPlane, float farPlane) {
    m_near = nearPlane;
    m_far = farPlane;
    updateProjectionMatrix();
}












void CamtransCamera::updateProjectionMatrix() {
    // from what I understand: projection matrix takes points in world space
    // and outputs points on screen. (1/z or something)

    // from Lab handout: there are no projection and view matrix
    // member variables, so in updateProjectionMatrix(),
    // call the update methods of mats that they are composed of.
    // Projection matrix is composed of Scale and Perspective

    updateScaleMatrix();
    updatePerspectiveMatrix();
}

void CamtransCamera::updatePerspectiveMatrix() {

    float c = -1.0f * m_near / m_far;
    m_perspectiveTransformation = glm::mat4x4(1.0f, 0.0f, 0.0f, 0.0f,
                                              0.0f, 1.0f, 0.0f, 0.0f,
                                              0.0f, 0.0f, (-1.0f / (1.0f + c)), -1.0f,
                                              0.0f, 0.0f, (c / (1.0f + c)), 0.0f
                                              );
}

void CamtransCamera::updateScaleMatrix() {

    float overFar = (1.0f / m_far);

    float overFarTanHeight  = overFar /  tan(m_thetaH/ 2.0f);
    float overFarTanWidth = overFarTanHeight / m_aspectRatio;


    m_scaleMatrix = glm::mat4(overFarTanWidth, 0.0f            , 0.0f   , 0.0f,
                              0.0f           , overFarTanHeight, 0.0f   , 0.0f,
                              0.0f           , 0.0f            , overFar, 0.0f,
                              0.0f           , 0.0f            , 0.0f   , 1.0f
                              );
}

void CamtransCamera::updateViewMatrix() { // Position of camera in space

    // from slides: final matrix use to transform into camera
    // coordinate space is m_rotate dot m_translate

    // from Lab handout: there are no projection and view matrix
    // member variables in stencil code, so in updateViewMatrix(),
    // call the update methods of mats that they are composed of.

    updateRotationMatrix();
    updateTranslationMatrix();

}

void CamtransCamera::updateRotationMatrix() {
    // [u_x, u_y, u_z, 0,
    //  v_x, v_y, v_z, 0,
    //  w_x, w_y, w_z, 0,
    //  0, 0, 0,  1 ]              rotationMatrix
    m_rotationMatrix = glm::transpose( glm::mat4( m_u.x, m_u.y, m_u.z, 0.0f,
                                  m_v.x, m_v.y, m_v.z, 0.0f,
                                  m_w.x, m_w.y, m_w.z, 0.0f,
                                  0.0f , 0.0f , 0.0f , 1.0f
                                  ));
}


void CamtransCamera::updateTranslationMatrix() {
    // [1, 0, 0, -Px,
    //  0, 1, 0, -Py,
    //  0, 0, 1, -Pz
    //  0, 0, 0,  1 ]               translation Matrix, given camera location P

    m_translationMatrix = glm::transpose(glm::mat4(1.0f, 0.0f, 0.0f, -m_eye.x,
                                    0.0f, 1.0f, 0.0f, -m_eye.y,
                                    0.0f, 0.0f, 1.0f, -m_eye.z,
                                    0.0f, 0.0f, 0.0f, 1.0f
                                    ));

}
