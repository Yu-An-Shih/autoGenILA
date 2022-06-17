module sha1_core(clk, reset_n, init, next, block, ready, digest, digest_valid);
  wire [31:0] _000_;
  wire [31:0] _001_;
  wire [31:0] _002_;
  wire [31:0] _003_;
  wire [31:0] _004_;
  wire [31:0] _005_;
  wire [31:0] _006_;
  wire [31:0] _007_;
  wire [31:0] _008_;
  wire _009_;
  wire [31:0] _010_;
  wire [6:0] _011_;
  wire [1:0] _012_;
  wire [30:0] _013_;
  wire [31:0] _014_;
  wire [31:0] _015_;
  wire [28:0] _016_;
  wire [31:0] _017_;
  wire [31:0] _018_;
  wire [31:0] _019_;
  wire [31:0] _020_;
  wire [31:0] _021_;
  wire [31:0] _022_;
  wire [31:0] _023_;
  wire [31:0] _024_;
  wire [31:0] _025_;
  wire [31:0] _026_;
  wire [31:0] _027_;
  wire [31:0] _028_;
  wire [31:0] _029_;
  wire _030_;
  wire [31:0] _031_;
  wire [31:0] _032_;
  wire [31:0] _033_;
  wire [31:0] _034_;
  wire [1:0] _035_;
  wire _036_;
  wire [31:0] _037_;
  wire [31:0] _038_;
  wire [31:0] _039_;
  wire [31:0] _040_;
  wire [31:0] _041_;
  wire [31:0] _042_;
  wire [31:0] _043_;
  wire [31:0] _044_;
  wire [31:0] _045_;
  wire [31:0] _046_;
  wire [31:0] _047_;
  wire [6:0] _048_;
  wire [31:0] _049_;
  wire [31:0] _050_;
  wire _051_;
  wire _052_;
  wire _053_;
  wire _054_;
  wire _055_;
  wire _056_;
  wire _057_;
  wire _058_;
  wire [31:0] _059_;
  wire [31:0] _060_;
  wire [31:0] _061_;
  wire [31:0] _062_;
  wire [1:0] _063_;
  wire _064_;
  wire [6:0] _065_;
  wire [31:0] _066_;
  wire [31:0] _067_;
  wire [31:0] _068_;
  wire [31:0] _069_;
  wire [31:0] _070_;
  wire [31:0] _071_;
  wire [31:0] _072_;
  wire [31:0] _073_;
  wire [31:0] _074_;
  wire [31:0] _075_;
  wire [31:0] _076_;
  wire [31:0] _077_;
  wire [31:0] _078_;
  wire [31:0] _079_;
  wire [31:0] _080_;
  wire [31:0] H0_new;
  reg [31:0] H0_reg;
  wire [31:0] H1_new;
  reg [31:0] H1_reg;
  wire [31:0] H2_new;
  reg [31:0] H2_reg;
  wire [31:0] H3_new;
  reg [31:0] H3_reg;
  wire [31:0] H4_new;
  reg [31:0] H4_reg;
  wire H_we;
  wire a_e_we;
  wire [31:0] a_new;
  reg [31:0] a_reg;
  wire [31:0] b_new;
  reg [31:0] b_reg;
  input [511:0] block;
  wire [31:0] c_new;
  reg [31:0] c_reg;
  input clk;
  wire [31:0] d_new;
  reg [31:0] d_reg;
  output [159:0] digest;
  wire digest_init;
  wire digest_update;
  output digest_valid;
  wire digest_valid_new;
  reg digest_valid_reg;
  wire digest_valid_we;
  wire [31:0] e_new;
  reg [31:0] e_reg;
  wire first_block;
  input init;
  input next;
  output ready;
  wire ready_flag;
  input reset_n;
  wire round_ctr_inc;
  wire [6:0] round_ctr_new;
  reg [6:0] round_ctr_reg;
  wire round_ctr_rst;
  wire round_ctr_we;
  wire [1:0] sha1_ctrl_new;
  reg [1:0] sha1_ctrl_reg;
  wire sha1_ctrl_we;
  wire state_init;
  wire state_update;
  wire [31:0] w;
  wire w_init;
  wire w_next;
  assign _039_ = H0_reg + a_reg;
  assign _040_ = H1_reg + b_reg;
  assign _041_ = H2_reg + c_reg;
  assign _042_ = H3_reg + d_reg;
  assign _043_ = H4_reg + e_reg;
  assign _044_ = { a_reg[26:0], a_reg[31:27] } + e_reg;
  assign _045_ = _044_ + _028_;
  assign _046_ = _045_ + _029_;
  assign _047_ = _046_ + w;
  assign _048_ = round_ctr_reg + 1'b1;
  assign _049_ = b_reg & c_reg;
  assign _050_ = _059_ & d_reg;
  assign _036_ = round_ctr_reg == 7'b1001111;
  assign _051_ = round_ctr_reg >= 5'b10100;
  assign _052_ = round_ctr_reg >= 6'b101000;
  assign _053_ = round_ctr_reg >= 6'b111100;
  assign _054_ = round_ctr_reg <= 5'b10011;
  assign _055_ = round_ctr_reg <= 6'b100111;
  assign _056_ = round_ctr_reg <= 6'b111011;
  assign _057_ = _051_ && _055_;
  assign _058_ = _052_ && _056_;
  assign _059_ = ~ b_reg;
  assign _060_ = b_reg | c_reg;
  assign _061_ = b_reg | d_reg;
  assign _062_ = c_reg | d_reg;
  always @(posedge clk)
      a_reg <= _005_;
  always @(posedge clk)
      b_reg <= _006_;
  always @(posedge clk)
      c_reg <= _007_;
  always @(posedge clk)
      d_reg <= _008_;
  always @(posedge clk)
      e_reg <= _010_;
  always @(posedge clk)
      H0_reg <= _000_;
  always @(posedge clk)
      H1_reg <= _001_;
  always @(posedge clk)
      H2_reg <= _002_;
  always @(posedge clk)
      H3_reg <= _003_;
  always @(posedge clk)
      H4_reg <= _004_;
  always @(posedge clk)
      round_ctr_reg <= _011_;
  always @(posedge clk)
      digest_valid_reg <= _009_;
  always @(posedge clk)
      sha1_ctrl_reg <= _012_;
  assign round_ctr_inc = sha1_ctrl_reg == 1'b1;
  assign _035_ = _036_ ? 2'b10 : 2'b00;
  assign ready = ! sha1_ctrl_reg;
  assign _030_ = next ? 1'b1 : init;
  function [0:0] _123_;
    input [0:0] a;
    input [2:0] b;
    input [2:0] s;
    casez (s) // synopsys parallel_case
      3'b??1:
        _123_ = b[0:0];
      3'b?1?:
        _123_ = b[1:1];
      3'b1??:
        _123_ = b[2:2];
      default:
        _123_ = a;
    endcase
  endfunction
  assign sha1_ctrl_we = _123_(1'b0, { _030_, _036_, 1'b1 }, { ready, round_ctr_inc, digest_update });
  assign digest_update = sha1_ctrl_reg == 2'b10;
  function [1:0] _125_;
    input [1:0] a;
    input [3:0] b;
    input [1:0] s;
    casez (s) // synopsys parallel_case
      2'b?1:
        _125_ = b[1:0];
      2'b1?:
        _125_ = b[3:2];
      default:
        _125_ = a;
    endcase
  endfunction
  assign sha1_ctrl_new = _125_(2'b00, { 1'b0, _030_, _035_ }, { ready, round_ctr_inc });
  function [0:0] _126_;
    input [0:0] a;
    input [1:0] b;
    input [1:0] s;
    casez (s) // synopsys parallel_case
      2'b?1:
        _126_ = b[0:0];
      2'b1?:
        _126_ = b[1:1];
      default:
        _126_ = a;
    endcase
  endfunction
  assign digest_valid_we = _126_(1'b0, { _030_, 1'b1 }, { ready, digest_update });
  assign round_ctr_rst = ready ? _030_ : 1'b0;
  assign digest_init = ready ? init : 1'b0;
  assign a_e_we = round_ctr_inc ? 1'b1 : round_ctr_rst;
  assign round_ctr_new = round_ctr_inc ? _048_ : 7'b0000000;
  assign _037_ = _053_ ? _078_ : 32'd0;
  assign _038_ = _053_ ? 32'd3395469782 : 32'd0;
  assign _033_ = _058_ ? _080_ : _037_;
  assign _034_ = _058_ ? 32'd2400959708 : _038_;
  assign _031_ = _057_ ? _078_ : _033_;
  assign _032_ = _057_ ? 32'd1859775393 : _034_;
  assign _028_ = _054_ ? _076_ : _031_;
  assign _029_ = _054_ ? 32'd1518500249 : _032_;
  assign e_new = round_ctr_inc ? d_reg : _022_;
  assign d_new = round_ctr_inc ? c_reg : _021_;
  assign c_new = round_ctr_inc ? { b_reg[1:0], b_reg[31:2] } : _020_;
  assign b_new = round_ctr_inc ? a_reg : _019_;
  assign a_new = round_ctr_inc ? _047_ : _018_;
  assign _027_ = digest_init ? 32'd3285377520 : H4_reg;
  assign _026_ = digest_init ? 32'd271733878 : H3_reg;
  assign _025_ = digest_init ? 32'd2562383102 : H2_reg;
  assign _024_ = digest_init ? 32'd4023233417 : H1_reg;
  assign _023_ = digest_init ? 32'd1732584193 : H0_reg;
  assign _022_ = round_ctr_rst ? _027_ : 32'd0;
  assign _021_ = round_ctr_rst ? _026_ : 32'd0;
  assign _020_ = round_ctr_rst ? _025_ : 32'd0;
  assign _019_ = round_ctr_rst ? _024_ : 32'd0;
  assign _018_ = round_ctr_rst ? _023_ : 32'd0;
  assign H_we = digest_update ? 1'b1 : digest_init;
  assign H4_new = digest_update ? _043_ : _017_;
  assign H3_new = digest_update ? _042_ : { 3'b000, _016_ };
  assign H2_new = digest_update ? _041_ : _015_;
  assign H1_new = digest_update ? _040_ : _014_;
  assign H0_new = digest_update ? _039_ : { 1'b0, _013_ };
  assign _017_ = digest_init ? 32'd3285377520 : 32'd0;
  assign _016_ = digest_init ? 29'b10000001100100101010001110110 : 29'b00000000000000000000000000000;
  assign _015_ = digest_init ? 32'd2562383102 : 32'd0;
  assign _014_ = digest_init ? 32'd4023233417 : 32'd0;
  assign _013_ = digest_init ? 31'b1100111010001010010001100000001 : 31'b0000000000000000000000000000000;
  assign _063_ = sha1_ctrl_we ? sha1_ctrl_new : sha1_ctrl_reg;
  assign _012_ = reset_n ? _063_ : 2'b00;
  assign _064_ = digest_valid_we ? digest_update : digest_valid_reg;
  assign _009_ = reset_n ? _064_ : 1'b0;
  assign _065_ = a_e_we ? round_ctr_new : round_ctr_reg;
  assign _011_ = reset_n ? _065_ : 7'b0000000;
  assign _066_ = H_we ? H4_new : H4_reg;
  assign _004_ = reset_n ? _066_ : 32'd0;
  assign _067_ = H_we ? H3_new : H3_reg;
  assign _003_ = reset_n ? _067_ : 32'd0;
  assign _068_ = H_we ? H2_new : H2_reg;
  assign _002_ = reset_n ? _068_ : 32'd0;
  assign _069_ = H_we ? H1_new : H1_reg;
  assign _001_ = reset_n ? _069_ : 32'd0;
  assign _070_ = H_we ? H0_new : H0_reg;
  assign _000_ = reset_n ? _070_ : 32'd0;
  assign _071_ = a_e_we ? e_new : e_reg;
  assign _010_ = reset_n ? _071_ : 32'd0;
  assign _072_ = a_e_we ? d_new : d_reg;
  assign _008_ = reset_n ? _072_ : 32'd0;
  assign _073_ = a_e_we ? c_new : c_reg;
  assign _007_ = reset_n ? _073_ : 32'd0;
  assign _074_ = a_e_we ? b_new : b_reg;
  assign _006_ = reset_n ? _074_ : 32'd0;
  assign _075_ = a_e_we ? a_new : a_reg;
  assign _005_ = reset_n ? _075_ : 32'd0;
  assign _076_ = _049_ ^ _050_;
  assign _077_ = b_reg ^ c_reg;
  assign _078_ = _077_ ^ d_reg;
  assign _079_ = _060_ ^ _061_;
  assign _080_ = _079_ ^ _062_;
  sha1_w_mem w_mem_inst (
    .block(block),
    .clk(clk),
    .init(round_ctr_rst),
    .next(round_ctr_inc),
    .reset_n(reset_n),
    .w(w)
  );
  assign digest = { H0_reg, H1_reg, H2_reg, H3_reg, H4_reg };
  assign digest_valid = digest_valid_reg;
  assign digest_valid_new = digest_update;
  assign first_block = digest_init;
  assign ready_flag = ready;
  assign round_ctr_we = a_e_we;
  assign state_init = round_ctr_rst;
  assign state_update = round_ctr_inc;
  assign w_init = round_ctr_rst;
  assign w_next = round_ctr_inc;
endmodule