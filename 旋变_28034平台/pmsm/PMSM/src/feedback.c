#ifndef _FEEDBACK_C_
#define _FEEDBACK_C_

#include "include_c.h"

void AdcSEQ2(void)
{
	sysFbkPara.AI1_fbk_ad = AdcResult.ADCRESULT6;//����1
	sysFbkPara.AI2_fbk_ad = AdcResult.ADCRESULT7;//����2
	sysFbkPara.Temp_mdl_ad =   AdcResult.ADCRESULT4;//T_MOS_AD 
	sysFbkPara.Temp_mtr_ad =   AdcResult.ADCRESULT3;//TEMP_AD
	sysFbkPara.V12_fbk_ad = AdcResult.ADCRESULT8; //V12_AD
}

void diagIO(void)
{
	_iq speed = 0;
	_iq trq = 0;
	_iq k_tmp = 0;
	_iq tooth_trq = 0;
	_iq x_value = 0;
	_iq trq_lmt = 0;
	_iq trq_deta = 0;
	_iq trq_step = 0;
	_iq trq_n_step_updown = 0;
	_iq trq_step_second = 0;
	_iq trq_step_third = 0;
	//_iq trq_gear_step = 0;
	//_iq pre_trq_value = 0;
	_iq tooth_trq_step = 0;
	_iq max_fwd_up_step = 0;
	_iq max_fwd_down_step = 0;
	_iq max_feedback_up_step = 0;
	_iq max_feedback_down_step = 0;
	_iq min_feedback_up_step = 0;
	_iq min_feedback_down_step = 0;
	_iq max_feedback_enter_down_step = 0;
	_iq gear_n_step = 0;
	_iq max_rev_down_step = 0;
	_iq max_rev_up_step = 0;

	if(TRQCMD_SOURCE_AI == EV_MCU_Para.field.TrqCmd_SourceType)  //����������Դ = 0 AI1
	{
		speed = sysFbkPara.Speed;  //��ǰת��
		tooth_trq = _IQ15toIQ(EV_MCU_Para.field.Tooth_trq);  //�������أ������������ 2/500��<< 9λ = 2.048
		//pre_trq_value = _IQmpy(_IQ(0.5),SysBase.invtorque);
		trq_lmt = _IQmpyI32(10,SysBase.invtorque);  // 1/50 = 0.02
		trq_step = _IQmpy(_IQ(0.01),SysBase.invtorque);  // 1/50000    ��С����
		trq_step_second = _IQmpy(_IQ(0.02125),SysBase.invtorque);  // 0.0000425
		trq_step_third = _IQmpy(_IQ(0.025),SysBase.invtorque);  // 1/20000 = 0.00005
		//trq_gear_step = _IQmpy(_IQ(0.015),SysBase.invtorque);
		tooth_trq_step = _IQmpy(_IQ(0.2),SysBase.invtorque); // 1/2500
		max_fwd_up_step = _IQmpy(_IQ(0.5),SysBase.invtorque);  // 1/1000
		max_fwd_down_step = _IQmpy(_IQ(1.05),SysBase.invtorque);  // 1.05/500
		max_feedback_up_step = _IQmpy(_IQ(3.5),SysBase.invtorque); // 3.5/500
		max_feedback_down_step = _IQmpy(_IQ(0.1),SysBase.invtorque);  // 0.1/500
		min_feedback_up_step = _IQmpy(_IQ(0.05),SysBase.invtorque);   // 0.05/500
		min_feedback_down_step = _IQmpy(_IQ(0.05),SysBase.invtorque);  // 0.05/500
		max_feedback_enter_down_step = _IQmpy(_IQ(3.5),SysBase.invtorque);  // 3.5/500
		gear_n_step = _IQmpy(_IQ(0.5),SysBase.invtorque); // 0.5/500
		max_rev_down_step = _IQmpy(_IQ(0.25),SysBase.invtorque);  // 0.25/500
		max_rev_up_step = _IQmpy(_IQ(1.5),SysBase.invtorque);  // 1.5/500
		trq_n_step_updown = _IQmpy(_IQ(0.1),SysBase.invtorque);  // 0.1/500
		
		sccw1.field.runLoop_mode = MTR_TRQ_LOOP;  //����ģʽ��Ϊ���ػ�

		//ɲ���źŲɼ�
		if(1 == vehicle_io_state.data.DI2_BRRAK_DSP)
		{
			Vehicle_cmd.cmdmode.data.brake_state = 1;
		}
		else
		{
			Vehicle_cmd.cmdmode.data.brake_state = 0;
		}

		//��λ�źŲɼ�
		if((1 == vehicle_io_state.data.DI4_DSP)//��
		 &&(0 == vehicle_io_state.data.DI5_DSP))
		{
			VCU_IOstate.field.In_fwdrev_new = 1;//D��
		}
		else if((0 == vehicle_io_state.data.DI4_DSP)
		      &&(1 == vehicle_io_state.data.DI5_DSP))
		{
			VCU_IOstate.field.In_fwdrev_new = 2;//R��
		}
		else
		{
			VCU_IOstate.field.In_fwdrev_new = 0;//N��
		}

		//�����ǹҵ�����
		if(VCU_IOstate.field.In_fwdrev_new != VCU_IOstate.field.In_fwdrev)//�ҵ�����
		{
			if(1 == VCU_IOstate.field.In_fwdrev_new)//ǰ����
			{
				if(((0 == VCU_IOstate.field.In_fwdrev)||(2 == VCU_IOstate.field.In_fwdrev))  //�ٶ�<50 && ��ǰ��״̬
				 &&(speed<50))
				{
					Vehicle_cmd.cmdmode.data.gear_swap_trq_flag = 1;   //  �ƶ�״̬
				}
				VCU_IOstate.field.In_fwdrev = VCU_IOstate.field.In_fwdrev_new;  //���µ�λֵ
				Vehicle_cmd.cmdmode.data.gear_swap_zero_flag = 1;  //  ����ɲ
				if(speed<0)
				{
					zero_speed_throad = 50-speed;   //����פ���ٶ�
				}
				else if(speed<50)
				{
					zero_speed_throad = 50;
				}
				else
				{
					zero_speed_throad = 15;
				}
			}
			else if(2 == VCU_IOstate.field.In_fwdrev_new)//���˵�
			{
				if(((0 == VCU_IOstate.field.In_fwdrev)||(1 == VCU_IOstate.field.In_fwdrev))  //�ٶ�>-50 && �Ǻ���״̬
				 &&(speed>-50))
				{
					Vehicle_cmd.cmdmode.data.gear_swap_trq_flag = 1;  //  �ƶ�״̬
				}
				VCU_IOstate.field.In_fwdrev = VCU_IOstate.field.In_fwdrev_new;   //���µ�λֵ
				Vehicle_cmd.cmdmode.data.gear_swap_zero_flag = 1;  //  ����ɲ
				if(speed>0)
				{
					zero_speed_throad = 50+speed;  //פ���ٶ�
				}
				else if(speed>-50)
				{
					zero_speed_throad = 50;
				}
				else
				{
					zero_speed_throad = 15;
				}
			}
			else//�յ�
			{
				VCU_IOstate.field.In_fwdrev = VCU_IOstate.field.In_fwdrev_new;  //���µ�λֵ
			}
		}

		switch(VCU_IOstate.field.In_fwdrev)  //���µ�λֵ
		{
			case 1:
				Vehicle_cmd.cmdmode.data.gear_state = 1;
				break;
			case 2:
				Vehicle_cmd.cmdmode.data.gear_state = 2;
				break;
			default:
				Vehicle_cmd.cmdmode.data.gear_state = 0;
				break;
		}

		//�жϻ��л���
		if(0 == Vehicle_cmd.cmdmode.data.slip_feedback_flag)  //���з�����־ = 0 ����λ
		{
			if((0 == Vehicle_cmd.cmdmode.data.brake_state)  //��ɲ���ź�
			 &&(speed>=500)  //�ٶ�>500
			 &&(canarev_cmd.soc_ref<95)  // ʣ�����<95%  ͨ��CAN����
			 &&(1 == Vehicle_cmd.cmdmode.data.gear_state)  // ǰ��״̬    0:�յ�        1��ǰ��        2������
			 &&(0 == Vehicle_cmd.torque_ref)  //���������
			 &&(sysCfgPara.TrqCmd_AI<=tooth_trq)  //   AI��������ֵ  <=  ��������
			 )
			{
				Vehicle_cmd.cmdmode.data.slip_feedback_flag = 0; //���з�����־ = 0 ����λ  �ǻ���״̬     �˴�ӦΪ1
			}
		}
		else
		{
			if((1 == Vehicle_cmd.cmdmode.data.brake_state) //��ɲ���ź�
			 ||(speed<450) //�ٶ�<450
			 ||(canarev_cmd.soc_ref>96)  //ʣ�����>96%  ͨ��CAN����
			 ||(1 != Vehicle_cmd.cmdmode.data.gear_state) //����ǰ��״̬
			 ||(0 != Vehicle_cmd.torque_ref))  //������ز�Ϊ0
			{
				Vehicle_cmd.cmdmode.data.slip_feedback_flag = 0; //���з�����־ = 0 ����λ  �ǻ���״̬
			}
		}
		
		//�ж��ƶ�����
		if(0 == Vehicle_cmd.cmdmode.data.brake_feedback_flag)  //�ƶ�ʱ��ǿ�ϵ�
		{
			if((1 == Vehicle_cmd.cmdmode.data.brake_state) //ɲ���ź�
			 &&(speed>=500) //�ٶ�>500
			 &&(canarev_cmd.soc_ref<95) // ʣ�����<95%  ͨ��CAN����
			 &&(1 == Vehicle_cmd.cmdmode.data.gear_state) // ǰ��״̬    0:�յ�        1��ǰ��        2������
			 &&(0 == Vehicle_cmd.torque_ref) //���������
			 &&(sysCfgPara.TrqCmd_AI<=tooth_trq)) //  AI��������ֵ  <=  ��������
			{
				Vehicle_cmd.cmdmode.data.brake_feedback_flag = 0; //ɲ��������־ = 0 �ƶ�ʱ��ǿ�ϵ�
			}
		}
		else
		{
			if((0 == Vehicle_cmd.cmdmode.data.brake_state)  //��ɲ���ź�
			 ||(speed<450) //�ٶ�<450
			 ||(canarev_cmd.soc_ref>96)  //ʣ�����>96%  ͨ��CAN����
			 ||(1 != Vehicle_cmd.cmdmode.data.gear_state)  //����ǰ��״̬
			 ||(0 != Vehicle_cmd.torque_ref))  //������ز�Ϊ0
			{
				Vehicle_cmd.cmdmode.data.brake_feedback_flag = 0;  //ɲ��������־ = 0 �ƶ�ʱ��ǿ�ϵ�
			}
		}
		
		//��������ع�ָ��
		if((0 != Vehicle_cmd.cmdmode.data.gear_state)  //�ǿյ�
		 &&(0 == scsw2.field.fault_flag)  //�޹���
		 &&(1 == canarev_cmd.cmdmode2.data.main_relay_state)  //�̵ܼ���״̬��
		 &&(0 == canarev_cmd.cmdmode2.data.charge_relay_state)  //���̵����ر�
		 &&(0 == canarev_cmd.cmdmode2.data.charge_state)  //�ǳ��״̬
		 &&(0 == canarev_cmd.cmdmode2.data.charge_line_state)  //�����״̬Ϊ0
		 &&(1 == canarev_cmd.cmdmode2.data.BMS_self_state)) //BMS�Լ�����
		{
			sccw1.field.run_enable              = 1;//���������
			motor_turn_off_cnt = 0;      //��������
		}
		else
		{
			if((0 == Vehicle_cmd.cmdmode.data.gear_state)  //�յ�
			 &&(_IQabs(speed) < 100)  //�ٶȵľ���ֵ<100
			 &&(0 == zero_station_trqCmd)  //��פ��
			 &&(0 == diff_trq_filter)  //δ�õ�
			 &&(0 == sysCfgPara.TrqCmd_AI))  //AI��������ֵ = 0
			{
				motor_turn_off_cnt++;
				if(motor_turn_off_cnt>400)  //4s
				{
					motor_turn_off_cnt = 400;
					sccw1.field.run_enable = 0;  //������ع�
				}
			}
			else
			{
				motor_turn_off_cnt = 0;
			}
		}	

		//���ظ���ָ��
		if(1 == sccw1.field.run_enable)
		{
			if(1 == Vehicle_cmd.cmdmode.data.gear_state)//ǰ����
			{
				if(1 == Vehicle_cmd.cmdmode.data.slip_feedback_flag)//���л���  ���ڻ���״̬
				{
					if(speed>=3000)//15kw��������        ����ת��ȷ����������
					{
						trq = 143250/speed;  //��������
						sysCfgPara.TrqCmd_NEW = 0-_IQmpyI32(trq,SysBase.invtorque);  //��������ֵ
					}
					else if(speed>=1500)//47N.m--->10N.m
					{
						trq = _IQmpyI32(37,SysBase.invtorque);  //  37/500
						k_tmp = _IQdiv(trq,_IQmpyI32(1500,SysBase.invspeed));  //    1500/12000
						k_tmp = _IQmpy(_IQmpyI32((speed-1500),SysBase.invspeed),k_tmp);
						trq = _IQmpyI32(10,SysBase.invtorque)+k_tmp;
						sysCfgPara.TrqCmd_NEW = 0-trq;  //��������ֵ
					}
					else if(speed>=500)//10N.m--->0N.m
					{
						trq = _IQmpyI32(10,SysBase.invtorque);
						k_tmp = _IQdiv(trq,_IQmpyI32(1000,SysBase.invspeed));
						k_tmp = _IQmpy(_IQmpyI32((speed-500),SysBase.invspeed),k_tmp);
						sysCfgPara.TrqCmd_NEW = 0-k_tmp;  //��������ֵ
					}
					else
					{
						sysCfgPara.TrqCmd_NEW = 0;
					}
					if(sysCfgPara.TrqCmd_NEW<-trqLoopPara.Max_TorqueGen)   //��������ֵ < -�����ֵ����
					{
						sysCfgPara.TrqCmd_NEW = -trqLoopPara.Max_TorqueGen;  //��������ֵ = -�����ֵ����
					}
				}
				else if(1 == Vehicle_cmd.cmdmode.data.brake_feedback_flag)// �ƶ�����
				{
					if(speed>=3000)//15kw��������
					{
						trq = 143250/speed;
						sysCfgPara.TrqCmd_NEW = 0-_IQmpyI32(trq,SysBase.invtorque);  //��������ֵ
					}
					else if(speed>=1500)//47N.m--->10N.m
					{
						trq = _IQmpyI32(37,SysBase.invtorque);
						k_tmp = _IQdiv(trq,_IQmpyI32(1500,SysBase.invspeed));
						k_tmp = _IQmpy(_IQmpyI32((speed-1500),SysBase.invspeed),k_tmp);  //��������ֵ
						trq = _IQmpyI32(10,SysBase.invtorque)+k_tmp;
						sysCfgPara.TrqCmd_NEW = 0-trq;
					}
					else if(speed>=500)//10N.m--->0N.m
					{
						trq = _IQmpyI32(10,SysBase.invtorque);
						k_tmp = _IQdiv(trq,_IQmpyI32(1000,SysBase.invspeed));
						k_tmp = _IQmpy(_IQmpyI32((speed-500),SysBase.invspeed),k_tmp);
						sysCfgPara.TrqCmd_NEW = 0-k_tmp;  //��������ֵ
					}
					else
					{
						sysCfgPara.TrqCmd_NEW = 0;
					}
					if(sysCfgPara.TrqCmd_NEW<-trqLoopPara.Max_TorqueGen)  //��������ֵ < -�����ֵ����
					{
						sysCfgPara.TrqCmd_NEW = -trqLoopPara.Max_TorqueGen;  //��������ֵ = -�����ֵ����
					}
				}
				else if(Vehicle_cmd.torque_ref>=0)//ǰ������
				{
					if(1 == Vehicle_cmd.cmdmode.data.brake_state)//��ɲ���ź�
					{
						if(1 == Vehicle_cmd.cmdmode.data.gear_swap_trq_flag)  //�ƶ�״̬  0:������    1���ƶ�
						{
							sysCfgPara.TrqCmd_NEW = 0;  //��������ֵ = 0
						}
						else
						{
							sysCfgPara.TrqCmd_NEW = tooth_trq;  //��������ֵ = ��������
						}
					}
					else
					{
						if(3 == VCU_IOstate.field.exit_zero_add_trq_flag)  // δפ��
						{
							sysCfgPara.TrqCmd_NEW = _IQ15toIQ(Vehicle_cmd.torque_ref)+zero_station_trqCmd_store; // ��������ֵ = ǰ������ + פ������
						}
						else  //פ��
						{
							sysCfgPara.TrqCmd_NEW = _IQ15toIQ(Vehicle_cmd.torque_ref); // ��������ֵ = ǰ������
						}
						if(1 != Vehicle_cmd.cmdmode.data.gear_swap_trq_flag)  //����״̬
						{
							if(sysCfgPara.TrqCmd_NEW<=tooth_trq)  // ��������ֵ < ��������
							{
								sysCfgPara.TrqCmd_NEW = tooth_trq;  // ��������ֵ = ��������
							}
						}
					}
					if(sysCfgPara.TrqCmd_NEW>trqLoopPara.Max_TorqueEle)    // ��������ֵ > �����������
					{
						sysCfgPara.TrqCmd_NEW = trqLoopPara.Max_TorqueEle; // ��������ֵ = �����������
					}
				}
				else  //�������
				{   
					sysCfgPara.TrqCmd_NEW = 0;
				}
			}
			else if(2 == Vehicle_cmd.cmdmode.data.gear_state)//���˵�
			{
				if(1 == Vehicle_cmd.cmdmode.data.brake_state) // ��ɲ���ź�
				{
					if(1 == Vehicle_cmd.cmdmode.data.gear_swap_trq_flag)  //�ƶ�״̬
					{
						sysCfgPara.TrqCmd_NEW = 0;  //  ���ؿ���ֵΪ0
					}
					else
					{
						sysCfgPara.TrqCmd_NEW = -tooth_trq;  //  ���ؿ���ֵ = -��������
					}
				}
				else  //��ɲ���ź�
				{
					if(Vehicle_cmd.torque_ref>=0)  //ǰ������
					{
						if(3 == VCU_IOstate.field.exit_zero_add_trq_flag)  //  δפ��״̬
						{
							sysCfgPara.TrqCmd_NEW = 0-_IQ15toIQ(Vehicle_cmd.torque_ref)+zero_station_trqCmd_store;  // ��������ֵ = -ǰ������ + פ������
						}
						else  //פ��
						{
							sysCfgPara.TrqCmd_NEW = 0-_IQ15toIQ(Vehicle_cmd.torque_ref);  // ��������ֵ = -ǰ������
						}
						if(1 != Vehicle_cmd.cmdmode.data.gear_swap_trq_flag)  //����״̬
						{
							if(sysCfgPara.TrqCmd_NEW>=(-tooth_trq))  // ��������ֵ > �����������
							{
								sysCfgPara.TrqCmd_NEW = -tooth_trq;  // ��������ֵ = �����������
							}
						}
					}
					else
					{
						sysCfgPara.TrqCmd_NEW = 0;
					}
				}
				if(sysCfgPara.TrqCmd_NEW<-trqLoopPara.Max_TorqueEle)  // ��������ֵ < -�����������
				{
					sysCfgPara.TrqCmd_NEW = -trqLoopPara.Max_TorqueEle;  // ��������ֵ = �����������
				}
			}
			else//�յ�
			{
				if(_IQabs(speed)<450)  // �ٶȾ���ֵ < 450
				{
					sysCfgPara.TrqCmd_NEW = 0;
				}
				else if(_IQabs(speed)>500) // �ٶȾ���ֵ > 500
				{
					if(speed>500)//��ת
					{
						sysCfgPara.TrqCmd_NEW = 0-_IQmpyI32(5,SysBase.invtorque);  //  �趨��������ֵ
					}
					else//��ת
					{
						sysCfgPara.TrqCmd_NEW = _IQmpyI32(5,SysBase.invtorque);  //  �趨��������ֵ
					}
				}
			}

			if(1 == alarm.field.break_error)  //ɲ������
			{
				sysCfgPara.TrqCmd_NEW = 0;  // �����趨ֵ = 0
			}

			if(1 == Vehicle_cmd.cmdmode.data.gear_state)//ǰ����
			{
				if(1 == Vehicle_cmd.cmdmode.data.slip_feedback_flag)//������������
				{
					if(_IQabs(sysCfgPara.TrqCmd_AI)<=tooth_trq)   //AI��������ֵ�ľ���ֵ < ��������     פ��ģ�� -> sysCfgPara.TrqCmd_AI -> ���ػ�
					{
						if(sysCfgPara.TrqCmd_AI<=0)  // AI��������ֵ <= 0
						{
							if((sysCfgPara.TrqCmd_AI == -tooth_trq)  // AI��������ֵ = -��������
							 ||(up_step>tooth_trq_step))  //�������� > �������ز���
							{
								up_step = tooth_trq_step;  //�������� = �������ز���
							}
							else
							{
								up_step = up_step-trq_step; //�������� = �������� - ��С����
								if(up_step<trq_step)  //  �������� < ��С����
								{
									up_step = trq_step;  //  �������� = ��С����
								}
							}
							if((sysCfgPara.TrqCmd_AI+up_step)>=0)  // AI��������ֵ + �������� >= 0
							{
								up_step = _IQabs(sysCfgPara.TrqCmd_AI)+1;  //�������� = AI��������ֵ�ľ���ֵ + 1
							}
							
							if((down_step<trq_step)  //�½����� < ��С����
							 ||(0 == sysCfgPara.TrqCmd_AI))  // AI��������ֵ = 0
							{
								down_step = trq_step;  //�½����� = ��С����
							}
							else
							{
								down_step = down_step+trq_step;   //  �½����� = �½����� + ��С����
								if(down_step>max_feedback_down_step)
								{
									down_step = max_feedback_down_step;
								}
							}
						}
						else   // AI��������ֵ > 0
						{
							up_step = trq_step;  //�������� = ��С���� = 1/50000
							
							if((sysCfgPara.TrqCmd_AI == tooth_trq)  // AI��������ֵ = ��������
							 ||(down_step>tooth_trq_step))  // �½����� > 1/2500
							{
								down_step = tooth_trq_step;  // �½����� = 1/2500
							}
							else
							{
								down_step = down_step-trq_step;  // �½����� - 1/50000
								if(down_step<trq_step)  //�½����� < ��С����
								{
									down_step = trq_step;  //�½����� = ��С����
								}
							}
							if((sysCfgPara.TrqCmd_AI-down_step)<=0)  //  AI��������ֵ < �½�����
							{
								down_step = _IQabs(sysCfgPara.TrqCmd_AI);  //�½����� =  AI��������ֵ�ľ���ֵ
							}
						}
					}
					else  //AI��������ֵ�ľ���ֵ > ��������
					{
						if(sysCfgPara.TrqCmd_AI>tooth_trq) // AI��������ֵ > ��������  (+)
						{
							up_step = _IQmpy(_IQ(0.1),SysBase.invtorque);  //��������
							down_step = _IQmpy(_IQ(1.0),SysBase.invtorque);  //�½�����
							if((sysCfgPara.TrqCmd_AI-down_step)<=tooth_trq)
							{
								down_step = _IQabs(sysCfgPara.TrqCmd_AI-tooth_trq);
							}
						}
						else  // AI��������ֵ < -�������� (-)
						{
							trq_deta = _IQabs(sysCfgPara.TrqCmd_NEW-sysCfgPara.TrqCmd_AI); //��������ֵ - AI��������ֵ
							if(sysCfgPara.TrqCmd_AI>=-trq_lmt)  // ��������ֵ > -���ؼ���ֵ
							{
								x_value = _IQabs(sysCfgPara.TrqCmd_AI);
								x_value = -_IQmpy(_IQ(6.25),x_value)+_IQ(0.125);
								up_step = -_IQmpy(_IQ(0.069),x_value)+_IQmpy(_IQ(3.5),SysBase.invtorque);   //��������
							
								x_value = _IQabs(sysCfgPara.TrqCmd_AI);
								x_value = _IQmpy(_IQ(6.25),x_value)-_IQ(0.025);
								down_step = _IQmpy(_IQ(0.001),x_value)+_IQmpy(_IQ(0.05),SysBase.invtorque); //�½�����
							}
							else if(trq_deta<trq_lmt)  // ��������ֵ < 0.02
							{
								x_value = trq_deta;
								x_value = -_IQmpy(_IQ(5.0),x_value)+_IQ(0.1);
								up_step = -_IQmpy(_IQ(0.069),x_value)+_IQmpy(_IQ(3.5),SysBase.invtorque);  //��������
								
								x_value = trq_deta;
								x_value = -_IQmpy(_IQ(5.0),x_value)+_IQ(0.1);
								down_step = -_IQmpy(_IQ(0.001),x_value)+_IQmpy(_IQ(0.1),SysBase.invtorque);  //�½�����
							}
							else
							{
								up_step = max_feedback_up_step;  //��������
								down_step = max_feedback_down_step;  //�½�����
							}
							//������������
							if(up_step>max_feedback_up_step)
							{
								up_step = max_feedback_up_step;
							}
							else if(up_step<min_feedback_up_step)
							{
								up_step = min_feedback_up_step;
							}

							if((sysCfgPara.TrqCmd_AI+up_step)>=-tooth_trq)
							{
								up_step = _IQabs(_IQabs(sysCfgPara.TrqCmd_AI)-tooth_trq);
							}
							//�½���������
							if(down_step>max_feedback_down_step)
							{
								down_step = max_feedback_down_step;
							}
							else if(down_step<min_feedback_down_step)
							{
								down_step = min_feedback_down_step;
							}
						}
					}
				}
				else if(1 == Vehicle_cmd.cmdmode.data.brake_feedback_flag)//�ƶ���������
				{

					if(_IQabs(sysCfgPara.TrqCmd_AI)<=tooth_trq)  //AI��������ֵ�ľ���ֵ <= ��������
					{
						if(sysCfgPara.TrqCmd_AI<=0) //AI��������ֵ <= 0
						{
							if((sysCfgPara.TrqCmd_AI == -tooth_trq)  // AI��������ֵ = -��������
							 ||(up_step>tooth_trq_step))  //�½����� > 1/2500
							{
								up_step = tooth_trq_step;  //�½����� = 1/2500
							}
							else
							{
								up_step = up_step-trq_step;
								if(up_step<trq_step)
								{
									up_step = trq_step;
								}
							}
							if((sysCfgPara.TrqCmd_AI+up_step)>=0)
							{
								up_step = _IQabs(sysCfgPara.TrqCmd_AI)+1;
							}
							
							if((down_step<trq_step)
							 ||(0 == sysCfgPara.TrqCmd_AI))
							{
								down_step = trq_step;
							}
							else
							{
								down_step = down_step+trq_step;
								if(down_step>max_feedback_down_step)
								{
									down_step = max_feedback_down_step;
								}
							}
						}
						else  //AI��������ֵ > 0
						{
							up_step = trq_step;
							
							if((sysCfgPara.TrqCmd_AI == tooth_trq)
							 ||(down_step>tooth_trq_step))
							{
								down_step = tooth_trq_step;
							}
							else
							{
								down_step = down_step-trq_step;
								if(down_step<trq_step)
								{
									down_step = trq_step;
								}
							}
							if((sysCfgPara.TrqCmd_AI-down_step)<=0)
							{
								down_step = _IQabs(sysCfgPara.TrqCmd_AI);
							}
						}
					}
					else
					{
						if(sysCfgPara.TrqCmd_AI>tooth_trq)
						{
							up_step = _IQmpy(_IQ(0.1),SysBase.invtorque);
							down_step = max_feedback_enter_down_step;
							if((sysCfgPara.TrqCmd_AI-down_step)<=tooth_trq)
							{
								down_step = _IQabs(sysCfgPara.TrqCmd_AI-tooth_trq);
							}
						}
						else
						{
							trq_deta = _IQabs(sysCfgPara.TrqCmd_NEW-sysCfgPara.TrqCmd_AI);
							if(sysCfgPara.TrqCmd_AI>=-trq_lmt)
							{
								x_value = _IQabs(sysCfgPara.TrqCmd_AI);
								x_value = -_IQmpy(_IQ(6.25),x_value)+_IQ(0.125);
								up_step = -_IQmpy(_IQ(0.069),x_value)+_IQmpy(_IQ(3.5),SysBase.invtorque);
							
								x_value = _IQabs(sysCfgPara.TrqCmd_AI);
								x_value = _IQmpy(_IQ(6.25),x_value)-_IQ(0.025);
								down_step = _IQmpy(_IQ(0.001),x_value)+_IQmpy(_IQ(0.05),SysBase.invtorque);
							}
							else if(trq_deta<trq_lmt)
							{
								x_value = trq_deta;
								x_value = -_IQmpy(_IQ(5.0),x_value)+_IQ(0.1);
								up_step = -_IQmpy(_IQ(0.069),x_value)+_IQmpy(_IQ(3.5),SysBase.invtorque);
								
								x_value = trq_deta;
								x_value = -_IQmpy(_IQ(5.0),x_value)+_IQ(0.1);
								down_step = -_IQmpy(_IQ(0.001),x_value)+_IQmpy(_IQ(0.1),SysBase.invtorque);
							}
							else
							{
								up_step = max_feedback_up_step;
								down_step = max_feedback_down_step;
							}
							if(up_step>max_feedback_up_step)
							{
								up_step = max_feedback_up_step;
							}
							else if(up_step<min_feedback_up_step)
							{
								up_step = min_feedback_up_step;
							}
							if((sysCfgPara.TrqCmd_AI+up_step)>=-tooth_trq)
							{
								up_step = _IQabs(_IQabs(sysCfgPara.TrqCmd_AI)-tooth_trq);
							}
							if(down_step>max_feedback_down_step)
							{
								down_step = max_feedback_down_step;
							}
							else if(down_step<min_feedback_down_step)
							{
								down_step = min_feedback_down_step;
							}
						}
					}

				}
				else//ǰ������
				{

					if(_IQabs(sysCfgPara.TrqCmd_AI)<=tooth_trq)
					{
						if(sysCfgPara.TrqCmd_AI<0)
						{
							//200ms
							up_step = tooth_trq_step;
							if((sysCfgPara.TrqCmd_AI+up_step)>=0)
							{
								up_step = _IQabs(sysCfgPara.TrqCmd_AI);
							}

							down_step = trq_step;
						}
						else
						{
							//200ms
							if((up_step<trq_step)   //��������<��С����
							 ||(0 == sysCfgPara.TrqCmd_AI)
							 ||(1 == Vehicle_cmd.cmdmode.data.brake_state)  //ɲ���ź�
							 ||((0 == Vehicle_cmd.torque_ref)&&(sysCfgPara.TrqCmd_NEW <= tooth_trq))
							 ||(1 == scsw2.field.zero_state)  //פ��
							 ||(1 == Vehicle_cmd.cmdmode.data.gear_swap_trq_flag))  //פ��
							{
								up_step = trq_step;
							}
							else
							{
								up_step = up_step+trq_step;
								if(up_step>tooth_trq_step)
								{
									up_step = tooth_trq_step;
								}
							}

							//200ms
							if((sysCfgPara.TrqCmd_AI == tooth_trq)
							 ||(down_step>tooth_trq_step))
							{
								down_step = tooth_trq_step;
							}
							else
							{
								down_step = down_step-trq_step;
								if(down_step<trq_step)
								{
									down_step = trq_step;
								}
							}
							if((sysCfgPara.TrqCmd_AI-down_step)<=0)
							{
								down_step = _IQabs(sysCfgPara.TrqCmd_AI)+1;
							}
						}
					}
					else if(sysCfgPara.TrqCmd_AI>tooth_trq)  //sysCfgPara.TrqCmd_AI > 0
					{
						trq_deta = _IQabs(sysCfgPara.TrqCmd_NEW-sysCfgPara.TrqCmd_AI);
						if(sysCfgPara.TrqCmd_AI<=trq_lmt)
						{
							//200ms
							if(up_step<trq_step)
							{
								up_step = trq_step;
							}
							else if(up_step<tooth_trq_step)
							{
								up_step = up_step+trq_step;
								if(up_step>tooth_trq_step)
								{
									up_step = tooth_trq_step;
								}
							}
							else
							{
								up_step = up_step+trq_step_second;
								if(up_step>max_fwd_up_step)
								{
									up_step = max_fwd_up_step;
								}
							}

							//100ms
							x_value = _IQabs(sysCfgPara.TrqCmd_AI);
							x_value = -_IQmpy(_IQ(1.25),x_value)+_IQ(0.025);
							down_step = -_IQmpy(_IQ(0.0925),x_value)+_IQmpy(_IQ(1.05),SysBase.invtorque);
						}
						else if(trq_deta<=trq_lmt)//�յ�׶�
						{
							x_value = trq_deta;
							x_value = -_IQmpy(_IQ(5.0),x_value)+_IQ(0.1);
							up_step = -_IQmpy(_IQ(0.02),x_value)+_IQmpy(_IQ(1.05),SysBase.invtorque);

							x_value = trq_deta;
							x_value = -_IQmpy(_IQ(5.0),x_value)+_IQ(0.1);
							down_step = -_IQmpy(_IQ(0.02),x_value)+_IQmpy(_IQ(1.05),SysBase.invtorque);
						}
						else if((trq_deta>trq_lmt)
							  &&(sysCfgPara.TrqCmd_AI<sysCfgPara.TrqCmd_NEW))//���ٽ׶�
						{
							if(up_step<trq_step)
							{
								up_step = trq_step;
							}
							else if(up_step<tooth_trq_step)
							{
								up_step = up_step+trq_step;
								if(up_step>tooth_trq_step)
								{
									up_step = tooth_trq_step;
								}
							}
							else
							{
								up_step = up_step+trq_step_third;
								if(up_step>max_fwd_up_step)
								{
									up_step = max_fwd_up_step;
								}
							}

							down_step = trq_step;
						}
						else if((trq_deta>trq_lmt)
							  &&(sysCfgPara.TrqCmd_AI>sysCfgPara.TrqCmd_NEW))//���ٽ׶�
						{
							up_step = trq_step;

							if(down_step<max_fwd_down_step)
							{
								down_step = down_step+_IQmpy(_IQ(0.05),SysBase.invtorque);
							}
							else
							{
								down_step = max_fwd_down_step;
							}
						}
						else
						{
							up_step = max_fwd_up_step;
							down_step = max_fwd_down_step;
						}
						if(up_step>max_fwd_up_step)
						{
							up_step = max_fwd_up_step;
						}
						else if(up_step<trq_step)
						{
							up_step = trq_step;
						}
						if(down_step>max_fwd_down_step)
						{
							down_step = max_fwd_down_step;
						}
						else if(down_step<trq_step)
						{
							down_step = trq_step;
						}
						if((sysCfgPara.TrqCmd_AI-down_step)<=tooth_trq)
						{
							down_step = _IQabs(sysCfgPara.TrqCmd_AI-tooth_trq);
						}
					}
					else  //sysCfgPara.TrqCmd_AI < 0
					{
						up_step = _IQmpy(_IQ(1.5),SysBase.invtorque);
						down_step = trq_step;
						if((sysCfgPara.TrqCmd_AI+up_step)>=-tooth_trq)
						{
							up_step = _IQabs(_IQabs(sysCfgPara.TrqCmd_AI)-tooth_trq);
						}
					}	



				}
			}
			else if(2 == Vehicle_cmd.cmdmode.data.gear_state)//���˵�
			{


				if(_IQabs(sysCfgPara.TrqCmd_AI)<=tooth_trq)
				{
					if(sysCfgPara.TrqCmd_AI>0)
					{
						up_step = trq_step;

						//200ms
						if((sysCfgPara.TrqCmd_AI == tooth_trq)
						 ||(down_step > tooth_trq_step))
						{
							down_step = tooth_trq_step;
						}
						else
						{
							down_step = down_step-trq_step;
							if(down_step<trq_step)
							{
								down_step = trq_step;
							}
						}
						if((sysCfgPara.TrqCmd_AI-down_step)<=0)
						{
							down_step = sysCfgPara.TrqCmd_AI;
						}
					}
					else  // sysCfgPara.TrqCmd_AI < 0
					{
						//200ms
						if((sysCfgPara.TrqCmd_AI == -tooth_trq)
						 ||(up_step>tooth_trq_step))
						{
							up_step = tooth_trq_step;
						}
						else
						{
							up_step = up_step-trq_step;
							if(up_step<trq_step)
							{
								up_step = trq_step;
							}
						}
						if((sysCfgPara.TrqCmd_AI+up_step)>=0)
						{
							down_step = _IQabs(sysCfgPara.TrqCmd_AI);
						}
						
						//200ms
						if((down_step<trq_step)
						 ||(0 == sysCfgPara.TrqCmd_AI)
						 ||(1 == Vehicle_cmd.cmdmode.data.brake_state)
						 ||((0 == Vehicle_cmd.torque_ref)&&(sysCfgPara.TrqCmd_NEW >= -tooth_trq))
						 ||(1 == scsw2.field.zero_state)
						 ||(1 == Vehicle_cmd.cmdmode.data.gear_swap_trq_flag))
						{
							down_step = trq_step;
						}
						else
						{
							down_step = down_step+trq_step;
							if(down_step>tooth_trq_step)
							{
								down_step = tooth_trq_step;
							}
						}
					}
				}
				else if(sysCfgPara.TrqCmd_AI>tooth_trq)
				{
					up_step = trq_step;
					
					if(sysCfgPara.TrqCmd_AI>trq_lmt)
					{
						down_step = _IQmpy(_IQ(1.5),SysBase.invtorque);
					}
					else
					{
						if(down_step>_IQmpy(_IQ(1.5),SysBase.invtorque))
						{
							down_step = _IQmpy(_IQ(1.5),SysBase.invtorque);
						}
						else
						{
							down_step = down_step-_IQmpy(_IQ(0.065),SysBase.invtorque);
							if(down_step<tooth_trq_step)
							{
								down_step = tooth_trq_step;
							}
						}
					}
					if((sysCfgPara.TrqCmd_AI-down_step)<=tooth_trq)
					{
						down_step = _IQabs(sysCfgPara.TrqCmd_AI-tooth_trq);
					}
				}
				else
				{
					trq_deta = _IQabs(sysCfgPara.TrqCmd_NEW-sysCfgPara.TrqCmd_AI);
					if(sysCfgPara.TrqCmd_AI>=-trq_lmt)
					{
						//200ms
						if(up_step>max_rev_up_step)
						{
							up_step = max_rev_up_step;
						}
						else
						{
							up_step = up_step-_IQmpy(_IQ(0.06875),SysBase.invtorque);
							if(up_step<tooth_trq_step)
							{
								up_step = tooth_trq_step;
							}
						}

						//200ms
						if(down_step<trq_step)
						{
							down_step = trq_step;
						}
						else if(down_step<tooth_trq_step)
						{
							down_step = down_step+trq_step;
							if(down_step>tooth_trq_step)
							{
								down_step = tooth_trq_step;
							}
						}
						else
						{
							down_step = down_step+_IQmpy(_IQ(0.00625),SysBase.invtorque);
							if(down_step>max_rev_down_step)
							{
								down_step = max_rev_down_step;
							}
						}
					}
					else if(trq_deta<trq_lmt)
					{
						//500ms
						if(up_step>max_rev_up_step)
						{
							up_step = max_rev_up_step;
						}
						else
						{
							up_step = up_step-_IQmpy(_IQ(0.03),SysBase.invtorque);
							if(up_step<trq_step)
							{
								up_step = trq_step;
							}
						}

						//500ms
						if(down_step>max_rev_down_step)
						{
							down_step = max_rev_down_step;
						}
						else
						{
							down_step = down_step-_IQmpy(_IQ(0.0048),SysBase.invtorque);
							if(down_step<trq_step)
							{
								down_step = trq_step;
							}
						}
					}
					else
					{
						if(sysCfgPara.TrqCmd_AI>sysCfgPara.TrqCmd_NEW)//���ٽ׶�
						{
							up_step = trq_step;
							
							if(down_step<max_rev_down_step)
							{
								down_step = down_step+_IQmpy(_IQ(0.0125),SysBase.invtorque);
							}
							else
							{
								down_step = max_rev_down_step;
							}
						}
						else//���ٽ׶�
						{
							if(up_step<max_rev_up_step)
							{
								up_step = up_step+_IQmpy(_IQ(0.075),SysBase.invtorque);
							}
							else
							{
								up_step = max_rev_up_step;
							}
							
							down_step = trq_step;
						}
					}
					if(up_step>max_rev_up_step)
					{
						up_step = max_rev_up_step;
					}
					else if(up_step<trq_step)
					{
						up_step = trq_step;
					}
					if((sysCfgPara.TrqCmd_AI+up_step)>=-tooth_trq)
					{
						up_step = _IQabs(_IQabs(sysCfgPara.TrqCmd_AI)-tooth_trq);
					}
					if(down_step>max_rev_down_step)
					{
						down_step = max_rev_down_step;
					}
					else if(down_step<trq_step)
					{
						down_step = trq_step;
					}
				}

			}
			else//�յ�
			{
				if(_IQabs(sysCfgPara.TrqCmd_AI)<=tooth_trq)
				{
					up_step = trq_n_step_updown;
					down_step = trq_n_step_updown;
				}
				else
				{
					if(sysCfgPara.TrqCmd_AI>tooth_trq)
					{
						up_step = trq_n_step_updown;
						down_step = gear_n_step;
						if((sysCfgPara.TrqCmd_AI-down_step)<=tooth_trq)
						{
							down_step = sysCfgPara.TrqCmd_AI-tooth_trq;
						}
					}
					else
					{
						up_step = gear_n_step;
						if((sysCfgPara.TrqCmd_AI+up_step)>=-tooth_trq)
						{
							up_step = _IQabs(sysCfgPara.TrqCmd_AI)-tooth_trq;
						}
						down_step = trq_n_step_updown;
					}
				}
			}
			
			if(sysCfgPara.TrqCmd_NEW>sysCfgPara.TrqCmd_AI)
			{
				sysCfgPara.TrqCmd_AI = sysCfgPara.TrqCmd_AI+up_step;
				if(sysCfgPara.TrqCmd_AI>sysCfgPara.TrqCmd_NEW)
				{
					sysCfgPara.TrqCmd_AI = sysCfgPara.TrqCmd_NEW;
				}
			}
			else if(sysCfgPara.TrqCmd_NEW<sysCfgPara.TrqCmd_AI)
			{
				sysCfgPara.TrqCmd_AI = sysCfgPara.TrqCmd_AI-down_step;
				if(sysCfgPara.TrqCmd_AI<sysCfgPara.TrqCmd_NEW)
				{
					sysCfgPara.TrqCmd_AI = sysCfgPara.TrqCmd_NEW;
				}
			}
		}
		else
		{
			sysCfgPara.TrqCmd_NEW	               = 0;
			sysCfgPara.TrqCmd_AI                   = 0;
		}
	}
	else
	{
		VCU_IOstate.field.In_fwdrev 	           = 0;
		VCU_IOstate.field.In_fwdrev_new            = 0;
		sysCfgPara.TrqCmd_NEW                      = 0;
		sysCfgPara.TrqCmd_AI                       = 0;
	}
}

