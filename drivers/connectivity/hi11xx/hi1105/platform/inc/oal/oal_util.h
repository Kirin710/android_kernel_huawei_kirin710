

#ifndef __OAL_UTIL_H__
#define __OAL_UTIL_H__

/* ����ͷ�ļ����� */
#include "oal_types.h"
#include "oal_mm.h"
#include "arch/oal_util.h"
#include "securec.h"
#include "platform_oneimage_define.h"

/* �궨�� */
#define OAL_VA_START va_start
#define OAL_VA_END   va_end

#define OAL_VA_LIST va_list

#define OAL_MAC_ADDRESS_LAN 6

#define OAL_COUNTRY_CODE_LAN 3
/* ƴ��Ϊ16 bit���� 32bit */
#define OAL_MAKE_WORD16(lsb, msb)           ((((oal_uint16)(msb) << 8) & 0xFF00) | (lsb))
#define OAL_MAKE_WORD32(lsw, msw)           ((((oal_uint32)(msw) << 16) & 0xFFFF0000) | (lsw))
#define OAL_JOIN_WORD32(lsb, ssb, asb, msb) (((oal_uint32)(msb) << 24) | ((oal_uint32)(asb) << 16) | \
                                             ((oal_uint32)(ssb) << 8) | (lsb))
#define OAL_JOIN_WORD20(lsw, msw)           ((((oal_uint32)(msw) << 10) & 0xFFC00) | ((lsw) & 0x3FF))

/* ����Ϊ�ֽڶ��������ĳ��� */
#define PADDING(x, size) (((x) + (size)-1) & (~((size)-1)))

/* increment with wrap-around */
#define OAL_INCR(_l, _sz)  \
    do {                   \
        (_l)++;            \
        (_l) &= ((_sz) - 1); \
    } while (0)

#define OAL_DECR(_l, _sz)  \
    do {                   \
        (_l)--;            \
        (_l) &= ((_sz) - 1); \
    } while (0)

/* ��ȡ��С */
#define OAL_SIZEOF sizeof

/* ��ȡ�����С */
#define OAL_ARRAY_SIZE(_ast_array) (sizeof(_ast_array) / sizeof((_ast_array)[0]))

/* ���ֽڶ��� */
#define OAL_GET_4BYTE_ALIGN_VALUE(_ul_size) (((_ul_size) + 0x03) & (~0x03))

/* ��ȡ��ǰ�߳���Ϣ */
#define OAL_CURRENT_TASK (current_thread_info()->task)

#define OAL_SWAP_BYTEORDER_16(_val) ((((_val)&0x00FF) << 8) + (((_val)&0xFF00) >> 8))

#if (_PRE_BIG_CPU_ENDIAN == _PRE_CPU_ENDIAN) /* BIG_ENDIAN */
#define OAL_BYTEORDER_TO_LE32(_val)     OAL_SWAP_BYTEORDER_32(_val)
#define OAL_BYTEORDER_TO_LE16(_val)     OAL_SWAP_BYTEORDER_16(_val)
#define OAL_MASK_INVERSE(_len, _offset) ((oal_uint32)(OAL_SWAP_BYTEORDER_32(~(((1 << (_len)) - 1) << (_offset)))))
#define OAL_MASK(_len, _offset)         ((oal_uint32)(OAL_SWAP_BYTEORDER_32(((1 << (_len)) - 1) << (_offset))))
#define OAL_NTOH_16(_val)               _val
#define OAL_NTOH_32(_val)               _val
#define OAL_HTON_16(_val)               _val
#define OAL_HTON_32(_val)               _val

#elif (_PRE_LITTLE_CPU_ENDIAN == _PRE_CPU_ENDIAN) /* LITTLE_ENDIAN */
#define OAL_BYTEORDER_TO_LE32(_val)     _val
#define OAL_BYTEORDER_TO_LE16(_val)     _val
#define OAL_MASK_INVERSE(_len, _offset) ((oal_uint32)(~(((1UL << (_len)) - 1) << (_offset))))
#define OAL_MASK(_len, _offset)         ((oal_uint32)(((1UL << (_len)) - 1) << (_offset)))
#define OAL_NTOH_16(_val)               OAL_SWAP_BYTEORDER_16(_val)
#define OAL_NTOH_32(_val)               OAL_SWAP_BYTEORDER_32(_val)
#define OAL_HTON_16(_val)               OAL_SWAP_BYTEORDER_16(_val)
#define OAL_HTON_32(_val)               OAL_SWAP_BYTEORDER_32(_val)
#endif

#define OAL_VALUE_EQ_ANY2(_value, _val0, _val1) \
    (((_val0) == (_value)) || ((_val1) == (_value)))
#define OAL_VALUE_EQ_ANY3(_value, _val0, _val1, _val2) \
    (((_val0) == (_value)) || ((_val1) == (_value)) || ((_val2) == (_value)))

#define OAL_VALUE_EQ_ANY4(_value, _val0, _val1, _val2, _val3) \
    (((_val0) == (_value)) || ((_val1) == (_value)) || ((_val2) == (_value)) || ((_val3) == (_value)))

#define OAL_VALUE_EQ_ALL2(_value, _val0, _val1) \
    (((_val0) == (_value)) && ((_val1) == (_value)))

#define OAL_VALUE_IN_VALID_RANGE(_value, _start, _end)       (((_value) >= (_start)) && ((_value) <=(_end)))
#define OAL_VALUE_NOT_IN_VALID_RANGE(_value, _start, _end)   (((_value) < (_start)) || ((_value) > (_end)))
#define OAL_ANY_TRUE_VALUE2(_val0, _val1)                    (((_val0) == OAL_TRUE) || ((_val1) == OAL_TRUE))
#define OAL_VALUE_NE_ALL2(value, _val0, _val1)               (((_val0) != (value)) && ((_val1) != (value)))
#define OAL_ANY_ZERO_VALUE2(_val0, _val1)                    (((_val0) == 0) || ((_val1) == 0))

#define OAL_ANY_NULL_PTR1(_ptr1)                             (((_ptr1) == NULL))
#define OAL_ANY_NULL_PTR2(_ptr1, _ptr2)                      (((_ptr1) == NULL) || ((_ptr2) == NULL))
#define OAL_ANY_NULL_PTR3(_ptr1, _ptr2, _ptr3)               (((_ptr1) == NULL) || ((_ptr2) == NULL) || \
                                                              ((_ptr3) == NULL))
