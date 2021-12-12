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

#ifndef PERIODIC_TASK_H
#define PERIODIC_TASK_H

#include <pthread.h>

//---------------------------------------------------------------------------------------------

struct periodic_task {
	int	period_ms;
	int (*body)(void *);
	void *args;
	
	pthread_t tid;
	struct timespec next_activation;
};

//---------------------------------------------------------------------------------------------

int periodic_task_init(struct periodic_task *self, int (*body)(void *), void *args,
						int period_ms);

int periodic_task_start(struct periodic_task *self);

int periodic_task_join(struct periodic_task *self);

#endif
