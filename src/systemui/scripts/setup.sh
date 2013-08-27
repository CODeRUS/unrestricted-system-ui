#!/bin/sh

if [ ! -d /home/user/.status-menu ]; then
	"mkdir -p /home/user/.status-menu"
fi
if [ ! -f /home/user/.status-menu/items-order.conf ]; then
	"cp -v /usr/share/sysuid/items-order.conf /home/user/.status-menu/"
fi
if [ ! -f /home/user/.status-menu/top-order.conf ]; then
	"cp -v /usr/share/sysuid/top-order.conf /home/user/.status-menu/"
fi
if [ ! -f /home/user/.status-menu/pannable-order.conf ]; then
	"cp -v /usr/share/sysuid/pannable-order.conf /home/user/.status-menu/"
fi

if [ -f /tmp/system-ui-crashed ]; then
	rm /tmp/system-ui-crashed
fi

exit 0