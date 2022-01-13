/*
 * ctrlsysfuntion.c
 *
 *  Created on: 2014-11-14
 *      Author: ThinkPad
 */

#ifndef _SYSCTRLFUN_C_
#define _SYSCTRLFUN_C_

#include "include_c.h"

void errordisplay(void)
{
	/*Uint16 i = 0;
	Uint16 temp = 0;
	
	error_toltalvalue                   =alarm.wValue;
	error_toltalvalue                   =error_toltalvalue<<16;
	error_toltalvalue                   =(error_toltalvalue|emsw2.wValue)<<16;
	error_toltalvalue                   =(error_toltalvalue|emsw1.wValue)<<16;
	error_toltalvalue                   =error_toltalvalue|emsw.wValue;
	if(0 != error_toltalvalue)
	{
		for(i=0;i<64;i++)
		{
			temp =(error_toltalvalue>>i)&0x01;
			if(1 == temp)
			{
				error_display = i;
				if((error_display != error_display_old)
				 ||(ALARM_BUZZER_INIT_STATE == error_alarm_state))
				{
					error_display_old = error_display;
					if(error_display<10)
					{
						error_alarm_state = ALARM_BUZZER_SHORT_ALARM_STATE;
					}
					else
					{
						error_alarm_state = ALARM_BUZZER_LONG_ALARM_STATE;
					}
				}
				else
				{
					error_alarm_state = error_alarm_state;
					error_display_old = error_display;
				}
				break;
			}
		}
	}
	else
	{
		error_display = 0;
		error_display_old = 0;
		error_alarm_cnt = 0;
		error_alarm_flag = 0;
		error_alarm_state = ALARM_BUZZER_INIT_STATE;
	}
	switch(error_alarm_state)
	{
		case ALARM_BUZZER_INIT_STATE:
			error_alarm_cnt = 0;
			error_alarm_flag = 0;
			error_alarm_state = error_alarm_state;
			sub_io_buzzer(SUB_IO_BUZZER_OFF);
			break;
		case ALARM_BUZZER_SHORT_ALARM_STATE:
			error_alarm_cnt++;
			if(error_alarm_cnt>(2*(error_display%10)+2))
			{
				error_alarm_cnt = 0;
				error_alarm_flag = 0;
				error_alarm_state = ALARM_BUZZER_DELAY_STATE;
				sub_io_buzzer(SUB_IO_BUZZER_OFF);
			}
			else
			{
				error_alarm_cnt = error_alarm_cnt;
				error_alarm_state = error_alarm_state;
				if(0 == error_alarm_flag)
				{
					sub_io_buzzer(SUB_IO_BUZZER_OFF);
					error_alarm_flag = 1;
				}
				else
				{
					sub_io_buzzer(SUB_IO_BUZZER_ON);
					error_alarm_flag = 0;
				}
			}
			break;
		case ALARM_BUZZER_LONG_ALARM_STATE:
			error_alarm_cnt++;
			if(error_alarm_cnt>=3*(error_display/10))
			{
				error_alarm_cnt = 0;
				error_alarm_flag = 0;
				error_alarm_state = ALARM_BUZZER_SHORT_ALARM_STATE;
				sub_io_buzzer(SUB_IO_BUZZER_OFF);
			}
			else if(0 == error_alarm_cnt%3)
			{
				error_alarm_cnt = error_alarm_cnt;
				error_alarm_flag = error_alarm_flag;
				error_alarm_state = error_alarm_state;
				sub_io_buzzer(SUB_IO_BUZZER_OFF);
			}
			else
			{
				error_alarm_cnt = error_alarm_cnt;
				error_alarm_flag = 0;
				error_alarm_state = error_alarm_state;
				sub_io_buzzer(SUB_IO_BUZZER_ON);
			}
			break;
		case ALARM_BUZZER_DELAY_STATE:
			error_alarm_cnt++;
			if(error_alarm_cnt>=5)
			{
				error_alarm_cnt = 0;
				error_alarm_flag = 0;
				if(error_display<10)
				{
					error_alarm_state = ALARM_BUZZER_SHORT_ALARM_STATE;
				}
				else
				{
					error_alarm_state = ALARM_BUZZER_LONG_ALARM_STATE;
				}
			}
			else
			{
				error_alarm_cnt = error_alarm_cnt;
				error_alarm_flag = error_alarm_flag;
				error_alarm_state = error_alarm_state;
			}
			break;
		default:
			break;
	}*/
}

