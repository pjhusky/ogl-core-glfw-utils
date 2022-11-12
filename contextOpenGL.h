#ifndef _CONTEXT_OPENGL_H_0277146d_c31c_426c_963a_55054ce367f3
#define _CONTEXT_OPENGL_H_0277146d_c31c_426c_963a_55054ce367f3

#include <utility> // for std::pair
#include <string>
#include <cstdint>

#include <glad/glad.h>

#include "statusType.h"


struct ContextOpenGL {

    ~ContextOpenGL();

    struct Settings_t {
        int32_t     windowW;
        int32_t     windowH;
        int32_t     glMajor;
        int32_t     glMinor;
        std::string windowTitle;
    };

    const Status_t init( const Settings_t& settings );

    void *const window() const { return mpWindow; }

    private:
    void* mpWindow;
};

#endif // _CONTEXT_OPENGL_H_0277146d_c31c_426c_963a_55054ce367f3
