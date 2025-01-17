#ifndef FUNC_EXTRACT_EXPR_H
#define FUNC_EXTRACT_EXPR_H

#include <string>
#include <iostream>
#include <fstream>
#include <regex>
#include <vector>
#include <algorithm>
#include <unordered_map>
#include "../../live_analysis/src/global_data.h"
#include "../../live_analysis/src/helper.h"
#include <assert.h>

namespace funcExtract {

void module_expr(std::string line, bool isMem=false);

void input_expr(std::string line);

void reg_expr(std::string line);

void wire_expr(std::string line);

void mem_expr(std::string line);

void dyn_sel_expr(std::string line);

void output_expr(std::string line);

void single_line_expr(std::string line);

void mem_if_assign_expr(std::string line);

void both_concat_expr(std::string line);

void dest_concat_expr(std::string line);

void nb_expr(std::string line);

void always_expr(std::string line, std::ifstream &input);

std::pair<std::string, std::string> nonblockif_expr(std::string line, 
                                                    std::ifstream &input,
                                                    bool insertNBTable=true);

void if_expr(std::string line, std::ifstream &input);

void always_clkrst_expr(std::string line, std::ifstream &input);

void always_if_else_expr(std::string line, std::ifstream &input);

void case_expr(std::string line, std::ifstream &input);

void switch_expr(std::ifstream &input);

void submodule_expr(std::string line, std::ifstream &input);

void put_into_reg2Slice(std::string destAndSlice);

} // end of namespace funcExtract
#endif
