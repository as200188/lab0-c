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
        free(e->value);
        e->value = NULL;
        list_del_init(iter);
        free(e);
    }

    free(q);
}

/* Insert an element at head of queue */
bool q_insert_head(struct list_head *head, char *s)
{
    if (head == NULL || s == NULL)
        return false;

    char *str = strdup(s);
    element_t *e = element_new();
    queue_t *q = container_of(head, queue_t, head);

    e->value = str;
    list_add(&e->list, head);
    q->size += 1;

    return true;
}

/* Insert an element at tail of queue */
bool q_insert_tail(struct list_head *head, char *s)
{
    if (head == NULL || s == NULL)
        return false;

    char *str = strdup(s);
    element_t *e = element_new();
    queue_t *q = container_of(head, queue_t, head);

    e->value = str;
    list_add_tail(&e->list, head);
    q->size += 1;

    return true;
}

/* Remove an element from head of queue */
element_t *q_remove_head(struct list_head *head, char *sp, size_t bufsize)
{
    if (head == NULL || sp == NULL || q_size(head) == 0)
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
    if (head == NULL || sp == NULL || q_size(head) == 0)
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
}

/* Reverse elements in queue */
void q_reverse(struct list_head *head) {}

/* Reverse the nodes of the list k at a time */
void q_reverseK(struct list_head *head, int k)
{
    // https://leetcode.com/problems/reverse-nodes-in-k-group/
}

/* Sort elements of queue in ascending order */
void q_sort(struct list_head *head) {}

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
