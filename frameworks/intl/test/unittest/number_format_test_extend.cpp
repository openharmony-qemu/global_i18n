/*
 * Copyright (c) 2024 Huawei Device Co., Ltd.
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include "number_format_test_extend.h"
#include <gtest/gtest.h>
#include "locale_config.h"
#include "number_format.h"
#include "simple_number_format.h"
#include "unicode/locid.h"
#include "parameter.h"

using namespace OHOS::Global::I18n;
using testing::ext::TestSize;
using namespace std;

namespace OHOS {
namespace Global {
namespace I18n {
/**
 * @tc.name: NumberFormatFuncTest0050
 * @tc.desc: Test Intl NumberFormat.format
 * @tc.type: FUNC
 */
HWTEST_F(NumberFormatTest, NumberFormatFuncTest0050, TestSize.Level1)
{
    string localeAR = "ar";
    vector<string> localesAR{localeAR};
    string localeHE = "HE";
    vector<string> localesHE{localeHE};
    string localeMY = "my";
    vector<string> localesMY{localeMY};
    string style = "unit";
    string unit = "beat-per-minute";
    map<string, string> options = { { "style", style},
                                    { "unit", unit },
                                    { "numberingSystem", "latn" } };
    std::unique_ptr<NumberFormat> numFmtAR = std::make_unique<NumberFormat>(localesAR, options);
    ASSERT_TRUE(numFmtAR != nullptr);
    EXPECT_EQ(numFmtAR->Format(0), "0 نبضات/د");
    EXPECT_EQ(numFmtAR->Format(1), "نبضة واحدة/د");
    EXPECT_EQ(numFmtAR->Format(2), "نبضتان/د");
    EXPECT_EQ(numFmtAR->Format(3), "3 نبضات/د");
    EXPECT_EQ(numFmtAR->Format(11), "11 نبضة/د");
    EXPECT_EQ(numFmtAR->Format(100), "100 نبضة/د");
    std::unique_ptr<NumberFormat> numFmtHE = std::make_unique<NumberFormat>(localesHE, options);
    ASSERT_TRUE(numFmtHE != nullptr);
    EXPECT_EQ(numFmtHE->Format(0), "0 פעימות לדקה");
    EXPECT_EQ(numFmtHE->Format(1), "פעימה 1 לדקה");
    EXPECT_EQ(numFmtHE->Format(2), "2 פעימות לדקה");
    EXPECT_EQ(numFmtHE->Format(3), "3 פעימות לדקה");
    std::unique_ptr<NumberFormat> numFmtMY = std::make_unique<NumberFormat>(localesMY, options);
    ASSERT_TRUE(numFmtMY != nullptr);
    EXPECT_EQ(numFmtMY->Format(123), "123 bpm");
}

/**
 * @tc.name: NumberFormatFuncTest0051
 * @tc.desc: Test Intl NumberFormat.format
 * @tc.type: FUNC
 */
HWTEST_F(NumberFormatTest, NumberFormatFuncTest0051, TestSize.Level1)
{
    string localeAR = "ar";
    vector<string> localesAR{localeAR};
    string localeHE = "HE";
    vector<string> localesHE{localeHE};
    string localeMY = "my";
    vector<string> localesMY{localeMY};
    string style = "unit";
    string unit = "body-weight-per-second";
    map<string, string> options = { { "style", style},
                                    { "unit", unit },
                                    { "numberingSystem", "latn" } };
    std::unique_ptr<NumberFormat> numFmtAR = std::make_unique<NumberFormat>(localesAR, options);
    ASSERT_TRUE(numFmtAR != nullptr);
    EXPECT_EQ(numFmtAR->Format(0), "وزن 0 أجسام/ث");
    EXPECT_EQ(numFmtAR->Format(1), "وزن جسم واحد/ث");
    EXPECT_EQ(numFmtAR->Format(2), "وزن جسمين/ث");
    EXPECT_EQ(numFmtAR->Format(3), "وزن 3 أجسام/ث");
    EXPECT_EQ(numFmtAR->Format(11), "وزن 11 جسمًا/ث");
    EXPECT_EQ(numFmtAR->Format(100), "وزن 100 جسم/ث");
    std::unique_ptr<NumberFormat> numFmtHE = std::make_unique<NumberFormat>(localesHE, options);
    ASSERT_TRUE(numFmtHE != nullptr);
    EXPECT_EQ(numFmtHE->Format(0), "0 BW/s");
    EXPECT_EQ(numFmtHE->Format(1), "1 BW/s");
    EXPECT_EQ(numFmtHE->Format(2), "2 BW/s");
    EXPECT_EQ(numFmtHE->Format(3), "3 BW/s");
    std::unique_ptr<NumberFormat> numFmtMY = std::make_unique<NumberFormat>(localesMY, options);
    ASSERT_TRUE(numFmtMY != nullptr);
    EXPECT_EQ(numFmtMY->Format(123), "123 BW/s");
}

