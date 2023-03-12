#ifndef _BUFFER_TEXTURE_H_0B1702B0_F38D_490A_9D1F_69006D029BB9
#define _BUFFER_TEXTURE_H_0B1702B0_F38D_490A_9D1F_69006D029BB9

#include "apiAbstractions.h"

#include "vbo.h"

namespace GfxAPI {

    struct BufferTexture {

        struct Desc {
            //uint32_t        numBytes;
        };

        explicit BufferTexture( const Desc& desc );
        ~BufferTexture();

        void bindToTexUnit( const int32_t texUnit );
        void unbindFromTexUnit();
        handle_t handle() const { return mHandle; }
        Desc desc() const { return mDesc; }

        void attachVbo( Vbo* pVbo );
        void detachVbo();
        Vbo* attachedVbo() const { return mpBackingVbo; }

        void* map( const GfxAPI::eMapMode mapMode );
        bool unmap();

    private:
        handle_t        mHandle;
        Vbo*            mpBackingVbo;
        const Desc      mDesc;

        int32_t         mBoundTexUnit;
    };
}


#endif // _BUFFER_TEXTURE_H_0B1702B0_F38D_490A_9D1F_69006D029BB9
