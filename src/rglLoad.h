#ifndef RGL_H
#define RGL_H

#ifndef __APPLE__
#include <GL/gl.h>
#else
#include <OpenGL/gl.h>
#endif

#ifndef GL_COMPILE_STATUS
#define GL_BLEND_EQUATION_RGB             0x8009
#define GL_VERTEX_ATTRIB_ARRAY_ENABLED    0x8622
#define GL_VERTEX_ATTRIB_ARRAY_SIZE       0x8623
#define GL_VERTEX_ATTRIB_ARRAY_STRIDE     0x8624
#define GL_VERTEX_ATTRIB_ARRAY_TYPE       0x8625
#define GL_CURRENT_VERTEX_ATTRIB          0x8626
#define GL_VERTEX_PROGRAM_POINT_SIZE      0x8642
#define GL_VERTEX_ATTRIB_ARRAY_POINTER    0x8645
#define GL_STENCIL_BACK_FUNC              0x8800
#define GL_STENCIL_BACK_FAIL              0x8801
#define GL_STENCIL_BACK_PASS_DEPTH_FAIL   0x8802
#define GL_STENCIL_BACK_PASS_DEPTH_PASS   0x8803
#define GL_MAX_DRAW_BUFFERS               0x8824
#define GL_DRAW_BUFFER0                   0x8825
#define GL_DRAW_BUFFER1                   0x8826
#define GL_DRAW_BUFFER2                   0x8827
#define GL_DRAW_BUFFER3                   0x8828
#define GL_DRAW_BUFFER4                   0x8829
#define GL_DRAW_BUFFER5                   0x882A
#define GL_DRAW_BUFFER6                   0x882B
#define GL_DRAW_BUFFER7                   0x882C
#define GL_DRAW_BUFFER8                   0x882D
#define GL_DRAW_BUFFER9                   0x882E
#define GL_DRAW_BUFFER10                  0x882F
#define GL_DRAW_BUFFER11                  0x8830
#define GL_DRAW_BUFFER12                  0x8831
#define GL_DRAW_BUFFER13                  0x8832
#define GL_DRAW_BUFFER14                  0x8833
#define GL_DRAW_BUFFER15                  0x8834
#define GL_BLEND_EQUATION_ALPHA           0x883D
#define GL_MAX_VERTEX_ATTRIBS             0x8869
#define GL_VERTEX_ATTRIB_ARRAY_NORMALIZED 0x886A
#define GL_MAX_TEXTURE_IMAGE_UNITS        0x8872
#define GL_FRAGMENT_SHADER                0x8B30
#define GL_VERTEX_SHADER                  0x8B31
#define GL_MAX_FRAGMENT_UNIFORM_COMPONENTS 0x8B49
#define GL_MAX_VERTEX_UNIFORM_COMPONENTS  0x8B4A
#define GL_MAX_VARYING_FLOATS             0x8B4B
#define GL_MAX_VERTEX_TEXTURE_IMAGE_UNITS 0x8B4C
#define GL_MAX_COMBINED_TEXTURE_IMAGE_UNITS 0x8B4D
#define GL_SHADER_TYPE                    0x8B4F
#define GL_FLOAT_VEC2                     0x8B50
#define GL_FLOAT_VEC3                     0x8B51
#define GL_FLOAT_VEC4                     0x8B52
#define GL_INT_VEC2                       0x8B53
#define GL_INT_VEC3                       0x8B54
#define GL_INT_VEC4                       0x8B55
#define GL_BOOL                           0x8B56
#define GL_BOOL_VEC2                      0x8B57
#define GL_BOOL_VEC3                      0x8B58
#define GL_BOOL_VEC4                      0x8B59
#define GL_FLOAT_MAT2                     0x8B5A
#define GL_FLOAT_MAT3                     0x8B5B
#define GL_FLOAT_MAT4                     0x8B5C
#define GL_SAMPLER_1D                     0x8B5D
#define GL_SAMPLER_2D                     0x8B5E
#define GL_SAMPLER_3D                     0x8B5F
#define GL_SAMPLER_CUBE                   0x8B60
#define GL_SAMPLER_1D_SHADOW              0x8B61
#define GL_SAMPLER_2D_SHADOW              0x8B62
#define GL_DELETE_STATUS                  0x8B80
#define GL_COMPILE_STATUS                 0x8B81
#define GL_LINK_STATUS                    0x8B82
#define GL_VALIDATE_STATUS                0x8B83
#define GL_INFO_LOG_LENGTH                0x8B84
#define GL_ATTACHED_SHADERS               0x8B85
#define GL_ACTIVE_UNIFORMS                0x8B86
#define GL_ACTIVE_UNIFORM_MAX_LENGTH      0x8B87
#define GL_SHADER_SOURCE_LENGTH           0x8B88
#define GL_ACTIVE_ATTRIBUTES              0x8B89
#define GL_ACTIVE_ATTRIBUTE_MAX_LENGTH    0x8B8A
#define GL_FRAGMENT_SHADER_DERIVATIVE_HINT 0x8B8B
#define GL_SHADING_LANGUAGE_VERSION       0x8B8C
#define GL_CURRENT_PROGRAM                0x8B8D
#define GL_POINT_SPRITE_COORD_ORIGIN      0x8CA0
#define GL_LOWER_LEFT                     0x8CA1
#define GL_UPPER_LEFT                     0x8CA2
#define GL_STENCIL_BACK_REF               0x8CA3
#define GL_STENCIL_BACK_VALUE_MASK        0x8CA4
#define GL_STENCIL_BACK_WRITEMASK         0x8CA5
#define GL_VERTEX_PROGRAM_TWO_SIDE        0x8643
#define GL_POINT_SPRITE                   0x8861
#define GL_COORD_REPLACE                  0x8862
#define GL_MAX_TEXTURE_COORDS             0x8871

