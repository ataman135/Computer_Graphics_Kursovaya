#include "Track.h"
#include "Shaders.h"
#include <cmath>
#include <fstream>
#include <sstream>
#include <iostream>
#define GLFW_INCLUDE_GLU
#include <GLFW/glfw3.h>

#define STB_IMAGE_IMPLEMENTATION_TRACK
#include "include/stb_image.h"

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

extern std::string data_folder;

Track::Track()
{
    m_trackWidth = 5.0;
    m_borderWidth = 0.4;
    m_roadTexture = 0;
    m_grassTexture = 0;
    m_borderTexture = 0;
    m_texturesLoaded = false;
    m_roadNormalTexture = 0;
    m_grassNormalTexture = 0;

    generateCircularTrack();
    loadTextures();
}

Track::~Track()
{
    if (m_roadTexture != 0) glDeleteTextures(1, &m_roadTexture);
    if (m_grassTexture != 0) glDeleteTextures(1, &m_grassTexture);
    if (m_borderTexture != 0) glDeleteTextures(1, &m_borderTexture);

    if (m_roadNormalTexture != 0) glDeleteTextures(1, &m_roadNormalTexture);
    if (m_grassNormalTexture != 0) glDeleteTextures(1, &m_grassNormalTexture);
}

void Track::generateCircularTrack()
{
    m_segments.clear();
    double radius = 25.0;
    int numControlPoints = 16;
    for (int i = 0; i < numControlPoints; ++i)
    {
        double angle = i * (2.0 * M_PI / numControlPoints);
        TrackSegment seg;
        seg.center = Vector3(radius * cos(angle), radius * sin(angle), 0.0);
        m_segments.push_back(seg);
    }
    rebuildSplineTrack();
}

void Track::rebuildSplineTrack()
{
    m_densePoints.clear();
    if (m_segments.empty()) return;

    size_t n = m_segments.size();
    int stepsPerSegment = 20;

    for (size_t i = 0; i < n; ++i)
    {
        Vector3 p0 = m_segments[(i + n - 1) % n].center;
        Vector3 p1 = m_segments[i].center;
        Vector3 p2 = m_segments[(i + 1) % n].center;
        Vector3 p3 = m_segments[(i + 2) % n].center;

        for (int step = 0; step < stepsPerSegment; ++step)
        {
            double t = (double)step / stepsPerSegment;
            double t2 = t * t;
            double t3 = t2 * t;

            double f1 = -0.5 * t3 + t2 - 0.5 * t;
            double f2 = 1.5 * t3 - 2.5 * t2 + 1.0;
            double f3 = -1.5 * t3 + 2.0 * t2 + 0.5 * t;
            double f4 = 0.5 * t3 - 0.5 * t2;

            Vector3 splinePoint = p0 * f1 + p1 * f2 + p2 * f3 + p3 * f4;
            m_densePoints.push_back(splinePoint);
        }
    }
}

