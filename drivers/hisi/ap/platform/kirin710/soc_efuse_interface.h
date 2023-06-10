#ifndef __SOC_EFUSE_INTERFACE_H__
#define __SOC_EFUSE_INTERFACE_H__ 
#ifdef __cplusplus
    #if __cplusplus
        extern "C" {
    #endif
#endif
#define EFUSE_ATTR_READABLE (0x00000001UL)
#define EFUSE_ATTR_READ_DEVICE (0x00000002UL)
#define EFUSE_ATTR_READ_SRAM (0x00000004UL)
#define EFUSE_ATTR_READ_ZERO (0x00000008UL)
#define EFUSE_ATTR_WRITEABLE (0x00000100UL)
#define EFUSE_ATTR_WRITE_NOT_UPDATE (0x00000200UL)
#define EFUSE_ATTR_WRITE_CHECK (0x00000400UL)
typedef struct {
 unsigned int start_bit;
 unsigned int bit_cnt;
 unsigned int id;
 unsigned int attr;
} efuse_attr_t;
enum {
 DIEID_VALUE = 0,
 AUTH_KEY = 1,
};
#define EFUSE_DIEID_VALUE_START 1024
#define EFUSE_DIEID_VALUE_SIZE 160
#define EFUSE_INSE_FLAG_START 1414
#define EFUSE_INSE_FLAG_SIZE 2
#define EFUSE_AUTH_KEY_START 1888
#define EFUSE_AUTH_KEY_SIZE 64
#define EFUSE_MAX_SIZE 2048
#define EFUSE_ITEM_MAX 160
#ifdef __cplusplus
    #if __cplusplus
        }
    #endif
#endif
#endif
