/**
 * @file VonBertalanffy.cpp
 * @author  Scott Rasmussen (scott.rasmussen@zaita.com)
 * @date 14/08/2013
 * @section LICENSE
 *
 * Copyright NIWA Science �2013 - www.niwa.co.nz
 *
 */

// headers
#include "VonBertalanffy.h"

#include "Model/Model.h"
#include "LengthWeights/Manager.h"
#include "TimeSteps/Manager.h"

// namespaces
namespace niwa {
namespace agelengths {

/**
 * Default constructor
 *
 * Bind any parameters that are allowed to be loaded from the configuration files.
 * Set bounds on registered parameters
 * Register any parameters that can be an estimated or utilised in other run modes (e.g profiling, yields, projections etc)
 * Set some initial values
 *
 * Note: The constructor is parsed to generate Latex for the documentation.
 */
VonBertalanffy::VonBertalanffy() {
  parameters_.Bind<Double>(PARAM_LINF, &linf_, "TBA", "")->set_lower_bound(0.0);
  parameters_.Bind<Double>(PARAM_K, &k_, "TBA", "")->set_lower_bound(0.0);
  parameters_.Bind<Double>(PARAM_T0, &t0_, "TBA", "");
  parameters_.Bind<string>(PARAM_LENGTH_WEIGHT, &length_weight_label_, "TBA", "");
  parameters_.Bind<Double>(PARAM_CV_FIRST, &cv_first_ , "CV for the first age class", "",Double(0.0))->set_lower_bound(0.0);
  parameters_.Bind<Double>(PARAM_CV_LAST, &cv_last_ , "CV for maximum age", "",Double(0.0))->set_lower_bound(0.0);
  parameters_.Bind<string>(PARAM_DISTRIBUTION, &distribution_, "TBA", "", PARAM_NORMAL);
  parameters_.Bind<bool>(PARAM_BY_LENGTH, &by_length_, "Specifies if the linear interpolation of CV's is a linear function of mean length at age. Default is just by age", "", true);

  RegisterAsEstimable(PARAM_LINF, &linf_);
  RegisterAsEstimable(PARAM_K, &k_);
  RegisterAsEstimable(PARAM_T0, &t0_);
  RegisterAsEstimable(PARAM_CV_FIRST, &cv_first_);
  RegisterAsEstimable(PARAM_CV_LAST, &cv_last_);

}

/**
 * Build any objects that will need to be utilised by this object.
 * Obtain smart_pointers to any objects that will be used by this object.
 */
void VonBertalanffy::DoBuild() {
  length_weight_ = lengthweights::Manager::Instance().GetLengthWeight(length_weight_label_);
  if (!length_weight_)
    LOG_ERROR_P(PARAM_LENGTH_WEIGHT) << "(" << length_weight_label_ << ") could not be found. Have you defined it?";
}

/**
 * Get the mean length of a single population
 *
 * @param year The year we want mean length for
 * @param age The age of the population we want mean length for
 * @return The mean length for 1 member
 */
Double VonBertalanffy::mean_length(unsigned year, unsigned age) {
  Double proportion = time_step_proportions_[timesteps::Manager::Instance().current_time_step()];

  if ((-k_ * ((age + proportion) - t0_)) > 10)
    LOG_ERROR_P(PARAM_K) << "exp(-k*(age-t0)) is enormous. The k or t0 parameters are probably wrong.";

  Double size = linf_ * (1 - exp(-k_ * ((age + proportion) - t0_)));
  if (size < 0.0)
    return 0.0;

  return size;
}

/**
 * Get the mean weight of a single population
 *
 * @param year The year we want mean weight for
 * @param age The age of the population we want mean weight for
 * @return mean weight for 1 member
 */
Double VonBertalanffy::mean_weight(unsigned year, unsigned age) {
  Double size = this->mean_length(year, age);
  Double mean_weight = 0.0; //
  mean_weight = length_weight_->mean_weight(size, distribution_, cvs_[age]);// make a map [key = age]
  return mean_weight;
}

/**
 * Create look up vector of CV's that gets feed into mean_weight
 * And Age Length Key.
 * if cv_last_ and cv_first_ are time varying then this should be built every year
 * also if by_length_ is called, it will be time varying because it calls mean_weight which has time_varying
 * parameters. Otherwise it only needs to be built once a model run I believe
 */
void VonBertalanffy::BuildCV(unsigned year) {
  unsigned min_age = Model::Instance()->min_age();
  unsigned max_age = Model::Instance()->max_age();

  if (cv_last_==0.0) { // A test that is robust... If cv_last_ is not in the input then assume cv_first_ represents the cv for all age classes i.e constant cv
    for (unsigned i = min_age; i <= max_age; ++i) {
      cvs_[i] = (cv_first_);
    }
  } else if(by_length_) {  // if passed the first test we have a min and max CV. So ask if this is interpolated by length at age
    for (unsigned i = min_age; i <= max_age; ++i) {
      cvs_[i] = ((mean_length(year, i) - mean_length(year, min_age)) * (cv_last_ - cv_first_) / (mean_length(year, max_age) - mean_length(year, min_age)) + cv_first_);
    }

  } else {
    // else Do linear interpolation between cv_first_ and cv_last_ based on age class
    for (unsigned i = min_age; i <= max_age; ++i) {
      cvs_[i] = (cv_first_ + (cv_last_ - cv_first_) * (i - min_age) / (max_age - min_age));
    }
  }
}

/**
 * Do the conversion of the partition structure from age to length
 *
 * @param category The current category to convert
 */

void VonBertalanffy::DoAgeToLengthConversion(std::shared_ptr<partition::Category> category) {
////
//  unsigned min_a = category->min_age_;
//  unsigned max_a = category->max_age_;
////  unsigned year = Model::Instance()->current_year();
////  category->age_length()->BuildCv(year);
////  category->UpdateMeanLengthData();
////
//  for (unsigned age = min_a; age < max_a; ++age) {
////
//    Double cv = 0.1;// category->age_length()->cv(age);
//    bool plus_grp = Model::Instance()->age_plus();
//    CummulativeNormal(category->length_per_[age], cv, &Age_freq_, class_mins_, distribution_, Temp_vec, plus_grp);
////
////    // Loop through the length bins and multiple the partition of the current age to go from
////    // length frequencies to age length numbers
//      for (unsigned iter = 0; iter < class_mins_.size(); ++iter) {
//      category->length_data_[age][iter] = category->data_[age] * Age_freq_[iter];
//    }
//  }
}

void VonBertalanffy::CummulativeNormal(Double mu, Double cv, vector<Double> *vprop_in_length, vector<Double> class_mins, string distribution, vector<Double>  Class_min_temp, bool plus_grp) {
  // est proportion of age class that are in each length interval
  Class_min_temp = class_mins; // Temp vec is a vector that all the work gets done on.
  Double sigma = cv; //* mu;
  if (distribution == "lognormal") { // if lognormal, convert growth parameters to log space
    // Put parameters in to log space
    Double cv = sigma / mu;
    Double Lvar = log(cv * cv + 1.0);
    mu = log(mu) - Lvar / 2.0;
    sigma = sqrt(Lvar);

    if (class_mins[0] < 0.0001)
      Class_min_temp[0] = log(0.0001);
    else
      Class_min_temp[0] = log(class_mins[0]);
    if (class_mins.size() > 1)
      for (unsigned h = 1; h < class_mins.size(); h++)
        Class_min_temp[h] = log(class_mins[h]);

  }

  Double z, tt, norm, ttt, tmp;
  Double sum = 0;

  vector<Double> cum;
  const Double kPi = 3.141592653589793;
  std::vector<int>::size_type sz = class_mins.size();
  vprop_in_length->resize(sz);

  for (unsigned j = 0; j < sz; ++j) {
    z = fabs((Class_min_temp[j] - mu)) / sigma;
    tt = 1.0 / (1.0 + 0.2316419 * z);
    norm = 1.0 / sqrt(2.0 * kPi) * exp(-0.5 * z * z);
    ttt = tt;
    tmp = 0.319381530 * ttt;
    ttt = ttt * tt;
    tmp = tmp - 0.356563782 * ttt;          // tt^2
    ttt = ttt * tt;
    tmp = tmp + 1.781477937 * ttt;       // tt^3
    ttt = ttt * tt;
    tmp = tmp - 1.821255978 * ttt;        // tt^4
    ttt = ttt * tt;
    tmp = tmp + 1.330274429 * ttt;      // tt^5

    cum.push_back(1.0 - norm * tmp); //Crashing
    if (Class_min_temp[j] < mu) {
      cum[j] = 1.0 - cum[j];
    }
    if (j > 0) {
      (*vprop_in_length)[j - 1] = cum[j] - cum[j - 1];
      sum += (*vprop_in_length)[j - 1];
    }
  }
  if (plus_grp) {
    (*vprop_in_length)[sz - 1] = 1.0 - sum - cum[0];
  } else
    vprop_in_length->resize(sz - 1);
}

} /* namespace agelengths */
} /* namespace niwa */
