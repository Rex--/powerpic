/** @file events.h
 * 
 * Event library for CasiOS.
*/

#include <xc.h>

#include "events.h"


/**
 * Array to hold our events. The size is configurable via macro. 
 * We store them as plain ints instead of our data type because we never
 * actually use the events.
*/
static unsigned int event_queue[EVENT_QUEUE_SIZE] = {0};

/** The head of our queue. i.e. the event that happened first. */
static unsigned char event_queue_head = 0;

/** The tail of our queue. i.e. the event that happened last, but most recently. */
static unsigned char event_queue_tail = 0;


void
event_add (unsigned int id)
{
    // Add event to queue and increment tail
    //
    event_queue[event_queue_tail++] = id;
    if (EVENT_QUEUE_SIZE <= event_queue_tail)
    {
        event_queue_tail = 0;
    }
    
    // Increment head if we're about to overlap.
    //
    if (event_queue_tail == event_queue_head)
    {
        event_queue_head++;
        if (EVENT_QUEUE_SIZE <= event_queue_head)
        {
            event_queue_head = 0;
        }
    }
}

void
event_isr (unsigned int id)
{
    // Add event to queue and increment tail
    //
    event_queue[event_queue_tail++] = id;
    if (EVENT_QUEUE_SIZE <= event_queue_tail)
    {
        event_queue_tail = 0;
    }
    
    // Increment head if we're about to overlap.
    //
    if (event_queue_tail == event_queue_head)
    {
        event_queue_head++;
        if (EVENT_QUEUE_SIZE <= event_queue_head)
        {
            event_queue_head = 0;
        }
    }
}

unsigned int
event_get (void)
{
    // Event queue is empty. We can return quickly
    //
    if (event_queue_head == event_queue_tail)
    {
        return 0;
    }

    unsigned int last_event = event_queue[event_queue_head++];
    if (EVENT_QUEUE_SIZE <= event_queue_head)
    {
        event_queue_head = 0;
    }

    return last_event;
}

unsigned int
event_check (void)
{
    // Event queue is empty
    //
    if (event_queue_head == event_queue_tail)
    {
        return 0;
    }

    return event_queue[event_queue_head];
}


// EOF //
