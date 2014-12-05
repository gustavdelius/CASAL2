/**
 * @file Schnute.h
 * @author  Scott Rasmussen (scott.rasmussen@zaita.com)
 * @date 12/08/2013
 * @section LICENSE
 *
 * Copyright NIWA Science �2013 - www.niwa.co.nz
 *
 * @section DESCRIPTION
 *
 * << Add Description >>
 */
#ifndef SCHNUTE_H_
#define SCHNUTE_H_

// headers
#include "AgeSizes/AgeSize.h"
#include "SizeWeights/SizeWeight.h"

// namespaces
namespace niwa {
namespace agesizes {

/**
 * class definitions
 */
class Schnute : public niwa::AgeSize {
public:
  // methods
  Schnute();
  virtual                     ~Schnute() = default;
  void                        DoValidate() override final;
  void                        DoBuild() override final;
  void                        DoReset() override final { };

  // accessors
  Double                      mean_size(unsigned year, unsigned age) override final;
  Double                      mean_weight(unsigned year, unsigned age) override final;

private:
  // members
  Double                      y1_;
  Double                      y2_;
  Double                      tau1_;
  Double                      tau2_;
  Double                      a_;
  Double                      b_;
  Double                      cv_;
  string                      distribution_;
  bool                        by_length_;
  string                      size_weight_label_;
  SizeWeightPtr               size_weight_;
};

} /* namespace agesizes */
} /* namespace niwa */
#endif /* SCHNUTE_H_ */
