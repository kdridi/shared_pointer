//
//  main.c
//  shared_pointer
//
//  Created by Karim DRIDI on 13/04/2018.
//  Copyright © 2018 Karim DRIDI. All rights reserved.
//

#include "shared_pointer.h"

#include <stddef.h>

typedef struct s_1 {
    int a;
    int b;
} t_1;

typedef struct s_2 {
    int c;
    int d;
    t_1* t1;
} t_2;

t_1* t1_create(int a, int b);
t_2* t2_create(int c, int d, t_1* t1);

#include <stdio.h>

int main()
{
    int *i_ptr, *j_ptr;
    
    i_ptr = shared_pointer_create(sizeof(*i_ptr), NULL);
    printf("%p\n", i_ptr);
    
    j_ptr = shared_pointer_retain(i_ptr);
    printf("%p\n", j_ptr);
    
    printf("%p\n", shared_pointer_release(i_ptr));
    printf("%p\n", shared_pointer_release(j_ptr));
    
    t_1* tt = t1_create(1, 2);
    t_2* t = t2_create(3, 4, tt);
    shared_pointer_release(tt);
    shared_pointer_release(t);
    
    return 0;
}

t_1* t1_create(int a, int b)
{
    t_1* t;
    
    t = shared_pointer_create(sizeof(*t), NULL);
    t->a = a;
    t->b = b;
    
    return (t);
}

static void t2_delete(void* t)
{
    shared_pointer_release(((t_2*) t)->t1);
}

t_2* t2_create(int c, int d, t_1* t1)
{
    t_2* t;
    
    t = shared_pointer_create(sizeof(*t), t2_delete);
    t->c = c;
    t->d = d;
    t->t1 = shared_pointer_retain(t1);
    
    return (t);
}

