/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2012-2018. All rights reserved.
 * foss@huawei.com
 *
 * If distributed as part of the Linux kernel, the following license terms
 * apply:
 *
 * * This program is free software; you can redistribute it and/or modify
 * * it under the terms of the GNU General Public License version 2 and
 * * only version 2 as published by the Free Software Foundation.
 * *
 * * This program is distributed in the hope that it will be useful,
 * * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * * GNU General Public License for more details.
 * *
 * * You should have received a copy of the GNU General Public License
 * * along with this program; if not, write to the Free Software
 * * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307, USA
 *
 * Otherwise, the following license terms apply:
 *
 * * Redistribution and use in source and binary forms, with or without
 * * modification, are permitted provided that the following conditions
 * * are met:
 * * 1) Redistributions of source code must retain the above copyright
 * *    notice, this list of conditions and the following disclaimer.
 * * 2) Redistributions in binary form must reproduce the above copyright
 * *    notice, this list of conditions and the following disclaimer in the
 * *    documentation and/or other materials provided with the distribution.
 * * 3) Neither the name of Huawei nor the names of its contributors may
 * *    be used to endorse or promote products derived from this software
 * *    without specific prior written permission.
 *
 * * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 *
 */

#ifndef __APPESMINTERFACE_H__
#define __APPESMINTERFACE_H__

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif


/*****************************************************************************
  1 Include Headfile
*****************************************************************************/
#include "AppNasComm.h"
#include "AppRrcInterface.h"

#if (VOS_OS_VER != VOS_WIN32)
#pragma pack(4)
#else
#pragma pack(push, 4)
#endif

/*****************************************************************************
  2 Macro
*****************************************************************************/

/*IP地址长度宏定义*/
#define APP_MAX_IPV4_ADDR_LEN                               4
#define APP_MAX_IPV6_ADDR_LEN                               16

#define APP_MAX_IPV6_PREFIX_LEN                             8

#define APP_ESM_MAX_IPV6_PREFIX_NUM     (6)
#define APP_ESM_IPV6_ADDR_LEN           (16)



#define APP_MAX_APN_LEN                                     99
#define APP_ESM_MAX_EPSB_NUM                                (11)                /*最大承载数*/
#define APP_ESM_MAX_ACCESS_NUM_LEN                          32

/* 产品线at手册规定AT^AUTHDATA用户名和密码长度最大为99 */
#define APP_ESM_MAX_USER_NAME_LEN                           99
#define APP_ESM_MAX_PASSWORD_LEN                            99

#define APP_ESM_MAX_USER_NAME_ENCODE_LEN                    99
#define APP_ESM_MAX_PASSWORD_ENCODE_LEN                     99


#define APP_ESM_MAX_SDF_PF_NUM                              16


/*APP->SM Command消息前3个字节宏定义 APP->SM*/
#define APP_ESM_MSG_ID_HEADER                               PS_MSG_ID_APP_TO_ESM_BASE

/*SM->APP Command消息前3个字节宏定义 SM->APP*/
#define ESM_APP_MSG_ID_HEADER                               PS_MSG_ID_ESM_TO_APP_BASE

#define APP_ESM_PPP_CHAP_CHALLNGE_LEN       (16)
#define APP_ESM_PPP_CHAP_CHALLNGE_NAME_LEN  (16)
#define APP_ESM_PPP_CHAP_RESPONSE_LEN       (16)


#define APP_ESM_PPP_MAX_USERNAME_LEN    (99)
#define APP_ESM_PPP_MAX_PASSWORD_LEN    (99)

#define APP_ESM_TFT_MAX_PF_NUM                              16

#define APP_ESM_DT_REPORT_STATUS_OPEN                       1
#define APP_ESM_DT_REPORT_STATUS_CLOSE                      0

#define APP_ESM_MAX_CUSTOM_PCO_CONTAINER_NUM                (3)
#define APP_ESM_MAX_CUSTOM_PCO_CONTAINER_CONTENT_LEN        (53)
#define APP_ESM_MAX_VZW_APN_INFO_NUM                        (8)
#define  APP_ESM_MAX_IPV4_EPDG_NUM                          (2)
#define  APP_ESM_MAX_IPV6_EPDG_NUM                          (2)


/*****************************************************************************
  3 Massage Declare
*****************************************************************************/


/*****************************************************************************
  4 Enum
*****************************************************************************/
/*****************************************************************************
 枚举名    : APP_ESM_MSG_TYPE_ENUM
 枚举说明  : APP与SM消息取值
*****************************************************************************/
enum APP_ESM_MSG_TYPE_ENUM
{
    /*参数设置原语*/
    ID_APP_ESM_SET_TFT_REQ              = 0x01+APP_ESM_MSG_ID_HEADER,/*_H2ASN_MsgChoice  APP_ESM_SET_TFT_REQ_STRU*/
    ID_APP_ESM_SET_TFT_CNF              = 0x02+ESM_APP_MSG_ID_HEADER,/*_H2ASN_MsgChoice  APP_ESM_SET_TFT_CNF_STRU*/

    ID_APP_ESM_SET_QOS_REQ              = 0x03+APP_ESM_MSG_ID_HEADER,/*_H2ASN_MsgChoice  APP_ESM_SET_QOS_REQ_STRU*/
    ID_APP_ESM_SET_QOS_CNF              = 0x04+ESM_APP_MSG_ID_HEADER,/*_H2ASN_MsgChoice  APP_ESM_SET_QOS_CNF_STRU*/

    ID_APP_ESM_SET_EPS_QOS_REQ          = 0x05+APP_ESM_MSG_ID_HEADER,/*_H2ASN_MsgChoice  APP_ESM_SET_EPS_QOS_REQ_STRU*/
    ID_APP_ESM_SET_EPS_QOS_CNF          = 0x06+ESM_APP_MSG_ID_HEADER,/*_H2ASN_MsgChoice  APP_ESM_SET_EPS_QOS_CNF_STRU*/

    ID_APP_ESM_SET_APN_REQ              = 0x07+APP_ESM_MSG_ID_HEADER,/*_H2ASN_MsgChoice  APP_ESM_SET_APN_REQ_STRU*/
    ID_APP_ESM_SET_APN_CNF              = 0x08+ESM_APP_MSG_ID_HEADER,/*_H2ASN_MsgChoice  APP_ESM_SET_APN_CNF_STRU*/

    ID_APP_ESM_SET_PCO_REQ              = 0x09+APP_ESM_MSG_ID_HEADER,/*_H2ASN_MsgChoice  APP_ESM_SET_PCO_REQ_STRU*/
    ID_APP_ESM_SET_PCO_CNF              = 0x0A+ESM_APP_MSG_ID_HEADER,/*_H2ASN_MsgChoice  APP_ESM_SET_PCO_CNF_STRU*/

    ID_APP_ESM_SET_PDN_TYPE_REQ         = 0x0B+APP_ESM_MSG_ID_HEADER,/*_H2ASN_MsgChoice  APP_ESM_SET_PDN_TYPE_REQ_STRU*/
    ID_APP_ESM_SET_PDN_TYPE_CNF         = 0x0C+ESM_APP_MSG_ID_HEADER,/*_H2ASN_MsgChoice  APP_ESM_SET_PDN_TYPE_CNF_STRU*/

    ID_APP_ESM_SET_BEARER_TYPE_REQ      = 0x0D+APP_ESM_MSG_ID_HEADER,/*_H2ASN_MsgChoice  APP_ESM_SET_BEARER_TYPE_REQ_STRU*/
    ID_APP_ESM_SET_BEARER_TYPE_CNF      = ESM_APP_MSG_ID_HEADER + 0x0E,/*_H2ASN_MsgChoice  APP_ESM_SET_BEARER_TYPE_CNF_STRU*/

    ID_APP_ESM_SET_PDP_MANAGER_TYPE_REQ = 0x0F+APP_ESM_MSG_ID_HEADER,/*_H2ASN_MsgChoice  APP_ESM_SET_PDP_MANAGER_TYPE_REQ_STRU*/
    ID_APP_ESM_SET_PDP_MANAGER_TYPE_CNF = 0x10+ESM_APP_MSG_ID_HEADER,/*_H2ASN_MsgChoice  APP_ESM_SET_PDP_MANAGER_TYPE_CNF_STRU*/

    ID_APP_ESM_SET_GW_AUTH_REQ          = 0x11+APP_ESM_MSG_ID_HEADER,/*_H2ASN_MsgChoice  APP_ESM_SET_GW_AUTH_REQ_STRU*/
    ID_APP_ESM_SET_GW_AUTH_CNF          = 0x11+ESM_APP_MSG_ID_HEADER,/*_H2ASN_MsgChoice  APP_ESM_SET_GW_AUTH_CNF_STRU*/

    ID_APP_ESM_IPV6_INFO_NOTIFY         = 0x12+APP_ESM_MSG_ID_HEADER,/*_H2ASN_MsgChoice  APP_ESM_IPV6_INFO_NOTIFY_STRU*/

    ID_APP_ESM_PROCEDURE_ABORT_NOTIFY   = 0x13+APP_ESM_MSG_ID_HEADER,/*_H2ASN_MsgChoice  APP_ESM_PROCEDURE_ABORT_NOTIFY_STRU*/

    ID_APP_ESM_NDISCONN_REQ             = 0x14+APP_ESM_MSG_ID_HEADER,/*_H2ASN_MsgChoice  APP_ESM_NDISCONN_REQ_STRU*/
    ID_APP_ESM_NDISCONN_CNF             = 0x12+ESM_APP_MSG_ID_HEADER,/*_H2ASN_MsgChoice  APP_ESM_NDISCONN_CNF_STRU*/

    ID_APP_ESM_SET_CGDCONT_REQ          = 0x15+APP_ESM_MSG_ID_HEADER,/*_H2ASN_MsgChoice  APP_ESM_SET_CGDCONT_REQ_STRU*/
    ID_APP_ESM_SET_CGDCONT_CNF          = 0x13+ESM_APP_MSG_ID_HEADER,/*_H2ASN_MsgChoice  APP_ESM_SET_CGDCONT_CNF_STRU*/

    ID_APP_ESM_APN_ENABLE_INFO_CHANGE_NTF  = 0x16+APP_ESM_MSG_ID_HEADER,/*_H2ASN_MsgChoice  APP_ESM_APN_ENABLE_INFO_CHANGE_NTF_STRU*/

    ID_APP_ESM_SET_APN_THROT_INFO_REQ= 0x17+APP_ESM_MSG_ID_HEADER,/*_H2ASN_MsgChoice  APP_ESM_SET_APN_THROT_INFO_REQ_STRU*/
    ID_APP_ESM_SET_APN_THROT_INFO_CNF          = 0x14+ESM_APP_MSG_ID_HEADER,/*_H2ASN_MsgChoice  APP_ESM_SET_APN_THROT_INFO_CNF_STRU*/

    /*参数查询原语*/
    ID_APP_ESM_INQ_TFT_REQ              = 0x31+APP_ESM_MSG_ID_HEADER,/*_H2ASN_MsgChoice  APP_ESM_INQ_TFT_REQ_STRU*/
    ID_APP_ESM_INQ_TFT_CNF              = 0x32+ESM_APP_MSG_ID_HEADER,/*_H2ASN_MsgChoice  APP_ESM_INQ_TFT_CNF_STRU*/

    ID_APP_ESM_INQ_QOS_REQ              = 0x33+APP_ESM_MSG_ID_HEADER,/*_H2ASN_MsgChoice  APP_ESM_INQ_QOS_REQ_STRU*/
    ID_APP_ESM_INQ_QOS_CNF              = 0x34+ESM_APP_MSG_ID_HEADER,/*_H2ASN_MsgChoice  APP_ESM_INQ_QOS_CNF_STRU*/

    ID_APP_ESM_INQ_EPS_QOS_REQ          = 0x35+APP_ESM_MSG_ID_HEADER,/*_H2ASN_MsgChoice  APP_ESM_INQ_EPS_QOS_REQ_STRU*/
    ID_APP_ESM_INQ_EPS_QOS_CNF          = 0x36+ESM_APP_MSG_ID_HEADER,/*_H2ASN_MsgChoice  APP_ESM_INQ_EPS_QOS_CNF_STRU*/

    ID_APP_ESM_INQ_APN_REQ              = 0x37+APP_ESM_MSG_ID_HEADER,/*_H2ASN_MsgChoice  APP_ESM_INQ_APN_REQ_STRU*/
    ID_APP_ESM_INQ_APN_CNF              = 0x38+ESM_APP_MSG_ID_HEADER,/*_H2ASN_MsgChoice  APP_ESM_INQ_APN_CNF_STRU*/

    ID_APP_ESM_INQ_PCO_REQ              = 0x39+APP_ESM_MSG_ID_HEADER,/*_H2ASN_MsgChoice  APP_ESM_INQ_PCO_REQ_STRU*/
    ID_APP_ESM_INQ_PCO_CNF              = 0x3A+ESM_APP_MSG_ID_HEADER,/*_H2ASN_MsgChoice  APP_ESM_INQ_PCO_CNF_STRU*/

    ID_APP_ESM_INQ_PDN_TYPE_REQ         = 0x3B+APP_ESM_MSG_ID_HEADER,/*_H2ASN_MsgChoice  APP_ESM_INQ_PDN_TYPE_REQ_STRU*/
    ID_APP_ESM_INQ_PDN_TYPE_CNF         = 0x3C+ESM_APP_MSG_ID_HEADER,/*_H2ASN_MsgChoice  APP_ESM_INQ_PDN_TYPE_CNF_STRU*/

    ID_APP_ESM_INQ_BEARER_TYPE_REQ      = 0x3D+APP_ESM_MSG_ID_HEADER,/*_H2ASN_MsgChoice  APP_ESM_INQ_BEARER_TYPE_REQ_STRU*/
    ID_APP_ESM_INQ_BEARER_TYPE_CNF      = ESM_APP_MSG_ID_HEADER + 0x3E,/*_H2ASN_MsgChoice  APP_ESM_INQ_BEARER_TYPE_CNF_STRU*/

    ID_APP_ESM_INQ_PDP_MANAGER_TYPE_REQ= 0x3F+APP_ESM_MSG_ID_HEADER,/*_H2ASN_MsgChoice  APP_ESM_INQ_PDP_MANAGER_TYPE_REQ_STRU*/
    ID_APP_ESM_INQ_PDP_MANAGER_TYPE_CNF= 0x40+ESM_APP_MSG_ID_HEADER,/*_H2ASN_MsgChoice  APP_ESM_INQ_PDP_MANAGER_TYPE_CNF_STRU*/

