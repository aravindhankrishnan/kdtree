#pragma once

// string str = "val3";
// BEGIN_STRING_SWITCH_ (str)
//      CASE_("val1")
//         
//
//      CASE_("val2")
//
//
//      CASE_("val3")
//
//      DEFAULT
//
//  END_STRING_SWITCH

#include <iostream>

#define BEGIN_STRING_SWITCH_(str) std::string& _i_n_p_u_t_ = str; if (0) {
#define CASE_(val) } else if (_i_n_p_u_t_ == val) {
#define DEFAULT_  } else {
#define END_STRING_SWITCH_ }
