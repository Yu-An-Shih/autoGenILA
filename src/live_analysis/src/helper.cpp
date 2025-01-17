#include <cstdlib>
#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#include <regex>
#include <vector>
#include <bitset>
#include <utility>
#include <assert.h>
#include <unordered_map>
#include "helper.h"
#include "taint_gen.h"
#include <algorithm>
#include <math.h>
#include <boost/algorithm/string.hpp>
/* help functions */

#define toStr(a) std::to_string(a)

using namespace syntaxPatterns;

namespace taintGen {

// This recognizes concatenation (e.g. 5'x1f+9'x1dd+8'x0) only if contained in curly braces.
bool isNum(std::string name) {
  remove_two_end_space(name);
  if(name.empty()) {
    toCout("Error: var not matched in isNum: "+name);
    return false;
  }

  size_t bracePos = name.find('{');
  if (bracePos == name.npos) {
    // Most common case: no '{'
    if (!isdigit(name[0])) {
      // Legal number must start with decimal digit.
      // Non-number is most common case.
      return false;
    }
    return std::regex_match(name, pNumNoGroup);
  } else {
    bool allAreNum = true;
    std::string singleVar;
    std::regex_token_iterator<std::string::iterator> rend;
    std::regex_token_iterator<std::string::iterator> it(name.begin(), name.end(), pVarName, 0);
    while( it != rend ) {
      singleVar = *it++;
      if(!isNum(singleVar))
        return false;
    }
    return true;
  }
}


bool is_number(const std::string& s) {
  std::string num = s;
  num = remove_signed(num);
  if(isNum(num)) return true; 
  return is_all_digits(num);
}


bool is_all_digits(const std::string& num) {
  std::string::const_iterator it = num.begin();
  while (it != num.end() && std::isdigit(*it)) ++it;
  return !num.empty() && it == num.end();
}


bool isOutput(const std::string& varAndSlice) {
  std::string var, varSlice;
  split_slice(varAndSlice, var, varSlice);
  auto it = std::find( moduleOutputs.begin(), moduleOutputs.end(), var );
  return it != moduleOutputs.end();
}


bool isInput(const std::string& var) {
  auto it = std::find( moduleInputs.begin(), moduleInputs.end(), var );
  return it != moduleInputs.end();
}


bool isReg(std::string var) {
  if(var.back() == ' ')
    var.pop_back();
  auto it = std::find( moduleTrueRegs.begin(), moduleTrueRegs.end(), var );
  return it != moduleTrueRegs.end();
}


// belongs to over-approximated reg set
bool isOAReg(const std::string& var) {
  auto it = std::find( moduleRegs.begin(), moduleRegs.end(), var );
  return it != moduleRegs.end();
}


bool isWire(const std::string& var) {
  auto it = std::find( moduleWires.begin(), moduleWires.end(), var );
  return it != moduleWires.end();
}


bool isMem(const std::string& varAndSlice) {
  std::string var, varSlice;
  split_slice(varAndSlice, var, varSlice);
  //auto it = std::find( moduleMems.begin(), moduleMems.end(), var );
  return moduleMems.find(var) != moduleMems.end();
}


std::string to_re(const std::string& input) {
  static const std::regex pNameBraces("\\(NAME\\)");
  // Below is the old varNameBraces. It has been used without errors for many designs.
  // The reason I replace it with the new one is because it failed in the ultra_riscv case
  //std::string varNameBraces("([\a\ba-zA-Z0-9_=\\.\\$\\\\'\\[\\]\\(\\)\\/\\:]+(?:\\s*\\[\\d+(?:\\:\\d+)?\\])?)(?:\\s)?");
  std::string varNameBraces("((?:\\\\\\S+|[0-9a-zA-Z_]+|[0-9]+\\'[bdh][0-9a-fx]+|\\$(?:un)?signed\\([0-9]+\\'[bdh][0-9a-fx]+\\)|\\$(?:un)?signed\\([0-9a-zA-Z_]+\\)|\\$(?:un)?signed\\(\\\\\\S+ \\))(?:\\s*\\[\\d+(?:\\:\\d+)?\\])?)(?:\\s)?");  
  auto res = std::regex_replace(input, pNameBraces, varNameBraces);

  static const std::regex pName("NAME");
  //std::string varName("[\a\ba-zA-Z0-9_=\\.\\$\\\\'\\[\\]\\(\\)\\/\\:]+(?:\\s*\\[\\d+(?:\\:\\d+)?\\])?(?:\\s)?");
  std::string varName("(?:\\\\\\S+|[0-9a-zA-Z_]+|[0-9]+\\'[bdh][0-9a-fx]+|\\$(?:un)?signed\\([0-9]+\\'[bdh][0-9a-fx]+\\)|\\$(?:un)?signed\\([0-9a-zA-Z_]+\\)|\\$(?:un)?signed\\(\\\\\\S+ \\))(?:\\s*\\[\\d+(?:\\:\\d+)?\\])?(?:\\s)?");

  res = std::regex_replace(res, pName, varName);

  static const std::regex pNUM("NUM");
  std::string regexNum("\\d+'(?:h|b)[\\dabcdef]+");
  res = std::regex_replace(res, pNUM, regexNum);
  static const std::regex pInt("INT");
  std::string regexInt("\\d+");
  res = std::regex_replace(res, pInt, regexInt);
  //std::cout << res << std::endl;
  return res;
}


std::string remove_bracket(const std::string& name) {
  static const std::regex pName("^([a-zA-Z0-9_.]+)\\[(\\d+)\\:(\\d+)\\]$");
  std::smatch match;
  if (std::regex_match(name, match, pName)) {
    // FIXME: how to deal with this more appropriately?
    return match.str(1) + "_" + match.str(2) + "t" + match.str(3);
    //return match.str(1);
  }
  return name;
}


// Doug: If the given string has a '[', return its position.
// But if the first non-blank char of the string is a '\', ignore
// any '[' chars until a space is encountered.  (This handles
// backslash-escaped names).  If no '[' is found, return the string length.
//
// This was originally written with a regex_match(), but I observed that
// call consuming 22% of the total runtime for a big design.
// Also, I noticed that the vast majority of calls give a simple
// Yosys-generated name like "_022509_".

uint32_t cut_pos(const std::string& name) {
  bool escaped = false;
  for (size_t i = 0; i < name.length(); ++i) {
    if(name[i] == '\\')
      escaped = true;
    else if(escaped && name[i] == ' ')
      escaped = false;
    else if (!escaped and name[i] == '[') {
      return i;
    }
  }
  return name.length();
}


// The returned name and slice may contain blanks,
// but the name's leading and trailing blanks will be removed.
bool split_slice(const std::string& slicedName, std::string &name, std::string &slice) {
  uint32_t pos = cut_pos(slicedName);
  if (pos == slicedName.length()) {
    // No slice present
    name = slicedName;
    remove_two_end_space(name);
    if(name.empty()) {
      toCout("Error: name is empty in split_slice, input is: "+slicedName);
      abort();
    }
    slice = "";
    return false;
  }
  else {
    name = slicedName.substr(0, pos);
    remove_two_end_space(name);
    if(name.empty()) {
      toCout("Error: name is empty in split_slice, input is: "+slicedName);
      abort();
    }
    slice = " " + slicedName.substr(pos);
    return true;
  }
}


// no matter leftIdx or rightIdx is bigger, always return a positive width
uint32_t get_width(const std::string& slice) {
  static const std::regex pSlice("^(?:\\s?)\\[(\\d+)\\:(\\d+)\\](?:\\s)?$");
  static const std::regex pSingleBit("^(?:\\s)?\\[\\d+\\](?:\\s)?$");
  std::smatch m;
  if (slice.empty())
    return 1;
  if( !std::regex_match(slice, m, pSlice) && !std::regex_match(slice, m, pSingleBit) )
    std::cout << "Wrong input to get_width:|" + slice << "|" << std::endl;
  if( std::regex_match(slice, m, pSingleBit) )
    return 1;
  else {
    std::regex_match(slice, m, pSlice);
    uint32_t leftIdx = str2int(m.str(1), "get_width 1st("+slice+")");
    uint32_t rightIdx = str2int(m.str(2), "get_width 2rd("+slice+")");
    if(leftIdx > rightIdx)
      return leftIdx - rightIdx + 1;
    else
      return rightIdx - leftIdx + 1;
  }
}


// return low index
uint32_t get_begin(const std::string& slice) {
  static const std::regex pSlice("^(?:\\s?)\\[(?:(\\d+)\\:)?(\\d+)\\](\\s)?$");
  std::smatch m;
  if( !std::regex_match(slice, m, pSlice) )
    std::cout << "Wrong input to get_begin:|" + slice << "|" << std::endl;
  return str2int(m.str(2), "get_begin("+slice+")");
}


// return the high index
uint32_t get_end(const std::string& slice) {
  static const std::regex pSlice("^(?:\\s?)\\[(\\d+)(?:\\:(\\d+))?\\](\\s)?$");
  std::smatch m;
  if( !std::regex_match(slice, m, pSlice) ) {
    std::cout << "Wrong input to get_end:|" + slice << "|" << std::endl;
    return 0;
  }
  return str2int(m.str(1), "get_end("+slice+")");
}


// the input op may contain slices
uint32_t find_version_num(const std::string& opAndSlice, bool &isNew, std::ofstream &output, bool forceNewVer) {
  uint32_t verNum;
  std::string op, opSlice;
  split_slice(opAndSlice, op, opSlice);
  if ( nextVersion.find(op) == nextVersion.end() ) {
    verNum = 0;
    nextVersion.insert( std::make_pair(op, 1) );
    nxtVerBits.emplace( op, std::vector<bool>{} );
    isNew = true;
  }
  else {
    verNum = nextVersion[op];    
    bool noConflict = check_bits(op, opSlice, nxtVerBits[op]);
    if(!noConflict or forceNewVer) {
      // ground unassigned wires
      std::vector<std::string> freeBitsVec;
      free_bits(op, freeBitsVec);
      if(freeBitsVec.size() > 0 && g_enable_taint) {
        output << "  assign "+add_taint(freeBitsVec, _r+toStr(verNum-1)) + " = 0;" << std::endl;
        //output << "  assign "+add_taint(freeBitsVec, _x+toStr(verNum-1)) + " = 0;" << std::endl;
        //output << "  assign "+add_taint(freeBitsVec, _c+toStr(verNum-1)) + " = 0;" << std::endl;
      }
      nextVersion[op]++;
      nxtVerBits[op].clear();
      isNew = true;
    }
    else if(forceNewVer){
      nextVersion[op]++;
      isNew = true;
      nxtVerBits[op].clear();
    }
    else {
      verNum--;
      isNew = false;
    }
  }
  merge_bits(op, opSlice, nxtVerBits[op]);  
  return verNum;
}


void free_bits(const std::string& op, std::vector<std::string> &freeBitsVec) {
  assert(freeBitsVec.empty());
  auto idxPairs = varWidth.get_idx_pair(op, "find_version_num for: "+op);
  size_t usedVerBitsSize = nxtVerBits[op].size();
  uint32_t lowIdx = idxPairs.second;
  uint32_t highIdx = idxPairs.first;
  if(usedVerBitsSize < lowIdx+1) {
    // no bits are used before
    freeBitsVec.push_back(op);
    return;
  }
  else if(usedVerBitsSize < highIdx+1) {
    // partial bits are used
    for(uint32_t i = lowIdx; i < usedVerBitsSize; i++) {
      if(nxtVerBits[op][i] == false) {
        freeBitsVec.push_back(op+" ["+toStr(i)+"]");
      }
    }
    freeBitsVec.push_back(op+" ["+toStr(highIdx)+":"+toStr(usedVerBitsSize)+"]");
    return;
  }
  else {
    // all bits have been used
    for(uint32_t i = lowIdx; i <= highIdx; i++) {
      if(nxtVerBits[op][i] == false) {
        freeBitsVec.push_back(op+" ["+toStr(i)+"]");
      }
    }
    return;    
  }
}


// returns bool: noConflict
bool check_bits(const std::string& op, std::string opSlice, const std::vector<bool> &bitVec) {
  // if opSlice is empty, must be conflict
  uint32_t highIdx, lowIdx;
  if(opSlice.empty()) {
    return false;
  }
  else {
    static const std::regex pSlice("^(?:\\s*)\\[(\\d+)(?:\\:(\\d+))?\\](?:\\s*)$");
    std::smatch m;
    if(!std::regex_match(opSlice, m, pSlice)) {
      toCout("Error: does not match slice: "+ opSlice);
      abort();
    }
    highIdx = str2int(m.str(1), "error for highIdx, Slice is: "+opSlice);
    if(m.str(2).empty()) {
      lowIdx = highIdx;
    }
    else {
      lowIdx = str2int(m.str(2), "error for lowIdx, Slice is: "+opSlice);
    }
  }
  size_t vecSize = bitVec.size();
  if(vecSize > highIdx) {
    for(long int i = highIdx; i >= lowIdx; i--) {
      if(bitVec[i]) {
        return false;
      }
    } // end of for
  }
  else if(vecSize > lowIdx){
    for(long int i = vecSize-1; i >= lowIdx; i--) {
      if(bitVec[i]) {
        return false;
      }
    } // end of for
  }
  else {
    return true;
  }
  return true;
}


// opSlice is merged to bitVec(although sometimes this is not 
// needed, because the bitVec should be cleared)
void merge_bits(const std::string& op, std::string opSlice, std::vector<bool> &bitVec) {
  // if opSlice is empty, must be conflict
  uint32_t highIdx, lowIdx;
  if(opSlice.empty()) {
    auto idxPair = varWidth.get_idx_pair(op, "check_bits for: "+op);
    highIdx = idxPair.first;
    lowIdx = idxPair.second;
  }
  else {
    static const std::regex pSlice("^(?:\\s*)\\[(\\d+)(?:\\:(\\d+))?\\](?:\\s*)$");
    std::smatch m;
    if(!std::regex_match(opSlice, m, pSlice)) {
      toCout("Error: does not match slice: "+ opSlice);
      abort();
    }
    highIdx = str2int(m.str(1), "error for highIdx, Slice is: "+opSlice);
    if(m.str(2).empty()) {
      lowIdx = highIdx;
    }
    else {
      lowIdx = str2int(m.str(2), "error for lowIdx, Slice is: "+opSlice);
    }
  }
  size_t vecSize = bitVec.size();
  if(vecSize > highIdx) {
    for(long int i = highIdx; i >= lowIdx; i--) {
      if(!bitVec[i]) {
        bitVec[i] = true;
      }
    } // end of for
  }
  else if(vecSize > lowIdx){
    for(long int i = vecSize-1; i >= lowIdx; i--) {
      if(!bitVec[i]) {
        bitVec[i] = true;
      }
    } // end of for
    while(vecSize++ <= highIdx) {
      bitVec.push_back(true);
    }
  }
  else {
    while(vecSize < lowIdx) {
      bitVec.push_back(false);
      vecSize++;
    }
    while(vecSize <= highIdx) {
      bitVec.push_back(true);
      vecSize++;
    }
  }
}


// taintBits include t,r,x,c
void parse_taintBits(const std::string& taintBits, bool &tExist, bool &rExist, bool &xExist, bool &cExist) {
  for(int i = 0; i < taintBits.length(); i++) {
    if( taintBits.compare(i, 1, "t") == 0 )
        tExist = true;
    else if ( taintBits.compare(i, 1, "r") == 0 ) 
      rExist = true;
    else if ( taintBits.compare(i, 1, "x") == 0 ) 
      xExist = true;
    else if ( taintBits.compare(i, 1, "c") == 0 ) 
      cExist = true;
    else
      std::cout << "Error: wrong taintBits!!" << std::endl;
  }
}


void collapse_bits(const std::string& varName, uint32_t bound1, uint32_t bound2, std::ofstream &output) {
  uint32_t begin = std::min(bound1, bound2);
  uint32_t end = std::max(bound1, bound2);
  for (uint32_t i = begin; i + 1 < end; ++i) {
    output << varName + "[" + std::to_string(i) + "] | ";
  }
  output << varName + "[" + std::to_string(end-1) + "];" << std::endl;
}


std::string extend(const std::string& in, uint32_t length) {
  return "{ " + std::to_string(length) + "{ " + in + " }}";
}


void debug_line(const std::string& line) {
  std::cout << "get_width() for " + line << std::endl;
}


// input slice might be empty
void ground_wires(const std::string& wireName, std::pair<uint32_t, uint32_t> idxPair, std::string slice, std::string blank, std::ofstream &output) {
  if (slice.empty())
    return;
  uint32_t sliceBegin = get_begin(slice);
  uint32_t sliceEnd = get_end(slice);
  uint32_t smallIdx = std::min(sliceBegin, sliceEnd);
  uint32_t bigIdx = std::max(sliceBegin, sliceEnd);

  uint32_t highIdx = idxPair.first;
  uint32_t lowIdx = idxPair.second;
  uint32_t highBound = std::max(highIdx, lowIdx);
  uint32_t lowBound = std::min(highIdx, lowIdx);
  
  if ( bigIdx < highBound ) {
    output << blank + "assign " + wireName + " [" + toStr(highBound) + ":" + toStr(bigIdx+1) + "] = 0;" << std::endl;
  }
  if ( smallIdx > lowBound ) {
    output << blank + "assign " + wireName + " [" + toStr(smallIdx-1) + ":" + toStr(lowBound) + "] = 0;" << std::endl;
  }
}


// assume the input is a list of vars, separated by comma.
// Aslo, the vars might contain numbers
// But blanks at the front and back are removed
// ATTENTION: by default the return vector contains slices!
void parse_var_list(std::string list, std::vector<std::string> &vec, bool noSlice) {
  assert(vec.size() == 0);
  // remove the last char since it is )
  int previous = -1;
  if(list.front() == '(') {
    list.pop_back();
    previous = 0;
  } 
  else if (list.front() == '{') {
    list.pop_back();
    previous = 0;
  }
  int current = 0;
  char delim = ',';
  std::string arg;
  // collect all non-numerical args in vector args
  while( current != std::string::npos ) {
    current = list.find(delim, previous + 1);
    arg = list.substr(previous+1, current-previous-1);
    //std::regex pLocal("^(\\s)*(\\S+)(\\s)*$");
    std::smatch m;
    std::regex pLocal;
    if(!noSlice) {
      pLocal.assign("^(?:\\s)*([\aa-zA-Z0-9_\\.\\$:\\\\'\\[\\]]+(?:\\s*\\[\\d+(?:\\:\\d+)?\\])?)(?:\\s)*$");
      std::regex_match(arg, m, pLocal);
      vec.push_back(m.str(1));
    }
    else {
      //pLocal.assign("^(?:\\s)*([\aa-zA-Z0-9_\\.:\\\\'\\[\\]]+)(?:\\s*\\[\\d+(?:\\:\\d+)?\\])?(?:\\s)*$");
      std::string var, varSlice;
      split_slice(arg, var, varSlice);
      vec.push_back(var);
    }
    previous = current;
  }
}


// the first idx is 1 instead of 0.
std::string get_nth_var_in_list(std::string list, uint32_t idx) {
  int previous = -1;
  if(list.front() == '(') {
    list.pop_back();
    previous = 0;
  }
  int current = 0;
  char delim = ',';
  std::string arg;
  uint32_t i = 0;
  // collect all non-numerical args in vector args
  while( current != std::string::npos ) {
    current = list.find(delim, previous + 1);
    arg = list.substr(previous+1, current-previous-1);
    i++;
    //std::regex pLocal("^(\\s)*(\\S+)(\\s)*$");
    static const std::regex pLocal("^(?:\\s)*([\aa-zA-Z0-9_\\.:\\\\'\\[\\]]+(?:\\s*\\[\\d+(?:\\:\\d+)?\\])?)(?:\\s)*$");
    std::smatch m;
    std::regex_match(arg, m, pLocal);
    if(i == idx)
      return m.str(1);
    previous = current;
  }
  abort();
}


// the most general function for getting width
uint32_t get_var_slice_width( std::string varAndSlice, VarWidth &varWidthIn) {
  varAndSlice = remove_signed(varAndSlice);
  if( varAndSlice.empty() )
    return 0;
  if(isNum(varAndSlice)) {
    std::smatch m;
    std::regex_match(varAndSlice, m, pNum);
    uint32_t width = str2int(m.str(1), "get_var_slice width("+varAndSlice+")");
    return width;
  }
  std::string var, varSlice;
  split_slice(varAndSlice, var, varSlice);
  // split_slice() removes leading/trailing blanks from var.
  
  if(isMem(var)) {
    auto dim = memDims[var];
    return get_width(dim.first);
  }
  uint32_t totalWidth = 0;
  if(!varSlice.empty()) {
    if(isSingleBit(varSlice))
      totalWidth = 1;
    else
      totalWidth = get_width(varSlice);
  }
  else {
    auto v = varWidthIn.get_from_var_width(var, varAndSlice);
    if (v == 0) {
      toCout("0 width found!");
      abort();
    }
    totalWidth = v;
  }
  return totalWidth;
}


std::string get_rhs_taint_list(std::vector<std::string> &updateVec, std::string taint, bool noSlice) {
  std::vector<std::string> taintVec;
  std::smatch m;
  bool allSliceEmpty = true;
  for(std::string singleUpdate : updateVec) {
    if(!isNum(singleUpdate)) {
      std::string update, updateSlice;
      split_slice(singleUpdate, update, updateSlice);
      if(noSlice)
        singleUpdate = update+taint;
      else
        singleUpdate = update+taint+updateSlice;
    }
    else { // if isNum
      if( !std::regex_match(singleUpdate, m, pNum)) {
        std::cout << "!! Error in matching number !!" << std::endl;
      }
      std::string numWidth = m.str(1);
      singleUpdate = numWidth + "'h0";
    }
    taintVec.push_back(singleUpdate);    
  }
  std::string returnList = " ";
  for (auto it = taintVec.begin(); it < taintVec.end() - 1; ++it) {
    returnList = returnList + *it + " , ";
  }
  returnList = returnList + taintVec.back() + " ";
  return returnList;
}


std::string get_rhs_taint_list(std::string updateList, std::string taint, bool noSlice) {
  std::vector<std::string> updateVec;
  parse_var_list(updateList, updateVec);
  if (updateList.front() == '{')
    return "{ "+get_rhs_taint_list(updateVec, taint, noSlice)+" }";
  else
    return get_rhs_taint_list(updateVec, taint, noSlice);
}


// used for output ports in module instantiation
std::string insert_taint(const std::string& signalAndSlice, std::string taint, std::string ver) {
  std::string signal, signalSlice;
  split_slice(signalAndSlice, signal, signalSlice);
  return signal + taint + ver + signalSlice;
}


/* The reason that here we have _ver version and non-ver version
 * is: for _t taint, version is not needed. But for _r, _x, _c 
 * taint, version is necessary */
// assume the updateVec does not contain numbers
std::string get_lhs_ver_taint_list(std::vector<std::string> &updateVec, std::string taint, std::ofstream &output, std::vector<uint32_t> verVec) {
  assert(updateVec.size() == verVec.size());
  std::vector<std::string> taintVec;
  std::smatch m;
  std::string update;
  std::string updateSlice;
  uint32_t i = 0;
  for(std::string updateAndSlice : updateVec) {
    std::string updateTaintSlice;
    if(!isNum(updateAndSlice)) {
      split_slice(updateAndSlice, update, updateSlice);
      uint32_t updateWidthNum = varWidth.get_from_var_width(update, updateAndSlice);
      auto updateBoundPair = varWidth.get_idx_pair(update, "get_lhs_ver_taint_list");
      std::string updateWidth = toStr(updateWidthNum);
      std::string localVer = toStr(verVec[i++]);
      output << "  logic [" + updateWidth + "-1:0] " + update + taint + localVer + " ;" << std::endl;
      //ground_wires(update+taint+localVer, updateBoundPair, updateSlice, "  ", output);
      updateTaintSlice = update+taint+localVer+updateSlice;
    }
    else { // if isNum
      if( !std::regex_match(updateAndSlice, m, pNum)) {
        std::cout << "!! Error in matching number : " + updateAndSlice << std::endl;
        abort();
      }
      std::string numWidth = m.str(1);
      int localIdx = USELESS_VAR++;
      // declare a dummy wire, just for being assigned
      output << "  logic [" + numWidth + "-1:0] nouse_zy" + toStr(localIdx) + " ;" << std::endl;
      updateTaintSlice = "nouse_zy" + toStr(localIdx);
    }
    taintVec.push_back(updateTaintSlice);    
  }
  std::string returnList = " ";
  for (auto it = taintVec.begin(); it < taintVec.end() - 1; ++it) {
    returnList = returnList + *it + " , ";
  }
  returnList = returnList + taintVec.back() + " ";
  return returnList;
}


std::string get_lhs_ver_taint_list(std::string list, std::string taint, std::ofstream &output, std::vector<uint32_t> localVer) {
  std::vector<std::string> vec;
  parse_var_list(list, vec);
  if (list.front() == '{')
    return "{ "+get_lhs_ver_taint_list(vec, taint, output, localVer)+" }";
  else
    return get_lhs_ver_taint_list(vec, taint, output, localVer);
}


// this function variant is created because sometines we do not want to create new nouse_zy logic in the middle
std::string get_lhs_ver_taint_list(std::vector<std::string> &updateVec, std::string taint, std::string &newLogic, std::vector<uint32_t> verVec) {
  assert(updateVec.size() == verVec.size());
  newLogic.clear();
  std::vector<std::string> taintVec;
  std::smatch m;
  std::string update;
  std::string updateSlice;
  uint32_t i = 0;
  for(std::string updateAndSlice : updateVec) {
    std::string updateTaintSlice;
    if(!isNum(updateAndSlice)) {
      split_slice(updateAndSlice, update, updateSlice);
      uint32_t updateWidthNum = varWidth.get_from_var_width(update, updateAndSlice);
      auto updateBoundPair = varWidth.get_idx_pair(update, "get_lhs_ver_taint_list");
      std::string updateWidth = toStr(updateWidthNum);
      std::string localVer = toStr(verVec[i]);
      //output << "  logic [" + updateWidth + "-1:0] " + update + taint + localVer + " ;" << std::endl;
      //ground_wires(update+taint+localVer, updateBoundPair, updateSlice, "  ", output);
      updateTaintSlice = update+taint+localVer+updateSlice;
    }
    else { // if isNum
      if( !std::regex_match(updateAndSlice, m, pNum)) {
        std::cout << "!! Error in matching number : " + updateAndSlice << std::endl;
        abort();
      }
      std::string numWidth = m.str(1);
      int localIdx = USELESS_VAR++;
      // declare a dummy wire, just for being assigned
      newLogic = "  logic [" + numWidth + "-1:0] nouse_zy" + toStr(localIdx) + " ;";
      updateTaintSlice = "nouse_zy" + toStr(localIdx);
    }
    taintVec.push_back(updateTaintSlice);
    i++;
  }
  std::string returnList = " ";
  for (auto it = taintVec.begin(); it < taintVec.end() - 1; ++it) {
    returnList = returnList + *it + " , ";
  }
  returnList = returnList + taintVec.back() + " ";
  return returnList;
}


std::string get_lhs_ver_taint_list(std::string list, std::string taint, std::string &newLogic, std::vector<uint32_t> localVer) {
  std::vector<std::string> vec;
  parse_var_list(list, vec);
  if (list.front() == '{')
    return "{ "+get_lhs_ver_taint_list(vec, taint, newLogic, localVer)+" }";
  else
    return get_lhs_ver_taint_list(vec, taint, newLogic, localVer);
}


void get_ver_vec(std::vector<std::string> varVec, std::vector<uint32_t> &verVec, std::ofstream &output) {
  assert(verVec.empty());
  bool pseudoIsNew;
  for(std::string var : varVec) {
    if(!isNum(var))
      verVec.push_back( find_version_num(var, pseudoIsNew, output, true) );
    else
      verVec.push_back( 0 );
  }
}


void get_ver_vec(std::string list, std::vector<uint32_t> &verVec, std::ofstream &output) {
  std::vector<std::string> vec;
  parse_var_list(list, vec, true);
  return get_ver_vec(vec, verVec, output);
}


void get_ver_vec(std::vector<std::string> varVec, std::vector<uint32_t> &verVec, std::vector<bool> &isNewVec, std::ofstream &output) {
  assert(verVec.empty());
  assert(isNewVec.empty());
  bool isNew;
  for(std::string var : varVec) {
    if(!isNum(var)) {
      verVec.push_back( find_version_num(var, isNew, output) );
      isNewVec.push_back(isNew);
    }
    else {
      verVec.push_back( 0 );
      isNewVec.push_back(isNew);
    }
  }
}


std::string get_lhs_taint_list(std::vector<std::string> &destVec, std::string taint, std::ofstream &output) {
  std::vector<std::string> taintVec;
  std::smatch m;
  for(std::string singleDest : destVec) {
    if(!isNum(singleDest)) {
      //std::regex_match(singleDest, m, pVarNameGroup);
      //singleDest = std::regex_replace(singleDest, pVarNameGroup, "$1"+taint+"$3");
      std::string dest, destSlice;
      split_slice(singleDest, dest, destSlice);
      singleDest = dest+taint+destSlice;
    }
    else { // if isNum
      if( !std::regex_match(singleDest, m, pNum)) {
        std::cout << "!! Error in matching number: " + singleDest << std::endl;
        abort();
      }
      std::string numWidth = m.str(1);
      int localIdx = USELESS_VAR++;
      // declare a dummy wire, just for being assigned
      output << "  wire [" + numWidth + "-1:0] nouse_zy" + toStr(localIdx) + " ;" << std::endl;
      singleDest = "nouse_zy" + toStr(localIdx);
    }
    taintVec.push_back(singleDest);    
  }
  std::string returnList = " ";
  for (auto it = taintVec.begin(); it < taintVec.end() - 1; ++it) {
    returnList = returnList + *it + " , ";
  }
  returnList = returnList + taintVec.back() + " ";
  return returnList;
}


std::string get_lhs_taint_list(std::string destList, std::string taint, std::ofstream &output) {
  std::vector<std::string> destVec;
  parse_var_list(destList, destVec);
  if (destList.front() == '{')
    return "{ "+get_lhs_taint_list(destVec, taint, output)+" }";
  else
    return get_lhs_taint_list(destVec, taint, output);
}


std::string get_lhs_taint_list(std::vector<std::string> &destVec, std::string taint, std::string &newLogic) {
  newLogic.clear();
  std::vector<std::string> taintVec;
  std::smatch m;
  for(std::string singleDest : destVec) {
    if(!isNum(singleDest)) {
      //std::regex_match(singleDest, m, pVarNameGroup);
      //singleDest = std::regex_replace(singleDest, pVarNameGroup, "$1"+taint+"$3");
      std::string dest, destSlice;
      split_slice(singleDest, dest, destSlice);
      singleDest = dest+taint+destSlice;
    }
    else { // if isNum
      if( !std::regex_match(singleDest, m, pNum)) {
        std::cout << "!! Error in matching number: " + singleDest << std::endl;
        abort();
      }
      std::string numWidth = m.str(1);
      int localIdx = USELESS_VAR++;
      // declare a dummy wire, just for being assigned
      newLogic = "  logic [" + numWidth + "-1:0] nouse_zy" + toStr(localIdx) + " ;";
      singleDest = "nouse_zy" + toStr(localIdx);
    }
    taintVec.push_back(singleDest);    
  }
  std::string returnList = " ";
  for (auto it = taintVec.begin(); it < taintVec.end() - 1; ++it) {
    returnList = returnList + *it + " , ";
  }
  returnList = returnList + taintVec.back() + " ";
  return returnList;
}


std::string get_lhs_taint_list(std::string destList, std::string taint, std::string &newLogic) {
  std::vector<std::string> destVec;
  parse_var_list(destList, destVec);

  if (destList.front() == '{')
    return "{ "+get_lhs_taint_list(destVec, taint, newLogic)+" }";
  else
    return get_lhs_taint_list(destVec, taint, newLogic);
}


std::string get_lhs_taint_list_no_slice(std::vector<std::string> &destVec, std::string taint, std::ofstream &output) {
  assert(taint.find(_sig) != std::string::npos);
  std::vector<std::string> taintVec;
  std::smatch m;
  for(std::string singleDest : destVec) {
    if(!isNum(singleDest)) {
      //std::regex_match(singleDest, m, pVarNameGroup);
      //singleDest = std::regex_replace(singleDest, pVarNameGroup, "$1"+taint+"$3");
      std::string dest, destSlice;
      split_slice(singleDest, dest, destSlice);
      singleDest = dest+taint;
    }
    else { // if isNum
      if( !std::regex_match(singleDest, m, pNum)) {
        std::cout << "!! Error in matching number: " + singleDest << std::endl;
        abort();
      }
      std::string numWidth = m.str(1);
      int localIdx = USELESS_VAR++;
      // declare a dummy wire, just for being assigned
      output << "  wire [" + numWidth + "-1:0] nouse_zy" + toStr(localIdx) + " ;" << std::endl;
      singleDest = "nouse_zy" + toStr(localIdx);
    }
    taintVec.push_back(singleDest);    
  }
  std::string returnList = " ";
  for (auto it = taintVec.begin(); it < taintVec.end() - 1; ++it) {
    returnList = returnList + *it + " , ";
  }
  returnList = returnList + taintVec.back() + " ";
  return returnList;
}


std::string get_lhs_taint_list_no_slice(std::string destList, std::string taint, std::ofstream &output) {
  std::vector<std::string> destVec;
  parse_var_list(destList, destVec);
  if (destList.front() == '{')
    return "{ "+get_lhs_taint_list_no_slice(destVec, taint, output)+" }";
  else
    return get_lhs_taint_list_no_slice(destVec, taint, output);
}


int str2int(const std::string& str, std::string info) {
  if(str.length() > 8) {
    toCout("Error: too large int found: "+str);
  }
  int res;
  try{
    res = std::stoi(str);
  }
  catch(std::invalid_argument arg) {
    std::cout << "Wrong input to stoi:" + str << std::endl;
    std::cout << "Info:" + info << std::endl;
    abort();
  }
  return res;
}


void toCout(const std::string& line) {
  std::cout << line << std::endl;
}


void toCoutVerb(const std::string& line) {
  if(g_verb)
    std::cout << line << std::endl;
}


bool isSingleBit(const std::string& slice) {
  static const std::regex pSingleBit("\\[\\d+\\]");
  if(std::regex_search( slice, pSingleBit ))
    return true;
  else
    return false;
}


std::string further_clean_line(std::string line) {
  static const std::regex pSigned("\\$signed");
  std::smatch m;
  if( !std::regex_search(line, m, pSigned) )
    return line;
  uint32_t i;
  size_t lineLen = line.length();
  int cur = line.find('$', 0);
  int closeBracePos = -1;
  while( cur != std::string::npos ) {
    assert( line.substr(cur, 8).compare("$signed(") == 0 );
    line.replace(cur, 8, "");
    int closeBracesExpected = 0;
    for( i = cur; i < lineLen; ++i ) {
      if( line.substr(i, 1) == "(" ) {
        closeBracesExpected++;
      }
      else if( line.substr(i, 1) == ")" && closeBracesExpected == 0 ) {
        closeBracePos = i;
        break;
      }
      else if( line.substr(i, 1) == ")" && closeBracesExpected > 0 ) {
        closeBracesExpected--;
      }
      else if( line.substr(i, 1) == ")" && closeBracesExpected < 0 ) {
        toCout("!! Error in searching for closed braces");
      }
    }
    line.replace(closeBracePos, 1, "");
    cur = line.find('$', cur);
  }
  static const std::regex pBlanks("( )+");
  line = std::regex_replace(line, pBlanks, " " );
  return line;
}


std::string get_recent_rst() {
  if(g_recentRst.compare(g_recentClk) == 0) {
    toCout("Error: rst is same as clk:"+g_recentRst+", "+g_recentClk);
    abort();
  }
  if(g_clkrst_exist && g_recentRst_positive) {
    if(is_neg_rst(g_recentRst)) {
      toCout("neg rst found for positive edge: "+g_recentRst);
    }
    return g_recentRst;
  }
  else if(g_clkrst_exist && !g_recentRst_positive){
    if(!is_neg_rst(g_recentRst)) {
      toCout("pos rst found for negative edge: "+g_recentRst);      
    }
    return "!"+g_recentRst;
  }
  else if(!g_recentRst.empty()) { // if !g_clkrst_exist
    if(is_neg_rst(g_recentRst))
      return "!"+g_recentRst;
    else
      return g_recentRst;
  }
  else {
    return "rst_zy";
  }
}


std::string get_rst() {
  if(g_hasRst)
    return get_recent_rst();
  else
    return "rst_zy";
}


bool isRFlag(const std::string& var) {
  static const std::regex pRFlag("_r_flag");
  std::smatch m;
  return std::regex_search(var, m, pRFlag);
}


/* ASSUME: the first line is either case or function
 * caseAssignPairs are the pairs of (caseValue, case assignment),
 * the input is recovered at the end 
 * If goToEnd, input does not seek beginning */
void parse_func_statements(std::vector<std::pair<std::string, std::string>> &caseAssignPairs, std::vector<std::string> &inputWidth, std::ifstream &input, bool goToEnd) {
  auto funcBegin = input.tellg();
  std::string line;
  std::smatch m;
  // the first line might be case or function definition
  for(int i = 0; i < 3; i++) {
    std::getline(input, line);
    std::regex_match(line, m, pInput);
    inputWidth.push_back(m.str(2));
  }
  std::getline(input, line);
  if( !std::regex_search(line, m, pCase) ) {
    toCout("Error: does not match pCase in parse_func_statements: "+line);
    abort();
  }
  parse_case_statements(caseAssignPairs, input);
  if( !goToEnd )
    input.seekg(funcBegin);
  else {
    std::getline(input, line);
    assert(line.find("endfunction", 0) != std::string::npos);
  }
}


// returns the returnVar in case
std::string parse_case_statements(std::vector<std::pair<std::string, std::string>> &caseAssignPairs, std::ifstream &input, bool returnBegin) {
  auto caseBegin = input.tellg();  
  std::string line;
  std::smatch m;
  std::string firstLine = "";

  bool readSwitchValue = true;
  std::string pairValue;
  std::string pairAssign;
  std::string returnVar;
  while( std::getline(input, line) && !std::regex_match(line, m, pDefault) ) {
    if( readSwitchValue ) {
      readSwitchValue = false;
      if(std::regex_search(line, m, pNumExist)) {
        pairValue = m[0];        
      }
      else {
        pairValue = "default";
      }
    }
    else {
      if(firstLine.empty()) firstLine = line;      
      readSwitchValue = true;
      if( !std::regex_match(line, m, pBlock) ) {
        std::cout << "!! Error in parsing case !!" << std::endl;
      }
      pairAssign = m.str(3);
      caseAssignPairs.emplace_back(pairValue, pairAssign);      
    }
  }
  std::getline(input, line);
  pairValue = "default";
  if( !std::regex_match(line, m, pBlock) ) {
    std::cout << "!! Error in parsing case !!" << std::endl;
  }
  pairAssign = m.str(3);
  returnVar = m.str(2);
  caseAssignPairs.emplace_back(pairValue, pairAssign);
  std::getline(input, line);
  if(line.find("endcase", 0) == std::string::npos){
    toCout("Error: endcase is not found, first line is: "+firstLine);
    abort();
  }
  if(returnBegin) input.seekg(caseBegin);
  return returnVar;
}


std::string pairVec2taintString( std::vector<std::pair<std::string, std::string>> &pairVec, std::string notIncluded, std::string taint, std::ofstream &output ) {
  assert(pairVec.size() >= 2);
  std::vector<std::string> rhsVec;
  for(auto singlePair : pairVec) {
    if( singlePair.second.compare(notIncluded) == 0 )
      continue;
    rhsVec.push_back(singlePair.second);
  }
  std::string res = get_lhs_taint_list(rhsVec, taint, output);
  res = "{" + res + "}";
  return res;
}


std::string max_num(uint32_t width) {
  std::string res = toStr(width)+"'b";
  for(uint32_t i = 0; i < width; i++) {
    res += "1";
  }
  return res;
}


std::string max_num(const std::string& widthStr) {
  uint32_t width = str2int(widthStr, "In max_mum function with input: "+widthStr);
  return max_num(width);
}


uint32_t max_num_dec(uint32_t width) {
  return (uint32_t)(pow(2, width)-1);
}


std::string dec2bin(uint32_t inNum) {
  std::string res="";
  while(inNum > 0) {
    uint32_t bit = inNum % 2;
    res = std::to_string(bit) + res;
    inNum = inNum / 2;
  }
  if (res.empty())
    res = "0";
  return res;
}


// input is a string of binary-format number
std::string get_bits(const std::string& inNum, uint32_t highIdx, uint32_t lowIdx) {
  if(inNum == "0") return "0";
  // input number must be binary
  uint32_t len = inNum.length();
  return inNum.substr(len-highIdx-1, highIdx-lowIdx+1);
}


std::string add_taint(std::vector<std::string> &freeBitsVec, std::string taint) {
  std::string res = "{ ";
  for(std::string freeBits: freeBitsVec) {
    res += insert_taint(freeBits, taint) + ", ";
  }
  res.pop_back();
  res.pop_back();
  res += " }";
  return res;
}


void assert_info(bool val, std::string info) {
  if(val) return;
  toCout(info);
  abort();
}


// in RHS of concatenation expressions, there might be one variable with different slices.
// They can be merged into one variable with a longer slice.
void merge_vec(std::vector<std::string> &srcVec, std::vector<std::string> &destVec) {
  std::string prevVar = "";
  std::string var, varSlice;
  // if useAllSlice = true, then current var should not be merged with next var
  bool useAllSlice = true;
  // if last is not pushed, it is waiting to be merged
  bool lastPushed = true;
  bool cannotMerge;
  uint32_t leftIdx, rightIdx;   // global index
  uint32_t highIdx, lowIdx;     // local index
  assert(destVec.size() == 0);
  for( std::string varAndSlice : srcVec ) {
    split_slice(varAndSlice, var, varSlice);
    if(varSlice.empty()) { // if see a whole var, it cannot be merged with anything
      if(lastPushed) { // if last is pushed, just push the current one
        destVec.push_back(varAndSlice);
        lastPushed = true;
      }
      else{ // if last not pushed, push both the last and current one
        destVec.push_back(prevVar+" ["+toStr(leftIdx)+":"+toStr(rightIdx)+"]");
        destVec.push_back(varAndSlice);
        lastPushed = true;        
      }
      continue;
    }

    // if current contains slices
 
    if(lastPushed) {
      prevVar = var;
      leftIdx = get_end(varSlice);
      rightIdx = get_begin(varSlice);
      if(leftIdx < rightIdx) { // push it and do not merge
        destVec.push_back(varAndSlice);        
        lastPushed = true;
        continue;
      }
      else {
        lastPushed = false;
        continue;        
      }
    }
    
    // if last is not pushed and current one contains slice, check if they can be merged

    if(prevVar.compare(var) != 0) { // if var name does not match, cannot merge
      destVec.push_back(prevVar+" ["+toStr(leftIdx)+":"+toStr(rightIdx)+"]");
      prevVar = var;
      leftIdx = get_end(varSlice);
      rightIdx = get_begin(varSlice);
      lastPushed = false;
      continue;
    }
    else { // var names are the same
      highIdx = get_end(varSlice);
      lowIdx = get_begin(varSlice);
      if(leftIdx > rightIdx) {
        if(highIdx >= lowIdx && rightIdx == highIdx + 1) {
          rightIdx = lowIdx;
          lastPushed = false;
        }
        else { 
          destVec.push_back(prevVar+" ["+toStr(leftIdx)+":"+toStr(rightIdx)+"]");
          prevVar = var;
          leftIdx = get_end(varSlice);
          rightIdx = get_begin(varSlice);
          lastPushed = false;
        }
      }
      else if (leftIdx < rightIdx) {
        toCout("Error: leftIdx < rightIdx for "+var);
        abort();
      }
      else { // leftIdx == rightIdx
        if(highIdx < lowIdx) {//&& rightIdx == highIdx - 1) {
          toCout("Error: leftIdx < rightIdx for "+var);
          abort();
        }
        else { // highIdx > lowIdx or not continuous
          if(rightIdx == highIdx + 1) {
            rightIdx = lowIdx;
            lastPushed = false;
          }
          else{
            destVec.push_back(prevVar+" ["+toStr(leftIdx)+":"+toStr(rightIdx)+"]");
            prevVar = var;
            leftIdx = get_end(varSlice);
            rightIdx = get_begin(varSlice);
            lastPushed = false;
          }
        }
      }
    }
  }
  // push the last
  if(!lastPushed)
    destVec.push_back(prevVar+" ["+toStr(leftIdx)+":"+toStr(rightIdx)+"]");
}


// assume the input must be a reset signal, 
// otherwise abort
bool is_neg_rst(const std::string& var) {
  if ( var.compare("reset_n") == 0
         || var.compare("resetn") == 0
         || var.compare("rstn") == 0 ) {
    return true;
  }
  else if(var.compare("rst") == 0
         || var.compare("i_rst") == 0
         || var.compare("reset") == 0) {
    return false;
  }
  else 
    return !g_rst_pos;
}


void printAndAbort(const std::string& in) {
  toCout(in);
  abort();
}


void checkCond(bool cond, std::string in) {
  if(!cond) {
    printAndAbort(in);
  }
}


std::string expand_slice(const std::string& slice) {
  uint32_t highIdx = get_end(slice);
  uint32_t lowIdx = get_begin(slice);
  return " [" + toStr(highIdx * g_sig_width-1) + ":" + toStr(lowIdx*g_sig_width) + "]";
}


// assume the pure fileName is after the last "/"
std::string extract_path(const std::string& fullFileName) {
  std::size_t pos = fullFileName.find_last_of("/");
  if(pos == std::string::npos) {
    toCout("Error: the fileName does not contain path!");
    abort();
  }
  return fullFileName.substr(0, pos);
}


uint32_t get_dest_ver(const std::string& destAndSlice) {
  std::string dest, destSlice;
  split_slice(destAndSlice, dest, destSlice);
  if(g_destVersion.find(dest) == g_destVersion.end()) {
    g_destVersion.emplace(dest, 0);
    return 0;
  }
  else {
    g_destVersion[dest]++;
    return g_destVersion[dest];
  }
}


// is_srcConcat is needed because it takes infinite time to match pSrcConcat
/*
bool is_srcConcat(std::string line) {
  if(line.find(srcConcatFeature) == std::string::npos)
    return false;
  if(line.find(bothConcatFeature) != std::string::npos)
    return false;

  bool noOperator = true;
  for(auto it = g_operators.begin(); it != g_operators.end(); it++) {
    if(line.find(*it) != std::string::npos) {
      noOperator = false;
      break;
    }
  }
  return noOperator;
}
*/


// is_srcConcat is needed because it takes infinite time to match pDestConcat
//bool is_destConcat(std::string line) {
//  if(line.find("} = ") == std::string::npos)
//    return false;
//  if(line.find(bothConcatFeature) != std::string::npos)
//    return false;
//
//  bool noOperator = true;
//  for(auto it = g_operators.begin(); it != g_operators.end(); it++) {
//    if(line.find(*it) != std::string::npos) {
//      noOperator = false;
//      break;
//    }
//  }
//  return noOperator;
//}


// is_srcConcat is needed because it takes infinite time to match pDestConcat
//bool is_srcDestConcat(std::string line) {
//  if(line.find(bothConcatFeature) == std::string::npos)
//    return false;
//
//  bool noOperator = true;
//  for(auto it = g_operators.begin(); it != g_operators.end(); it++) {
//    if(line.find(*it) != std::string::npos) {
//      noOperator = false;
//      break;
//    }
//  }
//  return noOperator;
//}


std::string extract_bin(const std::string& num, uint32_t highIdx, uint32_t lowIdx) {
  std::smatch m;
  if(std::regex_match(num, m, pBin)) {
    uint32_t width = std::stoi(m.str(1));
    std::string bits = m.str(2);
    if(highIdx >= width) {
      toCout("Error: highIdx is out of bound! highIdx: "+toStr(highIdx)+", num:"+num);
      abort();
    }
    return bits.substr(width-highIdx-1, highIdx-lowIdx+1);
  }
  else if(std::regex_match(num, m, pHex)) {
    uint32_t width = std::stoi(m.str(1));
    std::string bits = m.str(2);
    std::string binBits = hex2bin(bits);
    binBits = binBits.substr(binBits.length()-width);
    return binBits.substr(width-highIdx-1, highIdx-lowIdx+1);
  }
  else {
    toCout("Error: input num for extract_bin is not binary or hex: "+num);
    abort();
  }
}


std::string hex2bin(const std::string& hexNum) {
  std::string res = "";
  for(char c: hexNum) {
    if(c == 'f')
      res += "1111";
    else if(c == 'e')
      res += "1110";
    else if(c == 'd')
      res += "1101";
    else if(c == 'c')
      res += "1100";
    else if(c == 'b')
      res += "1011";
    else if(c == 'a')
      res += "1010";
    else if(c == '9')
      res += "1001";
    else if(c == '8')
      res += "1000";
    else if(c == '7')
      res += "0111";
    else if(c == '6')
      res += "0110";
    else if(c == '5')
      res += "0101";
    else if(c == '4')
      res += "0100";
    else if(c == '3')
      res += "0011";
    else if(c == '2')
      res += "0010";
    else if(c == '1')
      res += "0001";
    else if(c == '0')
      res += "0000";
    else {
      toCout("Error: unexpected char in hex:"+hexNum);
      abort();
    }
  }
  return res;
}


// input string might contain variables besides bit-vectors
std::string split_long_bit_vec(const std::string& varList) {
  size_t lastPos = 0;
  std::smatch m;
  std::string ret;
  if(varList.find(",") == std::string::npos)
    return varList;

  bool isBool;
  while(varList.find(", ", lastPos) != std::string::npos) {
    size_t end = varList.find(", ", lastPos);
    std::string var = varList.substr(lastPos, end-lastPos);
    lastPos = end+2;
    if(!isNum(var)) {
      ret = ret + var + ", ";
      continue;
    }
    if(std::regex_match(var, m, pBinX)) {
      uint32_t width = std::stoi(m.str(1));
      std::string num = m.str(2);
      ret = split_long_bin(var, width, num, ret);
    }
    else if(std::regex_match(var, m, pHex)) {
      uint32_t width = std::stoi(m.str(1));
      std::string num = m.str(2);
      ret = split_long_hex(var, width, num, ret);
    }
    else {
      toCout("Error: number does not match any pattern1: "+var);
      abort();
    }
  } // end of while

  // process the last one
  std::string var = varList.substr(lastPos);
  if(!isNum(var)) {
    ret = ret + var;
    return ret;
  }
  if(std::regex_match(var, m, pBinX)) {
    uint32_t width = std::stoi(m.str(1));
    std::string num = m.str(2);
    ret = split_long_bin(var, width, num, ret);
  }
  else if(std::regex_match(var, m, pHex)) {
    uint32_t width = std::stoi(m.str(1));
    std::string num = m.str(2);
    ret = split_long_hex(var, width, num, ret);
  }
  else {
    toCout("Error: number does not match any pattern2: "+var);
    abort();
  }

  assert(ret.length() > 3);
  ret.pop_back();
  if(ret.back() != ',') {
    toCout("Error: the last char is not comma: "+ret);
    abort();
  }
  ret.pop_back();
  return ret;
}


std::string remove_signed(const std::string &line) {
  std::smatch m;
  static const std::regex pSigned("\\$signed\\((.*)\\)");
  if(line.find("$signed") != std::string::npos) {
    return std::regex_replace(line, pSigned, "$1");
    //toCout("line to be matched: "+line);
  }
  else return line;
}


std::string split_long_hex(const std::string& var, uint32_t width, std::string num, std::string strToConcat) {
  if(width < 32) {
    strToConcat = strToConcat + var + ", ";
    return strToConcat;
  }
  // split into multiple 16 bit number
  size_t pos = 0;
  while(pos < width/4) {
    //toCout("begin ite, pos: "+toStr(pos));
    uint32_t subWidth = std::min(16, int(width-pos*4));
    std::string subVar;
    if(subWidth == 16) subVar = toStr(subWidth)+"'h"+num.substr(pos, subWidth/4);
    else subVar = toStr(subWidth)+"'h"+num.substr(pos);    
    pos += 4;
    //toCout("pos: "+toStr(pos));      
    strToConcat = strToConcat + subVar + ", ";
  }
  return strToConcat;
}


std::string split_long_bin(const std::string& var, uint32_t width, std::string num, std::string strToConcat) {
  if(width < 31) {
    strToConcat = strToConcat + var + ", ";
    return strToConcat;
  }
  // split into multiple 16 bit number
  size_t pos = 0;
  while(pos < width) {
    uint32_t subWidth = std::min(16, int(width-pos));
    std::string subVar = toStr(subWidth)+"'b"+num.substr(pos, subWidth);
    pos += 16;
    //toCout("pos: "+toStr(pos));
    strToConcat = strToConcat + subVar + ", ";
  }
  return strToConcat;
}


void fill_var_width(const std::string &line, VarWidth &varWidth) {
  uint32_t choice = parse_verilog_line(line, true);
  std::smatch m;
  bool inFunc = false;
  switch (choice) {
    case INPUT:
      {
        std::regex_match(line, m, pInput);
        std::string slice = m.str(2);
        std::string var = m.str(3);
        bool insertDone = false;
        if(!inFunc) {
          if(!slice.empty())
            insertDone = varWidth.var_width_insert(var, get_end(slice), get_begin(slice));
          else
            insertDone = varWidth.var_width_insert(var, 0, 0);
        }
        else {
          if(!slice.empty())
            insertDone = funcVarWidth.force_insert(var, get_end(slice), get_begin(slice));
          else
            insertDone = funcVarWidth.force_insert(var, 0, 0);
        }     
        //if (!insertDone) {
        //  std::cout << "insert failed in input case:" + line << std::endl;
        //  std::cout << "m.str(2):" + m.str(2) << std::endl;
        //  std::cout << "m.str(3):" + m.str(3) << std::endl;
        //}
      }
      break;
    case REG:
      {
        if(!std::regex_match(line, m, pReg)
            && !std::regex_match(line, m, pRegConst) ) {
          toCout("Error in matching pReg or pRegConst: "+line);
          abort();
        }
        std::string slice = m.str(2);
        std::string var = m.str(3);
        bool insertDone = false;
        if(!inFunc) {
          if(!slice.empty())
            insertDone = varWidth.var_width_insert(var, get_end(slice), get_begin(slice));
          else
            insertDone = varWidth.var_width_insert(var, 0, 0);
        }
        else {
          if(!slice.empty())
            insertDone = funcVarWidth.force_insert(var, get_end(slice), get_begin(slice));
          else
            insertDone = funcVarWidth.force_insert(var, 0, 0);
        }
        //if (!insertDone) {
        //  std::cout << "insert failed in reg case:" + line << std::endl;
        //  std::cout << "m.str(2):" + m.str(2) << std::endl;
        //  std::cout << "m.str(3):" + m.str(3) << std::endl;
        //}
      }
      break;
    case WIRE:
      {
        std::regex_match(line, m, pWire);
        std::string slice = m.str(2);
        std::string var = m.str(3);
        bool insertDone = false;
        if(!inFunc) {
          if(!slice.empty())
            insertDone = varWidth.var_width_insert(var, get_end(slice), get_begin(slice));
          else
            insertDone = varWidth.var_width_insert(var, 0, 0);
        }
        else {
          if(!slice.empty())
            insertDone = funcVarWidth.force_insert(var, get_end(slice), get_begin(slice));
          else
            insertDone = funcVarWidth.force_insert(var, 0, 0);
        }
        //if (!insertDone) {
        //  std::cout << "insert failed in wire case:" + line << std::endl;
        //  std::cout << "m.str(2):" + m.str(2) << std::endl;
        //  std::cout << "m.str(3):" + m.str(3) << std::endl;
        //}
      }
      break;
    case OUTPUT:
      {
        std::regex_match(line, m, pOutput);
        std::string slice = m.str(2);
        std::string var = m.str(3);
        bool insertDone = false;
        if(!inFunc) {
          if(!slice.empty())
            insertDone = varWidth.var_width_insert(var, get_end(slice), get_begin(slice));
          else
            insertDone = varWidth.var_width_insert(var, 0, 0);
        }
        else {
          if(!slice.empty())
            insertDone = funcVarWidth.force_insert(var, get_end(slice), get_begin(slice));
          else
            insertDone = funcVarWidth.force_insert(var, 0, 0);
        }
        //if (!insertDone) {
        //  std::cout << "insert failed in output case:" + line << std::endl;
        //  std::cout << "m.str(2):" + m.str(2) << std::endl;
        //  std::cout << "m.str(3):" + m.str(3) << std::endl;
        //}
      }
      break;
    case FUNCDEF:
      {
        inFunc = true;
      }
      break;
    case FUNCEND:
      {
        inFunc = false;
      }
      break;
    default:
      break;
  }
}


void remove_back_space(std::string &str) {
  while(!str.empty() && std::isspace(str.back()))
    str.pop_back();
}


void remove_front_space(std::string &str) {
  size_t pos = str.find_first_not_of(" \t");
  if(pos == std::string::npos) return; 
  str = str.substr(pos);
}

void remove_two_end_space(std::string &str) {
  if(str.empty()) {
    toCout("Warning: see an empty str in remove_two_end_space");
    return;
  }
  remove_front_space(str);
  remove_back_space(str);
}


bool is_srcConcat(const std::string &line) {
  if(line.find(" = {") != std::string::npos
      && line.find("assign") != std::string::npos) {
    auto pos = line.find(" = {");
    std::string destPart = line.substr(0, pos);
    if(destPart.find("{") == std::string::npos)
      toCoutVerb("Find src concat!");
    else
      return false;
  }
  else return false;
  if(line.find("[") != std::string::npos)
    toCoutVerb("Find it!");
  std::smatch m;
  //if(!std::regex_match(line, m, pSrcConcat))
  //  return false;
  size_t openBracePos = line.find("= {");
  std::string firstPart = line.substr(0, openBracePos+4);
  std::string secondPart = line.substr(openBracePos+5);
  size_t closeBracePos = secondPart.find("};");
  secondPart = secondPart.substr(0, closeBracePos);
  if(!std::regex_match(firstPart, m, pSrcConcatFirstPart)) {
    toCout("Error: pSrcConcatFirstPart is not matched: "+firstPart);
    abort();
  }

  std::string varList = secondPart;
  std::vector<std::string> varVec;
  boost::split(varVec, varList, boost::is_any_of(","));
  return vec_has_only_vars(varVec);
}


bool is_destConcat(const std::string &line) {
  static const std::regex pLocalName("[a-zA-Z0-9_=\\.\\$\\\\'\\[\\]\\(\\)]+(?:\\s*\\[\\d+(?:\\:\\d+)?\\])?");  
  std::smatch m;
  if(!std::regex_match(line, m, pDestConcat))
    return false;
  std::string varList = m.str(2);
  std::vector<std::string> varVec;
  boost::split(varVec, varList, boost::is_any_of(","));
  return vec_has_only_vars(varVec);
}


bool is_srcDestConcat(const std::string &line) {
  static const std::regex pLocalName("[a-zA-Z0-9_=\\.\\$\\\\'\\[\\]\\(\\)]+(?:\\s*\\[\\d+(?:\\:\\d+)?\\])?");  
  std::smatch m;
  if(!std::regex_match(line, m, pSrcDestBothConcat))
    return false;
  std::string srcList = m.str(2);
  std::string destList = m.str(3);
  std::vector<std::string> srcVec;
  std::vector<std::string> destVec;
  boost::split(srcVec, srcList, boost::is_any_of(","));
  boost::split(destVec, destList, boost::is_any_of(","));
  return vec_has_only_vars(srcVec) && vec_has_only_vars(destVec);
}


bool vec_has_only_vars(const std::vector<std::string> &vec) {
  static const std::regex pLocalName("[a-zA-Z0-9_=\\.\\$\\\\'\\[\\]\\(\\)]+(?:\\s*\\[\\d+(?:\\:\\d+)?\\])?");  
  std::smatch m;  
  for(std::string var: vec) {
    remove_two_end_space(var);
    if(!std::regex_match(var, m, pLocalName))
      return false;
  }
  return true;
}


bool is_concat(std::string var) {
  remove_two_end_space(var);
  if(var.front() != '{' || var.back() != '}') return false;
  if(var.find(",") == std::string::npos) {
    toCout("Error: no concatenations in braces: "+var);
  }
  return true;
}


bool split_concat(std::string var, std::vector<std::string> &vec) {
  remove_two_end_space(var);
  if(var.front() != '{' || var.back() != '}') return false;
  size_t pos = 1;
  size_t commaPos;
  do {
    commaPos = var.find_first_of(',' , pos);
    std::string oneVar;
    if(commaPos != std::string::npos) {
      oneVar = var.substr(pos, commaPos-pos);
    }
    else {
      oneVar = var.substr(pos);
      oneVar.pop_back();
    }
    remove_two_end_space(oneVar);
    vec.push_back(oneVar);
    pos = commaPos + 1;
  } while(commaPos != std::string::npos);
  return true;
}


bool check_input_val(const std::string& value) {
  static const std::regex pX("^(\\d+)'[b|h]x$");
  std::smatch m;
  if(value == "x" || is_number(value) 
      || value != "DIRTY" || std::regex_match(value, m, pX))
    return true;
  else if(value.find("+") != std::string::npos) {
    uint32_t pos = value.find("+");
    return check_input_val(value.substr(0, pos)) && check_input_val(value.substr(pos+1));
  }
  else
    return false;
}


void split_by(std::string str, std::string separator, std::vector<std::string> &vec) {
  assert(vec.empty());
  if(str.find(separator) == std::string::npos) {
    remove_two_end_space(str);
    vec.push_back(str);
    return;
  }
  remove_two_end_space(str);
  size_t pos = 0;
  while(str.find(separator, pos) != std::string::npos) {
    size_t endPos = str.find(separator, pos);
    std::string singleVal = str.substr(pos, endPos-pos);
    pos = endPos + 1;
    remove_two_end_space(singleVal);
    vec.push_back(singleVal);
  }
  std::string singleVal = str.substr(pos);
  remove_two_end_space(singleVal);
  vec.push_back(singleVal);
}

// A cooler version of split_by()
void split_by_regex(const std::string& str, const std::regex& re, std::vector<std::string> &vec) {

    auto words_begin =
        std::sregex_iterator(str.begin(), str.end(), re);
    auto words_end = std::sregex_iterator();

    for (std::sregex_iterator i = words_begin; i != words_end; ++i) {
        std::smatch match = *i;
        std::string match_str = match.str();
        remove_two_end_space(match_str);
        vec.push_back(match_str);
    }
}


// A slightly easier to use (but slower) version of split_by_regex()
void split_by_regex(const std::string& str, const std::string& re_str, std::vector<std::string> &vec) {
  std::regex re(re_str);
  split_by_regex(str, re, vec);
}



std::string merge_with(const std::vector<std::string> &vec, std::string connector) {
  std::string ret = vec.front();
  for(uint32_t i = 1; i < vec.size(); i++) {
    ret = ret + connector + vec[i];
  }
  return ret;
}


std::string dec2hex(const std::string& decimalValue) {
  uint32_t val = std::stoi(decimalValue);
  return dec2hex(val);
}


std::string dec2hex(uint32_t decimalValue) {
  std::stringstream ss;
  ss << std::hex << decimalValue; // int decimal_value
  std::string res ( ss.str() );
  return res;
}


void print_reg_list(const std::string& moduleName) {
  if(moduleTrueRegs.empty()) return;
  std::ofstream output;
  output.open(g_path+"/reg_list.txt", std::ios::app);
  output << "module:"+moduleName << std::endl;
  for(std::string reg: moduleTrueRegs) {
    output << reg << std::endl;
  }
  output << std::endl;
  output.close();
}


bool is_mem_if_assign(const std::string &line) {
  return false;
}

} // end of namespace taintGen
