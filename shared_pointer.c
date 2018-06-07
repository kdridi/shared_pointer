//
//  shared_pointer.c
//  shared_pointer
//
//  Created by Karim DRIDI on 13/04/2018.
//  Copyright © 2018 Karim DRIDI. All rights reserved.
//

#include "shared_pointer.h"

#include <stddef.h>
#include <stdlib.h>
#include <pthread.h>

#define DEFAULT_FREE    free
#define DEFAULT_MALLOC  malloc
#define DEFAULT_REALLOC realloc

struct s_shared_pointer
{
    size_t counter;
    f_shared_pointer_delete delete;
    t_shared_pointer_allocator allocator;
    char data[1];
};

static inline struct s_shared_pointer* shared_pointer_get(void* data)
{
    return (struct s_shared_pointer*) (((char*) data) - offsetof(struct s_shared_pointer, data));
}

static pthread_mutex_t allocator_mutex = PTHREAD_MUTEX_INITIALIZER;

static t_shared_pointer_allocator allocator = {
    .free = DEFAULT_FREE,
    .malloc = DEFAULT_MALLOC,
    .realloc = DEFAULT_REALLOC,
};

void shared_pointer_allocator(t_shared_pointer_allocator* a)
{
    pthread_mutex_lock(&allocator_mutex);
    {
        if (a == NULL)
        {
            allocator.free = DEFAULT_FREE;
            allocator.malloc = DEFAULT_MALLOC;
            allocator.realloc = DEFAULT_REALLOC;
        }
        else
        {
            allocator.malloc = a->malloc;
            allocator.free = a->free;
            allocator.realloc = a->realloc;
        }
    }
    pthread_mutex_unlock(&allocator_mutex);
}

void* shared_pointer_create(size_t size, f_shared_pointer_delete delete)
{
    struct s_shared_pointer *ptr;
    t_shared_pointer_allocator a;
    
    pthread_mutex_lock(&allocator_mutex);
    {
        a.malloc = allocator.malloc;
        a.free = allocator.free;
        a.realloc = allocator.realloc;
    }
    pthread_mutex_unlock(&allocator_mutex);

    if ((ptr = (struct s_shared_pointer*) a.malloc(sizeof(*ptr) + (size - 1) * sizeof(*(ptr->data)))) == NULL)
        return (NULL);

    ptr->allocator.malloc = a.malloc;
    ptr->allocator.free = a.free;
    ptr->allocator.realloc = a.realloc;

    ptr->counter = 0;
    ptr->delete = delete;
    
    return (ptr->data);
}

void* shared_pointer_retain(void* data)
{
    struct s_shared_pointer* ptr;
    
    ptr = shared_pointer_get(data);
    ptr->counter += 1;
    
    return (data);
}

void* shared_pointer_release(void* data)
{
    struct s_shared_pointer* ptr;
    
    ptr = shared_pointer_get(data);
    
    if (ptr->counter > 0)
    {
        ptr->counter -= 1;
        return (data);
    }
    
    if (ptr->delete)
        ptr->delete(data);
    
    ptr->allocator.free(ptr);
    
    return (NULL);
}
