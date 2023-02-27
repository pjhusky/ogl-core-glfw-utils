#ifndef _VBO_H_E70DF549_25AE_48FF_9C60_50233956A34D
#define _VBO_H_E70DF549_25AE_48FF_9C60_50233956A34D

#include "apiAbstractions.h"

namespace GfxAPI {

    struct Vbo {

        struct Desc {
            uint32_t        numBytes;
        };

        explicit Vbo( const Desc& desc );
        ~Vbo();

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

#endif // _VBO_H_E70DF549_25AE_48FF_9C60_50233956A34D
