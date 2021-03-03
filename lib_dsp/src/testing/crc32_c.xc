// Copyright (c) 2019-2021, XMOS Ltd, All rights reserved
// This software is available under the terms provided in LICENSE.txt.
#include <xs1.h>
#include <xclib.h>

void crc32_c(unsigned &checksum, unsigned data, unsigned poly){
	crc32(checksum, data, poly);
}
