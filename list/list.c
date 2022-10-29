#include "list.h"

#include <stdlib.h>
#include <string.h>

void static inline list_zeros(List* list)
{
    list->head = 0;
    list->tail = 0;
    list->length = 0;
}

static inline ListNode* new_node(vptr val, u64 size)
{
    ListNode* new = (ListNode*) malloc(sizeof(ListNode));
    if(!new)
        return NULL;

    vptr valcpy = malloc(size);
    if(!valcpy)
    {
        free(new);
        return NULL;
    }
    memcpy(valcpy, val, size);

    new->data = valcpy;
    new->next = 0;
    new->prev = 0;
    
    return new;
}

List list_create(u64 data_size)
{
    List list = {0, 0, 0, data_size};
    return list;
}

void list_delete(List* list)
{
    //if list is empty or null, just return
    if(!list || list_is_empty(list))
        return;
    //else iterate over each node and free it
    ListNode* node = list->head;
    while(node)
    {
        ListNode* next = node->next;
        free(node->data);
        free(node);
        node = next;
    }
    list_zeros(list);
}

List* list_copy(List* dest, const List* src)
{
    if(list_is_empty(src))
        return dest;
    list_zeros(dest);

    for(ListNode* it_src = list_begin(src); it_src; it_src = list_next(it_src))
        if(!list_append(dest, it_src->data))
        {
            list_delete(dest);
            return NULL;
        }
    return dest;
}

u32 list_length(const List* list)
{
    return list ? list->length : 0;
}

u64 list_data_size(const List* list)
{
    return list ? list->data_size : 0;
}

bool list_is_empty(const List* list)
{
    return list_length(list) == 0;
}


List* list_link(List* list1, List* list2)
{
    if(!list1)
        return NULL;

    if(list_is_empty(list2))
        return list1;

    if(list_is_empty(list1))
    {
        list1->head = list2->head;
        list1->tail = list2->tail;
        list1->length = list2->length;
    }
    else
    {
        list1->tail->next = list2->head;
        list1->tail = list2->tail;
        list1->length += list2->length;
    }
    list_zeros(list2);
    return list1;
}

ListNode* list_append(List* list, vptr val)
{
    if(!list)
        return NULL;
    
    ListNode* new = new_node(val, list_data_size(list));
    if(!new)
        return NULL;
    
    new->prev = list->tail;
    list->tail = new;
    if(list_is_empty(list))
        list->head = new;
    if(new->prev)
        new->prev->next = new;
    list->length++;
    
    return list->tail;
}

ListNode* list_prepend(List* list, vptr val)
{
    if(!list)
        return NULL;
   
    ListNode* new = new_node(val, list_data_size(list));
    if(!new)
        return NULL;

    new->next = list->head;
    list->head = new;
    if(list_is_empty(list))
        list->tail = new;
    if(new->next)
        new->next->prev = new;
    list->length++;
    
    return list->head;
}

ListNode* list_insert(List* list, vptr val, u32 idx)
{
    if(!list)
        return NULL;
    if(idx == 0)
        return list_prepend(list, val);
    if(idx == list_length(list))
        return list_append(list, val);
    if(idx > list_length(list))
        return NULL;

    ListNode* it;
    if(idx < list->length / 2)
    {
        it = list_begin(list);
        while(idx)
        {
            it = list_next(it);
            idx--;
        }
    }
    else
    {
        it = list_end(list);
        idx = list_length(list)-1 - idx;
        while(idx)
        {
            it = list_prev(it);
            idx--;
        }
    }
    return list_insert_before(list, it, val);
}


List* list_remove(List* list, vptr val)
{
    if(list_is_empty(list))
        return list;
    
    ListNode* it = list_begin(list);
    while(it)
    {
        ListNode* next = list_next(it);
        if(!memcmp(val, it->data, list_data_size(list)))
        {
            list = list_remove_node(list, it);
            break;
        }
        it = next;
    }
    return list;
}

List* list_remove_all(List* list, vptr val)
{
    if(list_is_empty(list))
        return list;
    
    ListNode* it = list_begin(list);
    while(it)
    {
        ListNode* next = list_next(it);
        if(!memcmp(val, it->data, list_data_size(list)))
        {
            list = list_remove_node(list, it);
            if(!list)
                return NULL;
        }
        
        it = next;
    }
    return list;
}

List* list_remove_fn(List* list, u32 (*comp)(vptr))
{
    if(list_is_empty(list))
        return list;
    
    ListNode* it = list_begin(list);
    while(it)
    {
        ListNode* next = list_next(it);
        if(!comp(it->data))
        {
            list = list_remove_node(list, it);
            break;
        }
        it = next;
    }
    return list;
}

List* list_remove_all_fn(List* list, u32 (*comp)(vptr))
{
    if(list_is_empty(list))
        return list;
    
    ListNode* it = list_begin(list);
    while(it)
    {
        ListNode* next = list_next(it);
        if(!comp(it->data))
        {
            list = list_remove_node(list, it);
            if(!list)
                return NULL;
        }
        it = next;
    }
    return list;
}

List* list_remove_node(List* list, ListNode* node)
{
    if(!list || !node)
        return NULL;
    if(list_is_empty(list))
        return list;
    
    if(node != list->head && node != list->tail)
    {
        node->prev->next = node->next;
        node->next->prev = node->prev;
    }
    if(node == list->head)
    {
        list->head = node->next;
        if(node->next)
            node->next->prev = 0;
    }
    if(node == list->tail)
    {
        list->tail = node->prev;
        if(node->prev)
            node->prev->next = 0;
    }

    free(node->data);
    free(node);
    return list;
}

List* list_pop_back(List* list)
{
    if(list_is_empty(list))
        return list;
    if(list_length(list) == 1)
    {
        free(list->tail);
        list_zeros(list);
        return list;
    }
    
    ListNode* node = list->tail;
    node->prev->next = 0;
    list->tail = node->prev;

    free(node->data);
    free(node);
    return list;
}

List* list_pop_front(List* list)
{
    if(list_is_empty(list))
        return list;
    if(list_length(list) == 1)
    {
        free(list->head);
        list_zeros(list);
        return list;
    }
    
    ListNode* node = list->head;
    node->next->prev = 0;
    list->tail = node->next;

    free(node->data);
    free(node);
    return list;
}


ListNode* list_begin(const List* list)
{
    return list ? list->head : 0;
}

ListNode* list_end(const List* list)
{
    return list ? list->tail : 0;
}

ListNode* list_next(const ListNode* node)
{
    return node ? node->next : 0;
}

ListNode* list_prev(const ListNode* node)
{
    return node ? node->prev : 0;
}

ListNode* list_insert_after(List* list, ListNode* node, vptr val)
{
    if(!node || !list)
        return NULL;
    ListNode* new = new_node(val, list_data_size(list));
    if(!new)
        return NULL;

    new->prev = node;
    new->next = node->next;
    if(node->next)
        node->next->prev = new;
    node->next = new;

    if(list->tail == node)
        list->tail = new;

    return new;
}

ListNode* list_insert_before(List* list, ListNode* node, vptr val)
{
    if(!node || !list)
        return NULL;
    ListNode* new = new_node(val, list_data_size(list));
    if(!new)
        return NULL;

    new->next = node;
    new->prev = node->prev;
    if(node->prev)
        node->prev->next = new;
    node->prev = new;

    if(list->head == node)
        list->head = new;

    return new;
}