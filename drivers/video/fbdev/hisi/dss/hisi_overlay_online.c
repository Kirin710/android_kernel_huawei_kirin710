/* Copyright (c) 2013-2014, Hisilicon Tech. Co., Ltd. All rights reserved.
*
* This program is free software; you can redistribute it and/or modify
* it under the terms of the GNU General Public License version 2 and
* only version 2 as published by the Free Software Foundation.
*
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
* GNU General Public License for more details.
*
*/
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Winteger-overflow"

#include "hisi_overlay_utils.h"
#include "hisi_dpe_utils.h"
#include "hisi_mmbuf_manager.h"
#include "hisi_mipi_dsi.h"
#include "product/rgb_stats/hisi_fb_rgb_stats.h"

#if CONFIG_SH_AOD_ENABLE
extern bool hisi_aod_get_aod_status(void);
#endif

/*lint -e776 -e737 -e574 -e648 -e570 -e565*/
int hisi_overlay_pan_display(struct hisi_fb_data_type *hisifd)
{
	int ret = 0;
	struct fb_info *fbi = NULL;
	dss_overlay_t *pov_req = NULL;
	dss_overlay_t *pov_req_prev = NULL;
	dss_overlay_block_t *pov_h_block_infos = NULL;
	dss_overlay_block_t *pov_h_block = NULL;
	dss_layer_t *layer = NULL;
	dss_rect_ltrb_t clip_rect;
	dss_rect_t aligned_rect;
	bool rdma_stretch_enable = false;
	uint32_t offset = 0;
	uint32_t addr = 0;
	int hal_format = 0;
	uint32_t cmdlist_pre_idxs = 0;
	uint32_t cmdlist_idxs = 0;
	int enable_cmdlist = 0;
	bool has_base = false;

	if (NULL == hisifd) {
		HISI_FB_ERR("hisi fd is invalid\n");
		return -EINVAL;
	}
	fbi = hisifd->fbi;
	if (NULL == fbi) {
		HISI_FB_ERR("hisifd fbi is invalid\n");
		return -EINVAL;
	}

	pov_req = &(hisifd->ov_req);
	pov_req_prev = &(hisifd->ov_req_prev);

	if (!hisifd->panel_power_on) {
		HISI_FB_INFO("fb%d, panel is power off!", hisifd->index);
		return 0;
	}

	if (g_debug_ldi_underflow) {
		if (g_err_status & (DSS_PDP_LDI_UNDERFLOW | DSS_SDP_LDI_UNDERFLOW))
			return 0;
	}

	offset = fbi->var.xoffset * (fbi->var.bits_per_pixel >> 3) +
		fbi->var.yoffset * fbi->fix.line_length;
	addr = fbi->fix.smem_start + offset;
	if (!fbi->fix.smem_start) {
		HISI_FB_ERR("fb%d, smem_start is null!\n", hisifd->index);
		return -EINVAL;
	}
	hisifd->fb_pan_display = true;

	if (fbi->fix.smem_len <= 0) {
		HISI_FB_ERR("fb%d, smem_len(%d) is out of range!\n",
			hisifd->index, fbi->fix.smem_len);
		return -EINVAL;
	}

	hal_format = hisi_get_hal_format(fbi);
	if (hal_format < 0) {
		HISI_FB_ERR("fb%d, not support this fb_info's format!\n", hisifd->index);
		return -EINVAL;
	}

	enable_cmdlist = g_enable_ovl_cmdlist_online;
	if ((hisifd->index == EXTERNAL_PANEL_IDX) && hisifd->panel_info.fake_external)
		enable_cmdlist = 0;

	hisifb_activate_vsync(hisifd);

	ret = hisi_vactive0_start_config(hisifd, pov_req);
	if (ret != 0) {
		HISI_FB_ERR("fb%d, hisi_vactive0_start_config failed! ret = %d\n", hisifd->index, ret);
		goto err_return;
	}

	if (g_debug_ovl_online_composer == 1) {
		HISI_FB_INFO("offset=%u.\n", offset);
		dumpDssOverlay(hisifd, pov_req);
	}

	memset(pov_req, 0, sizeof(dss_overlay_t));
	pov_req->ov_block_infos_ptr = (uint64_t)(uintptr_t)(&(hisifd->ov_block_infos));
	pov_req->ov_block_nums = 1;
	pov_req->ovl_idx = DSS_OVL0;
	pov_req->dirty_rect.x = 0;
	pov_req->dirty_rect.y = 0;
	pov_req->dirty_rect.w = fbi->var.xres + hisifd->panel_info.dummy_pixel_num;
	pov_req->dirty_rect.h = fbi->var.yres;

	pov_req->res_updt_rect.x = 0;
	pov_req->res_updt_rect.y = 0;
	pov_req->res_updt_rect.w = fbi->var.xres + hisifd->panel_info.dummy_pixel_num;
	pov_req->res_updt_rect.h = fbi->var.yres;
	pov_req->release_fence = -1;

	pov_h_block_infos = (dss_overlay_block_t *)(uintptr_t)(pov_req->ov_block_infos_ptr);
	pov_h_block = &(pov_h_block_infos[0]);
	pov_h_block->layer_nums = 1;

	layer = &(pov_h_block->layer_infos[0]);
	layer->img.format = hal_format;
	layer->img.width = fbi->var.xres;
	layer->img.height = fbi->var.yres;
	layer->img.bpp = fbi->var.bits_per_pixel >> 3;
	layer->img.stride = fbi->fix.line_length;
	layer->img.buf_size = layer->img.stride * layer->img.height;
	layer->img.phy_addr = addr;
	layer->img.vir_addr = addr;
	layer->img.mmu_enable = 1;
	layer->img.shared_fd = -1;
	layer->src_rect.x = 0;
	layer->src_rect.y = 0;
	layer->src_rect.w = fbi->var.xres;
	layer->src_rect.h = fbi->var.yres;
	layer->dst_rect.x = 0;
	layer->dst_rect.y = 0;
	layer->dst_rect.w = fbi->var.xres;
	layer->dst_rect.h = fbi->var.yres;
	layer->transform = HISI_FB_TRANSFORM_NOP;
	layer->blending = HISI_FB_BLENDING_NONE;
	layer->glb_alpha = 0xFF;
	layer->color = 0x0;
	layer->layer_idx = 0x0;
	layer->chn_idx = DSS_RCHN_D2;
	layer->need_cap = 0;

	hisi_dss_handle_cur_ovl_req(hisifd, pov_req);

	ret = hisi_dss_module_init(hisifd);
	if (ret != 0) {
		HISI_FB_ERR("fb%d, hisi_dss_module_init failed! ret = %d\n", hisifd->index, ret);
		goto err_return;
	}

	hisi_mmbuf_info_get_online(hisifd);

	if (enable_cmdlist) {
		hisifd->set_reg = hisi_cmdlist_set_reg;

		hisi_cmdlist_data_get_online(hisifd);

		ret = hisi_cmdlist_get_cmdlist_idxs(pov_req_prev, &cmdlist_pre_idxs, NULL);
		if (ret != 0) {
			HISI_FB_ERR("fb%d, hisi_cmdlist_get_cmdlist_idxs pov_req_prev failed! ret = %d\n", hisifd->index, ret);
			goto err_return;
		}

		ret = hisi_cmdlist_get_cmdlist_idxs(pov_req, &cmdlist_pre_idxs, &cmdlist_idxs);
		if (ret != 0) {
			HISI_FB_ERR("fb%d, hisi_cmdlist_get_cmdlist_idxs pov_req failed! ret = %d\n", hisifd->index, ret);
			goto err_return;
		}

		hisi_cmdlist_add_nop_node(hisifd, cmdlist_pre_idxs, 0, 0);
		hisi_cmdlist_add_nop_node(hisifd, cmdlist_idxs, 0, 0);
	} else {
		hisifd->set_reg = hisifb_set_reg;

		hisi_dss_mctl_mutex_lock(hisifd, pov_req->ovl_idx);
		cmdlist_pre_idxs = ~0;
	}

	hisi_dss_prev_module_set_regs(hisifd, pov_req_prev, cmdlist_pre_idxs, enable_cmdlist, NULL);

	hisi_dss_aif_handler(hisifd, pov_req, pov_h_block);

	ret = hisi_dss_ovl_base_config(hisifd, pov_req, NULL, NULL, pov_req->ovl_idx, 0);
	if (ret != 0) {
		HISI_FB_ERR("fb%d, hisi_dss_ovl_init failed! ret = %d\n", hisifd->index, ret);
		goto err_return;
	}

	ret = hisi_ov_compose_handler(hisifd, pov_req, pov_h_block, layer, NULL, NULL,
		&clip_rect, &aligned_rect, &rdma_stretch_enable, &has_base, true, enable_cmdlist);
	if (ret != 0) {
		HISI_FB_ERR("hisi_ov_compose_handler failed! ret = %d\n", ret);
		goto err_return;
	}

	ret = hisi_dss_mctl_ov_config(hisifd, pov_req, pov_req->ovl_idx, has_base, true);
	if (ret != 0) {
		HISI_FB_ERR("fb%d, hisi_dss_mctl_config failed! ret = %d\n", hisifd->index, ret);
		goto err_return;
	}

	if (hisifd->panel_info.dirty_region_updt_support) {
		ret= hisi_dss_dirty_region_dbuf_config(hisifd, pov_req);
		if (ret != 0) {
			HISI_FB_ERR("fb%d, hisi_dss_dirty_region_dbuf_config failed! ret = %d\n", hisifd->index, ret);
			goto err_return;
		}
	}

	ret = hisi_dss_post_scf_config(hisifd, pov_req);
	if (ret != 0) {
		HISI_FB_ERR("fb%d, hisi_dss_post_scf_config failed! ret = %d\n", hisifd->index, ret);
		goto err_return;
	}

#if defined(CONFIG_HISI_FB_970) || defined(CONFIG_HISI_FB_V501) || \
	defined(CONFIG_HISI_FB_V510) || defined(CONFIG_HISI_FB_V350) || \
	defined(CONFIG_HISI_FB_V600)
	ret = hisi_effect_arsr1p_config(hisifd, pov_req);
	if (ret != 0) {
		HISI_FB_ERR("fb%d, hisi_effect_arsr1p_config failed! ret = %d\n", hisifd->index, ret);
		goto err_return;
	}
#endif

	ret = hisi_dss_ov_module_set_regs(hisifd, pov_req, pov_req->ovl_idx, enable_cmdlist, 0, 0, true);
	if (ret != 0) {
		HISI_FB_ERR("fb%d, hisi_dss_module_config failed! ret = %d\n", hisifd->index, ret);
		goto err_return;
	}

	if (!g_fake_lcd_flag) {
		hisi_dss_unflow_handler(hisifd, pov_req, true);
	}

	if (enable_cmdlist) {
		//add taskend for share channel
		hisi_cmdlist_add_nop_node(hisifd, cmdlist_idxs, 0, 0);

		//remove ch cmdlist
		hisi_cmdlist_config_stop(hisifd, cmdlist_pre_idxs);

		cmdlist_idxs |= cmdlist_pre_idxs;
		hisi_cmdlist_flush_cache(hisifd, cmdlist_idxs);

		if (g_debug_ovl_cmdlist) {
			hisi_cmdlist_dump_all_node(hisifd, NULL, cmdlist_idxs);
		}

		if (hisi_cmdlist_config_start(hisifd, pov_req->ovl_idx, cmdlist_idxs, 0)) {
			ret = -EINVAL;
			goto err_return;
		}
	} else {
		hisi_dss_mctl_mutex_unlock(hisifd, pov_req->ovl_idx);
	}

	if (hisifd->panel_info.dirty_region_updt_support) {
		hisi_dss_dirty_region_updt_config(hisifd, pov_req);
	}

	single_frame_update(hisifd);
	hisifb_frame_updated(hisifd);

	hisifb_deactivate_vsync(hisifd);

	hisifd->frame_count++;
	memcpy(&hisifd->ov_req_prev_prev, &hisifd->ov_req_prev, sizeof(dss_overlay_t));
	memcpy(&(hisifd->ov_block_infos_prev_prev), &(hisifd->ov_block_infos_prev),
		hisifd->ov_req_prev.ov_block_nums * sizeof(dss_overlay_block_t));
	hisifd->ov_req_prev_prev.ov_block_infos_ptr = (uint64_t)(uintptr_t)(&(hisifd->ov_block_infos_prev_prev));

	memcpy(&hisifd->ov_req_prev, pov_req, sizeof(dss_overlay_t));
	memcpy(&(hisifd->ov_block_infos_prev), &(hisifd->ov_block_infos),
		pov_req->ov_block_nums * sizeof(dss_overlay_block_t));
	hisifd->ov_req_prev.ov_block_infos_ptr = (uint64_t)(uintptr_t)(&(hisifd->ov_block_infos_prev));

	return 0;

err_return:
	if (is_mipi_cmd_panel(hisifd)) {
		hisifd->vactive0_start_flag = 1;
	} else {
		single_frame_update(hisifd);
	}
	hisifb_deactivate_vsync(hisifd);

	return ret;
}

