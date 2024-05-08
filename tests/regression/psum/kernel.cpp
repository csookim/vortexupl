#include <stdint.h>
#include <vx_intrinsics.h>
#include <vx_spawn.h>
#include <vx_print.h>
#include "common.h"

void kernel_body(int task_id, kernel_arg_t* __UNIFORM__ arg) {
	// int cid = vx_core_id();
	// vx_printf("launched: cid=%d: task=%d\n", cid, task_id);
	
	int num_points = arg->num_points;
	auto src_ptr = (TYPE*)arg->src_addr;
	auto dst_ptr = (TYPE*)arg->dst_addr;
	int nth = arg->num_tasks;

	int stride = num_points / nth;

	auto ref_value = src_ptr[task_id];

	int sum = 0;
	int start = task_id * stride;
	for (uint32_t i = start; i < start + stride; ++i) {
		sum += src_ptr[i];
	}
	dst_ptr[task_id] = sum;

	// vx_printf("finished: cid=%d: task=%d\n", cid, task_id);
}

int main() {
	kernel_arg_t* arg = (kernel_arg_t*)KERNEL_ARG_DEV_MEM_ADDR;
	vx_spawn_tasks(arg->num_tasks, (vx_spawn_tasks_cb)kernel_body, arg);
	return 0;
}
