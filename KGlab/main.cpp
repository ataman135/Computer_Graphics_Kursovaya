#include <stdio.h>

#include "OpenGL.h"
#include <clocale>
#include <iostream>
#include<filesystem>
std::string data_folder;
OpenGL* ogl_object;

int main(void)
{
   data_folder = "data/";

   for (int i = 0; i <= 3; ++i)
   {
       if (!std::filesystem::exists(data_folder))
       {
           data_folder = "../" + data_folder;
       }
       else
       {
           break;
       }
   }

  

    OpenGL gl;
    ogl_object = &gl;
    gl.initWindow();
    gl.Render();

    return 0;
}

