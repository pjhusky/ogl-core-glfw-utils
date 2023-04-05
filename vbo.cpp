#include "vbo.h"

#include "eRetVal_GfxAPI.h"
#include "checkErrorGL.h"

#include <string.h> // for memcpy & memset
#include <assert.h>
#include <glad/glad.h>

void GfxAPI::Vbo::commonCtor() {
    // clear the entire handle mem, as the impl may only use a part of it, leaving the rest uninitialized otherwise
    memset( &mHandle, 0, sizeof( mHandle ) ); 

    // this step may only "fill" some of the allocated memory cell for the mHandle
    glGenBuffers( 1, reinterpret_cast<GLuint*>( &mHandle ) );
}

//GfxAPI::Vbo::Vbo() {
//    commonCtor();
//}

GfxAPI::Vbo::Vbo( const Vbo::Desc& desc )
    : mDesc( desc ) {
    commonCtor();
    create( desc );
}

void GfxAPI::Vbo::create( const Vbo::Desc& desc ) {
    bind( true );
    glBufferData( GL_ARRAY_BUFFER, mDesc.numBytes, NULL, GL_STATIC_DRAW );
    bind(false);
}

GfxAPI::Vbo::~Vbo() {
    glDeleteBuffers( 1, reinterpret_cast<GLuint*>( &mHandle ) );
}

void GfxAPI::Vbo::bind( const bool shouldBind ) {
    if (shouldBind) {
        glBindBuffer( GL_ARRAY_BUFFER, static_cast<GLuint>(mHandle) );
    } else {
        glBindBuffer( GL_ARRAY_BUFFER, 0 );
    }
    //mIsBound = shouldBind;
}

void* GfxAPI::Vbo::map( const GfxAPI::eMapMode mapMode ) {
    bind( true );
    if (mapMode == GfxAPI::eMapMode::readOnly) {
        return glMapBuffer( GL_ARRAY_BUFFER, GL_READ_ONLY );
    } else if (mapMode == GfxAPI::eMapMode::writeOnly) {
        return glMapBuffer( GL_ARRAY_BUFFER, GL_WRITE_ONLY );
    } else if (mapMode == GfxAPI::eMapMode::readWrite) {
        return glMapBuffer( GL_ARRAY_BUFFER, GL_READ_WRITE );
    }
    return nullptr;
}

bool GfxAPI::Vbo::unmap() {
    
    return glUnmapBuffer( GL_ARRAY_BUFFER );
}

