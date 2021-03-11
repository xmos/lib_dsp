COMMON_FLAGS = -O2 -g -report

BUILD_FLAGS_complex = $(COMMON_FLAGS) -DNUM_CHANS=3
BUILD_FLAGS_tworeals = $(COMMON_FLAGS) -DTWOREALS=1 -DNUM_CHANS=4

BUILD_FLAGS_complex_int16_buf = $(COMMON_FLAGS) -DINT16_BUFFERS=1 -DNUM_CHANS=6
BUILD_FLAGS_tworeals_int16_buf = $(COMMON_FLAGS) -DTWOREALS=1 -DINT16_BUFFERS=1 -DNUM_CHANS=8

XCOREAI ?= 0
ifeq ($(XCOREAI),1)
# Xcore.ai Builds
# Use `xmake XCOREAI=1`

# The TARGET variable determines what target system the application is
# compiled for. It either refers to an XN file in the source directories
# or a valid argument for the --target option when compiling
TARGET = XCORE-AI-EXPLORER

# The flags passed to xcc when building the application
# You can also set the following to override flags for a particular language:
# XCC_XC_FLAGS, XCC_C_FLAGS, XCC_ASM_FLAGS, XCC_CPP_FLAGS
# If the variable XCC_MAP_FLAGS is set it overrides the flags passed to
# xcc for the final link (mapping) stage.
XCC_FLAGS_xcoreai = $(BUILD_FLAGS)
XCC_FLAGS_complex_xcoreai = $(BUILD_FLAGS_complex)
XCC_FLAGS_tworeals_xcoreai = $(BUILD_FLAGS_tworeals)

XCC_FLAGS_complex_int16_buf_xcoreai = $(BUILD_FLAGS_complex_int16_buf)
XCC_FLAGS_tworeals_int16_buf_xcoreai = $(BUILD_FLAGS_tworeals_int16_buf)

else
# Xcore-200 Builds
# Use `xmake`
TARGET = xk-audio-216-mc

XCC_FLAGS_complex = $(BUILD_FLAGS_complex)
XCC_FLAGS_tworeals = $(BUILD_FLAGS_tworeals)

XCC_FLAGS_complex_int16_buf = $(BUILD_FLAGS_complex_int16_buf)
XCC_FLAGS_tworeals_int16_buf = $(BUILD_FLAGS_tworeals_int16_buf)
endif

# The APP_NAME variable determines the name of the final .xe file. It should
# not include the .xe postfix. If left blank the name will default to
# the project name

APP_NAME = app_fft_double_buf

# The USED_MODULES variable lists other build module used by the application.
USED_MODULES = lib_dsp(>=6.0.0)

#=============================================================================
# The following part of the Makefile includes the common build infrastructure
# for compiling XMOS applications. You should not need to edit below here.

XMOS_MAKE_PATH ?= ../..
include $(XMOS_MAKE_PATH)/xcommon/module_xcommon/build/Makefile.common
