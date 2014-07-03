/**
 * @file Estimable.h
 * @author  Scott Rasmussen (scott.rasmussen@zaita.com)
 * @date 20/06/2014
 * @section LICENSE
 *
 * Copyright NIWA Science �2014 - www.niwa.co.nz
 *
 * @section DESCRIPTION
 *
 * This report will print an estimable out during a year/time step
 * so you can track the value of it over time (useful when it's
 * been modified during a projection etc)
 */
#ifndef REPORTS_ESTIMABLE_H_
#define REPORTS_ESTIMABLE_H_

// headers
#include "Reports/Report.h"

// namespaces
namespace isam {
namespace reports {

/**
 * class definition
 */
class Estimable : public isam::Report {
public:
  // methods
  Estimable();
  virtual                     ~Estimable() = default;
  void                        DoValidate() override final;
  void                        DoBuild() override final;
  void                        DoPrepare() override final;
  void                        DoExecute() override final;
  void                        DoFinalise() override final;

private:
  // members
  string                      parameter_;
  Double*                     target_;
};

} /* namespace reports */
} /* namespace isam */

#endif /* ESTIMABLE_H_ */
