

#if (_PRE_WLAN_FEATURE_BLACKLIST_LEVEL != _PRE_WLAN_FEATURE_BLACKLIST_NONE)

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif

/* 1 头文件包含 */
#include "hmac_main.h"
#include "hmac_blacklist.h"
#include "mac_vap.h"
#include "hmac_vap.h"
#include "mac_resource.h"
#include "hmac_user.h"
#include "hmac_mgmt_ap.h"
#include "securec.h"
#include "securectype.h"

#undef THIS_FILE_ID
#define THIS_FILE_ID OAM_FILE_ID_HMAC_BLACKLIST_C

/* 2 STRUCT定义 */
/* 3 全局变量定义 */
/* 4 函数实现 */

OAL_STATIC oal_uint32 hmac_blacklist_mac_is_zero(oal_uint8 *puc_mac_addr)
{
    return (puc_mac_addr[0] == 0 &&
            puc_mac_addr[1] == 0 &&
            puc_mac_addr[2] == 0 &&
            puc_mac_addr[3] == 0 &&
            puc_mac_addr[4] == 0 &&
            puc_mac_addr[5] == 0);
}


OAL_STATIC oal_uint32 hmac_blacklist_mac_is_bcast(const oal_uint8 *puc_mac_addr)
{
    return ((*(const oal_uint16 *)(puc_mac_addr + 0) &
             *(const oal_uint16 *)(puc_mac_addr + 2) &
             *(const oal_uint16 *)(puc_mac_addr + 4)) == 0xffff);
}


OAL_STATIC oal_void hmac_blacklist_init(mac_vap_stru *pst_mac_vap,
                                        cs_blacklist_mode_enum_uint8 en_mode,
                                        oal_bool_enum_uint8 en_flush)
{
    oal_uint32 ul_size;
    mac_blacklist_info_stru *pst_blacklist_info = OAL_PTR_NULL;
    hmac_vap_stru *pst_hmac_vap = OAL_PTR_NULL;

    if ((pst_mac_vap->en_vap_mode != WLAN_VAP_MODE_BSS_AP) && (pst_mac_vap->en_vap_mode != WLAN_VAP_MODE_BSS_STA)) {
        OAM_WARNING_LOG0(0, OAM_SF_ANY, "{hmac_blacklist_init::not AP or STA mode!}\r\n");
        return;
    }
    pst_hmac_vap = (hmac_vap_stru *)mac_res_get_hmac_vap(pst_mac_vap->uc_vap_id);
    if (pst_hmac_vap == OAL_PTR_NULL) {
        OAM_ERROR_LOG0(pst_mac_vap->uc_vap_id, OAM_SF_CFG, "{hmac_blacklist_init::pst_hmac_vap null.}");
        return;
    }

    pst_blacklist_info = pst_hmac_vap->pst_blacklist_info;
    if (pst_blacklist_info == OAL_PTR_NULL) {
        OAM_ERROR_LOG0(0, OAM_SF_ANY, "{hmac_blacklist_init::pst_blacklist_info null!}\r\n");
        return;
    }
    ul_size = sizeof(mac_blacklist_info_stru);
    /* Max=32 => 新增加mac_vap结构大小= 0x494 = 1172 ; Max=8 => size = 308 */
    if (en_flush) {
        memset_s(pst_blacklist_info->ast_black_list,
                 sizeof(mac_blacklist_stru) * WLAN_BLACKLIST_MAX, 0,
                 sizeof(mac_blacklist_stru) * WLAN_BLACKLIST_MAX);
        memset_s(&(pst_blacklist_info->st_autoblacklist_info),
                 sizeof(mac_autoblacklist_info_stru), 0,
                 sizeof(mac_autoblacklist_info_stru));
        pst_blacklist_info->uc_list_num = 0;
    }
    /* cannot clear the following index */
    /* pst_blacklist_info->uc_blacklist_device_index;
    pst_blacklist_info->uc_blacklist_vap_index; */
    pst_blacklist_info->uc_mode = en_mode;
}


OAL_STATIC oal_bool_enum_uint8 hmac_blacklist_is_aged(mac_vap_stru *pst_mac_vap, mac_blacklist_stru *pst_blacklist)
{
    oal_time_us_stru st_cur_time;
    mac_blacklist_info_stru *pst_blacklist_info = OAL_PTR_NULL;
    oal_uint8 *puc_mac_addr = OAL_PTR_NULL;
    hmac_vap_stru *pst_hmac_vap = OAL_PTR_NULL;

    /* 1.1 全零地址，属于非法地址 */
    if (hmac_blacklist_mac_is_zero(pst_blacklist->auc_mac_addr)) {
        return OAL_FALSE;
    }

    /* 2.1 老化时间为0表示不需要老化 */
    if (pst_blacklist->ul_aging_time == 0) {
        return OAL_FALSE;
    }

    /* 2.2 没有超过老化时间 */
    oal_time_get_stamp_us(&st_cur_time);
    if (st_cur_time.i_sec < (oal_int)(pst_blacklist->ul_cfg_time + pst_blacklist->ul_aging_time)) {
        return OAL_FALSE;
    }

    pst_hmac_vap = (hmac_vap_stru *)mac_res_get_hmac_vap(pst_mac_vap->uc_vap_id);
    if (pst_hmac_vap == OAL_PTR_NULL) {
        OAM_ERROR_LOG0(pst_mac_vap->uc_vap_id, OAM_SF_CFG, "{hmac_blacklist_is_aged::pst_hmac_vap null.}");
        return OAL_ERR_CODE_PTR_NULL;
    }

    puc_mac_addr = pst_blacklist->auc_mac_addr;
    OAM_INFO_LOG4(pst_mac_vap->uc_vap_id, OAM_SF_ANY,
                  "{aging time reach delete MAC:=%02X:XX:XX:%02X:%02X:%02X}",
                  puc_mac_addr[0], puc_mac_addr[3], puc_mac_addr[4], puc_mac_addr[5]);

    /* 3.1 直接从黑名单中删除 */
    pst_blacklist_info = pst_hmac_vap->pst_blacklist_info;
    if (pst_blacklist_info == OAL_PTR_NULL) {
        OAM_WARNING_LOG0(pst_mac_vap->uc_vap_id, OAM_SF_ANY, "{hmac_blacklist_is_aged::pst_blacklist_info null!}\r\n");
        return OAL_ERR_CODE_PTR_NULL;
    }
    if (pst_blacklist_info->uc_mode == CS_BLACKLIST_MODE_BLACK) {
        memset_s(pst_blacklist, sizeof(mac_blacklist_stru), 0, sizeof(mac_blacklist_stru));
        if (pst_blacklist_info->uc_list_num > 0) {
            pst_blacklist_info->uc_list_num--; /* 2014.7.23 bug fixed */
            OAM_INFO_LOG1(pst_mac_vap->uc_vap_id, OAM_SF_ANY,
                          "{del from blacklist & num=%d}",
                          pst_blacklist_info->uc_list_num);
        }
        return OAL_TRUE;
    }

    /* 4.1 直接从白名单中恢复 */
    if (pst_blacklist_info->uc_mode == CS_BLACKLIST_MODE_WHITE) {
        pst_blacklist->ul_aging_time = 0;
        return OAL_TRUE;
    }

    return OAL_FALSE;
}


OAL_STATIC oal_uint32 hmac_blacklist_get(mac_vap_stru *pst_mac_vap,
                                         oal_uint8 *puc_mac_addr,
                                         mac_blacklist_stru **ppst_blacklist)
{
    oal_uint8 ul_blacklist_index;
    mac_blacklist_info_stru *pst_blacklist_info = OAL_PTR_NULL;
    hmac_vap_stru *pst_hmac_vap = OAL_PTR_NULL;

    /* 1.1 广播地址，属于非法地址 */
    if (hmac_blacklist_mac_is_bcast(puc_mac_addr)) {
        return OAL_ERR_CODE_SECURITY_MAC_INVALID;
    }

    /* 1.2 全零地址，属于非法地址 */
    if (hmac_blacklist_mac_is_zero(puc_mac_addr)) {
        return OAL_ERR_CODE_SECURITY_MAC_INVALID;
    }

    pst_hmac_vap = (hmac_vap_stru *)mac_res_get_hmac_vap(pst_mac_vap->uc_vap_id);
    if (pst_hmac_vap == OAL_PTR_NULL) {
        OAM_ERROR_LOG0(pst_mac_vap->uc_vap_id, OAM_SF_CFG, "{hmac_blacklist_get::pst_hmac_vap null.}");
        return OAL_ERR_CODE_PTR_NULL;
    }

    pst_blacklist_info = pst_hmac_vap->pst_blacklist_info;
    if (pst_blacklist_info == OAL_PTR_NULL) {
        OAM_WARNING_LOG0(pst_mac_vap->uc_vap_id, OAM_SF_ANY, "{hmac_blacklist_get::pst_blacklist_info null!}\r\n");
        return OAL_ERR_CODE_PTR_NULL;
    }

    /* 2.1 找到表单 */
    for (ul_blacklist_index = 0; ul_blacklist_index < WLAN_BLACKLIST_MAX; ul_blacklist_index++) {
        *ppst_blacklist = &pst_blacklist_info->ast_black_list[ul_blacklist_index];
        if (!oal_memcmp((*ppst_blacklist)->auc_mac_addr, puc_mac_addr, OAL_MAC_ADDR_LEN)) {
            break;
        }
        *ppst_blacklist = OAL_PTR_NULL;
    }

    return OAL_SUCC;
}


oal_uint32 hmac_backlist_get_drop_etc(mac_vap_stru *pst_mac_vap, oal_uint8 *puc_mac_addr)
{
    /* 2014.9.2 Add UT and found no init value set to it ! add initial value null */
    mac_blacklist_stru *pst_blacklist = OAL_PTR_NULL;

    /* 1.1 入参检查 */
    if (pst_mac_vap == OAL_PTR_NULL) {
        OAM_ERROR_LOG0(0, OAM_SF_ANY, "{hmac_backlist_get_drop_etc::null mac_vap}");
        return OAL_ERR_CODE_PTR_NULL;
    }

    hmac_blacklist_get(pst_mac_vap, puc_mac_addr, &pst_blacklist);
    if (pst_blacklist != OAL_PTR_NULL) {
        return pst_blacklist->ul_drop_counter;
    }

    return OAL_FAIL;
}


OAL_STATIC oal_uint32 hmac_backlist_sub_drop(mac_vap_stru *pst_mac_vap, oal_uint8 *puc_mac_addr)
{
    oal_uint32 ul_ret;
    /* 2014.9.2 Add UT and found no init value set to it ! add initial value null */
    mac_blacklist_stru *pst_blacklist = OAL_PTR_NULL;

    ul_ret = hmac_blacklist_get(pst_mac_vap, puc_mac_addr, &pst_blacklist);

    if (pst_blacklist != OAL_PTR_NULL) {
        if (pst_blacklist->ul_drop_counter > 0) {
            pst_blacklist->ul_drop_counter--;
        }
        ul_ret = OAL_SUCC;
    }
    return ul_ret;
}


