#ifndef _UN_HUN_
#define _UN_HUN_

#include "FreeRTOSConfig.h"
#include "FreeRTOS.h"
#include "portmacro.h"
#include "atomic.h"
#include "croutine.h"
#include "deprecated_definitions.h"
#include "event_groups.h"
#include "list.h"
#include "message_buffer.h"
#include "mpu_wrappers.h"
#include "portable.h"
#include "projdefs.h"
#include "queue.h"
#include "semphr.h"
#include "stack_macros.h"
#include "stream_buffer.h"
#include "task.h"
#include "timers.h"

/* Macros */
/* from event_groups.h */
#define RTOS_ALL_SYNC_BITS                           	 ALL_SYNC_BITS
/* from atomic.h */
#define RTOS_ATOMIC_COMPARE_AND_SWAP_FAILURE         	 ATOMIC_COMPARE_AND_SWAP_FAILURE
#define RTOS_ATOMIC_COMPARE_AND_SWAP_SUCCESS         	 ATOMIC_COMPARE_AND_SWAP_SUCCESS
#define RTOS_ATOMIC_H                                	 ATOMIC_H
/* from projdefs.h */
#define RTOS_BIG_ENDIAN                              	 pdBIG_ENDIAN
/* from event_groups.h */
#define RTOS_BIT_0                                   	 BIT_0
#define RTOS_BIT_4                                   	 BIT_4
/* from FreeRTOS.h */
#define RTOS_CONFIG_APPLICATION_ALLOCATED_HEAP       	 configAPPLICATION_ALLOCATED_HEAP
#define RTOS_CONFIG_ASSERT_DEFINED                   	 configASSERT_DEFINED
#define RTOS_CONFIG_CHECK_FOR_STACK_OVERFLOW         	 configCHECK_FOR_STACK_OVERFLOW
#define RTOS_CONFIG_ENABLE_BACKWARD_COMPATIBILITY    	 configENABLE_BACKWARD_COMPATIBILITY
#define RTOS_CONFIG_ENABLE_FPU                       	 configENABLE_FPU
#define RTOS_CONFIG_ENABLE_MPU                       	 configENABLE_MPU
#define RTOS_CONFIG_ENABLE_TRUSTZONE                 	 configENABLE_TRUSTZONE
#define RTOS_CONFIG_EXPECTED_IDLE_TIME_BEFORE_SLEEP  	 configEXPECTED_IDLE_TIME_BEFORE_SLEEP
#define RTOS_CONFIG_GENERATE_RUN_TIME_STATS          	 configGENERATE_RUN_TIME_STATS
#define RTOS_CONFIG_IDLE_SHOULD_YIELD                	 configIDLE_SHOULD_YIELD
#define RTOS_CONFIG_INCLUDE_APPLICATION_DEFINED_PRIVILEGED_FUNCTIONS 	 configINCLUDE_APPLICATION_DEFINED_PRIVILEGED_FUNCTIONS
#define RTOS_CONFIG_INCLUDE_FREERTOS_TASK_C_ADDITIONS_H 	 configINCLUDE_FREERTOS_TASK_C_ADDITIONS_H
#define RTOS_CONFIG_INITIAL_TICK_COUNT               	 configINITIAL_TICK_COUNT
/* from FreeRTOSConfig.h */
#define RTOS_CONFIG_KERNEL_INTERRUPT_PRIORITY        	 configKERNEL_INTERRUPT_PRIORITY
/* from list.h */
#define RTOS_CONFIG_LIST_VOLATILE                    	 configLIST_VOLATILE
/* from FreeRTOSConfig.h */
#define RTOS_CONFIG_MAX_API_CALL_INTERRUPT_PRIORITY  	 configMAX_API_CALL_INTERRUPT_PRIORITY
#define RTOS_CONFIG_MAX_CO_ROUTINE_PRIORITIES        	 configMAX_CO_ROUTINE_PRIORITIES
#define RTOS_CONFIG_MAX_PRIORITIES                   	 configMAX_PRIORITIES
#define RTOS_CONFIG_MAX_SYSCALL_INTERRUPT_PRIORITY   	 configMAX_SYSCALL_INTERRUPT_PRIORITY
/* from FreeRTOS.h */
#define RTOS_CONFIG_MAX_TASK_NAME_LEN                	 configMAX_TASK_NAME_LEN
#define RTOS_CONFIG_MESSAGE_BUFFER_LENGTH_TYPE       	 configMESSAGE_BUFFER_LENGTH_TYPE
/* from FreeRTOSConfig.h */
#define RTOS_CONFIG_MINIMAL_STACK_SIZE               	 configMINIMAL_STACK_SIZE
/* from FreeRTOS.h */
#define RTOS_CONFIG_NUM_CORES                        	 configNUM_CORES
#define RTOS_CONFIG_NUM_THREAD_LOCAL_STORAGE_POINTERS 	 configNUM_THREAD_LOCAL_STORAGE_POINTERS
#define RTOS_CONFIG_PRECONDITION_DEFINED             	 configPRECONDITION_DEFINED
#define RTOS_CONFIG_QUEUE_REGISTRY_SIZE              	 configQUEUE_REGISTRY_SIZE
#define RTOS_CONFIG_RECORD_STACK_HIGH_ADDRESS        	 configRECORD_STACK_HIGH_ADDRESS
#define RTOS_CONFIG_RUN_FREERTOS_SECURE_ONLY         	 configRUN_FREERTOS_SECURE_ONLY
#define RTOS_CONFIG_RUN_MULTIPLE_PRIORITIES          	 configRUN_MULTIPLE_PRIORITIES
#define RTOS_CONFIG_STACK_ALLOCATION_FROM_SEPARATE_HEAP 	 configSTACK_ALLOCATION_FROM_SEPARATE_HEAP
#define RTOS_CONFIG_STACK_DEPTH_TYPE                 	 configSTACK_DEPTH_TYPE
#define RTOS_CONFIG_SUPPORT_DYNAMIC_ALLOCATION       	 configSUPPORT_DYNAMIC_ALLOCATION
/* from FreeRTOSConfig.h */
#define RTOS_CONFIG_SUPPORT_PICO_SYNC_INTEROP        	 configSUPPORT_PICO_SYNC_INTEROP
#define RTOS_CONFIG_SUPPORT_PICO_TIME_INTEROP        	 configSUPPORT_PICO_TIME_INTEROP
/* from FreeRTOS.h */
#define RTOS_CONFIG_SUPPORT_STATIC_ALLOCATION        	 configSUPPORT_STATIC_ALLOCATION
#define RTOS_CONFIG_TASK_NOTIFICATION_ARRAY_ENTRIES  	 configTASK_NOTIFICATION_ARRAY_ENTRIES
/* from FreeRTOSConfig.h */
#define RTOS_CONFIG_TICK_CORE                        	 configTICK_CORE
#define RTOS_CONFIG_TICK_RATE_HZ                     	 configTICK_RATE_HZ
#define RTOS_CONFIG_TIMER_QUEUE_LENGTH               	 configTIMER_QUEUE_LENGTH
#define RTOS_CONFIG_TIMER_TASK_PRIORITY              	 configTIMER_TASK_PRIORITY
#define RTOS_CONFIG_TIMER_TASK_STACK_DEPTH           	 configTIMER_TASK_STACK_DEPTH
#define RTOS_CONFIG_TOTAL_HEAP_SIZE                  	 configTOTAL_HEAP_SIZE
#define RTOS_CONFIG_USE_16_BIT_TICKS                 	 configUSE_16_BIT_TICKS
/* from FreeRTOS.h */
#define RTOS_CONFIG_USE_ALTERNATIVE_API              	 configUSE_ALTERNATIVE_API
#define RTOS_CONFIG_USE_APPLICATION_TASK_TAG         	 configUSE_APPLICATION_TASK_TAG
#define RTOS_CONFIG_USE_CORE_AFFINITY                	 configUSE_CORE_AFFINITY
#define RTOS_CONFIG_USE_COUNTING_SEMAPHORES          	 configUSE_COUNTING_SEMAPHORES
#define RTOS_CONFIG_USE_CO_ROUTINES                  	 configUSE_CO_ROUTINES
#define RTOS_CONFIG_USE_DAEMON_TASK_STARTUP_HOOK     	 configUSE_DAEMON_TASK_STARTUP_HOOK
/* from FreeRTOSConfig.h */
#define RTOS_CONFIG_USE_IDLE_HOOK                    	 configUSE_IDLE_HOOK
/* from projdefs.h */
#define RTOS_CONFIG_USE_LIST_DATA_INTEGRITY_CHECK_BYTES 	 configUSE_LIST_DATA_INTEGRITY_CHECK_BYTES
/* from FreeRTOS.h */
#define RTOS_CONFIG_USE_MALLOC_FAILED_HOOK           	 configUSE_MALLOC_FAILED_HOOK
#define RTOS_CONFIG_USE_MUTEXES                      	 configUSE_MUTEXES
#define RTOS_CONFIG_USE_NEWLIB_REENTRANT             	 configUSE_NEWLIB_REENTRANT
#define RTOS_CONFIG_USE_PORT_OPTIMISED_TASK_SELECTION 	 configUSE_PORT_OPTIMISED_TASK_SELECTION
#define RTOS_CONFIG_USE_POSIX_ERRNO                  	 configUSE_POSIX_ERRNO
/* from FreeRTOSConfig.h */
#define RTOS_CONFIG_USE_PREEMPTION                   	 configUSE_PREEMPTION
/* from FreeRTOS.h */
#define RTOS_CONFIG_USE_QUEUE_SETS                   	 configUSE_QUEUE_SETS
#define RTOS_CONFIG_USE_RECURSIVE_MUTEXES            	 configUSE_RECURSIVE_MUTEXES
#define RTOS_CONFIG_USE_STATS_FORMATTING_FUNCTIONS   	 configUSE_STATS_FORMATTING_FUNCTIONS
#define RTOS_CONFIG_USE_TASK_FPU_SUPPORT             	 configUSE_TASK_FPU_SUPPORT
#define RTOS_CONFIG_USE_TASK_NOTIFICATIONS           	 configUSE_TASK_NOTIFICATIONS
#define RTOS_CONFIG_USE_TASK_PREEMPTION_DISABLE      	 configUSE_TASK_PREEMPTION_DISABLE
#define RTOS_CONFIG_USE_TICKLESS_IDLE                	 configUSE_TICKLESS_IDLE
/* from FreeRTOSConfig.h */
#define RTOS_CONFIG_USE_TICK_HOOK                    	 configUSE_TICK_HOOK
/* from FreeRTOS.h */
#define RTOS_CONFIG_USE_TIMERS                       	 configUSE_TIMERS
#define RTOS_CONFIG_USE_TIME_SLICING                 	 configUSE_TIME_SLICING
#define RTOS_CONFIG_USE_TRACE_FACILITY               	 configUSE_TRACE_FACILITY
/* from croutine.h */
#define RTOS_CO_ROUTINE_H                            	 CO_ROUTINE_H
/* from FreeRTOS.h */
#define RTOS_CO_ROUTINE_HANDLE                       	 xCoRoutineHandle
/* from deprecated_definitions.h */
#define RTOS_DEPRECATED_DEFINITIONS_H                	 DEPRECATED_DEFINITIONS_H
/* from projdefs.h */
#define RTOS_ERR_COULD_NOT_ALLOCATE_REQUIRED_MEMORY  	 errCOULD_NOT_ALLOCATE_REQUIRED_MEMORY
#define RTOS_ERR_QUEUE_BLOCKED                       	 errQUEUE_BLOCKED
#define RTOS_ERR_QUEUE_EMPTY                         	 errQUEUE_EMPTY
#define RTOS_ERR_QUEUE_FULL                          	 errQUEUE_FULL
#define RTOS_ERR_QUEUE_YIELD                         	 errQUEUE_YIELD
/* from event_groups.h */
#define RTOS_EVENT_GROUPS_H                          	 EVENT_GROUPS_H
/* from mpu_wrappers.h */
#define RTOS_EVENT_GROUP_CLEAR_BITS                  	 xEventGroupClearBits
/* from event_groups.h */
#define RTOS_EVENT_GROUP_CLEAR_BITS_FROM_ISR         	 xEventGroupClearBitsFromISR
/* from mpu_wrappers.h */
#define RTOS_EVENT_GROUP_CREATE                      	 xEventGroupCreate
#define RTOS_EVENT_GROUP_CREATE_STATIC               	 xEventGroupCreateStatic
#define RTOS_EVENT_GROUP_DELETE                      	 vEventGroupDelete
#define RTOS_EVENT_GROUP_SET_BITS                    	 xEventGroupSetBits
/* from event_groups.h */
#define RTOS_EVENT_GROUP_SET_BITS_FROM_ISR           	 xEventGroupSetBitsFromISR
/* from mpu_wrappers.h */
#define RTOS_EVENT_GROUP_SYNC                        	 xEventGroupSync
#define RTOS_EVENT_GROUP_WAIT_BITS                   	 xEventGroupWaitBits
/* from projdefs.h */
#define RTOS_FAIL                                    	 pdFAIL
#define RTOS_FALSE                                   	 pdFALSE
#define RTOS_FREERTOS_BIG_ENDIAN                     	 pdFREERTOS_BIG_ENDIAN
/* from FreeRTOSConfig.h */
#define RTOS_FREERTOS_CONFIG_H                       	 FREERTOS_CONFIG_H
/* from projdefs.h */
#define RTOS_FREERTOS_ERRNO_EACCES                   	 pdFREERTOS_ERRNO_EACCES
#define RTOS_FREERTOS_ERRNO_EADDRINUSE               	 pdFREERTOS_ERRNO_EADDRINUSE
#define RTOS_FREERTOS_ERRNO_EADDRNOTAVAIL            	 pdFREERTOS_ERRNO_EADDRNOTAVAIL
#define RTOS_FREERTOS_ERRNO_EAGAIN                   	 pdFREERTOS_ERRNO_EAGAIN
#define RTOS_FREERTOS_ERRNO_EALREADY                 	 pdFREERTOS_ERRNO_EALREADY
#define RTOS_FREERTOS_ERRNO_EBADE                    	 pdFREERTOS_ERRNO_EBADE
#define RTOS_FREERTOS_ERRNO_EBADF                    	 pdFREERTOS_ERRNO_EBADF
#define RTOS_FREERTOS_ERRNO_EBUSY                    	 pdFREERTOS_ERRNO_EBUSY
#define RTOS_FREERTOS_ERRNO_ECANCELED                	 pdFREERTOS_ERRNO_ECANCELED
#define RTOS_FREERTOS_ERRNO_EEXIST                   	 pdFREERTOS_ERRNO_EEXIST
#define RTOS_FREERTOS_ERRNO_EFAULT                   	 pdFREERTOS_ERRNO_EFAULT
#define RTOS_FREERTOS_ERRNO_EFTYPE                   	 pdFREERTOS_ERRNO_EFTYPE
#define RTOS_FREERTOS_ERRNO_EILSEQ                   	 pdFREERTOS_ERRNO_EILSEQ
#define RTOS_FREERTOS_ERRNO_EINPROGRESS              	 pdFREERTOS_ERRNO_EINPROGRESS
#define RTOS_FREERTOS_ERRNO_EINTR                    	 pdFREERTOS_ERRNO_EINTR
#define RTOS_FREERTOS_ERRNO_EINVAL                   	 pdFREERTOS_ERRNO_EINVAL
#define RTOS_FREERTOS_ERRNO_EIO                      	 pdFREERTOS_ERRNO_EIO
#define RTOS_FREERTOS_ERRNO_EISCONN                  	 pdFREERTOS_ERRNO_EISCONN
#define RTOS_FREERTOS_ERRNO_EISDIR                   	 pdFREERTOS_ERRNO_EISDIR
#define RTOS_FREERTOS_ERRNO_ENAMETOOLONG             	 pdFREERTOS_ERRNO_ENAMETOOLONG
#define RTOS_FREERTOS_ERRNO_ENMFILE                  	 pdFREERTOS_ERRNO_ENMFILE
#define RTOS_FREERTOS_ERRNO_ENOBUFS                  	 pdFREERTOS_ERRNO_ENOBUFS
#define RTOS_FREERTOS_ERRNO_ENODEV                   	 pdFREERTOS_ERRNO_ENODEV
#define RTOS_FREERTOS_ERRNO_ENOENT                   	 pdFREERTOS_ERRNO_ENOENT
#define RTOS_FREERTOS_ERRNO_ENOMEDIUM                	 pdFREERTOS_ERRNO_ENOMEDIUM
#define RTOS_FREERTOS_ERRNO_ENOMEM                   	 pdFREERTOS_ERRNO_ENOMEM
#define RTOS_FREERTOS_ERRNO_ENOPROTOOPT              	 pdFREERTOS_ERRNO_ENOPROTOOPT
#define RTOS_FREERTOS_ERRNO_ENOSPC                   	 pdFREERTOS_ERRNO_ENOSPC
#define RTOS_FREERTOS_ERRNO_ENOTCONN                 	 pdFREERTOS_ERRNO_ENOTCONN
#define RTOS_FREERTOS_ERRNO_ENOTDIR                  	 pdFREERTOS_ERRNO_ENOTDIR
#define RTOS_FREERTOS_ERRNO_ENOTEMPTY                	 pdFREERTOS_ERRNO_ENOTEMPTY
#define RTOS_FREERTOS_ERRNO_ENXIO                    	 pdFREERTOS_ERRNO_ENXIO
#define RTOS_FREERTOS_ERRNO_EOPNOTSUPP               	 pdFREERTOS_ERRNO_EOPNOTSUPP
#define RTOS_FREERTOS_ERRNO_EROFS                    	 pdFREERTOS_ERRNO_EROFS
#define RTOS_FREERTOS_ERRNO_ESPIPE                   	 pdFREERTOS_ERRNO_ESPIPE
#define RTOS_FREERTOS_ERRNO_ETIMEDOUT                	 pdFREERTOS_ERRNO_ETIMEDOUT
#define RTOS_FREERTOS_ERRNO_EUNATCH                  	 pdFREERTOS_ERRNO_EUNATCH
#define RTOS_FREERTOS_ERRNO_EWOULDBLOCK              	 pdFREERTOS_ERRNO_EWOULDBLOCK
#define RTOS_FREERTOS_ERRNO_EXDEV                    	 pdFREERTOS_ERRNO_EXDEV
#define RTOS_FREERTOS_ERRNO_NONE                     	 pdFREERTOS_ERRNO_NONE
#define RTOS_FREERTOS_LITTLE_ENDIAN                  	 pdFREERTOS_LITTLE_ENDIAN
/* from message_buffer.h */
#define RTOS_FREERTOS_MESSAGE_BUFFER_H               	 FREERTOS_MESSAGE_BUFFER_H
/* from mpu_wrappers.h */
#define RTOS_FREERTOS_SYSTEM_CALL                    	 FREERTOS_SYSTEM_CALL
/* from FreeRTOS.h */
#define RTOS_INCLUDE_QUEUE_GET_MUTEX_HOLDER          	 INCLUDE_xQueueGetMutexHolder
#define RTOS_INCLUDE_SEMAPHORE_GET_MUTEX_HOLDER      	 INCLUDE_xSemaphoreGetMutexHolder
#define RTOS_INCLUDE_TASK_ABORT_DELAY                	 INCLUDE_xTaskAbortDelay
#define RTOS_INCLUDE_TASK_DELAY                      	 INCLUDE_vTaskDelay
/* from FreeRTOSConfig.h */
#define RTOS_INCLUDE_TASK_DELAY_UNTIL                	 INCLUDE_vTaskDelayUntil
/* from FreeRTOS.h */
#define RTOS_INCLUDE_TASK_DELETE                     	 INCLUDE_vTaskDelete
#define RTOS_INCLUDE_TASK_GET_CURRENT_TASK_HANDLE    	 INCLUDE_xTaskGetCurrentTaskHandle
#define RTOS_INCLUDE_TASK_GET_HANDLE                 	 INCLUDE_xTaskGetHandle
#define RTOS_INCLUDE_TASK_GET_IDLE_TASK_HANDLE       	 INCLUDE_xTaskGetIdleTaskHandle
#define RTOS_INCLUDE_TASK_GET_SCHEDULER_STATE        	 INCLUDE_xTaskGetSchedulerState
#define RTOS_INCLUDE_TASK_GET_STACK_HIGH_WATER_MARK  	 INCLUDE_uxTaskGetStackHighWaterMark
#define RTOS_INCLUDE_TASK_GET_STACK_HIGH_WATER_MARK2 	 INCLUDE_uxTaskGetStackHighWaterMark2
#define RTOS_INCLUDE_TASK_GET_STATE                  	 INCLUDE_eTaskGetState
#define RTOS_INCLUDE_TASK_PRIORITY_GET               	 INCLUDE_uxTaskPriorityGet
#define RTOS_INCLUDE_TASK_PRIORITY_SET               	 INCLUDE_vTaskPrioritySet
#define RTOS_INCLUDE_TASK_RESUME_FROM_ISR            	 INCLUDE_xTaskResumeFromISR
#define RTOS_INCLUDE_TASK_SUSPEND                    	 INCLUDE_vTaskSuspend
#define RTOS_INCLUDE_TIMER_PEND_FUNCTION_CALL        	 INCLUDE_xTimerPendFunctionCall
#define RTOS_INC_FREERTOS_H                          	 INC_FREERTOS_H
/* from task.h */
#define RTOS_INC_TASK_H                              	 INC_TASK_H
/* from projdefs.h */
#define RTOS_INTEGRITY_CHECK_VALUE                   	 pdINTEGRITY_CHECK_VALUE
/* from queue.h */
#define RTOS_ITEM_SIZE                               	 ITEM_SIZE
/* from FreeRTOS.h */
#define RTOS_LIST                                    	 xList
/* from list.h */
#define RTOS_LIST_FIRST_LIST_INTEGRITY_CHECK_VALUE   	 listFIRST_LIST_INTEGRITY_CHECK_VALUE
#define RTOS_LIST_FIRST_LIST_ITEM_INTEGRITY_CHECK_VALUE 	 listFIRST_LIST_ITEM_INTEGRITY_CHECK_VALUE
#define RTOS_LIST_H                                  	 LIST_H
/* from FreeRTOS.h */
#define RTOS_LIST_ITEM                               	 xListItem
/* from list.h */
#define RTOS_LIST_SECOND_LIST_INTEGRITY_CHECK_VALUE  	 listSECOND_LIST_INTEGRITY_CHECK_VALUE
#define RTOS_LIST_SECOND_LIST_ITEM_INTEGRITY_CHECK_VALUE 	 listSECOND_LIST_ITEM_INTEGRITY_CHECK_VALUE
/* from projdefs.h */
#define RTOS_LITTLE_ENDIAN                           	 pdLITTLE_ENDIAN
/* from FreeRTOS.h */
#define RTOS_MEMORY_REGION                           	 xMemoryRegion
/* from mpu_wrappers.h */
#define RTOS_MPU_WRAPPERS_H                          	 MPU_WRAPPERS_H
/* from projdefs.h */
#define RTOS_PASS                                    	 pdPASS
/* from portable.h */
#define RTOS_PORTABLE_H                              	 PORTABLE_H
/* from portmacro.h */
#define RTOS_PORTMACRO_H                             	 PORTMACRO_H
/* from portable.h */
#define RTOS_PORT_ARCH_NAME                          	 portARCH_NAME
/* from portmacro.h */
#define RTOS_PORT_BASE_TYPE                          	 portBASE_TYPE
#define RTOS_PORT_BYTE_ALIGNMENT                     	 portBYTE_ALIGNMENT
/* from portable.h */
#define RTOS_PORT_BYTE_ALIGNMENT_MASK                	 portBYTE_ALIGNMENT_MASK
/* from portmacro.h */
#define RTOS_PORT_CHAR                               	 portCHAR
#define RTOS_PORT_CRITICAL_NESTING_IN_TCB            	 portCRITICAL_NESTING_IN_TCB
#define RTOS_PORT_DONT_DISCARD                       	 portDONT_DISCARD
#define RTOS_PORT_DOUBLE                             	 portDOUBLE
#define RTOS_PORT_ENTER_CRITICAL                     	 portENTER_CRITICAL
#define RTOS_PORT_EXIT_CRITICAL                      	 portEXIT_CRITICAL
#define RTOS_PORT_FLOAT                              	 portFLOAT
/* from atomic.h */
#define RTOS_PORT_FORCE_INLINE                       	 portFORCE_INLINE
/* from portable.h */
#define RTOS_PORT_FREE_STACK                         	 vPortFreeStack
#define RTOS_PORT_HAS_STACK_OVERFLOW_CHECKING        	 portHAS_STACK_OVERFLOW_CHECKING
/* from portmacro.h */
#define RTOS_PORT_LONG                               	 portLONG
#define RTOS_PORT_MAX_DELAY                          	 portMAX_DELAY
/* from portable.h */
#define RTOS_PORT_NUM_CONFIGURABLE_REGIONS           	 portNUM_CONFIGURABLE_REGIONS
/* from portmacro.h */
#define RTOS_PORT_NVIC_INT_CTRL_REG                  	 portNVIC_INT_CTRL_REG
#define RTOS_PORT_NVIC_PENDSVSET_BIT                 	 portNVIC_PENDSVSET_BIT
/* from FreeRTOS.h */
#define RTOS_PORT_POINTER_SIZE_TYPE                  	 portPOINTER_SIZE_TYPE
#define RTOS_PORT_PRIVILEGE_BIT                      	 portPRIVILEGE_BIT
/* from portmacro.h */
#define RTOS_PORT_SHORT                              	 portSHORT
#define RTOS_PORT_STACK_GROWTH                       	 portSTACK_GROWTH
/* from stack_macros.h */
#define RTOS_PORT_STACK_LIMIT_PADDING                	 portSTACK_LIMIT_PADDING
/* from portmacro.h */
#define RTOS_PORT_STACK_TYPE                         	 portSTACK_TYPE
#define RTOS_PORT_SUPPRESS_TICKS_AND_SLEEP           	 portSUPPRESS_TICKS_AND_SLEEP
#define RTOS_PORT_TICK_PERIOD_MS                     	 portTICK_PERIOD_MS
/* from FreeRTOS.h */
#define RTOS_PORT_TICK_RATE_MS                       	 portTICK_RATE_MS
#define RTOS_PORT_TICK_TYPE                          	 portTickType
/* from portmacro.h */
#define RTOS_PORT_TICK_TYPE_IS_ATOMIC                	 portTICK_TYPE_IS_ATOMIC
/* from FreeRTOS.h */
#define RTOS_PORT_TIMER_CALLBACK_ATTRIBUTE           	 portTIMER_CALLBACK_ATTRIBUTE
/* from mpu_wrappers.h */
#define RTOS_PORT_USING_MPU_WRAPPERS                 	 portUSING_MPU_WRAPPERS
/* from portmacro.h */
#define RTOS_PORT_YIELD                              	 portYIELD
/* from mpu_wrappers.h */
#define RTOS_PRIVILEGED_DATA                         	 PRIVILEGED_DATA
#define RTOS_PRIVILEGED_FUNCTION                     	 PRIVILEGED_FUNCTION
/* from projdefs.h */
#define RTOS_PROJDEFS_H                              	 PROJDEFS_H
/* from portable.h */
#define RTOS_PV_PORT_MALLOC_STACK                    	 pvPortMallocStack
/* from mpu_wrappers.h */
#define RTOS_PV_TASK_GET_THREAD_LOCAL_STORAGE_POINTER 	 pvTaskGetThreadLocalStoragePointer
#define RTOS_PV_TIMER_GET_TIMER_ID                   	 pvTimerGetTimerID
/* from FreeRTOS.h */
#define RTOS_PX_CONTAINER                            	 pxContainer
/* from mpu_wrappers.h */
#define RTOS_QUEUE_ADD_TO_REGISTRY                   	 vQueueAddToRegistry
#define RTOS_QUEUE_ADD_TO_SET                        	 xQueueAddToSet
#define RTOS_QUEUE_CREATE_COUNTING_SEMAPHORE         	 xQueueCreateCountingSemaphore
#define RTOS_QUEUE_CREATE_COUNTING_SEMAPHORE_STATIC  	 xQueueCreateCountingSemaphoreStatic
#define RTOS_QUEUE_CREATE_MUTEX                      	 xQueueCreateMutex
#define RTOS_QUEUE_CREATE_MUTEX_STATIC               	 xQueueCreateMutexStatic
#define RTOS_QUEUE_CREATE_SET                        	 xQueueCreateSet
#define RTOS_QUEUE_DELETE                            	 vQueueDelete
#define RTOS_QUEUE_GENERIC_CREATE                    	 xQueueGenericCreate
#define RTOS_QUEUE_GENERIC_CREATE_STATIC             	 xQueueGenericCreateStatic
#define RTOS_QUEUE_GENERIC_RESET                     	 xQueueGenericReset
#define RTOS_QUEUE_GENERIC_SEND                      	 xQueueGenericSend
#define RTOS_QUEUE_GET_MUTEX_HOLDER                  	 xQueueGetMutexHolder
#define RTOS_QUEUE_GET_NAME                          	 pcQueueGetName
/* from FreeRTOS.h */
#define RTOS_QUEUE_GET_QUEUE_NAME                    	 pcQueueGetQueueName
/* from mpu_wrappers.h */
#define RTOS_QUEUE_GIVE_MUTEX_RECURSIVE              	 xQueueGiveMutexRecursive
/* from queue.h */
#define RTOS_QUEUE_H                                 	 QUEUE_H
/* from FreeRTOS.h */
#define RTOS_QUEUE_HANDLE                            	 xQueueHandle
/* from queue.h */
#define RTOS_QUEUE_LENGTH                            	 QUEUE_LENGTH
/* from mpu_wrappers.h */
#define RTOS_QUEUE_MESSAGES_WAITING                  	 uxQueueMessagesWaiting
/* from queue.h */
#define RTOS_QUEUE_OVERWRITE                         	 queueOVERWRITE
/* from mpu_wrappers.h */
#define RTOS_QUEUE_PEEK                              	 xQueuePeek
/* from queue.h */
#define RTOS_QUEUE_QUEUE_TYPE_BASE                   	 queueQUEUE_TYPE_BASE
#define RTOS_QUEUE_QUEUE_TYPE_BINARY_SEMAPHORE       	 queueQUEUE_TYPE_BINARY_SEMAPHORE
#define RTOS_QUEUE_QUEUE_TYPE_COUNTING_SEMAPHORE     	 queueQUEUE_TYPE_COUNTING_SEMAPHORE
#define RTOS_QUEUE_QUEUE_TYPE_MUTEX                  	 queueQUEUE_TYPE_MUTEX
#define RTOS_QUEUE_QUEUE_TYPE_RECURSIVE_MUTEX        	 queueQUEUE_TYPE_RECURSIVE_MUTEX
#define RTOS_QUEUE_QUEUE_TYPE_SET                    	 queueQUEUE_TYPE_SET
/* from mpu_wrappers.h */
#define RTOS_QUEUE_RECEIVE                           	 xQueueReceive
#define RTOS_QUEUE_REMOVE_FROM_SET                   	 xQueueRemoveFromSet
#define RTOS_QUEUE_SELECT_FROM_SET                   	 xQueueSelectFromSet
#define RTOS_QUEUE_SEMAPHORE_TAKE                    	 xQueueSemaphoreTake
/* from queue.h */
#define RTOS_QUEUE_SEND_TO_BACK                      	 queueSEND_TO_BACK
#define RTOS_QUEUE_SEND_TO_FRONT                     	 queueSEND_TO_FRONT
/* from FreeRTOS.h */
#define RTOS_QUEUE_SET_HANDLE                        	 xQueueSetHandle
#define RTOS_QUEUE_SET_MEMBER_HANDLE                 	 xQueueSetMemberHandle
/* from mpu_wrappers.h */
#define RTOS_QUEUE_SPACES_AVAILABLE                  	 uxQueueSpacesAvailable
#define RTOS_QUEUE_TAKE_MUTEX_RECURSIVE              	 xQueueTakeMutexRecursive
#define RTOS_QUEUE_UNREGISTER_QUEUE                  	 vQueueUnregisterQueue
/* from semphr.h */
#define RTOS_SEMAPHORE_CREATE_BINARY                 	 vSemaphoreCreateBinary
#define RTOS_SEMAPHORE_H                             	 SEMAPHORE_H
/* from FreeRTOS.h */
#define RTOS_SEMAPHORE_HANDLE                        	 xSemaphoreHandle
/* from semphr.h */
#define RTOS_SEM_BINARY_SEMAPHORE_QUEUE_LENGTH       	 semBINARY_SEMAPHORE_QUEUE_LENGTH
#define RTOS_SEM_GIVE_BLOCK_TIME                     	 semGIVE_BLOCK_TIME
#define RTOS_SEM_SEMAPHORE_QUEUE_ITEM_LENGTH         	 semSEMAPHORE_QUEUE_ITEM_LENGTH
/* from stack_macros.h */
#define RTOS_STACK_MACROS_H                          	 STACK_MACROS_H
/* from task.h */
#define RTOS_STACK_SIZE                              	 STACK_SIZE
/* from stream_buffer.h */
#define RTOS_STORAGE_SIZE_BYTES                      	 STORAGE_SIZE_BYTES
/* from mpu_wrappers.h */
#define RTOS_STREAM_BUFFER_BYTES_AVAILABLE           	 xStreamBufferBytesAvailable
#define RTOS_STREAM_BUFFER_DELETE                    	 vStreamBufferDelete
#define RTOS_STREAM_BUFFER_GENERIC_CREATE            	 xStreamBufferGenericCreate
#define RTOS_STREAM_BUFFER_GENERIC_CREATE_STATIC     	 xStreamBufferGenericCreateStatic
/* from stream_buffer.h */
#define RTOS_STREAM_BUFFER_H                         	 STREAM_BUFFER_H
/* from mpu_wrappers.h */
#define RTOS_STREAM_BUFFER_IS_EMPTY                  	 xStreamBufferIsEmpty
#define RTOS_STREAM_BUFFER_IS_FULL                   	 xStreamBufferIsFull
#define RTOS_STREAM_BUFFER_NEXT_MESSAGE_LENGTH_BYTES 	 xStreamBufferNextMessageLengthBytes
#define RTOS_STREAM_BUFFER_RECEIVE                   	 xStreamBufferReceive
#define RTOS_STREAM_BUFFER_RESET                     	 xStreamBufferReset
#define RTOS_STREAM_BUFFER_SEND                      	 xStreamBufferSend
#define RTOS_STREAM_BUFFER_SET_TRIGGER_LEVEL         	 xStreamBufferSetTriggerLevel
#define RTOS_STREAM_BUFFER_SPACES_AVAILABLE          	 xStreamBufferSpacesAvailable
/* from event_groups.h */
#define RTOS_TASK_0_BIT                              	 TASK_0_BIT
#define RTOS_TASK_1_BIT                              	 TASK_1_BIT
#define RTOS_TASK_2_BIT                              	 TASK_2_BIT
/* from mpu_wrappers.h */
#define RTOS_TASK_ABORT_DELAY                        	 xTaskAbortDelay
#define RTOS_TASK_CALL_APPLICATION_TASK_HOOK         	 xTaskCallApplicationTaskHook
#define RTOS_TASK_CATCH_UP_TICKS                     	 xTaskCatchUpTicks
#define RTOS_TASK_CHECK_FOR_TIME_OUT                 	 xTaskCheckForTimeOut
/* from FreeRTOS.h */
#define RTOS_TASK_CODE                               	 pdTASK_CODE
/* from mpu_wrappers.h */
#define RTOS_TASK_CREATE                             	 xTaskCreate
#define RTOS_TASK_CREATE_STATIC                      	 xTaskCreateStatic
#define RTOS_TASK_DELAY                              	 vTaskDelay
/* from task.h */
#define RTOS_TASK_DELAY_UNTIL                        	 vTaskDelayUntil
/* from mpu_wrappers.h */
#define RTOS_TASK_DELETE                             	 vTaskDelete
#define RTOS_TASK_GENERIC_NOTIFY                     	 xTaskGenericNotify
#define RTOS_TASK_GENERIC_NOTIFY_STATE_CLEAR         	 xTaskGenericNotifyStateClear
#define RTOS_TASK_GENERIC_NOTIFY_TAKE                	 ulTaskGenericNotifyTake
#define RTOS_TASK_GENERIC_NOTIFY_VALUE_CLEAR         	 ulTaskGenericNotifyValueClear
#define RTOS_TASK_GENERIC_NOTIFY_WAIT                	 xTaskGenericNotifyWait
#define RTOS_TASK_GET_APPLICATION_TASK_TAG           	 xTaskGetApplicationTaskTag
#define RTOS_TASK_GET_CURRENT_TASK_HANDLE            	 xTaskGetCurrentTaskHandle
#define RTOS_TASK_GET_HANDLE                         	 xTaskGetHandle
/* from FreeRTOS.h */
#define RTOS_TASK_GET_IDLE_RUN_TIME_COUNTER          	 xTaskGetIdleRunTimeCounter
/* from mpu_wrappers.h */
#define RTOS_TASK_GET_IDLE_TASK_HANDLE               	 xTaskGetIdleTaskHandle
#define RTOS_TASK_GET_INFO                           	 vTaskGetInfo
#define RTOS_TASK_GET_NAME                           	 pcTaskGetName
#define RTOS_TASK_GET_NUMBER_OF_TASKS                	 uxTaskGetNumberOfTasks
#define RTOS_TASK_GET_RUN_TIME_STATS                 	 vTaskGetRunTimeStats
#define RTOS_TASK_GET_SCHEDULER_STATE                	 xTaskGetSchedulerState
#define RTOS_TASK_GET_STACK_HIGH_WATER_MARK          	 uxTaskGetStackHighWaterMark
#define RTOS_TASK_GET_STACK_HIGH_WATER_MARK2         	 uxTaskGetStackHighWaterMark2
#define RTOS_TASK_GET_STATE                          	 eTaskGetState
#define RTOS_TASK_GET_SYSTEM_STATE                   	 uxTaskGetSystemState
/* from FreeRTOS.h */
#define RTOS_TASK_GET_TASK_INFO                      	 vTaskGetTaskInfo
#define RTOS_TASK_GET_TASK_NAME                      	 pcTaskGetTaskName
/* from mpu_wrappers.h */
#define RTOS_TASK_GET_TICK_COUNT                     	 xTaskGetTickCount
/* from FreeRTOS.h */
#define RTOS_TASK_HANDLE                             	 xTaskHandle
#define RTOS_TASK_HOOK_CODE                          	 pdTASK_HOOK_CODE
/* from mpu_wrappers.h */
#define RTOS_TASK_LIST                               	 vTaskList
/* from FreeRTOS.h */
#define RTOS_TASK_PARAMETERS                         	 xTaskParameters
/* from mpu_wrappers.h */
#define RTOS_TASK_PRIORITY_GET                       	 uxTaskPriorityGet
#define RTOS_TASK_PRIORITY_SET                       	 vTaskPrioritySet
#define RTOS_TASK_RESUME                             	 vTaskResume
#define RTOS_TASK_RESUME_ALL                         	 xTaskResumeAll
/* from task.h */
#define RTOS_TASK_SCHEDULER_NOT_STARTED              	 taskSCHEDULER_NOT_STARTED
#define RTOS_TASK_SCHEDULER_RUNNING                  	 taskSCHEDULER_RUNNING
#define RTOS_TASK_SCHEDULER_SUSPENDED                	 taskSCHEDULER_SUSPENDED
/* from mpu_wrappers.h */
#define RTOS_TASK_SET_APPLICATION_TASK_TAG           	 vTaskSetApplicationTaskTag
#define RTOS_TASK_SET_THREAD_LOCAL_STORAGE_POINTER   	 vTaskSetThreadLocalStoragePointer
#define RTOS_TASK_SET_TIME_OUT_STATE                 	 vTaskSetTimeOutState
/* from FreeRTOS.h */
#define RTOS_TASK_STATE_GET                          	 eTaskStateGet
#define RTOS_TASK_STATUS_TYPE                        	 xTaskStatusType
/* from mpu_wrappers.h */
#define RTOS_TASK_SUSPEND                            	 vTaskSuspend
#define RTOS_TASK_SUSPEND_ALL                        	 vTaskSuspendAll
/* from timers.h */
#define RTOS_TIMERS_H                                	 TIMERS_H
/* from mpu_wrappers.h */
#define RTOS_TIMER_CREATE                            	 xTimerCreate
#define RTOS_TIMER_CREATE_STATIC                     	 xTimerCreateStatic
#define RTOS_TIMER_GENERIC_COMMAND                   	 xTimerGenericCommand
#define RTOS_TIMER_GET_EXPIRY_TIME                   	 xTimerGetExpiryTime
#define RTOS_TIMER_GET_NAME                          	 pcTimerGetName
#define RTOS_TIMER_GET_PERIOD                        	 xTimerGetPeriod
#define RTOS_TIMER_GET_RELOAD_MODE                   	 uxTimerGetReloadMode
#define RTOS_TIMER_GET_TIMER_DAEMON_TASK_HANDLE      	 xTimerGetTimerDaemonTaskHandle
/* from FreeRTOS.h */
#define RTOS_TIMER_GET_TIMER_NAME                    	 pcTimerGetTimerName
#define RTOS_TIMER_HANDLE                            	 xTimerHandle
/* from mpu_wrappers.h */
#define RTOS_TIMER_IS_TIMER_ACTIVE                   	 xTimerIsTimerActive
#define RTOS_TIMER_PEND_FUNCTION_CALL                	 xTimerPendFunctionCall
#define RTOS_TIMER_SET_RELOAD_MODE                   	 vTimerSetReloadMode
#define RTOS_TIMER_SET_TIMER_ID                      	 vTimerSetTimerID
/* from FreeRTOS.h */
#define RTOS_TIME_OUT_TYPE                           	 xTimeOutType
/* from timers.h */
#define RTOS_TMR_COMMAND_CHANGE_PERIOD               	 tmrCOMMAND_CHANGE_PERIOD
#define RTOS_TMR_COMMAND_CHANGE_PERIOD_FROM_ISR      	 tmrCOMMAND_CHANGE_PERIOD_FROM_ISR
#define RTOS_TMR_COMMAND_DELETE                      	 tmrCOMMAND_DELETE
#define RTOS_TMR_COMMAND_EXECUTE_CALLBACK            	 tmrCOMMAND_EXECUTE_CALLBACK
#define RTOS_TMR_COMMAND_EXECUTE_CALLBACK_FROM_ISR   	 tmrCOMMAND_EXECUTE_CALLBACK_FROM_ISR
#define RTOS_TMR_COMMAND_RESET                       	 tmrCOMMAND_RESET
#define RTOS_TMR_COMMAND_RESET_FROM_ISR              	 tmrCOMMAND_RESET_FROM_ISR
#define RTOS_TMR_COMMAND_START                       	 tmrCOMMAND_START
#define RTOS_TMR_COMMAND_START_DONT_TRACE            	 tmrCOMMAND_START_DONT_TRACE
#define RTOS_TMR_COMMAND_START_FROM_ISR              	 tmrCOMMAND_START_FROM_ISR
#define RTOS_TMR_COMMAND_STOP                        	 tmrCOMMAND_STOP
#define RTOS_TMR_COMMAND_STOP_FROM_ISR               	 tmrCOMMAND_STOP_FROM_ISR
#define RTOS_TMR_FIRST_FROM_ISR_COMMAND              	 tmrFIRST_FROM_ISR_COMMAND
/* from FreeRTOS.h */
#define RTOS_TMR_TIMER_CALLBACK                      	 tmrTIMER_CALLBACK
#define RTOS_TRACE_QUEUE_SET_SEND                    	 traceQUEUE_SET_SEND
/* from projdefs.h */
#define RTOS_TRUE                                    	 pdTRUE
/* from task.h */
#define RTOS_TSK_DEFAULT_INDEX_TO_NOTIFY             	 tskDEFAULT_INDEX_TO_NOTIFY
#define RTOS_TSK_IDLE_PRIORITY                       	 tskIDLE_PRIORITY
#define RTOS_TSK_KERNEL_VERSION_BUILD                	 tskKERNEL_VERSION_BUILD
#define RTOS_TSK_KERNEL_VERSION_MAJOR                	 tskKERNEL_VERSION_MAJOR
#define RTOS_TSK_KERNEL_VERSION_MINOR                	 tskKERNEL_VERSION_MINOR
#define RTOS_TSK_KERNEL_VERSION_NUMBER               	 tskKERNEL_VERSION_NUMBER
#define RTOS_TSK_MPU_REGION_DEVICE_MEMORY            	 tskMPU_REGION_DEVICE_MEMORY
#define RTOS_TSK_MPU_REGION_EXECUTE_NEVER            	 tskMPU_REGION_EXECUTE_NEVER
#define RTOS_TSK_MPU_REGION_NORMAL_MEMORY            	 tskMPU_REGION_NORMAL_MEMORY
#define RTOS_TSK_MPU_REGION_READ_ONLY                	 tskMPU_REGION_READ_ONLY
#define RTOS_TSK_MPU_REGION_READ_WRITE               	 tskMPU_REGION_READ_WRITE
#define RTOS_TSK_NO_AFFINITY                         	 tskNO_AFFINITY
/* from FreeRTOS.h */
#define RTOS_TSK_STATIC_AND_DYNAMIC_ALLOCATION_POSSIBLE 	 tskSTATIC_AND_DYNAMIC_ALLOCATION_POSSIBLE
#define _RTOS_INCLUDE_TASK_DELAY_UNTIL               	 INCLUDE_xTaskDelayUntil
/* from mpu_wrappers.h */
#define _RTOS_TASK_GET_IDLE_RUN_TIME_COUNTER         	 ulTaskGetIdleRunTimeCounter
/* from atomic.h */
#define rtos_atomic_enter_critical                   	 ATOMIC_ENTER_CRITICAL
#define rtos_atomic_exit_critical                    	 ATOMIC_EXIT_CRITICAL
/* from FreeRTOS.h */
#define rtos_config_assert                           	 configASSERT
#define rtos_config_max                              	 configMAX
#define rtos_config_min                              	 configMIN
#define rtos_config_post_sleep_processing            	 configPOST_SLEEP_PROCESSING
#define rtos_config_pre_sleep_processing             	 configPRE_SLEEP_PROCESSING
#define rtos_config_pre_suppress_ticks_and_sleep_processing 	 configPRE_SUPPRESS_TICKS_AND_SLEEP_PROCESSING
#define rtos_config_precondition                     	 configPRECONDITION
#define rtos_config_printf                           	 configPRINTF
/* from croutine.h */
#define rtos_cr_delay                                	 crDELAY
#define rtos_cr_end                                  	 crEND
#define rtos_cr_queue_receive                        	 crQUEUE_RECEIVE
#define rtos_cr_queue_receive_from_isr               	 crQUEUE_RECEIVE_FROM_ISR
#define rtos_cr_queue_send                           	 crQUEUE_SEND
#define rtos_cr_queue_send_from_isr                  	 crQUEUE_SEND_FROM_ISR
#define rtos_cr_set_state0                           	 crSET_STATE0
#define rtos_cr_set_state1                           	 crSET_STATE1
#define rtos_cr_start                                	 crSTART
/* from event_groups.h */
#define rtos_event_group_get_bits                    	 xEventGroupGetBits
/* from list.h */
#define rtos_list_current_list_length                	 listCURRENT_LIST_LENGTH
#define rtos_list_get_end_marker                     	 listGET_END_MARKER
#define rtos_list_get_head_entry                     	 listGET_HEAD_ENTRY
#define rtos_list_get_item_value_of_head_entry       	 listGET_ITEM_VALUE_OF_HEAD_ENTRY
#define rtos_list_get_list_item_owner                	 listGET_LIST_ITEM_OWNER
#define rtos_list_get_list_item_value                	 listGET_LIST_ITEM_VALUE
#define rtos_list_get_next                           	 listGET_NEXT
#define rtos_list_get_owner_of_head_entry            	 listGET_OWNER_OF_HEAD_ENTRY
#define rtos_list_get_owner_of_next_entry            	 listGET_OWNER_OF_NEXT_ENTRY
#define rtos_list_is_contained_within                	 listIS_CONTAINED_WITHIN
#define rtos_list_list_is_empty                      	 listLIST_IS_EMPTY
#define rtos_list_list_is_initialised                	 listLIST_IS_INITIALISED
#define rtos_list_list_item_container                	 listLIST_ITEM_CONTAINER
#define rtos_list_set_first_list_item_integrity_check_value 	 listSET_FIRST_LIST_ITEM_INTEGRITY_CHECK_VALUE
#define rtos_list_set_list_integrity_check_1_value   	 listSET_LIST_INTEGRITY_CHECK_1_VALUE
#define rtos_list_set_list_integrity_check_2_value   	 listSET_LIST_INTEGRITY_CHECK_2_VALUE
#define rtos_list_set_list_item_owner                	 listSET_LIST_ITEM_OWNER
#define rtos_list_set_list_item_value                	 listSET_LIST_ITEM_VALUE
#define rtos_list_set_second_list_item_integrity_check_value 	 listSET_SECOND_LIST_ITEM_INTEGRITY_CHECK_VALUE
#define rtos_list_test_list_integrity                	 listTEST_LIST_INTEGRITY
#define rtos_list_test_list_item_integrity           	 listTEST_LIST_ITEM_INTEGRITY
/* from message_buffer.h */
#define rtos_message_buffer_create                   	 xMessageBufferCreate
#define rtos_message_buffer_create_static            	 xMessageBufferCreateStatic
#define rtos_message_buffer_delete                   	 vMessageBufferDelete
#define rtos_message_buffer_is_empty                 	 xMessageBufferIsEmpty
#define rtos_message_buffer_is_full                  	 xMessageBufferIsFull
#define rtos_message_buffer_next_length_bytes        	 xMessageBufferNextLengthBytes
#define rtos_message_buffer_receive                  	 xMessageBufferReceive
#define rtos_message_buffer_receive_completed_from_isr 	 xMessageBufferReceiveCompletedFromISR
#define rtos_message_buffer_receive_from_isr         	 xMessageBufferReceiveFromISR
#define rtos_message_buffer_reset                    	 xMessageBufferReset
#define rtos_message_buffer_send                     	 xMessageBufferSend
#define rtos_message_buffer_send_completed_from_isr  	 xMessageBufferSendCompletedFromISR
#define rtos_message_buffer_send_from_isr            	 xMessageBufferSendFromISR
#define rtos_message_buffer_space_available          	 xMessageBufferSpaceAvailable
#define rtos_message_buffer_spaces_available         	 xMessageBufferSpacesAvailable
/* from projdefs.h */
#define rtos_ms_to_ticks                             	 pdMS_TO_TICKS
/* from FreeRTOS.h */
#define rtos_mt_coverage_test_delay                  	 mtCOVERAGE_TEST_DELAY
#define rtos_mt_coverage_test_marker                 	 mtCOVERAGE_TEST_MARKER
#define rtos_port_allocate_secure_context            	 portALLOCATE_SECURE_CONTEXT
#define rtos_port_assert_if_in_isr                   	 portASSERT_IF_IN_ISR
#define rtos_port_assert_if_interrupt_priority_invalid 	 portASSERT_IF_INTERRUPT_PRIORITY_INVALID
/* from portmacro.h */
#define rtos_port_check_if_in_isr                    	 portCHECK_IF_IN_ISR
/* from FreeRTOS.h */
#define rtos_port_clean_up_tcb                       	 portCLEAN_UP_TCB
/* from portmacro.h */
#define rtos_port_clear_interrupt_mask_from_isr      	 portCLEAR_INTERRUPT_MASK_FROM_ISR
/* from FreeRTOS.h */
#define rtos_port_configure_timer_for_run_time_stats 	 portCONFIGURE_TIMER_FOR_RUN_TIME_STATS
/* from portmacro.h */
#define rtos_port_disable_interrupts                 	 portDISABLE_INTERRUPTS
#define rtos_port_enable_interrupts                  	 portENABLE_INTERRUPTS
#define rtos_port_end_switching_isr                  	 portEND_SWITCHING_ISR
#define rtos_port_get_core_id                        	 portGET_CORE_ID
#define rtos_port_get_isr_lock                       	 portGET_ISR_LOCK
#define rtos_port_get_task_lock                      	 portGET_TASK_LOCK
#define rtos_port_memory_barrier                     	 portMEMORY_BARRIER
#define rtos_port_nop                                	 portNOP
/* from FreeRTOS.h */
#define rtos_port_pre_task_delete_hook               	 portPRE_TASK_DELETE_HOOK
/* from portmacro.h */
#define rtos_port_release_isr_lock                   	 portRELEASE_ISR_LOCK
#define rtos_port_release_task_lock                  	 portRELEASE_TASK_LOCK
#define rtos_port_restore_interrupts                 	 portRESTORE_INTERRUPTS
#define rtos_port_set_interrupt_mask_from_isr        	 portSET_INTERRUPT_MASK_FROM_ISR
/* from FreeRTOS.h */
#define rtos_port_setup_tcb                          	 portSETUP_TCB
#define rtos_port_software_barrier                   	 portSOFTWARE_BARRIER
/* from portmacro.h */
#define rtos_port_task_function                      	 portTASK_FUNCTION
#define rtos_port_task_function_proto                	 portTASK_FUNCTION_PROTO
/* from FreeRTOS.h */
#define rtos_port_task_uses_floating_point           	 portTASK_USES_FLOATING_POINT
#define rtos_port_tick_type_clear_interrupt_mask_from_isr 	 portTICK_TYPE_CLEAR_INTERRUPT_MASK_FROM_ISR
#define rtos_port_tick_type_enter_critical           	 portTICK_TYPE_ENTER_CRITICAL
#define rtos_port_tick_type_exit_critical            	 portTICK_TYPE_EXIT_CRITICAL
#define rtos_port_tick_type_set_interrupt_mask_from_isr 	 portTICK_TYPE_SET_INTERRUPT_MASK_FROM_ISR
/* from portmacro.h */
#define rtos_port_yield_core                         	 portYIELD_CORE
#define rtos_port_yield_from_isr                     	 portYIELD_FROM_ISR
/* from FreeRTOS.h */
#define rtos_queue_add_to_registry                   	 vQueueAddToRegistry
/* from queue.h */
#define rtos_queue_create                            	 xQueueCreate
#define rtos_queue_create_static                     	 xQueueCreateStatic
/* from FreeRTOS.h */
#define rtos_queue_get_name                          	 pcQueueGetName
/* from queue.h */
#define rtos_queue_overwrite                         	 xQueueOverwrite
#define rtos_queue_overwrite_from_isr                	 xQueueOverwriteFromISR
#define rtos_queue_reset                             	 xQueueReset
#define rtos_queue_send                              	 xQueueSend
#define rtos_queue_send_from_isr                     	 xQueueSendFromISR
#define rtos_queue_send_to_back                      	 xQueueSendToBack
#define rtos_queue_send_to_back_from_isr             	 xQueueSendToBackFromISR
#define rtos_queue_send_to_front                     	 xQueueSendToFront
#define rtos_queue_send_to_front_from_isr            	 xQueueSendToFrontFromISR
/* from FreeRTOS.h */
#define rtos_queue_unregister_queue                  	 vQueueUnregisterQueue
/* from semphr.h */
#define rtos_semaphore_create_binary                 	 xSemaphoreCreateBinary
#define rtos_semaphore_create_binary_static          	 xSemaphoreCreateBinaryStatic
#define rtos_semaphore_create_counting               	 xSemaphoreCreateCounting
#define rtos_semaphore_create_counting_static        	 xSemaphoreCreateCountingStatic
#define rtos_semaphore_create_mutex                  	 xSemaphoreCreateMutex
#define rtos_semaphore_create_mutex_static           	 xSemaphoreCreateMutexStatic
#define rtos_semaphore_create_recursive_mutex        	 xSemaphoreCreateRecursiveMutex
#define rtos_semaphore_create_recursive_mutex_static 	 xSemaphoreCreateRecursiveMutexStatic
#define rtos_semaphore_delete                        	 vSemaphoreDelete
#define rtos_semaphore_get_count                     	 uxSemaphoreGetCount
#define rtos_semaphore_get_mutex_holder              	 xSemaphoreGetMutexHolder
#define rtos_semaphore_get_mutex_holder_from_isr     	 xSemaphoreGetMutexHolderFromISR
#define rtos_semaphore_give                          	 xSemaphoreGive
#define rtos_semaphore_give_from_isr                 	 xSemaphoreGiveFromISR
#define rtos_semaphore_give_recursive                	 xSemaphoreGiveRecursive
#define rtos_semaphore_take                          	 xSemaphoreTake
#define rtos_semaphore_take_from_isr                 	 xSemaphoreTakeFromISR
#define rtos_semaphore_take_recursive                	 xSemaphoreTakeRecursive
/* from stream_buffer.h */
#define rtos_stream_buffer_create                    	 xStreamBufferCreate
#define rtos_stream_buffer_create_static             	 xStreamBufferCreateStatic
/* from stack_macros.h */
#define rtos_task_check_for_stack_overflow           	 taskCHECK_FOR_STACK_OVERFLOW
/* from task.h */
#define rtos_task_check_if_in_isr                    	 taskCHECK_IF_IN_ISR
#define rtos_task_disable_interrupts                 	 taskDISABLE_INTERRUPTS
#define rtos_task_enable_interrupts                  	 taskENABLE_INTERRUPTS
#define rtos_task_enter_critical                     	 taskENTER_CRITICAL
#define rtos_task_enter_critical_from_isr            	 taskENTER_CRITICAL_FROM_ISR
#define rtos_task_exit_critical                      	 taskEXIT_CRITICAL
#define rtos_task_exit_critical_from_isr             	 taskEXIT_CRITICAL_FROM_ISR
#define rtos_task_notify                             	 xTaskNotify
#define rtos_task_notify_and_query                   	 xTaskNotifyAndQuery
#define rtos_task_notify_and_query_from_isr          	 xTaskNotifyAndQueryFromISR
#define rtos_task_notify_and_query_indexed           	 xTaskNotifyAndQueryIndexed
#define rtos_task_notify_and_query_indexed_from_isr  	 xTaskNotifyAndQueryIndexedFromISR
#define rtos_task_notify_from_isr                    	 xTaskNotifyFromISR
#define rtos_task_notify_give                        	 xTaskNotifyGive
#define rtos_task_notify_give_from_isr               	 vTaskNotifyGiveFromISR
#define rtos_task_notify_give_indexed                	 xTaskNotifyGiveIndexed
#define rtos_task_notify_give_indexed_from_isr       	 vTaskNotifyGiveIndexedFromISR
#define rtos_task_notify_indexed                     	 xTaskNotifyIndexed
#define rtos_task_notify_indexed_from_isr            	 xTaskNotifyIndexedFromISR
#define rtos_task_notify_state_clear                 	 xTaskNotifyStateClear
#define rtos_task_notify_state_clear_indexed         	 xTaskNotifyStateClearIndexed
#define rtos_task_notify_take                        	 ulTaskNotifyTake
#define rtos_task_notify_take_indexed                	 ulTaskNotifyTakeIndexed
#define rtos_task_notify_value_clear                 	 ulTaskNotifyValueClear
#define rtos_task_notify_value_clear_indexed         	 ulTaskNotifyValueClearIndexed
#define rtos_task_notify_wait                        	 xTaskNotifyWait
#define rtos_task_notify_wait_indexed                	 xTaskNotifyWaitIndexed
#define rtos_task_restore_interrupts                 	 taskRESTORE_INTERRUPTS
#define rtos_task_valid_core_id                      	 taskVALID_CORE_ID
#define rtos_task_yield                              	 taskYIELD
/* from timers.h */
#define rtos_timer_change_period                     	 xTimerChangePeriod
#define rtos_timer_change_period_from_isr            	 xTimerChangePeriodFromISR
#define rtos_timer_delete                            	 xTimerDelete
#define rtos_timer_generic_command                   	 xTimerGenericCommand
#define rtos_timer_reset                             	 xTimerReset
#define rtos_timer_reset_from_isr                    	 xTimerResetFromISR
#define rtos_timer_start                             	 xTimerStart
#define rtos_timer_start_from_isr                    	 xTimerStartFromISR
#define rtos_timer_stop                              	 xTimerStop
#define rtos_timer_stop_from_isr                     	 xTimerStopFromISR
/* from FreeRTOS.h */
#define rtos_trace_blocking_on_queue_peek            	 traceBLOCKING_ON_QUEUE_PEEK
#define rtos_trace_blocking_on_queue_receive         	 traceBLOCKING_ON_QUEUE_RECEIVE
#define rtos_trace_blocking_on_queue_send            	 traceBLOCKING_ON_QUEUE_SEND
#define rtos_trace_blocking_on_stream_buffer_receive 	 traceBLOCKING_ON_STREAM_BUFFER_RECEIVE
#define rtos_trace_blocking_on_stream_buffer_send    	 traceBLOCKING_ON_STREAM_BUFFER_SEND
#define rtos_trace_create_counting_semaphore         	 traceCREATE_COUNTING_SEMAPHORE
#define rtos_trace_create_counting_semaphore_failed  	 traceCREATE_COUNTING_SEMAPHORE_FAILED
#define rtos_trace_create_mutex                      	 traceCREATE_MUTEX
#define rtos_trace_create_mutex_failed               	 traceCREATE_MUTEX_FAILED
#define rtos_trace_end                               	 traceEND
#define rtos_trace_event_group_clear_bits            	 traceEVENT_GROUP_CLEAR_BITS
#define rtos_trace_event_group_clear_bits_from_isr   	 traceEVENT_GROUP_CLEAR_BITS_FROM_ISR
#define rtos_trace_event_group_create                	 traceEVENT_GROUP_CREATE
#define rtos_trace_event_group_create_failed         	 traceEVENT_GROUP_CREATE_FAILED
#define rtos_trace_event_group_delete                	 traceEVENT_GROUP_DELETE
#define rtos_trace_event_group_set_bits              	 traceEVENT_GROUP_SET_BITS
#define rtos_trace_event_group_set_bits_from_isr     	 traceEVENT_GROUP_SET_BITS_FROM_ISR
#define rtos_trace_event_group_sync_block            	 traceEVENT_GROUP_SYNC_BLOCK
#define rtos_trace_event_group_sync_end              	 traceEVENT_GROUP_SYNC_END
#define rtos_trace_event_group_wait_bits_block       	 traceEVENT_GROUP_WAIT_BITS_BLOCK
#define rtos_trace_event_group_wait_bits_end         	 traceEVENT_GROUP_WAIT_BITS_END
#define rtos_trace_free                              	 traceFREE
#define rtos_trace_give_mutex_recursive              	 traceGIVE_MUTEX_RECURSIVE
#define rtos_trace_give_mutex_recursive_failed       	 traceGIVE_MUTEX_RECURSIVE_FAILED
#define rtos_trace_increase_tick_count               	 traceINCREASE_TICK_COUNT
#define rtos_trace_low_power_idle_begin              	 traceLOW_POWER_IDLE_BEGIN
#define rtos_trace_low_power_idle_end                	 traceLOW_POWER_IDLE_END
#define rtos_trace_malloc                            	 traceMALLOC
#define rtos_trace_moved_task_to_ready_state         	 traceMOVED_TASK_TO_READY_STATE
#define rtos_trace_pend_func_call                    	 tracePEND_FUNC_CALL
#define rtos_trace_pend_func_call_from_isr           	 tracePEND_FUNC_CALL_FROM_ISR
#define rtos_trace_post_moved_task_to_ready_state    	 tracePOST_MOVED_TASK_TO_READY_STATE
#define rtos_trace_queue_create                      	 traceQUEUE_CREATE
#define rtos_trace_queue_create_failed               	 traceQUEUE_CREATE_FAILED
#define rtos_trace_queue_delete                      	 traceQUEUE_DELETE
#define rtos_trace_queue_peek                        	 traceQUEUE_PEEK
#define rtos_trace_queue_peek_failed                 	 traceQUEUE_PEEK_FAILED
#define rtos_trace_queue_peek_from_isr               	 traceQUEUE_PEEK_FROM_ISR
#define rtos_trace_queue_peek_from_isr_failed        	 traceQUEUE_PEEK_FROM_ISR_FAILED
#define rtos_trace_queue_receive                     	 traceQUEUE_RECEIVE
#define rtos_trace_queue_receive_failed              	 traceQUEUE_RECEIVE_FAILED
#define rtos_trace_queue_receive_from_isr            	 traceQUEUE_RECEIVE_FROM_ISR
#define rtos_trace_queue_receive_from_isr_failed     	 traceQUEUE_RECEIVE_FROM_ISR_FAILED
#define rtos_trace_queue_registry_add                	 traceQUEUE_REGISTRY_ADD
#define rtos_trace_queue_send                        	 traceQUEUE_SEND
#define rtos_trace_queue_send_failed                 	 traceQUEUE_SEND_FAILED
#define rtos_trace_queue_send_from_isr               	 traceQUEUE_SEND_FROM_ISR
#define rtos_trace_queue_send_from_isr_failed        	 traceQUEUE_SEND_FROM_ISR_FAILED
#define rtos_trace_start                             	 traceSTART
#define rtos_trace_stream_buffer_create              	 traceSTREAM_BUFFER_CREATE
#define rtos_trace_stream_buffer_create_failed       	 traceSTREAM_BUFFER_CREATE_FAILED
#define rtos_trace_stream_buffer_create_static_failed 	 traceSTREAM_BUFFER_CREATE_STATIC_FAILED
#define rtos_trace_stream_buffer_delete              	 traceSTREAM_BUFFER_DELETE
#define rtos_trace_stream_buffer_receive             	 traceSTREAM_BUFFER_RECEIVE
#define rtos_trace_stream_buffer_receive_failed      	 traceSTREAM_BUFFER_RECEIVE_FAILED
#define rtos_trace_stream_buffer_receive_from_isr    	 traceSTREAM_BUFFER_RECEIVE_FROM_ISR
#define rtos_trace_stream_buffer_reset               	 traceSTREAM_BUFFER_RESET
#define rtos_trace_stream_buffer_send                	 traceSTREAM_BUFFER_SEND
#define rtos_trace_stream_buffer_send_failed         	 traceSTREAM_BUFFER_SEND_FAILED
#define rtos_trace_stream_buffer_send_from_isr       	 traceSTREAM_BUFFER_SEND_FROM_ISR
#define rtos_trace_take_mutex_recursive              	 traceTAKE_MUTEX_RECURSIVE
#define rtos_trace_take_mutex_recursive_failed       	 traceTAKE_MUTEX_RECURSIVE_FAILED
#define rtos_trace_task_create                       	 traceTASK_CREATE
#define rtos_trace_task_create_failed                	 traceTASK_CREATE_FAILED
#define rtos_trace_task_delay                        	 traceTASK_DELAY
#define rtos_trace_task_delay_until                  	 traceTASK_DELAY_UNTIL
#define rtos_trace_task_delete                       	 traceTASK_DELETE
#define rtos_trace_task_increment_tick               	 traceTASK_INCREMENT_TICK
#define rtos_trace_task_notify                       	 traceTASK_NOTIFY
#define rtos_trace_task_notify_from_isr              	 traceTASK_NOTIFY_FROM_ISR
#define rtos_trace_task_notify_give_from_isr         	 traceTASK_NOTIFY_GIVE_FROM_ISR
#define rtos_trace_task_notify_take                  	 traceTASK_NOTIFY_TAKE
#define rtos_trace_task_notify_take_block            	 traceTASK_NOTIFY_TAKE_BLOCK
#define rtos_trace_task_notify_wait                  	 traceTASK_NOTIFY_WAIT
#define rtos_trace_task_notify_wait_block            	 traceTASK_NOTIFY_WAIT_BLOCK
#define rtos_trace_task_priority_disinherit          	 traceTASK_PRIORITY_DISINHERIT
#define rtos_trace_task_priority_inherit             	 traceTASK_PRIORITY_INHERIT
#define rtos_trace_task_priority_set                 	 traceTASK_PRIORITY_SET
#define rtos_trace_task_resume                       	 traceTASK_RESUME
#define rtos_trace_task_resume_from_isr              	 traceTASK_RESUME_FROM_ISR
#define rtos_trace_task_suspend                      	 traceTASK_SUSPEND
#define rtos_trace_task_switched_in                  	 traceTASK_SWITCHED_IN
#define rtos_trace_task_switched_out                 	 traceTASK_SWITCHED_OUT
#define rtos_trace_timer_command_received            	 traceTIMER_COMMAND_RECEIVED
#define rtos_trace_timer_command_send                	 traceTIMER_COMMAND_SEND
#define rtos_trace_timer_create                      	 traceTIMER_CREATE
#define rtos_trace_timer_create_failed               	 traceTIMER_CREATE_FAILED
#define rtos_trace_timer_expired                     	 traceTIMER_EXPIRED


