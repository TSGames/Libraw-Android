APP_PLATFORM := android-16

LOCAL_PATH:= $(call my-dir)
#include $(CLEAR_VARS)
# first lib, which will be built statically
#include $(call opencv/Android.mk)
include $(call all-subdir-makefiles)

# second lib, which will depend on and include the first one
#
#include $(CLEAR_VARS)