oal_uint8 hmac_backlist_get_list_num_etc(mac_vap_stru *pst_mac_vap)
{
    mac_blacklist_info_stru *pst_blacklist_info = OAL_PTR_NULL;
    hmac_vap_stru *pst_hmac_vap = OAL_PTR_NULL;

    /* 1.1 入参检查 */
    if (pst_mac_vap == OAL_PTR_NULL) {
        OAM_ERROR_LOG0(0, OAM_SF_ANY, "{hmac_backlist_get_list_num_etc::null mac_vap}");
        return OAL_ERR_CODE_PTR_NULL;
    }

    pst_hmac_vap = (hmac_vap_stru *)mac_res_get_hmac_vap(pst_mac_vap->uc_vap_id);
    if (pst_hmac_vap == OAL_PTR_NULL) {
        OAM_ERROR_LOG0(pst_mac_vap->uc_vap_id, OAM_SF_CFG, "{hmac_backlist_get_list_num_etc::pst_hmac_vap null.}");
        return OAL_ERR_CODE_PTR_NULL;
    }

    pst_blacklist_info = pst_hmac_vap->pst_blacklist_info;
    if (pst_blacklist_info == OAL_PTR_NULL) {
        OAM_WARNING_LOG0(pst_mac_vap->uc_vap_id, OAM_SF_ANY,
                         "{hmac_backlist_get_list_num_etc::pst_blacklist_info null!}\r\n");
        return OAL_ERR_CODE_PTR_NULL;
    }

    return pst_blacklist_info->uc_list_num;
}


OAL_STATIC oal_uint32 hmac_autoblacklist_get(mac_vap_stru *pst_mac_vap,
                                             oal_uint8 *puc_mac_addr,
                                             mac_autoblacklist_stru **ppst_autoblacklist)
{
    oal_uint8 ul_blacklist_index;
    mac_autoblacklist_info_stru *pst_autoblacklist_info = OAL_PTR_NULL;
    oal_time_us_stru st_cur_time;
    oal_uint32 ul_cfg_time_old;
    oal_uint32 ul_blacklist_index_old;
    mac_autoblacklist_stru *pst_autoblacklist = OAL_PTR_NULL;
    hmac_vap_stru *pst_hmac_vap = OAL_PTR_NULL;

    /* 1.1 广播地址，属于非法地址 */
    if (hmac_blacklist_mac_is_bcast(puc_mac_addr)) {
        return OAL_ERR_CODE_SECURITY_MAC_INVALID;
    }

    /* 1.2 全零地址，属于非法地址 */
    if (hmac_blacklist_mac_is_zero(puc_mac_addr)) {
        return OAL_ERR_CODE_SECURITY_MAC_INVALID;
    }

    pst_hmac_vap = (hmac_vap_stru *)mac_res_get_hmac_vap(pst_mac_vap->uc_vap_id);
    if (OAL_ANY_NULL_PTR2(pst_hmac_vap, pst_hmac_vap->pst_blacklist_info)) {
        OAM_ERROR_LOG0(pst_mac_vap->uc_vap_id, OAM_SF_CFG,
                       "{hmac_autoblacklist_get::pst_hmac_vap or pst_blacklist_info null.}");
        return OAL_ERR_CODE_PTR_NULL;
    }
    pst_autoblacklist_info = &(pst_hmac_vap->pst_blacklist_info->st_autoblacklist_info);

    oal_time_get_stamp_us(&st_cur_time);

    *ppst_autoblacklist = OAL_PTR_NULL;

    /* 2.1 找到历史表单 */
    for (ul_blacklist_index = 0; ul_blacklist_index < WLAN_BLACKLIST_MAX; ul_blacklist_index++) {
        pst_autoblacklist = &pst_autoblacklist_info->ast_autoblack_list[ul_blacklist_index];
        /* 2.2 无效表单 */
        if (pst_autoblacklist->ul_cfg_time == 0) {
            continue;
        }

        /* 2.2 过期表单直接清0 */
        if (st_cur_time.i_sec > (oal_int)(pst_autoblacklist->ul_cfg_time + pst_autoblacklist_info->ul_reset_time)) {
            if (pst_autoblacklist_info->list_num > 0) {
                pst_autoblacklist_info->list_num--;
            }
            memset_s(pst_autoblacklist, OAL_SIZEOF(mac_autoblacklist_stru), 0, OAL_SIZEOF(mac_autoblacklist_stru));
            continue;
        }

        /* 2.3 有效表单，mac地址比对 */
        if (!oal_memcmp(pst_autoblacklist->auc_mac_addr, puc_mac_addr, OAL_MAC_ADDR_LEN)) {
            *ppst_autoblacklist = pst_autoblacklist;
            break;
        }
    }

    if (*ppst_autoblacklist != OAL_PTR_NULL) {
        OAM_INFO_LOG3(pst_mac_vap->uc_vap_id, OAM_SF_ANY,
                      "{Get a history item from auto_blacklist.index=%d.MAC=x.x.x.x.%02x.%02x.}",
                      ul_blacklist_index, puc_mac_addr[4], puc_mac_addr[5]);
        return OAL_SUCC;
    }

    /* 3.1 找到新表单 */
    ul_cfg_time_old = (st_cur_time.i_sec > 0) ? (oal_uint32)(st_cur_time.i_sec) : 0;
    ul_blacklist_index_old = 0;
    for (ul_blacklist_index = 0; ul_blacklist_index < WLAN_BLACKLIST_MAX; ul_blacklist_index++) {
        pst_autoblacklist = &pst_autoblacklist_info->ast_autoblack_list[ul_blacklist_index];
        /* 2.2 找到空闲表单 */
        if (pst_autoblacklist->ul_cfg_time == 0) {
            pst_autoblacklist_info->list_num++;
            *ppst_autoblacklist = pst_autoblacklist;
            break;
        }

        /* 2.3 记录最老配置的表单 */
        if (pst_autoblacklist->ul_cfg_time < ul_cfg_time_old) {
            ul_cfg_time_old = pst_autoblacklist->ul_cfg_time;
            ul_blacklist_index_old = ul_blacklist_index;
        }
    }

    /* 4.1 如果没有空闲的表单，就使用最老的表单 */
    if (*ppst_autoblacklist == OAL_PTR_NULL) {
        pst_autoblacklist = &pst_autoblacklist_info->ast_autoblack_list[ul_blacklist_index_old];
        *ppst_autoblacklist = pst_autoblacklist;
    }

    /* 5.1 更新表单 */
    OAM_INFO_LOG4(pst_mac_vap->uc_vap_id, OAM_SF_ANY,
                  "{add to auto_blacklist MAC:=%02X:XX:XX:%02X:%02X:%02X. assoc_count=1.}",
                  puc_mac_addr[0], puc_mac_addr[3], puc_mac_addr[4], puc_mac_addr[5]);
    if (pst_autoblacklist == OAL_PTR_NULL) {
        OAM_ERROR_LOG0(0, OAM_SF_ANY, "{hmac_autoblacklist_get::pst_autoblacklist NULL}");
        return OAL_ERR_CODE_PTR_NULL;
    }
    if (EOK != memcpy_s(pst_autoblacklist->auc_mac_addr,
                        sizeof(pst_autoblacklist->auc_mac_addr),
                        puc_mac_addr, OAL_MAC_ADDR_LEN)) {
        OAM_ERROR_LOG0(0, OAM_SF_ANY, "hmac_autoblacklist_get::memcpy fail!");
        return OAL_FAIL;
    }
    pst_autoblacklist->ul_cfg_time = (oal_uint32)st_cur_time.i_sec;
    pst_autoblacklist->ul_asso_counter = 0; /* bug 1 => 0 fixed */

    return OAL_SUCC;
}


OAL_STATIC oal_uint32 hmac_blacklist_update_delete_user(hmac_vap_stru *pst_hmac_vap, hmac_user_stru *pst_hmac_user)
{
    mac_vap_stru *pst_mac_vap = &(pst_hmac_vap->st_vap_base_info);
    mac_user_stru *pst_mac_user = &(pst_hmac_user->st_user_base_info);

    OAM_WARNING_LOG4(pst_mac_vap->uc_vap_id, OAM_SF_ANY,
                     "hmac_blacklist_update_delete_user: send disasoc frame to XX:XX:XX:%2X:%2X:%2X. blacklist_mode:%d",
                     pst_mac_user->auc_user_mac_addr[3],
                     pst_mac_user->auc_user_mac_addr[4],
                     pst_mac_user->auc_user_mac_addr[5],
                     pst_hmac_vap->pst_blacklist_info->uc_mode);

    hmac_mgmt_send_disassoc_frame_etc(pst_mac_vap, pst_mac_user->auc_user_mac_addr, MAC_DISAS_LV_SS, OAL_FALSE);

    /* 删除黑名单内用户，需要上报内核 */
    hmac_handle_disconnect_rsp_etc(pst_hmac_vap, pst_hmac_user, MAC_DISAS_LV_SS);

    hmac_user_del_etc(pst_mac_vap, pst_hmac_user);

    return OAL_SUCC;
}


OAL_STATIC oal_uint32 hmac_whitelist_check_user(mac_vap_stru *pst_mac_vap)
{
    oal_uint32 ul_ret;
    hmac_vap_stru *pst_hmac_vap = OAL_PTR_NULL;
    oal_dlist_head_stru *pst_entry = OAL_PTR_NULL;
    oal_dlist_head_stru *pst_dlist_tmp = OAL_PTR_NULL;
    mac_user_stru *pst_user_tmp = OAL_PTR_NULL;
    hmac_user_stru *pst_hmac_user_tmp = OAL_PTR_NULL;

    pst_hmac_vap = (hmac_vap_stru *)mac_res_get_hmac_vap(pst_mac_vap->uc_vap_id);
    if (pst_hmac_vap == OAL_PTR_NULL) {
        OAM_ERROR_LOG0(pst_mac_vap->uc_vap_id, OAM_SF_ANY, "hmac_whitelist_check_user: pst_hmac_vap is null");
        return OAL_ERR_CODE_PTR_NULL;
    }

    /* 遍历VAP下所有用户，不在白名单里的删除 */
    OAL_DLIST_SEARCH_FOR_EACH_SAFE(pst_entry, pst_dlist_tmp, &(pst_mac_vap->st_mac_user_list_head))
    {
        pst_user_tmp = OAL_DLIST_GET_ENTRY(pst_entry, mac_user_stru, st_user_dlist);
        if (pst_user_tmp == OAL_PTR_NULL) {
            continue;
        }

        pst_hmac_user_tmp = mac_res_get_hmac_user_etc(pst_user_tmp->us_assoc_id);
        if (pst_hmac_user_tmp == OAL_PTR_NULL) {
            continue;
        }

        ul_ret = hmac_blacklist_filter_etc(pst_mac_vap, pst_user_tmp->auc_user_mac_addr);
        if (ul_ret != OAL_TRUE) {
            continue;
        }
        /*
         * 2014.6.30 chenchongbao 在如上的hmac_blacklist_filter()中会
         * ul_drop_counter++ 它不是实际的过滤数，所以--恢复
         */
        hmac_backlist_sub_drop(pst_mac_vap, pst_user_tmp->auc_user_mac_addr);

        hmac_blacklist_update_delete_user(pst_hmac_vap, pst_hmac_user_tmp);
    }

    return OAL_SUCC;
}


