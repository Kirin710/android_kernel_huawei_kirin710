
#ifdef _PRE_WLAN_FEATURE_DAQ

/* 1 ͷ�ļ����� */
#include "wlan_spec.h"
#include "mac_resource.h"
#include "mac_device.h"
#include "hmac_data_acq.h"
#include "hmac_config.h"
#include "hmac_vap.h"
#include "hmac_main.h"

#undef THIS_FILE_ID
#define THIS_FILE_ID OAM_FILE_ID_HMAC_DATA_ACQ_C

/* 2 ȫ�ֱ������� */
oal_uint8 g_uc_data_acq_used = OAL_FALSE;

/* 3 ����ʵ�� */

oal_void hmac_data_acq_init(oal_void)
{
    g_uc_data_acq_used = OAL_FALSE;
}


oal_void hmac_data_acq_exit(oal_void)
{
    g_uc_data_acq_used = OAL_FALSE;
}


oal_void hmac_data_acq_down_vap(mac_vap_stru *pst_mac_vap)
{
    oal_uint32                   ul_ret;
    mac_device_stru             *pst_device;
    oal_uint8                    uc_vap_idx;
    mac_cfg_down_vap_param_stru  st_down_vap;
    hmac_vap_stru               *pst_hmac_vap;

    pst_device = mac_res_get_dev(pst_mac_vap->uc_device_id);
    if (pst_device == OAL_PTR_NULL) {
        OAM_WARNING_LOG0(pst_mac_vap->uc_vap_id, OAM_SF_ANY, "{hmac_data_acq_down_vap::pst_device null.}");
        return;
    }

    /* ����device������vap������vap �µ��ŵ��� */
    for (uc_vap_idx = 0; uc_vap_idx < pst_device->uc_vap_num; uc_vap_idx++) {
        pst_hmac_vap = mac_res_get_hmac_vap(pst_device->auc_vap_id[uc_vap_idx]);
        if (pst_hmac_vap == OAL_PTR_NULL) {
            OAM_WARNING_LOG0(pst_mac_vap->uc_vap_id, OAM_SF_ANY, "{hmac_data_acq_down_vap::pst_hmac_vap null.}");
            continue;
        }

        st_down_vap.pst_net_dev = pst_hmac_vap->pst_net_device;

        ul_ret = hmac_config_down_vap(&pst_hmac_vap->st_vap_base_info,
                                      OAL_SIZEOF(mac_cfg_down_vap_param_stru), (oal_uint8 *)&st_down_vap);
        if (OAL_UNLIKELY(ul_ret != OAL_SUCC)) {
            OAM_WARNING_LOG0(pst_mac_vap->uc_vap_id, OAM_SF_ANY,
                             "{hmac_data_acq_down_vap::hmac_config_down_vap failed.}");
        }
    }

    g_uc_data_acq_used = OAL_TRUE;
}

#endif /* end of _PRE_WLAN_FEATURE_DAQ */
