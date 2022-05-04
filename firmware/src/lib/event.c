/** @file events.c
 * This module provides interfaces and funtions for events.
 * 
*/

#include <xc.h>

#include "lib/event.h"

#define EVENT_QUEUE_SIZE        8


static event_t event_queue[EVENT_QUEUE_SIZE];

static int event_queue_head = 0;
static int event_queue_tail = 0;


void
event_add (unsigned char event_type, unsigned char event_data)
{
    event_t new_event;
    new_event.type = event_type;
    new_event.data = event_data;


    // Add event to queue and increment tail
    //
    event_queue[event_queue_tail++] = new_event;
    if (EVENT_QUEUE_SIZE <= event_queue_tail)
    {
        event_queue_tail = 0;
    }

    // Increment head if we're about to overlap
    //
    if (event_queue_tail == event_queue_head)
    {
        event_queue_head++;
    }
}

unsigned int
event_get_last (void)
{
    event_t last_event;

    if (event_queue_head == event_queue_tail)
    {
        return 0;
    }
    last_event = event_queue[event_queue_head++];
    if (EVENT_QUEUE_SIZE <= event_queue_head)
    {
        event_queue_head = 0;
    }

    return last_event.id;
}

event_t
event_get_last_event (void)
{
    event_t last_event;

    if (event_queue_head == event_queue_tail)
    {
        last_event.id = 0;
        return last_event;
    }
    last_event = event_queue[event_queue_head++];
    if (EVENT_QUEUE_SIZE <= event_queue_head)
    {
        event_queue_head = 0;
    }

    return last_event;
}

