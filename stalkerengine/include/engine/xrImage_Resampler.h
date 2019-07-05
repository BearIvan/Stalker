#ifndef xrImage_ResamplerH
#define xrImage_ResamplerH

#pragma once

enum EIMF_Type
{
    imf_filter = 0,
    imf_box,
    imf_triangle,
    imf_bell,
    imf_b_spline,
    imf_lanczos3,
    imf_mitchell,

    imf_FORCEDWORD = 0xffffffff
};
ENGINE_API void imf_Process(u32* dst, bsize dstW, bsize dstH, u32* src, bsize srcW, bsize srcH, EIMF_Type FILTER);

#endif
