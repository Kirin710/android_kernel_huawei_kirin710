/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2012-2015. All rights reserved.
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



#ifndef _USIMM_USIMMPSINTERFACE_COMM_H_
#define _USIMM_USIMMPSINTERFACE_COMM_H_

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif



/*****************************************************************************
1 其他头文件包含
*****************************************************************************/
#include "vos.h"
#include "product_config.h"
#include "mdrv.h"

#if (FEATURE_VSIM == FEATURE_ON)
#include "mdrv.h"
#endif

#pragma pack(4)

/*******************************************************************************
2 宏定义
*******************************************************************************/
#define PS_USIM_SERVICE_NOT_AVAILIABLE          (0)
#define PS_USIM_SERVICE_AVAILIABLE              (1)

#define USIM_VSIM_AES_KEY_LEN                   (32)

#define USIMM_NULL_ID                           0xFFFF

/* 定义卡密码长度 */
#define USIMM_PINNUMBER_LEN                     (8)

#define USIMM_T0_APDU_MAX_LEN                   (256)

/* USIMM模块支持的最大路径长度,UINT16为单位 */
#define USIMM_MAX_PATH_LEN                      (6)

#define USIMM_REFRESH_FILE_MAX_PATH_LEN         ((USIMM_MAX_PATH_LEN+2)*4)

#define USIMM_REFRESH_FILE_MAX_PATH_LEN_SUB_ONE (USIMM_REFRESH_FILE_MAX_PATH_LEN-1)

/* USIMM模块支持的一次REFRESH命令更新文件个数，根据STK数据长度最大256，计算最多的文件路径为65个 */
#define USIMM_MAX_REFRESH_FILE_NUM              (65)

/* APDU回复数据的最大长度 */
#define USIMM_APDU_RSP_MAX_LEN                  (260)

#define USIMM_APDU_HEADLEN                      (5)

#define USIMM_EF_IMSI_LEN                       (9)
#define USIMM_EF_CIMSI_LEN                      (10)
#define USIMM_EF_IMSI_COMM_LEN                  (10)
#define USIMM_EF_AD_MIN_LEN                     (3)

#define USIMM_EF_GID_LEN                        (1)      /* 目前锁网锁卡校验场景只会用到第一个字节 */
#define USIMM_EF_GID1_LEN                       (16)
#define USIMM_EF_GID2_LEN                       (16)

#define MFLAB                                   (0x3F)
#define DFUNDERMFLAB                            (0x7F)
#define DFUNDERDFLAB                            (0x5F)
#define MFID                                    (0x3F00)
#define EFIDUNDERMF                             (0x2F00)
#define EFIDUNDERMFDFDF                         (0x4F00)
#define EFIDUNDERMFDF                           (0x6F00)
#define DFIDUNDERMF                             (0x7F00)
#define DFIDUNDERMFDF                           (0x5F00)

#define EFBDN                                   (0x6F4D)
#define EFFDN                                   (0x6F3B)
#define EFMSISDN                                (0x6F40)
/* AID长度最小值和最大值 */
#define USIMM_AID_LEN_MIN                       (5)
#define USIMM_AID_LEN_MAX                       (16)

#define USIMM_TPDU_HEAD_LEN                     (5)
#define USIMM_TPDU_DATA_LEN_MAX                 (256)
#define USIMM_PRIVATECGLA_DATA_LEN_MAX          (USIMM_TPDU_DATA_LEN_MAX*3)

#define USIMM_BCMCSRSP_LEN_MAX                  (16)

#define USIMM_ERROR_INFO_MAX                    (20)

#define USIMM_ICCID_FILE_LEN                    (10)

#define USIMM_FILE_OPEN_MODE_R                  "rb"            /* open binary file for reading */
#define USIMM_FILE_OPEN_MODE_W                  "wb"            /* open binary file for writing */
#define USIMM_FILE_OPEN_MODE_RW                 "rb+"           /* open binary file for reading and writing */
#define USIMM_FILE_OPEN_MODE_NEW_RW             "wb+"           /* creat binary file for reading and writing*/
#define USIMM_FILE_OPEN_MODE_APEND              "a"
#define USIMM_FILE_OPEN_MODE_ADD_W              "ab"            /* creat binary file for add writing */

#define USIMM_FILE_SEEK_SET                     (0)             /* the beginning of the file*/
#define USIMM_FILE_SEEK_CUR                     (1)             /* the current value of the file position indicator */
#define USIMM_FILE_SEEK_END                     (2)             /* the end of the file */

#define USIMM_RUNCAVE_RAND_LEN                  (4)
#define USIMM_RUNCAVE_DIGLEN                    (3)
#define USIMM_ESN_MEID_LEN                      (7)
#define USIMM_CDMA_RANDSSD_LEN                  (7)
#define USIMM_CDMA_AUTHBS_LEN                   (3)
#define USIMM_AUTH_RUNCAVE_LEN                  (17)

/*定义鉴权数据的长度*/
#define USIMM_AUTH_IKSPACELEN                   (17)    /*Len+Data*/
#define USIMM_AUTH_CKSPACELEN                   (17)    /*Len+Data*/
#define USIMM_AUTH_KCSPACELEN                   (9)     /*Len+Data*/
#define USIMM_AUTH_AUTHSPACELEN                 (15)    /*Len+Data*/
#define USIMM_AUTH_RESSPACELEN                  (17)    /*Len+Data*/
#define USIMM_AUTH_ATUHRSPACELEN                (3)     /*Data*/
#define USIMM_AUTH_MSG_PADDING_LEN              (20)    /* 消息结构体中预留字节，为PCLint修改 */
#define USIMM_AUTH_BSCSPACELEN                  (4)


#define USIMM_PATHSTR_MAX_LEN                   (60)

#define USIMM_PATHSTR_MAX_LEN_SUB_ONE           (USIMM_PATHSTR_MAX_LEN - 1) /*For VOS_StrNLen Modify*/

#define USIMM_APDU_DATA_MAXLEN                  (255)

#define USIMM_SETMUTILFILE_MAX                  (14)

#define USIMM_ADF_STR                        "3F007FFF"

/* EF under MF */
#define USIMM_MF_STR                         "3F00"
#define USIMM_DIR_STR                        "3F002F00"
#define USIMM_ICCID_STR                      "3F002FE2"
#define USIMM_PL_STR                         "3F002F05"
#define USIMM_ARR_STR                        "3F002F06"

#define USIMM_USIM_STR                       "3F007FFF"
#define USIMM_USIM_EFLI_STR                  "3F007FFF6F05"
#define USIMM_USIM_EFARR_STR                 "3F007FFF6F06"
#define USIMM_USIM_EFIMSI_STR                "3F007FFF6F07"
#define USIMM_USIM_EFKEYS_STR                "3F007FFF6F08"
#define USIMM_USIM_EFKEYSPS_STR              "3F007FFF6F09"
#define USIMM_USIM_EFCSP_STR                 "3F007FFF6F15"
#define USIMM_USIM_EFDCK_STR                 "3F007FFF6F2C"
#define USIMM_USIM_EFHPPLMN_STR              "3F007FFF6F31"
#define USIMM_USIM_EFCNL_STR                 "3F007FFF6F32"
#define USIMM_USIM_EFACMMAX_STR              "3F007FFF6F37"
#define USIMM_USIM_EFUST_STR                 "3F007FFF6F38"
#define USIMM_USIM_EFACM_STR                 "3F007FFF6F39"
#define USIMM_USIM_EFFDN_STR                 "3F007FFF6F3B"
#define USIMM_USIM_EFSMS_STR                 "3F007FFF6F3C"
#define USIMM_USIM_EFGID1_STR                "3F007FFF6F3E"
#define USIMM_USIM_EFGID2_STR                "3F007FFF6F3F"
#define USIMM_USIM_EFMSISDN_STR              "3F007FFF6F40"
#define USIMM_USIM_EFPUCT_STR                "3F007FFF6F41"
#define USIMM_USIM_EFSMSP_STR                "3F007FFF6F42"
#define USIMM_USIM_EFSMSS_STR                "3F007FFF6F43"
#define USIMM_USIM_EFCBMI_STR                "3F007FFF6F45"
#define USIMM_USIM_EFSPN_STR                 "3F007FFF6F46"
#define USIMM_USIM_EFSMSR_STR                "3F007FFF6F47"
#define USIMM_USIM_EFCBMID_STR               "3F007FFF6F48"
#define USIMM_USIM_EFSDN_STR                 "3F007FFF6F49"
#define USIMM_USIM_EFEXT2_STR                "3F007FFF6F4B"
#define USIMM_USIM_EFEXT3_STR                "3F007FFF6F4C"
#define USIMM_USIM_EFBDN_STR                 "3F007FFF6F4D"
#define USIMM_USIM_EFEXT5_STR                "3F007FFF6F4E"
#define USIMM_USIM_EFCCP2_STR                "3F007FFF6F4F"
#define USIMM_USIM_EFCBMIR_STR               "3F007FFF6F50"
#define USIMM_USIM_EFEXT4_STR                "3F007FFF6F55"
#define USIMM_USIM_EFEST_STR                 "3F007FFF6F56"
#define USIMM_USIM_EFACL_STR                 "3F007FFF6F57"
#define USIMM_USIM_EFCMI_STR                 "3F007FFF6F58"
#define USIMM_USIM_EFSTART_HFN_STR           "3F007FFF6F5B"
#define USIMM_USIM_EFTHRESHOL_STR            "3F007FFF6F5C"
#define USIMM_USIM_EFPLMNWACT_STR            "3F007FFF6F60"
#define USIMM_USIM_EFOPLMNWACT_STR           "3F007FFF6F61"
#define USIMM_USIM_EFHPLMNwACT_STR           "3F007FFF6F62"
#define USIMM_USIM_EFPSLOCI_STR              "3F007FFF6F73"
#define USIMM_USIM_EFACC_STR                 "3F007FFF6F78"
#define USIMM_USIM_EFFPLMN_STR               "3F007FFF6F7B"
#define USIMM_USIM_EFLOCI_STR                "3F007FFF6F7E"
#define USIMM_USIM_EFICI_STR                 "3F007FFF6F80"
#define USIMM_USIM_EFOCI_STR                 "3F007FFF6F81"
#define USIMM_USIM_EFICT_STR                 "3F007FFF6F82"
#define USIMM_USIM_EFOCT_STR                 "3F007FFF6F83"
#define USIMM_USIM_EFAD_STR                  "3F007FFF6FAD"
#define USIMM_USIM_EFVGCS_STR                "3F007FFF6FB1"
#define USIMM_USIM_EFVGCSS_STR               "3F007FFF6FB2"
#define USIMM_USIM_EFVBS_STR                 "3F007FFF6FB3"
#define USIMM_USIM_EFVBSS_STR                "3F007FFF6FB4"
#define USIMM_USIM_EFEMLPP_STR               "3F007FFF6FB5"
#define USIMM_USIM_EFAAEM_STR                "3F007FFF6FB6"
#define USIMM_USIM_EFECC_STR                 "3F007FFF6FB7"
#define USIMM_USIM_EFHIDDENKEY_STR           "3F007FFF6FC3"
#define USIMM_USIM_EFNETPAR_STR              "3F007FFF6FC4"
#define USIMM_USIM_EFPNN_STR                 "3F007FFF6FC5"
#define USIMM_USIM_EFOPL_STR                 "3F007FFF6FC6"
#define USIMM_USIM_EFMBDN_STR                "3F007FFF6FC7"
#define USIMM_USIM_EFEXT6_STR                "3F007FFF6FC8"
#define USIMM_USIM_EFMBI_STR                 "3F007FFF6FC9"
#define USIMM_USIM_EFMWIS_STR                "3F007FFF6FCA"
#define USIMM_USIM_EFCFIS_STR                "3F007FFF6FCB"
#define USIMM_USIM_EFEXT7_STR                "3F007FFF6FCC"
#define USIMM_USIM_EFSPDI_STR                "3F007FFF6FCD"
#define USIMM_USIM_EFMMSN_STR                "3F007FFF6FCE"
#define USIMM_USIM_EFEXT8_STR                "3F007FFF6FCF"
#define USIMM_USIM_EFMMSICP_STR              "3F007FFF6FD0"
#define USIMM_USIM_EFMMSUP_STR               "3F007FFF6FD1"
#define USIMM_USIM_EFMMSUCP_STR              "3F007FFF6FD2"
#define USIMM_USIM_EFNIA_STR                 "3F007FFF6FD3"
#define USIMM_USIM_EFVGCSCA_STR              "3F007FFF6FD4"
#define USIMM_USIM_EFVBSCA_STR               "3F007FFF6FD5"
#define USIMM_USIM_EFGBAP_STR                "3F007FFF6FD6"
#define USIMM_USIM_EFMSK_STR                 "3F007FFF6FD7"
#define USIMM_USIM_EFMUK_STR                 "3F007FFF6FD8"
#define USIMM_USIM_EFEHPLMN_STR              "3F007FFF6FD9"
#define USIMM_USIM_EFGBANL_STR               "3F007FFF6FDA"
#define USIMM_USIM_EFEHPLMNPI_STR            "3F007FFF6FDB"
#define USIMM_USIM_EFLRPLMNSI_STR            "3F007FFF6FDC"
#define USIMM_USIM_EFNAFKCA_STR              "3F007FFF6FDD"
#define USIMM_USIM_EFSPNI_STR                "3F007FFF6FDE"
#define USIMM_USIM_EFPNNI_STR                "3F007FFF6FDF"
#define USIMM_USIM_EFNCPIP_STR               "3F007FFF6FE2"
#define USIMM_USIM_EFEPSLOCI_STR             "3F007FFF6FE3"
#define USIMM_USIM_EFEPSNSC_STR              "3F007FFF6FE4"
#define USIMM_USIM_EFUFC_STR                 "3F007FFF6FE6"
#define USIMM_USIM_EFUICCIARI_STR            "3F007FFF6FE7"
#define USIMM_USIM_EFNASCONFIG_STR           "3F007FFF6FE8"
#define USIMM_USIM_EFPWS_STR                 "3F007FFF6FEC"
#define USIMM_USIM_EFSDNURI_STR              "3F007FFF6FEF"
#define USIMM_USIM_EFEPDGID_STR              "3F007FFF6FF3"
#define USIMM_USIM_EFEPDGSELECTION_STR       "3F007FFF6FF4"
#define USIMM_USIM_EFEPDGIDEM_STR            "3F007FFF6FF5"
#define USIMM_USIM_EFEPDGSELECTIONEM_STR     "3F007FFF6FF6"

#define USIMM_USIM_DFPHONEBOOK_STR           "3F007FFF5F3A"
#define USIMM_USIM_EFPSC_STR                 "3F007FFF5F3A4F22"
#define USIMM_USIM_EFCC_STR                  "3F007FFF5F3A4F23"
#define USIMM_USIM_EFPUID_STR                "3F007FFF5F3A4F24"
#define USIMM_USIM_EFPBR_STR                 "3F007FFF5F3A4F30"

#define USIMM_USIM_DFGSM_ACCESS_STR          "3F007FFF5F3B"
#define USIMM_USIM_EFKC_STR                  "3F007FFF5F3B4F20"
#define USIMM_USIM_EFKCGPRS_STR              "3F007FFF5F3B4F52"
#define USIMM_USIM_EFCPBCCH_STR              "3F007FFF5F3B4F63"
#define USIMM_USIM_EFINVSCAN_STR             "3F007FFF5F3B4F64"

#define USIMM_USIM_DFMEXE_STR                "3F007FFF5F3C"
#define USIMM_USIM_EFMexE_ST_STR             "3F007FFF5F3C4F40"
#define USIMM_USIM_EFORPK_STR                "3F007FFF5F3C4F41"
#define USIMM_USIM_EFARPK_STR                "3F007FFF5F3C4F42"
#define USIMM_USIM_EFTPRK_STR                "3F007FFF5F3C4F43"

#define USIMM_USIM_DFSOLSA_STR               "3F007FFF5F70"
#define USIMM_USIM_EFSAI_STR                 "3F007FFF5F704F30"
#define USIMM_USIM_EFSLL_STR                 "3F007FFF5F704F31"

#define USIMM_USIM_DFWLAN_STR                "3F007FFF5F40"
#define USIMM_USIM_EFPSEUDO_STR              "3F007FFF5F404F41"
#define USIMM_USIM_EFUPLMNWLAN_STR           "3F007FFF5F404F42"
#define USIMM_USIM_EF0PLMNWLAN_STR           "3F007FFF5F404F43"
#define USIMM_USIM_EFUWSIDL_STR              "3F007FFF5F404F44"
#define USIMM_USIM_EFOWSIDL_STR              "3F007FFF5F404F45"
#define USIMM_USIM_EFWRI_STR                 "3F007FFF5F404F46"
#define USIMM_USIM_EFHWSIDL_STR              "3F007FFF5F404F47"
#define USIMM_USIM_EFWEHPLMNPI_STR           "3F007FFF5F404F48"
#define USIMM_USIM_EFWHPI_STR                "3F007FFF5F404F49"
#define USIMM_USIM_EFWLRPLMN_STR             "3F007FFF5F404F4A"
#define USIMM_USIM_EFHPLMNDAI_STR            "3F007FFF5F404F4B"

#define USIMM_USIM_DFHNB_STR                 "3F007FFF5F50"
#define USIMM_USIM_EFACSGL_STR               "3F007FFF5F504F81"
#define USIMM_USIM_EFCSGT_STR                "3F007FFF5F504F82"
#define USIMM_USIM_EFHNBN_STR                "3F007FFF5F504F83"
#define USIMM_USIM_EFOCSGL_STR               "3F007FFF5F504F84"
#define USIMM_USIM_EFOCSGT_STR               "3F007FFF5F504F85"
#define USIMM_USIM_EFOHNBN_STR               "3F007FFF5F504F86"

#define USIMM_USIM_DFPROSE_STR               "3F007FFF5F90"

#define USIMM_USIM_EF5GS3GPPLOCI_STR         "3F007FFF6FFF"
#define USIMM_USIM_EF5GAUTHKEYS_STR          "3F007FFF6F01"
#define USIMM_USIM_EF5GS3GPPNSC_STR          "3F007FFF6F02"
#define USIMM_USIM_EFUAC_AIC_STR             "3F007FFF6F0A"

#define USIMM_USIM_EFPST_STR                 "3F007FFF5F904F10"

#define USIMM_USIM_DFACDC_STR                "3F007FFF5FA0"
#define USIMM_USIM_EFACDCLIST_STR            "3F007FFF5FA04F01"

#define USIMM_TELE_STR                       "3F007F10"
#define USIMM_TELE_EFADN_STR                 "3F007F106F3A"
#define USIMM_TELE_EFFDN_STR                 "3F007F106F3B"
#define USIMM_TELE_EFSMS_STR                 "3F007F106F3C"
#define USIMM_TELE_EFCCP_STR                 "3F007F106F3D"
#define USIMM_TELE_EFMSISDN_STR              "3F007F106F40"
#define USIMM_TELE_EFSMSP_STR                "3F007F106F42"
#define USIMM_TELE_EFSMSS_STR                "3F007F106F43"
#define USIMM_TELE_EFLND_STR                 "3F007F106F44"
#define USIMM_TELE_EFSMSR_STR                "3F007F106F47"
#define USIMM_TELE_EFSDN_STR                 "3F007F106F49"
#define USIMM_TELE_EFEXT1_STR                "3F007F106F4A"
#define USIMM_TELE_EFEXT2_STR                "3F007F106F4B"
#define USIMM_TELE_EFEXT3_STR                "3F007F106F4C"
#define USIMM_TELE_EFBDN_STR                 "3F007F106F4D"
#define USIMM_TELE_EFEXT4_STR                "3F007F106F4E"
#define USIMM_TELE_EFECCP_STR                "3F007F106F4F"
#define USIMM_TELE_EFRMA_STR                 "3F007F106F53"
#define USIMM_TELE_EFSUME_STR                "3F007F106F54"
#define USIMM_TELE_EFICE_DN_STR              "3F007F106FE0"
#define USIMM_TELE_EFICE_FF_STR              "3F007F106FE1"
#define USIMM_TELE_EFPSISMSC_STR             "3F007F106FE5"

#define USIMM_TELE_DFGRAP_STR                "3F007F105F50"
#define USIMM_TELE_DFGRAP_EFIMG_STR          "3F007F105F504F20"
#define USIMM_TELE_DFGRAP_EFICEGRAPH_STR     "3F007F105F504F21"
#define USIMM_TELE_DFGRAP_EFLAUNCHSCWS_STR   "3F007F105F504F01"

#define USIMM_TELE_DFPB_STR                  "3F007F105F3A"
#define USIMM_TELE_DFPB_EFPSC_STR            "3F007F105F3A4F22"
#define USIMM_TELE_DFPB_EFCC_STR             "3F007F105F3A4F23"
#define USIMM_TELE_DFPB_EFPUID_STR           "3F007F105F3A4F24"
#define USIMM_TELE_DFPB_EFPBR_STR            "3F007F105F3A4F30"

#define USIMM_TELE_DFMM_STR                  "3F007F105F3B"
#define USIMM_TELE_DFMM_EFMML_STR            "3F007F105F3B4F47"
#define USIMM_TELE_DFMUM_EFMMDF_STR          "3F007F105F3B4F48"

#define USIMM_TELE_DFMSS_STR                 "3F007F105F3C"
#define USIMM_TELE_EFMLPL_STR                "3F007F105F3C4F20"
#define USIMM_TELE_EFMSPL_STR                "3F007F105F3C4F21"
#define USIMM_TELE_EFMMSSMODE_STR            "3F007F105F3C4F22"

#define USIMM_TELE_DFMCPTT_STR               "3F007F105F3D"
#define USIMM_TELE_EFMST_STR                 "3F007F105F3D4F01"

#define USIMM_CSIM_STR                       "3F007FFF"
#define USIMM_CSIM_EFCOUNT_STR               "3F007FFF6F21"
#define USIMM_CSIM_EFIMSIM_STR               "3F007FFF6F22"
#define USIMM_CSIM_EFIMSIT_STR               "3F007FFF6F23"
#define USIMM_CSIM_EFTMSI_STR                "3F007FFF6F24"
#define USIMM_CSIM_EFAH_STR                  "3F007FFF6F25"
#define USIMM_CSIM_EFAOP_STR                 "3F007FFF6F26"
#define USIMM_CSIM_EFALOC_STR                "3F007FFF6F27"
#define USIMM_CSIM_EFCDMAHOME_STR            "3F007FFF6F28"
#define USIMM_CSIM_EFZNREGI_STR              "3F007FFF6F29"
#define USIMM_CSIM_EFSNREGI_STR              "3F007FFF6F2A"
#define USIMM_CSIM_EFDISTREGI_STR            "3F007FFF6F2B"
#define USIMM_CSIM_EFACCOLC_STR              "3F007FFF6F2C"
#define USIMM_CSIM_EFTERM_STR                "3F007FFF6F2D"
#define USIMM_CSIM_EFSSCI_STR                "3F007FFF6F2E"
#define USIMM_CSIM_EFACP_STR                 "3F007FFF6F2F"
#define USIMM_CSIM_EFPRL_STR                 "3F007FFF6F30"
#define USIMM_CSIM_EFRUIMID_STR              "3F007FFF6F31"
#define USIMM_CSIM_EFCSIMST_STR              "3F007FFF6F32"
#define USIMM_CSIM_EFSPC_STR                 "3F007FFF6F33"
#define USIMM_CSIM_EFOTAPASPC_STR            "3F007FFF6F34"
#define USIMM_CSIM_EFNAMLOCK_STR             "3F007FFF6F35"
#define USIMM_CSIM_EFOTA_STR                 "3F007FFF6F36"
#define USIMM_CSIM_EFSP_STR                  "3F007FFF6F37"
#define USIMM_CSIM_EFESNMEID_STR             "3F007FFF6F38"
#define USIMM_CSIM_EFLI_STR                  "3F007FFF6F3A"
#define USIMM_CSIM_EFFDN_STR                 "3F007FFF6F3B"
#define USIMM_CSIM_EFSMS_STR                 "3F007FFF6F3C"
#define USIMM_CSIM_EFSMSP_STR                "3F007FFF6F3D"
#define USIMM_CSIM_EFSMSS_STR                "3F007FFF6F3E"
#define USIMM_CSIM_EFSSFC_STR                "3F007FFF6F3F"
#define USIMM_CSIM_EFSPN_STR                 "3F007FFF6F41"
#define USIMM_CSIM_EFUSGIND_STR              "3F007FFF6F42"
#define USIMM_CSIM_EFAD_STR                  "3F007FFF6F43"
#define USIMM_CSIM_EFMDN_STR                 "3F007FFF6F44"
#define USIMM_CSIM_EFMAXPRL_STR              "3F007FFF6F45"
#define USIMM_CSIM_EFSPCS_STR                "3F007FFF6F46"
#define USIMM_CSIM_EFECC_STR                 "3F007FFF6F47"
#define USIMM_CSIM_EFME3GPDOPC_STR           "3F007FFF6F48"
#define USIMM_CSIM_EF3GPDOPM_STR             "3F007FFF6F49"
#define USIMM_CSIM_EFSIPCAP_STR              "3F007FFF6F4A"
#define USIMM_CSIM_EFMIPCAP_STR              "3F007FFF6F4B"
#define USIMM_CSIM_EFSIPUPP_STR              "3F007FFF6F4C"
#define USIMM_CSIM_EFMIPUPP_STR              "3F007FFF6F4D"
#define USIMM_CSIM_EFSIPSP_STR               "3F007FFF6F4E"
#define USIMM_CSIM_EFMIPSP_STR               "3F007FFF6F4F"
#define USIMM_CSIM_EFSIPPAPSS_STR            "3F007FFF6F50"
#define USIMM_CSIM_EFPUZL_STR                "3F007FFF6F53"
#define USIMM_CSIM_EFMAXPUZL_STR             "3F007FFF6F54"
#define USIMM_CSIM_EFMECRP_STR               "3F007FFF6F55"
#define USIMM_CSIM_EFHRPDCAP_STR             "3F007FFF6F56"
#define USIMM_CSIM_EFHRPDUPP_STR             "3F007FFF6F57"
#define USIMM_CSIM_EFCSSPR_STR               "3F007FFF6F58"
#define USIMM_CSIM_EFATC_STR                 "3F007FFF6F59"
#define USIMM_CSIM_EFEPRL_STR                "3F007FFF6F5A"
#define USIMM_CSIM_EFBCSMSCFG_STR            "3F007FFF6F5B"
#define USIMM_CSIM_EFBCSMSPREF_STR           "3F007FFF6F5C"
#define USIMM_CSIM_EFBCSMSTABLE_STR          "3F007FFF6F5D"
#define USIMM_CSIM_EFBCSMSP_STR              "3F007FFF6F5E"
#define USIMM_CSIM_EFBAKPARA_STR             "3F007FFF6F63"
#define USIMM_CSIM_EFUPBAKPARA_STR           "3F007FFF6F64"
#define USIMM_CSIM_EFMMSN_STR                "3F007FFF6F65"
#define USIMM_CSIM_EFEXT8_STR                "3F007FFF6F66"
#define USIMM_CSIM_EFMMSICP_STR              "3F007FFF6F67"
#define USIMM_CSIM_EFMMSUP_STR               "3F007FFF6F68"
#define USIMM_CSIM_EFMMSUCP_STR              "3F007FFF6F69"
#define USIMM_CSIM_EFAUTHCAP_STR             "3F007FFF6F6A"
#define USIMM_CSIM_EF3GCIK_STR               "3F007FFF6F6B"
#define USIMM_CSIM_EFDCK_STR                 "3F007FFF6F6C"
#define USIMM_CSIM_EFGID1_STR                "3F007FFF6F6D"
#define USIMM_CSIM_EFGID2_STR                "3F007FFF6F6E"
#define USIMM_CSIM_EFCDMACNL_STR             "3F007FFF6F6F"
#define USIMM_CSIM_EFHOMETAG_STR             "3F007FFF6F70"
#define USIMM_CSIM_EFGROUPTAG_STR            "3F007FFF6F71"
#define USIMM_CSIM_EFSPECTAG_STR             "3F007FFF6F72"
#define USIMM_CSIM_EFCALLPROMPT_STR          "3F007FFF6F73"
#define USIMM_CSIM_EFSFEUIMID_STR            "3F007FFF6F74"
#define USIMM_CSIM_EFEST_STR                 "3F007FFF6F75"
#define USIMM_CSIM_EFHIDDENKEY_STR           "3F007FFF6F76"
#define USIMM_CSIM_EFLCSVER_STR              "3F007FFF6F77"
#define USIMM_CSIM_EFLCSCP_STR               "3F007FFF6F78"
#define USIMM_CSIM_EFSDN_STR                 "3F007FFF6F79"
#define USIMM_CSIM_EFEXT2_STR                "3F007FFF6F7A"
#define USIMM_CSIM_EFEXT3_STR                "3F007FFF6F7B"
#define USIMM_CSIM_EFICI_STR                 "3F007FFF6F7C"
#define USIMM_CSIM_EFOCI_STR                 "3F007FFF6F7D"
#define USIMM_CSIM_EFEXT5_STR                "3F007FFF6F7E"
#define USIMM_CSIM_EFCCP2_STR                "3F007FFF6F7F"
#define USIMM_CSIM_EFAPPLAB_STR              "3F007FFF6F80"
#define USIMM_CSIM_EFMODEL_STR               "3F007FFF6F81"
#define USIMM_CSIM_EFRC_STR                  "3F007FFF6F82"
#define USIMM_CSIM_EFSMSCAP_STR              "3F007FFF6F83"
#define USIMM_CSIM_EFMIPFLAGS_STR            "3F007FFF6F84"
#define USIMM_CSIM_EF3GPDUPPEXT_STR          "3F007FFF6F85"
#define USIMM_CSIM_EFIPV6CAP_STR             "3F007FFF6F87"
#define USIMM_CSIM_EFTCPCFG_STR              "3F007FFF6F88"
#define USIMM_CSIM_EFDGC_STR                 "3F007FFF6F89"
#define USIMM_CSIM_EFWAPBWCP_STR             "3F007FFF6F8A"
#define USIMM_CSIM_EFWAPBWBM_STR             "3F007FFF6F8B"
#define USIMM_CSIM_EFMMSCFG_STR              "3F007FFF6F8C"
#define USIMM_CSIM_EFJDL_STR                 "3F007FFF6F8D"

