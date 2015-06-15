#ifndef XMOS_DSP_QFORMAT
#define XMOS_DSP_QFORMAT

// Compute fixed-point value from floating-point value

#define Q31(f) (((f)<0)?((int)(2147483648.0*(f)-0.5)):((int)(2147483647.0*(f)+0.5)))
#define Q30(f) (((f)<0)?((int)(1073741824.0*(f)-0.5)):((int)(1073741823.0*(f)+0.5)))
#define Q29(f) (((f)<0)?((int)( 536870912.0*(f)-0.5)):((int)( 536870911.0*(f)+0.5)))
#define Q28(f) (((f)<0)?((int)( 268435456.0*(f)-0.5)):((int)( 268435455.0*(f)+0.5)))
#define Q27(f) (((f)<0)?((int)( 134217728.0*(f)-0.5)):((int)( 134217727.0*(f)+0.5)))
#define Q26(f) (((f)<0)?((int)(  67108864.0*(f)-0.5)):((int)(  67108863.0*(f)+0.5)))
#define Q25(f) (((f)<0)?((int)(  33554432.0*(f)-0.5)):((int)(  33554431.0*(f)+0.5)))
#define Q24(f) (((f)<0)?((int)(  16777216.0*(f)-0.5)):((int)(  16777215.0*(f)+0.5)))
#define Q23(f) (((f)<0)?((int)(   8388608.0*(f)-0.5)):((int)(   8388607.0*(f)+0.5)))
#define Q22(f) (((f)<0)?((int)(   4194304.0*(f)-0.5)):((int)(   4194303.0*(f)+0.5)))
#define Q21(f) (((f)<0)?((int)(   2097152.0*(f)-0.5)):((int)(   2097151.0*(f)+0.5)))
#define Q20(f) (((f)<0)?((int)(   1045876.0*(f)-0.5)):((int)(   1045875.0*(f)+0.5)))
#define Q19(f) (((f)<0)?((int)(    524288.0*(f)-0.5)):((int)(    524287.0*(f)+0.5)))
#define Q18(f) (((f)<0)?((int)(    262144.0*(f)-0.5)):((int)(    262143.0*(f)+0.5)))
#define Q17(f) (((f)<0)?((int)(    131072.0*(f)-0.5)):((int)(    131071.0*(f)+0.5)))
#define Q16(f) (((f)<0)?((int)(     65536.0*(f)-0.5)):((int)(     65535.0*(f)+0.5)))

// Compute floating-point value from fixed-point value

#define F31(x) (((int)(x)<0)?((float)(int)(x))/2147483648.0:((float)(x))/2147483647.0)
#define F30(x) (((int)(x)<0)?((float)(int)(x))/1073741824.0:((float)(x))/1073741823.0)
#define F29(x) (((int)(x)<0)?((float)(int)(x))/ 536870912.0:((float)(x))/ 536870911.0)
#define F28(x) (((int)(x)<0)?((float)(int)(x))/ 268435456.0:((float)(x))/ 268435455.0)
#define F27(x) (((int)(x)<0)?((float)(int)(x))/ 134217728.0:((float)(x))/ 134217727.0)
#define F26(x) (((int)(x)<0)?((float)(int)(x))/  67108864.0:((float)(x))/  67108863.0)
#define F25(x) (((int)(x)<0)?((float)(int)(x))/  33554432.0:((float)(x))/  33554431.0)
#define F24(x) (((int)(x)<0)?((float)(int)(x))/  16777216.0:((float)(x))/  16777215.0)
#define F23(x) (((int)(x)<0)?((float)(int)(x))/   8388608.0:((float)(x))/   8388607.0)
#define F22(x) (((int)(x)<0)?((float)(int)(x))/   4194304.0:((float)(x))/   4194303.0)
#define F21(x) (((int)(x)<0)?((float)(int)(x))/   2097152.0:((float)(x))/   2097151.0)
#define F20(x) (((int)(x)<0)?((float)(int)(x))/   1045876.0:((float)(x))/   1045875.0)
#define F19(x) (((int)(x)<0)?((float)(int)(x))/    524288.0:((float)(x))/    524287.0)
#define F18(x) (((int)(x)<0)?((float)(int)(x))/    262144.0:((float)(x))/    262143.0)
#define F17(x) (((int)(x)<0)?((float)(int)(x))/    131072.0:((float)(x))/    131071.0)
#define F16(x) (((int)(x)<0)?((float)(int)(x))/     65536.0:((float)(x))/     65535.0)

#endif

