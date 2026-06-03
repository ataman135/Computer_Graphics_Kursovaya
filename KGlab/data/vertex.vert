#version 120

varying vec2 TexCoord;
varying vec4 VertColor;
varying vec3 Normal;
varying vec3 ViewPos;

void main()
{
    TexCoord = gl_MultiTexCoord0.xy;
    VertColor = gl_Color;

    Normal = normalize(gl_NormalMatrix * gl_Normal);
    ViewPos = vec3(gl_ModelViewMatrix * gl_Vertex);

    gl_Position = gl_ModelViewProjectionMatrix * gl_Vertex;
}