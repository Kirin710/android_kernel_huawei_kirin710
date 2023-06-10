

#ifndef __WAL_CONFIG_H__
#define __WAL_CONFIG_H__

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif


/*****************************************************************************
  1 ����ͷ�ļ�����
*****************************************************************************/
#include "oal_ext_if.h"
#include "wlan_types.h"
#include "mac_vap.h"
#include "frw_ext_if.h"
#include "securec.h"
#include "securectype.h"

#undef  THIS_FILE_ID
#define THIS_FILE_ID OAM_FILE_ID_WAL_CONFIG_H
/*****************************************************************************
  2 �궨��
*****************************************************************************/
typedef oal_uint32  (*wal_config_get_func)(mac_vap_stru *pst_mac_vap, oal_uint16 *pus_len, oal_uint8 *puc_param);
typedef oal_uint32  (*wal_config_set_func)(mac_vap_stru *pst_mac_vap, oal_uint16 us_len, oal_uint8 *puc_param);

#define WAL_MSG_REQ_RESP_MEM_FREE(_st_msg_request)     \
    do                                                 \
    {                                                  \
        if (NULL != (_st_msg_request).pst_resp_mem) {  \
            oal_free((_st_msg_request).pst_resp_mem);  \
        }                                              \
    } while(0)

/* ��д������Ϣͷ */
#define WAL_CFG_MSG_HDR_INIT(_pst_cfg_msg_hdr, _en_type, _us_len, _uc_sn) \
    do                                                      \
    {                                                       \
        (_pst_cfg_msg_hdr)->en_msg_type = (_en_type);       \
        (_pst_cfg_msg_hdr)->us_msg_len  = (_us_len);        \
        (_pst_cfg_msg_hdr)->uc_msg_sn   = (_uc_sn);         \
    } while(0)

/* ��дwrite msg��Ϣͷ */
#define WAL_WRITE_MSG_HDR_INIT(_pst_write_msg, _en_wid, _us_len) \
    do                                                      \
    {                                                       \
        (_pst_write_msg)->en_wid = (_en_wid);               \
        (_pst_write_msg)->us_len = (_us_len);               \
    } while(0)

#define WAL_RECV_CMD_NEED_RESP(_pst_msg, _us_need_response)          \
    do {                                                             \
        if ((_pst_msg)->st_msg_hdr.en_msg_type == WAL_MSG_TYPE_QUERY) { \
            /* need response */                                      \
            (_us_need_response) = OAL_TRUE;                             \
        }                                                            \
    } while (0)

/* ��ȡmsg���кź� */
extern oal_atomic g_wal_config_seq_num_etc;
#define WAL_GET_MSG_SN()     (oal_atomic_inc_return(&g_wal_config_seq_num_etc))

#define WAL_MSG_WRITE_MSG_HDR_LENGTH   (OAL_SIZEOF(wal_msg_hdr_stru))

#define WAL_MSG_WRITE_MAX_LEN   (WLAN_MEM_EVENT_SIZE2 - FRW_EVENT_HDR_LEN - FRW_IPC_MSG_HEADER_LENGTH - WAL_MSG_WRITE_MSG_HDR_LENGTH)

#define WAL_ALWAYS_TX_PACK_LEN  (4000 - 28) /* 1024 */
#define WAL_BCAST_MAC_ADDR       255
#define WAL_MAX_RATE_NUM        16


/*****************************************************************************
  3 ö�ٶ���
*****************************************************************************/
/* ������Ϣ���� */
typedef enum
{
    WAL_MSG_TYPE_QUERY,     /* ��ѯ */
    WAL_MSG_TYPE_WRITE,     /* ���� */
    WAL_MSG_TYPE_RESPONSE,  /* ���� */

    WAL_MSG_TYPE_BUTT
}wal_msg_type_enum;
typedef oal_uint8 wal_msg_type_enum_uint8;


/* ���ʼ����࣬����ʹ�� */
typedef enum
{
    WAL_RF_TEST_11B_LEGACY_RATES,
    WAL_RF_TEST_20M_NORMAL_RATES,
    WAL_RF_TEST_20M_SHORT_GI_RATES,
    WAL_RF_TEST_40M_NORMAL_RATES,
    WAL_RF_TEST_40M_SHORT_GI_RATES,

    WAL_RF_TEST_RATES_BUTT
} wal_rf_test_enum;
/*****************************************************************************
  4 ȫ�ֱ�������
*****************************************************************************/


