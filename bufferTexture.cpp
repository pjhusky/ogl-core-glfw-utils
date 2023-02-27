#include "bufferTexture.h"

#include "eRetVal_GfxAPI.h"
#include "checkErrorGL.h"

#include <string.h> // for memcpy & memset
#include <assert.h>
#include <glad/glad.h>


GfxAPI::BufferTexture::BufferTexture( const BufferTexture::Desc& desc )
    : mDesc( desc )
    //, mBackingVbo( { .numBytes = desc.numBytes } ) 
    , mpBackingVbo( nullptr ) {

    // clear the entire handle mem, as the impl may only use a part of it, leaving the rest uninitialized otherwise
    memset( &mHandle, 0, sizeof( mHandle ) ); 
    glGenTextures( 1, reinterpret_cast<GLuint*>( &mHandle ) );
}

GfxAPI::BufferTexture::~BufferTexture() {
    glDeleteTextures( 1, reinterpret_cast< GLuint* >( &mHandle ) );
    detachVbo();
}

void GfxAPI::BufferTexture::bindToTexUnit( const int32_t texUnit ) {
    if ( mBoundTexUnit >= 0 ) {
        unbindFromTexUnit();
    }

    glCheckError();

    glActiveTexture( GL_TEXTURE0 + texUnit );
    glBindTexture( GL_TEXTURE_BUFFER, static_cast<GLuint>(mHandle) );

    glCheckError();

    mBoundTexUnit = texUnit;
}

void GfxAPI::BufferTexture::unbindFromTexUnit() {
    if (mBoundTexUnit < 0) { return; }

    glCheckError();

    glActiveTexture( GL_TEXTURE0 + mBoundTexUnit );
    glBindTexture( GL_TEXTURE_BUFFER, 0 );

    glCheckError();

    mBoundTexUnit = -1;
}

void GfxAPI::BufferTexture::attachVbo( Vbo* pVbo ) {
    mpBackingVbo = pVbo;

    bindToTexUnit( 0 );
    //glTexBuffer( GL_TEXTURE_BUFFER, GL_R32F, static_cast<GLuint>( mpBackingVbo->handle() ) );
    glTexBuffer( GL_TEXTURE_BUFFER, GL_RGBA32F, static_cast<GLuint>( mpBackingVbo->handle() ) );
    unbindFromTexUnit();
}

void GfxAPI::BufferTexture::detachVbo() {
    mpBackingVbo = nullptr;

    bindToTexUnit( 0 );
    glTexBuffer( GL_TEXTURE_BUFFER, GL_R32F, 0 );
    unbindFromTexUnit();
}

void* GfxAPI::BufferTexture::map( const GfxAPI::eMapMode mapMode ) {
    //if (mapMode == GfxAPI::eMapMode::readOnly) {
    //    return glMapBuffer( GL_UNIFORM_BUFFER, GL_READ_ONLY );
    //} else if (mapMode == GfxAPI::eMapMode::writeOnly) {
    //    return glMapBuffer( GL_UNIFORM_BUFFER, GL_WRITE_ONLY );
    //} else if (mapMode == GfxAPI::eMapMode::readWrite) {
    //    return glMapBuffer( GL_UNIFORM_BUFFER, GL_READ_WRITE );
    //}
    //return nullptr;
    if (mpBackingVbo == nullptr) { return nullptr; }
    return mpBackingVbo->map( mapMode );
}

bool GfxAPI::BufferTexture::unmap() {
    //return glUnmapBuffer( GL_UNIFORM_BUFFER );
    if (mpBackingVbo == nullptr) { return false; }
    return mpBackingVbo->unmap();
}

