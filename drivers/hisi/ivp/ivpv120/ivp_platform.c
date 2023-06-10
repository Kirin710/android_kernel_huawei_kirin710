/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2017-2019. All rights reserved.
 * Description: This file implements ivp initialization and control functions
 * Author: chenweiyu
 * Create: 2017-02-18
 */

#include "ivp_platform.h"
#include <linux/of.h>
#include <linux/clk.h>
#include <linux/dma-mapping.h>
#include "ivp_reg.h"
#include "ivp_log.h"
#include "ivp_manager.h"

#define REMAP_ADD                        0xE8D00000
#define DEAD_FLAG                        0xDEADBEEF
#define SIZE_16K                         (16 * 1024)

static void *g_iram = NULL;

static int ivp_get_memory_section(struct platform_device *plat_dev,
	struct ivp_device *ivp_devp)
{
	int ret;
	int i;
	unsigned int size = 0;
	dma_addr_t dma_addr = 0;

	ivp_devp->vaddr_memory = NULL;
	ret = of_property_read_u32(plat_dev->dev.of_node, OF_IVP_DYNAMIC_MEM, &size);
	if ((ret != 0) || (size == 0)) {
		ivp_err("get failed/not use dynamic mem, ret:%d, size:%d", ret, size);
		return -EINVAL;
	}
	ivp_devp->dynamic_mem_size = size;
	ivp_devp->ivp_meminddr_len = (int)size;

	ret = of_property_read_u32(plat_dev->dev.of_node, OF_IVP_DYNAMIC_MEM_SEC_SIZE, &size);
	if ((ret != 0) || (size == 0)) {
		ivp_err("get failed/not use dynamic mem, ret:%d, size:%d", ret, size);
		return -EINVAL;
	}

	ivp_devp->dynamic_mem_section_size = size;
	if ((ivp_devp->dynamic_mem_section_size * (ivp_devp->sect_count - DDR_SECTION_INDEX)) !=
		ivp_devp->dynamic_mem_size) {
		ivp_err("dynamic_mem should be sect_count-3 times dynamic_mem_section");
		return -EINVAL;
	}

	/*lint -save -e598 -e648*/
	dma_set_mask_and_coherent(&ivp_devp->ivp_pdev->dev, DMA_BIT_MASK(64)); /* bit mask is 64 */
	/*lint -restore*/
	ivp_devp->vaddr_memory = dma_alloc_coherent(&ivp_devp->ivp_pdev->dev,
		ivp_devp->dynamic_mem_size, &dma_addr, GFP_KERNEL);
	if (!ivp_devp->vaddr_memory) {
		ivp_err("get vaddr_memory failed");
		return -EINVAL;
	}

	for (i = DDR_SECTION_INDEX; i < ivp_devp->sect_count; i++) {
		if (i == DDR_SECTION_INDEX) {
			ivp_devp->sects[i].acpu_addr = dma_addr >> IVP_MMAP_SHIFT;
		} else {
			ivp_devp->sects[i].acpu_addr =
				((ivp_devp->sects[i - 1].acpu_addr << IVP_MMAP_SHIFT) +
				ivp_devp->sects[i - 1].len) >> IVP_MMAP_SHIFT;
			ivp_devp->sects[i].ivp_addr = ivp_devp->sects[i - 1].ivp_addr +
				ivp_devp->sects[i - 1].len;
		}
		ivp_devp->sects[i].len = ivp_devp->dynamic_mem_section_size;
		ivp_dbg("ivp sections 0x%pK", ivp_devp->sects[i].acpu_addr);
	}

	return ret;
}

static void ivp_free_memory_section(struct ivp_device *ivp_devp)
{
	dma_addr_t dma_addr;

	dma_addr = ivp_devp->sects[DDR_SECTION_INDEX].acpu_addr << IVP_MMAP_SHIFT;
	if (ivp_devp->vaddr_memory) {
		dma_free_coherent(&ivp_devp->ivp_pdev->dev,
			ivp_devp->dynamic_mem_size,
			ivp_devp->vaddr_memory, dma_addr);
		ivp_devp->vaddr_memory = NULL;
	}
}

static inline void ivp_hw_remap_ivp2ddr(struct ivp_device *ivp_devp,
	unsigned int ivp_addr, unsigned int len, unsigned long ddr_addr)
{
	ivp_reg_write(ivp_devp, ADDR_IVP_CFG_SEC_REG_START_REMAP_ADDR,
		ivp_addr / SIZE_1MB);
	ivp_reg_write(ivp_devp, ADDR_IVP_CFG_SEC_REG_REMAP_LENGTH, len);
	ivp_reg_write(ivp_devp, ADDR_IVP_CFG_SEC_REG_DDR_REMAP_ADDR,
		ddr_addr / SIZE_1MB);
}

