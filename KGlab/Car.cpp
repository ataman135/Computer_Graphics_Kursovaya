#include "Car.h"
#include "Track.h"
#include <cmath>
#include <iostream>
#define GLFW_INCLUDE_GLU
#include <GLFW/glfw3.h>

#define STB_IMAGE_IMPLEMENTATION
#include "include/stb_image.h"

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

extern std::string data_folder;

Car::Car(ControlScheme scheme, Vector3 color)
{
    m_position = Vector3(20.0, 0.0, 0.5);
    m_angle = M_PI / 2.0;
    m_speed = 0.0;
    m_steeringAngle = 0.0;

    m_acceleration = 15.0;
    m_maxSpeed = 25.0;
    m_friction = 5.0;
    m_turnSpeed = 2.5;

    m_currentCheckpoint = 0;
    m_lapCount = 0;

    m_lastValidPosition = m_position;
    m_lastValidAngle = m_angle;

    m_controlScheme = scheme;
    m_color = color;

    m_boostTimer = 0.0;
    m_boostMultiplier = 1.5;
    m_isBoosted = false;

    m_textureId = 0;
    m_useModel = false;

    std::string modelPath = data_folder + "Car.obj";
    int result = 0;
    try {
        result = m_carModel.LoadModel(modelPath.c_str());
    }
    catch (...) {
        result = 0;
    }

    if (result == 1)
    {
        m_useModel = true;
        std::string texturePath = data_folder + "CarTexture.png";
        loadTexture(texturePath.c_str());
        std::cout << "Car model loaded successfully" << std::endl;
    }
    else
    {
        std::cout << "Failed to load car model, using primitive" << std::endl;
    }
}

Car::~Car()
{
    if (m_textureId != 0)
    {
        glDeleteTextures(1, &m_textureId);
    }
}

void Car::loadTexture(const char* filename)
{
    int width, height, channels;
    unsigned char* data = stbi_load(filename, &width, &height, &channels, 0);

    if (data)
    {
        glGenTextures(1, &m_textureId);
        glBindTexture(GL_TEXTURE_2D, m_textureId);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        GLenum format = (channels == 4) ? GL_RGBA : GL_RGB;
        glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);

        stbi_image_free(data);
        std::cout << "Texture loaded: " << filename << " (" << width << "x" << height << ")" << std::endl;
    }
    else
    {
        std::cout << "Failed to load texture: " << filename << std::endl;
    }
}

void Car::reset(const Vector3& startPos, double startAngle)
{
    m_position = startPos;
    m_angle = startAngle;
    m_speed = 0.0;
    m_steeringAngle = 0.0;
    m_currentCheckpoint = 0;
    m_lapCount = 0;
    m_lastValidPosition = m_position;
    m_lastValidAngle = m_angle;
}

void Car::handleInput(OpenGL* ogl, double deltaTime)
{
    bool accelerating = false;

    int keyUp, keyDown, keyLeft, keyRight;

    if (m_controlScheme == ControlScheme::WASD)
    {
        keyUp = GLFW_KEY_W;
        keyDown = GLFW_KEY_S;
        keyLeft = GLFW_KEY_A;
        keyRight = GLFW_KEY_D;
    }
    else
    {
        keyUp = GLFW_KEY_UP;
        keyDown = GLFW_KEY_DOWN;
        keyLeft = GLFW_KEY_LEFT;
        keyRight = GLFW_KEY_RIGHT;
    }

    if (ogl->isKeyPressed(keyUp))
    {
        m_speed += m_acceleration * deltaTime;
        accelerating = true;
    }

    if (ogl->isKeyPressed(keyDown))
    {
        m_speed -= m_acceleration * deltaTime * 0.7;
        accelerating = true;
    }

    if (!accelerating)
    {
        if (m_speed > 0)
        {
            m_speed -= m_friction * deltaTime;
            if (m_speed < 0) m_speed = 0;
        }
        else if (m_speed < 0)
        {
            m_speed += m_friction * deltaTime;
            if (m_speed > 0) m_speed = 0;
        }
    }

    if (m_speed > m_maxSpeed) m_speed = m_maxSpeed;
    if (m_speed < -m_maxSpeed * 0.5) m_speed = -m_maxSpeed * 0.5;

    if (fabs(m_speed) > 0.1)
    {
        if (ogl->isKeyPressed(keyLeft))
        {
            double turnFactor = m_speed / m_maxSpeed;
            m_angle += m_turnSpeed * turnFactor * deltaTime;
        }

        if (ogl->isKeyPressed(keyRight))
        {
            double turnFactor = m_speed / m_maxSpeed;
            m_angle -= m_turnSpeed * turnFactor * deltaTime;
        }
    }
}