void AIFeedback(void)//���ŷ���
{
	int16 i = 0;
    static int16 ConversionCnt = 0;
    _iq AI1_ad  = 0;
	_iq AI1_ad_offset = 0;
	_iq tmp = 0;
	_iq ai_deta = 0;
	_iq AI1_ad_tmp = 0;

	if(TRQCMD_SOURCE_AI == EV_MCU_Para.field.TrqCmd_SourceType)   //����������Դ = 0 AI1
	{
		if(sysFbkPara.AI1_fbk_ad>EV_MCU_Para.field.AI1_offset_off)  // ���ŵ�AD����ֵ > 2350 = 100%
		{
			AI1_ad_tmp = EV_MCU_Para.field.AI1_offset_off+1; // 2350 + 1 = 100%
		}
		else
		{
			AI1_ad_tmp = sysFbkPara.AI1_fbk_ad;  // ��֮��ֵΪ��ǰ����ֵ
		}
		if(0 == AD_AISelf_Flag)
		{
			AD_AISelf_Cnt++;
			AD_AI1_Sum += AI1_ad_tmp;   //AD�����ۼ�ֵ
			if(AD_AISelf_Cnt>=128)  //1280ms
			{
				AD_AISelf_Flag = 1;
				AD_AISelf_Cnt = 0;
				AI1_ad_offset = AD_AI1_Sum>>7;  //����ƽ��ad����ֵ
				if(AI1_ad_offset > (EV_MCU_Para.field.AI1_offset_on-20))  //550  (<550 ��Ϊ��0)
				{
					emsw2.field.Torque1_err = 1;  // ��λ�����־
				}
				for (i = 0; i < AI_BUFFER_LENGTH; i++)  // AI_BUFFER_LENGTH = 8
				{
					AI1_Ad_Buffer[i]  = AI1_ad_offset;  //��ƽ��AD����ֵ���뻺������
				}
			}
		}
		else
		{
			if(1 == emsw2.field.Torque1_err)    // �����־Ϊ1
			{
				AD_AI1_Sum = AD_AI1_Sum-(AD_AI1_Sum>>7)+AI1_ad_tmp;  //AD�����ۼ�ֵ = AD�����ۼ�ֵ - ƽ��ֵ + ��ǰֵ  (�˲�)
				AI1_ad_offset = AD_AI1_Sum>>7; // ���µ�ƽ��ֵ
				if(AI1_ad_offset<(EV_MCU_Para.field.AI1_offset_on-30))  // ƽ��ֵ < 550 -30
				{
					emsw2.field.Torque1_err = 0;  // ��������־
				}
				for (i = 0; i < AI_BUFFER_LENGTH; i++) // AI_BUFFER_LENGTH = 8
				{
					AI1_Ad_Buffer[i]  = AI1_ad_offset; //��ƽ��AD����ֵ���뻺������
				}
			}
			else // �����־Ϊ0
			{
				AI1_Ad_Buffer[ConversionCnt] = AI1_ad_tmp;   //����ǰAD����ֵ���� ��������
				for (i = 0; i < AI_BUFFER_LENGTH; i++ )
				{
					AI1_ad += AI1_Ad_Buffer[i];  //�������ۼ�
				}
				AI1_ad  = AI1_ad>>3;  //����ƽ��ֵ
				ConversionCnt++;
				ConversionCnt = ConversionCnt%AI_BUFFER_LENGTH;  // ConversionCnt 0-7 ѭ��

				if(AI1_ad<EV_MCU_Para.field.AI1_offset_on)   // ����ƽ��ֵ < 550
				{
					AI1_ad = EV_MCU_Para.field.AI1_offset_on;  // ����ƽ��ֵ = 550
				}
				if(AI1_ad>EV_MCU_Para.field.AI1_offset_off)  // ����ƽ��ֵ > 2350
				{
					AI1_ad = EV_MCU_Para.field.AI1_offset_off;  // ����ƽ��ֵ = 2350
				}
				AI1_ad = AI1_ad-EV_MCU_Para.field.AI1_offset_on;  // ����ƽ��ֵ - 550

				ai_deta = (EV_MCU_Para.field.AI1_offset_off-EV_MCU_Para.field.AI1_offset_on);  //2350 - 550
				tmp = _IQ15div(AI1_ad,ai_deta);  // ���Ա���
				sysFbkPara.AI1_fbk = _IQ15mpy(tmp,EV_MCU_Para.field.Motor_PeakTorqueEle);//IQ15   //���Ա��� * ��ֵŤ��  150
				Vehicle_cmd.torque_ref = sysFbkPara.AI1_fbk;//IQ15
				
				if((1 == Vehicle_cmd.cmdmode.data.brake_state) //ɲ��״̬
				 &&(Vehicle_cmd.torque_ref>0))  //����ֵ>0
				{
					AD_AISelf_Cnt++;
					if(AD_AISelf_Cnt > 400)   //4S
					{
						alarm.field.break_error = 1;//ɲ������
						AD_AISelf_Cnt         = 500;
					}
				}
				else
				{
					AD_AISelf_Cnt = 0;
					alarm.field.break_error = 0;
				}
			}
		}
	}
	else
	{
		emsw2.field.Torque1_err = 0;
		emsw2.field.Torque2_err = 0;
	}
}