/*****************************************************************************
  5 ��Ϣͷ����
*****************************************************************************/


/*****************************************************************************
  6 ��Ϣ����
*****************************************************************************/


/*****************************************************************************
  7 STRUCT����
*****************************************************************************/
/* ������Ϣͷ */
typedef struct
{
    wal_msg_type_enum_uint8     en_msg_type;       /* msg type:W or Q */
    oal_uint8                   uc_msg_sn;         /* msg ���к� */
    oal_uint16                  us_msg_len;        /* msg ���� */
}wal_msg_hdr_stru;

/* ������Ϣ */
typedef struct
{
    wal_msg_hdr_stru            st_msg_hdr;         /* ������Ϣͷ */
    oal_uint8                   auc_msg_data[4];    /* ������Ϣpayload */
}wal_msg_stru;

typedef struct
{
    mac_vap_stru                *pst_mac_vap;
    oal_int8                     pc_param[4];      /* ��ѯ��������Ϣ */
}wal_event_stru;

/* write��Ϣ��ʽ */
typedef struct
{
    wlan_cfgid_enum_uint16  en_wid;
    oal_uint16              us_len;
    oal_uint8               auc_value[WAL_MSG_WRITE_MAX_LEN];
}wal_msg_write_stru;

/* write��Ϣʱ�ķ�����Ϣ */
typedef struct
{
    wlan_cfgid_enum_uint16  en_wid;
    oal_uint8               auc_resv[2];
    oal_uint32              ul_err_code;    /* write��Ϣ���صĴ����� */
}wal_msg_write_rsp_stru;

/* response��Ϣ��ʽ����Write��Ϣ��ʽ��ͬ */
typedef wal_msg_write_stru wal_msg_rsp_stru;

/* query��Ϣ��ʽ:2�ֽ�WID x N */
typedef struct
{
    wlan_cfgid_enum_uint16  en_wid;
}wal_msg_query_stru;

/* WID��Ӧ�Ĳ��� */
typedef struct
{
    wlan_cfgid_enum_uint16  en_cfgid;                /* widö�� */
    oal_bool_enum_uint8     en_reset;              /* �Ƿ�λ */
    oal_uint8               auc_resv[1];
    wal_config_get_func     p_get_func;            /* get���� */
    wal_config_set_func     p_set_func;            /* set���� */
}wal_wid_op_stru;


/* WMM SET��Ϣ��ʽ */
typedef struct
{
    wlan_cfgid_enum_uint16      en_cfg_id;
    oal_uint8                   uc_resv[2];
    oal_uint32                  ul_ac;
    oal_uint32                  ul_value;
}wal_msg_wmm_stru;

/* WMM query��Ϣ��ʽ:2�ֽ�WID x N */
typedef struct
{
    wlan_cfgid_enum_uint16  en_wid;
    oal_uint8               uc_resv[2];
    oal_uint32              ul_ac;
}wal_msg_wmm_query_stru;

/*WID request struct*/
typedef struct
{
    oal_dlist_head_stru pst_entry;
    oal_ulong      ul_request_address;
    oal_void       *pst_resp_mem;
    oal_uint32     ul_resp_len;
    oal_uint32     ul_ret;
}wal_msg_request_stru;

typedef struct
{
    oal_ulong  ul_request_address;
}wal_msg_rep_hdr;

#define DECLARE_WAL_MSG_REQ_STRU(name)  wal_msg_request_stru name;
#define WAL_MSG_REQ_STRU_INIT(name) do{memset_s((oal_void*)(&name), OAL_SIZEOF(name), 0, OAL_SIZEOF(name));name.ul_request_address = (oal_ulong)(uintptr_t)&name;}while(0)

typedef struct
{
    oal_dlist_head_stru st_head;
    oal_spin_lock_stru  st_lock;
    oal_wait_queue_head_stru  st_wait_queue;
    oal_uint32          count;
}wal_msg_queue;



/*****************************************************************************
  8 UNION����
*****************************************************************************/


/*****************************************************************************
  9 �궨��
*****************************************************************************/
#define WAL_MSG_HDR_LENGTH              OAL_SIZEOF(wal_msg_hdr_stru)
#define WAL_MSG_WID_LENGTH              OAL_SIZEOF(wlan_cfgid_enum_uint16)

