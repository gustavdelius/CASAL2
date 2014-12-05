/**
 * @file String.cpp
 * @author  Scott Rasmussen (scott.rasmussen@zaita.com)
 * @version 1.0
 * @date 20/12/2012
 * @section LICENSE
 *
 * Copyright NIWA Science �2012 - www.niwa.co.nz
 *
 * $Date: 2008-03-04 16:33:32 +1300 (Tue, 04 Mar 2008) $
 */

// Headers
#include "String.h"

#include <algorithm>
#include <boost/algorithm/string/replace.hpp>
#include <boost/algorithm/string/trim_all.hpp>
#include <boost/algorithm/string/split.hpp>
#include <boost/algorithm/string/join.hpp>

#include "Utilities/To.h"

// Namespaces
namespace niwa {
namespace utilities {
namespace strings {


bool is_valid(const std::string& test_string) {
 if (test_string.length() == 0)
   return false;

 std::string special_chars = "[]()._-:";

 bool invalid
 = std::find_if(test_string.begin(), test_string.end(),
                    [&special_chars](char c) { return (!std::isalpha(c) && special_chars.find(c) == string::npos); }) != test_string.end();

 return !invalid;
}

vector<std::string> explode(const std::string& source) {
  vector<std::string> result;

  vector<std::string> chunks;
  boost::split(chunks, source, boost::is_any_of(","));

  for (std::string index : chunks) {
    if (index.find(":") != std::string::npos) {
      vector<string> pieces;
      boost::split(pieces, index, boost::is_any_of(":"));
      if (pieces.size() != 2)
        return vector<string>();

      unsigned first = 0;
      unsigned second = 0;

      if (!utilities::To<std::string, unsigned>(pieces[0], first))
        return vector<string>();
      if (!utilities::To<std::string, unsigned>(pieces[1], second))
        return vector<string>();

      if (first < second) {
        for (unsigned i = first; i <= second; ++i)
          result.push_back(utilities::ToInline<unsigned, std::string>(i));
      } else {
        for (unsigned i = first; i >= second; --i)
          result.push_back(utilities::ToInline<unsigned, std::string>(i));
      }

    } else
      result.push_back(index);
  }

  return result;
}

}
} /* namespace utilities */
} /* namespace niwa */