void zero_stationcond()             //פ�¹���
{
	_iq speed = 0;
	//_iq pre_trq_value = 0;

	speed = sysFbkPara.Speed;
	//pre_trq_value = _IQmpy(_IQ(0.5),SysBase.invtorque)-3000;
	
	if((1 == EV_MCU_Para.field.Zero_Sation_Enable)  //פ��ʹ��
	 &&(1 == scsw2.field.igbt_state))               //���������    ʹ������оƬ
	{
		if(0 == scsw2.field.zero_state)// δפ�µ������
		{
			if((((1 == Vehicle_cmd.cmdmode.data.gear_state)&&(speed<-zero_speed_throad))     //ǰ������£��ٶ����ﵽ��ֵ
			  ||((2 == Vehicle_cmd.cmdmode.data.gear_state)&&(speed > zero_speed_throad)))   //��������£��ٶ���ǰ�ﵽ��ֵ
			 &&((0 == Vehicle_cmd.cmdmode.data.brake_state)&&(0 == alarm.field.gear_fault)&&(0 == Vehicle_cmd.torque_ref)))// &&δɲ��&&פ��ʧ�ܾ���=0&&��������=0
			{
				scsw2.field.zero_state = 1;   //����פ��
				VCU_IOstate.field.zero_disable_flag = 0;
				VCU_IOstate.field.zero_or_start_flag = 0;   //פ������
			}
			else if((1 == Vehicle_cmd.cmdmode.data.gear_swap_trq_flag)  //�ƶ�
				  &&(0 == Vehicle_cmd.cmdmode.data.brake_state)         //δɲ��
				  &&(0 != Vehicle_cmd.torque_ref)                       //�������ء�0
				  &&(0 != Vehicle_cmd.cmdmode.data.gear_state)          //���ǿյ�
				  /*&&(_IQabs(trqLoop_trqRampref)>=pre_trq_value)*/)//ǰ�󻻵���ɲ����������
			{
				scsw2.field.zero_state = 1;  //����פ��
				VCU_IOstate.field.zero_disable_flag = 0;
				VCU_IOstate.field.zero_or_start_flag = 1;   //פ������
			}

			if(1 == Vehicle_cmd.cmdmode.data.gear_swap_zero_flag)   //����ɲ
			{
				if((1 == scsw2.field.zero_state)    //פ��״̬
				 ||((1 == Vehicle_cmd.cmdmode.data.gear_state)&&(speed> 50))   //ǰ��״̬&&�ٶ�>50
				 ||((2 == Vehicle_cmd.cmdmode.data.gear_state)&&(speed<-50))   //����״̬&&�ٶ�<-50
				 ||(0 == Vehicle_cmd.cmdmode.data.gear_state))  //�յ�
				{
					Vehicle_cmd.cmdmode.data.gear_swap_zero_flag = 0;       //����Ҫ��ɲ
					zero_speed_throad = 15;                  //פ���ٶ���ֵ
				}
			}

			if(1 == zero_station_trqCmd_flag)    //ʹ��פ�����ر�־
			{
				if(((0 != Vehicle_cmd.cmdmode.data.gear_state) && (0 != Vehicle_cmd.torque_ref))  //�ǿյ���ǰ������
				 ||(1 == scsw2.field.zero_state))                                                 //����פ��״̬
				{
					zero_station_trqCmd_flag = 0;                                                 //��ʹ��פ������
					Vehicle_cmd.cmdmode.data.zero_station_free_flag = 0;                          //���ػ�
				}
			}
			if(0 != VCU_IOstate.field.exit_zero_add_trq_flag)    //�뿪פ��ʱ��Ҫ��������
			{
				if((0 == Vehicle_cmd.cmdmode.data.gear_state)    //�յ�
				 ||(1 == Vehicle_cmd.cmdmode.data.brake_state)   //��ɲ��
				 ||(1 == scsw2.field.zero_state)                 //פ��
				 ||(0 == Vehicle_cmd.torque_ref))                //����������
				{
					VCU_IOstate.field.exit_zero_add_trq_flag = 0;//�뿪פ��ʱ����Ҫ��������
				}
			}
		}
		else//�˳�פ��
		{
			if(0 == VCU_IOstate.field.zero_or_start_flag)    //��פ������
			{
				if(0 == VCU_IOstate.field.exit_zero_add_trq_flag)  //�뿪פ��ʱ����Ҫ��������
				{
					if((0 != Vehicle_cmd.torque_ref)         //����������
					 &&(0 != Vehicle_cmd.cmdmode.data.gear_state)) //�ǿյ�
					{
						VCU_IOstate.field.exit_zero_add_trq_flag = 1; //�뿪פ��ʱ��Ҫ��������
						zero_station_trqCmd_store = zero_station_trqCmd;   //
					}
				}
				else if(1 == VCU_IOstate.field.exit_zero_add_trq_flag)
				{
					if((0 == Vehicle_cmd.torque_ref)
					 &&(0 != Vehicle_cmd.cmdmode.data.gear_state))
					{
						VCU_IOstate.field.exit_zero_add_trq_flag = 0;
					}
				}
				if((MTR_TRQ_LOOP != sccw1.field.runLoop_mode)
				 ||(0 == Vehicle_cmd.cmdmode.data.gear_state)
				 ||(1 == Vehicle_cmd.cmdmode.data.brake_state)
				 ||((1 == Vehicle_cmd.cmdmode.data.gear_state) &&(((0 != Vehicle_cmd.torque_ref)&&(2 == VCU_IOstate.field.exit_zero_add_trq_flag))||(sysFbkPara.Speed_fbk_HMI>500)))
				 ||((2 == Vehicle_cmd.cmdmode.data.gear_state) &&(((0 != Vehicle_cmd.torque_ref)&&(2 == VCU_IOstate.field.exit_zero_add_trq_flag))||(sysFbkPara.Speed_fbk_HMI<-500)))
				 ||(1 == alarm.field.gear_fault))
				{
					scsw2.field.zero_state = 0;
					zero_station_trqCmd_flag = 1;
					if((MTR_TRQ_LOOP != sccw1.field.runLoop_mode)
					  ||(0 == Vehicle_cmd.cmdmode.data.gear_state)
					  ||(1 == Vehicle_cmd.cmdmode.data.brake_state)
					  ||(1 == alarm.field.gear_fault))
					{
						Vehicle_cmd.cmdmode.data.zero_station_free_flag = 0;
					}
					else if((Vehicle_cmd.cmdmode.data.gear_state==1 &&(((0 != Vehicle_cmd.torque_ref)&&(2 == VCU_IOstate.field.exit_zero_add_trq_flag))||(sysFbkPara.Speed_fbk_HMI>500)))
					      ||(Vehicle_cmd.cmdmode.data.gear_state==2 &&(((0 != Vehicle_cmd.torque_ref)&&(2 == VCU_IOstate.field.exit_zero_add_trq_flag))||(sysFbkPara.Speed_fbk_HMI<-500))))
					{
						if(((1 == Vehicle_cmd.cmdmode.data.gear_state) && (0 != Vehicle_cmd.torque_ref) && (2 == VCU_IOstate.field.exit_zero_add_trq_flag))
					     ||((2 == Vehicle_cmd.cmdmode.data.gear_state) && (0 != Vehicle_cmd.torque_ref) && (2 == VCU_IOstate.field.exit_zero_add_trq_flag)))
						{
							VCU_IOstate.field.exit_zero_add_trq_flag = 3;
							if((1 == Vehicle_cmd.cmdmode.data.gear_state)
							 &&(trqLoop_trqRampref<_IQmpyI32(2,SysBase.invtorque)))
							{
								Vehicle_cmd.cmdmode.data.zero_station_free_flag = 3;
							}
							else if((2 == Vehicle_cmd.cmdmode.data.gear_state)
								  &&(trqLoop_trqRampref>-_IQmpyI32(3,SysBase.invtorque)))
							{
								Vehicle_cmd.cmdmode.data.zero_station_free_flag = 3;
							}
							else
							{
								Vehicle_cmd.cmdmode.data.zero_station_free_flag = 0;
								zero_station_trqCmd_flag = 0;
								zero_station_trqCmd_store = zero_station_trqCmd = 0;
							}
							Vehicle_cmd.cmdmode.data.gear_swap_trq_flag = 0;
						}
						else
						{
							Vehicle_cmd.cmdmode.data.zero_station_free_flag = 2;
						}
					}
					else
					{
						Vehicle_cmd.cmdmode.data.zero_station_free_flag = 1;
					}
				}
			}
			else
			{
				if((0 == Vehicle_cmd.cmdmode.data.gear_swap_trq_flag)
				 ||(1 == Vehicle_cmd.cmdmode.data.brake_state)
				 ||(0 == Vehicle_cmd.torque_ref)
				 ||(0 == Vehicle_cmd.cmdmode.data.gear_state))
				{
					scsw2.field.zero_state = 0;
					VCU_IOstate.field.exit_zero_add_trq_flag = 3;
					zero_station_trqCmd_store = zero_station_trqCmd;
					sysCfgPara.TrqCmd_AIfilter = sysCfgPara.TrqCmd_AI = zero_station_trqCmd;
					
				}
				
			}
		}
	}
	else
	{
		scsw2.field.zero_state = 0;
		zero_station_trqCmd_flag = 0;
		zero_station_trqCmd_store = 0;
		Vehicle_cmd.cmdmode.data.zero_station_free_flag = 0;
		VCU_IOstate.field.exit_zero_add_trq_flag = 0;
		Vehicle_cmd.cmdmode.data.gear_swap_zero_flag = 0;
		VCU_IOstate.field.zero_disable_flag = 0;
		VCU_IOstate.field.zero_or_start_flag = 0;
	}
}

