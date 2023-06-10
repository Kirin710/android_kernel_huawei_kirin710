/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2017-2019. All rights reserved.
 * Description: this file provide initialization of the ivp dev
 * Author: chenweiyu
 * Create: 2017-02-18
 */

#ifndef _IVP_PLATFORM_H_
#define _IVP_PLATFORM_H_

#include <linux/fs.h>
#include <linux/atomic.h>
#include <linux/device.h>
#include <linux/miscdevice.h>
#include <linux/platform_device.h>
#include <linux/regulator/consumer.h>
#include "ivp.h"

#define LISTENTRY_SIZE                  0x00600000
#define GIC_IRQ_CLEAR_REG               0xE82B11A4

#define IVP_MEM_CTRL3_VAL               0x02600260
#define IVP_MEM_CTRL4_VAL               0x00000260
#define IVP_MST_MID_CFG_VAL             0x00006A69

#define IVP_APB_GATE_CLOCK_VAL          0x00003FFF
#define IVP_TIMER_WDG_RST_DIS_VAL       0x0000007F

#define RST_IVP32_PROCESSOR_EN          0x02
#define RST_IVP32_DEBUG_EN              0x01
#define RST_IVP32_JTAG_EN               0x04

#define MAX_DDR_LEN                     128

struct ivp_iomem_res {
	char __iomem *cfg_base_addr;
	char __iomem *pctrl_base_addr;
	char __iomem *pericrg_base_addr;
	char __iomem *gic_base_addr;
};

struct ivp_device {
	struct ivp_iomem_res io_res;
	struct clk *clk;
	unsigned int clk_rate;
	unsigned int clk_level;
	unsigned int clk_usrsetrate;
	unsigned int lowfrq_pd_clk_rate;
	unsigned int wdg_irq;
	atomic_t wdg_sleep;
	unsigned int dwaxi_dlock_irq;
	struct semaphore wdg_sem;
	int sect_count;
	struct ivp_sect_info *sects;
	struct ivp_sect_info *sec_sects;
	struct dentry *debugfs;
	struct miscdevice device;
	struct regulator *regulator;
	struct regulator *ivp_fake_regulator;
	unsigned long smmu_pgd_base;
	struct ivp_smmu_dev *smmu_dev;
	atomic_t accessible;
	atomic_t poweron_access;
	atomic_t poweron_success;
	void *vaddr_memory;
	struct wakeup_source ivp_power_wakelock;
	struct mutex ivp_wake_lock_mutex;
	struct mutex ivp_load_image_mutex;
	struct mutex ivp_power_up_off_mutex;
	struct mutex ivp_open_release_mutex;
	int ivp_meminddr_len;
	unsigned int dynamic_mem_size;
	unsigned int dynamic_mem_section_size;
	unsigned int ivp_sec_support;
	unsigned int ivp_secmode;
	int ivp_sec_buff_fd;
	struct platform_device *ivp_pdev;
};

int ivp_poweron_pri(struct ivp_device *ivp_devp);
int ivp_poweron_remap(struct ivp_device *ivp_devp);
int ivp_poweroff_pri(struct ivp_device *ivp_devp);
int ivp_init_pri(struct platform_device *plat_dev, struct ivp_device *ivp_devp);
void ivp_deinit_pri(struct ivp_device *ivp_devp);
int ivp_change_clk(const struct ivp_device *ivp_devp, unsigned int level);
int ivp_init_resethandler(const struct ivp_device *ivp_devp);
void ivp_deinit_resethandler(const struct ivp_device *ivp_devp);
int ivp_check_resethandler(const struct ivp_device *ivp_devp);
int ivp_sec_loadimage(const struct ivp_device *ivp_devp);
void ivp_dev_hwa_enable(struct ivp_device *ivp_devp);
void ivp_hw_enable_reset(struct ivp_device *ivp_devp);

#endif /* _IVP_PLATFORM_H_ */