/**
 * @tc.name: NumberFormatFuncTest0052
 * @tc.desc: Test Intl NumberFormat.format
 * @tc.type: FUNC
 */
HWTEST_F(NumberFormatTest, NumberFormatFuncTest0052, TestSize.Level1)
{
    string localeAR = "ar";
    vector<string> localesAR{localeAR};
    string localeHE = "HE";
    vector<string> localesHE{localeHE};
    string localeMY = "my";
    vector<string> localesMY{localeMY};
    string style = "unit";
    string unit = "breath-per-minute";
    map<string, string> options = { { "style", style},
                                    { "unit", unit },
                                    { "numberingSystem", "latn" } };
    std::unique_ptr<NumberFormat> numFmtAR = std::make_unique<NumberFormat>(localesAR, options);
    ASSERT_TRUE(numFmtAR != nullptr);
    EXPECT_EQ(numFmtAR->Format(0), "0 أنفاس/د");
    EXPECT_EQ(numFmtAR->Format(1), "نفس واحد/د");
    EXPECT_EQ(numFmtAR->Format(2), "نفسان/د");
    EXPECT_EQ(numFmtAR->Format(3), "3 أنفاس/د");
    EXPECT_EQ(numFmtAR->Format(11), "11 نفسًا/د");
    EXPECT_EQ(numFmtAR->Format(100), "100 نفس/د");
    std::unique_ptr<NumberFormat> numFmtHE = std::make_unique<NumberFormat>(localesHE, options);
    ASSERT_TRUE(numFmtHE != nullptr);
    EXPECT_EQ(numFmtHE->Format(0), "0 נשימות לדקה");
    EXPECT_EQ(numFmtHE->Format(1), "נשימה 1 לדקה");
    EXPECT_EQ(numFmtHE->Format(2), "2 נשימות לדקה");
    EXPECT_EQ(numFmtHE->Format(3), "3 נשימות לדקה");
    std::unique_ptr<NumberFormat> numFmtMY = std::make_unique<NumberFormat>(localesMY, options);
    ASSERT_TRUE(numFmtMY != nullptr);
    EXPECT_EQ(numFmtMY->Format(123), "123 brpm");
}

/**
 * @tc.name: NumberFormatFuncTest0053
 * @tc.desc: Test Intl NumberFormat.format
 * @tc.type: FUNC
 */
HWTEST_F(NumberFormatTest, NumberFormatFuncTest0053, TestSize.Level1)
{
    string localeAR = "ar";
    vector<string> localesAR{localeAR};
    string localeHE = "HE";
    vector<string> localesHE{localeHE};
    string localeMY = "my";
    vector<string> localesMY{localeMY};
    string style = "unit";
    string unit = "foot-per-hour";
    map<string, string> options = { { "style", style},
                                    { "unit", unit },
                                    { "numberingSystem", "latn" } };
    std::unique_ptr<NumberFormat> numFmtAR = std::make_unique<NumberFormat>(localesAR, options);
    ASSERT_TRUE(numFmtAR != nullptr);
    EXPECT_EQ(numFmtAR->Format(0), "0 أقدام/س");
    EXPECT_EQ(numFmtAR->Format(1), "قدم واحدة/س");
    EXPECT_EQ(numFmtAR->Format(2), "قدمان/س");
    EXPECT_EQ(numFmtAR->Format(3), "3 أقدام/س");
    EXPECT_EQ(numFmtAR->Format(11), "11 قدمًا/س");
    EXPECT_EQ(numFmtAR->Format(100), "100 قدم/س");
    std::unique_ptr<NumberFormat> numFmtHE = std::make_unique<NumberFormat>(localesHE, options);
    ASSERT_TRUE(numFmtHE != nullptr);
    EXPECT_EQ(numFmtHE->Format(0), "0 רגל לשעה");
    EXPECT_EQ(numFmtHE->Format(1), "1 רגל לשעה");
    EXPECT_EQ(numFmtHE->Format(2), "2 רגל לשעה");
    EXPECT_EQ(numFmtHE->Format(3), "3 רגל לשעה");
    std::unique_ptr<NumberFormat> numFmtMY = std::make_unique<NumberFormat>(localesMY, options);
    ASSERT_TRUE(numFmtMY != nullptr);
    EXPECT_EQ(numFmtMY->Format(123), "123 ft/h");
}

