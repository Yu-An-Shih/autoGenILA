module CKLNQD12(TE, E, CP, Q);
  (* src = "./vmod/vlibs/CKLNQD12.v:20" *)
  wire _0_;
  (* src = "./vmod/vlibs/CKLNQD12.v:17" *)
  input CP;
  (* src = "./vmod/vlibs/CKLNQD12.v:16" *)
  input E;
  (* src = "./vmod/vlibs/CKLNQD12.v:18" *)
  output Q;
  (* src = "./vmod/vlibs/CKLNQD12.v:15" *)
  input TE;
  (* src = "./vmod/vlibs/CKLNQD12.v:19" *)
  reg qd;
  assign Q = CP & (* src = "./vmod/vlibs/CKLNQD12.v:22" *) qd;
  assign _0_ = TE | (* src = "./vmod/vlibs/CKLNQD12.v:21" *) E;
  always @(negedge CP)
      qd <= _0_;
endmodule