#include <stdint.h>
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


/* Create an empty queue */
struct list_head *q_new()
{
    /* Create queue and initial it */
    queue_t *q = malloc(sizeof(queue_t));

    q->size = 0;
    INIT_LIST_HEAD(&q->head);

    return &q->head;
}

/* Free all storage used by queue */
void q_free(struct list_head *l) {}

/* Insert an element at head of queue */
bool q_insert_head(struct list_head *head, char *s)
{
    if (head == NULL)
        return false;
    if (s == NULL)
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
    if (head == NULL)
        return false;
    if (s == NULL)
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
    return NULL;
}

/* Remove an element from tail of queue */
element_t *q_remove_tail(struct list_head *head, char *sp, size_t bufsize)
{
    return NULL;
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