/* typedefs */
/* from timers.h */
#define rtos_base_type_t                             	 BaseType_t
/* from croutine.h */
#define rtos_co_routine_handle_t                     	 CoRoutineHandle_t
#define rtos_coroutine_code                          	 crCOROUTINE_CODE
#define rtos_crcb_t                                  	 CRCB_t
/* from event_groups.h */
#define rtos_event_bits_t                            	 EventBits_t
#define rtos_event_group_handle_t                    	 EventGroupHandle_t
/* from timers.h */
#define rtos_heap_region_t                           	 HeapRegion_t
#define rtos_heap_stats_t                            	 HeapStats_t
#define rtos_int16_t                                 	 int16_t
#define rtos_int32_t                                 	 int32_t
#define rtos_int64_t                                 	 int64_t
#define rtos_int8_t                                  	 int8_t
#define rtos_int_fast16_t                            	 int_fast16_t
#define rtos_int_fast32_t                            	 int_fast32_t
#define rtos_int_fast64_t                            	 int_fast64_t
#define rtos_int_fast8_t                             	 int_fast8_t
#define rtos_int_least16_t                           	 int_least16_t
#define rtos_int_least32_t                           	 int_least32_t
#define rtos_int_least64_t                           	 int_least64_t
#define rtos_int_least8_t                            	 int_least8_t
#define rtos_intmax_t                                	 intmax_t
#define rtos_intptr_t                                	 intptr_t
#define rtos_list_item_t                             	 ListItem_t
#define rtos_list_t                                  	 List_t
#define rtos_max_align_t                             	 max_align_t
#define rtos_memory_region_t                         	 MemoryRegion_t
/* from message_buffer.h */
#define rtos_message_buffer_handle_t                 	 MessageBufferHandle_t
/* from timers.h */
#define rtos_mini_list_item_t                        	 MiniListItem_t
#define rtos_notify_action                           	 eNotifyAction
#define rtos_pended_function_t                       	 PendedFunction_t
#define rtos_ptrdiff_t                               	 ptrdiff_t
/* from semphr.h */
#define rtos_queue_handle_t                          	 QueueHandle_t
#define rtos_queue_set_handle_t                      	 QueueSetHandle_t
#define rtos_queue_set_member_handle_t               	 QueueSetMemberHandle_t
#define rtos_semaphore_handle_t                      	 SemaphoreHandle_t
/* from timers.h */
#define rtos_size_t                                  	 size_t
#define rtos_sleep_mode_status                       	 eSleepModeStatus
#define rtos_stack_type_t                            	 StackType_t
#define rtos_static_event_group_t                    	 StaticEventGroup_t
#define rtos_static_list_item_t                      	 StaticListItem_t
#define rtos_static_list_t                           	 StaticList_t
#define rtos_static_message_buffer_t                 	 StaticMessageBuffer_t
#define rtos_static_mini_list_item_t                 	 StaticMiniListItem_t
#define rtos_static_queue_t                          	 StaticQueue_t
#define rtos_static_semaphore_t                      	 StaticSemaphore_t
#define rtos_static_stream_buffer_t                  	 StaticStreamBuffer_t
#define rtos_static_task_t                           	 StaticTask_t
#define rtos_static_timer_t                          	 StaticTimer_t
/* from stream_buffer.h */
#define rtos_stream_buffer_handle_t                  	 StreamBufferHandle_t
/* from timers.h */
#define rtos_task_function_t                         	 TaskFunction_t
#define rtos_task_handle_t                           	 TaskHandle_t
#define rtos_task_hook_function_t                    	 TaskHookFunction_t
#define rtos_task_parameters_t                       	 TaskParameters_t
#define rtos_task_state                              	 eTaskState
#define rtos_task_status_t                           	 TaskStatus_t
#define rtos_tick_type_t                             	 TickType_t
#define rtos_time_out_t                              	 TimeOut_t
#define rtos_timer_callback_function_t               	 TimerCallbackFunction_t
#define rtos_timer_handle_t                          	 TimerHandle_t
#define rtos_u_base_type_t                           	 UBaseType_t
#define rtos_uint16_t                                	 uint16_t
#define rtos_uint32_t                                	 uint32_t
#define rtos_uint64_t                                	 uint64_t
#define rtos_uint8_t                                 	 uint8_t
#define rtos_uint_fast16_t                           	 uint_fast16_t
#define rtos_uint_fast32_t                           	 uint_fast32_t
#define rtos_uint_fast64_t                           	 uint_fast64_t
#define rtos_uint_fast8_t                            	 uint_fast8_t
#define rtos_uint_least16_t                          	 uint_least16_t
#define rtos_uint_least32_t                          	 uint_least32_t
#define rtos_uint_least64_t                          	 uint_least64_t
#define rtos_uint_least8_t                           	 uint_least8_t
#define rtos_uintmax_t                               	 uintmax_t
#define rtos_uintptr_t                               	 uintptr_t
#define rtos_wchar_t                                 	 wchar_t