OAL_STATIC oal_uint32 hmac_blacklist_vap_check_user_by_macaddr(mac_vap_stru *pst_mac_vap, oal_uint8 *puc_mac_addr)
{
    mac_user_stru *pst_mac_user = OAL_PTR_NULL;
    hmac_user_stru *pst_hmac_user = OAL_PTR_NULL;
    hmac_vap_stru *pst_hmac_vap = OAL_PTR_NULL;
    oal_uint32 ul_ret;
    oal_uint16 us_idx;
    oal_bool_enum_uint8 bool_ret;

    if ((pst_mac_vap->en_vap_mode != WLAN_VAP_MODE_BSS_AP) && (pst_mac_vap->en_vap_mode != WLAN_VAP_MODE_BSS_STA)) {
        OAM_WARNING_LOG0(pst_mac_vap->uc_vap_id, OAM_SF_ANY,
                         "{hmac_blacklist_vap_check_user_by_macaddr::not AP or STA mode!}\r\n");
        return OAL_SUCC;
    }
    ul_ret = mac_vap_find_user_by_macaddr_etc(pst_mac_vap, puc_mac_addr, &us_idx);
    if (ul_ret != OAL_SUCC) {
        /* 该mac地址在此vap下不存在用户，不需要删除，直接返回成功 */
        return OAL_SUCC;
    }

    pst_mac_user = (mac_user_stru *)mac_res_get_mac_user_etc(us_idx);
    if (pst_mac_user == OAL_PTR_NULL) {
        return OAL_FAIL; /* 异常返回 */
    }

    bool_ret = hmac_blacklist_filter_etc(pst_mac_vap, pst_mac_user->auc_user_mac_addr);
    if (bool_ret != OAL_TRUE) {
        OAM_WARNING_LOG1(pst_mac_vap->uc_vap_id, OAM_SF_ANY,
                         "{hmac_blacklist_vap_check_user_by_macaddr::hmac_blacklist_filter_etc bool_ret=%d}", bool_ret);
        return OAL_FAIL;
    }
    /* 2014.6.30 chenchongbao 在如上的hmac_blacklist_filter()中会 ul_drop_counter++ 它不是实际的过滤数，所以--恢复 */
    hmac_backlist_sub_drop(pst_mac_vap, pst_mac_user->auc_user_mac_addr);

    pst_hmac_user = mac_res_get_hmac_user_etc(us_idx);
    if (pst_hmac_user == OAL_PTR_NULL) {
        OAM_WARNING_LOG1(pst_mac_vap->uc_vap_id, OAM_SF_ANY,
                         "{hmac_blacklist_vap_check_user_by_macaddr::mac_res_get_hmac_user_etc fail IDX:%d}", us_idx);
        return OAL_ERR_CODE_PTR_NULL;
    }

    pst_hmac_vap = (hmac_vap_stru *)mac_res_get_hmac_vap(pst_mac_vap->uc_vap_id);
    if (pst_hmac_vap == OAL_PTR_NULL) {
        return OAL_ERR_CODE_PTR_NULL;
    }

    hmac_blacklist_update_delete_user(pst_hmac_vap, pst_hmac_user);

    return ul_ret;
}

oal_void hmac_whitelist_check_device_user(oal_uint8 uc_device_id)
{
    mac_device_stru *pst_device;
    mac_vap_stru *pst_mac_vap = OAL_PTR_NULL;
    oal_uint8 uc_vap_index;

    pst_device = mac_res_get_dev_etc(uc_device_id);
    if (pst_device == OAL_PTR_NULL) {
        OAM_WARNING_LOG0(0, OAM_SF_ANY, "{hmac_whitelist_check_device_user::mac_res_get_dev_etc failed.");
        return;
    }
    for (uc_vap_index = 0; uc_vap_index < pst_device->uc_vap_num; uc_vap_index++) {
        pst_mac_vap = (mac_vap_stru *)mac_res_get_mac_vap(pst_device->auc_vap_id[uc_vap_index]);
        if (pst_mac_vap == OAL_PTR_NULL) {
            OAM_WARNING_LOG0(0, OAM_SF_ANY, "{hmac_whitelist_check_device_user::mac_res_get_mac_vap failed.");
            continue;
        }
        if ((pst_mac_vap->en_vap_mode != WLAN_VAP_MODE_BSS_AP) && (pst_mac_vap->en_vap_mode != WLAN_VAP_MODE_BSS_STA)) {
            OAM_WARNING_LOG0(0, OAM_SF_ANY, "{hmac_whitelist_check_device_user::not AP or STA mode.");
            continue;
        }
        hmac_whitelist_check_user(pst_mac_vap);
    }
}


oal_void hmac_whitelist_check_all_user(mac_vap_stru *pst_mac_vap)
{
#if (_PRE_WLAN_FEATURE_BLACKLIST_LEVEL == _PRE_WLAN_FEATURE_BLACKLIST_CHIP) ||  \
    (_PRE_WLAN_FEATURE_BLACKLIST_LEVEL == _PRE_WLAN_FEATURE_BLACKLIST_BOARD)
    mac_chip_stru *pst_chip;
    oal_uint8 uc_device_index;
#if (_PRE_WLAN_FEATURE_BLACKLIST_LEVEL == _PRE_WLAN_FEATURE_BLACKLIST_BOARD)
    oal_uint8 uc_chip_index;
#endif
#endif

#if (_PRE_WLAN_FEATURE_BLACKLIST_LEVEL == _PRE_WLAN_FEATURE_BLACKLIST_BOARD)
    for (uc_chip_index = 0; uc_chip_index < WLAN_CHIP_MAX_NUM_PER_BOARD; uc_chip_index++) {
        pst_chip = &g_pst_mac_board->ast_chip[uc_chip_index];
        for (uc_device_index = 0; uc_device_index < pst_chip->uc_device_nums; uc_device_index++) {
            hmac_whitelist_check_device_user(pst_chip->auc_device_id[uc_device_index]);
        }
    }
#elif (_PRE_WLAN_FEATURE_BLACKLIST_LEVEL == _PRE_WLAN_FEATURE_BLACKLIST_CHIP)
    pst_chip = &g_pst_mac_board->ast_chip[pst_mac_vap->uc_chip_id];
    for (uc_device_index = 0; uc_device_index < pst_chip->uc_device_nums; uc_device_index++) {
        hmac_whitelist_check_device_user(pst_chip->auc_device_id[uc_device_index]);
    }
#elif (_PRE_WLAN_FEATURE_BLACKLIST_LEVEL == _PRE_WLAN_FEATURE_BLACKLIST_DEVICE)
    hmac_whitelist_check_device_user(pst_mac_vap->uc_device_id);
#elif (_PRE_WLAN_FEATURE_BLACKLIST_LEVEL == _PRE_WLAN_FEATURE_BLACKLIST_VAP)
    if ((pst_mac_vap->en_vap_mode != WLAN_VAP_MODE_BSS_AP) && (pst_mac_vap->en_vap_mode != WLAN_VAP_MODE_BSS_STA)) {
        OAM_WARNING_LOG0(0, OAM_SF_ANY, "{hmac_whitelist_check_all_user::not AP or STA mode.");
        return;
    }
    hmac_whitelist_check_user(pst_mac_vap);
#endif
}


oal_uint32 hmac_blacklist_add_etc(mac_vap_stru *pst_mac_vap, oal_uint8 *puc_mac_addr, oal_uint32 ul_aging_time)
{
    oal_uint8 ul_blacklist_index;
    mac_blacklist_info_stru *pst_blacklist_info = OAL_PTR_NULL;
    mac_blacklist_stru *pst_blacklist = OAL_PTR_NULL;
    oal_time_us_stru st_cur_time;
    hmac_vap_stru *pst_hmac_vap = OAL_PTR_NULL;

    /* 1.1 入参检查 */
    if (pst_mac_vap == OAL_PTR_NULL) {
        OAM_ERROR_LOG0(0, OAM_SF_ANY, "{hmac_blacklist_add_etc::null mac_vap}");
        return OAL_ERR_CODE_PTR_NULL;
    }
    if ((pst_mac_vap->en_vap_mode != WLAN_VAP_MODE_BSS_AP) && (pst_mac_vap->en_vap_mode != WLAN_VAP_MODE_BSS_STA)) {
        OAM_WARNING_LOG0(0, OAM_SF_ANY, "{hmac_blacklist_add_etc::not AP or STA mode!}\r\n");
        return OAL_SUCC;
    }

    pst_hmac_vap = (hmac_vap_stru *)mac_res_get_hmac_vap(pst_mac_vap->uc_vap_id);
    if (pst_hmac_vap == OAL_PTR_NULL) {
        OAM_ERROR_LOG0(pst_mac_vap->uc_vap_id, OAM_SF_CFG, "{hmac_blacklist_add_etc::pst_hmac_vap null.}");
        return OAL_ERR_CODE_PTR_NULL;
    }

    pst_blacklist_info = pst_hmac_vap->pst_blacklist_info;
    if (pst_blacklist_info == OAL_PTR_NULL) {
        OAM_WARNING_LOG0(pst_mac_vap->uc_vap_id, OAM_SF_CFG, "{hmac_blacklist_add_etc::pst_blacklist_info null.}");
        return OAL_ERR_CODE_PTR_NULL;
    }

    /* 3.1 找到表单 */
    for (ul_blacklist_index = 0; ul_blacklist_index < WLAN_BLACKLIST_MAX; ul_blacklist_index++) {
        pst_blacklist = &pst_blacklist_info->ast_black_list[ul_blacklist_index];
        if (!oal_memcmp(pst_blacklist->auc_mac_addr, puc_mac_addr, OAL_MAC_ADDR_LEN)) {
            break;
        }
        pst_blacklist = OAL_PTR_NULL;
    }

    /* 4.1 表单已经存在，只更新老化时间 */
    if (pst_blacklist != OAL_PTR_NULL) {
        OAM_INFO_LOG1(pst_mac_vap->uc_vap_id, OAM_SF_ANY,
                      "{blacklist_add allready exist. update aging = %d}",
                      ul_aging_time);
        pst_blacklist->ul_aging_time = ul_aging_time;
        return OAL_SUCC;
    }

    /* 5.1 找一个地址为空的表单 */
    for (ul_blacklist_index = 0; ul_blacklist_index < WLAN_BLACKLIST_MAX; ul_blacklist_index++) {
        pst_blacklist = &pst_blacklist_info->ast_black_list[ul_blacklist_index];
        if (hmac_blacklist_mac_is_zero(pst_blacklist->auc_mac_addr)) {
            break;
        }
        pst_blacklist = OAL_PTR_NULL;
    }

    /* 6.1 无可用表单 */
    if (pst_blacklist == OAL_PTR_NULL) {
        OAM_WARNING_LOG4(pst_mac_vap->uc_vap_id, OAM_SF_ANY,
                         "{hmac_blacklist_add_etc:: src mac=x.x.x.%02x.%02x.%02x. new count is %d; full return}",
                         puc_mac_addr[3], puc_mac_addr[4], puc_mac_addr[5], pst_blacklist_info->uc_list_num);
        return OAL_ERR_CODE_SECURITY_LIST_FULL;
    }

    /* 7.1 更新表单 */
    oal_time_get_stamp_us(&st_cur_time);
    if (EOK != memcpy_s(pst_blacklist->auc_mac_addr, sizeof(pst_blacklist->auc_mac_addr),
                        puc_mac_addr, OAL_MAC_ADDR_LEN)) {
        OAM_ERROR_LOG0(0, OAM_SF_ANY, "hmac_blacklist_add_etc::memcpy fail!");
        return OAL_FAIL;
    }
    pst_blacklist->ul_cfg_time = (oal_uint32)st_cur_time.i_sec;
    pst_blacklist->ul_aging_time = ul_aging_time;
    pst_blacklist->ul_drop_counter = 0;
    pst_blacklist_info->uc_list_num++;

    OAM_WARNING_LOG4(pst_mac_vap->uc_vap_id, OAM_SF_ANY,
                     "{hmac_blacklist_add_etc:: src mac=x.x.x.x.%02x.%02x. new count is %d, time=%d}",
                     puc_mac_addr[4], puc_mac_addr[5], pst_blacklist_info->uc_list_num, pst_blacklist->ul_cfg_time);

    /* 黑白名单添加成功后刷新一把用户 */
    if (pst_blacklist_info->uc_mode == CS_BLACKLIST_MODE_BLACK) {
        hmac_blacklist_vap_check_user_by_macaddr(pst_mac_vap, puc_mac_addr);
    } else if (pst_blacklist_info->uc_mode == CS_BLACKLIST_MODE_WHITE) {
        hmac_whitelist_check_all_user(pst_mac_vap);
    }

    return OAL_SUCC;
}