#define OAL_ANY_NULL_PTR4(_ptr1, _ptr2, _ptr3, _ptr4)        (((_ptr1) == NULL) || ((_ptr2) == NULL) || \
                                                              ((_ptr3) == NULL) || ((_ptr4) == NULL))
#define OAL_ANY_NULL_PTR5(_ptr1, _ptr2, _ptr3, _ptr4, _ptr5) (((_ptr1) == NULL) || ((_ptr2) == NULL) || \
                                                              ((_ptr3) == NULL) || ((_ptr4) == NULL) || \
                                                              ((_ptr5) == NULL))
#define OAL_ALL_NOT_NULL_PTR2(_ptr1, _ptr2)                   (((_ptr1) != NULL) && ((_ptr2) != NULL))
#define OAL_ALL_TRUE_VALUE2(_val0, _val1)                     (((_val0) == OAL_TRUE) && ((_val1) == OAL_TRUE))

#define OAL_ALL_ZERO_VALUE2(_val0, _val1)                     (((_val0) == 0) && ((_val1) == 0))

#if (!defined(_PRE_PC_LINT) && !defined(WIN32))
#ifdef __GNUC__
#define OAL_BUILD_BUG_ON(_con) ((oal_void)sizeof(char[1 - 2 * !!(_con)]))
#else
#define OAL_BUILD_BUG_ON(_con)
#endif
#else
#define OAL_BUG_ON(_con)
#define OAL_BUILD_BUG_ON(_con)
#endif

#ifndef atomic_inc_return
#define oal_atomic_inc_return(a) 0
#else
#define oal_atomic_inc_return atomic_inc_return
#endif

/* �ȽϺ� */
#define OAL_MIN(_A, _B) (((_A) < (_B)) ? (_A) : (_B))

/* �ȽϺ� */
#define OAL_MAX(_A, _B) (((_A) > (_B)) ? (_A) : (_B))

#define OAL_SUB(_A, _B) (((_A) > (_B)) ? ((_A) - (_B)) : (0))

#define OAL_ABSOLUTE_SUB(_A, _B) (((_A) > (_B)) ? ((_A) - (_B)) : ((_B) - (_A)))

/* ��ĳ���豸��ȡĳ���Ĵ�����ַ��32-bit�Ĵ�����ֵ�� */
#define OAL_REG_READ32(_addr) \
    *((OAL_VOLATILE oal_uint32 *)(_addr))

#define OAL_REG_READ16(_addr) \
    *((OAL_VOLATILE oal_uint16 *)(_addr))

/* ��ĳ���豸ĳ��32-bit�Ĵ�����ַд��ĳ��ֵ */
#define OAL_REG_WRITE32(_addr, _val) \
    (*((OAL_VOLATILE oal_uint32 *)(_addr)) = (_val))
#define OAL_REG_WRITE16(_addr, _val) \
    (*((OAL_VOLATILE oal_uint16 *)(_addr)) = (_val))

/* Is val aligned to "align" ("align" must be power of 2) */
#ifndef IS_ALIGNED
#define OAL_IS_ALIGNED(val, align) \
    (((oal_uint32)(val) & ((align) - 1)) == 0)
#else
#define OAL_IS_ALIGNED IS_ALIGNED
#endif

/* Bit Values */
#define BIT31 ((oal_uint32)(1UL << 31))
#define BIT30 ((oal_uint32)(1 << 30))
#define BIT29 ((oal_uint32)(1 << 29))
#define BIT28 ((oal_uint32)(1 << 28))
#define BIT27 ((oal_uint32)(1 << 27))
#define BIT26 ((oal_uint32)(1 << 26))
#define BIT25 ((oal_uint32)(1 << 25))
#define BIT24 ((oal_uint32)(1 << 24))
#define BIT23 ((oal_uint32)(1 << 23))
#define BIT22 ((oal_uint32)(1 << 22))
#define BIT21 ((oal_uint32)(1 << 21))
#define BIT20 ((oal_uint32)(1 << 20))
#define BIT19 ((oal_uint32)(1 << 19))
#define BIT18 ((oal_uint32)(1 << 18))
#define BIT17 ((oal_uint32)(1 << 17))
#define BIT16 ((oal_uint32)(1 << 16))
#define BIT15 ((oal_uint32)(1 << 15))
#define BIT14 ((oal_uint32)(1 << 14))
#define BIT13 ((oal_uint32)(1 << 13))
#define BIT12 ((oal_uint32)(1 << 12))
#define BIT11 ((oal_uint32)(1 << 11))
#define BIT10 ((oal_uint32)(1 << 10))
#define BIT9  ((oal_uint32)(1 << 9))
#define BIT8  ((oal_uint32)(1 << 8))
#define BIT7  ((oal_uint32)(1 << 7))
#define BIT6  ((oal_uint32)(1 << 6))
#define BIT5  ((oal_uint32)(1 << 5))
#define BIT4  ((oal_uint32)(1 << 4))
#define BIT3  ((oal_uint32)(1 << 3))
#define BIT2  ((oal_uint32)(1 << 2))
#define BIT1  ((oal_uint32)(1 << 1))
#define BIT0  ((oal_uint32)(1 << 0))
#define ALL   0xFFFF

#define BIT(nr) (1UL << (nr))

#define OAL_BITS_PER_BYTE 8 /* һ���ֽ��а�����bit��Ŀ */

/* λ���� */
#define OAL_SET_BIT(_val)                        (1 << (_val))
#define OAL_LEFT_SHIFT(_data, _num)              ((_data) << (_num))
#define OAL_RGHT_SHIFT(_data, _num)              ((_data) >> (_num))
#define OAL_WRITE_BITS(_data, _val, _bits, _pos) \
    do {                                                                    \
        (_data) &= ~((((oal_uint32)1 << (_bits)) - 1) << (_pos));           \
        (_data) |= (((_val) & (((oal_uint32)1 << (_bits)) - 1)) << (_pos)); \
    } while (0)
#define OAL_GET_BITS(_data, _bits, _pos) (((_data) >> (_pos)) & (((oal_uint32)1 << (_bits)) - 1))

