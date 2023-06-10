

#ifndef __HMAC_DBAC_H__
#define __HMAC_DBAC_H__

/* 1 ����ͷ�ļ����� */
#include "hmac_vap.h"

#undef THIS_FILE_ID
#define THIS_FILE_ID OAM_FILE_ID_HMAC_DBAC_H
#ifdef _PRE_WLAN_FEATURE_DBAC
/* 2 �궨�� */
/* 3 ö�ٶ��� */
/* 4 ȫ�ֱ������� */
/* 5 ��Ϣͷ���� */
/* 6 ��Ϣ���� */
/* 7 STRUCT���� */
typedef struct {
    frw_timeout_stru st_dbac_timer;
} hmac_dbac_handle_stru;

/* 8 UNION���� */
/* 9 OTHERS���� */
/* 10 �������� */
extern oal_uint32 hmac_dbac_status_notify_etc(frw_event_mem_stru *pst_event_mem);
#endif

#endif /* end of hmac_dbac.h */