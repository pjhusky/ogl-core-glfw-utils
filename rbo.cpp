#include "texture.h"
#include "rbo.h"

#include "eRetVal_GfxAPI.h"
#include "checkErrorGL.h"

#include <string.h> // for memcpy & memset
#include <glad/glad.h>

using namespace GfxAPI;


Rbo::Rbo( const Rbo::Desc& desc )
    : mDesc( desc ) {

    // clear the entire handle mem, as the impl may only use a part of it, leaving the rest uninitialized otherwise
    memset( &mHandle, 0, sizeof( mHandle ) ); 

    // this step may only "fill" some of the allocated memory cell for the mHandle
    glGenRenderbuffers( 1, reinterpret_cast<GLuint*>( &mHandle ) );
}

Rbo::~Rbo() {
    glDeleteRenderbuffers( 1, reinterpret_cast<GLuint*>( &mHandle ) );
}

