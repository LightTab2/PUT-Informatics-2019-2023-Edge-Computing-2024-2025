DRIVOR_VERSION = 1.0
DRIVOR_SITE = $(BR2_EXTERNAL_LABY_PATH)/package/drivor
DRIVOR_SITE_METHOD = local

define KERNEL_MODULE_BUILD_CMDS
        $(MAKE) -C '$(@D)' LINUX_DIR='$(LINUX_DIR)' CC='$(TARGET_CC)' LD='$(TARGET_LD)' modules
endef

$(eval $(kernel-module))
$(eval $(generic-package))