    ID_APP_ESM_INQ_BEAER_QOS_REQ        =  0x41 + APP_ESM_MSG_ID_HEADER,/*_H2ASN_MsgChoice  APP_ESM_INQ_BEAER_QOS_REQ_STRU*/
    ID_APP_ESM_INQ_BEAER_QOS_CNF        =  0x42 + ESM_APP_MSG_ID_HEADER,/*_H2ASN_MsgChoice  APP_ESM_INQ_BEAER_QOS_CNF_STRU*/
    ID_APP_ESM_INQ_BEAER_QOS_IND        =  0x43 + ESM_APP_MSG_ID_HEADER,/*_H2ASN_MsgChoice  APP_ESM_INQ_BEAER_QOS_IND_STRU*/

    ID_APP_ESM_INQ_DYNAMIC_PDP_CONT_REQ = 0x44+APP_ESM_MSG_ID_HEADER,/*_H2ASN_MsgChoice  APP_ESM_INQ_DYNAMIC_PDP_CONT_REQ_STRU*/
    ID_APP_ESM_INQ_DYNAMIC_PDP_CONT_CNF = 0x45+ESM_APP_MSG_ID_HEADER,/*_H2ASN_MsgChoice  APP_ESM_INQ_DYNAMIC_PDP_CONT_CNF_STRU*/

    ID_APP_ESM_INQ_DYNAMIC_EPS_QOS_REQ  = 0x46+APP_ESM_MSG_ID_HEADER,/*_H2ASN_MsgChoice  APP_ESM_INQ_DYNAMIC_EPS_QOS_REQ_STRU*/
    ID_APP_ESM_INQ_DYNAMIC_EPS_QOS_CNF  = 0x47+ESM_APP_MSG_ID_HEADER,/*_H2ASN_MsgChoice  APP_ESM_INQ_DYNAMIC_EPS_QOS_CNF_STRU*/

    ID_APP_ESM_INQ_DYNAMIC_TFT_REQ      = 0x48+APP_ESM_MSG_ID_HEADER,/*_H2ASN_MsgChoice  APP_ESM_INQ_DYNAMIC_TFT_REQ_STRU*/
    ID_APP_ESM_INQ_DYNAMIC_TFT_CNF      = 0x49+ESM_APP_MSG_ID_HEADER,/*_H2ASN_MsgChoice  APP_ESM_INQ_DYNAMIC_TFT_CNF_STRU*/

    ID_APP_ESM_INQ_GW_AUTH_REQ          = 0x4A+APP_ESM_MSG_ID_HEADER,/*_H2ASN_MsgChoice  APP_ESM_INQ_GW_AUTH_REQ_STRU*/
    ID_APP_ESM_INQ_GW_AUTH_CNF          = 0x4A+ESM_APP_MSG_ID_HEADER,/*_H2ASN_MsgChoice  APP_ESM_INQ_GW_AUTH_CNF_STRU*/


    ID_APP_ESM_DT_INQ_PDP_INFO_REQ      = 0x4B+APP_ESM_MSG_ID_HEADER,/*_H2ASN_MsgChoice  APP_ESM_INQ_PDP_INFO_REQ_STRU*/
    ID_APP_ESM_DT_INQ_PDP_INFO_CNF      = 0x4B+ESM_APP_MSG_ID_HEADER,/*_H2ASN_MsgChoice  APP_ESM_INQ_PDP_INFO_CNF_STRU*/
    ID_APP_ESM_DT_INQ_PDP_INFO_IND      = 0x4C+ESM_APP_MSG_ID_HEADER,/*_H2ASN_MsgChoice  APP_ESM_INQ_PDP_INFO_IND_STRU*/

    ID_ESM_DSM_NOTIFICATION_IND         = 0x4D + APP_ESM_MSG_ID_HEADER,/* _H2ASN_MsgChoice ESM_DSM_NOTIFICATION_IND_STRU */
    ID_ESM_DSM_REG_CID_IND              = 0x4E + ESM_APP_MSG_ID_HEADER,/* _H2ASN_MsgChoice ESM_DSM_REG_CID_IND_STRU */

    /*承载建立、修改和释放消息原语结构*/
    ID_APP_ESM_PDP_SETUP_REQ         = 0x61+APP_ESM_MSG_ID_HEADER,/*_H2ASN_MsgChoice  APP_ESM_PDP_SETUP_REQ_STRU*/
    ID_APP_ESM_PDP_SETUP_CNF         = 0x62+ESM_APP_MSG_ID_HEADER,/*_H2ASN_MsgChoice  APP_ESM_PDP_SETUP_CNF_STRU*/
    ID_APP_ESM_PDP_SETUP_IND         = 0x63+ESM_APP_MSG_ID_HEADER,/*_H2ASN_MsgChoice  APP_ESM_PDP_SETUP_IND_STRU*/

    ID_APP_ESM_PDP_MODIFY_REQ        = 0x64+APP_ESM_MSG_ID_HEADER,/*_H2ASN_MsgChoice  APP_ESM_PDP_MODIFY_REQ_STRU*/
    ID_APP_ESM_PDP_MODIFY_CNF        = 0x65+ESM_APP_MSG_ID_HEADER,/*_H2ASN_MsgChoice  APP_ESM_PDP_MODIFY_CNF_STRU*/
    ID_APP_ESM_PDP_MODIFY_IND        = 0x66+ESM_APP_MSG_ID_HEADER,/*_H2ASN_MsgChoice  APP_ESM_PDP_MODIFY_IND_STRU*/

    ID_APP_ESM_PDP_RELEASE_REQ       = 0x67+APP_ESM_MSG_ID_HEADER,/*_H2ASN_MsgChoice  APP_ESM_PDP_RELEASE_REQ_STRU*/
    ID_APP_ESM_PDP_RELEASE_CNF       = 0x68+ESM_APP_MSG_ID_HEADER,/*_H2ASN_MsgChoice  APP_ESM_PDP_RELEASE_CNF_STRU*/
    ID_APP_ESM_PDP_RELEASE_IND       = 0x69+ESM_APP_MSG_ID_HEADER,/*_H2ASN_MsgChoice  APP_ESM_PDP_RELEASE_IND_STRU*/

    ID_APP_ESM_PDP_MANAGER_IND       = 0x6A+ESM_APP_MSG_ID_HEADER,/*_H2ASN_MsgChoice  APP_ESM_PDP_MANAGER_IND_STRU*/
    ID_APP_ESM_PDP_MANAGER_RSP       = 0x6B+APP_ESM_MSG_ID_HEADER,/*_H2ASN_MsgChoice  APP_ESM_PDP_MANAGER_RSP_STRU*/

    ID_APP_ESM_MSG_TYPE_BUTT
};
typedef VOS_UINT32 APP_ESM_MSG_TYPE_ENUM_UINT32;


/*****************************************************************************
 枚举名    : APP_ESM_PARA_SET_ENUM
 枚举说明  : 参数设置类型取值
*****************************************************************************/
enum APP_ESM_PARA_SET_ENUM
{
    APP_ESM_PARA_SETTING                = 0x00,
    APP_ESM_PARA_DELETE                 = 0x01,
    APP_ESM_PARA_BUTT
};
typedef VOS_UINT32 APP_ESM_PARA_SET_ENUM_UINT32;

/*****************************************************************************
 枚举名    : APP_ESM_PARA_SET_RESULT_ENUM
 枚举说明  : 参数设置结果取值
*****************************************************************************/
enum APP_ESM_PARA_SET_RESULT_ENUM
{
    APP_ESM_PARA_SET_SUCCESS            = 0x00,
    APP_ESM_PARA_SET_FAIL               = 0x01,
    APP_ESM_PARA_SET_BUTT
};
typedef VOS_UINT32 APP_ESM_PARA_SET_RESULT_ENUM_UINT32;

/*****************************************************************************
 枚举名    : APP_ESM_BEARER_STATE_ENUM
 枚举说明  : 承载状态取值
*****************************************************************************/
enum APP_ESM_BEARER_STATE_ENUM
{
    APP_ESM_BEARER_STATE_INACTIVE       = 0x00,
    APP_ESM_BEARER_STATE_ACTIVATED      = 0x01,
    APP_ESM_BEARER_STATE_BUTT
};
typedef VOS_UINT32 APP_ESM_BEARER_STATE_ENUM_UINT32;

/*****************************************************************************
 枚举名    : APP_ESM_CAUSE_ENUM_UINT32
 枚举说明  : SM原因取值
*****************************************************************************/

typedef VOS_UINT32 APP_ESM_CAUSE_ENUM_UINT32;

/*****************************************************************************
 枚举名    : APP_ESM_BEARER_TYPE_ENUM
 枚举说明  : 承载类型取值
*****************************************************************************/
enum APP_ESM_BEARER_TYPE_ENUM
{
    APP_ESM_BEARER_TYPE_DEFAULT         = 0x00,
    APP_ESM_BEARER_TYPE_DEDICATED       = 0x01,
    APP_ESM_BEARER_TYPE_EMERGENCY       = 0x02,
    APP_ESM_BEARER_TYPE_BUTT
};
typedef VOS_UINT32 APP_ESM_BEARER_TYPE_ENUM_UINT32;

/*****************************************************************************
 枚举名    : APP_ESM_BEARER_MODIFY_ENUM
 枚举说明  : 承载修改内容取值
*****************************************************************************/
enum APP_ESM_BEARER_MODIFY_ENUM
{
    APP_ESM_BEARER_MODIFY_TFT           = 0x00,
    APP_ESM_BEARER_MODIFY_QOS           = 0x01,
    APP_ESM_BEARER_MODIFY_TFT_QOS       = 0x02,
    APP_ESM_BEARER_MODIFY_BUTT
};
typedef VOS_UINT32 APP_ESM_BEARER_MODIFY_ENUM_UINT32;

/*****************************************************************************
 枚举名    : APP_ESM_BEARER_ACT_TYPE_ENUM
 枚举说明  : 承载激活类型取值
*****************************************************************************/
enum APP_ESM_BEARER_ACT_TYPE_ENUM
{
    APP_ESM_BEARER_ACT_TYPE_ACTIVE      = 0x00,
    APP_ESM_BEARER_ACT_TYPE_MODIFY      = 0x01,
    APP_ESM_BEARER_ACT_TYPE_BUTT
};
typedef VOS_UINT32 APP_ESM_BEARER_ACT_TYPE_ENUM_UINT32;

/*****************************************************************************
 枚举名    : APP_ESM_BEARER_ACT_RESULT_ENUM
 枚举说明  : 承载激活结果取值
*****************************************************************************/
enum APP_ESM_BEARER_ACT_RESULT_ENUM
{
    APP_ESM_BEARER_ACT_ACCEPT           = 0x00,
    APP_ESM_BEARER_ACT_REJ              = 0x01,
    APP_ESM_BEARER_ACT_BUTT
};
typedef VOS_UINT32 APP_ESM_BEARER_ACT_RESULT_ENUM_UINT32;

/*****************************************************************************
 枚举名    : APP_ESM_ANSWER_MODE_ENUM
 枚举说明  : 承载答复模式取值
*****************************************************************************/
enum APP_ESM_ANSWER_MODE_ENUM
{
    APP_ESM_ANSWER_MODE_AUTO            = 0x00,
    APP_ESM_ANSWER_MODE_MANI            = 0x01,
    APP_ESM_ANSWER_MODE_BUTT
};
typedef VOS_UINT32 APP_ESM_ANSWER_MODE_ENUM_UINT32;

/*****************************************************************************
 枚举名    : APP_ESM_ANSWER_RESULT_ENUM
 枚举说明  : 自动承载答复取值
*****************************************************************************/
enum APP_ESM_ANSWER_RESULT_ENUM
{
    APP_ESM_ANSWER_ACCEPT               = 0x00,
    APP_ESM_ANSWER_REJ                  = 0x01,
    APP_ESM_ANSWER_RESULT_BUTT
};
typedef VOS_UINT32 APP_ESM_ANSWER_RESULT_ENUM_UINT32;

/*****************************************************************************
 枚举名    : APP_ESM_PDN_TYPE_ENUM
 枚举说明  : PDN类型取值
*****************************************************************************/
enum APP_ESM_PDN_TYPE_ENUM
{
    APP_ESM_PDN_TYPE_IPV4               = 0x01,             /*遵从协议24301 9.9.4.10规定*/
    APP_ESM_PDN_TYPE_IPV6               = 0x02,
    APP_ESM_PDN_TYPE_IPV4_IPV6          = 0x03,
    APP_ESM_PDN_TYPE_BUTT
};
typedef VOS_UINT32 APP_ESM_PDN_TYPE_ENUM_UINT32;

/*****************************************************************************
 枚举名    : APP_ESM_PDN_REQUEST_TYPE_ENUM
 枚举说明  : PDN请求类型取值 24008 10.5.6.17
*****************************************************************************/
enum APP_ESM_PDN_REQUEST_TYPE_ENUM
{
    APP_ESM_PDN_REQUEST_TYPE_INITIAL    = 0x01,             /*建立一个新的PDN连接*/
    APP_ESM_PDN_REQUEST_TYPE_HANDOVER   = 0x02,             /*保持non-3GPP下建好的PDN连接*/
    APP_ESM_PDN_REQUEST_TYPE_BUTT
};
typedef VOS_UINT32 APP_ESM_PDN_REQUEST_TYPE_ENUM_UINT32;


/*****************************************************************************
 枚举名    : APP_ESM_CID_TYPE_ENUM
 枚举说明  : CID类型取值
*****************************************************************************/
enum APP_ESM_CID_TYPE_ENUM
{
    APP_ESM_CID_TYPE_DEFAULT             = 0x00,
    APP_ESM_CID_TYPE_DEDICATED           = 0x01,
    APP_ESM_CID_TYPE_EMERGENCY           = 0x02,
    APP_ESM_CID_TYPE_BUTT
};
typedef VOS_UINT32   APP_ESM_CID_TYPE_ENUM_UINT32;