void Car::updatePhysics(double deltaTime)
{
    double dx = cos(m_angle) * m_speed * deltaTime;
    double dy = sin(m_angle) * m_speed * deltaTime;

    m_position = m_position + Vector3(dx, dy, 0.0);
}

bool Car::checkCollision(const Car* other) const
{
    if (!other) return false;

    double dx = m_position.x() - other->m_position.x();
    double dy = m_position.y() - other->m_position.y();
    double distance = sqrt(dx * dx + dy * dy);

    return distance < 2.0;
}

void Car::handleCollision(const Car* other)
{
    if (!other || !checkCollision(other)) return;

    Vector3 collisionNormal = (m_position - other->getPosition()).normalize();
    m_position = m_position + collisionNormal * 0.1;
    m_speed *= 0.5;
}

void Car::checkBoundaries(const Track* track)
{
    if (track->isOnTrack(m_position.x(), m_position.y(), 0.5))
    {
        m_lastValidPosition = m_position;
        m_lastValidAngle = m_angle;

        int newCheckpoint = track->checkLapProgress(m_position.x(), m_position.y(), m_currentCheckpoint);
        if (newCheckpoint != m_currentCheckpoint)
        {
            m_currentCheckpoint = newCheckpoint;
            if (m_currentCheckpoint == 0)
            {
                m_lapCount++;
            }
        }
    }
    else
    {
        m_position = m_lastValidPosition;
        m_angle = m_lastValidAngle;
        m_speed = 0.0;
    }
}

void Car::update(double deltaTime, OpenGL* ogl, const Track* track, const Car* otherCar)
{
    if (m_isBoosted)
    {
        m_boostTimer -= deltaTime;
        if (m_boostTimer <= 0.0)
        {
            m_isBoosted = false;
            m_boostTimer = 0.0;
        }
    }

    handleInput(ogl, deltaTime);
    updatePhysics(deltaTime);
    checkBoundaries(track);
    handleCollision(otherCar);
}

void Car::applySpeedBoost()
{
    m_isBoosted = true;
    m_boostTimer = 3.0;
    m_speed *= m_boostMultiplier;
    if (m_speed > m_maxSpeed * m_boostMultiplier)
    {
        m_speed = m_maxSpeed * m_boostMultiplier;
    }
}

void Car::drawShadow()
{
    glDisable(GL_LIGHTING);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glPushMatrix();
    glTranslatef(m_position.x(), m_position.y(), 0.02f);
    glRotatef(m_angle * 180.0 / M_PI - 90.0, 0, 0, 1);

    glBegin(GL_TRIANGLE_FAN);

    glColor4f(0.0f, 0.0f, 0.0f, 0.4f);
    glVertex3f(0.0f, 0.0f, 0.0f);

    int numSegments = 16;
    float shadowLength = 1.3f;
    float shadowWidth = 0.65f;

    for (int i = 0; i <= numSegments; ++i)
    {
        float angle = i * (2.0f * M_PI / numSegments);

        glColor4f(0.0f, 0.0f, 0.0f, 0.0f);

        glVertex3f(
            cos(angle) * shadowWidth,
            sin(angle) * shadowLength,
            0.0f
        );
    }
    glEnd();

    glPopMatrix();

    glDisable(GL_BLEND);
    glEnable(GL_LIGHTING);

    glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
}

