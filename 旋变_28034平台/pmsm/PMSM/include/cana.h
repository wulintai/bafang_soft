#ifndef _CANA_H_
#define _CANA_H_

typedef struct
{
	union
	{
		struct
		{
			unsigned feedback_flag		   :1;	//0:�عܣ�    1������
			unsigned gear_swap_trq_flag   :2;  //0:������    1���ƶ�
			unsigned sport_mode_flag       :1;
			unsigned eco_mode_flag         :1;
			unsigned zero_station_free_flag         :2;  //0:���ػ��� 1���ٶȻ��� 2��������
			unsigned gear_state            :2;  //0:�յ�        1��ǰ��        2������
			unsigned slip_feedback_flag    :1;  //0:����λ��1����λ
			unsigned brake_feedback_flag   :1;  //0:�޶�ǿ�磬1����ǿ��
			unsigned gear_swap_zero_flag       :1;  //0:����ɲ��  1������ɲ
			unsigned ready_state           :1;  //0:Readyδ׼���� 1:Ready׼����
			unsigned key_state             :2;  //0:OFF 1:ACC 2:ON 3:START
			unsigned brake_state           :1;  //ɲ���ź�
		}data;
		Uint16 Value;
	}cmdmode;
	int torque_ref;
}tag_Vehicle_cmd;

typedef struct
{
	unsigned canMBX0index;
	int canMBX0value;
	int canMBX0value1;
	/*unsigned canMBX1index;
	int canMBX1value;
	unsigned canMBX2index;
	int canMBX2value;
	unsigned canMBX3index;
	int canMBX3value;
	unsigned canMBX4index;
	int canMBX4value;
	unsigned canMBX5index;
	int canMBX5value;*/
}tag_debug_cmd;

typedef struct
{
	unsigned cnt;
	unsigned prd;
	unsigned flag;
	unsigned txEnable;
	unsigned txBufFul;
	unsigned Enable;
}tag_mbxCfg;

typedef union
{
	struct
	{
		unsigned reserved1              :1;  //Ԥ��
		unsigned gear_state             :2;  //��λ
		unsigned drive_state            :2;  //ǰ������
		unsigned seat_state             :1;  //���ο���
		unsigned safe_state             :1;  //��ȫ��
		unsigned brk_state              :1;  //�ֽ�ɲ��
		
		unsigned speed_mode              :2;  //����ģʽ
		unsigned reserved2               :6;  //Ԥ��
	}data;
	unsigned wValue;
}tag_canafbk_state;

typedef union
{
	struct
	{
		unsigned reserved1            :2;  //Ԥ��
		unsigned gear_cmd             :2;  //��λָ��
		unsigned reserved2            :3;  //Ԥ��
		unsigned gear_fault_flag      :1;  //��Ч����
		
		unsigned vehicle_run_enable_state    :1;//��������ʻ״̬
		unsigned reserved3                   :2;//Ԥ��
		unsigned vehicle_speed_effect_state  :1;//������Чλ
		unsigned reserved4                   :4;//Ԥ��
	}data;
	unsigned wValue;
}tag_canafbk_state2;

typedef union
{
	struct
	{
		unsigned brake_signal            :1;  //�ƶ��ź�
		unsigned brake_fault             :1;  //�ƶ�ϵͳ����
		unsigned long_mode               :1;  //Longģʽ
		unsigned eco_mode                :1;  //ECOģʽ
		unsigned power_lmt_signal        :1;  //�޹����ź�
		unsigned reserved1               :2;  //Ԥ��
		unsigned ready                   :1;  //ready
		
		unsigned ai_selfcheck            :1;  //����̤���Լ�
		unsigned gen_state               :1;  //�ƶ�״̬
		unsigned sport_mode              :1;  //Sportģʽ
		unsigned reserved2               :5;//Ԥ��
	}data;
	unsigned wValue;
}tag_canafbk_state3;

