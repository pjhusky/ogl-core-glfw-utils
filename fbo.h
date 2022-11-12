#ifndef _FBO_H_616e284b_9c0b_445d_8c14_d8323b01a2d4
#define _FBO_H_616e284b_9c0b_445d_8c14_d8323b01a2d4

#include <cstdint>

struct Texture;

struct Fbo {

    using handle_t = intptr_t;

    struct Desc {

    };

    explicit Fbo( const Desc& desc );
    ~Fbo();

    private:
    handle_t        mHandle;
    const Desc      mDesc;
};

#endif // _FBO_H_616e284b_9c0b_445d_8c14_d8323b01a2d4
