#include <set>
#include <string>
#include <map>
#include <cmath>
#include <vector>
#include "../src/global_data_struct.h"
#include "../src/parse_fill.h"


// key of map is var name, the value vector is the
// vector of values for multiple cycles, since an
// instruction might span multiple cycles
typedef std::map<std::string, 
                     std::vector<std::string>> InstEncoding_t;

extern std::map<std::string, std::string> g_aes_special_func_call;
extern std::map<std::string, std::string> g_urv_special_func_call;

std::string update_function_name(const std::string& instr, const std::string& asv);

std::string c_type(uint32_t width);

std::string func_call(std::string indent, std::string writeASV,
                      const funcExtract::FuncTy_t& funcTy, std::string funcName, 
                      const InstEncoding_t &encoding,
                      std::pair<std::string, uint32_t> dataIn);                      

std::string get_arg_value(const std::string& arg, int cycle, const InstEncoding_t& encoding);


void print_func_declare(const funcExtract::FuncTy_t& funcTy, 
                        std::string funcName, 
                        std::ofstream &header);

void print_var_assignments(std::ofstream &cpp, std::string indent, 
                      const InstEncoding_t &inputInstr);

std::string apint2initializer(const llvm::APInt& val);
std::string apint2literal(const llvm::APInt& val);

void print_instr_calls(const InstEncoding_t &encoding,
                       std::string prefix,
                       std::ofstream &cpp);

void update_all_asvs(std::ofstream &cpp, std::string prefix);

void read_refinement(std::string fileName);

void read_skipped_target(std::string fileName);

void print_final_results(std::ofstream &cpp);

void print_array(std::string indent, std::string arrName, std::ofstream &cpp);

void read_mem_vals(std::string fileName, std::vector<llvm::APInt>& vals);


void print_update_mem(std::ofstream &cpp);

void print_update_mem_call(std::ofstream &cpp);

void print_urv_update_mem(std::ofstream &cpp);

void vta_ila_model(std::ofstream &cpp);

// Generate a call to the function to print ASV values.
void print_asvs(std::ofstream &cpp, const std::string& bannerLine="", bool always=false);

// Generate the body of the function to print ASV values.
void print_asvs_printer_func(std::ofstream &cpp);

bool is_array_var(const std::string& varName);
std::string get_array_position(const std::string& varName, int* idxp);

std::string build_printf(const std::string& prefix, const std::string& varName,
                          uint32_t width, std::string index="");

// printName is optional, defaults to same as varName
void print_var_value(std::string indent, std::ofstream &cpp, const std::string& varName,
                     uint32_t width, const std::string& printName = "");

bool is_in_array(const std::string& varName);
std::string get_c_rst_val(const std::string& asv, uint32_t width);


// Create a single function that does all the work for a particular instruction:
// calling each relevant update function, updating the ASVs, and printing debug info.
void print_instr_wrapper_func(const funcExtract::InstrInfo_t& instr,
                       std::ofstream &cpp);

// Print the declaration for the instruction wrapper function.
void print_instr_wrapper_decl(const std::string &instrName,
                              const std::string &indent,
                              std::ofstream &stream);

// Call the single function that does all the work for a particular instruction.
// Also set any necessary register values specified by the encoding
void print_instr_wrapper_call(const InstEncoding_t& encoding,
                              const std::string &indent,
                              std::ofstream &cpp);

// Make a C-clean name for a cycle-specific variable.  A cycle of 0 means non-cycle-specific
std::string var_name_cycle_convert(const std::string& varName, int cycle);
