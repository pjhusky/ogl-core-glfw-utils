#ifndef _CHECK_GL_ERROR_H_0277146d_c31c_426c_963a_55054ce367f3
#define _CHECK_GL_ERROR_H_0277146d_c31c_426c_963a_55054ce367f3

#include <stdint.h>

namespace GfxAPI {
    int32_t glCheckError_( const char *file, const int line );
}


#ifndef VERBOSE_GFX_DEBUG
    #if defined( DEBUG )
        #define VERBOSE_GFX_DEBUG   1
    #else
        #define VERBOSE_GFX_DEBUG   0
    #endif
#endif

#if ( VERBOSE_GFX_DEBUG != 0 )
    #define glCheckError() GfxAPI::glCheckError_(__FILE__, __LINE__) 
#else
    #define glCheckError()
#endif

#endif // _CHECK_GL_ERROR_H_0277146d_c31c_426c_963a_55054ce367f3