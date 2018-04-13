//
//  main.c
//  shared_pointer
//
//  Created by Karim DRIDI on 13/04/2018.
//  Copyright © 2018 Karim DRIDI. All rights reserved.
//

#include "shared_pointer.h"

#include <stddef.h>
#include <stdbool.h>

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

typedef struct s_int_list t_int_list;

t_int_list *int_list_create(void);
bool int_list_push_front(t_int_list *, int);
void int_list_iterate(t_int_list *, void*, bool(*)(void*, int));


static bool show_int(void* data, int value)
{
    int* iptr;
    
    iptr = (int *) data;
    printf("list[%d] = %d\n", *iptr, value);
    *iptr += 1;
    
    return (true);
}

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
    
    
    t_int_list* int_list;
    
    if ((int_list = int_list_create()) != NULL)
    {
        int index;
        
        int_list_push_front(int_list, 4);
        int_list_push_front(int_list, 3);
        int_list_push_front(int_list, 2);
        int_list_push_front(int_list, 1);
        
        index = 0;
        int_list_iterate(int_list, &index, show_int);
        
        shared_pointer_release(int_list);
    }
    
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

struct s_int_list_node
{
    int value;
    struct s_int_list_node* next;
};

static void int_list_node_delete(void* ptr)
{
    struct s_int_list_node *self;
    
    self = (struct s_int_list_node *) ptr;
    
    if (self->next != NULL)
        shared_pointer_release(self->next);
}

struct s_int_list_node *int_list_node_create(int value)
{
    struct s_int_list_node *node;
    
    if ((node = shared_pointer_create(sizeof(*node), int_list_node_delete)) == NULL)
        return (NULL);
    
    node->value = value;
    node->next = NULL;
    
    return (node);
};

struct s_int_list
{
    struct s_int_list_node* head;
};

static void int_list_delete(void* ptr)
{
    t_int_list *list;
    
    list = (t_int_list *) ptr;
    if (list->head != NULL)
        shared_pointer_release(list->head);
}

t_int_list *int_list_create(void)
{
    t_int_list *list;
    
    if ((list = shared_pointer_create(sizeof(*list), int_list_delete)) == NULL)
        return (NULL);
    
    list->head = NULL;
    
    return (list);
}

bool int_list_push_front(t_int_list *self, int value)
{
    struct s_int_list_node* node;
    
    if ((node = int_list_node_create(value)) == NULL)
        return (false);
    
    node->next = self->head;
    self->head = node;
    
    return true;
}

void int_list_iterate(t_int_list *self, void* data, bool(*iterate)(void*, int))
{
    struct s_int_list_node* node;
    
    node = self->head;
    while (node != NULL)
    {
        if (iterate(data, node->value) == false)
            break;
        
        node = node->next;
    }
}

