#pragma once

#include "Vector3.h"
#include "Track.h"
#include "OpenGL.h"
#include <string>

class TrackEditor
{
    Track* m_track;

    Vector3 m_cameraPosition;
    Vector3 m_cameraTarget;
    float m_cameraPitch;
    float m_cameraYaw;
    float m_cameraDistance;

    bool m_showGrid;
    float m_gridSize;
    int m_gridLines;

    Vector3 m_cursorPosition;
    bool m_isDragging;
    double m_lastMouseX;
    double m_lastMouseY;

public:
    TrackEditor(Track* track);
    ~TrackEditor();

    void update(double deltaTime, OpenGL* ogl);
    void render();
    void renderHUD(int windowWidth, int windowHeight);

    void handleKeyPress(int key, OpenGL* ogl);
    void saveTrack(const std::string& filename);
    void loadTrack(const std::string& filename);

private:
    void updateCamera(double deltaTime, OpenGL* ogl);
    void drawGrid();
    void drawSegmentHandles();
};