ccflags-y += -Wall -Werror
EXTRA_CFLAGS += -Idrivers/hisi/tzdriver

obj-y += rdr_hisi_ap_adapter.o rdr_hisi_ap_ringbuffer.o rdr_hisi_ap_hook.o rdr_hisi_ap_mntn.o
obj-y += rdr_hisi_ap_subtype.o rdr_hisi_ap_cleartext.o hisi_pstore.o rdr_hisi_ap_exception_logsave.o
obj-$(CONFIG_HISI_BB_DEBUG) += rdr_hisi_ap_test.o
