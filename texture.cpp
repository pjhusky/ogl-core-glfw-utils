#include "texture.h"
#include "eRetVal_GfxAPI.h"
#include "checkErrorGL.h"

#include <glad/glad.h>

#include <string.h> // for memcpy & memset
#include <cassert>

using namespace GfxAPI;

Texture::Texture( const Texture::Desc_t& desc ) {
    commonCtor();
    create( desc );
}

void Texture::commonCtor() {
    // clear the entire handle mem, as the impl may only use a part of it, leaving the rest uninitialized otherwise
    memset( &mHandle, 0, sizeof( mHandle ) );
    mBoundTexUnit = -1;
    mIsValid = false;
}

Texture::~Texture() {
    destroy();
}

eRetVal Texture::create( const Desc_t& desc ) {
    mDesc = desc;
    glCheckError();

    glGenTextures( 1, reinterpret_cast< GLuint* >( &mHandle ) );


    int32_t dataPtrFormat = 
        ( ( mDesc.numChannels == 1 ) ? GL_RED : ( ( mDesc.numChannels == 2 ) ? GL_RG : ( ( mDesc.numChannels == 3 ) ? GL_RGB : GL_RGBA ) ) );
    
    int32_t dataPtrType = GL_UNSIGNED_BYTE; 
    switch (mDesc.channelType) {
    case eChannelType::f16:
    case eChannelType::f32:
        dataPtrType = GL_FLOAT;
        break;
    case eChannelType::i16:
        dataPtrType = GL_SHORT;
        break;
    case eChannelType::i32:
        dataPtrType = GL_INT;
        break;
    }

    if ( ( static_cast< uint32_t >( mDesc.semantics ) & static_cast< uint32_t >( eSemantics::depth ) ) != 0 ) { 
        dataPtrFormat = GL_DEPTH_COMPONENT; 
        dataPtrType = GL_FLOAT; 
    }

    glCheckError();

    bool hasDimZ = ( mDesc.texDim[ 2 ] > 0 );   
    bool hasDimY = ( mDesc.texDim[ 1 ] > 0 );
    int32_t texTarget = ( hasDimZ ? GL_TEXTURE_3D : ( hasDimY ? GL_TEXTURE_2D : GL_TEXTURE_1D ) ); // TODO cube maps and texture arrays!
    mTexTarget = texTarget;
    assert( texTarget == GL_TEXTURE_2D || texTarget == GL_TEXTURE_3D ); // TODO: actually implement 1D and 3D textures

    glCheckError();

    // https://www.khronos.org/opengl/wiki/Common_Mistakes ==> Texture upload and pixel reads
    // GPU prefers RGBA8 over RGB8
    // can still grab image as RGB8 for storage in a 24bit ppm file

    bindToTexUnit( 0 );

    glCheckError();

    constexpr int32_t mipLvl = 0;
    constexpr int32_t border = 0;
    int32_t format = formatFromDesc();

    if (texTarget == GL_TEXTURE_2D) {
        glTexImage2D( texTarget, mipLvl, format, mDesc.texDim[0], mDesc.texDim[1], border, dataPtrFormat, dataPtrType, nullptr );
    } else if (texTarget == GL_TEXTURE_3D) {
        glTexImage3D( texTarget, mipLvl, format, mDesc.texDim[0], mDesc.texDim[1], mDesc.texDim[2], border, dataPtrFormat, dataPtrType, nullptr );
    }

    glCheckError();

#if 0
    int32_t w;
    glGetTexLevelParameteriv( GL_TEXTURE_2D, mipLvl, GL_TEXTURE_WIDTH, &w );
    assert( w == renderTargetW );
    int32_t h;
    glGetTexLevelParameteriv( GL_TEXTURE_2D, mipLvl, GL_TEXTURE_HEIGHT, &h );
    assert( h == renderTargetH );
#endif

    const int32_t minFilter = ( mDesc.isMipMapped ) ? GL_LINEAR_MIPMAP_LINEAR : GL_LINEAR;
    glTexParameteri( texTarget, GL_TEXTURE_MIN_FILTER, minFilter );
    glTexParameteri( texTarget, GL_TEXTURE_MAG_FILTER, GL_LINEAR );

    glCheckError();

    if ( mDesc.isMipMapped ) { glGenerateMipmap( texTarget ); }

    glCheckError();

    mIsValid = true;

    return eRetVal::OK;
}