#define USIMM_CSIM_DFPHONEBOOK_STR           "3F007FFF5F3A"
#define USIMM_CSIM_EFPSC_STR                 "3F007FFF5F3A4F22"
#define USIMM_CSIM_EFCC_STR                  "3F007FFF5F3A4F23"
#define USIMM_CSIM_EFPUID_STR                "3F007FFF5F3A4F24"
#define USIMM_CSIM_EFPBR_STR                 "3F007FFF5F3A4F30"

/* EF under MF/DFGSM */
#define USIMM_GSM_STR                        "3F007F20"
#define USIMM_GSM_EFLP_STR                   "3F007F206F05"
#define USIMM_GSM_EFIMSI_STR                 "3F007F206F07"
#define USIMM_GSM_EFKC_STR                   "3F007F206F20"
#define USIMM_GSM_EFDCK_STR                  "3F007F206F2C"
#define USIMM_GSM_EFPLMNSEL_STR              "3F007F206F30"
#define USIMM_GSM_EFHPLMN_STR                "3F007F206F31"
#define USIMM_GSM_EFCNL_STR                  "3F007F206F32"
#define USIMM_GSM_EFACMMAX_STR               "3F007F206F37"
#define USIMM_GSM_EFSST_STR                  "3F007F206F38"
#define USIMM_GSM_EFACM_STR                  "3F007F206F39"
#define USIMM_GSM_EFGID1_STR                 "3F007F206F3E"
#define USIMM_GSM_EFGID2_STR                 "3F007F206F3F"
#define USIMM_GSM_EFPUCT_STR                 "3F007F206F41"
#define USIMM_GSM_EFCBMI_STR                 "3F007F206F45"
#define USIMM_GSM_EFSPN_STR                  "3F007F206F46"
#define USIMM_GSM_EFCBMID_STR                "3F007F206F48"
#define USIMM_GSM_EFBCCH_STR                 "3F007F206F74"
#define USIMM_GSM_EFACC_STR                  "3F007F206F78"
#define USIMM_GSM_EFFPLMN_STR                "3F007F206F7B"
#define USIMM_GSM_EFLOCI_STR                 "3F007F206F7E"
#define USIMM_GSM_EFAD_STR                   "3F007F206FAD"
#define USIMM_GSM_EFPHASE_STR                "3F007F206FAE"
#define USIMM_GSM_EFVGCS_STR                 "3F007F206FB1"
#define USIMM_GSM_EFVGCSS_STR                "3F007F206FB2"
#define USIMM_GSM_EFVBS_STR                  "3F007F206FB3"
#define USIMM_GSM_EFVBSS_STR                 "3F007F206FB4"
#define USIMM_GSM_EFEMLPP_STR                "3F007F206FB5"
#define USIMM_GSM_EFAAEM_STR                 "3F007F206FB6"
#define USIMM_GSM_EFECC_STR                  "3F007F206FB7"
#define USIMM_GSM_EFCBMIR_STR                "3F007F206F50"
#define USIMM_GSM_EFNIA_STR                  "3F007F206F51"
#define USIMM_GSM_EFKCGPRS_STR               "3F007F206F52"
#define USIMM_GSM_EFLOCIGPRS_STR             "3F007F206F53"
#define USIMM_GSM_EFSUME_STR                 "3F007F206F54"
#define USIMM_GSM_EFCMI_STR                  "3F007F206F58"
#define USIMM_GSM_EFPLMNWACT_STR             "3F007F206F60"
#define USIMM_GSM_EFOPLMNWACT_STR            "3F007F206F61"
#define USIMM_GSM_EFHPLMNACT_STR             "3F007F206F62"
#define USIMM_GSM_EFCPBCCH_STR               "3F007F206F63"
#define USIMM_GSM_EFINVSCAN_STR              "3F007F206F64"
#define USIMM_GSM_EFPNN_STR                  "3F007F206FC5"
#define USIMM_GSM_EFOPL_STR                  "3F007F206FC6"
#define USIMM_GSM_EFMBDN_STR                 "3F007F206FC7"
#define USIMM_GSM_EFEXT6_STR                 "3F007F206FC8"
#define USIMM_GSM_EFMBI_STR                  "3F007F206FC9"
#define USIMM_GSM_EFMWIS_STR                 "3F007F206FCA"
#define USIMM_GSM_EFCFIS_STR                 "3F007F206FCB"
#define USIMM_GSM_EFEXT7_STR                 "3F007F206FCC"
#define USIMM_GSM_EFSPDI_STR                 "3F007F206FCD"
#define USIMM_GSM_EFMMSN_STR                 "3F007F206FCE"
#define USIMM_GSM_EFEXT8_STR                 "3F007F206FCF"
#define USIMM_GSM_EFMMSICP_STR               "3F007F206FD0"
#define USIMM_GSM_EFMMSUP_STR                "3F007F206FD1"
#define USIMM_GSM_EFMMSUCP_STR               "3F007F206FD2"

/* EF for CPHS, under DFGSM */
#define USIMM_GSM_EFVMW_STR                  "3F007F206F11"
#define USIMM_GSM_EFSVCSTBL_STR              "3F007F206F12"
#define USIMM_GSM_EFCFF_STR                  "3F007F206F13"
#define USIMM_GSM_EFONS_STR                  "3F007F206F14"
#define USIMM_GSM_EFCSP_STR                  "3F007F206F15"
#define USIMM_GSM_EFCPHSI_STR                "3F007F206F16"
#define USIMM_GSM_EFMN_STR                   "3F007F206F17"
#define USIMM_GSM_EFONSS_STR                 "3F007F206F18"
#define USIMM_GSM_EFINFONUM_STR              "3F007F206F19"

/* EF under MF/DFGSM/DFSOLSA */
#define USIMM_GSM_DFSOLSA_STR                "3F007F205F70"
#define USIMM_GSM_DFSOLSA_EFSAI_STR          "3F007F205F704F30"
#define USIMM_GSM_DFSOLSA_EFSLL_STR          "3F007F205F704F31"

/* EF under MF/DFGSM/DFMExE */
#define USIMM_GSM_DFMEXE_STR                 "3F007F205F3C"
#define USIMM_GSM_DFMEXE_EFST_STR            "3F007F205F3C4F40"
#define USIMM_GSM_DFMEXE_EFORPK_STR          "3F007F205F3C4F41"
#define USIMM_GSM_DFMEXE_EFARPK_STR          "3F007F205F3C4F42"
#define USIMM_GSM_DFMEXE_EFTPRPK_STR         "3F007F205F3C4F43"

/* EF for ATT */
#define USIMM_ATTGSM_STR                     "3F007F66"
#define USIMM_ATTGSM_EFTERMINALTBL_STR       "3F007F666FD2"
#define USIMM_ATTGSM_EFACTINGHPLMN_STR       "3F007F665F304F34"
#define USIMM_ATTGSM_EFPRMENABLE_STR         "3F007F665F404F40"
#define USIMM_ATTGSM_EFPRMPARA_STR           "3F007F665F404F41"
#define USIMM_ATTGSM_EFPRMOMCLR_STR          "3F007F665F404F42"
#define USIMM_ATTGSM_EFPRMOMC_STR            "3F007F665F404F43"
#define USIMM_ATTGSM_EFPRMVERINFO_STR        "3F007F665F404F44"

#define USIMM_ATTUSIM_STR                    "3F007FFF7F66"
#define USIMM_ATTUSIM_EFTERMINALTBL_STR      "3F007FFF7F666FD2"
#define USIMM_ATTUSIM_EFACTINGHPLMN_STR      "3F007FFF7F665F304F34"
#define USIMM_ATTUSIM_EFRATMODE_STR          "3F007FFF7F665F304F36"
#define USIMM_ATTUSIM_EFPRMENABLE_STR        "3F007FFF7F665F404F40"
#define USIMM_ATTUSIM_EFPRMPARA_STR          "3F007FFF7F665F404F41"
#define USIMM_ATTUSIM_EFPRMOMCLR_STR         "3F007FFF7F665F404F42"
#define USIMM_ATTUSIM_EFPRMOMC_STR           "3F007FFF7F665F404F43"
#define USIMM_ATTUSIM_EFPRMVERINFO_STR       "3F007FFF7F665F404F44"

/* EF under DFCDMA */
#define USIMM_CDMA_STR                       "3F007F25"
#define USIMM_CDMA_EFCOUNT_STR               "3F007F256F21"
#define USIMM_CDMA_EFIMSIM_STR               "3F007F256F22"
#define USIMM_CDMA_EFIMSIT_STR               "3F007F256F23"
#define USIMM_CDMA_EFTMSI_STR                "3F007F256F24"
#define USIMM_CDMA_EFAH_STR                  "3F007F256F25"
#define USIMM_CDMA_EFAOP_STR                 "3F007F256F26"
#define USIMM_CDMA_EFALOC_STR                "3F007F256F27"
#define USIMM_CDMA_EFCDMAHOME_STR            "3F007F256F28"
#define USIMM_CDMA_EFZNREGI_STR              "3F007F256F29"
#define USIMM_CDMA_EFSNREGI_STR              "3F007F256F2A"
#define USIMM_CDMA_EFDISTREGI_STR            "3F007F256F2B"
#define USIMM_CDMA_EFACCOLC_STR              "3F007F256F2C"
#define USIMM_CDMA_EFTERM_STR                "3F007F256F2D"
#define USIMM_CDMA_EFSSCI_STR                "3F007F256F2E"
#define USIMM_CDMA_EFACP_STR                 "3F007F256F2F"
#define USIMM_CDMA_EFPRL_STR                 "3F007F256F30"
#define USIMM_CDMA_EFRUIMID_STR              "3F007F256F31"
#define USIMM_CDMA_EFCST_STR                 "3F007F256F32"
#define USIMM_CDMA_EFSPC_STR                 "3F007F256F33"
#define USIMM_CDMA_EFOTAPASPC_STR            "3F007F256F34"
#define USIMM_CDMA_EFNAMLOCK_STR             "3F007F256F35"
#define USIMM_CDMA_EFOTA_STR                 "3F007F256F36"
#define USIMM_CDMA_EFSP_STR                  "3F007F256F37"
#define USIMM_CDMA_EFESNMEIDME_STR           "3F007F256F38"
#define USIMM_CDMA_EFREVISION_STR            "3F007F256F39"
#define USIMM_CDMA_EFRUIMPL_STR              "3F007F256F3A"
#define USIMM_CDMA_EFSMS_STR                 "3F007F256F3C"
#define USIMM_CDMA_EFSMSP_STR                "3F007F256F3D"
#define USIMM_CDMA_EFSMSS_STR                "3F007F256F3E"
#define USIMM_CDMA_EFSSFC_STR                "3F007F256F3F"
#define USIMM_CDMA_EFSPN_STR                 "3F007F256F41"
#define USIMM_CDMA_EFUSGIND_STR              "3F007F256F42"
#define USIMM_CDMA_EFAD_STR                  "3F007F256F43"
#define USIMM_CDMA_EFMDN_STR                 "3F007F256F44"
#define USIMM_CDMA_EFMAXPRL_STR              "3F007F256F45"
#define USIMM_CDMA_EFSPCS_STR                "3F007F256F46"
#define USIMM_CDMA_EFECC_STR                 "3F007F256F47"
#define USIMM_CDMA_EFME3GPDOPC_STR           "3F007F256F48"
#define USIMM_CDMA_EF3GPDOPM_STR             "3F007F256F49"
#define USIMM_CDMA_EFSIPCAP_STR              "3F007F256F4A"
#define USIMM_CDMA_EFMIPCAP_STR              "3F007F256F4B"
#define USIMM_CDMA_EFSIPUPP_STR              "3F007F256F4C"
#define USIMM_CDMA_EFMIPUPP_STR              "3F007F256F4D"
#define USIMM_CDMA_EFSIPSP_STR               "3F007F256F4E"
#define USIMM_CDMA_EFMIPSP_STR               "3F007F256F4F"
#define USIMM_CDMA_EFSIPPAPSS_STR            "3F007F256F50"
#define USIMM_CDMA_EFPUZL_STR                "3F007F256F53"
#define USIMM_CDMA_EFMAXPUZL_STR             "3F007F256F54"
#define USIMM_CDMA_EFMECRP_STR               "3F007F256F55"
#define USIMM_CDMA_EFHRPDCAP_STR             "3F007F256F56"
#define USIMM_CDMA_EFHRPDUPP_STR             "3F007F256F57"
#define USIMM_CDMA_EFCSSPR_STR               "3F007F256F58"
#define USIMM_CDMA_EFACT_STR                 "3F007F256F59"
#define USIMM_CDMA_EFEPRL_STR                "3F007F256F5A"
#define USIMM_CDMA_EFBCSMSOFG_STR            "3F007F256F5B"
#define USIMM_CDMA_EFBCSMSPREF_STR           "3F007F256F5C"
#define USIMM_CDMA_EFBCSMSTBL_STR            "3F007F256F5D"
#define USIMM_CDMA_EFBCSMSP_STR              "3F007F256F5E"
#define USIMM_CDMA_EFIMPI_STR                "3F007F256F5F"
#define USIMM_CDMA_EFDOMAIN_STR              "3F007F256F60"
#define USIMM_CDMA_EFIMPU_STR                "3F007F256F61"
#define USIMM_CDMA_EFPCSCF_STR               "3F007F256F62"
#define USIMM_CDMA_EFBAKPARA_STR             "3F007F256F63"
#define USIMM_CDMA_EFUPBAKPARA_STR           "3F007F256F64"
#define USIMM_CDMA_EFMMSN_STR                "3F007F256F65"
#define USIMM_CDMA_EFEXT8_STR                "3F007F256F66"
#define USIMM_CDMA_EFMMSICP_STR              "3F007F256F67"
#define USIMM_CDMA_EFMMSUP_STR               "3F007F256F68"
#define USIMM_CDMA_EFMMSUCP_STR              "3F007F256F69"
#define USIMM_CDMA_EFAUTHCAP_STR             "3F007F256F6A"
#define USIMM_CDMA_EF3GCIK_STR               "3F007F256F6B"
#define USIMM_CDMA_EFDCK_STR                 "3F007F256F6C"
#define USIMM_CDMA_EFGID1_STR                "3F007F256F6D"
#define USIMM_CDMA_EFGID2_STR                "3F007F256F6E"
#define USIMM_CDMA_EFCDMACNL_STR             "3F007F256F6F"
#define USIMM_CDMA_EFHOMETAG_STR             "3F007F256F70"
#define USIMM_CDMA_EFGROUPTAG_STR            "3F007F256F71"
#define USIMM_CDMA_EFSPECTAG_STR             "3F007F256F72"
#define USIMM_CDMA_EFCALLPROM_STR            "3F007F256F73"
#define USIMM_CDMA_EFSFEUIMID_STR            "3F007F256F74"
#define USIMM_CDMA_EFSMSCAP_STR              "3F007F256F76"
#define USIMM_CDMA_EFIPV6CAP_STR             "3F007F256F77"
#define USIMM_CDMA_EFMIPFLAG_STR             "3F007F256F78"
#define USIMM_CDMA_EFTCPCFG_STR              "3F007F256F79"
#define USIMM_CDMA_EFDGC_STR                 "3F007F256F7A"
#define USIMM_CDMA_EFWAPBRCP_STR             "3F007F256F7B"
#define USIMM_CDMA_EFWAPBRBM_STR             "3F007F256F7C"
#define USIMM_CDMA_EF3GPDUPPEX_STR           "3F007F256F7D"
#define USIMM_CDMA_EFMMSCFG_STR              "3F007F256F7E"
#define USIMM_CDMA_EFJDL_STR                 "3F007F256F7F"
#define USIMM_CDMA_EFMODEL_STR               "3F007F256F90"
#define USIMM_CDMA_EFRC_STR                  "3F007F256F91"
#define USIMM_CDMA_EFAPPLAB_STR              "3F007F256F92"

#define USIMM_ISIM_STR                       "3F007FFF"
#define USIMM_ISIM_EFIST_STR                 "3F007FFF6F07"
#define USIMM_ISIM_EFIMPI_STR                "3F007FFF6F02"
#define USIMM_ISIM_EFDOMAIN_STR              "3F007FFF6F03"
#define USIMM_ISIM_EFIMPU_STR                "3F007FFF6F04"
#define USIMM_ISIM_EFAD_STR                  "3F007FFF6FAD"
#define USIMM_ISIM_EFARR_STR                 "3F007FFF6F06"
#define USIMM_ISIM_EFPCSCF_STR               "3F007FFF6F09"
#define USIMM_ISIM_EFGBAP_STR                "3F007FFF6FD5"
#define USIMM_ISIM_EFGBANL_STR               "3F007FFF6FD7"
#define USIMM_ISIM_EFNAFKCA_STR              "3F007FFF6FDD"
#define USIMM_ISIM_EFUICCIARI_STR            "3F007FFF6FE7"
#define USIMM_ISIM_EFSMS_STR                 "3F007FFF6F3C"
#define USIMM_ISIM_EFSMSS_STR                "3F007FFF6F43"
#define USIMM_ISIM_EFSMSR_STR                "3F007FFF6F47"
#define USIMM_ISIM_EFSMSP_STR                "3F007FFF6F42"

#define    USIMM_SLOT_SIZE                  (3)



/*******************************************************************************
3 枚举定义
*******************************************************************************/
enum USIMM_CMDTYPE_REQ_ENUM
{
    USIMM_ACTIVECARD_REQ        = 0,
    USIMM_INITCONTINUE_REQ      = 1,
    USIMM_PROTECTRESET_REQ      = 2,
    USIMM_DEACTIVECARD_REQ      = 3,
    USIMM_AUTHENTICATION_REQ    = 4,
    USIMM_PINHANDLE_REQ         = 5,
    USIMM_UPDATEFILE_REQ        = 6,
    USIMM_READFILE_REQ          = 7,
    USIMM_QUERYFILE_REQ         = 8,
    USIMM_STATUSCMD_REQ         = 9,
    USIMM_SEARCHFILE_REQ        = 10,
    USIMM_FBDNPROCESS_REQ       = 11,
    USIMM_OPENCHANNEL_REQ       = 12,
    USIMM_CLOSECHANNEL_REQ      = 13,
    USIMM_SENDTPDUDATA_REQ      = 14,
    USIMM_STKTERMINALRSP_REQ    = 15,
    USIMM_STKENVELOPE_REQ       = 16,
    USIMM_STKREFRESH_REQ        = 17,
    USIMM_PBINIT_STATUS_IND     = 18,
    USIMM_RACCESS_REQ           = 19,
    USIMM_SETMUTILFILE_REQ      = 20,
    USIMM_CARD_INOUT_IND        = 21,
    USIMM_CARDSTATUS_CB_IND     = 22,
    USIMM_BSCHALLENGE_REQ       = 23,
    USIMM_GENERATE_KEYVPM_REQ   = 24,
    USIMM_MAMAGESSD_REQ         = 25,
    USIMM_CDMASPECAUTH_REQ      = 26,
    USIMM_QUERYESNMEIDRSLT_REQ  = 27,
    USIMM_VSIMFILEDATAWRITE_REQ = 28,
    USIMM_CHANGE_CARDMODE_REQ   = 29,
    USIMM_FDNQUERY_REQ          = 30,
    USIMM_QUERYVOLTAGE_REQ      = 31,
    USIMM_UPDATEFILE_WITHIMSI_REQ = 32,
    USIMM_PRIVATECGLAHANDLE_REQ = 33,
    USIMM_CARDTYPEEX_REQ        = 34,
    USIMM_SILENTPINHANDLE_REQ   = 35,
    USIMM_NVREFRESH_REQ         = 36,
    USIMM_SCI_CFG_SET_REQ       = 37,
    USIMM_SCICHG_IND            = 38,

    USIMM_CMDTYPE_REQ_BUTT
};
typedef VOS_UINT32 USIMM_CMDTYPE_REQ_ENUM_UINT32;

enum USIMM_CMDTYPE_CNF_ENUM
{
    USIMM_ACTIVECARD_CNF        = 0,
    USIMM_INITCONTINUE_CNF      = 1,
    USIMM_PROTECTRESET_CNF      = 2,
    USIMM_DEACTIVECARD_CNF      = 3,
    USIMM_AUTHENTICATION_CNF    = 4,
    USIMM_PINHANDLE_CNF         = 5,
    USIMM_UPDATEFILE_CNF        = 6,
    USIMM_READFILE_CNF          = 7,
    USIMM_QUERYFILE_CNF         = 8,
    USIMM_STATUSCMD_CNF         = 9,
    USIMM_SEARCHFILE_CNF        = 10,
    USIMM_FBDNPROCESS_CNF       = 11,
    USIMM_OPENCHANNEL_CNF       = 12,
    USIMM_CLOSECHANNEL_CNF      = 13,
    USIMM_SENDTPDUDATA_CNF      = 14,
    USIMM_BSCHALLENGE_CNF       = 15,
    USIMM_GENERATE_KEYVPM_CNF   = 16,
    USIMM_MANAGESSD_CNF         = 17,
    USIMM_STKTERMINALRSP_CNF    = 18,
    USIMM_STKREFRESH_CNF        = 19,
    USIMM_STKENVELOPE_CNF       = 20,
    USIMM_RACCESS_CNF           = 21,
    USIMM_SETMUTILFILE_CNF      = 22,
    USIMM_CDMASPECAUTH_CNF      = 23,
    USIMM_QUERYESNMEIDRSLT_CNF  = 24,
    USIMM_CHANGE_CARDMODE_CNF   = 25,
    USIMM_FDNQUERY_CNF          = 26,
    USIMM_QUERYVOLTAGE_CNF      = 27,
    USIMM_PRIVATECGLAHANDLE_CNF = 28,
    USIMM_CARDTYPEEX_CNF        = 29,
    USIMM_SILENTPINHANDLE_CNF   = 30,
    USIMM_NVREFRESH_CNF         = 31,
    USIMM_SCI_CFG_SET_CNF       = 32,
    USIMM_STKENVELOPE_RETRY     = 33,

    USIMM_CARDSTATUS_IND        = 100,
    USIMM_STKREFRESH_IND        = 101,
    USIMM_STKDATA_IND           = 102,
    USIMM_ECCNUMBER_IND         = 103,
    USIMM_VSIM_RDH_IND          = 104,
    USIMM_HOTINOUT_IND          = 105,
    USIMM_XECCNUMBER_IND        = 106,
    USIMM_CARDSCIERR_IND        = 107,
    USIMM_CARDERROR_IND         = 108,
    USIMM_ICCICCONTENT_IND      = 109,
    USIMM_HOTPLUGSTATUS_IND     = 110,
    USIMM_SWCHECKSTATUS_IND     = 111,
    USIMM_FDNSTATUS_IND         = 112,
    USIMM_PRIVATECGLAHANDLE_IND = 113,
    USIMM_CARDFETCH_IND         = 114,

    USIMM_ATR_RPT_IND           = 115,
    USIMM_PB_STATUS_IND         = 116,

    USIMM_SCICHG_CARDSTATUS_IND = 117,

    ID_USIMM_MTA_NV_REFRESH_RSP = 0x2002, /* 防止USIMM->MTA新增消息和ID_MTA_NV_REFRESH_RSP取值重复*/
    USIMM_CMDTYPE_BUTT
};
typedef VOS_UINT32 USIMM_CMDTYPE_CNF_ENUM_UINT32;

