#ifndef DSLIST_H
#define DSLIST_H

#include "defines.h"

typedef struct list
{
   struct list_node* tail;
   struct list_node* head;
   u32 length;
   const u64 data_size;
} List;

typedef struct list_node
{
    struct list_node* prev;
    struct list_node* next;
    vptr data;
} ListNode;

/**
 * @brief Initializes a List that holds elements of given size.
 * 
 * @param data_size The size of held data (sizeof).
 * 
 * @return A initialized empty list.
 */
List list_create(u64 data_size);

/**
 * @brief Deletes provided list nodes and contained data (see ListNode->data).
 * 
 * @param list The list to be deleted.
 */
void list_delete(List* list);

/**
 * @brief Copies src list nodes to dest list. Does not free previous dest list nodes, if any.
 * Will copy data contained at src using memcpy.
 * 
 * @param dest The list it will be copied to.
 * @param src The list to be copied.
 * @return List* The dest List, NULL if any errors.
 */
List* list_copy(List* dest, const List* src);

/**
 * @brief Gets the sizeof of contained data
 * 
 * @param list The list to be read from.
 * @return u64 Sizeof contained data
 */
u64 list_data_size(const List* list);

/**
 * @brief Gets the number of nodes in the given list. The time complexity is constant.
 * 
 * @param list The list to be read from.
 * @return u32 The length of the list, 0 if NULL or empty list
 */
u32 list_length(const List* list);

/**
 * @brief Verifies if given list is empty. The time complexity is constant.
 * 
 * @param list The list to be verified.
 * @return true If the list is empty or NULL.
 * @return false Otherwise.
 */
bool list_is_empty(const List* list);

/**
 * @brief Links the second list into the end of the first one. After use, list2 will be a empty list
 * and list1 will gain ownership over its data.
 * 
 * @param list1 The first list, concatenated into.
 * @param list2 The second list, to be concatenated.
 * @return List* The concatenated list, should be equal to list1, NULL if any errors.
 */
List* list_link(List* list1, List* list2);

/**
 * @brief Inserts a element at the end of the list (List->tail). Note that this function does copy
 * the memory block of the pointed data
 * 
 * @param list The list at which the element will be inserted.
 * @param val The element to be inserted.
 * @return ListNode* Iterator to inserted element, NULL if any errors.
 */
ListNode* list_append(List* list, vptr val);


/**
 * @brief Inserts a element at the beggining of the list (List->head). Note that this function does copy
 * the memory block of the pointed data
 * 
 * @param list The list at which the element will be inserted.
 * @param val The element to be inserted.
 * @return ListNode* Iterator to inserted element, NULL if any errors.
 */
ListNode* list_prepend(List* list, vptr val);

/**
 * @brief Inserts a element at the given index. Note that this function does copy
 * the memory block of the pointed data. This function will fail if
 * the index is bigger than the list length (returns NULL).
 * 
 * @param list The list at which the element will be inserted.
 * @param val The element to be inserted.
 * @param idx The index at which the element should be inserted.
 * @return ListNode* Iterator to inserted element, NULL if any errors.
 */
ListNode* list_insert(List* list, vptr val, u32 idx);

/**
 * @brief Removes the first element that equals to given element, compares memory blocks. Frees
 * contained data (see ListNode->data).
 * 
 * @param list The list from which the element will be removed.
 * @param val The element to be removed.
 * @return List* The resulting list, NULL if any errors.
 */
List* list_remove(List* list, vptr val);

/**
 * @brief Removes all the elements that equal to given element, compares memory blocks. Frees
 * contained data (see ListNode->data).
 * 
 * @param list The list from which the element will be removed.
 * @param val The element to be removed.
 * @return List* The resulting list, NULL if any errors.
 */
List* list_remove_all(List* list, vptr val);

/**
 * @brief Removes the first element that assert to zero in user defined function. Frees
 * contained data (see ListNode->data).
 * 
 * @param list The list from which the element will be removed.
 * @param comp Function that compares each element of the list with given value, removes element when asserted to 0.
 * @return List* The resulting list, NULL if any errors.
 */
List* list_remove_fn(List* list, u32 (*comp)(vptr));

/**
 * @brief Removes all that elements asserts to zero in user defined function. Frees
 * contained data (see ListNode->data).
 * 
 * @param list The list from which the element will be removed.
 * @param comp Function that compares each element of the list with given value, removes element when asserted to 0.
 * @return List* The resulting list, NULL if any errors.
 */
List* list_remove_all_fn(List* list, u32 (*comp)(vptr));

/**
 * @brief Removes given node from list, frees contained data.
 * 
 * @param list The list from which node will be removed
 * @param node The node to be removed
 * @return List* The resulting list, NULL if any errors.
 */
List* list_remove_node(List* list, ListNode* node);

/**
 * @brief Removes the last element of the list (List->tail), if any. Does
 * free contained data (see ListNode->data).
 * 
 * @param list The list from which the element will be removed.
 * @return List* The list, NULL if any errors.
 */
List* list_pop_back(List* list);

/**
 * @brief Removes the first element of the list (List->head), if any. Does
 * free contained data (see ListNode->data).
 * 
 * @param list The list from which the element will be removed.
 * @return List* The list, NULL if any errors.
 */
List* list_pop_front(List* list);

/**
 * @brief Returns iterator to the first element of the list (List->head).
 * 
 * @param list The list from which the first element will be read.
 * @return ListNode* The first element iterator, NULL if any errors.
 */
ListNode* list_begin(const List* list);

/**
 * @brief Returns iterator to the last element of the list (List->tail).
 * 
 * @param list The list from which the first element will be read.
 * @return ListNode* The last element iterator, NULL if any errors.
 */
ListNode* list_end(const List* list);

/**
 * @brief Gets next element of given iterator.
 * 
 * @param node Iterator to be incremented.
 * @return ListNode* Incremented iterator, NULL if end of list or error.
 */
ListNode* list_next(const ListNode* node);

/**
 * @brief Gets previous element of given iterator.
 * 
 * @param node Iterator to be decremented.
 * @return ListNode* Decremented iterator, NULL if end of list or error.
 */
ListNode* list_prev(const ListNode* node);

/**
 * @brief Inserts element after given iterator. Note that this function does copy
 * the memory block of the pointed data
 * 
 * @param list The list at which the first element will be add.
 * @param node Referenced iterator.
 * @param val The element to be inserted.
 * @return ListNode* Iterator to inserted element, NULL if any errors.
 */
ListNode* list_insert_after(List* list, ListNode* node, vptr val);

/**
 * @brief Inserts element before given iterator. Note that this function does copy
 * the memory block of the pointed data.
 * 
 * @param list The list at which the first element will be add.
 * @param node Referenced iterator.
 * @param val The element to be inserted.
 * @return ListNode* Iterator to inserted element, NULL if any errors.
 */
ListNode* list_insert_before(List* list, ListNode* node, vptr val);


#endif