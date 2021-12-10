#include <stdio.h>

#include <stdio.h>

#include "pico/stdlib.h"
#include "hardware/gpio.h"

#include "FreeRTOS.h"
#include "FreeRTOSConfig.h"
#include "task.h"
#include "rtos.h"

#define LED_PIN 25

void heartbeat(void *param)
{
	gpio_put(LED_PIN, 0);
	printf("off\n");
	RTOS_TASK_DELAY(1000);
	gpio_put(LED_PIN, 1);
	printf("on\n");
	RTOS_TASK_DELAY(1000);
}

int main()
{
	stdio_init_all();

	gpio_init(LED_PIN);
	gpio_set_dir(LED_PIN, GPIO_OUT);

	gpio_put(LED_PIN, 1);

	rtos_task_handle_t task_handle = NULL;

	uint32_t status = RTOS_TASK_CREATE(
		heartbeat,
		"hrtbt",
		64,
		NULL,
		RTOS_TSK_IDLE_PRIORITY,
		&task_handle);

	rtos_task_start_scheduler();

	for(;;);
}