/**
 * @tc.name: NumberFormatFuncTest0054
 * @tc.desc: Test Intl NumberFormat.format
 * @tc.type: FUNC
 */
HWTEST_F(NumberFormatTest, NumberFormatFuncTest0054, TestSize.Level1)
{
    string localeAR = "ar";
    vector<string> localesAR{localeAR};
    string localeHE = "HE";
    vector<string> localesHE{localeHE};
    string localeMY = "my";
    vector<string> localesMY{localeMY};
    string style = "unit";
    string unit = "jump-rope-per-minute";
    map<string, string> options = { { "style", style},
                                    { "unit", unit },
                                    { "numberingSystem", "latn" } };
    std::unique_ptr<NumberFormat> numFmtAR = std::make_unique<NumberFormat>(localesAR, options);
    ASSERT_TRUE(numFmtAR != nullptr);
    EXPECT_EQ(numFmtAR->Format(0), "0 قفزات/د");
    EXPECT_EQ(numFmtAR->Format(1), "قفزة/د");
    EXPECT_EQ(numFmtAR->Format(2), "قفزتان/د");
    EXPECT_EQ(numFmtAR->Format(3), "3 قفزات/د");
    EXPECT_EQ(numFmtAR->Format(11), "11 قفزة/د");
    EXPECT_EQ(numFmtAR->Format(100), "100 قفزة/د");
    std::unique_ptr<NumberFormat> numFmtHE = std::make_unique<NumberFormat>(localesHE, options);
    ASSERT_TRUE(numFmtHE != nullptr);
    EXPECT_EQ(numFmtHE->Format(0), "0 קפיצות לדקה");
    EXPECT_EQ(numFmtHE->Format(1), "קפיצה 1 לדקה");
    EXPECT_EQ(numFmtHE->Format(2), "2 קפיצות לדקה");
    EXPECT_EQ(numFmtHE->Format(3), "3 קפיצות לדקה");
    std::unique_ptr<NumberFormat> numFmtMY = std::make_unique<NumberFormat>(localesMY, options);
    ASSERT_TRUE(numFmtMY != nullptr);
    EXPECT_EQ(numFmtMY->Format(123), "တစ်မိနစ်လျှင် 123 ကြိမ်");
}

/**
 * @tc.name: NumberFormatFuncTest0055
 * @tc.desc: Test Intl NumberFormat.format
 * @tc.type: FUNC
 */
HWTEST_F(NumberFormatTest, NumberFormatFuncTest0055, TestSize.Level1)
{
    string localeAR = "ar";
    vector<string> localesAR{localeAR};
    string localeHE = "HE";
    vector<string> localesHE{localeHE};
    string localeMY = "my";
    vector<string> localesMY{localeMY};
    string style = "unit";
    string unit = "meter-per-hour";
    map<string, string> options = { { "style", style},
                                    { "unit", unit },
                                    { "numberingSystem", "latn" } };
    std::unique_ptr<NumberFormat> numFmtAR = std::make_unique<NumberFormat>(localesAR, options);
    ASSERT_TRUE(numFmtAR != nullptr);
    EXPECT_EQ(numFmtAR->Format(0), "0 م/س");
    EXPECT_EQ(numFmtAR->Format(1), "1 م/س");
    EXPECT_EQ(numFmtAR->Format(2), "2 م/س");
    EXPECT_EQ(numFmtAR->Format(3), "3 م/س");
    EXPECT_EQ(numFmtAR->Format(11), "11 م/س");
    EXPECT_EQ(numFmtAR->Format(100), "100 م/س");
    std::unique_ptr<NumberFormat> numFmtHE = std::make_unique<NumberFormat>(localesHE, options);
    ASSERT_TRUE(numFmtHE != nullptr);
    EXPECT_EQ(numFmtHE->Format(0), "0 מטר לשעה");
    EXPECT_EQ(numFmtHE->Format(1), "1 מטר לשעה");
    EXPECT_EQ(numFmtHE->Format(2), "2 מטר לשעה");
    EXPECT_EQ(numFmtHE->Format(3), "3 מטר לשעה");
    std::unique_ptr<NumberFormat> numFmtMY = std::make_unique<NumberFormat>(localesMY, options);
    ASSERT_TRUE(numFmtMY != nullptr);
    EXPECT_EQ(numFmtMY->Format(123), "123 m/h");
}

