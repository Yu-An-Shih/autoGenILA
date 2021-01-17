module NV_NVDLA_MCIF_WRITE_IG_SPT_dfifo(nvdla_core_clk, nvdla_core_rstn, dfifo_wr_count, dfifo_wr_pvld, dfifo_wr_pd, dfifo_rd_prdy, dfifo_rd_pvld, dfifo_rd_pd, pwrbus_ram_pd);
  (* src = "./vmod/nvdla/nocif/NV_NVDLA_MCIF_WRITE_IG_spt.v:543" *)
  wire [2:0] _00_;
  (* src = "./vmod/nvdla/nocif/NV_NVDLA_MCIF_WRITE_IG_spt.v:582" *)
  wire [2:0] _01_;
  (* src = "./vmod/nvdla/nocif/NV_NVDLA_MCIF_WRITE_IG_spt.v:582" *)
  wire _02_;
  (* src = "./vmod/nvdla/nocif/NV_NVDLA_MCIF_WRITE_IG_spt.v:582" *)
  wire _03_;
  (* src = "./vmod/nvdla/nocif/NV_NVDLA_MCIF_WRITE_IG_spt.v:516" *)
  wire [2:0] _04_;
  (* src = "./vmod/nvdla/nocif/NV_NVDLA_MCIF_WRITE_IG_spt.v:495" *)
  wire [2:0] _05_;
  (* src = "./vmod/nvdla/nocif/NV_NVDLA_MCIF_WRITE_IG_spt.v:492" *)
  wire [2:0] _06_;
  (* src = "./vmod/nvdla/nocif/NV_NVDLA_MCIF_WRITE_IG_spt.v:521" *)
  wire [2:0] _07_;
  (* src = "./vmod/nvdla/nocif/NV_NVDLA_MCIF_WRITE_IG_spt.v:542" *)
  wire [2:0] _08_;
  (* src = "./vmod/nvdla/nocif/NV_NVDLA_MCIF_WRITE_IG_spt.v:573" *)
  wire [2:0] _09_;
  (* src = "./vmod/nvdla/nocif/NV_NVDLA_MCIF_WRITE_IG_spt.v:521" *)
  wire _10_;
  (* src = "./vmod/nvdla/nocif/NV_NVDLA_MCIF_WRITE_IG_spt.v:542" *)
  wire _11_;
  (* src = "./vmod/nvdla/nocif/NV_NVDLA_MCIF_WRITE_IG_spt.v:627" *)
  wire _12_;
  (* src = "./vmod/nvdla/nocif/NV_NVDLA_MCIF_WRITE_IG_spt.v:588" *)
  wire _13_;
  (* src = "./vmod/nvdla/nocif/NV_NVDLA_MCIF_WRITE_IG_spt.v:627" *)
  wire _14_;
  (* src = "./vmod/nvdla/nocif/NV_NVDLA_MCIF_WRITE_IG_spt.v:627" *)
  wire _15_;
  (* src = "./vmod/nvdla/nocif/NV_NVDLA_MCIF_WRITE_IG_spt.v:627" *)
  wire _16_;
  wire _17_;
  (* src = "./vmod/nvdla/nocif/NV_NVDLA_MCIF_WRITE_IG_spt.v:491" *)
  wire [2:0] _18_;
  (* src = "./vmod/nvdla/nocif/NV_NVDLA_MCIF_WRITE_IG_spt.v:572" *)
  wire [2:0] _19_;
  (* src = "./vmod/nvdla/nocif/NV_NVDLA_MCIF_WRITE_IG_spt.v:521" *)
  wire [2:0] _20_;
  (* src = "./vmod/nvdla/nocif/NV_NVDLA_MCIF_WRITE_IG_spt.v:526" *)
  reg [2:0] dfifo_rd_adr;
  (* src = "./vmod/nvdla/nocif/NV_NVDLA_MCIF_WRITE_IG_spt.v:569" *)
  reg [2:0] dfifo_rd_count;
  (* src = "./vmod/nvdla/nocif/NV_NVDLA_MCIF_WRITE_IG_spt.v:468" *)
  output [513:0] dfifo_rd_pd;
  (* src = "./vmod/nvdla/nocif/NV_NVDLA_MCIF_WRITE_IG_spt.v:466" *)
  input dfifo_rd_prdy;
  (* src = "./vmod/nvdla/nocif/NV_NVDLA_MCIF_WRITE_IG_spt.v:467" *)
  output dfifo_rd_pvld;
  reg dfifo_rd_pvld;
  (* src = "./vmod/nvdla/nocif/NV_NVDLA_MCIF_WRITE_IG_spt.v:567" *)
  reg dfifo_rd_pvld_int;
  (* src = "./vmod/nvdla/nocif/NV_NVDLA_MCIF_WRITE_IG_spt.v:514" *)
  reg [2:0] dfifo_wr_adr;
  (* src = "./vmod/nvdla/nocif/NV_NVDLA_MCIF_WRITE_IG_spt.v:463" *)
  output [2:0] dfifo_wr_count;
  reg [2:0] dfifo_wr_count;
  (* src = "./vmod/nvdla/nocif/NV_NVDLA_MCIF_WRITE_IG_spt.v:465" *)
  input [513:0] dfifo_wr_pd;
  (* src = "./vmod/nvdla/nocif/NV_NVDLA_MCIF_WRITE_IG_spt.v:464" *)
  input dfifo_wr_pvld;
  (* src = "./vmod/nvdla/nocif/NV_NVDLA_MCIF_WRITE_IG_spt.v:461" *)
  input nvdla_core_clk;
  (* src = "./vmod/nvdla/nocif/NV_NVDLA_MCIF_WRITE_IG_spt.v:482" *)
  wire nvdla_core_clk_mgated;
  (* src = "./vmod/nvdla/nocif/NV_NVDLA_MCIF_WRITE_IG_spt.v:481" *)
  wire nvdla_core_clk_mgated_enable;
  (* src = "./vmod/nvdla/nocif/NV_NVDLA_MCIF_WRITE_IG_spt.v:462" *)
  input nvdla_core_rstn;
  (* src = "./vmod/nvdla/nocif/NV_NVDLA_MCIF_WRITE_IG_spt.v:469" *)
  input [31:0] pwrbus_ram_pd;
  (* src = "./vmod/nvdla/nocif/NV_NVDLA_MCIF_WRITE_IG_spt.v:527" *)
  wire ram_we;
  (* src = "./vmod/nvdla/nocif/NV_NVDLA_MCIF_WRITE_IG_spt.v:542" *)
  wire [2:0] rd_adr_next_popping;
  (* src = "./vmod/nvdla/nocif/NV_NVDLA_MCIF_WRITE_IG_spt.v:576" *)
  wire [2:0] rd_count_next;
  (* src = "./vmod/nvdla/nocif/NV_NVDLA_MCIF_WRITE_IG_spt.v:573" *)
  wire [2:0] rd_count_next_no_rd_popping;
  (* src = "./vmod/nvdla/nocif/NV_NVDLA_MCIF_WRITE_IG_spt.v:579" *)
  wire rd_count_next_no_rd_popping_not_0;
  (* src = "./vmod/nvdla/nocif/NV_NVDLA_MCIF_WRITE_IG_spt.v:580" *)
  wire rd_count_next_not_0;
  (* src = "./vmod/nvdla/nocif/NV_NVDLA_MCIF_WRITE_IG_spt.v:571" *)
  wire [2:0] rd_count_next_rd_popping;
  (* src = "./vmod/nvdla/nocif/NV_NVDLA_MCIF_WRITE_IG_spt.v:578" *)
  wire rd_count_next_rd_popping_not_0;
  (* src = "./vmod/nvdla/nocif/NV_NVDLA_MCIF_WRITE_IG_spt.v:541" *)
  wire rd_popping;
  (* src = "./vmod/nvdla/nocif/NV_NVDLA_MCIF_WRITE_IG_spt.v:562" *)
  wire rd_pushing;
  (* src = "./vmod/nvdla/nocif/NV_NVDLA_MCIF_WRITE_IG_spt.v:493" *)
  wire [2:0] wr_count_next;
  (* src = "./vmod/nvdla/nocif/NV_NVDLA_MCIF_WRITE_IG_spt.v:492" *)
  wire [2:0] wr_count_next_no_wr_popping;
  (* src = "./vmod/nvdla/nocif/NV_NVDLA_MCIF_WRITE_IG_spt.v:491" *)
  wire [2:0] wr_count_next_wr_popping;
  (* src = "./vmod/nvdla/nocif/NV_NVDLA_MCIF_WRITE_IG_spt.v:489" *)
  wire wr_popping;
  (* src = "./vmod/nvdla/nocif/NV_NVDLA_MCIF_WRITE_IG_spt.v:510" *)
  wire wr_pushing;
  (* src = "./vmod/nvdla/nocif/NV_NVDLA_MCIF_WRITE_IG_spt.v:487" *)
  wire wr_reserving;
  assign _06_ = dfifo_wr_count + (* src = "./vmod/nvdla/nocif/NV_NVDLA_MCIF_WRITE_IG_spt.v:492" *) 1'b1;
  assign _07_ = dfifo_wr_adr + (* src = "./vmod/nvdla/nocif/NV_NVDLA_MCIF_WRITE_IG_spt.v:521" *) 1'b1;
  assign _08_ = dfifo_rd_adr + (* src = "./vmod/nvdla/nocif/NV_NVDLA_MCIF_WRITE_IG_spt.v:542" *) 1'b1;
  assign _09_ = dfifo_rd_count + (* src = "./vmod/nvdla/nocif/NV_NVDLA_MCIF_WRITE_IG_spt.v:573" *) 1'b1;
  assign _10_ = dfifo_wr_adr == (* src = "./vmod/nvdla/nocif/NV_NVDLA_MCIF_WRITE_IG_spt.v:521" *) 3'b100;
  assign _11_ = dfifo_rd_adr == (* src = "./vmod/nvdla/nocif/NV_NVDLA_MCIF_WRITE_IG_spt.v:542" *) 3'b100;
  assign rd_popping = dfifo_rd_pvld_int && (* src = "./vmod/nvdla/nocif/NV_NVDLA_MCIF_WRITE_IG_spt.v:568" *) dfifo_rd_prdy;
  assign _12_ = dfifo_rd_pvld && (* src = "./vmod/nvdla/nocif/NV_NVDLA_MCIF_WRITE_IG_spt.v:627" *) dfifo_rd_prdy;
  assign _13_ = dfifo_wr_pvld || (* src = "./vmod/nvdla/nocif/NV_NVDLA_MCIF_WRITE_IG_spt.v:588" *) rd_popping;
  assign _14_ = _13_ || (* src = "./vmod/nvdla/nocif/NV_NVDLA_MCIF_WRITE_IG_spt.v:627" *) dfifo_wr_pvld;
  assign _15_ = _13_ || (* src = "./vmod/nvdla/nocif/NV_NVDLA_MCIF_WRITE_IG_spt.v:627" *) _12_;
  assign _16_ = _14_ || (* src = "./vmod/nvdla/nocif/NV_NVDLA_MCIF_WRITE_IG_spt.v:627" *) _15_;
  assign nvdla_core_clk_mgated_enable = _16_ || (* src = "./vmod/nvdla/nocif/NV_NVDLA_MCIF_WRITE_IG_spt.v:627" *) dfifo_wr_pvld;
  assign rd_count_next_rd_popping_not_0 = | (* src = "./vmod/nvdla/nocif/NV_NVDLA_MCIF_WRITE_IG_spt.v:578" *) rd_count_next_rd_popping;
  assign rd_count_next_no_rd_popping_not_0 = | (* src = "./vmod/nvdla/nocif/NV_NVDLA_MCIF_WRITE_IG_spt.v:579" *) rd_count_next_no_rd_popping;
  always @(posedge nvdla_core_clk_mgated or negedge nvdla_core_rstn)
    if (!nvdla_core_rstn)
      dfifo_rd_pvld <= 1'b0;
    else
      dfifo_rd_pvld <= _02_;
  always @(posedge nvdla_core_clk_mgated or negedge nvdla_core_rstn)
    if (!nvdla_core_rstn)
      dfifo_rd_pvld_int <= 1'b0;
    else
      dfifo_rd_pvld_int <= _03_;
  always @(posedge nvdla_core_clk_mgated or negedge nvdla_core_rstn)
    if (!nvdla_core_rstn)
      dfifo_rd_count <= 3'b000;
    else
      dfifo_rd_count <= _01_;
  always @(posedge nvdla_core_clk_mgated or negedge nvdla_core_rstn)
    if (!nvdla_core_rstn)
      dfifo_rd_adr <= 3'b000;
    else
      dfifo_rd_adr <= _00_;
  always @(posedge nvdla_core_clk_mgated or negedge nvdla_core_rstn)
    if (!nvdla_core_rstn)
      dfifo_wr_adr <= 3'b000;
    else
      dfifo_wr_adr <= _04_;
  always @(posedge nvdla_core_clk_mgated or negedge nvdla_core_rstn)
    if (!nvdla_core_rstn)
      dfifo_wr_count <= 3'b000;
    else
      dfifo_wr_count <= _05_;
  assign _01_ = _13_ ? (* src = "./vmod/nvdla/nocif/NV_NVDLA_MCIF_WRITE_IG_spt.v:588" *) rd_count_next : dfifo_rd_count;
  assign _03_ = _13_ ? (* src = "./vmod/nvdla/nocif/NV_NVDLA_MCIF_WRITE_IG_spt.v:606" *) rd_count_next_not_0 : dfifo_rd_pvld_int;
  assign _02_ = _13_ ? (* src = "./vmod/nvdla/nocif/NV_NVDLA_MCIF_WRITE_IG_spt.v:597" *) rd_count_next_not_0 : dfifo_rd_pvld;
  assign _00_ = rd_popping ? (* src = "./vmod/nvdla/nocif/NV_NVDLA_MCIF_WRITE_IG_spt.v:547" *) rd_adr_next_popping : dfifo_rd_adr;
  assign _04_ = dfifo_wr_pvld ? (* src = "./vmod/nvdla/nocif/NV_NVDLA_MCIF_WRITE_IG_spt.v:520" *) _20_ : dfifo_wr_adr;
  assign _05_ = _17_ ? (* src = "./vmod/nvdla/nocif/NV_NVDLA_MCIF_WRITE_IG_spt.v:499" *) wr_count_next : dfifo_wr_count;
  assign _18_ = dfifo_wr_count - (* src = "./vmod/nvdla/nocif/NV_NVDLA_MCIF_WRITE_IG_spt.v:491" *) 1'b1;
  assign _19_ = dfifo_rd_count - (* src = "./vmod/nvdla/nocif/NV_NVDLA_MCIF_WRITE_IG_spt.v:572" *) 1'b1;
  assign wr_count_next_wr_popping = dfifo_wr_pvld ? (* src = "./vmod/nvdla/nocif/NV_NVDLA_MCIF_WRITE_IG_spt.v:491" *) dfifo_wr_count : _18_;
  assign wr_count_next_no_wr_popping = dfifo_wr_pvld ? (* src = "./vmod/nvdla/nocif/NV_NVDLA_MCIF_WRITE_IG_spt.v:492" *) _06_ : dfifo_wr_count;
  assign wr_count_next = rd_popping ? (* src = "./vmod/nvdla/nocif/NV_NVDLA_MCIF_WRITE_IG_spt.v:494" *) wr_count_next_wr_popping : wr_count_next_no_wr_popping;
  assign _20_ = _10_ ? (* src = "./vmod/nvdla/nocif/NV_NVDLA_MCIF_WRITE_IG_spt.v:521" *) 3'b000 : _07_;
  assign rd_adr_next_popping = _11_ ? (* src = "./vmod/nvdla/nocif/NV_NVDLA_MCIF_WRITE_IG_spt.v:542" *) 3'b000 : _08_;
  assign rd_count_next_rd_popping = dfifo_wr_pvld ? (* src = "./vmod/nvdla/nocif/NV_NVDLA_MCIF_WRITE_IG_spt.v:572" *) dfifo_rd_count : _19_;
  assign rd_count_next_no_rd_popping = dfifo_wr_pvld ? (* src = "./vmod/nvdla/nocif/NV_NVDLA_MCIF_WRITE_IG_spt.v:574" *) _09_ : dfifo_rd_count;
  assign rd_count_next = rd_popping ? (* src = "./vmod/nvdla/nocif/NV_NVDLA_MCIF_WRITE_IG_spt.v:577" *) rd_count_next_rd_popping : rd_count_next_no_rd_popping;
  assign rd_count_next_not_0 = rd_popping ? (* src = "./vmod/nvdla/nocif/NV_NVDLA_MCIF_WRITE_IG_spt.v:581" *) rd_count_next_rd_popping_not_0 : rd_count_next_no_rd_popping_not_0;
  assign _17_ = dfifo_wr_pvld ^ (* src = "./vmod/nvdla/nocif/NV_NVDLA_MCIF_WRITE_IG_spt.v:499" *) rd_popping;
  (* module_not_derived = 32'd1 *)
  (* src = "./vmod/nvdla/nocif/NV_NVDLA_MCIF_WRITE_IG_spt.v:483" *)
  NV_CLK_gate_power nvdla_core_clk_mgate (
    .clk(nvdla_core_clk),
    .clk_en(nvdla_core_clk_mgated_enable),
    .clk_gated(nvdla_core_clk_mgated),
    .reset_(nvdla_core_rstn)
  );
  (* module_not_derived = 32'd1 *)
  (* src = "./vmod/nvdla/nocif/NV_NVDLA_MCIF_WRITE_IG_spt.v:532" *)
  NV_NVDLA_MCIF_WRITE_IG_SPT_dfifo_flopram_rwsa_5x514 ram (
    .clk(nvdla_core_clk_mgated),
    .di(dfifo_wr_pd),
    .dout(dfifo_rd_pd),
    .pwrbus_ram_pd(pwrbus_ram_pd),
    .ra(dfifo_rd_adr),
    .wa(dfifo_wr_adr),
    .we(dfifo_wr_pvld)
  );
  assign ram_we = dfifo_wr_pvld;
  assign rd_pushing = dfifo_wr_pvld;
  assign wr_popping = rd_popping;
  assign wr_pushing = dfifo_wr_pvld;
  assign wr_reserving = dfifo_wr_pvld;
endmodule