#include "rbo.h"
#include "texture.h"
#include "fbo.h"

#include "eRetVal_GfxAPI.h"
#include "checkErrorGL.h"

#include <string.h> // for memcpy & memset
#include <assert.h>
#include <glad/glad.h>


GfxAPI::Rbo::Rbo( const Rbo::Desc& desc )
    : mDesc( desc ) {

    // clear the entire handle mem, as the impl may only use a part of it, leaving the rest uninitialized otherwise
    memset( &mHandle, 0, sizeof( mHandle ) ); 

    // this step may only "fill" some of the allocated memory cell for the mHandle
    glGenRenderbuffers( 1, reinterpret_cast<GLuint*>( &mHandle ) );
    glBindRenderbuffer( GL_RENDERBUFFER, static_cast<GLuint>( mHandle ) );
    glRenderbufferStorage( GL_RENDERBUFFER, toApiChannelType( mDesc.channelType, mDesc.numChannels ), mDesc.w, mDesc.h );
}

void GfxAPI::Rbo::attachToFbo( Fbo& fbo, const int32_t attachmentNumber ) {
    fbo.bind( true );
    if (mDesc.semantics == eSemantics::color) {
        glFramebufferRenderbuffer( GL_DRAW_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + attachmentNumber, GL_RENDERBUFFER, static_cast<GLuint>(mHandle) );
    } else {
        const bool isDepthAttachment = ( (static_cast<uint32_t>(mDesc.semantics) & static_cast<uint32_t>(eSemantics::depth)) != 0u );
        const bool isStencilAttachment = ( (static_cast<uint32_t>(mDesc.semantics) & static_cast<uint32_t>(eSemantics::stencil)) != 0u );
        uint32_t attachmentType = 0;

        if ( isDepthAttachment ) {
            if ( isStencilAttachment) {
                attachmentType = GL_DEPTH_STENCIL_ATTACHMENT;
            } else {
                attachmentType = GL_DEPTH_ATTACHMENT;
            }
        } else if (isStencilAttachment) {
            attachmentType = GL_STENCIL_ATTACHMENT;
        } else {
            assert( false ); 
        }

        glFramebufferRenderbuffer( GL_DRAW_FRAMEBUFFER, attachmentType, GL_RENDERBUFFER, static_cast<GLuint>(mHandle) );
    }
}

GfxAPI::Rbo::~Rbo() {
    glDeleteRenderbuffers( 1, reinterpret_cast<GLuint*>( &mHandle ) );
}

