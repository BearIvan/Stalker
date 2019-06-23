#pragma once

// deque
template <typename T, typename allocator = xalloc<T> >
class xr_deque : public std::deque < T, allocator >
{
public:
    typedef typename allocator allocator_type;
    typedef typename allocator_type::value_type value_type;
    typedef typename allocator_type::size_type size_type;
    u32 size() const { return (u32)__super::size(); }
};

#define DEF_DEQUE(N,T) typedef xr_deque< T > N; typedef N::iterator N##_it;
#define DEFINE_DEQUE(T,N,I) typedef xr_deque< T > N; typedef N::iterator I;