enum APP_ESM_TRANSFER_DIRECTION_ENUM
{
    APP_ESM_TRANSFER_DIRECTION_PRE_REL7_TFT_FILTER          = 0x00,
    APP_ESM_TRANSFER_DIRECTION_DOWNLINK                     = 0x01,
    APP_ESM_TRANSFER_DIRECTION_UPLINK                       = 0x02,
    APP_ESM_TRANSFER_DIRECTION_UPLINK_DOWNLINK              = 0x03,
    APP_ESM_TRANSFER_DIRECTION_BUTT
};
typedef VOS_UINT8 APP_ESM_TRANSFER_DIRECTION_ENUM_UINT8;

enum APP_ESM_BEARER_OPERATE_TYPE_ENUM
{
    APP_ESM_BEARER_OPERATE_TYPE_DEF_ACT      = 0x00,
    APP_ESM_BEARER_OPERATE_TYPE_DEDI_ACT     = 0x01,
    APP_ESM_BEARER_OPERATE_TYPE_MODIFY       = 0x02,
    APP_ESM_BEARER_OPERATE_TYPE_BUTT
};
typedef VOS_UINT32 APP_ESM_BEARER_OPERATE_TYPE_ENUM_UINT32;

enum APP_ESM_IPV4_ADDR_ALLOC_TYPE_ENUM
{
   APP_ESM_IPV4_ADDR_ALLOC_TYPE_NAS_SIGNALING   = 0x00,
   APP_ESM_IPV4_ADDR_ALLOC_TYPE_DHCP            = 0x01,

   APP_ESM_IPV4_ADDR_ALLOC_TYPE_BUTT
};
typedef VOS_UINT32 APP_ESM_IPV4_ADDR_ALLOC_TYPE_ENUM_UINT32;

enum APP_ESM_PDP_SETUP_TYPE_ENUM
{
    APP_ESM_PDP_SETUP_TYPE_NORMAL       = 0x00,
    APP_ESM_PDP_SETUP_TYPE_PPP          = 0x01,

    APP_ESM_PDP_SETUP_TYPE_BUTT         = 0xFF
};

typedef VOS_UINT32 APP_ESM_PDP_SETUP_TYPE_ENUM_UINT32;
/* 用于EAB特性，记录APP下发的承载优先级 */
enum APP_ESM_BEARER_PRIO_ENUM
{
    APP_ESM_BEARER_PRIO_NORMAL          = 0x00,
    APP_ESM_BEARER_PRIO_LOW             = 0x01,

    APP_ESM_BEARER_PRIO_BUTT
};

typedef VOS_UINT32 APP_ESM_BEARER_PRIO_ENUM_UINT32;


/*****************************************************************************
 枚举名称: APP_ESM_AUTH_TYPE_ENUM
 枚举说明: Authentication Type
*****************************************************************************/
enum APP_ESM_AUTH_TYPE_ENUM
{
    APP_ESM_AUTH_TYPE_NONE              = 0x00,
    APP_ESM_AUTH_TYPE_PAP               = 0x01,
    APP_ESM_AUTH_TYPE_CHAP              = 0x02,

    APP_ESM_AUTH_TYPE_BUTT              = 0xFF
};

typedef VOS_UINT8 APP_ESM_AUTH_TYPE_ENUM_UINT8;

enum APP_ESM_BEARER_MANAGE_TYPE_ENUM
{
    APP_ESM_BEARER_MANAGE_TYPE_ACCEPT = 0,                  /*接受*/
    APP_ESM_BEARER_MANAGE_TYPE_REJ,                         /*拒绝*/

    APP_ESM_BEARER_MANANGE_TYPE_BUTT
};
typedef VOS_UINT8  APP_ESM_BEARER_MANAGE_TYPE_ENUM_UINT8;

enum APP_ESM_BEARER_MANAGE_MODE_ENUM
{
    APP_ESM_BEARER_MANAGE_MODE_AUTO = 0,                    /*自动*/
    APP_ESM_BEARER_MANAGE_MODE_MANUAL,                      /*手动*/

    APP_ESM_BEARER_MANAGE_MODE_BUTT
};
typedef VOS_UINT8  APP_ESM_BEARER_MANAGE_MODE_ENUM_UINT8;


/*****************************************************************************
 结构名    : NAS_ESM_DT_BEARER_TYPE_ENUM
 结构说明  : 给路测上报的承载类型
*****************************************************************************/

enum NAS_ESM_DT_BEARER_TYPE_ENUM
{
    NAS_ESM_DT_BEARER_TYPE_DEFAULT = 0,                /*缺省承载*/
    NAS_ESM_DT_BEARER_TYPE_DEDICATED,                  /*专有承载*/

    NAS_ESM_DT_BEARER_TYPE_EMERGENCY,                  /*紧急承载*/


    NAS_ESM_DT_BEARER_TYPE_BUTT
};/*承载的属性*/
typedef VOS_UINT8  NAS_ESM_DT_BEARER_TYPE_ENUM_UINT8;

enum APP_ESM_PCSCF_DISCOVERY_ENUM
{
    APP_ESM_PCSCF_DISCOVERY_NOT_INFLUENCED  = 0x00,
    APP_ESM_PCSCF_DISCOVERY_THROUGH_NAS_SIG = 0x01,
    APP_ESM_PCSCF_DISCOVERY_THROUGH_DHCP    = 0x02,

    APP_ESM_PCSCF_DISCOVERY_BUTT
};
typedef VOS_UINT32 APP_ESM_PCSCF_DISCOVERY_ENUM_UINT32;

enum APP_ESM_IMS_CN_SIG_FLAG_ENUM
{
    APP_ESM_PDP_NOT_FOR_IMS_CN_SIG_ONLY     = 0x00,
    APP_ESM_PDP_FOR_IMS_CN_SIG_ONLY         = 0x01,

    APP_ESM_IMS_CN_SIG_FLAG_BUTT
};
typedef VOS_UINT32 APP_ESM_IMS_CN_SIG_FLAG_ENUM_UINT32;

enum APP_ESM_PDP_EMC_IND_ENUM
{
    APP_ESM_PDP_NOT_FOR_EMC             = 0x00,
    APP_ESM_PDP_FOR_EMC                 = 0x01,

    APP_ESM_PDP_EMC_IND_BUTT
};
typedef VOS_UINT32 APP_ESM_PDP_EMC_IND_ENUM_UINT32;
/*****************************************************************************
 枚举名    : APP_ESM_PDP_RELEASE_CAUSE_ENUM
 枚举说明  : PDP释放原因
*****************************************************************************/
enum APP_ESM_PDP_RELEASE_CAUSE_ENUM
{
    APP_ESM_PDP_RELEASE_CAUSE_NORMAL                  = 0,
    APP_ESM_PDP_RELEASE_CAUSE_IPV6_ADDR_ALLOC_FAIL    = 1,
    APP_ESM_PDP_RELEASE_CAUSE_IPV6_ADDR_TIME_OUT      = 2,
    APP_ESM_PDP_RELEASE_CAUSE_PCSCF_ADDR_ALLOC_FAIL   = 3,

    APP_ESM_PDP_RELEASE_CAUSE_DATA_OFF                = 4,
    APP_ESM_PDP_RELEASE_CAUSE_CELLULAR2W_HO           = 5,

    APP_ESM_PDP_RELEASE_CAUSE_BUTT
};
typedef VOS_UINT8 APP_ESM_PDP_RELEASE_CAUSE_ENUM_UINT8;

/*****************************************************************************
 枚举名    : APP_ESM_APN_TYPE_ENUM_UINT8
 枚举说明  : 注册使用的APN 类型
*****************************************************************************/
enum APP_ESM_APN_TYPE_ENUM
{
    APP_ESM_APN_TYPE_INVALID = 0x00,
    APP_ESM_APN_TYPE_CLASS1  = 0x01,
    APP_ESM_APN_TYPE_CLASS2  = 0x02,
    APP_ESM_APN_TYPE_CLASS3  = 0x03,
    APP_ESM_APN_TYPE_CLASS4  = 0x04,
    APP_ESM_APN_TYPE_CLASS5  = 0x05,
    APP_ESM_APN_TYPE_CLASS6  = 0x06,
    APP_ESM_APN_TYPE_CLASS7  = 0x07,
    APP_ESM_APN_TYPE_CLASS8  = 0x08,

    APP_ESM_APN_TYPE_BUTT
};
typedef VOS_UINT8 APP_ESM_APN_TYPE_ENUM_UINT8;

/*****************************************************************************
 枚举名    : ESM_DSM_NOTIFICATION_IND_ENUM_UINT32
 枚举说明  : SRVCC处理类型
*****************************************************************************/
enum ESM_DSM_NOTIFICATION_IND_ENUM
{
    ESM_DSM_NOTIFICATION_IND_SRVCC_HO_CANCELLED         = 0,

    ESM_DSM_NOTIFICATION_IND_BUTT
};
typedef VOS_UINT32 ESM_DSM_NOTIFICATION_IND_ENUM_UINT32;

/*****************************************************************************
  5 STRUCT
*****************************************************************************/
/*****************************************************************************
*                                                                            *
*                           参数设置消息结构                                 *
*                                                                            *
******************************************************************************/

/*****************************************************************************
 枚举名    : ESM_DSM_NOTIFICATION_IND_STRU
 枚举说明  : SRVCC通知信息
*****************************************************************************/
typedef struct
{
    VOS_MSG_HEADER
    VOS_UINT32                              ulMsgId;
    ESM_DSM_NOTIFICATION_IND_ENUM_UINT32    enNotificationIndicator;
}ESM_DSM_NOTIFICATION_IND_STRU;


/*****************************************************************************
 结构名称   : ESM_DSM_REG_CID_IND_STRU
 结构说明   : ESM -> DSM
              将注册时使用的SDF中的CID通知给DSM
*****************************************************************************/
typedef struct
{
    VOS_MSG_HEADER
    VOS_UINT32                          ulMsgId;
    VOS_UINT32                          ulRegCid;
}ESM_DSM_REG_CID_IND_STRU;

/*****************************************************************************
 结构名    : APP_ESM_APN_CLASS_INFO_STRU
 结构说明  : 注册APN信息结构
*****************************************************************************/
typedef struct
{
    VOS_UINT32                          ulCid;  /*CID [0,11]*/
    APP_ESM_APN_TYPE_ENUM_UINT8         enApnType;    /*apn class*/
    VOS_UINT8                           ucDisableFlag;/*apn disable flag*/
    VOS_UINT8                           aucReserved[2];
    VOS_UINT32                          ulInactivityTimerValue;/*Inactivity Timer Value in second*/
}APP_ESM_APN_CLASS_INFO_STRU;


/*****************************************************************************
 结构名    : APP_ESM_PARA_SET_CNF_STRU
 结构说明  : 参数设置结果回复的通用结构
*****************************************************************************/
typedef struct
{
    VOS_MSG_HEADER                                          /*_H2ASN_Skip*/
    VOS_UINT32                          ulMsgId;            /*_H2ASN_Skip*/
    APP_MSG_HEADER
    VOS_UINT32                          ulOpId;             /*本次操作标识*/
    VOS_UINT32                          ulCid;              /*上下文序列号:0~31   */
    VOS_UINT32                          ulSetRslt;          /*参数设置结果:0:成功,1:失败   */
}APP_ESM_PARA_SET_CNF_STRU;

/*****************************************************************************
 结构名    : APP_ESM_TFT_INFO_STRU
 结构说明  : TFT  结构
*****************************************************************************/
typedef struct
{
    VOS_UINT32                          bitOpRmtIpv4AddrAndMask        : 1;
    VOS_UINT32                          bitOpRmtIpv6AddrAndMask        : 1;
    VOS_UINT32                          bitOpProtocolId                : 1;
    VOS_UINT32                          bitOpSingleLocalPort           : 1;
    VOS_UINT32                          bitOpLocalPortRange            : 1;
    VOS_UINT32                          bitOpSingleRemotePort          : 1;
    VOS_UINT32                          bitOpRemotePortRange           : 1;
    VOS_UINT32                          bitOpSecuParaIndex             : 1;
    VOS_UINT32                          bitOpTypeOfService             : 1;
    VOS_UINT32                          bitOpFlowLabelType             : 1;
    VOS_UINT32                          bitOpLocalIpv4AddrAndMask      : 1;
    VOS_UINT32                          bitOpLocalIpv6AddrAndMask      : 1;
    VOS_UINT32                          bitOpSpare                     : 20;

    VOS_UINT8                           ucPacketFilterId;
    VOS_UINT8                           ucNwPacketFilterId;
    APP_ESM_TRANSFER_DIRECTION_ENUM_UINT8 enDirection;
    VOS_UINT8                           ucPrecedence;             /*packet filter evaluation precedence*/


    VOS_UINT32                          ulSecuParaIndex;          /*SPI*/
    VOS_UINT16                          usSingleLcPort;
    VOS_UINT16                          usLcPortHighLimit;
    VOS_UINT16                          usLcPortLowLimit;
    VOS_UINT16                          usSingleRmtPort;
    VOS_UINT16                          usRmtPortHighLimit;
    VOS_UINT16                          usRmtPortLowLimit;
    VOS_UINT8                           ucProtocolId;             /*协议号*/
    VOS_UINT8                           ucTypeOfService;          /*TOS*/

    VOS_UINT8                           ucTypeOfServiceMask;      /*TOS Mask*/
    VOS_UINT8                           aucReserved[1];

    VOS_UINT8                           aucRmtIpv4Address[APP_MAX_IPV4_ADDR_LEN];
                                                                  /*ucSourceIpAddress[0]为IP地址高字节位
                                                                  ucSourceIpAddress[3]为低字节位*/
    VOS_UINT8                           aucRmtIpv4Mask[APP_MAX_IPV4_ADDR_LEN];
                                                                  /*ucSourceIpMask[0]为IP地址高字节位 ,
                                                                  ucSourceIpMask[3]为低字节位*/
    VOS_UINT8                           aucRmtIpv6Address[APP_MAX_IPV6_ADDR_LEN];
                                                                  /*ucRmtIpv6Address[0]为IPv6接口标识高字节位
                                                                    ucRmtIpv6Address[7]为IPv6接口标识低字节位*/
    VOS_UINT8                           aucRmtIpv6Mask[APP_MAX_IPV6_ADDR_LEN];
                                                                  /*ucRmtIpv6Mask[0]为高字节位
                                                                    ucRmtIpv6Mask[7]为低字节位*/

    VOS_UINT32                          ulFlowLabelType;          /*FlowLabelType*/

    VOS_UINT8                           aucLocalIpv4Addr[APP_MAX_IPV4_ADDR_LEN];
    VOS_UINT8                           aucLocalIpv4Mask[APP_MAX_IPV4_ADDR_LEN];
    VOS_UINT8                           aucLocalIpv6Addr[APP_MAX_IPV6_ADDR_LEN];
    VOS_UINT8                           ucLocalIpv6Prefix;
    VOS_UINT8                           aucReserved2[3];
}APP_ESM_TFT_INFO_STRU;
/*****************************************************************************
 结构名    : APP_ESM_SET_TFT_REQ_STRU
 结构说明  : TFT  参数设置
*****************************************************************************/
typedef struct
{
    VOS_MSG_HEADER                                          /*_H2ASN_Skip*/
    VOS_UINT32                          ulMsgId;            /*_H2ASN_Skip*/
    APP_MSG_HEADER
    VOS_UINT32                          ulOpId;             /*本次操作标识*/
    VOS_UINT32                          ulCid;              /*上下文序列号:0~31       */
    APP_ESM_PARA_SET_ENUM_UINT32        enSetType;          /*设置类型:0:设置,1:删除*/
    APP_ESM_TFT_INFO_STRU               stAppTftInfo;       /*TFT消息*/
} APP_ESM_SET_TFT_REQ_STRU;

