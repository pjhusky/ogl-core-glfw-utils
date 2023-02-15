#include "ubo.h"

#include "eRetVal_GfxAPI.h"
#include "checkErrorGL.h"

#include <string.h> // for memcpy & memset
#include <assert.h>
#include <glad/glad.h>


GfxAPI::Ubo::Ubo( const Ubo::Desc& desc )
    : mDesc( desc ) {

    // clear the entire handle mem, as the impl may only use a part of it, leaving the rest uninitialized otherwise
    memset( &mHandle, 0, sizeof( mHandle ) ); 

    // this step may only "fill" some of the allocated memory cell for the mHandle
    glGenBuffers( 1, reinterpret_cast<GLuint*>( &mHandle ) );
    glBindBuffer( GL_UNIFORM_BUFFER, static_cast<GLuint>( mHandle ) );
    glBufferData( GL_UNIFORM_BUFFER, mDesc.numBytes, NULL, GL_STATIC_DRAW );

    bind(false);
}

GfxAPI::Ubo::~Ubo() {
    glDeleteBuffers( 1, reinterpret_cast<GLuint*>( &mHandle ) );
}

void GfxAPI::Ubo::bind( const bool shouldBind ) {
    //if (shouldBind) {
    //    glBindBuffer( GL_UNIFORM_BUFFER, static_cast<GLuint>(mHandle) );
    //} else {
    //    glBindBuffer( GL_UNIFORM_BUFFER, 0 );
    //}
    //mIsBound = shouldBind;
}

void* GfxAPI::Ubo::map( const GfxAPI::eMapMode mapMode ) {
    if (mapMode == GfxAPI::eMapMode::readOnly) {
        return glMapBuffer( GL_UNIFORM_BUFFER, GL_READ_ONLY );
    } else if (mapMode == GfxAPI::eMapMode::writeOnly) {
        return glMapBuffer( GL_UNIFORM_BUFFER, GL_WRITE_ONLY );
    } else if (mapMode == GfxAPI::eMapMode::readWrite) {
        return glMapBuffer( GL_UNIFORM_BUFFER, GL_READ_WRITE );
    }
}

bool GfxAPI::Ubo::unmap() {
    return glUnmapBuffer( GL_UNIFORM_BUFFER );
}

