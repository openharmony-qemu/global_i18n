/*
 * Copyright (c) 2021-2022 Huawei Device Co., Ltd.
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

#include "number_format_test.h"
#include <gtest/gtest.h>
#include "locale_config.h"
#include "number_format.h"
#include "unicode/locid.h"
#include "parameter.h"

using namespace OHOS::Global::I18n;
using testing::ext::TestSize;
using namespace std;

namespace OHOS {
namespace Global {
namespace I18n {
class NumberFormatTest : public testing::Test {
public:
    static string originalLanguage;
    static string originalRegion;
    static string originalLocale;
    static void SetUpTestCase(void);
    static void TearDownTestCase(void);
    void SetUp();
    void TearDown();
};

string NumberFormatTest::originalLanguage;
string NumberFormatTest::originalRegion;
string NumberFormatTest::originalLocale;

void NumberFormatTest::SetUpTestCase(void)
{
    originalLanguage = LocaleConfig::GetSystemLanguage();
    originalRegion = LocaleConfig::GetSystemRegion();
    originalLocale = LocaleConfig::GetSystemLocale();
    LocaleConfig::SetSystemLanguage("zh-Hans");
    LocaleConfig::SetSystemRegion("CN");
    LocaleConfig::SetSystemLocale("zh-Hans-CN");
}

void NumberFormatTest::TearDownTestCase(void)
{
    LocaleConfig::SetSystemLanguage(originalLanguage);
    LocaleConfig::SetSystemRegion(originalRegion);
    LocaleConfig::SetSystemLocale(originalLocale);
}

void NumberFormatTest::SetUp(void)
{}

void NumberFormatTest::TearDown(void)
{}

/**
 * @tc.name: NumberFormatFuncTest001
 * @tc.desc: Test Intl NumberFormat.format
 * @tc.type: FUNC
 */
HWTEST_F(NumberFormatTest, NumberFormatFuncTest001, TestSize.Level1)
{
    string locale = "en-IN";
    string expects = "+1,23,456.79 euros";
    vector<string> locales{locale};
    string useGrouping = "true";
    string minimumIntegerDigits = "7";
    string maximumFractionDigits = "2";
    string style = "currency";
    string currency = "978";
    map<string, string> options = { { "useGrouping", useGrouping },
                                    { "style", style },
                                    { "currency", currency },
                                    { "currencyDisplay", "name" },
                                    { "currencySign", "accounting" },
                                    { "signDisplay", "always" } };
    std::unique_ptr<NumberFormat> numFmt = std::make_unique<NumberFormat>(locales, options);
    ASSERT_TRUE(numFmt != nullptr);
    string out = numFmt->Format(123456.789);
    EXPECT_EQ(out, expects);
    EXPECT_EQ(numFmt->GetUseGrouping(), useGrouping);
    EXPECT_EQ(numFmt->GetStyle(), style);
    EXPECT_EQ(numFmt->GetCurrency(), "EUR");
}

/**
 * @tc.name: NumberFormatFuncTest002
 * @tc.desc: Test Intl NumberFormat.format
 * @tc.type: FUNC
 */
HWTEST_F(NumberFormatTest, NumberFormatFuncTest002, TestSize.Level1)
{
    string locale = "en-IN";
    string expects = "+1,23,456.789";
    vector<string> locales{locale};
    string useGrouping = "true";
    string minimumIntegerDigits = "7";
    string maximumFractionDigits = "2";
    string style = "currency";
    string currency = "111";
    map<string, string> options = { { "useGrouping", useGrouping },
                                    { "style", style },
                                    { "currency", currency },
                                    { "currencyDisplay", "name" },
                                    { "currencySign", "accounting" },
                                    { "signDisplay", "always" } };
    std::unique_ptr<NumberFormat> numFmt = std::make_unique<NumberFormat>(locales, options);
    ASSERT_TRUE(numFmt != nullptr);
    string out = numFmt->Format(123456.789);
    EXPECT_EQ(out, expects);
    EXPECT_EQ(numFmt->GetUseGrouping(), useGrouping);
    EXPECT_EQ(numFmt->GetStyle(), style);
    EXPECT_EQ(numFmt->GetCurrency(), "");
}

/**
 * @tc.name: NumberFormatFuncTest003
 * @tc.desc: Test Intl NumberFormat.format
 * @tc.type: FUNC
 */