typedef  APP_ESM_PARA_SET_CNF_STRU APP_ESM_SET_TFT_CNF_STRU;

/*****************************************************************************
 结构名    : APP_ESM_QOS_INFO_STRU
 结构说明  : QOS  结构
*****************************************************************************/
typedef struct
{
    VOS_UINT8                           ucDelayClass;
    VOS_UINT8                           ucReliabClass;
    VOS_UINT8                           ucPeakThrough;
    VOS_UINT8                           ucPrecedenceClass;
    VOS_UINT8                           ucMeanThrough;
    VOS_UINT8                           ucTrafficClass;
    VOS_UINT8                           ucDeliverOrder;
    VOS_UINT8                           ucDeliverOfErrSdu;
    VOS_UINT8                           ucMaximSduSize;
    VOS_UINT8                           ucMaxBitRateForUp;
    VOS_UINT8                           ucMaxBitRateForDown;
    VOS_UINT8                           ucResidualBer;
    VOS_UINT8                           ucSduErrRatio;
    VOS_UINT8                           ucTransDelay;
    VOS_UINT8                           ucTraffHandlPrior;
    VOS_UINT8                           ucGuarantBitRateForUp;
    VOS_UINT8                           ucGuarantBitRateForDown;
    VOS_UINT8                           ucSrcStatisticsDescriptor;
    VOS_UINT8                           ucSignallingIndication;
    VOS_UINT8                           ucMaxBitRateForDownExt;
    VOS_UINT8                           ucGuarantBitRateForDownExt;
    VOS_UINT8                           ucMaxBitRateForUpExt;
    VOS_UINT8                           ucGuarantBitRateForUpExt;
    VOS_UINT8                           ucMaxBitRateForDownExt_2;
    VOS_UINT8                           ucGuarantBitRateForDownExt_2;
    VOS_UINT8                           ucMaxBitRateForUpExt_2;
    VOS_UINT8                           ucGuarantBitRateForUpExt_2;

    VOS_UINT8                           ucReserved[1];
}APP_ESM_QOS_INFO_STRU;


/*****************************************************************************
 结构名    : APP_ESM_SET_QOS_REQ_STRU
 结构说明  : QOS  参数设置
*****************************************************************************/
typedef struct
{
    VOS_MSG_HEADER                                          /*_H2ASN_Skip*/
    VOS_UINT32                          ulMsgId;            /*_H2ASN_Skip*/
    APP_MSG_HEADER
    VOS_UINT32                          ulOpId;
    VOS_UINT32                          ulCid;              /*上下文序列号:0~31       */
    APP_ESM_PARA_SET_ENUM_UINT32        enSetType;          /*设置类型:0:设置,1:删除*/
    APP_ESM_QOS_INFO_STRU               stAppQoSInfo;       /*APP_ESM_QOS_INFO_STRU消息结构*/
}APP_ESM_SET_QOS_REQ_STRU;

typedef  APP_ESM_PARA_SET_CNF_STRU APP_ESM_SET_QOS_CNF_STRU;

/*****************************************************************************
 结构名    : APP_ESM_EPS_QOS_INFO_STRU
 结构说明  : EPS_QOS  参数
*****************************************************************************/
typedef struct
{
    VOS_UINT8                           ucQCI;
    VOS_UINT8                           aucRsv[3];
    VOS_UINT32                          ulULMaxRate;    /* 单位为kbps */
    VOS_UINT32                          ulDLMaxRate;    /* 单位为kbps */
    VOS_UINT32                          ulULGMaxRate;   /* 单位为kbps */
    VOS_UINT32                          ulDLGMaxRate;   /* 单位为kbps */
}APP_ESM_EPS_QOS_INFO_STRU;

typedef struct
{
    VOS_UINT32                           ulEpsbId;
    APP_ESM_EPS_QOS_INFO_STRU            stQos;
} APP_EPS_QOS_STRU;
/*****************************************************************************
 结构名    : APP_ESM_SET_EPS_QOS_REQ_STRU
 结构说明  : SDF QOS  参数设置
*****************************************************************************/
typedef struct
{
    VOS_MSG_HEADER                                          /*_H2ASN_Skip*/
    VOS_UINT32                          ulMsgId;            /*_H2ASN_Skip*/
    APP_MSG_HEADER
    VOS_UINT32                          ulOpId;
    VOS_UINT32                          ulCid;

    APP_ESM_PARA_SET_ENUM_UINT32        enSetType;          /*0:SET;1:DEL*/
    APP_ESM_EPS_QOS_INFO_STRU           stSdfQosInfo;

}APP_ESM_SET_EPS_QOS_REQ_STRU;


typedef  APP_ESM_PARA_SET_CNF_STRU APP_ESM_SET_EPS_QOS_CNF_STRU;


/*****************************************************************************
 结构名    : APP_ESM_APN_INFO_STRU
 结构说明  : APN  参数
*****************************************************************************/
typedef struct
{
    VOS_UINT8                           ucApnLen;
    VOS_UINT8                           aucApnName[APP_MAX_APN_LEN];
}APP_ESM_APN_INFO_STRU;

/*****************************************************************************
 结构名    : APP_ESM_SET_APN_REQ_STRU
 结构说明  : APN  参数设置
*****************************************************************************/
typedef struct
{
    VOS_MSG_HEADER                                          /*_H2ASN_Skip*/
    VOS_UINT32                          ulMsgId;            /*_H2ASN_Skip*/
    APP_MSG_HEADER
    VOS_UINT32                          ulOpId;
    VOS_UINT32                          ulCid;

    APP_ESM_PARA_SET_ENUM_UINT32        enSetType;          /*0:SET;1:DEL*/
    APP_ESM_APN_INFO_STRU               stApnInfo;
}APP_ESM_SET_APN_REQ_STRU;

typedef  APP_ESM_PARA_SET_CNF_STRU APP_ESM_SET_APN_CNF_STRU;

/*****************************************************************************
 结构名    : APP_ESM_SET_EXCESSIVE_PDN_CTRL_PARA_REQ_STRU
 结构说明  : APN  参数设置
*****************************************************************************/
typedef struct
{
    APP_ESM_APN_INFO_STRU               stApnInfo;

    VOS_UINT32                          ulWaitTime;            /* 在PDN去连接之后，在waittime之后才能发起PDN建立流程，单位:s*/
    VOS_UINT32                          ulPdnMaxConnTime;      /* 最大连接时间，标识每一个PDN第一次发起以来经过的最大时间，单位:s*/
    VOS_UINT32                          ulPdnMaxConnCount;     /* 最大连接次数，在最大连接时间，允许发起PDN的最大连接次数*/

    VOS_UINT8                           ucCid;
    APP_ESM_APN_TYPE_ENUM_UINT8         enApnClassType;
    VOS_UINT8                           aucRsv[2];

}APP_ESM_EXCESSIVE_PDN_CTRL_PARA_STRU;

/*****************************************************************************
 结构名    : APP_ESM_SET_APN_THROT_INFO_REQ_STRU
 结构说明  : APN  参数设置
*****************************************************************************/
typedef struct
{
    VOS_MSG_HEADER                                          /*_H2ASN_Skip*/
    VOS_UINT32                                      ulMsgId;            /*_H2ASN_Skip*/
    APP_MSG_HEADER

    APP_ESM_EXCESSIVE_PDN_CTRL_PARA_STRU    stExcessivePdnCtrlPara;
}APP_ESM_SET_APN_THROT_INFO_REQ_STRU;

/*****************************************************************************
 结构名    : APP_ESM_SET_EXCESSIVE_PDN_CTRL_PARA_CNF_STRU
 结构说明  : APN  参数设置
*****************************************************************************/
typedef struct
{
    VOS_MSG_HEADER                                          /*_H2ASN_Skip*/
    VOS_UINT32                          ulMsgId;            /*_H2ASN_Skip*/
    APP_MSG_HEADER

    VOS_UINT32                          ulRslt;

}APP_ESM_SET_APN_THROT_INFO_CNF_STRU;

/*****************************************************************************
 结构名    : APP_ESM_APN_AMBR_INFO_STRU
 结构说明  : APN AMBR 参数
*****************************************************************************/
typedef struct
{
    VOS_UINT32                          bitOpDLApnAmbrExt                 : 1;
    VOS_UINT32                          bitOpULApnAmbrExt                 : 1;
    VOS_UINT32                          bitOpDLApnAmbrExt2                : 1;
    VOS_UINT32                          bitOpULApnAmbrExt2                : 1;
    VOS_UINT32                          bitOpSpare                        : 28;

    VOS_UINT8                           ucDLApnAmbr;
    VOS_UINT8                           ucULApnAmbr;
    VOS_UINT8                           ucDLApnAmbrExt;
    VOS_UINT8                           ucULApnAmbrExt;
    VOS_UINT8                           ucDLApnAmbrExt2;
    VOS_UINT8                           ucULApnAmbrExt2;
    VOS_UINT8                           aucRsv[2];

}APP_ESM_APN_AMBR_INFO_STRU;

/*****************************************************************************
 结构名    : APP_ESM_PCO_INFO_STRU
 结构说明  : PCO  参数
*****************************************************************************/
typedef struct
{

    VOS_UINT32                          bitOpAPL                          : 1;
    VOS_UINT32                          bitOpCPOL                         : 1;
    VOS_UINT32                          bitOpPCSCFAddReq                  : 1;
    VOS_UINT32                          bitOpIMCNFlag                     : 1;
    VOS_UINT32                          bitOpDNSAddrReq                   : 1;
    VOS_UINT32                          bitOpMSBearCtrlID                 : 1;
    VOS_UINT32                          bitOpPrefBearCtrlMode             : 1;

    VOS_UINT32                          bitOpSpare                        : 25;

    VOS_UINT8                           ucConfigProtocol;
    VOS_UINT8                           ucBearMode;       /*container identifier为Preferred Bearer Control Mode的内容值*/
    VOS_UINT8                           ucReserved[2];
    VOS_UINT16                          usAPLCid;          /*Additional parameters list的container identifier*/
    VOS_UINT16                          usCPOLPid;         /*Configuration protocol options list的protocol identifier*/
    VOS_UINT8                           aucPPPPacket[248];
}APP_ESM_PCO_INFO_STRU;



/*****************************************************************************
 结构名    : APP_ESM_SET_PCO_REQ_STRU
 结构说明  : PCO 参数设置
*****************************************************************************/
typedef struct
{
    VOS_MSG_HEADER                                          /*_H2ASN_Skip*/
    VOS_UINT32                          ulMsgId;            /*_H2ASN_Skip*/
    APP_MSG_HEADER
    VOS_UINT32                          ulOpId;             /*本次操作标识*/
    VOS_UINT32                          ulCid;              /*上下文序列号:0~31       */

    APP_ESM_PARA_SET_ENUM_UINT32        enSetType;          /*设置类型:0:设置,1:删除*/
    APP_ESM_PCO_INFO_STRU               stAppPcoInfo;       /*PCO消息*/
} APP_ESM_SET_PCO_REQ_STRU;

typedef  APP_ESM_PARA_SET_CNF_STRU APP_ESM_SET_PCO_CNF_STRU;

/*****************************************************************************
 结构名    : APP_ESM_SET_PDN_TYPE_REQ_STRU
 结构说明  : PDN_TYPE 参数设置
*****************************************************************************/
typedef struct
{
    VOS_MSG_HEADER                                          /*_H2ASN_Skip*/
    VOS_UINT32                          ulMsgId;            /*_H2ASN_Skip*/
    APP_MSG_HEADER
    VOS_UINT32                          ulOpId;
    VOS_UINT32                          ulCid;

    VOS_UINT32                          bitOpIpv4AddrAllocType    :1;
    VOS_UINT32                          bitOpSpare              :31;

    APP_ESM_PARA_SET_ENUM_UINT32        enSetType;          /*0:SET;1:DEL*/
    APP_ESM_PDN_TYPE_ENUM_UINT32        enPdnType;      /*1:IPV4,2:IPV6,3:BOTH*/
    APP_ESM_IPV4_ADDR_ALLOC_TYPE_ENUM_UINT32 enIpv4AddrAllocType;
}APP_ESM_SET_PDN_TYPE_REQ_STRU;
typedef  APP_ESM_PARA_SET_CNF_STRU APP_ESM_SET_PDN_TYPE_CNF_STRU;

/*****************************************************************************
 结构名    : APP_ESM_SET_BEARER_TYPE_REQ_STRU
 结构说明  : BEARER_TYPE  承载类型参数设置
*****************************************************************************/
typedef struct
{
    VOS_MSG_HEADER                                          /*_H2ASN_Skip*/
    VOS_UINT32                          ulMsgId;            /*_H2ASN_Skip*/
    APP_MSG_HEADER
    VOS_UINT32                          ulOpId;
    VOS_UINT32                          ulCid;

    VOS_UINT32                          bitOpLinkdCid       :1;
    VOS_UINT32                          bitOpSpare          :31;

    APP_ESM_PARA_SET_ENUM_UINT32        enSetType;
    APP_ESM_CID_TYPE_ENUM_UINT32        enBearCidType;

    VOS_UINT32                          ulLinkdCid;

}APP_ESM_SET_BEARER_TYPE_REQ_STRU;

