EXTRA_CFLAGS += -DENV_ARMLINUX_KERNEL
EXTRA_CFLAGS += -DHIVDEC_SMMU_SUPPORT

EXTRA_CFLAGS += -Idrivers/vcodec/hi_vcodec/vdec_hivna/include
EXTRA_CFLAGS += -Idrivers/vcodec/hi_vcodec/vdec_hivna/vdec/common
EXTRA_CFLAGS += -Idrivers/vcodec/hi_vcodec/vdec_hivna/vdec/v1.0
EXTRA_CFLAGS += -Idrivers/vcodec/hi_vcodec/vdec_hivna/vfmw/v4.0
EXTRA_CFLAGS += -Idrivers/vcodec/hi_vcodec/vdec_hivna/vfmw/v4.0/firmware/osal/linux_kernel
EXTRA_CFLAGS += -Idrivers/vcodec/hi_vcodec/vdec_hivna/vfmw/v4.0/firmware/osal
EXTRA_CFLAGS += -Idrivers/vcodec/hi_vcodec/vdec_hivna/vfmw/v4.0/firmware/common
EXTRA_CFLAGS += -Idrivers/vcodec/hi_vcodec/vdec_hivna/vfmw/v4.0/scene/kirin
EXTRA_CFLAGS += -fno-pic

ifneq ($(TARGET_BUILD_VARIANT), user)
EXTRA_CFLAGS += -DUSER_DISABLE_VDEC_PROC
endif

ifneq ($(CONFIG_VCODEC_PLATFORM_HIVCODECV500), y)
EXTRA_CFLAGS += -Idrivers/vcodec/hi_vcodec/vdec_hivna/vfmw/v4.0/firmware/hal/HiSCDV300
EXTRA_CFLAGS += -Idrivers/vcodec/hi_vcodec/vdec_hivna/vfmw/v4.0/firmware/product/HiVCodecV200
EXTRA_CFLAGS += -Idrivers/vcodec/hi_vcodec/vdec_hivna/vfmw/v4.0/firmware/hal/HiVDHV500R002
else
EXTRA_CFLAGS += -Idrivers/vcodec/hi_vcodec/vdec_hivna/vfmw/v4.0/firmware/hal/HiSCDV310
EXTRA_CFLAGS += -Idrivers/vcodec/hi_vcodec/vdec_hivna/vfmw/v4.0/firmware/product/HiVCodecV500
EXTRA_CFLAGS += -Idrivers/vcodec/hi_vcodec/vdec_hivna/vfmw/v4.0/firmware/hal/HiVDHV500R003
endif

ifeq ($(TARGET_BOARD_PLATFORM), kirin970)
EXTRA_CFLAGS +=-DPLATFORM_KIRIN970
endif

ifeq ($(CONFIG_VCODEC_PLATFORM_HIVCODECV300), y)
EXTRA_CFLAGS += -DPLATFORM_HIVCODECV300
EXTRA_CFLAGS += -DVDEC_AVS_LOW_CFG
endif

ifeq ($(CONFIG_VCODEC_PLATFORM_HIVCODECV500), y)
EXTRA_CFLAGS += -DPLATFORM_HIVCODECV500
EXTRA_CFLAGS += -DLOWER_FREQUENCY_SUPPORT
endif

ifeq ($(CONFIG_VCODEC_PLATFORM_HIVCODECV210), y)
EXTRA_CFLAGS += -DPLATFORM_HIVCODECV210
endif

ifeq ($(CONFIG_VCODEC_PLATFORM_HIVCODECV310), y)
EXTRA_CFLAGS += -DPLATFORM_HIVCODECV310
EXTRA_CFLAGS += -DLOWER_FREQUENCY_SUPPORT
endif


ifeq ($(es_low_freq),true)
EXTRA_CFLAGS += -DCONFIG_ES_VDEC_LOW_FREQ
endif

#build in
obj-$(CONFIG_HI_VCODEC_VDEC_HIVNA) += hi_omxvdec.o
hi_omxvdec-objs += omxvdec.o ../common/regulator.o ../common/memory.o

