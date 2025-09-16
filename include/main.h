#pragma once

#include "types.h"

extern uint8_t __data_start;
extern uint8_t __data_load_start;
extern uint8_t __data_end;
extern uint8_t __bss_start;
extern uint8_t __bss_end;

static void _disable_wdt(void);
static void _init_data(void);
static void _init_bss(void);
static void _start(void);
