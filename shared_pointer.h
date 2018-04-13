//
//  shared_pointer.h
//  shared_pointer
//
//  Created by Karim DRIDI on 13/04/2018.
//  Copyright © 2018 Karim DRIDI. All rights reserved.
//

#ifndef shared_pointer_h
#define shared_pointer_h

#include <stddef.h>

typedef void (*shared_pointer_delete)(void*);

#ifdef __cplusplus
extern "C" {
#endif
    
    void* shared_pointer_create(size_t, shared_pointer_delete);
    void* shared_pointer_retain(void*);
    void* shared_pointer_release(void*);
    
#ifdef __cplusplus
}
#endif

#endif /* shared_pointer_h */

