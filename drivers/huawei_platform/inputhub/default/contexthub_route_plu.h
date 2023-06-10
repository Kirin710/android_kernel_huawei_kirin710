/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2012-2020. All rights reserved.
 * Description: contexthub route source file
 * Author: DIVS_SENSORHUB
 * Create: 2020-07-29
 */
#ifndef __LINUX_INPUTHUB_ROUTE_PLU_H__
#define __LINUX_INPUTHUB_ROUTE_PLU_H__

#include <linux/mtd/hisi_nve_interface.h>

#define DROP_NV_NUM 440
#define DROP_NV_SIZE 100
#define DROP_FASTBOOT_RECORD 30
#define DROP_FASTBOOT_NUM 8

#pragma pack(4)
struct pkt_drop_info_record_t {
	long time;
	int height;
};

struct pkt_drop_fastboot_record_t {
	struct pkt_drop_info_record_t drop_info[DROP_FASTBOOT_NUM];
	int index;
};
#pragma pack()

extern struct hisi_nve_info_user user_info;

#endif /* __LINUX_INPUTHUB_ROUTE_PLU_H__ */