oal_bool_enum_uint8 hmac_blacklist_get_vap_in_device(mac_device_stru *pst_device,
                                                     oal_uint8 *puc_mac_addr,
                                                     mac_vap_stru **pst_assoc_vap)
{
    oal_bool_enum_uint8 en_assoc_vap = OAL_FALSE;
    oal_uint8 uc_vap_index;
    mac_vap_stru *pst_mac_vap = OAL_PTR_NULL;
    mac_user_stru *pst_mac_user = OAL_PTR_NULL;
    oal_dlist_head_stru *pst_entry = OAL_PTR_NULL;

    for (uc_vap_index = 0; uc_vap_index < pst_device->uc_vap_num; uc_vap_index++) {
        pst_mac_vap = (mac_vap_stru *)mac_res_get_mac_vap(pst_device->auc_vap_id[uc_vap_index]);
        if (pst_mac_vap == OAL_PTR_NULL) {
            OAM_WARNING_LOG0(0, OAM_SF_ANY, "{hmac_blacklist_get_user_in_device::mac_res_get_mac_vap failed.");
            continue;
        }
        if ((pst_mac_vap->en_vap_mode != WLAN_VAP_MODE_BSS_AP) && (pst_mac_vap->en_vap_mode != WLAN_VAP_MODE_BSS_STA)) {
            OAM_WARNING_LOG0(0, OAM_SF_ANY, "{hmac_blacklist_get_user_in_device::not AP or STA mode.");
            break;
        }

        if (en_assoc_vap == OAL_TRUE) {
            break;
        }
        /* 遍历vap下所有用户，若能找到当前user，则返回该vap */
        OAL_DLIST_SEARCH_FOR_EACH(pst_entry, &(pst_mac_vap->st_mac_user_list_head))
        {
            pst_mac_user = OAL_DLIST_GET_ENTRY(pst_entry, mac_user_stru, st_user_dlist);
            if (pst_mac_user == OAL_PTR_NULL) {
                continue;
            }

            if (!OAL_MEMCMP(pst_mac_user->auc_user_mac_addr, puc_mac_addr, WLAN_MAC_ADDR_LEN)) {
                *pst_assoc_vap = pst_mac_vap;
                en_assoc_vap = OAL_TRUE;
                break;
            }
        }
    }

    return en_assoc_vap;
}


oal_bool_enum_uint8 hmac_blacklist_get_assoc_ap(mac_vap_stru *pst_cur_vap,
                                                oal_uint8 *puc_mac_addr,
                                                mac_vap_stru **pst_assoc_vap)
{
    oal_bool_enum_uint8 en_assoc_vap = OAL_FALSE;
#if (_PRE_WLAN_FEATURE_BLACKLIST_LEVEL != _PRE_WLAN_FEATURE_BLACKLIST_VAP)
    mac_device_stru *pst_device = OAL_PTR_NULL;
#if (_PRE_WLAN_FEATURE_BLACKLIST_LEVEL != _PRE_WLAN_FEATURE_BLACKLIST_DEVICE)
    mac_chip_stru *pst_chip = OAL_PTR_NULL;
    oal_uint8 uc_device_index;
#if (_PRE_WLAN_FEATURE_BLACKLIST_LEVEL != _PRE_WLAN_FEATURE_BLACKLIST_CHIP)
    oal_uint8 uc_chip_index;
#endif
#endif
#endif

#if (_PRE_WLAN_FEATURE_BLACKLIST_LEVEL == _PRE_WLAN_FEATURE_BLACKLIST_BOARD)
    for (uc_chip_index = 0; uc_chip_index < WLAN_CHIP_MAX_NUM_PER_BOARD; uc_chip_index++) {
        if (en_assoc_vap == OAL_TRUE) {
            break;
        }
        pst_chip = &g_pst_mac_board->ast_chip[uc_chip_index];
        for (uc_device_index = 0; uc_device_index < pst_chip->uc_device_nums; uc_device_index++) {
            pst_device = mac_res_get_dev_etc(pst_chip->auc_device_id[uc_device_index]);
            en_assoc_vap = hmac_blacklist_get_vap_in_device(pst_device, puc_mac_addr, pst_assoc_vap);
            if (en_assoc_vap == OAL_TRUE) {
                break;
            }
        }
    }

#elif (_PRE_WLAN_FEATURE_BLACKLIST_LEVEL == _PRE_WLAN_FEATURE_BLACKLIST_CHIP)
    pst_chip = &g_pst_mac_board->ast_chip[pst_cur_vap->uc_chip_id];
    for (uc_device_index = 0; uc_device_index < pst_chip->uc_device_nums; uc_device_index++) {
        pst_device = mac_res_get_dev_etc(pst_chip->auc_device_id[uc_device_index]);
        if (pst_device == OAL_PTR_NULL) {
            return OAL_FALSE;
        }
        en_assoc_vap = hmac_blacklist_get_vap_in_device(pst_device, puc_mac_addr, pst_assoc_vap);
        if (en_assoc_vap == OAL_TRUE) {
            break;
        }
    }

#elif (_PRE_WLAN_FEATURE_BLACKLIST_LEVEL == _PRE_WLAN_FEATURE_BLACKLIST_DEVICE)
    /* 获取device结构体 */
    pst_device = mac_res_get_dev_etc(pst_cur_vap->uc_device_id);
    if (pst_device == OAL_PTR_NULL) {
        return OAL_FALSE;
    }
    en_assoc_vap = hmac_blacklist_get_vap_in_device(pst_device, puc_mac_addr, pst_assoc_vap);
#elif (_PRE_WLAN_FEATURE_BLACKLIST_LEVEL == _PRE_WLAN_FEATURE_BLACKLIST_VAP)
    *pst_assoc_vap = pst_cur_vap;
    en_assoc_vap = OAL_TRUE;
#endif

    return en_assoc_vap;
}

oal_uint32 hmac_blacklist_add_only_etc(mac_vap_stru *pst_mac_vap, oal_uint8 *puc_mac_addr, oal_uint32 ul_aging_time)
{
    oal_uint8 ul_blacklist_index;
    mac_blacklist_info_stru *pst_blacklist_info = OAL_PTR_NULL;
    mac_blacklist_stru *pst_blacklist = OAL_PTR_NULL;
    oal_time_us_stru st_cur_time;
    hmac_vap_stru *pst_hmac_vap = OAL_PTR_NULL;

    /* 1.1 入参检查 */
    if (pst_mac_vap == OAL_PTR_NULL) {
        OAM_ERROR_LOG0(0, OAM_SF_ANY, "{hmac_blacklist_add_only_etc::null mac_vap}");
        return OAL_ERR_CODE_PTR_NULL;
    }
    if ((pst_mac_vap->en_vap_mode != WLAN_VAP_MODE_BSS_AP) && (pst_mac_vap->en_vap_mode != WLAN_VAP_MODE_BSS_STA)) {
        OAM_WARNING_LOG0(0, OAM_SF_ANY, "{hmac_blacklist_add_only_etc::not AP or STA mode!}\r\n");
        return OAL_SUCC;
    }

    pst_hmac_vap = (hmac_vap_stru *)mac_res_get_hmac_vap(pst_mac_vap->uc_vap_id);
    if (pst_hmac_vap == OAL_PTR_NULL) {
        OAM_ERROR_LOG0(pst_mac_vap->uc_vap_id, OAM_SF_CFG, "{hmac_blacklist_add_only_etc::pst_hmac_vap null.}");
        return OAL_ERR_CODE_PTR_NULL;
    }

    pst_blacklist_info = pst_hmac_vap->pst_blacklist_info;
    if (pst_blacklist_info == OAL_PTR_NULL) {
        OAM_WARNING_LOG0(pst_mac_vap->uc_vap_id, OAM_SF_CFG, "{hmac_blacklist_add_only_etc::pst_blacklist_info null.}");
        return OAL_ERR_CODE_PTR_NULL;
    }

    /* 3.1 找到表单 */
    for (ul_blacklist_index = 0; ul_blacklist_index < WLAN_BLACKLIST_MAX; ul_blacklist_index++) {
        pst_blacklist = &pst_blacklist_info->ast_black_list[ul_blacklist_index];
        if (!oal_memcmp(pst_blacklist->auc_mac_addr, puc_mac_addr, OAL_MAC_ADDR_LEN)) {
            break;
        }
        pst_blacklist = OAL_PTR_NULL;
    }

    /* 4.1 表单已经存在，只更新老化时间 */
    if (pst_blacklist != OAL_PTR_NULL) {
        OAM_INFO_LOG1(pst_mac_vap->uc_vap_id, OAM_SF_ANY,
                      "{blacklist_add allready exist. update aging = %d}", ul_aging_time);
        pst_blacklist->ul_aging_time = ul_aging_time;
        return OAL_SUCC;
    }

    /* 5.1 找一个地址为空的表单 */
    for (ul_blacklist_index = 0; ul_blacklist_index < WLAN_BLACKLIST_MAX; ul_blacklist_index++) {
        pst_blacklist = &pst_blacklist_info->ast_black_list[ul_blacklist_index];
        if (hmac_blacklist_mac_is_zero(pst_blacklist->auc_mac_addr)) {
            break;
        }
        pst_blacklist = OAL_PTR_NULL;
    }

    /* 6.1 无可用表单 */
    if (pst_blacklist == OAL_PTR_NULL) {
        OAM_INFO_LOG4(pst_mac_vap->uc_vap_id, OAM_SF_ANY,
                      "{hmac_blacklist_add_only_etc:: src mac=x.x.x.%02x.%02x.%02x. new count is %d; full return}",
                      puc_mac_addr[3], puc_mac_addr[4], puc_mac_addr[5], pst_blacklist_info->uc_list_num);
        return OAL_ERR_CODE_SECURITY_LIST_FULL;
    }

    /* 7.1 更新表单 */
    oal_time_get_stamp_us(&st_cur_time);
    if (EOK != memcpy_s(pst_blacklist->auc_mac_addr, OAL_MAC_ADDR_LEN,
                        puc_mac_addr, OAL_MAC_ADDR_LEN)) {
        OAM_ERROR_LOG0(0, OAM_SF_ANY, "hmac_blacklist_add_only_etc::memcpy fail!");
        return OAL_FAIL;
    }
    pst_blacklist->ul_cfg_time = (oal_uint32)st_cur_time.i_sec;
    pst_blacklist->ul_aging_time = ul_aging_time;
    pst_blacklist->ul_drop_counter = 0;
    pst_blacklist_info->uc_list_num++;

    OAM_WARNING_LOG4(pst_mac_vap->uc_vap_id, OAM_SF_ANY,
                     "{hmac_blacklist_add_only_etc:: src mac=x.x.x.x.%02x.%02x. new count is %d, time=%d}",
                     puc_mac_addr[4], puc_mac_addr[5], pst_blacklist_info->uc_list_num, pst_blacklist->ul_cfg_time);

    return OAL_SUCC;
}