static int hisi_dss_check_vsync_before_iteration_layer(struct hisi_fb_data_type *hisifd,
		dss_overlay_t *pov_req, bool *vsync_time_checked)
{
	int ret = 0;

#ifdef CONFIG_HISI_FB_V320
	if (g_mipi_dphy_version == DPHY_VER_14){
		if (is_mipi_video_panel(hisifd) || is_dp_panel(hisifd)) {
			*vsync_time_checked = true;
			ret = hisi_dss_check_vsync_timediff(hisifd, pov_req);
		}
	}
#endif
	return ret;
}

static int hisi_dss_check_vsync_during_iteration_layer(struct hisi_fb_data_type *hisifd,
		dss_overlay_t *pov_req, bool *vsync_time_checked)
{
	int ret = 0;
#ifdef CONFIG_HISI_FB_V320
	if (g_mipi_dphy_version != DPHY_VER_14){
		if (is_mipi_video_panel(hisifd) || is_dp_panel(hisifd)) {
			*vsync_time_checked = true;
			ret = hisi_dss_check_vsync_timediff(hisifd, pov_req);
		}
	}
#else
	if (is_mipi_video_panel(hisifd) || is_dp_panel(hisifd)) {
		*vsync_time_checked = true;
		ret = hisi_dss_check_vsync_timediff(hisifd, pov_req);
	}
#endif
	return ret;
}

