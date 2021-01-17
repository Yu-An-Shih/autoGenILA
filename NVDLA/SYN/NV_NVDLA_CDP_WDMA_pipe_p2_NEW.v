module NV_NVDLA_CDP_WDMA_pipe_p2(nvdla_core_clk_orig, nvdla_core_rstn, dma_wr_req_pd, mc_dma_wr_req_vld, mc_int_wr_req_ready, mc_dma_wr_req_rdy, mc_int_wr_req_pd, mc_int_wr_req_valid);
  (* src = "./vmod/nvdla/cdp/NV_NVDLA_CDP_wdma.v:3088" *)
  wire [514:0] _00_;
  (* src = "./vmod/nvdla/cdp/NV_NVDLA_CDP_wdma.v:3081" *)
  wire _01_;
  (* src = "./vmod/nvdla/cdp/NV_NVDLA_CDP_wdma.v:3057" *)
  wire [514:0] _02_;
  (* src = "./vmod/nvdla/cdp/NV_NVDLA_CDP_wdma.v:3046" *)
  wire _03_;
  (* src = "./vmod/nvdla/cdp/NV_NVDLA_CDP_wdma.v:3043" *)
  wire _04_;
  (* src = "./vmod/nvdla/cdp/NV_NVDLA_CDP_wdma.v:3090" *)
  wire _05_;
  (* src = "./vmod/nvdla/cdp/NV_NVDLA_CDP_wdma.v:3043" *)
  wire _06_;
  (* src = "./vmod/nvdla/cdp/NV_NVDLA_CDP_wdma.v:3044" *)
  wire _07_;
  (* src = "./vmod/nvdla/cdp/NV_NVDLA_CDP_wdma.v:3079" *)
  wire _08_;
  (* src = "./vmod/nvdla/cdp/NV_NVDLA_CDP_wdma.v:2822" *)
  input [514:0] dma_wr_req_pd;
  (* src = "./vmod/nvdla/cdp/NV_NVDLA_CDP_wdma.v:2825" *)
  output mc_dma_wr_req_rdy;
  (* src = "./vmod/nvdla/cdp/NV_NVDLA_CDP_wdma.v:2823" *)
  input mc_dma_wr_req_vld;
  (* src = "./vmod/nvdla/cdp/NV_NVDLA_CDP_wdma.v:2826" *)
  output [514:0] mc_int_wr_req_pd;
  (* src = "./vmod/nvdla/cdp/NV_NVDLA_CDP_wdma.v:2824" *)
  input mc_int_wr_req_ready;
  (* src = "./vmod/nvdla/cdp/NV_NVDLA_CDP_wdma.v:2827" *)
  output mc_int_wr_req_valid;
  (* src = "./vmod/nvdla/cdp/NV_NVDLA_CDP_wdma.v:2820" *)
  input nvdla_core_clk_orig;
  (* src = "./vmod/nvdla/cdp/NV_NVDLA_CDP_wdma.v:2821" *)
  input nvdla_core_rstn;
  (* src = "./vmod/nvdla/cdp/NV_NVDLA_CDP_wdma.v:3110" *)
  wire p2_assert_clk;
  (* src = "./vmod/nvdla/cdp/NV_NVDLA_CDP_wdma.v:2831" *)
  reg [514:0] p2_pipe_data;
  (* src = "./vmod/nvdla/cdp/NV_NVDLA_CDP_wdma.v:2832" *)
  wire [514:0] p2_pipe_rand_data;
  (* src = "./vmod/nvdla/cdp/NV_NVDLA_CDP_wdma.v:2833" *)
  reg p2_pipe_rand_ready;
  (* src = "./vmod/nvdla/cdp/NV_NVDLA_CDP_wdma.v:2834" *)
  wire p2_pipe_rand_valid;
  (* src = "./vmod/nvdla/cdp/NV_NVDLA_CDP_wdma.v:2835" *)
  wire p2_pipe_ready;
  (* src = "./vmod/nvdla/cdp/NV_NVDLA_CDP_wdma.v:2836" *)
  wire p2_pipe_ready_bc;
  (* src = "./vmod/nvdla/cdp/NV_NVDLA_CDP_wdma.v:2837" *)
  reg p2_pipe_valid;
  (* src = "./vmod/nvdla/cdp/NV_NVDLA_CDP_wdma.v:2838" *)
  wire p2_skid_catch;
  (* src = "./vmod/nvdla/cdp/NV_NVDLA_CDP_wdma.v:2839" *)
  reg [514:0] p2_skid_data;
  (* src = "./vmod/nvdla/cdp/NV_NVDLA_CDP_wdma.v:2840" *)
  wire [514:0] p2_skid_pipe_data;
  (* src = "./vmod/nvdla/cdp/NV_NVDLA_CDP_wdma.v:2841" *)
  wire p2_skid_pipe_ready;
  (* src = "./vmod/nvdla/cdp/NV_NVDLA_CDP_wdma.v:2842" *)
  wire p2_skid_pipe_valid;
  (* src = "./vmod/nvdla/cdp/NV_NVDLA_CDP_wdma.v:2843" *)
  wire p2_skid_ready;
  (* src = "./vmod/nvdla/cdp/NV_NVDLA_CDP_wdma.v:2844" *)
  wire p2_skid_ready_flop;
  (* src = "./vmod/nvdla/cdp/NV_NVDLA_CDP_wdma.v:2845" *)
  reg p2_skid_valid;
  assign _04_ = mc_dma_wr_req_vld && (* src = "./vmod/nvdla/cdp/NV_NVDLA_CDP_wdma.v:3043" *) p2_pipe_rand_ready;
  assign p2_skid_catch = _04_ && (* src = "./vmod/nvdla/cdp/NV_NVDLA_CDP_wdma.v:3043" *) _06_;
  assign _05_ = p2_pipe_ready_bc && (* src = "./vmod/nvdla/cdp/NV_NVDLA_CDP_wdma.v:3090" *) p2_skid_pipe_valid;
  assign _06_ = ! (* src = "./vmod/nvdla/cdp/NV_NVDLA_CDP_wdma.v:3043" *) p2_pipe_ready_bc;
  assign _07_ = ! (* src = "./vmod/nvdla/cdp/NV_NVDLA_CDP_wdma.v:3044" *) p2_skid_catch;
  assign _08_ = ! (* src = "./vmod/nvdla/cdp/NV_NVDLA_CDP_wdma.v:3079" *) p2_pipe_valid;
  assign p2_pipe_ready_bc = mc_int_wr_req_ready || (* src = "./vmod/nvdla/cdp/NV_NVDLA_CDP_wdma.v:3079" *) _08_;
  always @(posedge nvdla_core_clk_orig)
      p2_pipe_data <= _00_;
  always @(posedge nvdla_core_clk_orig or negedge nvdla_core_rstn)
    if (!nvdla_core_rstn)
      p2_pipe_valid <= 1'b0;
    else
      p2_pipe_valid <= _01_;
  always @(posedge nvdla_core_clk_orig)
      p2_skid_data <= _02_;
  always @(posedge nvdla_core_clk_orig or negedge nvdla_core_rstn)
    if (!nvdla_core_rstn)
      p2_pipe_rand_ready <= 1'b1;
    else
      p2_pipe_rand_ready <= p2_skid_ready;
  always @(posedge nvdla_core_clk_orig or negedge nvdla_core_rstn)
    if (!nvdla_core_rstn)
      p2_skid_valid <= 1'b0;
    else
      p2_skid_valid <= _03_;
  assign p2_skid_ready = p2_skid_valid ? (* src = "./vmod/nvdla/cdp/NV_NVDLA_CDP_wdma.v:3044" *) p2_pipe_ready_bc : _07_;
  assign _03_ = p2_skid_valid ? (* src = "./vmod/nvdla/cdp/NV_NVDLA_CDP_wdma.v:3052" *) _06_ : p2_skid_catch;
  assign _02_ = p2_skid_catch ? (* src = "./vmod/nvdla/cdp/NV_NVDLA_CDP_wdma.v:3059" *) dma_wr_req_pd : p2_skid_data;
  assign p2_skid_pipe_valid = p2_pipe_rand_ready ? (* src = "./vmod/nvdla/cdp/NV_NVDLA_CDP_wdma.v:3069" *) mc_dma_wr_req_vld : p2_skid_valid;
  assign p2_skid_pipe_data = p2_pipe_rand_ready ? (* src = "./vmod/nvdla/cdp/NV_NVDLA_CDP_wdma.v:3071" *) dma_wr_req_pd : p2_skid_data;
  assign _01_ = p2_pipe_ready_bc ? (* src = "./vmod/nvdla/cdp/NV_NVDLA_CDP_wdma.v:3085" *) p2_skid_pipe_valid : 1'b1;
  assign _00_ = _05_ ? (* src = "./vmod/nvdla/cdp/NV_NVDLA_CDP_wdma.v:3090" *) p2_skid_pipe_data : p2_pipe_data;
  assign mc_dma_wr_req_rdy = p2_pipe_rand_ready;
  assign mc_int_wr_req_pd = p2_pipe_data;
  assign mc_int_wr_req_valid = p2_pipe_valid;
  assign p2_assert_clk = nvdla_core_clk_orig;
  assign p2_pipe_rand_data = dma_wr_req_pd;
  assign p2_pipe_rand_valid = mc_dma_wr_req_vld;
  assign p2_pipe_ready = mc_int_wr_req_ready;
  assign p2_skid_pipe_ready = p2_pipe_ready_bc;
  assign p2_skid_ready_flop = p2_pipe_rand_ready;
endmodule