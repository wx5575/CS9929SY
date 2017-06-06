/**
  ******************************************************************************
  * @file    cs_list.c
  * @author  王鑫
  * @version V0.0.1
  * @date    2017.4.18
  * @brief   链表操作
  ******************************************************************************
  */

#include "cs_list.h"

/* FUNCTIONS */
void list_init( struct list_head *list)
{
	INIT_LIST_HEAD(list);
}
/* Insert an entry _after_ the specified entry */
void list_add( struct list_head *newent, struct list_head *afterthisent )
{
    struct list_head *next = afterthisent->next;
    newent->next = next;
    newent->prev = afterthisent;
    afterthisent->next = newent;
    next->prev = newent;
} /* list_add() */

/* Insert an entry _before_ the specified entry */
void list_add_tail( struct list_head *newent, struct list_head *beforethisent )
{
    struct list_head *prev = beforethisent->prev;
    newent->prev = prev;
    newent->next = beforethisent;
    beforethisent->prev = newent;
    prev->next = newent;
} /* list_add_tail() */

/* Delete the specified entry */
void list_del( struct list_head *ent )
{
    ent->prev->next = ent->next;
    ent->next->prev = ent->prev;
} /* list_del() */

/* Is this list empty? */
int list_empty( struct list_head *list )
{
    return ( list->next == list );
} /* list_empty() */

/************************ (C) COPYRIGHT Nanjing Changsheng *****END OF FILE****/