void udcloop_cond(void)
{
	if((_IQtoIQ15(sysFbkPara.Vdc_filter_protect)>EV_MCU_Para.field.Udcloop_UdcUplmt) || (_IQtoIQ15(sysFbkPara.Vdc_filter_protect)<EV_MCU_Para.field.Udcloop_UdcDownlmt))
	{
		scsw2.field.Udcloop_state = 1;
	}
	if(sysFbkPara.Speed_fbk_HMI_abs<EV_MCU_Para.field.Udcloop_Speedmin)
	{
		scsw2.field.Udcloop_state = 0;
	}
}

void speedlooplmtcond()
{
	//_iq factor = 0;
	_iq speed_up = 0;
	_iq speed_lmt = 0;    //�ٶ�����
	_iq speed_exit = 0;   //�뿪���ٵ���ֵ
	_iq tooth_trq = 0;
	_iq tooth_trq_exit = 0;  //����������ֵ
	_iq spd_lmt_up_step = 0;
	_iq spd_lmt_down_step = 0;
	_iq x_value= 0;
	_iq trq_lmt = 0;
	_iq spd_lmt_exit_trq_deta = 0;
	_iq trq_step = 0;
	_iq max_unload_trq_step = 0;
	
	if((1 == EV_MCU_Para.field.Spd_Over_Limt_enable)   //����ʹ��
	 &&(1 == scsw2.field.igbt_state)) //���������
	{
		tooth_trq = _IQ15toIQ(EV_MCU_Para.field.Tooth_trq);//�������أ������������ 2/500��<< 9λ = 2.048
		tooth_trq_exit = tooth_trq+SysBase.invtorque;//����������ֵ 2.048 +1/500 = 2.05
		trq_lmt = _IQmpyI32(10,SysBase.invtorque);// 1/50 = 0.02
		spd_lmt_exit_trq_deta = _IQmpyI32(5,SysBase.invtorque);// 1/100
		trq_step = _IQmpy(_IQ(0.01),SysBase.invtorque);// 1/50000
		max_unload_trq_step = _IQmpy(_IQ(0.03),SysBase.invtorque);  // 3/50000
		
		if(1 == VCU_IOstate.field.In_fwdrev)//��ǰ�ĵ�λΪD��  ǰ����    1 = D  2 = R  0 = N
		{
			if(1 == vehicle_io_state.data.DI6_DSP)//ɲ��\���ٵ�  δ��ֵ
			{
				speed_up = EV_MCU_Para.field.Spd_FWD_L_Limt_Spd-_IQ15div(300,SysBase.speed); //(L��ǰ���ٶ����� 4500/12000) - 300/12000  = 4200/12000 = 0.35
				speed_exit = EV_MCU_Para.field.Spd_FWD_L_Limt_Spd-_IQ15div(1000,SysBase.speed);//(L��ǰ���ٶ����� 4500/12000) - 1000/12000  = 3500/12000 �� 0.292
				if(speed_up<0)//N
				{
					speed_up = EV_MCU_Para.field.Spd_FWD_L_Limt_Spd;
				}
				if(speed_exit<0)//N
				{
					speed_exit = 0;
				}
				speed_lmt = EV_MCU_Para.field.Spd_FWD_L_Limt_Spd;// 4500/12000 = 0.375 = 3/8
			}
			else//���ٵ�
			{
				speed_up = EV_MCU_Para.field.Spd_FWD_Limt_Spd-_IQ15div(300,SysBase.speed); //(ǰ���ٶ����� 4500/12000) - 300/12000  = 4200/12000 = 0.35
				speed_exit = EV_MCU_Para.field.Spd_FWD_Limt_Spd-_IQ15div(1000,SysBase.speed);//(ǰ���ٶ����� 4500/12000) - 1000/12000  = 3500/12000 �� 0.292
				if(speed_up<0)
				{
					speed_up = EV_MCU_Para.field.Spd_FWD_Limt_Spd;// 4500/12000 = 0.375 = 3/8
				}
				if(speed_exit<0)
				{
					speed_exit = 0;
				}
				speed_lmt = EV_MCU_Para.field.Spd_FWD_Limt_Spd;// 4500/12000 = 0.375 = 3/8
			}
			
		}
		else if(2 == VCU_IOstate.field.In_fwdrev)//���˵�
		{
			if(1 == vehicle_io_state.data.DI6_DSP)//ɲ��\���ٵ�  δ��ֵ
			{
				speed_up = (-EV_MCU_Para.field.Spd_FWD_E_Limt_Spd)+_IQ15div(300,SysBase.speed);// -(E��ǰ���ٶ����� 1600/12000) + 300/12000 = -1300/12000 �� -0.108
				speed_exit = (-EV_MCU_Para.field.Spd_FWD_E_Limt_Spd)+_IQ15div(600,SysBase.speed);// -(E��ǰ���ٶ����� 1600/12000) + 600/12000 = -1000/12000 �� -0.083
				if(speed_up>0)//N
				{
					speed_up = 0-EV_MCU_Para.field.Spd_FWD_E_Limt_Spd;
				}
				if(speed_exit>0)//Y
				{
					speed_exit = 0;
				}
				speed_lmt = 0-EV_MCU_Para.field.Spd_FWD_E_Limt_Spd;//  -(E��ǰ���ٶ����� 1600/12000) �� -0.133
			}
			else//���ٵ�
			{
				speed_up = (-EV_MCU_Para.field.Spd_REV_Limt_Spd)+_IQ15div(300,SysBase.speed);// -(�����ٶ����� 750/12000) + 300/12000  = -450/12000 = -0.0875 = -7/80
				speed_exit = (-EV_MCU_Para.field.Spd_REV_Limt_Spd)+_IQ15div(1200,SysBase.speed);// -(�����ٶ����� 750/12000) + 1200/12000 = 450/12000 = 0.0875 = 7/80
				if(speed_up>0)//N
				{
					speed_up = 0-EV_MCU_Para.field.Spd_REV_Limt_Spd;
				}
				if(speed_exit>0)//Y
				{
					speed_exit = 0;
				}
				speed_lmt = 0-EV_MCU_Para.field.Spd_REV_Limt_Spd;//  -(�����ٶ����� 750/12000) = -0.0625 = -5/80
			}
		}
		else // �յ�
		{
			speed_up = (-EV_MCU_Para.field.Spd_REV_Limt_Spd)+_IQ15div(300,SysBase.speed);// -(�����ٶ����� 750/12000) + 300/12000 = -450/12000 = -0.0875 = -7/80
			speed_exit = (-EV_MCU_Para.field.Spd_REV_Limt_Spd)+_IQ15div(1200,SysBase.speed);// -(�����ٶ����� 750/12000) + 1200/12000 = 450/12000 = 0.0875 = 7/80
			if(speed_up>0)//N
			{
				speed_up = 0-EV_MCU_Para.field.Spd_REV_Limt_Spd;
			}
			if(speed_exit>0)//Y
			{
				speed_exit = 0;
			}
			speed_lmt = 0-EV_MCU_Para.field.Spd_REV_Limt_Spd;//  -(�����ٶ����� 750/12000) = -0.0625 = -5/80
		}

		//�ٶ�С��һ��ֵ�����VCU���ٱ�־
		if(1 == VCU_IOstate.field.limit_speed_flag)// VCU���ٱ�־
		{
			if(1 == Vehicle_cmd.cmdmode.data.gear_state)//ǰ���� - ȡ��VCU_IOstate.field.In_fwdrev��ֵ
			{
				if(sysFbkPara.Speed_fbk_HMI<speed_lmt+_IQ15div(50,SysBase.speed))//(�˲�����ٶȷ���/˲ʱ�ٶ�)<�ٶ�����(0.375 4500rpm)+ (50/12000��0.0042) = 0.3792
				{
					VCU_IOstate.field.limit_speed_flag = 0;// ���VCU���ٱ�־
				}
			}
			else//���˵� - ȡ��VCU_IOstate.field.In_fwdrev��ֵ
			{
				if(sysFbkPara.Speed_fbk_HMI>speed_lmt-_IQ15div(50,SysBase.speed))//(�˲�����ٶȷ���/˲ʱ�ٶ�)> �ٶ�����(-0.0625 -750rpm) - (50/12000��0.0042) = -0.0067
				{
					VCU_IOstate.field.limit_speed_flag = 0;// ���VCU���ٱ�־
				}
			}
		}
		else//�ٶȴ���һ��ֵ����λ���ٱ�־
		{
			if(1 == Vehicle_cmd.cmdmode.data.gear_state)//ǰ���� - ȡ��VCU_IOstate.field.In_fwdrev��ֵ
			{
				if(sysFbkPara.Speed_fbk_HMI>speed_lmt+_IQ15div(100,SysBase.speed))//(�˲�����ٶȷ���/˲ʱ�ٶ�)>�ٶ�����(0.375 4500rpm)+ (50/12000��0.0042) = 0.3792
				{
					VCU_IOstate.field.limit_speed_flag = 1;//��λVCU���ٱ�־
				}
			}
			else//���˵� - ȡ��VCU_IOstate.field.In_fwdrev��ֵ
			{
				if(sysFbkPara.Speed_fbk_HMI<speed_lmt-_IQ15div(100,SysBase.speed))//(�˲�����ٶȷ���/˲ʱ�ٶ�)< �ٶ�����(-0.0625 -750rpm) - (50/12000��0.0042) = -0.0067
				{
					VCU_IOstate.field.limit_speed_flag = 1;//��λVCU���ٱ�־
				}
			}
		}
		

		if(0 == scsw2.field.spdlmt_flag) //  scsw2���ٱ�־Ϊ0
		{
			if(0 != scsw2.field.SpdLmt_state) //������           0:������    1����������   2����������
			{
				if(1 == scsw2.field.SpdLmt_state)           //��������״̬
				{
					if(((sysCfgPara.TrqCmd_AIfilter+spd_lmt_exit_trq_deta)<spdLoopPara.TrqCmd_real)//(����+10Nm(0.01))���ٶȱջ�����
					 ||((_IQabs(sysCfgPara.TrqCmd_AIfilter)<=tooth_trq_exit)&&(0 == Vehicle_cmd.torque_ref))//��������<����������ֵ&&��������=0
					 ||(sysFbkPara.Speed_fbk_HMI<speed_exit))// ת�� < �뿪���ٵ���ֵ = 0.292 3500rpm
					{
						scsw2.field.SpdLmt_state    					=  0;//����״̬��Ϊ������
						spdlmt_cmd                                      =  0;//���ٿ�������
					}
				}
				else                                        //��������״̬
				{
					if(((sysCfgPara.TrqCmd_AIfilter-spd_lmt_exit_trq_deta)>spdLoopPara.TrqCmd_real)//(����-10Nm(0.01))>�ٶȱջ�����
					 ||((_IQabs(sysCfgPara.TrqCmd_AIfilter)<=tooth_trq_exit)&&(0 == Vehicle_cmd.torque_ref))//��������<����������ֵ&&��������=0
					 ||(sysFbkPara.Speed_fbk_HMI>speed_exit))// ת�� > �뿪���ٵ���ֵ = -0.0625  -1950rpm
					{
						scsw2.field.SpdLmt_state    					=  0;//����״̬��Ϊ������
						spdlmt_cmd                                      = 0;//���ٿ�������
					}
				}
			}
			else //������
			{
				if((1 == Vehicle_cmd.cmdmode.data.gear_state)  //ǰ���� - ȡ��VCU_IOstate.field.In_fwdrev��ֵ
				  &&(sysFbkPara.Speed_fbk_HMI>speed_up)//(�˲�����ٶȷ���/˲ʱ�ٶ�)>�����ٶ�(0.35 4200rpm)
				  &&(0 == VCU_IOstate.field.limit_speed_flag)//���ٱ�־Ϊ0/δ����
				  &&(0 != Vehicle_cmd.torque_ref))//�������ء�0
				{
					scsw2.field.spdlmt_flag                         =  1; //��������
					spdlmt_trq_filter = spdlmt_trq                                      =  trqLoop_trqRampref;
			      //����Ť���˲�          ����Ť��                                                Ť��ָ��
				}
				else if((2 == Vehicle_cmd.cmdmode.data.gear_state) //���˵� - ȡ��VCU_IOstate.field.In_fwdrev��ֵ
					  &&(sysFbkPara.Speed_fbk_HMI<speed_up)//(�˲�����ٶȷ���/˲ʱ�ٶ�)<�����ٶ�(-0.0875 -1050rpm)
					  &&(0 == VCU_IOstate.field.limit_speed_flag)//���ٱ�־Ϊ0/δ����
					  &&(0 != Vehicle_cmd.torque_ref))//�������ء�0
				{
					scsw2.field.spdlmt_flag                         =  2; //��������
					spdlmt_trq_filter = spdlmt_trq                                      =  trqLoop_trqRampref;
				   //����Ť���˲�          ����Ť��                                                Ť��ָ��
				}
			}
		}
		else //scsw2���ٱ�־Ϊ��0
		{
			if(1 == scsw2.field.spdlmt_flag)  //�������ٱ�־
			{
				if(spdlmt_trq>tooth_trq)//����Ť�� > ����Ť�� = 2.048
				{
					if(spdlmt_trq<=trq_lmt) //����Ť�� <= Ť������ = 0.02
					{
						x_value = _IQabs(spdlmt_trq); //����Ť�صľ���ֵ
						x_value = -_IQmpy(_IQ(31.25),x_value)+_IQ(0.625); // 0.625-(����Ť�صľ���ֵ*31.25)
						spd_lmt_down_step = -_IQmpy(_IQ(0.00008),x_value)+_IQmpy(_IQ(0.03),SysBase.invtorque);//����Ť���½��Ĳ���
					}
					else //����Ť�� > Ť������ = 0.02
					{
						spd_lmt_down_step = max_unload_trq_step; //����Ť���½��Ĳ��� = �޸������Ť���½����� = 3/50000
					}
					if(spd_lmt_down_step>max_unload_trq_step)//����Ť���½��Ĳ��� > �޸������Ť���½����� = 3/50000
					{
						spd_lmt_down_step = max_unload_trq_step;//����Ť���½��Ĳ��� = �޸������Ť���½����� = 3/50000
					}
					else if(spd_lmt_down_step<trq_step)//����Ť���½��Ĳ��� < ��СŤ���½����� = 1/50000
					{
						spd_lmt_down_step = trq_step;//����Ť���½��Ĳ��� = ��СŤ���½����� = 1/50000
					}
					
					if((spdlmt_trq-spd_lmt_down_step)>tooth_trq)//����Ť�� - ����Ť���½��Ĳ��� > ����Ť��
					{
						spdlmt_trq = spdlmt_trq-spd_lmt_down_step;//����Ť�� = ����Ť�� - ����Ť���½��Ĳ���
					}
					else//����Ť�� - ����Ť���½��Ĳ��� < ����Ť��
					{
						spdlmt_trq = tooth_trq;//����Ť�� = ����Ť��
					}
				}
				else if(spdlmt_trq<-tooth_trq)//����Ť�� < -����Ť�� = -2.048
				{
					if(spdlmt_trq>=-trq_lmt)//����Ť�� >= -Ť������ = -0.02
					{
						x_value = _IQabs(spdlmt_trq); //����Ť�صľ���ֵ
						x_value = -_IQmpy(_IQ(31.25),x_value)+_IQ(0.625); // 0.625-(����Ť�صľ���ֵ*31.25)
						spd_lmt_up_step = -_IQmpy(_IQ(0.00008),x_value)+_IQmpy(_IQ(0.03),SysBase.invtorque); //����Ť���½��Ĳ���
					}
					else//����Ť�� < -Ť������ = -0.02
					{
						spd_lmt_up_step = max_unload_trq_step; //����Ť���½��Ĳ��� = �޸������Ť���½����� = 3/50000
					}
					if(spd_lmt_up_step>max_unload_trq_step)//����Ť���½��Ĳ��� > �޸������Ť���½����� = 3/50000
					{
						spd_lmt_up_step = max_unload_trq_step; //����Ť���½��Ĳ��� = �޸������Ť���½����� = 3/50000
					}
					else if(spd_lmt_up_step<trq_step)//����Ť���½��Ĳ��� < ��СŤ���½����� = 1/50000
					{
						spd_lmt_up_step = trq_step;//����Ť���½��Ĳ��� = ��СŤ���½����� = 1/50000
					}
					
					if((spdlmt_trq+spd_lmt_up_step)<-tooth_trq)//����Ť�� + ����Ť���½��Ĳ��� < -����Ť��
					{
						spdlmt_trq = spdlmt_trq+spd_lmt_up_step;//����Ť�� = ����Ť�� + ����Ť���½��Ĳ���
					}
					else//����Ť�� + ����Ť���½��Ĳ��� > -����Ť��
					{
						spdlmt_trq = -tooth_trq;//����Ť�� = -����Ť��
					}
				}
				else// -2.048 = -����Ť�� < ����Ť�� < ����Ť�� = 2.048
				{
					spd_lmt_up_step = trq_step; //����Ť���½��Ĳ��� = ��СŤ���½����� = 1/50000
					spdlmt_trq = spdlmt_trq+spd_lmt_up_step;//����Ť�� = ����Ť�� + ����Ť���½��Ĳ���
					if(spdlmt_trq>tooth_trq)//����Ť�� > ����Ť��
					{
						spdlmt_trq = tooth_trq;//����Ť�� = ����Ť��
					}
				}

				if(1 == VCU_IOstate.field.limit_speed_flag)//VCU���ٱ�־Ϊ1 = ����
				{
					spdlmt_trq = tooth_trq;//����Ť�� = ����Ť��
				}

				if(spdlmt_trq == tooth_trq)//����Ť�� = ����Ť��
				{
					scsw2.field.SpdLmt_state = 1;//scsw2����״̬��Ϊ1 ��������
					scsw2.field.spdlmt_flag = 0;//scsw2���ٱ�־����
					spdlmt_cmd = sysFbkPara.Speed_fbk_Filter+_IQdiv(50,SysBase.speed);//���ٿ��� = (�ٶȷ���/ƽ���ٶ�) + (50/12000)
					if(spdlmt_cmd>_IQ15toIQ(speed_lmt))//���ٿ��� > �ٶ�����<<9λ = (0.375*512) = 192
					{
						spdlmt_cmd = _IQ15toIQ(speed_lmt);//���ٿ��� = �ٶ�����<<9λ = (0.375*512) = 192
					}
				}
			}
			else //�������ٱ�־
			{
				if(spdlmt_trq>tooth_trq)//����Ť�� > ����Ť�� = 2.048
				{
					if(spdlmt_trq<=trq_lmt)//����Ť�� <= Ť������ = 0.02
					{
						x_value = _IQabs(spdlmt_trq);//����Ť�صľ���ֵ
						x_value = -_IQmpy(_IQ(62.5),x_value)+_IQ(1.25);// 1.25-(����Ť�صľ���ֵ*62.5)
						spd_lmt_down_step = -_IQmpy(_IQ(0.00008),x_value)+_IQmpy(_IQ(0.03),SysBase.invtorque);//����Ť���½��Ĳ���
					}
					else//����Ť�� > Ť������ = 0.02
					{
						spd_lmt_down_step = max_unload_trq_step;//����Ť���½��Ĳ��� = �޸������Ť���½����� = 3/50000
					}
					if(spd_lmt_down_step>max_unload_trq_step)//����Ť���½��Ĳ��� > �޸������Ť���½����� = 3/50000
					{
						spd_lmt_down_step = max_unload_trq_step;//����Ť���½��Ĳ��� = �޸������Ť���½����� = 3/50000
					}
					else if(spd_lmt_down_step<trq_step)//����Ť���½��Ĳ��� < ��СŤ���½����� = 1/50000
					{
						spd_lmt_down_step = trq_step;//����Ť���½��Ĳ��� = ��СŤ���½����� = 1/50000
					}
					
					if((spdlmt_trq-spd_lmt_down_step)>tooth_trq)//����Ť�� - ����Ť���½��Ĳ��� > ����Ť��
					{
						spdlmt_trq = spdlmt_trq-spd_lmt_down_step;//����Ť�� = ����Ť�� - ����Ť���½��Ĳ���
					}
					else//����Ť�� + ����Ť���½��Ĳ��� > -����Ť��
					{
						spdlmt_trq = tooth_trq;//����Ť�� = ����Ť��
					}
				}
				else if(spdlmt_trq<-tooth_trq)//����Ť�� < -����Ť�� = -2.048
				{
					if(spdlmt_trq>=-trq_lmt)//����Ť�� >= -Ť������ = -0.02
					{
						x_value = _IQabs(spdlmt_trq);//����Ť�صľ���ֵ
						x_value = -_IQmpy(_IQ(62.5),x_value)+_IQ(1.25);// 1.25-(����Ť�صľ���ֵ*62.5)
						spd_lmt_up_step = -_IQmpy(_IQ(0.00036),x_value)+_IQmpy(_IQ(0.19),SysBase.invtorque);//����Ť���½��Ĳ���
					}
					else//����Ť�� < -Ť������ = -0.02
					{
						spd_lmt_up_step = _IQmpy(_IQ(0.19),SysBase.invtorque);//����Ť���½��Ĳ��� = 0.19*1/500 = 19/50000
					}
					if(spd_lmt_up_step>_IQmpy(_IQ(0.19),SysBase.invtorque))//����Ť���½��Ĳ��� > 0.19*1/500 = 19/50000
					{
						spd_lmt_up_step = _IQmpy(_IQ(0.19),SysBase.invtorque);//����Ť���½��Ĳ��� = 0.19*1/500 = 19/50000
					}
					else if(spd_lmt_up_step<trq_step)//����Ť���½��Ĳ��� < ��СŤ���½����� = 1/50000
					{
						spd_lmt_up_step = trq_step;//����Ť���½��Ĳ��� = ��СŤ���½����� = 1/50000
					}
					
					if((spdlmt_trq+spd_lmt_up_step)<-tooth_trq)//����Ť�� + ����Ť���½��Ĳ��� < -����Ť��
					{
						spdlmt_trq = spdlmt_trq+spd_lmt_up_step;//����Ť�� = ����Ť�� + ����Ť���½��Ĳ���
					}
					else//����Ť�� + ����Ť���½��Ĳ��� > -����Ť��
					{
						spdlmt_trq = -tooth_trq;//����Ť�� = -����Ť��
					}
				}
				else
				{
					spd_lmt_down_step = trq_step;  //����Ť���½��Ĳ��� = ��СŤ���½����� = 1/50000
					spdlmt_trq = spdlmt_trq-spd_lmt_down_step;//����Ť�� = ����Ť�� - ����Ť���½��Ĳ���
					if(spdlmt_trq<-tooth_trq)//����Ť��<-����Ť��
					{
						spdlmt_trq = -tooth_trq;//����Ť�� = -����Ť��
					}
				}

				if(1 == VCU_IOstate.field.limit_speed_flag)//VCU���ٱ�־Ϊ1 = ����
				{
					spdlmt_trq = -tooth_trq;//����Ť�� = -����Ť��
				}

				if(spdlmt_trq == -tooth_trq)//����Ť�� = -����Ť��
				{
					scsw2.field.SpdLmt_state = 2;//scsw2����״̬��Ϊ2 ��������
					scsw2.field.spdlmt_flag = 0;//scsw2���ٱ�־����
					spdlmt_cmd = sysFbkPara.Speed_fbk_Filter-_IQdiv(50,SysBase.speed);//���ٿ��� = (�ٶȷ���/ƽ���ٶ�) - (50/12000)
					if(spdlmt_cmd<_IQ15toIQ(speed_lmt))//���ٿ��� < �ٶ�����<<9λ = (0.375*512) = 192
					{
						spdlmt_cmd = _IQ15toIQ(speed_lmt);//���ٿ��� = �ٶ�����<<9λ = (0.375*512) = 192
					}
				}
			}
		}
		
		if(((0 == scsw2.field.igbt_state)//������ع�
		  ||(MTR_TRQ_LOOP != sccw1.field.runLoop_mode)//��Ϊ���ػ�
		  ||(0 == Vehicle_cmd.cmdmode.data.gear_state)) //�յ�
		 &&((0 != scsw2.field.SpdLmt_state)||(0 != scsw2.field.spdlmt_flag)))//scsw2Ϊ����״̬||scsw2���ٱ�־λ��λ
		{
			sysCfgPara.TrqCmd_AIfilter = sysCfgPara.TrqCmd_AI = trqLoop_trqRampref;//�������� = ��ǰ���� = Ť��ָ��
			scsw2.field.SpdLmt_state = 0;//scsw2����״̬����
			scsw2.field.spdlmt_flag = 0;//���scsw2��־λ
		}
	}
	else
	{
		scsw2.field.SpdLmt_state = 0;//scsw2����״̬����
		scsw2.field.spdlmt_flag = 0;//���scsw2��־λ
	}
}