typedef  APP_ESM_PARA_SET_CNF_STRU APP_ESM_SET_BEARER_TYPE_CNF_STRU;


/*****************************************************************************
 结构名    : APP_ESM_SET_PDP_MANAGER_TYPE_REQ_STRU
 结构说明  : 设置APP对承载激活的应答方式
*****************************************************************************/
typedef struct
{
    VOS_MSG_HEADER                                          /*_H2ASN_Skip*/
    VOS_UINT32                          ulMsgId;            /*_H2ASN_Skip*/
    APP_MSG_HEADER
    VOS_UINT32                          ulOpId;

    APP_ESM_PARA_SET_ENUM_UINT32        enSetType;
    APP_ESM_ANSWER_MODE_ENUM_UINT32     enAnsMode;          /*APP应答模式:0:自动,1:手动*/
    APP_ESM_ANSWER_RESULT_ENUM_UINT32   enAnsType;          /*APP应答类型:0:接收,1:拒绝,手动模式下此参数无效*/

} APP_ESM_SET_PDP_MANAGER_TYPE_REQ_STRU;

typedef struct
{
    VOS_MSG_HEADER                              /*_H2ASN_Skip*/
    VOS_UINT32              ulMsgId;            /*_H2ASN_Skip*/
    APP_MSG_HEADER
    VOS_UINT32              ulOpId;
    VOS_UINT32              ulSetRslt;
} APP_ESM_SET_PDP_MANAGER_TYPE_CNF_STRU;

/*****************************************************************************
 结构名    : APP_ESM_SET_GW_AUTH_REQ_STRU
 结构说明  : 设置网关鉴权数据信息
*****************************************************************************/
typedef struct
{
    APP_ESM_AUTH_TYPE_ENUM_UINT8        enGwAuthType;
    VOS_UINT8                           ucAccNumLen;
    VOS_UINT8                           aucRsv1[2];
    VOS_UINT8                           auAccessNum[APP_ESM_MAX_ACCESS_NUM_LEN];/*此参数保留，暂时不使用*/
    VOS_UINT8                           ucUserNameLen;
    VOS_UINT8                           aucRsv2[2];
    VOS_UINT8                           aucUserName[APP_ESM_MAX_USER_NAME_LEN + 1];
    VOS_UINT8                           ucPwdLen;
    VOS_UINT8                           aucRsv3[2];
    VOS_UINT8                           aucPwd[APP_ESM_MAX_PASSWORD_LEN + 1];
}APP_ESM_GW_AUTH_INFO_STRU;

/*****************************************************************************
 结构名    : APP_ESM_SDF_PARA_STRU
 结构说明  : APP与ESM 业务流SDF参数的数据结构
*****************************************************************************/
typedef struct
{
    VOS_UINT32                          bitOpPdnType         : 1;
    VOS_UINT32                          bitOpApn             : 1;
    VOS_UINT32                          bitOpSdfQos          : 1;
    VOS_UINT32                          bitOpLinkdCId        : 1;
    VOS_UINT32                          bitIpv4AddrAllocType : 1;
    VOS_UINT32                          bitOpGwAuthInfo      : 1;

    VOS_UINT32                          bitOpPcscfDiscovery  : 1;
    VOS_UINT32                          bitOpImsCnSignalFlag : 1;
    VOS_UINT32                          bitOpSpare           : 24;

    APP_ESM_PDN_TYPE_ENUM_UINT32        enPdnType;
    APP_ESM_IPV4_ADDR_ALLOC_TYPE_ENUM_UINT32 enIpv4AddrAllocType;
    APP_ESM_BEARER_TYPE_ENUM_UINT32      enBearerCntxtType;  /* SM上下文对应承载的类型 */

    VOS_UINT32                          ulCId;              /* 承载上下文ID */
    VOS_UINT32                          ulLinkdCId;         /*关联CID*/

    VOS_UINT32                          ulPfNum;
    APP_ESM_EPS_QOS_INFO_STRU           stSdfQosInfo;
    APP_ESM_APN_INFO_STRU               stApnInfo;
    APP_ESM_GW_AUTH_INFO_STRU           stGwAuthInfo;
    APP_ESM_TFT_INFO_STRU               astCntxtTftInfo[APP_ESM_MAX_SDF_PF_NUM];

    APP_ESM_PCSCF_DISCOVERY_ENUM_UINT32 enPcscfDiscovery;
    APP_ESM_IMS_CN_SIG_FLAG_ENUM_UINT32 enImsCnSignalFlag;
}APP_ESM_SDF_PARA_STRU;

/*lint -save -e959*/
typedef struct
{
    VOS_MSG_HEADER                                          /*_H2ASN_Skip*/
    VOS_UINT32                          ulMsgId;            /*_H2ASN_Skip*/
    APP_MSG_HEADER
    VOS_UINT32                          ulOpId;
    VOS_UINT32                          ulCid;

    APP_ESM_PARA_SET_ENUM_UINT32        enSetType;
    APP_ESM_GW_AUTH_INFO_STRU           stGwAuthInfo;
}APP_ESM_SET_GW_AUTH_REQ_STRU;
/*lint -restore*/

typedef  APP_ESM_PARA_SET_CNF_STRU APP_ESM_SET_GW_AUTH_CNF_STRU;


/*****************************************************************************
*                                                                            *
*                            参数查询消息结构                                *
*                                                                            *
******************************************************************************/

/*****************************************************************************
 结构名    : APP_ESM_INQ_PARA_REQ_STRU
 结构说明  : 参数查询通用结构
*****************************************************************************/
typedef struct
{
    VOS_MSG_HEADER                                          /*_H2ASN_Skip*/
    VOS_UINT32                          ulMsgId;            /*_H2ASN_Skip*/
    APP_MSG_HEADER
    VOS_UINT32                          ulOpId;
    VOS_UINT32                          ulCid;
}APP_ESM_INQ_PARA_REQ_STRU;

typedef APP_ESM_INQ_PARA_REQ_STRU APP_ESM_INQ_TFT_REQ_STRU;

/*****************************************************************************
 结构名    : APP_ESM_INQ_TFT_CNF_STRU
 结构说明  : 参数查询:TFT
*****************************************************************************/
typedef struct
{
    VOS_MSG_HEADER                                          /*_H2ASN_Skip*/
    VOS_UINT32                          ulMsgId;            /*_H2ASN_Skip*/
    APP_MSG_HEADER
    VOS_UINT32                  ulOpId;
    VOS_UINT32                  ulCid;
    VOS_UINT32                  ulRslt;
    VOS_UINT32                  ulPfNum;           /*有效的PF数*/
    APP_ESM_TFT_INFO_STRU       astPfInfo[APP_ESM_TFT_MAX_PF_NUM];
}APP_ESM_INQ_TFT_CNF_STRU;

typedef APP_ESM_INQ_PARA_REQ_STRU APP_ESM_INQ_QOS_REQ_STRU;

/*****************************************************************************
 结构名    : APP_ESM_INQ_QOS_CNF_STRU
 结构说明  : 参数查询:QOS
*****************************************************************************/
typedef struct
{
    VOS_MSG_HEADER                                          /*_H2ASN_Skip*/
    VOS_UINT32                          ulMsgId;            /*_H2ASN_Skip*/
    APP_MSG_HEADER
    VOS_UINT32                          ulOpId;
    VOS_UINT32                          ulCid;
    VOS_UINT32                          ulRslt;
    APP_ESM_QOS_INFO_STRU               stAppQoSInfo;       /*APP_ESM_QOS_INFO_STRU结构定义*/
}APP_ESM_INQ_QOS_CNF_STRU;


typedef APP_ESM_INQ_PARA_REQ_STRU APP_ESM_INQ_PCO_REQ_STRU;

/*****************************************************************************
 结构名    : APP_ESM_INQ_PCO_CNF_STRU
 结构说明  : 参数查询:PCO
*****************************************************************************/
typedef struct
{
    VOS_MSG_HEADER                                          /*_H2ASN_Skip*/
    VOS_UINT32                          ulMsgId;            /*_H2ASN_Skip*/
    APP_MSG_HEADER
    VOS_UINT32                          ulOpId;
    VOS_UINT32                          ulCid;
    VOS_UINT32                          ulRslt;
    APP_ESM_PCO_INFO_STRU               stAppPcoInfo;       /*PCO消息*/
} APP_ESM_INQ_PCO_CNF_STRU;


typedef APP_ESM_INQ_PARA_REQ_STRU APP_ESM_INQ_BEARER_TYPE_REQ_STRU;

/*****************************************************************************
 结构名    : APP_ESM_INQ_BEARER_TYPE_CNF_STRU
 结构说明  : 参数查询:BEARER_TYPE
*****************************************************************************/
typedef struct
{
    VOS_MSG_HEADER                                          /*_H2ASN_Skip*/
    VOS_UINT32                          ulMsgId;            /*_H2ASN_Skip*/
    APP_MSG_HEADER
    VOS_UINT32                          ulOpId;
    VOS_UINT32                          ulCid;
    VOS_UINT32                          bitOpLinkdCid       :1;
    VOS_UINT32                          bitOpSpare          :31;

    VOS_UINT32                          ulRslt;
    APP_ESM_CID_TYPE_ENUM_UINT32        enBearCidType;
    VOS_UINT32                          ulLinkdCid;         /*关联承载的CID*/

} APP_ESM_INQ_BEARER_TYPE_CNF_STRU;


typedef APP_ESM_INQ_PARA_REQ_STRU APP_ESM_INQ_PDN_TYPE_REQ_STRU;

/*****************************************************************************
 结构名    : APP_ESM_INQ_PDN_TYPE_CNF_STRU
 结构说明  : 参数查询:PDN_TYPE
*****************************************************************************/

typedef struct
{
    VOS_MSG_HEADER                                                  /*_H2ASN_Skip*/
    VOS_UINT32                                  ulMsgId;            /*_H2ASN_Skip*/
    APP_MSG_HEADER
    VOS_UINT32                                  ulOpId;
    VOS_UINT32                                  ulCid;

    VOS_UINT32                                  bitOpIpv4AddrAllocType    :1;
    VOS_UINT32                                  bitOpSpare                :31;

    VOS_UINT32                                  ulRslt;
    APP_ESM_PDN_TYPE_ENUM_UINT32                enPdnType;          /*1:IPV4,2:IPV6,3:BOTH*/
    APP_ESM_IPV4_ADDR_ALLOC_TYPE_ENUM_UINT32    enIpv4AddrAllocType;
} APP_ESM_INQ_PDN_TYPE_CNF_STRU;

typedef APP_ESM_INQ_PARA_REQ_STRU APP_ESM_INQ_APN_REQ_STRU;

/*****************************************************************************
 结构名    : APP_ESM_INQ_APN_TYPE_CNF_STRU
 结构说明  : 参数查询:APN
*****************************************************************************/
typedef struct
{
    VOS_MSG_HEADER                                          /*_H2ASN_Skip*/
    VOS_UINT32                          ulMsgId;            /*_H2ASN_Skip*/
    APP_MSG_HEADER
    VOS_UINT32                          ulOpId;
    VOS_UINT32                          ulCid;

    VOS_UINT32                          ulRslt;
    APP_ESM_APN_INFO_STRU               stApnInfo;
} APP_ESM_INQ_APN_CNF_STRU;

typedef APP_ESM_INQ_PARA_REQ_STRU APP_ESM_INQ_EPS_QOS_REQ_STRU;

/*****************************************************************************
 结构名    : APP_ESM_INQ_EPS_QOS_CNF_STRU
 结构说明  : 参数查询:SDF QOS
*****************************************************************************/
typedef struct
{
    VOS_MSG_HEADER                                          /*_H2ASN_Skip*/
    VOS_UINT32                          ulMsgId;            /*_H2ASN_Skip*/
    APP_MSG_HEADER
    VOS_UINT32                          ulOpId;
    VOS_UINT32                          ulCid;
    VOS_UINT32                          ulRslt;

    APP_ESM_EPS_QOS_INFO_STRU           stSdfQosInfo;
} APP_ESM_INQ_EPS_QOS_CNF_STRU;

/*****************************************************************************
 结构名    : APP_ESM_INQ_PDP_MANAGER_TYPE_REQ_STRU
 结构说明  : 参数查询:BEARER_MANAGER_TYPE
*****************************************************************************/
typedef struct
{
    VOS_MSG_HEADER                                          /*_H2ASN_Skip*/
    VOS_UINT32               ulMsgId;                       /*_H2ASN_Skip*/
    APP_MSG_HEADER
    VOS_UINT32               ulOpId;
} APP_ESM_INQ_PDP_MANAGER_TYPE_REQ_STRU;

/*****************************************************************************
 结构名    : APP_ESM_INQ_PDP_MANAGER_TYPE_CNF_STRU
 结构说明  : 参数查询:BEARER_MANAGER_TYPE
*****************************************************************************/
typedef struct
{
    VOS_MSG_HEADER                                          /*_H2ASN_Skip*/
    VOS_UINT32                          ulMsgId;            /*_H2ASN_Skip*/
    APP_MSG_HEADER
    VOS_UINT32                          ulOpId;

    VOS_UINT32                          ulRslt;
    APP_ESM_ANSWER_MODE_ENUM_UINT32     enAnsMode;          /*APP应答模式:0:自动,1:手动*/
    APP_ESM_ANSWER_RESULT_ENUM_UINT32   enAnsType;          /*APP应答类型:0:接收,1:拒绝,手动模式下此参数无效*/
} APP_ESM_INQ_PDP_MANAGER_TYPE_CNF_STRU;

/*****************************************************************************
 结构名    : APP_ESM_INQ_BEAER_QOS_REQ_STRU
 结构说明  : 参数查询:BEARER_QOS
*****************************************************************************/
typedef struct
{
    VOS_MSG_HEADER                                  /*_H2ASN_Skip*/
    VOS_UINT32 ulMsgId;    /*_H2ASN_Skip*/

    APP_MSG_HEADER
    VOS_UINT32                             ulEpsbId;
} APP_ESM_INQ_BEAER_QOS_REQ_STRU;