/*****************************************************************************
  10 ��������
*****************************************************************************/
extern oal_uint32  wal_config_process_pkt_etc(frw_event_mem_stru *pst_event_mem);
#ifdef _PRE_SUPPORT_ACS
extern oal_uint32  wal_acs_netlink_recv_handle(frw_event_mem_stru *pst_event_mem);
#endif
extern oal_uint32  wal_config_get_wmm_params_etc(oal_net_device_stru *pst_net_dev, oal_uint8 *puc_param);

#if defined(_PRE_WLAN_FEATURE_MCAST) || defined(_PRE_WLAN_FEATURE_HERA_MCAST)
extern oal_int32  wal_config_get_snoop_table(oal_net_device_stru *pst_net_dev, oal_net_dev_ioctl_data_stru *pst_ioctl_data);
#endif
#ifdef _PRE_WLAN_FEATURE_WDS
extern oal_uint32  wal_config_get_wds_vap_info(oal_net_device_stru *pst_net_dev, oal_net_dev_ioctl_data_stru *pst_ioctl_data);
#endif
extern oal_int32   wal_recv_config_cmd_etc(oal_uint8 *puc_buf, oal_uint16 us_len);
extern oal_uint32  wal_config_get_assoc_req_ie_etc(mac_vap_stru *pst_mac_vap, oal_uint16 *pus_len, oal_uint8 *puc_param);
extern oal_uint32 wal_config_get_sta_11h_abillty(mac_vap_stru *pst_mac_vap, oal_uint16 us_len, oal_uint8 *puc_param);
#ifdef _PRE_WLAN_FEATURE_11R_AP
extern oal_uint32 wal_config_get_sta_11r_abillty(mac_vap_stru *pst_mac_vap, oal_uint16 us_len, oal_uint8 *puc_param);
#endif
extern oal_void    wal_drv_cfg_func_hook_init_etc(oal_void);
extern oal_uint32  hmac_config_send_event_etc(
                mac_vap_stru                     *pst_mac_vap,
                wlan_cfgid_enum_uint16            en_cfg_id,
                oal_uint16                        us_len,
                oal_uint8                        *puc_param);
extern oal_void wal_cfg_msg_task_sched_etc(oal_void);
extern oal_int32  wal_set_msg_response_by_addr_etc(oal_ulong addr,oal_void * pst_resp_mem ,oal_uint32 ul_resp_ret,
                                                 oal_uint32 uc_rsp_len);
extern oal_void wal_msg_request_add_queue_etc(wal_msg_request_stru* pst_msg);
extern oal_void wal_msg_request_remove_queue_etc(wal_msg_request_stru* pst_msg);
extern oal_void wal_msg_queue_init_etc(oal_void);
extern oal_uint32 wal_get_request_msg_count_etc(oal_void);

extern oal_uint32 wal_send_cali_matrix_data(oal_net_device_stru *pst_net_dev);
extern oal_uint32 wal_send_cali_data_etc(oal_net_device_stru *pst_net_dev);
#ifdef _PRE_WLAN_ONLINE_DPD
extern oal_uint32 wal_dpd_report2sdt(frw_event_mem_stru *pst_event_mem);
#endif
extern oal_uint32 wal_check_and_release_msg_resp_etc(wal_msg_stru   *pst_rsp_msg);
#if defined(_PRE_WLAN_FEATURE_DATA_SAMPLE) || defined(_PRE_WLAN_FEATURE_PSD_ANALYSIS)
extern oal_uint32 wal_sample_report2sdt(frw_event_mem_stru *pst_event_mem);
#endif

#ifdef _PRE_WLAN_RF_AUTOCALI
extern oal_uint32 wal_autocali_report2sdt(frw_event_mem_stru *pst_event_mem);
#endif
#ifdef _PRE_WLAN_FEATURE_DFS
extern oal_uint32  wal_config_get_dfs_chn_status(oal_net_device_stru *pst_net_dev, oal_uint8 *puc_param);
#endif

extern oal_uint32  wal_config_fem_lp_flag(mac_vap_stru *pst_mac_vap, oal_uint16 us_len, oal_uint8 *puc_param);
extern oal_uint32  wal_config_softap_mimo_mode(mac_vap_stru *pst_mac_vap, oal_uint16 us_len, oal_uint8 *puc_param);
#ifdef _PRE_WLAN_FEATURE_IP_FILTER
extern oal_uint32  wal_config_assigned_filter(mac_vap_stru *pst_mac_vap, oal_uint16 us_len, oal_uint8 *puc_param);
#endif

#ifdef __cplusplus
    #if __cplusplus
        }
    #endif
#endif

#endif /* end of wal_config.h */