void diff_deal(void)
{
	}

void diff_deal_clear(void)
{
	/*diff_limit_flag = PI_NO_LIMIT;
	scsw2.field.shake_flag = 0;
	diff_spd_err_old = diff_spd_err = sysFbkPara.Speed_fbk_Filter_Lowspeed-sysFbkPara.Speed_fbk;
	diff_trq = diff_trq_ui = diff_trq_filter = 0;*/

	scsw2.field.stop_shake_flag = 0;
	diff_stop_spd_err_old = diff_stop_spd_err = Speed_fbk_Filter_Stop_Lowspeed-sysFbkPara.Speed_fbk;
	diff_stop_trq = diff_stop_trq_ui = diff_stop_trq_filter = 0;
}

void sub_io_yb(Uint16 buzzer_io_state)
{
	switch(buzzer_io_state)
	{
		case SUB_IO_BUZZER_OFF:
			GpioDataRegs.GPACLEAR.bit.GPIO10 = 1;
			break;
		case SUB_IO_BUZZER_ON:
			GpioDataRegs.GPASET.bit.GPIO10 = 1;
			break;
		default:
			GpioDataRegs.GPACLEAR.bit.GPIO10 = 1;
			break;
	}
}

void sub_io_buzzer(Uint16 buzzer_io_state)
{
	switch(buzzer_io_state)
	{
		case SUB_IO_BUZZER_OFF:
			GpioDataRegs.GPACLEAR.bit.GPIO11 = 1;
			break;
		case SUB_IO_BUZZER_ON:
			GpioDataRegs.GPASET.bit.GPIO11 = 1;
			break;
		default:
			GpioDataRegs.GPACLEAR.bit.GPIO11 = 1;
			break;
	}
}

