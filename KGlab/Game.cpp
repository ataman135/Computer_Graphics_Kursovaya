#include "Game.h"
#include "Light.h"
#include <iostream>
#include <cstdio>
#include <cmath>

extern Light light;
extern std::string data_folder;

void drawStrokeChar(char c, float x, float y, float size)
{
    glLineWidth(2.0f);
    glBegin(GL_LINES);
    float w = size * 0.6f;
    float h = size;

    switch (c) {
    case '0':
        glVertex2f(x, y); glVertex2f(x + w, y);
        glVertex2f(x + w, y); glVertex2f(x + w, y + h);
        glVertex2f(x + w, y + h); glVertex2f(x, y + h);
        glVertex2f(x, y + h); glVertex2f(x, y);
        break;
    case '1':
        glVertex2f(x + w / 2, y); glVertex2f(x + w / 2, y + h);
        break;
    case '2':
        glVertex2f(x, y); glVertex2f(x + w, y);
        glVertex2f(x + w, y); glVertex2f(x + w, y + h / 2);
        glVertex2f(x + w, y + h / 2); glVertex2f(x, y + h / 2);
        glVertex2f(x, y + h / 2); glVertex2f(x, y + h);
        glVertex2f(x, y + h); glVertex2f(x + w, y + h);
        break;
    case '3':
        glVertex2f(x, y); glVertex2f(x + w, y);
        glVertex2f(x + w, y); glVertex2f(x + w, y + h);
        glVertex2f(x + w, y + h); glVertex2f(x, y + h);
        glVertex2f(x, y + h / 2); glVertex2f(x + w, y + h / 2);
        break;
    case '4':
        glVertex2f(x, y); glVertex2f(x, y + h / 2);
        glVertex2f(x, y + h / 2); glVertex2f(x + w, y + h / 2);
        glVertex2f(x + w, y); glVertex2f(x + w, y + h);
        break;
    case '5':
        glVertex2f(x + w, y); glVertex2f(x, y);
        glVertex2f(x, y); glVertex2f(x, y + h / 2);
        glVertex2f(x, y + h / 2); glVertex2f(x + w, y + h / 2);
        glVertex2f(x + w, y + h / 2); glVertex2f(x + w, y + h);
        glVertex2f(x + w, y + h); glVertex2f(x, y + h);
        break;
    case '6':
        glVertex2f(x + w, y); glVertex2f(x, y);
        glVertex2f(x, y); glVertex2f(x, y + h);
        glVertex2f(x, y + h); glVertex2f(x + w, y + h);
        glVertex2f(x + w, y + h); glVertex2f(x + w, y + h / 2);
        glVertex2f(x + w, y + h / 2); glVertex2f(x, y + h / 2);
        break;
    case '7':
        glVertex2f(x, y); glVertex2f(x + w, y);
        glVertex2f(x + w, y); glVertex2f(x + w, y + h);
        break;
    case '8':
        glVertex2f(x, y); glVertex2f(x + w, y);
        glVertex2f(x + w, y); glVertex2f(x + w, y + h);
        glVertex2f(x + w, y + h); glVertex2f(x, y + h);
        glVertex2f(x, y + h); glVertex2f(x, y);
        glVertex2f(x, y + h / 2); glVertex2f(x + w, y + h / 2);
        break;
    case '9':
        glVertex2f(x, y); glVertex2f(x + w, y);
        glVertex2f(x + w, y); glVertex2f(x + w, y + h);
        glVertex2f(x, y + h / 2); glVertex2f(x + w, y + h / 2);
        glVertex2f(x, y + h / 2); glVertex2f(x, y + h / 2);
        glVertex2f(x, y); glVertex2f(x, y + h / 2);
        break;
    case 'P':
        glVertex2f(x, y); glVertex2f(x, y + h);
        glVertex2f(x, y); glVertex2f(x + w, y);
        glVertex2f(x + w, y); glVertex2f(x + w, y + h / 2);
        glVertex2f(x + w, y + h / 2); glVertex2f(x, y + h / 2);
        break;
    case 'S':
        glVertex2f(x + w, y); glVertex2f(x, y);
        glVertex2f(x, y); glVertex2f(x, y + h / 2);
        glVertex2f(x, y + h / 2); glVertex2f(x + w, y + h / 2);
        glVertex2f(x + w, y + h / 2); glVertex2f(x + w, y + h);
        glVertex2f(x + w, y + h); glVertex2f(x, y + h);
        break;
    case 'E':
        glVertex2f(x + w, y); glVertex2f(x, y);
        glVertex2f(x, y); glVertex2f(x, y + h);
        glVertex2f(x, y + h); glVertex2f(x + w, y + h);
        glVertex2f(x, y + h / 2); glVertex2f(x + w / 2, y + h / 2);
        break;
    case 'D':
        glVertex2f(x, y); glVertex2f(x, y + h);
        glVertex2f(x, y); glVertex2f(x + w * 0.8f, y);
        glVertex2f(x + w * 0.8f, y); glVertex2f(x + w, y + h * 0.2f);
        glVertex2f(x + w, y + h * 0.2f); glVertex2f(x + w, y + h * 0.8f);
        glVertex2f(x + w, y + h * 0.8f); glVertex2f(x + w * 0.8f, y + h);
        glVertex2f(x + w * 0.8f, y + h); glVertex2f(x, y + h);
        break;
    case 'L':
        glVertex2f(x, y); glVertex2f(x, y + h);
        glVertex2f(x, y + h); glVertex2f(x + w, y + h);
        break;
    case 'A':
        glVertex2f(x, y + h); glVertex2f(x, y + h / 2);
        glVertex2f(x, y + h / 2); glVertex2f(x + w / 2, y);
        glVertex2f(x + w / 2, y); glVertex2f(x + w, y + h / 2);
        glVertex2f(x + w, y + h / 2); glVertex2f(x + w, y + h);
        glVertex2f(x, y + h / 2); glVertex2f(x + w, y + h / 2);
        break;
    case 'K':
        glVertex2f(x, y); glVertex2f(x, y + h);
        glVertex2f(x, y + h / 2); glVertex2f(x + w, y);
        glVertex2f(x, y + h / 2); glVertex2f(x + w, y + h);
        break;
    case 'M':
        glVertex2f(x, y + h); glVertex2f(x, y);
        glVertex2f(x, y); glVertex2f(x + w / 2, y + h / 2);
        glVertex2f(x + w / 2, y + h / 2); glVertex2f(x + w, y);
        glVertex2f(x + w, y); glVertex2f(x + w, y + h);
        break;
    case 'H':
        glVertex2f(x, y); glVertex2f(x, y + h);
        glVertex2f(x + w, y); glVertex2f(x + w, y + h);
        glVertex2f(x, y + h / 2); glVertex2f(x + w, y + h / 2);
        break;
    case ':':
        glVertex2f(x + w / 2, y + h * 0.3f); glVertex2f(x + w / 2, y + h * 0.35f);
        glVertex2f(x + w / 2, y + h * 0.65f); glVertex2f(x + w / 2, y + h * 0.7f);
        break;
    case '/':
        glVertex2f(x, y + h); glVertex2f(x + w, y);
        break;
    }
    glEnd();
}

