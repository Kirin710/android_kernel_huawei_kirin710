/* Copyright (c) 2013-2014, Hisilicon Tech. Co., Ltd. All rights reserved.
*
* This program is free software; you can redistribute it and/or modify
* it under the terms of the GNU General Public License version 2 and
* only version 2 as published by the Free Software Foundation.
*
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.	See the
* GNU General Public License for more details.
*
*/

#include "hisi_overlay_utils.h"
#include "hisi_mmbuf_manager.h"

uint32_t g_fpga_flag = 0;
uint32_t g_rog_config_scf_cnt = 0;
//static int g_dss_module_resource_initialized = 0;
void *g_smmu_rwerraddr_virt = NULL;

int hisi_dss_aif_handler(struct hisi_fb_data_type *hisifd, dss_overlay_t *pov_req, dss_overlay_block_t *pov_h_block)
{
	int i = 0;
	int k = 0;
	dss_layer_t *layer = NULL;
	dss_wb_layer_t *wb_layer = NULL;
	int chn_idx = 0;
	dss_aif_bw_t *aif_bw = NULL;
	dss_aif_bw_t *aif1_bw = NULL;

	if (NULL == hisifd) {
		HISI_FB_ERR("hisifd is NULL");
		return -EINVAL;
	}
	if (NULL == pov_req) {
		HISI_FB_ERR("pov_req is NULL");
		return -EINVAL;
	}
	if (NULL == pov_h_block) {
		HISI_FB_ERR("pov_h_block is NULL");
		return -EINVAL;
	}

	if (pov_req->wb_enable) {
		if ((pov_req->wb_layer_nums <= 0) || (pov_req->wb_layer_nums > MAX_DSS_DST_NUM)) {
			HISI_FB_ERR("wb_layer_nums=%d out of range", pov_req->wb_layer_nums);
			return -EINVAL;
		}

		for (k = 0; k < pov_req->wb_layer_nums; k++) {/*lint !e574 */
			wb_layer = &(pov_req->wb_layer_infos[k]);
			chn_idx = wb_layer->chn_idx;

			aif_bw = &(hisifd->dss_module.aif_bw[chn_idx]);
			aif_bw->chn_idx = chn_idx;
			aif_bw->axi_sel = AXI_CHN1;
			aif_bw->is_used = 1;
		}
	}

	for (i = 0; i < pov_h_block->layer_nums; i++) {/*lint !e574 */
		layer = &pov_h_block->layer_infos[i];
		chn_idx = layer->chn_idx;

		if (layer->need_cap & (CAP_BASE | CAP_DIM | CAP_PURE_COLOR))
			continue;

		aif_bw = &(hisifd->dss_module.aif_bw[chn_idx]);
		aif_bw->is_used = 1;
		aif_bw->chn_idx = chn_idx;
		if (pov_req->ovl_idx == DSS_OVL0) {
			aif_bw->axi_sel = AXI_CHN0;
		} else {
			aif_bw->axi_sel = AXI_CHN1;
		}

		if (layer->need_cap & CAP_AFBCD) {
			aif1_bw = &(hisifd->dss_module.aif1_bw[chn_idx]);
			aif1_bw->is_used = 1;
			aif1_bw->chn_idx = chn_idx;
			aif1_bw->axi_sel = AXI_CHN0;
		}
	}

	return 0;
}

void hisi_dss_qos_on(struct hisi_fb_data_type *hisifd)
{
	outp32(hisifd->noc_dss_base + 0xc, 0x2);
	//outp32(hisifd->noc_dss_base + 0x8c, 0x2);
	outp32(hisifd->noc_dss_base + 0x10c, 0x2);
	outp32(hisifd->noc_dss_base + 0x18c, 0x2);
	return;
}

/*******************************************************************************
** DSS AIF
*/
static int mid_array[DSS_CHN_MAX_DEFINE] = {0xb, 0xa, 0x9, 0x8, 0x7, 0x6, 0x5, 0x4, 0x2, 0x1, 0x3, 0x0};
#define CREDIT_STEP_LOWER_ENABLE

void hisi_dss_aif_init(const char __iomem *aif_ch_base,
	dss_aif_t *s_aif)
{
	if (NULL == aif_ch_base) {
		HISI_FB_ERR("aif_ch_base is NULL");
		return;
	}
	if (NULL == s_aif) {
		HISI_FB_ERR("s_aif is NULL");
		return;
	}

	memset(s_aif, 0, sizeof(dss_aif_t));

	s_aif->aif_ch_ctl = inp32(aif_ch_base + AIF_CH_CTL);

	s_aif->aif_ch_hs = inp32(aif_ch_base + AIF_CH_HS);
	s_aif->aif_ch_ls = inp32(aif_ch_base + AIF_CH_LS);
}

void hisi_dss_aif_ch_set_reg(struct hisi_fb_data_type *hisifd,
	char __iomem *aif_ch_base, dss_aif_t *s_aif)
{
	if (hisifd == NULL) {
		HISI_FB_ERR("hisifd is null");
		return;
	}

	if (aif_ch_base == NULL) {
		HISI_FB_ERR("aif_ch_base is null");
		return;
	}

	if (s_aif == NULL) {
		HISI_FB_ERR("s_aif is null");
		return;
	}

	hisifd->set_reg(hisifd, aif_ch_base + AIF_CH_CTL,
		s_aif->aif_ch_ctl, 32, 0);

	hisifd->set_reg(hisifd, aif_ch_base + AIF_CH_HS,
		s_aif->aif_ch_hs, 32, 0);
	hisifd->set_reg(hisifd, aif_ch_base + AIF_CH_LS,
		s_aif->aif_ch_ls, 32, 0);
}

int hisi_dss_aif_ch_config(struct hisi_fb_data_type *hisifd, dss_overlay_t *pov_req,
	dss_layer_t *layer, dss_rect_t *wb_dst_rect, dss_wb_layer_t *wb_layer, int ovl_idx)
{
	dss_aif_t *aif = NULL;
	dss_aif_bw_t *aif_bw = NULL;
	int chn_idx = 0;
	int mid = 0;
	uint32_t credit_step = 0;
	uint32_t credit_step_lower = 0;
	uint64_t dss_core_rate = 0;
	uint32_t scfd_h = 0;
	uint32_t scfd_v = 0;
	uint32_t online_offline_rate = 1;

	if (hisifd == NULL) {
		HISI_FB_ERR("hisifd is NULL Point!");
		return -EINVAL;
	}
	if (pov_req == NULL){
		HISI_FB_ERR("pov_req is NULL Point!");
		return -EINVAL;
	}
	if ((layer == NULL) && (wb_layer == NULL)){
		HISI_FB_ERR("layer & wb_layer is NULL Point!");
		return -EINVAL;
	}
	if ((ovl_idx < DSS_OVL0) || (ovl_idx >= DSS_OVL_IDX_MAX)){
		HISI_FB_ERR("ovl_idx(%d) is invalid!\n", ovl_idx);
		return -EINVAL;
	}
	/*lint -e613*/
	if (wb_layer) {
		chn_idx = wb_layer->chn_idx;
	} else {
		chn_idx = layer->chn_idx;
	}
	/*lint +e613*/
	aif = &(hisifd->dss_module.aif[chn_idx]);
	hisifd->dss_module.aif_ch_used[chn_idx] = 1;

	aif_bw = &(hisifd->dss_module.aif_bw[chn_idx]);
	if (aif_bw->is_used != 1) {
		HISI_FB_ERR("fb%d, aif_bw->is_used(%d) is invalid!", hisifd->index, aif_bw->is_used);
		return -EINVAL;
	}

	mid = mid_array[chn_idx];
	if (mid < 0 || mid > 0xb) {
		HISI_FB_ERR("fb%d, mid(%d) is invalid!", hisifd->index, mid);
		return -EINVAL;
	}

	aif->aif_ch_ctl = set_bits32(aif->aif_ch_ctl, aif_bw->axi_sel, 1, 0);

	//aif->aif_ch_ctl = set_bits32(aif->aif_ch_ctl, mid, 4, 4);

	aif->aif_ch_ctl = set_bits32(aif->aif_ch_ctl, 0x0, 3, 8);//credit mode

	if (g_fpga_flag == 0) {
		if ((ovl_idx == DSS_OVL2) || (ovl_idx == DSS_OVL3)) {
			if (layer && ((layer->need_cap & CAP_AFBCD) != CAP_AFBCD)) {
				dss_core_rate = hisifd->dss_vote_cmd.dss_pri_clk_rate;
				if (dss_core_rate == 0) {
					HISI_FB_ERR("fb%d, dss_core_rate(%llu) is invalid!",
						hisifd->index, dss_core_rate);
					dss_core_rate = DEFAULT_DSS_CORE_CLK_07V_RATE;
				}

				credit_step_lower = g_dss_min_bandwidth_inbusbusy * 1000000UL * 8 / dss_core_rate;
				/*lint -e573 */
				if ((layer->src_rect.w > layer->dst_rect.w) &&
					(layer->src_rect.w > get_panel_xres(hisifd))) {/*lint !e574 */
					scfd_h = layer->src_rect.w * 100 / get_panel_xres(hisifd);
				} else {
					scfd_h = 100;
				}

				//after stretch
				if (layer->src_rect.h > layer->dst_rect.h) {
					scfd_v = layer->src_rect.h * 100 / layer->dst_rect.h;
				} else {
					scfd_v = 100;
				}

				if (pov_req->wb_compose_type == DSS_WB_COMPOSE_COPYBIT) {
					if (wb_dst_rect) {
						online_offline_rate = wb_dst_rect->w * wb_dst_rect->h /
							(hisifd->panel_info.xres * hisifd->panel_info.yres);
					}

					if (online_offline_rate == 0)
						online_offline_rate = 1;
				}
				/*lint +e573 */
				//credit_step = pix_f*128/(core_f*16/4)*scfd_h*scfd_v
				credit_step = hisifd->panel_info.pxl_clk_rate * online_offline_rate * 32 * scfd_h * scfd_v /
						dss_core_rate  / (100 * 100);

				if (g_debug_ovl_online_composer || g_debug_ovl_offline_composer || g_debug_ovl_credit_step) {
					HISI_FB_INFO("fb%d, layer_idx(%d), chn_idx(%d), src_rect(%d,%d,%d,%d),"
						"dst_rect(%d,%d,%d,%d), scfd_h=%d, scfd_v=%d, credit_step=%d.\n",
						hisifd->index, layer->layer_idx, layer->chn_idx,
						layer->src_rect.x, layer->src_rect.y, layer->src_rect.w, layer->src_rect.h,
						layer->dst_rect.x, layer->dst_rect.y, layer->dst_rect.w, layer->dst_rect.h,
						scfd_h, scfd_v, credit_step);
				}

				/* credit en lower */
				aif->aif_ch_ctl = set_bits32(aif->aif_ch_ctl, 0x1, 3, 8);   //credit step lower mode
				aif->aif_ch_ctl = set_bits32(aif->aif_ch_ctl, 0xf, 4, 12);
				aif->aif_ch_ls = set_bits32(aif->aif_ch_ls, 0x0, 9, 21);
				aif->aif_ch_ls = set_bits32(aif->aif_ch_ls, 0x40, 9, 12);
				aif->aif_ch_ls = set_bits32(aif->aif_ch_ls, credit_step_lower, 7, 4);
				aif->aif_ch_ls = set_bits32(aif->aif_ch_ls, 0x1, 1, 0);
			}

			if (wb_layer) {
				dss_core_rate = hisifd->dss_vote_cmd.dss_pri_clk_rate;
				if (dss_core_rate == 0) {
					HISI_FB_ERR("fb%d, dss_core_rate(%llu) is invalid!",
						hisifd->index, dss_core_rate);
					dss_core_rate = DEFAULT_DSS_CORE_CLK_07V_RATE;
				}

				credit_step_lower = g_dss_min_bandwidth_inbusbusy * 1000000UL * 8 / dss_core_rate;

				/* credit en lower */
				aif->aif_ch_ctl = set_bits32(aif->aif_ch_ctl, 0x1, 3, 8);   //credit step lower mode
				aif->aif_ch_ctl = set_bits32(aif->aif_ch_ctl, 0xf, 4, 12);
				aif->aif_ch_ls = set_bits32(aif->aif_ch_ls, 0x0, 9, 21);
				aif->aif_ch_ls = set_bits32(aif->aif_ch_ls, 0x40, 9, 12);
				aif->aif_ch_ls = set_bits32(aif->aif_ch_ls, credit_step_lower, 7, 4);
				aif->aif_ch_ls = set_bits32(aif->aif_ch_ls, 0x1, 1, 0);
			}
		}
	} else {
		if ((ovl_idx == DSS_OVL2) || (ovl_idx == DSS_OVL3)) {
			if (layer && ((layer->need_cap & CAP_AFBCD) != CAP_AFBCD)) {
				/* credit en lower */
				aif->aif_ch_ctl = set_bits32(aif->aif_ch_ctl, 0x1, 3, 8);//credit mode
				aif->aif_ch_ls = set_bits32(aif->aif_ch_ls, 0x1, 1, 0);//credit enable lower
				aif->aif_ch_ls = set_bits32(aif->aif_ch_ls, 0x3, 7, 4);//credit step lower
				aif->aif_ch_ls = set_bits32(aif->aif_ch_ls, 0x40, 9, 12);//credit uth lower
			}
		}
	}

	return 0;
}

int hisi_dss_aif1_ch_config(struct hisi_fb_data_type *hisifd, dss_overlay_t *pov_req,
	dss_layer_t *layer, dss_wb_layer_t *wb_layer, int ovl_idx)
{
	dss_aif_t *aif1 = NULL;
	dss_aif_bw_t *aif1_bw = NULL;
	int chn_idx = 0;
	uint32_t need_cap = 0;
	int mid = 0;
	uint32_t credit_step = 0;
	uint32_t credit_step_lower = 0;
	uint64_t dss_core_rate = 0;
	uint32_t scfd_h = 0;
	uint32_t scfd_v = 0;

	if (hisifd == NULL) {
		HISI_FB_ERR("hisifd is NULL Point!");
		return -EINVAL;
	}
	if (pov_req == NULL){
		HISI_FB_ERR("pov_req is NULL Point!");
		return -EINVAL;
	}
	if ((layer == NULL) && (wb_layer == NULL)){
		HISI_FB_ERR("layer & wb_layer is NULL Point!");
		return -EINVAL;
	}
	if ((ovl_idx < DSS_OVL0) || (ovl_idx >= DSS_OVL_IDX_MAX)){
		HISI_FB_ERR("ovl_idx(%d) is invalid!\n", ovl_idx);
		return -EINVAL;
	}
	/*lint -e613 */
	if (wb_layer) {
		chn_idx = wb_layer->chn_idx;
		need_cap = wb_layer->need_cap;
	} else {
		chn_idx = layer->chn_idx;
		need_cap = layer->need_cap;
	}
	/*lint +e613 */
	if (!(need_cap & CAP_AFBCD))
		return 0;

	aif1 = &(hisifd->dss_module.aif1[chn_idx]);
	hisifd->dss_module.aif1_ch_used[chn_idx] = 1;

	aif1_bw = &(hisifd->dss_module.aif1_bw[chn_idx]);
	if (aif1_bw->is_used != 1) {
		HISI_FB_ERR("fb%d, aif1_bw->is_used=%d no equal to 1 is err!", hisifd->index, aif1_bw->is_used);
		return 0;
	}

	mid = mid_array[chn_idx];
	if (mid < 0 || mid > 0xb) {
		HISI_FB_ERR("fb%d, mid=%d is invalid!", hisifd->index, mid);
		return 0;
	}

	aif1->aif_ch_ctl = set_bits32(aif1->aif_ch_ctl, aif1_bw->axi_sel, 1, 0);
	aif1->aif_ch_ctl = set_bits32(aif1->aif_ch_ctl, mid, 4, 4);
	aif1->aif_ch_ctl = set_bits32(aif1->aif_ch_ctl, 0x0, 3, 8);//credit mode

	if (g_fpga_flag == 0) {
		if ((ovl_idx == DSS_OVL0) || (ovl_idx == DSS_OVL1)) {
			if (layer && (layer->need_cap & CAP_AFBCD)) {
				dss_core_rate = hisifd->dss_vote_cmd.dss_pri_clk_rate;
				if (dss_core_rate == 0) {
					HISI_FB_ERR("fb%d, dss_core_rate(%llu) is invalid!",
						hisifd->index, dss_core_rate);
					dss_core_rate = DEFAULT_DSS_CORE_CLK_07V_RATE;
				}

				if ((layer->src_rect.w > layer->dst_rect.w) &&
					(layer->src_rect.w > get_panel_xres(hisifd))) {/*lint !e574 */
					scfd_h = layer->src_rect.w * 100 / get_panel_xres(hisifd);/*lint !e573 */
				} else {
					scfd_h = 100;
				}

				//after stretch
				if (layer->src_rect.h > layer->dst_rect.h) {
					scfd_v = layer->src_rect.h * 100 / layer->dst_rect.h;
				} else {
					scfd_v = 100;
				}

				//credit_step = pix_f*128/(core_f*16/4)*1.25*scfd_h*scfd_v
				credit_step = hisifd->panel_info.pxl_clk_rate * 32 * 150 * scfd_h * scfd_v /
					dss_core_rate  / (100 * 100 * 100);

				if (g_debug_ovl_online_composer || g_debug_ovl_offline_composer || g_debug_ovl_credit_step) {
					HISI_FB_INFO("fb%d, layer_idx(%d), chn_idx(%d), src_rect(%d,%d,%d,%d),"
						"dst_rect(%d,%d,%d,%d), scfd_h=%d, scfd_v=%d, credit_step=%d.\n",
						hisifd->index, layer->layer_idx, layer->chn_idx,
						layer->src_rect.x, layer->src_rect.y, layer->src_rect.w, layer->src_rect.h,
						layer->dst_rect.x, layer->dst_rect.y, layer->dst_rect.w, layer->dst_rect.h,
						scfd_h, scfd_v, credit_step);
				}

				if (credit_step < 32) {
					credit_step = 32;
				}

				aif1->aif_ch_ctl = set_bits32(aif1->aif_ch_ctl, 0x0, 3, 8);//credit mode
				if (credit_step > 64) {
					aif1->aif_ch_hs = set_bits32(aif1->aif_ch_hs, 0x0, 1, 0);   //credit en enable
				} else {
					aif1->aif_ch_hs = set_bits32(aif1->aif_ch_hs, 0x1, 1, 0);   //credit en enable
				}

				aif1->aif_ch_hs = set_bits32(aif1->aif_ch_hs, credit_step, 7, 4);	//credit step hs
			}
		} else {
			if (layer && (layer->need_cap & CAP_AFBCD)) {
				dss_core_rate = hisifd->dss_vote_cmd.dss_pri_clk_rate;
				if (dss_core_rate == 0) {
					HISI_FB_ERR("fb%d, dss_core_rate(%llu is invalid!",
						hisifd->index, dss_core_rate);
					dss_core_rate = DEFAULT_DSS_CORE_CLK_07V_RATE;
				}

				credit_step_lower = g_dss_min_bandwidth_inbusbusy * 1000000UL * 8 / dss_core_rate;

				/* credit en lower */
				aif1->aif_ch_ctl = set_bits32(aif1->aif_ch_ctl, 0x1, 3, 8);	//credit step lower mode
				aif1->aif_ch_ctl = set_bits32(aif1->aif_ch_ctl, 0xf, 4, 12);//max_cnt
				aif1->aif_ch_ls = set_bits32(aif1->aif_ch_ls, 0x0, 9, 21);
				aif1->aif_ch_ls = set_bits32(aif1->aif_ch_ls, 0x40, 9, 12);
				aif1->aif_ch_ls = set_bits32(aif1->aif_ch_ls, credit_step_lower, 7, 4);
				aif1->aif_ch_ls = set_bits32(aif1->aif_ch_ls, 0x1, 1, 0);
			}
		}
	} else {
		if ((ovl_idx == DSS_OVL2) || (ovl_idx == DSS_OVL3)) {
			if (layer && (layer->need_cap & CAP_AFBCD)) {
				/* credit en lower */
				aif1->aif_ch_ctl = set_bits32(aif1->aif_ch_ctl, 0x1, 3, 8);//credit mode
				aif1->aif_ch_ls = set_bits32(aif1->aif_ch_ls, 0x1, 1, 0);//credit enable lower
				aif1->aif_ch_ls = set_bits32(aif1->aif_ch_ls, 0x3, 7, 4);//credit step lower
				aif1->aif_ch_ls = set_bits32(aif1->aif_ch_ls, 0x40, 9, 12);//credit uth lower
			}
		}
	}

	return 0;
}

