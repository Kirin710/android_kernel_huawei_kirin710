/*
 * teek_client_id.h
 *
 * define exported data for secboot CA
 *
 * Copyright (c) 2012-2018 Huawei Technologies Co., Ltd.
 *
 * This software is licensed under the terms of the GNU General Public
 * License version 2, as published by the Free Software Foundation, and
 * may be copied, distributed, and modified under those terms.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 */
#ifndef _TEE_CLIENT_ID_H_
#define _TEE_CLIENT_ID_H_

#define TEE_SERVICE_SECBOOT \
{ \
	0x08080808, \
	0x0808, \
	0x0808, \
	{ \
		0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08 \
	} \
}

/* e7ed1f64-4687-41da-96dc-cbe4f27c838f */
#define TEE_SERVICE_ANTIROOT \
{ \
	0xE7ED1F64, \
	0x4687, \
	0x41DA, \
	{ \
		0x96, 0xDC, 0xCB, 0xE4, 0xF2, 0x7C, 0x83, 0x8F \
	} \
}
/* dca5ae8a-769e-4e24-896b-7d06442c1c0e */
#define TEE_SERVICE_SECISP \
{ \
	0xDCA5AE8A, \
	0x769E, \
	0x4E24, \
	{ \
		0x89, 0x6B, 0x7D, 0x06, 0x44, 0x2C, 0x1C, 0x0E \
	} \
}
/*
 * @ingroup  TEE_COMMON_DATA
 *
 * ��ȫ����secboot֧�ֵ�����ID
 */
enum SVC_SECBOOT_CMD_ID {
	SECBOOT_CMD_ID_INVALID = 0x0,    /* Secboot Task ��ЧID */
	SECBOOT_CMD_ID_COPY_VRL,         /* Secboot Task ����VRL */
	SECBOOT_CMD_ID_COPY_DATA,        /* Secboot Task ��������*/
	SECBOOT_CMD_ID_VERIFY_DATA,      /* Secboot Task ��֤*/
	SECBOOT_CMD_ID_RESET_IMAGE,      /* Secboot Task ��λSoC*/
	SECBOOT_CMD_ID_COPY_VRL_TYPE,    /* Secboot Task ����VRL��������SoC Type */
	SECBOOT_CMD_ID_COPY_DATA_TYPE,   /* Secboot Task ��������,������SoC Type */
	SECBOOT_CMD_ID_VERIFY_DATA_TYPE, /* Secboot Task У�飬������SoC Type��У��ɹ��⸴λSoC */
	SECBOOT_CMD_ID_VERIFY_DATA_TYPE_LOCAL, /* Secboot Taskԭ��У�飬������SoC Type,У��ɹ��⸴λSoC */
	SECBOOT_CMD_ID_COPY_IMG_TYPE,          /* Secboot Task Copy img from secure buffer to run addr> */
	SECBOOT_CMD_ID_BSP_MODEM_CALL,         /* Secboot Task ִ�ж�Ӧ����*/
	SECBOOT_CMD_ID_BSP_MODULE_VERIFY,      /* Secboot Task modem moduleУ�麯��*/
	SECBOOT_CMD_ID_BSP_ICC_OPEN_THREAD,    /* Secboot Task icc open����*/
	SECBOOT_CMD_ID_BSP_RFILE_RW_THREAD,    /* Secboot Task rfile thread����*/
	SECBOOT_CMD_ID_GET_RNG_NUM,            /* Secboot Task ��ȡӲ������� */
};

/*
 * @ingroup TEE_COMMON_DATA
 *
 * ��ȫ����secboot֧�ֵľ�������
 */
#ifdef CONFIG_HISI_SECBOOT_IMG

#define CAS 0xff
enum SVC_SECBOOT_IMG_TYPE {
    MODEM,
    DSP,
    XDSP,
    TAS,
    WAS,
    MODEM_COMM_IMG,
    MODEM_DTB,
    NVM,
    NVM_S,
    MBN_R,
    MBN_A,
    MODEM_COLD_PATCH,
    DSP_COLD_PATCH,
    MODEM_CERT,
    MAX_SOC_MODEM,
    HIFI,
    ISP,
    IVP,
    SOC_MAX
};
#elif defined(CONFIG_HISI_SECBOOT_IMG_V2)
enum SVC_SECBOOT_IMG_TYPE {
	HIFI,
	ISP,
	IVP,
	MAX_AP_SOC,
	MODEM_START = 0x100,
	MODEM_END = 0x1FF,
	MAX_SOC,
};
#else
enum SVC_SECBOOT_IMG_TYPE {
	MODEM,
	HIFI,
	DSP,
	XDSP,
	TAS,
	WAS,
	CAS,
	MODEM_DTB,
	ISP,
/* miami c30����Ҫ֧���䲹�����ԣ��÷�֧��modem������miami_c30 Modem���빲��֧��
  * �����䲹��ö����Ϊ���ڸ÷�֧����ͨ��miami�汾����ȫOS��û�����Ӷ�Ӧ��ö�٣��÷�֧�ϱ����miami�汾��֧���䲹�����ԣ�
  * ��֧��Ӧ��ϵ���ӣ�http://3ms.huawei.com/hi/group/8729/wiki_5190309.html
  */
#ifdef CONFIG_COLD_PATCH
	MODEM_COLD_PATCH,
	DSP_COLD_PATCH,
#endif
	SOC_MAX
};
#endif

#endif