int hisi_ov_online_play(struct hisi_fb_data_type *hisifd, void __user *argp)
{
	static int dss_free_buffer_refcount = 0;
	dss_overlay_t *pov_req = NULL;
	dss_overlay_t *pov_req_prev = NULL;
	dss_overlay_block_t *pov_h_block_infos = NULL;
	dss_overlay_block_t *pov_h_block = NULL;
	dss_layer_t *layer = NULL;
	dss_rect_ltrb_t clip_rect;
	dss_rect_t aligned_rect;
	bool rdma_stretch_enable = false;
	uint32_t cmdlist_pre_idxs = 0;
	uint32_t cmdlist_idxs = 0;
	uint64_t ov_block_infos_ptr;
	int enable_cmdlist = 0;
	bool has_base = false;
	unsigned long flags = 0;
	int need_skip = 0;
	int i = 0;
	int m = 0;
	int ret = 0;
	uint32_t timediff = 0;
	bool vsync_time_checked = false;
	bool masklayer_maxbacklight_flag = false;
	struct list_head lock_list;
	struct timeval tv0;
	struct timeval tv1;
	struct timeval tv2;
	struct timeval tv3;
	struct timeval returntv0;
	struct timeval returntv1;
	struct hisi_fb_data_type *fb1 = NULL;
	int need_wait_1vsync = 0;

	if (NULL == hisifd) {
		HISI_FB_ERR("NULL Pointer!\n");
		return -EINVAL;
	}

	if (NULL == argp) {
		HISI_FB_ERR("NULL Pointer!\n");
		return -EINVAL;
	}

#if CONFIG_SH_AOD_ENABLE
	if ((hisifd->index == EXTERNAL_PANEL_IDX) && hisi_aod_get_aod_status()) {
		HISI_FB_INFO("In aod mode!\n");
		return 0;
	}
#endif

	fb1 = hisifd_list[EXTERNAL_PANEL_IDX];
	pov_req = &(hisifd->ov_req);
	pov_req_prev = &(hisifd->ov_req_prev);
	INIT_LIST_HEAD(&lock_list);

	if (!hisifd->panel_power_on) {
		HISI_FB_INFO("fb%d, panel is power off!\n", hisifd->index);
		hisifd->backlight.bl_updated = 0;
		return 0;
	}

	if (g_debug_ldi_underflow) {
		if (g_err_status & (DSS_PDP_LDI_UNDERFLOW | DSS_SDP_LDI_UNDERFLOW)) {
			mdelay(HISI_DSS_COMPOSER_HOLD_TIME);
			return 0;
		}
	}

	if (g_debug_ovl_online_composer_return) {
		return 0;
	}

	if (hisi_online_play_bypass_check(hisifd)) {
		// resync fence with hwcomposer
		hisifb_buf_sync_suspend(hisifd);

		HISI_FB_INFO("fb%d online play is bypassed!\n", hisifd->index);
		return 0;
	}

	if (g_debug_ovl_online_composer_timediff & 0x2) {
		hisifb_get_timestamp(&tv0);
	}
	hisifb_get_timestamp(&returntv0);

	enable_cmdlist = g_enable_ovl_cmdlist_online;
	if ((hisifd->index == EXTERNAL_PANEL_IDX) && hisifd->panel_info.fake_external) {
		enable_cmdlist = 0;
	}

	hisifb_activate_vsync(hisifd);

	hisifb_snd_cmd_before_frame(hisifd);

	if (g_debug_ovl_online_composer_timediff & 0x4) {
		hisifb_get_timestamp(&tv2);
	}

	ret = hisi_dss_get_ov_data_from_user(hisifd, pov_req, argp);
	if (ret != 0) {
		HISI_FB_ERR("fb%d, hisi_dss_get_ov_data_from_user failed! ret=%d\n", hisifd->index, ret);
		need_skip = 1;
		goto err_return;
	}

	if ((is_mipi_video_panel(hisifd)) && (hisifd->online_play_count == 1)) {
		need_wait_1vsync = 1;
		HISI_FB_INFO("video panel wait a vsync when first frame displayed! \n");
	}

	if (is_mipi_cmd_panel(hisifd) || (dss_free_buffer_refcount < 2) || need_wait_1vsync) {
		ret = hisi_vactive0_start_config(hisifd, pov_req);
		if (ret != 0) {
			HISI_FB_ERR("fb%d, hisi_vactive0_start_config failed! ret=%d\n", hisifd->index, ret);
			need_skip = 1;
			goto err_return;
		}
	}

	if (g_debug_ovl_online_composer_timediff & 0x4) {
		hisifb_get_timestamp(&tv3);
		timediff = hisifb_timestamp_diff(&tv2, &tv3);
		if (timediff >= g_debug_ovl_online_composer_time_threshold)
			HISI_FB_ERR("ONLINE_VACTIVE_TIMEDIFF is %u us!\n", timediff);
	}

	down(&hisifd->blank_sem0);

	if (g_debug_ovl_online_composer == 1) {
		dumpDssOverlay(hisifd, pov_req);
	}

	ret = hisifb_layerbuf_lock(hisifd, pov_req, &lock_list);
	if (ret != 0) {
		HISI_FB_ERR("fb%d, hisifb_layerbuf_lock failed! ret=%d\n", hisifd->index, ret);
		goto err_return;
	}

	hisi_dss_handle_cur_ovl_req(hisifd, pov_req);

	ret = hisi_dss_module_init(hisifd);
	if (ret != 0) {
		HISI_FB_ERR("fb%d, hisi_dss_module_init failed! ret = %d\n", hisifd->index, ret);
		goto err_return;
	}

	hisi_mmbuf_info_get_online(hisifd);
	hisifd->use_mmbuf_cnt = 0;

	if (enable_cmdlist) {
		hisifd->set_reg = hisi_cmdlist_set_reg;

		hisi_cmdlist_data_get_online(hisifd);

		ret = hisi_cmdlist_get_cmdlist_idxs(pov_req_prev, &cmdlist_pre_idxs, NULL);
		if (ret != 0) {
			HISI_FB_ERR("fb%d, hisi_cmdlist_get_cmdlist_idxs pov_req_prev failed! ret = %d\n", hisifd->index, ret);
			goto err_return;
		}

		ret = hisi_cmdlist_get_cmdlist_idxs(pov_req, &cmdlist_pre_idxs, &cmdlist_idxs);
		if (ret != 0) {
			HISI_FB_ERR("fb%d, hisi_cmdlist_get_cmdlist_idxs pov_req failed! ret = %d\n", hisifd->index, ret);
			goto err_return;
		}

		hisi_cmdlist_add_nop_node(hisifd, cmdlist_pre_idxs, 0, 0);
		hisi_cmdlist_add_nop_node(hisifd, cmdlist_idxs, 0, 0);
	} else {
		hisifd->set_reg = hisifb_set_reg;
		hisi_dss_mctl_mutex_lock(hisifd, pov_req->ovl_idx);
		cmdlist_pre_idxs = ~0;
	}
	hisi_dss_prev_module_set_regs(hisifd, pov_req_prev, cmdlist_pre_idxs, enable_cmdlist, NULL);

	pov_h_block_infos = (dss_overlay_block_t *)(uintptr_t)(pov_req->ov_block_infos_ptr);
	for (m = 0; m < pov_req->ov_block_nums; m++) {
		pov_h_block = &(pov_h_block_infos[m]);

		ret = hisi_dss_module_init(hisifd);
		if (ret != 0) {
			HISI_FB_ERR("fb%d, hisi_dss_module_init failed! ret = %d\n", hisifd->index, ret);
			goto err_return;
		}

		hisi_dss_aif_handler(hisifd, pov_req, pov_h_block);

		ret = hisi_dss_ovl_base_config(hisifd, pov_req, pov_h_block, NULL, pov_req->ovl_idx, m);
		if (ret != 0) {
			HISI_FB_ERR("fb%d, hisi_dss_ovl_init failed! ret = %d\n", hisifd->index, ret);
			goto err_return;
		}

#if VIDEO_IDLE_GPU_COMPOSE_ENABLE
		if (m == 0) {
			(void)hisifb_video_panel_idle_display_ctrl(hisifd,
				pov_req->video_idle_status);
		}
#endif

		if (m == 0) {
			ret = hisi_dss_check_vsync_before_iteration_layer(hisifd, pov_req, &vsync_time_checked);
			if (ret < 0)
				goto err_return;
		}

		/* Go through all layers */
		for (i = 0; i < pov_h_block->layer_nums; i++) {
			layer = &(pov_h_block->layer_infos[i]);
			memset(&clip_rect, 0, sizeof(dss_rect_ltrb_t));
			memset(&aligned_rect, 0, sizeof(dss_rect_t));
			rdma_stretch_enable = false;

			ret = hisi_ov_compose_handler(hisifd, pov_req, pov_h_block, layer, NULL, NULL,
				&clip_rect, &aligned_rect, &rdma_stretch_enable, &has_base, true, enable_cmdlist);
			if (ret != 0) {
				HISI_FB_ERR("fb%d, hisi_ov_compose_handler failed! ret = %d\n", hisifd->index, ret);
				goto err_return;
			}
		}

		ret = hisi_dss_mctl_ov_config(hisifd, pov_req, pov_req->ovl_idx, has_base, (m == 0));
		if (ret != 0) {
			HISI_FB_ERR("fb%d, hisi_dss_mctl_config failed! ret = %d\n", hisifd->index, ret);
			goto err_return;
		}

		if (m == 0) {
			if (hisifd->panel_info.dirty_region_updt_support) {
				ret= hisi_dss_dirty_region_dbuf_config(hisifd, pov_req);
				if (ret != 0) {
					HISI_FB_ERR("fb%d, hisi_dss_dirty_region_dbuf_config failed! ret = %d\n", hisifd->index, ret);
					goto err_return;
				}
			}

			ret = hisi_dss_post_scf_config(hisifd, pov_req);
			if (ret != 0) {
				HISI_FB_ERR("fb%d, hisi_dss_post_scf_config failed! ret = %d\n", hisifd->index, ret);
				goto err_return;
			}

		#if defined(CONFIG_HISI_FB_970) || defined(CONFIG_HISI_FB_V501) || \
			defined(CONFIG_HISI_FB_V510) || defined(CONFIG_HISI_FB_V350) || \
			defined(CONFIG_HISI_FB_V600)
			ret = hisi_effect_arsr1p_config(hisifd, pov_req);
			if (ret != 0) {
				HISI_FB_ERR("fb%d, hisi_effect_arsr1p_config failed! ret = %d\n", hisifd->index, ret);
				goto err_return;
			}
			ret = hisi_effect_hiace_config(hisifd);
			if (ret != 0) {
				HISI_FB_ERR("fb%d, hisi_effect_hiace_config failed! ret = %d\n", hisifd->index, ret);
			}
		#elif defined(CONFIG_HISI_FB_V320) || defined(CONFIG_HISI_FB_V330)
			ret = hisi_effect_hiace_config(hisifd);

			if (ret != 0) {
				HISI_FB_ERR("fb%d, hisi_effect_hiace_config failed! ret = %d\n", hisifd->index, ret);
			}
		#endif
			ret = hisi_dss_check_vsync_during_iteration_layer(hisifd, pov_req, &vsync_time_checked);
			if (ret < 0)
				goto err_return;
		}

		ret = hisi_dss_ov_module_set_regs(hisifd, pov_req, pov_req->ovl_idx, enable_cmdlist, 0, 0, (m == 0));
		if (ret != 0) {
			HISI_FB_ERR("fb%d, hisi_dss_module_config failed! ret = %d\n", hisifd->index, ret);
			goto err_return;
		}
	}

	hisi_sec_mctl_set_regs(hisifd);

	if (enable_cmdlist) {
		g_online_cmdlist_idxs |= cmdlist_idxs;
		//add taskend for share channel
		hisi_cmdlist_add_nop_node(hisifd, cmdlist_idxs, 0, 0);

		//remove ch cmdlist
		hisi_cmdlist_config_stop(hisifd, cmdlist_pre_idxs);

		cmdlist_idxs |= cmdlist_pre_idxs;
		hisi_cmdlist_flush_cache(hisifd, cmdlist_idxs);
	}

	if (!g_fake_lcd_flag) {
		hisi_dss_unflow_handler(hisifd, pov_req, true);
	}

	if (!vsync_time_checked) {
		ret = hisi_dss_check_vsync_timediff(hisifd, pov_req);
		if (ret < 0) {
			goto err_return;
		}
	}

	ret = hisifb_buf_sync_create_fence(hisifd, &pov_req->release_fence, &pov_req->retire_fence);
	if (ret != 0) {
		HISI_FB_INFO("fb%d, hisi_create_fence failed! \n", hisifd->index);
	}

#if defined(CONFIG_HISI_FB_V320) || defined(CONFIG_HISI_FB_V330)
	if (is_mipi_video_panel(hisifd))
		hisifb_mask_layer_backlight_config(hisifd, pov_req_prev, pov_req, &masklayer_maxbacklight_flag);
#endif

	spin_lock_irqsave(&hisifd->buf_sync_ctrl.refresh_lock, flags);
	hisifd->buf_sync_ctrl.refresh++;
	spin_unlock_irqrestore(&hisifd->buf_sync_ctrl.refresh_lock, flags);

	hisifb_wait_for_mipi_resource_available(hisifd);

#if defined(CONFIG_HISI_FB_V320) || defined(CONFIG_HISI_FB_V330)
	if (is_mipi_cmd_panel(hisifd))
		hisifb_mask_layer_backlight_config(hisifd, pov_req_prev, pov_req, &masklayer_maxbacklight_flag);
#else
	hisifb_mask_layer_backlight_config(hisifd, pov_req_prev, pov_req, &masklayer_maxbacklight_flag);
#endif

	if (enable_cmdlist) {
		if (hisi_cmdlist_config_start(hisifd, pov_req->ovl_idx, cmdlist_idxs, 0)) {
			hisifb_buf_sync_close_fence(&pov_req->release_fence, &pov_req->retire_fence);
			ret = -EFAULT;
			goto err_return;
		}
	} else {
		hisi_dss_mctl_mutex_unlock(hisifd, pov_req->ovl_idx);
	}

	if (hisifd->panel_info.dirty_region_updt_support) {
		hisi_dss_dirty_region_updt_config(hisifd, pov_req);
	}


	/* cpu config drm layer */
	hisi_drm_layer_online_config(hisifd, pov_req_prev, pov_req);

	hisifb_dc_backlight_config(hisifd);

	if ((hisifd->index == PRIMARY_PANEL_IDX) && (hisifd->online_play_count > 1))
		hisifb_rgb_read_register(hisifd);

	hisi_vactive0_end_isr_handler(hisifd);

	single_frame_update(hisifd);

	hisifb_frame_updated(hisifd);

	ov_block_infos_ptr = pov_req->ov_block_infos_ptr;
	pov_req->ov_block_infos_ptr = (uint64_t)0; // clear ov_block_infos_ptr

	hisifb_get_timestamp(&returntv1);
	pov_req->online_wait_timediff = hisifb_timestamp_diff(&returntv0, &returntv1);

	if (copy_to_user((struct dss_overlay_t __user *)argp,
			pov_req, sizeof(dss_overlay_t))) {
		HISI_FB_ERR("fb%d, copy_to_user failed.\n", hisifd->index);
		hisifb_buf_sync_close_fence(&pov_req->release_fence, &pov_req->retire_fence);
		ret = -EFAULT;
		goto err_return;
	}

	if (hisifd->ov_req_prev.ov_block_nums > HISI_DSS_OV_BLOCK_NUMS) {
		HISI_FB_ERR("ov_block_nums %d is out of range!\n", hisifd->ov_req_prev.ov_block_nums);
		goto err_return;
	}
	/* pass to hwcomposer handle, driver doesn't use it no longer */
	pov_req->release_fence = -1;
	pov_req->retire_fence = -1;
	/* restore the original value from the variable ov_block_infos_ptr */
	pov_req->ov_block_infos_ptr = ov_block_infos_ptr;

	hisifd->enter_idle = false;
	hisifd->emi_protect_check_count = 0;
	hisifb_deactivate_vsync(hisifd);

	hisifb_layerbuf_flush(hisifd, &lock_list);

	hisifb_masklayer_backlight_flag_config(hisifd, masklayer_maxbacklight_flag);

	if ((hisifd->index == PRIMARY_PANEL_IDX) && (dss_free_buffer_refcount > 1)) {
		if (!hisifd->fb_mem_free_flag) {
			hisifb_free_fb_buffer(hisifd);
			hisifd->fb_mem_free_flag = true;
		}

		if (g_logo_buffer_base && g_logo_buffer_size) {
			hisifb_free_logo_buffer(hisifd);
			HISI_FB_INFO("dss_free_buffer_refcount = %d !\n", dss_free_buffer_refcount);
		}

		/*wakeup DP when android system has been startup*/
		if (fb1 != NULL) {
			fb1->dp.dptx_gate = true;
			if (fb1->dp_wakeup != NULL) {
				fb1->dp_wakeup(fb1);
			}
		}
	}

	if (g_debug_ovl_online_composer == 2) {
		dumpDssOverlay(hisifd, pov_req);
	}

	if (g_debug_ovl_cmdlist && enable_cmdlist) {
		hisi_cmdlist_dump_all_node(hisifd, NULL, cmdlist_idxs);
	}

	hisifd->frame_count++;
	dss_free_buffer_refcount++;

	memcpy(&hisifd->ov_req_prev_prev, &hisifd->ov_req_prev, sizeof(dss_overlay_t));
	memcpy(&(hisifd->ov_block_infos_prev_prev), &(hisifd->ov_block_infos_prev),
		hisifd->ov_req_prev.ov_block_nums * sizeof(dss_overlay_block_t));
	hisifd->ov_req_prev_prev.ov_block_infos_ptr = (uint64_t)(uintptr_t)(&(hisifd->ov_block_infos_prev_prev));

	memcpy(&hisifd->ov_req_prev, pov_req, sizeof(dss_overlay_t));
	memcpy(&(hisifd->ov_block_infos_prev), &(hisifd->ov_block_infos),
		pov_req->ov_block_nums * sizeof(dss_overlay_block_t));
	hisifd->ov_req_prev.ov_block_infos_ptr = (uint64_t)(uintptr_t)(&(hisifd->ov_block_infos_prev));
	hisifd->vsync_ctrl.vsync_timestamp_prev = hisifd->vsync_ctrl.vsync_timestamp;

	if (g_debug_ovl_online_composer_timediff & 0x2) {
		hisifb_get_timestamp(&tv1);
		timediff = hisifb_timestamp_diff(&tv0, &tv1);
		if (timediff >= g_debug_ovl_online_composer_time_threshold)  /*lint !e737*/
			HISI_FB_WARNING("ONLINE_TIMEDIFF is %u us!\n", timediff);
	}
	up(&hisifd->blank_sem0);

	return 0;

err_return:
	if (is_mipi_cmd_panel(hisifd)) {
		hisifd->vactive0_start_flag = 1;
	}
	hisifb_layerbuf_lock_exception(hisifd, &lock_list);
	hisifb_deactivate_vsync(hisifd);
	if (!need_skip) {
		up(&hisifd->blank_sem0);
	}
	return ret;
}