void drawStrokeText(const std::string& text, float x, float y, float size)
{
    float currentX = x;
    float spacing = size * 0.8f;
    for (char c : text) {
        if (c >= 'a' && c <= 'z') c = c - 'a' + 'A';
        drawStrokeChar(c, currentX, y, size);
        currentX += spacing;
    }
}

Game::Game() : m_track(nullptr), m_car1(nullptr), m_car2(nullptr),
m_camera1(nullptr), m_camera2(nullptr), m_editor(nullptr), m_raceShader(nullptr),
m_mode(GameMode::RACING), m_useShaders(true), m_escPressed(false),
m_windowWidth(1280), m_windowHeight(720)
{
}

Game::~Game()
{
    delete m_track;
    delete m_car1;
    delete m_car2;
    delete m_camera1;
    delete m_camera2;
    delete m_editor;
    delete m_raceShader;
}

void Game::init()
{
    m_track = new Track();
    m_car1 = new Car(ControlScheme::WASD, Vector3(0.8f, 0.1f, 0.1f));
    m_car2 = new Car(ControlScheme::ARROWS, Vector3(0.1f, 0.1f, 0.8f));

    Vector3 startPos = m_track->getStartPosition();
    double startAngle = m_track->getStartAngle();
    m_car1->reset(startPos + Vector3(0, 1.5, 0), startAngle);
    m_car2->reset(startPos + Vector3(0, -1.5, 0), startAngle);

    m_camera1 = new CameraFollow();
    m_camera2 = new CameraFollow();
    m_editor = new TrackEditor(m_track);

    m_raceShader = new Shader();
    m_raceShader->VshaderFileName = data_folder + "vertex.vert";
    m_raceShader->FshaderFileName = data_folder + "fragment.frag";
    m_raceShader->LoadShaderFromFile();
    m_raceShader->Compile();

    extern PFNGLGETUNIFORMLOCATIONARBPROC glGetUniformLocationARB;
    extern PFNGLUNIFORM1IARBPROC glUniform1iARB;
    m_raceShader->UseShader();
    glUniform1iARB(glGetUniformLocationARB(m_raceShader->program, "tex"), 0);
    glUniform1iARB(glGetUniformLocationARB(m_raceShader->program, "normalMap"), 1);
    m_raceShader->DontUseShaders();
}