static inline int ivp_remap_addr_ivp2ddr(struct ivp_device *ivp_devp,
	unsigned int ivp_addr, int len, unsigned long ddr_addr)
{
	ivp_dbg("ivp_addr:%#pK, len:%#x, ddr_addr:%#pK",
		ivp_addr, len, ddr_addr);
	if ((ivp_addr & MASK_1MB) != 0 ||
		(ddr_addr & MASK_1MB) != 0 ||
		len >= MAX_DDR_LEN * SIZE_1MB) {
		ivp_err("not aligned");
		return -EINVAL;
	}
	len = (len + SIZE_1MB - 1) / SIZE_1MB - 1;
	ivp_hw_remap_ivp2ddr(ivp_devp, ivp_addr, (u32)len, (u32)ddr_addr);

	return 0;
}

int ivp_poweron_pri(struct ivp_device *ivp_devp)
{
	int ret;

	if (!ivp_devp) {
		ivp_err("invalid input param ivp_devp");
		return -EINVAL;
	}
	/* 0.Enable the power */
	ret = regulator_enable(ivp_devp->ivp_fake_regulator);
	if (ret) {
		ivp_err("ivp_fake_regulator enable failed %d", ret);
		return ret;
	}

	/* 1.Set Clock rate */
	ret = clk_set_rate(ivp_devp->clk, (unsigned long)ivp_devp->clk_rate);
	if (ret != 0) {
		ivp_err("set rate %#x fail, ret:%d", ivp_devp->clk_rate, ret);
		goto err_clk_set_rate;
	}

	/* 2.Enable the clock */
	ret = clk_prepare_enable(ivp_devp->clk);
	if (ret) {
		ivp_err("clk prepare enable failed, ret = %d", ret);
		goto err_clk_set_rate;
	}

	ivp_info("set core success to: %ld", clk_get_rate(ivp_devp->clk));
	/* 3.Enable the power */
	ret = regulator_enable(ivp_devp->regulator);
	if (ret) {
		ivp_err("regularot enable failed %d", ret);
		goto err_regulator_enable_ivp;
	}

	ivp_reg_write(ivp_devp, IVP_REG_OFF_MEM_CTRL3, IVP_MEM_CTRL3_VAL);
	ivp_reg_write(ivp_devp, IVP_REG_OFF_MEM_CTRL4, IVP_MEM_CTRL4_VAL);

	return ret;

err_regulator_enable_ivp:
	clk_disable_unprepare(ivp_devp->clk);

err_clk_set_rate:
	ret = regulator_disable(ivp_devp->ivp_fake_regulator);
	if (ret)
		ivp_err("ivp_fake_regulator disable failed %d", ret);

	return -EINVAL;
}

int ivp_poweron_remap(struct ivp_device *ivp_devp)
{
	int ret;

	if (!ivp_devp) {
		ivp_err("invalid input param ivp_devp");
		return -EINVAL;
	}
	ret = ivp_remap_addr_ivp2ddr(ivp_devp,
		ivp_devp->sects[DDR_SECTION_INDEX].ivp_addr,
		ivp_devp->ivp_meminddr_len,
		ivp_devp->sects[DDR_SECTION_INDEX].acpu_addr << IVP_MMAP_SHIFT);
	if (ret)
		ivp_err("remap addr failed %d", ret);

	return ret;
}

int ivp_poweroff_pri(struct ivp_device *ivp_devp)
{
	int ret;

	if (!ivp_devp) {
		ivp_err("invalid input param ivp_devp");
		return -EINVAL;
	}
	ivp_hw_runstall(ivp_devp, IVP_RUNSTALL_STALL);
	ivp_hw_enable_reset(ivp_devp);
	ret = regulator_disable(ivp_devp->regulator);
	if (ret)
		ivp_err("regulator power off failed %d", ret);

	ret = clk_set_rate(ivp_devp->clk,
		(unsigned long)ivp_devp->lowfrq_pd_clk_rate);
	if (ret != 0)
		ivp_warn("set lfrq pd rate %#x fail, ret:%d",
			ivp_devp->lowfrq_pd_clk_rate, ret);

	clk_disable_unprepare(ivp_devp->clk);

	ret = regulator_disable(ivp_devp->ivp_fake_regulator);
	if (ret)
		ivp_err("ivp_fake_regulator power off failed %d", ret);

	return ret;
}

static int ivp_setup_regulator(struct platform_device *plat_dev,
	struct ivp_device *ivp_devp)
{
	struct regulator *regulator = NULL;
	struct regulator *ivp_fake_regulator = NULL;

	regulator = devm_regulator_get(&plat_dev->dev, IVP_REGULATOR);
	if (IS_ERR(regulator)) {
		ivp_err("Get ivp regulator failed");
		return -ENODEV;
	} else {
		ivp_devp->regulator = regulator;
	}

	ivp_fake_regulator = devm_regulator_get(&plat_dev->dev,
		IVP_MEDIA_REGULATOR);
	if (IS_ERR(ivp_fake_regulator)) {
		ivp_err("Get ivp regulator failed");
		return -ENODEV;
	} else {
		ivp_devp->ivp_fake_regulator = ivp_fake_regulator;
	}

