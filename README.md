# Smart Watchdog Health Framework

A Device Tree–enabled Linux watchdog driver with a user-space
health monitoring daemon for OpenWrt-based embedded platforms.

## Features
- Platform watchdog driver
- Linux watchdog framework
- Panic + automatic reboot
- Device Tree configuration
- User-space health daemon

## Architecture
DTS → Kernel Watchdog → /dev/watchdog → Health Daemon → System Recovery

## Build Steps

### Add DTS overlay
cp dts/mt7981-smart-watchdog.dtso \
 target/linux/mediatek/files-5.15/arch/arm64/boot/dts/mediatek/

### Build kernel
make target/linux/compile V=s

### Build kmod
make package/kernel/kmod-smart-watchdog/compile V=s

### Install on board
opkg install kmod-smart-watchdog*.ipk

### Run daemon
gcc userspace/watchdog_healthd.c -o watchdog_healthd
./watchdog_healthd