void Track::draw()
{
    glDisable(GL_LIGHTING);
    if (m_densePoints.empty()) return;

    extern PFNGLACTIVETEXTUREPROC glActiveTexture;

    if (m_texturesLoaded)
    {
        glActiveTexture(GL_TEXTURE0);
        glEnable(GL_TEXTURE_2D);
        glBindTexture(GL_TEXTURE_2D, m_grassTexture);

        glActiveTexture(GL_TEXTURE1);
        glEnable(GL_TEXTURE_2D);
        glBindTexture(GL_TEXTURE_2D, m_grassNormalTexture);
    }

    glColor3f(0.2f, 0.7f, 0.2f);
    glBegin(GL_QUADS);
    if (m_texturesLoaded) {
        glTexCoord2f(0.0f, 0.0f); glVertex3f(-150, -150, 0);
        glTexCoord2f(40.0f, 0.0f); glVertex3f(150, -150, 0);
        glTexCoord2f(40.0f, 40.0f); glVertex3f(150, 150, 0);
        glTexCoord2f(0.0f, 40.0f); glVertex3f(-150, 150, 0);
    }
    else {
        glVertex3f(-150, -150, 0); glVertex3f(150, -150, 0);
        glVertex3f(150, 150, 0); glVertex3f(-150, 150, 0);
    }
    glEnd();

    size_t n = m_densePoints.size();

    if (m_texturesLoaded)
    {
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, m_roadTexture);

        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, m_roadNormalTexture);
    }

    glColor3f(0.25f, 0.25f, 0.25f);
    glBegin(GL_QUAD_STRIP);
    for (size_t i = 0; i <= n; ++i)
    {
        Vector3 curr = m_densePoints[i % n];
        Vector3 next = m_densePoints[(i + 1) % n];
        Vector3 dir = (next - curr).normalize();
        Vector3 side = Vector3(-dir.y(), dir.x(), 0.0);

        Vector3 pLeft = curr - side * (m_trackWidth * 0.5);
        Vector3 pRight = curr + side * (m_trackWidth * 0.5);

        float u = (float)i * 0.2f;
        if (m_texturesLoaded) {
            glTexCoord2f(u, 0.0f); glVertex3f(pLeft.x(), pLeft.y(), 0.01f);
            glTexCoord2f(u, 1.0f); glVertex3f(pRight.x(), pRight.y(), 0.01f);
        }
        else {
            glVertex3f(pLeft.x(), pLeft.y(), 0.01f);
            glVertex3f(pRight.x(), pRight.y(), 0.01f);
        }
    }
    glEnd();

    if (m_texturesLoaded)
    {
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, m_borderTexture);
    }

    glColor3f(0.8f, 0.1f, 0.1f);
    glBegin(GL_QUAD_STRIP);
    for (size_t i = 0; i <= n; ++i)
    {
        Vector3 curr = m_densePoints[i % n];
        Vector3 next = m_densePoints[(i + 1) % n];
        Vector3 dir = (next - curr).normalize();
        Vector3 side = Vector3(-dir.y(), dir.x(), 0.0);

        Vector3 p1 = curr - side * (m_trackWidth * 0.5 + m_borderWidth);
        Vector3 p2 = curr - side * (m_trackWidth * 0.5);

        float u = (float)i * 0.8f;
        if (m_texturesLoaded) {
            glTexCoord2f(u, 0.0f); glVertex3f(p1.x(), p1.y(), 0.02f);
            glTexCoord2f(u, 1.0f); glVertex3f(p2.x(), p2.y(), 0.02f);
        }
        else {
            glVertex3f(p1.x(), p1.y(), 0.02f);
            glVertex3f(p2.x(), p2.y(), 0.02f);
        }
    }
    glEnd();

    glBegin(GL_QUAD_STRIP);
    for (size_t i = 0; i <= n; ++i)
    {
        Vector3 curr = m_densePoints[i % n];
        Vector3 next = m_densePoints[(i + 1) % n];
        Vector3 dir = (next - curr).normalize();
        Vector3 side = Vector3(-dir.y(), dir.x(), 0.0);

        Vector3 p1 = curr + side * (m_trackWidth * 0.5);
        Vector3 p2 = curr + side * (m_trackWidth * 0.5 + m_borderWidth);

        float u = (float)i * 0.8f;
        if (m_texturesLoaded) {
            glTexCoord2f(u, 0.0f); glVertex3f(p1.x(), p1.y(), 0.02f);
            glTexCoord2f(u, 1.0f); glVertex3f(p2.x(), p2.y(), 0.02f);
        }
        else {
            glVertex3f(p1.x(), p1.y(), 0.02f);
            glVertex3f(p2.x(), p2.y(), 0.02f);
        }
    }
    glEnd();

    if (m_texturesLoaded)
    {
        glActiveTexture(GL_TEXTURE1);
        glDisable(GL_TEXTURE_2D);
        glActiveTexture(GL_TEXTURE0);
        glDisable(GL_TEXTURE_2D);
    }

    glEnable(GL_LIGHTING);
}

bool Track::isOnTrack(double x, double y, double tolerance) const
{
    if (m_densePoints.empty()) return false;

    double minDist = 1e9;
    for (const auto& pt : m_densePoints)
    {
        double dx = pt.x() - x;
        double dy = pt.y() - y;
        double dist = sqrt(dx * dx + dy * dy);
        if (dist < minDist) minDist = dist;
    }
    return minDist <= (m_trackWidth * 0.5 + tolerance);
}

