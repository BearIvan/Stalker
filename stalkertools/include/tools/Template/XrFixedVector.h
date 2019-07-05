#pragma once

template <class T, const bsize dim,class SizeT=bsize>
class svector
{
public:
    typedef size_t size_type;
    typedef T value_type;
    typedef value_type* iterator;
    typedef const value_type* const_iterator;
    typedef value_type& reference;
    typedef const value_type& const_reference;

private:
    value_type array[dim];
	SizeT count;

public:
    svector() : count(0)
    {}
    svector(iterator p, bsize c)
    {
        assign(p, c);
    }

    IC iterator begin() { return array; }
    IC iterator end() { return array + count; }
    IC const_iterator begin() const { return array; }
    IC const_iterator end() const { return array + count; }
    IC SizeT size() const { return count; }
    IC void clear() { count = 0; }
    IC void resize(bsize c) { VERIFY(c <= dim); count = c; }
    IC void reserve(bsize c) { }

    IC void push_back(value_type e) { VERIFY(count < dim); array[static_cast<bsize>(count++)] = e; }
    IC void pop_back() { VERIFY(count); count--; }

    IC reference operator[] (SizeT id) { VERIFY(id < count); return array[static_cast<bsize>(id)]; }
    IC const_reference operator[] (SizeT id) const { VERIFY(id < count); return array[static_cast<bsize>(id)]; }

    IC reference front() { return array[0]; }
    IC reference back() { return array[static_cast<bsize>(count - 1)]; }
    IC reference last() { VERIFY(count < dim); return array[static_cast<bsize>(count)]; }
    IC const_reference front() const { return array[0]; }
    IC const_reference back() const { return array[static_cast<bsize>(count - 1)]; }
    IC const_reference last() const { VERIFY(count < dim); return array[static_cast<bsize>(count)]; }
    IC void inc() { count++; }
    IC bool empty() const { return 0 == count; }

    IC void erase(SizeT id)
    {
        VERIFY(id < count);
        count--;
        for (bsize i = id; i <static_cast<bsize>( count); i++)
            array[i] = array[i + 1];
    }
    IC void erase(iterator it) { erase(static_cast<SizeT>(it - begin())); }

    IC void insert(SizeT id, reference V)
    {
        VERIFY(id < count);
        for (bsize i = count; i > static_cast<bsize>(id); i--) array[i] = array[i - 1];
        count++;
        array[static_cast<bsize>(id)] = V;
    }
    IC void assign(iterator p, bsize c) { VERIFY(c > 0 && c < dim); CopyMemory(array, p, c*sizeof(value_type)); count = c; }
    IC BOOL equal(const svector<value_type, dim>& base) const
    {
        if (size() != base.size()) return FALSE;
        for (bsize cmp = 0; cmp < size(); cmp++) if ((*this)[cmp] != base[cmp]) return FALSE;
        return TRUE;
    }
};
