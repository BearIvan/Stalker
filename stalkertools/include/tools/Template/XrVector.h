#pragma once

// vector
template <typename T, typename allocator = xalloc<T> >
class xr_vector : public std::vector < T, allocator >
{
private:
	using inherited = typename std::vector<T, allocator> ;

public:
	using allocator_type = allocator ;

public:
	xr_vector() : inherited() {}
	xr_vector(size_t _count, const T& _value) : inherited(_count, _value) {}
	explicit xr_vector(size_t _count) : inherited(_count) {}
	bsize size() const { return (bsize)inherited::size(); }

	void clear_and_free() { inherited::clear(); }
	void clear_not_free() { inherited::erase( inherited::begin(), inherited::end()); }
	void clear_and_reserve() { if (inherited::capacity() <= (inherited::size() + inherited::size() / 4)) clear_not_free(); else { bsize old = inherited::size(); clear_and_free(); inherited::reserve(old); } }

	void clear() { clear_and_free(); }

	typename inherited::const_reference operator[] (bsize _Pos) const { {VERIFY2(_Pos < size(), make_string("index is out of range: index requested[%d], size of container[%d]", _Pos, size()).c_str()); } return (*( inherited::begin() + _Pos)); }
	typename inherited::reference operator[] (bsize _Pos) { {VERIFY2(_Pos < size(), make_string("index is out of range: index requested[%d], size of container[%d]", _Pos, size()).c_str()); } return (*( inherited::begin() + _Pos)); }
};

template <>
class xr_vector<bool, xalloc<bool> > : public std::vector < bool, xalloc<bool> >
{
private:
    typedef std::vector<bool, xalloc<bool> > inherited;

public:
    bsize size() const { return (bsize)inherited::size(); }
    void clear() { inherited::erase(inherited::begin(),inherited::end()); }
};

template <typename allocator>
class xr_vector<bool, allocator> : public std::vector < bool, allocator >
{
private:
    typedef std::vector<bool, allocator> inherited;

public:
    bsize size() const { return (bsize)inherited::size(); }
    void clear() { inherited::erase(inherited::begin(), inherited::end()); }
};


#define DEF_VECTOR(N,T) typedef xr_vector< T > N; typedef typename N::iterator N##_it;
#define DEFINE_VECTOR(T,N,I) typedef xr_vector< T > N; typedef typename N::iterator I;
// auxilary definition
DEFINE_VECTOR(bool, boolVec, boolIt);
DEFINE_VECTOR(BOOL, BOOLVec, BOOLIt);
DEFINE_VECTOR(BOOL*, LPBOOLVec, LPBOOLIt);
DEFINE_VECTOR(Frect, FrectVec, FrectIt);
DEFINE_VECTOR(Irect, IrectVec, IrectIt);
DEFINE_VECTOR(Fplane, PlaneVec, PlaneIt);
DEFINE_VECTOR(Fvector2, Fvector2Vec, Fvector2It);
DEFINE_VECTOR(Fvector, FvectorVec, FvectorIt);
DEFINE_VECTOR(Fvector*, LPFvectorVec, LPFvectorIt);
DEFINE_VECTOR(Fcolor, FcolorVec, FcolorIt);
DEFINE_VECTOR(Fcolor*, LPFcolorVec, LPFcolorIt);
DEFINE_VECTOR(LPSTR, LPSTRVec, LPSTRIt);
DEFINE_VECTOR(LPCSTR, LPCSTRVec, LPCSTRIt);
DEFINE_VECTOR(string64, string64Vec, string64It);
DEFINE_VECTOR(xr_string, SStringVec, SStringVecIt);

DEFINE_VECTOR(s8, S8Vec, S8It);
DEFINE_VECTOR(s8*, LPS8Vec, LPS8It);
DEFINE_VECTOR(s16, S16Vec, S16It);
DEFINE_VECTOR(s16*, LPS16Vec, LPS16It);
DEFINE_VECTOR(s32, S32Vec, S32It);
DEFINE_VECTOR(s32*, LPS32Vec, LPS32It);
DEFINE_VECTOR(u8, U8Vec, U8It);
DEFINE_VECTOR(u8*, LPU8Vec, LPU8It);
DEFINE_VECTOR(u16, U16Vec, U16It);
DEFINE_VECTOR(u16*, LPU16Vec, LPU16It);