void Car::drawPrimitive()
{
    glColor3f(m_color.x(), m_color.y(), m_color.z());
    glBegin(GL_QUADS);
    glNormal3f(0, 0, 1);
    glVertex3f(-0.5, -1.0, 0);
    glVertex3f(0.5, -1.0, 0);
    glVertex3f(0.5, 1.0, 0);
    glVertex3f(-0.5, 1.0, 0);

    glNormal3f(0, 0, -1);
    glVertex3f(-0.5, -1.0, 0.3);
    glVertex3f(-0.5, -1.0, 0);
    glVertex3f(0.5, -1.0, 0);
    glVertex3f(0.5, -1.0, 0.3);

    glVertex3f(0.5, -1.0, 0.3);
    glVertex3f(0.5, -1.0, 0);
    glVertex3f(0.5, 1.0, 0);
    glVertex3f(0.5, 1.0, 0.3);

    glVertex3f(0.5, 1.0, 0.3);
    glVertex3f(0.5, 1.0, 0);
    glVertex3f(-0.5, 1.0, 0);
    glVertex3f(-0.5, 1.0, 0.3);

    glVertex3f(-0.5, 1.0, 0.3);
    glVertex3f(-0.5, 1.0, 0);
    glVertex3f(-0.5, -1.0, 0);
    glVertex3f(-0.5, -1.0, 0.3);

    glNormal3f(0, 0, 1);
    glVertex3f(-0.5, -1.0, 0.3);
    glVertex3f(0.5, -1.0, 0.3);
    glVertex3f(0.5, 1.0, 0.3);
    glVertex3f(-0.5, 1.0, 0.3);
    glEnd();

    glColor3f(0.1f, 0.1f, 0.1f);
    glPushMatrix();
    glTranslatef(-0.4, -0.6, 0);
    glScalef(0.2, 0.3, 0.3);
    GLUquadric* quad = gluNewQuadric();
    gluSphere(quad, 1.0, 8, 8);
    gluDeleteQuadric(quad);
    glPopMatrix();

    glPushMatrix();
    glTranslatef(0.4, -0.6, 0);
    glScalef(0.2, 0.3, 0.3);
    quad = gluNewQuadric();
    gluSphere(quad, 1.0, 8, 8);
    gluDeleteQuadric(quad);
    glPopMatrix();

    glPushMatrix();
    glTranslatef(-0.4, 0.6, 0);
    glScalef(0.2, 0.3, 0.3);
    quad = gluNewQuadric();
    gluSphere(quad, 1.0, 8, 8);
    gluDeleteQuadric(quad);
    glPopMatrix();

    glPushMatrix();
    glTranslatef(0.4, 0.6, 0);
    glScalef(0.2, 0.3, 0.3);
    quad = gluNewQuadric();
    gluSphere(quad, 1.0, 8, 8);
    gluDeleteQuadric(quad);
    glPopMatrix();
}

void Car::draw()
{
    drawShadow();

    glPushMatrix();
    glTranslatef(m_position.x(), m_position.y(), m_position.z() - 0.3);
    glRotatef(270, 0, 0, 1);
    glRotatef(m_angle * 180.0 / M_PI - 90, 0, 0, 1);

    if (m_useModel)
    {
        glEnable(GL_TEXTURE_2D);
        if (m_textureId != 0)
        {
            glBindTexture(GL_TEXTURE_2D, m_textureId);
        }

        glColor3f(1.0f, 1.0f, 1.0f);

        glPushMatrix();
        glScaled(0.005, 0.005, 0.005);
        m_carModel.Draw();
        glPopMatrix();

        glDisable(GL_TEXTURE_2D);
    }
    else
    {
        drawPrimitive();
    }

    glPopMatrix();
}
