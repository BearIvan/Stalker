#pragma once
template <typename K, class P = std::less<K>, typename allocator = xalloc<K> > class xr_multiset : public std::multiset < K, P, allocator > { public: bsize size() const { return (bsize)__super::size(); } };