typedef union
{
	struct
	{
		unsigned reserved1                                  :8;

		unsigned reserved2                                  :2;
		unsigned mtr_lackphase                              :1;
		unsigned reserved3                                  :1;
		
		unsigned handbrake_pull_fault                       :1;
		unsigned handbrake_free_fault                       :1;
		unsigned reserved4                                  :2;
		
	}data;
	unsigned wValue;
}tag_MCU_EER_STATUS3;

typedef union
{
	struct
	{
		unsigned eeprom_fault         				        :1;
		unsigned eeprom_overflow_fault            			:1;
		unsigned eeprom_modify_fault                        :1;
		unsigned can_fault                                  :1;

		unsigned dsp_eeprom_fault                           :1;
		unsigned diagstudy_error                            :1;
		unsigned reserved1                                  :1;
		unsigned acc_fault                                  :1;

		unsigned mtr_sensor_fault                           :1;
		unsigned v5_fault                                   :1;
		unsigned external_current_fault                     :1;
		unsigned v12_fault                                  :1;

		unsigned acc_output_over                            :1;
		unsigned reserved2                                  :1;
		unsigned acc_brake_over_current                     :1;
		unsigned high_pedal_fault                           :1;
	}data;
	unsigned wValue;
}tag_MCU_EER_STATUS2;

typedef union
{
	struct
	{
		unsigned udc_lackvolt                               :1;
		unsigned udc_ovvolt             				    :1;
		unsigned udc_ovvolt_heavy          				    :1;
		unsigned precharge_fault                            :1;
		
		unsigned main_relay_adhesion                        :1;
		unsigned main_relay_close_failure                   :1;
		unsigned main_relay_openshort                       :1;
		unsigned mtr_temp                                   :1;
		
		unsigned mcu_overcurrent                            :1;
		unsigned current_sensor_fault                       :1;
		unsigned resolver_fault                             :1;
		unsigned open_phase                                 :1;
		
		unsigned mdl_temp_low_heavy                         :1;
		unsigned mdl_temp_high                              :1;
		unsigned mdl_temp_high_heavy                        :1;
		unsigned udc_lackvolt_heavy                         :1;
	}data;
	unsigned wValue;
}tag_MCU_EER_STATUS1;

typedef struct
{
	union
	{
		struct
		{
			unsigned speed_mode                      :2;  //����ģʽ
			unsigned reserved1                       :6;  //Ԥ��

			unsigned reserved2                       :8;  //Ԥ��
		}data;
		Uint16 wValue;
	}cmdmode1;
	union
	{
		struct
		{
			unsigned reserved1                       :2;  //Ԥ��
			unsigned charge_state                    :2;  //���״̬
			unsigned charge_line_state               :2;  //�����״̬
			unsigned BMS_self_state                  :2;  //BMS�Լ�״̬

			unsigned reserved2                       :5;  //Ԥ��
			unsigned charge_relay_state              :1;  //��̵���״̬
			unsigned reserved3                       :1;//Ԥ��
			unsigned main_relay_state                :1;//�����̵���״̬
		}data;
		Uint16 wValue;
	}cmdmode2;
	union
	{
		struct
		{
			unsigned insolution_fault                          :2;  //��Ե����
			unsigned battery_temp_low                          :2;  //����¶ȵ�
			unsigned battery_temp_high                         :2;  //����¶ȸ�
			unsigned battery_temp_diff                         :2;  //�²����

			unsigned monomer_voltage_over                      :2;  //�����ѹ����
			unsigned monomer_voltage_diff                      :2;  //����ѹ�����
			unsigned discharge_current_over                    :2;  //�ŵ��������
			unsigned charge_current_over                       :2;  //����������
		}data;
		Uint16 wValue;
	}errmode1;
	union
	{
		struct
		{
			unsigned soc_lack                                  :2;  //SOC����
			unsigned battery_voltage_low                       :2;  //�粨�ܵ�ѹ����
			unsigned battery_voltage_high                      :2;  //����ܵ�ѹ����
			unsigned monomer_voltage_lack                      :2;  //�����ѹ����

			unsigned precharge_fault                           :1;  //Ԥ������
			unsigned precharge_relay_adhesion_fault            :1;  //Ԥ��Ӵ���ճ������
			unsigned heat_relay_adhesion_fault                 :1;  //���ȽӴ���ճ������
			unsigned main_positive_relay_adhesion_fault        :1;  //�����Ӵ���ճ������
			unsigned power_battery_pack_fault                  :1;  //�������ذ���ƥ��
			unsigned reserved1                                 :1;  //Ԥ��
			unsigned can_fault                                 :1;  //����CANͨ�Ŷ�ʧ
			unsigned reserved2                                 :1;  //Ԥ��
		}data;
		Uint16 wValue;
	}errmode2;
	union
	{
		struct
		{
			unsigned reserved1                                 :4;  //Ԥ��
			unsigned BMS_selfcheck                             :1;  //BMS�Լ�
			unsigned feedback_current_over                     :1;  //������������
			unsigned reserved2                                 :2;  //Ԥ��

			unsigned reserved3                                 :6;  //Ԥ��
			unsigned soc_over                                  :2;  //SOC����
		}data;
		Uint16 wValue;
	}errmode3;
	Uint16 speed_mode;//����ģʽ
	Uint16 soc_ref;//ʣ�����SOC
}tag_canarev_cmd;

