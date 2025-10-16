#define PROCS \
    PROC(PFNGLCREATESHADERPROC, glCreateShader) \
    PROC(PFNGLSHADERSOURCEPROC, glShaderSource) \
    PROC(PFNGLCOMPILESHADERPROC, glCompileShader) \
    PROC(PFNGLGETSHADERIVPROC, glGetShaderiv) \
    PROC(PFNGLGETSHADERINFOLOGPROC, glGetShaderInfoLog) \
    PROC(PFNGLCREATEPROGRAMPROC, glCreateProgram) \
    PROC(PFNGLATTACHSHADERPROC, glAttachShader) \
    PROC(PFNGLLINKPROGRAMPROC, glLinkProgram) \
    PROC(PFNGLGETPROGRAMIVPROC, glGetProgramiv) \
    PROC(PFNGLGETPROGRAMINFOLOGPROC, glGetProgramInfoLog) \
    PROC(PFNGLDELETESHADERPROC, glDeleteShader) \
    PROC(PFNGLUSEPROGRAMPROC, glUseProgram) \
    PROC(PFNGLGENVERTEXARRAYSPROC, glGenVertexArrays) \
    PROC(PFNGLBINDVERTEXARRAYPROC, glBindVertexArray) \
    PROC(PFNGLDELETEPROGRAMPROC, glDeleteProgram) \
    PROC(PFNGLGETUNIFORMLOCATIONPROC, glGetUniformLocation) \
    PROC(PFNGLUNIFORM2FPROC, glUniform2f) \
    PROC(PFNGLGENBUFFERSPROC, glGenBuffers) \
    PROC(PFNGLBINDBUFFERPROC, glBindBuffer) \
    PROC(PFNGLBUFFERDATAPROC, glBufferData) \
    PROC(PFNGLENABLEVERTEXATTRIBARRAYPROC, glEnableVertexAttribArray) \
    PROC(PFNGLVERTEXATTRIBPOINTERPROC, glVertexAttribPointer) \
    PROC(PFNGLUNIFORM1FPROC, glUniform1f) \
    PROC(PFNGLBUFFERSUBDATAPROC, glBufferSubData) \
    PROC(PFNGLGENFRAMEBUFFERSPROC, glGenFramebuffers) \
    PROC(PFNGLBINDFRAMEBUFFERPROC, glBindFramebuffer) \
    PROC(PFNGLFRAMEBUFFERTEXTURE2DPROC, glFramebufferTexture2D) \
    PROC(PFNGLCHECKFRAMEBUFFERSTATUSPROC, glCheckFramebufferStatus) \
    PROC(PFNGLUNIFORM1IPROC, glUniform1i) \
    PROC(PFNGLDRAWBUFFERSPROC, glDrawBuffers) \
    PROC(PFNGLUNIFORM4FPROC, glUniform4f) \
    PROC(PFNGLUNIFORM1UIPROC, glUniform1ui)
// TODO: there is something fishy with Windows gl.h header
// // Let's try to ship our own gl.h just like glext.h
// #ifdef _WIN32
// static PFNGLACTIVETEXTUREPROC glActiveTexture = NULL;
// #endif // _WIN32

#define PROC(type, name) static type name = NULL;
PROCS
#undef PROC

static bool load_gl_extensions(void)
{
    #define PROC(type, name) \
        name = (type) RGFW_getProcAddress(#name); \
        if (name == NULL) { \
            fprintf(stderr, "ERROR: could not load function %s\n", #name); \
            return false; \
        }
    PROCS
    #undef PROC
    return true;
}
