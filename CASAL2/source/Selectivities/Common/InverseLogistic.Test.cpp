/**
 * @file InverseLogistic.Test.cpp
 * @author  Scott Rasmussen (scott.rasmussen@zaita.com)
 * @version 1.0
 * @date 22/01/2013
 * @section LICENSE
 *
 * Copyright NIWA Science �2013 - www.niwa.co.nz
 *
 * $Date: 2008-03-04 16:33:32 +1300 (Tue, 04 Mar 2008) $
 */
#ifdef TESTMODE

// Headers
#include "InverseLogistic.h"

#include <gmock/gmock.h>
#include <gtest/gtest.h>
#include <boost/lexical_cast.hpp>

#include "TestResources/MockClasses/Model.h"

// Namespaces
namespace niwa {

using ::testing::Return;
using ::testing::ReturnRef;

/**
 * Test the results of our selectivity are correct
 */
TEST(Selectivities, InverseLogistic_Age) {
  MockModel model;
  EXPECT_CALL(model, min_age()).WillRepeatedly(Return(10));
  EXPECT_CALL(model, max_age()).WillRepeatedly(Return(20));
  EXPECT_CALL(model, age_spread()).WillRepeatedly(Return(11));
  EXPECT_CALL(model, partition_type()).WillRepeatedly(Return(PartitionType::kAge));

  niwa::selectivities::InverseLogistic inverse_logistic(&model);

  inverse_logistic.parameters().Add(PARAM_LABEL, "unit_test_inverse_logistic", __FILE__, __LINE__);
  inverse_logistic.parameters().Add(PARAM_TYPE, "not needed in test", __FILE__, __LINE__);
  inverse_logistic.parameters().Add(PARAM_A50,   "2",  __FILE__, __LINE__);
  inverse_logistic.parameters().Add(PARAM_ATO95, "7",  __FILE__, __LINE__);
  inverse_logistic.Validate();
  inverse_logistic.Build();

  ASSERT_THROW(inverse_logistic.GetAgeResult(9, nullptr), std::string); // Below model->min_age()
  EXPECT_DOUBLE_EQ(0.033405028356377708,      inverse_logistic.GetAgeResult(10, nullptr)); // At model->min_age()
  EXPECT_DOUBLE_EQ(0.022189270565607933,      inverse_logistic.GetAgeResult(11, nullptr));
  EXPECT_DOUBLE_EQ(0.014682011272418749,      inverse_logistic.GetAgeResult(12, nullptr));
  EXPECT_DOUBLE_EQ(0.0096895015990552391,     inverse_logistic.GetAgeResult(13, nullptr));
  EXPECT_DOUBLE_EQ(0.0063836592207018317,     inverse_logistic.GetAgeResult(14, nullptr));
  EXPECT_DOUBLE_EQ(0.0042009122314798919,     inverse_logistic.GetAgeResult(15, nullptr));
  EXPECT_DOUBLE_EQ(0.0027624309392264568,     inverse_logistic.GetAgeResult(16, nullptr));
  EXPECT_DOUBLE_EQ(0.001815618012518061,      inverse_logistic.GetAgeResult(17, nullptr));
  EXPECT_DOUBLE_EQ(0.0011929334946871872,     inverse_logistic.GetAgeResult(18, nullptr));
  EXPECT_DOUBLE_EQ(0.00078363726063745975,    inverse_logistic.GetAgeResult(19, nullptr));
  EXPECT_DOUBLE_EQ(0.00051469845718343787,    inverse_logistic.GetAgeResult(20, nullptr)); // At model->max_age()
  ASSERT_THROW(inverse_logistic.GetAgeResult(21, nullptr), std::string); // This is above model->max_age()
}

TEST(Selectivities, InverseLogistic_Length) {
  MockModel model;
  EXPECT_CALL(model, min_age()).WillRepeatedly(Return(10));
  EXPECT_CALL(model, max_age()).WillRepeatedly(Return(20));
  EXPECT_CALL(model, age_spread()).WillRepeatedly(Return(11));
  vector<unsigned> lengths = {10, 20, 30, 40, 50, 60, 120};
  EXPECT_CALL(model, length_bins()).WillRepeatedly(ReturnRef(lengths));
  EXPECT_CALL(model, partition_type()).WillRepeatedly(Return(PartitionType::kLength));

  niwa::selectivities::InverseLogistic inverse_logistic(&model);

  vector<double> expected_values = {0.64310991374259796, 0.50000000000000000, 0.35689008625740204, 0.23545238646229960, 0.14595776020489171, 0.08662554508809206, 0.0027624309392264568};

  inverse_logistic.parameters().Add(PARAM_LABEL, "unit_test_inverse_logistic", __FILE__, __LINE__);
  inverse_logistic.parameters().Add(PARAM_TYPE, "not needed in test", __FILE__, __LINE__);
  inverse_logistic.parameters().Add(PARAM_A50,   "20",  __FILE__, __LINE__);
  inverse_logistic.parameters().Add(PARAM_ATO95, "50",  __FILE__, __LINE__);
  inverse_logistic.Validate();
  inverse_logistic.Build();

  for (unsigned i = 0; i < lengths.size(); ++i) {
    EXPECT_DOUBLE_EQ(expected_values[i],  inverse_logistic.GetLengthResult(i));
  }

}

}

#endif /* ifdef TESTMODE */
