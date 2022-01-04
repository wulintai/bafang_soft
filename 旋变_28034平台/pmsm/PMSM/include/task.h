#ifndef _TASK_H_
#define _TASK_H_

#define TASK_DISABLE				0
#define TASK_ENABLE					1
#define TASK_CLEAR_FLAG				0
#define TASK_SET_FLAG				1

#define TASK500US		            0
#define TASK1MS		                1
#define TASK2MS                     2
#define TASK5MS		                3
#define TASK10MS		            4
#define TASK20MS		            5
#define TASK50MS		            6
#define TASK100MS		            7
#define TASK500MS                   8
#define TASK1000MS		            9
#define TASK_NUM                    10

typedef void (*TASK_FUNC_PTR)(void);

#define PWM_SYS_FRE				(_iq)(30000000)

typedef struct
{
	Uint16 task_cnt;
	Uint16 task_flag;
	Uint16 task_period;
	Uint16 task_enable;
	TASK_FUNC_PTR task_func;
}TASK_STRUCT,* TASK_STRUCT_PTR;

typedef union
{
	Uint16 wValue;
	struct
	{
		Uint16 DI1			    :1;//加速器开关
		Uint16 DI2			    :1;//三速1
		Uint16 DI3			    :1;//三速3
		Uint16 DI4				:1;//巡航
		Uint16 DI5				:1;//后退信号
		Uint16 DI6				:1;//前进信号
		Uint16 DI7	            :1;//高低速模式选择
		Uint16 DI8			    :1;//刹车信号

		Uint16 DI9              :1;
		Uint16 DI10             :1;
		Uint16 DI11             :1;
		Uint16 reserved			:5;//预留
	}data1;
	struct
	{
		Uint16 DI1_WAKEUP_DSP   :1;//输入口1
		Uint16 DI2_BRRAK_DSP	:1;//输入口2
		Uint16 DI3_DSP			:1;//输入口3
		Uint16 DI4_DSP			:1;//油门
		Uint16 DI5_DSP			:1;//睡眠
		Uint16 DI6_DSP		    :1;//刹车
		Uint16 DI7_DSP	        :1;//备用
		Uint16 DI8_DSP			:1;//后退档

		Uint16 reserved1        :1;//前进档
		Uint16 LOT              :1;//
		Uint16 DOS              :1;//
		Uint16 reserved2		:5;//预留
	}data;
}VEHICLE_IO_STATE;

/*typedef union
{
	Uint16 wValue;
	struct
	{
		Uint16 LOT			    :1;
		Uint16 DOS			    :1;
		Uint16 reserved1        :6;
		
		Uint16 reserved2		:8;//预留
	}data;
	struct
	{
		Uint16 resolver_code	:2;
		Uint16 reserved1        :6;
		
		Uint16 reserved2		:8;//预留
	}data1;
}VEHICLE_RESOLVER_STATE;*/

#ifdef _TASK_C_
	TASK_STRUCT task_struct[TASK_NUM];
	VEHICLE_IO_STATE vehicle_io_state_new = {0};
	VEHICLE_IO_STATE vehicle_io_state_old = {0};
	VEHICLE_IO_STATE vehicle_io_state = {0};
	//VEHICLE_RESOLVER_STATE vehicle_resolver_new_status = {0};
	Uint16 vehicle_io_state_count[8] = {0};
	Uint16 vehicle_resolver_sample_count = 0;
	Uint16 vehicle_resolver_fault_count = 0;
	/*_iq PWM_Period_lowspeed = 0;
	_iq PWM_Period_highspeed = 0;
	_iq PWM_Period_time = 0;
	Uint16 PWM_Period_low = 0;
	Uint16 PWM_Period_high = 0;
	Uint16 PWM_Period = 0;
	Uint16 OC_PWM_cnt = 0;
	Uint16 OC_PWM_fault_cnt = 0;*/
	void taskSchedule(void);
	void taskInit(void);
	void task500usCtrl(void);
	void task1msCtrl(void);
	void task2msCtrl(void);
	void task5msCtrl(void);
	void task10msCtrl(void);
	void task20msCtrl(void);
	void task50msCtrl(void);
	void task100msCtrl(void);
	void task500msCtrl(void);
	void task1000msCtrl(void);
	void taskGetVehicleIoState(void);
	void hmi_display(void);
#else
	extern TASK_STRUCT task_struct[TASK_NUM];
	extern VEHICLE_IO_STATE vehicle_io_state;
	extern VEHICLE_IO_STATE vehicle_io_state_new;
	extern VEHICLE_IO_STATE vehicle_io_state_old;
	//extern VEHICLE_RESOLVER_STATE vehicle_resolver_new_status;
	extern Uint16 vehicle_resolver_sample_count;
	extern Uint16 vehicle_resolver_fault_count;
	/*extern _iq PWM_Period_lowspeed;
	extern _iq PWM_Period_highspeed;
	extern Uint16 PWM_Period_low;
	extern Uint16 PWM_Period_high;
	extern Uint16 PWM_Period;
	extern Uint16 OC_PWM_cnt;
	extern Uint16 OC_PWM_fault_cnt;
	extern _iq PWM_Period_time;*/
	extern void taskSchedule(void);
	extern void taskInit(void);
	extern void task500usCtrl(void);
	extern void task1msCtrl(void);
	extern void task2msCtrl(void);
	extern void task5msCtrl(void);
	extern void task10msCtrl(void);
	extern void task20msCtrl(void);
	extern void task50msCtrl(void);
	extern void task100msCtrl(void);
	extern void task500msCtrl(void);
	extern void task1000msCtrl(void);
	extern void taskGetVehicleIoState(void);
	extern void hmi_display(void);
#endif

#endif