enum USIMM_SWCHECK_ENUM
{
    USIMM_SW_OK                 = VOS_OK,   /*命令执行正常*/
    USIMM_SW_ERR                ,   /*= 1,   命令执行错误*/
    USIMM_SW_OK_WITH_RESOPNSE   ,   /*= 2,   命令执行正常，有数据返回*/
    USIMM_SW_OK_WITH_SAT        ,   /*= 3,   命令执行正常，有SAT数据返回*/
    USIMM_SW_CMD_REMAINTIME     ,   /*= 4,   当前命令的剩余次数*/
    USIMM_SW_CARD_BUSY          ,   /*= 5,   当前卡忙*/
    USIMM_SW_AUTN_ERROR         ,   /*= 6,   鉴权错误*/
    USIMM_SW_MEMORY_ERROR       ,   /*= 7,   卡内存错误*/
    USIMM_SW_NOFILE_SELECT      ,   /*= 8,   当前无任何文件选中*/
    USIMM_SW_OUTOF_RANGE        ,   /*= 9,   超出范围*/
    USIMM_SW_NOFILE_FOUND       ,   /*= 10,  文件找不到*/
    USIMM_SW_FILETYPE_ERROR     ,   /*= 11,  文件类型矛盾*/
    USIMM_SW_SECURITY_ERROR     ,   /*= 12,  安全状态不满足*/
    USIMM_SW_PIN_BLOCK          ,   /*= 13,  密码被锁*/
    USIMM_SW_PINSTATUS_ERROR    ,   /*= 14,  密码状态矛盾*/
    USIMM_SW_PINNUMBER_ERROR    ,   /*= 15,  密码索引错误*/
    USIMM_SW_FILESTATUS_ERROR   ,   /*= 16,  文件状态矛盾*/
    USIMM_SW_INCREACE_ERROR     ,   /*= 17,  INCREACE操作错误*/
    USIMM_SW_CLA_ERROR          ,   /*= 18,  CLA错误*/
    USIMM_SW_CMD_ERROR          ,   /*= 19,  参数1、2、3、命令数据错误*/
    USIMM_SW_P3_ERROR           ,   /*= 20,  推荐P3长度*/
    USIMM_SW_NORECORD_FOUND     ,   /*= 21,  无记录*/
    USIMM_SW_DOWNLOAD_ERROR     ,   /*= 22,  下载数据错误*/
    USIMM_SW_CHANNEL_NOSUPPORT  ,   /*= 23,  不支持逻辑通道*/
    USIMM_SW_WARNING            ,   /*= 24,  命令执行告警,需要获取信息*/
    USIMM_SW_OTHER_WARNING      ,   /*= 25,  命令执行告警*/
    USIMM_SW_OTHER_ERROR        ,   /*= 26,  命令执行错误*/
    USIMM_SW_SENDCMD_ERROR      ,   /*= 27,  发送命令错误*/
    USIMM_SW_NO_INFO            ,   /*= 28,   No information given, state of non volatile memory unchanged */
    USIMM_SW_DATA_CORRUPTED     ,   /*= 29,   Part of returned data may be corrupted */
    USIMM_SW_END_OF_FILE        ,   /*= 30,   End of file/record reached before reading Le bytes */
    USIMM_SW_INVALID_FILE       ,   /*= 31,   Selected file invalidated */
    USIMM_SW_TERMINATE_FILE     ,   /*= 32,   Selected file in termination state */
    USIMM_SW_MORE_DATA1         ,   /*= 33,   More data available */
    USIMM_SW_MORE_DATA2         ,   /*= 34,   More data available and proactive command pending */
    USIMM_SW_RESPONSE_DATA      ,   /*= 35,   Authentication response data available */
    USIMM_SW_MORE_DATA3         ,   /*= 36,   More data expected */
    USIMM_SW_MORE_DATA4         ,   /*= 37,   More data expected and proactive command pending */
    USIMM_SW_TECH_ERROR         ,   /*= 38,   Technical problem, no precise diagnosis */
    USIMM_SW_COMMAND_ERROR      ,   /*= 39,   Command not allowed - secure channel - security not satisfied */
    USIMM_SW_EXPIRED_ERROR      ,   /*= 40,   Security session or association expired */
    USIMM_SW_UNHANDLE_ERROR     ,   /*= 41,   状态字与协议定义的INS字段不符 */
    USIMM_SW_CDMA_AUTN_ERROR    ,   /*= 42,   CDMA鉴权过程错误 */
    USIMM_SW_MSGCHECK_ERROR     ,   /*= 43,   参数检查错误 */
    USIMM_SW_INVALID_BAKID      ,   /*= 44,   BCMCS特殊错误 */
    USIMM_SW_INVALID_BCMCSFID   ,   /*= 45,   BCMCS特殊错误 */
    USIMM_SW_DFPATH_ERROR       ,   /*= 46, Status路径不匹配 */
    USIMM_SW_CLA_NOT_SUPPORT    ,   /*= 47,  CLA错误,需往ICC切*/

    USIMM_SW_BUTT
};
typedef VOS_UINT32 USIMM_SWCHECK_ENUM_UINT32;

enum USIMM_CARDAPP_ENUM
{
    USIMM_GUTL_APP          = 0,
    USIMM_IMS_APP           = 1,
    USIMM_CDMA_APP          = 2,
    USIMM_CARDAPP_BUTT      = 3,
    USIMM_UNLIMIT_APP1      = USIMM_CARDAPP_BUTT,
    USIMM_UNLIMIT_APP2      = 4,
    USIMM_UNLIMIT_APP3      = 5,
    USIMM_UNLIMIT_APP4      = 6,
    USIMM_UNLIMIT_APP5      = 7,
    USIMM_UNLIMIT_APP6      = 8,
    USIMM_UNLIMIT_APP7      = 9,
    USIMM_UNLIMIT_APP8      = 10,
    USIMM_UNLIMIT_APP9      = 11,
    USIMM_UNLIMIT_APP10     = 12,
    USIMM_UNLIMIT_APP11     = 13,
    USIMM_UNLIMIT_APP12     = 14,
    USIMM_UNLIMIT_APP13     = 15,
    USIMM_UNLIMIT_APP14     = 16,
    USIMM_UNLIMIT_APP15     = 17,
    USIMM_UNLIMIT_APP16     = 18,
    USIMM_UNLIMIT_APP17     = 19,
    USIMM_UNLIMIT_APP18     = 20,
    USIMM_UNLIMIT_APP19     = 21,
    USIMM_UNLIMIT_AUTO      = 22,
    USIMM_UNLIMIT_APP_BUTT
};
typedef VOS_UINT32 USIMM_CARDAPP_ENUM_UINT32;

enum USIMM_PHYCARD_TYPE_ENUM
{
    USIMM_PHYCARD_TYPE_ICC              = 0x00,
    USIMM_PHYCARD_TYPE_UICC             = 0x01,
    USIMM_PHYCARD_TYPE_NOCARD           = 0xFF,
    USIMM_PHYCARD_TYPE_BUTT
};

typedef VOS_UINT32 USIMM_PHYCARD_TYPE_ENUM_UINT32;

enum USIMM_API_RESULT_ENUM              /* 对外接口返回的错误码 */
{
    USIMM_API_SUCCESS           = VOS_OK,   /* 函数执行成功 */
    USIMM_API_FAILED,                       /* 函数执行失败，原因不明 */
    USIMM_API_WRONG_PARA,                   /* 因为参数错误，函数执行失败 */
    USIMM_API_NOTAVAILABLE,                 /* 因为没有可用应用，函数执行失败 */
    USIMM_API_RESULT_BUTT
};

enum USIMM_PIN_CMD_TYPE_ENUM
{
    USIMM_PINCMD_NULL           = 0x00000000,
    USIMM_PINCMD_VERIFY         = 0x00000020,   /*校验PIN码*/
    USIMM_PINCMD_CHANGE         = 0x00000024,   /*更换PIN码的密码*/
    USIMM_PINCMD_DISABLE        = 0x00000026,   /*禁止使用PIN码*/
    USIMM_PINCMD_ENABLE         = 0x00000028,   /*使用PIN码*/
    USIMM_PINCMD_UNBLOCK        = 0x0000002C,   /*解除阻止PIN码*/
    USIMM_PINCMD_BUTT
};
typedef VOS_UINT32      USIMM_PIN_CMD_TYPE_ENUM_UINT32;

enum USIMM_PIN_TYPE_ENUM
{
    USIMM_PIN                   = 0x00000000,      /* PIN1*/
    USIMM_PUK                   = 0x00000001,      /* PUK1*/
    USIMM_PIN2                  = 0x00000006,      /* PIN2*/
    USIMM_PUK2                  = 0x00000007,      /* PUK2 */
    USIMM_PHSIM_PIN             = 0x00000008,      /* PH-SIM PIN*/
    USIMM_PHNET_PIN             = 0x00000009,      /* PH-NET PIN*/
    USIMM_SIM_NON               = 0x000000FF,      /* 不确定操作PIN的类型时填入此值*/
    USIMM_PIN_TYPE_BUTT
};
typedef VOS_UINT32      USIMM_PIN_TYPE_ENUM_UINT32;

enum  USIMM_PIN_ENABLE_STATUS_ENUM
{
    USIMM_PIN_DISABLED      = 0,      /*未使能*/
    USIMM_PIN_ENABLED       = 1,       /*已经使能*/
    USIMM_PIN_ENABLE_BUTT
};
typedef VOS_UINT32      USIMM_PIN_ENABLE_STATUS_ENUM_UINT32;

enum USIMM_PIN_VERIY_STATUS_ENUM
{
    USIMM_PIN_NONEED        = 0, /*不需要验证*/
    USIMM_PIN_NEED          = 1, /*需要验证*/
    USIMM_PUK_NEED          = 2, /*需要验证puk*/
    USIMM_PUK_BLOCK         = 3, /*需要验证ADM*/
    USIMM_PIN_VERIY_BUTT
};
typedef VOS_UINT32      USIMM_PIN_VERIY_STATUS_ENUM_UINT32;

enum USIMM_REFRESH_TYPE_ENUM
{
    USIMM_INITIAL_FULL_FILE_CHANGE_NOTIFY= 0,
    USIMM_FILE_CHANGE_NOTIFY             = 1,
    USIMM_INITIAL_FILE_CHANGE_NOTIFY     = 2,
    USIMM_INITIALIZATION                 = 3,
    USIMM_RESET                          = 4,
    USIMM_APPLICATION_RESET_3G_ONLY      = 5,
    USIMM_SESSION_RESET_3G_ONLY          = 6,
    USIMM_STEERING_OF_ROAMING            = 7,
    USIMM_REFRESH_BUTT
};
typedef VOS_UINT32      USIMM_REFRESH_TYPE_ENUM_UINT32;

enum USIMM_DEF_FILEID_ENUM
{
/* EF under MF */
    USIMM_MF_ID                         = 0,
    USIMM_DIR_ID                       ,
    USIMM_ICCID_ID                     ,
    USIMM_PL_ID                        ,
    USIMM_ARR_ID                       ,
    USIMM_USIM_ID                      ,
    USIMM_USIM_EFLI_ID                 ,
    USIMM_USIM_EFARR_ID                ,
    USIMM_USIM_EFIMSI_ID               ,
    USIMM_USIM_EFKEYS_ID               ,
    USIMM_USIM_EFKEYSPS_ID             ,
    USIMM_USIM_EFCSP_ID                ,
    USIMM_USIM_EFDCK_ID                ,
    USIMM_USIM_EFHPPLMN_ID             ,
    USIMM_USIM_EFCNL_ID                ,
    USIMM_USIM_EFACMMAX_ID             ,
    USIMM_USIM_EFUST_ID                ,
    USIMM_USIM_EFACM_ID                ,
    USIMM_USIM_EFFDN_ID                ,
    USIMM_USIM_EFSMS_ID                ,
    USIMM_USIM_EFGID1_ID               ,
    USIMM_USIM_EFGID2_ID               ,
    USIMM_USIM_EFMSISDN_ID             ,
    USIMM_USIM_EFPUCT_ID               ,
    USIMM_USIM_EFSMSP_ID               ,
    USIMM_USIM_EFSMSS_ID               ,
    USIMM_USIM_EFCBMI_ID               ,
    USIMM_USIM_EFSPN_ID                ,
    USIMM_USIM_EFSMSR_ID               ,
    USIMM_USIM_EFCBMID_ID              ,
    USIMM_USIM_EFSDN_ID                ,
    USIMM_USIM_EFEXT2_ID               ,
    USIMM_USIM_EFEXT3_ID               ,
    USIMM_USIM_EFBDN_ID                ,
    USIMM_USIM_EFEXT5_ID               ,
    USIMM_USIM_EFCCP2_ID               ,
    USIMM_USIM_EFCBMIR_ID              ,
    USIMM_USIM_EFEXT4_ID               ,
    USIMM_USIM_EFEST_ID                ,
    USIMM_USIM_EFACL_ID                ,
    USIMM_USIM_EFCMI_ID                ,
    USIMM_USIM_EFSTART_HFN_ID          ,
    USIMM_USIM_EFTHRESHOL_ID           ,
    USIMM_USIM_EFPLMNWACT_ID           ,
    USIMM_USIM_EFOPLMNWACT_ID          ,
    USIMM_USIM_EFHPLMNwACT_ID          ,
    USIMM_USIM_EFPSLOCI_ID             ,
    USIMM_USIM_EFACC_ID                ,
    USIMM_USIM_EFFPLMN_ID              ,
    USIMM_USIM_EFLOCI_ID               ,
    USIMM_USIM_EFICI_ID                ,
    USIMM_USIM_EFOCI_ID                ,
    USIMM_USIM_EFICT_ID                ,
    USIMM_USIM_EFOCT_ID                ,
    USIMM_USIM_EFAD_ID                 ,
    USIMM_USIM_EFVGCS_ID               ,
    USIMM_USIM_EFVGCSS_ID              ,
    USIMM_USIM_EFVBS_ID                ,
    USIMM_USIM_EFVBSS_ID               ,
    USIMM_USIM_EFEMLPP_ID              ,
    USIMM_USIM_EFAAEM_ID               ,
    USIMM_USIM_EFECC_ID                ,
    USIMM_USIM_EFHIDDENKEY_ID          ,
    USIMM_USIM_EFNETPAR_ID             ,
    USIMM_USIM_EFPNN_ID                ,
    USIMM_USIM_EFOPL_ID                ,
    USIMM_USIM_EFMBDN_ID               ,
    USIMM_USIM_EFEXT6_ID               ,
    USIMM_USIM_EFMBI_ID                ,
    USIMM_USIM_EFMWIS_ID               ,
    USIMM_USIM_EFCFIS_ID               ,
    USIMM_USIM_EFEXT7_ID               ,
    USIMM_USIM_EFSPDI_ID               ,
    USIMM_USIM_EFMMSN_ID               ,
    USIMM_USIM_EFEXT8_ID               ,
    USIMM_USIM_EFMMSICP_ID             ,
    USIMM_USIM_EFMMSUP_ID              ,
    USIMM_USIM_EFMMSUCP_ID             ,
    USIMM_USIM_EFNIA_ID                ,
    USIMM_USIM_EFVGCSCA_ID             ,
    USIMM_USIM_EFVBSCA_ID              ,
    USIMM_USIM_EFGBAP_ID               ,
    USIMM_USIM_EFMSK_ID                ,
    USIMM_USIM_EFMUK_ID                ,
    USIMM_USIM_EFEHPLMN_ID             ,
    USIMM_USIM_EFGBANL_ID              ,
    USIMM_USIM_EFEHPLMNPI_ID           ,
    USIMM_USIM_EFLRPLMNSI_ID           ,
    USIMM_USIM_EFNAFKCA_ID             ,
    USIMM_USIM_EFSPNI_ID               ,
    USIMM_USIM_EFPNNI_ID               ,
    USIMM_USIM_EFNCPIP_ID              ,
    USIMM_USIM_EFEPSLOCI_ID            ,
    USIMM_USIM_EFEPSNSC_ID             ,
    USIMM_USIM_EFUFC_ID                ,
    USIMM_USIM_EFUICCIARI_ID           ,
    USIMM_USIM_EFNASCONFIG_ID          ,
    USIMM_USIM_EFPWS_ID                ,
    USIMM_USIM_EFSDNURI_ID             ,
    USIMM_USIM_EFEPDGID_ID             ,
    USIMM_USIM_EFEPDGSELECTION_ID      ,
    USIMM_USIM_EFEPDGIDEM_ID           ,
    USIMM_USIM_EFEPDGSELECTIONEM_ID    ,
    USIMM_USIM_DFPHONEBOOK_ID          ,
    USIMM_USIM_EFPSC_ID                ,
    USIMM_USIM_EFCC_ID                 ,
    USIMM_USIM_EFPUID_ID               ,
    USIMM_USIM_EFPBR_ID                ,
    USIMM_USIM_DFGSM_ACCESS_ID         ,
    USIMM_USIM_EFKC_ID                 ,
    USIMM_USIM_EFKCGPRS_ID             ,
    USIMM_USIM_EFCPBCCH_ID             ,
    USIMM_USIM_EFINVSCAN_ID            ,
    USIMM_USIM_DFMEXE_ID               ,
    USIMM_USIM_EFMexE_ST_ID            ,
    USIMM_USIM_EFORPK_ID               ,
    USIMM_USIM_EFARPK_ID               ,
    USIMM_USIM_EFTPRK_ID               ,
    USIMM_USIM_DFSOLSA_ID              ,
    USIMM_USIM_EFSAI_ID                ,
    USIMM_USIM_EFSLL_ID                ,
    USIMM_USIM_DFWLAN_ID               ,
    USIMM_USIM_EFPSEUDO_ID             ,
    USIMM_USIM_EFUPLMNWLAN_ID          ,
    USIMM_USIM_EF0PLMNWLAN_ID          ,
    USIMM_USIM_EFUWSIDL_ID             ,
    USIMM_USIM_EFOWSIDL_ID             ,
    USIMM_USIM_EFWRI_ID                ,
    USIMM_USIM_EFHWSIDL_ID             ,
    USIMM_USIM_EFWEHPLMNPI_ID          ,
    USIMM_USIM_EFWHPI_ID               ,
    USIMM_USIM_EFWLRPLMN_ID            ,
    USIMM_USIM_EFHPLMNDAI_ID           ,
    USIMM_USIM_DFHNB_ID                ,
    USIMM_USIM_EFACSGL_ID              ,
    USIMM_USIM_EFCSGT_ID               ,
    USIMM_USIM_EFHNBN_ID               ,
    USIMM_USIM_EFOCSGL_ID              ,
    USIMM_USIM_EFOCSGT_ID              ,
    USIMM_USIM_EFOHNBN_ID              ,
    USIMM_USIM_DFPROSE_ID              ,
    USIMM_USIM_EFPST_ID                ,
    USIMM_USIM_DFACDC_ID               ,
    USIMM_USIM_EFACDCLIST_ID           ,
    USIMM_USIM_EF5GS3GPPLOCI_ID        ,
    USIMM_USIM_EF5GAUTHKEYS_ID         ,
    USIMM_USIM_EF5GS3GPPNSC_ID         ,
    USIMM_USIM_EFUAC_AIC_ID            ,
    USIMM_TELE_ID                      ,
    USIMM_TELE_EFADN_ID                ,
    USIMM_TELE_EFFDN_ID                ,
    USIMM_TELE_EFSMS_ID                ,
    USIMM_TELE_EFCCP_ID                ,
    USIMM_TELE_EFMSISDN_ID             ,
    USIMM_TELE_EFSMSP_ID               ,
    USIMM_TELE_EFSMSS_ID               ,
    USIMM_TELE_EFLND_ID                ,
    USIMM_TELE_EFSMSR_ID               ,
    USIMM_TELE_EFSDN_ID                ,
    USIMM_TELE_EFEXT1_ID               ,
    USIMM_TELE_EFEXT2_ID               ,
    USIMM_TELE_EFEXT3_ID               ,
    USIMM_TELE_EFBDN_ID                ,
    USIMM_TELE_EFEXT4_ID               ,
    USIMM_TELE_EFECCP_ID               ,
    USIMM_TELE_EFRMA_ID                ,
    USIMM_TELE_EFSUME_ID               ,
    USIMM_TELE_EFICE_DN_ID             ,
    USIMM_TELE_EFICE_FF_ID             ,
    USIMM_TELE_EFPSISMSC_ID            ,
    USIMM_TELE_DFGRAP_ID               ,
    USIMM_TELE_DFGRAP_EFIMG_ID         ,
    USIMM_TELE_DFGRAP_EFICEGRAPH_ID    ,
    USIMM_TELE_DFGRAP_EFLAUNCHSCWS_ID  ,
    USIMM_TELE_DFPB_ID                 ,
    USIMM_TELE_DFPB_EFPSC_ID           ,
    USIMM_TELE_DFPB_EFCC_ID            ,
    USIMM_TELE_DFPB_EFPUID_ID          ,
    USIMM_TELE_DFPB_EFPBR_ID           ,
    USIMM_TELE_DFMM_ID                 ,
    USIMM_TELE_DFMM_EFMML_ID           ,
    USIMM_TELE_DFMUM_EFMMDF_ID         ,
    USIMM_TELE_DFMSS_ID                ,
    USIMM_TELE_EFMLPL_ID               ,
    USIMM_TELE_EFMSPL_ID               ,
    USIMM_TELE_EFMMSSMODE_ID           ,
    USIMM_TELE_DFMCPTT_ID              ,
    USIMM_TELE_EFMST_ID                ,
    USIMM_CSIM_ID                      ,
    USIMM_CSIM_EFCOUNT_ID              ,
    USIMM_CSIM_EFIMSIM_ID              ,
    USIMM_CSIM_EFIMSIT_ID              ,
    USIMM_CSIM_EFTMSI_ID               ,
    USIMM_CSIM_EFAH_ID                 ,
    USIMM_CSIM_EFAOP_ID                ,
    USIMM_CSIM_EFALOC_ID               ,
    USIMM_CSIM_EFCDMAHOME_ID           ,
    USIMM_CSIM_EFZNREGI_ID             ,
    USIMM_CSIM_EFSNREGI_ID             ,
    USIMM_CSIM_EFDISTREGI_ID           ,
    USIMM_CSIM_EFACCOLC_ID             ,
    USIMM_CSIM_EFTERM_ID               ,
    USIMM_CSIM_EFSSCI_ID               ,
    USIMM_CSIM_EFACP_ID                ,
    USIMM_CSIM_EFPRL_ID                ,
    USIMM_CSIM_EFRUIMID_ID             ,
    USIMM_CSIM_EFCSIMST_ID             ,
    USIMM_CSIM_EFSPC_ID                ,
    USIMM_CSIM_EFOTAPASPC_ID           ,
    USIMM_CSIM_EFNAMLOCK_ID            ,
    USIMM_CSIM_EFOTA_ID                ,
    USIMM_CSIM_EFSP_ID                 ,
    USIMM_CSIM_EFESNMEID_ID            ,
    USIMM_CSIM_EFLI_ID                 ,
    USIMM_CSIM_EFFDN_ID                ,
    USIMM_CSIM_EFSMS_ID                ,
    USIMM_CSIM_EFSMSP_ID               ,
    USIMM_CSIM_EFSMSS_ID               ,
    USIMM_CSIM_EFSSFC_ID               ,
    USIMM_CSIM_EFSPN_ID                ,
    USIMM_CSIM_EFUSGIND_ID             ,
    USIMM_CSIM_EFAD_ID                 ,
    USIMM_CSIM_EFMDN_ID                ,
    USIMM_CSIM_EFMAXPRL_ID             ,
    USIMM_CSIM_EFSPCS_ID               ,
    USIMM_CSIM_EFECC_ID                ,
    USIMM_CSIM_EFME3GPDOPC_ID          ,
    USIMM_CSIM_EF3GPDOPM_ID            ,
    USIMM_CSIM_EFSIPCAP_ID             ,
    USIMM_CSIM_EFMIPCAP_ID             ,
    USIMM_CSIM_EFSIPUPP_ID             ,
    USIMM_CSIM_EFMIPUPP_ID             ,
    USIMM_CSIM_EFSIPSP_ID              ,
    USIMM_CSIM_EFMIPSP_ID              ,
    USIMM_CSIM_EFSIPPAPSS_ID           ,
    USIMM_CSIM_EFPUZL_ID               ,
    USIMM_CSIM_EFMAXPUZL_ID            ,
    USIMM_CSIM_EFMECRP_ID              ,
    USIMM_CSIM_EFHRPDCAP_ID            ,
    USIMM_CSIM_EFHRPDUPP_ID            ,
    USIMM_CSIM_EFCSSPR_ID              ,
    USIMM_CSIM_EFATC_ID                ,
    USIMM_CSIM_EFEPRL_ID               ,
    USIMM_CSIM_EFBCSMSCFG_ID           ,
    USIMM_CSIM_EFBCSMSPREF_ID          ,
    USIMM_CSIM_EFBCSMSTABLE_ID         ,
    USIMM_CSIM_EFBCSMSP_ID             ,
    USIMM_CSIM_EFBAKPARA_ID            ,
    USIMM_CSIM_EFUPBAKPARA_ID          ,
    USIMM_CSIM_EFMMSN_ID               ,
    USIMM_CSIM_EFEXT8_ID               ,
    USIMM_CSIM_EFMMSICP_ID             ,
    USIMM_CSIM_EFMMSUP_ID              ,
    USIMM_CSIM_EFMMSUCP_ID             ,
    USIMM_CSIM_EFAUTHCAP_ID            ,
    USIMM_CSIM_EF3GCIK_ID              ,
    USIMM_CSIM_EFDCK_ID                ,
    USIMM_CSIM_EFGID1_ID               ,
    USIMM_CSIM_EFGID2_ID               ,
    USIMM_CSIM_EFCDMACNL_ID            ,
    USIMM_CSIM_EFHOMETAG_ID            ,
    USIMM_CSIM_EFGROUPTAG_ID           ,
    USIMM_CSIM_EFSPECTAG_ID            ,
    USIMM_CSIM_EFCALLPROMPT_ID         ,
    USIMM_CSIM_EFSFEUIMID_ID           ,
    USIMM_CSIM_EFEST_ID                ,
    USIMM_CSIM_EFHIDDENKEY_ID          ,
    USIMM_CSIM_EFLCSVER_ID             ,
    USIMM_CSIM_EFLCSCP_ID              ,
    USIMM_CSIM_EFSDN_ID                ,
    USIMM_CSIM_EFEXT2_ID               ,
    USIMM_CSIM_EFEXT3_ID               ,
    USIMM_CSIM_EFICI_ID                ,
    USIMM_CSIM_EFOCI_ID                ,
    USIMM_CSIM_EFEXT5_ID               ,
    USIMM_CSIM_EFCCP2_ID               ,
    USIMM_CSIM_EFAPPLAB_ID             ,
    USIMM_CSIM_EFMODEL_ID              ,
    USIMM_CSIM_EFRC_ID                 ,
    USIMM_CSIM_EFSMSCAP_ID             ,
    USIMM_CSIM_EFMIPFLAGS_ID           ,
    USIMM_CSIM_EF3GPDUPPEXT_ID         ,
    USIMM_CSIM_EFIPV6CAP_ID            ,
    USIMM_CSIM_EFTCPCFG_ID             ,
    USIMM_CSIM_EFDGC_ID                ,
    USIMM_CSIM_EFWAPBWCP_ID            ,
    USIMM_CSIM_EFWAPBWBM_ID            ,
    USIMM_CSIM_EFMMSCFG_ID             ,
    USIMM_CSIM_EFJDL_ID                ,
    USIMM_CSIM_DFPHONEBOOK_ID          ,
    USIMM_CSIM_EFPSC_ID                ,
    USIMM_CSIM_EFCC_ID                 ,
    USIMM_CSIM_EFPUID_ID               ,
    USIMM_CSIM_EFPBR_ID                ,
/* EF under MF/DFGSM */
    USIMM_GSM_ID                       ,
    USIMM_GSM_EFLP_ID                  ,
    USIMM_GSM_EFIMSI_ID                ,
    USIMM_GSM_EFKC_ID                  ,
    USIMM_GSM_EFDCK_ID                 ,
    USIMM_GSM_EFPLMNSEL_ID             ,
    USIMM_GSM_EFHPLMN_ID               ,
    USIMM_GSM_EFCNL_ID                 ,
    USIMM_GSM_EFACMMAX_ID              ,
    USIMM_GSM_EFSST_ID                 ,
    USIMM_GSM_EFACM_ID                 ,
    USIMM_GSM_EFGID1_ID                ,
    USIMM_GSM_EFGID2_ID                ,
    USIMM_GSM_EFPUCT_ID                ,
    USIMM_GSM_EFCBMI_ID                ,
    USIMM_GSM_EFSPN_ID                 ,
    USIMM_GSM_EFCBMID_ID               ,
    USIMM_GSM_EFBCCH_ID                ,
    USIMM_GSM_EFACC_ID                 ,
    USIMM_GSM_EFFPLMN_ID               ,
    USIMM_GSM_EFLOCI_ID                ,
    USIMM_GSM_EFAD_ID                  ,
    USIMM_GSM_EFPHASE_ID               ,
    USIMM_GSM_EFVGCS_ID                ,
    USIMM_GSM_EFVGCSS_ID               ,
    USIMM_GSM_EFVBS_ID                 ,
    USIMM_GSM_EFVBSS_ID                ,
    USIMM_GSM_EFEMLPP_ID               ,
    USIMM_GSM_EFAAEM_ID                ,
    USIMM_GSM_EFECC_ID                 ,
    USIMM_GSM_EFCBMIR_ID               ,
    USIMM_GSM_EFNIA_ID                 ,
    USIMM_GSM_EFKCGPRS_ID              ,
    USIMM_GSM_EFLOCIGPRS_ID            ,
    USIMM_GSM_EFSUME_ID                ,
    USIMM_GSM_EFCMI_ID                 ,
    USIMM_GSM_EFPLMNWACT_ID            ,
    USIMM_GSM_EFOPLMNWACT_ID           ,
    USIMM_GSM_EFHPLMNACT_ID            ,
    USIMM_GSM_EFCPBCCH_ID              ,
    USIMM_GSM_EFINVSCAN_ID             ,
    USIMM_GSM_EFPNN_ID                 ,
    USIMM_GSM_EFOPL_ID                 ,
    USIMM_GSM_EFMBDN_ID                ,
    USIMM_GSM_EFEXT6_ID                ,
    USIMM_GSM_EFMBI_ID                 ,
    USIMM_GSM_EFMWIS_ID                ,
    USIMM_GSM_EFCFIS_ID                ,
    USIMM_GSM_EFEXT7_ID                ,
    USIMM_GSM_EFSPDI_ID                ,
    USIMM_GSM_EFMMSN_ID                ,
    USIMM_GSM_EFEXT8_ID                ,
    USIMM_GSM_EFMMSICP_ID              ,
    USIMM_GSM_EFMMSUP_ID               ,
    USIMM_GSM_EFMMSUCP_ID              ,
/* EF for CPHS, under DFGSM */
    USIMM_GSM_EFVMW_ID                 ,
    USIMM_GSM_EFSVCSTBL_ID             ,
    USIMM_GSM_EFCFF_ID                 ,
    USIMM_GSM_EFONS_ID                 ,
    USIMM_GSM_EFCSP_ID                 ,
    USIMM_GSM_EFCPHSI_ID               ,
    USIMM_GSM_EFMN_ID                  ,
    USIMM_GSM_EFONSS_ID                ,
    USIMM_GSM_EFINFONUM_ID             ,
/* EF under MF/DFGSM/DFSOLSA */
    USIMM_GSM_DFSOLSA_ID               ,
    USIMM_GSM_DFSOLSA_EFSAI_ID         ,
    USIMM_GSM_DFSOLSA_EFSLL_ID         ,
/* EF under MF/DFGSM/DFMExE */
    USIMM_GSM_DFMEXE_ID                ,
    USIMM_GSM_DFMEXE_EFST_ID           ,
    USIMM_GSM_DFMEXE_EFORPK_ID         ,
    USIMM_GSM_DFMEXE_EFARPK_ID         ,
    USIMM_GSM_DFMEXE_EFTPRPK_ID        ,
/* EF for ATT */
    USIMM_ATTGSM_ID                    ,
    USIMM_ATTGSM_EFTERMINALTBL_ID      ,
    USIMM_ATTGSM_EFACTINGHPLMN_ID      ,
    USIMM_ATTGSM_EFPRMENABLE_ID        ,
    USIMM_ATTGSM_EFPRMPARA_ID          ,
    USIMM_ATTGSM_EFPRMOMCLR_ID         ,
    USIMM_ATTGSM_EFPRMOMC_ID           ,
    USIMM_ATTGSM_EFPRMVERINFO_ID       ,
    USIMM_ATTUSIM_ID                   ,
    USIMM_ATTUSIM_EFTERMINALTBL_ID     ,
    USIMM_ATTUSIM_EFACTINGHPLMN_ID     ,
    USIMM_ATTUSIM_EFRATMODE_ID         ,
    USIMM_ATTUSIM_EFPRMENABLE_ID       ,
    USIMM_ATTUSIM_EFPRMPARA_ID         ,
    USIMM_ATTUSIM_EFPRMOMCLR_ID        ,
    USIMM_ATTUSIM_EFPRMOMC_ID          ,
    USIMM_ATTUSIM_EFPRMVERINFO_ID      ,
/* EF under DFCDMA */
    USIMM_CDMA_ID                      ,
    USIMM_CDMA_EFCOUNT_ID              ,
    USIMM_CDMA_EFIMSIM_ID              ,
    USIMM_CDMA_EFIMSIT_ID              ,
    USIMM_CDMA_EFTMSI_ID               ,
    USIMM_CDMA_EFAH_ID                 ,
    USIMM_CDMA_EFAOP_ID                ,
    USIMM_CDMA_EFALOC_ID               ,
    USIMM_CDMA_EFCDMAHOME_ID           ,
    USIMM_CDMA_EFZNREGI_ID             ,
    USIMM_CDMA_EFSNREGI_ID             ,
    USIMM_CDMA_EFDISTREGI_ID           ,
    USIMM_CDMA_EFACCOLC_ID             ,
    USIMM_CDMA_EFTERM_ID               ,
    USIMM_CDMA_EFSSCI_ID               ,
    USIMM_CDMA_EFACP_ID                ,
    USIMM_CDMA_EFPRL_ID                ,
    USIMM_CDMA_EFRUIMID_ID             ,
    USIMM_CDMA_EFCST_ID                ,
    USIMM_CDMA_EFSPC_ID                ,
    USIMM_CDMA_EFOTAPASPC_ID           ,
    USIMM_CDMA_EFNAMLOCK_ID            ,
    USIMM_CDMA_EFOTA_ID                ,
    USIMM_CDMA_EFSP_ID                 ,
    USIMM_CDMA_EFESNMEIDME_ID          ,
    USIMM_CDMA_EFREVISION_ID           ,
    USIMM_CDMA_EFRUIMPL_ID             ,
    USIMM_CDMA_EFSMS_ID                ,
    USIMM_CDMA_EFSMSP_ID               ,
    USIMM_CDMA_EFSMSS_ID               ,
    USIMM_CDMA_EFSSFC_ID               ,
    USIMM_CDMA_EFSPN_ID                ,
    USIMM_CDMA_EFUSGIND_ID             ,
    USIMM_CDMA_EFAD_ID                 ,
    USIMM_CDMA_EFMDN_ID                ,
    USIMM_CDMA_EFMAXPRL_ID             ,
    USIMM_CDMA_EFSPCS_ID               ,
    USIMM_CDMA_EFECC_ID                ,
    USIMM_CDMA_EFME3GPDOPC_ID          ,
    USIMM_CDMA_EF3GPDOPM_ID            ,
    USIMM_CDMA_EFSIPCAP_ID             ,
    USIMM_CDMA_EFMIPCAP_ID             ,
    USIMM_CDMA_EFSIPUPP_ID             ,
    USIMM_CDMA_EFMIPUPP_ID             ,
    USIMM_CDMA_EFSIPSP_ID              ,
    USIMM_CDMA_EFMIPSP_ID              ,
    USIMM_CDMA_EFSIPPAPSS_ID           ,
    USIMM_CDMA_EFPUZL_ID               ,
    USIMM_CDMA_EFMAXPUZL_ID            ,
    USIMM_CDMA_EFMECRP_ID              ,
    USIMM_CDMA_EFHRPDCAP_ID            ,
    USIMM_CDMA_EFHRPDUPP_ID            ,
    USIMM_CDMA_EFCSSPR_ID              ,
    USIMM_CDMA_EFACT_ID                ,
    USIMM_CDMA_EFEPRL_ID               ,
    USIMM_CDMA_EFBCSMSOFG_ID           ,
    USIMM_CDMA_EFBCSMSPREF_ID          ,
    USIMM_CDMA_EFBCSMSTBL_ID           ,
    USIMM_CDMA_EFBCSMSP_ID             ,
    USIMM_CDMA_EFIMPI_ID               ,
    USIMM_CDMA_EFDOMAIN_ID             ,
    USIMM_CDMA_EFIMPU_ID               ,
    USIMM_CDMA_EFPCSCF_ID              ,
    USIMM_CDMA_EFBAKPARA_ID            ,
    USIMM_CDMA_EFUPBAKPARA_ID          ,
    USIMM_CDMA_EFMMSN_ID               ,
    USIMM_CDMA_EFEXT8_ID               ,
    USIMM_CDMA_EFMMSICP_ID             ,
    USIMM_CDMA_EFMMSUP_ID              ,
    USIMM_CDMA_EFMMSUCP_ID             ,
    USIMM_CDMA_EFAUTHCAP_ID            ,
    USIMM_CDMA_EF3GCIK_ID              ,
    USIMM_CDMA_EFDCK_ID                ,
    USIMM_CDMA_EFGID1_ID               ,
    USIMM_CDMA_EFGID2_ID               ,
    USIMM_CDMA_EFCDMACNL_ID            ,
    USIMM_CDMA_EFHOMETAG_ID            ,
    USIMM_CDMA_EFGROUPTAG_ID           ,
    USIMM_CDMA_EFSPECTAG_ID            ,
    USIMM_CDMA_EFCALLPROM_ID           ,
    USIMM_CDMA_EFSFEUIMID_ID           ,
    USIMM_CDMA_EFSMSCAP_ID             ,
    USIMM_CDMA_EFIPV6CAP_ID            ,
    USIMM_CDMA_EFMIPFLAG_ID            ,
    USIMM_CDMA_EFTCPCFG_ID             ,
    USIMM_CDMA_EFDGC_ID                ,
    USIMM_CDMA_EFWAPBRCP_ID            ,
    USIMM_CDMA_EFWAPBRBM_ID            ,
    USIMM_CDMA_EF3GPDUPPEX_ID          ,
    USIMM_CDMA_EFMMSCFG_ID             ,
    USIMM_CDMA_EFJDL_ID                ,
    USIMM_CDMA_EFMODEL_ID              ,
    USIMM_CDMA_EFRC_ID                 ,
    USIMM_CDMA_EFAPPLAB_ID             ,
    /* EF under ADFISIM */
    USIMM_ISIM_ID                      ,
    USIMM_ISIM_EFIST_ID                ,
    USIMM_ISIM_EFIMPI_ID               ,
    USIMM_ISIM_EFDOMAIN_ID             ,
    USIMM_ISIM_EFIMPU_ID               ,
    USIMM_ISIM_EFAD_ID                 ,
    USIMM_ISIM_EFARR_ID                ,
    USIMM_ISIM_EFPCSCF_ID              ,
    USIMM_ISIM_EFGBAP_ID               ,
    USIMM_ISIM_EFGBANL_ID              ,
    USIMM_ISIM_EFNAFKCA_ID             ,
    USIMM_ISIM_EFUICCIARI_ID           ,
    USIMM_ISIM_EFSMS_ID                ,
    USIMM_ISIM_EFSMSS_ID               ,
    USIMM_ISIM_EFSMSR_ID               ,
    USIMM_ISIM_EFSMSP_ID               ,
    USIMM_DEF_FILEID_BUTT
};
typedef VOS_UINT32      USIMM_DEF_FILEID_ENUM_UINT32;
typedef VOS_UINT16      USIMM_DEF_FILEID_ENUM_UINT16;

