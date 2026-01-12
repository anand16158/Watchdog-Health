// SPDX-License-Identifier: GPL-2.0
/*
 * Smart Watchdog Driver
 *
 * - Device Tree enabled
 * - Uses Linux watchdog framework
 * - Panic + reboot on timeout
 * - Controlled via /dev/watchdog
 */

#include <linux/module.h>
#include <linux/platform_device.h>
#include <linux/of.h>
#include <linux/watchdog.h>
#include <linux/timer.h>
#include <linux/reboot.h>

#define DRV_NAME "smart_watchdog"

struct smart_wdt {
	struct watchdog_device wdd;
	struct timer_list timer;
	unsigned int timeout;
};

static struct smart_wdt swdt;

/* ---------- TIMEOUT HANDLER ---------- */
static void smart_wdt_expire(struct timer_list *t)
{
	pr_emerg("smart_watchdog: timeout expired, system panic\n");
	panic("Smart Watchdog Timeout");
}

/* ---------- WATCHDOG OPS ---------- */
static int smart_wdt_start(struct watchdog_device *wdd)
{
	mod_timer(&swdt.timer, jiffies + swdt.timeout * HZ);
	return 0;
}

static int smart_wdt_stop(struct watchdog_device *wdd)
{
	del_timer_sync(&swdt.timer);
	return 0;
}

static int smart_wdt_ping(struct watchdog_device *wdd)
{
	mod_timer(&swdt.timer, jiffies + swdt.timeout * HZ);
	return 0;
}

static const struct watchdog_ops smart_wdt_ops = {
	.owner = THIS_MODULE,
	.start = smart_wdt_start,
	.stop  = smart_wdt_stop,
	.ping  = smart_wdt_ping,
	.ioctl = smart_wdt_ioctl,
};

static const struct watchdog_info smart_wdt_info = {
	.identity = "Smart Watchdog",
	.options  = WDIOF_KEEPALIVEPING | WDIOF_MAGICCLOSE,
};

/* ---------- PROBE ---------- */
static int smart_wdt_probe(struct platform_device *pdev)
{
	struct device *dev = &pdev->dev;
	u32 timeout;

	if (of_property_read_u32(dev->of_node, "timeout-sec", &timeout))
		timeout = 10;

	swdt.timeout = timeout;

	timer_setup(&swdt.timer, smart_wdt_expire, 0);

	swdt.wdd.info = &smart_wdt_info;
	swdt.wdd.ops  = &smart_wdt_ops;
	swdt.wdd.timeout = timeout;
	swdt.wdd.parent = dev;

	watchdog_set_nowayout(&swdt.wdd, true);
	watchdog_init_timeout(&swdt.wdd, timeout, dev);

	watchdog_register_device(&swdt.wdd);

	dev_info(dev, "Smart Watchdog registered (timeout=%u sec)\n", timeout);
	return 0;
}

/* ---------- REMOVE ---------- */
static int smart_wdt_remove(struct platform_device *pdev)
{
	watchdog_unregister_device(&swdt.wdd);
	del_timer_sync(&swdt.timer);
	return 0;
}

/* ---------- DT MATCH ---------- */
static const struct of_device_id smart_wdt_of_match[] = {
	{ .compatible = "innovate,smart-watchdog" },
	{ }
};
MODULE_DEVICE_TABLE(of, smart_wdt_of_match);

/* ---------- PLATFORM DRIVER ---------- */
static struct platform_driver smart_wdt_driver = {
	.probe  = smart_wdt_probe,
	.remove = smart_wdt_remove,
	.driver = {
		.name = DRV_NAME,
		.of_match_table = smart_wdt_of_match,
	},
};
static int smart_wdt_ioctl(struct watchdog_device *wdd,
			   unsigned int cmd, unsigned long arg)
{
	int timeout;

	switch (cmd) {
	case WDIOC_GETTIMEOUT:
		return put_user(wdd->timeout, (int __user *)arg);

	case WDIOC_SETTIMEOUT:
		if (get_user(timeout, (int __user *)arg))
			return -EFAULT;

		if (timeout <= 0)
			return -EINVAL;

		wdd->timeout = timeout;
		swdt.timeout = timeout;
		mod_timer(&swdt.timer, jiffies + timeout * HZ);
		return 0;

	case WDIOC_KEEPALIVE:
		mod_timer(&swdt.timer, jiffies + swdt.timeout * HZ);
		return 0;
	}

	return -ENOTTY;
}

module_platform_driver(smart_wdt_driver);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Anand Kumar");
MODULE_DESCRIPTION("Smart Device Tree Based Watchdog Driver");
