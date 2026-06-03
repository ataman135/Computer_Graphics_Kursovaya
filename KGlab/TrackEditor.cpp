#include "TrackEditor.h"
#include <cmath>
#include <iostream>
#include <fstream>
#include <sstream>
#define GLFW_INCLUDE_GLU
#include <GLFW/glfw3.h>

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

extern std::string data_folder;

TrackEditor::TrackEditor(Track* track)
{
    m_track = track;
    m_cameraTarget = Vector3(0, 0, 0);
    m_cameraPitch = 45.0f;
    m_cameraYaw = 0.0f;
    m_cameraDistance = 60.0f;

    m_showGrid = true;
    m_gridSize = 5.0f;
    m_gridLines = 25;

    m_cursorPosition = Vector3(0, 0, 0);
    m_isDragging = false;
    m_lastMouseX = 0;
    m_lastMouseY = 0;
}

TrackEditor::~TrackEditor() {}

void TrackEditor::updateCamera(double deltaTime, OpenGL* ogl)
{
    GLFWwindow* window = ogl->getWindow();
    float moveSpeed = 25.0f * deltaTime;
    float rotateSpeed = 90.0f * deltaTime;

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
    {
        m_cameraDistance -= moveSpeed;
        if (m_cameraDistance < 5.0f) m_cameraDistance = 5.0f;
    }
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
    {
        m_cameraDistance += moveSpeed;
        if (m_cameraDistance > 200.0f) m_cameraDistance = 200.0f;
    }
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) m_cameraYaw += rotateSpeed;
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) m_cameraYaw -= rotateSpeed;

    if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS)
    {
        m_cameraPitch += rotateSpeed * 0.5f;
        if (m_cameraPitch > 85.0f) m_cameraPitch = 85.0f;
    }
    if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS)
    {
        m_cameraPitch -= rotateSpeed * 0.5f;
        if (m_cameraPitch < 10.0f) m_cameraPitch = 10.0f;
    }

    double xpos, ypos;
    glfwGetCursorPos(window, &xpos, &ypos);

    if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS)
    {
        if (!m_isDragging)
        {
            m_isDragging = true;
            m_lastMouseX = xpos;
            m_lastMouseY = ypos;
        }
        else
        {
            double dx = xpos - m_lastMouseX;
            double dy = ypos - m_lastMouseY;

            m_cameraYaw -= dx * 0.5f;
            m_cameraPitch -= dy * 0.5f;

            if (m_cameraPitch > 85.0f) m_cameraPitch = 85.0f;
            if (m_cameraPitch < 10.0f) m_cameraPitch = 10.0f;

            m_lastMouseX = xpos;
            m_lastMouseY = ypos;
        }
    }
    else
    {
        m_isDragging = false;
    }

    float yawRad = m_cameraYaw * M_PI / 180.0f;
    float pitchRad = m_cameraPitch * M_PI / 180.0f;

    m_cameraPosition = Vector3(
        m_cameraDistance * cos(pitchRad) * cos(yawRad),
        m_cameraDistance * cos(pitchRad) * sin(yawRad),
        m_cameraDistance * sin(pitchRad)
    );
}

void TrackEditor::update(double deltaTime, OpenGL* ogl)
{
    updateCamera(deltaTime, ogl);

    if (glfwGetKey(ogl->getWindow(), GLFW_KEY_T) == GLFW_PRESS)
    {
        m_track->moveNearestPoint(m_cursorPosition);
    }
}

