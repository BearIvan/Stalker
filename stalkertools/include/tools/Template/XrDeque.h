#pragma once

// deque
template <typename T, typename allocator = xalloc<T>>
class xr_deque : public std::deque<T, allocator>
{
    using base =typename std::deque<T, allocator>;
public:
    using allocator_type =  allocator;
    using value_type = typename allocator_type::value_type;
    using size_type = typename allocator_type::size_type;
    bsize size() const { return (bsize) base::size(); }
};

#define DEF_DEQUE(N, T)    \
    typedef xr_deque<T> N; \
    typedef N::iterator N##_it;

#define DEFINE_DEQUE(T, N, I) \
    typedef xr_deque<T> N;    \
    typedef N::iterator I;
