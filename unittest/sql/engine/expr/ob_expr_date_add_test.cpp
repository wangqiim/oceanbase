/**
 * Copyright (c) 2021 OceanBase
 * OceanBase CE is licensed under Mulan PubL v2.
 * You can use this software according to the terms and conditions of the Mulan PubL v2.
 * You may obtain a copy of Mulan PubL v2 at:
 *          http://license.coscl.org.cn/MulanPubL-2.0
 * THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND,
 * EITHER EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT,
 * MERCHANTABILITY OR FIT FOR A PARTICULAR PURPOSE.
 * See the Mulan PubL v2 for more details.
 */

#include <gtest/gtest.h>
#include "lib/ob_date_unit_type.h"
#include "lib/time/ob_time_utility.h"
#include "sql/engine/expr/ob_expr_date_add.h"
#include "ob_expr_test_utils.h"

using namespace oceanbase::common;
using namespace oceanbase::sql;

class ObExprDateaddTest : public ::testing::Test
{
public:
  ObExprDateaddTest();
  virtual ~ObExprDateaddTest();
  virtual void SetUp();
  virtual void TearDown();
private:
  // disallow copy
  ObExprDateaddTest(const ObExprDateaddTest &other);
  ObExprDateaddTest& operator=(const ObExprDateaddTest &other);
private:
  // data members
};

ObExprDateaddTest::ObExprDateaddTest()
{
}

ObExprDateaddTest::~ObExprDateaddTest()
{
}

void ObExprDateaddTest::SetUp()
{
}

void ObExprDateaddTest::TearDown()
{
}

int64_t get_usec(int year, int month, int day, int hour, int minute, int sec, int64_t usec)
{
  int64_t ret = 0;
  struct tm t;
  memset(&t, 0, sizeof(struct tm));
  t.tm_year = year - 1900;
  t.tm_mon = month - 1;
  t.tm_mday = day;
  t.tm_hour = hour;
  t.tm_min = minute;
  t.tm_sec = sec;
//ObTimeUtility::timestamp_to_usec(t, usec, ret);
  return ret;
}

#define T(obj, t1, v1, t2, v2, t3, v3, ref_type, ref_value) EXPECT_RESULT3(obj, &buf, calc_result3, t1, v1, t2, v2, t3, v3, ref_type, ref_value)
#define F(obj, t1, v1, t2, v2, t3, v3) EXPECT_FAIL_RESULT3(obj, &buf, calc_result3, t1, v1, t2, v2, t3, v3, precise_datetime, 0)

