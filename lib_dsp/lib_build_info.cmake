set(LIB_NAME lib_dsp)
set(LIB_VERSION 6.3.0)
set(LIB_INCLUDES api)
set(LIB_DEPENDENT_MODULES "lib_logging(3.2.0)")
set(LIB_OPTIONAL_HEADERS debug_conf.h xmos_dsp_elements.h)

XMOS_REGISTER_MODULE()
