#include "pch.h"

XRayFProgressive::XRayFProgressive()
{
    xSWI = 0;
}

XRayFProgressive::~XRayFProgressive()
{

}
void 		XRayFProgressive::Release()
{
    xr_free(nSWI.sw);
    if (xSWI) {
        xr_free(xSWI->sw);
        xr_delete(xSWI);
        xSWI = 0;

    }
}
void XRayFProgressive::Load(const char* N, IReader* data, u32 dwFlags)
{
    XRayFVisual::Load(N, data, dwFlags);

    // normal SWI
    destructor<IReader> lods(data->open_chunk(OGF_SWIDATA));
    nSWI.reserved[0] = lods().r_u32();	// reserved 16 bytes
    nSWI.reserved[1] = lods().r_u32();
    nSWI.reserved[2] = lods().r_u32();
    nSWI.reserved[3] = lods().r_u32();
    nSWI.count = lods().r_u32();
    VERIFY(NULL == nSWI.sw); 
    nSWI.sw = xr_alloc<FSlideWindow>(nSWI.count);
    lods().r(nSWI.sw, nSWI.count * sizeof(FSlideWindow));
}
#define PCOPY(a)	a = pFrom->a
void XRayFProgressive::Copy(XRayRenderVisual* from)
{
    XRayFVisual::Copy(from);
    XRayFProgressive* pFrom = (XRayFProgressive*)from;
    PCOPY(nSWI);
    PCOPY(xSWI);
}
