# The TARGET variable determines what target system the application is
# compiled for. It either refers to an XN file in the source directories
# or a valid argument for the --target option when compiling
TARGET = XCORE-200-EXPLORER

# The APP_NAME variable determines the name of the final .xe file. It should
# not include the .xe postfix. If left blank the name will default to
# the project name
APP_NAME = app_fft

# The USED_MODULES variable lists other modules used by the application.
USED_MODULES = lib_dsp(>=4.0.0)

# The flags passed to xcc when building the application
# You can also set the following to override flags for a particular language:
# XCC_XC_FLAGS, XCC_C_FLAGS, XCC_ASM_FLAGS, XCC_CPP_FLAGS
# If the variable XCC_MAP_FLAGS is set it overrides the flags passed to
# xcc for the final link (mapping) stage.
XCC_FLAGS_complex = -O2 -g -report
XCC_FLAGS_tworeals = -O2 -g -report -DTWOREALS=1

XCC_FLAGS_complex_int16_buf = -O2 -g -report -DINT16_BUFFERS=1
XCC_FLAGS_tworeals_int16_buf = -O2 -g -report -DTWOREALS=1 -DINT16_BUFFERS=1

# The XCORE_ARM_PROJECT variable, if set to 1, configures this
# project to create both xCORE and ARM binaries.
XCORE_ARM_PROJECT = 0

# The VERBOSE variable, if set to 1, enables verbose output from the make system.
VERBOSE = 0

XMOS_MAKE_PATH ?= ../..
-include $(XMOS_MAKE_PATH)/xcommon/module_xcommon/build/Makefile.common
