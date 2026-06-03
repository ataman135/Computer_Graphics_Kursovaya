#pragma once

#include "OpenGL.h"

class Light
{
    GLdouble m_posX = 1;
    GLdouble m_posY = 1;
    GLdouble m_posZ = 1;

  public:
    void SetPosition(GLdouble x, GLdouble y, GLdouble z);
    void ApplyLight();
    void DrawLightGizmo();
};