/*******************************************************************************
** DSS SMMU
*/
void hisi_dss_smmu_on(struct hisi_fb_data_type *hisifd)
{
	char __iomem *smmu_base = NULL;
	int idx0 = 0;
	int idx1 = 0;
	int idx2 = 0;
	uint64_t smmu_rwerraddr_phys = 0;
	uint32_t fama_ptw_msb;

	if (NULL == hisifd) {
		HISI_FB_ERR("hisifd is NULL");
		return;
	}

	smmu_base = hisifd->dss_base + DSS_SMMU_OFFSET;

	set_reg(smmu_base + SMMU_SCR, 0x0, 1, 0);  //global bypass cancel
	//set_reg(smmu_base + SMMU_SCR_S, 0x3, 2, 0);  //nscfg  using default value 0x3
	set_reg(smmu_base + SMMU_SCR, 0x1, 8, 20);   //ptw_mid
	set_reg(smmu_base + SMMU_SCR, g_dss_smmu_outstanding - 1, 4, 16);  //pwt_pf
	set_reg(smmu_base + SMMU_SCR, 0x7, 3, 3);  //interrupt cachel1 cach3l2 en
	set_reg(smmu_base + SMMU_LP_CTRL, 0x1, 1, 0);  //auto_clk_gt_en

	//Long Descriptor
	set_reg(smmu_base + SMMU_CB_TTBCR, 0x1, 1, 0);

	//RWERRADDR
	if (g_smmu_rwerraddr_virt) {
		smmu_rwerraddr_phys = virt_to_phys(g_smmu_rwerraddr_virt);

		set_reg(smmu_base + SMMU_ERR_RDADDR,
			(uint32_t)(smmu_rwerraddr_phys & 0xFFFFFFFF), 32, 0);
		set_reg(smmu_base + SMMU_ADDR_MSB,
			(uint32_t)((smmu_rwerraddr_phys >> 32) & 0x7F), 7, 0);
		set_reg(smmu_base + SMMU_ERR_WRADDR,
			(uint32_t)(smmu_rwerraddr_phys & 0xFFFFFFFF), 32, 0);
		set_reg(smmu_base + SMMU_ADDR_MSB,
			(uint32_t)((smmu_rwerraddr_phys >> 32) & 0x7F), 7, 7);
	} else {
		set_reg(smmu_base + SMMU_ERR_RDADDR, 0x7FF00000, 32, 0);
		//set_reg(smmu_base + SMMU_ADDR_MSB, 0x0, 2, 0);
		set_reg(smmu_base + SMMU_ERR_WRADDR, 0x7FFF0000, 32, 0);
		//set_reg(smmu_base + SMMU_ADDR_MSB, 0x0, 2, 2);
	}

	// disable cmdlist, dbg, reload
	set_reg(smmu_base + SMMU_RLD_EN0_NS, 0xFF1FFFFF, 32, 0);
	set_reg(smmu_base + SMMU_RLD_EN1_NS, 0xffffffff, 32, 0);
	set_reg(smmu_base + SMMU_RLD_EN2_NS, 0x00ffffff, 32, 0);

	idx0 = 22; //debug stream id
	idx1 = 23; //cmd unsec stream id
	idx2 = 24; //cmd sec stream id
	/*lint -e679 */
	//cmdlist stream bypass
	set_reg(smmu_base + SMMU_SMRx_NS + idx0 * 0x4, 0x1d, 32, 0);
	set_reg(smmu_base + SMMU_SMRx_NS + idx1 * 0x4, 0x1d, 32, 0);
	set_reg(smmu_base + SMMU_SMRx_NS + idx2 * 0x4, 0x1d, 32, 0);
	/*lint +e679 */
	//TTBR0
	set_reg(smmu_base + SMMU_CB_TTBR0, (uint32_t)hisi_dss_domain_get_ttbr(), 32, 0);

#if defined (CONFIG_DRMDRIVER)
	configure_dss_service_security(DSS_SMMU_INIT, 0/*not used*/, 0/*not used*/);
#endif
	fama_ptw_msb = (hisi_dss_domain_get_ttbr() >> 32) & 0x7F;
	set_reg(smmu_base + SMMU_FAMA_CTRL0, 0x80, 14, 0);
	set_reg(smmu_base + SMMU_FAMA_CTRL1, fama_ptw_msb, 7, 0);
}
/*lint -e613 -e838 -e679*/
void hisi_mdc_smmu_on(struct hisi_fb_data_type *hisifd)
{
	char __iomem *smmu_base = NULL;
	int idx0 = 0;
	int idx1 = 0;
	int idx2 = 0;
	uint64_t smmu_rwerraddr_phys = 0;
	uint32_t fama_ptw_msb;

	if (hisifd == NULL) {
		HISI_FB_ERR("hisifd is NULL point.\n");
		return;
	}

	smmu_base = hisifd->dss_base + DSS_SMMU_OFFSET;

	set_reg(smmu_base + SMMU_SCR, 0x0, 1, 0);  //global bypass cancel
	//set_reg(smmu_base + SMMU_SCR_S, 0x3, 2, 0);  //nscfg  using default value 0x3
	set_reg(smmu_base + SMMU_SCR, 0x1, 8, 20);   //ptw_mid
	set_reg(smmu_base + SMMU_SCR, g_dss_smmu_outstanding - 1, 4, 16);  //pwt_pf
	set_reg(smmu_base + SMMU_SCR, 0x7, 3, 3);  //interrupt cachel1 cach3l2 en
	set_reg(smmu_base + SMMU_LP_CTRL, 0x1, 1, 0);  //auto_clk_gt_en

	//Long Descriptor
	set_reg(smmu_base + SMMU_CB_TTBCR, 0x1, 1, 0);

	//RWERRADDR
	if (g_smmu_rwerraddr_virt) {
		smmu_rwerraddr_phys = virt_to_phys(g_smmu_rwerraddr_virt);

		set_reg(smmu_base + SMMU_ERR_RDADDR,
			(uint32_t)(smmu_rwerraddr_phys & 0xFFFFFFFF), 32, 0);
		//set_reg(smmu_base + SMMU_ADDR_MSB,
		//	(uint32_t)((smmu_rwerraddr_phys >> 32) & 0x3), 2, 0);
		set_reg(smmu_base + SMMU_ERR_WRADDR,
			(uint32_t)(smmu_rwerraddr_phys & 0xFFFFFFFF), 32, 0);
		//set_reg(smmu_base + SMMU_ADDR_MSB,
		//	(uint32_t)((smmu_rwerraddr_phys >> 32) & 0x3), 2, 2);
	} else {
		set_reg(smmu_base + SMMU_ERR_RDADDR, 0x7FF00000, 32, 0);
		//set_reg(smmu_base + SMMU_ADDR_MSB, 0x0, 2, 0);
		set_reg(smmu_base + SMMU_ERR_WRADDR, 0x7FFF0000, 32, 0);
		//set_reg(smmu_base + SMMU_ADDR_MSB, 0x0, 2, 2);
	}

	// disable cmdlist, dbg, reload
	set_reg(smmu_base + SMMU_RLD_EN0_NS, 0xffffffff, 32, 0);
	set_reg(smmu_base + SMMU_RLD_EN1_NS, 0xffffffcf, 32, 0);
	set_reg(smmu_base + SMMU_RLD_EN2_NS, 0x00ffffff, 32, 0);

	idx0 = 36; //debug stream id
	idx1 = 37; //cmd unsec stream id
	idx2 = 38; //cmd sec stream id

	//cmdlist stream bypass
	set_reg(smmu_base + SMMU_SMRx_NS + idx0 * 0x4, 0x1d, 32, 0);
	set_reg(smmu_base + SMMU_SMRx_NS + idx1 * 0x4, 0x1d, 32, 0);
	set_reg(smmu_base + SMMU_SMRx_NS + idx2 * 0x4, 0x1d, 32, 0);

	//TTBR0
	set_reg(smmu_base + SMMU_CB_TTBR0, (uint32_t)hisi_dss_domain_get_ttbr(), 32, 0);

	fama_ptw_msb = (hisi_dss_domain_get_ttbr() >> 32) & 0x7F;
	set_reg(smmu_base + SMMU_FAMA_CTRL0, 0x80, 14, 0);
	set_reg(smmu_base + SMMU_FAMA_CTRL1, fama_ptw_msb, 7, 0);
}
/*lint +e613 +e838 +e679*/

void hisi_dss_smmu_init(char __iomem *smmu_base,
	dss_smmu_t *s_smmu)
{
	if (NULL == smmu_base) {
		HISI_FB_ERR("smmu_base is NULL");
		return;
	}
	if (NULL == s_smmu) {
		HISI_FB_ERR("s_smmu is NULL");
		return;
	}

	memset(s_smmu, 0, sizeof(dss_smmu_t));
}
/*lint -e838 -e679 -e568 -e685*/
void hisi_dss_smmu_ch_set_reg(struct hisi_fb_data_type *hisifd,
	char __iomem *smmu_base, dss_smmu_t *s_smmu, int chn_idx)
{
	uint32_t idx = 0;
	uint32_t i = 0;

	if (hisifd == NULL) {
		HISI_FB_ERR("hisifd is null");
		return;
	}

	if (smmu_base == NULL) {
		HISI_FB_ERR("smmu_base is null");
		return;
	}

	if (s_smmu == NULL) {
		HISI_FB_ERR("s_smmu is null");
		return;
	}

	if (s_smmu->smmu_smrx_ns_used[chn_idx] == 0)
		return;

	for (i = 0; i < g_dss_chn_sid_num[chn_idx]; i++) {
		idx = g_dss_smmu_smrx_idx[chn_idx] + i;
		if (idx >= SMMU_SID_NUM) {
			HISI_FB_ERR("idx is invalid");
			return;
		}

		hisifd->set_reg(hisifd, smmu_base + SMMU_SMRx_NS + idx * 0x4,
			s_smmu->smmu_smrx_ns[idx], 32, 0);
	}
}
/*lint +e838 +e679 +e568 +e685*/
int hisi_dss_smmu_ch_config(struct hisi_fb_data_type *hisifd,
	dss_layer_t *layer, dss_wb_layer_t *wb_layer)
{
	dss_smmu_t *smmu = NULL;
	int chn_idx = 0;
	dss_img_t *img = NULL;
	uint32_t idx = 0;
	uint32_t i = 0;

	if (NULL == hisifd) {
		HISI_FB_ERR("hisifd is NULL");
		return -EINVAL;
	}
	if ((layer == NULL) && (wb_layer == NULL)) {
		HISI_FB_ERR("layer or wb_layer is NULL");
		return -EINVAL;
	}
	/*lint -e613 -e838 -e568 -e685*/
	if (wb_layer) {
		img = &(wb_layer->dst);
		chn_idx = wb_layer->chn_idx;
	} else {
		img = &(layer->img);
		chn_idx = layer->chn_idx;
	}

	smmu = &(hisifd->dss_module.smmu);
	hisifd->dss_module.smmu_used = 1;

	smmu->smmu_smrx_ns_used[chn_idx] = 1;

	for (i = 0; i < g_dss_chn_sid_num[chn_idx]; i++) {
		idx = g_dss_smmu_smrx_idx[chn_idx] + i;
		if (idx >= SMMU_SID_NUM) {
			HISI_FB_ERR("idx is invalid");
			return -EINVAL;
		}

		if (img->mmu_enable == 0) {
			smmu->smmu_smrx_ns[idx] = set_bits32(smmu->smmu_smrx_ns[idx], 0x1, 1, 0);
		} else {
			/* stream config */
			smmu->smmu_smrx_ns[idx] = set_bits32(smmu->smmu_smrx_ns[idx], 0x0, 1, 0);  //smr_bypass
			smmu->smmu_smrx_ns[idx] = set_bits32(smmu->smmu_smrx_ns[idx], 0x1, 1, 4);  //smr_invld_en
			smmu->smmu_smrx_ns[idx] = set_bits32(smmu->smmu_smrx_ns[idx], 0x3, 2, 2);  //smr_ptw_qos
			//smmu->smmu_smrx_ns[idx] = set_bits32(smmu->smmu_smrx_ns[idx],  , 20, 12);  //smr_offset_addr

		}
       }
	/*lint +e613 +e838 +e568 +e685*/
	return 0;
}

void hisifb_adjust_block_rect(int block_num, dss_rect_t *ov_block_rects[], dss_wb_layer_t *wb_layer)
{
	return ;
}

/*******************************************************************************
** DSS CSC
*/
#define CSC_ROW	(3)
#define CSC_COL	(5)

/*
** Boston CS
** [ p00 p01 p02 idc2 odc2 ]
** [ p10 p11 p12 idc1 odc1 ]
** [ p20 p21 p22 idc0 odc0 ]
*/
/*static int CSC_COE_YUV2RGB601_WIDE[CSC_ROW][CSC_COL] = {
	{0x4000, 0x00000, 0x059ba, 0x000, 0x000},
	{0x4000, 0x1e9fa, 0x1d24c, 0x600, 0x000},
	{0x4000, 0x07168, 0x00000, 0x600, 0x000},
};

static int CSC_COE_RGB2YUV601_WIDE[CSC_ROW][CSC_COL] = {
	{0x01323, 0x02591, 0x0074c, 0x000, 0x000},
	{0x1f533, 0x1eacd, 0x02000, 0x000, 0x200},
	{0x02000, 0x1e534, 0x1facc, 0x000, 0x200},
};

static int CSC_COE_YUV2RGB601_NARROW[CSC_ROW][CSC_COL] = {
	{0x4a85, 0x00000, 0x06625, 0x7c0, 0x000},
	{0x4a85, 0x1e6ed, 0x1cbf8, 0x600, 0x000},
	{0x4a85, 0x0811a, 0x00000, 0x600, 0x000},
};

static int CSC_COE_RGB2YUV601_NARROW[CSC_ROW][CSC_COL] = {
	{0x0106f, 0x02044, 0x00644, 0x000, 0x040},
	{0x1f684, 0x1ed60, 0x01c1c, 0x000, 0x200},
	{0x01c1c, 0x1e876, 0x1fb6e, 0x000, 0x200},
};

static int CSC_COE_YUV2RGB709_WIDE[CSC_ROW][CSC_COL] = {
	{0x4000, 0x00000, 0x064ca, 0x000, 0x000},
	{0x4000, 0x1f403, 0x1e20a, 0x600, 0x000},
	{0x4000, 0x076c2, 0x00000, 0x600, 0x000},
};

static int CSC_COE_RGB2YUV709_WIDE[CSC_ROW][CSC_COL] = {
	{0x00d9b, 0x02dc6, 0x0049f, 0x000, 0x000},
	{0x1f8ab, 0x1e755, 0x02000, 0x000, 0x200},
	{0x02000, 0x1e2ef, 0x1fd11, 0x000, 0x200},
};

static int CSC_COE_YUV2RGB709_NARROW[CSC_ROW][CSC_COL] = {
	{0x4a85, 0x00000, 0x072bc, 0x7c0, 0x000},
	{0x4a85, 0x1f25a, 0x1dde5, 0x600, 0x000},
	{0x4a85, 0x08732, 0x00000, 0x600, 0x000},
};

static int CSC_COE_RGB2YUV709_NARROW[CSC_ROW][CSC_COL] = {
	{0x00baf, 0x02750, 0x003f8, 0x000, 0x040},
	{0x1f98f, 0x1ea55, 0x01c1c, 0x000, 0x200},
	{0x01c1c, 0x1e678, 0x1fd6c, 0x000, 0x200},
};*/

/*
** Boston ES
** Rec.601 for Computer
** [ p00 p01 p02 cscidc2 cscodc2 ]
** [ p10 p11 p12 cscidc1 cscodc1 ]
** [ p20 p21 p22 cscidc0 cscodc0 ]
*/
/* application: mode 2 is used in rgb2yuv, mode 0 is used in yuv2rgb */
#define CSC_MPREC_MODE_0 (0)
#define CSC_MPREC_MODE_1 (1)  //never used for ES
#define CSC_MPREC_MODE_2 (2)  //yuv2rgb is not supported by mode 2

#define CSC_MPREC_MODE_RGB2YUV (CSC_MPREC_MODE_2)
#define CSC_MPREC_MODE_YUV2RGB (CSC_MPREC_MODE_0)

static int CSC_COE_YUV2RGB601_NARROW_MPREC0[CSC_ROW][CSC_COL] = {
	{0x4a8, 0x000, 0x662, 0x7f0, 0x000},
	{0x4a8, 0x1e6f, 0x1cc0, 0x780, 0x000},
	{0x4a8, 0x812, 0x000, 0x780, 0x000}
};

static int CSC_COE_RGB2YUV601_NARROW_MPREC2[CSC_ROW][CSC_COL] = {
	{0x41C, 0x811, 0x191, 0x000, 0x010},
	{0x1DA1, 0x1B58, 0x707, 0x000, 0x080},
	{0x707, 0x1A1E, 0x1EDB, 0x000, 0x080}
};

static int CSC_COE_YUV2RGB709_NARROW_MPREC0[CSC_ROW][CSC_COL] = {
	{0x4a8, 0x000, 0x72c, 0x7f0, 0x000},
	{0x4a8, 0x1f26, 0x1dde, 0x77f, 0x000},
	{0x4a8, 0x873, 0x000, 0x77f, 0x000}
};

static int CSC_COE_RGB2YUV709_NARROW_MPREC2[CSC_ROW][CSC_COL] = {
	{0x2EC, 0x9D4, 0x0FE, 0x000, 0x010},
	{0x1E64, 0x1A95, 0x707, 0x000, 0x081},
	{0x707, 0x199E, 0x1F5B, 0x000, 0x081}
};

static int CSC_COE_YUV2RGB601_WIDE_MPREC0[CSC_ROW][CSC_COL] = {
	{0x400, 0x000, 0x59c, 0x000, 0x000},
	{0x400, 0x1ea0, 0x1d25, 0x77f, 0x000},
	{0x400, 0x717, 0x000, 0x77f, 0x000}
};

static int CSC_COE_RGB2YUV601_WIDE_MPREC2[CSC_ROW][CSC_COL] = {
	{0x4C9, 0x964, 0x1d3, 0x000, 0x000},
	{0x1D4D, 0x1AB3, 0x800, 0x000, 0x081},
	{0x800, 0x194D, 0x1EB3, 0x000, 0x081},
};

static int CSC_COE_YUV2RGB709_WIDE_MPREC0[CSC_ROW][CSC_COL] = {
	{0x400, 0x000, 0x64d, 0x000, 0x000},
	{0x400, 0x1f40, 0x1e21, 0x77f, 0x000},
	{0x400, 0x76c, 0x000, 0x77f, 0x000}
};

static int CSC_COE_RGB2YUV709_WIDE_MPREC2[CSC_ROW][CSC_COL] = {
	{0x367, 0xB71, 0x128, 0x000, 0x000},
	{0x1E2B, 0x19D5, 0x800, 0x000, 0x081},
	{0x800, 0x18BC, 0x1F44, 0x000, 0x081},
};
/*lint -e732*/
void hisi_dss_csc_init(const char __iomem *csc_base, dss_csc_t *s_csc)
{
	if (NULL == csc_base) {
		HISI_FB_ERR("csc_base is NULL");
		return;
	}
	if (NULL == s_csc) {
		HISI_FB_ERR("s_csc is NULL");
		return;
	}

	memset(s_csc, 0, sizeof(dss_csc_t));

	s_csc->idc0 = inp32(csc_base + CSC_IDC0);
	s_csc->idc2 = inp32(csc_base + CSC_IDC2);
	s_csc->odc0 = inp32(csc_base + CSC_ODC0);
	s_csc->odc2 = inp32(csc_base + CSC_ODC2);

	s_csc->p0 = inp32(csc_base + CSC_P0);
	s_csc->p1 = inp32(csc_base + CSC_P1);
	s_csc->p2 = inp32(csc_base + CSC_P2);
	s_csc->p3 = inp32(csc_base + CSC_P3);
	s_csc->p4 = inp32(csc_base + CSC_P4);
	s_csc->icg_module = inp32(csc_base + CSC_ICG_MODULE);
	s_csc->mprec= inp32(csc_base + CSC_MPREC);
}
/*lint +e732*/
void hisi_dss_csc_set_reg(struct hisi_fb_data_type *hisifd,
	char __iomem *csc_base, dss_csc_t *s_csc)
{
	if (hisifd == NULL) {
		HISI_FB_ERR("hisifd is null");
		return;
	}

	if (csc_base == NULL) {
		HISI_FB_ERR("csc_base is null");
		return;
	}

	if (s_csc == NULL) {
		HISI_FB_ERR("s_csc is null");
		return;
	}

	hisifd->set_reg(hisifd, csc_base + CSC_IDC0, s_csc->idc0, 32, 0);
	hisifd->set_reg(hisifd, csc_base + CSC_IDC2, s_csc->idc2, 32, 0);
	hisifd->set_reg(hisifd, csc_base + CSC_ODC0, s_csc->odc0, 32, 0);
	hisifd->set_reg(hisifd, csc_base + CSC_ODC2, s_csc->odc2, 32, 0);

	hisifd->set_reg(hisifd, csc_base + CSC_P0, s_csc->p0, 32, 0);
	hisifd->set_reg(hisifd, csc_base + CSC_P1, s_csc->p1, 32, 0);
	hisifd->set_reg(hisifd, csc_base + CSC_P2, s_csc->p2, 32, 0);
	hisifd->set_reg(hisifd, csc_base + CSC_P3, s_csc->p3, 32, 0);
	hisifd->set_reg(hisifd, csc_base + CSC_P4, s_csc->p4, 32, 0);
	hisifd->set_reg(hisifd, csc_base + CSC_ICG_MODULE, s_csc->icg_module, 32, 0);
	hisifd->set_reg(hisifd, csc_base + CSC_MPREC, s_csc->mprec, 32, 0);
}

