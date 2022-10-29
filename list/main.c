#include "list.h"

#include <stdio.h>

int main(void)
{
    struct user
    {
        char name[64];
        const char nacionality[64];
        const char gender[64];
        int age;
    };

    struct user users[] =
    {
        {"Alexander", "Greek", "Male", 32},
        {"Maria", "South African", "Female", 27},
        {"Marcos", "Brazilian", "Male", 17},
        {"Ha-Yun", "South Korean", "Female", 38},
        {"Michael", "American", "Male", 15}
    };
    int len = sizeof(users)/sizeof(struct user);

    List list = list_create(sizeof(struct user));
    for(int i = 0; i < len; i++)
       list_append(&list, &users[i]);

    for(ListNode* it = list_begin(&list); it; it = list_next(it))
    {
        struct user* user = (struct user*) it->data;
        printf("%s - %s - %s - %d\n", user->name, user->nacionality, user->gender, user->age);
    }

    list_delete(&list);
}