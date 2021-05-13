#!/bin/bash

# Target arch
export RK_ARCH=arm64
# Uboot defconfig
export RK_UBOOT_DEFCONFIG=evb-rk3308
# Kernel defconfig
export RK_KERNEL_DEFCONFIG=rk3308_linux_defconfig
# Kernel dts
export RK_KERNEL_DTS=rk3308-cmcc-rns

export RK_KERNEL_IMG=kernel/arch/arm64/boot/Image.lz4
# boot image type
export RK_BOOT_IMG=zboot.img
# parameter for GPT table
export RK_PARAMETER=cmcc_soundai/parameter-64bit-soundai_oem.txt
# Buildroot config
export RK_CFG_BUILDROOT=rockchip_rk3308_soundai_release
# Recovery config
export RK_CFG_RECOVERY=rockchip_rk3308_recovery
# Pcba config
export RK_CFG_PCBA=rockchip_rk3308_pcba
# Build jobs
export RK_JOBS=12
# target chip
export RK_TARGET_PRODUCT=rk3308
# Set rootfs type, including ext2 ext4 squashfs
export RK_ROOTFS_TYPE=squashfs
# rootfs image path
export RK_ROOTFS_IMG=rockdev/rootfs.${RK_ROOTFS_TYPE}
# Set oem partition type, including ext2 squashfs
export RK_OEM_FS_TYPE=ext2
# Set userdata partition type, including ext2, fat
export RK_USERDATA_FS_TYPE=ext2
#OEM config: /oem/dueros/aispeech-6mic-64bit/aispeech-2mic-64bit/aispeech-4mic-32bit/aispeech-2mic-32bit/aispeech-2mic-kongtiao-32bit/iflytekSDK/CaeDemo_VAD/smart_voice
export RK_OEM_DIR=cmcc_soundai
#Build oem to rootfs 
export RK_BUILD_OEM_ROOTFS=false
#userdata config
export RK_USERDATA_DIR=userdata_empty
MIC_NUM=6

