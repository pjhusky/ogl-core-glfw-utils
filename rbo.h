#ifndef _RBO_H_616e284b_9c0b_445d_8c14_d8323b01a2d4
#define _RBO_H_616e284b_9c0b_445d_8c14_d8323b01a2d4

#include <stdint.h>
#include <vector>

#include "apiAbstractions.h"

namespace GfxAPI {

    struct Fbo;

    struct Rbo {

        struct Desc {
            uint32_t        w;
            uint32_t        h;
            int32_t         numChannels;
            eChannelType    channelType;
            eSemantics      semantics;
        };

        explicit Rbo( const Desc& desc );
        ~Rbo();

        void attachToFbo( Fbo& fbo, const int32_t attachmentNumber );

    private:
        handle_t        mHandle;
        const Desc      mDesc;
    };
}
#endif // _RBO_H_616e284b_9c0b_445d_8c14_d8323b01a2d4
