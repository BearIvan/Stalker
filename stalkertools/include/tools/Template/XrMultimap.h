#pragma once
template <typename K, class V, class P = std::less<K>, typename allocator = xalloc<std::pair<K, V> > > class xr_multimap : public std::multimap < K, V, P, allocator > { public: bsize size() const { return (bsize)__super::size(); } };
#define DEFINE_MMAP(K,T,N,I) typedef xr_multimap< K, T > N; typedef N::iterator I;
