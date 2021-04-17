/**
 * Task events get defined here for later use.
 * The task bit has to be unique.
 * So that ex 0b101 wont execute events on 0b100 and 0b001.
 * If multiple events should be called they can however be combined.
 */

#define TASK_EVENT_FAN1_CalcRpm 				0b0000000000001
#define TASK_EVENT_FAN2_CalcRpm 				0b0000000000010
#define TASK_EVENT_FAN1_RpmUpdated 				0b0000000000100
#define TASK_EVENT_FAN2_RpmUpdated 				0b0000000001000
#define TASK_EVENT_FAN1_PwmUpdated 				0b0000000010000
#define TASK_EVENT_FAN2_PwmUpdated 				0b0000000100000
#define TASK_EVENT_SENSOR1_TemperatureUpdated 	0b0000001000000
#define TASK_EVENT_SENSOR1_HumidityUpdated 		0b0000010000000
#define TASK_EVENT_SENSOR2_TemperatureUpdated 	0b0000100000000
#define TASK_EVENT_SENSOR2_HumidityUpdated 		0b0001000000000
#define TASK_EVENT_LED1_StateUpdated 			0b0010000000000
#define TASK_EVENT_NEXTION_CompClicked 			0b0100000000000
#define TASK_EVENT_LED2_EffectChanged 			0b1000000000000