bool is_pcsc_needed(dss_layer_t *layer)
{
	if (layer->chn_idx != DSS_RCHN_V1)
		return false;

	if (layer->need_cap & CAP_2D_SHARPNESS)
		return true;

	/*horizental shrink is not supported by arsr2p */
	if ((layer->dst_rect.h != layer->src_rect.h) || (layer->dst_rect.w > layer->src_rect.w))
		return true;

	return false;
}
/*lint -e701 -e732 */
//only for csc8b
int hisi_dss_csc_config(struct hisi_fb_data_type *hisifd,
	dss_layer_t *layer, dss_wb_layer_t *wb_layer)
{
	dss_csc_t *csc = NULL;
	int chn_idx = 0;
	uint32_t format = 0;
	uint32_t csc_mode = 0;
	int (*csc_coe_yuv2rgb)[CSC_COL];
	int (*csc_coe_rgb2yuv)[CSC_COL];

	if (hisifd == NULL) {
		HISI_FB_ERR("hisifd is NULL Point!");
		return -EINVAL;
	}

	if (wb_layer) {
		chn_idx = wb_layer->chn_idx;
		format = wb_layer->dst.format;
		csc_mode = wb_layer->dst.csc_mode;
	} else {
		if (layer) {
			chn_idx = layer->chn_idx;
			format = layer->img.format;
			csc_mode = layer->img.csc_mode;
		}
	}

	if (chn_idx != DSS_RCHN_V1) {
		if (!isYUV(format))
			return 0;
		hisifd->dss_module.csc_used[chn_idx] = 1;
	} else if ((chn_idx == DSS_RCHN_V1) && (!isYUV(format))){ //v1, rgb format
		if (layer) {
			if (!is_pcsc_needed(layer)) {
				return 0;
			}
		}

		hisifd->dss_module.csc_used[DSS_RCHN_V1] = 1;
		hisifd->dss_module.pcsc_used[DSS_RCHN_V1] = 1;
	} else {//v1, yuv format
		hisifd->dss_module.csc_used[chn_idx] = 1;
	}

	if (csc_mode == DSS_CSC_601_WIDE) {
		csc_coe_yuv2rgb = CSC_COE_YUV2RGB601_WIDE_MPREC0;
		csc_coe_rgb2yuv = CSC_COE_RGB2YUV601_WIDE_MPREC2;
	} else if (csc_mode == DSS_CSC_601_NARROW) {
		csc_coe_yuv2rgb = CSC_COE_YUV2RGB601_NARROW_MPREC0;
		csc_coe_rgb2yuv = CSC_COE_RGB2YUV601_NARROW_MPREC2;
	} else if (csc_mode == DSS_CSC_709_WIDE) {
		csc_coe_yuv2rgb = CSC_COE_YUV2RGB709_WIDE_MPREC0;
		csc_coe_rgb2yuv = CSC_COE_RGB2YUV709_WIDE_MPREC2;
	} else if (csc_mode == DSS_CSC_709_NARROW) {
		csc_coe_yuv2rgb = CSC_COE_YUV2RGB709_NARROW_MPREC0;
		csc_coe_rgb2yuv = CSC_COE_RGB2YUV709_NARROW_MPREC2;
	} else {
	    /* TBD  add csc mprec mode 1 and mode 2*/
		HISI_FB_ERR("not support this csc_mode(%d)!\n", csc_mode);
		csc_coe_yuv2rgb = CSC_COE_YUV2RGB601_WIDE_MPREC0;
		csc_coe_rgb2yuv = CSC_COE_RGB2YUV601_WIDE_MPREC2;
	}

	/* config rch csc */
	if (layer && hisifd->dss_module.csc_used[chn_idx]) {
		csc = &(hisifd->dss_module.csc[chn_idx]);
		csc->mprec = CSC_MPREC_MODE_YUV2RGB;
		csc->icg_module = set_bits32(csc->icg_module, 0x1, 1, 0);

		csc->idc0 = set_bits32(csc->idc0,
			(csc_coe_yuv2rgb[2][3]) |
			(csc_coe_yuv2rgb[1][3] << 16), 27, 0);
		csc->idc2 = set_bits32(csc->idc2,
			(csc_coe_yuv2rgb[0][3]), 11, 0);

		csc->odc0 = set_bits32(csc->odc0,
			(csc_coe_yuv2rgb[2][4]) |
			(csc_coe_yuv2rgb[1][4] << 16), 27, 0);
		csc->odc2 = set_bits32(csc->odc2,
			(csc_coe_yuv2rgb[0][4]), 11, 0);

		csc->p0 = set_bits32(csc->p0, csc_coe_yuv2rgb[0][0], 13, 0);
		csc->p0 = set_bits32(csc->p0, csc_coe_yuv2rgb[0][1], 13, 16);

		csc->p1 = set_bits32(csc->p1, csc_coe_yuv2rgb[0][2], 13, 0);
		csc->p1 = set_bits32(csc->p1, csc_coe_yuv2rgb[1][0], 13, 16);

		csc->p2 = set_bits32(csc->p2, csc_coe_yuv2rgb[1][1], 13, 0);
		csc->p2 = set_bits32(csc->p2, csc_coe_yuv2rgb[1][2], 13, 16);

		csc->p3 = set_bits32(csc->p3, csc_coe_yuv2rgb[2][0], 13, 0);
		csc->p3 = set_bits32(csc->p3, csc_coe_yuv2rgb[2][1], 13, 16);

		csc->p4 = set_bits32(csc->p4, csc_coe_yuv2rgb[2][2], 13, 0);
	}

	/* config rch pcsc */
	if (layer && hisifd->dss_module.pcsc_used[chn_idx]) {
		csc = &(hisifd->dss_module.pcsc[chn_idx]);
		csc->mprec = CSC_MPREC_MODE_RGB2YUV;
		csc->icg_module = set_bits32(csc->icg_module, 0x1, 1, 0);

		csc->idc0 = set_bits32(csc->idc0,
			(csc_coe_rgb2yuv[2][3]) |
			(csc_coe_rgb2yuv[1][3] << 16), 27, 0);
		csc->idc2 = set_bits32(csc->idc2,
			(csc_coe_rgb2yuv[0][3]), 11, 0);

		csc->odc0 = set_bits32(csc->odc0,
			(csc_coe_rgb2yuv[2][4]) |
			(csc_coe_rgb2yuv[1][4] << 16), 27, 0);
		csc->odc2 = set_bits32(csc->odc2,
			(csc_coe_rgb2yuv[0][4]), 11, 0);

		csc->p0 = set_bits32(csc->p0, csc_coe_rgb2yuv[0][0], 13, 0);
		csc->p0 = set_bits32(csc->p0, csc_coe_rgb2yuv[0][1], 13, 16);

		csc->p1 = set_bits32(csc->p1, csc_coe_rgb2yuv[0][2], 13, 0);
		csc->p1 = set_bits32(csc->p1, csc_coe_rgb2yuv[1][0], 13, 16);

		csc->p2 = set_bits32(csc->p2, csc_coe_rgb2yuv[1][1], 13, 0);
		csc->p2 = set_bits32(csc->p2, csc_coe_rgb2yuv[1][2], 13, 16);

		csc->p3 = set_bits32(csc->p3, csc_coe_rgb2yuv[2][0], 13, 0);
		csc->p3 = set_bits32(csc->p3, csc_coe_rgb2yuv[2][1], 13, 16);

		csc->p4 = set_bits32(csc->p4, csc_coe_rgb2yuv[2][2], 13, 0);
	}

	/* config wch csc */
	if (wb_layer) {
		csc = &(hisifd->dss_module.csc[chn_idx]);
		csc->mprec = CSC_MPREC_MODE_RGB2YUV;
		csc->icg_module = set_bits32(csc->icg_module, 0x1, 1, 0);

		csc->idc0 = set_bits32(csc->idc0,
			(csc_coe_rgb2yuv[2][3]) |
			(csc_coe_rgb2yuv[1][3] << 16), 27, 0);
		csc->idc2 = set_bits32(csc->idc2,
			(csc_coe_rgb2yuv[0][3]), 11, 0);

		csc->odc0 = set_bits32(csc->odc0,
			(csc_coe_rgb2yuv[2][4]) |
			(csc_coe_rgb2yuv[1][4] << 16), 27, 0);
		csc->odc2 = set_bits32(csc->odc2,
			(csc_coe_rgb2yuv[0][4]), 11, 0);

		csc->p0 = set_bits32(csc->p0, csc_coe_rgb2yuv[0][0], 13, 0);
		csc->p0 = set_bits32(csc->p0, csc_coe_rgb2yuv[0][1], 13, 16);

		csc->p1 = set_bits32(csc->p1, csc_coe_rgb2yuv[0][2], 13, 0);
		csc->p1 = set_bits32(csc->p1, csc_coe_rgb2yuv[1][0], 13, 16);

		csc->p2 = set_bits32(csc->p2, csc_coe_rgb2yuv[1][1], 13, 0);
		csc->p2 = set_bits32(csc->p2, csc_coe_rgb2yuv[1][2], 13, 16);

		csc->p3 = set_bits32(csc->p3, csc_coe_rgb2yuv[2][0], 13, 0);
		csc->p3 = set_bits32(csc->p3, csc_coe_rgb2yuv[2][1], 13, 16);

		csc->p4 = set_bits32(csc->p4, csc_coe_rgb2yuv[2][2], 13, 0);
	}

	return 0;
}
/*lint +e701 +e732 */
/*lint -e679 -e730 -e732 -e838*/
void hisi_dss_ovl_init(const char __iomem *ovl_base, dss_ovl_t *s_ovl, int ovl_idx)
{
	int i = 0;

	if (NULL == ovl_base) {
		HISI_FB_ERR("ovl_base is NULL");
		return;
	}
	if (NULL == s_ovl) {
		HISI_FB_ERR("s_ovl is NULL");
		return;
	}

	memset(s_ovl, 0, sizeof(dss_ovl_t));

	s_ovl->ovl_size = inp32(ovl_base + OVL_SIZE);
	s_ovl->ovl_bg_color = inp32(ovl_base + OVL_BG_COLOR);
	s_ovl->ovl_dst_startpos = inp32(ovl_base + OVL_DST_STARTPOS);
	s_ovl->ovl_dst_endpos = inp32(ovl_base + OVL_DST_ENDPOS);
	s_ovl->ovl_gcfg = inp32(ovl_base + OVL_GCFG);


	for (i = 0; i < OVL_6LAYER_NUM; i++) {
		s_ovl->ovl_layer[i].layer_pos =
			inp32(ovl_base + OVL_LAYER0_POS + i * 0x3C);
		s_ovl->ovl_layer[i].layer_size =
			inp32(ovl_base + OVL_LAYER0_SIZE + i * 0x3C);
		s_ovl->ovl_layer[i].layer_pattern =
			inp32(ovl_base + OVL_LAYER0_PATTERN + i * 0x3C);
		s_ovl->ovl_layer[i].layer_alpha =
			inp32(ovl_base + OVL_LAYER0_ALPHA + i * 0x3C);
		s_ovl->ovl_layer[i].layer_cfg =
			inp32(ovl_base + OVL_LAYER0_CFG + i * 0x3C);

		s_ovl->ovl_layer_pos[i].layer_pspos =
			inp32(ovl_base + OVL_LAYER0_PSPOS + i * 0x3C);
		s_ovl->ovl_layer_pos[i].layer_pepos =
			inp32(ovl_base + OVL_LAYER0_PEPOS + i * 0x3C);
	}

	s_ovl->ovl_block_size = inp32(ovl_base + OVL6_BLOCK_SIZE);
}

void hisi_dss_ovl_set_reg(struct hisi_fb_data_type *hisifd,
	char __iomem *ovl_base, dss_ovl_t *s_ovl, int ovl_idx)
{
	int i = 0;

	if (hisifd == NULL) {
		HISI_FB_ERR("hisifd is null");
		return;
	}

	if (ovl_base == NULL) {
		HISI_FB_ERR("ovl_base is null");
		return;
	}

	if (s_ovl == NULL) {
		HISI_FB_ERR("s_ovl is null");
		return;
	}

	if ((ovl_idx == DSS_OVL1) || (ovl_idx == DSS_OVL3)) {
		HISI_FB_ERR("ovl_idx not surport:%d ",ovl_idx);
		return;
	}

	hisifd->set_reg(hisifd, ovl_base + OVL6_REG_DEFAULT, 0x1, 32, 0);
	hisifd->set_reg(hisifd, ovl_base + OVL6_REG_DEFAULT, 0x0, 32, 0);

	hisifd->set_reg(hisifd, ovl_base + OVL_SIZE, s_ovl->ovl_size, 32, 0);
	hisifd->set_reg(hisifd, ovl_base + OVL_BG_COLOR, s_ovl->ovl_bg_color, 32, 0);
	hisifd->set_reg(hisifd, ovl_base + OVL_DST_STARTPOS, s_ovl->ovl_dst_startpos, 32, 0);
	hisifd->set_reg(hisifd, ovl_base + OVL_DST_ENDPOS, s_ovl->ovl_dst_endpos, 32, 0);
	hisifd->set_reg(hisifd, ovl_base + OVL_GCFG, s_ovl->ovl_gcfg, 32, 0);

	for (i = 0; i < OVL_6LAYER_NUM; i++) {
		if (s_ovl->ovl_layer_used[i] == 1) {
			hisifd->set_reg(hisifd, ovl_base + OVL_LAYER0_POS + i * 0x3C,
				s_ovl->ovl_layer[i].layer_pos, 32, 0);
			hisifd->set_reg(hisifd, ovl_base + OVL_LAYER0_SIZE + i * 0x3C,
				s_ovl->ovl_layer[i].layer_size, 32, 0);
			hisifd->set_reg(hisifd, ovl_base + OVL_LAYER0_PATTERN + i * 0x3C,
				s_ovl->ovl_layer[i].layer_pattern, 32, 0);
			hisifd->set_reg(hisifd, ovl_base + OVL_LAYER0_ALPHA + i * 0x3C,
				s_ovl->ovl_layer[i].layer_alpha, 32, 0);
			hisifd->set_reg(hisifd, ovl_base + OVL_LAYER0_CFG + i * 0x3C,
				s_ovl->ovl_layer[i].layer_cfg, 32, 0);

			hisifd->set_reg(hisifd, ovl_base + OVL_LAYER0_PSPOS + i * 0x3C,
				s_ovl->ovl_layer_pos[i].layer_pspos, 32, 0);
			hisifd->set_reg(hisifd, ovl_base + OVL_LAYER0_PEPOS + i * 0x3C,
				s_ovl->ovl_layer_pos[i].layer_pepos, 32, 0);
		} else {
			hisifd->set_reg(hisifd, ovl_base + OVL_LAYER0_POS + i * 0x3C,
				s_ovl->ovl_layer[i].layer_pos, 32, 0);
			hisifd->set_reg(hisifd, ovl_base + OVL_LAYER0_SIZE + i * 0x3C,
				s_ovl->ovl_layer[i].layer_size, 32, 0);
			hisifd->set_reg(hisifd, ovl_base + OVL_LAYER0_CFG + i * 0x3C,
				s_ovl->ovl_layer[i].layer_cfg, 32, 0);
		}
	}

	hisifd->set_reg(hisifd, ovl_base + OVL6_BLOCK_SIZE, s_ovl->ovl_block_size, 32, 0);
}
/*lint +e679 +e730 +e732 +e838*/

void hisi_dss_ov_set_reg_default_value(struct hisi_fb_data_type *hisifd,
	char __iomem *ovl_base, int ovl_idx)
{
	if (hisifd == NULL) {
		HISI_FB_ERR("hisifd is null");
		return;
	}

	if (ovl_base == NULL) {
		HISI_FB_ERR("ovl_base is null");
		return;
	}

	hisifd->set_reg(hisifd, ovl_base + OVL6_REG_DEFAULT, 0x1, 32, 0);
	hisifd->set_reg(hisifd, ovl_base + OVL6_REG_DEFAULT, 0x0, 32, 0);
}
/*lint -e838*/
uint32_t hisi_dss_mif_get_invalid_sel(dss_img_t *img, uint32_t transform, int v_scaling_factor,
	uint8_t is_tile, bool rdma_stretch_enable)
{
	uint32_t invalid_sel_val = 0;
	uint32_t tlb_tag_org = 0;

	if (img == NULL) {
		HISI_FB_ERR("img is null");
		return 0;
	}

	if ((transform == (HISI_FB_TRANSFORM_ROT_90 | HISI_FB_TRANSFORM_FLIP_H))
		|| (transform == (HISI_FB_TRANSFORM_ROT_90 | HISI_FB_TRANSFORM_FLIP_V))) {
		transform = HISI_FB_TRANSFORM_ROT_90;
	}

	tlb_tag_org =  (transform & 0x7) |
		((is_tile ? 1 : 0) << 3) | ((rdma_stretch_enable ? 1 : 0) << 4);

	switch (tlb_tag_org) {
	case MMU_TLB_TAG_ORG_0x0:
		invalid_sel_val = 1;
		break;
	case MMU_TLB_TAG_ORG_0x1:
		invalid_sel_val = 1;
		break;
	case MMU_TLB_TAG_ORG_0x2:
		invalid_sel_val = 2;
		break;
	case MMU_TLB_TAG_ORG_0x3:
		invalid_sel_val = 2;
		break;
	case MMU_TLB_TAG_ORG_0x4:
		invalid_sel_val = 0;
		break;
	case MMU_TLB_TAG_ORG_0x7:
		invalid_sel_val = 0;
		break;

	case MMU_TLB_TAG_ORG_0x8:
		invalid_sel_val = 3;
		break;
	case MMU_TLB_TAG_ORG_0x9:
		invalid_sel_val = 3;
		break;
	case MMU_TLB_TAG_ORG_0xA:
		invalid_sel_val = 3;
		break;
	case MMU_TLB_TAG_ORG_0xB:
		invalid_sel_val = 3;
		break;
	case MMU_TLB_TAG_ORG_0xC:
		invalid_sel_val = 0;
		break;
	case MMU_TLB_TAG_ORG_0xF:
		invalid_sel_val = 0;
		break;

	case MMU_TLB_TAG_ORG_0x10:
		invalid_sel_val = 1;
		break;
	case MMU_TLB_TAG_ORG_0x11:
		invalid_sel_val = 1;
		break;
	case MMU_TLB_TAG_ORG_0x12:
		invalid_sel_val = 2;
		break;
	case MMU_TLB_TAG_ORG_0x13:
		invalid_sel_val = 2;
		break;
	case MMU_TLB_TAG_ORG_0x14:
		invalid_sel_val = 0;
		break;
	case MMU_TLB_TAG_ORG_0x17:
		invalid_sel_val = 0;
		break;

	case MMU_TLB_TAG_ORG_0x18:
		invalid_sel_val = 3;
		break;
	case MMU_TLB_TAG_ORG_0x19:
		invalid_sel_val = 3;
		break;
	case MMU_TLB_TAG_ORG_0x1A:
		invalid_sel_val = 3;
		break;
	case MMU_TLB_TAG_ORG_0x1B:
		invalid_sel_val = 3;
		break;
	case MMU_TLB_TAG_ORG_0x1C:
		invalid_sel_val = 0;
		break;
	case MMU_TLB_TAG_ORG_0x1F:
		invalid_sel_val = 0;
		break;

	default:
		invalid_sel_val = 0;
		HISI_FB_ERR("not support this tlb_tag_org(0x%x)!\n", tlb_tag_org);
		break;
	}

	return invalid_sel_val;
}
/*lint +e838*/
/*******************************************************************************
** DSS ARSR2P
*/
#define ARSR2P_PHASE_NUM	(9)
#define ARSR2P_TAP4	(4)
#define ARSR2P_TAP6	(6)
#define ARSR2P_MIN_INPUT (16)
#define ARSR2P_MAX_WIDTH (2560)
#define ARSR2P_MAX_HEIGHT (8192)
#define ARSR2P_SCALE_MAX (60)


#define ARSR2P_SCL_UP_OFFSET (0x48)
#define ARSR2P_COEF_H0_OFFSET (0x100)
#define ARSR2P_COEF_H1_OFFSET (0x200)