/**
 * @tc.name: NumberFormatFuncTest0056
 * @tc.desc: Test Intl NumberFormat.format
 * @tc.type: FUNC
 */
HWTEST_F(NumberFormatTest, NumberFormatFuncTest0056, TestSize.Level1)
{
    string localeAR = "ar";
    vector<string> localesAR{localeAR};
    string localeHE = "HE";
    vector<string> localesHE{localeHE};
    string localeMY = "my";
    vector<string> localesMY{localeMY};
    string style = "unit";
    string unit = "milliliter-per-minute-per-kilogram";
    map<string, string> options = { { "style", style},
                                    { "unit", unit },
                                    { "numberingSystem", "latn" } };
    std::unique_ptr<NumberFormat> numFmtAR = std::make_unique<NumberFormat>(localesAR, options);
    ASSERT_TRUE(numFmtAR != nullptr);
    EXPECT_EQ(numFmtAR->Format(0), "0 ملم/كغم/د");
    EXPECT_EQ(numFmtAR->Format(1), "1 ملم/كغم/د");
    EXPECT_EQ(numFmtAR->Format(2), "2 ملم/كغم/د");
    EXPECT_EQ(numFmtAR->Format(3), "3 ملم/كغم/د");
    EXPECT_EQ(numFmtAR->Format(11), "11 ملم/كغم/د");
    EXPECT_EQ(numFmtAR->Format(100), "100 ملم/كغم/د");
    std::unique_ptr<NumberFormat> numFmtHE = std::make_unique<NumberFormat>(localesHE, options);
    ASSERT_TRUE(numFmtHE != nullptr);
    EXPECT_EQ(numFmtHE->Format(0), "0 ml/kg/min");
    EXPECT_EQ(numFmtHE->Format(1), "1 ml/kg/min");
    EXPECT_EQ(numFmtHE->Format(2), "2 ml/kg/min");
    EXPECT_EQ(numFmtHE->Format(3), "3 ml/kg/min");
    std::unique_ptr<NumberFormat> numFmtMY = std::make_unique<NumberFormat>(localesMY, options);
    ASSERT_TRUE(numFmtMY != nullptr);
    EXPECT_EQ(numFmtMY->Format(123), "123 ml/kg/min");
}

/**
 * @tc.name: NumberFormatFuncTest0057
 * @tc.desc: Test Intl NumberFormat.format
 * @tc.type: FUNC
 */
HWTEST_F(NumberFormatTest, NumberFormatFuncTest0057, TestSize.Level1)
{
    string localeAR = "ar";
    vector<string> localesAR{localeAR};
    string localeHE = "HE";
    vector<string> localesHE{localeHE};
    string localeMY = "my";
    vector<string> localesMY{localeMY};
    string style = "unit";
    string unit = "rotation-per-minute";
    map<string, string> options = { { "style", style},
                                    { "unit", unit },
                                    { "numberingSystem", "latn" } };
    std::unique_ptr<NumberFormat> numFmtAR = std::make_unique<NumberFormat>(localesAR, options);
    ASSERT_TRUE(numFmtAR != nullptr);
    EXPECT_EQ(numFmtAR->Format(0), "0 دورات/د");
    EXPECT_EQ(numFmtAR->Format(1), "دورة واحدة/د");
    EXPECT_EQ(numFmtAR->Format(2), "دورتان/د");
    EXPECT_EQ(numFmtAR->Format(3), "3 دورات/د");
    EXPECT_EQ(numFmtAR->Format(11), "11 دورة/د");
    EXPECT_EQ(numFmtAR->Format(100), "100 دورة/د");
    std::unique_ptr<NumberFormat> numFmtHE = std::make_unique<NumberFormat>(localesHE, options);
    ASSERT_TRUE(numFmtHE != nullptr);
    EXPECT_EQ(numFmtHE->Format(0), "0 סל\u0022ד");
    EXPECT_EQ(numFmtHE->Format(1), "1 סל\u0022ד");
    EXPECT_EQ(numFmtHE->Format(2), "2 סל\u0022ד");
    EXPECT_EQ(numFmtHE->Format(3), "3 סל\u0022ד");
    std::unique_ptr<NumberFormat> numFmtMY = std::make_unique<NumberFormat>(localesMY, options);
    ASSERT_TRUE(numFmtMY != nullptr);
    EXPECT_EQ(numFmtMY->Format(123), "123 rpm");
}

