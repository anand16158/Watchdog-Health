- Uses Linux watchdog framework
- DT-driven configuration
- Timer-based timeout handling
- Panic triggers reboot via kernel
- Suitable for routers, gateways, APs


The watchdog driver implements WDIOC_GETTIMEOUT,
WDIOC_SETTIMEOUT, and WDIOC_KEEPALIVE ioctl commands
for runtime control from user space.