#define LSC_ROW	(2)
#define LSC_COL (27)
//arsr1p lsc gain
static const uint32_t ARSR1P_LSC_GAIN_TABLE[LSC_ROW][LSC_COL] = {
	{1024,1085,1158,1232,1305,1382,1454,1522,1586,1646,1701,1755,1809,1864,1926,1989,2058,2131,2207,2291,2376,2468,2576,2687,2801,2936,3038}, //pgainlsc0
	{1052,1122,1192,1268,1345,1418,1488,1554,1616,1674,1728,1783,1838,1895,1957,2023,2089,2165,2245,2331,2424,2523,2629,2744,2866,3006,3038}  //pgainlsc1
};

//c0, c1, c2, c3
static const int COEF_AUV_SCL_UP_TAP4[ARSR2P_PHASE_NUM][ARSR2P_TAP4] = {
	{ -3, 254, 6, -1},
	{ -9, 255, 13, -3},
	{ -18, 254, 27, -7},
	{ -23, 245, 44, -10},
	{ -27, 233, 64, -14},
	{ -29, 218, 85, -18},
	{ -29, 198, 108, -21},
	{ -29, 177, 132, -24},
	{ -27, 155, 155, -27}
};

//c0, c1, c2, c3
static const int COEF_AUV_SCL_DOWN_TAP4[ARSR2P_PHASE_NUM][ARSR2P_TAP4] = {
	{ 31, 194, 31, 0},
	{ 23, 206, 44, -17},
	{ 14, 203, 57, -18},
	{ 6, 198, 70, -18},
	{ 0, 190, 85, -19},
	{ -5, 180, 99, -18},
	{ -10, 170, 114, -18},
	{ -13, 157, 129, -17},
	{ -15, 143, 143, -15}
};

//c0, c1, c2, c3, c4, c5
static const int COEF_Y_SCL_UP_TAP6[ARSR2P_PHASE_NUM][ARSR2P_TAP6] = {
	{ 0, -3, 254, 6, -1, 0},
	{ 4, -12, 252, 15, -5, 2},
	{ 7, -22, 245, 31, -9, 4},
	{ 10, -29, 234, 49, -14, 6},
	{ 12, -34, 221, 68, -19, 8},
	{ 13, -37, 206, 88, -24, 10},
	{ 14, -38, 189, 108, -29, 12},
	{ 14, -38, 170, 130, -33, 13},
	{ 14, -36, 150, 150, -36, 14}
};

static const int COEF_Y_SCL_DOWN_TAP6[ARSR2P_PHASE_NUM][ARSR2P_TAP6] = {
	{ -22, 43, 214, 43, -22, 0},
	{ -18, 29, 205, 53, -23, 10},
	{ -16, 18, 203, 67, -25, 9},
	{ -13, 9, 198, 80, -26, 8},
	{ -10, 0, 191, 95, -27, 7},
	{ -7, -7, 182, 109, -27, 6},
	{ -5, -14, 174, 124, -27, 4},
	{ -2, -18, 162, 137, -25, 2},
	{ 0, -22, 150, 150, -22, 0}
};

/*******************************************************************************
** DSS ARSR
*/
/*lint -e679, -e701, -e730, -e732, -e838, -e613*/
int hisi_dss_arsr2p_write_coefs(struct hisi_fb_data_type *hisifd, bool enable_cmdlist,
	char __iomem *addr, const int **p, int row, int col)
{
	int coef_value = 0;
	int coef_num = 0;
	int i= 0;
	int j = 0;

	if (NULL == hisifd) {
		HISI_FB_ERR("hisifd is NULL");
		return -EINVAL;
	}
	if (NULL == addr) {
		HISI_FB_ERR("addr is NULL");
		return -EINVAL;
	}

	if ((row != ARSR2P_PHASE_NUM) || ((col != ARSR2P_TAP4) && (col != ARSR2P_TAP6))) {
		HISI_FB_ERR("arsr2p filter coefficients is err, arsr2p_phase_num = %d, arsr2p_tap_num = %d\n", row, col);
		return -EINVAL;
	}

	coef_num = (col == ARSR2P_TAP4 ? 2 : 3);

	for (i = 0; i < row; i++) {
		for (j = 0; j < 2; j++) {
			if (coef_num == 2) {
				coef_value = (*((int*)p + i * col + j * coef_num) & 0x1FF) | ((*((int*)p + i * col + j * coef_num + 1)	& 0x1FF) << 9);
			} else {
				coef_value = (*((int*)p + i * col + j * coef_num) & 0x1FF) | ((*((int*)p + i * col + j * coef_num + 1)	& 0x1FF) << 9) |((*((int*)p + i * col + j * coef_num + 2)  & 0x1FF) << 18);
			}

			if (enable_cmdlist) {
				hisifd->set_reg(hisifd, addr + 0x8 * i + j * 0x4, coef_value, 32, 0);
			} else {
				set_reg(addr + 0x8 * i + j * 0x4, coef_value, 32, 0);
			}
		}
	}

	return 0;
}

void hisi_dss_arsr2p_write_config_coefs(struct hisi_fb_data_type *hisifd, bool enable_cmdlist,
	char __iomem *addr, const int **scl_down, const int **scl_up, int row, int col)
{
	int ret = 0;

	ret = hisi_dss_arsr2p_write_coefs(hisifd, enable_cmdlist, addr, scl_down, row, col);
	if (ret < 0) {
		HISI_FB_ERR("Error to write COEF_SCL_DOWN coefficients.\n");
		return;
	}

	ret = hisi_dss_arsr2p_write_coefs(hisifd, enable_cmdlist, addr + ARSR2P_SCL_UP_OFFSET, scl_up, row, col);
	if (ret < 0) {
		HISI_FB_ERR("Error to write COEF_SCL_UP coefficients.\n");
		return;
	}

}

/*lint -e570*/
void hisi_dss_arsr2p_init(const char __iomem * arsr2p_base, dss_arsr2p_t *s_arsr2p)
{
	if (NULL == arsr2p_base) {
		HISI_FB_ERR("arsr2p_base is NULL");
		return;
	}
	if (NULL == s_arsr2p) {
		HISI_FB_ERR("s_arsr2p is NULL");
		return;
	}

	memset(s_arsr2p, 0, sizeof(dss_arsr2p_t));

	s_arsr2p->arsr_input_width_height = inp32(arsr2p_base + ARSR2P_INPUT_WIDTH_HEIGHT);
	s_arsr2p->arsr_output_width_height = inp32(arsr2p_base + ARSR2P_OUTPUT_WIDTH_HEIGHT);
	s_arsr2p->ihleft = inp32(arsr2p_base + ARSR2P_IHLEFT);
	s_arsr2p->ihright = inp32(arsr2p_base + ARSR2P_IHRIGHT);
	s_arsr2p->ivtop = inp32(arsr2p_base + ARSR2P_IVTOP);
	s_arsr2p->ivbottom = inp32(arsr2p_base + ARSR2P_IVBOTTOM);
	s_arsr2p->ihinc = inp32(arsr2p_base + ARSR2P_IHINC);
	s_arsr2p->ivinc = inp32(arsr2p_base + ARSR2P_IVINC);
	s_arsr2p->offset = inp32(arsr2p_base + ARSR2P_UV_OFFSET);
	s_arsr2p->mode = inp32(arsr2p_base + ARSR2P_MODE);

	s_arsr2p->arsr2p_effect.skin_thres_y = (75 | (83 << 8) | (150 << 16)); //(ythresl | (ythresh << 8) | (yexpectvalue << 16))
	s_arsr2p->arsr2p_effect.skin_thres_u = (5 | (10 << 8) | (113 << 16)); //(uthresl | (uthresh << 8) | (uexpectvalue << 16))
	s_arsr2p->arsr2p_effect.skin_thres_v = (6 | (12 << 8) | (145 << 16)); //(vthresl | (vthresh << 8) | (vexpectvalue << 16))
	s_arsr2p->arsr2p_effect.skin_cfg0 = (512 | (3 << 12)); //(yslop | (clipdata << 12))
	s_arsr2p->arsr2p_effect.skin_cfg1 = (819); //(uslop)
	s_arsr2p->arsr2p_effect.skin_cfg2 = (682); //(vslop)
	s_arsr2p->arsr2p_effect.shoot_cfg1 = (512 | (20 << 16)); //(shootslop1 | (shootgradalpha << 16))
	s_arsr2p->arsr2p_effect.shoot_cfg2 = (-16 & 0x1ff); //(shootgradsubthrl | (shootgradsubthrh << 16)), default shootgradsubthrh is 0
	s_arsr2p->arsr2p_effect.sharp_cfg1 = (2 | (6 << 8) | (48 << 16) | (64 << 24)); //(dvarshctrllow0 | (dvarshctrllow1 << 8) | (dvarshctrlhigh0 << 16) | (dvarshctrlhigh1 << 24))
	s_arsr2p->arsr2p_effect.sharp_cfg2 = (8 | (24 << 8) | (24 << 16) | (40 << 24)); //(sharpshootctrll | (sharpshootctrlh << 8) | (shptocuthigh0 << 16) | (shptocuthigh1 << 24))
	s_arsr2p->arsr2p_effect.sharp_cfg3 = (2 | (1 << 8) | (2500 << 16)); //(blendshshootctrl  | (sharpcoring << 8) | (skinadd2 << 16))
	s_arsr2p->arsr2p_effect.sharp_cfg4 = (10 | (6 << 8) | (6 << 16) | (12 << 24)); //(skinthresh | (skinthresl << 8) | (shctrllowstart << 16) | (shctrlhighend << 24))
	s_arsr2p->arsr2p_effect.sharp_cfg5 = (2 | (12 << 8)); //(linearshratiol | (linearshratioh << 8))
	s_arsr2p->arsr2p_effect.sharp_cfg6 = (640 | (64 << 16)); //(gainctrlslopl | (gainctrlsloph << 16))
	s_arsr2p->arsr2p_effect.sharp_cfg7 = (3 | (250 << 16)); //(sharplevel | (sharpgain << 16))
	s_arsr2p->arsr2p_effect.sharp_cfg8 = (-48000 & 0x3ffffff); //(skinslop1)
	s_arsr2p->arsr2p_effect.sharp_cfg9 = (-32000 & 0x3ffffff); //(skinslop2)
	s_arsr2p->arsr2p_effect.texturw_analysts = (15 | (20 << 16)); //(difflow | (diffhigh << 16))
	s_arsr2p->arsr2p_effect.intplshootctrl = (2); //(intplshootctrl)

	s_arsr2p->arsr2p_effect_scale_up.skin_thres_y = (75 | (83 << 8) | (150 << 16)); //(ythresl | (ythresh << 8) | (yexpectvalue << 16))
	s_arsr2p->arsr2p_effect_scale_up.skin_thres_u = (5 | (10 << 8) | (113 << 16)); //(uthresl | (uthresh << 8) | (uexpectvalue << 16))
	s_arsr2p->arsr2p_effect_scale_up.skin_thres_v = (6 | (12 << 8) | (145 << 16)); //(vthresl | (vthresh << 8) | (vexpectvalue << 16))
	s_arsr2p->arsr2p_effect_scale_up.skin_cfg0 = (512 | (3 << 12)); //(yslop | (clipdata << 12))
	s_arsr2p->arsr2p_effect_scale_up.skin_cfg1 = (819); //(uslop)
	s_arsr2p->arsr2p_effect_scale_up.skin_cfg2 = (682); //(vslop)
	s_arsr2p->arsr2p_effect_scale_up.shoot_cfg1 = (512 | (20 << 16)); //(shootslop1 | (shootgradalpha << 16))
	s_arsr2p->arsr2p_effect_scale_up.shoot_cfg2 = (-16 & 0x1ff); //(shootgradsubthrl | (shootgradsubthrh << 16)), default shootgradsubthrh is 0
	s_arsr2p->arsr2p_effect_scale_up.sharp_cfg1 = (2 | (6 << 8) | (48 << 16) | (64 << 24)); //(dvarshctrllow0 | (dvarshctrllow1 << 8) | (dvarshctrlhigh0 << 16) | (dvarshctrlhigh1 << 24))
	s_arsr2p->arsr2p_effect_scale_up.sharp_cfg2 = (8 | (24 << 8) | (24 << 16) | (40 << 24)); //(sharpshootctrll | (sharpshootctrlh << 8) | (shptocuthigh0 << 16) | (shptocuthigh1 << 24))
	s_arsr2p->arsr2p_effect_scale_up.sharp_cfg3 = (2 | (2 << 8) | (2650 << 16)); //(blendshshootctrl  | (sharpcoring << 8) | (skinadd2 << 16))
	s_arsr2p->arsr2p_effect_scale_up.sharp_cfg4 = (10 | (6 << 8) | (6 << 16) | (13 << 24)); //(skinthresh | (skinthresl << 8) | (shctrllowstart << 16) | (shctrlhighend << 24))
	s_arsr2p->arsr2p_effect_scale_up.sharp_cfg5 = (2 | (12 << 8)); //(linearshratiol | (linearshratioh << 8))
	s_arsr2p->arsr2p_effect_scale_up.sharp_cfg6 = (640 | (48 << 16)); //(gainctrlslopl | (gainctrlsloph << 16))
	s_arsr2p->arsr2p_effect_scale_up.sharp_cfg7 = (3 | (265 << 16)); //(sharplevel | (sharpgain << 16))
	s_arsr2p->arsr2p_effect_scale_up.sharp_cfg8 = (-50880 & 0x3ffffff); //(skinslop1)
	s_arsr2p->arsr2p_effect_scale_up.sharp_cfg9 = (-33920 & 0x3ffffff); //(skinslop2)
	s_arsr2p->arsr2p_effect_scale_up.texturw_analysts = (15 | (20 << 16)); //(difflow | (diffhigh << 16))
	s_arsr2p->arsr2p_effect_scale_up.intplshootctrl = (2); //(intplshootctrl)

	s_arsr2p->arsr2p_effect_scale_down.skin_thres_y = (75 | (83 << 8) | (150 << 16)); //(ythresl | (ythresh << 8) | (yexpectvalue << 16))
	s_arsr2p->arsr2p_effect_scale_down.skin_thres_u = (5 | (10 << 8) | (113 << 16)); //(uthresl | (uthresh << 8) | (uexpectvalue << 16))
	s_arsr2p->arsr2p_effect_scale_down.skin_thres_v = (6 | (12 << 8) | (145 << 16)); //(vthresl | (vthresh << 8) | (vexpectvalue << 16))
	s_arsr2p->arsr2p_effect_scale_down.skin_cfg0 = (512 | (3 << 12)); //(yslop | (clipdata << 12))
	s_arsr2p->arsr2p_effect_scale_down.skin_cfg1 = (819); //(uslop)
	s_arsr2p->arsr2p_effect_scale_down.skin_cfg2 = (682); //(vslop)
	s_arsr2p->arsr2p_effect_scale_down.shoot_cfg1 = (512 | (20 << 16)); //(shootslop1 | (shootgradalpha << 16))
	s_arsr2p->arsr2p_effect_scale_down.shoot_cfg2 = (-16 & 0x1ff); //(shootgradsubthrl | (shootgradsubthrh << 16)), default shootgradsubthrh is 0
	s_arsr2p->arsr2p_effect_scale_down.sharp_cfg1 = (2 | (6 << 8) | (48 << 16) | (64 << 24)); //(dvarshctrllow0 | (dvarshctrllow1 << 8) | (dvarshctrlhigh0 << 16) | (dvarshctrlhigh1 << 24))
	s_arsr2p->arsr2p_effect_scale_down.sharp_cfg2 = (8 | (24 << 8) | (24 << 16) | (40 << 24)); //(sharpshootctrll | (sharpshootctrlh << 8) | (shptocuthigh0 << 16) | (shptocuthigh1 << 24))
	s_arsr2p->arsr2p_effect_scale_down.sharp_cfg3 = (2 | (1 << 8) | (500 << 16)); //(blendshshootctrl  | (sharpcoring << 8) | (skinadd2 << 16))
	s_arsr2p->arsr2p_effect_scale_down.sharp_cfg4 = (10 | (6 << 8) | (6 << 16) | (8 << 24)); //(skinthresh | (skinthresl << 8) | (shctrllowstart << 16) | (shctrlhighend << 24))
	s_arsr2p->arsr2p_effect_scale_down.sharp_cfg5 = (2 | (12 << 8)); //(linearshratiol | (linearshratioh << 8))
	s_arsr2p->arsr2p_effect_scale_down.sharp_cfg6 = (640 | (128 << 16)); //(gainctrlslopl | (gainctrlsloph << 16))
	s_arsr2p->arsr2p_effect_scale_down.sharp_cfg7 = (3 | (50 << 16)); //(sharplevel | (sharpgain << 16))
	s_arsr2p->arsr2p_effect_scale_down.sharp_cfg8 = (-9600 & 0x3ffffff); //(skinslop1)
	s_arsr2p->arsr2p_effect_scale_down.sharp_cfg9 = (-6400 & 0x3ffffff); //(skinslop2)
	s_arsr2p->arsr2p_effect_scale_down.texturw_analysts = (15 | (20 << 16)); //(difflow | (diffhigh << 16))
	s_arsr2p->arsr2p_effect_scale_down.intplshootctrl = (2); //(intplshootctrl)

	s_arsr2p->ihleft1 = inp32(arsr2p_base + ARSR2P_IHLEFT1);
	s_arsr2p->ihright1 = inp32(arsr2p_base + ARSR2P_IHRIGHT1);
	s_arsr2p->ivbottom1 = inp32(arsr2p_base + ARSR2P_IVBOTTOM1);
}
/*lint +e570*/
void hisi_dss_arsr2p_set_reg(struct hisi_fb_data_type *hisifd,
	char __iomem * arsr2p_base, dss_arsr2p_t *s_arsr2p)
{
	if (hisifd == NULL) {
		HISI_FB_ERR("hisifd is null");
		return;
	}

	if (arsr2p_base == NULL) {
		HISI_FB_ERR("arsr2p_base is null");
		return;
	}

	if (s_arsr2p == NULL) {
		HISI_FB_ERR("s_arsr2p is null");
		return;
	}

	hisifd->set_reg(hisifd, arsr2p_base + ARSR2P_INPUT_WIDTH_HEIGHT, s_arsr2p->arsr_input_width_height, 32, 0);
	hisifd->set_reg(hisifd, arsr2p_base + ARSR2P_OUTPUT_WIDTH_HEIGHT, s_arsr2p->arsr_output_width_height, 32, 0);
	hisifd->set_reg(hisifd, arsr2p_base + ARSR2P_IHLEFT, s_arsr2p->ihleft, 32, 0);
	hisifd->set_reg(hisifd, arsr2p_base + ARSR2P_IHRIGHT, s_arsr2p->ihright, 32, 0);
	hisifd->set_reg(hisifd, arsr2p_base + ARSR2P_IVTOP, s_arsr2p->ivtop, 32, 0);
	hisifd->set_reg(hisifd, arsr2p_base + ARSR2P_IVBOTTOM, s_arsr2p->ivbottom, 32, 0);
	hisifd->set_reg(hisifd, arsr2p_base + ARSR2P_IHINC, s_arsr2p->ihinc, 32, 0);
	hisifd->set_reg(hisifd, arsr2p_base + ARSR2P_IVINC, s_arsr2p->ivinc, 32, 0);
	hisifd->set_reg(hisifd, arsr2p_base + ARSR2P_UV_OFFSET, s_arsr2p->offset, 32, 0);
	hisifd->set_reg(hisifd, arsr2p_base + ARSR2P_MODE, s_arsr2p->mode, 32, 0);

	if (hisifd->dss_module.arsr2p_effect_used[DSS_RCHN_V1]) {
		hisifd->set_reg(hisifd, arsr2p_base + ARSR2P_SKIN_THRES_Y, s_arsr2p->arsr2p_effect.skin_thres_y, 32, 0);
		hisifd->set_reg(hisifd, arsr2p_base + ARSR2P_SKIN_THRES_U, s_arsr2p->arsr2p_effect.skin_thres_u, 32, 0);
		hisifd->set_reg(hisifd, arsr2p_base + ARSR2P_SKIN_THRES_V, s_arsr2p->arsr2p_effect.skin_thres_v, 32, 0);
		hisifd->set_reg(hisifd, arsr2p_base + ARSR2P_SKIN_CFG0, s_arsr2p->arsr2p_effect.skin_cfg0, 32, 0);
		hisifd->set_reg(hisifd, arsr2p_base + ARSR2P_SKIN_CFG1, s_arsr2p->arsr2p_effect.skin_cfg1, 32, 0);
		hisifd->set_reg(hisifd, arsr2p_base + ARSR2P_SKIN_CFG2, s_arsr2p->arsr2p_effect.skin_cfg2, 32, 0);
		hisifd->set_reg(hisifd, arsr2p_base + ARSR2P_SHOOT_CFG1, s_arsr2p->arsr2p_effect.shoot_cfg1, 32, 0);
		hisifd->set_reg(hisifd, arsr2p_base + ARSR2P_SHOOT_CFG2, s_arsr2p->arsr2p_effect.shoot_cfg2, 32, 0);
		hisifd->set_reg(hisifd, arsr2p_base + ARSR2P_SHARP_CFG1, s_arsr2p->arsr2p_effect.sharp_cfg1, 32, 0);
		hisifd->set_reg(hisifd, arsr2p_base + ARSR2P_SHARP_CFG2, s_arsr2p->arsr2p_effect.sharp_cfg2, 32, 0);
		hisifd->set_reg(hisifd, arsr2p_base + ARSR2P_SHARP_CFG3, s_arsr2p->arsr2p_effect.sharp_cfg3, 32, 0);
		hisifd->set_reg(hisifd, arsr2p_base + ARSR2P_SHARP_CFG4, s_arsr2p->arsr2p_effect.sharp_cfg4, 32, 0);
		hisifd->set_reg(hisifd, arsr2p_base + ARSR2P_SHARP_CFG5, s_arsr2p->arsr2p_effect.sharp_cfg5, 32, 0);
		hisifd->set_reg(hisifd, arsr2p_base + ARSR2P_SHARP_CFG6, s_arsr2p->arsr2p_effect.sharp_cfg6, 32, 0);
		hisifd->set_reg(hisifd, arsr2p_base + ARSR2P_SHARP_CFG7, s_arsr2p->arsr2p_effect.sharp_cfg7, 32, 0);
		hisifd->set_reg(hisifd, arsr2p_base + ARSR2P_SHARP_CFG8, s_arsr2p->arsr2p_effect.sharp_cfg8, 32, 0);
		hisifd->set_reg(hisifd, arsr2p_base + ARSR2P_SHARP_CFG9, s_arsr2p->arsr2p_effect.sharp_cfg9, 32, 0);
		hisifd->set_reg(hisifd, arsr2p_base + ARSR2P_TEXTURW_ANALYSTS, s_arsr2p->arsr2p_effect.texturw_analysts, 32, 0);
		hisifd->set_reg(hisifd, arsr2p_base + ARSR2P_INTPLSHOOTCTRL, s_arsr2p->arsr2p_effect.intplshootctrl, 32, 0);
	}
	hisifd->set_reg(hisifd, arsr2p_base + ARSR2P_IHLEFT1, s_arsr2p->ihleft1, 32, 0);
	hisifd->set_reg(hisifd, arsr2p_base + ARSR2P_IHRIGHT1, s_arsr2p->ihright1, 32, 0);
	hisifd->set_reg(hisifd, arsr2p_base + ARSR2P_IVBOTTOM1, s_arsr2p->ivbottom1, 32, 0);
}


