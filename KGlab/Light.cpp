#include "Light.h"

void Light::SetPosition(GLdouble x, GLdouble y, GLdouble z)
{
    m_posX = x;
    m_posY = y;
    m_posZ = z;
}

void Light::ApplyLight()
{
    GLfloat lamb[] = {0.2f, 0.2f, 0.2f, 0.0f};
    GLfloat ldif[] = {0.7f, 0.7f, 0.7f, 0.0f};
    GLfloat lspec[] = {1.0f, 1.0f, 1.0f, 0.0f};
    GLfloat lposition[] = {(GLfloat)m_posX, (GLfloat)m_posY, (GLfloat)m_posZ, 1.f};

    glLightfv(GL_LIGHT0, GL_POSITION, lposition);
    glLightfv(GL_LIGHT0, GL_AMBIENT, lamb);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, ldif);
    glLightfv(GL_LIGHT0, GL_SPECULAR, lspec);
    glEnable(GL_LIGHT0);
}

void Light::DrawLightGizmo()
{
    GLfloat pointSize;
    glGetFloatv(GL_POINT_SIZE, &pointSize);
    glPointSize(10);

    GLboolean texture_mode, depth_mode, lightning_mode;
    glGetBooleanv(GL_DEPTH_TEST, &depth_mode);
    glGetBooleanv(GL_TEXTURE_2D, &texture_mode);
    glGetBooleanv(GL_LIGHTING, &lightning_mode);

    glDisable(GL_DEPTH_TEST);
    glDisable(GL_TEXTURE_2D);
    glDisable(GL_LIGHTING);

    glBegin(GL_POINTS);
    glColor3d(1, 0.7, 0.1);
    glVertex3d(m_posX, m_posY, m_posZ);
    glEnd();

    glPointSize(pointSize);

    if (depth_mode) glEnable(GL_DEPTH_TEST);
    if (texture_mode) glEnable(GL_TEXTURE_2D);
    if (lightning_mode) glEnable(GL_LIGHTING);
}
