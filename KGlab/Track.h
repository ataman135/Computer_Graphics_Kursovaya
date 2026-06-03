#pragma once

#include "Vector3.h"
#include <vector>
#include <string>
#define GLFW_INCLUDE_GLU
#include <GLFW/glfw3.h>

struct TrackSegment
{
    Vector3 center;
    Vector3 direction;
    double width;
    double length;
    bool isCurve;
    double curveAngle;
};

class Track
{
    std::vector<TrackSegment> m_segments; // Опорные точки для редактора
    std::vector<Vector3> m_densePoints;  // Сглаженные точки сплайна для физики и отрисовки
    double m_trackWidth;
    double m_borderWidth;

    GLuint m_roadTexture;
    GLuint m_grassTexture;
    GLuint m_roadNormalTexture;
    GLuint m_grassNormalTexture;
    GLuint m_borderTexture;
    bool m_texturesLoaded;

    void generateCircularTrack();
    void loadTextures();
    void loadTexture(const std::string& filename, GLuint& textureId);

public:
    Track();
    ~Track();

    void draw();
    void drawWithTextures();
    bool isOnTrack(double x, double y, double tolerance = 0.0) const;
    Vector3 getNearestPointOnTrack(double x, double y) const;
    double getTrackWidth() const { return m_trackWidth; }

    int checkLapProgress(double x, double y, int currentCheckpoint) const;
    Vector3 getStartPosition() const;
    double getStartAngle() const;

    void saveToFile(const std::string& filename) const;
    void loadFromFile(const std::string& filename);

    void setUseTextures(bool use) { m_texturesLoaded = use; }
    bool areTexturesLoaded() const { return m_texturesLoaded; }

    void moveNearestPoint(const Vector3& newPos);
    void rebuildSplineTrack(); // Метод генерации сглаженного сплайна
};