void Track::moveNearestPoint(const Vector3& newPos)
{
    if (m_segments.empty()) return;

    size_t closestIdx = 0;
    double minDist = 1e9;
    for (size_t i = 0; i < m_segments.size(); ++i)
    {
        double dx = m_segments[i].center.x() - newPos.x();
        double dy = m_segments[i].center.y() - newPos.y();
        double dist = sqrt(dx * dx + dy * dy);
        if (dist < minDist) { minDist = dist; closestIdx = i; }
    }
    m_segments[closestIdx].center = Vector3(newPos.x(), newPos.y(), 0.0);

    rebuildSplineTrack();
}

Vector3 Track::getNearestPointOnTrack(double x, double y) const
{
    if (m_densePoints.empty()) return Vector3(0, 0, 0);
    size_t closestIdx = 0;
    double minDist = 1e9;
    for (size_t i = 0; i < m_densePoints.size(); ++i)
    {
        double dx = m_densePoints[i].x() - x;
        double dy = m_densePoints[i].y() - y;
        double dist = sqrt(dx * dx + dy * dy);
        if (dist < minDist) { minDist = dist; closestIdx = i; }
    }
    return m_densePoints[closestIdx];
}

int Track::checkLapProgress(double x, double y, int currentCheckpoint) const
{
    if (m_densePoints.empty()) return currentCheckpoint;
    size_t closestIdx = 0;
    double minDist = 1e9;
    for (size_t i = 0; i < m_densePoints.size(); ++i)
    {
        double dx = m_densePoints[i].x() - x;
        double dy = m_densePoints[i].y() - y;
        double dist = sqrt(dx * dx + dy * dy);
        if (dist < minDist) { minDist = dist; closestIdx = i; }
    }

    int checkpoint = (int)((closestIdx * 4) / m_densePoints.size());
    if (checkpoint != currentCheckpoint && checkpoint == (currentCheckpoint + 1) % 4)
    {
        return checkpoint;
    }
    return currentCheckpoint;
}

Vector3 Track::getStartPosition() const
{
    if (m_densePoints.empty()) return Vector3(25.0, 0.0, 0.5);
    return m_densePoints[0] + Vector3(0, 0, 0.5);
}

double Track::getStartAngle() const
{
    if (m_densePoints.size() < 2) return M_PI / 2.0;
    Vector3 dir = (m_densePoints[1] - m_densePoints[0]).normalize();
    return atan2(dir.y(), dir.x());
}

void Track::loadTexture(const std::string& filename, GLuint& textureId)
{
    int width, height, channels;
    std::string fullPath = data_folder + filename;
    unsigned char* data = stbi_load(fullPath.c_str(), &width, &height, &channels, 0);
    if (data)
    {
        glGenTextures(1, &textureId);
        glBindTexture(GL_TEXTURE_2D, textureId);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        GLenum format = (channels == 4) ? GL_RGBA : GL_RGB;
        glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
        stbi_image_free(data);
    }
}

void Track::loadTextures()
{
    loadTexture("textures/road.png", m_roadTexture);
    loadTexture("textures/grass.png", m_grassTexture);
    loadTexture("textures/border.png", m_borderTexture);
    loadTexture("textures/road_normal.png", m_roadNormalTexture);
    loadTexture("textures/grass_normal.png", m_grassNormalTexture);
    m_texturesLoaded = (m_roadTexture != 0);
}

void Track::drawWithTextures() { draw(); }

void Track::saveToFile(const std::string& filename) const
{
    std::ofstream file(filename);
    if (!file.is_open()) return;
    file << "segments " << m_segments.size() << "\n";
    for (const auto& seg : m_segments)
    {
        file << "segment " << seg.center.x() << " " << seg.center.y() << " " << seg.center.z() << "\n";
    }
    file.close();
}

void Track::loadFromFile(const std::string& filename)
{
    std::ifstream file(filename);
    if (!file.is_open()) return;
    m_segments.clear();
    std::string line;
    while (std::getline(file, line))
    {
        if (line.empty() || line[0] == '#') continue;
        std::istringstream iss(line);
        std::string command;
        iss >> command;
        if (command == "segment")
        {
            TrackSegment seg;
            double cx, cy, cz;
            iss >> cx >> cy >> cz;
            seg.center = Vector3(cx, cy, cz);
            m_segments.push_back(seg);
        }
    }
    file.close();

    rebuildSplineTrack();
}