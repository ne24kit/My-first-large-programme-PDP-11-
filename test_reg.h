#pragma once

#include "run.h"
#include "log.h"
#include "mem.h"
#include <assert.h>

void tests_on_cmd();

void test_mov();

void test_mode0();

void test_parse_mov();

void test_mode1_from_reg_to_mem();

void test_mode1_from_mem_to_reg();

void test_mode1_from_mem_to_mem();

void test_mode2_from_reg_to_mem();

void test_mode2_from_mem_to_reg();

void test_mode2_from_mem_to_mem();

void test_mode3_from_mem_to_reg();

void test_mode3_from_reg_to_mem();

void test_mode3_from_mem_to_mem();

void test_mode4_from_mem_to_reg();

void test_mode4_from_reg_to_mem();

void test_mode4_from_mem_to_mem();

void test_mode5_from_mem_to_reg();

void test_mode5_from_reg_to_mem();

void test_mode5_from_mem_to_mem();