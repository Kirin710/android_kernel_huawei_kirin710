

#ifndef __BOARD_HI1103_H__
#define __BOARD_HI1103_H__

/* ����ͷ�ļ����� */
#include "plat_type.h"
#include "hw_bfg_ps.h"

/* �궨�� */
#define BOARD_VERSION_NAME_HI1103  "hi1103"
#define BOARD_VERSION_NAME_HI1105  "hi1105"
#define BOARD_VERSION_NAME_HI1102A "hi1102a"

#define GPIO_BASE_ADDR        0X50004000
#define CHECK_DEVICE_RDY_ADDR 0X50000000
#define WLAN_HOST2DEV_GPIO ((unsigned int)(1 << 1))
#define WLAN_DEV2HOST_GPIO ((unsigned int)(1 << 0))

#define GPIO_LEVEL_CONFIG_REGADDR       0x0  /* GPIO�ܽŵĵ�ƽֵ���߻����ͼĴ��� */
#define GPIO_INOUT_CONFIG_REGADDR       0x04 /* GPIO�ܽŵ����ݷ������ */
#define GPIO_TYPE_CONFIG_REGADDR        0x30 /* GPIO�ܽŵ�ģʽ�Ĵ���:IO or INT */
#define GPIO_INT_POLARITY_REGADDR       0x3C /* GPIO�жϼ��ԼĴ��� */
#define GPIO_INT_TYPE_REGADDR           0x38 /* GPIO�жϴ������ͼĴ���:��ƽ��������ش��� */
#define GPIO_INT_CLEAR_REGADDR          0x4C /* GPIO����жϼĴ�����ֻ�Ա��ش������ж���Ч */
#define GPIO_LEVEL_GET_REGADDR          0x50 /* GPIO�ܽŵ�ǰ��ƽֵ�Ĵ��� */
#define GPIO_INTERRUPT_DEBOUNCE_REGADDR 0x48 /* GPIO�ܽ��Ƿ�ʹ��ȥ���� */

/* just for p10+ start */
#define DTS_PROP_TCXO_CTRL_ENABLE "hi110x,tcxo_ctrl_enable"
#define DTS_PROP_GPIO_TCXO_1P95V  "hi110x,gpio_tcxo_1p95v"
#define PROC_NAME_GPIO_TCXO_1P95V "hi110x_tcxo_1p95v"
#define PROC_NAME_GPIO_WLAN_FLOWCTRL  "hi110x_wlan_flowctrl"
/* just for p10+ end */
/* test ssi write bcpu code */
/* EXTERN VARIABLE */
#ifdef PLATFORM_DEBUG_ENABLE
extern int32 device_monitor_enable;
#endif

/* �������� */
extern int32 hi1103_get_board_power_gpio(struct platform_device *pdev);
extern void hi1103_free_board_power_gpio_etc(struct platform_device *pdev);
extern int32 hi1103_board_wakeup_gpio_init_etc(struct platform_device *pdev);
extern void hi1103_free_board_wakeup_gpio_etc(struct platform_device *pdev);
extern int32 hi1103_board_wifi_tas_gpio_init_etc(void);
extern void hi1103_free_board_wifi_tas_gpio_etc(void);
extern int32 hi1103_check_pmu_clk_share(void);
extern int32 hi1103_bfgx_dev_power_on(void);
extern int32 hi1103_bfgx_dev_power_off(void);
extern int32 hi1103_wlan_power_off(void);
extern int32 hi1103_wlan_power_on(void);
extern int32 hi1103_board_power_on(uint32 ul_subsystem);
extern int32 hi1103_board_power_off(uint32 ul_subsystem);
extern int32 hi1103_board_power_reset(uint32 ul_subsystem);
extern int32 hi1103_wifi_subsys_reset(void);
extern void hi1103_bfgx_subsys_reset(void);
extern int32 hi1103_get_board_pmu_clk32k(void);
extern int32 hi1103_get_board_uart_port(void);
extern int32 hi1103_board_ir_ctrl_init(struct platform_device *pdev);
extern int32 hi1103_check_evb_or_fpga(void);
extern int32 hi1103_check_hi110x_subsystem_support(void);
extern int32 hi1103_board_get_power_pinctrl(struct platform_device *pdev);
extern int32 hi1103_get_ini_file_name_from_dts(int8 *dts_prop, int8 *prop_value, uint32 size);
extern void hi1103_chip_power_on(void);
extern void hi1103_bfgx_enable(void);
extern int32 hi1103_wifi_enable(void);
extern void hi1103_chip_power_off(void);
extern void hi1103_bfgx_disable(void);
extern int32 hi1103_wifi_disable(void);
#if defined(CONFIG_PCIE_KIRIN_SLT_HI110X) && defined(CONFIG_HISI_DEBUG_FS)
extern int32 hi1103_pcie_chip_rc_slt_register(void);
extern int32 hi1103_pcie_chip_rc_slt_unregister(void);
#endif
extern int32 hi1103_board_flowctrl_gpio_init_etc(void);
extern void hi1103_free_board_flowctrl_gpio_etc(void);
#ifdef _PRE_HI_DRV_GPIO
extern int PdmGetHwVer(void); /* Get board type. */
extern int32 hitv_get_board_power_gpio(struct platform_device *pdev);
extern int32 hitv_board_wakeup_gpio_init_etc(struct platform_device *pdev);
#endif
#ifdef _PRE_CONFIG_S4_KIRIN_BUGFIX
extern void hi1103_suspend_gpio_etc(void);
extern void hi1103_resume_gpio_etc(void);
#endif
#endif