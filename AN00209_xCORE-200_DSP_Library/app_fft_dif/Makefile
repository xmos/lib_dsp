BUILD_FLAGS = -O2 -g -report

XCOREAI ?= 0
ifeq ($(XCOREAI),0)
# The TARGET variable determines what target system the application is
# compiled for. It either refers to an XN file in the source directories
# or a valid argument for the --target option when compiling
TARGET = XCORE-200-EXPLORER

XCC_FLAGS = $(BUILD_FLAGS)
else
# Xcore.ai Builds
# Use `xmake XCOREAI=1`
TARGET = XCORE-AI-EXPLORER
XCC_FLAGS_xcoreai = $(BUILD_FLAGS)
endif

APP_NAME =

USED_MODULES = lib_dsp(>=6.0.0)


# The XCORE_ARM_PROJECT variable, if set to 1, configures this
# project to create both xCORE and ARM binaries.
XCORE_ARM_PROJECT = 0

# The VERBOSE variable, if set to 1, enables verbose output from the make system.
VERBOSE = 0

XMOS_MAKE_PATH ?= ../..
-include $(XMOS_MAKE_PATH)/xcommon/module_xcommon/build/Makefile.common