/* λ������ */
#define NUM_1_BITS  1
#define NUM_2_BITS  2
#define NUM_3_BITS  3
#define NUM_4_BITS  4
#define NUM_5_BITS  5
#define NUM_6_BITS  6
#define NUM_7_BITS  7
#define NUM_8_BITS  8
#define NUM_9_BITS  9
#define NUM_10_BITS 10
#define NUM_11_BITS 11
#define NUM_12_BITS 12
#define NUM_13_BITS 13
#define NUM_14_BITS 14
#define NUM_15_BITS 15
#define NUM_16_BITS 16
#define NUM_17_BITS 17
#define NUM_18_BITS 18
#define NUM_19_BITS 19
#define NUM_20_BITS 20
#define NUM_21_BITS 21
#define NUM_22_BITS 22
#define NUM_23_BITS 23
#define NUM_24_BITS 24
#define NUM_25_BITS 25
#define NUM_26_BITS 26
#define NUM_27_BITS 27
#define NUM_28_BITS 28
#define NUM_29_BITS 29
#define NUM_30_BITS 30
#define NUM_31_BITS 31
#define NUM_32_BITS 32

/* λƫ�ƶ��� */
#define BIT_OFFSET_0  0
#define BIT_OFFSET_1  1
#define BIT_OFFSET_2  2
#define BIT_OFFSET_3  3
#define BIT_OFFSET_4  4
#define BIT_OFFSET_5  5
#define BIT_OFFSET_6  6
#define BIT_OFFSET_7  7
#define BIT_OFFSET_8  8
#define BIT_OFFSET_9  9
#define BIT_OFFSET_10 10
#define BIT_OFFSET_11 11
#define BIT_OFFSET_12 12
#define BIT_OFFSET_13 13
#define BIT_OFFSET_14 14
#define BIT_OFFSET_15 15
#define BIT_OFFSET_16 16
#define BIT_OFFSET_17 17
#define BIT_OFFSET_18 18
#define BIT_OFFSET_19 19
#define BIT_OFFSET_20 20
#define BIT_OFFSET_21 21
#define BIT_OFFSET_22 22
#define BIT_OFFSET_23 23
#define BIT_OFFSET_24 24
#define BIT_OFFSET_25 25
#define BIT_OFFSET_26 26
#define BIT_OFFSET_27 27
#define BIT_OFFSET_28 28
#define BIT_OFFSET_29 29
#define BIT_OFFSET_30 30
#define BIT_OFFSET_31 31

/* �������������룬��ȡ��������, fract_bitsΪС��λ�� */
#define _ROUND_POS(fix_num, fract_bits) ((oal_uint16)((fix_num) + (1u << ((oal_uint16)(fract_bits)-1))) >> \
                                         ((oal_uint16)(fract_bits)))
#define _ROUND_NEG(fix_num, fract_bits) (-_ROUND_POS(-(fix_num), (fract_bits)))
#define OAL_ROUND(fix_num, fract_bits)  ((fix_num) > 0 ? _ROUND_POS(fix_num, fract_bits) : \
                                                         _ROUND_NEG(fix_num, fract_bits))

#define OAL_RSSI_INIT_MARKER   0x320 /* RSSIƽ��ֵ��ʼֵ */
#define OAL_RSSI_MAX_DELTA     24    /* ��󲽳� 24/8 = 3 */
#define OAL_RSSI_FRACTION_BITS 3
#define OAL_RSSI_SIGNAL_MIN    (-103)   /* �ϱ�RSSI�±߽� */
#define OAL_RSSI_SIGNAL_MAX    5      /* �ϱ�RSSI�ϱ߽� */
#define OAL_SNR_INIT_VALUE     0x7F   /* SNR�ϱ��ĳ�ʼֵ */
#define OAL_RSSI_INIT_VALUE    (-128) /* RSSI�ĳ�ʼֵ */

#define OAL_IPV6_ADDR_LEN 16

/* STRUCT���� */
#define HI11XX_LOG_ERR     0
#define HI11XX_LOG_WARN    1
#define HI11XX_LOG_INFO    2
#define HI11XX_LOG_DBG     3
#define HI11XX_LOG_VERBOSE 4

#ifdef CONFIG_PRINTK
#include <linux/module.h>

#include "platform_oneimage_define.h"

#ifndef HI11XX_LOG_MODULE_NAME
#define HI11XX_LOG_MODULE_NAME "[HI11XX]"
extern oal_int32 hi11xx_loglevel;
#endif

#ifndef HI11XX_LOG_MODULE_NAME_VAR
#define HI11XX_LOG_MODULE_NAME_VAR hi11xx_loglevel
extern oal_int32 hi11xx_loglevel;
#else
static oal_int32 HI11XX_LOG_MODULE_NAME_VAR = HI11XX_LOG_INFO;
#if defined(PLATFORM_DEBUG_ENABLE) && (_PRE_OS_VERSION_LINUX == _PRE_OS_VERSION)
module_param(HI11XX_LOG_MODULE_NAME_VAR, int, S_IRUGO | S_IWUSR);
#endif
#endif

#define HI11XX_LOG_MODULE_NAME_NUMS_STR(num) #num

extern char *hi11xx_loglevel_format[];

