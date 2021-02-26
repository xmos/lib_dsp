BUILD_FLAGS = -O2 -g -report

XCOREAI ?= 0
ifeq ($(XCOREAI),0)
TARGET = XCORE-200-EXPLORER
XCC_FLAGS= $(BUILD_FLAGS)
else
# Xcore.ai Builds
# Use `xmake XCOREAI=1`
TARGET = XCORE-AI-EXPLORER
XCC_FLAGS_xcoreai = $(BUILD_FLAGS)
endif

APP_NAME =
USED_MODULES = lib_dsp(>=6.0.0)

XCORE_ARM_PROJECT = 0
VERBOSE = 0

XMOS_MAKE_PATH ?= ../..
-include $(XMOS_MAKE_PATH)/xcommon/module_xcommon/build/Makefile.common