void hisi_dss_arsr2p_coef_on(struct hisi_fb_data_type *hisifd, bool enable_cmdlist)
{
	uint32_t module_base = 0;
	char __iomem *arsr2p_base;
	char __iomem *coefy_v = NULL;
	char __iomem *coefa_v = NULL;
	char __iomem *coefuv_v = NULL;

	if (NULL == hisifd) {
		HISI_FB_ERR("hisifd is NULL");
		return;
	}

	module_base = g_dss_module_base[DSS_RCHN_V1][MODULE_ARSR2P_LUT];
	coefy_v = hisifd->dss_base + module_base + ARSR2P_LUT_COEFY_V_OFFSET;
	coefa_v = hisifd->dss_base + module_base + ARSR2P_LUT_COEFA_V_OFFSET;
	coefuv_v = hisifd->dss_base + module_base + ARSR2P_LUT_COEFUV_V_OFFSET;
	arsr2p_base = hisifd->dss_base + g_dss_module_base[DSS_RCHN_V1][MODULE_ARSR2P];

	/* COEFY_V COEFY_H */
	hisi_dss_arsr2p_write_config_coefs(hisifd, enable_cmdlist, coefy_v, (const int **)COEF_Y_SCL_DOWN_TAP6, (const int **)COEF_Y_SCL_UP_TAP6, ARSR2P_PHASE_NUM, ARSR2P_TAP6);
	hisi_dss_arsr2p_write_config_coefs(hisifd, enable_cmdlist, coefy_v + ARSR2P_COEF_H0_OFFSET, (const int **)COEF_Y_SCL_DOWN_TAP6, (const int **)COEF_Y_SCL_UP_TAP6, ARSR2P_PHASE_NUM, ARSR2P_TAP6);
	hisi_dss_arsr2p_write_config_coefs(hisifd, enable_cmdlist, coefy_v + ARSR2P_COEF_H1_OFFSET, (const int **)COEF_Y_SCL_DOWN_TAP6, (const int **)COEF_Y_SCL_UP_TAP6, ARSR2P_PHASE_NUM, ARSR2P_TAP6);

	/* COEFA_V COEFA_H */
	hisi_dss_arsr2p_write_config_coefs(hisifd, enable_cmdlist, coefa_v, (const int **)COEF_AUV_SCL_DOWN_TAP4, (const int **)COEF_AUV_SCL_UP_TAP4, ARSR2P_PHASE_NUM, ARSR2P_TAP4);
	hisi_dss_arsr2p_write_config_coefs(hisifd, enable_cmdlist, coefa_v + ARSR2P_COEF_H0_OFFSET, (const int **)COEF_AUV_SCL_DOWN_TAP4, (const int **)COEF_AUV_SCL_UP_TAP4, ARSR2P_PHASE_NUM, ARSR2P_TAP4);
	hisi_dss_arsr2p_write_config_coefs(hisifd, enable_cmdlist, coefa_v + ARSR2P_COEF_H1_OFFSET, (const int **)COEF_AUV_SCL_DOWN_TAP4, (const int **)COEF_AUV_SCL_UP_TAP4, ARSR2P_PHASE_NUM, ARSR2P_TAP4);

	/* COEFUV_V COEFUV_H */
	hisi_dss_arsr2p_write_config_coefs(hisifd, enable_cmdlist, coefuv_v, (const int **)COEF_AUV_SCL_DOWN_TAP4, (const int **)COEF_AUV_SCL_UP_TAP4, ARSR2P_PHASE_NUM, ARSR2P_TAP4);
	hisi_dss_arsr2p_write_config_coefs(hisifd, enable_cmdlist, coefuv_v + ARSR2P_COEF_H0_OFFSET, (const int **)COEF_AUV_SCL_DOWN_TAP4, (const int **)COEF_AUV_SCL_UP_TAP4, ARSR2P_PHASE_NUM, ARSR2P_TAP4);
	hisi_dss_arsr2p_write_config_coefs(hisifd, enable_cmdlist, coefuv_v + ARSR2P_COEF_H1_OFFSET, (const int **)COEF_AUV_SCL_DOWN_TAP4, (const int **)COEF_AUV_SCL_UP_TAP4, ARSR2P_PHASE_NUM, ARSR2P_TAP4);
}

static int hisi_dss_arsr2p_config_check_width(dss_rect_t *dest_rect, int source_width, bool hscl_en, bool vscl_en)
{
	if (NULL == dest_rect) {
		HISI_FB_ERR("dest_rect is NULL");
		return -EINVAL;
	}

	/*check arsr2p input and output width*/
	if ((source_width < ARSR2P_MIN_INPUT) || (dest_rect->w < ARSR2P_MIN_INPUT) ||
		(source_width > ARSR2P_MAX_WIDTH) || (dest_rect->w > ARSR2P_MAX_WIDTH)) {
		if ((!hscl_en) && (!vscl_en)) {
			//sharpen_en = false;
			HISI_FB_INFO("src_rect.w(%d) or dst_rect.w(%d) is smaller than 16 or larger than 2560, arsr2p bypass!\n",
				source_width, dest_rect->w);
			return 0;
		} else {
			HISI_FB_ERR("src_rect.w(%d) or dst_rect.w(%d) is smaller than 16 or larger than 2560!\n",
				source_width, dest_rect->w);
			return -EINVAL;
		}
	}
	return 1;
}


static int hisi_dss_arsr2p_config_check_heigh(dss_rect_t *dest_rect, dss_rect_t *source_rect,dss_layer_t *layer, int source_width)
{
	if (NULL == dest_rect) {
		HISI_FB_ERR("dest_rect is NULL");
		return -EINVAL;
	}
	if (NULL == source_rect) {
		HISI_FB_ERR("source_rect is NULL");
		return -EINVAL;
	}
	if (NULL == layer) {
		HISI_FB_ERR("layer is NULL");
		return -EINVAL;
	}

	if ((dest_rect->w > (source_width * ARSR2P_SCALE_MAX))
		|| (source_width > (dest_rect->w * ARSR2P_SCALE_MAX))) {
		HISI_FB_ERR("width out of range, original_src_rec(%d, %d, %d, %d) "
			"new_src_rect(%d, %d, %d, %d), dst_rect(%d, %d, %d, %d)\n",
			layer->src_rect.x, layer->src_rect.y, source_width, layer->src_rect.h,
			source_rect->x, source_rect->y, source_width, source_rect->h,
			dest_rect->x, dest_rect->y, dest_rect->w, dest_rect->h);

		return -EINVAL;
	}

	/*check arsr2p input and output height*/
	if ((source_rect->h > ARSR2P_MAX_HEIGHT) || (dest_rect->h > ARSR2P_MAX_HEIGHT)) {
		HISI_FB_ERR("src_rect.h(%d) or dst_rect.h(%d) is smaller than 16 or larger than 8192!\n",
			source_rect->h, dest_rect->h);
		return -EINVAL;
	}

	if ((dest_rect->h > (source_rect->h * ARSR2P_SCALE_MAX))
		|| (source_rect->h > (dest_rect->h * ARSR2P_SCALE_MAX))) {
		HISI_FB_ERR("height out of range, original_src_rec(%d, %d, %d, %d) "
			"new_src_rect(%d, %d, %d, %d), dst_rect(%d, %d, %d, %d).\n",
			layer->src_rect.x, layer->src_rect.y, layer->src_rect.w, layer->src_rect.h,
			source_rect->x, source_rect->y, source_rect->w, source_rect->h,
			dest_rect->x, dest_rect->y, dest_rect->w, dest_rect->h);
		return -EINVAL;
	}
	return 0;
}

int hisi_dss_arsr2p_config(struct hisi_fb_data_type *hisifd, dss_layer_t *layer, dss_rect_t *aligned_rect, bool rdma_stretch_enable)
{
	dss_arsr2p_t *arsr2p = NULL;
	dss_rect_t src_rect;
	dss_rect_t dst_rect;
	uint32_t need_cap = 0;
	int chn_idx = 0;
	dss_block_info_t *pblock_info = NULL;
	int extraw = 0, extraw_left = 0, extraw_right = 0;

	bool en_hscl = false;
	bool en_vscl = false;

	/* arsr mode */
	bool imageintpl_dis = false; //bit8
	bool hscldown_enabled = false; //bit7
	bool nearest_en = false; //bit6
	bool diintpl_en = false; //bit5
	bool textureanalyhsisen_en = false; //bit4
	bool arsr2p_bypass = true; //bit0

	bool hscldown_flag = false;

	int ih_inc = 0;
	int iv_inc = 0;
	int ih_left = 0;  //input left acc
	int ih_right = 0; //input end position
	int iv_top = 0; //input top position
	int iv_bottom = 0; //input bottom position
	int uv_offset = 0;
	int src_width = 0;
	int dst_whole_width = 0;
	int ret = 0;

	int outph_left = 0;  //output left acc
	int outph_right = 0; //output end position
	int outpv_bottom = 0; //output bottom position

	if (NULL == hisifd) {
		HISI_FB_ERR("hisifd is NULL");
		return -EINVAL;
	}
	if (NULL == layer) {
		HISI_FB_ERR("layer is NULL");
		return -EINVAL;
	}

	chn_idx = layer->chn_idx;
	if (chn_idx != DSS_RCHN_V1) {
		return 0;
	}


	need_cap = layer->need_cap;

	src_rect = layer->src_rect;
	dst_rect = layer->dst_rect;
	pblock_info = &(layer->block_info);

	//if (pblock_info && pblock_info->h_ratio_arsr2p && pblock_info->both_vscfh_arsr2p_used) { //new added
	if (pblock_info && pblock_info->h_ratio_arsr2p) { //new added
		src_rect = pblock_info->arsr2p_in_rect; //src_rect = arsr2p_in_rect when both arsr2p and vscfh are extended
	}

	/* if vertical ratio of scaling down is larger than or equal to 2, set src_rect height to aligned rect height */
	/*if (aligned_rect) {
		src_rect.h = aligned_rect->h;
	}*/
	src_rect.h = aligned_rect->h;

	if (g_debug_ovl_offline_composer) {
		HISI_FB_INFO("aligned_rect = (%d, %d, %d, %d)\n", aligned_rect->x, aligned_rect->y, aligned_rect->w, aligned_rect->h);
		HISI_FB_INFO("layer->src_rect = (%d, %d, %d, %d)\n", layer->src_rect.x, layer->src_rect.y, layer->src_rect.w, layer->src_rect.h);
		HISI_FB_INFO("layer->dst rect = (%d, %d, %d, %d)\n", layer->dst_rect.x, layer->dst_rect.y, layer->dst_rect.w, layer->dst_rect.h);
		HISI_FB_INFO("arsr2p_in_rect rect = (%d, %d, %d, %d)\n", pblock_info->arsr2p_in_rect.x, pblock_info->arsr2p_in_rect.y,
			pblock_info->arsr2p_in_rect.w, pblock_info->arsr2p_in_rect.h);
	}


	/* horizental scaler compute */
	do {
            //offline subblock
		if (pblock_info && pblock_info->h_ratio_arsr2p) {
			ih_inc = pblock_info->h_ratio_arsr2p;
			src_width = src_rect.w;
			dst_whole_width = pblock_info->arsr2p_dst_w;
			src_rect.x = src_rect.x - pblock_info->arsr2p_src_x;
			src_rect.y = src_rect.y - pblock_info->arsr2p_src_y;
			dst_rect.x = dst_rect.x - pblock_info->arsr2p_dst_x;
			dst_rect.y = dst_rect.y - pblock_info->arsr2p_dst_y;

			if (pblock_info->both_vscfh_arsr2p_used) {
				hscldown_flag = true; //horizental scaling down
			}

			if (rdma_stretch_enable) {
				en_hscl = true;
			}

			if (ih_inc && ih_inc != ARSR2P_INC_FACTOR) {
				en_hscl = true;
			}
		} else {
			/* horizental scaling down is not supported by arsr2p, set src_rect.w = dst_rect.w */
			if (src_rect.w > dst_rect.w) {
				src_width = dst_rect.w;
				hscldown_flag = true; //horizental scaling down
			} else {
				src_width = src_rect.w;
			}
			dst_whole_width = dst_rect.w;

			src_rect.x = 0;  //set src rect to zero, in case
			src_rect.y = 0;
			dst_rect.x = 0;  //set dst rect to zero, in case
			dst_rect.y = 0;

			if (src_width != dst_rect.w)
				en_hscl = true;

			//ihinc=(arsr_input_width*65536+65536-ihleft)/(arsr_output_width+1)
			ih_inc = (DSS_WIDTH(src_width) * ARSR2P_INC_FACTOR + ARSR2P_INC_FACTOR -ih_left) /dst_rect.w;
		}

		//ihleft1 = starto*ihinc - (strati <<16)
		outph_left = dst_rect.x * ih_inc - (src_rect.x * ARSR2P_INC_FACTOR);
		if (outph_left < 0) outph_left = 0;

		//ihleft = ihleft1 - even(8*65536/ihinc) * ihinc
		extraw = (8 * ARSR2P_INC_FACTOR) / ih_inc;/*lint !e414 */
		extraw_left = (extraw % 2) ? (extraw + 1) : (extraw);
		ih_left = outph_left - extraw_left * ih_inc;
		if (ih_left < 0) ih_left = 0;

		//ihright1 = endo * ihinc - (strati <<16);
		outph_right = (dst_rect.x + dst_rect.w - 1) * ih_inc - (src_rect.x * ARSR2P_INC_FACTOR);

		if (dst_whole_width == dst_rect.w) {
			//ihright = ihright1 + even(2*65536/ihinc) * ihinc
			extraw = (2 * ARSR2P_INC_FACTOR) / ih_inc;/*lint !e414 */
			extraw_right = (extraw % 2) ? (extraw + 1) : (extraw);
			ih_right = outph_right + extraw_right * ih_inc;

			/*if(ihright+(starti << 16)) >(width - 1)* ihinc);
			ihright = endo*ihinc-(starti<<16);*/
			extraw = (dst_whole_width - 1) * ih_inc - (src_rect.x * ARSR2P_INC_FACTOR);  //ihright is checked in every tile

			if (ih_right > extraw) {
				ih_right = extraw;
			}
		} else {
			//(endi-starti+1) << 16 - 1
			ih_right = src_width * ARSR2P_INC_FACTOR - 1;
		}
	} while(0);

	/* vertical scaler compute */
	do {
		if (src_rect.h != dst_rect.h)
			en_vscl = true;

		if (src_rect.h > dst_rect.h) {
			//ivinc=(arsr_input_height*65536+65536/2-ivtop)/(arsr_output_height)
			iv_inc = (DSS_HEIGHT(src_rect.h) * ARSR2P_INC_FACTOR + ARSR2P_INC_FACTOR / 2 - iv_top) /
				DSS_HEIGHT(dst_rect.h);
		} else {
			//ivinc=(arsr_input_height*65536+65536-ivtop)/(arsr_output_height+1)
			iv_inc = (DSS_HEIGHT(src_rect.h) * ARSR2P_INC_FACTOR + ARSR2P_INC_FACTOR - iv_top) /dst_rect.h;
		}

		//ivbottom = arsr_output_height*ivinc + ivtop
		iv_bottom = DSS_HEIGHT(dst_rect.h) * iv_inc + iv_top;
		outpv_bottom = iv_bottom;

	} while(0);

	if ((!en_hscl) && (!en_vscl)) {

		if (hisifd->ov_req.wb_compose_type == DSS_WB_COMPOSE_COPYBIT) {
			return 0;
		}

		if (!hscldown_flag){
			/*if only sharpness is needed, disable image interplo, enable textureanalyhsis*/
			imageintpl_dis = true;
			textureanalyhsisen_en = true;
		}
	}

	arsr2p = &(hisifd->dss_module.arsr2p[chn_idx]);
	hisifd->dss_module.arsr2p_used[chn_idx] = 1;

	/*check arsr2p input and output width*/
	ret = hisi_dss_arsr2p_config_check_width(&dst_rect, src_width, en_hscl, en_vscl);
	if (ret <= 0) {
		return ret;
	}

	ret = hisi_dss_arsr2p_config_check_heigh(&dst_rect, &src_rect, layer, src_width);
	if (ret < 0) {
		return ret;
	}

	/*if arsr2p is enabled, hbp+hfp+hsw > 20*/
	/*if (hisifd_primary && (hisifd_primary->panel_info.ldi.h_back_porch + hisifd_primary->panel_info.ldi.h_front_porch
        + hisifd_primary->panel_info.ldi.h_pulse_width) <= 20) {
		HISI_FB_ERR("ldi hbp+hfp+hsw is not larger than 20, return!\n");
		return -EINVAL;
	}*/

	/*config arsr2p mode , start*/
	arsr2p_bypass = false;
	do {
		if (hscldown_flag) { //horizental scale down
			hscldown_enabled = true;
			break;
		}

		if (!en_hscl && (iv_inc >= 2 * ARSR2P_INC_FACTOR) && !pblock_info->h_ratio_arsr2p) {
			//only vertical scale down, enable nearest scaling down, disable sharp in non-block scene
			nearest_en = true;
			break;
		}

		if ((!en_hscl) && (!en_vscl)) {
			break;
		}

		diintpl_en = true;
		//imageintpl_dis = true;
		textureanalyhsisen_en = true;
	} while(0);
	/*config arsr2p mode , end*/

	/*config the effect parameters as long as arsr2p is used*/
	hisi_effect_arsr2p_config(&(arsr2p->arsr2p_effect), ih_inc, iv_inc);
	hisifd->dss_module.arsr2p_effect_used[chn_idx] = 1;


	arsr2p->arsr_input_width_height = set_bits32(arsr2p->arsr_input_width_height, DSS_HEIGHT(src_rect.h), 13, 0);
	arsr2p->arsr_input_width_height = set_bits32(arsr2p->arsr_input_width_height, DSS_WIDTH(src_width), 13, 16);
	arsr2p->arsr_output_width_height = set_bits32(arsr2p->arsr_output_width_height, DSS_HEIGHT(dst_rect.h), 13, 0);
	arsr2p->arsr_output_width_height = set_bits32(arsr2p->arsr_output_width_height, DSS_WIDTH(dst_rect.w), 13, 16);
	arsr2p->ihleft = set_bits32(arsr2p->ihleft, ih_left, 29, 0);
	arsr2p->ihright = set_bits32(arsr2p->ihright, ih_right, 29, 0);
	arsr2p->ivtop = set_bits32(arsr2p->ivtop, iv_top, 29, 0);
	arsr2p->ivbottom = set_bits32(arsr2p->ivbottom, iv_bottom, 29, 0);
	arsr2p->ihinc = set_bits32(arsr2p->ihinc, ih_inc, 22, 0);
	arsr2p->ivinc = set_bits32(arsr2p->ivinc, iv_inc, 22, 0);
	arsr2p->offset = set_bits32(arsr2p->offset, uv_offset, 22, 0);

	arsr2p->mode = set_bits32(arsr2p->mode, arsr2p_bypass, 1, 0);
	arsr2p->mode = set_bits32(arsr2p->mode, arsr2p->arsr2p_effect.sharp_enable, 1, 1);
	arsr2p->mode = set_bits32(arsr2p->mode, arsr2p->arsr2p_effect.shoot_enable, 1, 2);
	arsr2p->mode = set_bits32(arsr2p->mode, arsr2p->arsr2p_effect.skin_enable, 1, 3);
	arsr2p->mode = set_bits32(arsr2p->mode, textureanalyhsisen_en, 1, 4);
	arsr2p->mode = set_bits32(arsr2p->mode, diintpl_en, 1, 5);
	arsr2p->mode = set_bits32(arsr2p->mode, nearest_en, 1, 6);
	arsr2p->mode = set_bits32(arsr2p->mode, hscldown_enabled, 1, 7);
	arsr2p->mode = set_bits32(arsr2p->mode, imageintpl_dis, 1, 8);

	arsr2p->ihleft1 = set_bits32(arsr2p->ihleft1, outph_left, 29, 0);
	arsr2p->ihright1 = set_bits32(arsr2p->ihright1, outph_right, 29, 0);
	arsr2p->ivbottom1 = set_bits32(arsr2p->ivbottom1, outpv_bottom, 29, 0);

	return 0;
}



