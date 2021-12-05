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
#define RTOS_ATOMIC_ENTER_CRITICAL                   	 ATOMIC_ENTER_CRITICAL
#define RTOS_ATOMIC_EXIT_CRITICAL                    	 ATOMIC_EXIT_CRITICAL
#define RTOS_ATOMIC_H                                	 ATOMIC_H
/* from projdefs.h */
#define RTOS_BIG_ENDIAN                              	 pdBIG_ENDIAN
/* from event_groups.h */
#define RTOS_BIT_0                                   	 BIT_0
#define RTOS_BIT_4                                   	 BIT_4
/* from FreeRTOS.h */
#define RTOS_CONFIG_APPLICATION_ALLOCATED_HEAP       	 configAPPLICATION_ALLOCATED_HEAP
#define RTOS_CONFIG_ASSERT                           	 configASSERT
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
/* from FreeRTOS.h */
#define RTOS_CONFIG_MAX                              	 configMAX
/* from FreeRTOSConfig.h */
#define RTOS_CONFIG_MAX_API_CALL_INTERRUPT_PRIORITY  	 configMAX_API_CALL_INTERRUPT_PRIORITY
#define RTOS_CONFIG_MAX_CO_ROUTINE_PRIORITIES        	 configMAX_CO_ROUTINE_PRIORITIES
#define RTOS_CONFIG_MAX_PRIORITIES                   	 configMAX_PRIORITIES
#define RTOS_CONFIG_MAX_SYSCALL_INTERRUPT_PRIORITY   	 configMAX_SYSCALL_INTERRUPT_PRIORITY
/* from FreeRTOS.h */
#define RTOS_CONFIG_MAX_TASK_NAME_LEN                	 configMAX_TASK_NAME_LEN
#define RTOS_CONFIG_MESSAGE_BUFFER_LENGTH_TYPE       	 configMESSAGE_BUFFER_LENGTH_TYPE
#define RTOS_CONFIG_MIN                              	 configMIN
/* from FreeRTOSConfig.h */
#define RTOS_CONFIG_MINIMAL_STACK_SIZE               	 configMINIMAL_STACK_SIZE
/* from FreeRTOS.h */
#define RTOS_CONFIG_NUM_CORES                        	 configNUM_CORES
#define RTOS_CONFIG_NUM_THREAD_LOCAL_STORAGE_POINTERS 	 configNUM_THREAD_LOCAL_STORAGE_POINTERS
#define RTOS_CONFIG_POST_SLEEP_PROCESSING            	 configPOST_SLEEP_PROCESSING
#define RTOS_CONFIG_PRECONDITION                     	 configPRECONDITION
#define RTOS_CONFIG_PRECONDITION_DEFINED             	 configPRECONDITION_DEFINED
#define RTOS_CONFIG_PRE_SLEEP_PROCESSING             	 configPRE_SLEEP_PROCESSING
#define RTOS_CONFIG_PRE_SUPPRESS_TICKS_AND_SLEEP_PROCESSING 	 configPRE_SUPPRESS_TICKS_AND_SLEEP_PROCESSING
#define RTOS_CONFIG_PRINTF                           	 configPRINTF
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
/* from croutine.h */
#define RTOS_CR_DELAY                                	 crDELAY
#define RTOS_CR_END                                  	 crEND
#define RTOS_CR_QUEUE_RECEIVE                        	 crQUEUE_RECEIVE
#define RTOS_CR_QUEUE_RECEIVE_FROM_ISR               	 crQUEUE_RECEIVE_FROM_ISR
#define RTOS_CR_QUEUE_SEND                           	 crQUEUE_SEND
#define RTOS_CR_QUEUE_SEND_FROM_ISR                  	 crQUEUE_SEND_FROM_ISR
#define RTOS_CR_SET_STATE0                           	 crSET_STATE0
#define RTOS_CR_SET_STATE1                           	 crSET_STATE1
#define RTOS_CR_START                                	 crSTART
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
/* from event_groups.h */
#define RTOS_EVENT_GROUP_GET_BITS                    	 xEventGroupGetBits
/* from mpu_wrappers.h */
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
#define RTOS_INCLUDE_TASK_GET_STATE                  	 INCLUDE_eTaskGetState
#define RTOS_INCLUDE_TASK_PRIORITY_SET               	 INCLUDE_vTaskPrioritySet
#define RTOS_INCLUDE_TASK_RESUME_FROM_ISR            	 INCLUDE_xTaskResumeFromISR
#define RTOS_INCLUDE_TASK_SUSPEND                    	 INCLUDE_vTaskSuspend
#define RTOS_INCLUDE_TIMER_PEND_FUNCTION_CALL        	 INCLUDE_xTimerPendFunctionCall
#define RTOS_INCLUDE_UX_TASK_GET_STACK_HIGH_WATER_MARK 	 INCLUDE_uxTaskGetStackHighWaterMark
#define RTOS_INCLUDE_UX_TASK_GET_STACK_HIGH_WATER_MARK2 	 INCLUDE_uxTaskGetStackHighWaterMark2
#define RTOS_INCLUDE_UX_TASK_PRIORITY_GET            	 INCLUDE_uxTaskPriorityGet
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
#define RTOS_LIST_CURRENT_LIST_LENGTH                	 listCURRENT_LIST_LENGTH
#define RTOS_LIST_FIRST_LIST_INTEGRITY_CHECK_VALUE   	 listFIRST_LIST_INTEGRITY_CHECK_VALUE
#define RTOS_LIST_FIRST_LIST_ITEM_INTEGRITY_CHECK_VALUE 	 listFIRST_LIST_ITEM_INTEGRITY_CHECK_VALUE
#define RTOS_LIST_GET_END_MARKER                     	 listGET_END_MARKER
#define RTOS_LIST_GET_HEAD_ENTRY                     	 listGET_HEAD_ENTRY
#define RTOS_LIST_GET_ITEM_VALUE_OF_HEAD_ENTRY       	 listGET_ITEM_VALUE_OF_HEAD_ENTRY
#define RTOS_LIST_GET_LIST_ITEM_OWNER                	 listGET_LIST_ITEM_OWNER
#define RTOS_LIST_GET_LIST_ITEM_VALUE                	 listGET_LIST_ITEM_VALUE
#define RTOS_LIST_GET_NEXT                           	 listGET_NEXT
#define RTOS_LIST_GET_OWNER_OF_HEAD_ENTRY            	 listGET_OWNER_OF_HEAD_ENTRY
#define RTOS_LIST_GET_OWNER_OF_NEXT_ENTRY            	 listGET_OWNER_OF_NEXT_ENTRY
#define RTOS_LIST_H                                  	 LIST_H
#define RTOS_LIST_IS_CONTAINED_WITHIN                	 listIS_CONTAINED_WITHIN
/* from FreeRTOS.h */
#define RTOS_LIST_ITEM                               	 xListItem
/* from list.h */
#define RTOS_LIST_LIST_IS_EMPTY                      	 listLIST_IS_EMPTY
#define RTOS_LIST_LIST_IS_INITIALISED                	 listLIST_IS_INITIALISED
#define RTOS_LIST_LIST_ITEM_CONTAINER                	 listLIST_ITEM_CONTAINER
#define RTOS_LIST_SECOND_LIST_INTEGRITY_CHECK_VALUE  	 listSECOND_LIST_INTEGRITY_CHECK_VALUE
#define RTOS_LIST_SECOND_LIST_ITEM_INTEGRITY_CHECK_VALUE 	 listSECOND_LIST_ITEM_INTEGRITY_CHECK_VALUE
#define RTOS_LIST_SET_FIRST_LIST_ITEM_INTEGRITY_CHECK_VALUE 	 listSET_FIRST_LIST_ITEM_INTEGRITY_CHECK_VALUE
#define RTOS_LIST_SET_LIST_INTEGRITY_CHECK_1_VALUE   	 listSET_LIST_INTEGRITY_CHECK_1_VALUE
#define RTOS_LIST_SET_LIST_INTEGRITY_CHECK_2_VALUE   	 listSET_LIST_INTEGRITY_CHECK_2_VALUE
#define RTOS_LIST_SET_LIST_ITEM_OWNER                	 listSET_LIST_ITEM_OWNER
#define RTOS_LIST_SET_LIST_ITEM_VALUE                	 listSET_LIST_ITEM_VALUE
#define RTOS_LIST_SET_SECOND_LIST_ITEM_INTEGRITY_CHECK_VALUE 	 listSET_SECOND_LIST_ITEM_INTEGRITY_CHECK_VALUE
#define RTOS_LIST_TEST_LIST_INTEGRITY                	 listTEST_LIST_INTEGRITY
#define RTOS_LIST_TEST_LIST_ITEM_INTEGRITY           	 listTEST_LIST_ITEM_INTEGRITY
/* from projdefs.h */
#define RTOS_LITTLE_ENDIAN                           	 pdLITTLE_ENDIAN
/* from FreeRTOS.h */
#define RTOS_MEMORY_REGION                           	 xMemoryRegion
/* from message_buffer.h */
#define RTOS_MESSAGE_BUFFER_CREATE                   	 xMessageBufferCreate
#define RTOS_MESSAGE_BUFFER_CREATE_STATIC            	 xMessageBufferCreateStatic
#define RTOS_MESSAGE_BUFFER_DELETE                   	 vMessageBufferDelete
#define RTOS_MESSAGE_BUFFER_IS_EMPTY                 	 xMessageBufferIsEmpty
#define RTOS_MESSAGE_BUFFER_IS_FULL                  	 xMessageBufferIsFull
#define RTOS_MESSAGE_BUFFER_NEXT_LENGTH_BYTES        	 xMessageBufferNextLengthBytes
#define RTOS_MESSAGE_BUFFER_RECEIVE                  	 xMessageBufferReceive
#define RTOS_MESSAGE_BUFFER_RECEIVE_COMPLETED_FROM_ISR 	 xMessageBufferReceiveCompletedFromISR
#define RTOS_MESSAGE_BUFFER_RECEIVE_FROM_ISR         	 xMessageBufferReceiveFromISR
#define RTOS_MESSAGE_BUFFER_RESET                    	 xMessageBufferReset
#define RTOS_MESSAGE_BUFFER_SEND                     	 xMessageBufferSend
#define RTOS_MESSAGE_BUFFER_SEND_COMPLETED_FROM_ISR  	 xMessageBufferSendCompletedFromISR
#define RTOS_MESSAGE_BUFFER_SEND_FROM_ISR            	 xMessageBufferSendFromISR
#define RTOS_MESSAGE_BUFFER_SPACES_AVAILABLE         	 xMessageBufferSpacesAvailable
#define RTOS_MESSAGE_BUFFER_SPACE_AVAILABLE          	 xMessageBufferSpaceAvailable
/* from mpu_wrappers.h */
#define RTOS_MPU_WRAPPERS_H                          	 MPU_WRAPPERS_H
/* from projdefs.h */
#define RTOS_MS_TO_TICKS                             	 pdMS_TO_TICKS
/* from FreeRTOS.h */
#define RTOS_MT_COVERAGE_TEST_DELAY                  	 mtCOVERAGE_TEST_DELAY
#define RTOS_MT_COVERAGE_TEST_MARKER                 	 mtCOVERAGE_TEST_MARKER
/* from projdefs.h */
#define RTOS_PASS                                    	 pdPASS
/* from portable.h */
#define RTOS_PORTABLE_H                              	 PORTABLE_H
/* from portmacro.h */
#define RTOS_PORTMACRO_H                             	 PORTMACRO_H
/* from FreeRTOS.h */
#define RTOS_PORT_ALLOCATE_SECURE_CONTEXT            	 portALLOCATE_SECURE_CONTEXT
/* from portable.h */
#define RTOS_PORT_ARCH_NAME                          	 portARCH_NAME
/* from FreeRTOS.h */
#define RTOS_PORT_ASSERT_IF_INTERRUPT_PRIORITY_INVALID 	 portASSERT_IF_INTERRUPT_PRIORITY_INVALID
#define RTOS_PORT_ASSERT_IF_IN_ISR                   	 portASSERT_IF_IN_ISR
/* from portmacro.h */
#define RTOS_PORT_BASE_TYPE                          	 portBASE_TYPE
#define RTOS_PORT_BYTE_ALIGNMENT                     	 portBYTE_ALIGNMENT
/* from portable.h */
#define RTOS_PORT_BYTE_ALIGNMENT_MASK                	 portBYTE_ALIGNMENT_MASK
/* from portmacro.h */
#define RTOS_PORT_CHAR                               	 portCHAR
#define RTOS_PORT_CHECK_IF_IN_ISR                    	 portCHECK_IF_IN_ISR
/* from FreeRTOS.h */
#define RTOS_PORT_CLEAN_UP_TCB                       	 portCLEAN_UP_TCB
/* from portmacro.h */
#define RTOS_PORT_CLEAR_INTERRUPT_MASK_FROM_ISR      	 portCLEAR_INTERRUPT_MASK_FROM_ISR
/* from FreeRTOS.h */
#define RTOS_PORT_CONFIGURE_TIMER_FOR_RUN_TIME_STATS 	 portCONFIGURE_TIMER_FOR_RUN_TIME_STATS
/* from portmacro.h */
#define RTOS_PORT_CRITICAL_NESTING_IN_TCB            	 portCRITICAL_NESTING_IN_TCB
#define RTOS_PORT_DISABLE_INTERRUPTS                 	 portDISABLE_INTERRUPTS
#define RTOS_PORT_DONT_DISCARD                       	 portDONT_DISCARD
#define RTOS_PORT_DOUBLE                             	 portDOUBLE
#define RTOS_PORT_ENABLE_INTERRUPTS                  	 portENABLE_INTERRUPTS
#define RTOS_PORT_END_SWITCHING_ISR                  	 portEND_SWITCHING_ISR
#define RTOS_PORT_ENTER_CRITICAL                     	 portENTER_CRITICAL
#define RTOS_PORT_EXIT_CRITICAL                      	 portEXIT_CRITICAL
#define RTOS_PORT_FLOAT                              	 portFLOAT
/* from atomic.h */
#define RTOS_PORT_FORCE_INLINE                       	 portFORCE_INLINE
/* from portable.h */
#define RTOS_PORT_FREE_STACK                         	 vPortFreeStack
/* from portmacro.h */
#define RTOS_PORT_GET_CORE_ID                        	 portGET_CORE_ID
#define RTOS_PORT_GET_ISR_LOCK                       	 portGET_ISR_LOCK
#define RTOS_PORT_GET_TASK_LOCK                      	 portGET_TASK_LOCK
/* from portable.h */
#define RTOS_PORT_HAS_STACK_OVERFLOW_CHECKING        	 portHAS_STACK_OVERFLOW_CHECKING
/* from portmacro.h */
#define RTOS_PORT_LONG                               	 portLONG
#define RTOS_PORT_MAX_DELAY                          	 portMAX_DELAY
#define RTOS_PORT_MEMORY_BARRIER                     	 portMEMORY_BARRIER
#define RTOS_PORT_NOP                                	 portNOP
/* from portable.h */
#define RTOS_PORT_NUM_CONFIGURABLE_REGIONS           	 portNUM_CONFIGURABLE_REGIONS
/* from portmacro.h */
#define RTOS_PORT_NVIC_INT_CTRL_REG                  	 portNVIC_INT_CTRL_REG
#define RTOS_PORT_NVIC_PENDSVSET_BIT                 	 portNVIC_PENDSVSET_BIT
/* from FreeRTOS.h */
#define RTOS_PORT_POINTER_SIZE_TYPE                  	 portPOINTER_SIZE_TYPE
#define RTOS_PORT_PRE_TASK_DELETE_HOOK               	 portPRE_TASK_DELETE_HOOK
#define RTOS_PORT_PRIVILEGE_BIT                      	 portPRIVILEGE_BIT
/* from portmacro.h */
#define RTOS_PORT_RELEASE_ISR_LOCK                   	 portRELEASE_ISR_LOCK
#define RTOS_PORT_RELEASE_TASK_LOCK                  	 portRELEASE_TASK_LOCK
#define RTOS_PORT_RESTORE_INTERRUPTS                 	 portRESTORE_INTERRUPTS
/* from FreeRTOS.h */
#define RTOS_PORT_SETUP_TCB                          	 portSETUP_TCB
/* from portmacro.h */
#define RTOS_PORT_SET_INTERRUPT_MASK_FROM_ISR        	 portSET_INTERRUPT_MASK_FROM_ISR
#define RTOS_PORT_SHORT                              	 portSHORT
/* from FreeRTOS.h */
#define RTOS_PORT_SOFTWARE_BARRIER                   	 portSOFTWARE_BARRIER
/* from portmacro.h */
#define RTOS_PORT_STACK_GROWTH                       	 portSTACK_GROWTH
/* from stack_macros.h */
#define RTOS_PORT_STACK_LIMIT_PADDING                	 portSTACK_LIMIT_PADDING
/* from portmacro.h */
#define RTOS_PORT_STACK_TYPE                         	 portSTACK_TYPE
#define RTOS_PORT_SUPPRESS_TICKS_AND_SLEEP           	 portSUPPRESS_TICKS_AND_SLEEP
#define RTOS_PORT_TASK_FUNCTION                      	 portTASK_FUNCTION
#define RTOS_PORT_TASK_FUNCTION_PROTO                	 portTASK_FUNCTION_PROTO
/* from FreeRTOS.h */
#define RTOS_PORT_TASK_USES_FLOATING_POINT           	 portTASK_USES_FLOATING_POINT
/* from portmacro.h */
#define RTOS_PORT_TICK_PERIOD_MS                     	 portTICK_PERIOD_MS
/* from FreeRTOS.h */
#define RTOS_PORT_TICK_RATE_MS                       	 portTICK_RATE_MS
#define RTOS_PORT_TICK_TYPE                          	 portTickType
#define RTOS_PORT_TICK_TYPE_CLEAR_INTERRUPT_MASK_FROM_ISR 	 portTICK_TYPE_CLEAR_INTERRUPT_MASK_FROM_ISR
#define RTOS_PORT_TICK_TYPE_ENTER_CRITICAL           	 portTICK_TYPE_ENTER_CRITICAL
#define RTOS_PORT_TICK_TYPE_EXIT_CRITICAL            	 portTICK_TYPE_EXIT_CRITICAL
/* from portmacro.h */
#define RTOS_PORT_TICK_TYPE_IS_ATOMIC                	 portTICK_TYPE_IS_ATOMIC
/* from FreeRTOS.h */
#define RTOS_PORT_TICK_TYPE_SET_INTERRUPT_MASK_FROM_ISR 	 portTICK_TYPE_SET_INTERRUPT_MASK_FROM_ISR
#define RTOS_PORT_TIMER_CALLBACK_ATTRIBUTE           	 portTIMER_CALLBACK_ATTRIBUTE
/* from mpu_wrappers.h */
#define RTOS_PORT_USING_MPU_WRAPPERS                 	 portUSING_MPU_WRAPPERS
/* from portmacro.h */
#define RTOS_PORT_YIELD                              	 portYIELD
#define RTOS_PORT_YIELD_CORE                         	 portYIELD_CORE
#define RTOS_PORT_YIELD_FROM_ISR                     	 portYIELD_FROM_ISR
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
/* from queue.h */
#define RTOS_QUEUE_CREATE                            	 xQueueCreate
/* from mpu_wrappers.h */
#define RTOS_QUEUE_CREATE_COUNTING_SEMAPHORE         	 xQueueCreateCountingSemaphore
#define RTOS_QUEUE_CREATE_COUNTING_SEMAPHORE_STATIC  	 xQueueCreateCountingSemaphoreStatic
#define RTOS_QUEUE_CREATE_MUTEX                      	 xQueueCreateMutex
#define RTOS_QUEUE_CREATE_MUTEX_STATIC               	 xQueueCreateMutexStatic
#define RTOS_QUEUE_CREATE_SET                        	 xQueueCreateSet
/* from queue.h */
#define RTOS_QUEUE_CREATE_STATIC                     	 xQueueCreateStatic
/* from mpu_wrappers.h */
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
#define RTOS_QUEUE_OVERWRITE                         	 xQueueOverwrite
#define RTOS_QUEUE_OVERWRITE_FROM_ISR                	 xQueueOverwriteFromISR
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
/* from queue.h */
#define RTOS_QUEUE_RESET                             	 xQueueReset
/* from mpu_wrappers.h */
#define RTOS_QUEUE_SELECT_FROM_SET                   	 xQueueSelectFromSet
#define RTOS_QUEUE_SEMAPHORE_TAKE                    	 xQueueSemaphoreTake
/* from queue.h */
#define RTOS_QUEUE_SEND                              	 xQueueSend
#define RTOS_QUEUE_SEND_FROM_ISR                     	 xQueueSendFromISR
#define RTOS_QUEUE_SEND_TO_BACK                      	 xQueueSendToBack
#define RTOS_QUEUE_SEND_TO_BACK_FROM_ISR             	 xQueueSendToBackFromISR
#define RTOS_QUEUE_SEND_TO_FRONT                     	 xQueueSendToFront
#define RTOS_QUEUE_SEND_TO_FRONT_FROM_ISR            	 xQueueSendToFrontFromISR
/* from FreeRTOS.h */
#define RTOS_QUEUE_SET_HANDLE                        	 xQueueSetHandle
#define RTOS_QUEUE_SET_MEMBER_HANDLE                 	 xQueueSetMemberHandle
/* from mpu_wrappers.h */
#define RTOS_QUEUE_TAKE_MUTEX_RECURSIVE              	 xQueueTakeMutexRecursive
#define RTOS_QUEUE_UNREGISTER_QUEUE                  	 vQueueUnregisterQueue
/* from semphr.h */
#define RTOS_SEMAPHORE_CREATE_BINARY                 	 xSemaphoreCreateBinary
#define RTOS_SEMAPHORE_CREATE_BINARY_STATIC          	 xSemaphoreCreateBinaryStatic
#define RTOS_SEMAPHORE_CREATE_COUNTING               	 xSemaphoreCreateCounting
#define RTOS_SEMAPHORE_CREATE_COUNTING_STATIC        	 xSemaphoreCreateCountingStatic
#define RTOS_SEMAPHORE_CREATE_MUTEX                  	 xSemaphoreCreateMutex
#define RTOS_SEMAPHORE_CREATE_MUTEX_STATIC           	 xSemaphoreCreateMutexStatic
#define RTOS_SEMAPHORE_CREATE_RECURSIVE_MUTEX        	 xSemaphoreCreateRecursiveMutex
#define RTOS_SEMAPHORE_CREATE_RECURSIVE_MUTEX_STATIC 	 xSemaphoreCreateRecursiveMutexStatic
#define RTOS_SEMAPHORE_DELETE                        	 vSemaphoreDelete
#define RTOS_SEMAPHORE_GET_MUTEX_HOLDER              	 xSemaphoreGetMutexHolder
#define RTOS_SEMAPHORE_GET_MUTEX_HOLDER_FROM_ISR     	 xSemaphoreGetMutexHolderFromISR
#define RTOS_SEMAPHORE_GIVE                          	 xSemaphoreGive
#define RTOS_SEMAPHORE_GIVE_FROM_ISR                 	 xSemaphoreGiveFromISR
#define RTOS_SEMAPHORE_GIVE_RECURSIVE                	 xSemaphoreGiveRecursive
#define RTOS_SEMAPHORE_H                             	 SEMAPHORE_H
/* from FreeRTOS.h */
#define RTOS_SEMAPHORE_HANDLE                        	 xSemaphoreHandle
/* from semphr.h */
#define RTOS_SEMAPHORE_TAKE                          	 xSemaphoreTake
#define RTOS_SEMAPHORE_TAKE_FROM_ISR                 	 xSemaphoreTakeFromISR
#define RTOS_SEMAPHORE_TAKE_RECURSIVE                	 xSemaphoreTakeRecursive
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
/* from stream_buffer.h */
#define RTOS_STREAM_BUFFER_CREATE                    	 xStreamBufferCreate
#define RTOS_STREAM_BUFFER_CREATE_STATIC             	 xStreamBufferCreateStatic
/* from mpu_wrappers.h */
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
/* from stack_macros.h */
#define RTOS_TASK_CHECK_FOR_STACK_OVERFLOW           	 taskCHECK_FOR_STACK_OVERFLOW
/* from mpu_wrappers.h */
#define RTOS_TASK_CHECK_FOR_TIME_OUT                 	 xTaskCheckForTimeOut
/* from task.h */
#define RTOS_TASK_CHECK_IF_IN_ISR                    	 taskCHECK_IF_IN_ISR
/* from FreeRTOS.h */
#define RTOS_TASK_CODE                               	 pdTASK_CODE
/* from mpu_wrappers.h */
#define RTOS_TASK_CREATE                             	 xTaskCreate
#define RTOS_TASK_CREATE_STATIC                      	 xTaskCreateStatic
#define RTOS_TASK_DELAY                              	 vTaskDelay
#define RTOS_TASK_DELAY_UNTIL                        	 xTaskDelayUntil
#define RTOS_TASK_DELETE                             	 vTaskDelete
/* from task.h */
#define RTOS_TASK_DISABLE_INTERRUPTS                 	 taskDISABLE_INTERRUPTS
#define RTOS_TASK_ENABLE_INTERRUPTS                  	 taskENABLE_INTERRUPTS
#define RTOS_TASK_ENTER_CRITICAL                     	 taskENTER_CRITICAL
#define RTOS_TASK_ENTER_CRITICAL_FROM_ISR            	 taskENTER_CRITICAL_FROM_ISR
#define RTOS_TASK_EXIT_CRITICAL                      	 taskEXIT_CRITICAL
#define RTOS_TASK_EXIT_CRITICAL_FROM_ISR             	 taskEXIT_CRITICAL_FROM_ISR
/* from mpu_wrappers.h */
#define RTOS_TASK_GENERIC_NOTIFY                     	 xTaskGenericNotify
#define RTOS_TASK_GENERIC_NOTIFY_STATE_CLEAR         	 xTaskGenericNotifyStateClear
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
#define RTOS_TASK_GET_RUN_TIME_STATS                 	 vTaskGetRunTimeStats
#define RTOS_TASK_GET_SCHEDULER_STATE                	 xTaskGetSchedulerState
#define RTOS_TASK_GET_STATE                          	 eTaskGetState
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
/* from task.h */
#define RTOS_TASK_NOTIFY                             	 xTaskNotify
#define RTOS_TASK_NOTIFY_AND_QUERY                   	 xTaskNotifyAndQuery
#define RTOS_TASK_NOTIFY_AND_QUERY_FROM_ISR          	 xTaskNotifyAndQueryFromISR
#define RTOS_TASK_NOTIFY_AND_QUERY_INDEXED           	 xTaskNotifyAndQueryIndexed
#define RTOS_TASK_NOTIFY_AND_QUERY_INDEXED_FROM_ISR  	 xTaskNotifyAndQueryIndexedFromISR
#define RTOS_TASK_NOTIFY_FROM_ISR                    	 xTaskNotifyFromISR
#define RTOS_TASK_NOTIFY_GIVE                        	 xTaskNotifyGive
#define RTOS_TASK_NOTIFY_GIVE_FROM_ISR               	 vTaskNotifyGiveFromISR
#define RTOS_TASK_NOTIFY_GIVE_INDEXED                	 xTaskNotifyGiveIndexed
#define RTOS_TASK_NOTIFY_GIVE_INDEXED_FROM_ISR       	 vTaskNotifyGiveIndexedFromISR
#define RTOS_TASK_NOTIFY_INDEXED                     	 xTaskNotifyIndexed
#define RTOS_TASK_NOTIFY_INDEXED_FROM_ISR            	 xTaskNotifyIndexedFromISR
#define RTOS_TASK_NOTIFY_STATE_CLEAR                 	 xTaskNotifyStateClear
#define RTOS_TASK_NOTIFY_STATE_CLEAR_INDEXED         	 xTaskNotifyStateClearIndexed
#define RTOS_TASK_NOTIFY_WAIT                        	 xTaskNotifyWait
#define RTOS_TASK_NOTIFY_WAIT_INDEXED                	 xTaskNotifyWaitIndexed
/* from FreeRTOS.h */
#define RTOS_TASK_PARAMETERS                         	 xTaskParameters
/* from mpu_wrappers.h */
#define RTOS_TASK_PRIORITY_SET                       	 vTaskPrioritySet
/* from task.h */
#define RTOS_TASK_RESTORE_INTERRUPTS                 	 taskRESTORE_INTERRUPTS
/* from mpu_wrappers.h */
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
/* from task.h */
#define RTOS_TASK_VALID_CORE_ID                      	 taskVALID_CORE_ID
#define RTOS_TASK_YIELD                              	 taskYIELD
/* from timers.h */
#define RTOS_TIMERS_H                                	 TIMERS_H
#define RTOS_TIMER_CHANGE_PERIOD                     	 xTimerChangePeriod
#define RTOS_TIMER_CHANGE_PERIOD_FROM_ISR            	 xTimerChangePeriodFromISR
/* from mpu_wrappers.h */
#define RTOS_TIMER_CREATE                            	 xTimerCreate
#define RTOS_TIMER_CREATE_STATIC                     	 xTimerCreateStatic
/* from timers.h */
#define RTOS_TIMER_DELETE                            	 xTimerDelete
#define RTOS_TIMER_GENERIC_COMMAND                   	 xTimerGenericCommand
/* from mpu_wrappers.h */
#define RTOS_TIMER_GET_EXPIRY_TIME                   	 xTimerGetExpiryTime
#define RTOS_TIMER_GET_NAME                          	 pcTimerGetName
#define RTOS_TIMER_GET_PERIOD                        	 xTimerGetPeriod
#define RTOS_TIMER_GET_TIMER_DAEMON_TASK_HANDLE      	 xTimerGetTimerDaemonTaskHandle
/* from FreeRTOS.h */
#define RTOS_TIMER_GET_TIMER_NAME                    	 pcTimerGetTimerName
#define RTOS_TIMER_HANDLE                            	 xTimerHandle
/* from mpu_wrappers.h */
#define RTOS_TIMER_IS_TIMER_ACTIVE                   	 xTimerIsTimerActive
#define RTOS_TIMER_PEND_FUNCTION_CALL                	 xTimerPendFunctionCall
/* from timers.h */
#define RTOS_TIMER_RESET                             	 xTimerReset
#define RTOS_TIMER_RESET_FROM_ISR                    	 xTimerResetFromISR
/* from mpu_wrappers.h */
#define RTOS_TIMER_SET_RELOAD_MODE                   	 vTimerSetReloadMode
#define RTOS_TIMER_SET_TIMER_ID                      	 vTimerSetTimerID
/* from timers.h */
#define RTOS_TIMER_START                             	 xTimerStart
#define RTOS_TIMER_START_FROM_ISR                    	 xTimerStartFromISR
#define RTOS_TIMER_STOP                              	 xTimerStop
#define RTOS_TIMER_STOP_FROM_ISR                     	 xTimerStopFromISR
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
#define RTOS_TRACE_BLOCKING_ON_QUEUE_PEEK            	 traceBLOCKING_ON_QUEUE_PEEK
#define RTOS_TRACE_BLOCKING_ON_QUEUE_RECEIVE         	 traceBLOCKING_ON_QUEUE_RECEIVE
#define RTOS_TRACE_BLOCKING_ON_QUEUE_SEND            	 traceBLOCKING_ON_QUEUE_SEND
#define RTOS_TRACE_BLOCKING_ON_STREAM_BUFFER_RECEIVE 	 traceBLOCKING_ON_STREAM_BUFFER_RECEIVE
#define RTOS_TRACE_BLOCKING_ON_STREAM_BUFFER_SEND    	 traceBLOCKING_ON_STREAM_BUFFER_SEND
#define RTOS_TRACE_CREATE_COUNTING_SEMAPHORE         	 traceCREATE_COUNTING_SEMAPHORE
#define RTOS_TRACE_CREATE_COUNTING_SEMAPHORE_FAILED  	 traceCREATE_COUNTING_SEMAPHORE_FAILED
#define RTOS_TRACE_CREATE_MUTEX                      	 traceCREATE_MUTEX
#define RTOS_TRACE_CREATE_MUTEX_FAILED               	 traceCREATE_MUTEX_FAILED
#define RTOS_TRACE_END                               	 traceEND
#define RTOS_TRACE_EVENT_GROUP_CLEAR_BITS            	 traceEVENT_GROUP_CLEAR_BITS
#define RTOS_TRACE_EVENT_GROUP_CLEAR_BITS_FROM_ISR   	 traceEVENT_GROUP_CLEAR_BITS_FROM_ISR
#define RTOS_TRACE_EVENT_GROUP_CREATE                	 traceEVENT_GROUP_CREATE
#define RTOS_TRACE_EVENT_GROUP_CREATE_FAILED         	 traceEVENT_GROUP_CREATE_FAILED
#define RTOS_TRACE_EVENT_GROUP_DELETE                	 traceEVENT_GROUP_DELETE
#define RTOS_TRACE_EVENT_GROUP_SET_BITS              	 traceEVENT_GROUP_SET_BITS
#define RTOS_TRACE_EVENT_GROUP_SET_BITS_FROM_ISR     	 traceEVENT_GROUP_SET_BITS_FROM_ISR
#define RTOS_TRACE_EVENT_GROUP_SYNC_BLOCK            	 traceEVENT_GROUP_SYNC_BLOCK
#define RTOS_TRACE_EVENT_GROUP_SYNC_END              	 traceEVENT_GROUP_SYNC_END
#define RTOS_TRACE_EVENT_GROUP_WAIT_BITS_BLOCK       	 traceEVENT_GROUP_WAIT_BITS_BLOCK
#define RTOS_TRACE_EVENT_GROUP_WAIT_BITS_END         	 traceEVENT_GROUP_WAIT_BITS_END
#define RTOS_TRACE_FREE                              	 traceFREE
#define RTOS_TRACE_GIVE_MUTEX_RECURSIVE              	 traceGIVE_MUTEX_RECURSIVE
#define RTOS_TRACE_GIVE_MUTEX_RECURSIVE_FAILED       	 traceGIVE_MUTEX_RECURSIVE_FAILED
#define RTOS_TRACE_INCREASE_TICK_COUNT               	 traceINCREASE_TICK_COUNT
#define RTOS_TRACE_LOW_POWER_IDLE_BEGIN              	 traceLOW_POWER_IDLE_BEGIN
#define RTOS_TRACE_LOW_POWER_IDLE_END                	 traceLOW_POWER_IDLE_END
#define RTOS_TRACE_MALLOC                            	 traceMALLOC
#define RTOS_TRACE_MOVED_TASK_TO_READY_STATE         	 traceMOVED_TASK_TO_READY_STATE
#define RTOS_TRACE_PEND_FUNC_CALL                    	 tracePEND_FUNC_CALL
#define RTOS_TRACE_PEND_FUNC_CALL_FROM_ISR           	 tracePEND_FUNC_CALL_FROM_ISR
#define RTOS_TRACE_POST_MOVED_TASK_TO_READY_STATE    	 tracePOST_MOVED_TASK_TO_READY_STATE
#define RTOS_TRACE_QUEUE_CREATE                      	 traceQUEUE_CREATE
#define RTOS_TRACE_QUEUE_CREATE_FAILED               	 traceQUEUE_CREATE_FAILED
#define RTOS_TRACE_QUEUE_DELETE                      	 traceQUEUE_DELETE
#define RTOS_TRACE_QUEUE_PEEK                        	 traceQUEUE_PEEK
#define RTOS_TRACE_QUEUE_PEEK_FAILED                 	 traceQUEUE_PEEK_FAILED
#define RTOS_TRACE_QUEUE_PEEK_FROM_ISR               	 traceQUEUE_PEEK_FROM_ISR
#define RTOS_TRACE_QUEUE_PEEK_FROM_ISR_FAILED        	 traceQUEUE_PEEK_FROM_ISR_FAILED
#define RTOS_TRACE_QUEUE_RECEIVE                     	 traceQUEUE_RECEIVE
#define RTOS_TRACE_QUEUE_RECEIVE_FAILED              	 traceQUEUE_RECEIVE_FAILED
#define RTOS_TRACE_QUEUE_RECEIVE_FROM_ISR            	 traceQUEUE_RECEIVE_FROM_ISR
#define RTOS_TRACE_QUEUE_RECEIVE_FROM_ISR_FAILED     	 traceQUEUE_RECEIVE_FROM_ISR_FAILED
#define RTOS_TRACE_QUEUE_REGISTRY_ADD                	 traceQUEUE_REGISTRY_ADD
#define RTOS_TRACE_QUEUE_SEND                        	 traceQUEUE_SEND
#define RTOS_TRACE_QUEUE_SEND_FAILED                 	 traceQUEUE_SEND_FAILED
#define RTOS_TRACE_QUEUE_SEND_FROM_ISR               	 traceQUEUE_SEND_FROM_ISR
#define RTOS_TRACE_QUEUE_SEND_FROM_ISR_FAILED        	 traceQUEUE_SEND_FROM_ISR_FAILED
#define RTOS_TRACE_QUEUE_SET_SEND                    	 traceQUEUE_SET_SEND
#define RTOS_TRACE_START                             	 traceSTART
#define RTOS_TRACE_STREAM_BUFFER_CREATE              	 traceSTREAM_BUFFER_CREATE
#define RTOS_TRACE_STREAM_BUFFER_CREATE_FAILED       	 traceSTREAM_BUFFER_CREATE_FAILED
#define RTOS_TRACE_STREAM_BUFFER_CREATE_STATIC_FAILED 	 traceSTREAM_BUFFER_CREATE_STATIC_FAILED
#define RTOS_TRACE_STREAM_BUFFER_DELETE              	 traceSTREAM_BUFFER_DELETE
#define RTOS_TRACE_STREAM_BUFFER_RECEIVE             	 traceSTREAM_BUFFER_RECEIVE
#define RTOS_TRACE_STREAM_BUFFER_RECEIVE_FAILED      	 traceSTREAM_BUFFER_RECEIVE_FAILED
#define RTOS_TRACE_STREAM_BUFFER_RECEIVE_FROM_ISR    	 traceSTREAM_BUFFER_RECEIVE_FROM_ISR
#define RTOS_TRACE_STREAM_BUFFER_RESET               	 traceSTREAM_BUFFER_RESET
#define RTOS_TRACE_STREAM_BUFFER_SEND                	 traceSTREAM_BUFFER_SEND
#define RTOS_TRACE_STREAM_BUFFER_SEND_FAILED         	 traceSTREAM_BUFFER_SEND_FAILED
#define RTOS_TRACE_STREAM_BUFFER_SEND_FROM_ISR       	 traceSTREAM_BUFFER_SEND_FROM_ISR
#define RTOS_TRACE_TAKE_MUTEX_RECURSIVE              	 traceTAKE_MUTEX_RECURSIVE
#define RTOS_TRACE_TAKE_MUTEX_RECURSIVE_FAILED       	 traceTAKE_MUTEX_RECURSIVE_FAILED
#define RTOS_TRACE_TASK_CREATE                       	 traceTASK_CREATE
#define RTOS_TRACE_TASK_CREATE_FAILED                	 traceTASK_CREATE_FAILED
#define RTOS_TRACE_TASK_DELAY                        	 traceTASK_DELAY
#define RTOS_TRACE_TASK_DELAY_UNTIL                  	 traceTASK_DELAY_UNTIL
#define RTOS_TRACE_TASK_DELETE                       	 traceTASK_DELETE
#define RTOS_TRACE_TASK_INCREMENT_TICK               	 traceTASK_INCREMENT_TICK
#define RTOS_TRACE_TASK_NOTIFY                       	 traceTASK_NOTIFY
#define RTOS_TRACE_TASK_NOTIFY_FROM_ISR              	 traceTASK_NOTIFY_FROM_ISR
#define RTOS_TRACE_TASK_NOTIFY_GIVE_FROM_ISR         	 traceTASK_NOTIFY_GIVE_FROM_ISR
#define RTOS_TRACE_TASK_NOTIFY_TAKE                  	 traceTASK_NOTIFY_TAKE
#define RTOS_TRACE_TASK_NOTIFY_TAKE_BLOCK            	 traceTASK_NOTIFY_TAKE_BLOCK
#define RTOS_TRACE_TASK_NOTIFY_WAIT                  	 traceTASK_NOTIFY_WAIT
#define RTOS_TRACE_TASK_NOTIFY_WAIT_BLOCK            	 traceTASK_NOTIFY_WAIT_BLOCK
#define RTOS_TRACE_TASK_PRIORITY_DISINHERIT          	 traceTASK_PRIORITY_DISINHERIT
#define RTOS_TRACE_TASK_PRIORITY_INHERIT             	 traceTASK_PRIORITY_INHERIT
#define RTOS_TRACE_TASK_PRIORITY_SET                 	 traceTASK_PRIORITY_SET
#define RTOS_TRACE_TASK_RESUME                       	 traceTASK_RESUME
#define RTOS_TRACE_TASK_RESUME_FROM_ISR              	 traceTASK_RESUME_FROM_ISR
#define RTOS_TRACE_TASK_SUSPEND                      	 traceTASK_SUSPEND
#define RTOS_TRACE_TASK_SWITCHED_IN                  	 traceTASK_SWITCHED_IN
#define RTOS_TRACE_TASK_SWITCHED_OUT                 	 traceTASK_SWITCHED_OUT
#define RTOS_TRACE_TIMER_COMMAND_RECEIVED            	 traceTIMER_COMMAND_RECEIVED
#define RTOS_TRACE_TIMER_COMMAND_SEND                	 traceTIMER_COMMAND_SEND
#define RTOS_TRACE_TIMER_CREATE                      	 traceTIMER_CREATE
#define RTOS_TRACE_TIMER_CREATE_FAILED               	 traceTIMER_CREATE_FAILED
#define RTOS_TRACE_TIMER_EXPIRED                     	 traceTIMER_EXPIRED
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
/* from mpu_wrappers.h */
#define RTOS_UL_TASK_GENERIC_NOTIFY_TAKE             	 ulTaskGenericNotifyTake
#define RTOS_UL_TASK_GENERIC_NOTIFY_VALUE_CLEAR      	 ulTaskGenericNotifyValueClear
#define RTOS_UL_TASK_GET_IDLE_RUN_TIME_COUNTER       	 ulTaskGetIdleRunTimeCounter
/* from task.h */
#define RTOS_UL_TASK_NOTIFY_TAKE                     	 ulTaskNotifyTake
#define RTOS_UL_TASK_NOTIFY_TAKE_INDEXED             	 ulTaskNotifyTakeIndexed
#define RTOS_UL_TASK_NOTIFY_VALUE_CLEAR              	 ulTaskNotifyValueClear
#define RTOS_UL_TASK_NOTIFY_VALUE_CLEAR_INDEXED      	 ulTaskNotifyValueClearIndexed
/* from mpu_wrappers.h */
#define RTOS_UX_QUEUE_MESSAGES_WAITING               	 uxQueueMessagesWaiting
#define RTOS_UX_QUEUE_SPACES_AVAILABLE               	 uxQueueSpacesAvailable
/* from semphr.h */
#define RTOS_UX_SEMAPHORE_GET_COUNT                  	 uxSemaphoreGetCount
/* from mpu_wrappers.h */
#define RTOS_UX_TASK_GET_NUMBER_OF_TASKS             	 uxTaskGetNumberOfTasks
#define RTOS_UX_TASK_GET_STACK_HIGH_WATER_MARK       	 uxTaskGetStackHighWaterMark
#define RTOS_UX_TASK_GET_STACK_HIGH_WATER_MARK2      	 uxTaskGetStackHighWaterMark2
#define RTOS_UX_TASK_GET_SYSTEM_STATE                	 uxTaskGetSystemState
#define RTOS_UX_TASK_PRIORITY_GET                    	 uxTaskPriorityGet
#define RTOS_UX_TIMER_GET_RELOAD_MODE                	 uxTimerGetReloadMode


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
#define rtos___assert                                	 __assert
#define rtos___assert_fail                           	 __assert_fail
#define rtos___assert_perror_fail                    	 __assert_perror_fail
#define rtos_application_stack_overflow_hook         	 vApplicationStackOverflowHook
#define rtos_application_tick_hook                   	 vApplicationTickHook
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
#define rtos_queue_add_to_registry                   	 vQueueAddToRegistry
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
#define rtos_queue_get_name                          	 pcQueueGetName
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
#define rtos_queue_unregister_queue                  	 vQueueUnregisterQueue
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
