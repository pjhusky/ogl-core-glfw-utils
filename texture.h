#ifndef _TEXTURE_H_76ce320f_da67_4920_b1fc_ca6232466835
#define _TEXTURE_H_76ce320f_da67_4920_b1fc_ca6232466835

#include "eRetVal_GfxAPI.h"

#include <stddef.h>
#include <stdint.h>
#include <array>

namespace GfxAPI {
    struct Texture {

        using handle_t = intptr_t;

        using i32vec3_t = std::array<int32_t, 3>;

        enum class eChannelType {
            i8,
            //u8,
            i16,
            i32,
            f16,
            f32,
            i24depth,
            i32depth,
            f32depth,
        };

        enum class eSemantics { // depth and stencil can be logically OR'ed
            color = (1u << 0u),
            depth = (1u << 1u),
            stencil = (1u << 2u),
        };

        enum class eBorderMode {
            clamp,
            repeat,
            mirrorRepeat,
        };

        // enum class eCompareMode {
        //     refToTexture,
        //     none
        // };

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

        int32_t formatFromDesc();

        void bindToTexUnit( const int32_t texUnit );
        void bindDepthForVisToTexUnit( const int32_t texUnit );
        void unbindFromTexUnit( /* const int32_t texUnit */ );
        void unbindDepthForVisFromTexUnit( /* const int32_t texUnit */ );
        void createMipmaps();
        void uploadData( const void* pData, uint32_t format, uint32_t type, uint32_t mipLvl );

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
