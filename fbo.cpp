#include "texture.h"
#include "fbo.h"

#include "eRetVal_GfxAPI.h"
#include "checkErrorGL.h"

#include <string.h> // for memcpy & memset
#include <assert.h>
#include <glad/glad.h>

using namespace GfxAPI;


Fbo::Fbo( const Fbo::Desc& desc ) 
    : mDesc( desc )
    , mIsBound( false ) {

    // clear the entire handle mem, as the impl may only use a part of it, leaving the rest uninitialized otherwise
    memset( &mHandle, 0, sizeof( mHandle ) ); 

    // this step may only "fill" some of the allocated memory cell for the mHandle
    glGenFramebuffers( 1, reinterpret_cast<GLuint*>( &mHandle ) ); 

    bind( true );

    glBindTexture( GL_TEXTURE_2D, 0 );
    for (uint32_t i = 0; i < static_cast<uint32_t>(mDesc.colorAttachments.size()); i++) {
        glFramebufferTexture( GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + i, static_cast<GLuint>(mDesc.colorAttachments[i]->handle()), 0 );
        
        //glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + i, GL_TEXTURE_2D, static_cast<GLuint>(mDesc.colorAttachments[i]->handle()), 0);  
    }

    if (mDesc.depthStencilAttachment != nullptr) {
        const bool isDepthAttachment = ( (static_cast<uint32_t>(mDesc.depthStencilAttachment->desc().semantics) & static_cast<uint32_t>(eSemantics::depth)) != 0u );
        const bool isStencilAttachment = ( (static_cast<uint32_t>(mDesc.depthStencilAttachment->desc().semantics) & static_cast<uint32_t>(eSemantics::stencil)) != 0u );
        if ( isDepthAttachment ) {
            if ( isStencilAttachment) {
                glFramebufferRenderbuffer( GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, static_cast<GLuint>(mDesc.depthStencilAttachment->handle()) );
            } else {
                glFramebufferRenderbuffer( GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, static_cast<GLuint>(mDesc.depthStencilAttachment->handle()) );
            }
        } else if (isStencilAttachment) {
            glFramebufferRenderbuffer( GL_FRAMEBUFFER, GL_STENCIL_ATTACHMENT, GL_RENDERBUFFER, static_cast<GLuint>(mDesc.depthStencilAttachment->handle()) );
        } else {
            assert( false ); 
        }
        
    }

    const auto checkResult = glCheckFramebufferStatus( GL_FRAMEBUFFER );
    if (checkResult != GL_FRAMEBUFFER_COMPLETE) {
        printf( "\n   ## FBO incomplete!!! ## \n" );
    }

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

