/**
 * @file Beta.cpp
 * @author  Scott Rasmussen (scott.rasmussen@zaita.com)
 * @date 24/09/2014
 * @section LICENSE
 *
 * Copyright NIWA Science �2014 - www.niwa.co.nz
 *
 */

// headers
#include "Beta.h"

// namespaces
namespace niwa {
namespace estimates {
namespace creators {

/**
 * Default constructor
 */
Beta::Beta() {
  parameters_.Bind<Double>(PARAM_MU, &mu_, "Mu", "");
  parameters_.Bind<Double>(PARAM_SIGMA, &sigma_, "Sigma", "");
  parameters_.Bind<Double>(PARAM_A, &a_, "A", "");
  parameters_.Bind<Double>(PARAM_B, &b_, "B", "");
}

/**
 *
 */
void Beta::DoCopyParameters(niwa::EstimatePtr estimate, unsigned index) {
  estimate->parameters().CopyFrom(parameters_, PARAM_MU, index);
  estimate->parameters().CopyFrom(parameters_, PARAM_SIGMA, index);
  estimate->parameters().CopyFrom(parameters_, PARAM_A, index);
  estimate->parameters().CopyFrom(parameters_, PARAM_B, index);
}


} /* namespace creators */
} /* namespace estimates */
} /* namespace niwa */