typedef char GLchar;
typedef int GLsizei;
typedef ptrdiff_t GLintptr;
typedef ptrdiff_t GLsizeiptr;

#define GL_VERTEX_SHADER   0x8B31
#define GL_FRAGMENT_SHADER 0x8B30
#define GL_ARRAY_BUFFER         0x8892
#define GL_ELEMENT_ARRAY_BUFFER 0x8893
#define GL_STATIC_DRAW  0x88E4
#define GL_DYNAMIC_DRAW 0x88E8
#define GL_TEXTURE0 0x84C0
#define GL_CLAMP_TO_BORDER 0x812D
#endif

#include <stddef.h>

#define PROCS \
    PROC(void, glShaderSource, GLuint shader, GLsizei count, const GLchar *const*string, const GLint *length) \
    PROC(GLuint, glCreateShader, GLenum type) \
    PROC(void, glCompileShader, GLuint shader) \
    PROC(GLuint, glCreateProgram, void) \
    PROC(void, glAttachShader, GLuint program, GLuint shader) \
    PROC(void, glBindAttribLocation, GLuint program, GLuint index, const GLchar *name) \
    PROC(void, glLinkProgram, GLuint program) \
    PROC(void, glBindBuffer, GLenum target, GLuint buffer) \
    PROC(void, glBufferData, GLenum target, GLsizeiptr size, const void *data, GLenum usage) \
    PROC(void, glEnableVertexAttribArray, GLuint index) \
    PROC(void, glVertexAttribPointer, GLuint index, GLint size, GLenum type, GLboolean normalized, GLsizei stride, const void *pointer) \
    PROC(void, glDisableVertexAttribArray, GLuint index) \
    PROC(void, glDeleteBuffers, GLsizei n, const GLuint *buffers) \
    PROC(void, glDeleteVertexArrays, GLsizei n, const GLuint *arrays) \
    PROC(void, glUseProgram, GLuint program) \
    PROC(void, glDetachShader, GLuint program, GLuint shader) \
    PROC(void, glDeleteShader, GLuint shader) \
    PROC(void, glDeleteProgram, GLuint program) \
    PROC(void, glBufferSubData, GLenum target, GLintptr offset, GLsizeiptr size, const void *data) \
    PROC(void, glGetShaderiv, GLuint shader, GLenum pname, GLint *params) \
    PROC(void, glGetShaderInfoLog, GLuint shader, GLsizei bufSize, GLsizei *length, GLchar *infoLog) \
    PROC(void, glGetProgramiv, GLuint program, GLenum pname, GLint *params) \
    PROC(void, glGetProgramInfoLog, GLuint program, GLsizei bufSize, GLsizei *length, GLchar *infoLog) \
    PROC(void, glGenVertexArrays, GLsizei n, GLuint *arrays) \
    PROC(void, glGenBuffers, GLsizei n, GLuint *buffers) \
    PROC(void, glBindVertexArray, GLuint array) \
    PROC(GLint, glGetUniformLocation, GLuint program, const GLchar *name) \
    PROC(void, glUniformMatrix4fv, GLint location, GLsizei count, GLboolean transpose, const GLfloat *value) \
    PROC(void, glTexImage2D, GLenum target, GLint level, GLint internalformat, GLsizei width, GLsizei height, GLint border, GLenum format, GLenum type, const void *pixels) \
    PROC(void, glActiveTexture, GLenum texture) \
    PROC(void, glDebugMessageCallback, void* callback, const void*) \
    PROC(void, glDrawElements, GLenum mode, GLsizei count, GLenum type, const void * indices) \
    PROC(void, glClear, GLbitfield mask) \
    PROC(void, glClearColor, GLfloat, GLfloat, GLfloat, GLfloat) \
    PROC(void, glUniform2f, GLint location, GLfloat v0, GLfloat v1) \
    PROC(void, glUniform4f, GLint location, GLfloat v0, GLfloat v1, GLfloat v2, GLfloat v3) \
    PROC(void, glUniform1i, GLint location, GLint v0)

