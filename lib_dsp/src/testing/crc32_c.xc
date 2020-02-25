// Copyright (c) 2019, XMOS Ltd, All rights reserved
#include <xs1.h>
#include <xclib.h>

void crc32_c(unsigned &checksum, unsigned data, unsigned poly){
	crc32(checksum, data, poly);
}
