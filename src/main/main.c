#include <stdio.h>

#include "picon.h"
#include "picon/dev.h"          
#include "picon/io.h"           
#include "picon/console.h"      
#include "picon/ioctl.h"        
#include "picon/uart.h"
#include "picon/mem.h"
#include "shell/shell.h"

#include "FreeRTOS.h"
#include "FreeRTOSConfig.h"
#include "task.h"
#include "rtos.h"


static const DEVICE_FILE *dt[] = {
	DEV_ENTRY("/dev/mem", MEM_DEV, 0, NULL)
        DEV_ENTRY("/dev/uart0", UART_DEV, 0, NULL)
        NULL
};


int main()
{
	picon_init();
	io_init();
        dev_init(dt);

        CONSOLE_CREATE("/dev/uart0", "ttyS0");

	rtos_task_start_scheduler();

	for(;;);
}