int hisi_online_play_bypass(struct hisi_fb_data_type *hisifd, const void __user *argp)
{
	int ret;
	int bypass;

	if (hisifd == NULL) {
		HISI_FB_ERR("NULL Pointer!\n");
		return -EINVAL;
	}

	if (argp == NULL) {
		HISI_FB_ERR("NULL Pointer!\n");
		return -EINVAL;
	}

	// only bypass primary display
	if (hisifd->index != PRIMARY_PANEL_IDX)
		return -EINVAL;

	// only active with cmd panel
	if (!is_mipi_cmd_panel(hisifd))
		return -EINVAL;

	HISI_FB_INFO("+\n");

	ret = (int)copy_from_user(&bypass, argp, sizeof(bypass));
	if (ret) {
		HISI_FB_ERR("arg is invalid");
		return -EINVAL;
	}

	if (!hisifd->panel_power_on) {
		HISI_FB_INFO("fb%d, panel is power off!\n", hisifd->index);
		if (bypass) {
			HISI_FB_INFO("cannot bypss when power off!\n");
			return -EINVAL;
		}
	}

	(void) hisi_online_play_bypass_set(hisifd, bypass);

	HISI_FB_INFO("-\n");
	return 0;
}

bool hisi_online_play_bypass_set(struct hisi_fb_data_type *hisifd, int bypass)
{
	if (hisifd == NULL) {
		HISI_FB_ERR("NULL Pointer!\n");
		return false;
	}

	// only bypass primary display
	if (hisifd->index != PRIMARY_PANEL_IDX)
		return false;

	// only active with cmd panel
	if (!is_mipi_cmd_panel(hisifd))
		return false;

	HISI_FB_INFO("bypass = %d\n", bypass);

	if (bypass != 0) {
		if (hisifd->bypass_info.bypass_count > ONLINE_PLAY_BYPASS_MAX_COUNT) {
			HISI_FB_WARNING("already reach the max bypass count, disable bypass first\n");
			return false;
		}
		hisifd->bypass_info.bypass = true;
	} else {
		// reset state, be available to start a new bypass cycle
		hisifd->bypass_info.bypass = false;
		hisifd->bypass_info.bypass_count = 0;
	}

	return true;
}

