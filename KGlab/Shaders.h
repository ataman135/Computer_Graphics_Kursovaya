#pragma once

#include <string>
#include <windows.h>
#include <GL/gl.h>

#define GL_VERTEX_SHADER_ARB              0x8B31
#define GL_FRAGMENT_SHADER_ARB            0x8B30
#define GL_OBJECT_COMPILE_STATUS_ARB      0x8B81
#define GL_OBJECT_LINK_STATUS_ARB         0x8B82
#define GL_OBJECT_INFO_LOG_LENGTH_ARB     0x8B84

typedef unsigned int GLhandleARB;
typedef char GLcharARB;

typedef void (APIENTRY* PFNGLDELETEOBJECTARBPROC) (GLhandleARB obj);
typedef GLhandleARB(APIENTRY* PFNGLGETHANDLEARBPROC) (GLhandleARB programObj);
typedef void (APIENTRY* PFNGLDETACHOBJECTARBPROC) (GLhandleARB containerObj, GLhandleARB attachedObj);
typedef GLhandleARB(APIENTRY* PFNGLCREATESHADEROBJECTARBPROC) (GLenum shaderType);
typedef void (APIENTRY* PFNGLSHADERSOURCEARBPROC) (GLhandleARB shaderObj, GLsizei count, const GLcharARB** string, const GLint* length);
typedef void (APIENTRY* PFNGLCOMPILESHADERARBPROC) (GLhandleARB shaderObj);
typedef GLhandleARB(APIENTRY* PFNGLCREATEPROGRAMOBJECTARBPROC) (void);
typedef void (APIENTRY* PFNGLATTACHOBJECTARBPROC) (GLhandleARB containerObj, GLhandleARB obj);
typedef void (APIENTRY* PFNGLLINKPROGRAMARBPROC) (GLhandleARB programObj);
typedef void (APIENTRY* PFNGLVALIDATEPROGRAMARBPROC) (GLhandleARB programObj);
typedef void (APIENTRY* PFNGLUSEPROGRAMOBJECTARBPROC) (GLhandleARB programObj);
typedef void (APIENTRY* PFNGLGETOBJECTPARAMETERIVARBPROC) (GLhandleARB obj, GLenum pname, GLint* params);
typedef void (APIENTRY* PFNGLGETINFOLOGARBPROC) (GLhandleARB obj, GLsizei maxLength, GLsizei* length, GLcharARB* infoLog);
typedef GLint(APIENTRY* PFNGLGETUNIFORMLOCATIONARBPROC) (GLhandleARB programObj, const GLcharARB* name);
typedef void (APIENTRY* PFNGLUNIFORM1FARBPROC) (GLint location, GLfloat v0);
typedef void (APIENTRY* PFNGLUNIFORM2FARBPROC) (GLint location, GLfloat v0, GLfloat v1);
typedef void (APIENTRY* PFNGLUNIFORM3FARBPROC) (GLint location, GLfloat v0, GLfloat v1, GLfloat v2);
typedef void (APIENTRY* PFNGLUNIFORM4FARBPROC) (GLint location, GLfloat v0, GLfloat v1, GLfloat v2, GLfloat v3);
typedef void (APIENTRY* PFNGLUNIFORM1FVARBPROC) (GLint location, GLsizei count, const GLfloat* value);
typedef void (APIENTRY* PFNGLUNIFORM2FVARBPROC) (GLint location, GLsizei count, const GLfloat* value);
typedef void (APIENTRY* PFNGLUNIFORM3FVARBPROC) (GLint location, GLsizei count, const GLfloat* value);
typedef void (APIENTRY* PFNGLUNIFORM4FVARBPROC) (GLint location, GLsizei count, const GLfloat* value);
typedef void (APIENTRY* PFNGLUNIFORM1IARBPROC) (GLint location, GLint v0);
typedef void (APIENTRY* PFNGLACTIVETEXTUREPROC) (GLenum texture);

class Shader
{
public:
    GLhandleARB program = 0;
    GLhandleARB vertex = 0;
    GLhandleARB fragment = 0;

    std::string VshaderFileName;
    std::string FshaderFileName;

    void LoadShaderFromFile();
    void Compile();
    void UseShader();
    void DontUseShaders();
};

#define GL_TEXTURE0 0x84C0
#define GL_TEXTURE1 0x84C1

extern PFNGLACTIVETEXTUREPROC glActiveTexture;