#ifdef _CANA_C_

	tag_Vehicle_cmd 	Vehicle_cmd = {0};
	tag_canafbk_state	canafbk_state = {0};
	tag_canafbk_state3  canafbk_state3 = {0};
	tag_canafbk_state2  canafbk_state2 = {0};
	tag_MCU_EER_STATUS3 MCU_err_status3 = {0};
	tag_MCU_EER_STATUS2 MCU_err_status2 = {0};
	tag_MCU_EER_STATUS1 MCU_err_status1 = {0};
	tag_canarev_cmd     canarev_cmd = {0};
	Uint16 display_var_index = 0;
	
	void CANA_init(void);
	void canaunpackMbx16(void);//4180
	void canaunpackMbx17(void);
	void canadebugMbxSlotCtrlint(void);
	void canaMbxSlotCtrl(void);
	void canatransmit(void);
	void canareceive(void);
	void packcanaMbx8(void);
	void packcanaMbx9(void);
	void packcanaMbx10(void);
	void packcanaMbx11(void);
	void packcanaMbx12(void);
	void packcanaMbx13(void);
	void packcanaMbx14(void);
	void packcanaMbx15(void);
	void packcanaMbx18(void);
	void packcanaMbx19(void);
	void packcanaMbx20(void);
	void packcanaMbx21(void);
	void packcanaMbx22(void);
	void packcanaMbx23(void);
	void packcanaMbx24(void);
	void packcanaMbx25(void);
	void packcanaMbx26(void);
	void packcanaMbx27(void);
	void packcanaMbx28(void);
	void packcanaMbx29(void);
	void packcanaMbx30(void);
	tag_debug_cmd canb_cmd = {0};
	Uint16 can_debugmode = 0;
	int canaerr_cnt = 0;
	Uint16 can_tbox_cnt = 0;
	tag_mbxCfg canambxCfg[21] = {0};
	Uint16 eeprom_displaycnt_cana = 0;
	Uint16 eeprom_error_recall_cnt_cana = 0;
	Uint16 eeprom_ValueLimit_cnt_cana = 0;
	Uint16 bootcandate[8] = {0};
#else
	extern 	void CANA_init(void);
	extern void canaunpackMbx16(void);//4180
	extern void canaunpackMbx17(void);
	extern void canadebugMbxSlotCtrlint(void);
	extern void canaMbxSlotCtrl(void);
	extern void canatransmit(void);
	extern void canareceive(void);
	extern tag_canarev_cmd     canarev_cmd;
	extern tag_Vehicle_cmd 	Vehicle_cmd;
	extern tag_debug_cmd canb_cmd;
	extern Uint16 can_debugmode;
	extern int canaerr_cnt;
	extern Uint16 bootcandate[8];
#endif

#endif