oal_uint32 hmac_blacklist_del_etc(mac_vap_stru *pst_mac_vap, oal_uint8 *puc_mac_addr)
{
    oal_uint8 ul_blacklist_index;
    mac_blacklist_stru *pst_blacklist = OAL_PTR_NULL;
    mac_blacklist_info_stru *pst_blacklist_info = OAL_PTR_NULL;
    hmac_vap_stru *pst_hmac_vap = OAL_PTR_NULL;
    oal_uint32 ul_ret;
    oal_uint16 us_user_idx = 0xffff;
    hmac_user_stru *pst_hmac_user_tmp = OAL_PTR_NULL;

    /* 1.1 入参检查 */
    if (pst_mac_vap == OAL_PTR_NULL) {
        OAM_ERROR_LOG0(0, OAM_SF_ANY, "{hmac_blacklist_del_etc::null mac_vap}");
        return OAL_ERR_CODE_PTR_NULL;
    }
    if ((pst_mac_vap->en_vap_mode != WLAN_VAP_MODE_BSS_AP) && (pst_mac_vap->en_vap_mode != WLAN_VAP_MODE_BSS_STA)) {
        OAM_WARNING_LOG0(pst_mac_vap->uc_vap_id, OAM_SF_ANY, "{hmac_blacklist_del_etc::not AP or STA mode.");
        return OAL_SUCC;
    }

    pst_hmac_vap = (hmac_vap_stru *)mac_res_get_hmac_vap(pst_mac_vap->uc_vap_id);
    if (pst_hmac_vap == OAL_PTR_NULL) {
        OAM_ERROR_LOG0(pst_mac_vap->uc_vap_id, OAM_SF_CFG,
                       "{hmac_blacklist_del_etc::pst_hmac_vap null.}");

        return OAL_ERR_CODE_PTR_NULL;
    }

    /* 2.1 如果模式不一致，不需要删除，返回失败即可 */

    pst_blacklist_info = pst_hmac_vap->pst_blacklist_info;
    if (pst_blacklist_info == OAL_PTR_NULL) {
        OAM_WARNING_LOG0(pst_mac_vap->uc_vap_id, OAM_SF_ANY, "{hmac_blacklist_del_etc::pst_blacklist_info null!}\r\n");
        return OAL_ERR_CODE_PTR_NULL;
    }

    /* 3.1 广播地址，需要删除所有表单 */
    if (hmac_blacklist_mac_is_bcast(puc_mac_addr)) {
        OAM_WARNING_LOG0(pst_mac_vap->uc_vap_id, OAM_SF_ANY,
                         "{hmac_blacklist_del_etc::broadcast addr; delete all hmac_blacklist}");

        hmac_blacklist_init(pst_mac_vap, pst_blacklist_info->uc_mode, OAL_TRUE);

        return OAL_SUCC;
    }

    /* 4.1 找到表单 */
    for (ul_blacklist_index = 0; ul_blacklist_index < WLAN_BLACKLIST_MAX; ul_blacklist_index++) {
        pst_blacklist = &pst_blacklist_info->ast_black_list[ul_blacklist_index];
        if (!oal_memcmp(pst_blacklist->auc_mac_addr, puc_mac_addr, OAL_MAC_ADDR_LEN)) {
            break;
        }
        pst_blacklist = OAL_PTR_NULL;
    }

    /* 5.1 如果找到表单，删除 */
    if (pst_blacklist != OAL_PTR_NULL) {
        memset_s(pst_blacklist, sizeof(mac_blacklist_stru), 0, sizeof(mac_blacklist_stru));
        pst_blacklist_info->uc_list_num--;
        OAM_WARNING_LOG4(pst_mac_vap->uc_vap_id, OAM_SF_ANY,
                         "{hmac_blacklist_del_etc:: mac=x.x.x.%02x.%02x.%02x. new count is %d}",
                         puc_mac_addr[3], puc_mac_addr[4], puc_mac_addr[5], pst_blacklist_info->uc_list_num);

        if (pst_blacklist_info->uc_mode == CS_BLACKLIST_MODE_WHITE) {
            /* 根据mac addr找到sta索引 */
            ul_ret = mac_vap_find_user_by_macaddr_etc(pst_mac_vap, puc_mac_addr, &us_user_idx);
            if (ul_ret != OAL_SUCC) {
                OAM_WARNING_LOG1(pst_mac_vap->uc_vap_id, OAM_SF_ANY,
                                 "{hmac_blacklist_del_etc::find_user_by_macaddr failed[%d].}", ul_ret);
                return ul_ret;
            }
            pst_hmac_user_tmp = mac_res_get_hmac_user_etc(us_user_idx);
            if (pst_hmac_user_tmp == OAL_PTR_NULL) {
                OAM_ERROR_LOG1(pst_mac_vap->uc_vap_id, OAM_SF_ANY,
                               "{hmac_blacklist_del_etc::pst_hmac_user null,user_idx=%d.}", us_user_idx);
                return OAL_ERR_CODE_PTR_NULL;
            }

            hmac_blacklist_update_delete_user(pst_hmac_vap, pst_hmac_user_tmp);
        }
    } else {
        OAM_WARNING_LOG4(pst_mac_vap->uc_vap_id, OAM_SF_ANY,
                         "{hmac_blacklist_del_etc:: didn't find this mac: %02X:XX:XX:%02X:%02X:%02X}",
                         puc_mac_addr[0], puc_mac_addr[3], puc_mac_addr[4], puc_mac_addr[5]);
    }
    return OAL_SUCC;
}


oal_uint32 hmac_blacklist_set_mode_etc(mac_vap_stru *pst_mac_vap, oal_uint8 uc_mode)
{
    /* 1.1 入参检查 */
    if (pst_mac_vap == OAL_PTR_NULL) {
        OAM_ERROR_LOG0(0, OAM_SF_ANY, "{hmac_blacklist_set_mode_etc::null mac_vap}");
        return OAL_ERR_CODE_PTR_NULL;
    }
    if ((pst_mac_vap->en_vap_mode != WLAN_VAP_MODE_BSS_AP) && (pst_mac_vap->en_vap_mode != WLAN_VAP_MODE_BSS_STA)) {
        OAM_WARNING_LOG0(0, OAM_SF_ANY, "{hmac_blacklist_set_mode_etc::not AP or STA mode!}\r\n");
        return OAL_SUCC;
    }

    OAM_WARNING_LOG1(pst_mac_vap->uc_vap_id, OAM_SF_ANY, "{hmac_blacklist_set_mode_etc::mode = %d}", uc_mode);
    /* 2.1 根据不同模式增加表单 */
    switch (uc_mode) {
        case CS_BLACKLIST_MODE_NONE:
        case CS_BLACKLIST_MODE_BLACK:
        case CS_BLACKLIST_MODE_WHITE:
            /* hipriv.sh debug suggest OAL_FALSE: don't flush all bssids in blacklist */
            /* FWK configuration suggest OAL_TRUE:Flush all bssids in blacklist */
            hmac_blacklist_init(pst_mac_vap, uc_mode, OAL_TRUE); /* OAL_TRUE:Flush blacklist; OAL_FALSE:don't flush */
            break;

        default:
            OAM_ERROR_LOG1(pst_mac_vap->uc_vap_id, OAM_SF_ANY, "{unknown mode = %d}", uc_mode);
            return OAL_ERR_CODE_SECURITY_MODE_INVALID;
    }

    return OAL_SUCC;
}


oal_uint32 hmac_blacklist_get_mode(mac_vap_stru *pst_mac_vap, oal_uint8 *uc_mode)
{
    hmac_vap_stru *pst_hmac_vap = OAL_PTR_NULL;

    /*  入参检查 */
    if (pst_mac_vap == OAL_PTR_NULL) {
        OAM_ERROR_LOG0(0, OAM_SF_ANY, "{hmac_blacklist_get_mode::null mac_vap}");
        return OAL_ERR_CODE_PTR_NULL;
    }
    if ((pst_mac_vap->en_vap_mode != WLAN_VAP_MODE_BSS_AP) && (pst_mac_vap->en_vap_mode != WLAN_VAP_MODE_BSS_STA)) {
        return OAL_SUCC;
    }

    pst_hmac_vap = (hmac_vap_stru *)mac_res_get_hmac_vap(pst_mac_vap->uc_vap_id);
    if (pst_hmac_vap == OAL_PTR_NULL) {
        OAM_ERROR_LOG0(pst_mac_vap->uc_vap_id, OAM_SF_CFG, "{hmac_blacklist_get_mode::pst_hmac_vap null.}");
        return OAL_ERR_CODE_PTR_NULL;
    }

    *((oal_uint32 *)uc_mode) = pst_hmac_vap->pst_blacklist_info->uc_mode;
    return OAL_SUCC;
}


