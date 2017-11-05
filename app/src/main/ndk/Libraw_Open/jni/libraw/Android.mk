LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

LOCAL_MODULE := liblibraw

LOCAL_LDLIBS    := -lm -llog -ljnigraphics

#path to LibRAW source root:
LIBRAW_PATH := $(LOCAL_PATH)/../src

LOCAL_CFLAGS := -fsigned-char

LOCAL_CPPFLAGS := $(LOCAL_CFLAGS) -fexceptions 

LOCAL_C_INCLUDES := $(LIBRAW_PATH)/internal $(LIBRAW_PATH)/libraw $(LIBRAW_PATH)/


FILE_LIST := $(LOCAL_PATH)/swab.c \
$(LOCAL_PATH)/libraw.c \
$(LIBRAW_PATH)/internal/dcraw_common.cpp \
$(LIBRAW_PATH)/internal/dcraw_fileio.cpp \
$(LIBRAW_PATH)/internal/demosaic_packs.cpp \
$(LIBRAW_PATH)/src/libraw_datastream.cpp \
$(LIBRAW_PATH)/src/libraw_cxx.cpp \
$(LIBRAW_PATH)/src/libraw_c_api.cpp

LOCAL_EXPORT_C_INCLUDES	:= $(LIBRAW_PATH)/libraw $(LOCAL_PATH)/

LOCAL_SRC_FILES := $(FILE_LIST)

include $(BUILD_SHARED_LIBRARY)
