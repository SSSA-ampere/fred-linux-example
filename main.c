/*
 * Fred for Linux. Experimental support.
 *
 * Copyright (C) 2018-2021, Marco Pagani, ReTiS Lab.
 * <marco.pag(at)outlook.com>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
*/

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "fred_lib.h"
#include "periodic_task.h"
#include "demo_hw_task.h"

//-------------------------------------------------------------------------------

struct demo_hw_task sum_hw_task;
const uint32_t sum_hw_task_hw_id = 100;

struct periodic_task sum_task;
const int sum_task_period = 100;

uint32_t sum_op(uint32_t a, uint32_t b)
{
	return (a + b);
}

//-------------------------------------------------------------------------------

struct demo_hw_task sub_hw_task;
const uint32_t sub_hw_task_hw_id = 101;

struct periodic_task sub_task;
const int sub_task_period = 110;

uint32_t sub_op(uint32_t a, uint32_t b)
{
	return (a - b);
}

//-------------------------------------------------------------------------------

struct demo_hw_task mul_hw_task;
const uint32_t mul_hw_task_hw_id = 102;

struct periodic_task mul_task;
const int mul_task_period = 120;

uint32_t mul_op(uint32_t a, uint32_t b)
{
	return (a * b);
}

//-------------------------------------------------------------------------------

struct demo_hw_task xor_hw_task;
const uint32_t xor_hw_task_hw_id = 200;

struct periodic_task xor_task;
const int xor_task_period = 130;

uint32_t xor_op(uint32_t a, uint32_t b)
{
	return (a ^ b);
}

//-------------------------------------------------------------------------------

struct demo_hw_task nor_hw_task;
const uint32_t nor_hw_task_hw_id = 201;

struct periodic_task nor_task;
const int nor_task_period = 140;

uint32_t nor_op(uint32_t a, uint32_t b)
{
	return ~(a | b);
}

//-------------------------------------------------------------------------------

int main()
{
	int retval;

	//---------------------------------------------------------------------------

	srand(time(NULL));

	//---------------------------------------------------------------------------

	// Initialize hw-task component
	retval = demo_hw_task_init(&sum_hw_task, sum_hw_task_hw_id, sum_op);
	if (retval)
		return -1;

	// initialize periodic sw-task wrapper
	retval = periodic_task_init(&sum_task ,demo_hw_task_wrapper,
								&sum_hw_task, sum_task_period);
	if (retval)
		return -1;

	//---------------------------------------------------------------------------

	retval = demo_hw_task_init(&sub_hw_task, sub_hw_task_hw_id, sub_op);
	if (retval)
		return -1;

	retval = periodic_task_init(&sub_task ,demo_hw_task_wrapper,
								&sub_hw_task, sub_task_period);
	if (retval)
		return -1;

	//---------------------------------------------------------------------------

	retval = demo_hw_task_init(&mul_hw_task, mul_hw_task_hw_id, mul_op);
	if (retval)
		return -1;

	retval = periodic_task_init(&mul_task ,demo_hw_task_wrapper,
								&mul_hw_task, mul_task_period);
	if (retval)
		return -1;

	//---------------------------------------------------------------------------

	retval = demo_hw_task_init(&xor_hw_task, xor_hw_task_hw_id, xor_op);
	if (retval)
		return -1;

	retval = periodic_task_init(&xor_task ,demo_hw_task_wrapper,
								&xor_hw_task, xor_task_period);
	if (retval)
		return -1;

	//---------------------------------------------------------------------------

	retval = demo_hw_task_init(&nor_hw_task, nor_hw_task_hw_id, nor_op);
	if (retval)
		return -1;

	retval = periodic_task_init(&nor_task ,demo_hw_task_wrapper,
								&nor_hw_task, nor_task_period);
	if (retval)
		return -1;

	//---------------------------------------------------------------------------

	// Start all sw-tasks
	retval = periodic_task_start(&sum_task);
	if (retval)
		return -1;

	retval = periodic_task_start(&sub_task);
	if (retval)
		return -1;

	retval = periodic_task_start(&mul_task);
	if (retval)
		return -1;

	retval = periodic_task_start(&xor_task);
	if (retval)
		return -1;

	retval = periodic_task_start(&nor_task);
	if (retval)
		return -1;

	//---------------------------------------------------------------------------

	periodic_task_join(&sum_task);
	periodic_task_join(&sub_task);
	periodic_task_join(&mul_task);
	periodic_task_join(&xor_task);
	periodic_task_join(&nor_task);
	
	//---------------------------------------------------------------------------

	demo_hw_task_free(&sum_hw_task);
	demo_hw_task_free(&sub_hw_task);
	demo_hw_task_free(&mul_hw_task);
	demo_hw_task_free(&xor_hw_task);
	demo_hw_task_free(&nor_hw_task);

	//---------------------------------------------------------------------------

	return 0;
}