/*lint +e679, +e701, +e730, +e732, +e838, +e613*/
/*lint -e679, -e701, -e730, -e732 -e838*/
int hisi_dss_post_scl_load_filter_coef(struct hisi_fb_data_type *hisifd, bool enable_cmdlist,
	char __iomem *scl_lut_base, int coef_lut_idx)
{
	char __iomem *h0_y_addr = NULL;
	char __iomem *y_addr = NULL;
	char __iomem *uv_addr = NULL;
	int ret = 0;

	if (NULL == hisifd) {
		HISI_FB_ERR("hisifd is NULL");
		return -EINVAL;
	}
	if (NULL == scl_lut_base) {
		HISI_FB_ERR("scl_lut_base is NULL");
		return -EINVAL;
	}

	h0_y_addr = scl_lut_base + DSS_SCF_H0_Y_COEF_OFFSET;
	y_addr = scl_lut_base + DSS_SCF_Y_COEF_OFFSET;
	uv_addr = scl_lut_base + DSS_SCF_UV_COEF_OFFSET;

	ret = hisi_dss_scl_write_coefs(hisifd, enable_cmdlist, h0_y_addr, (const int **)COEF_LUT_TAP6[coef_lut_idx], PHASE_NUM, TAP6);
	if (ret < 0) {
		HISI_FB_ERR("Error to write H0_Y_COEF coefficients.\n");
	}

	ret = hisi_dss_scl_write_coefs(hisifd, enable_cmdlist, y_addr, (const int **)COEF_LUT_TAP5[coef_lut_idx], PHASE_NUM, TAP5);
	if (ret < 0) {
		HISI_FB_ERR("Error to write Y_COEF coefficients.\n");
	}

	ret = hisi_dss_scl_write_coefs(hisifd, enable_cmdlist, uv_addr, (const int **)COEF_LUT_TAP4[coef_lut_idx], PHASE_NUM, TAP4);
	if (ret < 0) {
		HISI_FB_ERR("Error to write UV_COEF coefficients.\n");
	}

	return ret;
}
/*lint +e613 +838*/

/*******************************************************************************
** DSS remove mctl ch&ov mutex for offline
*/
/*lint -e838*/
void hisi_remove_mctl_mutex(struct hisi_fb_data_type *hisifd, int mctl_idx, uint32_t cmdlist_idxs)
{
	dss_module_reg_t *dss_module = NULL;
	int i = 0;
	char __iomem *chn_mutex_base = NULL;
	char __iomem *cmdlist_base = NULL;
	uint32_t offset = 0;
	uint32_t cmdlist_idxs_temp = 0;

	if (NULL == hisifd) {
		HISI_FB_ERR("hisifd is NULL");
		return;
	}

	dss_module = &(hisifd->dss_module);
	cmdlist_base = hisifd->dss_base + DSS_CMDLIST_OFFSET;

	for (i = 0; i < DSS_CHN_MAX_DEFINE; i++) {
		if (dss_module->mctl_ch_used[i] == 1) {
			chn_mutex_base = dss_module->mctl_ch_base[i].chn_mutex_base +
				g_dss_module_ovl_base[mctl_idx][MODULE_MCTL_BASE];
			if (NULL == chn_mutex_base) {
				HISI_FB_ERR("chn_mutex_base is NULL");
				return;
			}

			set_reg(chn_mutex_base, 0, 32, 0);
		}
	}

	set_reg(dss_module->mctl_base[mctl_idx] + MCTL_CTL_MUTEX_OV, 0, 32, 0);

	offset = 0x40;
	cmdlist_idxs_temp = cmdlist_idxs;

	for (i = 0; i < HISI_DSS_CMDLIST_MAX; i++) {
		if ((cmdlist_idxs_temp & 0x1) == 0x1) {
			if((i != 2) && (i != 3) && (i != 9) && (i != 11)){
			set_reg(cmdlist_base + CMDLIST_CH0_CTRL + i * offset, 0x6, 3, 2); //start sel
			}
		}

		cmdlist_idxs_temp = cmdlist_idxs_temp >> 1;
	}

}
/*lint +e838*/
void hisi_dss_mctl_ov_set_ctl_dbg_reg(struct hisi_fb_data_type *hisifd, char __iomem *mctl_base, bool enable_cmdlist)
{
	if (hisifd == NULL) {
		HISI_FB_ERR("hisifd is null");
		return;
	}

	if (mctl_base == NULL) {
		HISI_FB_ERR("mctl_base is null!\n");
		return;
	}

	if (enable_cmdlist) {
		set_reg(mctl_base + MCTL_CTL_DBG, 0xB03A20, 32, 0);
		set_reg(mctl_base + MCTL_CTL_TOP, 0x1, 32, 0);
	} else {
		set_reg(mctl_base + MCTL_CTL_DBG, 0xB13A00, 32, 0);
		if (hisifd->index == PRIMARY_PANEL_IDX) {
			set_reg(mctl_base + MCTL_CTL_TOP, 0x2, 32, 0);
		} else if (hisifd->index == EXTERNAL_PANEL_IDX) {
			set_reg(mctl_base + MCTL_CTL_TOP, 0x3, 32, 0);
		} else {
			;
		}
	}
}

void hisi_dss_post_clip_set_reg(struct hisi_fb_data_type *hisifd,
	char __iomem *post_clip_base, dss_post_clip_t *s_post_clip, int chn_idx)
{
	if (NULL == hisifd || NULL == post_clip_base || NULL == s_post_clip) {
		HISI_FB_ERR("hisifd post_clip_base or s_post_clip is null\n");
		return;
	}

	post_clip_base = hisifd->dss_base + g_dss_module_base[chn_idx][MODULE_POST_CLIP_ES];
	hisifd->set_reg(hisifd, post_clip_base + POST_CLIP_DISP_SIZE_ES, s_post_clip->disp_size, 32, 0);
	hisifd->set_reg(hisifd, post_clip_base + POST_CLIP_CTL_HRZ_ES, s_post_clip->clip_ctl_hrz, 32, 0);
	hisifd->set_reg(hisifd, post_clip_base + POST_CLIP_CTL_VRZ_ES, s_post_clip->clip_ctl_vrz, 32, 0);
	hisifd->set_reg(hisifd, post_clip_base + POST_CLIP_EN_ES, s_post_clip->ctl_clip_en, 32, 0);
}

static int hisi_dss_check_userdata_check(struct hisi_fb_data_type *hisifd,
	dss_overlay_t *pov_req, dss_overlay_block_t *pov_h_block_infos)
{
	if (hisifd == NULL) {
		HISI_FB_ERR("invalid hisifd!");
		return -EINVAL;
	}

	if (pov_req == NULL) {
		HISI_FB_ERR("fb%d, invalid pov_req!", hisifd->index);
		return -EINVAL;
	}

	if (pov_h_block_infos == NULL) {
		HISI_FB_ERR("fb%d, invalid pov_h_block_infos!", hisifd->index);
		return -EINVAL;
	}

	if ((pov_req->ov_block_nums <= 0) ||
		(pov_req->ov_block_nums > HISI_DSS_OV_BLOCK_NUMS)) {
		HISI_FB_ERR("fb%d, invalid ov_block_nums=%d!",
			hisifd->index, pov_req->ov_block_nums);
		return -EINVAL;
	}

	if ((pov_h_block_infos->layer_nums <= 0)
		|| (pov_h_block_infos->layer_nums > OVL_LAYER_NUM_MAX)) {
		HISI_FB_ERR("fb%d, invalid layer_nums=%d!",
			hisifd->index, pov_h_block_infos->layer_nums);
		return -EINVAL;
	}

	if ((pov_req->ovl_idx != DSS_OVL0) &&
		(pov_req->ovl_idx != DSS_OVL2)) {
		HISI_FB_ERR("fb%d, invalid ovl_idx=%d!\n", hisifd->index, pov_req->ovl_idx);
		return -EINVAL;
	}

	if ((hisifd->index != PRIMARY_PANEL_IDX) && (hisifd->index != AUXILIARY_PANEL_IDX)){
		HISI_FB_ERR("hisi fb%d is invalid!\n", hisifd->index);
		return -EINVAL;
	}
	return 0;
}


/*lint -e613*/
static int hisi_dss_check_wblayer_buff(struct hisi_fb_data_type *hisifd, dss_wb_layer_t *wb_layer)
{
	if (hisi_dss_check_addr_validate(&wb_layer->dst)) {
		return 0;
	}

	if (wb_layer->need_cap & (CAP_BASE | CAP_DIM | CAP_PURE_COLOR)) {
		return 0;
	}

	return -EINVAL;
}
/*lint +e613*/

static int hisi_dss_check_userdata_dst(dss_wb_layer_t *wb_layer, uint32_t index)
{
	if (wb_layer == NULL) {
		HISI_FB_ERR("fb%d, invalid wb_layer!", index);
		return -EINVAL;
	}

	if (wb_layer->need_cap & CAP_AFBCE) {
		if ((wb_layer->dst.afbc_header_stride == 0) || (wb_layer->dst.afbc_payload_stride == 0)) {
			HISI_FB_ERR("fb%d, afbc_header_stride = %d, afbc_payload_stride = %d is invalid!",
				index, wb_layer->dst.afbc_header_stride, wb_layer->dst.afbc_payload_stride);
			return -EINVAL;
		}
	}

	if (wb_layer->need_cap & CAP_HFBCE) {
		if ((wb_layer->dst.hfbc_header_stride0 == 0) || (wb_layer->dst.hfbc_payload_stride0 == 0) ||
			(wb_layer->dst.hfbc_header_stride1 == 0) || (wb_layer->dst.hfbc_payload_stride1 == 0) ||
			(wb_layer->chn_idx != DSS_WCHN_W1)) {
				HISI_FB_ERR("fb%d, hfbc_header_stride0 = %d, hfbc_payload_stride0 = %d,"
					"hfbc_header_stride1 = %d, hfbc_payload_stride1 = %d is invalid or wchn_idx = %d no support hfbce!\n",
					index, wb_layer->dst.hfbc_header_stride0, wb_layer->dst.hfbc_payload_stride0,
					wb_layer->dst.hfbc_header_stride1, wb_layer->dst.hfbc_payload_stride1, wb_layer->chn_idx);
				return -EINVAL;
		}
	}
	return 0;
}

int hisi_dss_check_userdata(struct hisi_fb_data_type *hisifd,
	dss_overlay_t *pov_req, dss_overlay_block_t *pov_h_block_infos)
{
	int i = 0;
	dss_wb_layer_t *wb_layer = NULL;

	if (hisifd == NULL) {
		HISI_FB_ERR("invalid hisifd!");
		return -EINVAL;
	}

	if (pov_req == NULL) {
		HISI_FB_ERR("fb%d, invalid pov_req!", hisifd->index);
		return -EINVAL;
	}

	if (pov_h_block_infos == NULL) {
		HISI_FB_ERR("fb%d, invalid pov_h_block_infos!", hisifd->index);
		return -EINVAL;
	}

	if (hisi_dss_check_userdata_check(hisifd, pov_req, pov_h_block_infos) == -EINVAL) {
		return -EINVAL;
	}

	if (hisifd->index == PRIMARY_PANEL_IDX) {
		if (hisifd->panel_info.dirty_region_updt_support) {
			if (pov_req->dirty_rect.x < 0 || pov_req->dirty_rect.y < 0 ||
				pov_req->dirty_rect.w < 0 || pov_req->dirty_rect.h < 0) {
				HISI_FB_ERR("dirty_rect(%d, %d, %d, %d) is out of range!\n",
					pov_req->dirty_rect.x, pov_req->dirty_rect.y,
					pov_req->dirty_rect.w, pov_req->dirty_rect.h);
				return -EINVAL;
			}
		}
	}

	if (hisifd->index == AUXILIARY_PANEL_IDX) {
		if (pov_req->wb_enable != 1) {
			HISI_FB_ERR("pov_req->wb_enable=%u is invalid!\n", pov_req->wb_enable);
			return -EINVAL;
		}

		if ((pov_req->wb_layer_nums <= 0) ||
			(pov_req->wb_layer_nums > MAX_DSS_DST_NUM)) {
			HISI_FB_ERR("fb%d, invalid wb_layer_nums=%d!",
				hisifd->index, pov_req->wb_layer_nums);
			return -EINVAL;
		}

		if (pov_req->wb_ov_rect.x < 0 || pov_req->wb_ov_rect.y < 0) {
			HISI_FB_ERR("wb_ov_rect(%d, %d) is out of range!\n",
				pov_req->wb_ov_rect.x, pov_req->wb_ov_rect.y);
			return -EINVAL;
		}

		if (pov_req->wb_compose_type >= DSS_WB_COMPOSE_TYPE_MAX) {
			HISI_FB_ERR("wb_compose_type=%u is invalid!\n", pov_req->wb_compose_type);
			return -EINVAL;
		}
		/*lint -e574 */
		for (i = 0; i < pov_req->wb_layer_nums; i++) {
			wb_layer = &(pov_req->wb_layer_infos[i]);
			/*lint +e574 */
			if (wb_layer->chn_idx != DSS_WCHN_W2) {
				if (wb_layer->chn_idx < DSS_WCHN_W0 || wb_layer->chn_idx > DSS_WCHN_W1) {
					HISI_FB_ERR("fb%d, wchn_idx=%d is invalid!", hisifd->index, wb_layer->chn_idx);
					return -EINVAL;
				}
			}

			if (hisi_dss_check_wblayer_buff(hisifd, wb_layer)) {
				HISI_FB_ERR("fb%d, failed to check_wblayer_buff!", hisifd->index);
				return -EINVAL;
			}

			if (hisi_dss_check_userdata_dst(wb_layer, hisifd->index) < 0) {
				return -EINVAL;
			}

			if (wb_layer->dst.format >= HISI_FB_PIXEL_FORMAT_MAX) {
				HISI_FB_ERR("fb%d, format=%d is invalid!", hisifd->index, wb_layer->dst.format);
				return -EINVAL;
			}

			if ((wb_layer->dst.bpp == 0) || (wb_layer->dst.width == 0) || (wb_layer->dst.height == 0)
				|| (wb_layer->dst.stride == 0)) {
				HISI_FB_ERR("fb%d, bpp=%d, width=%d, height=%d, stride=%d is invalid!",
					hisifd->index, wb_layer->dst.bpp, wb_layer->dst.width, wb_layer->dst.height,
					wb_layer->dst.stride);
				return -EINVAL;
			}

			if (wb_layer->need_cap & CAP_AFBCE) {
				if ((wb_layer->dst.afbc_header_stride == 0) || (wb_layer->dst.afbc_payload_stride == 0)) {
					HISI_FB_ERR("fb%d, afbc_header_stride=%d, afbc_payload_stride=%d is invalid!",
						hisifd->index, wb_layer->dst.afbc_header_stride, wb_layer->dst.afbc_payload_stride);
					return -EINVAL;
				}
			}

			if (wb_layer->dst.csc_mode >= DSS_CSC_MOD_MAX) {
				HISI_FB_ERR("fb%d, csc_mode=%d is invalid!", hisifd->index, wb_layer->dst.csc_mode);
				return -EINVAL;
			}

			if (wb_layer->dst.afbc_scramble_mode >= DSS_AFBC_SCRAMBLE_MODE_MAX) {
				HISI_FB_ERR("fb%d, afbc_scramble_mode=%d is invalid!", hisifd->index, wb_layer->dst.afbc_scramble_mode);
				return -EINVAL;
			}

			if (wb_layer->src_rect.x < 0 || wb_layer->src_rect.y < 0 ||
				wb_layer->src_rect.w <= 0 || wb_layer->src_rect.h <= 0) {
				HISI_FB_ERR("src_rect(%d, %d, %d, %d) is out of range!\n",
					wb_layer->src_rect.x, wb_layer->src_rect.y,
					wb_layer->src_rect.w, wb_layer->src_rect.h);
				return -EINVAL;
			}
			/*lint -e574 -e737*/
			if (wb_layer->dst_rect.x < 0 || wb_layer->dst_rect.y < 0 ||
				wb_layer->dst_rect.w <= 0 || wb_layer->dst_rect.h <= 0 ||
				wb_layer->dst_rect.w > wb_layer->dst.width || wb_layer->dst_rect.h > wb_layer->dst.height) {
				HISI_FB_ERR("dst_rect(%d, %d, %d, %d), dst(%d, %d) is out of range!\n",
					wb_layer->dst_rect.x, wb_layer->dst_rect.y, wb_layer->dst_rect.w,
					wb_layer->dst_rect.h, wb_layer->dst.width, wb_layer->dst.height);
				return -EINVAL;
			}
			/*lint +e574 +e737*/
		}
	} else {
		;
	}

	return 0;
}

static int hisi_dss_check_layer_par_base(struct hisi_fb_data_type *hisifd, dss_layer_t *layer)
{
	if (hisifd == NULL) {
		HISI_FB_ERR("hisifd is NULL, return!");
		return -EINVAL;
	}

	if (layer == NULL) {
		HISI_FB_ERR("layer is NULL, return!");
		return -EINVAL;
	}

	if (layer->layer_idx < 0 || layer->layer_idx >= OVL_LAYER_NUM_MAX) {
		HISI_FB_ERR("fb%d, layer_idx=%d is invalid!", hisifd->index, layer->layer_idx);
		return -EINVAL;
	}

	if (layer->need_cap & (CAP_BASE | CAP_DIM | CAP_PURE_COLOR)) {
		return 0;
	}
	return 1;
}

/*lint -e613*/
static int hisi_dss_check_layer_buff(struct hisi_fb_data_type *hisifd, dss_layer_t *layer)
{
	if (hisi_dss_check_addr_validate(&layer->img)) {
		return 0;
	}

	if (layer->need_cap & (CAP_BASE | CAP_DIM | CAP_PURE_COLOR)) {
		return 0;
	}

	return -EINVAL;
}
/*lint +e613*/

