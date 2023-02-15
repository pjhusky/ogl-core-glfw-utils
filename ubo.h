#ifndef _UBO_H_616e284b_9c0b_445d_8c14_d8323b01a2d4
#define _UBO_H_616e284b_9c0b_445d_8c14_d8323b01a2d4

#include <stdint.h>
#include <vector>

#include "apiAbstractions.h"

namespace GfxAPI {

    struct Ubo {

        struct Desc {
            uint32_t        numBytes;
        };

        explicit Ubo( const Desc& desc );
        ~Ubo();

        void bind( const bool shouldBind );
        handle_t handle() const { return mHandle; }
        Desc desc() const { return mDesc; }

        void* map( const GfxAPI::eMapMode mapMode );
        bool unmap();

    private:
        handle_t        mHandle;
        const Desc      mDesc;
        bool            mIsBound;
    };
}
#endif // _UBO_H_616e284b_9c0b_445d_8c14_d8323b01a2d4