HWTEST_F(NumberFormatTest, NumberFormatFuncTest003, TestSize.Level1)
{
    string locale = "en-IN";
    string expects = "+1,23,456.789";
    vector<string> locales{locale};
    string useGrouping = "true";
    string minimumIntegerDigits = "7";
    string maximumFractionDigits = "2";
    string style = "currency";
    string currency = "a1b";
    map<string, string> options = { { "useGrouping", useGrouping },
                                    { "style", style },
                                    { "currency", currency },
                                    { "currencyDisplay", "name" },
                                    { "currencySign", "accounting" },
                                    { "signDisplay", "always" } };
    std::unique_ptr<NumberFormat> numFmt = std::make_unique<NumberFormat>(locales, options);
    ASSERT_TRUE(numFmt != nullptr);
    string out = numFmt->Format(123456.789);
    EXPECT_EQ(out, expects);
    EXPECT_EQ(numFmt->GetUseGrouping(), useGrouping);
    EXPECT_EQ(numFmt->GetStyle(), style);
    EXPECT_EQ(numFmt->GetCurrency(), "");
}

/**
 * @tc.name: NumberFormatFuncTest004
 * @tc.desc: Test Intl NumberFormat.format
 * @tc.type: FUNC
 */
HWTEST_F(NumberFormatTest, NumberFormatFuncTest004, TestSize.Level1)
{
    string locale = "en-IN";
    string expects = "+1,23,456.789";
    vector<string> locales{locale};
    string useGrouping = "true";
    string minimumIntegerDigits = "7";
    string maximumFractionDigits = "2";
    string style = "currency";
    string currency = "a#b";
    map<string, string> options = { { "useGrouping", useGrouping },
                                    { "style", style },
                                    { "currency", currency },
                                    { "currencyDisplay", "name" },
                                    { "currencySign", "accounting" },
                                    { "signDisplay", "always" } };
    std::unique_ptr<NumberFormat> numFmt = std::make_unique<NumberFormat>(locales, options);
    ASSERT_TRUE(numFmt != nullptr);
    string out = numFmt->Format(123456.789);
    EXPECT_EQ(out, expects);
    EXPECT_EQ(numFmt->GetUseGrouping(), useGrouping);
    EXPECT_EQ(numFmt->GetStyle(), style);
    EXPECT_EQ(numFmt->GetCurrency(), "");
}

/**
 * @tc.name: NumberFormatFuncTest005
 * @tc.desc: Test Intl NumberFormat.format
 * @tc.type: FUNC
 */
HWTEST_F(NumberFormatTest, NumberFormatFuncTest005, TestSize.Level1)
{
    string locale = "en-IN";
    string expects = "+1,23,456.789";
    vector<string> locales{locale};
    string useGrouping = "true";
    string minimumIntegerDigits = "7";
    string maximumFractionDigits = "2";
    string style = "currency";
    string currency = "ab";
    map<string, string> options = { { "useGrouping", useGrouping },
                                    { "style", style },
                                    { "currency", currency },
                                    { "currencyDisplay", "name" },
                                    { "currencySign", "accounting" },
                                    { "signDisplay", "always" } };
    std::unique_ptr<NumberFormat> numFmt = std::make_unique<NumberFormat>(locales, options);
    ASSERT_TRUE(numFmt != nullptr);
    string out = numFmt->Format(123456.789);
    EXPECT_EQ(out, expects);
    EXPECT_EQ(numFmt->GetUseGrouping(), useGrouping);
    EXPECT_EQ(numFmt->GetStyle(), style);
    EXPECT_EQ(numFmt->GetCurrency(), "");
}

/**
 * @tc.name: NumberFormatFuncTest006
 * @tc.desc: Test Intl NumberFormat.format
 * @tc.type: FUNC
 */
HWTEST_F(NumberFormatTest, NumberFormatFuncTest006, TestSize.Level1)
{
    string locale = "en-IN";
    string expects = "+1,23,456.789";
    vector<string> locales{locale};
    string useGrouping = "true";
    string minimumIntegerDigits = "7";
    string maximumFractionDigits = "2";
    string style = "currency";
    string currency = "abcd";
    map<string, string> options = { { "useGrouping", useGrouping },
                                    { "style", style },
                                    { "currency", currency },
                                    { "currencyDisplay", "name" },
                                    { "currencySign", "accounting" },
                                    { "signDisplay", "always" } };
    std::unique_ptr<NumberFormat> numFmt = std::make_unique<NumberFormat>(locales, options);
    ASSERT_TRUE(numFmt != nullptr);
    string out = numFmt->Format(123456.789);
    EXPECT_EQ(out, expects);
    EXPECT_EQ(numFmt->GetUseGrouping(), useGrouping);
    EXPECT_EQ(numFmt->GetStyle(), style);
    EXPECT_EQ(numFmt->GetCurrency(), "");
}