#define EFPLMNsel                               (0x6F30) /*暂时为NAS保留*/
#define EF_CUIM_PRL                             (0x6F30)   /*T*/

#define EF_CUIM_SSCI                            (0x6F2E)   /*T*/
#define EF_CUIM_TERM                            (0x6F2D)   /*T*/
#define EF_CUIM_SMS                             (0x6F3C)   /*T*/
#define EF_CUIM_EPRL                            (0x6F5A)   /*T*/

typedef VOS_UINT16      USIMM_CUIM_FILEID_ENUM_UINT16;

enum UICC_SERVICES_TYPE_ENUM
{
/* 00 */ UICC_SVR_NONE = 0,

/* 01 */ SIM_SVR_CHV1,                   /* CHV1 disable function                                           */
/* 02 */ SIM_SVR_ADN,                    /* Abbreviated Dialling Numbers (ADN)                              */
/* 03 */ SIM_SVR_FDN,                    /* Fixed Dialling Numbers (FDN)                                    */
/* 04 */ SIM_SVR_SMS,                    /* Short Message Storage (SMS)                                     */
/* 05 */ SIM_SVR_AOC,                    /* Advice of Charge (AoC)                                          */
/* 06 */ SIM_SVR_CCP,                    /* Capability Configuration Parameters (CCP)                       */
/* 07 */ SIM_SVR_PLMN_SELECTOR,          /* PLMN selector                                                   */
/* 08 */ SIM_SVR_RFU1,                   /* RFU                                                             */
/* 09 */ SIM_SVR_MSISDN,                 /* MSISDN                                                          */
/* 10 */ SIM_SVR_EXT1,                   /* Extension1                                                      */
/* 11 */ SIM_SVR_EXT2,                   /* Extension2                                                      */
/* 12 */ SIM_SVR_SMS_PARA,               /* SMS Parameters                                                  */
/* 13 */ SIM_SVR_LND,                    /* Last Number Dialled (LND)                                       */
/* 14 */ SIM_SVR_CBMID,                  /* Cell Broadcast Message Identifier                               */
/* 15 */ SIM_SVR_GIDL1,                  /* Group Identifier Level 1                                        */
/* 16 */ SIM_SVR_GIDL2,                  /* Group Identifier Level 2                                        */
/* 17 */ SIM_SVR_SERVICE_PRO_NAME,       /* Service Provider Name                                           */
/* 18 */ SIM_SVR_SDN,                    /* Service Dialling Numbers (SDN)                                  */
/* 19 */ SIM_SVR_EXT3,                   /* Extension3                                                      */
/* 20 */ SIM_SVR_RFU2,                   /* RFU                                                             */
/* 21 */ SIM_SVR_EFVGCS_EFVGCSS,         /* VGCS Group Identifier List (EFVGCS and EFVGCSS)                 */
/* 22 */ SIM_SVR_FVBS_EFVBSS,            /* VBS Group Identifier List (EFVBS and EFVBSS)                    */
/* 23 */ SIM_SVR_ENH_ML_SVC,             /* enhanced Multi Level Precedence and Pre emption Service         */
/* 24 */ SIM_SVR_AUTO_ANSWER_FOR_EMLPP,  /* Automatic Answer for eMLPP                                      */
/* 25 */ SIM_SVR_DD_VIA_SMS_CB,          /* Data download via SMS CB                                        */
/* 26 */ SIM_SVR_DD_VIA_SMS_PP,          /* Data download via SMS PP                                        */
/* 27 */ SIM_SVR_MENU_SELECTION,         /* Menu selection                                                  */
/* 28 */ SIM_SVR_CALL_CONTROL,           /* Call control                                                    */
/* 29 */ SIM_SVR_PRO_SIM,                /* Proactive SIM                                                   */
/* 30 */ SIM_SVR_CBMIR,                  /* Cell Broadcast Message Identifier Ranges                        */
/* 31 */ SIM_SVR_BDN,                    /* Barred Dialling Numbers (BDN)                                   */
/* 32 */ SIM_SVR_EXT4,                   /* Extension4                                                      */
/* 33 */ SIM_SVR_DPCK,                   /* De personalization Control Keys                                 */
/* 34 */ SIM_SVR_CONL,                   /* Co operative Network List                                       */
/* 35 */ SIM_SVR_SMS_REPORTS,            /* Short Message Status Reports                                    */
/* 36 */ SIM_SVR_NID,                    /* Network's indication of alerting in the MS                      */
/* 37 */ SIM_SVR_MOSMC,                  /* Mobile Originated Short Message control by SIM                  */
/* 38 */ SIM_SVR_GPRS,                   /* GPRS                                                            */
/* 39 */ SIM_SVR_IMG,                    /* Image (IMG)                                                     */
/* 40 */ SIM_SVR_SOLSA,                  /* SoLSA (Support of Local Service Area)                           */
/* 41 */ SIM_SVR_USSD,                   /* USSD string data object supported in Call Control               */
/* 42 */ SIM_SVR_RUN_AT_CMD,             /* RUN AT COMMAND command                                          */
/* 43 */ SIM_SVR_USER_CTRL_PLMN,         /* User controlled PLMN Selector with Access Technology            */
/* 44 */ SIM_SVR_OPERATOR_CTRL_PLMN,     /* Operator controlled PLMN Selector with Access Technology        */
/* 45 */ SIM_SVR_HPLMN,                  /* HPLMN Selector with Access Technology                           */
/* 46 */ SIM_SVR_CPBCCHI,                /* CPBCCH Information                                              */
/* 47 */ SIM_SVR_INV_SCAN,               /* Investigation Scan                                              */
/* 48 */ SIM_SVR_ECCP,                   /* Extended Capability Configuration Parameters                    */
/* 49 */ SIM_SVR_MEXE,                   /* MExE                                                            */
/* 50 */ SIM_SVR_RSV,                    /* Reserved and shall be ignored                                   */
/* 51 */ SIM_SVR_PLMNNN,                 /* PLMN Network Name                                               */
/* 52 */ SIM_SVR_OPLMNL,                 /* Operator PLMN List                                              */
/* 53 */ SIM_SVR_MDN,                    /* Mailbox Dialling Numbers                                        */
/* 54 */ SIM_SVR_MWIS,                   /* Message Waiting Indication Status                               */
/* 55 */ SIM_SVR_CFIS,                   /* Call Forwarding Indication Status                               */
/* 56 */ SIM_SVR_SPDI,                   /* Service Provider Display Information                            */
/* 57 */ SIM_SVR_MMS,                    /* Multimedia Messaging Service (MMS)                              */
/* 58 */ SIM_SVR_EXT8,                   /* Extension 8                                                     */
/* 59 */ SIM_SVR_MMS_UCP,                /* MMS User Connectivity Parameters                                */
         SIM_SVR_BUTT,

/* 60 */ USIM_SVR_LOCAL_PHONEBOOK = SIM_SVR_BUTT,       /* Local Phonebook, UADF/Phonebook/ADN  */
/* 61 */ USIM_SVR_FDN,                   /* Forbidden Dialling Numbers                                      */
/* 62 */ USIM_SVR_EXT2,                  /* Extension 2                                                     */
/* 63 */ USIM_SVR_SDN,                   /* Service Dialling Numbers                                        */
/* 64 */ USIM_SVR_EXT3,                  /* Extension 3                                                     */
/* 65 */ USIM_SVR_BDN,                   /* Barred Dialing Numbers                                          */
/* 66 */ USIM_SVR_EXT4,                  /* Extension 4                                                     */
/* 67 */ USIM_SVR_OCI_OCT,               /* Outgoing Call information/timer                                 */
/* 68 */ USIM_SVR_ICI_ICT,               /* Incoming call information/timer                                 */
/* 69 */ USIM_SVR_SMS,                   /* Short Message Services                                          */
/* 70 */ USIM_SVR_SMSR,                  /* Short Message Status Reports                                    */
/* 71 */ USIM_SVR_SMSP,                  /* SMS Parameters                                                  */
/* 72 */ USIM_SVR_AOC,                   /* Advice of Charge                                                */
/* 73 */ USIM_SVR_CCP,                   /* Configuration Capability Params                                 */
/* 74 */ USIM_SVR_CBMI,                  /* Cell Broadcast Message Identifier                               */
/* 75 */ USIM_SVR_CBMIR,                 /* Cell Broadcast Message Identifiers Range                        */
/* 76 */ USIM_SVR_GID1,                  /* Group Identifier 1                                              */
/* 77 */ USIM_SVR_GID2,                  /* Group Identifier 2                                              */
/* 78 */ USIM_SVR_SPN,                   /* Service Provider Name                                           */
/* 79 */ USIM_SVR_UPLMN_SEL_WACT,        /* User controlled PLMN Selector with Access Technology            */
/* 80 */ USIM_SVR_MSISDN,                /* MSISDN                                                          */
/* 81 */ USIM_SVR_IMAGE,                 /* Image (IMG)                                                     */
/* 82 */ USIM_SVR_SOLSA,                 /* Support of Local Service Area                                   */
/* 83 */ USIM_SVR_ENH_ML_SVC,            /* Enhanced Multi Level precedence and Premption Service           */
/* 84 */ USIM_SVR_AA_EMLPP,              /* Automatic Answer for eMLPP                                      */
/* 85 */ USIM_SVR_RFU1,                  /* RFU                                                             */
/* 86 */ USIM_SVR_GSM_ACCESS_IN_USIM,    /* USIM ADF Access to USIM files for GSM Roaming                   */
/* 87 */ USIM_SVR_DATA_DL_SMSPP,         /* Data download via SMS-PP                                        */
/* 88 */ USIM_SVR_DATA_DL_SMSCB,         /* Data download via SMS-CB                                        */
/* 89 */ USIM_SVR_CALL_CONTROL,          /* Call Control                                                    */
/* 90 */ USIM_SVR_MOSMS_CONTROL,         /* MO SMS control                                                  */
/* 91 */ USIM_SVR_RUN_AT_CMD,            /* RUN AT COMMAND command                                          */
/* 92 */ USIM_SVR_SET_ONE,               /* shall be set to1                                                */
/* 93 */ USIM_SVR_EST,                   /* Enable Service Table in USIM                                    */
/* 94 */ USIM_SVR_ACL,                   /* APN Control list                                                */
/* 95 */ USIM_SVR_DEPERSON_KEYS,         /* Depersonalization Keys                                          */
/* 96 */ USIM_SVR_COOP_NETWK_LIST,       /* Co-operative Network List                                       */
/* 97 */ USIM_SVR_GSM_SECURITY_CONTEXT,  /* GSM Security Context                                            */
/* 98 */ USIM_SVR_CPBCCH,                /* CP BCCH                                                         */
/* 99 */  USIM_SVR_INV_SCAN,              /* Investigation Scan                                              */
/* 100 */ USIM_SVR_MEXE,                  /* MExE info                                                       */
/* 101 */ USIM_SVR_OPLMN_SEL_WACT,        /* Operator controlled PLMN Selector with Access Technology        */
/* 102 */ USIM_SVR_HPLMN_WACT,            /* HPLMN Selector with Access Technology                           */
/* 103 */ USIM_SVR_EXT5,                  /* Extension 5                                                     */
/* 104 */ USIM_SVR_PLMN_NTWRK_NAME,       /* PLMN Network Name                                               */
/* 105 */ USIM_SVR_OPLMN_LIST,            /* Operator PLMN list                                              */
/* 106 */ USIM_SVR_MAILBOX_DIAL,          /* Mailbox dialling numbers                                        */
/* 107 */ USIM_SVR_MSG_WAIT,              /* Message Wait indication                                         */
/* 108 */ USIM_SVR_CALL_FWD_IND,          /* Call Forward indication status                                  */
/* 109 */ USIM_SVR_RFU2,                  /* RFU                                                             */
/* 110 */ USIM_SVR_SP_DISP_INFO,          /* Service Provider Display Information                            */
/* 111 */ USIM_SVR_MMS,                   /* Multimedia Messaging Service                                    */
/* 112 */ USIM_SVR_EXT8,                  /* Extension 8                                                     */
/* 113 */ USIM_SVR_GPRS_USIM,             /* Call control on GPRS by USIM                                    */
/* 114 */ USIM_SVR_MMS_USR_P,             /* MMS User Connectivity Parameters                                */
/* 115 */ USIM_SVR_NIA,                   /* Network's indication of alerting in the MS                      */
/* 116 */ USIM_SVR_VGCS_GID_LIST,         /* VGCS Group Identifier List                                      */
/* 117 */ USIM_SVR_VBS_GID_LIST,          /* VBS Group Identifier List                                       */
/* 118 */ USIM_SVR_PSEUDONYM,             /* Pseudonmy                                                       */
/* 119 */ USIM_SVR_UPLMN_WLAN_ACC,        /* User Controlled PLMNselector for WLAN access                    */
/* 120 */ USIM_SVR_OPLMN_WLAN_ACC,        /* Opeator Controlled PLMN selector for WLAN access                */
/* 121 */ USIM_SVR_USER_CTRL_WSID,        /* User controlled WSID list                                       */
/* 122 */ USIM_SVR_OPER_CTRL_WSID,        /* Operator controlled WSID list                                   */
/* 123 */ USIM_SVR_VGCS_SECURITY,         /* VGCS Security                                                   */
/* 124 */ USIM_SVR_VBS_SECURITY,          /* VBS Security                                                    */
/* 125 */ USIM_SVR_WLAN_REAUTH_ID,        /* WLAN Reauthentication Identity                                  */
/* 126 */ USIM_SVR_MM_STORAGE,            /* Multimedia Messaging Storage                                    */
/* 127 */ USIM_SVR_GBA,                   /* Generic Bootstrapping Architecture                              */
/* 128 */ USIM_SVR_MBMS_SECURITY,         /* MBMS security                                                   */
/* 129 */ USIM_SVR_DATA_DL_USSD,          /* Data Downlod via USSD and USSD application mode                 */
/* 130 */ USIM_SVR_EHPLMN,                /* EHPLMN                                                          */
/* 131 */ USIM_SVR_TP_AFTER_UICC_ACT,     /* additional TERMINAL PROFILE after UICC activation               */
/* 132 */ USIM_SVR_EHPLMN_IND,            /* Equivalent HPLMN Presentation Indication                        */
/* 133 */ USIM_SVR_RPLMN_LACT,            /* RPLMN Last used Access Technology                               */
/* 134 */ USIM_SVR_OMA_BCAST_PROFILE,     /* OMA BCAST Smart Card Profile                                     */
/* 135 */ USIM_SVR_GBAKEY_EM,             /* GBA-based Local Key Establishment Mechanism                      */
/* 136 */ USIM_SVR_TERMINAL_APP,          /* Terminal Applications                                            */
/* 137 */ USIM_SVR_SPN_ICON,              /* Service Provider Name Icon                                       */
/* 138 */ USIM_SVR_PNN_ICON,              /* PLMN Network Name Icon                                           */
/* 139 */ USIM_SVR_CONNECT_SIM_IP,        /* Connectivity Parameters for USIM IP connections                  */
/* 140 */ USIM_SVR_IWLEN_ID_LIST,         /* Home I-WLAN Specific Identifier List                             */
/* 141 */ USIM_SVR_IWLEN_EHPLMN_IND,      /* I-WLAN Equivalent HPLMN Presentation Indication                  */
/* 142 */ USIM_SVR_IWLEN_HPLMN_IND,       /* I-WLAN HPLMN Priority Indication                                 */
/* 143 */ USIM_SVR_IWLEN_RPLMN,           /* I-WLAN Last Registered PLMN                                      */
/* 144 */ USIM_SVR_EPS_INFO,              /* EPS Mobility Management Information                              */
/* 145 */ USIM_SVR_CSG_COR_IND,           /* Allowed CSG Lists and corresponding indications                  */
/* 146 */ USIM_SVR_CALL_CTRL_EPS,         /* Call control on EPS PDN connection by USIM                       */
/* 147 */ USIM_SVR_HPLMN_DIR_ACCESS,      /* HPLMN Direct Access                                              */
/* 148 */ USIM_SVR_ECALL_DATA,            /* eCall Data                                                       */
/* 149 */ USIM_SVR_OP_CGS,                /* Operator CSG Lists and corresponding indications                 */
/* 150 */ USIM_SVR_SM_VOER_IP,            /* Support for SM-over-IP                                           */
/* 151 */ USIM_SVR_CSG_CTRL,              /* Support of CSG Display Control                                   */
/* 152 */ USIM_SVR_CTRL_IMS,              /* Communication Control for IMS by USIM                            */
/* 153 */ USIM_SVR_APP_EX,                /* Extended Terminal Applications                                   */
/* 154 */ USIM_SVR_ACCESS_IMS,            /* Support of UICC access to IMS                                    */
/* 155 */ USIM_SVR_NO_ACCESS,             /* Non-Access Stratum configuration by USIM                         */
/* 156 */ USIM_SVR_PWS_CONFIG,            /* PWS configuration by USIM                                        */
/* 157 */ USIM_SVR_RFU,                   /* RFU                                                              */
/* 158 */ USIM_SVR_URI_UICC,              /* URI support by UICC                                              */
/* 159 */ USIM_SVR_EXTENDED_EARFCN,       /* Extended EARFCN support                                          */
/* 160 */ USIM_SVR_PROSE,                 /* ProSe                                                            */
/* 161 */ USIM_SVR_USAT,                  /* USAT Application Pairing                                         */
/* 162 */ USIM_SVR_MEDIA_TYPE,            /* Media Type support                                               */
/* 163 */ USIM_SVR_IMS_CALL,              /* IMS call disconnection cause                                     */
/* 164 */ USIM_SVR_URI_MO_SHORT_MESSAGE,  /* URI support for MO SHORT MESSAGE CONTROL                         */
/* 165 */ USIM_SVR_EPDG,                  /* ePDG configuration information support                           */
/* 166 */ USIM_SVR_EPDG_CONFIGURED,       /* ePDG configuration information configured                        */
/* 167 */ USIM_SVR_ACDC,                  /* ACDC support                                                     */
/* 168 */ USIM_SVR_MCPTT,                 /* MCPTT                                                            */
/* 169 */ USIM_SVR_EPDG_EMRG,             /* ePDG configuration information for Emergency Service support     */
/* 170 */ USIM_SVR_EPDG_EMRG_CONFIGURED,  /* ePDG configuration information for Emergency Service configured  */
/* 171 */ USIM_SVR_ECALL_DATA_OVER_IMS,   /* eCall Data over IMS                                              */
          USIM_SVR_BUTT,

/* 172 */ ISIM_SVR_PCSCF_ADDR = USIM_SVR_BUTT,            /* P-CSCF address */
/* 173 */ ISIM_SVR_GBA,                   /* Generic Bootstrapping Architecture (GBA)                         */
/* 174 */ ISIM_SVR_HTTP_DIGEST,           /* HTTP Digest                                                      */
/* 175 */ ISIM_SVR_LOCAL_GBA,             /* GBA-based Local Key Establishment Mechanism                      */
/* 176 */ ISIM_SVR_IMS_PCSCF,             /* Support of P-CSCF discovery for IMS Local Break Out              */
/* 177 */ ISIM_SVR_SMS,                   /* Short Message Storage (SMS)                                      */
/* 178 */ ISIM_SVR_SMSR,                  /* Short Message Status Reports (SMSR)                              */
/* 179 */ ISIM_SVR_SOI,                   /* Support for SM-over-IP including data download via SMS-PP as defined in TS 31.111 [31] */
/* 180 */ ISIM_SVR_COMM_CTRL,             /* Communication Control for IMS by ISIM                            */
/* 181 */ ISIM_SVR_UICC_IMS,              /* Support of UICC access to IMS                                    */
          ISIM_SVR_BUTT,

/* 182 */ UIM_SVR_CHV_DISABLE = ISIM_SVR_BUTT,  /* CHV Disable fuction */
/* 183 */ UIM_SVR_ADN,                   /* Abbreviated Dialing Numbers */
/* 184 */ UIM_SVR_FDN,                   /* Fix Dialing Numbers */
/* 185 */ UIM_SVR_SMS,                   /* Short Message Stroage */
/* 186 */ UIM_SVR_HRPD,                  /* HRPD */
/* 187 */ UIM_SVR_EPB,                   /* Enhanced Phone Book */
/* 188 */ UIM_SVR_MMD,                   /* Multi Media Domain */
/* 189 */ UIM_SVR_SF_EUIMID,             /* SF_EUIMID-based EUIMID */
/* 190 */ UIM_SVR_MEID,                  /* MEID Support */
/* 191 */ UIM_SVR_EXT1,                  /* Extension1 */
/* 192 */ UIM_SVR_EXT2,                  /* Extension2 */
/* 193 */ UIM_SVR_SMSP,                  /* SMS Parameters */
/* 194 */ UIM_SVR_LND,                   /* Last Number Dialed */
/* 195 */ UIM_SVR_SCP_BC_SMS,            /* Service Category Program for BC-SMS */
/* 196 */ UIM_SVR_3GPD_EXT,              /* Messaging and 3GPD Extensions */
/* 197 */ UIM_SVR_ROOT_CA,               /* Root Certificates */
/* 198 */ UIM_SVR_HOME_SPN,              /* CDMA Home Service Provider Name */
/* 199 */ UIM_SVR_SDN,                   /* Service Dialing Numbers */
/* 200 */ UIM_SVR_EXT3,                  /* Extension 3 */
/* 201 */ UIM_SVR_3GPD_SIP,              /* 3GPD SIP */
/* 202 */ UIM_SVR_WAP,                   /* WAP Browser */
/* 203 */ UIM_SVR_JAVA,                  /* Java */
/* 204 */ UIM_SVR_RSV_CDG1,              /* Reserved for CDG */
/* 205 */ UIM_SVR_RSV_CDG2,              /* Reserved for CDG */
/* 206 */ UIM_SVR_CBS_DOWNLOAD,          /* Data Download via SMS Broadcast */
/* 207 */ UIM_SVR_SMS_PP_DOWNOLAD,       /* Data Download via SMS-PP */
/* 208 */ UIM_SVR_MENU_SELECTION,        /* Menu Selection  */
/* 209 */ UIM_SVR_CALL_CONTROL,          /* Call Control */
/* 210 */ UIM_SVR_PROACTIVE_UIM,         /* Proactive R-UIM */
/* 211 */ UIM_SVR_AKA,                   /* AKA */
/* 212 */ UIM_SVR_IPV6,                  /* IPV6 */
/* 213 */ UIM_SVR_RFU1,                  /* RFU */
/* 214 */ UIM_SVR_RFU2,                  /* RFU */
/* 215 */ UIM_SVR_RFU3,                  /* RFU */
/* 216 */ UIM_SVR_RFU4,                  /* RFU */
/* 217 */ UIM_SVR_RFU5,                  /* RFU */
/* 218 */ UIM_SVR_RFU6,                  /* RFU */
/* 219 */ UIM_SVR_3GPD_MIP,              /* 3GPD MIP */
/* 220 */ UIM_SVR_BCMCS,                 /* BCMCS */
/* 221 */ UIM_SVR_MMS,                   /* Multimedia Messaging Service */
/* 222 */ UIM_SVR_EXT8,                  /* Extension 8 */
/* 223 */ UIM_SVR_MMSCP,                 /* MMS User Connectivity Parameters */
/* 224 */ UIM_SVR_APP_AUTH,              /* Application Authentication */
/* 225 */ UIM_SVR_GID1,                  /* Group Identifier Level 1 */
/* 226 */ UIM_SVR_GID2,                  /* Group Identifier Level 2 */
/* 227 */ UIM_SVR_CPCK,                  /* De-Personalization Control Keys */
/* 228 */ UIM_SVR_CNL,                   /* Cooperative Network List */
          UIM_SVR_BUTT,

/* 229 */ CSIM_SVR_LOCAL_PB = UIM_SVR_BUTT,  /* Local Phone Book */
/* 230 */ CSIM_SVR_FDN,                  /* Fixed Dialing Numbers */
/* 231 */ CSIM_SVR_EXT2,                 /* Extension 2 */
/* 232 */ CSIM_SVR_SDN,                  /* Service Dialing Numbers */
/* 233 */ CSIM_SVR_EXT3,                 /* Extension 2 */
/* 234 */ CSIM_SVR_SMS,                  /* Short Message Storage */
/* 235 */ CSIM_SVR_SMSP,                 /* Short Message Parameters */
/* 236 */ CSIM_SVR_HRPD,                 /* HRPD */
/* 237 */ CSIM_SVR_SCP_BCSMS,            /* Service Category Program for BC-SMS */
/* 238 */ CSIM_SVR_HOME_SPN,             /* CDMA Home Service Provider Name */
/* 239 */ CSIM_SVR_CBS_DOWNLOAD,         /* Data Download via SMS Broadcast */
/* 240 */ CSIM_SVR_SMSPP_DOWNLOAD,       /* Data Download via SMS PP */
/* 241 */ CSIM_SVR_CALL_CONTROL,         /* Call Control */
/* 242 */ CSIM_SVR_3GPD_SIP,             /* 3GPD_SIP */
/* 243 */ CSIM_SVR_3GPD_MIP,             /* 3GPD_MIP */
/* 244 */ CSIM_SVR_AKA,                  /* AKA */
/* 245 */ CSIM_SVR_IP_LCS,               /* IP based Location Services */
/* 246 */ CSIM_SVR_BCMCS,                /* BCMCS */
/* 247 */ CSIM_SVR_MMS,                  /* Multimedia Messaging Service */
/* 248 */ CSIM_SVR_EXT8,                 /* Extension 8 */
/* 249 */ CSIM_SVR_MMS_UCP,              /* MMS User Connectivity Parameters */
/* 250 */ CSIM_SVR_APP_AUTH,             /* Application Authentication */
/* 251 */ CSIM_SVR_GID1,                 /* Group Identifier Level 1 */
/* 252 */ CSIM_SVR_GID2,                 /* Group Identifier Level 2 */
/* 253 */ CSIM_SVR_DPCK,                 /* De-Personalization Control Keys */
/* 254 */ CSIM_SVR_CNL,                  /* Cooperative Network List */
/* 255 */ CSIM_SVR_OCI,                  /* Outgoing Call Information */
/* 256 */ CSIM_SVR_ICI,                  /* Incoming Call Information */
/* 257 */ CSIM_SVR_EXT5,                 /* Extension 5 */
/* 258 */ CSIM_SVR_MS,                   /* Multimedia Stroage */
/* 259 */ CSIM_SVR_IMG,                  /* Image EFIMG */
/* 260 */ CSIM_SVR_EST,                  /* Enabled Services Table */
/* 261 */ CSIM_SVR_CCP,                  /* Capability Configuration Parameters */
/* 262 */ CSIM_SVR_SF_EUIMID,            /* SF_EUIMID-based EUIMID */
/* 263 */ CSIM_SVR_3GPD_EXT,             /* Messaging and 3GPD Extensions */
/* 264 */ CSIM_SVR_ROOT_CA,              /* Root Certificates */
/* 265 */ CSIM_SVR_WAP_BROWSER,          /* WAP Browser */
/* 266 */ CSIM_SVR_JAVA,                 /* Java */
/* 267 */ CSIM_SVR_RSV_CDG1,             /* Reserved for CDG */
/* 268 */ CSIM_SVR_RSV_CDG2,             /* Reserved for CDG */
/* 269 */ CSIM_SVR_IPV6,                 /* IPV6 */
          CSIM_SVR_BUTT

};

