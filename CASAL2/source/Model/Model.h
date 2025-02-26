/**
 * @file Model.h
 * @author  Scott Rasmussen (scott.rasmussen@zaita.com)
 * @version 1.0
 * @date 16/11/2012
 * @section LICENSE
 *
 * Copyright NIWA Science �2012 - www.niwa.co.nz
 *
 * @section DESCRIPTION
 *
 * This class is the primary representation of our model and it's states
 *
 * $Date: 2008-03-04 16:33:32 +1300 (Tue, 04 Mar 2008) $
 */
#ifndef MODEL_H_
#define MODEL_H_

// Headers
#include "BaseClasses/Executor.h"
#include "BaseClasses/Object.h"
#include "GlobalConfiguration/GlobalConfiguration.h"
#include "Utilities/Math.h"
#include "Utilities/PartitionType.h"
#include "Utilities/RunMode.h"

// Namespaces
namespace niwa {
using base::Executor;
class Managers;
class Objects;
class Categories;
class Factory;
class Partition;
class ObjectiveFunction;
class EquationParser;

namespace State {
enum Type {
  kStartUp, // system is loading from configuration file
  kValidate, // validating user supplied values for variables
  kBuild, // building and checking relationships between objects
  kVerify, // verifying business rules (not yet implemented)
  kInitialise, // running through the initialisation phases
  kExecute, // execute the object
  kIterationComplete, // a single iteration of the model is complete
  kReset, // called between iterations to ensure objects caches are reset
  kInputIterationComplete, // a single run of the mode is complete using an input file to set estimables
  kFinalise // the model is finished
};
}

namespace Units {
enum Type {
  kGrams,
  kKilograms,
  kTonnes
};
} /* namespace Units */

/**
 * Class definition
 */
class Model : public base::Object {
public:
  // Methods
  Model();
  virtual                     ~Model();
  bool                        Start(RunMode::Type run_mode);
  void                        FullIteration();
  void                        Subscribe(State::Type state, Executor* executor) { executors_[state].push_back(executor); }
  void                        PopulateParameters();

  // Accessors
  RunMode::Type               run_mode() const { return run_mode_; }
  State::Type                 state() const { return state_; }
  virtual unsigned            start_year() const { return start_year_; }
  virtual unsigned            final_year() const { return final_year_; }
  unsigned                    projection_final_year() const { return projection_final_year_;}
  Double                      b0(string derived_quantity_label) {return b0_[derived_quantity_label];}
  void                        set_b0(string derived_quantity_label, Double new_b0) {b0_[derived_quantity_label] = new_b0;}
  Double                      binitial(string derived_quantity_label) {return binitial_[derived_quantity_label];}
  void                        set_binitial(string derived_quantity_label, Double new_binitial) {binitial_[derived_quantity_label] = new_binitial;}
  bool                        b0_initialised(string derived_quantity_label) {return b0_initialised_[derived_quantity_label];}
  void                        set_b0_initialised(string derived_quantity_label, bool new_b0_initialised) {b0_initialised_[derived_quantity_label] = new_b0_initialised;}
  bool                        projection_final_phase() {return projection_final_phase_;}
  void                        set_projection_final_phase(bool phase) {projection_final_phase_ = phase;}
  virtual vector<unsigned>    years() const;
  virtual vector<unsigned>    years_all() const;
  unsigned                    year_spread() const;
  virtual unsigned            current_year() const { return current_year_; }
  virtual string              base_weight_units() const { return base_weight_units_; }
  virtual unsigned            min_age() const { return min_age_; }
  virtual unsigned            max_age() const { return max_age_; }
  virtual unsigned            age_spread() const { return (max_age_ - min_age_) + 1; }
  virtual bool                age_plus() const { return age_plus_; }
  virtual const vector<string>& time_steps() const { return time_steps_; }
  virtual const vector<string>& initialisation_phases() const { return initialisation_phases_; }
  void                        set_partition_type(PartitionType partition_type) { partition_type_ = partition_type; }
  virtual PartitionType       partition_type() const { return partition_type_; }
  virtual const vector<unsigned>&     length_bins() const { return length_bins_; }
  virtual bool                length_plus() const { return length_plus_; }
  virtual unsigned            length_plus_group() const { return length_plus_group_; }

  // manager accessors
  virtual Managers&           managers();
  virtual Objects&            objects();
  GlobalConfiguration&        global_configuration() { return *global_configuration_; }
  virtual Categories*         categories() { return categories_; }
  virtual Factory&            factory();
  virtual Partition&          partition();
  virtual ObjectiveFunction&  objective_function();
  EquationParser&             equation_parser();

protected:
  // Methods
  void                        Validate();
  void                        Build();
  void                        Verify();
  void                        Iterate();
  void                        Reset();
  void                        RunBasic();
  void                        RunEstimation();
  bool                        RunMCMC();
  void                        RunProfiling();
  void                        RunSimulation();
  void                        RunProjection();

  // Members
  RunMode::Type               run_mode_ = RunMode::kInvalid;
  State::Type                 state_    = State::kStartUp;
  unsigned                    start_year_ = 0;
  unsigned                    final_year_ = 0;
  unsigned                    projection_final_year_ = 0;
  unsigned                    current_year_ = 0;
  unsigned                    min_age_ = 0;
  unsigned                    max_age_ = 0;
  string                      base_weight_units_ = "";
  map<string, Double>         b0_;
  map<string, Double>         binitial_;
  map<string, bool>           b0_initialised_;
  bool                        age_plus_ = true;
  vector<string>              initialisation_phases_;
  vector<string>              time_steps_;
  vector<unsigned>            length_bins_;
  bool                        length_plus_ = true;
  unsigned                    length_plus_group_ = 0;
  bool                        addressable_values_file_ = false;
  unsigned                    adressable_values_count_ = 1;
  PartitionType               partition_type_ = PartitionType::kInvalid;
  Managers*                   managers_ = nullptr;
  Objects*                    objects_ = nullptr;
  GlobalConfiguration*        global_configuration_ = nullptr;
  Categories*                 categories_ = nullptr;
  Factory*                    factory_ = nullptr;
  Partition*                  partition_ = nullptr;
  ObjectiveFunction*          objective_function_ = nullptr;
  EquationParser*             equation_parser_ = nullptr;
  bool                        projection_final_phase_ = false; // this parameter is for the projection classes. most of the methods are in the reset but they don't need to be applied
  // if the model is in the first iteration and storeing values.
  map<State::Type, vector<Executor*>> executors_;
};

} /* namespace niwa */
#endif /* MODEL_H_ */