void sub_io_ad2s1205_fs1(Uint16 fan_io_state)
{
	switch(fan_io_state)
	{
		case SUB_IO_LOW:
			GpioDataRegs.GPACLEAR.bit.GPIO6 = 1;
			break;
		case SUB_IO_HIGH:
			GpioDataRegs.GPASET.bit.GPIO6 = 1;
			break;
		default:
			GpioDataRegs.GPACLEAR.bit.GPIO6 = 1;
			break;
	}
}

void sub_io_ad2s1205_fs2(Uint16 fan_io_state)
{
	switch(fan_io_state)
	{
		case SUB_IO_LOW:
			GpioDataRegs.GPACLEAR.bit.GPIO7 = 1;
			break;
		case SUB_IO_HIGH:
			GpioDataRegs.GPASET.bit.GPIO7 = 1;
			break;
		default:
			GpioDataRegs.GPACLEAR.bit.GPIO7 = 1;
			break;
	}
}

void sub_io_ad2s1205_rst(Uint16 fan_io_state)
{
	switch(fan_io_state)
	{
		case SUB_IO_LOW:
			GpioDataRegs.GPACLEAR.bit.GPIO8 = 1;
			break;
		case SUB_IO_HIGH:
			GpioDataRegs.GPASET.bit.GPIO8 = 1;
			break;
		default:
			GpioDataRegs.GPACLEAR.bit.GPIO8 = 1;
			break;
	}
}

void sub_io_relay(Uint16 fan_io_state)
{
	switch(fan_io_state)
	{
		case SUB_IO_LOW:
			GpioDataRegs.GPBCLEAR.bit.GPIO40 = 1;
			break;
		case SUB_IO_HIGH:
			GpioDataRegs.GPBSET.bit.GPIO40 = 1;
			break;
		default:
			GpioDataRegs.GPBSET.bit.GPIO40 = 1;
			break;
	}
}

void sub_io_re_cs(Uint16 fan_io_state)
{
	switch(fan_io_state)
	{
		case SUB_IO_LOW:
			GpioDataRegs.GPBCLEAR.bit.GPIO39 = 1;
			break;
		case SUB_IO_HIGH:
			GpioDataRegs.GPBSET.bit.GPIO39 = 1;
			break;
		default:
			GpioDataRegs.GPBSET.bit.GPIO39 = 1;
			break;
	}
}

#endif

