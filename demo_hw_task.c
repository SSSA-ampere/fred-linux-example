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

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

#include "fred_support/fred_lib.h"
#include "demo_hw_task.h"

#define ARRAY_SIZE	1024

int demo_hw_task_init(struct demo_hw_task *self, uint32_t hw_id,
						data_t (*op)(data_t, data_t))
{
	int retval;

	assert(self);
	assert(op);

	self->hw_id = hw_id;
	self->op = op;

	retval = fred_init(&self->fred_data);
	if (retval) {
		printf("fred_init failed for simple hw-task %u\n", self->hw_id);
		return -1;
	}

	printf("fred_init done for simple hw-task %u\n", self->hw_id);

	retval = fred_bind(self->fred_data, &self->fred_hw_task, self->hw_id);
	if (retval) {
		printf("fred_bind failed for simple hw-task %u\n", self->hw_id);
		return -1;
	}

	printf("fred_init done for simple hw-task %u\n", self->hw_id);


	self->out = fred_map_buff(self->fred_data, self->fred_hw_task, 0);
	if (!self->out) {
		printf("fred_map_buff failed on buff 0 for simple hw-task %u\n", self->hw_id);
		return -1;
	}

	self->in_a = fred_map_buff(self->fred_data, self->fred_hw_task, 1);
	if (!self->in_a) {
		printf("fred_map_buff failed on buff 1 for simple hw-task %u\n", self->hw_id);
		return -1;
	}

	self->in_b = fred_map_buff(self->fred_data, self->fred_hw_task, 2);
	if (!self->in_b) {
		printf("fred_map_buff failed on buff 2 for simple hw-task %u\n", self->hw_id);
		return -1;
	}

	return 0;
}

int demo_hw_task_accel(struct demo_hw_task *self)
{
	int retval;
	data_t out_sw;

	printf("Requesting acceleration for simple hw-task %u\n", self->hw_id);

	// Initialize random input data
	for (int i = 0; i < ARRAY_SIZE; ++i) {
		self->in_a[i] = rand() % 1024;
		self->in_b[i] = rand() % 1024;
	}

	retval = fred_accel(self->fred_data, self->fred_hw_task);
	if (retval) {
		printf("Acceleration request failed for simple hw-task %u\n", self->hw_id);
		return -1;
	}

	printf("Acceleration done for hw-task %u\n", self->hw_id);

	// Check output data
	for (int i = 0; i < ARRAY_SIZE; ++i) {
		out_sw = self->op(self->in_a[i], self->in_b[i]);

		if (self->out[i] != out_sw) {
			printf("Result mismatch for simple hw-task %u: SW:%u, HW:%u)\n",
					self->hw_id, out_sw, self->out[i]);
			return -1;
		} else if (i == 0) {
			printf("Result sample for simple hw-task %u: SW:%u, HW:%u)\n",
					self->hw_id, out_sw, self->out[i]);
		}
	}

	return 0;
}

void demo_hw_task_free(struct demo_hw_task *self)
{
	if (self->fred_data)
		fred_free(self->fred_data);
}