enum USIMM_PB_INIT_STATUS_ENUM
{
    USIMM_PB_IDLE           = 0,
    USIMM_PB_INITIALING     = 1
};
typedef VOS_UINT16      USIMM_PB_INIT_STATUS_ENUM_UINT16;

typedef VOS_UINT32      UICC_SERVICES_TYPE_ENUM_UINT32;

enum USIMM_RESTRIC_CMD_ENUM
{
    USIMM_AUTHENTICATION                = 136,
    USIMM_READ_BINARY                   = 176,
    USIMM_READ_RECORD                   = 178,
    USIMM_GET_RESPONSE                  = 192,
    USIMM_UPDATE_BINARY                 = 214,
    USIMM_UPDATE_RECORD                 = 220,
    USIMM_STATUS                        = 242,
    USIMM_CMD_BUTT
};
typedef VOS_UINT32      USIMM_RESTRIC_CMD_ENUM_UINT32;

enum USIMM_POLL_FCP_ENUM
{
    USIMM_POLL_NO_NEED_FCP              = 0,
    USIMM_POLL_NEED_FCP                 = 1,
    USIMM_POLL_FCP_BUTT
};
typedef VOS_UINT32      USIMM_POLL_FCP_ENUM_UINT32;

enum USIMM_FBDN_HANDLE_ENUM
{
    USIMM_FDN_DEACTIVE              = 0,
    USIMM_FDN_ACTIVE                = 1,
    USIMM_BDN_DEACTIVE              = 2,
    USIMM_BDN_ACTIVE                = 3,
    USIMM_FBDN_BUTT
};
typedef VOS_UINT32 USIMM_FBDN_HANDLE_ENUM_UINT32;

enum USIMM_FDNSTATUS_ENUM
{
    USIMM_FDNSTATUS_OFF                 = USIMM_FDN_DEACTIVE,
    USIMM_FDNSTATUS_ON                  = USIMM_FDN_ACTIVE,
    USIMM_FDNSTATUS_BUTT
};
typedef VOS_UINT32 USIMM_FDNSTATUS_ENUM_UINT32;

enum USIMM_BDNSTATUS_ENUM
{
    USIMM_BDNSTATUS_OFF                 = 0,
    USIMM_BDNSTATUS_ON                  = 1,
    USIMM_BDNSTATUS_BUTT
};
typedef VOS_UINT32      USIMM_BDNSTATUS_ENUM_UINT32;

/*PIN码操作失败结果*/
enum USIMM_PIN_ERROR_ENUM
{
    USIMM_SIM_PIN_REQUIRED              = 151,
    USIMM_SIM_PUK_REQUIRED              = 152,
    USIMM_SIM_FAILURE                   = 153,
    USIMM_SIM_BUSY                      = 154,
    USIMM_SIM_WRONG                     = 155,
    USIMM_INCORRECT_PASSWORD            = 156,
    USIMM_SIM_PIN2_REQUIRED             = 157,
    USIMM_SIM_PUK2_REQUIRED             = 158,
    USIMM_OPERATION_NOT_ALLOW           = 167,
    USIMM_PIN_ERROR_BUTT
};
typedef VOS_UINT32 USIMM_PIN_ERROR_ENUM_UINT32;

enum USIMM_AUTH_TYPE_ENUM
{
    USIMM_3G_AUTH                       = 0x00000000,
    USIMM_2G_AUTH                       = 0x00000001,
    USIMM_IMS_AUTH                      = 0x00000002,
    USIMM_HTTP_AUTH                     = 0x00000003,
    USIMM_GBA_AUTH                      = 0x00000004,
    USIMM_NAF_AUTH                      = 0x00000005,
    USIMM_RUNCAVE_AUTH                  = 0x00000006,
    USIMM_EAP_AUTH                      = 0x00000007,
    USIMM_WLAM_AUTH                     = 0x00000008,
    USIMM_AUTH_BUTT
};
typedef VOS_UINT32 USIMM_AUTH_TYPE_ENUM_UINT32;

/* Api_UsimAuthCnf ucResult宏定义 */
enum USIMM_AUTH_RESULT_ENUM
{
    USIMM_AUTH_UMTS_SUCCESS         = 0x00000000,
    USIMM_AUTH_GSM_SUCCESS          = 0x00000001,
    USIMM_AUTH_IMS_SUCCESS          = 0x00000002,        /*鉴权成功*/
    USIMM_AUTH_CDMA_SUCCESS         = 0x00000003,
    USIMM_AUTH_MAC_FAILURE          = 0x00000004,        /*MAC失败*/
    USIMM_AUTH_SYNC_FAILURE         = 0x00000005,        /*重同步失败*/
    USIMM_AUTH_UMTS_OTHER_FAILURE   = 0x00000006,
    USIMM_AUTH_GSM_OTHER_FAILURE    = 0x00000007,
    USIMM_AUTH_IMS_OTHER_FAILURE    = 0x00000008,        /*其他失败*/
    USIMM_AUTH_CDMA_OTHER_FAILURE   = 0x00000009,
    USIMM_AUTH_RESULT_BUTT
};
typedef VOS_UINT32 USIMM_AUTH_RESULT_ENUM_UINT32;

enum USIMM_REFRESH_FILE_TYPE
{
    USIMM_REFRESH_ALL_FILE,
    USIMM_REFRESH_FILE_LIST,
    USIMM_REFRESH_3G_SESSION_RESET,
    USIMM_REFRESH_FILE_BUTT
};
typedef VOS_UINT16 USIMM_REFRESH_FILE_TYPE_UINT16;

enum USIMM_EFRWFLAG_ENUM
{
    USIMM_EF_RW_NOT     = 0,
    USIMM_EF_RW_OK      = 1,
    USIMM_EF_BUTT
};
typedef VOS_UINT8      USIMM_EFRWFLAG_ENUM_UINT8;

enum USIMM_EFSTATUS_ENUM
{
    USIMM_EFSTATUS_DEACTIVE     = 0,
    USIMM_EFSTATUS_ACTIVE       = 1,
    USIMM_EFSTATUS_BUTT
};
typedef VOS_UINT8      USIMM_EFSTATUS_ENUM_UINT8;


enum USIMM_AID_TYPE_ENUM
{
    USIMM_AID_TYPE_USIM                 = USIMM_GUTL_APP,    /* USIM的AID */
    USIMM_AID_TYPE_ISIM                 = USIMM_IMS_APP,     /* ISIM的AID */
    USIMM_AID_TYPE_CSIM                 = USIMM_CDMA_APP,    /* CSIM的AID */
    USIMM_AID_TYPE_BUTT
};
typedef VOS_UINT32  USIMM_AID_TYPE_ENUM_UINT32;


enum USIMM_TR_PARA_ENUM
{
    USIMM_TR_TYPE_BALONG    = 0,
    USIMM_TR_TYPE_CBP       = 0x5A5A5A5A,
    USIMM_TR_TYPE_BUTT
};
typedef VOS_UINT32  USIMM_TR_PARA_ENUM_32;

enum USIMM_RUNCAVE_RANDTYPE_ENUM
{
    USIMM_RUNCAVE_RAND          = 0x00,
    USIMM_RUNCAVE_RANDU         = 0x01,
    USIMM_RUNCAVE_RANDBUTT      = 0x02,
    USIMM_RUNCAVE_RANDTYPE_BUTT
};
typedef VOS_UINT8 USIMM_RUNCAVE_RANDTYPE_ENUM_UINT8;

enum USIMM_MANAGESSD_TYPE_ENUM
{
    USIMM_MANAGESSD_UPDATE      = 0x00000000,
    USIMM_MANAGESSD_CONFIRM     = 0x00000001,
    USIMM_MANAGESSD_BUTT
};
typedef VOS_UINT32 USIMM_MANAGESSD_TYPE_ENUM_UINT32;

/*****************************************************************************
 结构名    : USIMM_SENDPARA_ENUM
 结构说明  : 临时定义区分不同的TPDU执行来源
*****************************************************************************/
enum USIMM_SENDPARA_ENUM
{
    USIMM_CSIM_SENDPARA             = 1,
    USIMM_CGLA_SENDPARA             = 2,
    USIMM_ISDB_SENDPARA             = 3,
    USIMM_CL_SENDPARA               = 4,
    USIMM_SENDPARA_BUTT
};
typedef VOS_UINT32  USIMM_SENDPARA_ENUM_UINT32;

/*******************************************************************************
 * Name        : USIMM_COMPUTEIP_AUTH_ENUM_UINT32
 * Description : This Enum defines the type of Computer IP Authentication
 * command, that included by CDMA special authentication command set.  It maps
 * the P1 parameter of this command.
 * See section 4.8.1 in <Invalid document reference (C.S0023 D_v2.0)> and
 * section 9.4.4.1 in 3GPP2 C.S0065 B v2.0: "cdma2000 Application on UICC for
 * Spread Spectrum Systems".
 *******************************************************************************/

enum USIMM_COMPUTEIP_AUTH_ENUM
{
    USIMM_COMPUTEIP_CHAP    = 0x00000000,
    USIMM_COMPUTEIP_MNHA    = 0x00000001,
    USIMM_COMPUTEIP_MIP     = 0x00000002,
    USIMM_COMPUTEIP_MNAAA   = 0x00000003,
    USIMM_COMPUTEIP_HRPD    = 0x00000004,
    USIMM_COMPUTEIP_BUTT
};
typedef VOS_UINT32 USIMM_COMPUTEIP_AUTH_ENUM_UINT32;

/*******************************************************************************
 * Name        : USIMM_SPECAUTH_BLOCK_ENUM_UINT8
 * Description : This Enum defines the type of parameter, of MN HA
 * Authenticator command, that is one of CDMA special authentication commands.
 * It maps P2 parameter of this command.
 * See section 4.8.1.2 in <Invalid document reference (C.S0023 D_v2.0)>.
 *******************************************************************************/

enum USIMM_SPECAUTH_BLOCK_ENUM
{
    USIMM_SPECAUTH_BLOCK_FIRST  = 0x00,
    USIMM_SPECAUTH_BLOCK_NEXT   = 0x01,
    USIMM_SPECAUTH_BLOCK_SINGLE = 0x02,
    USIMM_SPECAUTH_BLOCK_LAST   = 0x03,
    USIMM_SPECAUTH_BLOCK_BUTT
};
typedef VOS_UINT8 USIMM_SPECAUTH_BLOCK_ENUM_UINT8;

/*******************************************************************************
 * Name        : USIMM_BCMCS_AUTH_ENUM_UINT32
 * Description : This Enum defines the type of BCMSC command parameter. It
 * maps the BCMCS command P1 parameter.
 * See section 4.9 in <Invalid document reference (C.S0023 D_v2.0)> and
 * section 9.4.5 in 3GPP2 C.S0065 B v2.0: "cdma2000 Application on UICC for
 * Spread Spectrum Systems".
 *******************************************************************************/

enum USIMM_BCMCS_AUTH_ENUM
{
    USIMM_BCMCS_RETRIEVESK      = 0x00000000,
    USIMM_BCMCS_UPDATABAK       = 0x00000001,
    USIMM_BCMCS_DELETEBAK       = 0x00000002,
    USIMM_BCMCS_RETRIEVE_SRTPSK = 0x00000003,
    USIMM_BCMCS_GENAUTH_SIGN    = 0x00000004,
    USIMM_BCMCS_AUTHENTICATION  = 0x00000005,
    USIMM_BCMCS_BUTT
};
typedef VOS_UINT32 USIMM_BCMCS_AUTH_ENUM_UINT32;

/*******************************************************************************
 * Name        : USIMM_CDMAOTA_AUTH_ENUM_UINT32
 * Description : This Enum defines the type of Application Authentication,
 * that belong to CDMA special authentication command set. It maps the
 * Application Authentication command data's first byte, that is
 * authentication mechanism directive.
 * See section 4.10.1 Table 10 in <Invalid document reference
 * (C.S0023 D_v2.0)>.
 *******************************************************************************/
enum USIMM_CDMAAPP_AUTH_ENUM
{
    USIMM_CDMAAPP_CRAMMD5       = 0x00000000,
    USIMM_CDMAAPP_HTTPMD5       = 0x00000001,
    USIMM_CDMAAPP_HTTPMD5SESS   = 0x00000002,
    USIMM_CDMAAPP_HTTPV1MD5     = 0x00000003,
    USIMM_CDMAAPP_HTTPV1MD5SESS = 0x00000004,
    USIMM_CDMAAPP_SASL          = 0x00000005,
    USIMM_CDMAAPP_SASLOTP       = 0x00000006,
    USIMM_CDMAAPP_SASLGSS       = 0x00000007,
    USIMM_CDMAAPP_BUTT
};
typedef VOS_UINT32 USIMM_CDMAAPP_AUTH_ENUM_UINT32;

/*******************************************************************************
 * Name        : USIMM_CDMASPEC_AUTHTYPE_ENUM_UINT32
 * Description : This Enum defines the type of command, that process CDMA
 * special authentication request. There are several command component this
 * command set. So that this request need this input parameter to index which
 * command could be used.
 *******************************************************************************/