/*****************************************************************************
 结构名    : APP_ESM_INQ_BEAER_QOS_CNF_STRU
 结构说明  : 参数查询:BEARER_QOS
*****************************************************************************/
typedef struct
{
    VOS_MSG_HEADER                                  /*_H2ASN_Skip*/
    VOS_UINT32 ulMsgId;    /*_H2ASN_Skip*/

    APP_MSG_HEADER
    VOS_UINT32                             ulEpsbIdNum;
    APP_EPS_QOS_STRU                       astQosInfo[APP_ESM_MAX_EPSB_NUM];
    APP_ESM_ANSWER_RESULT_ENUM_UINT32      enAnsType;
}APP_ESM_INQ_BEAER_QOS_CNF_STRU;

typedef struct
{
    VOS_MSG_HEADER                                  /*_H2ASN_Skip*/
    VOS_UINT32 ulMsgId;    /*_H2ASN_Skip*/

    APP_MSG_HEADER
    VOS_UINT32                             ulEpsbId;
    APP_ESM_EPS_QOS_INFO_STRU              stQosInfo;
}APP_ESM_INQ_BEAER_QOS_IND_STRU;

typedef APP_ESM_INQ_PARA_REQ_STRU APP_ESM_INQ_DYNAMIC_PDP_CONT_REQ_STRU;

/*****************************************************************************
 结构名    : APP_ESM_IP_ADDR_STRU
 结构说明  : IP地址的结构体
*****************************************************************************/
typedef struct
{
    VOS_UINT8                           ucIpType;
    VOS_UINT8                           ucReserved[3];
    VOS_UINT8                           aucIpV4Addr[APP_MAX_IPV4_ADDR_LEN];
    VOS_UINT8                           aucIpV6Addr[APP_MAX_IPV6_ADDR_LEN];
}APP_ESM_IP_ADDR_STRU;

/*****************************************************************************
 结构名    : APP_ESM_INQ_DYNAMIC_PDP_CONT_CNF_STRU
 结构说明  : 查询回复:动态PDP上下文信息
*****************************************************************************/
typedef struct
{
    VOS_MSG_HEADER                                          /*_H2ASN_Skip*/
    VOS_UINT32                          ulMsgId;            /*_H2ASN_Skip*/
    APP_MSG_HEADER
    VOS_UINT32                          ulOpId;
    VOS_UINT32                          ulCid;

    VOS_UINT32                          bitOpPdnAddr         :1;
    VOS_UINT32                          bitOpApn             :1;
    VOS_UINT32                          bitOpLinkCid         :1;
    VOS_UINT32                          bitOpDnsPrim         :1;
    VOS_UINT32                          bitOpDnsSec          :1;
    VOS_UINT32                          bitOpPCscfPrim       :1;
    VOS_UINT32                          bitOpPCscfSec        :1;
    VOS_UINT32                          bitOpGateWayAddrInfo :1;
    VOS_UINT32                          bitOpImsCnSignalFlag :1;
    VOS_UINT32                          bitOpIpv4Mtu         : 1;
    VOS_UINT32                          bitOpSpare           :22;

    VOS_UINT32                          ulRslt;
    VOS_UINT32                          ulEpsbId;
    APP_ESM_IP_ADDR_STRU                stPDNAddrInfo;
    APP_ESM_IP_ADDR_STRU                stSubnetMask;
    APP_ESM_IP_ADDR_STRU                stGateWayAddrInfo;
    APP_ESM_IP_ADDR_STRU                stDnsPrimAddrInfo;  /*主DNS信息 */
    APP_ESM_IP_ADDR_STRU                stDnsSecAddrInfo;   /*辅DNS信息 */
    APP_ESM_IP_ADDR_STRU                stPCscfPrimAddrInfo;/*主P-CSCF信息 */
    APP_ESM_IP_ADDR_STRU                stPCscfSecAddrInfo; /* 辅P-CSCF信息 */
    APP_ESM_APN_INFO_STRU               stApnInfo;
    VOS_UINT32                          ulLinkCid;
    APP_ESM_IMS_CN_SIG_FLAG_ENUM_UINT32 enImsCnSignalFlag;
    VOS_UINT16                          usIpv4Mtu;
    VOS_UINT8                           aucReserved[2];
} APP_ESM_INQ_DYNAMIC_PDP_CONT_CNF_STRU;

typedef APP_ESM_INQ_PARA_REQ_STRU APP_ESM_INQ_DYNAMIC_EPS_QOS_REQ_STRU;

/*****************************************************************************
 结构名    : APP_ESM_INQ_DYNAMIC_EPS_QOS_CNF_STRU
 结构说明  : 查询回复:动态EPS QoS信息
*****************************************************************************/
typedef struct
{
    VOS_MSG_HEADER                                           /*_H2ASN_Skip*/
    VOS_UINT32                          ulMsgId;             /*_H2ASN_Skip*/
    APP_MSG_HEADER
    VOS_UINT32                          ulOpId;
    VOS_UINT32                          ulCid;

    VOS_UINT32                          ulRslt;
    APP_ESM_EPS_QOS_INFO_STRU           stSdfQosInfo;
} APP_ESM_INQ_DYNAMIC_EPS_QOS_CNF_STRU;

typedef APP_ESM_INQ_PARA_REQ_STRU APP_ESM_INQ_DYNAMIC_TFT_REQ_STRU;

/*****************************************************************************
 结构名    : APP_ESM_INQ_DYNAMIC_TFT_CNF_STRU
 结构说明  : 查询回复:动态TFT信息
*****************************************************************************/
typedef struct
{
    VOS_MSG_HEADER                                           /*_H2ASN_Skip*/
    VOS_UINT32                          ulMsgId;             /*_H2ASN_Skip*/
    APP_MSG_HEADER
    VOS_UINT32                          ulOpId;
    VOS_UINT32                          ulCid;

    VOS_UINT32                          ulRslt;
    VOS_UINT32                          ulPfNum;             /*有效的PF数*/
    APP_ESM_TFT_INFO_STRU               astPfInfo[APP_ESM_TFT_MAX_PF_NUM];
} APP_ESM_INQ_DYNAMIC_TFT_CNF_STRU;


typedef APP_ESM_INQ_PARA_REQ_STRU APP_ESM_INQ_GW_AUTH_REQ_STRU;

/*****************************************************************************
 结构名    : APP_ESM_INQ_GW_AUTH_CNF_STRU
 结构说明  : 查询回复:网关鉴权信息
*****************************************************************************/
/*lint -save -e959*/
typedef struct
{
    VOS_MSG_HEADER                                           /*_H2ASN_Skip*/
    VOS_UINT32                          ulMsgId;             /*_H2ASN_Skip*/
    APP_MSG_HEADER
    VOS_UINT32                          ulOpId;
    VOS_UINT32                          ulCid;
    VOS_UINT32                          ulRslt;
    APP_ESM_GW_AUTH_INFO_STRU           stGwAuthInfo;
} APP_ESM_INQ_GW_AUTH_CNF_STRU;
/*lint -restore*/

/*****************************************************************************
*                                                                            *
*                            承载建立、释放结构                              *
*                                                                            *
******************************************************************************/
/*****************************************************************************
 结构名称: APP_ESM_CHAP_PARAM_STRU
 结构说明: The struct contains the parameter of CHAP.
           ulChallengeLen     - Length of username
           aucChallenge       - Array of challenge
           ulChallengeNameLen - Length of challenge name
           aucChallengeName   - Array of challenge name
           ulResponseLen      - Length of response
           aucResponse        - Array of response
           ulResponseNameLen  - Length of response name (username)
           aucResponseName    - Array of response name (username)
*****************************************************************************/
typedef struct
{
    VOS_UINT32                          ulChallengeLen;
    VOS_UINT8                           aucChallenge[APP_ESM_PPP_CHAP_CHALLNGE_LEN];
    VOS_UINT32                          ulChallengeNameLen;
    VOS_UINT8                           aucChallengeName[APP_ESM_PPP_CHAP_CHALLNGE_NAME_LEN];
    VOS_UINT32                          ulResponseLen;
    VOS_UINT8                           aucResponse[APP_ESM_PPP_CHAP_RESPONSE_LEN];
    VOS_UINT8                           ucResponseNameLen;
    VOS_UINT8                           aucResponseName[APP_ESM_PPP_MAX_USERNAME_LEN];

} APP_ESM_CHAP_PARAM_STRU;

/*****************************************************************************
 结构名称: APP_ESM_PAP_PARAM_STRU
 结构说明: The struct contains the parameter of PAP.
           ucUsernameLen - Length of username
           aucUsername   - Array of username
           ucPasswordLen - Length of password
           aucPassword   - Array of password
*****************************************************************************/
typedef struct
{
    VOS_UINT8                           ucUsernameLen;
    VOS_UINT8                           aucUsername[APP_ESM_PPP_MAX_USERNAME_LEN];
    VOS_UINT8                           ucPasswordLen;
    VOS_UINT8                           aucPassword[APP_ESM_PPP_MAX_PASSWORD_LEN];

} APP_ESM_PAP_PARAM_STRU;

/*****************************************************************************
 结构名称: APP_ESM_AUTH_INFO_STRU
 结构说明: The struct contains the authentication information.
           enAuthType  - Auth Type (NONE/PAP/CHAP)
           stChapParam - Parameters of CHAP
           stPapParam  - Parameters of PAP
*****************************************************************************/
typedef struct
{
    APP_ESM_AUTH_TYPE_ENUM_UINT8        enAuthType;
    VOS_UINT8                           ucAuthId;
    VOS_UINT8                           aucRsv[2];
    union
    {
        APP_ESM_CHAP_PARAM_STRU         stChapParam;
        APP_ESM_PAP_PARAM_STRU          stPapParam;
    } unAuthParam;

} APP_ESM_AUTH_INFO_STRU;

/*****************************************************************************
 结构名称: APP_ESM_IPCP_INFO_STRU
 结构说明: The struct contains the IPCP information.
           aucPrimDns  - Primary DNS address
           aucSecDns   - Secondary DNS address
           aucPrimNbns - Primary NBNS address
           aucSecNbns  - Secondary NBNS address
*****************************************************************************/
typedef struct
{
    VOS_UINT32                          bitOpIpAddr         :1;
    VOS_UINT32                          bitOpPrimDns        :1;
    VOS_UINT32                          bitOpSecDns         :1;
    VOS_UINT32                          bitOpPrimNbns       :1;
    VOS_UINT32                          bitOpSecNbns        :1;
    VOS_UINT32                          bitOpSpare          :27;

    VOS_UINT8                           aucIpAddr[APP_MAX_IPV4_ADDR_LEN];
    VOS_UINT8                           aucPrimDns[APP_MAX_IPV4_ADDR_LEN];
    VOS_UINT8                           aucSecDns[APP_MAX_IPV4_ADDR_LEN];
    VOS_UINT8                           aucPrimNbns[APP_MAX_IPV4_ADDR_LEN];
    VOS_UINT8                           aucSecNbns[APP_MAX_IPV4_ADDR_LEN];

} APP_ESM_IPCP_INFO_STRU;


/*****************************************************************************
 结构名称: APP_ESM_PPP_INFO_STRU
 结构说明: The struct contains the PPP information.
           stAuthInfo - Authentication information
           stIpcpInfo - IPCP information
*****************************************************************************/
typedef struct
{
    APP_ESM_AUTH_INFO_STRU              stAuthInfo;       /*PPP拨号鉴权信息*/
    APP_ESM_IPCP_INFO_STRU              stIpcpInfo;       /*PPP拨号IPCP信息，ESM不使用 */

} APP_ESM_PPP_INFO_STRU;
typedef struct
{
    VOS_UINT32                           ulEpsbId;
    NAS_ESM_DT_BEARER_TYPE_ENUM_UINT8    enBearerType; /*承载类型*/
    VOS_UINT8                            ucRsv[3];
 	APP_ESM_APN_INFO_STRU                stApn;         /*APN信息 */
    APP_ESM_IP_ADDR_STRU                 stIpAddr;      /*UE IP地址信息 */
    APP_ESM_EPS_QOS_INFO_STRU            stSdfQosInfo;  /*QoS参数 */
} NAS_OM_EPSB_INFO_STRU;

typedef struct
{
    VOS_UINT32                ulActEpsbNum;                /*激活承载的个数 */
    NAS_OM_EPSB_INFO_STRU     stEpsbInfo[APP_ESM_MAX_EPSB_NUM];
}NAS_OM_ACT_PDP_INFO_STRU;


typedef struct
{
    VOS_MSG_HEADER                                  /*_H2ASN_Skip*/
    VOS_UINT32                          ulMsgId;    /*_H2ASN_Skip*/

    APP_MSG_HEADER
    VOS_UINT32                          ulOpId;

    DT_CMD_ENUM_UINT8                   enCmd;
    VOS_UINT8                           ucRsv[3];
    VOS_UINT32                          ulRptPeriod;
}APP_ESM_DT_INQ_CMD_REQ_STRU;

typedef   APP_ESM_DT_INQ_CMD_REQ_STRU APP_ESM_INQ_PDP_INFO_REQ_STRU;

typedef struct
{
    VOS_MSG_HEADER                                           /*_H2ASN_Skip*/
    VOS_UINT32                          ulMsgId;             /*_H2ASN_Skip*/
    APP_MSG_HEADER
    VOS_UINT32                          ulOpId;
    VOS_UINT32                          ulRslt;
} APP_ESM_INQ_PDP_INFO_CNF_STRU;

typedef struct
{
    VOS_MSG_HEADER                                           /*_H2ASN_Skip*/
    VOS_UINT32                          ulMsgId;             /*_H2ASN_Skip*/
    APP_MSG_HEADER
    VOS_UINT32                          ulOpId;

    NAS_OM_ACT_PDP_INFO_STRU            stActPdpInfo;
} APP_ESM_INQ_PDP_INFO_IND_STRU;



/*****************************************************************************
*                                                                            *
*                            承载建立、释放结构                              *
*                                                                            *
******************************************************************************/

/*****************************************************************************
 结构名称   : APP_ESM_CUSTOM_PCO_CONTAINER_STRU
 协议表格   : PCO信元格式参考24008-10.5.6.3
              PCO-FF00H项的格式:
              Container Identifier     -FF00H(2 bytes)
              Container Content Length -MAX 50 octets(1 bytes)
              Container Content Format -MCCMNC(VZW-311480, 3 octets),
                                        ACTION(value:0-5;  6-255:reserved)
              4字节对齐
 ASN.1 描述 :
 结构说明   : ESM -> APS
*****************************************************************************/
typedef struct
{
    VOS_UINT16                          usContainerId;
    VOS_UINT8                           ucContentLen;
    VOS_UINT8                           aucContents[APP_ESM_MAX_CUSTOM_PCO_CONTAINER_CONTENT_LEN];
}APP_ESM_CUSTOM_PCO_CONTAINER_STRU;