OAL_STATIC oal_void hmac_show_autoblacklist_info(mac_autoblacklist_info_stru *pst_autoblacklist_info)
{
    oal_uint8 ul_blacklist_index;
    oal_uint8 *puc_sa = OAL_PTR_NULL;
    mac_autoblacklist_stru *pst_autoblacklist = OAL_PTR_NULL;
    oal_int8 *pc_print_buff;
    oal_uint8 buff_index;

    pc_print_buff = (oal_int8 *)OAL_MEM_ALLOC(OAL_MEM_POOL_ID_LOCAL, OAM_REPORT_MAX_STRING_LEN, OAL_TRUE);
    if (pc_print_buff == OAL_PTR_NULL) {
        OAM_ERROR_LOG0(0, OAM_SF_CFG, "{hmac_show_autoblacklist_info::pc_print_buff null.}");
        return;
    }
    memset_s(pc_print_buff, OAM_REPORT_MAX_STRING_LEN, 0, OAM_REPORT_MAX_STRING_LEN);

    /* 2.1 黑名单配置信息 */

    OAM_INFO_LOG4(0, OAM_SF_ANY,
        "{hmac_show_autoblacklist_info::AUTOBLACKLIST[%d] info: THRESHOLD: %u. RESET_TIME: %u sec. AGING_TIME: %u sec}",
        pst_autoblacklist_info->uc_enabled, pst_autoblacklist_info->ul_threshold,
        pst_autoblacklist_info->ul_reset_time, pst_autoblacklist_info->ul_aging_time);

    buff_index = (oal_uint8)snprintf_s(pc_print_buff, OAM_REPORT_MAX_STRING_LEN,
                                       OAM_REPORT_MAX_STRING_LEN - 1,
                                       "\nAUTOBLACKLIST[%d] info:\n"
                                       "  THRESHOLD: %u\n"
                                       "  RESET_TIME: %u sec\n"
                                       "  AGING_TIME: %u sec\n",
                                       pst_autoblacklist_info->uc_enabled,
                                       pst_autoblacklist_info->ul_threshold,
                                       pst_autoblacklist_info->ul_reset_time,
                                       pst_autoblacklist_info->ul_aging_time);

    /* 4.1 打印黑名单表单 */
    for (ul_blacklist_index = 0; ul_blacklist_index < WLAN_BLACKLIST_MAX; ul_blacklist_index++) {
        pst_autoblacklist = &pst_autoblacklist_info->ast_autoblack_list[ul_blacklist_index];
        if (hmac_blacklist_mac_is_zero(pst_autoblacklist->auc_mac_addr)) {
            continue;
        }
        puc_sa = pst_autoblacklist->auc_mac_addr;
        OAM_INFO_LOG4(0, OAM_SF_ANY,
                      "{src mac=%02X:XX:XX:%02X:%02X:%02X}", puc_sa[0], puc_sa[3], puc_sa[4], puc_sa[5]);
        OAM_INFO_LOG2(0, OAM_SF_ANY, "{ ASSO_CNT: %u. cfg_time=%d. }",
                      pst_autoblacklist->ul_asso_counter, pst_autoblacklist->ul_cfg_time);

        buff_index += (oal_uint8)snprintf_s(pc_print_buff + buff_index,
                                            (OAM_REPORT_MAX_STRING_LEN - buff_index),
                                            (OAM_REPORT_MAX_STRING_LEN - buff_index) - 1,
                                            "\n[%02d] MAC: %02X:XX:XX:%02X:%02X:%02X\n"
                                            " cfg_time=%d. ASSO_CNT: %d\n",
                                            ul_blacklist_index,
                                            pst_autoblacklist->auc_mac_addr[0],
                                            pst_autoblacklist->auc_mac_addr[3],
                                            pst_autoblacklist->auc_mac_addr[4],
                                            pst_autoblacklist->auc_mac_addr[5],
                                            pst_autoblacklist->ul_cfg_time,
                                            pst_autoblacklist->ul_asso_counter);
    }
    oam_print_etc(pc_print_buff);
    OAL_MEM_FREE(pc_print_buff, OAL_TRUE);
}


oal_void hmac_show_blacklist_info_etc(mac_vap_stru *pst_mac_vap)
{
    oal_uint8 *puc_sa = OAL_PTR_NULL;
    oal_uint8 buff_index;
    oal_uint8 ul_blacklist_index;
    mac_blacklist_stru *pst_blacklist = OAL_PTR_NULL;
    mac_blacklist_info_stru *pst_blacklist_info = OAL_PTR_NULL;
    oal_time_us_stru st_cur_time;
    oal_int8 *pc_print_buff = OAL_PTR_NULL;
    hmac_vap_stru *pst_hmac_vap = OAL_PTR_NULL;

    /* 1.1 入参检查 */
    if (pst_mac_vap == OAL_PTR_NULL) {
        OAM_ERROR_LOG0(0, OAM_SF_ANY, "{hmac_show_blacklist_info_etc::null mac_vap}");
        return;
    }
    if ((pst_mac_vap->en_vap_mode != WLAN_VAP_MODE_BSS_AP) && (pst_mac_vap->en_vap_mode != WLAN_VAP_MODE_BSS_STA)) {
        OAM_WARNING_LOG0(0, OAM_SF_ANY, "{hmac_show_blacklist_info_etc::not AP or STA mode!}\r\n");
        return;
    }

    pc_print_buff = (oal_int8 *)OAL_MEM_ALLOC(OAL_MEM_POOL_ID_LOCAL, OAM_REPORT_MAX_STRING_LEN, OAL_TRUE);
    if (pc_print_buff == OAL_PTR_NULL) {
        OAM_ERROR_LOG0(pst_mac_vap->uc_vap_id, OAM_SF_CFG, "{hmac_show_blacklist_info_etc::pc_print_buff null.}");
        return;
    }
    memset_s(pc_print_buff, OAM_REPORT_MAX_STRING_LEN, 0, OAM_REPORT_MAX_STRING_LEN);

    pst_hmac_vap = (hmac_vap_stru *)mac_res_get_hmac_vap(pst_mac_vap->uc_vap_id);
    if (pst_hmac_vap == OAL_PTR_NULL) {
        OAM_ERROR_LOG0(pst_mac_vap->uc_vap_id, OAM_SF_CFG, "{hmac_show_blacklist_info_etc::pst_hmac_vap null.}");
        OAL_MEM_FREE(pc_print_buff, OAL_TRUE);

        return;
    }

    pst_blacklist_info = pst_hmac_vap->pst_blacklist_info;
    if (pst_blacklist_info == OAL_PTR_NULL) {
        OAM_ERROR_LOG0(pst_mac_vap->uc_vap_id, OAM_SF_CFG, "{hmac_show_blacklist_info_etc::pst_blacklist_info null.}");
        return;
    }

    /* 3.1 黑名单模式信息 */
    if (pst_blacklist_info->uc_mode == CS_BLACKLIST_MODE_BLACK) {
        OAM_WARNING_LOG1(pst_mac_vap->uc_vap_id, OAM_SF_ANY,
                         "{BLACKLIST[BLACK] num=%d info:}", pst_blacklist_info->uc_list_num);
        buff_index = (oal_uint8)snprintf_s(pc_print_buff, OAM_REPORT_MAX_STRING_LEN,
                                           OAM_REPORT_MAX_STRING_LEN - 1,
                                           "BLACKLIST[BLACK] num=%d info:\n",
                                           pst_blacklist_info->uc_list_num);
    } else if (pst_blacklist_info->uc_mode == CS_BLACKLIST_MODE_WHITE) {
        OAM_WARNING_LOG1(pst_mac_vap->uc_vap_id, OAM_SF_ANY,
                         "{BLACKLIST[WHITE] num=%d info:}", pst_blacklist_info->uc_list_num);
        buff_index = (oal_uint8)snprintf_s(pc_print_buff, OAM_REPORT_MAX_STRING_LEN,
                                           OAM_REPORT_MAX_STRING_LEN - 1,
                                           "BLACKLIST[WHITE] num=%d info:\n",
                                           pst_blacklist_info->uc_list_num);
    } else {
        OAM_WARNING_LOG1(pst_mac_vap->uc_vap_id, OAM_SF_ANY,
                         "{BLACKLIST not enable! num=%d info:}", pst_blacklist_info->uc_list_num);
        buff_index = (oal_uint8)snprintf_s(pc_print_buff, OAM_REPORT_MAX_STRING_LEN,
                                           OAM_REPORT_MAX_STRING_LEN - 1,
                                           "BLACKLIST not enable! num=%d info:\n",
                                           pst_blacklist_info->uc_list_num);
    }

    /* 5.1 打印黑名单表单 */
    for (ul_blacklist_index = 0; ul_blacklist_index < WLAN_BLACKLIST_MAX; ul_blacklist_index++) {
        pst_blacklist = &pst_blacklist_info->ast_black_list[ul_blacklist_index];
        if (hmac_blacklist_mac_is_zero(pst_blacklist->auc_mac_addr)) {
            continue;
        }
        puc_sa = pst_blacklist->auc_mac_addr;
        OAM_WARNING_LOG4(pst_mac_vap->uc_vap_id, OAM_SF_ANY,
                         "{src mac=%02X:XX:XX:%02X:%02X:%02X}", puc_sa[0], puc_sa[3], puc_sa[4], puc_sa[5]);
        OAM_WARNING_LOG3(pst_mac_vap->uc_vap_id, OAM_SF_ANY, "{ CFG_TIME: %u. AGE_TIME: %u. DROP_CNT: %u.}",
                         pst_blacklist->ul_cfg_time, pst_blacklist->ul_aging_time, pst_blacklist->ul_drop_counter);

        buff_index += (oal_uint8)snprintf_s(pc_print_buff + buff_index, (OAM_REPORT_MAX_STRING_LEN - buff_index),
                                            (OAM_REPORT_MAX_STRING_LEN - buff_index) - 1,
                                            "\n[%02d] MAC: %02X:XX:XX:%02X:%02X:%02X\n"
                                            " CFG_TIME: %u\t AGE_TIME: %u\t DROP_CNT: %u\n",
                                            ul_blacklist_index,
                                            pst_blacklist->auc_mac_addr[0],
                                            pst_blacklist->auc_mac_addr[3],
                                            pst_blacklist->auc_mac_addr[4],
                                            pst_blacklist->auc_mac_addr[5],
                                            pst_blacklist->ul_cfg_time, pst_blacklist->ul_aging_time,
                                            pst_blacklist->ul_drop_counter);
    }
    oam_print_etc(pc_print_buff);
    OAL_MEM_FREE(pc_print_buff, OAL_TRUE);

    /* 4.1 自动黑名单信息 */
    hmac_show_autoblacklist_info(&pst_blacklist_info->st_autoblacklist_info);

    /* 加入当前时间 */
    oal_time_get_stamp_us(&st_cur_time);
    OAM_INFO_LOG1(pst_mac_vap->uc_vap_id, OAM_SF_ANY, "{ CURR_TIME: %d}", (oal_uint32)st_cur_time.i_sec);
}


