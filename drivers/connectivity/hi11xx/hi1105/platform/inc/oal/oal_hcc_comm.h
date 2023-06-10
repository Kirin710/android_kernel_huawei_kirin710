

#ifndef __OAL_HCC_COMM_H
#define __OAL_HCC_COMM_H

#undef CONFIG_HCC_DEBUG
#undef CONFIG_HCC_TX_MULTI_BUF_CHECK
#define CONFIG_HCC_HEADER_CHECK_SUM

typedef enum _hcc_action_type__ {
    HCC_ACTION_TYPE_WIFI = 0,      /* data from wifi */
    HCC_ACTION_TYPE_OAM = 1,       /* data from oam,sdt etc. */
    HCC_ACTION_TYPE_TEST = 2,      /* used for hcc transfer test */
    HCC_ACTION_TYPE_CHR = 3,       /* used for device chr exception */
    HCC_ACTION_TYPE_CUSTOMIZE = 4, /* used for device customization data transmit */
    HCC_ACTION_TYPE_BUS = 5,       /* used for hcc bus */

    HCC_ACTION_TYPE_BUTT
} hcc_action_type;

typedef enum _hcc_chan_type_ {
    HCC_TX = 0x0,
    HCC_RX = 0x1,
    HCC_DIR_COUNT
} hcc_chan_type;

typedef enum _oam_sub_type_ {
    DEAULT_VALUE = 0,
    DUMP_REG = 1,
    DUMP_MEM = 2,
} oam_sub_type;

typedef enum _hcc_sub_type_bus_ {
    HCC_SUB_BUS_SWITCH = 0,
    HCC_SUB_BUS_IP_PM = 1, /* WiFi IO IP PowerManager */
} hcc_sub_type_bus;

/*
 * hcc protocol header
 * |-------hcc total(64B)-----|-----------package mem--------------|
 * |hcc hdr|pad hdr|hcc extend|pad_payload|--------payload---------|
 */
#pragma pack(push, 1)
/* 6bytes */
struct hcc_header {
#define HCC_SUB_TYPE_BUTT 16   /* 4 bits bound [0~15] */
    oal_uint8 sub_type : 4;    /* sub type to hcc type,refer to hcc_action_type */
    oal_uint8 main_type : 4;   /* main type to hcc type,refer to hcc_action_type */

    oal_uint8 seq;              /* seq num for debug */
    oal_uint8 pad_hdr;          /* pad_hdr only need 6 bits, the 2bits reserved,
	                                   pad_hdr used to algin hcc tcm hdr(64B) */
    oal_uint16 more : 1;        /* for hcc aggregation */
    oal_uint16 option : 1;      /* reserved */
    oal_uint16 pad_payload : 2; /* the pad to algin the payload addr */
    oal_uint16 pay_len : 12;    /* the payload length,
                                        did't contain the extend hcc hdr area */
} __OAL_DECLARE_PACKED;
#pragma pack(pop)
#define HCC_HDR_TOTAL_LEN        64
#define HCC_HDR_LEN              (OAL_SIZEOF(struct hcc_header))
#define HCC_HDR_RESERVED_MAX_LEN (HCC_HDR_TOTAL_LEN - HCC_HDR_LEN)

typedef enum _hcc_test_case_ {
    HCC_TEST_CASE_TX = 0,
    HCC_TEST_CASE_RX = 1,
    HCC_TEST_CASE_LOOP = 2,
    HCC_TEST_CASE_COUNT
} hcc_test_case;

/* less than 16,4bits for sub_type */
typedef enum _hcc_test_subtype_ {
    HCC_TEST_SUBTYPE_CMD = 0, /* command mode */
    HCC_TEST_SUBTYPE_DATA = 1,
    HCC_TEST_SUBTYPE_BUTT
} hcc_test_subtype;

#define HCC_TEST_CMD_START_TX                      0
#define HCC_TEST_CMD_START_RX                      1
#define HCC_TEST_CMD_START_LOOP                    2
#define HCC_TEST_CMD_STOP_TEST                     3
#define HCC_TEST_CMD_CFG_FIX_FREQ                  4
#define HCC_TEST_CMD_PCIE_MAC_LOOPBACK_TST         5
#define HCC_TEST_CMD_PCIE_PHY_LOOPBACK_TST         6 /* PHY内部回环 */
#define HCC_TEST_CMD_PCIE_REMOTE_PHY_LOOPBACK_TST  7 /* PHY外部环回，TX/RX需要短接 */

#define HCC_TEST_MAC_IDX        0
#define HCC_TEST_PHY_IDX        1
#define HCC_TEST_REMOTE_PHY_IDX 2

typedef struct _hcc_test_cmd_stru_ {
    oal_uint16 cmd_type;
    oal_uint16 cmd_len;
} hcc_test_cmd_stru;
#define hcc_get_test_cmd_data(base)     (((oal_uint8*)(base)) + OAL_SIZEOF(hcc_test_cmd_stru))
#define HCC_PCIE_RESPONSE_INITIAL_VALUE (-199)
typedef struct _hcc_pcie_test_request_stru_ {
    oal_uint64 request_flag;
    oal_int32 response_ret;
    oal_uint32 link_up_cost;
    oal_uint32 rw_test_cost;
    oal_uint32 total_test_cost;
    oal_uint32 linkup_timeout;
    oal_uint32 rw_test_count;
    oal_uint32 bias_cldo3;
    oal_uint32 bias_vptx;
    oal_uint32 bias_vph;
} hcc_pcie_test_request_stru;

typedef struct _hsdio_trans_test_info_ {
    oal_uint32 actual_tx_pkts;
    oal_uint64 total_h2d_trans_bytes; /* total bytes trans by from host to device */
    oal_uint64 total_d2h_trans_bytes; /* total bytes trans by from device to host */
} hsdio_trans_test_info;

typedef struct _hcc_trans_test_rx_info_ {
    oal_uint32 total_trans_pkts;
    oal_uint32 pkt_len;
    oal_uint8 pkt_value;
} hcc_trans_test_rx_info;

enum _hcc_descr_type_ {
    HCC_DESCR_ASSEM_RESET = 0,
    HCC_NETBUF_QUEUE_SWITCH = 1,
    HCC_DESCR_TYPE_BUTT
};

typedef oal_uint32 hcc_descr_type;
/*
 * This descr buff is reversed in device,
 * the callback function can't cost much time,
 * just for transfer sdio buff message
 */
struct hcc_descr_header {
    hcc_descr_type descr_type;
};

enum _hcc_netbuf_queue_type_ {
    HCC_NETBUF_NORMAL_QUEUE = 0, /* netbuf is shared with others */
    HCC_NETBUF_HIGH_QUEUE = 1,   /* netbuf is special for high pri */
    HCC_NETBUF_QUEUE_BUTT
};

typedef enum _hcc_netbuf_queue_type_ hcc_netbuf_queue_type;

typedef struct EXCEPTION_BCPU_DUMP_HEADER {
    oal_uint32 align_type;
    uintptr_t start_addr;
    oal_uint32 men_len;
} exception_bcpu_dump_header;

enum dump_h2d_cmd {
    READ_MEM = 0,
    READ_REG = 1,

    DUMP_H2D_BUTT,
};
enum dump_d2h_cmd {
    DATA = 0,

    DUMP_D2H_BUTT,
};

#endif
