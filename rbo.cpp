#include "rbo.h"
#include "texture.h"
#include "fbo.h"

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
    glBindRenderbuffer( GL_RENDERBUFFER, mHandle );
    glRenderbufferStorage( GL_RENDERBUFFER, toApiChannelType( mDesc.channelType, mDesc.numChannels ), mDesc.w, mDesc.h );
}

void Rbo::attachToFbo( Fbo& fbo, const int32_t attachmentNumber ) {
    fbo.bind( true );
    if (mDesc.semantics == eSemantics::color) {
        glFramebufferRenderbuffer( GL_DRAW_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + attachmentNumber, GL_RENDERBUFFER, mHandle );
    } else {
        glFramebufferRenderbuffer( GL_DRAW_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, mHandle );
    }
}

Rbo::~Rbo() {
    glDeleteRenderbuffers( 1, reinterpret_cast<GLuint*>( &mHandle ) );
}