/* functions */
/* from timers.h */
#define __rtos_assert                                	 __assert
#define __rtos_assert_fail                           	 __assert_fail
#define __rtos_assert_perror_fail                    	 __assert_perror_fail
/* from semphr.h */
#define _rtos_queue_add_to_registry                  	 vQueueAddToRegistry
#define _rtos_queue_get_name                         	 pcQueueGetName
#define _rtos_queue_unregister_queue                 	 vQueueUnregisterQueue
/* from atomic.h */
#define rtos_atomic_add_u32                          	 Atomic_Add_u32
#define rtos_atomic_and_u32                          	 Atomic_AND_u32
#define rtos_atomic_compare_and_swap_pointers_p32    	 Atomic_CompareAndSwapPointers_p32
#define rtos_atomic_compare_and_swap_u32             	 Atomic_CompareAndSwap_u32
#define rtos_atomic_decrement_u32                    	 Atomic_Decrement_u32
#define rtos_atomic_increment_u32                    	 Atomic_Increment_u32
#define rtos_atomic_nand_u32                         	 Atomic_NAND_u32
#define rtos_atomic_or_u32                           	 Atomic_OR_u32
#define rtos_atomic_subtract_u32                     	 Atomic_Subtract_u32
#define rtos_atomic_swap_pointers_p32                	 Atomic_SwapPointers_p32
#define rtos_atomic_xor_u32                          	 Atomic_XOR_u32
/* from timers.h */
#define rtos_clear_interrupt_mask_from_isr           	 vClearInterruptMaskFromISR
/* from croutine.h */
#define rtos_co_routine_add_to_delayed_list          	 vCoRoutineAddToDelayedList
#define rtos_co_routine_create                       	 xCoRoutineCreate
#define rtos_co_routine_remove_from_event_list       	 xCoRoutineRemoveFromEventList
#define rtos_co_routine_schedule                     	 vCoRoutineSchedule
/* from event_groups.h */
#define rtos_event_group_clear_bits                  	 xEventGroupClearBits
#define rtos_event_group_clear_bits_callback         	 vEventGroupClearBitsCallback
#define rtos_event_group_clear_bits_from_isr         	 xEventGroupClearBitsFromISR
#define rtos_event_group_create                      	 xEventGroupCreate
#define rtos_event_group_delete                      	 vEventGroupDelete
#define rtos_event_group_get_bits_from_isr           	 xEventGroupGetBitsFromISR
#define rtos_event_group_get_number                  	 uxEventGroupGetNumber
#define rtos_event_group_set_bits                    	 xEventGroupSetBits
#define rtos_event_group_set_bits_callback           	 vEventGroupSetBitsCallback
#define rtos_event_group_set_bits_from_isr           	 xEventGroupSetBitsFromISR
#define rtos_event_group_set_number                  	 vEventGroupSetNumber
#define rtos_event_group_sync                        	 xEventGroupSync
#define rtos_event_group_wait_bits                   	 xEventGroupWaitBits
/* from timers.h */
#define rtos_list_initialise                         	 vListInitialise
#define rtos_list_initialise_item                    	 vListInitialiseItem
#define rtos_list_insert                             	 vListInsert
#define rtos_list_insert_end                         	 vListInsertEnd
#define rtos_list_remove                             	 uxListRemove
#define rtos_port_define_heap_regions                	 vPortDefineHeapRegions
#define rtos_port_end_scheduler                      	 vPortEndScheduler
#define rtos_port_enter_critical                     	 vPortEnterCritical
#define rtos_port_exit_critical                      	 vPortExitCritical
#define rtos_port_free                               	 vPortFree
#define rtos_port_get_free_heap_size                 	 xPortGetFreeHeapSize
#define rtos_port_get_heap_stats                     	 vPortGetHeapStats
#define rtos_port_get_minimum_ever_free_heap_size    	 xPortGetMinimumEverFreeHeapSize
#define rtos_port_initialise_blocks                  	 vPortInitialiseBlocks
#define rtos_port_initialise_stack                   	 pxPortInitialiseStack
#define rtos_port_malloc                             	 pvPortMalloc
#define rtos_port_start_scheduler                    	 xPortStartScheduler
#define rtos_port_suppress_ticks_and_sleep           	 vPortSuppressTicksAndSleep
#define rtos_port_yield                              	 vPortYield
/* from semphr.h */
#define rtos_queue_add_to_set                        	 xQueueAddToSet
#define rtos_queue_cr_receive                        	 xQueueCRReceive
#define rtos_queue_cr_receive_from_isr               	 xQueueCRReceiveFromISR
#define rtos_queue_cr_send                           	 xQueueCRSend
#define rtos_queue_cr_send_from_isr                  	 xQueueCRSendFromISR
#define rtos_queue_create_counting_semaphore         	 xQueueCreateCountingSemaphore
#define rtos_queue_create_counting_semaphore_static  	 xQueueCreateCountingSemaphoreStatic
#define rtos_queue_create_mutex                      	 xQueueCreateMutex
#define rtos_queue_create_mutex_static               	 xQueueCreateMutexStatic
#define rtos_queue_create_set                        	 xQueueCreateSet
#define rtos_queue_delete                            	 vQueueDelete
#define rtos_queue_generic_create                    	 xQueueGenericCreate
#define rtos_queue_generic_reset                     	 xQueueGenericReset
#define rtos_queue_generic_send                      	 xQueueGenericSend
#define rtos_queue_generic_send_from_isr             	 xQueueGenericSendFromISR
#define rtos_queue_get_mutex_holder                  	 xQueueGetMutexHolder
#define rtos_queue_get_mutex_holder_from_isr         	 xQueueGetMutexHolderFromISR
#define rtos_queue_get_queue_number                  	 uxQueueGetQueueNumber
#define rtos_queue_get_queue_type                    	 ucQueueGetQueueType
#define rtos_queue_give_from_isr                     	 xQueueGiveFromISR
#define rtos_queue_give_mutex_recursive              	 xQueueGiveMutexRecursive
#define rtos_queue_is_queue_empty_from_isr           	 xQueueIsQueueEmptyFromISR
#define rtos_queue_is_queue_full_from_isr            	 xQueueIsQueueFullFromISR
#define rtos_queue_messages_waiting                  	 uxQueueMessagesWaiting
#define rtos_queue_messages_waiting_from_isr         	 uxQueueMessagesWaitingFromISR
#define rtos_queue_peek                              	 xQueuePeek
#define rtos_queue_peek_from_isr                     	 xQueuePeekFromISR
#define rtos_queue_receive                           	 xQueueReceive
#define rtos_queue_receive_from_isr                  	 xQueueReceiveFromISR
#define rtos_queue_remove_from_set                   	 xQueueRemoveFromSet
#define rtos_queue_select_from_set                   	 xQueueSelectFromSet
#define rtos_queue_select_from_set_from_isr          	 xQueueSelectFromSetFromISR
#define rtos_queue_semaphore_take                    	 xQueueSemaphoreTake
#define rtos_queue_set_queue_number                  	 vQueueSetQueueNumber
#define rtos_queue_spaces_available                  	 uxQueueSpacesAvailable
#define rtos_queue_take_mutex_recursive              	 xQueueTakeMutexRecursive
#define rtos_queue_wait_for_message_restricted       	 vQueueWaitForMessageRestricted
/* from timers.h */
#define rtos_set_interrupt_mask_from_isr             	 ulSetInterruptMaskFromISR
/* from stream_buffer.h */
#define rtos_stream_buffer_bytes_available           	 xStreamBufferBytesAvailable
#define rtos_stream_buffer_delete                    	 vStreamBufferDelete
#define rtos_stream_buffer_generic_create            	 xStreamBufferGenericCreate
#define rtos_stream_buffer_generic_create_static     	 xStreamBufferGenericCreateStatic
#define rtos_stream_buffer_get_stream_buffer_number  	 uxStreamBufferGetStreamBufferNumber
#define rtos_stream_buffer_get_stream_buffer_type    	 ucStreamBufferGetStreamBufferType
#define rtos_stream_buffer_is_empty                  	 xStreamBufferIsEmpty
#define rtos_stream_buffer_is_full                   	 xStreamBufferIsFull
#define rtos_stream_buffer_next_message_length_bytes 	 xStreamBufferNextMessageLengthBytes
#define rtos_stream_buffer_receive                   	 xStreamBufferReceive
#define rtos_stream_buffer_receive_completed_from_isr 	 xStreamBufferReceiveCompletedFromISR
#define rtos_stream_buffer_receive_from_isr          	 xStreamBufferReceiveFromISR
#define rtos_stream_buffer_reset                     	 xStreamBufferReset
#define rtos_stream_buffer_send                      	 xStreamBufferSend
#define rtos_stream_buffer_send_completed_from_isr   	 xStreamBufferSendCompletedFromISR
#define rtos_stream_buffer_send_from_isr             	 xStreamBufferSendFromISR
#define rtos_stream_buffer_set_stream_buffer_number  	 vStreamBufferSetStreamBufferNumber
#define rtos_stream_buffer_set_trigger_level         	 xStreamBufferSetTriggerLevel
#define rtos_stream_buffer_spaces_available          	 xStreamBufferSpacesAvailable
/* from timers.h */
#define rtos_task_abort_delay                        	 xTaskAbortDelay
#define rtos_task_allocate_mpu_regions               	 vTaskAllocateMPURegions
#define rtos_task_call_application_task_hook         	 xTaskCallApplicationTaskHook
#define rtos_task_catch_up_ticks                     	 xTaskCatchUpTicks
#define rtos_task_check_for_time_out                 	 xTaskCheckForTimeOut
#define rtos_task_confirm_sleep_mode_status          	 eTaskConfirmSleepModeStatus
#define rtos_task_create                             	 xTaskCreate
#define rtos_task_delay                              	 vTaskDelay
#define rtos_task_delay_until                        	 xTaskDelayUntil
#define rtos_task_delete                             	 vTaskDelete
#define rtos_task_end_scheduler                      	 vTaskEndScheduler
#define rtos_task_generic_notify                     	 xTaskGenericNotify
#define rtos_task_generic_notify_from_isr            	 xTaskGenericNotifyFromISR
#define rtos_task_generic_notify_give_from_isr       	 vTaskGenericNotifyGiveFromISR
#define rtos_task_generic_notify_state_clear         	 xTaskGenericNotifyStateClear
#define rtos_task_generic_notify_take                	 ulTaskGenericNotifyTake
#define rtos_task_generic_notify_value_clear         	 ulTaskGenericNotifyValueClear
#define rtos_task_generic_notify_wait                	 xTaskGenericNotifyWait
#define rtos_task_get_current_task_handle            	 xTaskGetCurrentTaskHandle
#define rtos_task_get_current_task_handle_cpu        	 xTaskGetCurrentTaskHandleCPU
#define rtos_task_get_handle                         	 xTaskGetHandle
#define rtos_task_get_idle_run_time_counter          	 ulTaskGetIdleRunTimeCounter
#define rtos_task_get_idle_task_handle               	 xTaskGetIdleTaskHandle
#define rtos_task_get_info                           	 vTaskGetInfo
#define rtos_task_get_name                           	 pcTaskGetName
#define rtos_task_get_number_of_tasks                	 uxTaskGetNumberOfTasks
#define rtos_task_get_run_time_stats                 	 vTaskGetRunTimeStats
#define rtos_task_get_scheduler_state                	 xTaskGetSchedulerState
#define rtos_task_get_stack_high_water_mark          	 uxTaskGetStackHighWaterMark
#define rtos_task_get_stack_high_water_mark2         	 uxTaskGetStackHighWaterMark2
#define rtos_task_get_state                          	 eTaskGetState
#define rtos_task_get_system_state                   	 uxTaskGetSystemState
#define rtos_task_get_task_number                    	 uxTaskGetTaskNumber
#define rtos_task_get_thread_local_storage_pointer   	 pvTaskGetThreadLocalStoragePointer
#define rtos_task_get_tick_count                     	 xTaskGetTickCount
#define rtos_task_get_tick_count_from_isr            	 xTaskGetTickCountFromISR
#define rtos_task_increment_mutex_held_count         	 pvTaskIncrementMutexHeldCount
#define rtos_task_increment_tick                     	 xTaskIncrementTick
#define rtos_task_internal_set_time_out_state        	 vTaskInternalSetTimeOutState
#define rtos_task_list                               	 vTaskList
#define rtos_task_missed_yield                       	 vTaskMissedYield
#define rtos_task_place_on_event_list                	 vTaskPlaceOnEventList
#define rtos_task_place_on_event_list_restricted     	 vTaskPlaceOnEventListRestricted
#define rtos_task_place_on_unordered_event_list      	 vTaskPlaceOnUnorderedEventList
#define rtos_task_preemption_disable                 	 vTaskPreemptionDisable
#define rtos_task_preemption_enable                  	 vTaskPreemptionEnable
#define rtos_task_priority_disinherit                	 xTaskPriorityDisinherit
#define rtos_task_priority_disinherit_after_timeout  	 vTaskPriorityDisinheritAfterTimeout
#define rtos_task_priority_get                       	 uxTaskPriorityGet
#define rtos_task_priority_get_from_isr              	 uxTaskPriorityGetFromISR
#define rtos_task_priority_inherit                   	 xTaskPriorityInherit
#define rtos_task_priority_set                       	 vTaskPrioritySet
#define rtos_task_remove_from_event_list             	 xTaskRemoveFromEventList
#define rtos_task_remove_from_unordered_event_list   	 vTaskRemoveFromUnorderedEventList
#define rtos_task_reset_event_item_value             	 uxTaskResetEventItemValue
#define rtos_task_resume                             	 vTaskResume
#define rtos_task_resume_all                         	 xTaskResumeAll
#define rtos_task_resume_from_isr                    	 xTaskResumeFromISR
#define rtos_task_set_task_number                    	 vTaskSetTaskNumber
#define rtos_task_set_thread_local_storage_pointer   	 vTaskSetThreadLocalStoragePointer
#define rtos_task_set_time_out_state                 	 vTaskSetTimeOutState
#define rtos_task_start_scheduler                    	 vTaskStartScheduler
#define rtos_task_step_tick                          	 vTaskStepTick
#define rtos_task_suspend                            	 vTaskSuspend
#define rtos_task_suspend_all                        	 vTaskSuspendAll
#define rtos_task_yield_within_api                   	 vTaskYieldWithinAPI
#define rtos_timer_create                            	 xTimerCreate
#define rtos_timer_create_timer_task                 	 xTimerCreateTimerTask
#define rtos_timer_generic_command_from_isr          	 xTimerGenericCommandFromISR
#define rtos_timer_generic_command_from_task         	 xTimerGenericCommandFromTask
#define rtos_timer_get_expiry_time                   	 xTimerGetExpiryTime
#define rtos_timer_get_name                          	 pcTimerGetName
#define rtos_timer_get_period                        	 xTimerGetPeriod
#define rtos_timer_get_reload_mode                   	 uxTimerGetReloadMode
#define rtos_timer_get_timer_daemon_task_handle      	 xTimerGetTimerDaemonTaskHandle
#define rtos_timer_get_timer_id                      	 pvTimerGetTimerID
#define rtos_timer_get_timer_number                  	 uxTimerGetTimerNumber
#define rtos_timer_is_timer_active                   	 xTimerIsTimerActive
#define rtos_timer_pend_function_call                	 xTimerPendFunctionCall
#define rtos_timer_pend_function_call_from_isr       	 xTimerPendFunctionCallFromISR
#define rtos_timer_set_reload_mode                   	 vTimerSetReloadMode
#define rtos_timer_set_timer_id                      	 vTimerSetTimerID
#define rtos_timer_set_timer_number                  	 vTimerSetTimerNumber


#endif
