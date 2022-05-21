/** @file mode.c
 * Types and util procedures used by Mode Applications.
*/

#include <xc.h>
#include <stdio.h>

#include "dev_config.h"

#include "lib/mode.h"

#if DEV_BUILD
#   define DEBUG_PRINTS
#endif

void
mode_init (mode_app_t * mode_app, mode_config_t * mode_config)
{
    if (mode_app->init != NULL)
    {
#       ifdef DEBUG_PRINTS
        printf("Initializing mode: %s\n\r", mode_app->id);
#       endif
        mode_app->init(mode_config);
    }
    else
    {
        printf("Noting empty init function: %s\n\r", mode_app->id);
    }
}

void
mode_start (mode_app_t * mode_app, mode_config_t * mode_config)
{
    if (mode_app->init != NULL)
    {
#       ifdef DEBUG_PRINTS
        printf("Starting mode: %s\n\r", mode_app->id);
#       endif        
        mode_app->start(mode_config);
    }
    else
    {
        printf("Noting empty start function: %s\n\r", mode_app->id);
    }
}

void
mode_run (mode_app_t * mode_app, mode_config_t * mode_config, unsigned int event)
{
    if (mode_app->run != NULL)
    {
#       ifdef DEBUG_PRINTS
        printf("Running mode: %s (%x)\n\r", mode_app->id, event);
#       endif
        mode_app->run(mode_config, event);
    }
    else
    {
        printf("Noting empty run function: %s\n\r", mode_app->id);
    }
}

void
mode_stop (mode_app_t * mode_app, mode_config_t * mode_config)
{
    if (mode_app->stop != NULL)
    {
#       ifdef DEBUG_PRINTS
        printf("Stopping mode: %s\n\r", mode_app->id);
#       endif
        mode_app->stop(mode_config);
    }
    else
    {
        printf("Noting empty stop function: %s\n\r", mode_app->id);
    }
}

/*** EOF ***/
