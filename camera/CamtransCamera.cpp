/**
 * @file   CamtransCamera.cpp
 *
 * This is the perspective camera class you will need to fill in for the Camtrans lab.  See the
 * lab handout for more details.
 */

#include "CamtransCamera.h"
#include <Settings.h>
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtx/transform.hpp"

CamtransCamera::CamtransCamera() : m_aspectRatio(1.f), m_near(1.f), m_far(30.f), m_thetaH(glm::radians(60.f))
{
    CamtransCamera::orientLook(glm::vec4(2, 2, 2, 1), glm::vec4(-2, -2, -2, 0), glm::vec4(0, 1, 0, 0));
}

void CamtransCamera::setAspectRatio(float a)
{
    m_aspectRatio = a;
    updateProjectionMatrix();
}

glm::mat4x4 CamtransCamera::getProjectionMatrix() const {
    return m_perspectiveTransformation * m_scaleMatrix;
}

glm::mat4x4 CamtransCamera::getViewMatrix() const {
    return m_rotationMatrix * m_translationMatrix;
}

glm::mat4x4 CamtransCamera::getScaleMatrix() const {
    return m_scaleMatrix;
}

glm::mat4x4 CamtransCamera::getPerspectiveMatrix() const {
    return m_perspectiveTransformation;
}

glm::vec4 CamtransCamera::getPosition() const {
    return m_eye;
}

glm::vec4 CamtransCamera::getLook() const {
    return -m_w;
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

glm::vec4 CamtransCamera::getU() const
{
    return m_u;
}

glm::vec4 CamtransCamera::getV() const
{
    return m_v;
}

glm::vec4 CamtransCamera::getW() const
{
    return m_w;
}

void CamtransCamera::orientLook(const glm::vec4 &eye, const glm::vec4 &look, const glm::vec4 &up) {
    m_eye = eye;
    m_up = up;
    m_w = glm::vec4(glm::normalize(-glm::vec3(look)), 0.f);
    m_v = glm::vec4(glm::normalize(glm::vec3(up) - glm::dot(glm::vec3(up), glm::vec3(m_w)) * glm::vec3(m_w)), 0.f);
    m_u = glm::vec4(glm::normalize(glm::cross(glm::vec3(m_v), glm::vec3(m_w))), 0.f);

    updateViewMatrix();
    updateProjectionMatrix();
}

void CamtransCamera::setHeightAngle(float h) {
    m_thetaH = glm::radians(h);
    updateProjectionMatrix();
}

void CamtransCamera::translate(const glm::vec4 &v) {
    m_eye = glm::vec4(glm::vec3(m_eye) + glm::vec3(v), 1.f);
    updateViewMatrix();
}

void CamtransCamera::rotateU(float degrees) {
    glm::mat4 rotation = rotate(glm::vec3(m_u), glm::radians(degrees));
    m_v = rotation * m_v;
    m_w = rotation * m_w;
    m_up = rotation * m_up;
    updateViewMatrix();
}

void CamtransCamera::rotateV(float degrees) {
    glm::mat4 rotation = rotate(glm::vec3(m_v), glm::radians(degrees));
    m_u = rotation * m_u;
    m_w = rotation * m_w;
    m_up = rotation * m_up;
    updateViewMatrix();
}

void CamtransCamera::rotateW(float degrees) {
    glm::mat4 rotation = rotate(glm::vec3(m_w), glm::radians(degrees));
    m_v = rotation * m_v;
    m_u = rotation * m_u;
    m_up = rotation * m_up;
    updateViewMatrix();
}

void CamtransCamera::setClip(float nearPlane, float farPlane) {
    m_near = nearPlane;
    m_far = farPlane;
    updateProjectionMatrix();
}

void CamtransCamera::updateProjectionMatrix()
{
    updatePerspectiveMatrix();
    updateScaleMatrix();
}

void CamtransCamera::updatePerspectiveMatrix()
{
    float c = - m_near / m_far;
    m_perspectiveTransformation = glm::mat4(1, 0, 0, 0,
                                            0, 1, 0, 0,
                                            0, 0, -1.f / (1.f + c), -1,
                                            0, 0, c / (1.f + c), 0);
}

void CamtransCamera::updateScaleMatrix()
{
    float tanH = glm::tan(m_thetaH / 2.f);
    float tanW = m_aspectRatio * tanH;
    m_scaleMatrix = glm::mat4(1.f / (m_far * tanW), 0, 0, 0,
                              0, 1.f / (m_far * tanH), 0, 0,
                              0, 0, 1.f / m_far, 0,
                              0, 0, 0, 1);
}

void CamtransCamera::updateViewMatrix()
{
    updateTranslationMatrix();
    updateRotationMatrix();
}

void CamtransCamera::updateRotationMatrix()
{
    m_rotationMatrix = glm::mat4(m_u.x, m_v.x, m_w.x, 0,
                                 m_u.y, m_v.y, m_w.y, 0,
                                 m_u.z, m_v.z, m_w.z, 0,
                                 0, 0, 0, 1);
}

void CamtransCamera::updateTranslationMatrix()
{
    m_translationMatrix = glm::mat4(1, 0, 0, 0,
                                    0, 1, 0, 0,
                                    0, 0, 1, 0,
                                    -m_eye.x, -m_eye.y, -m_eye.z, 1);
}

glm::mat4 CamtransCamera::rotate(glm::vec3 axis, float angle)
{
    float cosine = glm::cos(angle);
    float sine = glm::sin(angle);

    glm::vec3 temp = (1.f - cosine) * axis;

    glm::mat4 rotation(1.f);
    rotation[0][0] = cosine + temp[0] * axis[0];
    rotation[0][1] = temp[0] * axis[1] + sine * axis[2];
    rotation[0][2] = temp[0] * axis[2] - sine * axis[1];

    rotation[1][0] = temp[1] * axis[0] - sine * axis[2];
    rotation[1][1] = cosine + temp[1] * axis[1];
    rotation[1][2] = temp[1] * axis[2] + sine * axis[0];

    rotation[2][0] = temp[2] * axis[0] + sine * axis[1];
    rotation[2][1] = temp[2] * axis[1] - sine * axis[0];
    rotation[2][2] = cosine + temp[2] * axis[2];
    return rotation;
}

