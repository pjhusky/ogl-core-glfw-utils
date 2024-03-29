#include "apiAbstractions.h"

#include <glad/glad.h>


int32_t GfxAPI::toApiChannelType( const eChannelType channelType, const int32_t numChannels ) {
    if (channelType == eChannelType::i24depth) { return GL_DEPTH_COMPONENT24; }
    if (channelType == eChannelType::i32depth) { return GL_DEPTH_COMPONENT32; }
    if (channelType == eChannelType::f32depth) { return GL_DEPTH_COMPONENT32F; }

    if (numChannels == 1) {
        if (channelType == eChannelType::i8  || channelType == eChannelType::u8 ) { return GL_R8; }
        if (channelType == eChannelType::i16 || channelType == eChannelType::u16) { return GL_R16; }
        if (channelType == eChannelType::i32 || channelType == eChannelType::u32) { return GL_R32I; }
        if (channelType == eChannelType::f16                                    ) { return GL_R16F; }
        if (channelType == eChannelType::f32                                    ) { return GL_R32F; }
    }
    else if (numChannels == 2) {
        if (channelType == eChannelType::i8  || channelType == eChannelType::u8 ) { return GL_RG8; }
        if (channelType == eChannelType::i16 || channelType == eChannelType::u16) { return GL_RG16; }
        if (channelType == eChannelType::i32 || channelType == eChannelType::u32) { return GL_RG32I; }
        if (channelType == eChannelType::f16                                    ) { return GL_RG16F; }
        if (channelType == eChannelType::f32                                    ) { return GL_RG32F; }
    }
    else if (numChannels == 3) {
        if (channelType == eChannelType::i8  || channelType == eChannelType::u8 ) { return GL_RGB8; }
        if (channelType == eChannelType::i16 || channelType == eChannelType::u16) { return GL_RGB16; }
        if (channelType == eChannelType::i32 || channelType == eChannelType::u32) { return GL_RGB32I; }
        if (channelType == eChannelType::f16                                    ) { return GL_RGB16F; }
        if (channelType == eChannelType::f32                                    ) { return GL_RGB32F; }
    }
    else if (numChannels == 4) {
        if (channelType == eChannelType::i8  || channelType == eChannelType::u8 ) { return GL_RGBA8; }
        if (channelType == eChannelType::i16 || channelType == eChannelType::u16) { return GL_RGBA16; }
        if (channelType == eChannelType::i32 || channelType == eChannelType::u32) { return GL_RGBA32I; }
        if (channelType == eChannelType::f16                                      ) { return GL_RGBA16F; }
        if (channelType == eChannelType::f32                                      ) { return GL_RGBA32F; }
    }
    return -1;
}

int32_t GfxAPI::toApiFormatFromNumChannels( const int32_t numChannels ) {
    return ((numChannels == 1) ? GL_RED : ((numChannels == 2) ? GL_RG : ((numChannels == 3) ? GL_RGB : GL_RGBA)));
}

int32_t GfxAPI::toApiDataChannelType( const eChannelType channelType ) {
    int32_t dataPtrType = GL_UNSIGNED_BYTE; 
    switch (channelType) {
    case eChannelType::i8:
        dataPtrType = GL_BYTE;
        break;
    case eChannelType::f16:
        dataPtrType = GL_HALF_FLOAT;
        break;
    case eChannelType::f32:
        dataPtrType = GL_FLOAT;
        break;
    case eChannelType::i16:
        dataPtrType = GL_SHORT;
        break;
    case eChannelType::u16:
        dataPtrType = GL_UNSIGNED_SHORT;
        break;
    case eChannelType::i32:
        dataPtrType = GL_INT;
        break;
    case eChannelType::u32:
        dataPtrType = GL_UNSIGNED_INT;
        break;
    }
    return dataPtrType;
}