bool hisi_online_play_bypass_check(struct hisi_fb_data_type *hisifd)
{
	bool ret = true;

	if (hisifd == NULL) {
		HISI_FB_ERR("NULL Pointer!\n");
		return false;
	}

	// only bypass primary display
	if (hisifd->index != PRIMARY_PANEL_IDX)
		return false;

	// only active with cmd panel
	if (!is_mipi_cmd_panel(hisifd))
		return false;

	if (g_debug_online_play_bypass) {
		if (g_debug_online_play_bypass < 0)
			g_debug_online_play_bypass = 0;
		if (hisifd->panel_power_on)
			hisi_online_play_bypass_set(hisifd, g_debug_online_play_bypass);
		g_debug_online_play_bypass = 0;
	}

	if (!hisifd->bypass_info.bypass)
		return false;

	hisifd->bypass_info.bypass_count++;
	if (hisifd->bypass_info.bypass_count > ONLINE_PLAY_BYPASS_MAX_COUNT) {
		// not reset the bypass_count, wait the call of stopping bypass
		hisifd->bypass_info.bypass = false;
		ret = false;
		HISI_FB_WARNING("reach the max bypass count, restore online play\n");
	}

	return ret;
}

#pragma GCC diagnostic pop
/*lint +e776 +e737 +e574 +e648 +e570 +e565*/
