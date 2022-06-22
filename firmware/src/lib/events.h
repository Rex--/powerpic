/** @file events.h
 * 
 * Event library for PowerOS.
 * 
 * This library implements a FIFO queue for system-wide events.
*/

#ifndef _events_h_
#define _events_h_

// MAX 255
#define EVENT_QUEUE_SIZE        8

/**
 * Event data type.
 * 
 * NOTE: We never actually use this type anywhere. Should we?
 * 
 * Each event is 2 bytes long and consists of:
 *  type - event type byte.
 *  data - event data byte.
 * 
 * These two bytes combine to form:
 *  id - 16-bit event ID.
*/
typedef struct
{
    union
    {
        struct
        {
            unsigned char type;
            unsigned char data;
        };
        unsigned int id;
    };
} event_t;

/** Macro to extract the event type from an id. */
#define     EVENT_TYPE(id)          ((id) & 0xFF)

/** Macro to extract the event data from an id. */
#define     EVENT_DATA(id)          ((id) >> 8)

/** Macro to combine an event type and its data to form an id. */
#define     EVENT_ID(type, data)    (((data) << 8) | (type))

/**
 * Add an event to the queue.
 * 
 * @param[in]   id      16-bit event ID.
*/
void
event_add (unsigned int id);

/**
 * Add an event to the queue for ISRs.
 * 
 * @param[in]   id      16-bit event ID.
*/
void
event_isr (unsigned int id);

/**
 * Get an event from the queue.
 * 
 * This returns the first event in the queue and increments the head of queue.
 * 
 * @returns     16-bit event ID.
*/
unsigned int
event_get (void);

/**
 * Check the first event without consuming it.
 * This return the first event in the queue similiar to event_get(), but does
 * not increment the head.
*/
unsigned int
event_check (void);

// Pre-Defined Event Types.
// Each library implements their own event data.
// NOTE: Libraries also implement macros with these values. Should we get rid
// of them here?

#define EVENT_TICK          0x01    // Ticks

#define EVENT_ALARM         0x0A    // 'A' for alarm

#define EVENT_BUTTON        0x0B    // 'B' for button

#define EVENT_KEYPAD        0x0C    // 'C' for ceypad :)

#endif

// EOF //