enum USIMM_CDMASPEC_AUTHTYPE_ENUM
{
    USIMM_CDMASPEC_COMPUTEIP    = 0x00000000,
    USIMM_CDMASPEC_BCMCS        = 0x00000001,
    USIMM_CDMASPEC_APPAUTH      = 0x00000002,
    USIMM_CDMASPEC_BUTT
};
typedef VOS_UINT32 USIMM_CDMASPEC_AUTHTYPE_ENUM_UINT32;

enum USIMM_VSIM_STATE_ENUM
{
    USIMM_VSIM_STATE_OFF        = 0x0,          /*VSIM关闭*/
    USIMM_VSIM_STATE_ON         = 0x0000A5A5,   /*VSIM打开*/
    USIMM_VSIM_STATE_BUTT
};
typedef VOS_UINT32 USIMM_VSIM_STATE_ENUM_UINT32;

enum USIMM_REALISIM_STATE_ENUM
{
    USIMM_REALISIM_STATE_OFF     = 0x0,         /*USIM模拟ISIM*/
    USIMM_REALISIM_STATE_ON      = 0x00005A5A,  /*真实ISIM*/
    USIMM_REALISIM_STATE_BUTT
};
typedef VOS_UINT32 USIMM_REALISIM_STATE_ENUM_UINT32;

/*****************************************************************************
 枚举名    : USIMM_STATUS_IND_ENUM
 枚举说明  : 卡状态通知类型枚举
 *****************************************************************************/
enum USIMM_CARD_STATUS_IND_ENUM
{
    USIMM_CARD_STATUS_INIT_IND               = 0,
    USIMM_CARD_STATUS_NV_REFRESH_IND         = 1,
    USIMM_CARD_STATUS_SWITCH_CARD_IND        = 2,
    USIMM_CARD_STATUS_IND_BUTT
};
typedef VOS_UINT8  USIMM_CARD_STATUS_IND_ENUM_UINT8;

enum USIMM_CARD_TYPE_ENUM
{
    USIMM_CARD_SIM          = 0x00,
    USIMM_CARD_USIM         = 0x01,
    USIMM_CARD_ISIM         = 0x02,
    USIMM_CARD_UIM          = 0x03,
    USIMM_CARD_CSIM         = 0x04,
    USIMM_CARD_ROM_SIM      = 0xF0,
    USIMM_CARD_UNKNOWN      = 0xF1,
    USIMM_CARD_NOCARD       = 0xFF,
    USIMM_CARD_TYPE_BUTT
};
typedef VOS_UINT32 USIMM_CARD_TYPE_ENUM_UINT32;

/*****************************************************************************
 枚举名    : USIMM_CARDAPP_SERVIC_ENUM
 枚举说明  : 服务类型枚举
*****************************************************************************/
enum USIMM_CARDAPP_SERVIC_ENUM
{
    USIMM_CARDAPP_SERVIC_ABSENT            = 0x00000000,
    USIMM_CARDAPP_SERVIC_UNAVAILABLE       = 0x00000001,
    USIMM_CARDAPP_SERVIC_SIM_PIN           = 0x00000002,
    USIMM_CARDAPP_SERVIC_SIM_PUK           = 0x00000003,
    USIMM_CARDAPP_SERVIC_NET_LCOK          = 0x00000004,
    USIMM_CARDAPP_SERVICE_SUBNET_LOCK      = 0x00000005,
    USIMM_CARDAPP_SERVICE_GID1_LOCK        = 0x00000006,
    USIMM_CARDAPP_SERVICE_GID2_LOCK        = 0x00000007,
    USIMM_CARDAPP_SERVICE_IMSI_LOCK        = 0x00000008,
    USIMM_CARDAPP_SERVIC_AVAILABLE         = 0x00000009,
    USIMM_CARDAPP_SERVIC_BUTT
};
typedef VOS_UINT32 USIMM_CARDAPP_SERVIC_ENUM_UINT32;

/*****************************************************************************
 枚举名    : USIMM_CARD_MEDIUM_TYPE_ENUM_UINT32
 枚举说明  : 卡类型枚举
*****************************************************************************/
enum USIMM_CARD_MEDIUM_TYPE_ENUM
{
    USIMM_CARD_MEDIUM_REAL              = 0x0,     /* 实卡 */
    USIMM_CARD_MEDIUM_VSIM,                        /* 虚拟卡 */
    USIMM_CARD_MEDIUM_ROM,                         /* 快速开机模式 */
#if (FEATURE_ESIM_ADAPT == FEATURE_ON)
    USIMM_CARD_MEDIUM_ESIM,                        /* eSIM卡 */
#endif
    USIMM_CARD_MEDIUM_BUTT,
};
typedef VOS_UINT32 USIMM_CARD_MEDIUM_TYPE_ENUM_UINT32;

/*****************************************************************************
 枚举名    : USIMM_CARD_FETCH_TYPE_ENUM_UINT32
 枚举说明  : 卡Fetch Type枚举
*****************************************************************************/
enum USIMM_CARD_FETCH_TYPE
{
    USIMM_CARD_FETCH_PPDOWN             = 0x01,     /* ppdown */
    USIMM_CARD_FETCH_WRITE_EFLOCI       = 0x02,     /* efloci */

    USIMM_CARD_FETCH_TYPE_BUTT
};
typedef VOS_UINT32 USIMM_CARD_FETCH_TYPE_ENUM_UINT32;

enum SI_PIH_CARD_SLOT_ENUM
{
    SI_PIH_CARD_SLOT_0      = 0, /*SCI_ID_0*/
    SI_PIH_CARD_SLOT_1      = 1, /*SCI_ID_1*/
    SI_PIH_CARD_SLOT_2      = 2, /*SCI_ID_2*/
    SI_PIH_CARD_SLOT_BUTT
};
typedef VOS_UINT32  SI_PIH_CARD_SLOT_ENUM_UINT32;

/*****************************************************************************
 枚举名    : USIMM_SIMSQ_STATE_ENUM
 枚举说明  : SIMSQ状态枚举变量
*****************************************************************************/
enum USIMM_SIMSQ_STATE_ENUM
{
    USIMM_SIMSQ_NOT_INSERTED            = 0,
    USIMM_SIMSQ_INSERTED                = 1,
    USIMM_SIMSQ_BUTT
};
typedef VOS_UINT32 USIMM_SIMSQ_STATE_ENUM_UINT32;

/*******************************************************************************
4 struct定义
*******************************************************************************/
typedef struct
{
    VOS_UINT8                           ucPin1RemainTime;       /* PIN1码剩余次数      */
    VOS_UINT8                           ucPin2RemainTime;       /* PIN2码剩余次数      */
    VOS_UINT8                           ucPuk1RemainTime;       /* PUK1码剩余次数     */
    VOS_UINT8                           ucPuk2RemainTime;       /* PUK2码剩余次数     */
    USIMM_PIN_ENABLE_STATUS_ENUM_UINT32 enPin1Enable;           /* PIN1激活状态        */
    USIMM_PIN_ENABLE_STATUS_ENUM_UINT32 enPin2Enable;           /* PIN2激活状态        */
}USIMM_PIN_INFO_STRU;

typedef struct
{
    VOS_UINT8                           ucCommandNum;
    VOS_UINT8                           ucCommandType;
    VOS_UINT8                           ucCommandQua;
    VOS_UINT8                           ucRsv;
}USIMM_STK_COMMAND_DETAILS_STRU;

typedef struct
{
    VOS_UINT8                           ucPin1RemainTime;    /* PIN1码剩余次数 */
    VOS_UINT8                           ucPin2RemainTime;    /* PIN2码剩余次数 */
    VOS_UINT8                           ucPuk1RemainTime;    /* PUK1码剩余次数 */
    VOS_UINT8                           ucPuk2RemainTime;    /* PUK2码剩余次数 */
}USIMM_PIN_REMAIN_STRU;

typedef struct
{
    VOS_UINT32                  ulDataLen;
    VOS_UINT8                   *pucData;
}USIMM_U8_LVDATA_STRU;

typedef struct
{
    VOS_UINT8                   ucDataLen;
    VOS_UINT8                   aucData[USIMM_APDU_DATA_MAXLEN];
}USIMM_U8_DATA_STRU;

typedef struct
{
    VOS_UINT32                  ulDataLen;
    VOS_UINT16                  *pusData;
}USIMM_U16_LVDATA_STRU;

typedef struct
{
    VOS_UINT32                      ulPathLen;
    VOS_CHAR                        acPath[USIMM_PATHSTR_MAX_LEN];
}USIMM_FILEPATH_INFO_STRU;

/*存储符合电话本的相关信息*/
typedef struct
{
    USIMM_FILEPATH_INFO_STRU        stFilePath;
    VOS_UINT16                      usDataLen;
    VOS_UINT16                      ausRsv;
    VOS_UINT8                       ucRecordNum;
    VOS_UINT8                       aucContent[255];
}USIMM_MUTILFILE_DATA_STRU;

/*****************************************************************************
 结构名    : USIMM_RESET_INFO_STRU
 结构说明  : STK发给USIMM模块的RESET消息中RESET信息字段结构
*****************************************************************************/
typedef struct
{
    VOS_UINT16                      usFileListLen;              /* FILE列表长度 */
    VOS_UINT8                       aucFileList[256];           /* FILE列表内容 */
    VOS_UINT16                      usAidLen;                   /* AID长度 */
    VOS_UINT8                       aucAid[USIMM_AID_LEN_MAX];  /* AID内容 */
}USIMM_RESET_INFO_STRU;

/*****************************************************************************
 结构名    : USIMM_2G_AUTH_STRU
 结构说明  : 2G鉴权数据
*****************************************************************************/
typedef struct
{
    USIMM_U8_DATA_STRU                  stRand;
}USIMM_2G_AUTH_STRU;

/*****************************************************************************
 结构名    : USIMM_3G_AUTH_STRU
 结构说明  : 3G鉴权数据
*****************************************************************************/
typedef struct
{
    USIMM_U8_DATA_STRU                  stRand;
    USIMM_U8_DATA_STRU                  stAutn;
}USIMM_3G_AUTH_STRU;

/*****************************************************************************
 结构名    : USIMM_GBA_AUTH_STRU
 结构说明  : GBA鉴权数据
*****************************************************************************/
typedef struct
{
    USIMM_U8_DATA_STRU                  stRand;
    USIMM_U8_DATA_STRU                  stAutn;
}USIMM_GBA_AUTH_STRU;

/*****************************************************************************
 结构名    : USIMM_NAF_AUTH_STRU
 结构说明  : NAF鉴权数据
*****************************************************************************/
typedef struct
{
    USIMM_U8_DATA_STRU                  stNafID;
    USIMM_U8_DATA_STRU                  stImpi;
}USIMM_NAF_AUTH_STRU;

/*****************************************************************************
 结构名    : USIMM_HTTP_AUTH_STRU
 结构说明  : HTTP鉴权数据
*****************************************************************************/
typedef struct
{
    USIMM_U8_DATA_STRU                  stRealm;
    USIMM_U8_DATA_STRU                  stNonce;
    USIMM_U8_DATA_STRU                  stCnonce;
}USIMM_HTTP_AUTH_STRU;

/*****************************************************************************
 结构名    : USIMM_IMS_AUTH_STRU
 结构说明  : IMS鉴权数据
*****************************************************************************/
typedef struct
{
    USIMM_U8_DATA_STRU                  stRand;
    USIMM_U8_DATA_STRU                  stAutn;
}USIMM_IMS_AUTH_STRU;

/*****************************************************************************
 结构名    : USIMM_RUNCAVE_AUTH_STRU
 结构说明  : RUN CAVE鉴权数据
*****************************************************************************/
typedef struct
{
    USIMM_RUNCAVE_RANDTYPE_ENUM_UINT8   ucRandType;
    VOS_UINT8                           ucDigLen;
    VOS_UINT8                           ucProcessCtrl;
    VOS_UINT8                           aucRand[USIMM_RUNCAVE_RAND_LEN];
    VOS_UINT8                           aucDigits[USIMM_RUNCAVE_DIGLEN];
    VOS_UINT8                           aucEsn[USIMM_ESN_MEID_LEN];
    VOS_UINT8                           aucRsv[3];
}USIMM_RUNCAVE_AUTH_STRU;

typedef struct
{
    USIMM_U8_DATA_STRU                  stRand;
    USIMM_U8_DATA_STRU                  stAutn;
}USIMM_EAP_AUTH_STRU;

typedef struct
{
    USIMM_U8_DATA_STRU                  stRand;
    USIMM_U8_DATA_STRU                  stAutn;
}USIMM_WLAN_AUTH_STRU;

typedef union
{
    USIMM_3G_AUTH_STRU                  st3GAuth;
    USIMM_2G_AUTH_STRU                  st2GAuth;
    USIMM_IMS_AUTH_STRU                 stImsAuth;
    USIMM_HTTP_AUTH_STRU                stHttpAuth;
    USIMM_GBA_AUTH_STRU                 stGbaAuth;
    USIMM_NAF_AUTH_STRU                 stNafAuth;
    USIMM_RUNCAVE_AUTH_STRU             stRunCaveAuth;
    USIMM_EAP_AUTH_STRU                 stEapAuth;
    USIMM_WLAN_AUTH_STRU                stWlanAuth;
}USIMM_AUTH_DATA_UNION;

#if (FEATURE_ON == FEATURE_UE_MODE_CDMA)
typedef struct
{
    VOS_UINT8                           aucAuthBS[USIMM_CDMA_AUTHBS_LEN];
    VOS_UINT8                           ucRsv;
}USIMM_CONFIRMSSD_DATA_STRU;

typedef struct
{
    VOS_UINT8                           aucRandSSD[USIMM_CDMA_RANDSSD_LEN];
    VOS_UINT8                           ucProcessCtrl;
    VOS_UINT8                           aucESN[USIMM_ESN_MEID_LEN];
    VOS_UINT8                           ucRsv;
}USIMM_UPDATESSD_DATA_STRU;

typedef union
{
    USIMM_CONFIRMSSD_DATA_STRU          stConfirmSSD;
    USIMM_UPDATESSD_DATA_STRU           stUpdateSSD;
}USIMM_MANAGESSD_DATA_UNION;

typedef struct
{
    USIMM_MANAGESSD_TYPE_ENUM_UINT32    enCmdType;
    USIMM_MANAGESSD_DATA_UNION          uSSDData;
}USIMM_MANAGESSD_DATA_STRU;
#endif
/*****************************************************************************
 结构名    : USIM_REFRESH_FILE_STRU
 结构说明  : USIMM REFRESH主动上报的文件信息
*****************************************************************************/
typedef struct
{
    USIMM_DEF_FILEID_ENUM_UINT16        usFileId;                               /* 转换后的文件ID */
    VOS_UINT16                          usPathLen;                              /* 文件路径长度 */
    VOS_UINT8                           aucPath[USIMM_REFRESH_FILE_MAX_PATH_LEN];     /* 文件全路径，字符串类型 */
    USIMM_CARDAPP_ENUM_UINT32           enAppType;                              /* APP类型 */
}USIM_REFRESH_FILE_STRU;

typedef struct
{
    VOS_UINT8                           aucIK[USIMM_AUTH_IKSPACELEN];          /*Len+Data*/
    VOS_UINT8                           aucCK[USIMM_AUTH_CKSPACELEN];          /*Len+Data*/
    VOS_UINT8                           aucGsmKc[USIMM_AUTH_KCSPACELEN];       /*Len+Data*/
    VOS_UINT8                           aucAuts[USIMM_AUTH_AUTHSPACELEN];      /*Len+Data*/
    VOS_UINT8                           aucAuthRes[USIMM_AUTH_RESSPACELEN];    /*Len+Data*/
    VOS_UINT8                           aucRsv[5];
}USIMM_TELECOM_AUTH_CNF_STRU;

typedef struct
{
    VOS_UINT8                           aucIK[USIMM_AUTH_IKSPACELEN];            /*Len+Data*/
    VOS_UINT8                           aucCK[USIMM_AUTH_CKSPACELEN];            /*Len+Data*/
    VOS_UINT8                           aucAuts[USIMM_AUTH_AUTHSPACELEN];        /*Len+Data*/
    VOS_UINT8                           aucAuthRes[USIMM_AUTH_RESSPACELEN];      /*Len+Data*/
    VOS_UINT8                           aucResponse[255];                        /*Len+Data*/
    VOS_UINT8                           aucSessionKey[255];                      /*Len+Data*/
    VOS_UINT8                           aucKs_ext_NAF[6];                   /*Len+Data*/
    VOS_UINT8                           aucRsv[2];
}USIMM_IMS_AUTH_CNF_STRU;

typedef struct
{
    VOS_UINT8                           aucAuthr[USIMM_AUTH_ATUHRSPACELEN];        /*Data*/
    VOS_UINT8                           ucRsv;
}USIMM_RUNCAVE_AUTH_CNF_STRU;

typedef union
{
    USIMM_TELECOM_AUTH_CNF_STRU         stTELECnf;
    USIMM_IMS_AUTH_CNF_STRU             stIMSCnf;
    USIMM_RUNCAVE_AUTH_CNF_STRU         stRunCaveCnf;
}USIMM_AUTNCNF_DATA_UNION;

typedef struct
{
    VOS_UINT8                           aucTPDUHead[USIMM_TPDU_HEAD_LEN];
    VOS_UINT8                           ucRsv;
    VOS_UINT16                          usDataLen;
    VOS_UINT8                           aucData[USIMM_TPDU_DATA_LEN_MAX];
}USIMM_TPDU_DATA_STRU;

typedef struct
{
    VOS_UINT8                           aucInputCmd[USIMM_TPDU_HEAD_LEN];
    VOS_UINT8                           ucSW1;
    VOS_UINT8                           ucSW2;
    VOS_UINT8                           ucRsv;
    VOS_UINT32                          ulDataLen;
    VOS_UINT8                           aucRspData[USIMM_TPDU_DATA_LEN_MAX];
}USIMM_TPDURSP_DATA_STRU;

typedef struct
{
    VOS_UINT8                           aucInputCmd[USIMM_TPDU_HEAD_LEN];
    VOS_UINT8                           ucSW1;
    VOS_UINT8                           ucSW2;
    VOS_UINT8                           ucLastDataFlag;
    VOS_UINT32                          ulDataLen;
    VOS_UINT8                           aucRspData[USIMM_PRIVATECGLA_DATA_LEN_MAX];
}USIMM_CGLARSP_DATA_STRU;

/*****************************************************************************
6 消息定义
*****************************************************************************/

typedef struct
{
    VOS_MSG_HEADER
    USIMM_CMDTYPE_REQ_ENUM_UINT32       enMsgName;
    USIMM_CARDAPP_ENUM_UINT32           enAppType;
    VOS_UINT32                          ulSendPara;
}USIMM_CMDHEADER_REQ_STRU;

typedef struct
{
    USIMM_CMDHEADER_REQ_STRU            stMsgHeader;
} USIMM_ACTIVECARD_REQ_STRU;

typedef struct
{
    USIMM_CMDHEADER_REQ_STRU            stMsgHeader;
} USIMM_INITCONTINUE_REQ_STRU;

/*****************************************************************************
 结构名    : USIMM_QUERYESNMEIDRSLT_REQ_STRU
 结构说明  : Store MEID/PESN消息处理结构
*****************************************************************************/
typedef struct
{
    USIMM_CMDHEADER_REQ_STRU            stMsgHeader;
} USIMM_QUERYESNMEIDRSLT_REQ_STRU;

typedef struct
{
    USIMM_CMDHEADER_REQ_STRU            stMsgHeader;
} USIMM_QUERYVOLTAGE_REQ_STRU;

/*******************************************************************************
 * Name        : USIMM_TLV_DATA_STRU
 * Description : This structure defines the TLV data format. It could be used
 * for some command's data.
 *******************************************************************************/
typedef struct
{
    VOS_UINT8                           ucTag;
    VOS_UINT8                           ucLen;
    VOS_UINT8                           aucData[USIMM_APDU_DATA_MAXLEN];
    VOS_UINT8                           aucRsv[3];
}USIMM_TLV_DATA_STRU;

/*******************************************************************************
 * Name        : USIMM_COMPUTEIP_CHAP_STRU
 * Description : This structure defines the input data format of CHAP command
 * type, See section 4.8.1.1 in <Invalid document reference (C.S0023 D_v2.0)>
 * and section 9.4.4.1 in 3GPP2 C.S0065 B v2.0: "cdma2000 Application on UICC
 * for Spread Spectrum Systems".
 *******************************************************************************/

typedef struct
{
    VOS_UINT8                           ucChapID;
    VOS_UINT8                           ucNAI_Enter_Index;
    VOS_UINT8                           aucRsv[2];
    USIMM_U8_DATA_STRU                  stChapChallenge;
}USIMM_COMPUTEIP_CHAP_STRU;

/*******************************************************************************
 * Name        : USIMM_COMPUTEIP_MNHA_STRU
 * Description : This structure defines the input data format of MN HA
 * Authenticator command type. See section 4.8.1.2 in <Invalid document
 * reference (C.S0023 D_v2.0)> and section 9.4.4.1 in 3GPP2 C.S0065 B v2.0:
 * "cdma2000 Application on UICC for Spread Spectrum Systems".
 *******************************************************************************/

typedef struct
{
    USIMM_SPECAUTH_BLOCK_ENUM_UINT8     enBlockType;
    VOS_UINT8                           ucNAI_Enter_Index;
    VOS_UINT8                           aucRsv[2];
    USIMM_U8_DATA_STRU                  stChapChallenge;
}USIMM_COMPUTEIP_MNHA_STRU;

/*******************************************************************************
 * Name        : USIMM_COMPUTEIP_MIPRRQHASH_STRU
 * Description : This structure defines the input data format of MIP RRQ Hash
 * command type. See section 4.8.1.3 in <Invalid document reference
 * (C.S0023 D_v2.0)> and section 9.4.4.1 in 3GPP2 C.S0065 B v2.0: "cdma2000
 * Application on UICC for Spread Spectrum Systems".
 *******************************************************************************/

typedef struct
{
    USIMM_SPECAUTH_BLOCK_ENUM_UINT8     enBlockType;
    VOS_UINT8                           aucRsv[3];
    USIMM_U8_DATA_STRU                  stMIPRRQData;
    USIMM_U8_DATA_STRU                  stMNAAAExHeader;
}USIMM_COMPUTEIP_MIPRRQHASH_STRU;

/*******************************************************************************
 * Name        : USIMM_COMPUTEIP_MNAAA_STRU
 * Description : This structure defines the input data format of MN AAA
 * Authenticator command type. See section 4.8.1.4 in <Invalid document
 * reference (C.S0023 D_v2.0)> and section 9.4.4.1 in 3GPP2 C.S0065 B v2.0:
 * "cdma2000 Application on UICC for Spread Spectrum Systems".
 *******************************************************************************/

typedef struct
{
    VOS_UINT8                           ucNAI_Enter_Index;
    VOS_UINT8                           aucRsv[3];
    USIMM_U8_DATA_STRU                  stChallenge;
}USIMM_COMPUTEIP_MNAAA_STRU;

/*******************************************************************************
 * Name        : USIMM_COMPUTEIP_MNAAA_STRU
 * Description : This structure defines the input data format of MN AAA
 * Authenticator command type. See section 4.8.1.4 in <Invalid document
 * reference (C.S0023 D_v2.0)> and section 9.4.4.1 in 3GPP2 C.S0065 B v2.0:
 * "cdma2000 Application on UICC for Spread Spectrum Systems".
 *******************************************************************************/

typedef struct
{
    VOS_UINT8                           ucCHAPID;
    VOS_UINT8                           aucRsv[3];
    USIMM_U8_DATA_STRU                  stChapChallenge;
}USIMM_COMPUTEIP_HRPD_STRU;

/*******************************************************************************
 * Name        : USIMM_COMPUTEIP_DATA_UNION
 * Description : This union defines the data container in
 * USIMM_COMPUTEIP_AUTH_STRU.
 *    stChapData: Input data format of CHAP command type, See section 4.8.1.1
 *    in <Invalid document reference (C.S0023 D_v2.0)> and section 9.4.4.1
 *    3GPP2 C.S0065 B v2.0: "cdma2000 Application on UICC for Spread Spectrum
 *    Systems";
 *    stMNHAData: Input data format of MN HA Authenticator command type. See
 *    section 4.8.1.2 in <Invalid document reference (C.S0023 D_v2.0)> and
 *    section 9.4.4.1 3GPP2 C.S0065 B v2.0: "cdma2000 Application on UICC for
 *    Spread Spectrum Systems";
 *    stMIPData: Input data format of MIP RRQ Hash command type. See section
 *    4.8.1.3 in <Invalid document reference (C.S0023 D_v2.0)> and section
 *    9.4.4.1 3GPP2 C.S0065 B v2.0: "cdma2000 Application on UICC for Spread
 *    Spectrum Systems";
 *    stMNAAAData: Input data format of MN AAA Authenticator command type. See
 *    section 4.8.1.4 in <Invalid document reference (C.S0023 D_v2.0)> and
 *    section 9.4.4.1 3GPP2 C.S0065 B v2.0: "cdma2000 Application on UICC for
 *    Spread Spectrum Systems";
 *    stHRPDData: Input data format of HRPD Access Authentication command
 *    type. See section 4.8.1.5 in <Invalid document reference
 *    (C.S0023 D_v2.0)> and section 9.4.4.1 3GPP2 C.S0065 B v2.0: "cdma2000
 *    Application on UICC for Spread Spectrum Systems";
 *******************************************************************************/

typedef union
{
    USIMM_COMPUTEIP_CHAP_STRU           stChapData;
    USIMM_COMPUTEIP_MNHA_STRU           stMNHAData;
    USIMM_COMPUTEIP_MIPRRQHASH_STRU     stMIPData;
    USIMM_COMPUTEIP_MNAAA_STRU          stMNAAAData;
    USIMM_COMPUTEIP_HRPD_STRU           stHRPDData;
}USIMM_COMPUTEIP_DATA_UNION;

/*******************************************************************************
 * Name        : USIMM_COMPUTEIP_AUTH_STRU
 * Description : This structure defines the input data format, that used for
 * Compute IP command, which belong to CDMA special authentication command set.
 * See section 4.8.1 in <Invalid document reference (C.S0023 D_v2.0)> and
 * section 9.4.4.1 in 3GPP2 C.S0065 B v2.0: "cdma2000 Application on UICC for
 * Spread Spectrum Systems".
 *    enComputeIPType: The command type that used to process the Compute IP
 *    request;
 *    uComputeIPData: The input data of Compute IP command;
 *******************************************************************************/

typedef struct
{
    USIMM_COMPUTEIP_AUTH_ENUM_UINT32    enComputeIPType;
    USIMM_COMPUTEIP_DATA_UNION          uComputeIPData;
}USIMM_COMPUTEIP_AUTH_STRU;