/**
 * @tc.name: NumberFormatFuncTest007
 * @tc.desc: Test Intl NumberFormat.format
 * @tc.type: FUNC
 */
HWTEST_F(NumberFormatTest, NumberFormatFuncTest007, TestSize.Level1)
{
    string locale_cn = "zh-CN";
    vector<string> locales_cn{locale_cn};
    string locale_us = "en-US";
    vector<string> locales_us{locale_us};
    string style = "unit";
    string unit = "beat-per-minute";
    string unitStyle = "long";
    map<string, string> options = { { "style", style},
                                    { "unit", unit },
                                    { "unitStyle", unitStyle } };
    std::unique_ptr<NumberFormat> numFmt_cn = std::make_unique<NumberFormat>(locales_cn, options);
    ASSERT_TRUE(numFmt_cn != nullptr);
    string out = numFmt_cn->Format(1234567.89);
    EXPECT_EQ(out, "1,234,567.89 次/分钟");
    std::unique_ptr<NumberFormat> numFmt_us = std::make_unique<NumberFormat>(locales_us, options);
    ASSERT_TRUE(numFmt_us != nullptr);
    out = numFmt_us->Format(1234567.89);
    EXPECT_EQ(out, "1,234,567.89 bpm");
}

/**
 * @tc.name: NumberFormatFuncTest008
 * @tc.desc: Test Intl NumberFormat.format
 * @tc.type: FUNC
 */
HWTEST_F(NumberFormatTest, NumberFormatFuncTest008, TestSize.Level1)
{
    string locale_cn = "zh-CN";
    vector<string> locales_cn{locale_cn};
    string locale_us = "en-US";
    vector<string> locales_us{locale_us};
    string style = "unit";
    string unit = "body-weight-per-second";
    string unitStyle = "long";
    map<string, string> options = { { "style", style},
                                    { "unit", unit },
                                    { "unitStyle", unitStyle } };
    std::unique_ptr<NumberFormat> numFmt_cn = std::make_unique<NumberFormat>(locales_cn, options);
    ASSERT_TRUE(numFmt_cn != nullptr);
    string out = numFmt_cn->Format(1234567.89);
    EXPECT_EQ(out, "1,234,567.89 BW/s");
    std::unique_ptr<NumberFormat> numFmt_us = std::make_unique<NumberFormat>(locales_us, options);
    ASSERT_TRUE(numFmt_us != nullptr);
    out = numFmt_us->Format(1234567.89);
    EXPECT_EQ(out, "1,234,567.89 BW/s");
}

/**
 * @tc.name: NumberFormatFuncTest009
 * @tc.desc: Test Intl NumberFormat.format
 * @tc.type: FUNC
 */
HWTEST_F(NumberFormatTest, NumberFormatFuncTest009, TestSize.Level1)
{
    string locale_cn = "zh-CN";
    vector<string> locales_cn{locale_cn};
    string locale_us = "en-US";
    vector<string> locales_us{locale_us};
    string style = "unit";
    string unit = "breath-per-minute";
    string unitStyle = "long";
    map<string, string> options = { { "style", style},
                                    { "unit", unit },
                                    { "unitStyle", unitStyle } };
    std::unique_ptr<NumberFormat> numFmt_cn = std::make_unique<NumberFormat>(locales_cn, options);
    ASSERT_TRUE(numFmt_cn != nullptr);
    string out = numFmt_cn->Format(1234567.89);
    EXPECT_EQ(out, "1,234,567.89 次/分");
    std::unique_ptr<NumberFormat> numFmt_us = std::make_unique<NumberFormat>(locales_us, options);
    ASSERT_TRUE(numFmt_us != nullptr);
    out = numFmt_us->Format(1234567.89);
    EXPECT_EQ(out, "1,234,567.89 brpm");
}

/**
 * @tc.name: NumberFormatFuncTest0010
 * @tc.desc: Test Intl NumberFormat.format
 * @tc.type: FUNC
 */
