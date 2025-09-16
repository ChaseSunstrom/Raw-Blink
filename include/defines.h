#pragma once

#include "types.h"

// Special function register defs
#define _SFR_MEM8(_address) (*(volatile uint8_t *)(_address))
#define _SFR_MEM16(_address) (*(volatile uint16_t *)(_address))
