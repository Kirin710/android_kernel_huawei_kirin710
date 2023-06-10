/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2012-2020. All rights reserved.
 * Description: contexthub route source file
 * Author: DIVS_SENSORHUB
 * Create: 2012-05-29
 */

#include <contexthub_route_plu.h>
#include <linux/kernel.h>
#include <linux/rtc.h>
#include <linux/time64.h>
#include <linux/types.h>
#include <linux/timer.h>
#include <securec.h>
#include "protocol.h"
#include "sensor_config.h"
#include "sensor_detect.h"
#include "sensor_feima.h"
#include "sensor_sysfs.h"

void drop_fastboot_record(const pkt_drop_data_req_t *head)
{
	static int drop_record_num;
	static struct pkt_drop_fastboot_record_t drop_record;
	struct timeval time = {0};

	if (head->data.material != 1 || head->data.height <= DROP_FASTBOOT_RECORD)
		return;

	read_calibrate_data_from_nv(DROP_NV_NUM, DROP_NV_SIZE, "DROP");
	if (memcpy_s(&drop_record, sizeof(struct pkt_drop_fastboot_record_t),
		user_info.nv_data, DROP_NV_SIZE) != EOK)
		hwlog_info("drop_fastboot_record copy nv fail\n");

	drop_record_num = drop_record.index;
	do_gettimeofday(&time);
	drop_record.drop_info[drop_record_num].time = time.tv_sec;
	drop_record.drop_info[drop_record_num].height = head->data.height;
	hwlog_info("drop_fastboot_record time: %ld, height: %d index: %d\n",
		drop_record.drop_info[drop_record_num].time,
		drop_record.drop_info[drop_record_num].height,
		drop_record.index);

	drop_record_num++;
	if (drop_record_num >= DROP_FASTBOOT_NUM)
		drop_record_num = 0;

	drop_record.index = drop_record_num;
	if (write_calibrate_data_to_nv(DROP_NV_NUM,
		DROP_NV_SIZE, "DROP", (char *)&drop_record))
		hwlog_info("drop_fastboot_record write nv fail\n");
}


