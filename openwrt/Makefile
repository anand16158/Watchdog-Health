define Package/watchdog-healthd
  SECTION:=utils
  CATEGORY:=Utilities
  TITLE:=Watchdog Health Daemon
endef

define Package/watchdog-healthd/install
	$(INSTALL_DIR) $(1)/usr/sbin
	$(INSTALL_BIN) ./files/watchdog_healthd $(1)/usr/sbin/

	$(INSTALL_DIR) $(1)/etc/init.d
	$(INSTALL_BIN) ./files/watchdog_healthd.init $(1)/etc/init.d/watchdog_healthd
endef

$(eval $(call BuildPackage,watchdog-healthd))
