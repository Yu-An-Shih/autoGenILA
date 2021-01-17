module HLS_fp16_to_fp17_core_core_fsm(nvdla_core_clk, nvdla_core_rstn, core_wen, fsm_output);
  (* src = "./vmod/vlibs/HLS_fp16_to_fp17.v:217" *)
  wire _0_;
  (* src = "./vmod/vlibs/HLS_fp16_to_fp17.v:193" *)
  input core_wen;
  (* src = "./vmod/vlibs/HLS_fp16_to_fp17.v:194" *)
  output [1:0] fsm_output;
  (* src = "./vmod/vlibs/HLS_fp16_to_fp17.v:191" *)
  input nvdla_core_clk;
  (* src = "./vmod/vlibs/HLS_fp16_to_fp17.v:192" *)
  input nvdla_core_rstn;
  (* src = "./vmod/vlibs/HLS_fp16_to_fp17.v:200" *)
  reg state_var;
  (* src = "./vmod/vlibs/HLS_fp16_to_fp17.v:201" *)
  wire state_var_NS;
  always @(posedge nvdla_core_clk or negedge nvdla_core_rstn)
    if (!nvdla_core_rstn)
      state_var <= 1'b0;
    else
      state_var <= _0_;
  assign _0_ = core_wen ? (* src = "./vmod/vlibs/HLS_fp16_to_fp17.v:221" *) 1'b1 : state_var;
  assign fsm_output = state_var ? (* full_case = 32'd1 *) (* src = "./vmod/vlibs/HLS_fp16_to_fp17.v:206|./vmod/vlibs/HLS_fp16_to_fp17.v:205" *) 2'b10 : 2'b01;
  assign state_var_NS = 1'b1;
endmodule