void motorParameterInit(void)
{
	sysFbkPara.V12_fbk = sysFbkPara.V12_fbk_filter = _IQmpyI32(12,SysBase.invudc);
	sysFbkPara.V12_fbk_HMI = _IQtoIQ15(sysFbkPara.V12_fbk_filter);
	Motor_Encoder_offset_current = Motor_Encoder_offset = (EV_MCU_Para.field.Motor_Encoder_offset<<3);
	Motor_Encoder_offset_coeff = EV_MCU_Para.field.Motor_Encoder_offset_elecoeff;
}

void motorLookupOffsetTable(void)
{
	_iq speed = 0;
	_iq encoder_index_pre = 0;
	_iq encoder_index_nex = 0;
	_iq encoder_deta_speed = 0;
	_iq encoder_speed_abs = 0;
	_iq encoder_current_value = 0;
	_iq deta_speed = 0;
	_iq deta_encoder = 0;
	_iq k_encoder = 0;
	_iq Motor_Encoder_offset_base = 0;
	int16 *table = (int *)0;

	speed = _IQ15mpy(sysFbkPara.Speed_fbk_HMI,SysBase.speed);
	if(1 == EV_MCU_Para.field.Motor_Direction)
	{
		speed = 0-speed;
	}
	if(speed<-500)
	{
		Motor_Encoder_offset_base = (EV_MCU_Para.field.Motor_Encoder_offset<<3);
		table = (int *)(&encoder_offsetrev[0]);
	}
	else
	{
		Motor_Encoder_offset_base = (EV_MCU_Para.field.Motor_Encoder_offset<<3);
		table = (int *)(&encoder_offsetfwd[0]);
	}
	if(4 == EV_MCU_Para.field.Weak_enable)
	{
		Motor_Encoder_offset_current = Motor_Encoder_offset_base;
	}
	else
	{
		encoder_speed_abs = _IQabs(sysFbkPara.Speed_fbk_Filter);
		if(encoder_speed_abs<TRQ_Table.encoder_minspeed)
		{
			Motor_Encoder_offset_current = Motor_Encoder_offset_base;
		}
		else if(encoder_speed_abs>=TRQ_Table.encoder_maxspeed)
		{
			encoder_index_pre = encoder_index_nex = TRQ_Table.encoder_num-1;
			Motor_Encoder_offset_current = Motor_Encoder_offset_base+table[encoder_index_nex];
		}
		else
		{
			encoder_deta_speed = encoder_speed_abs-TRQ_Table.encoder_minspeed;
			encoder_index_pre  = encoder_deta_speed/TRQ_Table.encoder_detaspeed;
			encoder_index_nex  = encoder_index_pre+1;
			deta_speed = encoder_deta_speed-_IQmpyI32(encoder_index_pre,TRQ_Table.encoder_detaspeed);
			deta_encoder = table[encoder_index_nex]-table[encoder_index_pre];
			k_encoder = _IQdiv(deta_speed,TRQ_Table.encoder_detaspeed);
			k_encoder = _IQmpy(k_encoder,deta_encoder);
			encoder_current_value = table[encoder_index_pre]+k_encoder;
			Motor_Encoder_offset_current = Motor_Encoder_offset_base+encoder_current_value;
		}
		while((Motor_Encoder_offset_current<0)||(Motor_Encoder_offset_current>=_IQ15(1.0)))
		{
			if(Motor_Encoder_offset_current<0)
			{
				Motor_Encoder_offset_current = Motor_Encoder_offset_current+_IQ15(1.0);
			}
			else
			{
				Motor_Encoder_offset_current = Motor_Encoder_offset_current-_IQ15(1.0);
			}
		}
	}

	if(_IQmpy(trqLoop_trqRampref ,sysFbkPara.Speed_fbk ) >= 0)
	{
		Motor_Encoder_offset_coeff = EV_MCU_Para.field.Motor_Encoder_offset_elecoeff;
	}
	else
	{
		Motor_Encoder_offset_coeff = EV_MCU_Para.field.Motor_Encoder_offset_gencoeff;
	}
}