HWTEST_F(NumberFormatTest, NumberFormatFuncTest0010, TestSize.Level1)
{
    string locale_cn = "zh-CN";
    vector<string> locales_cn{locale_cn};
    string locale_us = "en-US";
    vector<string> locales_us{locale_us};
    string style = "unit";
    string unit = "foot-per-hour";
    string unitStyle = "long";
    map<string, string> options = { { "style", style},
                                    { "unit", unit },
                                    { "unitStyle", unitStyle } };
    std::unique_ptr<NumberFormat> numFmt_cn = std::make_unique<NumberFormat>(locales_cn, options);
    ASSERT_TRUE(numFmt_cn != nullptr);
    string out = numFmt_cn->Format(1234567.89);
    EXPECT_EQ(out, "1,234,567.89 ft/h");
    std::unique_ptr<NumberFormat> numFmt_us = std::make_unique<NumberFormat>(locales_us, options);
    ASSERT_TRUE(numFmt_us != nullptr);
    out = numFmt_us->Format(1234567.89);
    EXPECT_EQ(out, "1,234,567.89 ft/h");
}

/**
 * @tc.name: NumberFormatFuncTest0011
 * @tc.desc: Test Intl NumberFormat.format
 * @tc.type: FUNC
 */
HWTEST_F(NumberFormatTest, NumberFormatFuncTest0011, TestSize.Level1)
{
    string locale_cn = "zh-CN";
    vector<string> locales_cn{locale_cn};
    string locale_us = "en-US";
    vector<string> locales_us{locale_us};
    string locale_gb = "en-GB";
    vector<string> locales_gb{locale_gb};
    string style = "unit";
    string unit = "jump-rope-speed";
    string unitStyle = "long";
    map<string, string> options = { { "style", style},
                                    { "unit", unit },
                                    { "unitStyle", unitStyle } };
    std::unique_ptr<NumberFormat> numFmt_cn = std::make_unique<NumberFormat>(locales_cn, options);
    ASSERT_TRUE(numFmt_cn != nullptr);
    string out = numFmt_cn->Format(1234567.89);
    EXPECT_EQ(out, "1,234,567.89 个/分钟");
    std::unique_ptr<NumberFormat> numFmt_us = std::make_unique<NumberFormat>(locales_us, options);
    ASSERT_TRUE(numFmt_us != nullptr);
    out = numFmt_us->Format(1234567.89);
    EXPECT_EQ(out, "1,234,567.89 jumps/minute");
    out = numFmt_us->Format(1);
    EXPECT_EQ(out, "1 jump/minute");
    std::unique_ptr<NumberFormat> numFmt_gb = std::make_unique<NumberFormat>(locales_gb, options);
    ASSERT_TRUE(numFmt_gb != nullptr);
    out = numFmt_gb->Format(1234567.89);
    EXPECT_EQ(out, "1,234,567.89 skips/minute");
    out = numFmt_gb->Format(1);
    EXPECT_EQ(out, "1 skip/minute");
}

/**
 * @tc.name: NumberFormatFuncTest0012
 * @tc.desc: Test Intl NumberFormat.format
 * @tc.type: FUNC
 */
HWTEST_F(NumberFormatTest, NumberFormatFuncTest0012, TestSize.Level1)
{
    string locale_cn = "zh-CN";
    vector<string> locales_cn{locale_cn};
    string locale_us = "en-US";
    vector<string> locales_us{locale_us};
    string style = "unit";
    string unit = "meter-per-hour";
    string unitStyle = "long";
    map<string, string> options = { { "style", style},
                                    { "unit", unit },
                                    { "unitStyle", unitStyle } };
    std::unique_ptr<NumberFormat> numFmt_cn = std::make_unique<NumberFormat>(locales_cn, options);
    ASSERT_TRUE(numFmt_cn != nullptr);
    string out = numFmt_cn->Format(1234567.89);
    EXPECT_EQ(out, "1,234,567.89 m/h");
    std::unique_ptr<NumberFormat> numFmt_us = std::make_unique<NumberFormat>(locales_us, options);
    ASSERT_TRUE(numFmt_us != nullptr);
    out = numFmt_us->Format(1234567.89);
    EXPECT_EQ(out, "1,234,567.89 m/h");
}

/**
 * @tc.name: NumberFormatFuncTest0013
 * @tc.desc: Test Intl NumberFormat.format
 * @tc.type: FUNC
 */
HWTEST_F(NumberFormatTest, NumberFormatFuncTest0013, TestSize.Level1)
{
    string locale_cn = "zh-CN";
    vector<string> locales_cn{locale_cn};
    string locale_us = "en-US";
    vector<string> locales_us{locale_us};
    string style = "unit";
    string unit = "milliliter-per-minute-per-kilogram";
    string unitStyle = "long";
    map<string, string> options = { { "style", style},
                                    { "unit", unit },
                                    { "unitStyle", unitStyle } };
    std::unique_ptr<NumberFormat> numFmt_cn = std::make_unique<NumberFormat>(locales_cn, options);
    ASSERT_TRUE(numFmt_cn != nullptr);
    string out = numFmt_cn->Format(1234567.89);
    EXPECT_EQ(out, "1,234,567.89 ml/kg/min");
    std::unique_ptr<NumberFormat> numFmt_us = std::make_unique<NumberFormat>(locales_us, options);
    ASSERT_TRUE(numFmt_us != nullptr);
    out = numFmt_us->Format(1234567.89);
    EXPECT_EQ(out, "1,234,567.89 ml/kg/min");
}

