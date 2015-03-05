
# Copyright (C) 2010 The Android Open Source Project

#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#      http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.
#

SAMPLE_PATH := $(call my-dir)/../../
GAMEPLAY_LOCAL_PATH = ../../../GamePlay/

GAMEPLAY_PATH := $(SAMPLE_PATH)$(GAMEPLAY_LOCAL_PATH)
GAMEPLAY_DEPS := $(SAMPLE_PATH)$(GAMEPLAY_LOCAL_PATH)external-deps/lib/android/$(TARGET_ARCH_ABI)

LIBYAML_PATH := $(GAMEPLAY_PATH)external-deps/lib/android/$(TARGET_ARCH_ABI)
LIBSIGCPP_PATH := $(SAMPLE_PATH)../../third_party/libsigc++/lib/android/$(TARGET_ARCH_ABI)
LIBCURL_PATH := $(SAMPLE_PATH)../../third_party/libcurl/lib/android/$(TARGET_ARCH_ABI)
LIBICU_PATH := $(SAMPLE_PATH)../../third_party/icu/lib/android/$(TARGET_ARCH_ABI)
LIBZIP_PATH := $(SAMPLE_PATH)../../third_party/zip/lib/android/$(TARGET_ARCH_ABI)

# gameplay
LOCAL_PATH := $(GAMEPLAY_PATH)gameplay/android/libs/$(TARGET_ARCH_ABI)
include $(CLEAR_VARS)
LOCAL_MODULE    := libgameplay
LOCAL_SRC_FILES := libgameplay.so
include $(PREBUILT_SHARED_LIBRARY)

# libgameplay-deps
LOCAL_PATH := $(GAMEPLAY_DEPS)
include $(CLEAR_VARS)
LOCAL_MODULE    := libgameplay-deps 
LOCAL_SRC_FILES := libgameplay-deps.a
include $(PREBUILT_STATIC_LIBRARY)

# libyaml
LOCAL_PATH := $(LIBYAML_PATH)
include $(CLEAR_VARS)
LOCAL_MODULE    := libyaml
LOCAL_SRC_FILES := libyaml.a
include $(PREBUILT_STATIC_LIBRARY)

# libsigc++
LOCAL_PATH := $(LIBSIGCPP_PATH)
include $(CLEAR_VARS)
LOCAL_MODULE    := libsigc++
LOCAL_SRC_FILES := libsigc++.a
include $(PREBUILT_STATIC_LIBRARY)

# libcurl
LOCAL_PATH := $(LIBCURL_PATH)
include $(CLEAR_VARS)
LOCAL_MODULE    := libcurl
LOCAL_SRC_FILES := libcurl.a
include $(PREBUILT_STATIC_LIBRARY)

# libicu
LOCAL_PATH := $(LIBICU_PATH)
include $(CLEAR_VARS)
LOCAL_MODULE    := libicuuc
LOCAL_SRC_FILES := libicuuc.a
include $(PREBUILT_STATIC_LIBRARY)

# libicu
LOCAL_PATH := $(LIBICU_PATH)
include $(CLEAR_VARS)
LOCAL_MODULE    := libicui18n
LOCAL_SRC_FILES := libicui18n.a
include $(PREBUILT_STATIC_LIBRARY)

# libicu
LOCAL_PATH := $(LIBICU_PATH)
include $(CLEAR_VARS)
LOCAL_MODULE    := libicudata
LOCAL_SRC_FILES := libicudata.a
include $(PREBUILT_STATIC_LIBRARY)

# libzip
LOCAL_PATH := $(LIBZIP_PATH)
include $(CLEAR_VARS)
LOCAL_MODULE    := libzip
LOCAL_SRC_FILES := libzip.a
include $(PREBUILT_STATIC_LIBRARY)

# dfg-gameplay
LOCAL_PATH := $(SAMPLE_PATH)
include $(CLEAR_VARS)