/*****************************************************************************
 结构名称   : APP_ESM_CUSTOM_PCO_INFO_STRU
 协议表格   : PCO信元中的运营商定制项，PCO信元格式参考24008-10.5.6.3，
              除VERIZON定制项外,预留两项预备其他运营商定制用
              定制列表中的3项默认第一项为FF00H
 ASN.1 描述 :
 结构说明   : ESM -> APS
*****************************************************************************/
typedef struct
{
    VOS_UINT32                                  ulContainerNum;
    APP_ESM_CUSTOM_PCO_CONTAINER_STRU           astContainerList[APP_ESM_MAX_CUSTOM_PCO_CONTAINER_NUM];
}APP_ESM_CUSTOM_PCO_INFO_STRU;
typedef struct
{
    VOS_UINT8                           aucIpV4Addr[APP_MAX_IPV4_ADDR_LEN];
}APP_ESM_IPV4_EPDG_STRU;

typedef struct
{
    VOS_UINT8                           aucIpV6Addr[APP_MAX_IPV6_ADDR_LEN];
}APP_ESM_IPV6_EPDG_STRU;

/*****************************************************************************
 结构名称   : APP_ESM_EPDG_INFO_STRU
 协议表格   : R1定制EPDG地址信息
 ASN.1 描述 :
 结构说明   : ESM -> TAF
*****************************************************************************/
typedef struct
{
    VOS_UINT16                          usIpv4EpdgNum;
    VOS_UINT16                          usIpv6EpdgNum;
    APP_ESM_IPV4_EPDG_STRU              astIpv4EpdgList[APP_ESM_MAX_IPV4_EPDG_NUM];
    APP_ESM_IPV6_EPDG_STRU              astIpv6EpdgList[APP_ESM_MAX_IPV6_EPDG_NUM];
}APP_ESM_EPDG_INFO_STRU;


/*****************************************************************************
结构名    : APP_ESM_PDP_SETUP_REQ_STRU
结构说明  : APP发起承载建立请求消息
*****************************************************************************/
typedef struct
{
    VOS_MSG_HEADER                                          /*_H2ASN_Skip*/
    VOS_UINT32                          ulMsgId;            /*_H2ASN_Skip*/
    APP_MSG_HEADER
    VOS_UINT32                          ulOpId;             /*本次操作标识符*/

    APP_ESM_PDP_SETUP_TYPE_ENUM_UINT32  enSetupType;        /* 正常PDP激活还是PPP拨号导致 */

    VOS_UINT32                          ulCid;              /*上下文序列号，范围:0~31*/
    APP_ESM_PPP_INFO_STRU               stPppInfo;          /* PPP拨号参数信息*/
    APP_ESM_BEARER_PRIO_ENUM_UINT32     enBearerPrio;       /* NAS signalling low priority标识 */
    VOS_UINT32                          ulPsCallId;         /*呼叫实体ID，范围:0~31*/
}APP_ESM_PDP_SETUP_REQ_STRU;


/*****************************************************************************
 结构名    : APP_ESM_PDP_SETUP_CNF_STRU
 结构说明  : 承载建立回复消息
*****************************************************************************/
typedef struct
{
    VOS_MSG_HEADER                                          /*_H2ASN_Skip*/
    VOS_UINT32                          ulMsgId;            /*_H2ASN_Skip*/
    APP_MSG_HEADER
    VOS_UINT32                          ulOpId;
    VOS_UINT32                          ulCid;

    VOS_UINT32                          bitOpLinkCid         :1;
    VOS_UINT32                          bitOpBearerState     :1;
    VOS_UINT32                          bitOpBearerType      :1;
    VOS_UINT32                          bitOpPdnAddr         :1;
    VOS_UINT32                          bitOpRabId           :1;
    VOS_UINT32                          bitOpApn             :1;
    VOS_UINT32                          bitOpSdfQos          :1;
    VOS_UINT32                          bitOpDnsPrim         :1;
    VOS_UINT32                          bitOpDnsSec          :1;
    VOS_UINT32                          bitOpPCscfPrim       :1;
    VOS_UINT32                          bitOpPCscfSec        :1;
    VOS_UINT32                          bitOpGateWayAddrInfo :1;
    VOS_UINT32                          bitOpCustomPco       :1; /* 用于VERRIZON定制需求中,把FF00H上报给APS时,指示FF00H是否需要上报。 1:需要 0:不需要 */

    VOS_UINT32                          bitOpEpdgInfo        : 1; /* 用于R1定制需求中,把EPDG地址上报给APS时,指示是否要上报给IMSA。 1:需要 0:不需要 */
    VOS_UINT32                          bitOpSpare           : 18;

    VOS_UINT32                          ulRslt;       /*删除操作结果;取值范围:参见附录3.1*/
    VOS_UINT32                          ulLinkCid;
    VOS_UINT32                          ulPsCallId;         /*呼叫实体ID，范围:0~31*/
    APP_ESM_BEARER_STATE_ENUM_UINT32    enBearerState;      /*当前承载状态:0~1*/
    APP_ESM_BEARER_TYPE_ENUM_UINT32     enBearerType;       /*承载类型*/
    APP_ESM_IP_ADDR_STRU                stPDNAddrInfo;
    APP_ESM_IP_ADDR_STRU                stSubnetMask;
    APP_ESM_IP_ADDR_STRU                stGateWayAddrInfo;  /*网关信息*/
    APP_ESM_IP_ADDR_STRU                stDnsPrimAddrInfo;  /*主DNS信息 */
    APP_ESM_IP_ADDR_STRU                stDnsSecAddrInfo;   /*辅DNS信息 */
    APP_ESM_IP_ADDR_STRU                stPCscfPrimAddrInfo;/*主P-CSCF信息 */
    APP_ESM_IP_ADDR_STRU                stPCscfSecAddrInfo; /* 辅P-CSCF信息 */
    VOS_UINT32                          ulRabId;            /*用户面承载标识，数传时使用，其值暂时为ulEpsBid*/
    APP_ESM_APN_INFO_STRU               stApnInfo;
    APP_ESM_EPS_QOS_INFO_STRU           stSdfQosInfo;

    VOS_UINT8                           ucExpiredCount;     /* 发起激活的超时次数 */
    VOS_UINT8                           aucReserved[3];
    APP_ESM_CUSTOM_PCO_INFO_STRU        stCustomPcoInfo;
    APP_ESM_EPDG_INFO_STRU               stEpdgInfo;
} APP_ESM_PDP_SETUP_CNF_STRU;


typedef APP_ESM_PDP_SETUP_CNF_STRU APP_ESM_PDP_SETUP_IND_STRU;

/*****************************************************************************
 结构名    : APP_ESM_PDP_MODIFY_REQ_STRU
 结构说明  : APP发起承载修改请求消息
*****************************************************************************/
typedef struct
{
    VOS_MSG_HEADER                                          /*_H2ASN_Skip*/
    VOS_UINT32                          ulMsgId;            /*_H2ASN_Skip*/
    APP_MSG_HEADER
    VOS_UINT32                          ulOpId;             /*本次操作标识符*/
    VOS_UINT32                          ulCid;              /*上下文序列号，范围:0~31*/

    APP_ESM_BEARER_PRIO_ENUM_UINT32     enBearerPrio;       /* NAS signalling priority标识 */

    VOS_UINT32                          ulPsCallId;         /*呼叫实体ID，范围:0~31*/
}APP_ESM_PDP_MODIFY_REQ_STRU;

typedef struct
{
    VOS_MSG_HEADER                                          /*_H2ASN_Skip*/
    VOS_UINT32                          ulMsgId;            /*_H2ASN_Skip*/
    APP_MSG_HEADER
    VOS_UINT32                          ulOpId;
    VOS_UINT32                          ulCid;
    VOS_UINT32                          ulPsCallId;         /*呼叫实体ID，范围:0~31*/
    APP_ESM_BEARER_MODIFY_ENUM_UINT32   enBearerModifyType;

    VOS_UINT32                          bitOpBearerState    :1;
    VOS_UINT32                          bitOpBearerType     :1;
    VOS_UINT32                          bitOpRabId          :1;
    VOS_UINT32                          bitOpSdfQos         :1;
    VOS_UINT32                          bitOpCustomPco      :1; /* 用于VERRIZON定制需求中,把FF00H上报给APS时,指示FF00H是否需要上报。 1:需要 0:不需要 */

    VOS_UINT32                          bitOpSpare          :27;

    VOS_UINT32                          ulRslt ;            /*删除操作结果;取值范围:参见附录3.1*/

    APP_ESM_BEARER_STATE_ENUM_UINT32    enBearerState;      /*当前承载状态:0~1*/
    APP_ESM_BEARER_TYPE_ENUM_UINT32     enBearerType;       /*承载类型*/

    VOS_UINT32                          ulRabId;            /*用户面承载标识，数传时使用，其值暂时为ulEpsBid*/

    APP_ESM_EPS_QOS_INFO_STRU           stSdfQosInfo;

    APP_ESM_CUSTOM_PCO_INFO_STRU        stCustomPcoInfo;
} APP_ESM_PDP_MODIFY_CNF_STRU;

typedef APP_ESM_PDP_MODIFY_CNF_STRU APP_ESM_PDP_MODIFY_IND_STRU;


/*****************************************************************************
 结构名    : APP_ESM_PDP_RELEASE_REQ_STRU
 结构说明  : APP发起承载释放请求
*****************************************************************************/
typedef struct
{
    VOS_MSG_HEADER                                          /*_H2ASN_Skip*/
    VOS_UINT32                          ulMsgId;            /*_H2ASN_Skip*/
    APP_MSG_HEADER
    VOS_UINT32                          ulOpId;
    VOS_UINT32                          ulCid;
    VOS_UINT32                          ulPsCallId;         /*呼叫实体ID，范围:0~31*/

    APP_ESM_PDP_RELEASE_CAUSE_ENUM_UINT8          enCause;       /* 释放原因 */
    VOS_UINT8                                     ucDetachInd;   /* 是否需要执行DETACH，VOS_TRUE是，VOS_FALS否 */
    VOS_UINT8                                     ucRsv[2];      /* 保留 */

}APP_ESM_PDP_RELEASE_REQ_STRU;

/*****************************************************************************
 结构名    : APP_ESM_PDP_RELEASE_CNF_STRU
 结构说明  : 承载释放回复消息
*****************************************************************************/
typedef struct
{
    VOS_MSG_HEADER                                          /*_H2ASN_Skip*/
    VOS_UINT32                          ulMsgId;            /*_H2ASN_Skip*/
    APP_MSG_HEADER
    VOS_UINT32                          ulOpId;
    VOS_UINT32                          ulCid;
    VOS_UINT32                          ulPsCallId;         /*呼叫实体ID，范围:0~31*/

    VOS_UINT32                          bitOpLinkCid        :1;
    VOS_UINT32                          bitOpPdnAddr        :1;
    VOS_UINT32                          bitOpCustomPco      :1; /* 用于VERRIZON定制需求中,把FF00H上报给APS时,指示FF00H是否需要上报。 1:需要 0:不需要 */

    VOS_UINT32                          bitOpSpare          :29;

    VOS_UINT32                          ulRslt;


    VOS_UINT32                          ulLinkCid;
    APP_ESM_IP_ADDR_STRU                stPDNAddrInfo;

    /* 给APP上报RELEASE IND时(RELEASE CNF不填);    */
    VOS_UINT32                          ulEpsbId;

    APP_ESM_CUSTOM_PCO_INFO_STRU        stCustomPcoInfo;
}APP_ESM_PDP_RELEASE_CNF_STRU;

typedef APP_ESM_PDP_RELEASE_CNF_STRU APP_ESM_PDP_RELEASE_IND_STRU;

/*****************************************************************************
 结构名    : APP_ESM_PDP_MANAGER_IND_STRU
 结构说明  : 承载建立请求APP响应
*****************************************************************************/
typedef struct
{
    VOS_MSG_HEADER                                          /*_H2ASN_Skip*/
    VOS_UINT32                          ulMsgId;            /*_H2ASN_Skip*/
    APP_MSG_HEADER
    VOS_UINT32                                  ulOpId;
    VOS_UINT32                                  ulCid;
    VOS_UINT32                                  ulPsCallId;         /*呼叫实体ID，范围:0~31，上报前，先调用TAF_APS_GetUnusedPsCallId()接口获取*/
    VOS_UINT32                                  bitOpLinkCid      : 1;
    VOS_UINT32                                  bitOpModifyTpye   : 1;
    VOS_UINT32                                  bitOpSpare        : 30;

    VOS_UINT32                                  ulLinkCid;
    APP_ESM_BEARER_OPERATE_TYPE_ENUM_UINT32     enOperateType;
    APP_ESM_BEARER_MODIFY_ENUM_UINT32           enBearerModifyType;

}APP_ESM_PDP_MANAGER_IND_STRU;

/*****************************************************************************
 结构名    : APP_ESM_PDP_MANAGER_RSP_STRU
 结构说明  : APP响应承载建立请求
*****************************************************************************/
typedef struct
{
    VOS_MSG_HEADER                                          /*_H2ASN_Skip*/
    VOS_UINT32                          ulMsgId;            /*_H2ASN_Skip*/
    APP_MSG_HEADER
    VOS_UINT32                              ulOpId;
    VOS_UINT32                              ulCid;
    VOS_UINT32                              ulPsCallId;        /*呼叫实体ID，范围:0~31*/
    APP_ESM_BEARER_ACT_RESULT_ENUM_UINT32   ulSetupRslt;       /*响应结果:0:接收分配或修改请求,1:拒绝请求*/


}APP_ESM_PDP_MANAGER_RSP_STRU;

/*****************************************************************************
 结构名    : APP_ESM_IPV6_PREFIX_STRU
 结构说明  : APS-->ESM IPv6 Prefix结构
*****************************************************************************/

