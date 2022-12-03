#ifndef _RBO_H_616e284b_9c0b_445d_8c14_d8323b01a2d4
#define _RBO_H_616e284b_9c0b_445d_8c14_d8323b01a2d4

#include <stdint.h>
#include <vector>

namespace GfxAPI {

    struct Rbo {

        struct Desc {
            std::vector< Texture* > colorAttachments;
            Texture*                depthStencilAttachment;
        };

        explicit Rbo( const Desc& desc );
        ~Rbo();

    private:
        handle_t        mHandle;
        const Desc      mDesc;
    };
}
#endif // _RBO_H_616e284b_9c0b_445d_8c14_d8323b01a2d4