oal_uint32 hmac_autoblacklist_enable_etc(mac_vap_stru *pst_mac_vap, oal_uint8 uc_enabled)
{
    mac_autoblacklist_info_stru *pst_autoblacklist_info = OAL_PTR_NULL;
    hmac_vap_stru *pst_hmac_vap = OAL_PTR_NULL;

    /* 1.1 入参检查 */
    if (pst_mac_vap == OAL_PTR_NULL) {
        OAM_ERROR_LOG0(0, OAM_SF_ANY, "{hmac_autoblacklist_enable_etc::null mac_vap}");
        return OAL_ERR_CODE_PTR_NULL;
    }
    if ((pst_mac_vap->en_vap_mode != WLAN_VAP_MODE_BSS_AP) && (pst_mac_vap->en_vap_mode != WLAN_VAP_MODE_BSS_STA)) {
        OAM_WARNING_LOG0(pst_mac_vap->uc_vap_id, OAM_SF_ANY,
                         "{hmac_autoblacklist_enable_etc::not AP or STA mode!}\r\n");
        return OAL_ERR_CODE_PTR_NULL;
    }

    /* 2.1 获取参数 */
    pst_hmac_vap = (hmac_vap_stru *)mac_res_get_hmac_vap(pst_mac_vap->uc_vap_id);
    if (pst_hmac_vap == OAL_PTR_NULL) {
        OAM_ERROR_LOG0(pst_mac_vap->uc_vap_id, OAM_SF_CFG, "{hmac_autoblacklist_enable_etc::pst_hmac_vap null.}");
        return OAL_ERR_CODE_PTR_NULL;
    }
    if (pst_hmac_vap->pst_blacklist_info == OAL_PTR_NULL) {
        OAM_ERROR_LOG0(pst_mac_vap->uc_vap_id, OAM_SF_CFG, "{hmac_autoblacklist_enable_etc::pst_blacklist_info null.}");
        return OAL_ERR_CODE_PTR_NULL;
    }
    pst_autoblacklist_info = &(pst_hmac_vap->pst_blacklist_info->st_autoblacklist_info);

    /* 参数跟原有一样 */
    if (uc_enabled == pst_autoblacklist_info->uc_enabled) {
        OAM_WARNING_LOG1(pst_mac_vap->uc_vap_id, OAM_SF_ANY,
                         "{hmac_autoblacklist_enable_etc:autoblacklist is already %d}", uc_enabled);

        return OAL_SUCC;
    }

    /* 重新初始化 */
    if (uc_enabled == 1) {
        hmac_blacklist_init(pst_mac_vap, CS_BLACKLIST_MODE_BLACK, OAL_TRUE);
        pst_autoblacklist_info->uc_enabled = uc_enabled;
        pst_autoblacklist_info->ul_aging_time = CS_DEFAULT_AGING_TIME;
        pst_autoblacklist_info->ul_reset_time = CS_DEFAULT_RESET_TIME;
        pst_autoblacklist_info->ul_threshold = CS_DEFAULT_THRESHOLD;
    } else {                                                                /* 关闭自动黑名单，清零相关数据 */
        hmac_blacklist_init(pst_mac_vap, CS_BLACKLIST_MODE_NONE, OAL_TRUE); /* 2013.7.23 add clean */
    }

    OAM_INFO_LOG1(pst_mac_vap->uc_vap_id, OAM_SF_ANY,
                  "{hmac_autoblacklist_enable_etc:autoblacklist is %d}", uc_enabled);

    return OAL_SUCC;
}


oal_uint32 hmac_autoblacklist_set_aging_etc(mac_vap_stru *pst_mac_vap, oal_uint32 ul_aging_time)
{
    mac_autoblacklist_info_stru *pst_autoblacklist_info = OAL_PTR_NULL;
    hmac_vap_stru *pst_hmac_vap = OAL_PTR_NULL;

    /* 1.1 入参检查 */
    if (pst_mac_vap == OAL_PTR_NULL) {
        OAM_ERROR_LOG0(0, OAM_SF_ANY, "{hmac_autoblacklist_set_aging_etc::null mac_vap}");
        return OAL_ERR_CODE_PTR_NULL;
    }

    /* 2.1 获取参数 */
    if (ul_aging_time == 0) {
        OAM_ERROR_LOG0(pst_mac_vap->uc_vap_id, OAM_SF_ANY, "{ul_aging_time should not be 0}");
        return OAL_ERR_CODE_SECURITY_AGING_INVALID;
    }

    pst_hmac_vap = (hmac_vap_stru *)mac_res_get_hmac_vap(pst_mac_vap->uc_vap_id);
    if (pst_hmac_vap == OAL_PTR_NULL) {
        OAM_ERROR_LOG0(pst_mac_vap->uc_vap_id, OAM_SF_CFG, "{hmac_autoblacklist_set_aging_etc::pst_hmac_vap null.}");
        return OAL_ERR_CODE_PTR_NULL;
    }
    if (pst_hmac_vap->pst_blacklist_info == OAL_PTR_NULL) {
        OAM_ERROR_LOG0(pst_mac_vap->uc_vap_id, OAM_SF_CFG,
                       "{hmac_autoblacklist_set_aging_etc::pst_blacklist_info null.}");
        return OAL_ERR_CODE_PTR_NULL;
    }
    pst_autoblacklist_info = &(pst_hmac_vap->pst_blacklist_info->st_autoblacklist_info);

    /* 3.1 自动黑名单没有使能 */
    if (pst_autoblacklist_info->uc_enabled == 0) {
        hmac_autoblacklist_enable_etc(pst_mac_vap, 1);
    }

    /* 4.1 更新老化时间 */
    pst_autoblacklist_info->ul_aging_time = ul_aging_time;

    OAM_INFO_LOG1(pst_mac_vap->uc_vap_id, OAM_SF_ANY, "{blacklist set auto aging = %d}", ul_aging_time);
    return OAL_SUCC;
}


oal_uint32 hmac_autoblacklist_set_threshold_etc(mac_vap_stru *pst_mac_vap, oal_uint32 ul_threshold)
{
    mac_autoblacklist_info_stru *pst_autoblacklist_info = OAL_PTR_NULL;
    hmac_vap_stru *pst_hmac_vap = OAL_PTR_NULL;

    /* 1.1 入参检查 */
    if (pst_mac_vap == OAL_PTR_NULL) {
        OAM_ERROR_LOG0(0, OAM_SF_ANY, "{hmac_autoblacklist_set_threshold_etc::null mac_vap}");
        return OAL_ERR_CODE_PTR_NULL;
    }

    /* 2.1 获取参数 */
    if (ul_threshold == 0) {
        OAM_ERROR_LOG0(pst_mac_vap->uc_vap_id, OAM_SF_ANY, "{ul_threshold should not be 0}");
        return OAL_ERR_CODE_SECURITY_THRESHOLD_INVALID;
    }

    pst_hmac_vap = (hmac_vap_stru *)mac_res_get_hmac_vap(pst_mac_vap->uc_vap_id);
    if (pst_hmac_vap == OAL_PTR_NULL) {
        OAM_ERROR_LOG0(pst_mac_vap->uc_vap_id, OAM_SF_CFG,
                       "{hmac_autoblacklist_set_threshold_etc::pst_hmac_vap null.}");
        return OAL_ERR_CODE_PTR_NULL;
    }
    if (pst_hmac_vap->pst_blacklist_info == OAL_PTR_NULL) {
        OAM_ERROR_LOG0(pst_mac_vap->uc_vap_id, OAM_SF_CFG,
                       "{hmac_autoblacklist_set_threshold_etc::pst_blacklist_info null.}");
        return OAL_ERR_CODE_PTR_NULL;
    }
    pst_autoblacklist_info = &(pst_hmac_vap->pst_blacklist_info->st_autoblacklist_info);

    /* 3.1 自动黑名单没有使能 */
    if (pst_autoblacklist_info->uc_enabled == 0) {
        hmac_autoblacklist_enable_etc(pst_mac_vap, 1);
    }

    /* 4.1 更新门限 */
    pst_autoblacklist_info->ul_threshold = ul_threshold;

    OAM_INFO_LOG1(pst_mac_vap->uc_vap_id, OAM_SF_ANY, "{blacklist set auto threshold = %d}", ul_threshold);
    return OAL_SUCC;
}


oal_uint32 hmac_autoblacklist_set_reset_time_etc(mac_vap_stru *pst_mac_vap, oal_uint32 ul_reset_time)
{
    mac_autoblacklist_info_stru *pst_autoblacklist_info = OAL_PTR_NULL;
    hmac_vap_stru *pst_hmac_vap = OAL_PTR_NULL;

    /* 1.1 入参检查 */
    if (pst_mac_vap == OAL_PTR_NULL) {
        OAM_ERROR_LOG0(0, OAM_SF_ANY, "{hmac_autoblacklist_set_reset_time_etc::null mac_vap}");
        return OAL_ERR_CODE_PTR_NULL;
    }

    /* 2.1 获取参数 */
    if (ul_reset_time == 0) {
        OAM_ERROR_LOG0(pst_mac_vap->uc_vap_id, OAM_SF_ANY, "{ul_aging_time should not be 0}");
        return OAL_ERR_CODE_SECURITY_RESETIME_INVALID;
    }

    pst_hmac_vap = (hmac_vap_stru *)mac_res_get_hmac_vap(pst_mac_vap->uc_vap_id);
    if (pst_hmac_vap == OAL_PTR_NULL) {
        OAM_ERROR_LOG0(pst_mac_vap->uc_vap_id, OAM_SF_CFG,
                       "{hmac_autoblacklist_set_reset_time_etc::pst_hmac_vap null.}");
        return OAL_ERR_CODE_PTR_NULL;
    }
    if (pst_hmac_vap->pst_blacklist_info == OAL_PTR_NULL) {
        OAM_ERROR_LOG0(pst_mac_vap->uc_vap_id, OAM_SF_CFG,
                       "{hmac_autoblacklist_set_reset_time_etc::pst_blacklist_info null.}");
        return OAL_ERR_CODE_PTR_NULL;
    }
    pst_autoblacklist_info = &(pst_hmac_vap->pst_blacklist_info->st_autoblacklist_info);

    /* 3.1 自动黑名单没有使能 */
    if (pst_autoblacklist_info->uc_enabled == 0) {
        hmac_autoblacklist_enable_etc(pst_mac_vap, 1);
    }

    /* 4.1 更新重置时间 */
    pst_autoblacklist_info->ul_reset_time = ul_reset_time;

    OAM_INFO_LOG1(pst_mac_vap->uc_vap_id, OAM_SF_ANY, "{blacklist set auto reset_time = %d}", ul_reset_time);
    return OAL_SUCC;
}


