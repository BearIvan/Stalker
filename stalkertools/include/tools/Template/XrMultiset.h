#pragma once
template <typename K, class P = std::less<K>, typename allocator = xalloc<K> > class xr_multiset : public std::multiset < K, P, allocator > { public: u32 size() const { return (u32)__super::size(); } };