int hisi_dss_check_layer_par(struct hisi_fb_data_type *hisifd, dss_layer_t *layer)
{
	int ret = 0;
	if (hisifd == NULL) {
		HISI_FB_ERR("hisifd is NULL, return!");
		return -EINVAL;
	}

	if (layer == NULL) {
		HISI_FB_ERR("layer is NULL, return!");
		return -EINVAL;
	}
	/*lint -e838*/
	ret = hisi_dss_check_layer_par_base(hisifd, layer);
	if (ret <= 0) {
		return ret;
	}
	/*lint +e838*/
	if (hisifd->index == AUXILIARY_PANEL_IDX) {
		if (layer->chn_idx != DSS_RCHN_V2) {
			if ((layer->chn_idx < 0 ||layer->chn_idx >= DSS_WCHN_W0) ||
			(layer->chn_idx == DSS_RCHN_G0) || (layer->chn_idx == DSS_RCHN_V0)) {
				HISI_FB_ERR("fb%d, rchn_idx=%d is invalid!", hisifd->index, layer->chn_idx);
				return -EINVAL;
			}
		}

		if (layer->chn_idx == DSS_RCHN_D2) {
			HISI_FB_ERR("fb%d, chn_idx[%d] does not used by offline play!", hisifd->index, layer->chn_idx);
			return -EINVAL;
		}
	} else if (hisifd->index == PRIMARY_PANEL_IDX){
		if ((layer->chn_idx > DSS_WCHN_W0) ||
			(layer->chn_idx == DSS_RCHN_G0) ||
			(layer->chn_idx == DSS_RCHN_V0) ||
			(layer->chn_idx < DSS_RCHN_D2)) {
			HISI_FB_ERR("fb%d, rchn_idx=%d is invalid!", hisifd->index, layer->chn_idx);
			return -EINVAL;
		}
	}

	if (layer->blending < 0 || layer->blending >= HISI_FB_BLENDING_MAX) {
		HISI_FB_ERR("fb%d, blending=%d is invalid!", hisifd->index, layer->blending);
		return -EINVAL;
	}

	if (layer->img.format >= HISI_FB_PIXEL_FORMAT_MAX) {
		HISI_FB_ERR("fb%d, format=%d is invalid!", hisifd->index, layer->img.format);
		return -EINVAL;
	}

	if ((layer->img.bpp == 0) || (layer->img.width == 0) || (layer->img.height == 0)
		||(layer->img.stride == 0)) {
		HISI_FB_ERR("fb%d, bpp=%d, width=%d, height=%d, stride=%d is invalid!",
			hisifd->index, layer->img.bpp, layer->img.width, layer->img.height,
			layer->img.stride);
		return -EINVAL;
	}

	if (hisi_dss_check_layer_buff(hisifd, layer)) {
		HISI_FB_ERR("fb%d, failed to check_layer_buff!", hisifd->index);
		return -EINVAL;
	}

	if (layer->need_cap & CAP_AFBCD) {
		if ((layer->img.afbc_header_stride == 0) || (layer->img.afbc_payload_stride == 0)
			|| (layer->img.mmbuf_size == 0)) {
			HISI_FB_ERR("fb%d, afbc_header_stride=%d, afbc_payload_stride=%d, mmbuf_size=%d is invalid!",
				hisifd->index, layer->img.afbc_header_stride,
				layer->img.afbc_payload_stride, layer->img.mmbuf_size);
			return -EINVAL;
		}
	}

	if (layer->transform & HISI_FB_TRANSFORM_ROT_90) {
		if(((layer->chn_idx == DSS_RCHN_V0) || (layer->chn_idx == DSS_RCHN_V1)) &&
			(layer->need_cap & CAP_HFBCD)) {
			HISI_FB_DEBUG("fb%d, ch%d need rot and hfbcd!\n!", hisifd->index, layer->chn_idx);
		} else {
			HISI_FB_ERR("fb%d, ch%d is not support need_cap=%d,transform=%d!\n",
				hisifd->index, layer->chn_idx, layer->need_cap, layer->transform);
			return -EINVAL;
		}
	}

	if (layer->img.csc_mode >= DSS_CSC_MOD_MAX) {
		HISI_FB_ERR("fb%d, csc_mode=%d is invalid!", hisifd->index, layer->img.csc_mode);
		return -EINVAL;
	}

	if (layer->img.afbc_scramble_mode >= DSS_AFBC_SCRAMBLE_MODE_MAX) {
		HISI_FB_ERR("fb%d, afbc_scramble_mode=%d is invalid!", hisifd->index, layer->img.afbc_scramble_mode);
		return -EINVAL;
	}

	if ((layer->layer_idx != 0) && (layer->need_cap & CAP_BASE)) {
		HISI_FB_ERR("fb%d, layer%d is not base!", hisifd->index, layer->layer_idx);
		return -EINVAL;
	}

	if (layer->src_rect.x < 0 || layer->src_rect.y < 0 ||
		layer->src_rect.w <= 0 || layer->src_rect.h <= 0) {
		HISI_FB_ERR("src_rect(%d, %d, %d, %d) is out of range!\n",
			layer->src_rect.x, layer->src_rect.y,
			layer->src_rect.w, layer->src_rect.h);
		return -EINVAL;
	}

	if (layer->src_rect_mask.x < 0 || layer->src_rect_mask.y < 0 ||
		layer->src_rect_mask.w < 0 || layer->src_rect_mask.h < 0) {
		HISI_FB_ERR("src_rect_mask(%d, %d, %d, %d) is out of range!\n",
			layer->src_rect_mask.x, layer->src_rect_mask.y,
			layer->src_rect_mask.w, layer->src_rect_mask.h);
		return -EINVAL;
	}

	if (layer->dst_rect.x < 0 || layer->dst_rect.y < 0 ||
		layer->dst_rect.w <= 0 || layer->dst_rect.h <= 0) {
		HISI_FB_ERR("dst_rect(%d, %d, %d, %d) is out of range!\n",
			layer->dst_rect.x, layer->dst_rect.y,
			layer->dst_rect.w, layer->dst_rect.h);
		return -EINVAL;
	}

	return 0;
}

/*******************************************************************************
** DSS disreset
*/
/*lint -e747 -e778 -e774 -e732 -e838*/
void hisifb_dss_disreset(struct hisi_fb_data_type *hisifd)
{
	char __iomem *peri_crg_base = hisifd->peri_crg_base;
	char __iomem *pmctrl_base = hisifd->pmctrl_base;
	char __iomem *sctrl_base = hisifd->sctrl_base;
	char __iomem *media_crg_base = hisifd->media_crg_base;
	uint32_t ret;

	///////////////////////////////////////////////////////////////////////////
	//
	//MEDIA1_SUBSYS_PU
	//
	//1: module mtcmos on
	outp32(peri_crg_base + PERPWREN, 0x00000020);
	udelay(100);

	//2: module unrst
	outp32(peri_crg_base + PERRSTDIS5, 0x00040000);

	//3: module clk enable
	outp32(peri_crg_base + PEREN6, 0x1c002028);
	outp32(sctrl_base + SCPEREN4, 0x00000040);
	outp32(peri_crg_base + PEREN4, 0x00000040);
	udelay(1);

	//4: module clk disable
	outp32(peri_crg_base + PERDIS6, 0x1c002028);
	outp32(sctrl_base + SCPERDIS4, 0x00000040);
	outp32(peri_crg_base + PERDIS4, 0x00000040);
	udelay(1);

	//5: module iso disable
	outp32(peri_crg_base + ISODIS, 0x00000040);

	//6: module unrst
	outp32(peri_crg_base + PERRSTDIS5, 0x00020000);

	//7: module clk enable
	outp32(peri_crg_base + PEREN6, 0x1c002028);
	outp32(sctrl_base + SCPEREN4, 0x00000040);
	outp32(peri_crg_base + PEREN4, 0x00000040);
	//////////////////////////////////////////////////////////////////////////
	//
	//VIVOBUS_PU
	//
	//1: module clk enable
	outp32(media_crg_base + MEDIA_CLKDIV9, 0x00080008);
	outp32(media_crg_base + MEDIA_PEREN0, 0x08040040);
	udelay(1);

	//2: module clk disable
	outp32(media_crg_base + MEDIA_PERDIS0, 0x00040040);
	udelay(1);

	//3: module clk enable
	outp32(media_crg_base + MEDIA_PEREN0, 0x00040040);

	//4: bus idle clear
	outp32(pmctrl_base + NOC_POWER_IDLEREQ, 0x80000000);

	ret = inp32(pmctrl_base + NOC_POWER_IDLEACK);
	udelay(1);
	HISI_FB_DEBUG("pmctrl_base + NOC_POWER_IDLEACK = 0x%x\n", ret);

	ret = inp32(pmctrl_base + NOC_POWER_IDLE);
	udelay(1);
	HISI_FB_DEBUG("pmctrl_base + NOC_POWER_IDLE = 0x%x\n", ret);
	//////////////////////////////////////////////////////////////////////////
	//
	//DSS_PU
	//
	//1.1: module unrst
	outp32(media_crg_base + MEDIA_PERRSTDIS0, 0x02000000);

	//2: module clk enable
	outp32(media_crg_base + MEDIA_CLKDIV9, 0xC280C280);
	outp32(media_crg_base + MEDIA_PEREN0, 0x0009C000);
	outp32(media_crg_base + MEDIA_PEREN1, 0x00660000);
	outp32(media_crg_base + MEDIA_PEREN2, 0x0000003F);
	udelay(1);

	//3: module clk disable
	outp32(media_crg_base + MEDIA_PERDIS0, 0x0009C000);
	outp32(media_crg_base + MEDIA_PERDIS1, 0x00600000);
	outp32(media_crg_base + MEDIA_PERDIS2, 0x0000003F);
	udelay(1);

	//4: module unrst
	outp32(media_crg_base + MEDIA_PERRSTDIS0, 0x000000C0);
	outp32(media_crg_base + MEDIA_PERRSTDIS1, 0x000000F0);

	//5: module clk enable
	outp32(media_crg_base + MEDIA_PEREN0, 0x0009C000);
	outp32(media_crg_base + MEDIA_PEREN1, 0x00600000);
	outp32(media_crg_base + MEDIA_PEREN2, 0x0000003F);

	//6: bus idle clear
	/*outp32(pmctrl_base + NOC_POWER_IDLEREQ, 0x20000000);

	ret = inp32(pmctrl_base + NOC_POWER_IDLEACK);
	udelay(1);
	HISI_FB_DEBUG("pmctrl_base + NOC_POWER_IDLEACK = 0x%x\n", ret);

	ret = inp32(pmctrl_base + NOC_POWER_IDLE);
	udelay(1);
	HISI_FB_DEBUG("pmctrl_base + NOC_POWER_IDLE = 0x%x\n", ret);*/
}
/*lint -e438 -e550 -e573 -e647 -701 -e712 -e713 -e737 -e834 -e838 -845*/

/*static int hisi_dss_mmbuf_config(struct hisi_fb_data_type *hisifd, int ovl_idx,
	dss_layer_t *layer, uint32_t hfbcd_block_type, bool is_pixel_10bit)
{
	int chn_idx;
	dss_rect_t new_src_rect;
	bool mm_alloc_needed = false;

	dss_rect_ltrb_t hfbcd_rect;
	uint32_t mmbuf_line_num = 0;

	chn_idx = layer->chn_idx;
	new_src_rect = layer->src_rect;

	if (ovl_idx <= DSS_OVL1) {
		mm_alloc_needed = true;
	} else {
		if (hisifd->mmbuf_info->mm_used[chn_idx] == 1) {
			mm_alloc_needed = false;
		} else {
			mm_alloc_needed = true;
		}
	}

	if (mm_alloc_needed) {
		hfbcd_rect.left = ALIGN_DOWN(new_src_rect.x, MMBUF_ADDR_ALIGN);
		hfbcd_rect.right = ALIGN_UP(new_src_rect.x - hfbcd_rect.left + new_src_rect.w, MMBUF_ADDR_ALIGN);

		if (hfbcd_block_type == 0) {
			if (layer->need_cap & CAP_ROT) {
				mmbuf_line_num = MMBUF_BLOCK0_ROT_LINE_NUM;
				hfbcd_rect.right = ALIGN_UP(new_src_rect.x - hfbcd_rect.left + new_src_rect.h, MMBUF_ADDR_ALIGN);
			} else {
				mmbuf_line_num = MMBUF_BLOCK0_LINE_NUM;
			}
		} else if (hfbcd_block_type == 1) {
			mmbuf_line_num = MMBUF_BLOCK1_LINE_NUM;
		} else {
			HISI_FB_ERR("hfbcd_block_type=%d no support!\n", layer->img.hfbcd_block_type);
			return -EINVAL;
		}

		hisifd->mmbuf_info->mm_size0_y8[chn_idx] = hfbcd_rect.right * layer->img.bpp * mmbuf_line_num;
		hisifd->mmbuf_info->mm_base0_y8[chn_idx] = hisi_dss_mmbuf_alloc(g_mmbuf_gen_pool,
			hisifd->mmbuf_info->mm_size0_y8[chn_idx]);
		hisifd->mmbuf_info->mm_size1_c8[chn_idx] = hisifd->mmbuf_info->mm_size0_y8[chn_idx] / 2;
		hisifd->mmbuf_info->mm_base1_c8[chn_idx] = hisi_dss_mmbuf_alloc(g_mmbuf_gen_pool,
			hisifd->mmbuf_info->mm_size1_c8[chn_idx]);

		if ((hisifd->mmbuf_info->mm_base0_y8[chn_idx] < MMBUF_BASE) ||
			(hisifd->mmbuf_info->mm_base1_c8[chn_idx] < MMBUF_BASE)) {
			HISI_FB_ERR("fb%d, chn%d failed to alloc mmbuf, mm_base0_y8=0x%x, mm_base1_c8=0x%x.\n",
				hisifd->index, chn_idx, hisifd->mmbuf_info->mm_base0_y8[chn_idx], hisifd->mmbuf_info->mm_base1_c8[chn_idx]);
				return -EINVAL;
		}

		if (is_pixel_10bit) {
			hfbcd_rect.left = ALIGN_DOWN(new_src_rect.x, MMBUF_ADDR_ALIGN);
			if (layer->need_cap & CAP_ROT) {
				hfbcd_rect.right = ALIGN_UP((new_src_rect.x - hfbcd_rect.left + new_src_rect.h) / 4, MMBUF_ADDR_ALIGN);
			} else {
				hfbcd_rect.right = ALIGN_UP((new_src_rect.x - hfbcd_rect.left + new_src_rect.w) / 4, MMBUF_ADDR_ALIGN);
			}

			hisifd->mmbuf_info->mm_size2_y2[chn_idx] = hfbcd_rect.right * mmbuf_line_num;
			hisifd->mmbuf_info->mm_base2_y2[chn_idx] = hisi_dss_mmbuf_alloc(g_mmbuf_gen_pool,
				hisifd->mmbuf_info->mm_size2_y2[chn_idx]);
			hisifd->mmbuf_info->mm_size3_c2[chn_idx] = hisifd->mmbuf_info->mm_size2_y2[chn_idx] / 2;
			hisifd->mmbuf_info->mm_base3_c2[chn_idx] = hisi_dss_mmbuf_alloc(g_mmbuf_gen_pool,
				hisifd->mmbuf_info->mm_size3_c2[chn_idx]);

			if ((hisifd->mmbuf_info->mm_base2_y2[chn_idx] < MMBUF_BASE) ||
				(hisifd->mmbuf_info->mm_base3_c2[chn_idx] < MMBUF_BASE)) {
				HISI_FB_ERR("fb%d, chn%d failed to alloc mmbuf, mm_base2_y2=0x%x, mm_base3_c2=0x%x.\n",
					hisifd->index, chn_idx, hisifd->mmbuf_info->mm_base2_y2[chn_idx], hisifd->mmbuf_info->mm_base3_c2[chn_idx]);
				return -EINVAL;
			}
		}
	}

	hisifd->mmbuf_info->mm_used[chn_idx] = 1;
	return 0;
}*/
void hisi_dss_mctl_sys_init(const char __iomem *mctl_sys_base, dss_mctl_sys_t *s_mctl_sys)
{
	int i;

	if (NULL == mctl_sys_base || NULL == s_mctl_sys) {
		HISI_FB_ERR("NULL ptr\n");
		return;
	}

	memset(s_mctl_sys, 0, sizeof(dss_mctl_sys_t));

	for (i= 0; i < DSS_OVL_IDX_MAX; i++) {
		s_mctl_sys->chn_ov_sel[i] = inp32(mctl_sys_base + MCTL_RCH_OV0_SEL + i * 0x4);
	}

	for (i= 0; i < DSS_WCH_MAX; i++) {
		s_mctl_sys->wchn_ov_sel[i] = inp32(mctl_sys_base + MCTL_WCH_OV2_SEL + i * 0x4);
	}
}

static uint32_t *hisi_dss_get_acm_lut_hue_table(struct hisi_fb_data_type *hisifd, uint8_t last_gamma_type)
{
	struct hisi_panel_info *pinfo = &(hisifd->panel_info);

	if (last_gamma_type == 1) {
		if (pinfo->acm_lut_hue_table_len > 0 && pinfo->cinema_acm_lut_hue_table) {
			return pinfo->cinema_acm_lut_hue_table;
		}
	} else {
		if (pinfo->acm_lut_hue_table_len > 0 && pinfo->acm_lut_hue_table) {
			return pinfo->acm_lut_hue_table;
		}
	}
	return NULL;
}

static uint32_t *hisi_dss_get_acm_lut_sata_table(struct hisi_fb_data_type *hisifd, uint8_t last_gamma_type)
{
	struct hisi_panel_info *pinfo = &(hisifd->panel_info);

	if (last_gamma_type == 1) {
		if ( pinfo->acm_lut_sata_table_len > 0 && pinfo->cinema_acm_lut_sata_table) {
			return pinfo->cinema_acm_lut_sata_table;
		}
	} else {
		if (pinfo->acm_lut_sata_table_len > 0 && pinfo->acm_lut_sata_table) {
			return pinfo->acm_lut_sata_table;
		}
	}
	return NULL;
}

static uint32_t *hisi_dss_get_acm_lut_satr0_table(struct hisi_fb_data_type *hisifd, uint8_t last_gamma_type)
{
	struct hisi_panel_info *pinfo = &(hisifd->panel_info);

	if (last_gamma_type == 1) {
		if (pinfo->acm_lut_satr0_table_len > 0 && pinfo->cinema_acm_lut_satr0_table) {
			return pinfo->cinema_acm_lut_satr0_table;
		}
	} else {
		if (pinfo->acm_lut_satr0_table_len > 0 && pinfo->acm_lut_satr0_table) {
			return pinfo->acm_lut_satr0_table;
		}
	}
	return NULL;
}

static uint32_t *hisi_dss_get_acm_lut_satr1_table(struct hisi_fb_data_type *hisifd, uint8_t last_gamma_type)
{
	struct hisi_panel_info *pinfo = &(hisifd->panel_info);

	if (last_gamma_type == 1) {
		if (pinfo->acm_lut_satr1_table_len > 0 && pinfo->cinema_acm_lut_satr1_table) {
			return pinfo->cinema_acm_lut_satr1_table;
		}
	} else {
		if (pinfo->acm_lut_satr1_table_len > 0 && pinfo->acm_lut_satr1_table) {
			return pinfo->acm_lut_satr1_table;
		}
	}
	return NULL;
}

static uint32_t *hisi_dss_get_acm_lut_satr2_table(struct hisi_fb_data_type *hisifd, uint8_t last_gamma_type)
{
	struct hisi_panel_info *pinfo = &(hisifd->panel_info);

	if (last_gamma_type == 1) {
		if (pinfo->acm_lut_satr2_table_len > 0 && pinfo->cinema_acm_lut_satr2_table) {
			return pinfo->cinema_acm_lut_satr2_table;
		}
	} else {
		if (pinfo->acm_lut_satr2_table_len > 0 && pinfo->acm_lut_satr2_table) {
			return pinfo->acm_lut_satr2_table;
		}
	}
	return NULL;
}

static uint32_t *hisi_dss_get_acm_lut_satr3_table(struct hisi_fb_data_type *hisifd, uint8_t last_gamma_type)
{
	struct hisi_panel_info *pinfo = &(hisifd->panel_info);

	if (last_gamma_type == 1) {
		if (pinfo->acm_lut_satr3_table_len > 0 && pinfo->cinema_acm_lut_satr3_table) {
			return pinfo->cinema_acm_lut_satr3_table;
		}
	} else {
		if (pinfo->acm_lut_satr3_table_len > 0 && pinfo->acm_lut_satr3_table) {
			return pinfo->acm_lut_satr3_table;
		}
	}
	return NULL;
}

static uint32_t *hisi_dss_get_acm_lut_satr4_table(struct hisi_fb_data_type *hisifd, uint8_t last_gamma_type)
{
	struct hisi_panel_info *pinfo = &(hisifd->panel_info);

	if (last_gamma_type == 1) {
		if (pinfo->acm_lut_satr4_table_len > 0 && pinfo->cinema_acm_lut_satr4_table) {
			return pinfo->cinema_acm_lut_satr4_table;
		}
	} else {
		if (pinfo->acm_lut_satr4_table_len > 0 && pinfo->acm_lut_satr4_table) {
			return pinfo->acm_lut_satr4_table;
		}
	}
	return NULL;
}

static uint32_t *hisi_dss_get_acm_lut_satr5_table(struct hisi_fb_data_type *hisifd, uint8_t last_gamma_type)
{
	struct hisi_panel_info *pinfo = &(hisifd->panel_info);

	if (last_gamma_type == 1) {
		if (pinfo->acm_lut_satr5_table_len > 0 && pinfo->cinema_acm_lut_satr5_table) {
			return pinfo->cinema_acm_lut_satr5_table;
		}
	} else {
		if (pinfo->acm_lut_satr5_table_len > 0 && pinfo->acm_lut_satr5_table) {
			return pinfo->acm_lut_satr5_table;
		}
	}
	return NULL;
}
static uint32_t *hisi_dss_get_acm_lut_satr6_table(struct hisi_fb_data_type *hisifd, uint8_t last_gamma_type)
{
	struct hisi_panel_info *pinfo = &(hisifd->panel_info);

	if (last_gamma_type == 1) {
		if (pinfo->acm_lut_satr6_table_len > 0 && pinfo->cinema_acm_lut_satr6_table) {
			return pinfo->cinema_acm_lut_satr6_table;
		}
	} else {
		if (pinfo->acm_lut_satr6_table_len > 0 && pinfo->acm_lut_satr6_table) {
			return pinfo->acm_lut_satr6_table;
		}
	}
	return NULL;
}

