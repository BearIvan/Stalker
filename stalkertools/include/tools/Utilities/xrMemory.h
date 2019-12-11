#pragma once

# define DUMP_PHASE do {} while (0)

#undef CopyMemory
#define CopyMemory(a,b,c) memcpy(a,b,c) 

#undef FillMemory
#define FillMemory(a,b,c)  bear_fill(reinterpret_cast<void*>(a),b,c);

#undef ZeroMemory
#define ZeroMemory(a,b) bear_fill(reinterpret_cast<void*>(a),b);

template <class T>
IC T* xr_new()
{
	T* ptr = (T*)BearMemory::Malloc(sizeof(T), "XRAY");
	return new (ptr)T();
}
// new(1)
template <class T, class P1>
IC T* xr_new(const P1& p1)
{
	T* ptr = (T*)BearMemory::Malloc(sizeof(T), "XRAY");
	return new (ptr)T(p1);
}
// new(2)
template <class T, class P1, class P2>
IC T* xr_new(const P1& p1, const P2& p2)
{
	T* ptr = (T*)BearMemory::Malloc(sizeof(T), "XRAY");
	return new (ptr)T(p1, p2);
}
// new(3)
template <class T, class P1, class P2, class P3>
IC T* xr_new(const P1& p1, const P2& p2, const P3& p3)
{
	T* ptr = (T*)BearMemory::Malloc(sizeof(T), "XRAY");
	return new (ptr)T(p1, p2, p3);
}
// new(4)
template <class T, class P1, class P2, class P3, class P4>
IC T* xr_new(const P1& p1, const P2& p2, const P3& p3, const P4& p4)
{
	T* ptr = (T*)BearMemory::Malloc(sizeof(T), "XRAY");
	return new (ptr)T(p1, p2, p3, p4);
}
// new(5)
template <class T, class P1, class P2, class P3, class P4, class P5>
IC T* xr_new(const P1& p1, const P2& p2, const P3& p3, const P4& p4, const P5& p5)
{
	T* ptr = (T*)BearMemory::Malloc(sizeof(T), "XRAY");
	return new (ptr)T(p1, p2, p3, p4, p5);
}
// new(6)
template <class T, class P1, class P2, class P3, class P4, class P5, class P6>
IC T* xr_new(const P1& p1, const P2& p2, const P3& p3, const P4& p4, const P5& p5, const P6& p6)
{
	T* ptr = (T*)BearMemory::Malloc(sizeof(T), "XRAY");
	return new (ptr)T(p1, p2, p3, p4, p5, p6);
}
// new(7)
template <class T, class P1, class P2, class P3, class P4, class P5, class P6, class P7>
IC T* xr_new(const P1& p1, const P2& p2, const P3& p3, const P4& p4, const P5& p5, const P6& p6, const P7& p7)
{
	T* ptr = (T*)BearMemory::Malloc(sizeof(T), "XRAY");
	return new (ptr)T(p1, p2, p3, p4, p5, p6, p7);
}
// new(8)
template <class T, class P1, class P2, class P3, class P4, class P5, class P6, class P7, class P8>
IC T* xr_new(const P1& p1, const P2& p2, const P3& p3, const P4& p4, const P5& p5, const P6& p6, const P7& p7, const P8& p8)
{
	T* ptr = (T*)BearMemory::Malloc(sizeof(T), "XRAY");
	return new (ptr)T(p1, p2, p3, p4, p5, p6, p7, p8);
}
// new(9)
template <class T, class P1, class P2, class P3, class P4, class P5, class P6, class P7, class P8, class P9>
IC T* xr_new(const P1& p1, const P2& p2, const P3& p3, const P4& p4, const P5& p5, const P6& p6, const P7& p7, const P8& p8, const P8& p9)
{
	T* ptr = (T*)BearMemory::Malloc(sizeof(T), "XRAY");
	return new (ptr)T(p1, p2, p3, p4, p5, p6, p7, p8, p9);
}
template <bool _is_pm, typename T>
struct xr_special_free
{
	IC void operator()(T*& ptr)
	{
		void* _real_ptr = dynamic_cast<void*>(ptr);
		ptr->~T();
		BearMemory::Free(_real_ptr);
	}
};

template <typename T>
struct xr_special_free < false, T >
{
	IC void operator()(T*& ptr)
	{
		ptr->~T();
		BearMemory::Free(ptr);
	}
};

template <class T>
IC void xr_delete(T*& ptr)
{
	if (ptr)
	{
		xr_special_free<is_polymorphic<T>::result, T>()(ptr);
		ptr = NULL;
	}
}
template <class T>
IC void xr_delete(T* const& ptr)
{
	if (ptr)
	{
		xr_special_free<is_polymorphic<T>::result, T>(ptr);
		((T*&)ptr) = NULL;
	}
}
template <class T>
IC T* xr_alloc (bsize count) { return (T*)BearMemory::Malloc(count*sizeof(T),"XRAY"); }
template <class T>
IC void xr_free (T*& P) { if (P) { BearMemory::Free((void*)P); P=NULL; }; }
IC void* xr_malloc(size_t size) { return BearMemory::Malloc(size, "XRAY"); }
IC void* xr_realloc (void* P, size_t size) { return  BearMemory::Realloc(P,size, "XRAY"); }



template <class T>
class xalloc
{
public:
    typedef size_t size_type;
    typedef ptrdiff_t difference_type;
    typedef T* pointer;
    typedef const T* const_pointer;
    typedef T& reference;
    typedef const T& const_reference;
    typedef T value_type;

public:
    template<class _Other>
    struct rebind { typedef xalloc<_Other> other; };
public:
    pointer address(reference _Val) const { return (&_Val); }
    const_pointer address(const_reference _Val) const { return (&_Val); }
    xalloc() { }
    xalloc(const xalloc<T>&) { }
    template<class _Other> xalloc(const xalloc<_Other>&) { }
    template<class _Other> xalloc<T>& operator= (const xalloc<_Other>&) { return (*this); }
    pointer allocate(size_type n, const void* p = 0) const { return xr_alloc<T>((u32)n); }
    char* _charalloc(size_type n) { return (char*)allocate(n); }
    void deallocate(pointer p, size_type n) const { xr_free(p); }
    void deallocate(void* p, size_type n) const { xr_free(p); }
	template<class C,class T>
    void construct(C* p, const T& _Val) { ::new ((void*)p) C(_Val); }
	template<class C>
	void destroy(C* p) { p->~C(); }
    size_type max_size() const { size_type _Count = (size_type)(-1) / sizeof(T); return (0 < _Count ? _Count : 1); }
};

struct xr_allocator
{
    template <typename T>
    struct helper
    {
        typedef xalloc<T> result;
    };

    static void* alloc(const u32& n) { return xr_malloc((u32)n); }
    template <typename T>
    static void dealloc(T*& p) { xr_free(p); }
};

template<class _Ty, class _Other> inline bool operator==(const xalloc<_Ty>&, const xalloc<_Other>&) { return (true); }
template<class _Ty, class _Other> inline bool operator!=(const xalloc<_Ty>&, const xalloc<_Other>&) { return (false); }

namespace std
{
template<class _Tp1, class _Tp2> inline xalloc<_Tp2>& __stl_alloc_rebind(xalloc<_Tp1>& __a, const _Tp2*) { return (xalloc<_Tp2>&)(__a); }
template<class _Tp1, class _Tp2> inline xalloc<_Tp2> __stl_alloc_create(xalloc<_Tp1>&, const _Tp2*) { return xalloc<_Tp2>(); }
};