LOCAL_MODULE    := libdfg-gameplay
LOCAL_SRC_FILES := \
    pch.cpp \
    uuidlib/clear.c \
    uuidlib/compare.c \
    uuidlib/copy.c \
    uuidlib/gen_uuid.c \
    uuidlib/isnull.c \
    uuidlib/pack.c \
    uuidlib/parse.c \
    uuidlib/unpack.c \
    uuidlib/unparse.c \
    uuidlib/uuid_time.c \
    base/main/asset.cpp \
    base/main/dictionary.cpp \
    base/main/gameplay_assets.cpp \
    base/main/memory_stream.cpp \
    base/main/zip_package.cpp \
    base/render/particle_system.cpp \
    base/services/debug_service.cpp \
    base/services/input_service.cpp \
    base/services/render_service.cpp \
    base/services/service.cpp \
    base/services/service_manager.cpp \
    base/services/social_service.cpp \
    base/services/storefront_service.cpp \
    base/services/tracker_service.cpp \
    base/utils/profiler.cpp \
    base/utils/singleton.cpp \
    base/utils/utils.cpp \
    base/main.cpp \

# LOCAL_LDLIBS    := -llog -landroid -lEGL -lGLESv2 -lOpenSLES
LOCAL_CFLAGS    := -O3 -D__ANDROID__ -Wno-psabi -fpermissive -fexceptions \
    -I"$(GAMEPLAY_PATH)external-deps/include" \
    -I"$(SAMPLE_PATH)../../third_party/libsigc++/include" \
    -I"$(SAMPLE_PATH)../../third_party/libcurl/include" \
    -I"$(SAMPLE_PATH)../../third_party/icu/include" \
    -I"$(SAMPLE_PATH)../../third_party/zip/include" \
    -I"$(SAMPLE_PATH)base" \
    -I"$(GAMEPLAY_PATH)gameplay/src" \
    -DHAVE_INTTYPES_H \
    -DHAVE_UNISTD_H \
    -DHAVE_ERRNO_H \
    -DHAVE_NETINET_IN_H \
    -DHAVE_SYS_IOCTL_H \
    -DHAVE_SYS_MMAN_H \
    -DHAVE_SYS_MOUNT_H \
    -DHAVE_SYS_PRCTL_H \
    -DHAVE_SYS_RESOURCE_H \
    -DHAVE_SYS_SELECT_H \
    -DHAVE_SYS_STAT_H \
    -DHAVE_SYS_TYPES_H \
    -DHAVE_STDLIB_H \
    -DHAVE_STRDUP \
    -DHAVE_MMAP \
    -DHAVE_UTIME_H \
    -DHAVE_GETPAGESIZE \
    -DHAVE_LSEEK64 \
    -DHAVE_LSEEK64_PROTOTYPE \
    -DHAVE_EXT2_IOCTLS \
    -DHAVE_LINUX_FD_H \
    -DHAVE_TYPE_SSIZE_T \
    -DHAVE_SYS_TIME_H \
    -DHAVE_SYS_PARAM_H \
    -DHAVE_SYSCONF \
    -D_GLIBCXX_USE_C99_STDINT_TR1 \
    -DU_DISABLE_RENAMING=1 \
    -DGP_USE_SOCIAL \
    -DGP_USE_STOREFRONT \

LOCAL_CPPFLAGS := -std=c++11 -Wno-switch-enum -Wno-switch
LOCAL_STATIC_LIBRARIES := android_native_app_glue libgameplay-deps libyaml libsigc++ libcurl libicui18n libicuuc libicudata libzip
LOCAL_ARM_MODE := arm
LOCAL_SHARED_LIBRARIES := gameplay


#include $(BUILD_SHARED_LIBRARY)            # uncomment this line to build a shared library
include $(BUILD_STATIC_LIBRARY)           # here, we are building a static library
$(call import-module,android/native_app_glue)