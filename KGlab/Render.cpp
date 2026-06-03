#include "OpenGL.h"
#include "Light.h"
#include "Game.h"
#include <iostream>

extern OpenGL* ogl_object;

Light light;
Game* game = nullptr;

void InitRender()
{
    light.SetPosition(10, 10, 20);

    game = new Game();
    game->init();

    std::cout << "Split-screen racing game initialized!" << std::endl;
}

void Render(double delta_time)
{
    if (game)
    {
        game->update(delta_time, ogl_object);
        game->render(ogl_object);
    }
}
void HandleKeyPress(int key, int action)
{
    if (game && action == GLFW_PRESS)
    {
        game->handleKeyPress(key, ogl_object);
    }
}