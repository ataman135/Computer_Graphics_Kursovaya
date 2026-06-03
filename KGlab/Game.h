#pragma once

#include "Car.h"
#include "Track.h"
#include "CameraFollow.h"
#include "TrackEditor.h"
#include "OpenGL.h"
#include "Shaders.h"

enum class GameMode
{
    RACING,
    EDITOR
};

class Game
{
    Track* m_track;
    Car* m_car1;
    Car* m_car2;
    CameraFollow* m_camera1;
    CameraFollow* m_camera2;
    TrackEditor* m_editor;
    Shader* m_raceShader;

    GameMode m_mode;
    bool m_useShaders;
    bool m_escPressed;

    int m_windowWidth;
    int m_windowHeight;

public:
    Game();
    ~Game();

    void init();
    void update(double deltaTime, OpenGL* ogl);
    void render(OpenGL* ogl);
    void handleKeyPress(int key, OpenGL* ogl);

    void toggleMode();
    void toggleShaders();
    GameMode getMode() const { return m_mode; }

private:
    void renderScene(Car* car, Car* otherCar);
    void renderHUD();
    void setupViewport(int x, int y, int width, int height);
};