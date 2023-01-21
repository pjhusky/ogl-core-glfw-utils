#include "texture.h"
#include "fbo.h"

#include "eRetVal_GfxAPI.h"
#include "checkErrorGL.h"

#include <string.h> // for memcpy & memset
#include <glad/glad.h>

using namespace GfxAPI;


Fbo::Fbo( const Fbo::Desc& desc ) 
    : mDesc( desc )
    , mIsBound( false ) {

    // clear the entire handle mem, as the impl may only use a part of it, leaving the rest uninitialized otherwise
    memset( &mHandle, 0, sizeof( mHandle ) ); 

    // this step may only "fill" some of the allocated memory cell for the mHandle
    glGenFramebuffers( 1, reinterpret_cast<GLuint*>( &mHandle ) ); ( 1, reinterpret_cast<GLuint*>( &mHandle ) );

    bind( false );
}

void Fbo::bind( const bool shouldBind ) {
    if (shouldBind) {
        glBindFramebuffer( GL_FRAMEBUFFER, static_cast<GLuint>(mHandle) );
    } else {
        glBindFramebuffer( GL_FRAMEBUFFER, 0 );
    }
    mIsBound = shouldBind;
}

Fbo::~Fbo() {
    glDeleteFramebuffers( 1, reinterpret_cast<GLuint*>( &mHandle ) );
}

