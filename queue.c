#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "list.h"
#include "queue.h"

/* Notice: sometimes, Cppcheck would find the potential NULL pointer bugs,
 * but some of them cannot occur. You can suppress them by adding the
 * following line.
 *   cppcheck-suppress nullPointer
 */

typedef struct {
    int size;
    struct list_head head;
} queue_t;

/* Initial element */
static inline void INIT_ELEMENT(element_t *e)
{
    if (e == NULL)
        return;

    e->value = NULL;
    INIT_LIST_HEAD(&e->list);
}

/* Create element and initial it */
element_t *element_new()
{
    element_t *e = malloc(sizeof(element_t));
    INIT_ELEMENT(e);
    return e;
}

/* Delete element and update list and queue */
static void element_del(element_t *e, struct list_head *head)
{
    if (e == NULL || head == NULL)
        return;

    queue_t *q = container_of(head, queue_t, head);

    free(e->value);
    e->value = NULL;
    list_del_init(&e->list);
    free(e);
    q->size -= 1;
}

/* Initial queue */
static inline void INIT_QUEUE(queue_t *q)
{
    if (q == NULL)
        return;

    q->size = 0;
    INIT_LIST_HEAD(&q->head);
}

/* Create an empty queue and initial it */
struct list_head *q_new()
{
    /* Create queue and initial it */
    queue_t *q = malloc(sizeof(queue_t));
    INIT_QUEUE(q);

    return (q != NULL) ? &q->head : NULL;
}

/* Free all storage used by queue */
void q_free(struct list_head *head)
{
    if (head == NULL)
        return;

    queue_t *q = container_of(head, queue_t, head);
    struct list_head *iter = NULL, *next = NULL;
    element_t *e = NULL;

    list_for_each_safe (iter, next, head) {
        e = container_of(iter, element_t, list);
        element_del(e, head);
    }

    free(q);
}

/* Insert an element at head of queue */
bool q_insert_head(struct list_head *head, char *s)
{
    if (head == NULL || s == NULL)
        return false;

    char *str = strdup(s);
    if (str == NULL)
        goto malloc_failed;
    element_t *e = element_new();
    if (e == NULL)
        goto malloc_failed;

    queue_t *q = container_of(head, queue_t, head);

    e->value = str;
    list_add(&e->list, head);
    q->size += 1;

    return true;

malloc_failed:
    if (str)
        free(str);
    if (e)
        free(e);
    return false;
}

/* Insert an element at tail of queue */
bool q_insert_tail(struct list_head *head, char *s)
{
    if (head == NULL || s == NULL)
        return false;

    char *str = strdup(s);
    if (str == NULL)
        goto malloc_failed;
    element_t *e = element_new();
    if (e == NULL)
        goto malloc_failed;

    queue_t *q = container_of(head, queue_t, head);

    e->value = str;
    list_add_tail(&e->list, head);
    q->size += 1;

    return true;

malloc_failed:
    if (str)
        free(str);
    if (e)
        free(e);
    return false;
}

/* Remove an element from head of queue */
element_t *q_remove_head(struct list_head *head, char *sp, size_t bufsize)
{
    if (head == NULL || sp == NULL || list_empty(head))
        return NULL;

    element_t *e = list_first_entry(head, element_t, list);
    queue_t *q = container_of(head, queue_t, head);

    strncpy(sp, e->value, bufsize - 1);
    sp[bufsize - 1] = '\0';
    list_del(&e->list);
    q->size -= 1;

    return e;
}

/* Remove an element from tail of queue */
element_t *q_remove_tail(struct list_head *head, char *sp, size_t bufsize)
{
    if (head == NULL || sp == NULL || list_empty(head))
        return NULL;

    element_t *e = list_last_entry(head, element_t, list);
    queue_t *q = container_of(head, queue_t, head);

    strncpy(sp, e->value, bufsize - 1);
    sp[bufsize - 1] = '\0';
    list_del(&e->list);
    q->size -= 1;

    return e;
}

/* Return number of elements in queue */
int q_size(struct list_head *head)
{
    if (head == NULL)
        return -1;

    queue_t *q = container_of(head, queue_t, head);
    return q->size;
}

/* Delete the middle node in queue */
bool q_delete_mid(struct list_head *head)
{
    // https://leetcode.com/problems/delete-the-middle-node-of-a-linked-list/
    if (head == NULL || list_empty(head))
        return false;

    struct list_head *front = head->next, *end = head->prev;
    element_t *e = NULL;

    while (front != end && front->next != end) {
        front = front->next;
        end = end->prev;
    }

    e = container_of(end, element_t, list);
    element_del(e, head);

    return true;
}