void v12Feedback(void)
{
	sysFbkPara.V12_fbk = _IQmpyI32(sysFbkPara.V12_fbk_ad,EV_MCU_Para.field.V12_adjustCoeff);//IQ24-BASE
	LPFilter(sysFbkPara.V12_fbk,&sysFbkPara.V12_fbk_filter,_IQ(0.3),_IQ(0.7));//IQ24-BASE
	sysFbkPara.V12_fbk_HMI = _IQtoIQ15(sysFbkPara.V12_fbk_filter);//IQ15-BASE

	motor_sys_init_cnt++;
	if(motor_sys_init_cnt>250)
	{
		motor_sys_init_cnt = 250;
	}
	if(motor_sys_init_cnt>=250)
	{
		if(sysFbkPara.V12_fbk_HMI > EV_MCU_Para.field.Motor_Protect_V12Ov)
		{
			emsw1.field.V12_overVolt = 1;
		}
		else
		{
			if((1 == emsw1.field.V12_overVolt)
			 &&(1 == sccw2.field.fault_recover))
			{
				emsw1.field.V12_overVolt = 0;
			}
		}

		if(sysFbkPara.V12_fbk_HMI < EV_MCU_Para.field.Motor_Protect_V12Lv)
		{
			emsw1.field.V12_lackVolt = 1;
		}
		else
		{
			if((1 == emsw1.field.V12_lackVolt)
			 &&(1 == sccw2.field.fault_recover))
			{
				emsw1.field.V12_lackVolt = 0;
			}
		}
	}
}

