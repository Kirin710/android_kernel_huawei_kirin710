obj-y += rdr_hisi_audio_adapter.o
obj-y += rdr_hisi_audio_codec.o
obj-y += rdr_hisi_audio_soc.o

ifeq ($(TARGET_VERSION_MODE),factory)
EXTRA_CFLAGS += -DAUDIO_FACTORY_MODE
endif

ifeq ($(CONFIG_HIFI_EXTRA_MODEM_MODE),y)
obj-y += rdr_audio_notify_modem.o
EXTRA_CFLAGS += -DHISI_EXTERNAL_MODEM
endif

EXTRA_CFLAGS += -Idrivers/hisi/hifi_dsp/
EXTRA_CFLAGS += -Idrivers/hisi/hi64xx_dsp/
EXTRA_CFLAGS += -Idrivers/hisi/hi64xx_dsp/om/
EXTRA_CFLAGS += -Idrivers/hisi/mntn/blackbox/
EXTRA_CFLAGS += -Isound/usb/hifi/

ccflags-y += -I$(srctree)/drivers/hisi/tzdriver

EXTRA_CFLAGS += -DSECOS_RELOAD_HIFI
EXTRA_CFLAGS += -DRDR_SOCHIFI
EXTRA_CFLAGS += -DRDR_CODECDSP

ccflags-y += -Idrivers/devkit/audiokit/
