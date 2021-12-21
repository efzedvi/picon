#include <stdio.h>

#include "picon.h"

#include "FreeRTOS.h"
#include "FreeRTOSConfig.h"
#include "task.h"
#include "rtos.h"


int main()
{
	picon_init();

	rtos_task_start_scheduler();

	for(;;);
}