static inline RGFW_proc load_proc_or_die(const char *name, RGFW_proc *proc);

// Each lazy loaded OpenGL function call might be slighty slower due to an additional condition check, but
// this is not a performance critical application. Change this to something better when performance issues
// occur.
#define LAZY_LOAD_PROC(name) (name##SRC ? name##SRC : (name##PROC)load_proc_or_die(#name, (RGFW_proc*)&name##SRC))

// I wish I could generate these defines with the X-macro too...
#define glShaderSource             LAZY_LOAD_PROC(glShaderSource)
#define glCreateShader             LAZY_LOAD_PROC(glCreateShader)
#define glCompileShader            LAZY_LOAD_PROC(glCompileShader)
#define glCreateProgram            LAZY_LOAD_PROC(glCreateProgram)
#define glAttachShader             LAZY_LOAD_PROC(glAttachShader)
#define glBindAttribLocation       LAZY_LOAD_PROC(glBindAttribLocation)
#define glLinkProgram              LAZY_LOAD_PROC(glLinkProgram)
#define glBindBuffer               LAZY_LOAD_PROC(glBindBuffer)
#define glBufferData               LAZY_LOAD_PROC(glBufferData)
#define glEnableVertexAttribArray  LAZY_LOAD_PROC(glEnableVertexAttribArray)
#define glVertexAttribPointer      LAZY_LOAD_PROC(glVertexAttribPointer)
#define glDisableVertexAttribArray LAZY_LOAD_PROC(glDisableVertexAttribArray)
#define glDeleteBuffers            LAZY_LOAD_PROC(glDeleteBuffers)
#define glDeleteVertexArrays       LAZY_LOAD_PROC(glDeleteVertexArrays)
#define glUseProgram               LAZY_LOAD_PROC(glUseProgram)
#define glDetachShader             LAZY_LOAD_PROC(glDetachShader)
#define glDeleteShader             LAZY_LOAD_PROC(glDeleteShader)
#define glDeleteProgram            LAZY_LOAD_PROC(glDeleteProgram)
#define glBufferSubData            LAZY_LOAD_PROC(glBufferSubData)
#define glGetShaderiv              LAZY_LOAD_PROC(glGetShaderiv)
#define glGetShaderInfoLog         LAZY_LOAD_PROC(glGetShaderInfoLog)
#define glGetProgramiv             LAZY_LOAD_PROC(glGetProgramiv)
#define glGetProgramInfoLog        LAZY_LOAD_PROC(glGetProgramInfoLog)
#define glGenVertexArrays          LAZY_LOAD_PROC(glGenVertexArrays)
#define glGenBuffers               LAZY_LOAD_PROC(glGenBuffers)
#define glBindVertexArray          LAZY_LOAD_PROC(glBindVertexArray)
#define glGetUniformLocation       LAZY_LOAD_PROC(glGetUniformLocation)
#define glUniformMatrix4fv         LAZY_LOAD_PROC(glUniformMatrix4fv)
#define glTexImage2D               LAZY_LOAD_PROC(glTexImage2D)
#define glActiveTexture            LAZY_LOAD_PROC(glActiveTexture)
#define glDebugMessageCallback     LAZY_LOAD_PROC(glDebugMessageCallback)
#define glDrawElements             LAZY_LOAD_PROC(glDrawElements)
#define glClear                    LAZY_LOAD_PROC(glClear)
#define glClearColor               LAZY_LOAD_PROC(glClearColor)
#define glUniform2f                LAZY_LOAD_PROC(glUniform2f)
#define glUniform4f                LAZY_LOAD_PROC(glUniform4f)
#define glUniform1i                LAZY_LOAD_PROC(glUniform1i)

#define PROC(ret, name, ...) typedef ret (*name##PROC)(__VA_ARGS__);
PROCS
#undef PROC

#define PROC(ret, name, ...) name##PROC name##SRC = NULL;
PROCS
#undef PROC

#endif // RGL_H

#ifdef RGL_LOAD_IMPLEMENTATION

static inline RGFW_proc load_proc_or_die(const char *name, RGFW_proc *proc)
{
    printf("INFO: loading OpenGL function %s\n", name);
    *proc = RGFW_getProcAddress_OpenGL(name);
    if (*proc) return *proc;
    fprintf(stderr, "ERROR: could not load OpenGL function %s\n", name);
    abort();
}

#endif  // RGL_LOAD_IMPLEMENTATION
