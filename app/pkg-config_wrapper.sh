#!/bin/sh
PKG_CONFIG_SYSROOT_DIR=/home/garybap/axess/yocto_dunfell/build/tmp/work/x86_64-linux/qtbase-native/5.14.2+gitAUTOINC+3a6d8df521-r0/recipe-sysroot-native
export PKG_CONFIG_SYSROOT_DIR
PKG_CONFIG_LIBDIR=/home/garybap/axess/yocto_dunfell/build/tmp/work/x86_64-linux/qtbase-native/5.14.2+gitAUTOINC+3a6d8df521-r0/recipe-sysroot-native/usr/lib/pkgconfig:/home/garybap/axess/yocto_dunfell/build/tmp/work/x86_64-linux/qtbase-native/5.14.2+gitAUTOINC+3a6d8df521-r0/recipe-sysroot-native/usr/share/pkgconfig:/home/garybap/axess/yocto_dunfell/build/tmp/work/x86_64-linux/qtbase-native/5.14.2+gitAUTOINC+3a6d8df521-r0/recipe-sysroot-native/usr/lib/x86_64-linux-gnu/pkgconfig
export PKG_CONFIG_LIBDIR
exec pkg-config "$@"
