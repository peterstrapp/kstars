LOCAL_PATH:= $(call my-dir)

include $(CLEAR_VARS)

LOCAL_MODULE := libjpeg

LOCAL_MODULE_FILENAME := libjpeg

LOCAL_SRC_FILES := $(wildcard $(LOCAL_PATH)/src/*.c)

LOCAL_C_INCLUDES := $(LOCAL_PATH)/src

LOCAL_EXPORT_C_INCLUDES := $(LOCAL_C_INCLUDES)

include $(BUILD_STATIC_LIBRARY)
include $(CLEAR_VARS)