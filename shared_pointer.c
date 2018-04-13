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

struct s_shared_pointer
{
    size_t counter;
    shared_pointer_delete delete;
    char data[1];
};

static inline struct s_shared_pointer* shared_pointer_get(void* data);

void* shared_pointer_create(size_t size, shared_pointer_delete delete)
{
    struct s_shared_pointer *ptr;
    
    if ((ptr = (struct s_shared_pointer*) malloc(sizeof(*ptr) + size - 1)) == NULL)
        return (NULL);
    
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
    
    free(ptr);
    
    return (NULL);
}

static inline struct s_shared_pointer* shared_pointer_get(void* data)
{
    return (struct s_shared_pointer*) (((char*) data) - offsetof(struct s_shared_pointer, data));
}
