/**
  ******************************************************************************
  * @file    cs_list.h
  * @author  王鑫
  * @version V0.0.1
  * @date    2017.4.18
  * @brief   链表操作
  ******************************************************************************
  */

#ifndef __CS_LIST_H__
#define __CS_LIST_H__


/* -----------------------------------------------------------------------*/

/* Doubly linked list implementation to replace the GPL'd one used in
   the Linux kernel. */

#include <stddef.h>


/* TYPES */

struct list_head {
    struct list_head *next;
    struct list_head *prev;
};
typedef struct list_head CS_LIST;
/* MACROS */

#define LIST_HEAD_INIT(name) { &(name), &(name) }

#define LIST_HEAD(name) \
        struct list_head name = LIST_HEAD_INIT(name)

#define INIT_LIST_HEAD( _list_ )              \
(_list_)->next = (_list_)->prev = (_list_); 


/* list_entry - Assuming you have a struct of type _type_ that contains a
   list which has the name _member_ in that struct type, then given the
   address of that list in the struct, _list_, this returns the address
   of the container structure */

#define list_entry( _list_, _type_, _member_ ) \
    ((_type_ *)((char *)(_list_)-(char *)(offsetof(_type_,_member_))))

/* list_for_each - using _ent_, iterate through list _list_ */

#define list_for_each( _ent_, _list_ )   \
    for ( (_ent_) = (_list_)->next;      \
    (_ent_) != (_list_);                 \
    (_ent_) = (_ent_)->next )

#define list_for_each_reverse( _ent_, _list_ )   \
    for ( (_ent_) = (_list_)->prev;      \
    (_ent_) != (_list_);                 \
    (_ent_) = (_ent_)->prev )
/*
 * list_for_each_entry - this function can be use to iterate over all
 * items in a list* _list_ with it's head at _head_ and link _item_
 */
#if defined (__GNUC__)   
#define list_for_each_entry(_list_, _head_, _item_)                     \
for ((_list_) = list_entry((_head_)->next, typeof(*_list_), _item_); \
     &((_list_)->_item_) != (_head_);                                 \
     (_list_) = list_entry((_list_)->_item_.next, typeof(*_list_), _item_))

#elif defined (MSVC)
#define list_for_each_entry(_list_, _head_, _item_)                     \
for ((_list_) = list_entry((_head_)->next, struct jffs2_compressor, _item_); \
     &((_list_)->_item_) != (_head_);                                 \
     (_list_) = list_entry((_list_)->_item_.next, struct jffs2_compressor, _item_))
#else
#endif

void list_init( struct list_head *list);
void list_add( struct list_head *newent, struct list_head *afterthisent );
void list_add_tail( struct list_head *newent, struct list_head *beforethisent );
void list_del( struct list_head *ent);
int list_empty( struct list_head *list );

/* -----------------------------------------------------------------------*/
#endif /* #ifndef __CS_LIST_H__ */
/* EOF list.h */