/**
 * @tc.name: NumberFormatFuncTest0014
 * @tc.desc: Test Intl NumberFormat.format
 * @tc.type: FUNC
 */
HWTEST_F(NumberFormatTest, NumberFormatFuncTest0014, TestSize.Level1)
{
    string locale_cn = "zh-CN";
    vector<string> locales_cn{locale_cn};
    string locale_us = "en-US";
    vector<string> locales_us{locale_us};
    string style = "unit";
    string unit = "rotation-per-minute";
    string unitStyle = "long";
    map<string, string> options = { { "style", style},
                                    { "unit", unit },
                                    { "unitStyle", unitStyle } };
    std::unique_ptr<NumberFormat> numFmt_cn = std::make_unique<NumberFormat>(locales_cn, options);
    ASSERT_TRUE(numFmt_cn != nullptr);
    string out = numFmt_cn->Format(1234567.89);
    EXPECT_EQ(out, "1,234,567.89 转/分钟");
    std::unique_ptr<NumberFormat> numFmt_us = std::make_unique<NumberFormat>(locales_us, options);
    ASSERT_TRUE(numFmt_us != nullptr);
    out = numFmt_us->Format(1234567.89);
    EXPECT_EQ(out, "1,234,567.89 rpm");
}

/**
 * @tc.name: NumberFormatFuncTest0015
 * @tc.desc: Test Intl NumberFormat.format
 * @tc.type: FUNC
 */
HWTEST_F(NumberFormatTest, NumberFormatFuncTest0015, TestSize.Level1)
{
    string locale_cn = "zh-CN";
    vector<string> locales_cn{locale_cn};
    string locale_us = "en-US";
    vector<string> locales_us{locale_us};
    string style = "unit";
    string unit = "step-per-minute";
    string unitStyle = "long";
    map<string, string> options = { { "style", style},
                                    { "unit", unit },
                                    { "unitStyle", unitStyle } };
    std::unique_ptr<NumberFormat> numFmt_cn = std::make_unique<NumberFormat>(locales_cn, options);
    ASSERT_TRUE(numFmt_cn != nullptr);
    string out = numFmt_cn->Format(1234567.89);
    EXPECT_EQ(out, "1,234,567.89 步/分钟");
    std::unique_ptr<NumberFormat> numFmt_us = std::make_unique<NumberFormat>(locales_us, options);
    ASSERT_TRUE(numFmt_us != nullptr);
    out = numFmt_us->Format(1234567.89);
    EXPECT_EQ(out, "1,234,567.89 steps/min");
    out = numFmt_us->Format(1);
    EXPECT_EQ(out, "1 step/min");
}

/**
 * @tc.name: NumberFormatFuncTest0016
 * @tc.desc: Test Intl NumberFormat.format
 * @tc.type: FUNC
 */
HWTEST_F(NumberFormatTest, NumberFormatFuncTest0016, TestSize.Level1) {
    string locale_cn = "zh-CN";
    vector<string> locales_cn{locale_cn};
    string locale_us = "en-US";
    vector<string> locales_us{locale_us};
    string style = "unit";
    string unit = "stroke-per-minute";
    string unitStyle = "long";
    map<string, string> options = { { "style", style},
                                    { "unit", unit },
                                    { "unitStyle", unitStyle } };
    std::unique_ptr<NumberFormat> numFmt_cn = std::make_unique<NumberFormat>(locales_cn, options);
    ASSERT_TRUE(numFmt_cn != nullptr);
    string out = numFmt_cn->Format(1234567.89);
    EXPECT_EQ(out, "1,234,567.89 次/分钟");
    std::unique_ptr<NumberFormat> numFmt_us = std::make_unique<NumberFormat>(locales_us, options);
    ASSERT_TRUE(numFmt_us != nullptr);
    out = numFmt_us->Format(1234567.89);
    EXPECT_EQ(out, "1,234,567.89 strokes/min");
    out = numFmt_us->Format(1);
    EXPECT_EQ(out, "1 stroke/min");
}
} // namespace I18n
} // namespace Global
} // namespace OHOS