void trqFeedback(void)
{
	if(1 == scsw2.field.igbt_state)
	{
		trqLoopPara.Torque_fbk_HMI = trqLoopPara.Torque_ref_HMI;
	}
	else
	{
		trqLoopPara.Torque_fbk_HMI = 0;
	}
}

_iq tempLookupMtrTable(Uint16 temp_type,_iq temp_ad,const int* table)
{
	int  i = 0;
	_iq tempa = 0;
	_iq tempc = 0;
	_iq tempd = 0;
	_iq tempe = 0;
	
	switch(temp_type)
	{
		case TEMPSENSOR_PT100:
			tempa = temp_ad;//????AD
			tempd = table[52];//210???AD-???AD?
			if(tempa>tempd)//????210?
			{
				i = 51;//205????
				while(0 != i)
				{
					tempc = table[i];//???????AD?
					tempe = i*50-500;//???????(???10?)
					if(tempc<tempa)
					{
						tempd = tempc;
						i--;
					}
					else if(tempc>tempa)
					{
						tempa = tempa-tempc;
						tempc = tempd-tempc;
						tempa = (tempa*50)/tempc+tempe;
						return tempa;
					}
					else
					{
						tempa = tempe;
						return tempa;
					}
				}
				tempa = -500;//??-50?(???10?)
				return tempa;
			}
			else
			{
				tempa = 2100;
				return tempa;//??210?(???10?)
			}
			//break;
		case TEMPSENSOR_PT1000:
			tempa = temp_ad;//????AD
			tempd = table[52];//210???AD-???AD?
			if(tempa>tempd)//????210?
			{
				i = 51;//205????
				while(0 != i)
				{
					tempc = table[i];//???????AD?
					tempe = i*50-500;//???????(???10?)
					if(tempc<tempa)
					{
						tempd = tempc;
						i--;
					}
					else if(tempc>tempa)
					{
						tempa = tempa-tempc;
						tempc = tempd-tempc;
						tempa = (tempa*50)/tempc+tempe;
						return tempa;
					}
					else
					{
						tempa = tempe;
						return tempa;
					}
				}
				tempa = -500;//??-500?(???10?)
				return tempa;
			}
			else
			{
				tempa = 2100;
				return tempa;//??210?(???10?)
			}
			//break;
		case TEMPSENSOR_NTC_10K:
			tempa = temp_ad;//????AD
			tempd = table[250];//210???AD-???AD?
			if(tempa<tempd)//????210?
			{
				i = 249;//205????
				while(0 != i)
				{
					tempc = table[i];//???????AD?
					tempe = i*10-400;//???????(???10?)
					if(tempc>tempa)
					{
						tempd = tempc;
						i--;
					}
					else if(tempc<tempa)
					{
						tempa = tempa-tempc;
						tempc = tempd-tempc;
						tempa = (tempa*10)/tempc+tempe;
						return tempa;
					}
					else
					{
						tempa = tempe;
						return tempa;
					}
				}
				tempa = -400;//??-50?(???10?)
				return tempa;
			}
			else
			{
				tempa = 2100;
				return tempa;//??210?(???10?)
			}
			//break;
		case TEMPSENSOR_NTC_30K:
			tempa = temp_ad;//????AD
			tempd = table[50];//210???AD-???AD?
			if(tempa<tempd)//????210?
			{
				i = 49;//205????
				while(0 != i)
				{
					tempc = table[i];//???????AD?
					tempe = i*50-400;//???????(???10?)
					if(tempc>tempa)
					{
						tempd = tempc;
						i--;
					}
					else if(tempc<tempa)
					{
						tempa = tempa-tempc;
						tempc = tempd-tempc;
						tempa = (tempa*50)/tempc+tempe;
						return tempa;
					}
					else
					{
						tempa = tempe;
						return tempa;
					}
				}
				tempa = -400;//??-500?(???10?)
				return tempa;
			}
			else
			{
				tempa = 2100;
				return tempa;//??210?(???10?)
			}
			//break;
		case TEMPSENSOR_NTC_100K:
			tempa = temp_ad;//????AD
			tempd = table[151];//210???AD-???AD?
			if(tempa<tempd)//????210?
			{
				i = 150;//205????
				while(0 != i)
				{
					tempc = table[i];//???????AD?
					tempe = i*10-300;//???????(???10?)
					if(tempc>tempa)
					{
						tempd = tempc;
						i--;
					}
					else if(tempc<tempa)
					{
						tempa = tempa-tempc;
						tempc = tempd-tempc;
						tempa = (tempa*10)/tempc+tempe;
						return tempa;
					}
					else
					{
						tempa = tempe;
						return tempa;
					}
				}
				tempa = -300;//??-500?(???10?)
				return tempa;
			}
			else
			{
				tempa = 1250;
				return tempa;//??210?(???10?)
			}
			//break;
		case TMP_MTR_NTC_15C054:
			tempa = temp_ad;//????AD
			tempd = table[250];//210???AD-???AD?
			if(tempa<tempd)//????210?
			{
				i = 249;//205????
				while(0 != i)
				{
					tempc = table[i];//???????AD?
					tempe = i*10-400;//???????(???10?)
					if(tempc>tempa)
					{
						tempd = tempc;
						i--;
					}
					else if(tempc<tempa)
					{
						tempa = tempa-tempc;
						tempc = tempd-tempc;
						tempa = (tempa*10)/tempc+tempe;
						return tempa;
					}
					else
					{
						tempa = tempe;
						return tempa;
					}
				}
				tempa = -400;//??-50?(???10?)
				return tempa;
			}
			else
			{
				tempa = 2100;
				return tempa;//??210?(???10?)
			}
			//break;
		case TEMPSENSOR_KTY84_150:
			tempa = temp_ad;//????AD
			tempd = table[24];//210???AD-???AD?
			if(tempa>tempd)//????210?
			{
				i = 23;//205????
				while(0 != i)
				{
					tempc = table[i];//???????AD?
					tempe = i*100-400;//???????(???10?)
					if(tempc<tempa)
					{
						tempd = tempc;
						i--;
					}
					else if(tempc>tempa)
					{
						tempa = tempa-tempc;
						tempc = tempd-tempc;
						tempa = (tempa*100)/tempc+tempe;
						return tempa;
					}
					else
					{
						tempa = tempe;
						return tempa;
					}
				}
				tempa = -400;//??-50?(???10?)
				return tempa;
			}
			else
			{
				tempa = 1900;
				return tempa;//??210?(???10?)
			}
		default:
			tempa = 2100;
			return tempa;//??210?(???10?)
	}
}

