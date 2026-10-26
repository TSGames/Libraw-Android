LOCAL_PATH:= $(call my-dir)/jpeglib
include $(CLEAR_VARS)

LOCAL_ARM_MODE := arm
LOCAL_SRC_FILES := \
    jcapimin.c jcapistd.c jccoefct.c jccolor.c jcdctmgr.c jchuff.c \
    jcinit.c jcmainct.c jcmarker.c jcmaster.c jcomapi.c jcparam.c \
    jcphuff.c jcprepct.c jcsample.c jctrans.c jdapimin.c jdapistd.c \
    jdatadst.c jdatasrc.c jdcoefct.c jdcolor.c jddctmgr.c jdhuff.c \
    jdinput.c jdmainct.c jdmarker.c jdmaster.c jdmerge.c jdphuff.c \
    jdpostct.c jdsample.c jdtrans.c jerror.c jfdctflt.c jfdctfst.c \
    jfdctint.c jidctflt.c jidctfst.c jidctint.c jidctred.c jquant1.c \
    jquant2.c jutils.c jmemmgr.c armv6_idct.S


# unbundled branch, built against NDK.
LOCAL_SDK_VERSION := 17
# the original android memory manager.
# use sdcard as libjpeg decoder's backing store
LOCAL_SRC_FILES += \
    jmem-android.c


LOCAL_CFLAGS += -DAVOID_TABLES
LOCAL_CFLAGS += -O3 -fstrict-aliasing -fprefetch-loop-arrays
#LOCAL_CFLAGS += -march=armv6j

# enable tile based decode
LOCAL_CFLAGS += -DANDROID_TILE_BASED_DECODE

ifeq ($(TARGET_ARCH),x86)
  LOCAL_CFLAGS += -DANDROID_INTELSSE2_IDCT
  LOCAL_SRC_FILES += jidctintelsse.c
endif

ifeq ($(strip $(TARGET_ARCH)),arm)
  ifeq ($(ARCH_ARM_HAVE_NEON),true)
    #use NEON accelerations
    LOCAL_CFLAGS += -DNV_ARM_NEON
    LOCAL_SRC_FILES += \
        jsimd_arm_neon.S \
        jsimd_neon.c
  else
    # crashes on targeting arm
    # enable armv6 idct assembly
    #LOCAL_CFLAGS += -DANDROID_ARMV6_IDCT
  endif
endif

# use mips assembler IDCT implementation if MIPS DSP-ASE is present
ifeq ($(strip $(TARGET_ARCH)),mips)
  ifeq ($(strip $(ARCH_MIPS_HAS_DSP)),true)
  LOCAL_CFLAGS += -DANDROID_MIPS_IDCT
  LOCAL_SRC_FILES += \
      mips_jidctfst.c \
      mips_idct_le.S
  endif
endif

LOCAL_MODULE := libjpeg

include $(BUILD_STATIC_LIBRARY)
