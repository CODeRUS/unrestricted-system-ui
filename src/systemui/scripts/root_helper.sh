#!/bin/sh

restart_sysuid()
{
		/sbin/initctl stop xsession/sysuid || echo "sysuid not running"
		sleep 5
		/sbin/initctl start xsession/sysuid
}

case "$1" in
	"menu-modern")
		sed -i -e "s/StatusIndicatorMenuVerticalView/StatusIndicatorMenuDropDownView/" /usr/share/themes/blanco/meegotouch/sysuid/sysuid.conf
		restart_sysuid
		;;
	"menu-classic")
		sed -i -e "s/StatusIndicatorMenuDropDownView/StatusIndicatorMenuVerticalView/" /usr/share/themes/blanco/meegotouch/sysuid/sysuid.conf
		restart_sysuid
		;;
	*)
		;;
esac

exit 0