void Game::update(double deltaTime, OpenGL* ogl)
{
    m_windowWidth = ogl->window_width();
    m_windowHeight = ogl->window_height();

    if (m_mode == GameMode::RACING)
    {
        m_car1->update(deltaTime, ogl, m_track, m_car2);
        m_car2->update(deltaTime, ogl, m_track, m_car1);
        m_camera1->update(m_car1, deltaTime);
        m_camera2->update(m_car2, deltaTime);
    }
    else
    {
        m_editor->update(deltaTime, ogl);
    }
}

void Game::setupViewport(int x, int y, int width, int height)
{
    glViewport(x, y, width, height);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45.0, (GLdouble)width / (GLdouble)height, 0.1, 1000.0);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

void Game::renderScene(Car* car, Car* otherCar)
{
    if (m_useShaders && m_raceShader)
    {
        m_raceShader->UseShader();
    }

    light.ApplyLight();

    m_track->draw();
    m_car1->draw();
    m_car2->draw();

    if (m_useShaders && m_raceShader)
    {
        m_raceShader->DontUseShaders();
    }
}

void Game::render(OpenGL* ogl)
{
    int w = ogl->fb_width();
    int h = ogl->fb_height();

    if (m_mode == GameMode::RACING)
    {
        setupViewport(0, 0, w / 2, h);
        m_camera1->applyCamera();
        renderScene(m_car1, m_car2);

        setupViewport(w / 2, 0, w / 2, h);
        m_camera2->applyCamera();
        renderScene(m_car2, m_car1);

        setupViewport(0, 0, w, h);
        glDisable(GL_LIGHTING);
        glMatrixMode(GL_PROJECTION);
        glPushMatrix();
        glLoadIdentity();
        glOrtho(0, w, h, 0, -1, 1);
        glMatrixMode(GL_MODELVIEW);
        glPushMatrix();
        glLoadIdentity();

        glColor3f(1.0f, 1.0f, 1.0f);
        glLineWidth(4.0f);
        glBegin(GL_LINES);
        glVertex2i(w / 2, 0);
        glVertex2i(w / 2, h);
        glEnd();

        glPopMatrix();
        glMatrixMode(GL_PROJECTION);
        glPopMatrix();
        glMatrixMode(GL_MODELVIEW);
        glEnable(GL_LIGHTING);

        renderHUD();
    }
    else
    {
        setupViewport(0, 0, w, h);
        m_editor->render();
    }
}

void Game::renderHUD()
{
    glPushAttrib(GL_ALL_ATTRIB_BITS);
    glPushMatrix();
    glDisable(GL_LIGHTING);
    glDisable(GL_DEPTH_TEST);
    glDisable(GL_TEXTURE_2D);

    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
    glLoadIdentity();
    glOrtho(0, m_windowWidth, m_windowHeight, 0, -1, 1);
    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glLoadIdentity();

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glColor4f(0.0f, 0.0f, 0.0f, 0.6f);
    glBegin(GL_QUADS);
    glVertex2f(10, 10); glVertex2f(280, 10); glVertex2f(280, 75); glVertex2f(10, 75);
    glEnd();

    glBegin(GL_QUADS);
    glVertex2f(m_windowWidth / 2 + 10, 10); glVertex2f(m_windowWidth / 2 + 280, 10);
    glVertex2f(m_windowWidth / 2 + 280, 75); glVertex2f(m_windowWidth / 2 + 10, 75);
    glEnd();

    glDisable(GL_BLEND);

    char hud1[128], hud2[128], laps1[64], laps2[64];
    sprintf(hud1, "P1 SPEED: %d KM/H", (int)(m_car1->getSpeed() * 10));
    sprintf(laps1, "LAPS: %d", m_car1->getLapCount());
    sprintf(hud2, "P2 SPEED: %d KM/H", (int)(m_car2->getSpeed() * 10));
    sprintf(laps2, "LAPS: %d", m_car2->getLapCount());

    glColor3f(1.0f, 1.0f, 1.0f);
    drawStrokeText(hud1, 20, 20, 16);
    drawStrokeText(laps1, 20, 45, 16);

    int offsetX = m_windowWidth / 2;
    drawStrokeText(hud2, offsetX + 20, 20, 16);
    drawStrokeText(laps2, offsetX + 20, 45, 16);

    glPopMatrix();
    glMatrixMode(GL_PROJECTION);
    glPopMatrix();
    glMatrixMode(GL_MODELVIEW);
    glPopMatrix();
    glPopAttrib();
}

void Game::handleKeyPress(int key, OpenGL* ogl)
{
    if (key == GLFW_KEY_TAB) toggleMode();

    if (m_mode == GameMode::EDITOR)
    {
        m_editor->handleKeyPress(key, ogl);
    }
}

void Game::toggleMode()
{
    m_mode = (m_mode == GameMode::RACING) ? GameMode::EDITOR : GameMode::RACING;
}

void Game::toggleShaders()
{
    m_useShaders = !m_useShaders;
}