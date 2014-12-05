/**
 * @file Info.h
 * @author  Scott Rasmussen (scott.rasmussen@zaita.com)
 * @version 1.0
 * @date 14/01/2013
 * @section LICENSE
 *
 * Copyright NIWA Science �2013 - www.niwa.co.nz
 *
 * @section DESCRIPTION
 *
 * This class is a the configuration file representation of the
 * estimate that we've specified. We use an intermediate stage because
 * there is not going to be a 1:1 relationship between the configuration
 * file block and the number of estimates we need to create for it.
 *
 * This class is responsible for creating N estimates per configuration
 * block. This class does have a 1:1 relationship to the configuration
 * file.
 *
 * $Date: 2008-03-04 16:33:32 +1300 (Tue, 04 Mar 2008) $
 */
#ifndef ESTIMATES_INFO_H_
#define ESTIMATES_INFO_H_

// Headers
#include <boost/enable_shared_from_this.hpp>

#include "BaseClasses/Object.h"

// Namespaces
namespace niwa {
namespace estimates {

/**
 * Class definition
 */
class Info : public niwa::base::Object, public boost::enable_shared_from_this<Info> {
public:
  // Methods
  Info();
  virtual                     ~Info();
  void                        Validate();
  void                        BuildEstimates();

private:
  // Members
  string                      parameter_;
  Double                      lower_bound_;
  Double                      upper_bound_;
  string                      prior_label_;
  string                      same_;
  string                      estimation_phase_;
  string                      mcmc_;
};

typedef boost::shared_ptr<niwa::estimates::Info> EstimateInfoPtr;

} /* namespace estimates */
} /* namespace niwa */
#endif /* ESTIMATES_INFO_H_ */
