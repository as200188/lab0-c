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
#define likely(x) __builtin_expect(!!(x), 1)
#define unlikely(x) __builtin_expect(!!(x), 0)

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

int compare(const void *a, const void *b)
{
    const struct list_head *a_list = a;
    const struct list_head *b_list = b;
    element_t *a_e = container_of(a_list, element_t, list);
    element_t *b_e = container_of(b_list, element_t, list);
    return strcmp(a_e->value, b_e->value);
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
        return 0;

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
    if (head == NULL || list_empty(head) || list_is_singular(head))
        return false;

    struct list_head *first = head->next, *second = first->next,
                     *second_next = second->next, *first_prev = NULL;
    element_t *e = NULL;

    while (first != head && second != head) {
        if (compare(first, second) == 0) {
            first_prev = first->prev;
            while (second != head && compare(first, second) == 0) {
                e = container_of(second, element_t, list);
                element_del(e, head);
                second = second_next;
                second_next = second->next;
            }

            e = container_of(first, element_t, list);
            element_del(e, head);
            first_prev->next = second;
            second->prev = first_prev;
        }

        first = second;
        second = second_next;
        second_next = second->next;
    }

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
    if (head == NULL || list_empty(head) || list_is_singular(head))
        return;

    int num_of_groups = q_size(head) / k;
    int i = 0, j;
    struct list_head *iter = head->next, *next = iter->next;
    LIST_HEAD(res_head);

    for (; i < num_of_groups; i++) {
        LIST_HEAD(tmp_head);

        for (j = 0; j < k; j++) {
            list_move_tail(iter, &tmp_head);
            iter = next;
            next = iter->next;
        }

        q_reverse(&tmp_head);
        list_splice_tail(&tmp_head, &res_head);
    }

    list_splice(&res_head, head);
}

void my_merge(struct list_head *head,
              struct list_head *left_head,
              struct list_head *right_head)
{
    struct list_head *left_iter = left_head->next,
                     *right_iter = right_head->next;
    struct list_head *left_next = left_iter->next,
                     *right_next = right_iter->next;

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


        if (compare(left_iter, right_iter) < 0) {
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
    my_merge(head, &left_head, &right_head);
}

static struct list_head *merge(void *priv,
                               struct list_head *a,
                               struct list_head *b)
{
    struct list_head *head = NULL, **tail = &head;

    for (;;) {
        /* if equal, take 'a' -- important for sort stability */
        if (compare(a, b) <= 0) {
            *tail = a;
            tail = &a->next;
            a = a->next;
            if (!a) {
                *tail = b;
                break;
            }
        } else {
            *tail = b;
            tail = &b->next;
            b = b->next;
            if (!b) {
                *tail = a;
                break;
            }
        }
    }
    return head;
}

static void merge_final(void *priv,
                        struct list_head *head,
                        struct list_head *a,
                        struct list_head *b)
{
    struct list_head *tail = head;
    unsigned char count = 0;

    for (;;) {
        /* if equal, take 'a' -- important for sort stability */
        if (compare(a, b) <= 0) {
            tail->next = a;
            a->prev = tail;
            tail = a;
            a = a->next;
            if (!a)
                break;
        } else {
            tail->next = b;
            b->prev = tail;
            tail = b;
            b = b->next;
            if (!b) {
                b = a;
                break;
            }
        }
    }

    /* Finish linking remainder of list b on to tail */
    tail->next = b;
    do {
        /*
         * If the merge is highly unbalanced (e.g. the input is
         * already sorted), this loop may run many iterations.
         * Continue callbacks to the client even though no
         * element comparison is needed, so the client's cmp()
         * routine can invoke cond_resched() periodically.
         */
        if (unlikely(!++count))
            compare(b, b);
        b->prev = tail;
        tail = b;
        b = b->next;
    } while (b);

    /* And the final links to make a circular doubly-linked list */
    tail->next = head;
    head->prev = tail;
}

void list_sort(void *priv, struct list_head *head)
{
    struct list_head *list = head->next, *pending = NULL;
    size_t count = 0; /* Count of pending */

    if (list == head->prev) /* Zero or one elements */
        return;

    /* Convert to a null-terminated singly-linked list. */
    head->prev->next = NULL;

    /*
     * Data structure invariants:
     * - All lists are singly linked and null-terminated; prev
     *   pointers are not maintained.
     * - pending is a prev-linked "list of lists" of sorted
     *   sublists awaiting further merging.
     * - Each of the sorted sublists is power-of-two in size.
     * - Sublists are sorted by size and age, smallest & newest at front.
     * - There are zero to two sublists of each size.
     * - A pair of pending sublists are merged as soon as the number
     *   of following pending elements equals their size (i.e.
     *   each time count reaches an odd multiple of that size).
     *   That ensures each later final merge will be at worst 2:1.
     * - Each round consists of:
     *   - Merging the two sublists selected by the highest bit
     *     which flips when count is incremented, and
     *   - Adding an element from the input as a size-1 sublist.
     */
    do {
        size_t bits;
        struct list_head **tail = &pending;

        /* Find the least-significant clear bit in count */
        for (bits = count; bits & 1; bits >>= 1)
            tail = &(*tail)->prev;
        /* Do the indicated merge */
        if (likely(bits)) {
            struct list_head *a = *tail, *b = a->prev;

            a = merge(priv, b, a);
            /* Install the merged result in place of the inputs */
            a->prev = b->prev;
            *tail = a;
        }

        /* Move one element from input list to pending */
        list->prev = pending;
        pending = list;
        list = list->next;
        pending->next = NULL;
        count++;
    } while (list);

    /* End of input; merge together all the pending lists. */
    list = pending;
    pending = pending->prev;
    for (;;) {
        struct list_head *next = pending->prev;

        if (!next)
            break;
        list = merge(priv, pending, list);
        pending = next;
    }
    /* The final merge, rebuilding prev links */
    merge_final(priv, head, pending, list);
}

/* Sort elements of queue in ascending order */
void q_sort(struct list_head *head)
{
    if (head == NULL || list_empty(head) || list_is_singular(head))
        return;

    list_sort(NULL, head);
}

/* Remove every node which has a node with a strictly greater value anywhere to
 * the right side of it */
int q_descend(struct list_head *head)
{
    // https://leetcode.com/problems/remove-nodes-from-linked-list/
    if (head == NULL || list_empty(head) || list_is_singular(head))
        return 0;

    struct list_head *iter = head->prev, *prev = iter->prev, *max = iter;
    element_t *e = NULL;

    while (iter != head) {
        if (compare(iter, max) > 0)
            max = iter;
        else if (compare(iter, max) < 0) {
            e = container_of(iter, element_t, list);
            element_del(e, head);
        }

        iter = prev;
        prev = iter->prev;
    }

    return q_size(head);
}

/* Merge all the queues into one sorted queue, which is in ascending order */
int q_merge(struct list_head *head)
{
    // https://leetcode.com/problems/merge-k-sorted-lists/
    return 0;
}
