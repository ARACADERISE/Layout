#ifndef MocaMemory
#define MocaMemory

static size_t bump_allocator_b = 0x1000000;

void *moca_alloc(size_t count, size_t alignment)
{
        size_t new_b = bump_allocator_b;
        if(new_b & (alignment - 1))
        {
                new_b &= ~(alignment - 1);
                new_b += alignment;
        }
        void *ret = (void *)new_b;
        new_b += count;
        bump_allocator_b = new_b;
        return ret;
}

void *moca_malloc(size_t size)
{
    return moca_alloc(size, 4);
}

#endif