oal_bool_enum_uint8 hmac_blacklist_filter_etc(mac_vap_stru *pst_mac_vap, oal_uint8 *puc_mac_addr)
{
    mac_blacklist_stru *pst_blacklist = OAL_PTR_NULL;
    hmac_vap_stru *pst_hmac_vap = OAL_PTR_NULL;
    mac_blacklist_info_stru *pst_blacklist_info = OAL_PTR_NULL;
    oal_uint32 ul_ret;
    oal_bool_enum_uint8 b_ret;

    /* 1.1 入参检查 */
    if (OAL_ANY_NULL_PTR2(pst_mac_vap, puc_mac_addr)) {
        OAM_ERROR_LOG0(0, OAM_SF_ANY, "{hmac_blacklist_filter_etc::null mac_vap or null mac addr}");
        return OAL_FALSE;
    }

    pst_hmac_vap = (hmac_vap_stru *)mac_res_get_hmac_vap(pst_mac_vap->uc_vap_id);
    if (pst_hmac_vap == OAL_PTR_NULL) {
        OAM_ERROR_LOG0(pst_mac_vap->uc_vap_id, OAM_SF_CFG,
                       "{hmac_blacklist_filter_etc::pst_hmac_vap null.}");
        return OAL_ERR_CODE_PTR_NULL;
    }
    if (pst_hmac_vap->pst_blacklist_info == OAL_PTR_NULL) {
        OAM_ERROR_LOG0(pst_mac_vap->uc_vap_id, OAM_SF_CFG, "{hmac_blacklist_filter_etc::pst_blacklist_info null.}");
        return OAL_ERR_CODE_PTR_NULL;
    }
    pst_blacklist_info = pst_hmac_vap->pst_blacklist_info;

    /* 1.1 功能没有开启，不需要过滤 */
    if (pst_blacklist_info->uc_mode == CS_BLACKLIST_MODE_NONE) {
        return OAL_FALSE;
    }

    /* 2.1 黑名单模式下 */
    if (pst_blacklist_info->uc_mode == CS_BLACKLIST_MODE_BLACK) {
        ul_ret = hmac_blacklist_get(pst_mac_vap, puc_mac_addr, &pst_blacklist);
        /* 表示puc_mac_addr地址为广播地址或者全零地址, 不过滤 */
        if (ul_ret != OAL_SUCC) {
            OAM_WARNING_LOG3(pst_mac_vap->uc_vap_id, OAM_SF_ANY,
                             "{hmac_blacklist_filter_etc::there is no MAC=x.x.x.%02x.%02x.%02x in blacklist}",
                             puc_mac_addr[3], puc_mac_addr[4], puc_mac_addr[5]);
            return OAL_FALSE;
        }
        /* 2.2 如果找不到表单，不需要过滤 */
        if (pst_blacklist == OAL_PTR_NULL) {
            return OAL_FALSE;
        }

        /* 2.3 老化时间到了，不需要过滤 */
        b_ret = hmac_blacklist_is_aged(pst_mac_vap, pst_blacklist);
        if (b_ret == OAL_TRUE) {
            return OAL_FALSE;
        }
        /* 2.4 其他情况下都需要过滤 */
        pst_blacklist->ul_drop_counter++;
        OAM_INFO_LOG4(pst_mac_vap->uc_vap_id, OAM_SF_ANY,
                      "{hmac_blacklist_filter_etc::blacklist_filter drop_counter = %d MAC:=x.x.x.%02x.%02x.%02x}",
                      pst_blacklist->ul_drop_counter, puc_mac_addr[3], puc_mac_addr[4], puc_mac_addr[5]);

        return OAL_TRUE;
    }

    /* 3.1 白名单模式下 */
    if (pst_blacklist_info->uc_mode == CS_BLACKLIST_MODE_WHITE) {
        ul_ret = hmac_blacklist_get(pst_mac_vap, puc_mac_addr, &pst_blacklist);
        /* 表示puc_mac_addr地址为广播地址或者全零地址, 不过滤 */
        if (ul_ret != OAL_SUCC) {
            return OAL_FALSE;
        }
        /* 3.2 如果找不到表单，需要过滤 */
        if (pst_blacklist == OAL_PTR_NULL) {
            OAM_WARNING_LOG4(pst_mac_vap->uc_vap_id, OAM_SF_ANY,
                             "{hmac_blacklist_filter_etc::whitelist_filter MAC:=%02X:XX:XX:%02X:%02X:%02X}",
                             puc_mac_addr[0], puc_mac_addr[3], puc_mac_addr[4], puc_mac_addr[5]);
            return OAL_TRUE;
        }

        /* 3.3 老化时间到了，不需要过滤 */
        b_ret = hmac_blacklist_is_aged(pst_mac_vap, pst_blacklist);
        if (b_ret == OAL_TRUE) {
            OAM_INFO_LOG4(pst_mac_vap->uc_vap_id, OAM_SF_ANY,
                          "{hmac_blacklist_filter_etc::aging reach. go through=%02X:XX:XX:%02X:%02X:%02X}",
                          puc_mac_addr[0], puc_mac_addr[3], puc_mac_addr[4], puc_mac_addr[5]);
            return OAL_FALSE;
        }

        /* 3.4 老化时间没有到，需要过滤 */
        if (pst_blacklist->ul_aging_time != 0) {
            OAM_INFO_LOG4(pst_mac_vap->uc_vap_id, OAM_SF_ANY,
                          "{hmac_blacklist_filter_etc::aging not zero; whilte_filter MAC=%02X:XX:XX:%02X:%02X:%02X}",
                          puc_mac_addr[0], puc_mac_addr[3], puc_mac_addr[4], puc_mac_addr[5]);

            return OAL_TRUE;
        }
    }

    /* 不满足黑白名单过滤条件的，均不需要过滤 */
    return OAL_FALSE;
}


oal_void hmac_autoblacklist_filter_etc(mac_vap_stru *pst_mac_vap, oal_uint8 *puc_mac_addr)
{
    mac_autoblacklist_stru *pst_autoblacklist = OAL_PTR_NULL; /* 添加初始化值，便于做UT打桩后此值是null */
    mac_autoblacklist_info_stru *pst_autoblacklist_info = OAL_PTR_NULL;
    oal_uint32 ul_ret;
    hmac_vap_stru *pst_hmac_vap = OAL_PTR_NULL;
    mac_blacklist_stru *pst_blacklist = OAL_PTR_NULL;

    /* 1.1 入参检查 */
    if (OAL_ANY_NULL_PTR2(pst_mac_vap, puc_mac_addr)) {
        OAM_ERROR_LOG0(0, OAM_SF_ANY, "{hmac_autoblacklist_filter_etc::null mac_vap or null mac addr}");
        return;
    }
    if ((pst_mac_vap->en_vap_mode != WLAN_VAP_MODE_BSS_AP) && (pst_mac_vap->en_vap_mode != WLAN_VAP_MODE_BSS_STA)) {
        OAM_WARNING_LOG0(0, OAM_SF_ANY, "{hmac_autoblacklist_filter_etc::not AP or STA mode!}\r\n");
        return;
    }

    pst_hmac_vap = (hmac_vap_stru *)mac_res_get_hmac_vap(pst_mac_vap->uc_vap_id);
    if (pst_hmac_vap == OAL_PTR_NULL) {
        OAM_ERROR_LOG0(pst_mac_vap->uc_vap_id, OAM_SF_CFG, "{hmac_autoblacklist_filter_etc::pst_hmac_vap null.}");
        return;
    }
    if (pst_hmac_vap->pst_blacklist_info == OAL_PTR_NULL) {
        OAM_ERROR_LOG0(pst_mac_vap->uc_vap_id, OAM_SF_CFG, "{hmac_autoblacklist_filter_etc::pst_blacklist_info null.}");
        return;
    }
    pst_autoblacklist_info = &(pst_hmac_vap->pst_blacklist_info->st_autoblacklist_info);

    /* 1.1 功能没有开启 */
    if (pst_autoblacklist_info->uc_enabled == 0) {
        return;
    }

    /* 1.2 检查是否已经在黑名单中, 若在黑名单中，直接返回 */
    hmac_blacklist_get(pst_mac_vap, puc_mac_addr, &pst_blacklist);
    if (pst_blacklist != OAL_PTR_NULL) {
        return;
    }

    OAM_INFO_LOG4(pst_mac_vap->uc_vap_id, OAM_SF_ANY,
                  "{hmac_autoblacklist_filter_etc:: MAC:=%02X:XX:XX:%02X:%02X:%02X}",
                  puc_mac_addr[0], puc_mac_addr[3], puc_mac_addr[4], puc_mac_addr[5]);

    /* 2.1 找到关联请求统计表单  */
    ul_ret = hmac_autoblacklist_get(pst_mac_vap, puc_mac_addr, &pst_autoblacklist);
    if (ul_ret != OAL_SUCC) {
        OAM_ERROR_LOG1(pst_mac_vap->uc_vap_id, OAM_SF_ANY, "{hmac_autoblacklist_get failed %d}", ul_ret);
        return;
    }
    /* 2.2 如果找不到表单，不处理 */
    if (pst_autoblacklist == OAL_PTR_NULL) {
        OAM_ERROR_LOG0(pst_mac_vap->uc_vap_id, OAM_SF_ANY, "{Can't find item to add}");
        return;
    }

    /* 3.1 关联计数器判断 */
    if (++pst_autoblacklist->ul_asso_counter > pst_autoblacklist_info->ul_threshold) {
        OAM_INFO_LOG2(pst_mac_vap->uc_vap_id, OAM_SF_ANY,
                      "{hmac_autoblacklist_filter_etc: asso_counter=%d. threshold=%d. add to blacklist}",
                      pst_autoblacklist->ul_asso_counter, pst_autoblacklist_info->ul_threshold);
        ul_ret = hmac_blacklist_add_etc(pst_mac_vap, puc_mac_addr, pst_autoblacklist_info->ul_aging_time);
        if (ul_ret != OAL_SUCC) {
            OAM_ERROR_LOG1(pst_mac_vap->uc_vap_id, OAM_SF_ANY, "{hmac_blacklist_add_etc failed %d}", ul_ret);
            return;
        }
        /* 3.2 删除统计表单 */
        memset_s(pst_autoblacklist, OAL_SIZEOF(mac_autoblacklist_stru), 0, OAL_SIZEOF(mac_autoblacklist_stru));
        if (pst_autoblacklist_info->list_num > 0) {
            pst_autoblacklist_info->list_num--;
        }
    }

    return;
}

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif

#endif /* #if (_PRE_WLAN_FEATURE_BLACKLIST_LEVEL != _PRE_WLAN_FEATURE_BLACKLIST_NONE) */
