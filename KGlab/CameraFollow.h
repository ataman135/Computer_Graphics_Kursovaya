#pragma once

#include "Vector3.h"

class Car;

class CameraFollow
{
    Vector3 m_position;
    Vector3 m_target;
    double m_distance;
    double m_height;
    double m_smoothness;

public:
    CameraFollow();

    void update(const Car* car, double deltaTime);
    void applyCamera();

    void setDistance(double distance) { m_distance = distance; }
    void setHeight(double height) { m_height = height; }
};
