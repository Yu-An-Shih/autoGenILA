module HLS_fp17_add_core_chn_o_rsci_chn_o_wait_ctrl(nvdla_core_clk, nvdla_core_rstn, chn_o_rsci_oswt, core_wen, core_wten, chn_o_rsci_iswt0, chn_o_rsci_ld_core_psct, chn_o_rsci_biwt, chn_o_rsci_bdwt, chn_o_rsci_ld_core_sct, chn_o_rsci_vd);
  (* src = "./vmod/vlibs/HLS_fp17_add.v:429" *)
  wire _00_;
  (* src = "./vmod/vlibs/HLS_fp17_add.v:424" *)
  wire _01_;
  (* src = "./vmod/vlibs/HLS_fp17_add.v:434" *)
  wire _02_;
  (* src = "./vmod/vlibs/HLS_fp17_add.v:434" *)
  wire _03_;
  (* src = "./vmod/vlibs/HLS_fp17_add.v:416" *)
  output chn_o_rsci_bdwt;
  (* src = "./vmod/vlibs/HLS_fp17_add.v:415" *)
  output chn_o_rsci_biwt;
  (* src = "./vmod/vlibs/HLS_fp17_add.v:422" *)
  reg chn_o_rsci_icwt;
  (* src = "./vmod/vlibs/HLS_fp17_add.v:413" *)
  input chn_o_rsci_iswt0;
  (* src = "./vmod/vlibs/HLS_fp17_add.v:414" *)
  input chn_o_rsci_ld_core_psct;
  (* src = "./vmod/vlibs/HLS_fp17_add.v:417" *)
  output chn_o_rsci_ld_core_sct;
  (* src = "./vmod/vlibs/HLS_fp17_add.v:420" *)
  wire chn_o_rsci_ogwt;
  (* src = "./vmod/vlibs/HLS_fp17_add.v:410" *)
  input chn_o_rsci_oswt;
  (* src = "./vmod/vlibs/HLS_fp17_add.v:421" *)
  wire chn_o_rsci_pdswt0;
  (* src = "./vmod/vlibs/HLS_fp17_add.v:418" *)
  input chn_o_rsci_vd;
  (* src = "./vmod/vlibs/HLS_fp17_add.v:411" *)
  input core_wen;
  (* src = "./vmod/vlibs/HLS_fp17_add.v:412" *)
  input core_wten;
  (* src = "./vmod/vlibs/HLS_fp17_add.v:408" *)
  input nvdla_core_clk;
  (* src = "./vmod/vlibs/HLS_fp17_add.v:409" *)
  input nvdla_core_rstn;
  assign chn_o_rsci_pdswt0 = _01_ & (* src = "./vmod/vlibs/HLS_fp17_add.v:424" *) chn_o_rsci_iswt0;
  assign chn_o_rsci_biwt = chn_o_rsci_ogwt & (* src = "./vmod/vlibs/HLS_fp17_add.v:425" *) chn_o_rsci_vd;
  assign chn_o_rsci_bdwt = chn_o_rsci_oswt & (* src = "./vmod/vlibs/HLS_fp17_add.v:427" *) core_wen;
  assign chn_o_rsci_ld_core_sct = chn_o_rsci_ld_core_psct & (* src = "./vmod/vlibs/HLS_fp17_add.v:428" *) chn_o_rsci_ogwt;
  assign _01_ = ~ (* src = "./vmod/vlibs/HLS_fp17_add.v:424" *) core_wten;
  assign _02_ = ~ (* src = "./vmod/vlibs/HLS_fp17_add.v:434" *) chn_o_rsci_ogwt;
  assign _00_ = ~ (* src = "./vmod/vlibs/HLS_fp17_add.v:434" *) _03_;
  assign chn_o_rsci_ogwt = chn_o_rsci_pdswt0 | (* src = "./vmod/vlibs/HLS_fp17_add.v:426" *) chn_o_rsci_icwt;
  assign _03_ = _02_ | (* src = "./vmod/vlibs/HLS_fp17_add.v:434" *) chn_o_rsci_biwt;
  always @(posedge nvdla_core_clk or negedge nvdla_core_rstn)
    if (!nvdla_core_rstn)
      chn_o_rsci_icwt <= 1'b0;
    else
      chn_o_rsci_icwt <= _00_;
endmodule