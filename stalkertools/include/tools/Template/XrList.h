#pragma once
template <typename T, typename allocator = xalloc<T> > class xr_list : public std::list < T, allocator > { public: bsize size() const { return (bsize)__super::size(); } };
#define DEF_LIST(N,T) typedef xr_list< T > N; typedef N::iterator N##_it;
#define DEFINE_LIST(T,N,I) typedef xr_list< T > N; typedef N::iterator I;
