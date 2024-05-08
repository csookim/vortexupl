# utils.cpp

```
// @@@@@
  uint64_t std_warps = 0;
  uint64_t act_warps = 0;
// @@@@@

// @@@@@
      {
        uint64_t std_warps_per_core = get_csr_64(staging_buf.data(), VX_CSR_MPM_STD_WP);
        uint64_t act_warps_per_core = get_csr_64(staging_buf.data(), VX_CSR_MPM_ACT_WP);
        if (num_cores > 1) fprintf(stream, "PERF: core%d: stalled warps=%ld\n", core_id, std_warps_per_core);
        if (num_cores > 1) fprintf(stream, "PERF: core%d: active warps=%ld\n", core_id, act_warps_per_core);
        std_warps += std_warps_per_core;
        act_warps += act_warps_per_core;
      }
// @@@@@

// @@@@@
    float warp_efficiency = std_warps / act_warps;
    fprintf(stream, "PERF: warp efficiency=%ld\n", warp_efficiency);
// @@@@@
```

# VX_types.vh
```
// @@@@@
`define VX_CSR_MPM_STD_WP               12'hB12
`define VX_CSR_MPM_STD_WP_H             12'hB92
`define VX_CSR_MPM_ACT_WP               12'hB13
`define VX_CSR_MPM_ACT_WP_H             12'hB93
// @@@@@
```

# VX_csr_data.sv
```
// @@@@@
                        `VX_CSR_MPM_STD_WP          : read_data_ro_r = pipeline_perf_if.stalled_warps[31:0];
                        `VX_CSR_MPM_STD_WP_H        : read_data_ro_r = 32'(pipeline_perf_if.stalled_warps[`PERF_CTR_BITS-1:32]);
                        `VX_CSR_MPM_ACT_WP          : read_data_ro_r = pipeline_perf_if.active_warps[31:0];
                        `VX_CSR_MPM_ACT_WP_H        : read_data_ro_r = 32'(pipeline_perf_if.active_warps[`PERF_CTR_BITS-1:32]);
// @@@@@
```

# VX_pipeline_perf_if.sv

```
// @@@@@
    wire [`PERF_CTR_BITS-1:0] stalled_warps;
    wire [`PERF_CTR_BITS-1:0] active_warps;
// @@@@@

// @@@@@
        output stalled_warps,
        output active_warps,
// @@@@@

// @@@@@
        input stalled_warps,
        input active_warps,
// @@@@@
```

# VX_schedule.sv

```
// @@@@@
`ifdef PERF_ENABLE
    reg [`PERF_CTR_BITS-1:0] sum_stalled;
    reg [`PERF_CTR_BITS-1:0] sum_active;

    always @(posedge clk) begin
        if (reset) begin
            sum_stalled <= '0;
            sum_active <= '0;
        end else begin
            integer i;
            for (i=0; i<`PERF_CTR_BITS; i=i+1) begin
                sum_stalled <= sum_stalled + `PERF_CTR_BITS'(stalled_warps[i]);
                sum_active <= sum_active + `PERF_CTR_BITS'(active_warps[i]);
            end
        end
    end
    assign perf_schedule_if.stalled_warps = sum_stalled;
    assign perf_schedule_if.active_warps = sum_active;
`endif
// @@@@@
```


