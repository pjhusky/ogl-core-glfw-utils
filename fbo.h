#ifndef _FBO_H_616e284b_9c0b_445d_8c14_d8323b01a2d4
#define _FBO_H_616e284b_9c0b_445d_8c14_d8323b01a2d4

#include "eRetVal_GfxAPI.h"
#include "apiAbstractions.h"

#include <stdint.h>
#include <vector>

namespace GfxAPI {
    struct Texture;
    struct Rbo;

    struct Fbo {

        struct Desc {
            std::vector< Texture* > colorAttachments;
            Texture*                depthStencilAttachment = nullptr;
        };

        explicit Fbo( const Desc& desc );
        ~Fbo();

        void bind( const bool shouldBind );

    private:
        handle_t        mHandle;
        const Desc      mDesc;
        bool            mIsBound;
    };
}
#endif // _FBO_H_616e284b_9c0b_445d_8c14_d8323b01a2d4
