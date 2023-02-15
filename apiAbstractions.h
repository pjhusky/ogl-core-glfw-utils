#ifndef _API_ABSTRACTIONS_H_616e284b_9c0b_445d_8c14_d8323b01a2d4
#define _API_ABSTRACTIONS_H_616e284b_9c0b_445d_8c14_d8323b01a2d4

#include "stdint.h"

namespace GfxAPI {

    using handle_t = intptr_t;

    enum class eChannelType {
        i8,
        u8,
        i16,
        u16,
        i32,
        u32,
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
        clampToEdge,
        repeat,
        mirrorRepeat,
    };

    enum class eMapMode {
        readOnly,
        writeOnly,
        readWrite,
    };

    int32_t toApiChannelType( const eChannelType channelType, const int32_t numChannels );
}

#endif // _API_ABSTRACTIONS_H_616e284b_9c0b_445d_8c14_d8323b01a2d4
