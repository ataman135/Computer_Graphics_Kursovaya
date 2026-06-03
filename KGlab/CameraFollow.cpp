#include "CameraFollow.h"
#include "Car.h"
#include <cmath>
#define GLFW_INCLUDE_GLU
#include <GLFW/glfw3.h>

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

CameraFollow::CameraFollow()
{
    m_position = Vector3(0, 0, 0);
    m_target = Vector3(0, 0, 0);
    m_distance = 8.0;
    m_height = 4.0;
    m_smoothness = 5.0;
}

void CameraFollow::update(const Car* car, double deltaTime)
{
    Vector3 carPos = car->getPosition();
    double carAngle = car->getAngle();

    Vector3 desiredPosition = carPos + Vector3(
        -cos(carAngle) * m_distance,
        -sin(carAngle) * m_distance,
        m_height
    );

    double lerpFactor = 1.0 - exp(-m_smoothness * deltaTime);
    m_position = m_position + (desiredPosition - m_position) * lerpFactor;

    Vector3 desiredTarget = carPos + Vector3(0, 0, 0.5);
    m_target = m_target + (desiredTarget - m_target) * lerpFactor;
}

void CameraFollow::applyCamera()
{
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    gluLookAt(
        m_position.x(), m_position.y(), m_position.z(),
        m_target.x(), m_target.y(), m_target.z(),
        0, 0, 1
    );
}
