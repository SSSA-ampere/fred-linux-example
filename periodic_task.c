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

#include <time.h>
#include "periodic_task.h"

// Returns
//  1 if t1 > t2
// -1 if t1 < t2
//  0 if t1 == t2
static inline
int time_compare_(const struct timespec *t1, const struct timespec *t2)
{
	if (t1->tv_sec > t2->tv_sec)
		return 1;
	if (t1->tv_sec < t2->tv_sec)
		return -1;
	
	if (t1->tv_nsec > t2->tv_nsec)
		return 1;
	if (t1->tv_nsec < t2->tv_nsec)
		return -1;
	
	return 0;
}

static inline
void time_add_ms_(struct timespec *t, int ms)
{
	t->tv_sec += ms / 1000;
	t->tv_nsec += (ms % 1000) * 1000000;
	
	if (t->tv_nsec > 1000000000) {
		t->tv_nsec -= 1000000000;
		t->tv_sec += 1;
	}
}

static inline
void time_copy_(struct timespec *td, const struct timespec *ts)
{
	td->tv_sec = ts->tv_sec;
	td->tv_nsec = ts->tv_nsec;
}

static inline
void set_period_(struct periodic_task *self)
{
	struct timespec t_now;
	
	// Grab task's start time and compute next activation
	clock_gettime(CLOCK_MONOTONIC, &t_now);
	time_copy_(&self->next_activation, &t_now);
	
	time_add_ms_(&self->next_activation, self->period_ms);
}

static inline
void wait_for_period_(struct periodic_task *self)
{
	struct timespec t_now;
	int overrun;

	// Sleep until next activation
	clock_nanosleep(CLOCK_MONOTONIC, TIMER_ABSTIME, &self->next_activation, NULL);
	
	// Compute next activation time (based on start time)
	clock_gettime(CLOCK_MONOTONIC, &t_now);
	do {
		time_add_ms_(&self->next_activation, self->period_ms);
		overrun = time_compare_(&t_now, &self->next_activation);
	} while (overrun == 1);
}

static inline
void *periodic_body_(void *self)
{
	struct periodic_task *task;
	int retval;

	task = (struct periodic_task *)self;

	// Set first period
	set_period_(task);
	
	while (1) {
		// Call the task body function
		retval = task->body(task->args);
		if (retval)
			break;
		
		// wait until end of current period and set next period
		wait_for_period_(task);
	}

	return NULL;
}

int periodic_task_init(struct periodic_task *self, int (*body)(void *), void *args,
						int period_ms)
{
	if (!body || period_ms <= 0)
		return -1;

	self->body = body;
	self->args = args;
	self->period_ms = period_ms;
	
	return 0;
}

int periodic_task_start(struct periodic_task *self)
{
	return pthread_create(&self->tid, NULL, periodic_body_, self);
}

int periodic_task_join(struct periodic_task *self)
{
	return pthread_join(self->tid, NULL);
}

