LOCAL_PATH := $(call my-dir)
include $(CLEAR_VARS)

LOCAL_MODULE := liblibraw

LOCAL_LDLIBS    := -lm -llog -ljnigraphics -latomic

#path to LibRAW source root:
LIBRAW_PATH := $(LOCAL_PATH)/../src

#LOCAL_CFLAGS := -DUSE_LCMS2 -DUSE_JPEG8 -DUSE_JPEG
LOCAL_CFLAGS := -DUSE_JPEG8 -DLIBRAW_LIBRARY_BUILD

LOCAL_CFLAGS += -Ofast
#LOCAL_CFLAGS += -O0
LOCAL_CFLAGS += -fsigned-char

#LOCAL_CFLAGS += -fopenmp
#LOCAL_LDFLAGS += -fopenmp

LOCAL_CPPFLAGS := $(LOCAL_CFLAGS) -frtti -fexceptions -fnon-call-exceptions

LOCAL_C_INCLUDES := $(LIBRAW_PATH)/internal $(LIBRAW_PATH)/libraw $(LIBRAW_PATH)/ $(LOCAL_PATH)/../jpeglib/jpeglib


FILE_LIST := $(LOCAL_PATH)/swab.c \
$(LOCAL_PATH)/libraw.c \
$(LIBRAW_PATH)/internal/dcraw_common.cpp \
$(LIBRAW_PATH)/internal/dcraw_fileio.cpp \
$(LIBRAW_PATH)/internal/demosaic_packs.cpp \
$(LIBRAW_PATH)/src/libraw_datastream.cpp \
$(LIBRAW_PATH)/src/libraw_cxx.cpp \
$(LIBRAW_PATH)/src/libraw_c_api.cpp

#FILE_LIST:= libraw.cpp
LOCAL_EXPORT_C_INCLUDES	:= $(LIBRAW_PATH)/libraw $(LOCAL_PATH)/

LOCAL_SRC_FILES := $(FILE_LIST)

LOCAL_STATIC_LIBRARIES := jpeg

include $(BUILD_SHARED_LIBRARY)