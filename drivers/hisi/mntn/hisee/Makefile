ccflags-y += -Wall -Werror
EXTRA_CFLAGS += -Idrivers/hisi/tzdriver

ifeq ($(TARGET_VERSION_MODE),factory)
ccflags-y += -DFACTORY_TEST_HISEE
endif

obj-$(CONFIG_HISI_HISEE_MNTN) += hisee_mntn.o
