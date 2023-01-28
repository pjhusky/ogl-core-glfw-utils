#ifndef _TEXTURE_H_76ce320f_da67_4920_b1fc_ca6232466835
#define _TEXTURE_H_76ce320f_da67_4920_b1fc_ca6232466835

#include "eRetVal_GfxAPI.h"
#include "apiAbstractions.h"

#include <stddef.h>
#include <stdint.h>
#include <array>



namespace GfxAPI {
    struct Texture {

        using i32vec3_t = std::array<int32_t, 3>;

        struct Desc_t {
            i32vec3_t           texDim;
            int32_t             numChannels;
            eChannelType        channelType;
            eSemantics          semantics;
            bool                isMipMapped;

            // Desc_t()

            // Desc_t( const linAlg::i32vec3_t& in_texDim,
            //         const int32_t            in_numChannels,
            //         const eChannelType       in_channelType,
            //         const eSemantics         in_semantics,
            //         const bool               in_isMipMapped )
            //     : texDim( in_texDim )
            //     , numChannels( in_numChannels )
            //     , channelType( in_channelType )
            //     , semantics( in_semantics )
            //     , isMipMapped( in_isMipMapped )
            // {}
        };

        Texture() { commonCtor(); }
        explicit Texture( const Desc_t& desc );
        ~Texture();

        eRetVal create( const Desc_t& desc );
        eRetVal destroy();


        Desc_t desc() const { return mDesc; }

        int32_t formatFromDesc() { return toApiChannelType( mDesc.channelType, mDesc.numChannels ); }

        void bindToTexUnit( const int32_t texUnit );
        void bindDepthForVisToTexUnit( const int32_t texUnit );
        void unbindFromTexUnit( /* const int32_t texUnit */ );
        void unbindDepthForVisFromTexUnit( /* const int32_t texUnit */ );
        void createMipmaps();
        void uploadData( const void* pData, uint32_t format, uint32_t type, int32_t mipLvl );

        void setWrapModeForDimension( eBorderMode borderMode, uint32_t dim );

        handle_t handle() const { return mHandle; }

        static void unbindAllTextures();

    private:

        void commonCtor();

        handle_t            mHandle;
        Desc_t              mDesc;
        int32_t             mBoundTexUnit;
        int32_t             mTexTarget;

        bool                mIsValid;
    };
}
#endif //_TEXTURE_H_76ce320f_da67_4920_b1fc_ca6232466835