#define oal_print_hi11xx_log(loglevel, fmt, arg...)                         \
    do {                                                                    \
        if (OAL_UNLIKELY(HI11XX_LOG_MODULE_NAME_VAR >= loglevel)) {         \
            printk("%s%s" fmt "[%s:%d]\n",                                  \
                hi11xx_loglevel_format[loglevel], HI11XX_LOG_MODULE_NAME, \
                ##arg, __FUNCTION__, __LINE__);                             \
        }                                                                   \
    } while (0)
#else
#define oal_print_hi11xx_log
#endif

#ifdef _PRE_CONFIG_HISI_PANIC_DUMP_SUPPORT
typedef struct _hwifi_panic_log_ hwifi_panic_log;
typedef oal_int32 (*hwifi_panic_log_cb)(oal_void *data, char *pst_buf, oal_int32 buf_len);
struct _hwifi_panic_log_ {
    struct list_head list;
    /* the log module name */
    char *name;
    hwifi_panic_log_cb cb;
    oal_void *data;
};
#define DECLARE_WIFI_PANIC_STRU(module_name, func) \
    hwifi_panic_log module_name = {                \
        .name = #module_name,                      \
        .cb = (hwifi_panic_log_cb)func,            \
    }
#endif

/* �������� */
#ifdef _PRE_CONFIG_HISI_PANIC_DUMP_SUPPORT
/*
 * �� �� ��  : hwifi_panic_log_register_etc
 * ��������  : Kernl Panic �Ļص�����ע��
 */
extern oal_void hwifi_panic_log_register_etc(hwifi_panic_log *log, void *data);
extern oal_void hwifi_panic_log_dump_etc(char *print_level);
#else
OAL_STATIC OAL_INLINE oal_void hwifi_panic_log_dump_etc(char *print_level)
{
}
#endif

OAL_STATIC OAL_INLINE oal_void oal_print_inject_check_stack(oal_void)
{
#if (_PRE_OS_VERSION_LINUX == _PRE_OS_VERSION)
    const oal_uint32 trinity_name_len = 50;
    char trinity_name[trinity_name_len];

    memset_s(trinity_name, sizeof(trinity_name), 0, sizeof(trinity_name));
    if (memcpy_s(trinity_name, sizeof(trinity_name), current->comm, OAL_STRLEN("trinity")) != EOK) {
        OAL_IO_PRINT("memcpy_s error, destlen=%u, srclen=%u\n ",
                     (oal_uint32)sizeof(trinity_name), (oal_uint32)OAL_STRLEN("trinity"));
        return;
    }

    if (unlikely(!memcmp((void *)"trinity", (void *)trinity_name, OAL_STRLEN("trinity")))) {
        /* Debug */
        WARN_ON(1);
    }
#endif
}

OAL_STATIC OAL_INLINE oal_uint8 oal_strtohex(const char *c_string)
{
    oal_uint8 uc_ret = 0;
    if (OAL_UNLIKELY(c_string == NULL)) {
        OAL_WARN_ON(1);
        return 0;
    }

    if (*c_string >= '0' && *c_string <= '9') {
        uc_ret = (oal_uint8)(*c_string - '0');
    } else if (*c_string >= 'A' && *c_string <= 'F') {
        uc_ret = (oal_uint8)(*c_string - 'A' + 10); /* ��10Ϊ�˱�֤ȡ����'A'~'F'����10~15 */
    } else if (*c_string >= 'a' && *c_string <= 'f') {
        uc_ret = (oal_uint8)(*c_string - 'a' + 10); /* ��10Ϊ�˱�֤ȡ����'a'~'f'����10~15 */
    }

    return uc_ret;
}

/*
 * �� �� ��  : oal_strtoaddr
 * ��������  : �ַ���תMAC��ַ
 * �������  : pc_param: MAC��ַ�ַ���, ��ʽ xx:xx:xx:xx:xx:xx  �ָ���֧��':'��'-'
 * �������  : puc_mac_addr: ת����16���ƺ��MAC��ַ
 */
OAL_STATIC OAL_INLINE oal_void oal_strtoaddr(const char *pc_param,
    oal_uint8 uc_param_len, oal_uint8 *puc_mac_addr, oal_uint8 uc_mac_addr_len)
{
    oal_uint8 uc_char_index;
    const oal_uint32 uc_mac_max_num = 12; /* ���ֽڱ����ǲ�����ȷ��mac��ַ:xx:xx:xx:xx:xx:xx���������ּ���ַ� */
    const char *pc_param_end = NULL;

    if (OAL_UNLIKELY((pc_param == NULL) || (puc_mac_addr == NULL))) {
        OAL_WARN_ON(1);
        return;
    }

    pc_param_end = pc_param + uc_param_len;

    /* ��ȡmac��ַ,16����ת�� */
    for (uc_char_index = 0; uc_char_index < uc_mac_max_num; uc_char_index++) {
        if (pc_param >= pc_param_end) {
            OAL_WARN_ON(1);
            return;
        }

        if ((*pc_param == ':') || (*pc_param == '-')) {
            pc_param++;
            if (uc_char_index != 0) {
                uc_char_index--;
            }

            continue;
        }
        if ((uc_char_index / 2) >= uc_mac_addr_len) {
            OAL_WARN_ON(1);
            return;
        }

        /* ���м����ǰ��ַ���ת���ɵ�mac��ַд��buff�xx:xx ֻ��Ӧ����mac��ַ */
        puc_mac_addr[uc_char_index / 2] =
            (oal_uint8)(puc_mac_addr[uc_char_index / 2] * 16 * (uc_char_index % 2) +
                        oal_strtohex(pc_param));
        pc_param++;
    }
}

/*
 * �� �� ��  : oal_strtoipv6
 * ��������  : �ַ���תipv6��ַ
 * �������  : pc_param: ipv6��ַ�ַ���, ��ʽ xx:xx:xx:xx:xx:xx:xx:xx:xx:xx:xx:xx:xx:xx:xx:xx
 * �������  : puc_mac_addr: ת����16���ƺ��ipv6��ַ
 */
OAL_STATIC OAL_INLINE oal_void oal_strtoipv6(const char *pc_param, unsigned char *puc_ipv6_addr)
{
    oal_uint8 uc_char_index;
    const oal_uint32 uc_ipv6_max_num = OAL_IPV6_ADDR_LEN * 2; /* ���ֽڱ����ǲ�����ȷ��ipv6��ַ */

    if (OAL_UNLIKELY((pc_param == NULL) || (puc_ipv6_addr == NULL))) {
        OAL_WARN_ON(1);
        return;
    }

    /* ��ȡipv6��ַ,16����ת�� */
    for (uc_char_index = 0; uc_char_index < uc_ipv6_max_num; uc_char_index++) {
        if ((*pc_param == ':')) {
            pc_param++;
            if (uc_char_index != 0) {
                uc_char_index--;
            }

            continue;
        }
        /* ��ipv6�ַ���ת��Ϊ16�������� */
        puc_ipv6_addr[uc_char_index >> 1] =
            (oal_uint8)(((puc_ipv6_addr[uc_char_index >> 1]) << 4) * (uc_char_index % 2) +
                        oal_strtohex(pc_param));
        pc_param++;
    }
}

OAL_STATIC OAL_INLINE oal_int oal_memcmp(OAL_CONST void *p_buf1, OAL_CONST void *p_buf2, oal_uint32 ul_count)
{
    return OAL_MEMCMP(p_buf1, p_buf2, ul_count);
}

OAL_STATIC OAL_INLINE oal_int oal_strncmp(const char *p_buf1, const char *p_buf2, oal_uint32 ul_count)
{
    return OAL_STRNCMP(p_buf1, p_buf2, ul_count);
}

OAL_STATIC OAL_INLINE oal_int oal_strncasecmp(OAL_CONST oal_int8 *p_buf1,
                                              OAL_CONST oal_int8 *p_buf2,
                                              oal_uint32 ul_count)
{
#if (_PRE_OS_VERSION_LINUX == _PRE_OS_VERSION)
    return OAL_STRNCASECMP(p_buf1, p_buf2, ul_count);
#else
    return OAL_STRNCMP(p_buf1, p_buf2, ul_count); /* windows still use strncmp */
#endif
}

/*
 * �� �� ��  : oal_get_random_bytes
 * ��������  : ��urandom�ļ���ȡ���ֵ
 * �������  : pc_random_buf :���ļ��������ݺ��ŵ�buf
 *             ul_random_len :��ȡ���ݴ�С(�ֽ���)
 */
OAL_STATIC OAL_INLINE void oal_get_random_bytes(oal_int8 *pc_random_buf, oal_uint32 ul_random_len)
{
#if (_PRE_OS_VERSION_LINUX == _PRE_OS_VERSION)
    if (OAL_UNLIKELY(pc_random_buf == NULL)) {
        OAL_WARN_ON(1);
        return;
    }
    get_random_bytes(pc_random_buf, ul_random_len);
#endif
}

/*
 * �� �� ��  : oal_gen_random
 * ��������  : ���������
 * �������  : ul_val:�������   us_rst_flag:0:������������ӣ���0:�����������
 */
OAL_STATIC OAL_INLINE oal_uint8 oal_gen_random(oal_uint32 ul_val, oal_uint8 us_rst_flag)
{
    OAL_STATIC oal_uint32 ul_rand = 0;
    if (us_rst_flag != 0) {
        ul_rand = ul_val;
    }
    ul_rand = ul_rand * 1664525L + 1013904223L;
    return (oal_uint8)(ul_rand >> 24);
}

/*
 * �� �� ��  : oal_bit_get_num_one_byte
 * ��������  : ��ȡ���ֽ��е�bit1�ĸ���
 * �������  : uc_byte:��Ҫ�������ֽ�
 * �� �� ֵ  : bit�ĸ���
 */
OAL_STATIC OAL_INLINE oal_uint8 oal_bit_get_num_one_byte(oal_uint8 uc_byte)
{
    uc_byte = (uc_byte & 0x55) + ((uc_byte >> 1) & 0x55);
    uc_byte = (uc_byte & 0x33) + ((uc_byte >> 2) & 0x33);
    uc_byte = (uc_byte & 0x0F) + ((uc_byte >> 4) & 0x0F);

    return uc_byte;
}

/*
 * �� �� ��  : oal_bit_get_num_four_byte
 * ��������  : ��ȡ4�ֽ���bit1�ĸ���
 * �������  : uc_byte:��Ҫ�������ֽ�
 * �� �� ֵ  : bit�ĸ���
 */
OAL_STATIC OAL_INLINE oal_uint32 oal_bit_get_num_four_byte(oal_uint32 ul_byte)
{
    ul_byte = (ul_byte & 0x55555555) + ((ul_byte >> 1) & 0x55555555);
    ul_byte = (ul_byte & 0x33333333) + ((ul_byte >> 2) & 0x33333333);
    ul_byte = (ul_byte & 0x0F0F0F0F) + ((ul_byte >> 4) & 0x0F0F0F0F);
    ul_byte = (ul_byte & 0x00FF00FF) + ((ul_byte >> 8) & 0x00FF00FF);
    ul_byte = (ul_byte & 0x0000FFFF) + ((ul_byte >> 16) & 0x0000FFFF);

    return ul_byte;
}

/*
 * �� �� ��  : oal_bit_set_bit_one_byte
 * ��������  : ��1�ֽڵ�ָ��λ��һ
 * �������  : puc_byte: Ҫ����λ�����ı�����ַ
 *             nr: ��λ��λ��
 */
OAL_STATIC OAL_INLINE oal_void oal_bit_set_bit_one_byte(oal_uint8 *puc_byte, oal_bitops nr)
{
    if (OAL_UNLIKELY(puc_byte == NULL)) {
        OAL_WARN_ON(1);
        return;
    }
    *puc_byte |= ((oal_uint8)(1 << nr));
}

/*
 * �� �� ��  : oal_bit_clear_bit_one_byte
 * ��������  : ��1�ֽڵ�ָ��λ����
 * �������  : puc_byte: Ҫ����λ�����ı�����ַ
 *             nr: �����λ��
 */
OAL_STATIC OAL_INLINE oal_void oal_bit_clear_bit_one_byte(oal_uint8 *puc_byte, oal_bitops nr)
{
    if (OAL_UNLIKELY(puc_byte == NULL)) {
        OAL_WARN_ON(1);
        return;
    }
    *puc_byte &= (~((oal_uint8)(1 << nr)));
}

OAL_STATIC OAL_INLINE oal_uint8 oal_bit_get_bit_one_byte(oal_uint8 uc_byte, oal_bitops nr)
{
    return ((uc_byte >> nr) & 0x1);
}

/*
 * �� �� ��  : oal_bit_set_bit_four_byte
 * ��������  : ��4�ֽڵ�ָ��λ��һ
 * �������  : pul_byte: Ҫ����λ�����ı�����ַ
 *            nr: ��λ��λ��
 */
OAL_STATIC OAL_INLINE oal_void oal_bit_set_bit_four_byte(oal_uint32 *pul_byte, oal_bitops nr)
{
    if (OAL_UNLIKELY(pul_byte == NULL)) {
        OAL_WARN_ON(1);
        return;
    }
    *pul_byte |= ((oal_uint32)(1 << nr));
}

/*
 * �� �� ��  : oal_bit_clear_bit_four_byte
 * ��������  : ��4�ֽڵ�ָ��λ����
 * �������  : pul_byte: Ҫ����λ�����ı�����ַ
 *             nr: �����λ��
 */
OAL_STATIC OAL_INLINE oal_void oal_bit_clear_bit_four_byte(oal_uint32 *pul_byte, oal_bitops nr)
{
    if (OAL_UNLIKELY(pul_byte == NULL)) {
        OAL_WARN_ON(1);
        return;
    }
    *pul_byte &= ~((oal_uint32)(1 << nr));
}

/*
 * �� �� ��  : oal_bit_set_bit_eight_byte
 * ��������  : ��8�ֽڵ�ָ��λ��һ
 */
OAL_STATIC OAL_INLINE oal_void oal_bit_set_bit_eight_byte(oal_uint64 *pull_byte, oal_bitops nr)
{
    if (OAL_UNLIKELY(pull_byte == NULL)) {
        OAL_WARN_ON(1);
        return;
    }
    *pull_byte |= ((oal_uint64)1 << nr);
}

/*
 * �� �� ��  : oal_bit_clear_bit_eight_byte
 * ��������  : ��8�ֽڵ�ָ��λ��0
 */
OAL_STATIC OAL_INLINE oal_void oal_bit_clear_bit_eight_byte(oal_uint64 *pull_byte, oal_bitops nr)
{
    if (OAL_UNLIKELY(pull_byte == NULL)) {
        OAL_WARN_ON(1);
        return;
    }
    *pull_byte &= ~((oal_uint64)1 << nr);
}

/*
 * �� �� ��  : oal_bit_find_first_bit_one_byte
 * ��������  : �ҵ�1�ֽ�������һ����1��λ��
 * �������  : uc_byte: Ҫ���ҵ��ֽ�
 * �� �� ֵ  : ������һ����1��λ��
 */
OAL_STATIC OAL_INLINE oal_uint8 oal_bit_find_first_bit_one_byte(oal_uint8 uc_byte)
{
    oal_uint8 uc_ret = 0;

    uc_byte = uc_byte & (oal_uint8)(-uc_byte);

    while (uc_byte != 1) {
        uc_ret++;
        uc_byte = (uc_byte >> 1);

        if (uc_ret > 7) { /* ��ʱ��˵���Ѿ��������bit�� */
            return uc_ret;
        }
    }

    return uc_ret;
}

/*
 * �� �� ��  : oal_bit_find_first_zero_one_byte
 * ��������  : �ҵ�1�ֽ�������һ����0��λ��
 * �������  : uc_byte: Ҫ���ҵ��ֽ�
 * �� �� ֵ  : ������һ����0��λ��
 */
OAL_STATIC OAL_INLINE oal_uint8 oal_bit_find_first_zero_one_byte(oal_uint8 uc_byte)
{
    oal_uint8 uc_ret = 0;

    uc_byte = ~uc_byte;
    uc_byte = uc_byte & (oal_uint8)(-uc_byte);

    while (uc_byte != 1) {
        uc_ret++;
        uc_byte = (uc_byte >> 1);

        if (uc_ret > 7) { /* ��ʱ��˵���Ѿ��������bit�� */
            return uc_ret;
        }
    }

    return uc_ret;
}

/*
 * �� �� ��  : oal_bit_find_first_bit_four_byte
 * ��������  : �ҵ�1�ֽ�������һ����1��λ��
 * �������  : puc_byte: Ҫ���ҵ��ֽ�
 * �� �� ֵ  : ������һ����1��λ��
 */
OAL_STATIC OAL_INLINE oal_uint8 oal_bit_find_first_bit_four_byte(oal_uint32 ul_byte)
{
    oal_uint8 uc_ret = 0;

    if (ul_byte == 0) {
        return uc_ret;
    }

    if (!(ul_byte & 0xffff)) {
        ul_byte >>= 16; /* ˵��32bit���ݵĵ�16bitȫΪ0�����Կ���ֱ�ӴӸ�16bit��ʼ��Ϊ1��λ�� */
        uc_ret += 16;
    }

    if (!(ul_byte & 0xff)) {
        ul_byte >>= 8; /* ˵��16bit���ݵĵ�8bitȫΪ0�����Կ���ֱ�ӴӸ�8bit��ʼ��Ϊ1��λ�� */
        uc_ret += 8;
    }

    if (!(ul_byte & 0xf)) {
        ul_byte >>= 4; /* ˵��8bit���ݵĵ�4bitȫΪ0�����Կ���ֱ�ӴӸ�4bit��ʼ��Ϊ1��λ�� */
        uc_ret += 4;
    }

    if (!(ul_byte & 3)) {
        ul_byte >>= 2; /* ˵��4bit���ݵĵ�2bitȫΪ0�����Կ���ֱ�ӴӸ�2bit��ʼ��Ϊ1��λ�� */
        uc_ret += 2;
    }

    if (!(ul_byte & 1)) {
        uc_ret += 1;
    }

    return uc_ret;
}

/*
 * �� �� ��  : oal_bit_find_first_zero_four_byte
 * ��������  : �ҵ�1�ֽ�������һ����0��λ��
 * �������  : puc_byte: Ҫ���ҵ��ֽ�
 * �� �� ֵ  : ������һ����0��λ��
 */
OAL_STATIC OAL_INLINE oal_uint8 oal_bit_find_first_zero_four_byte(oal_uint32 ul_byte)
{
    oal_uint8 uc_ret = 0;

    ul_byte = ~ul_byte;

    if (!(ul_byte & 0xffff)) {
        ul_byte >>= 16; /* ˵��32bit���ݵĵ�16bitȫΪ1�����Կ���ֱ�ӴӸ�16bit��ʼ��Ϊ0��λ�� */
        uc_ret += 16;
    }

    if (!(ul_byte & 0xff)) {
        ul_byte >>= 8; /* ˵��16bit���ݵĵ�8bitȫΪ1�����Կ���ֱ�ӴӸ�8bit��ʼ��Ϊ0��λ�� */
        uc_ret += 8;
    }

    if (!(ul_byte & 0xf)) {
        ul_byte >>= 4; /* ˵��8bit���ݵĵ�4bitȫΪ1�����Կ���ֱ�ӴӸ�4bit��ʼ��Ϊ0��λ�� */
        uc_ret += 4;
    }

    if (!(ul_byte & 3)) {
        ul_byte >>= 2; /* ˵��4bit���ݵĵ�2bitȫΪ1�����Կ���ֱ�ӴӸ�2bit��ʼ��Ϊ0��λ�� */
        uc_ret += 2;
    }

    if (!(ul_byte & 1)) {
        uc_ret += 1;
    }

    return uc_ret;
}

#define OAL_COMPARE_MAC_ADDR(puc_mac_addr1, puc_mac_addr2)  \
    (oal_uint32)oal_memcmp((void *)(puc_mac_addr1), (void *)(puc_mac_addr2), OAL_MAC_ADDRESS_LAN)

/*
 * �� �� ��  : oal_set_mac_addr
 * ��������  : ���ڶ���mac��ַ ��ֵ����һ��mac��ַ
 * �������  : puc_mac_addr1: ��һ��mac��ַ
 *             puc_mac_addr2: �ڶ���mac��ַ
 */
OAL_STATIC OAL_INLINE oal_void oal_set_mac_addr(unsigned char *puc_mac_addr1, const unsigned char *puc_mac_addr2)
{
    if (memcpy_s(puc_mac_addr1, OAL_MAC_ADDRESS_LAN, puc_mac_addr2, OAL_MAC_ADDRESS_LAN) != EOK) {
        OAL_IO_PRINT("oal_set_mac_addr: memcpy_s failed.\n");
    }
}

OAL_STATIC OAL_INLINE oal_void oal_set_mac_addr_zero(unsigned char *puc_mac_addr)
{
    memset_s(puc_mac_addr, OAL_MAC_ADDRESS_LAN, 0, OAL_MAC_ADDRESS_LAN);
}

OAL_STATIC OAL_INLINE oal_uint32 oal_compare_mac_addr(const unsigned char *puc_mac_addr1,
                                                      const unsigned char *puc_mac_addr2)
{
    return (oal_uint32)oal_memcmp((void *)puc_mac_addr1, (void *)puc_mac_addr2, OAL_MAC_ADDRESS_LAN);
}
OAL_STATIC OAL_INLINE oal_uint32 oal_compare_country_code(const unsigned char *puc_country_code1,
                                                          const unsigned char *puc_country_code2)
{
    return (oal_uint32)oal_memcmp((void *)puc_country_code1, (void *)puc_country_code2, OAL_COUNTRY_CODE_LAN);
}
/*
 * �� �� ��  : oal_cmp_seq_num
 * ��������  : �Ƚ������������������ж�˳��ŵĴ�С��seq_num1����seq_num2������
 */
OAL_STATIC OAL_INLINE oal_bool_enum_uint8 oal_cmp_seq_num(oal_uint32 ul_seq_num1,
                                                          oal_uint32 ul_seq_num2,
                                                          oal_uint32 ul_diff_value)
{
    if (((ul_seq_num1 < ul_seq_num2) && ((ul_seq_num2 - ul_seq_num1) < ul_diff_value)) ||
        ((ul_seq_num1 > ul_seq_num2) && ((ul_seq_num1 - ul_seq_num2) > ul_diff_value))) {
        return OAL_TRUE;
    }

    return OAL_FALSE;
}

OAL_STATIC OAL_INLINE oal_int32 oal_strcmp(const char *pc_src, const char *pc_dst)
{
    oal_int8 c_c1;
    oal_int8 c_c2;
    oal_int32 l_ret = 0;

    if (OAL_UNLIKELY((pc_src == NULL) || (pc_dst == NULL))) {
        OAL_WARN_ON(1);
        return 1;
    }

    do {
        c_c1 = *pc_src++;
        c_c2 = *pc_dst++;
        l_ret = c_c1 - c_c2;
        if (l_ret) {
            break;
        }
    } while (c_c1);

    return l_ret;
}

/*
 * �� �� ��  : oal_strim
 * ��������  : ȥ���ַ�����ʼ���β�Ŀո�
 */
OAL_STATIC OAL_INLINE oal_int8 *oal_strim(oal_int8 *pc_s)
{
    oal_uint32 ul_size;
    oal_int8 *pc_end = NULL;

    if (OAL_UNLIKELY(pc_s == NULL)) {
        OAL_WARN_ON(1);
        return NULL;
    }

    while (*pc_s == ' ') {
        ++pc_s;
    }

    ul_size = OAL_STRLEN(pc_s);
    if (!ul_size) {
        return pc_s;
    }

    pc_end = pc_s + ul_size - 1;
    while (pc_end >= pc_s && *pc_end == ' ') {
        pc_end--;
    }

    *(pc_end + 1) = '\0';

    return pc_s;
}

OAL_STATIC OAL_INLINE oal_int8 *oal_strstr(oal_int8 *pc_s1, oal_int8 *pc_s2)
{
    return OAL_STRSTR(pc_s1, pc_s2);
}

/*
 * �� �� ��  : oal_init_lut
 * ��������  : ��ʼ�������LUT BITMAP��
 */
OAL_STATIC OAL_INLINE oal_uint32 oal_init_lut(oal_uint8 *puc_lut_index_table, oal_uint8 uc_bmap_len)
{
    oal_uint8 uc_lut_idx;

    if (OAL_UNLIKELY(puc_lut_index_table == NULL)) {
        OAL_WARN_ON(1);
        return OAL_FAIL;
    }

    for (uc_lut_idx = 0; uc_lut_idx < uc_bmap_len; uc_lut_idx++) {
        puc_lut_index_table[uc_lut_idx] = 0;
    }

    return OAL_SUCC;
}

/*
 * �� �� ��  : oal_get_lut_index
 * ��������  : ��LUT index bitmap���У���ȡһ��û�б�ʹ�õ�������û���ҵ��Ļ���
 *             ���ز����õ�������ʶ(�ǹؼ�·����δ�����Ż�����ʱ������Ż�)
 */
OAL_STATIC OAL_INLINE oal_uint8 oal_get_lut_index(oal_uint8 *puc_lut_index_table,
                                                  oal_uint8 uc_bmap_len,
                                                  oal_uint16 us_max_lut_size,
                                                  oal_uint16 us_start,
                                                  oal_uint16 us_stop)
{
    oal_uint8 uc_byte = 0;
    oal_uint8 uc_bit = 0;
    oal_uint8 uc_temp = 0;
    oal_uint16 us_index = 0;

    if (OAL_UNLIKELY(puc_lut_index_table == NULL)) {
        OAL_WARN_ON(1);
        return 0;
    }

    for (uc_byte = 0; uc_byte < uc_bmap_len; uc_byte++) {
        uc_temp = puc_lut_index_table[uc_byte];

        for (uc_bit = 0; uc_bit < 8; uc_bit++) { /* ��һ��bitΪ��λ����ĳһλΪ0 */
            if ((uc_temp & (1 << uc_bit)) == 0x0) {
                us_index = (uc_byte * 8 + uc_bit); /* ���Ϊ0��bitλ��λ�� */
                if ((us_index < us_start) || (us_index >= us_stop)) {
                    continue;
                }
                if (us_index < us_max_lut_size) {
                    puc_lut_index_table[uc_byte] |= (oal_uint8)(1 << uc_bit);

                    return (oal_uint8)us_index;
                } else {
                    return (oal_uint8)us_max_lut_size;
                }
            }
        }
    }

    return (oal_uint8)us_max_lut_size;
}

/*
 * �� �� ��  : oal_del_lut_index
 * ��������  : ��LUT index bitmap���У�ɾ��һ��LUT index (ע:%������Ϊ�Ż���)
 */
OAL_STATIC OAL_INLINE oal_void oal_del_lut_index(oal_uint8 *puc_lut_index_table, oal_uint8 uc_idx)
{
    oal_uint8 uc_byte = uc_idx >> 3;
    oal_uint8 uc_bit = uc_idx & 0x07;

    if (OAL_UNLIKELY(puc_lut_index_table == NULL)) {
        OAL_WARN_ON(1);
        return;
    }

    puc_lut_index_table[uc_byte] &= ~(oal_uint8)(1 << uc_bit);
}

OAL_STATIC OAL_INLINE oal_bool_enum oal_is_active_lut_index(oal_uint8 *puc_lut_idx_status_table,
                                                            oal_uint16 us_max_lut_size, oal_uint8 uc_idx)
{
    oal_uint8 uc_byte = uc_idx >> 3;
    oal_uint8 uc_bit = uc_idx & 0x07;

    if (OAL_UNLIKELY(puc_lut_idx_status_table == NULL)) {
        OAL_WARN_ON(1);
        return OAL_FALSE;
    }

    if (uc_idx >= us_max_lut_size) {
        return OAL_FALSE;
    }

    return puc_lut_idx_status_table[uc_byte] & ((oal_uint8)(1 << uc_bit)) ? OAL_TRUE : OAL_FALSE;
}

OAL_STATIC OAL_INLINE oal_void oal_set_lut_index_status(oal_uint8 *puc_lut_idx_status_table,
                                                        oal_uint16 us_max_lut_size, oal_uint8 uc_idx)
{
    oal_uint8 uc_byte = uc_idx >> 3;
    oal_uint8 uc_bit = uc_idx & 0x07;

    if (OAL_UNLIKELY(puc_lut_idx_status_table == NULL)) {
        OAL_WARN_ON(1);
        return;
    }

    if (uc_idx >= us_max_lut_size) {
        return;
    }

    puc_lut_idx_status_table[uc_byte] |= (oal_uint8)(1 << uc_bit);
}

OAL_STATIC OAL_INLINE oal_void oal_reset_lut_index_status(oal_uint8 *puc_lut_idx_status_table,
                                                          oal_uint16 us_max_lut_size, oal_uint8 uc_idx)
{
    oal_uint8 uc_byte = uc_idx >> 3;
    oal_uint8 uc_bit = uc_idx & 0x07;

    if (OAL_UNLIKELY(puc_lut_idx_status_table == NULL)) {
        OAL_WARN_ON(1);
        return;
    }

    if (uc_idx >= us_max_lut_size) {
        return;
    }

    puc_lut_idx_status_table[uc_byte] &= ~(oal_uint8)(1 << uc_bit);
}

/*
 * �� �� ��  : oal_get_virt_addr
 * ��������  : ������ַת��Ϊ�����ַ
 */
OAL_STATIC OAL_INLINE oal_uint32 *oal_get_virt_addr(oal_uint32 *pul_phy_addr)
{
    /* ��ָ������ת�� */
    if (pul_phy_addr == OAL_PTR_NULL) {
        return pul_phy_addr;
    }

    return (oal_uint32 *)OAL_PHY_TO_VIRT_ADDR((uintptr_t)pul_phy_addr);
}

extern oal_int32 oal_dump_stack_str(oal_uint8 *puc_str, oal_uint32 ul_max_size);

OAL_STATIC OAL_INLINE oal_int8 oal_get_real_rssi(oal_int16 s_scaled_rssi)
{
    /* �������� */
    return OAL_ROUND(s_scaled_rssi, OAL_RSSI_FRACTION_BITS);
}

OAL_STATIC OAL_INLINE oal_void oal_rssi_smooth(oal_int16 *ps_old_rssi, oal_int8 c_new_rssi)
{
    oal_int16 s_delta;

    if (OAL_UNLIKELY(ps_old_rssi == NULL)) {
        OAL_WARN_ON(1);
        return;
    }

    /* ���ϱ���ֵ�����˺Ϸ���Χ���򲻽���ƽ������������ֱ�ӷ��� */
    if (c_new_rssi < OAL_RSSI_SIGNAL_MIN || c_new_rssi > OAL_RSSI_SIGNAL_MAX) {
        return;
    }

    /* ���ϱ���ֵΪ0������������δ��rssiֵ,������ƽ�� */
    if (c_new_rssi == 0) {
        return;
    }

    /* �ж�Ϊ��ʼֵ,оƬ�ϱ���rssi��Ϊƽ����� */
    if (*ps_old_rssi == OAL_RSSI_INIT_MARKER) {
        *ps_old_rssi = (oal_int16)c_new_rssi << OAL_RSSI_FRACTION_BITS;
    }

    /* old_rssi������������delta */
    s_delta = (oal_int16)c_new_rssi - oal_get_real_rssi(*ps_old_rssi);

    if (s_delta > OAL_RSSI_MAX_DELTA) {
        s_delta = OAL_RSSI_MAX_DELTA;
    }
    if (s_delta < -OAL_RSSI_MAX_DELTA) {
        s_delta = -OAL_RSSI_MAX_DELTA;
    }
    *ps_old_rssi += s_delta;
}

#endif /* end of oal_util.h */