/*******************************************************************************
 * Name        : USIMM_BCMCS_RETRIEVESK_STRU
 * Description : This structure defines the command RETRIEVE SK input data
 * format, See section 4.9.1 in <Invalid document reference (C.S0023 D_v2.0)>.
 * The tag could see Annex B in <Invalid document reference (C.S0023 D_v2.0)>,
 * and it defines by
 * USIMM_BCMCS_TAG_ENUM_UINT8 type.
 *******************************************************************************/
typedef struct
{
    USIMM_TLV_DATA_STRU                 stBCMCSFlowID;
    USIMM_TLV_DATA_STRU                 stBAKID;
    USIMM_TLV_DATA_STRU                 stSKRand;
}USIMM_BCMCS_RETRIEVESK_STRU;

/*******************************************************************************
 * Name        : USIMM_BCMCS_UPDATABAK_STRU
 * Description : This structure defines the command Update BAK input data
 * format, See section 4.9.2 in <Invalid document reference (C.S0023 D_v2.0)>.
 * The tag could see Annex B in <Invalid document reference (C.S0023 D_v2.0)>,
 * and it defines by
 * USIMM_BCMCS_TAG_ENUM_UINT8 type.
 *******************************************************************************/
typedef struct
{
    USIMM_TLV_DATA_STRU                 stBCMCSFlowID;
    USIMM_TLV_DATA_STRU                 stBAKID;
    USIMM_TLV_DATA_STRU                 stBAKExprie;
    USIMM_TLV_DATA_STRU                 stTKRand;
    USIMM_U8_DATA_STRU                  stEncryptedBAK;
}USIMM_BCMCS_UPDATABAK_STRU;

/*******************************************************************************
 * Name        : USIMM_BCMCS_DELETEBAK_STRU
 * Description : This structure defines the command Delete BAK input data
 * format, See section 4.9.3 in <Invalid document reference (C.S0023 D_v2.0)>.
 * The tag could see Annex B in <Invalid document reference (C.S0023 D_v2.0)>,
 * and it defines by
 * USIMM_BCMCS_TAG_ENUM_UINT8 type.
 *******************************************************************************/
typedef struct
{
    USIMM_TLV_DATA_STRU                 stBCMCSFlowID;
    USIMM_TLV_DATA_STRU                 stBAKID;
}USIMM_BCMCS_DELETEBAK_STRU;

/*******************************************************************************
 * Name        : USIMM_BCMCS_RETRIEVE_SRTPSK_STRU
 * Description : This structure defines the command Delete BAK input data
 * format, See section 4.9.3 in <Invalid document reference (C.S0023 D_v2.0)>.
 * The tag could see Annex B in <Invalid document reference (C.S0023 D_v2.0)>,
 * and it defines by
 * USIMM_BCMCS_TAG_ENUM_UINT8 type.
 *******************************************************************************/
typedef struct
{
    USIMM_TLV_DATA_STRU                 stBCMCSFlowID;
    USIMM_TLV_DATA_STRU                 stBAKID;
    USIMM_TLV_DATA_STRU                 stSKRand;
    USIMM_TLV_DATA_STRU                 stPacketIndex;
}USIMM_BCMCS_RETRIEVE_SRTPSK_STRU;

/*******************************************************************************
 * Name        : USIMM_BCMCS_GENAUTH_SIGN_STRU
 * Description : This structure defines the command Generate Authorization
 * Signature input data format, See section 4.9.5 in <Invalid document
 * reference (C.S0023 D_v2.0)>.
 * The tag could see Annex B in <Invalid document reference (C.S0023 D_v2.0)>,
 * and it defines by
 * USIMM_BCMCS_TAG_ENUM_UINT8 type.
 *******************************************************************************/
typedef struct
{
    USIMM_TLV_DATA_STRU                 stBCMCSFlowID;
    USIMM_TLV_DATA_STRU                 stBAKID;
    USIMM_TLV_DATA_STRU                 stTimestamp;
}USIMM_BCMCS_GENAUTH_SIGN_STRU;

/*******************************************************************************
 * Name        : USIMM_BCMCS_AUTHENTICATION_STRU
 * Description : This structure defines the command data that input by BCMCS
 * Authentication command.
 * See section 4.9.6 in <Invalid document reference (C.S0023 D_v2.0)> and
 * section 9.4.5 in 3GPP2 C.S0065 B v2.0: "cdma2000 Application on UICC for
 * Spread Spectrum Systems".
 *    stRand: TLV Data structure;
 *    stChallenge: TLV Data structure;
 * The tag could see Annex B in <Invalid document reference (C.S0023 D_v2.0)>.
 *******************************************************************************/
typedef struct
{
    USIMM_TLV_DATA_STRU                 stRand;
    USIMM_TLV_DATA_STRU                 stChallenge;
}USIMM_BCMCS_AUTHENTICATION_STRU;

/*******************************************************************************
 * Name        : USIMM_BCMCS_DATA_UNION
 * Description : This union define the data container in the
 * USIMM_BCMCS_DATA_STRU.
 *    stRetrieveSK: Command RETRIEVE SK input data format, See section 4.9.1
 *    in <Invalid document reference (C.S0023 D_v2.0)>;
 *    stUpdateBAK: Command Update BAK input data format, See section 4.9.2 in
 *    <Invalid document reference (C.S0023 D_v2.0)>;
 *    stDeleteBAK: Command Delete BAK input data format, See section 4.9.3 in
 *    <Invalid document reference (C.S0023 D_v2.0)>;
 *    stRESRTPSK: Command Retrieve SRTP SK input data format, See section
 *    4.9.4 in <Invalid document reference (C.S0023 D_v2.0)>;
 *    stGenAuthSign: Command Generate Authorization Signature input data
 *    format, See section 4.9.5 in <Invalid document reference
 *    (C.S0023 D_v2.0)>;
 *    stAuthentication: Command BCMCS Authentication input data format, See
 *    section 4.9.6 in <Invalid document reference (C.S0023 D_v2.0)>
 *******************************************************************************/
typedef union
{
    USIMM_BCMCS_RETRIEVESK_STRU         stRetrieveSK;
    USIMM_BCMCS_UPDATABAK_STRU          stUpdateBAK;
    USIMM_BCMCS_DELETEBAK_STRU          stDeleteBAK;
    USIMM_BCMCS_RETRIEVE_SRTPSK_STRU    stReSRTPSK;
    USIMM_BCMCS_GENAUTH_SIGN_STRU       stGenAuthSign;
    USIMM_BCMCS_AUTHENTICATION_STRU     stAuthentication;
}USIMM_BCMCS_DATA_UNION;

/*******************************************************************************
 * Name        : USIMM_BCMCS_DATA_STRU
 * Description : This structure defines all of BCMCS command input data format.
 *    enBCMCSType: The command type;
 *    ulServiceType: the value '01' is corresponding to "3GPP2 BCMCS", other
 *    value is RFU;
 *    uBCMCSData: BCMCS input data;
 *******************************************************************************/

typedef struct
{
    USIMM_BCMCS_AUTH_ENUM_UINT32        enBCMCSType;
    VOS_UINT32                          ulServiceType;
    USIMM_BCMCS_DATA_UNION              uBCMCSData;
}USIMM_BCMCS_DATA_STRU;

/*******************************************************************************
 * Name        : USIMM_CDMAOTA_AUTH_STRU
 * Description : This structure defines the input data that need by processing
 * the Application Authentication command. See section 4.10.1 in <Invalid
 * document reference (C.S0023 D_v2.0)> and section 9.4.6.1 in 3GPP2 C.S0065 B
 * v2.0: "cdma2000 Application on UICC for Spread Spectrum Systems".
 *******************************************************************************/
typedef struct
{
    USIMM_CDMAAPP_AUTH_ENUM_UINT32      enAuthMech;
    VOS_UINT32                          ulAppID;
    USIMM_U8_DATA_STRU                  stRealm;
    USIMM_U8_DATA_STRU                  stServiceNonce;
    USIMM_U8_DATA_STRU                  stClientNonce;
}USIMM_CDMAAPP_AUTH_STRU;

/*******************************************************************************
 * Name        : USIMM_CDMASPECAUTH_UNION
 * Description : This union defines the data container in the
 * USIMM_CDMASPEC_AUTH_STRU.
 *    stComputeIP: Command Compute IP's input data;
 *    stBCMCS: Command BCMCS's input data;
 *    stOTAAuth: Command Application Authentication's input data;
 *******************************************************************************/

typedef union
{
    USIMM_COMPUTEIP_AUTH_STRU           stComputeIP;
    USIMM_BCMCS_DATA_STRU               stBCMCS;
    USIMM_CDMAAPP_AUTH_STRU             stAppAuth;
}USIMM_CDMASPECAUTH_UNION;


/*****************************************************************************
 结构名    : USIMM_PROTECTRESET_REQ_STRU
 结构说明  : SIM卡保护性复位请求消息结构
*****************************************************************************/
typedef struct
{
    USIMM_CMDHEADER_REQ_STRU            stMsgHeader;
} USIMM_PROTECTRESET_REQ_STRU;

/*****************************************************************************
 结构名    : USIMM_DEACTIVECARD_REQ_STRU
 结构说明  : SIM卡去激活请求消息结构
*****************************************************************************/
typedef struct
{
    USIMM_CMDHEADER_REQ_STRU            stMsgHeader;
}USIMM_DEACTIVECARD_REQ_STRU;

/*****************************************************************************
 结构名    : USIMM_AUTHENTICATION_REQ_STRU
 结构说明  : SIM卡鉴权请求消息结构
*****************************************************************************/
typedef struct
{
    USIMM_CMDHEADER_REQ_STRU            stMsgHeader;
    USIMM_AUTH_TYPE_ENUM_UINT32         enAuthType;
    USIMM_AUTH_DATA_UNION               uAuth;
} USIMM_AUTHENTICATION_REQ_STRU;

/*****************************************************************************
 结构名    : USIMM_PINHANDLE_REQ_STRU
 结构说明  : SIM卡PIN码请求消息结构
*****************************************************************************/
typedef struct
{
    USIMM_CMDHEADER_REQ_STRU            stMsgHeader;
    USIMM_PIN_CMD_TYPE_ENUM_UINT32      enCmdType;
    USIMM_PIN_TYPE_ENUM_UINT32          enPINType;
    VOS_UINT8                           aucOldPIN[USIMM_PINNUMBER_LEN];
    VOS_UINT8                           aucNewPIN[USIMM_PINNUMBER_LEN];
} USIMM_PINHANDLE_REQ_STRU;

typedef struct
{
    USIMM_CMDHEADER_REQ_STRU            stMsgHeader;
    USIMM_FILEPATH_INFO_STRU            stFilePath;
    VOS_UINT16                          usRsv;
    VOS_UINT16                          usDataLen;
    VOS_UINT8                           ucRecordNum;
    VOS_UINT8                           aucContent[3];
}USIMM_UPDATEFILE_REQ_STRU;

/*****************************************************************************
 结构名    : USIMM_UPDATEFILE_WITHIMSI_REQ_STRU
 结构说明  : 带imsi检查的卡文件更新请求消息结构
*****************************************************************************/
typedef struct
{
    USIMM_CMDHEADER_REQ_STRU            stMsgHeader;
    VOS_UINT8                           aucImsi[USIMM_EF_IMSI_LEN];
    VOS_UINT8                           aucRsv[3];
    USIMM_FILEPATH_INFO_STRU            stFilePath;
    VOS_UINT16                          usRsv;
    VOS_UINT16                          usDataLen;
    VOS_UINT8                           ucRecordNum;
    VOS_UINT8                           aucContent[3];
}USIMM_UPDATEFILE_WITHIMSI_REQ_STRU;

typedef struct
{
    USIMM_CMDHEADER_REQ_STRU            stMsgHeader;
    USIMM_FILEPATH_INFO_STRU            stFilePath;
    VOS_BOOL                            bNeedReadCache;
    VOS_UINT16                          usRsv;
    VOS_UINT8                           ucRecordNum;
    VOS_UINT8                           ucRsv;
}USIMM_READFILE_REQ_STRU;

typedef struct
{
    USIMM_CMDHEADER_REQ_STRU            stMsgHeader;
    USIMM_FILEPATH_INFO_STRU            stFilePath;
} USIMM_QUERYFILE_REQ_STRU;

/*****************************************************************************
 结构名    : USIMM_STATUSCMD_REQ_STRU
 结构说明  : SIM卡周期性轮询请求消息结构
*****************************************************************************/
typedef struct
{
    USIMM_CMDHEADER_REQ_STRU       stMsgHeader;
    USIMM_FILEPATH_INFO_STRU       stFilePath;
    USIMM_POLL_FCP_ENUM_UINT32     enNeedFcp;
} USIMM_STATUSCMD_REQ_STRU;

typedef struct
{
    USIMM_CMDHEADER_REQ_STRU       stMsgHeader;
    USIMM_FILEPATH_INFO_STRU       stFilePath;
    VOS_UINT32                     ulDataLen;
    VOS_UINT8                      aucData[4];
} USIMM_SEARCHFILE_REQ_STRU;

/*****************************************************************************
 结构名    : USIMM_FDNPROCESS_REQ_STRU
 结构说明  : FDN激活去激活请求消息结构
*****************************************************************************/
typedef struct
{
    USIMM_CMDHEADER_REQ_STRU       stMsgHeader;
    USIMM_FBDN_HANDLE_ENUM_UINT32  enHandleType;
    VOS_UINT8                      aucPIN[USIMM_PINNUMBER_LEN];
} USIMM_FDNPROCESS_REQ_STRU;

/*****************************************************************************
 结构名    : USIMM_CHANNEL_INFO_STRU
 结构说明  : USIM模块CHANNEL INFO的数据结构
*****************************************************************************/
typedef struct
{
    VOS_UINT32                          ulSessionId;                            /* 会话id */
    VOS_UINT32                          ulChanNum;                              /* 通道号 */
    VOS_UINT32                          ulAIDLen;                               /* AID长度 */
    VOS_UINT8                           aucADFName[2*USIMM_AID_LEN_MAX];        /* AID的内容 */
    VOS_UINT8                           ucAPDUP2;
    VOS_UINT8                           ucRsv[3];
}USIMM_CHANNEL_INFO_STRU;

/*****************************************************************************
 结构名    : USIMM_OPENCHANNEL_REQ_STRU
 结构说明  : 逻辑通道打开请求消息结构
*****************************************************************************/
typedef struct
{
    USIMM_CMDHEADER_REQ_STRU            stMsgHeader;
    USIMM_CHANNEL_INFO_STRU             stChannelInfo;
} USIMM_OPENCHANNEL_REQ_STRU;

/*****************************************************************************
 结构名    : USIMM_CLOSECHANNEL_REQ_STRU
 结构说明  : 逻辑通道关闭请求消息结构
*****************************************************************************/
typedef struct
{
    USIMM_CMDHEADER_REQ_STRU            stMsgHeader;
    VOS_UINT32                          ulSessionID;                            /* 会话ID */
} USIMM_CLOSECHANNEL_REQ_STRU;

typedef struct
{
    USIMM_CMDHEADER_REQ_STRU            stMsgHeader;
    USIMM_FILEPATH_INFO_STRU            stFilePath;
    USIMM_TPDU_DATA_STRU                stTPDUData;
} USIMM_SENDTPDUDATA_REQ_STRU;

#if (FEATURE_ON == FEATURE_UE_MODE_CDMA)
/*****************************************************************************
 结构名    : USIMM_BSCHALLENGE_REQ_STRU
 结构说明  : BASE STATION CHALLENGE请求消息结构
*****************************************************************************/
typedef struct
{
    USIMM_CMDHEADER_REQ_STRU       stMsgHeader;
    VOS_UINT8                      aucData[USIMM_AUTH_BSCSPACELEN];
} USIMM_BSCHALLENGE_REQ_STRU;

/*****************************************************************************
 结构名    : USIMM_GENERATE_KEYVPM_REQ_STRU
 结构说明  : Generate KEY VMP请求消息结构
*****************************************************************************/
typedef struct
{
    USIMM_CMDHEADER_REQ_STRU       stMsgHeader;
    VOS_UINT8                      ucFirstOctet;
    VOS_UINT8                      ucLastOctet;
    VOS_UINT8                      aucRsv[2];
} USIMM_GENERATE_KEYVPM_REQ_STRU;

/*****************************************************************************
 结构名    : USIMM_MAMAGESSD_REQ_STRU
 结构说明  : Manage SSD请求消息结构
*****************************************************************************/
typedef struct
{
    USIMM_CMDHEADER_REQ_STRU        stMsgHeader;
    USIMM_MANAGESSD_DATA_STRU       stSsdData;
} USIMM_MANAGESSD_REQ_STRU;
#endif

/*****************************************************************************
 结构名    : USIMM_TERMINALRESPONSE_REQ_STRU
 结构说明  : TERMINAL RESPONSE请求消息结构
*****************************************************************************/
typedef struct
{
    USIMM_CMDHEADER_REQ_STRU            stMsgHeader;
    VOS_UINT8                           ucDataLen;
    VOS_UINT8                           aucContent[3];
}USIMM_TERMINALRESPONSE_REQ_STRU;

/*****************************************************************************
 结构名    : USIMM_PB_INIT_STATUS_IND_STRU
 结构说明  : 电话本初始化状态指示消息结构
*****************************************************************************/
typedef struct
{
    USIMM_CMDHEADER_REQ_STRU            stMsgHeader;
    USIMM_PB_INIT_STATUS_ENUM_UINT16    enPBInitStatus;
    VOS_UINT16                          usRsv;
}USIMM_PBINIT_STATUS_IND_STRU;

/*****************************************************************************
 结构名    : USIMM_STKREFRESH_REQ_STRU
 结构说明  : REFRESH请求消息结构
*****************************************************************************/
typedef struct
{
    USIMM_CMDHEADER_REQ_STRU            stMsgHeader;
    VOS_UINT32                          ulCommandNum;
    VOS_UINT32                          ulCommandType;
    USIMM_REFRESH_TYPE_ENUM_UINT32      enRefreshType;
    VOS_UINT32                          ulAidLen;
    VOS_UINT8                           aucAid[USIMM_AID_LEN_MAX];
    VOS_UINT16                          usLen;
    VOS_UINT8                           aucFileList[6];
}USIMM_STKREFRESH_REQ_STRU;

/*****************************************************************************
 结构名    : USIMM_STKENVELOPE_REQ_STRU
 结构说明  : ENVELOPE请求消息结构
*****************************************************************************/
typedef struct
{
    USIMM_CMDHEADER_REQ_STRU            stMsgHeader;
    VOS_UINT8                           ucDataLen;
    VOS_UINT8                           aucContent[3];
}USIMM_STKENVELOPE_REQ_STRU;

/*****************************************************************************
 结构名    : USIMM_RACCESS_REQ_STRU
 结构说明  : 受限制SIM卡访问请求消息结构
*****************************************************************************/
typedef struct
{
    USIMM_CMDHEADER_REQ_STRU            stMsgHeader;
    USIMM_RESTRIC_CMD_ENUM_UINT32       enCmdType;
    VOS_UINT16                          usEfId;
    VOS_UINT16                          usPathLen;
    VOS_UINT8                           ucP1;
    VOS_UINT8                           ucP2;
    VOS_UINT8                           ucP3;
    VOS_UINT8                           ucRsv;
    VOS_UINT16                          ausPath[USIMM_MAX_PATH_LEN];
    VOS_UINT8                           aucContent[USIMM_T0_APDU_MAX_LEN+1];
    VOS_UINT8                           aucRsv[3];
}USIMM_RACCESS_REQ_STRU;

typedef struct
{
    USIMM_CMDHEADER_REQ_STRU            stMsgHeader;
    VOS_UINT32                          ulFileCount;
    USIMM_MUTILFILE_DATA_STRU           stFileData[USIMM_SETMUTILFILE_MAX];/*最多14个文件*/
}USIMM_SETMUTILFILE_REQ_STRU;

#if (FEATURE_ON == FEATURE_UE_MODE_CDMA)
typedef struct
{
    USIMM_CMDHEADER_REQ_STRU            stMsgHeader;
    USIMM_CDMASPEC_AUTHTYPE_ENUM_UINT32 enAuthType;
    USIMM_CDMASPECAUTH_UNION            uAuthData;
}USIMM_CDMASPECAUTH_REQ_STRU;
#endif

#if (FEATURE_VSIM == FEATURE_ON)
typedef struct
{
    USIMM_CMDHEADER_REQ_STRU            stMsgHeader;
    USIMM_PHYCARD_TYPE_ENUM_UINT32      enCardType;
    VOS_UINT32                          ulAuthType;
    VOS_UINT32                          ulAesKeylen;
    VOS_UINT8                           aucAeskey[32];
    VOS_UINT32                          ulDataLen;              /* FILE DATA 长度 */
    VOS_UINT8                           aucContent[4];          /* FILE DATA 内容 */
}USIMM_VSIMFILEDATAWRITE_REQ_STRU;
#endif


typedef struct
{
    USIMM_CMDHEADER_REQ_STRU            stMsgHeader;        /* PIH消息头 */
    SI_PIH_CARD_SLOT_ENUM_UINT32        enCard0Slot;
    SI_PIH_CARD_SLOT_ENUM_UINT32        enCard1Slot;
    SI_PIH_CARD_SLOT_ENUM_UINT32        enCard2Slot;
} USIMM_SCICFG_SET_REQ_STRU;


/*****************************************************************************
8 消息声明
*****************************************************************************/

/*****************************************************************************
 结构名    : USIMM_CARDAPP_STATUS_STRU
 结构说明  : 卡应用状态信息
*****************************************************************************/
typedef struct
{
    USIMM_CARDAPP_SERVIC_ENUM_UINT32    enCardAppService;
    VOS_UINT32                          ulIsTestCard;                    /* VOS_TRUE为测试卡，VOS_FALSE为非测试卡 */
    VOS_UINT16                          usIsIMSIValid;                   /* VOS_TRUE为有效，VOS_FALSE为无效 */
    VOS_UINT8                           aucIMSI[USIMM_EF_IMSI_COMM_LEN]; /* UsimSim应用为IMSI，CsimUim为IMSI_M*/
    VOS_UINT8                           ucGID1Len;                       /* 长度为0无效(服务状态位无效，读取失败填0)，其它值有效，最长16个字节 */
    VOS_UINT8                           aucGID1[USIMM_EF_GID1_LEN];      /* UsimSim应用为6F3E，CsimUim为6F6D*/
    VOS_UINT8                           ucGID2Len;                       /* 长度为0无效(服务状态位无效，读取失败填0)，其它值有效，最长16个字节 */
    VOS_UINT8                           aucGID2[USIMM_EF_GID2_LEN];      /* UsimSim应用为6F3F，CsimUim为6F6E*/
    VOS_UINT8                           aucRsv[2];
}USIMM_CARDAPP_STATUS_STRU;

/*****************************************************************************
 结构名    : USIMM_CARDSTATUS_ADDINFO_STRU
 结构说明  : USIMM 主动上报的卡状态附加信息，卡状态有效的时候才有意义
*****************************************************************************/
typedef struct
{
    USIMM_CARD_MEDIUM_TYPE_ENUM_UINT32  enMediumType;      /* 卡物理存储介质 */
    USIMM_REALISIM_STATE_ENUM_UINT32    enISIMState;
    VOS_UINT8                           ucHasCModule;
    VOS_UINT8                           ucHasWGModule;
    VOS_UINT8                           aucICCID[USIMM_ICCID_FILE_LEN];
    VOS_UINT8                           ucIsICCIDValid;
    USIMM_CARD_STATUS_IND_ENUM_UINT8    enStatusType;
    VOS_UINT8                           aucRev[2];
}USIMM_CARDSTATUS_ADDINFO_STRU;

/* 卡状态消息和RIL有耦合，该结构变动需知会RIL */
typedef struct
{
    VOS_MSG_HEADER
    USIMM_CMDTYPE_CNF_ENUM_UINT32       ulMsgName;
    USIMM_PHYCARD_TYPE_ENUM_UINT32      enPhyCardType;  /*物理卡状态*/
    USIMM_CARDAPP_STATUS_STRU           stUsimSimInfo;  /*GUL SIM状态*/
    USIMM_CARDAPP_STATUS_STRU           stCsimUimInfo;  /*CDMA SIM状态*/
    USIMM_CARDAPP_STATUS_STRU           stIsimInfo;     /*ISIM状态*/
    USIMM_CARDSTATUS_ADDINFO_STRU       stAddInfo;      /*卡状态有效时候才能使用里面的信息*/
}USIMM_CARDSTATUS_IND_STRU;

/*****************************************************************************
 结构名    : USIMM_CMDHEADER_CNF_STRU
 结构说明  : USIMM 回复消息头结构
*****************************************************************************/
typedef struct
{
    VOS_MSG_HEADER
    USIMM_CMDTYPE_CNF_ENUM_UINT32       enMsgName;
    VOS_UINT32                          ulResult;
    VOS_UINT32                          ulErrorCode;
    USIMM_CARDAPP_ENUM_UINT32           enApptype;
    VOS_UINT32                          ulSendPara;
}USIMM_CMDHEADER_CNF_STRU;

/*****************************************************************************
 结构名    : USIMM_CMDHEADER_IND_STRU
 结构说明  : USIMM 主动上报消息头结构
*****************************************************************************/
typedef struct
{
    VOS_MSG_HEADER
    USIMM_CMDTYPE_CNF_ENUM_UINT32       enMsgName;
}USIMM_CMDHEADER_IND_STRU;


/*****************************************************************************
 结构名    : USIMM_AUTHENTICATION_CNF_STRU
 结构说明  : USIMM 鉴权回复消息结构
*****************************************************************************/
typedef struct
{
    USIMM_CMDHEADER_CNF_STRU            stCmdResult;
    USIMM_AUTH_TYPE_ENUM_UINT32         enAuthType;
    USIMM_AUTH_RESULT_ENUM_UINT32       enResult;
    USIMM_AUTNCNF_DATA_UNION            uCnfData;
} USIMM_AUTHENTICATION_CNF_STRU;

typedef struct
{
    USIMM_CMDHEADER_CNF_STRU            stCmdResult;
    USIMM_PIN_CMD_TYPE_ENUM_UINT32      enCmdType;
    USIMM_PIN_TYPE_ENUM_UINT32          enPinType;
    USIMM_PIN_INFO_STRU                 stPinInfo;
} USIMM_PINHANDLE_CNF_STRU;

typedef struct
{
    USIMM_CMDHEADER_CNF_STRU            stCmdResult;
    USIMM_PIN_CMD_TYPE_ENUM_UINT32      enCmdType;
    USIMM_PIN_TYPE_ENUM_UINT32          enPinType;
    USIMM_PIN_INFO_STRU                 stPinInfo;
    VOS_UINT8                           aucOldPIN[USIMM_PINNUMBER_LEN];
} USIMM_SILENT_PINHANDLE_CNF_STRU;