_iq tempLookupTable(_iq temp,const int* table)
{
	int  i = 0;
	_iq tempa = 0;
	_iq tempc = 0;
	_iq tempd = 0;
	_iq tempe = 0;
	
	tempa = temp;//????AD
	tempd = table[165];//295???AD-???AD?
	if(tempa>tempd)//????295?
	{
		i = 164;//115????
		while(0 != i)
		{
			tempc = table[i];//???????AD?
			tempe = i*10-400;//???????(???10?)
			if(tempc<tempa)
			{
				tempd = tempc;
				i--;
			}
			else if(tempc>tempa)
			{
				tempa = tempc-tempa;
				tempc = tempc-tempd;
				tempa = (tempa*10)/tempc+tempe;
				return tempa;
			}
			else
			{
				tempa = tempe;
				return tempa;
			}
		}
		tempa = -400;//??-30?(???10?)
		return tempa;
	}
	else
	{
		tempa = 1250;
		return tempa;//??1200?(???10?)
	}
}

void tempFeedback(void)
{
	_iq temp = 0;
	
	sysFbkPara.Temp_mtr_ad          = (sysFbkPara.Temp_mtr_ad*EV_MCU_Para.field.Mtrtemp_adjustCoeff)>>10;
	sysFbkPara.Temp_mdl_ad_filter	= (sysFbkPara.Temp_mdl_ad*28+sysFbkPara.Temp_mdl_ad_filter*100)>>7;
	sysFbkPara.Temp_mtr_ad_filter	= (sysFbkPara.Temp_mtr_ad*28+sysFbkPara.Temp_mtr_ad_filter*100)>>7;
	sysFbkPara.TA_ad_filter         = (sysFbkPara.TA_ad*28+sysFbkPara.TA_ad_filter*100)>>7;
	
	temp = tempLookupTable(sysFbkPara.Temp_mdl_ad_filter,&TMP_NTC_TAB[0]);
	sysFbkPara.Temp_igbt = temp/10;
	temp = tempLookupTable(sysFbkPara.TA_ad_filter,&TMP_NTC_TAB[0]);
	sysFbkPara.Temp_ta = temp/10;
	sysFbkPara.Temp_mdl = sysFbkPara.Temp_igbt;//max(sysFbkPara.Temp_igbt,sysFbkPara.Temp_ta);
	
	if(motor_sys_init_cnt>=250)
	{
		if(sysFbkPara.Temp_mdl_ad_filter<1140)
		{
			emsw1.field.mdlsensor_short_error = 1;
		}
		else if(sysFbkPara.Temp_mdl_ad_filter>3354)
		{
			emsw1.field.mdlsensor_open_error = 1;
		}
		else
		{
			if(sysFbkPara.Temp_mdl > EV_MCU_Para.field.Motor_Protect_OverTempCtr)
			{
				emsw.field.mdl_temp = 1;
			}
			else
			{
				if((1 == emsw.field.mdl_temp)
				 &&(1 == sccw2.field.fault_recover))
				{
					emsw.field.mdl_temp = 0;
				}
			}
		}
	}

    switch (EV_MCU_Para.field.Motor_TempSensor_type)
	{
		case TEMPSENSOR_PT100:
			temp = tempLookupMtrTable(TEMPSENSOR_PT100,sysFbkPara.Temp_mtr_ad_filter,&TMP_MTR_PT100_TAB[0]);
			sysFbkPara.Temp_mtr = temp/10;
			break;
		case TEMPSENSOR_PT1000:
			temp = tempLookupMtrTable(TEMPSENSOR_PT1000,sysFbkPara.Temp_mtr_ad_filter,&TMP_MTR_PT1000_TAB[0]);
			sysFbkPara.Temp_mtr = temp/10;
			break;
		case TEMPSENSOR_NTC_10K:
			temp = tempLookupMtrTable(TEMPSENSOR_NTC_10K,sysFbkPara.Temp_mtr_ad_filter,&TMP_MTR_NTC_10K_TAB[0]);
			sysFbkPara.Temp_mtr = temp/10;
			break;
		case TEMPSENSOR_NTC_30K:
			temp = tempLookupMtrTable(TEMPSENSOR_NTC_30K,sysFbkPara.Temp_mtr_ad_filter,&TMP_MTR_NTC_30K_TAB[0]);
			sysFbkPara.Temp_mtr = temp/10;
			break;
		case TEMPSENSOR_NTC_100K:
			temp = tempLookupMtrTable(TEMPSENSOR_NTC_100K,sysFbkPara.Temp_mtr_ad_filter,&TMP_MTR_NTC_100K_TAB[0]);
			sysFbkPara.Temp_mtr = temp/10;
			break;
		case TMP_MTR_NTC_15C054:
			temp = tempLookupMtrTable(TMP_MTR_NTC_15C054,sysFbkPara.Temp_mtr_ad_filter,&TMP_MTR_NTC_15C054_TAB[0]);
			sysFbkPara.Temp_mtr = temp/10;
			break;
		case TEMPSENSOR_KTY84_150:
			temp = tempLookupMtrTable(TEMPSENSOR_KTY84_150,sysFbkPara.Temp_mtr_ad_filter,&TMP_MTR_KTY84_150_TAB[0]);
			sysFbkPara.Temp_mtr = temp/10;
			break;
		default:
			sysFbkPara.Temp_mtr=sysFbkPara.Temp_mdl+3;
			break;
	}

	if((1 == sccw2.field.mtrtempFbk_ChkMode)
	 &&(motor_sys_init_cnt>=250))
	{
		if(sysFbkPara.Temp_mtr_ad_filter > 3480)//����¶ȶ�·����
		{
			sysFbkPara.Temp_mtr         = -50;
			emsw1.field.mtrsensor_open_error = 1;
		}
		else if(sysFbkPara.Temp_mtr_ad_filter<880)//����¶ȶ�·����
		{
			sysFbkPara.Temp_mtr         = 210;
			emsw1.field.mtrsensor_short_error = 1;
		}
		else
		{
			if (sysFbkPara.Temp_mtr > EV_MCU_Para.field.Motor_Protect_OverTempMtr)
			{
				emsw.field.mtr_temp = 1;
			}
			else
			{
				if((1 == emsw.field.mtr_temp)
				 &&(1 == sccw2.field.fault_recover))
				{
					emsw.field.mtr_temp = 0;
					emsw1.field.mtrsensor_short_error=0;
					emsw1.field.mtrsensor_open_error=0;
				}
			}
			
			if(sysFbkPara.Temp_mtr>EV_MCU_Para.field.Motor_Protect_AlarmTempMtr)
			{
				GpioDataRegs.GPACLEAR.bit.GPIO9 = 1;
			}
			else if(sysFbkPara.Temp_mtr<(EV_MCU_Para.field.Motor_Protect_AlarmTempMtr-5))
			{
				GpioDataRegs.GPASET.bit.GPIO9 = 1;
			}
		}
	}
	else
	{
		emsw.field.mtr_temp = 0;
		emsw1.field.mtrsensor_short_error=0;
		emsw1.field.mtrsensor_open_error=0;
	}
	if(LimitI2CUpdate_en)
	{
		if(I2CValueLimit_Record.field.mdl_temp < sysFbkPara.Temp_mdl)
		{
			I2CValueLimit_Record.field.mdl_temp = sysFbkPara.Temp_mdl;
			eeprom_valuelimit_index[5]    = 1;
			LimitI2CRecord_en             = 1;
		}
		if(I2CValueLimit_Record.field.mtr_temp < sysFbkPara.Temp_mtr)
		{
			I2CValueLimit_Record.field.mtr_temp = sysFbkPara.Temp_mtr;
			eeprom_valuelimit_index[6]    = 1;
			LimitI2CRecord_en             = 1;
		}
	}
}