/**
 * @tc.name: NumberFormatFuncTest0058
 * @tc.desc: Test Intl NumberFormat.format
 * @tc.type: FUNC
 */
HWTEST_F(NumberFormatTest, NumberFormatFuncTest0058, TestSize.Level1)
{
    string localeAR = "ar";
    vector<string> localesAR{localeAR};
    string localeHE = "HE";
    vector<string> localesHE{localeHE};
    string localeMY = "my";
    vector<string> localesMY{localeMY};
    string style = "unit";
    string unit = "step-per-minute";
    map<string, string> options = { { "style", style},
                                    { "unit", unit },
                                    { "numberingSystem", "latn" } };
    std::unique_ptr<NumberFormat> numFmtAR = std::make_unique<NumberFormat>(localesAR, options);
    ASSERT_TRUE(numFmtAR != nullptr);
    EXPECT_EQ(numFmtAR->Format(0), "0 خطوات/د");
    EXPECT_EQ(numFmtAR->Format(1), "خطوة واحدة/د");
    EXPECT_EQ(numFmtAR->Format(2), "خطوتان/د");
    EXPECT_EQ(numFmtAR->Format(3), "3 خطوات/د");
    EXPECT_EQ(numFmtAR->Format(11), "11 خطوة/د");
    EXPECT_EQ(numFmtAR->Format(100), "100 خطوة/د");
    std::unique_ptr<NumberFormat> numFmtHE = std::make_unique<NumberFormat>(localesHE, options);
    ASSERT_TRUE(numFmtHE != nullptr);
    EXPECT_EQ(numFmtHE->Format(0), "0 צעדים לדקה");
    EXPECT_EQ(numFmtHE->Format(1), "צעד 1 לדקה");
    EXPECT_EQ(numFmtHE->Format(2), "2 צעדים לדקה");
    EXPECT_EQ(numFmtHE->Format(3), "3 צעדים לדקה");
    std::unique_ptr<NumberFormat> numFmtMY = std::make_unique<NumberFormat>(localesMY, options);
    ASSERT_TRUE(numFmtMY != nullptr);
    EXPECT_EQ(numFmtMY->Format(123), "တစ်မိနစ်လျှင် 123 လှမ်း");
}

/**
 * @tc.name: NumberFormatFuncTest0059
 * @tc.desc: Test Intl NumberFormat.format
 * @tc.type: FUNC
 */
HWTEST_F(NumberFormatTest, NumberFormatFuncTest0059, TestSize.Level1)
{
    string localeAR = "ar";
    vector<string> localesAR{localeAR};
    string localeHE = "HE";
    vector<string> localesHE{localeHE};
    string localeMY = "my";
    vector<string> localesMY{localeMY};
    string style = "unit";
    string unit = "stroke-per-minute";
    map<string, string> options = { { "style", style},
                                    { "unit", unit },
                                    { "numberingSystem", "latn" } };
    std::unique_ptr<NumberFormat> numFmtAR = std::make_unique<NumberFormat>(localesAR, options);
    ASSERT_TRUE(numFmtAR != nullptr);
    EXPECT_EQ(numFmtAR->Format(0), "0 ضربات/د");
    EXPECT_EQ(numFmtAR->Format(1), "ضربة واحدة/د");
    EXPECT_EQ(numFmtAR->Format(2), "ضربتان/د");
    EXPECT_EQ(numFmtAR->Format(3), "3 ضربات/د");
    EXPECT_EQ(numFmtAR->Format(11), "11 ضربة/د");
    EXPECT_EQ(numFmtAR->Format(100), "100 ضربة/د");
    std::unique_ptr<NumberFormat> numFmtHE = std::make_unique<NumberFormat>(localesHE, options);
    ASSERT_TRUE(numFmtHE != nullptr);
    EXPECT_EQ(numFmtHE->Format(0), "0 תנועות לדקה");
    EXPECT_EQ(numFmtHE->Format(1), "תנועה 1 לדקה");
    EXPECT_EQ(numFmtHE->Format(2), "2 תנועות לדקה");
    EXPECT_EQ(numFmtHE->Format(3), "3 תנועות לדקה");
    std::unique_ptr<NumberFormat> numFmtMY = std::make_unique<NumberFormat>(localesMY, options);
    ASSERT_TRUE(numFmtMY != nullptr);
    EXPECT_EQ(numFmtMY->Format(123), "တစ်မိနစ်လျှင် 123 ချက်");
}
} // namespace I18n
} // namespace Global
} // namespace OHOS