/* Delete all nodes that have duplicate string */
bool q_delete_dup(struct list_head *head)
{
    // https://leetcode.com/problems/remove-duplicates-from-sorted-list-ii/
    return true;
}

/* Swap every two adjacent nodes */
void q_swap(struct list_head *head)
{
    // https://leetcode.com/problems/swap-nodes-in-pairs/
    if (head == NULL || list_empty(head) || list_is_singular(head))
        return;

    struct list_head *iter = NULL, *next = NULL, *next_next = NULL,
                     *prev = NULL;

    for (iter = head->next, next_next = iter->next->next;
         iter != head && iter != head->prev;
         iter = next_next, next_next = iter->next->next) {
        prev = iter->prev;
        next = iter->next;

        iter->next = next_next;
        next_next->prev = iter;
        prev->next = next;
        next->prev = prev;
        next->next = iter;
        iter->prev = next;

        if (head->prev == next)
            head->prev = iter;
    }
}

/* Reverse elements in queue */
void q_reverse(struct list_head *head)
{
    if (head == NULL || list_empty(head) || list_is_singular(head))
        return;

    struct list_head *old_front = head->next, *old_end = head->prev;
    struct list_head *iter = NULL, *next = NULL, *tmp = NULL;

    list_for_each_safe (iter, next, head) {
        tmp = iter->prev;
        iter->prev = iter->next;
        iter->next = tmp;
    }

    head->next = old_end;
    head->prev = old_front;
}

/* Reverse the nodes of the list k at a time */
void q_reverseK(struct list_head *head, int k)
{
    // https://leetcode.com/problems/reverse-nodes-in-k-group/
}

void merge(struct list_head *head,
           struct list_head *left_head,
           struct list_head *right_head)
{
    struct list_head *left_iter = left_head->next,
                     *right_iter = right_head->next;
    struct list_head *left_next = left_iter->next,
                     *right_next = right_iter->next;
    element_t *left_e = NULL, *right_e = NULL;

    while (left_iter != left_head || right_iter != right_head) {
        if (left_iter == left_head) {
            list_move_tail(right_iter, head);
            right_iter = right_next;
            right_next = right_iter->next;
            continue;
        }

        if (right_iter == right_head) {
            list_move_tail(left_iter, head);
            left_iter = left_next;
            left_next = left_iter->next;
            continue;
        }

        left_e = container_of(left_iter, element_t, list);
        right_e = container_of(right_iter, element_t, list);

        if (strcmp(left_e->value, right_e->value) < 0) {
            list_move_tail(left_iter, head);
            left_iter = left_next;
            left_next = left_iter->next;
        } else {
            list_move_tail(right_iter, head);
            right_iter = right_next;
            right_next = right_iter->next;
        }
    }
}

void merge_sort(struct list_head *head)
{
    if (head == NULL || list_empty(head) || list_is_singular(head))
        return;

    LIST_HEAD(left_head);
    LIST_HEAD(right_head);
    struct list_head *front = head->next, *end = head->prev, *mid = NULL;
    struct list_head *iter = NULL, *next = NULL;

    while (front != end && front->next != end) {
        front = front->next;
        end = end->prev;
    }

    mid = end;

    for (iter = head->next, next = iter->next; iter != mid;
         iter = next, next = iter->next) {
        list_move_tail(iter, &left_head);
    }

    for (iter = mid, next = iter->next; iter != head;
         iter = next, next = iter->next) {
        list_move_tail(iter, &right_head);
    }

    merge_sort(&left_head);
    merge_sort(&right_head);
    merge(head, &left_head, &right_head);
}

/* Sort elements of queue in ascending order */
void q_sort(struct list_head *head)
{
    if (head == NULL || list_empty(head) || list_is_singular(head))
        return;

    merge_sort(head);
}

/* Remove every node which has a node with a strictly greater value anywhere to
 * the right side of it */
int q_descend(struct list_head *head)
{
    // https://leetcode.com/problems/remove-nodes-from-linked-list/
    return 0;
}

/* Merge all the queues into one sorted queue, which is in ascending order */
int q_merge(struct list_head *head)
{
    // https://leetcode.com/problems/merge-k-sorted-lists/
    return 0;
}
