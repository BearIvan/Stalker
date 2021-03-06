#include "stdafx.h"
#pragma hdrstop

using namespace std;

void XrSharedMemoryContainer::Initialize()
{
	cs = bear_new< xrCriticalSection>();
	container = bear_new< xr_vector<XrSharedMemoryValue*>>();
}

void XrSharedMemoryContainer::Destroy()
{
    clean();
	bear_delete(cs);
	bear_delete(container);


}
xrCriticalSection *XrSharedMemoryContainer::cs =0;
xr_vector<XrSharedMemoryValue*> *XrSharedMemoryContainer::container= 0;

XrSharedMemoryValue* XrSharedMemoryContainer::dock(u32 dwCRC, bsize dwLength, void* ptr)
{
    VERIFY(dwCRC && dwLength && ptr);

    cs->Enter();
    XrSharedMemoryValue* result = 0;

    // search a place to insert
    u8 storage[sizeof(XrSharedMemoryValue)];
    XrSharedMemoryValue* value = (XrSharedMemoryValue*)storage;
    value->dwReference = 0;
    value->dwCRC = dwCRC;
    value->dwLength = dwLength;
    cdb::iterator it = std::lower_bound(container->begin(), container->end(), value, smem_search);
    cdb::iterator saved_place = it;
    if (container->end() != it)
    {
        // supposedly found
        for (;; it++)
        {
            if (it == container->end()) break;
            if ((*it)->dwCRC != dwCRC) break;
            if ((*it)->dwLength != dwLength) break;
            if (0 == memcmp((*it)->value, ptr, dwLength))
            {
                // really found
                result = *it;
                break;
            }
        }
    }

    // if not found - create new entry
    if (0 == result)
    {
        result = (XrSharedMemoryValue*)BearMemory::Malloc(sizeof(XrSharedMemoryValue) + dwLength
                                               , "storage: smem"
                                              );
        result->dwReference = 0;
        result->dwCRC = dwCRC;
        result->dwLength = dwLength;
        CopyMemory((uint8*)result+ sizeof(XrSharedMemoryValue), ptr, dwLength);
		result->value = (uint8*)result + sizeof(XrSharedMemoryValue);
        container->insert(saved_place, result);
    }

    // exit
    cs->Leave();
    return result;
}

void XrSharedMemoryContainer::clean()
{
    cs->Enter();
    cdb::iterator it = container->begin();
    cdb::iterator end = container->end();
    for (; it != end; it++) if (0 == (*it)->dwReference) xr_free(*it);
    container->erase(remove(container->begin(), container->end(), (XrSharedMemoryValue*)0), container->end());
    if (container->empty()) container->clear();
    cs->Leave();
}

void XrSharedMemoryContainer::dump()
{
	return;
   /* cs->Enter();
    cdb::iterator it = container->begin();
    cdb::iterator end = container->end();
    FILE* F = fopen("x:\\$smem_dump$.txt", "w");
    for (; it != end; it++)
        fprintf(F, "%4u : crc[%6x], %u bytes\n", (*it)->dwReference, (*it)->dwCRC, (*it)->dwLength);
    fclose(F);
    cs->Leave();*/
}

u32 XrSharedMemoryContainer::stat_economy()
{
    cs->Enter();
    cdb::iterator it = container->begin();
    cdb::iterator end = container->end();
    s64 counter = 0;
    counter -= sizeof(XrSharedMemoryContainer);
    counter -= sizeof(cdb::allocator_type);
    const int node_size = 20;
    for (; it != end; it++)
    {
        counter -= 16;
        counter -= node_size;
        counter += s64((s64((*it)->dwReference) - 1)*s64((*it)->dwLength));
    }
    cs->Leave();

    return u32(s64(counter) / s64(1024));
}


