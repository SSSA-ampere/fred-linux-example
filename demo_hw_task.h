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

#ifndef DEMO_HW_TASK_H_
#define DEMO_HW_TASK_H_

#include <stdint.h>
#include <assert.h>

typedef uint32_t data_t;

//---------------------------------------------------------------------------------------------

struct demo_hw_task {
	uint32_t hw_id;
	struct fred_data *fred_data;
	struct fred_hw_task *fred_hw_task;

	data_t *in_a;
	data_t *in_b;
	data_t *out;

	data_t (*op)(data_t in_a, data_t in_b);
};

//---------------------------------------------------------------------------------------------

int demo_hw_task_init(struct demo_hw_task *self, uint32_t hw_id, data_t (*op)(data_t, data_t));

int demo_hw_task_accel(struct demo_hw_task *self);

void demo_hw_task_free(struct demo_hw_task *self);

//---------------------------------------------------------------------------------------------

static inline
uint32_t demo_hw_task_get_hw_id(const struct demo_hw_task *self)
{
	assert(self);

	return self->hw_id;
}

static inline
int demo_hw_task_wrapper(void *self)
{
	struct demo_hw_task *task;

	assert(self);

	task = (struct demo_hw_task *)self;

	return demo_hw_task_accel(task);
}

//---------------------------------------------------------------------------------------------

#endif /* DEMO_HW_TASK_H_ */