TEST_F(ObExprDateaddTest, base_test)
{
  ObExprDateAdd dateadd;
  ObExprStringBuf buf;

  ASSERT_EQ(3, dateadd.get_param_num());

  T(dateadd, precise_datetime, get_usec(2014, 1, 3, 20, 2, 10, 999), int, 2, int, DATE_UNIT_DAY, precise_datetime, get_usec(2014, 1, 5, 20, 2, 10, 999));
  T(dateadd, varchar, "2014-01-03 20:02:10.999", int, 2, int, DATE_UNIT_MICROSECOND, precise_datetime, get_usec(2014, 01, 03, 20, 02, 10, 999002));
  T(dateadd, varchar, "2014-01-03 20:02:10.999", int, 2, int, DATE_UNIT_SECOND, precise_datetime, get_usec(2014, 01, 03, 20, 02, 12, 999000));
  T(dateadd, varchar, "2014-01-03 20:02:10.999", int, 2, int, DATE_UNIT_MINUTE, precise_datetime, get_usec(2014, 01, 03, 20, 04, 10, 999000));
  T(dateadd, varchar, "2014-01-03 20:02:10.999", int, 2, int, DATE_UNIT_HOUR, precise_datetime, get_usec(2014, 01, 03, 22, 02, 10, 999000));
  T(dateadd, varchar, "2014-01-03 20:02:10.999", int, 2, int, DATE_UNIT_DAY, precise_datetime, get_usec(2014, 01, 05, 20, 02, 10, 999000));
  T(dateadd, varchar, "2014-01-03 20:02:10.999", int, 2, int, DATE_UNIT_MONTH, precise_datetime, get_usec(2014, 03, 03, 20, 02, 10, 999000));
  T(dateadd, varchar, "2014-01-03 20:02:10.999", int, 2, int, DATE_UNIT_QUARTER, precise_datetime, get_usec(2014, 07, 03, 20, 02, 10, 999000));
  T(dateadd, varchar, "2014-01-03 20:02:10.999", int, 2, int, DATE_UNIT_YEAR, precise_datetime, get_usec(2016, 01, 03, 20, 02, 10, 999000));
  T(dateadd, varchar, "2014-01-03 20:02:10.999", varchar, "02.0002", int, DATE_UNIT_SECOND_MICROSECOND, precise_datetime, get_usec(2014, 01, 03, 20, 02, 12, 999200));
  T(dateadd, varchar, "2014-01-03 20:02:10.999", varchar, "02:02", int, DATE_UNIT_MINUTE_SECOND, precise_datetime, get_usec(2014, 01, 03, 20, 04, 12, 999000));
  T(dateadd, varchar, "2014-01-03 20:02:10.999", varchar, "02:02.0002", int, DATE_UNIT_MINUTE_MICROSECOND, precise_datetime, get_usec(2014, 01, 03, 20, 04, 12, 999200));
  T(dateadd, varchar, "2014-01-03 20:02:10.999", varchar, "02:02", int, DATE_UNIT_HOUR_MINUTE, precise_datetime, get_usec(2014, 01, 03, 22, 04, 10, 999000));
  T(dateadd, varchar, "2014-01-03 20:02:10.999", varchar, "02:02:02", int, DATE_UNIT_HOUR_SECOND, precise_datetime, get_usec(2014, 01, 03, 22, 04, 12, 999000));
  T(dateadd, varchar, "2014-01-03 20:02:10.999", varchar, "02:02:02.0002", int, DATE_UNIT_HOUR_MICROSECOND, precise_datetime, get_usec(2014, 01, 03, 22, 04, 12, 999200));
  T(dateadd, varchar, "2014-01-03 20:02:10.999", varchar, "02 02", int, DATE_UNIT_DAY_HOUR, precise_datetime, get_usec(2014, 01, 05, 22, 02, 10, 999000));
  T(dateadd, varchar, "2014-01-03 20:02:10.999", varchar, "02 02:02", int, DATE_UNIT_DAY_MINUTE, precise_datetime, get_usec(2014, 01, 05, 22, 04, 10, 999000));
  T(dateadd, varchar, "2014-01-03 20:02:10.999", varchar, "02 02:02:02", int, DATE_UNIT_DAY_SECOND, precise_datetime, get_usec(2014, 01, 05, 22, 04, 12, 999000));
  T(dateadd, varchar, "2014-01-03 20:02:10.999", varchar, "02 02:02:02.0002", int, DATE_UNIT_DAY_MICROSECOND, precise_datetime, get_usec(2014, 01, 05, 22, 04, 12, 999200));
}

TEST_F(ObExprDateaddTest, fail_test)
{
  ObExprDateAdd dateadd;
  ObExprStringBuf buf;

  ASSERT_EQ(3, dateadd.get_param_num());

  F(dateadd, int, 10, int, 2, int, DATE_UNIT_MICROSECOND);
  F(dateadd, varchar, "2014-01", int, 2, int, DATE_UNIT_DAY);
  F(dateadd, double, 1.2, int, 2, int, DATE_UNIT_DAY);
  F(dateadd, varchar, "2014-01-03", varchar, "2.2", int, DATE_UNIT_DAY);
  F(dateadd, varchar, "2014-01-03", double, 2.5, int, DATE_UNIT_DAY);
}

int main(int argc, char **argv)
{
  oceanbase::common::ObLogger::get_logger().set_log_level("DEBUG");
  ::testing::InitGoogleTest(&argc,argv);
  return RUN_ALL_TESTS();
}