#pragma CODE_SECTION(Resolver_read, 	"ramfuncs");
void Resolver_read(void)
{	
}

#pragma CODE_SECTION(ThetaCal_Resolver, 	"ramfuncs");
void ThetaCal_Resolver()
{
	_iq fTemp = 0;
	_iq fTemp1 = 0;
	_iq dir_encoder = 0;
	_iq Tmp = 0;
	
	if(MOTOR_CLOCKWISE == EV_MCU_Para.field.Motor_Encoder_Direction)
	{
		sysFbkPara.Theta_fbk_raw = mtrPara.Encoder_rollOver.value - sysFbkPara.Theta_fbk_raw;
	}
	fTemp = _IQmpy((sysFbkPara.Theta_fbk_raw<<12),SysBase.mtr_ratio);
	Theta_fbk_raw_n_1 = Theta_fbk_raw_n;
	Theta_fbk_raw_n = fTemp;
	Theta_fbk_raw_deta_n_1 = Theta_fbk_raw_deta_n;
	Theta_fbk_raw_deta_n = Theta_fbk_raw_n-Theta_fbk_raw_n_1;
	if(1 == resolver_fault_type)
	{
		Tmp = Theta_fbk_raw_deta_n-Theta_fbk_raw_deta_n_1;//IQ24
		if(Tmp>=_IQ(0.5))
		{
			Tmp = Tmp-_IQ(1.0);
		}
		else if(Tmp<=-_IQ(0.5))
		{
			Tmp = Tmp+_IQ(1.0);
		}
		Theta_fbk_raw_deta_err = _IQabs(Tmp);//IQ24abs	
		if(Theta_fbk_raw_deta_err>_IQ12toIQ(EV_MCU_Para.field.resolver_fault_theta_maxerr))
		{
			Theta_fbk_fault_cnt++;
		}
		Theta_fbk_sample_cnt++;
		if(Theta_fbk_sample_cnt>=100)
		{
			Theta_fbk_sample_cnt = 0;
			if(Theta_fbk_fault_cnt>=10)
			{
				emsw.field.resolver_error = 1;
			}
			Theta_fbk_fault_cnt = 0;
		}
	}
	else
	{
		Theta_fbk_sample_cnt = 0;
		Theta_fbk_fault_cnt = 0;
	}

	if((MTR_VOLT_LOOP != scsw2.field.runLoop_state)
	 &&(SYS_DIAGNOSIS != sysState))
	{
		dir_encoder = Motor_Encoder_offset-Motor_Encoder_offset_current;//IQ15
		if(Motor_Encoder_offset<Motor_Encoder_offset_current)
		{
			if(_IQabs(dir_encoder)<_IQ15(0.5))
			{
				Motor_Encoder_cnt++;
				if(Motor_Encoder_cnt>=1)
				{
					Motor_Encoder_cnt = 0;
					Motor_Encoder_offset = Motor_Encoder_offset+8;
				}
			}
			else
			{
				Motor_Encoder_cnt++;
				if(Motor_Encoder_cnt>=1)
				{
					Motor_Encoder_cnt = 0;
					Motor_Encoder_offset = Motor_Encoder_offset-8;
				}
			}
		}
		else if(Motor_Encoder_offset>Motor_Encoder_offset_current)
		{
			if(_IQabs(dir_encoder)<_IQ15(0.5))
			{
				Motor_Encoder_cnt++;
				if(Motor_Encoder_cnt>=1)
				{
					Motor_Encoder_cnt = 0;
					Motor_Encoder_offset = Motor_Encoder_offset-8;
				}
			}
			else
			{
				Motor_Encoder_cnt++;
				if(Motor_Encoder_cnt>=1)
				{
					Motor_Encoder_cnt = 0;
					Motor_Encoder_offset = Motor_Encoder_offset+8;
				}
			}
		}
		else
		{
			Motor_Encoder_cnt = 0;
		}
		fTemp1 = Motor_Encoder_offset+((Motor_Encoder_offset_coeff-2048)<<3);//IQ15		
		sysFbkPara.Theta_fbk_Ele = fTemp - (fTemp1<<9);//IQ24
	}
	else
	{
		fTemp1 = EV_MCU_Para.field.Motor_Encoder_offset+(Motor_Encoder_offset_coeff-2048);
		sysFbkPara.Theta_fbk_Ele = fTemp - (fTemp1<<12);//IQ24
	}
	while((sysFbkPara.Theta_fbk_Ele >= _IQ(1))
		||(sysFbkPara.Theta_fbk_Ele < 0))
	{
		if (sysFbkPara.Theta_fbk_Ele >= _IQ(1))
		{
			sysFbkPara.Theta_fbk_Ele -= _IQ(1);
		}
		else if (sysFbkPara.Theta_fbk_Ele < 0)
		{
			sysFbkPara.Theta_fbk_Ele += _IQ(1);
		}
	}
	sysFbkPara.Theta_fbk_Ele_HMI = sysFbkPara.Theta_fbk_Ele>>12;//IQ12
}