	return 0;
}

static int ivp_setup_clk(struct platform_device *plat_dev,
	struct ivp_device *ivp_devp)
{
	int ret;
	u32 clk_rate = 0;

	ivp_devp->clk = devm_clk_get(&plat_dev->dev, OF_IVP_CLK_NAME);
	if (IS_ERR(ivp_devp->clk)) {
		ivp_err("get clk failed");
		return -ENODEV;
	}

	ret = of_property_read_u32(plat_dev->dev.of_node, OF_IVP_CLK_RATE_NAME,
		&clk_rate);
	if (ret) {
		ivp_err("get rate failed, ret:%d", ret);
		return -ENOMEM;
	}
	ivp_devp->clk_rate = clk_rate;
	ivp_info("get clk rate: %u", clk_rate);

	ret = of_property_read_u32(plat_dev->dev.of_node,
		OF_IVP_LOWFREQ_CLK_RATE_NAME, &clk_rate);
	if (ret) {
		ivp_err("get rate failed, ret:%d", ret);
		return -ENOMEM;
	}
	ivp_devp->lowfrq_pd_clk_rate = clk_rate;
	ivp_info("get lowfrq pd clk rate: %u", clk_rate);

	return ret;
}

int ivp_change_clk(const struct ivp_device *ivp_devp, unsigned int level)
{
	ivp_info("ivp change clk do nothing");
	return 0;
}

int ivp_init_pri(struct platform_device *plat_dev, struct ivp_device *ivp_devp)
{
	int ret;

	if (!plat_dev || !ivp_devp) {
		ivp_err("invalid input param plat_dev or ivp_devp");
		return -EINVAL;
	}
	ret = ivp_setup_regulator(plat_dev, ivp_devp);
	if (ret) {
		ivp_err("setup regulator failed, ret:%d", ret);
		return ret;
	}

	ret = ivp_setup_clk(plat_dev, ivp_devp);
	if (ret) {
		ivp_err("setup clk failed, ret:%d", ret);
		return ret;
	}

	ret = ivp_get_memory_section(plat_dev, ivp_devp);
	if (ret)
		ivp_err("get memory section failed, ret:%d", ret);

	return ret;
}

void ivp_deinit_pri(struct ivp_device *ivp_devp)
{
	if (!ivp_devp) {
		ivp_err("invalid input param ivp_devp");
		return;
	}
	ivp_free_memory_section(ivp_devp);
}

int ivp_init_resethandler(const struct ivp_device *ivp_devp __attribute__((unused)))
{
	/* init code to remap address */
	g_iram = ioremap(REMAP_ADD, SIZE_16K);
	if (!g_iram) {
		ivp_err("Can't map ivp base address");
		return -ENOMEM;
	}
	iowrite32(DEAD_FLAG, g_iram);

	return 0;
}

int ivp_check_resethandler(const struct ivp_device *ivp_devp __attribute__((unused)))
{
	/* check init code in remap address */
	int inited = 0;
	uint32_t flag = 0;

	if (g_iram)
		flag = ioread32(g_iram);
	if (flag != DEAD_FLAG)
		inited = 1;

	return inited;
}

void ivp_deinit_resethandler(const struct ivp_device *ivp_devp __attribute__((unused)))
{
	/* deinit remap address */
	if (g_iram) {
		iounmap(g_iram);
		g_iram = NULL;
	}
}

int ivp_sec_loadimage(const struct ivp_device *ivp_devp __attribute__((unused)))
{
	ivp_err("not support sec ivp");
	return -EINVAL;
}

void ivp_dev_hwa_enable(struct ivp_device *ivp_devp)
{
	if (!ivp_devp) {
		ivp_err("invalid input param ivp_devp");
		return;
	}
	/* enable apb gate clock, watdog, timer */
	ivp_info("ivp will enable hwa");
	ivp_reg_write(ivp_devp, IVP_REG_OFF_APB_GATE_CLOCK, IVP_APB_GATE_CLOCK_VAL);
	ivp_reg_write(ivp_devp, IVP_REG_OFF_TIMER_WDG_RST_DIS, IVP_TIMER_WDG_RST_DIS_VAL);
}

void ivp_hw_enable_reset(struct ivp_device *ivp_devp)
{
	if (!ivp_devp) {
		ivp_err("invalid input param ivp_devp");
		return;
	}
	ivp_reg_write(ivp_devp, IVP_REG_OFF_DSP_CORE_RESET_EN, RST_IVP32_PROCESSOR_EN);
	ivp_reg_write(ivp_devp, IVP_REG_OFF_DSP_CORE_RESET_EN, RST_IVP32_DEBUG_EN);
	ivp_reg_write(ivp_devp, IVP_REG_OFF_DSP_CORE_RESET_EN, RST_IVP32_JTAG_EN);
}

