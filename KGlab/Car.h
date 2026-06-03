#pragma once

#include "Vector3.h"
#include "OpenGL.h"
#include "ObjLoader.h"

class Track;
class Car;

enum class ControlScheme
{
    WASD,
    ARROWS
};

class Car
{
    Vector3 m_position;
    double m_angle;
    double m_speed;
    double m_steeringAngle;

    double m_acceleration;
    double m_maxSpeed;
    double m_friction;
    double m_turnSpeed;

    int m_currentCheckpoint;
    int m_lapCount;

    Vector3 m_lastValidPosition;
    double m_lastValidAngle;

    ControlScheme m_controlScheme;
    Vector3 m_color;

    double m_boostTimer;
    double m_boostMultiplier;
    bool m_isBoosted;

    ObjModel m_carModel;
    GLuint m_textureId;
    bool m_useModel;

public:
    Car(ControlScheme scheme = ControlScheme::WASD, Vector3 color = Vector3(0.8, 0.1, 0.1));
    ~Car();

    void reset(const Vector3& startPos, double startAngle);
    void update(double deltaTime, OpenGL* ogl, const Track* track, const Car* otherCar = nullptr);
    void draw();

    Vector3 getPosition() const { return m_position; }
    double getAngle() const { return m_angle; }
    double getSpeed() const { return m_speed; }
    int getLapCount() const { return m_lapCount; }
    Vector3 getColor() const { return m_color; }
    bool isBoosted() const { return m_isBoosted; }

    bool checkCollision(const Car* other) const;
    void applySpeedBoost();

private:
    void handleInput(OpenGL* ogl, double deltaTime);
    void updatePhysics(double deltaTime);
    void checkBoundaries(const Track* track);
    void handleCollision(const Car* other);
    void drawShadow();
    void drawPrimitive();
    void loadTexture(const char* filename);
};