/* APS融合新增内容, IPV6前缀*/
typedef struct
{
    VOS_UINT8                           ucPrefixLen;
    VOS_UINT8                           aucReserved[3];

    VOS_UINT8                           aucPrefix[APP_ESM_IPV6_ADDR_LEN];
}APP_ESM_IPV6_PREFIX_STRU;

/*****************************************************************************
 结构名    : APP_ESM_IPV6_INFO_NOTIFY_STRU
 结构说明  : APS-->ESM IPv6 Notify
*****************************************************************************/
typedef struct
{
    VOS_MSG_HEADER                                      /*_H2ASN_Skip*/
    VOS_UINT32                          ulMsgId;        /*_H2ASN_Skip*/
    APP_MSG_HEADER
    VOS_UINT32                          ulOpId;
    VOS_UINT8                           ucEpsbId;
    VOS_UINT8                           ucIpv6PrefixNum;
    VOS_UINT8                           aucReserved[2];
    APP_ESM_IPV6_PREFIX_STRU            astIpv6PrefixArray[APP_ESM_MAX_IPV6_PREFIX_NUM];
}APP_ESM_IPV6_INFO_NOTIFY_STRU;

/*****************************************************************************
 结构名    : APP_ESM_PROCEDURE_ABORT_NOTIFY_STRU
 结构说明  : APS-->ESM 流程终止消息结构
*****************************************************************************/
typedef struct
{
    VOS_MSG_HEADER                                      /*_H2ASN_Skip*/
    VOS_UINT32                          ulMsgId;        /*_H2ASN_Skip*/
    APP_MSG_HEADER
    VOS_UINT32                          ulOpId;
    VOS_UINT32                          ulCid;
    VOS_UINT32                          ulPsCallId;     /*呼叫实体ID，范围:0~31*/
}APP_ESM_PROCEDURE_ABORT_NOTIFY_STRU;

/*****************************************************************************
 结构名    : APP_ESM_NDISCONN_REQ_STRU
 结构说明  : APS-->ESM 拨号消息结构
*****************************************************************************/
typedef struct
{
    VOS_MSG_HEADER                                          /*_H2ASN_Skip*/
    VOS_UINT32                                  ulMsgId;    /*_H2ASN_Skip*/
    APP_MSG_HEADER
    VOS_UINT32                                  ulOpId;
    VOS_UINT32                                  ulCid;
    VOS_UINT32                                  ulPsCallId;        /*呼叫实体ID，范围:0~31*/

    VOS_UINT32                                  bitOpPdnType           :1;
    VOS_UINT32                                  bitOpApn               :1;
    VOS_UINT32                                  bitOpGwAuth            :1;

    VOS_UINT32                                  bitOpIpv4AddrAlloc     :1;                /* Ipv4AddrAlloc*/
    VOS_UINT32                                  bitOpPcscfDiscovery    :1;                /* P-CSCF discovery */
    VOS_UINT32                                  bitOpImCnSignalFlg     :1;                /* IM CN Signalling Flag */
    VOS_UINT32                                  bitOpSpare             :26;


    APP_ESM_APN_INFO_STRU                       stApnInfo;
    APP_ESM_PDN_TYPE_ENUM_UINT32                enPdnType;
    APP_ESM_GW_AUTH_INFO_STRU                   stGwAuthInfo;
    APP_ESM_PDN_REQUEST_TYPE_ENUM_UINT32        enPdnReqestType;
    APP_ESM_PDP_EMC_IND_ENUM_UINT32             enEmergencyInd;
    APP_ESM_IPV4_ADDR_ALLOC_TYPE_ENUM_UINT32    enIpv4AddrAlloc;
    APP_ESM_PCSCF_DISCOVERY_ENUM_UINT32         enPcscfDiscovery;
    APP_ESM_IMS_CN_SIG_FLAG_ENUM_UINT32         enImCnSignalFlg;
    APP_ESM_BEARER_PRIO_ENUM_UINT32               enBearerPrio;
} APP_ESM_NDISCONN_REQ_STRU;


/*****************************************************************************
 结构名    : APP_ESM_NDISCONN_CNF_STRU
 结构说明  : 回复APS-->ESM 拨号消息结构
*****************************************************************************/
typedef struct
{
    VOS_MSG_HEADER                                          /*_H2ASN_Skip*/
    VOS_UINT32                                  ulMsgId;            /*_H2ASN_Skip*/
    APP_MSG_HEADER
    VOS_UINT32                                  ulOpId;
    VOS_UINT32                                  ulCid;
    VOS_UINT32                                  ulPsCallId;        /*呼叫实体ID，范围:0~31*/

    VOS_UINT32                                  bitOpBearerState    :1;
    VOS_UINT32                                  bitOpBearerType     :1;
    VOS_UINT32                                  bitOpPdnAddr        :1;
    VOS_UINT32                                  bitOpRabId          :1;
    VOS_UINT32                                  bitOpApn            :1;
    VOS_UINT32                                  bitOpSdfQos         :1;
    VOS_UINT32                                  bitOpDnsPrim        :1;
    VOS_UINT32                                  bitOpDnsSec         :1;
    VOS_UINT32                                  bitOpPCscfPrim      :1;
    VOS_UINT32                                  bitOpPCscfSec       :1;
    VOS_UINT32                                  bitOpGateWayAddrInfo    :1;
    VOS_UINT32                                  bitOpCustomPco      :1; /* 用于VERRIZON定制需求中,把FF00H上报给APS时,指示FF00H是否需要上报。 1:需要 0:不需要 */

    VOS_UINT32                                  bitOpEpdgInfo        : 1; /* 用于R1定制需求中,把EPDG地址上报给APS时,指示是否上报给IMSA。 1:需要 0:不需要 */
    VOS_UINT32                                  bitOpSpare           : 19;

    VOS_UINT32                                  ulRslt;       /*取值范围:参见附录3.1*/

    APP_ESM_BEARER_STATE_ENUM_UINT32            enBearerState;      /*当前承载状态:0~1*/
    APP_ESM_BEARER_TYPE_ENUM_UINT32             enBearerType;       /*承载类型*/

    APP_ESM_IP_ADDR_STRU                        stPDNAddrInfo;
    APP_ESM_IP_ADDR_STRU                        stSubnetMask;
    APP_ESM_IP_ADDR_STRU                        stGateWayAddrInfo;
    APP_ESM_IP_ADDR_STRU                        stDnsPrimAddrInfo;   /*主DNS信息*/
    APP_ESM_IP_ADDR_STRU                        stDnsSecAddrInfo;    /*辅DNS信息*/
    APP_ESM_IP_ADDR_STRU                        stPCscfPrimAddrInfo; /*主P-CSCF信息*/
    APP_ESM_IP_ADDR_STRU                        stPCscfSecAddrInfo;  /*辅P-CSCF信息*/
    VOS_UINT32                                  ulRabId;            /*用户面承载标识，数传时使用，其值为ulEpsbId*/
    APP_ESM_APN_INFO_STRU                       stApnInfo;
    APP_ESM_EPS_QOS_INFO_STRU                   stSdfQosInfo;

    VOS_UINT8                                   ucExpiredCount;     /* 发起激活的超时次数 */
    VOS_UINT8                                   aucReserved[3];
    APP_ESM_CUSTOM_PCO_INFO_STRU                stCustomPcoInfo;
    APP_ESM_EPDG_INFO_STRU                      stEpdgInfo;
} APP_ESM_NDISCONN_CNF_STRU;
/*****************************************************************************
 结构名    : APP_ESM_SET_CGDCONT_REQ_STRU
 结构说明  : CGDCONTAT命令的消息结构
*****************************************************************************/
typedef struct
{
    VOS_MSG_HEADER                                          /*_H2ASN_Skip*/
    VOS_UINT32                                  ulMsgId;    /*_H2ASN_Skip*/
    APP_MSG_HEADER
    VOS_UINT32                                  ulOpId;
    VOS_UINT32                                  ulCid;

    VOS_UINT32                                  bitOpBearType          :1;
    VOS_UINT32                                  bitOpPdnType           :1;
    VOS_UINT32                                  bitOpApn               :1;

    VOS_UINT32                                  bitOpIpv4AddrAllocType :1;

    VOS_UINT32                                  bitOpPcscfDiscovery    :1;
    VOS_UINT32                                  bitOpImsCnSignalFlag   :1;

    VOS_UINT32                                  bitOpSpare             :26;


    APP_ESM_PARA_SET_ENUM_UINT32                enSetType;       /*0:SET;1:DEL*/
    APP_ESM_CID_TYPE_ENUM_UINT32                enBearCidType;
    APP_ESM_APN_INFO_STRU                       stApnInfo;
    APP_ESM_PDN_TYPE_ENUM_UINT32                enPdnType;
    APP_ESM_IPV4_ADDR_ALLOC_TYPE_ENUM_UINT32    enIpv4AddrAllocType;
    APP_ESM_PCSCF_DISCOVERY_ENUM_UINT32         enPcscfDiscovery;
    APP_ESM_IMS_CN_SIG_FLAG_ENUM_UINT32         enImsCnSignalFlag;
} APP_ESM_SET_CGDCONT_REQ_STRU;

/*****************************************************************************
 结构名    : APP_ESM_SET_CGDCONT_CNF_STRU
 结构说明  : 回复CGDCONTAT命令的消息结构
*****************************************************************************/
typedef struct
{
    VOS_MSG_HEADER                                          /*_H2ASN_Skip*/
    VOS_UINT32                                  ulMsgId;    /*_H2ASN_Skip*/
    APP_MSG_HEADER
    VOS_UINT32                                  ulOpId;
    VOS_UINT32                                  ulCid;

    VOS_UINT32                                  ulRslt;

} APP_ESM_SET_CGDCONT_CNF_STRU;
/*****************************************************************************
 结构名    : APP_ESM_APN_ENABLE_INFO_CHANGE_NTF_STRU
 结构说明  : APN enable参数设置的消息结构
*****************************************************************************/
typedef struct
{
    VOS_MSG_HEADER                                          /*_H2ASN_Skip*/
    VOS_UINT32                                  ulMsgId;    /*_H2ASN_Skip*/
    APP_MSG_HEADER
    VOS_UINT32                                  ulOpId;

    VOS_UINT32                                  ulCid;          /* CID [0,11] */
    APP_ESM_APN_TYPE_ENUM_UINT8                 enApnType;      /* apn class */
    VOS_UINT8                                   ucEnableFlag;  /* apn enable flag */
    VOS_UINT8                                   aucReserved[2];
} APP_ESM_APN_ENABLE_INFO_CHANGE_NTF_STRU;

/*****************************************************************************
 结构名    : APP_ESM_PDP_MANAGE_INFO_STRU
 结构说明  : PDP管理模式信息结构
*****************************************************************************/
typedef struct
{
    APP_ESM_BEARER_MANAGE_MODE_ENUM_UINT8    enMode;         /* 承载管理模式 */
    APP_ESM_BEARER_MANAGE_TYPE_ENUM_UINT8    enType;         /* 承载管理类型 */
    VOS_UINT8                                aucReserve1[2]; /* 四字节对齐，保留 */
}APP_ESM_PDP_MANAGE_INFO_STRU;

/*****************************************************************************
 结构名    : APP_ESM_DATA_SWITCH_INFO_STRU
 结构说明  : 数据业务开关信息结构
*****************************************************************************/
typedef struct
{
    VOS_UINT8                               ucDataSwitch;                       /* 数据业务开关状态 */
    VOS_UINT8                               ucDataRoamSwitch;                   /* 数据业务漫游开关状态 */

    VOS_UINT8                               aucResv[2];
}APP_ESM_DATA_SWITCH_INFO_STRU;

typedef struct
{
    APP_ESM_MSG_TYPE_ENUM_UINT32        enMsgId;            /*_H2ASN_MsgChoice_Export APP_ESM_MSG_TYPE_ENUM_UINT32*/
    VOS_UINT8                           aucMsg[4];
    /***************************************************************************
        _H2ASN_MsgChoice_When_Comment          APP_ESM_MSG_TYPE_ENUM_UINT32
    ****************************************************************************/
}APP_ESM_INTERFACE_MSG_DATA;
/*_H2ASN_Length UINT32*/


typedef struct
{
    VOS_MSG_HEADER
    APP_ESM_INTERFACE_MSG_DATA           stMsgData;
} AppEsmInterface_MSG;






/*****************************************************************************
  6 UNION
*****************************************************************************/


/*****************************************************************************
  7 Extern Global Variable
*****************************************************************************/


/*****************************************************************************
  8 Fuction Extern
*****************************************************************************/
extern VOS_UINT32 APP_GetSdfParaByModemId
(
    MODEM_ID_ENUM_UINT16      enModemId,
    VOS_UINT32                         *pulSdfNum,
    APP_ESM_SDF_PARA_STRU              *pstSdfPara
);
extern VOS_UINT32 APP_GetPdpManageInfoByModemId
(
    MODEM_ID_ENUM_UINT16      enModemId,
    APP_ESM_PDP_MANAGE_INFO_STRU  *pstPdpManageInfo
);
extern VOS_UINT32 APP_GetCidImsSuppFlagByModemId
(
    MODEM_ID_ENUM_UINT16      enModemId,
    VOS_UINT8                           ucCid,
    VOS_UINT8                          *pucImsSuppFlag
);
/* 与闫志吉确认:此接口仅在VZW场景下会返回有效的APN个数和APN信息;
非VZW场景下,APN个数为0,返回VOS_OK */
extern VOS_UINT32 APP_ESM_GetAllApnClassInfo
(
    MODEM_ID_ENUM_UINT16                enModemId,
    VOS_UINT32                         *pulApnNum,
    APP_ESM_APN_CLASS_INFO_STRU        *pstApnClassInfo
);
extern VOS_UINT32 APP_GetDataSwitchInfoByModemId
(
    MODEM_ID_ENUM_UINT16                enModemId,
    APP_ESM_DATA_SWITCH_INFO_STRU      *pstDataSwitchInfo
);
extern VOS_UINT32 APP_AllocPsCallIdByModemId
(
    MODEM_ID_ENUM_UINT16                enModemId,
    VOS_UINT8                          *pucPsCallId
);
/*****************************************************************************
  9 OTHERS
*****************************************************************************/










#if (VOS_OS_VER != VOS_WIN32)
#pragma pack()
#else
#pragma pack(pop)
#endif





#ifdef __cplusplus
    #if __cplusplus
        }
    #endif
#endif

#endif /* end of AppEsmInterface.h */