//#pragma CODE_SECTION(SpeedCal_Resolver, 	"ramfuncs");
void SpeedCal_Resolver()
{
	_iq	fTemp1 = 0;

	SpeedCal_Count++;
	if(SpeedCal_Count>=EV_MCU_Para.field.SpdLoop_Cycle)
	{
		SpeedCal_Count = 0;
		SpeedCal_ThetaEle = sysFbkPara.Theta_fbk_Ele;
		if(0 == SpeedCal_First_Flag)
		{
			SpeedCal_First_Flag = 1;
			SpeedCal_ThetaEleOld = SpeedCal_ThetaEle;
		}
		SpeedCal_deltaTime = _IQmpyI32(sysCfgPara.Omega_period.value,EV_MCU_Para.field.SpdLoop_Cycle);
		SpeedCal_deltaTheta     = SpeedCal_ThetaEle - SpeedCal_ThetaEleOld;
		SpeedCal_ThetaEleOld    = SpeedCal_ThetaEle;
		if (SpeedCal_deltaTheta > _IQ(0.5))
		{
			SpeedCal_deltaTheta -= _IQ(1);
		}
		else if (SpeedCal_deltaTheta < -_IQ(0.5))
		{
			SpeedCal_deltaTheta += _IQ(1);
		}

		fTemp1			= _IQmpyI32(SysBase.invomega,1000);  //ʱ������msΪ��С��λ������Ҫ��1000
		SpeedCal_spdFbk = _IQdiv(_IQmpy(SpeedCal_deltaTheta,fTemp1),(SpeedCal_deltaTime*EV_MCU_Para.field.Motor_Np)); //�ٶȼ���

		if(MOTOR_CLOCKWISE == EV_MCU_Para.field.Motor_Direction)
	    {
	    	SpeedCal_spdFbk = 0-SpeedCal_spdFbk;
	    }
		sysFbkPara.Speed_fbk = SpeedCal_spdFbk;
		sysFbkPara.Omega_fbk_Mech = sysFbkPara.Speed_fbk;
		sysFbkPara.Omega_fbk_Ele = _IQmpyI32(sysFbkPara.Omega_fbk_Mech , (long)EV_MCU_Para.field.Motor_Np);
		LPFilter(sysFbkPara.Omega_fbk_Mech,&sysFbkPara.Speed_fbk_Filter,_IQ(0.1),_IQ(0.9));
		if(_IQabs(sysFbkPara.Speed_fbk_Filter) < SysBase.invspeed)
		{
			sysFbkPara.Speed_fbk_Filter = 0;
		}
		if(_IQabs(sysFbkPara.Speed_fbk_Filter)>(_IQ15toIQ(EV_MCU_Para.field.Motor_PeakSpeedFwd)+_IQ(0.05)))
		{
			emsw2.field.data_overflow = 1;
		}
		else
		{
			if((emsw2.field.data_overflow == 1)&&(sccw2.field.fault_recover == 1))
			{
				emsw2.field.data_overflow = 0;
			}
		}
	    sysFbkPara.Speed_fbk_HMI = _IQtoIQ15(sysFbkPara.Speed_fbk_Filter);
	    sysFbkPara.Speed_fbk_HMI_abs = _IQ15abs(sysFbkPara.Speed_fbk_HMI);
		sysFbkPara.Speed = _IQ15mpy(sysFbkPara.Speed_fbk_HMI,SysBase.speed);
		sysFbkPara.Speed_abs = _IQabs(sysFbkPara.Speed);
	    SpeedFbk_FaultChecking();
	    SpeedDetectProtect();
		if(LimitI2CUpdate_en)
		{
			fTemp1 = sysFbkPara.Speed_abs;
			if(I2CValueLimit_Record.field.speed < fTemp1)
			{
				I2CValueLimit_Record.field.speed = fTemp1;
				eeprom_valuelimit_index[3]    = 1;
				LimitI2CRecord_en             = 1;
			}
		}
#if 0
		LPFilter(sysFbkPara.Speed_fbk,&sysFbkPara.Speed_fbk_Filter_Lowspeed,_IQ(0.005),_IQ(0.995));
#else
		LPFilter(sysFbkPara.Speed_fbk,&sysFbkPara.Speed_fbk_Filter_Lowspeed,_IQ(0.3),_IQ(0.7));
		LPFilter(sysFbkPara.Speed_fbk,&Speed_fbk_Filter_Stop_Lowspeed,_IQ(0.005),_IQ(0.995));
#endif
	}
}

#endif

