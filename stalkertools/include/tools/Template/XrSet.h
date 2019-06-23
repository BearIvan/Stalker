#pragma once
template <typename K, class P = std::less<K>, typename allocator = xalloc<K> > class xr_set : public std::set < K, P, allocator > { public: u32 size() const { return (u32)__super::size(); } };
#define DEFINE_SET(T,N,I) typedef xr_set< T > N; typedef N::iterator I;