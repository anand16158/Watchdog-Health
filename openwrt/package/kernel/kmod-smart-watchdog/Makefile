include $(TOPDIR)/rules.mk
include $(INCLUDE_DIR)/kernel.mk

PKG_NAME:=smart-watchdog
PKG_RELEASE:=1

include $(INCLUDE_DIR)/package.mk

define KernelPackage/smart-watchdog
  SUBMENU:=Other modules
  TITLE:=Smart Watchdog Driver
  FILES:=$(PKG_BUILD_DIR)/watchdog_smart.ko
  AUTOLOAD:=$(call AutoLoad,40,watchdog_smart)
endef

define Build/Prepare
	$(call Build/Prepare/Default)
	$(CP) ./src/* $(PKG_BUILD_DIR)/
endef

define Build/Compile
	$(KERNEL_MAKE) M="$(PKG_BUILD_DIR)" modules
endef

$(eval $(call KernelPackage,smart-watchdog))