void TrackEditor::drawGrid()
{
    if (!m_showGrid) return;

    glDisable(GL_LIGHTING);
    glColor3f(0.35f, 0.35f, 0.35f);
    glLineWidth(1.0f);

    glBegin(GL_LINES);
    for (int i = -m_gridLines; i <= m_gridLines; ++i)
    {
        float pos = i * m_gridSize;
        glVertex3f(pos, -m_gridLines * m_gridSize, 0);
        glVertex3f(pos, m_gridLines * m_gridSize, 0);

        glVertex3f(-m_gridLines * m_gridSize, pos, 0);
        glVertex3f(m_gridLines * m_gridSize, pos, 0);
    }
    glEnd();

    glLineWidth(2.0f);
    glBegin(GL_LINES);
    glColor3f(0.7f, 0.1f, 0.1f);
    glVertex3f(-m_gridLines * m_gridSize, 0, 0); glVertex3f(m_gridLines * m_gridSize, 0, 0);
    glColor3f(0.1f, 0.7f, 0.1f);
    glVertex3f(0, -m_gridLines * m_gridSize, 0); glVertex3f(0, m_gridLines * m_gridSize, 0);
    glEnd();

    glEnable(GL_LIGHTING);
}

void TrackEditor::drawSegmentHandles()
{
    glDisable(GL_LIGHTING);
    glColor3f(1.0f, 0.9f, 0.0f);
    glPointSize(12.0f);
    glBegin(GL_POINTS);
    glVertex3f(m_cursorPosition.x(), m_cursorPosition.y(), 0.1f);
    glEnd();
    glEnable(GL_LIGHTING);
}

void TrackEditor::render()
{
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45.0, 16.0 / 9.0, 0.1, 1000.0);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    gluLookAt(m_cameraPosition.x(), m_cameraPosition.y(), m_cameraPosition.z(),
        m_cameraTarget.x(), m_cameraTarget.y(), m_cameraTarget.z(),
        0, 0, 1);

    GLint viewport[4];
    GLdouble modelview[16];
    GLdouble projection[16];
    glGetIntegerv(GL_VIEWPORT, viewport);
    glGetDoublev(GL_MODELVIEW_MATRIX, modelview);
    glGetDoublev(GL_PROJECTION_MATRIX, projection);

    GLFWwindow* window = glfwGetCurrentContext();
    if (window)
    {
        double xpos, ypos;
        glfwGetCursorPos(window, &xpos, &ypos);
        int winW, winH;
        glfwGetWindowSize(window, &winW, &winH);

        double fb_x = xpos * ((double)viewport[2] / winW);
        double fb_y = (winH - ypos) * ((double)viewport[3] / winH);

        GLdouble nearX, nearY, nearZ;
        GLdouble farX, farY, farZ;
        gluUnProject(fb_x, fb_y, 0.0, modelview, projection, viewport, &nearX, &nearY, &nearZ);
        gluUnProject(fb_x, fb_y, 1.0, modelview, projection, viewport, &farX, &farY, &farZ);

        if (fabs(farZ - nearZ) > 0.0001)
        {
            double t = -nearZ / (farZ - nearZ);
            m_cursorPosition = Vector3(nearX + t * (farX - nearX), nearY + t * (farY - nearY), 0.0);
        }
    }

    drawGrid();

    if (m_track) m_track->draw();

    drawSegmentHandles();
}

void TrackEditor::handleKeyPress(int key, OpenGL* ogl)
{
    if (key == GLFW_KEY_G) m_showGrid = !m_showGrid;

    if (key == GLFW_KEY_S && (ogl->isKeyPressed(GLFW_KEY_LEFT_CONTROL) || ogl->isKeyPressed(GLFW_KEY_RIGHT_CONTROL)))
    {
        saveTrack(data_folder + "road/track_data.txt");
    }
    if (key == GLFW_KEY_L && (ogl->isKeyPressed(GLFW_KEY_LEFT_CONTROL) || ogl->isKeyPressed(GLFW_KEY_RIGHT_CONTROL)))
    {
        loadTrack(data_folder + "road/track_data.txt");
    }
}

void TrackEditor::saveTrack(const std::string& filename)
{
    if (m_track) m_track->saveToFile(data_folder + "road/road_geometry.txt");
}

void TrackEditor::loadTrack(const std::string& filename)
{
    if (m_track) m_track->loadFromFile(data_folder + "road/road_geometry.txt");
}