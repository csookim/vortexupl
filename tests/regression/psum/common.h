#ifndef _COMMON_H_
#define _COMMON_H_

#define KERNEL_ARG_DEV_MEM_ADDR 0x7ffff000

#ifndef TYPE
#define TYPE int
#endif

typedef struct {
  uint32_t num_points;
  uint32_t num_tasks;
  uint64_t src_addr;  // input
  uint64_t dst_addr;  // output
} kernel_arg_t;

#endif