static uint32_t *hisi_dss_get_acm_lut_satr7_table(struct hisi_fb_data_type *hisifd, uint8_t last_gamma_type)
{
	struct hisi_panel_info *pinfo = &(hisifd->panel_info);

	if (last_gamma_type == 1) {
		if (pinfo->acm_lut_satr7_table_len > 0 && pinfo->cinema_acm_lut_satr7_table) {
			return pinfo->cinema_acm_lut_satr7_table;
		}
	} else {
		if (pinfo->acm_lut_satr7_table_len > 0 && pinfo->acm_lut_satr7_table) {
			return pinfo->acm_lut_satr7_table;
		}
	}
	return NULL;
}

void hisi_dss_dpp_acm_gm_set_reg(struct hisi_fb_data_type *hisifd) {
	struct hisi_panel_info *pinfo = NULL;
	char __iomem *lcp_base = NULL;
	char __iomem *gmp_base = NULL;
	char __iomem *xcc_base = NULL;
	char __iomem *acm_base = NULL;
	char __iomem *gamma_base = NULL;
	char __iomem *gamma_lut_base = NULL;
	char __iomem *acm_lut_base = NULL;
	static uint8_t last_gamma_type=0;
	static uint32_t gamma_config_flag = 0;
	uint32_t index = 0;
	uint32_t i;
	uint32_t gama_lut_sel;

	uint32_t *local_gamma_lut_table_R = NULL;
	uint32_t *local_gamma_lut_table_G = NULL;
	uint32_t *local_gamma_lut_table_B = NULL;

	uint32_t *local_acm_lut_hue_table = NULL;
	uint32_t *local_acm_lut_sata_table = NULL;
	uint32_t *local_acm_lut_satr0_table = NULL;
	uint32_t *local_acm_lut_satr1_table = NULL;
	uint32_t *local_acm_lut_satr2_table = NULL;
	uint32_t *local_acm_lut_satr3_table = NULL;
	uint32_t *local_acm_lut_satr4_table = NULL;
	uint32_t *local_acm_lut_satr5_table = NULL;
	uint32_t *local_acm_lut_satr6_table = NULL;
	uint32_t *local_acm_lut_satr7_table = NULL;

	if (NULL == hisifd) {
		HISI_FB_ERR("hisifd, NUll pointer warning.\n");
		goto func_exit;
	}

	pinfo = &(hisifd->panel_info);

	if (0 == pinfo->gamma_support || 0 == pinfo->acm_support) {
		goto func_exit;
	}

	if (!HISI_DSS_SUPPORT_DPP_MODULE_BIT(DPP_MODULE_GAMA) || !HISI_DSS_SUPPORT_DPP_MODULE_BIT(DPP_MODULE_ACM)) {//lint !e845 !e774
		HISI_FB_DEBUG("gamma or acm are not suppportted in this platform.\n");
		goto func_exit;
	}

	if (PRIMARY_PANEL_IDX == hisifd->index) {
		lcp_base = hisifd->dss_base + DSS_DPP_LCP_OFFSET;
		gmp_base = hisifd->dss_base + DSS_DPP_GMP_OFFSET;
		xcc_base = hisifd->dss_base + DSS_DPP_XCC_OFFSET;
		acm_base = hisifd->dss_base + DSS_DPP_ACM_OFFSET;
		gamma_base = hisifd->dss_base + DSS_DPP_GAMA_OFFSET;
		gamma_lut_base = hisifd->dss_base + DSS_DPP_GAMA_LUT_OFFSET;
		acm_lut_base = hisifd->dss_base + DSS_DPP_ACM_LUT_OFFSET;
	} else {
		HISI_FB_ERR("fb%d, not support!\n", hisifd->index);
		goto func_exit;
	}

	if (0 == gamma_config_flag) {
		if (last_gamma_type != hisifd->panel_info.gamma_type) {
			//disable acm
			set_reg(acm_base + ACM_EN, 0x0, 1, 0);
			//disable gamma
			set_reg(gamma_base + GAMA_EN, 0x0, 1, 0);

			//disable gmp
			set_reg(gmp_base + GMP_EN, 0x0, 1, 0);
			//disable xcc
			set_reg(xcc_base + LCP_XCC_BYPASS_EN, 0x1, 1, 0);

			gamma_config_flag = 1;
			last_gamma_type = hisifd->panel_info.gamma_type;
		}
		goto func_exit;
	}

	if (1 == gamma_config_flag) {

		local_acm_lut_hue_table = hisi_dss_get_acm_lut_hue_table(hisifd, last_gamma_type);
		local_acm_lut_sata_table = hisi_dss_get_acm_lut_sata_table(hisifd, last_gamma_type);
		local_acm_lut_satr0_table = hisi_dss_get_acm_lut_satr0_table(hisifd, last_gamma_type);
		local_acm_lut_satr1_table = hisi_dss_get_acm_lut_satr1_table(hisifd, last_gamma_type);
		local_acm_lut_satr2_table = hisi_dss_get_acm_lut_satr2_table(hisifd, last_gamma_type);
		local_acm_lut_satr3_table = hisi_dss_get_acm_lut_satr3_table(hisifd, last_gamma_type);
		local_acm_lut_satr4_table = hisi_dss_get_acm_lut_satr4_table(hisifd, last_gamma_type);
		local_acm_lut_satr5_table = hisi_dss_get_acm_lut_satr5_table(hisifd, last_gamma_type);
		local_acm_lut_satr6_table = hisi_dss_get_acm_lut_satr6_table(hisifd, last_gamma_type);
		local_acm_lut_satr7_table = hisi_dss_get_acm_lut_satr7_table(hisifd, last_gamma_type);

		if (1 == last_gamma_type) {
			//set gamma cinema parameter
			if (pinfo->cinema_gamma_lut_table_len > 0 && pinfo->cinema_gamma_lut_table_R
				&& pinfo->cinema_gamma_lut_table_G && pinfo->cinema_gamma_lut_table_B) {
					local_gamma_lut_table_R = pinfo->cinema_gamma_lut_table_R;
					local_gamma_lut_table_G = pinfo->cinema_gamma_lut_table_G;
					local_gamma_lut_table_B = pinfo->cinema_gamma_lut_table_B;
			} else {
				HISI_FB_ERR("can't get gamma cinema paramter from pinfo.\n");
				goto func_exit;
			}
		} else {
			if (pinfo->gamma_lut_table_len > 0 && pinfo->gamma_lut_table_R
				&& pinfo->gamma_lut_table_G && pinfo->gamma_lut_table_B) {
				local_gamma_lut_table_R = pinfo->gamma_lut_table_R;
				local_gamma_lut_table_G = pinfo->gamma_lut_table_G;
				local_gamma_lut_table_B = pinfo->gamma_lut_table_B;
			} else {
				HISI_FB_ERR("can't get gamma normal parameter from pinfo.\n");
				goto func_exit;
			}
		}

		if (!(local_acm_lut_hue_table && local_acm_lut_sata_table
			&& local_acm_lut_satr0_table && local_acm_lut_satr1_table
			&& local_acm_lut_satr2_table && local_acm_lut_satr3_table
			&& local_acm_lut_satr4_table && local_acm_lut_satr5_table
			&& local_acm_lut_satr6_table && local_acm_lut_satr7_table)) {
			HISI_FB_ERR("can't get acm normal parameter from pinfo.\n");
			goto func_exit;
		}

		//config regsiter use default or cinema parameter
		for (index = 0; index < pinfo->gamma_lut_table_len / 2; index++) {
			i = index << 1;
			outp32(gamma_lut_base + (U_GAMA_R_COEF + index * 4), (local_gamma_lut_table_R[i] | (local_gamma_lut_table_R[i+1] << 16)));
			outp32(gamma_lut_base + (U_GAMA_G_COEF + index * 4), (local_gamma_lut_table_G[i] | (local_gamma_lut_table_G[i+1] << 16)));
			outp32(gamma_lut_base + (U_GAMA_B_COEF + index * 4), (local_gamma_lut_table_B[i] | (local_gamma_lut_table_B[i+1] << 16)));
				//GAMA  PRE LUT
        }
		outp32(gamma_lut_base + U_GAMA_R_LAST_COEF, local_gamma_lut_table_R[pinfo->gamma_lut_table_len - 1]);
		outp32(gamma_lut_base + U_GAMA_G_LAST_COEF, local_gamma_lut_table_G[pinfo->gamma_lut_table_len - 1]);
		outp32(gamma_lut_base + U_GAMA_B_LAST_COEF, local_gamma_lut_table_B[pinfo->gamma_lut_table_len - 1]);
			//GAMA  PRE LUT

		acm_set_lut_hue(acm_lut_base + ACM_U_H_COEF, local_acm_lut_hue_table, pinfo->acm_lut_hue_table_len);
		acm_set_lut(acm_lut_base + ACM_U_SATA_COEF, local_acm_lut_sata_table, pinfo->acm_lut_sata_table_len);
		acm_set_lut(acm_lut_base + ACM_U_SATR0_COEF, local_acm_lut_satr0_table, pinfo->acm_lut_satr0_table_len);
		acm_set_lut(acm_lut_base + ACM_U_SATR1_COEF, local_acm_lut_satr1_table, pinfo->acm_lut_satr1_table_len);
		acm_set_lut(acm_lut_base + ACM_U_SATR2_COEF, local_acm_lut_satr2_table, pinfo->acm_lut_satr2_table_len);
		acm_set_lut(acm_lut_base + ACM_U_SATR3_COEF, local_acm_lut_satr3_table, pinfo->acm_lut_satr3_table_len);
		acm_set_lut(acm_lut_base + ACM_U_SATR4_COEF, local_acm_lut_satr4_table, pinfo->acm_lut_satr4_table_len);
		acm_set_lut(acm_lut_base + ACM_U_SATR5_COEF, local_acm_lut_satr5_table, pinfo->acm_lut_satr5_table_len);
		acm_set_lut(acm_lut_base + ACM_U_SATR6_COEF, local_acm_lut_satr6_table, pinfo->acm_lut_satr6_table_len);
		acm_set_lut(acm_lut_base + ACM_U_SATR7_COEF, local_acm_lut_satr7_table, pinfo->acm_lut_satr7_table_len);
	}

	gama_lut_sel = (uint32_t)inp32(gamma_base + GAMA_LUT_SEL);
	set_reg(gamma_base + GAMA_LUT_SEL, (~(gama_lut_sel & 0x1)) & 0x1, 1, 0);

	//enable gamma
	set_reg(gamma_base + GAMA_EN, 0x1, 1, 0);

	//enable gmp
	set_reg(gmp_base + GMP_EN, 0x1, 1, 0);
	//enable xcc
	set_reg(xcc_base + LCP_XCC_BYPASS_EN, 0x0, 1, 0);

	//enable acm
	set_reg(acm_base + ACM_EN, 0x1, 1, 0);
	gamma_config_flag = 0;
func_exit:
	return;//lint !e438
}//lint !e550

void hisi_dss_post_scf_init(char __iomem *dss_base,
	const char __iomem *post_scf_base, dss_scl_t *s_post_scf)
{
	if (NULL == dss_base) {
		HISI_FB_ERR("dss_base is NULL");
		return;
	}
	if (NULL == post_scf_base) {
		HISI_FB_ERR("post_scf_base is NULL");
		return;
	}
	if (NULL == s_post_scf) {
		HISI_FB_ERR("s_post_scf is NULL");
		return;
	}

	memset(s_post_scf, 0, sizeof(dss_scl_t));
}

void hisi_dss_post_scf_set_reg(struct hisi_fb_data_type *hisifd,
	char __iomem *post_scf_base, dss_scl_t *s_post_scf)
{
	if (hisifd == NULL) {
		HISI_FB_DEBUG("hisifd is NULL!\n");
		return;
	}

	if (post_scf_base == NULL) {
		HISI_FB_DEBUG("post_scf_base is NULL!\n");
		return;
	}

	if (s_post_scf == NULL) {
		HISI_FB_DEBUG("s_post_scf is NULL!\n");
		return;
	}

	hisifd->set_reg(hisifd, post_scf_base + SCF_EN_HSCL_STR, s_post_scf->en_hscl_str, 32, 0);
	hisifd->set_reg(hisifd, post_scf_base + SCF_EN_VSCL_STR, s_post_scf->en_vscl_str, 32, 0);
	hisifd->set_reg(hisifd, post_scf_base + SCF_H_V_ORDER, s_post_scf->h_v_order, 32, 0);
	hisifd->set_reg(hisifd, post_scf_base + SCF_INPUT_WIDTH_HEIGHT, s_post_scf->input_width_height, 32, 0);
	hisifd->set_reg(hisifd, post_scf_base + SCF_OUTPUT_WIDTH_HEIGHT, s_post_scf->output_width_height, 32, 0);
	hisifd->set_reg(hisifd, post_scf_base + SCF_EN_HSCL, s_post_scf->en_hscl, 32, 0);
	hisifd->set_reg(hisifd, post_scf_base + SCF_EN_VSCL, s_post_scf->en_vscl, 32, 0);
	hisifd->set_reg(hisifd, post_scf_base + SCF_ACC_HSCL, s_post_scf->acc_hscl, 32, 0);
	hisifd->set_reg(hisifd, post_scf_base + SCF_INC_HSCL, s_post_scf->inc_hscl, 32, 0);
	hisifd->set_reg(hisifd, post_scf_base + SCF_INC_VSCL, s_post_scf->inc_vscl, 32, 0);
	hisifd->set_reg(hisifd, post_scf_base + SCF_EN_MMP, s_post_scf->en_mmp, 32, 0);
}
/*lint +e747 +e778 +e774 +e732 +e838*/
/*lint -e838*/
int hisi_dss_post_scf_config(struct hisi_fb_data_type *hisifd, dss_overlay_t *pov_req)
{
	struct hisi_panel_info *pinfo = NULL;
	dss_rect_t src_rect = {0};
	dss_rect_t dst_rect = {0};
	dss_scl_t *post_scf = NULL;
	bool en_hscl = false;
	bool en_vscl = false;
	bool en_mmp = false;
	uint32_t h_ratio = 0;
	uint32_t v_ratio = 0;
	uint32_t h_v_order = 0;
	uint32_t acc_hscl = 0;
	uint32_t acc_vscl = 0;
	uint32_t scf_en_vscl = 0;

	if (NULL == hisifd) {
		HISI_FB_ERR("hisifd is NULL");
		return -EINVAL;
	}
	pinfo = &(hisifd->panel_info);

	if (!HISI_DSS_SUPPORT_DPP_MODULE_BIT(DPP_MODULE_POST_SCF)) { /*lint !e774*/
		return 0;
	}

	if (pov_req) {
		if ((pov_req->res_updt_rect.h <= 0) || (pov_req->res_updt_rect.w <= 0)) {
			HISI_FB_DEBUG("fb%d, res_updt_rect[%d,%d, %d,%d] is invalid!\n", hisifd->index,
				pov_req->res_updt_rect.x, pov_req->res_updt_rect.y,
				pov_req->res_updt_rect.w, pov_req->res_updt_rect.h);
			return 0;
		}

		if ((pov_req->res_updt_rect.h == hisifd->ov_req_prev.res_updt_rect.h)
			&& (pov_req->res_updt_rect.w == hisifd->ov_req_prev.res_updt_rect.w)) {
			if (g_rog_config_scf_cnt > 0) {
				HISI_FB_DEBUG("flush same config 5 times! g_rog_config_scf_cnt = %d \n", g_rog_config_scf_cnt);
				g_rog_config_scf_cnt --;
			} else {
				return 0;
			}
		} else {
			g_rog_config_scf_cnt = 5;
		}

		HISI_FB_DEBUG("fb%d, post_scf res_updt_rect[%d, %d]->lcd_rect[%d, %d]\n",
			hisifd->index,
			pov_req->res_updt_rect.w,
			pov_req->res_updt_rect.h,
			pinfo->xres, pinfo->yres);

		src_rect = pov_req->res_updt_rect;
	} else {
		src_rect.y = 0;
		src_rect.x = 0;
		src_rect.w = pinfo->xres;
		src_rect.h = pinfo->yres;
	}

	dst_rect.y = 0;
	dst_rect.x = 0;
	dst_rect.w = pinfo->xres;
	dst_rect.h = pinfo->yres;

	post_scf = &(hisifd->dss_module.post_scf);
	hisifd->dss_module.post_scf_used = 1;

	do {
		if (src_rect.w == dst_rect.w)
			break;

		en_hscl = true;

		h_ratio = DSS_WIDTH(src_rect.w) * SCF_INC_FACTOR / DSS_WIDTH(dst_rect.w);
		if ((DSS_WIDTH(dst_rect.w) > (DSS_WIDTH(src_rect.w) * SCF_UPSCALE_MAX))
			|| (DSS_WIDTH(src_rect.w) > (DSS_WIDTH(dst_rect.w) * SCF_DOWNSCALE_MAX))) {
			HISI_FB_ERR("width out of range, src_rect(%d, %d, %d, %d), dst_rect(%d, %d, %d, %d).\n",
				src_rect.x, src_rect.y, src_rect.w, src_rect.h,
				dst_rect.x, dst_rect.y, dst_rect.w, dst_rect.h);

			return -EINVAL;
		}
	} while(0);

	do {
		if (src_rect.h == dst_rect.h)
			break;

		en_vscl = true;
		scf_en_vscl = 1;

		v_ratio = (DSS_HEIGHT(src_rect.h) * SCF_INC_FACTOR + SCF_INC_FACTOR / 2 - acc_vscl) /
			DSS_HEIGHT(dst_rect.h);
		if ((DSS_HEIGHT(dst_rect.h) > (DSS_HEIGHT(src_rect.h) * SCF_UPSCALE_MAX))
			|| (DSS_HEIGHT(src_rect.h) > (DSS_HEIGHT(dst_rect.h) * SCF_DOWNSCALE_MAX))) {
			HISI_FB_ERR("height out of range, src_rect(%d, %d, %d, %d), dst_rect(%d, %d, %d, %d).\n",
				src_rect.x, src_rect.y, src_rect.w, src_rect.h,
				dst_rect.x, dst_rect.y, dst_rect.w, dst_rect.h);
			return -EINVAL;
		}
	} while(0);

	if (en_hscl || en_vscl) {
		/* scale down, do hscl first; scale up, do vscl first*/
		h_v_order = (src_rect.w > dst_rect.w) ? 0 : 1;

		post_scf->en_hscl_str = set_bits32(post_scf->en_hscl_str, 0x0, 1, 0);

		//if (DSS_HEIGHT(src_rect.h) * 2 >= DSS_HEIGHT(dst_rect.h)) {
		if (v_ratio >= 2 * SCF_INC_FACTOR) {
			post_scf->en_vscl_str = set_bits32(post_scf->en_vscl_str, 0x1, 1, 0);
		} else {
			post_scf->en_vscl_str = set_bits32(post_scf->en_vscl_str, 0x0, 1, 0);
		}

		if (src_rect.h > dst_rect.h) {
			scf_en_vscl = 0x3;
		}
		en_mmp = 0x1;
		/*lint -e732 */
		post_scf->h_v_order = set_bits32(post_scf->h_v_order, h_v_order, 1, 0);
		post_scf->input_width_height = set_bits32(post_scf->input_width_height,
			DSS_HEIGHT(src_rect.h), 13, 0);
		post_scf->input_width_height = set_bits32(post_scf->input_width_height,
			DSS_WIDTH(src_rect.w), 13, 16);
		post_scf->output_width_height = set_bits32(post_scf->output_width_height,
			DSS_HEIGHT(dst_rect.h), 13, 0);
		post_scf->output_width_height = set_bits32(post_scf->output_width_height,
			DSS_WIDTH(dst_rect.w), 13, 16);
		/*lint +e732 */
		post_scf->en_hscl = set_bits32(post_scf->en_hscl, (en_hscl ? 0x1 : 0x0), 1, 0);
		post_scf->en_vscl = set_bits32(post_scf->en_vscl, scf_en_vscl, 2, 0);
		post_scf->acc_hscl = set_bits32(post_scf->acc_hscl, acc_hscl, 31, 0);
		post_scf->inc_hscl = set_bits32(post_scf->inc_hscl, h_ratio, 24, 0);
		post_scf->inc_vscl = set_bits32(post_scf->inc_vscl, v_ratio, 24, 0);
		post_scf->en_mmp = set_bits32(post_scf->en_mmp, en_mmp, 1, 0);
	} else {
		post_scf->en_hscl = set_bits32(post_scf->en_hscl, 0x0, 1, 0);
		post_scf->en_vscl = set_bits32(post_scf->en_vscl, 0x0, 2, 0);
	}

	return 0;
}
/*lint +e838*/

/*lint -e666 -e570 -e648 -e713*/
void hisi_dump_current_info(struct hisi_fb_data_type *hisifd)
{
	return;
}
/*lint +e666 +e570 +e648 +e713*/
