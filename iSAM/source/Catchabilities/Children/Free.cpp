/**
 * @file Free.cpp
 * @author Scott Rasmussen (scott.rasmussen@zaita.com)
 * @github https://github.com/Zaita
 * @date 12/11/2014
 * @section LICENSE
 *
 * Copyright NIWA Science �2014 - www.niwa.co.nz
 *
 */

// headers
#include "Free.h"

// namespaces
namespace niwa {
namespace catchabilities {

/**
 * Default constructor
 */
Free::Free() {
  parameters_.Bind<Double>(PARAM_Q, &q_, "The catchability amount", "");

  RegisterAsEstimable(PARAM_Q, &q_);
}

/**
 *
 */
void Free::DoValidate() { }


} /* namespace catchabilities */
} /* namespace niwa */
