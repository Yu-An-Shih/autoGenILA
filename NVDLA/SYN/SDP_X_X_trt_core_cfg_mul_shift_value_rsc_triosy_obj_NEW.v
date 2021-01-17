module SDP_X_X_trt_core_cfg_mul_shift_value_rsc_triosy_obj(nvdla_core_clk, nvdla_core_rstn, cfg_mul_shift_value_rsc_triosy_lz, cfg_mul_shift_value_rsc_triosy_obj_oswt, core_wen, core_wten, cfg_mul_shift_value_rsc_triosy_obj_iswt0, cfg_mul_shift_value_rsc_triosy_obj_bawt);
  (* src = "./vmod/nvdla/sdp/NV_NVDLA_SDP_CORE_x.v:3484" *)
  output cfg_mul_shift_value_rsc_triosy_lz;
  (* src = "./vmod/nvdla/sdp/NV_NVDLA_SDP_CORE_x.v:3489" *)
  output cfg_mul_shift_value_rsc_triosy_obj_bawt;
  (* src = "./vmod/nvdla/sdp/NV_NVDLA_SDP_CORE_x.v:3492" *)
  wire cfg_mul_shift_value_rsc_triosy_obj_bdwt;
  (* src = "./vmod/nvdla/sdp/NV_NVDLA_SDP_CORE_x.v:3491" *)
  wire cfg_mul_shift_value_rsc_triosy_obj_biwt;
  (* src = "./vmod/nvdla/sdp/NV_NVDLA_SDP_CORE_x.v:3488" *)
  input cfg_mul_shift_value_rsc_triosy_obj_iswt0;
  (* src = "./vmod/nvdla/sdp/NV_NVDLA_SDP_CORE_x.v:3485" *)
  input cfg_mul_shift_value_rsc_triosy_obj_oswt;
  (* src = "./vmod/nvdla/sdp/NV_NVDLA_SDP_CORE_x.v:3486" *)
  input core_wen;
  (* src = "./vmod/nvdla/sdp/NV_NVDLA_SDP_CORE_x.v:3487" *)
  input core_wten;
  (* src = "./vmod/nvdla/sdp/NV_NVDLA_SDP_CORE_x.v:3482" *)
  input nvdla_core_clk;
  (* src = "./vmod/nvdla/sdp/NV_NVDLA_SDP_CORE_x.v:3483" *)
  input nvdla_core_rstn;
  (* module_not_derived = 32'd1 *)
  (* src = "./vmod/nvdla/sdp/NV_NVDLA_SDP_CORE_x.v:3498" *)
  SDP_X_X_trt_core_cfg_mul_shift_value_rsc_triosy_obj_cfg_mul_shift_value_rsc_triosy_wait_ctrl X_trt_core_cfg_mul_shift_value_rsc_triosy_obj_cfg_mul_shift_value_rsc_triosy_wait_ctrl_inst (
    .cfg_mul_shift_value_rsc_triosy_obj_bdwt(cfg_mul_shift_value_rsc_triosy_obj_bdwt),
    .cfg_mul_shift_value_rsc_triosy_obj_biwt(cfg_mul_shift_value_rsc_triosy_obj_biwt),
    .cfg_mul_shift_value_rsc_triosy_obj_iswt0(cfg_mul_shift_value_rsc_triosy_obj_iswt0),
    .cfg_mul_shift_value_rsc_triosy_obj_oswt(cfg_mul_shift_value_rsc_triosy_obj_oswt),
    .core_wen(core_wen),
    .core_wten(core_wten)
  );
  (* module_not_derived = 32'd1 *)
  (* src = "./vmod/nvdla/sdp/NV_NVDLA_SDP_CORE_x.v:3508" *)
  SDP_X_X_trt_core_cfg_mul_shift_value_rsc_triosy_obj_cfg_mul_shift_value_rsc_triosy_wait_dp X_trt_core_cfg_mul_shift_value_rsc_triosy_obj_cfg_mul_shift_value_rsc_triosy_wait_dp_inst (
    .cfg_mul_shift_value_rsc_triosy_obj_bawt(cfg_mul_shift_value_rsc_triosy_obj_bawt),
    .cfg_mul_shift_value_rsc_triosy_obj_bdwt(cfg_mul_shift_value_rsc_triosy_obj_bdwt),
    .cfg_mul_shift_value_rsc_triosy_obj_biwt(cfg_mul_shift_value_rsc_triosy_obj_biwt),
    .nvdla_core_clk(nvdla_core_clk),
    .nvdla_core_rstn(nvdla_core_rstn)
  );
  (* src = "./vmod/nvdla/sdp/NV_NVDLA_SDP_CORE_x.v:3494" *)
  \$paramod\SDP_X_mgc_io_sync_v1\valid=0  cfg_mul_shift_value_rsc_triosy_obj (
    .ld(cfg_mul_shift_value_rsc_triosy_obj_biwt),
    .lz(cfg_mul_shift_value_rsc_triosy_lz)
  );
endmodule