typedef struct
{
    USIMM_CMDHEADER_CNF_STRU            stCmdResult;
    USIMM_FILEPATH_INFO_STRU            stFilePath;
    VOS_UINT16                          usRsv;
    VOS_UINT16                          usEFLen;
    VOS_UINT8                           ucRecordNum;
    VOS_UINT8                           ucSW1;
    VOS_UINT8                           ucSW2;
    VOS_UINT8                           ucRsv;
} USIMM_UPDATEFILE_CNF_STRU;

typedef struct
{
    USIMM_CMDHEADER_CNF_STRU            stCmdResult;
    USIMM_FILEPATH_INFO_STRU            stFilePath;
    VOS_UINT16                          usRsv;
    VOS_UINT16                          usEfLen;
    VOS_UINT8                           ucRecordNum;
    VOS_UINT8                           ucTotalNum;
    VOS_UINT8                           ucSW1;
    VOS_UINT8                           ucSW2;
    VOS_UINT8                           aucEf[4];
} USIMM_READFILE_CNF_STRU;

typedef struct
{
    USIMM_CMDHEADER_CNF_STRU            stCmdResult;
    USIMM_FILEPATH_INFO_STRU            stFilePath;
    VOS_UINT16                          usRsv;
    VOS_UINT16                          usFileLen;
    USIMM_EFSTATUS_ENUM_UINT8           enFileStatus; /*文件的激活状态*/
    USIMM_EFRWFLAG_ENUM_UINT8           enFileRWFlag; /*文件可读和可更新标志*/
    VOS_UINT8                           ucRecordNum;
    VOS_UINT8                           ucRecordLen;
    VOS_UINT8                           aucFileInfo[4];
} USIMM_QUERYFILE_CNF_STRU;

typedef struct
{
    USIMM_CMDHEADER_CNF_STRU            stCmdResult;
    VOS_UINT8                           ucSW1;
    VOS_UINT8                           ucSW2;
    VOS_UINT16                          usRsv;
    VOS_UINT32                          ulDataLen;
    VOS_UINT8                           aucData[4];
} USIMM_STATUSCMD_CNF_STRU;

typedef struct
{
    USIMM_CMDHEADER_CNF_STRU            stCmdResult;
    USIMM_FILEPATH_INFO_STRU            stFilePath;
    VOS_UINT16                          usTotalRecNum;      /* 文件中记录数 */
    VOS_UINT8                           ucSW1;
    VOS_UINT8                           ucSW2;
    VOS_UINT16                          usDataLen;
    VOS_UINT8                           aucData[2];
} USIMM_SEARCHFILE_CNF_STRU;

/*****************************************************************************
 结构名    : USIMM_QUERYESNMEIDRSLT_CNF_STRU
 结构说明  : Store MEID/PESN处理回复消息结构
*****************************************************************************/
typedef struct
{
    USIMM_CMDHEADER_CNF_STRU            stCmdResult;
    VOS_UINT8                           ucIsStored;          /* VOS_TRUE:Store MEID/PESN */
    VOS_UINT8                           ucStoreResult;
    VOS_UINT8                           aucRsv[2];
}USIMM_QUERYESNMEIDRSLT_CNF_STRU;

/*****************************************************************************
 结构名    : USIMM_FDNPROCESS_CNF_STRU
 结构说明  : FDN处理回复消息结构
*****************************************************************************/
typedef struct
{
    USIMM_CMDHEADER_CNF_STRU            stCmdResult;
    USIMM_FDNSTATUS_ENUM_UINT32         enFDNState;
} USIMM_FDNPROCESS_CNF_STRU;

/*****************************************************************************
 结构名    : USIMM_OPENCHANNEL_CNF_STRU
 结构说明  : 逻辑通道打开处理回复消息结构
*****************************************************************************/
typedef struct
{
    USIMM_CMDHEADER_CNF_STRU            stCmdResult;
    VOS_UINT32                          ulSessionID;
    VOS_UINT8                           ucChannelID;
    VOS_UINT8                           aucRsv[3];
} USIMM_OPENCHANNEL_CNF_STRU;

/*****************************************************************************
 结构名    : USIMM_CLOSECHANNEL_CNF_STRU
 结构说明  : 逻辑通道关闭处理回复消息结构
*****************************************************************************/
typedef struct
{
    USIMM_CMDHEADER_CNF_STRU            stCmdResult;
} USIMM_CLOSECHANNEL_CNF_STRU;

typedef struct
{
    USIMM_CMDHEADER_CNF_STRU            stCmdResult;
    USIMM_FILEPATH_INFO_STRU            stFilePath;
    USIMM_TPDURSP_DATA_STRU             stRspData;
} USIMM_SENDTPDUDATA_CNF_STRU;

typedef struct
{
    USIMM_CMDHEADER_CNF_STRU            stCmdResult;
    USIMM_CGLARSP_DATA_STRU             stRspData;
} USIMM_PRIVATECGLAHANDLE_CNF_STRU;

/*****************************************************************************
 结构名    : USIMM_BSCHALLENGE_CNF_STRU
 结构说明  : CDMA Base Station Challenge处理回复消息结构
*****************************************************************************/
typedef struct
{
    USIMM_CMDHEADER_CNF_STRU            stCmdResult;
    VOS_UINT32                          ulDataLen;
    VOS_UINT8                           aucData[4];
} USIMM_BSCHALLENGE_CNF_STRU;

/*****************************************************************************
 结构名    : USIMM_GENERATE_KEYVPM_CNF_STRU
 结构说明  : USIMM GENERATE KEY & VMP 处理回复消息结构
*****************************************************************************/
typedef struct
{
    USIMM_CMDHEADER_CNF_STRU            stCmdResult;
    VOS_UINT8                           aucKey[8];
    VOS_UINT32                          ulVPMLen;
    VOS_UINT8                           aucVPM[4];
} USIMM_GENERATE_KEYVPM_CNF_STRU;

/*****************************************************************************
 结构名    : USIM_STKTERMINALRSP_CNF_STRU
 结构说明  : USIMM TERMINAL RESPONSE处理回复消息结构
*****************************************************************************/
typedef struct
{
    USIMM_CMDHEADER_CNF_STRU            stCmdResult;
    VOS_UINT8                           ucSW1;
    VOS_UINT8                           ucSW2;
    VOS_UINT8                           aucRsv[2];
}USIMM_STKTERMINALRSP_CNF_STRU;

/*****************************************************************************
 结构名    : USIMM_SINGLECMD_CNF_STRU
 结构说明  : USIMM简单命令处理回复消息结构
*****************************************************************************/
typedef struct
{
    USIMM_CMDHEADER_CNF_STRU            stCmdResult;
}USIMM_SINGLECMD_CNF_STRU;

/*****************************************************************************
 结构名    : USIMM_STKREFRESH_CNF_STRU
 结构说明  : USIMM REFRESH处理回复消息结构
*****************************************************************************/
typedef struct
{
    USIMM_CMDHEADER_CNF_STRU            stCmdResult;
    USIMM_REFRESH_TYPE_ENUM_UINT32      enRefreshType;
    VOS_UINT32                          ulSatLen;           /* 后续主动命令的长度 */
}USIMM_STKREFRESH_CNF_STRU;

/*****************************************************************************
 结构名    : USIMM_STKENVELOPE_CNF_STRU
 结构说明  : USIMM ENVELOPE CNF消息结构
*****************************************************************************/
typedef struct
{
    USIMM_CMDHEADER_CNF_STRU            stCmdResult;
    VOS_UINT8                           ucSW1;
    VOS_UINT8                           ucSW2;
    VOS_UINT8                           ucDataType;
    VOS_UINT8                           rsv;
    VOS_UINT32                          dataLen;
    VOS_UINT8                           aucData[USIMM_T0_APDU_MAX_LEN];
}USIMM_STKENVELOPE_CNF_STRU;

/*****************************************************************************
 结构名    : USIMM_STKENVELOPE_RETRY_STRU
 结构说明  : USIMM发送给的重试STKENVELOPE的消息结构
*****************************************************************************/
typedef struct
{
    VOS_MSG_HEADER
    USIMM_CMDTYPE_REQ_ENUM_UINT32       enMsgName;
    USIMM_STKENVELOPE_REQ_STRU          stEnvelopeReqMsgData;
}USIMM_STKENVELOPE_RETRY_STRU;

/*****************************************************************************
 结构名    : USIMM_SETMUTILFILE_CNF_STRU
 结构说明  : 电话本多文件更新 CNF消息结构
*****************************************************************************/
typedef struct
{
    USIMM_CMDHEADER_CNF_STRU            stCmdResult;
    VOS_UINT16                          ausEfId[USIMM_SETMUTILFILE_MAX];    /* 要更新的文件的ID*/
    VOS_UINT16                          ausEfLen[USIMM_SETMUTILFILE_MAX];   /* EF长度 */
    VOS_UINT8                           aucRecordNum[USIMM_SETMUTILFILE_MAX];/* linear fixed或cycle fixed类型EF记录号*/
    VOS_UINT8                           ucFileNum;
    VOS_UINT8                           ucRsv;
}USIMM_SETMUTILFILE_CNF_STRU;

/*****************************************************************************
 结构名    : USIMM_OMTRANS_MSG_STRU
 结构说明  : 发给OM的透明消息
*****************************************************************************/
typedef struct
{
    VOS_MSG_HEADER                                      /* 该消息头填充TOOL_ID和PS的PID,保证消息能转发给相应的PS模块 */
    VOS_UINT16                      usTransPrimId;       /* 固定填写0x5001*/
    VOS_UINT16                      usReserve;
    VOS_UINT8                       ucFuncType;          /*数据查询04,空口消息上报:02*/
    VOS_UINT8                       ucReserve;
    VOS_UINT16                      usAppLength;         /*下一个字节到包尾的长度*/
    VOS_UINT32                      ulSn;                /* 上报消息的序列号 */
    VOS_UINT32                      ulTimeStamp;         /*消息的时间戳，填当前的SOC Tick值*/
    VOS_UINT16                      usPrimId;            /* 原语ID*/
    VOS_UINT16                      usToolsId;           /* 工具ID */
    VOS_UINT8                       ucResult;            /*传输消息的内容*/
    VOS_UINT8                       ucEFLen;             /*前两个字节填写读取的长度*/
    VOS_UINT8                       aucData[2];
}USIMM_OMTRANS_MSG_STRU;

/*****************************************************************************
 结构名    : USIMM_STKDATA_IND_STRU
 结构说明  : USIMM STK主动上报消息结构
*****************************************************************************/
typedef struct
{
    USIMM_CMDHEADER_IND_STRU            stIndHdr;
    VOS_UINT16                          usLen;          /* 主动命令长度 */
    VOS_UINT8                           ucCmdType;      /* 主动命令类型 */
    VOS_UINT8                           ucRsv;
    VOS_UINT8                           aucContent[USIMM_T0_APDU_MAX_LEN];
}USIMM_STKDATA_IND_STRU;

/*****************************************************************************
 结构名    : USIMM_STKREFRESH_IND_STRU
 结构说明  : USIMM REFRESH主动上报消息结构
*****************************************************************************/
typedef struct
{
    USIMM_CMDHEADER_IND_STRU            stIndHdr;
    USIMM_REFRESH_TYPE_ENUM_UINT32      enUsimRefreshType;  /* 卡刷新类型 */
    USIMM_REFRESH_FILE_TYPE_UINT16      enRefreshType;      /* 文件更新类型 */
    VOS_UINT16                          usEfNum;            /* 更新的文件的个数 */
    USIM_REFRESH_FILE_STRU              astEfId[1];         /* 更新的文件ID 信息，可变长数组*/
}USIMM_STKREFRESH_IND_STRU;

/*****************************************************************************
 结构名    : USIMM_RACCESS_CNF_STRU
 结构说明  : USIMM 受限制访问CNF消息结构
*****************************************************************************/
typedef struct
{
    USIMM_CMDHEADER_CNF_STRU            stCmdResult;
    VOS_UINT8                           ucSW1;                                      /* Status Word 1*/
    VOS_UINT8                           ucSW2;                                      /* Status Word 2 */
    VOS_UINT16                          usLen;                                      /* 返回数据长度,不包含SW1和SW2       */
    VOS_UINT8                           aucContent[USIMM_APDU_RSP_MAX_LEN];         /* 返回Data*/
}USIMM_RACCESS_CNF_STRU;

/*****************************************************************************
 结构名    : USIMM_ECCNUMBER_IND_STRU
 结构说明  : ECC号码上报消息结构
*****************************************************************************/
typedef struct
{
    USIMM_CMDHEADER_IND_STRU            stIndHdr;
    VOS_UINT16                          usFileSize;     /*ucContent中数据长度*/
    VOS_UINT8                           ucEccType;      /*紧急呼叫号码是SIM卡还是USIM卡*/
    VOS_UINT8                           ucRecordNum;    /*USIM卡紧急呼叫号码记录数，SIM卡填0*/
    VOS_UINT8                           ucContent[4];   /*紧急呼叫号码记录信息*/
}USIMM_ECCNUMBER_IND_STRU;

/*****************************************************************************
结构名    : USIMM_VSIM_RDH_IND_STRU
结构说明  : 虚拟卡DH重协议上报指示消息结构
*****************************************************************************/
typedef struct
{
    USIMM_CMDHEADER_IND_STRU            stIndHdr;
}USIMM_VSIM_RDH_IND_STRU;

/*****************************************************************************
结构名    : USIMM_HOTINOUT_IND_STRU
结构说明  : MBB产品上卡拔插上报指示消息结构
*****************************************************************************/
typedef struct
{
    USIMM_CMDHEADER_IND_STRU            stIndHdr;
    VOS_UINT32                          ulCardInOutStatus;
}USIMM_HOTINOUT_IND_STRU;

/*****************************************************************************
 结构名    : USIMM_APP_SESSION_INFO_STRU
 结构说明  : 应用SESSION信息
*****************************************************************************/
typedef struct
{
    VOS_UINT32                          ulChannelID;
    VOS_UINT32                          ulSessionID;
}USIMM_APP_SESSION_INFO_STRU;

/*****************************************************************************
 结构名    : USIMM_CARDSESSION_CNF_STRU
 结构说明  : 查询应用SESSION处理回复消息结构
*****************************************************************************/
typedef struct
{
    USIMM_CMDHEADER_CNF_STRU            stCmdResult;
    USIMM_APP_SESSION_INFO_STRU         astSessionInfo[USIMM_CARDAPP_BUTT];
}USIMM_CARDSESSION_CNF_STRU;

#if (FEATURE_ON == FEATURE_UE_MODE_CDMA)
/*****************************************************************************
 结构名    : USIMM_XECCNUMBER_IND_STRU
 结构说明  : XECC号码上报消息结构
*****************************************************************************/
typedef struct
{
    USIMM_CMDHEADER_IND_STRU            stIndHdr;
    VOS_UINT16                          usFileSize;     /*ucContent中数据长度*/
    VOS_UINT8                           ucContent[2];   /*紧急呼叫号码记录信息*/
}USIMM_XECCNUMBER_IND_STRU;

/*******************************************************************************
 * Name        : USIMM_COMPUTEIP_CNF_STRU
 * Description : This structure defines the reply data format after processing
 * the Compute IP request.
 *    enComputeIPType: The command type of Compute IP request, that input by
 *    request massage;
 *    ulDataLen: The data length that receive from card;
 *    ucRspData: The data content that receive from card;
 *******************************************************************************/
typedef struct
{
    USIMM_COMPUTEIP_AUTH_ENUM_UINT32    enComputeIPType;
    VOS_UINT32                          ulDataLen;
    VOS_UINT8                           aucRspData[USIMM_APDU_RSP_MAX_LEN];
}USIMM_COMPUTEIP_CNF_STRU;

/*******************************************************************************
 * Name        : USIMM_BCMCS_CNF_STRU
 * Description : This structure defines the BCMCS command response data
 * format, that contain by return massage.
 *    enBSMCSType: The BCMCS command type that input by request;
 *    ucBCMCSRspTag: The response data tag, which could see Annex B in
 *    <Invalid document reference (C.S0023 D_v2.0)>;
 *    ucBCMCSRspLen: The response data length;
 *    aucBCMCSRsp: The response data, and the max value is 16 bytes, according
 *    all of BCMCS command;
 *******************************************************************************/
typedef struct
{
    USIMM_BCMCS_AUTH_ENUM_UINT32        enBCMCSType;
    VOS_UINT8                           ucBCMCSRspTag;
    VOS_UINT8                           ucBCMCSRspLen;
    VOS_UINT8                           aucBCMCSRsp[USIMM_BCMCSRSP_LEN_MAX];
    VOS_UINT8                           aucRsv[2];
}USIMM_BCMCS_CNF_STRU;

/*******************************************************************************
 * Name        : USIMM_CDMAAPP_AUTH_CNF_STRU
 * Description : This structure defines the response data that by processing
 * the Application Authentication command. See section 4.10.1 in <Invalid
 * document reference (C.S0023 D_v2.0)> and section 9.4.6.1 in 3GPP2 C.S0065 B
 * v2.0: "cdma2000 Application on UICC for Spread Spectrum Systems".
 *    enOTAAuthType: The command type which input by request massage;
 *    stRsp: response data;
 *    stSessionKey1: Session key 1;
 *    stSessionKey2: Session key 2;
 *******************************************************************************/
typedef struct
{
    USIMM_CDMAAPP_AUTH_ENUM_UINT32      enAppAuthType;
    USIMM_U8_DATA_STRU                  stResponse;
    USIMM_U8_DATA_STRU                  stSessionKey1;
    USIMM_U8_DATA_STRU                  stSessionKey2;
}USIMM_CDMAAPP_AUTH_CNF_STRU;

/*******************************************************************************
 * Name        : USIMM_CDMASPECAUTH_DATA_UNION
 * Description : This union defines the data container in
 * PS_USIM_CDMASPECAUTH_CNF .
 *    stComputeIPCnf: The response data of Compute IP command, defined by
 *    USIMM_COMPUTEIP_CNF_STRU;
 *    stBCMCSCnf: The response data of BCMCS command, defined by
 *    USIMM_BCMCS_CNF_STRU;
 *    stOTAAuthCnf: The response data of OTA, defined by
 *    USIMM_CDMAOTA_AUTH_CNF_STRU;
 *******************************************************************************/

typedef union
{
    USIMM_COMPUTEIP_CNF_STRU            stComputeIPCnf;
    USIMM_BCMCS_CNF_STRU                stBCMCSCnf;
    USIMM_CDMAAPP_AUTH_CNF_STRU         stAPPAuthCnf;
}USIMM_CDMASPECAUTH_DATA_UNION;

/*****************************************************************************
 结构名    : USIMM_RACCESS_CNF_STRU
 结构说明  : USIMM 受限制访问CNF消息结构
*****************************************************************************/
typedef struct
{
    USIMM_CMDHEADER_CNF_STRU            stCmdResult;
    USIMM_CDMASPEC_AUTHTYPE_ENUM_UINT32 enAuthType;
    USIMM_CDMASPECAUTH_DATA_UNION       uAuthCnf;         /* 返回Data*/
}USIMM_CDMASPECAUTH_CNF_STRU;
#endif

typedef struct
{
    USIMM_CMDHEADER_IND_STRU            stIndHdr;
    USIMM_CMDTYPE_REQ_ENUM_UINT32       enCmdType;
}USIMM_CARDSCIERR_IND_STRU;

typedef struct
{
    VOS_UINT32                              ulErrNum;
    VOS_UINT32                              aulErrInfo[USIMM_ERROR_INFO_MAX];
}USIMM_ERRORINFO_DATA_STRU;

typedef struct
{
    USIMM_CMDHEADER_IND_STRU            stIndHdr;
    USIMM_ERRORINFO_DATA_STRU           stErrorInfo;
}USIMM_CARDERROR_IND_STRU;

typedef struct
{
    USIMM_CMDHEADER_IND_STRU            stIndHdr;
    USIMM_CARD_FETCH_TYPE_ENUM_UINT32   enCardFetchType;
}USIMM_CARDFETCH_IND_STRU;

/*****************************************************************************
 结构名    : USIMM_ICCIDCONTENT_IND_STRU
 结构说明  : ICCID上报消息
*****************************************************************************/
typedef struct
{
    USIMM_CMDHEADER_IND_STRU            stIndHdr;
    VOS_UINT8                           aucIccid[USIMM_ICCID_FILE_LEN];
    VOS_UINT8                           aucRcv[2];
}USIMM_ICCIDCONTENT_IND_STRU;

/*****************************************************************************
 结构名    : USIMM_CHANGE_CARDMODE_REQ_STRU
 结构说明  : 请求ICC切模消息结构
*****************************************************************************/
typedef struct
{
    VOS_MSG_HEADER
    VOS_UINT32                          ulMsgName;
    USIMM_CARDAPP_ENUM_UINT32           enNewCardApp;
}USIMM_CHANGE_CARDMODE_REQ_STRU;

/*****************************************************************************
 结构名    : USIMM_CHANGE_MODE_ERROR_ENUM_UINT32
 结构说明  : ICC切模错误码枚举
*****************************************************************************/
enum USIMM_CHANGE_MODE_ERROR_ENUM
{
    USIMM_CHANGE_MODE_SUCC                  = 0,
    USIMM_CHANGE_MODE_APP_TYPE_ERROR        = 1, /* 期望切换APP类型错误 */
    USIMM_CHANGE_MODE_NOT_ICC               = 2, /* CCT国漫切换需只有ICC卡才需要USIM模块重启卡 */
    USIMM_CHANGE_MODE_NO_CWG                = 3, /* 非双模卡 */
    USIMM_CHANGE_MODE_UNAVAILABLE           = 4, /* 当前卡复位不可用 */
    USIMM_CHANGE_MODE_SET_PRF_APP_FAIL      = 5, /* 设置应用优先级顺序失败 */
    USIMM_CHANGE_MODE_SAME_APP              = 6, /* 需要切换的就是当前应用 */
    USIMM_CHANGE_MODE_FEATURE_NOT_SUPPORT   = 7, /* 特性不支持:切卡只有CDMA特性宏打开才支持 */

    USIMM_CHANGE_MODE_BUTT
};
typedef VOS_UINT32 USIMM_CHANGE_MODE_ERROR_ENUM_UINT32;

/*****************************************************************************
 结构名    : USIMM_CHANGE_CARDMODE_CNF_STRU
 结构说明  : ICC切模回复消息结构
*****************************************************************************/
typedef struct
{
    VOS_MSG_HEADER
    VOS_UINT32                                  ulMsgName;
    VOS_UINT32                                  ulResult;
    USIMM_CHANGE_MODE_ERROR_ENUM_UINT32         enErrCode;
}USIMM_CHANGE_CARDMODE_CNF_STRU;

/*****************************************************************************
 结构名    : USIMM_FDNQUERY_REQ_STRU
 结构说明  : FDN状态查询消息
*****************************************************************************/
typedef struct
{
    USIMM_CMDHEADER_REQ_STRU            stMsgHeader;
} USIMM_FDNQUERY_REQ_STRU;

/*****************************************************************************
 结构名    : USIMM_FDNQUERY_CNF_STRU
 结构说明  : FDN状态回复消息
*****************************************************************************/
typedef struct
{
    USIMM_CMDHEADER_CNF_STRU            stCmdResult;
    USIMM_FDNSTATUS_ENUM_UINT32         enFDNState;
} USIMM_FDNQUERY_CNF_STRU;

/*****************************************************************************
 结构名    : USIMM_QUERY_CARDTYPE_CNF_STRU
 结构说明  : CardtypeEx状态回复消息
*****************************************************************************/
typedef struct
{
    USIMM_CMDHEADER_CNF_STRU            stCmdResult;
    VOS_UINT32                          ulCardMode;
    VOS_BOOL                            bHasCModule;
    VOS_BOOL                            bHasGModule;
} USIMM_QUERY_CARDTYPE_CNF_STRU;

/*****************************************************************************
 结构名    : USIMM_FDNSTATUS_IND_STRU
 结构说明  : FDN状态主动上报消息
*****************************************************************************/
typedef struct
{
    USIMM_CMDHEADER_CNF_STRU            stCmdResult;
    USIMM_FDNSTATUS_ENUM_UINT32         enFDNState;
} USIMM_FDNSTATUS_IND_STRU;

typedef struct
{
    USIMM_CMDHEADER_CNF_STRU            stCmdResult;
    VOS_UINT32                          ulVoltage;
    VOS_UINT8                           ucCharaByte;
    VOS_UINT8                           ucRsv[3];
}USIMM_QUERYVOLTAGE_CNF_STRU;


typedef struct
{
    VOS_UINT32                          bitModemNum      : 3;
    VOS_UINT32                          bitReserved0     : 5;  /* bit[3-7]    : 保留。 */
    VOS_UINT32                          bitModem0Slot    : 3;  /* bit[8-10]   : modem 0使用的SCI接口。
                                                                                   0：使用SCI0；
                                                                                   1：使用SCI1；
                                                                                   2：使用SCI2；
                                                                                 其余值:多卡，暂不支持 */
    VOS_UINT32                          bitModem1Slot   : 3;  /* bit[11-13]  : modem 1使用的SCI接口。
                                                                                   0：使用SCI0；
                                                                                   1：使用SCI1；
                                                                                   2：使用SCI2；
                                                                                 其余值:多卡，暂不支持 */
    VOS_UINT32                          bitModem2Slot   : 3;  /* bit[14-16]  : modem 2使用的SCI接口。
                                                                                   0：使用SCI0；
                                                                                   1：使用SCI1；
                                                                                   2：使用SCI2；
                                                                                 其余值:多卡，暂不支持*/
    VOS_UINT32                          bitReserved1     : 15;  /* bit[17-31]  : 保留 */
}USIMM_SCI_CFG_STRU;

/*****************************************************************************
 结构名    : USIMM_PB_STATUS_CNF_STRU
 结构说明  : USIMM 回复Pb功能状态 消息头结构
*****************************************************************************/
typedef struct
{
    VOS_MSG_HEADER
    USIMM_CMDTYPE_CNF_ENUM_UINT32       enMsgName;
    VOS_BOOL                            bPbReady;           /* VOS_TRUE:pb ready  VOS_FALSE:pb not ready */
}USIMM_PB_STATUS_CNF_STRU;

/*****************************************************************************
 结构名    : USIMM_SIMSQSTATUS_IND_STRU
 结构说明  : USIMM 回复Simsq状态 消息头结构
*****************************************************************************/
typedef struct
{
    VOS_MSG_HEADER
    USIMM_CMDTYPE_CNF_ENUM_UINT32       enMsgName;
    USIMM_SIMSQ_STATE_ENUM_UINT32       enSimSqStatus;
}USIMM_SIMSQSTATUS_IND_STRU;

/*****************************************************************************
7 api声明
*****************************************************************************/


/**************************************************************************
8 OTHERS定义
*****************************************************************************/

#if ((VOS_OS_VER == VOS_WIN32) || (VOS_OS_VER == VOS_NUCLEUS))
#pragma pack()
#else
#pragma pack(0)
#endif


#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif                                                                          /* __cpluscplus*/

#endif