eRetVal Texture::destroy() {
    if (mHandle == 0) { return eRetVal::OK; }

    glDeleteTextures( 1, reinterpret_cast< GLuint* >( &mHandle ) );
    mIsValid = false;
    memset( &mHandle, 0, sizeof( mHandle ) );
    mHandle = -1;

    return eRetVal::OK;
}

void Texture::bindToTexUnit( const int32_t texUnit ) {
    if ( mBoundTexUnit >= 0 ) {
        unbindFromTexUnit();
    }

    glCheckError();

    glActiveTexture( GL_TEXTURE0 + texUnit );

    glCheckError();

    glBindTexture( mTexTarget, static_cast<GLuint>(mHandle) );

    glCheckError();

    mBoundTexUnit = texUnit;
}

void Texture::bindDepthForVisToTexUnit( const int32_t texUnit ) {
    bindToTexUnit( texUnit );
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_COMPARE_MODE, GL_NONE );
}

void Texture::unbindFromTexUnit( /* const int32_t texUnit */ ) {
    if ( mBoundTexUnit < 0 ) { return; }

    glCheckError();

    glActiveTexture( GL_TEXTURE0 + mBoundTexUnit );
    
    glCheckError();
    
    glBindTexture( mTexTarget, 0 );
    
    glCheckError();

    mBoundTexUnit = -1;    
}

void Texture::unbindDepthForVisFromTexUnit( /* const int32_t texUnit */ ) {
    if ( mBoundTexUnit < 0 ) { return; }

    glActiveTexture( GL_TEXTURE0 + mBoundTexUnit );
    glBindTexture( mTexTarget, static_cast<GLuint>(mHandle) );
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_COMPARE_MODE, GL_NONE );

    glBindTexture( mTexTarget, 0 );
    mBoundTexUnit = -1;    
}

void Texture::createMipmaps() {
    if ( !mDesc.isMipMapped ) { return; }

    if ( mBoundTexUnit < 0 ) {
        bindToTexUnit( 0 );
    }
    glGenerateMipmap( mTexTarget );
}

void Texture::uploadData( const void* pData, uint32_t format, uint32_t type, uint32_t mipLvl ) {
    glCheckError();
    this->bindToTexUnit( 0 );
    glCheckError();
    if (mTexTarget == GL_TEXTURE_2D) {
        glTexSubImage2D( mTexTarget, mipLvl, 0, 0, mDesc.texDim[0], mDesc.texDim[1], format, type, pData );
    } else if (mTexTarget == GL_TEXTURE_3D) {
        glTexSubImage3D( mTexTarget, mipLvl, 0, 0, 0, mDesc.texDim[0], mDesc.texDim[1], mDesc.texDim[2], format, type, pData );
    }
    glCheckError();
    this->unbindFromTexUnit();
    glCheckError();
}

void Texture::setWrapModeForDimension( eBorderMode borderMode, uint32_t dim ) {
    glCheckError();
    this->bindToTexUnit( 0 );
    uint32_t glDim = GL_TEXTURE_WRAP_S;
    switch (dim) {
    case 0:
        glDim = GL_TEXTURE_WRAP_S;
        break;
    case 1:
        glDim = GL_TEXTURE_WRAP_T;
        break;
    case 2:
        glDim = GL_TEXTURE_WRAP_R;
        break;
    };

    uint32_t glBorderMode = GL_CLAMP_TO_BORDER;
    switch (borderMode) {
    case eBorderMode::clamp:
        glBorderMode = GL_CLAMP_TO_BORDER;
        break;
    case eBorderMode::mirrorRepeat:
        glBorderMode = GL_MIRRORED_REPEAT;
        break;
    case eBorderMode::repeat:
        glBorderMode = GL_REPEAT;
        break;
    };

    glTexParameteri( mTexTarget, glDim, glBorderMode );
    this->unbindFromTexUnit();
    glCheckError();
}

void Texture::unbindAllTextures() {
    int32_t maxTextureImageUnits;
    glGetIntegerv( GL_MAX_TEXTURE_IMAGE_UNITS, &maxTextureImageUnits );
    //printf( "maxTextureImageUnits = %d\n", maxTextureImageUnits );
    for ( int32_t i = maxTextureImageUnits - 1; i >= 0; i-- ) {
        glActiveTexture( GL_TEXTURE0 + i );
        glBindTexture( GL_TEXTURE_2D, 0 );
    }
}
