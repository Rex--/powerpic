#ifndef EVENT_H
#define EVENT_H

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

void event_add (unsigned char event_id, unsigned char event_data);

unsigned int event_get_last (void);
event_t event_get_last_event (void);

#endif