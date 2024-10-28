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
    string localeCN = "zh-CN";
    vector<string> localesCN{localeCN};
    string localeUS = "en-US";
    vector<string> localesUS{localeUS};
    string localeGB = "en-GB";
    vector<string> localesGB{localeGB};
    string localeBO = "bo";
    vector<string> localesBO{localeBO};
    string localeUG = "ug";
    vector<string> localesUG{localeUG};
    string localeHK = "zh-HK";
    vector<string> localesHK{localeHK};
    string localeTW = "zh-TW";
    vector<string> localesTW{localeTW};
    string style = "unit";
    string unit = "beat-per-minute";
    string unitStyle = "long";
    map<string, string> options = { { "style", style},
                                    { "unit", unit },
                                    { "unitStyle", unitStyle } };
    std::unique_ptr<NumberFormat> numFmtCN = std::make_unique<NumberFormat>(localesCN, options);
    ASSERT_TRUE(numFmtCN != nullptr);
    EXPECT_EQ(numFmtCN->Format(1234567.89), "1,234,567.89 次/分钟");
    std::unique_ptr<NumberFormat> numFmtUS = std::make_unique<NumberFormat>(localesUS, options);
    ASSERT_TRUE(numFmtUS != nullptr);
    EXPECT_EQ(numFmtUS->Format(1), "1 bpm");
    EXPECT_EQ(numFmtUS->Format(1234567.89), "1,234,567.89 bpm");
    std::unique_ptr<NumberFormat> numFmtGB = std::make_unique<NumberFormat>(localesGB, options);
    ASSERT_TRUE(numFmtGB != nullptr);
    EXPECT_EQ(numFmtGB->Format(1), "1 bpm");
    EXPECT_EQ(numFmtGB->Format(1234567.89), "1,234,567.89 bpm");
    std::unique_ptr<NumberFormat> numFmtBO = std::make_unique<NumberFormat>(localesBO, options);
    ASSERT_TRUE(numFmtBO != nullptr);
    EXPECT_EQ(numFmtBO->Format(1234567.89), "སྐར་མ་རེར་ཐེངས་ 1,234,567.89");
    std::unique_ptr<NumberFormat> numFmtUG = std::make_unique<NumberFormat>(localesUG, options);
    ASSERT_TRUE(numFmtUG != nullptr);
    EXPECT_EQ(numFmtUG->Format(1), "ق/م 1");
    EXPECT_EQ(numFmtUG->Format(1234567.89), "ق/م 1,234,567.89");
    std::unique_ptr<NumberFormat> numFmtHK = std::make_unique<NumberFormat>(localesHK, options);
    ASSERT_TRUE(numFmtHK != nullptr);
    EXPECT_EQ(numFmtHK->Format(1234567.89), "1,234,567.89 下/分鐘");
    std::unique_ptr<NumberFormat> numFmtTW = std::make_unique<NumberFormat>(localesTW, options);
    ASSERT_TRUE(numFmtTW != nullptr);
    EXPECT_EQ(numFmtTW->Format(1234567.89), "1,234,567.89 次/分鐘");
}

/**
 * @tc.name: NumberFormatFuncTest008
 * @tc.desc: Test Intl NumberFormat.format
 * @tc.type: FUNC
 */
HWTEST_F(NumberFormatTest, NumberFormatFuncTest008, TestSize.Level1)
{
    string localeCN = "zh-CN";
    vector<string> localesCN{localeCN};
    string localeUS = "en-US";
    vector<string> localesUS{localeUS};
    string localeGB = "en-GB";
    vector<string> localesGB{localeGB};
    string localeBO = "bo";
    vector<string> localesBO{localeBO};
    string localeUG = "ug";
    vector<string> localesUG{localeUG};
    string localeHK = "zh-HK";
    vector<string> localesHK{localeHK};
    string localeTW = "zh-TW";
    vector<string> localesTW{localeTW};
    string style = "unit";
    string unit = "beat-per-minute";
    string unitStyle = "short";
    map<string, string> options = { { "style", style},
                                    { "unit", unit },
                                    { "unitStyle", unitStyle } };
    std::unique_ptr<NumberFormat> numFmtCN = std::make_unique<NumberFormat>(localesCN, options);
    ASSERT_TRUE(numFmtCN != nullptr);
    EXPECT_EQ(numFmtCN->Format(1234567.89), "1,234,567.89 次/分钟");
    std::unique_ptr<NumberFormat> numFmtUS = std::make_unique<NumberFormat>(localesUS, options);
    ASSERT_TRUE(numFmtUS != nullptr);
    EXPECT_EQ(numFmtUS->Format(1), "1 bpm");
    EXPECT_EQ(numFmtUS->Format(1234567.89), "1,234,567.89 bpm");
    std::unique_ptr<NumberFormat> numFmtGB = std::make_unique<NumberFormat>(localesGB, options);
    ASSERT_TRUE(numFmtGB != nullptr);
    EXPECT_EQ(numFmtGB->Format(1), "1 bpm");
    EXPECT_EQ(numFmtGB->Format(1234567.89), "1,234,567.89 bpm");
    std::unique_ptr<NumberFormat> numFmtBO = std::make_unique<NumberFormat>(localesBO, options);
    ASSERT_TRUE(numFmtBO != nullptr);
    EXPECT_EQ(numFmtBO->Format(1234567.89), "སྐར་མ་རེར་ཐེངས་ 1,234,567.89");
    std::unique_ptr<NumberFormat> numFmtUG = std::make_unique<NumberFormat>(localesUG, options);
    ASSERT_TRUE(numFmtUG != nullptr);
    EXPECT_EQ(numFmtUG->Format(1), "ق/م 1");
    EXPECT_EQ(numFmtUG->Format(1234567.89), "ق/م 1,234,567.89");
    std::unique_ptr<NumberFormat> numFmtHK = std::make_unique<NumberFormat>(localesHK, options);
    ASSERT_TRUE(numFmtHK != nullptr);
    EXPECT_EQ(numFmtHK->Format(1234567.89), "1,234,567.89 下/分鐘");
    std::unique_ptr<NumberFormat> numFmtTW = std::make_unique<NumberFormat>(localesTW, options);
    ASSERT_TRUE(numFmtTW != nullptr);
    EXPECT_EQ(numFmtTW->Format(1234567.89), "1,234,567.89 次/分鐘");
}

/**
 * @tc.name: NumberFormatFuncTest009
 * @tc.desc: Test Intl NumberFormat.format
 * @tc.type: FUNC
 */
HWTEST_F(NumberFormatTest, NumberFormatFuncTest009, TestSize.Level1)
{
    string localeCN = "zh-CN";
    vector<string> localesCN{localeCN};
    string localeUS = "en-US";
    vector<string> localesUS{localeUS};
    string localeGB = "en-GB";
    vector<string> localesGB{localeGB};
    string localeBO = "bo";
    vector<string> localesBO{localeBO};
    string localeUG = "ug";
    vector<string> localesUG{localeUG};
    string localeHK = "zh-HK";
    vector<string> localesHK{localeHK};
    string localeTW = "zh-TW";
    vector<string> localesTW{localeTW};
    string style = "unit";
    string unit = "beat-per-minute";
    string unitStyle = "narrow";
    map<string, string> options = { { "style", style},
                                    { "unit", unit },
                                    { "unitStyle", unitStyle } };
    std::unique_ptr<NumberFormat> numFmtCN = std::make_unique<NumberFormat>(localesCN, options);
    ASSERT_TRUE(numFmtCN != nullptr);
    EXPECT_EQ(numFmtCN->Format(1234567.89), "1,234,567.89 次/分钟");
    std::unique_ptr<NumberFormat> numFmtUS = std::make_unique<NumberFormat>(localesUS, options);
    ASSERT_TRUE(numFmtUS != nullptr);
    EXPECT_EQ(numFmtUS->Format(1), "1 bpm");
    EXPECT_EQ(numFmtUS->Format(1234567.89), "1,234,567.89 bpm");
    std::unique_ptr<NumberFormat> numFmtGB = std::make_unique<NumberFormat>(localesGB, options);
    ASSERT_TRUE(numFmtGB != nullptr);
    EXPECT_EQ(numFmtGB->Format(1), "1 bpm");
    EXPECT_EQ(numFmtGB->Format(1234567.89), "1,234,567.89 bpm");
    std::unique_ptr<NumberFormat> numFmtBO = std::make_unique<NumberFormat>(localesBO, options);
    ASSERT_TRUE(numFmtBO != nullptr);
    EXPECT_EQ(numFmtBO->Format(1234567.89), "སྐར་མ་རེར་ཐེངས་ 1,234,567.89");
    std::unique_ptr<NumberFormat> numFmtUG = std::make_unique<NumberFormat>(localesUG, options);
    ASSERT_TRUE(numFmtUG != nullptr);
    EXPECT_EQ(numFmtUG->Format(1), "ق/م 1");
    EXPECT_EQ(numFmtUG->Format(1234567.89), "ق/م 1,234,567.89");
    std::unique_ptr<NumberFormat> numFmtHK = std::make_unique<NumberFormat>(localesHK, options);
    ASSERT_TRUE(numFmtHK != nullptr);
    EXPECT_EQ(numFmtHK->Format(1234567.89), "1,234,567.89 下/分鐘");
    std::unique_ptr<NumberFormat> numFmtTW = std::make_unique<NumberFormat>(localesTW, options);
    ASSERT_TRUE(numFmtTW != nullptr);
    EXPECT_EQ(numFmtTW->Format(1234567.89), "1,234,567.89 次/分鐘");
}

/**
 * @tc.name: NumberFormatFuncTest0010
 * @tc.desc: Test Intl NumberFormat.format
 * @tc.type: FUNC
 */
HWTEST_F(NumberFormatTest, NumberFormatFuncTest0010, TestSize.Level1)
{
    string localeCN = "zh-CN";
    vector<string> localesCN{localeCN};
    string localeUS = "en-US";
    vector<string> localesUS{localeUS};
    string localeGB = "en-GB";
    vector<string> localesGB{localeGB};
    string localeBO = "bo";
    vector<string> localesBO{localeBO};
    string localeUG = "ug";
    vector<string> localesUG{localeUG};
    string localeHK = "zh-HK";
    vector<string> localesHK{localeHK};
    string localeTW = "zh-TW";
    vector<string> localesTW{localeTW};
    string style = "unit";
    string unit = "body-weight-per-second";
    string unitStyle = "long";
    map<string, string> options = { { "style", style},
                                    { "unit", unit },
                                    { "unitStyle", unitStyle } };
    std::unique_ptr<NumberFormat> numFmtCN = std::make_unique<NumberFormat>(localesCN, options);
    ASSERT_TRUE(numFmtCN != nullptr);
    EXPECT_EQ(numFmtCN->Format(1234567.89), "1,234,567.89 BW/s");
    std::unique_ptr<NumberFormat> numFmtUS = std::make_unique<NumberFormat>(localesUS, options);
    ASSERT_TRUE(numFmtUS != nullptr);
    EXPECT_EQ(numFmtUS->Format(1), "1 BW/s");
    EXPECT_EQ(numFmtUS->Format(1234567.89), "1,234,567.89 BW/s");
    std::unique_ptr<NumberFormat> numFmtGB = std::make_unique<NumberFormat>(localesGB, options);
    ASSERT_TRUE(numFmtGB != nullptr);
    EXPECT_EQ(numFmtGB->Format(1), "1 BW/s");
    EXPECT_EQ(numFmtGB->Format(1234567.89), "1,234,567.89 BW/s");
    std::unique_ptr<NumberFormat> numFmtBO = std::make_unique<NumberFormat>(localesBO, options);
    ASSERT_TRUE(numFmtBO != nullptr);
    EXPECT_EQ(numFmtBO->Format(1234567.89), "1,234,567.89 BW/s");
    std::unique_ptr<NumberFormat> numFmtUG = std::make_unique<NumberFormat>(localesUG, options);
    ASSERT_TRUE(numFmtUG != nullptr);
    EXPECT_EQ(numFmtUG->Format(1), "1 BW/s");
    EXPECT_EQ(numFmtUG->Format(1234567.89), "1234567.89 BW/s");
    std::unique_ptr<NumberFormat> numFmtHK = std::make_unique<NumberFormat>(localesHK, options);
    ASSERT_TRUE(numFmtHK != nullptr);
    EXPECT_EQ(numFmtHK->Format(1234567.89), "1,234,567.89 體重/秒");
    std::unique_ptr<NumberFormat> numFmtTW = std::make_unique<NumberFormat>(localesTW, options);
    ASSERT_TRUE(numFmtTW != nullptr);
    EXPECT_EQ(numFmtTW->Format(1234567.89), "1,234,567.89 BW/s");
}

/**
 * @tc.name: NumberFormatFuncTest0011
 * @tc.desc: Test Intl NumberFormat.format
 * @tc.type: FUNC
 */
HWTEST_F(NumberFormatTest, NumberFormatFuncTest0011, TestSize.Level1)
{
    string localeCN = "zh-CN";
    vector<string> localesCN{localeCN};
    string localeUS = "en-US";
    vector<string> localesUS{localeUS};
    string localeGB = "en-GB";
    vector<string> localesGB{localeGB};
    string localeBO = "bo";
    vector<string> localesBO{localeBO};
    string localeUG = "ug";
    vector<string> localesUG{localeUG};
    string localeHK = "zh-HK";
    vector<string> localesHK{localeHK};
    string localeTW = "zh-TW";
    vector<string> localesTW{localeTW};
    string style = "unit";
    string unit = "body-weight-per-second";
    string unitStyle = "short";
    map<string, string> options = { { "style", style},
                                    { "unit", unit },
                                    { "unitStyle", unitStyle } };
    std::unique_ptr<NumberFormat> numFmtCN = std::make_unique<NumberFormat>(localesCN, options);
    ASSERT_TRUE(numFmtCN != nullptr);
    EXPECT_EQ(numFmtCN->Format(1234567.89), "1,234,567.89 BW/s");
    std::unique_ptr<NumberFormat> numFmtUS = std::make_unique<NumberFormat>(localesUS, options);
    ASSERT_TRUE(numFmtUS != nullptr);
    EXPECT_EQ(numFmtUS->Format(1), "1 BW/s");
    EXPECT_EQ(numFmtUS->Format(1234567.89), "1,234,567.89 BW/s");
    std::unique_ptr<NumberFormat> numFmtGB = std::make_unique<NumberFormat>(localesGB, options);
    ASSERT_TRUE(numFmtGB != nullptr);
    EXPECT_EQ(numFmtGB->Format(1), "1 BW/s");
    EXPECT_EQ(numFmtGB->Format(1234567.89), "1,234,567.89 BW/s");
    std::unique_ptr<NumberFormat> numFmtBO = std::make_unique<NumberFormat>(localesBO, options);
    ASSERT_TRUE(numFmtBO != nullptr);
    EXPECT_EQ(numFmtBO->Format(1234567.89), "1,234,567.89 BW/s");
    std::unique_ptr<NumberFormat> numFmtUG = std::make_unique<NumberFormat>(localesUG, options);
    ASSERT_TRUE(numFmtUG != nullptr);
    EXPECT_EQ(numFmtUG->Format(1), "1 BW/s");
    EXPECT_EQ(numFmtUG->Format(1234567.89), "1234567.89 BW/s");
    std::unique_ptr<NumberFormat> numFmtHK = std::make_unique<NumberFormat>(localesHK, options);
    ASSERT_TRUE(numFmtHK != nullptr);
    EXPECT_EQ(numFmtHK->Format(1234567.89), "1,234,567.89 體重/秒");
    std::unique_ptr<NumberFormat> numFmtTW = std::make_unique<NumberFormat>(localesTW, options);
    ASSERT_TRUE(numFmtTW != nullptr);
    EXPECT_EQ(numFmtTW->Format(1234567.89), "1,234,567.89 BW/s");
}

/**
 * @tc.name: NumberFormatFuncTest0012
 * @tc.desc: Test Intl NumberFormat.format
 * @tc.type: FUNC
 */
HWTEST_F(NumberFormatTest, NumberFormatFuncTest0012, TestSize.Level1)
{
    string localeCN = "zh-CN";
    vector<string> localesCN{localeCN};
    string localeUS = "en-US";
    vector<string> localesUS{localeUS};
    string localeGB = "en-GB";
    vector<string> localesGB{localeGB};
    string localeBO = "bo";
    vector<string> localesBO{localeBO};
    string localeUG = "ug";
    vector<string> localesUG{localeUG};
    string localeHK = "zh-HK";
    vector<string> localesHK{localeHK};
    string localeTW = "zh-TW";
    vector<string> localesTW{localeTW};
    string style = "unit";
    string unit = "body-weight-per-second";
    string unitStyle = "narrow";
    map<string, string> options = { { "style", style},
                                    { "unit", unit },
                                    { "unitStyle", unitStyle } };
    std::unique_ptr<NumberFormat> numFmtCN = std::make_unique<NumberFormat>(localesCN, options);
    ASSERT_TRUE(numFmtCN != nullptr);
    EXPECT_EQ(numFmtCN->Format(1234567.89), "1,234,567.89 BW/s");
    std::unique_ptr<NumberFormat> numFmtUS = std::make_unique<NumberFormat>(localesUS, options);
    ASSERT_TRUE(numFmtUS != nullptr);
    EXPECT_EQ(numFmtUS->Format(1), "1 BW/s");
    EXPECT_EQ(numFmtUS->Format(1234567.89), "1,234,567.89 BW/s");
    std::unique_ptr<NumberFormat> numFmtGB = std::make_unique<NumberFormat>(localesGB, options);
    ASSERT_TRUE(numFmtGB != nullptr);
    EXPECT_EQ(numFmtGB->Format(1), "1 BW/s");
    EXPECT_EQ(numFmtGB->Format(1234567.89), "1,234,567.89 BW/s");
    std::unique_ptr<NumberFormat> numFmtBO = std::make_unique<NumberFormat>(localesBO, options);
    ASSERT_TRUE(numFmtBO != nullptr);
    EXPECT_EQ(numFmtBO->Format(1234567.89), "1,234,567.89 BW/s");
    std::unique_ptr<NumberFormat> numFmtUG = std::make_unique<NumberFormat>(localesUG, options);
    ASSERT_TRUE(numFmtUG != nullptr);
    EXPECT_EQ(numFmtUG->Format(1), "1 BW/s");
    EXPECT_EQ(numFmtUG->Format(1234567.89), "1234567.89 BW/s");
    std::unique_ptr<NumberFormat> numFmtHK = std::make_unique<NumberFormat>(localesHK, options);
    ASSERT_TRUE(numFmtHK != nullptr);
    EXPECT_EQ(numFmtHK->Format(1234567.89), "1,234,567.89 體重/秒");
    std::unique_ptr<NumberFormat> numFmtTW = std::make_unique<NumberFormat>(localesTW, options);
    ASSERT_TRUE(numFmtTW != nullptr);
    EXPECT_EQ(numFmtTW->Format(1234567.89), "1,234,567.89 BW/s");
}

/**
 * @tc.name: NumberFormatFuncTest0013
 * @tc.desc: Test Intl NumberFormat.format
 * @tc.type: FUNC
 */
HWTEST_F(NumberFormatTest, NumberFormatFuncTest0013, TestSize.Level1)
{
    string localeCN = "zh-CN";
    vector<string> localesCN{localeCN};
    string localeUS = "en-US";
    vector<string> localesUS{localeUS};
    string localeGB = "en-GB";
    vector<string> localesGB{localeGB};
    string localeBO = "bo";
    vector<string> localesBO{localeBO};
    string localeUG = "ug";
    vector<string> localesUG{localeUG};
    string localeHK = "zh-HK";
    vector<string> localesHK{localeHK};
    string localeTW = "zh-TW";
    vector<string> localesTW{localeTW};
    string style = "unit";
    string unit = "breath-per-minute";
    string unitStyle = "long";
    map<string, string> options = { { "style", style},
                                    { "unit", unit },
                                    { "unitStyle", unitStyle } };
    std::unique_ptr<NumberFormat> numFmtCN = std::make_unique<NumberFormat>(localesCN, options);
    ASSERT_TRUE(numFmtCN != nullptr);
    EXPECT_EQ(numFmtCN->Format(1234567.89), "1,234,567.89 次/分");
    std::unique_ptr<NumberFormat> numFmtUS = std::make_unique<NumberFormat>(localesUS, options);
    ASSERT_TRUE(numFmtUS != nullptr);
    EXPECT_EQ(numFmtUS->Format(1), "1 brpm");
    EXPECT_EQ(numFmtUS->Format(1234567.89), "1,234,567.89 brpm");
    std::unique_ptr<NumberFormat> numFmtGB = std::make_unique<NumberFormat>(localesGB, options);
    ASSERT_TRUE(numFmtGB != nullptr);
    EXPECT_EQ(numFmtGB->Format(1), "1 brpm");
    EXPECT_EQ(numFmtGB->Format(1234567.89), "1,234,567.89 brpm");
    std::unique_ptr<NumberFormat> numFmtBO = std::make_unique<NumberFormat>(localesBO, options);
    ASSERT_TRUE(numFmtBO != nullptr);
    EXPECT_EQ(numFmtBO->Format(1234567.89), "སྐར་མ་རེར་ཐེངས་ 1,234,567.89");
    std::unique_ptr<NumberFormat> numFmtUG = std::make_unique<NumberFormat>(localesUG, options);
    ASSERT_TRUE(numFmtUG != nullptr);
    EXPECT_EQ(numFmtUG->Format(1), "1 ق/م");
    EXPECT_EQ(numFmtUG->Format(1234567.89), "1234567.89 ق/م");
    std::unique_ptr<NumberFormat> numFmtHK = std::make_unique<NumberFormat>(localesHK, options);
    ASSERT_TRUE(numFmtHK != nullptr);
    EXPECT_EQ(numFmtHK->Format(1234567.89), "1,234,567.89 下/分鐘");
    std::unique_ptr<NumberFormat> numFmtTW = std::make_unique<NumberFormat>(localesTW, options);
    ASSERT_TRUE(numFmtTW != nullptr);
    EXPECT_EQ(numFmtTW->Format(1234567.89), "1,234,567.89 次/分");
}

/**
 * @tc.name: NumberFormatFuncTest0014
 * @tc.desc: Test Intl NumberFormat.format
 * @tc.type: FUNC
 */
HWTEST_F(NumberFormatTest, NumberFormatFuncTest0014, TestSize.Level1)
{
    string localeCN = "zh-CN";
    vector<string> localesCN{localeCN};
    string localeUS = "en-US";
    vector<string> localesUS{localeUS};
    string localeGB = "en-GB";
    vector<string> localesGB{localeGB};
    string localeBO = "bo";
    vector<string> localesBO{localeBO};
    string localeUG = "ug";
    vector<string> localesUG{localeUG};
    string localeHK = "zh-HK";
    vector<string> localesHK{localeHK};
    string localeTW = "zh-TW";
    vector<string> localesTW{localeTW};
    string style = "unit";
    string unit = "breath-per-minute";
    string unitStyle = "short";
    map<string, string> options = { { "style", style},
                                    { "unit", unit },
                                    { "unitStyle", unitStyle } };
    std::unique_ptr<NumberFormat> numFmtCN = std::make_unique<NumberFormat>(localesCN, options);
    ASSERT_TRUE(numFmtCN != nullptr);
    EXPECT_EQ(numFmtCN->Format(1234567.89), "1,234,567.89 次/分");
    std::unique_ptr<NumberFormat> numFmtUS = std::make_unique<NumberFormat>(localesUS, options);
    ASSERT_TRUE(numFmtUS != nullptr);
    EXPECT_EQ(numFmtUS->Format(1), "1 brpm");
    EXPECT_EQ(numFmtUS->Format(1234567.89), "1,234,567.89 brpm");
    std::unique_ptr<NumberFormat> numFmtGB = std::make_unique<NumberFormat>(localesGB, options);
    ASSERT_TRUE(numFmtGB != nullptr);
    EXPECT_EQ(numFmtGB->Format(1), "1 brpm");
    EXPECT_EQ(numFmtGB->Format(1234567.89), "1,234,567.89 brpm");
    std::unique_ptr<NumberFormat> numFmtBO = std::make_unique<NumberFormat>(localesBO, options);
    ASSERT_TRUE(numFmtBO != nullptr);
    EXPECT_EQ(numFmtBO->Format(1234567.89), "སྐར་མ་རེར་ཐེངས་ 1,234,567.89");
    std::unique_ptr<NumberFormat> numFmtUG = std::make_unique<NumberFormat>(localesUG, options);
    ASSERT_TRUE(numFmtUG != nullptr);
    EXPECT_EQ(numFmtUG->Format(1), "1 ق/م");
    EXPECT_EQ(numFmtUG->Format(1234567.89), "1234567.89 ق/م");
    std::unique_ptr<NumberFormat> numFmtHK = std::make_unique<NumberFormat>(localesHK, options);
    ASSERT_TRUE(numFmtHK != nullptr);
    EXPECT_EQ(numFmtHK->Format(1234567.89), "1,234,567.89 下/分鐘");
    std::unique_ptr<NumberFormat> numFmtTW = std::make_unique<NumberFormat>(localesTW, options);
    ASSERT_TRUE(numFmtTW != nullptr);
    EXPECT_EQ(numFmtTW->Format(1234567.89), "1,234,567.89 次/分");
}

/**
 * @tc.name: NumberFormatFuncTest0015
 * @tc.desc: Test Intl NumberFormat.format
 * @tc.type: FUNC
 */
HWTEST_F(NumberFormatTest, NumberFormatFuncTest0015, TestSize.Level1)
{
    string localeCN = "zh-CN";
    vector<string> localesCN{localeCN};
    string localeUS = "en-US";
    vector<string> localesUS{localeUS};
    string localeGB = "en-GB";
    vector<string> localesGB{localeGB};
    string localeBO = "bo";
    vector<string> localesBO{localeBO};
    string localeUG = "ug";
    vector<string> localesUG{localeUG};
    string localeHK = "zh-HK";
    vector<string> localesHK{localeHK};
    string localeTW = "zh-TW";
    vector<string> localesTW{localeTW};
    string style = "unit";
    string unit = "breath-per-minute";
    string unitStyle = "narrow";
    map<string, string> options = { { "style", style},
                                    { "unit", unit },
                                    { "unitStyle", unitStyle } };
    std::unique_ptr<NumberFormat> numFmtCN = std::make_unique<NumberFormat>(localesCN, options);
    ASSERT_TRUE(numFmtCN != nullptr);
    EXPECT_EQ(numFmtCN->Format(1234567.89), "1,234,567.89 次/分");
    std::unique_ptr<NumberFormat> numFmtUS = std::make_unique<NumberFormat>(localesUS, options);
    ASSERT_TRUE(numFmtUS != nullptr);
    EXPECT_EQ(numFmtUS->Format(1), "1 brpm");
    EXPECT_EQ(numFmtUS->Format(1234567.89), "1,234,567.89 brpm");
    std::unique_ptr<NumberFormat> numFmtGB = std::make_unique<NumberFormat>(localesGB, options);
    ASSERT_TRUE(numFmtGB != nullptr);
    EXPECT_EQ(numFmtGB->Format(1), "1 brpm");
    EXPECT_EQ(numFmtGB->Format(1234567.89), "1,234,567.89 brpm");
    std::unique_ptr<NumberFormat> numFmtBO = std::make_unique<NumberFormat>(localesBO, options);
    ASSERT_TRUE(numFmtBO != nullptr);
    EXPECT_EQ(numFmtBO->Format(1234567.89), "སྐར་མ་རེར་ཐེངས་ 1,234,567.89");
    std::unique_ptr<NumberFormat> numFmtUG = std::make_unique<NumberFormat>(localesUG, options);
    ASSERT_TRUE(numFmtUG != nullptr);
    EXPECT_EQ(numFmtUG->Format(1), "1 ق/م");
    EXPECT_EQ(numFmtUG->Format(1234567.89), "1234567.89 ق/م");
    std::unique_ptr<NumberFormat> numFmtHK = std::make_unique<NumberFormat>(localesHK, options);
    ASSERT_TRUE(numFmtHK != nullptr);
    EXPECT_EQ(numFmtHK->Format(1234567.89), "1,234,567.89 下/分鐘");
    std::unique_ptr<NumberFormat> numFmtTW = std::make_unique<NumberFormat>(localesTW, options);
    ASSERT_TRUE(numFmtTW != nullptr);
    EXPECT_EQ(numFmtTW->Format(1234567.89), "1,234,567.89 次/分");
}

/**
 * @tc.name: NumberFormatFuncTest0016
 * @tc.desc: Test Intl NumberFormat.format
 * @tc.type: FUNC
 */
HWTEST_F(NumberFormatTest, NumberFormatFuncTest0016, TestSize.Level1)
{
    string localeCN = "zh-CN";
    vector<string> localesCN{localeCN};
    string localeUS = "en-US";
    vector<string> localesUS{localeUS};
    string localeGB = "en-GB";
    vector<string> localesGB{localeGB};
    string localeBO = "bo";
    vector<string> localesBO{localeBO};
    string localeUG = "ug";
    vector<string> localesUG{localeUG};
    string localeHK = "zh-HK";
    vector<string> localesHK{localeHK};
    string localeTW = "zh-TW";
    vector<string> localesTW{localeTW};
    string style = "unit";
    string unit = "foot-per-hour";
    string unitStyle = "long";
    map<string, string> options = { { "style", style},
                                    { "unit", unit },
                                    { "unitStyle", unitStyle } };
    std::unique_ptr<NumberFormat> numFmtCN = std::make_unique<NumberFormat>(localesCN, options);
    ASSERT_TRUE(numFmtCN != nullptr);
    EXPECT_EQ(numFmtCN->Format(1234567.89), "1,234,567.89 英尺/小时");
    std::unique_ptr<NumberFormat> numFmtUS = std::make_unique<NumberFormat>(localesUS, options);
    ASSERT_TRUE(numFmtUS != nullptr);
    EXPECT_EQ(numFmtUS->Format(1), "1 ft/h");
    EXPECT_EQ(numFmtUS->Format(1234567.89), "1,234,567.89 ft/h");
    std::unique_ptr<NumberFormat> numFmtGB = std::make_unique<NumberFormat>(localesGB, options);
    ASSERT_TRUE(numFmtGB != nullptr);
    EXPECT_EQ(numFmtGB->Format(1), "1 ft/h");
    EXPECT_EQ(numFmtGB->Format(1234567.89), "1,234,567.89 ft/h");
    std::unique_ptr<NumberFormat> numFmtBO = std::make_unique<NumberFormat>(localesBO, options);
    ASSERT_TRUE(numFmtBO != nullptr);
    EXPECT_EQ(numFmtBO->Format(1234567.89), "1,234,567.89 ft/h");
    std::unique_ptr<NumberFormat> numFmtUG = std::make_unique<NumberFormat>(localesUG, options);
    ASSERT_TRUE(numFmtUG != nullptr);
    EXPECT_EQ(numFmtUG->Format(1), "1 ft/h");
    EXPECT_EQ(numFmtUG->Format(1234567.89), "1,234,567.89 ft/h");
    std::unique_ptr<NumberFormat> numFmtHK = std::make_unique<NumberFormat>(localesHK, options);
    ASSERT_TRUE(numFmtHK != nullptr);
    EXPECT_EQ(numFmtHK->Format(1234567.89), "1,234,567.89 呎/小時");
    std::unique_ptr<NumberFormat> numFmtTW = std::make_unique<NumberFormat>(localesTW, options);
    ASSERT_TRUE(numFmtTW != nullptr);
    EXPECT_EQ(numFmtTW->Format(1234567.89), "1,234,567.89 英尺/小時");
}

/**
 * @tc.name: NumberFormatFuncTest0017
 * @tc.desc: Test Intl NumberFormat.format
 * @tc.type: FUNC
 */
HWTEST_F(NumberFormatTest, NumberFormatFuncTest0017, TestSize.Level1)
{
    string localeCN = "zh-CN";
    vector<string> localesCN{localeCN};
    string localeUS = "en-US";
    vector<string> localesUS{localeUS};
    string localeGB = "en-GB";
    vector<string> localesGB{localeGB};
    string localeBO = "bo";
    vector<string> localesBO{localeBO};
    string localeUG = "ug";
    vector<string> localesUG{localeUG};
    string localeHK = "zh-HK";
    vector<string> localesHK{localeHK};
    string localeTW = "zh-TW";
    vector<string> localesTW{localeTW};
    string style = "unit";
    string unit = "foot-per-hour";
    string unitStyle = "short";
    map<string, string> options = { { "style", style},
                                    { "unit", unit },
                                    { "unitStyle", unitStyle } };
    std::unique_ptr<NumberFormat> numFmtCN = std::make_unique<NumberFormat>(localesCN, options);
    ASSERT_TRUE(numFmtCN != nullptr);
    EXPECT_EQ(numFmtCN->Format(1234567.89), "1,234,567.89 英尺/小时");
    std::unique_ptr<NumberFormat> numFmtUS = std::make_unique<NumberFormat>(localesUS, options);
    ASSERT_TRUE(numFmtUS != nullptr);
    EXPECT_EQ(numFmtUS->Format(1), "1 ft/h");
    EXPECT_EQ(numFmtUS->Format(1234567.89), "1,234,567.89 ft/h");
    std::unique_ptr<NumberFormat> numFmtGB = std::make_unique<NumberFormat>(localesGB, options);
    ASSERT_TRUE(numFmtGB != nullptr);
    EXPECT_EQ(numFmtGB->Format(1), "1 ft/h");
    EXPECT_EQ(numFmtGB->Format(1234567.89), "1,234,567.89 ft/h");
    std::unique_ptr<NumberFormat> numFmtBO = std::make_unique<NumberFormat>(localesBO, options);
    ASSERT_TRUE(numFmtBO != nullptr);
    EXPECT_EQ(numFmtBO->Format(1234567.89), "1,234,567.89 ft/h");
    std::unique_ptr<NumberFormat> numFmtUG = std::make_unique<NumberFormat>(localesUG, options);
    ASSERT_TRUE(numFmtUG != nullptr);
    EXPECT_EQ(numFmtUG->Format(1), "1 ft/h");
    EXPECT_EQ(numFmtUG->Format(1234567.89), "1,234,567.89 ft/h");
    std::unique_ptr<NumberFormat> numFmtHK = std::make_unique<NumberFormat>(localesHK, options);
    ASSERT_TRUE(numFmtHK != nullptr);
    EXPECT_EQ(numFmtHK->Format(1234567.89), "1,234,567.89 呎/小時");
    std::unique_ptr<NumberFormat> numFmtTW = std::make_unique<NumberFormat>(localesTW, options);
    ASSERT_TRUE(numFmtTW != nullptr);
    EXPECT_EQ(numFmtTW->Format(1234567.89), "1,234,567.89 英尺/小時");
}

/**
 * @tc.name: NumberFormatFuncTest0018
 * @tc.desc: Test Intl NumberFormat.format
 * @tc.type: FUNC
 */
HWTEST_F(NumberFormatTest, NumberFormatFuncTest0018, TestSize.Level1)
{
    string localeCN = "zh-CN";
    vector<string> localesCN{localeCN};
    string localeUS = "en-US";
    vector<string> localesUS{localeUS};
    string localeGB = "en-GB";
    vector<string> localesGB{localeGB};
    string localeBO = "bo";
    vector<string> localesBO{localeBO};
    string localeUG = "ug";
    vector<string> localesUG{localeUG};
    string localeHK = "zh-HK";
    vector<string> localesHK{localeHK};
    string localeTW = "zh-TW";
    vector<string> localesTW{localeTW};
    string style = "unit";
    string unit = "foot-per-hour";
    string unitStyle = "narrow";
    map<string, string> options = { { "style", style},
                                    { "unit", unit },
                                    { "unitStyle", unitStyle } };
    std::unique_ptr<NumberFormat> numFmtCN = std::make_unique<NumberFormat>(localesCN, options);
    ASSERT_TRUE(numFmtCN != nullptr);
    EXPECT_EQ(numFmtCN->Format(1234567.89), "1,234,567.89 英尺/小时");
    std::unique_ptr<NumberFormat> numFmtUS = std::make_unique<NumberFormat>(localesUS, options);
    ASSERT_TRUE(numFmtUS != nullptr);
    EXPECT_EQ(numFmtUS->Format(1), "1 ft/h");
    EXPECT_EQ(numFmtUS->Format(1234567.89), "1,234,567.89 ft/h");
    std::unique_ptr<NumberFormat> numFmtGB = std::make_unique<NumberFormat>(localesGB, options);
    ASSERT_TRUE(numFmtGB != nullptr);
    EXPECT_EQ(numFmtGB->Format(1), "1 ft/h");
    EXPECT_EQ(numFmtGB->Format(1234567.89), "1,234,567.89 ft/h");
    std::unique_ptr<NumberFormat> numFmtBO = std::make_unique<NumberFormat>(localesBO, options);
    ASSERT_TRUE(numFmtBO != nullptr);
    EXPECT_EQ(numFmtBO->Format(1234567.89), "1,234,567.89 ft/h");
    std::unique_ptr<NumberFormat> numFmtUG = std::make_unique<NumberFormat>(localesUG, options);
    ASSERT_TRUE(numFmtUG != nullptr);
    EXPECT_EQ(numFmtUG->Format(1), "1 ft/h");
    EXPECT_EQ(numFmtUG->Format(1234567.89), "1,234,567.89 ft/h");
    std::unique_ptr<NumberFormat> numFmtHK = std::make_unique<NumberFormat>(localesHK, options);
    ASSERT_TRUE(numFmtHK != nullptr);
    EXPECT_EQ(numFmtHK->Format(1234567.89), "1,234,567.89 呎/小時");
    std::unique_ptr<NumberFormat> numFmtTW = std::make_unique<NumberFormat>(localesTW, options);
    ASSERT_TRUE(numFmtTW != nullptr);
    EXPECT_EQ(numFmtTW->Format(1234567.89), "1,234,567.89 英尺/小時");
}

/**
 * @tc.name: NumberFormatFuncTest0019
 * @tc.desc: Test Intl NumberFormat.format
 * @tc.type: FUNC
 */
HWTEST_F(NumberFormatTest, NumberFormatFuncTest0019, TestSize.Level1)
{
    string localeCN = "zh-CN";
    vector<string> localesCN{localeCN};
    string localeUS = "en-US";
    vector<string> localesUS{localeUS};
    string localeGB = "en-GB";
    vector<string> localesGB{localeGB};
    string localeBO = "bo";
    vector<string> localesBO{localeBO};
    string localeUG = "ug";
    vector<string> localesUG{localeUG};
    string localeHK = "zh-HK";
    vector<string> localesHK{localeHK};
    string localeTW = "zh-TW";
    vector<string> localesTW{localeTW};
    string style = "unit";
    string unit = "jump-rope-speed";
    string unitStyle = "long";
    map<string, string> options = { { "style", style},
                                    { "unit", unit },
                                    { "unitStyle", unitStyle } };
    std::unique_ptr<NumberFormat> numFmtCN = std::make_unique<NumberFormat>(localesCN, options);
    ASSERT_TRUE(numFmtCN != nullptr);
    EXPECT_EQ(numFmtCN->Format(1234567.89), "1,234,567.89 个/分钟");
    std::unique_ptr<NumberFormat> numFmtUS = std::make_unique<NumberFormat>(localesUS, options);
    ASSERT_TRUE(numFmtUS != nullptr);
    EXPECT_EQ(numFmtUS->Format(1), "1 jump/minute");
    EXPECT_EQ(numFmtUS->Format(1234567.89), "1,234,567.89 jumps/minute");
    std::unique_ptr<NumberFormat> numFmtGB = std::make_unique<NumberFormat>(localesGB, options);
    ASSERT_TRUE(numFmtGB != nullptr);
    EXPECT_EQ(numFmtGB->Format(1), "1 skip/minute");
    EXPECT_EQ(numFmtGB->Format(1234567.89), "1,234,567.89 skips/minute");
    std::unique_ptr<NumberFormat> numFmtBO = std::make_unique<NumberFormat>(localesBO, options);
    ASSERT_TRUE(numFmtBO != nullptr);
    EXPECT_EQ(numFmtBO->Format(1234567.89), "སྐར་མ་རེར་ 1,234,567.89");
    std::unique_ptr<NumberFormat> numFmtUG = std::make_unique<NumberFormat>(localesUG, options);
    ASSERT_TRUE(numFmtUG != nullptr);
    EXPECT_EQ(numFmtUG->Format(1), "1 ئاتلام/مىنۇت");
    EXPECT_EQ(numFmtUG->Format(1234567.89), "1234567.89 ئاتلام/مىنۇت");
    std::unique_ptr<NumberFormat> numFmtHK = std::make_unique<NumberFormat>(localesHK, options);
    ASSERT_TRUE(numFmtHK != nullptr);
    EXPECT_EQ(numFmtHK->Format(1234567.89), "1,234,567.89 下/分鐘");
    std::unique_ptr<NumberFormat> numFmtTW = std::make_unique<NumberFormat>(localesTW, options);
    ASSERT_TRUE(numFmtTW != nullptr);
    EXPECT_EQ(numFmtTW->Format(1234567.89), "1,234,567.89 個/分鐘");
}

/**
 * @tc.name: NumberFormatFuncTest0020
 * @tc.desc: Test Intl NumberFormat.format
 * @tc.type: FUNC
 */
HWTEST_F(NumberFormatTest, NumberFormatFuncTest0020, TestSize.Level1)
{
    string localeCN = "zh-CN";
    vector<string> localesCN{localeCN};
    string localeUS = "en-US";
    vector<string> localesUS{localeUS};
    string localeGB = "en-GB";
    vector<string> localesGB{localeGB};
    string localeBO = "bo";
    vector<string> localesBO{localeBO};
    string localeUG = "ug";
    vector<string> localesUG{localeUG};
    string localeHK = "zh-HK";
    vector<string> localesHK{localeHK};
    string localeTW = "zh-TW";
    vector<string> localesTW{localeTW};
    string style = "unit";
    string unit = "jump-rope-speed";
    string unitStyle = "short";
    map<string, string> options = { { "style", style},
                                    { "unit", unit },
                                    { "unitStyle", unitStyle } };
    std::unique_ptr<NumberFormat> numFmtCN = std::make_unique<NumberFormat>(localesCN, options);
    ASSERT_TRUE(numFmtCN != nullptr);
    EXPECT_EQ(numFmtCN->Format(1234567.89), "1,234,567.89 个/分钟");
    std::unique_ptr<NumberFormat> numFmtUS = std::make_unique<NumberFormat>(localesUS, options);
    ASSERT_TRUE(numFmtUS != nullptr);
    EXPECT_EQ(numFmtUS->Format(1), "1 jump/minute");
    EXPECT_EQ(numFmtUS->Format(1234567.89), "1,234,567.89 jumps/minute");
    std::unique_ptr<NumberFormat> numFmtGB = std::make_unique<NumberFormat>(localesGB, options);
    ASSERT_TRUE(numFmtGB != nullptr);
    EXPECT_EQ(numFmtGB->Format(1), "1 skip/minute");
    EXPECT_EQ(numFmtGB->Format(1234567.89), "1,234,567.89 skips/minute");
    std::unique_ptr<NumberFormat> numFmtBO = std::make_unique<NumberFormat>(localesBO, options);
    ASSERT_TRUE(numFmtBO != nullptr);
    EXPECT_EQ(numFmtBO->Format(1234567.89), "སྐར་མ་རེར་ 1,234,567.89");
    std::unique_ptr<NumberFormat> numFmtUG = std::make_unique<NumberFormat>(localesUG, options);
    ASSERT_TRUE(numFmtUG != nullptr);
    EXPECT_EQ(numFmtUG->Format(1), "1 ئاتلام/مىنۇت");
    EXPECT_EQ(numFmtUG->Format(1234567.89), "1234567.89 ئاتلام/مىنۇت");
    std::unique_ptr<NumberFormat> numFmtHK = std::make_unique<NumberFormat>(localesHK, options);
    ASSERT_TRUE(numFmtHK != nullptr);
    EXPECT_EQ(numFmtHK->Format(1234567.89), "1,234,567.89 下/分鐘");
    std::unique_ptr<NumberFormat> numFmtTW = std::make_unique<NumberFormat>(localesTW, options);
    ASSERT_TRUE(numFmtTW != nullptr);
    EXPECT_EQ(numFmtTW->Format(1234567.89), "1,234,567.89 個/分鐘");
}

/**
 * @tc.name: NumberFormatFuncTest0021
 * @tc.desc: Test Intl NumberFormat.format
 * @tc.type: FUNC
 */
HWTEST_F(NumberFormatTest, NumberFormatFuncTest0021, TestSize.Level1)
{
    string localeCN = "zh-CN";
    vector<string> localesCN{localeCN};
    string localeUS = "en-US";
    vector<string> localesUS{localeUS};
    string localeGB = "en-GB";
    vector<string> localesGB{localeGB};
    string localeBO = "bo";
    vector<string> localesBO{localeBO};
    string localeUG = "ug";
    vector<string> localesUG{localeUG};
    string localeHK = "zh-HK";
    vector<string> localesHK{localeHK};
    string localeTW = "zh-TW";
    vector<string> localesTW{localeTW};
    string style = "unit";
    string unit = "jump-rope-speed";
    string unitStyle = "narrow";
    map<string, string> options = { { "style", style},
                                    { "unit", unit },
                                    { "unitStyle", unitStyle } };
    std::unique_ptr<NumberFormat> numFmtCN = std::make_unique<NumberFormat>(localesCN, options);
    ASSERT_TRUE(numFmtCN != nullptr);
    EXPECT_EQ(numFmtCN->Format(1234567.89), "1,234,567.89 个/分钟");
    std::unique_ptr<NumberFormat> numFmtUS = std::make_unique<NumberFormat>(localesUS, options);
    ASSERT_TRUE(numFmtUS != nullptr);
    EXPECT_EQ(numFmtUS->Format(1), "1 jump/minute");
    EXPECT_EQ(numFmtUS->Format(1234567.89), "1,234,567.89 jumps/minute");
    std::unique_ptr<NumberFormat> numFmtGB = std::make_unique<NumberFormat>(localesGB, options);
    ASSERT_TRUE(numFmtGB != nullptr);
    EXPECT_EQ(numFmtGB->Format(1), "1 skip/minute");
    EXPECT_EQ(numFmtGB->Format(1234567.89), "1,234,567.89 skips/minute");
    std::unique_ptr<NumberFormat> numFmtBO = std::make_unique<NumberFormat>(localesBO, options);
    ASSERT_TRUE(numFmtBO != nullptr);
    EXPECT_EQ(numFmtBO->Format(1234567.89), "སྐར་མ་རེར་ 1,234,567.89");
    std::unique_ptr<NumberFormat> numFmtUG = std::make_unique<NumberFormat>(localesUG, options);
    ASSERT_TRUE(numFmtUG != nullptr);
    EXPECT_EQ(numFmtUG->Format(1), "1 ئاتلام/مىنۇت");
    EXPECT_EQ(numFmtUG->Format(1234567.89), "1234567.89 ئاتلام/مىنۇت");
    std::unique_ptr<NumberFormat> numFmtHK = std::make_unique<NumberFormat>(localesHK, options);
    ASSERT_TRUE(numFmtHK != nullptr);
    EXPECT_EQ(numFmtHK->Format(1234567.89), "1,234,567.89 下/分鐘");
    std::unique_ptr<NumberFormat> numFmtTW = std::make_unique<NumberFormat>(localesTW, options);
    ASSERT_TRUE(numFmtTW != nullptr);
    EXPECT_EQ(numFmtTW->Format(1234567.89), "1,234,567.89 個/分鐘");
}

/**
 * @tc.name: NumberFormatFuncTest0022
 * @tc.desc: Test Intl NumberFormat.format
 * @tc.type: FUNC
 */
HWTEST_F(NumberFormatTest, NumberFormatFuncTest0022, TestSize.Level1)
{
    string localeCN = "zh-CN";
    vector<string> localesCN{localeCN};
    string localeUS = "en-US";
    vector<string> localesUS{localeUS};
    string localeGB = "en-GB";
    vector<string> localesGB{localeGB};
    string localeBO = "bo";
    vector<string> localesBO{localeBO};
    string localeUG = "ug";
    vector<string> localesUG{localeUG};
    string localeHK = "zh-HK";
    vector<string> localesHK{localeHK};
    string localeTW = "zh-TW";
    vector<string> localesTW{localeTW};
    string style = "unit";
    string unit = "meter-per-hour";
    string unitStyle = "long";
    map<string, string> options = { { "style", style},
                                    { "unit", unit },
                                    { "unitStyle", unitStyle } };
    std::unique_ptr<NumberFormat> numFmtCN = std::make_unique<NumberFormat>(localesCN, options);
    ASSERT_TRUE(numFmtCN != nullptr);
    EXPECT_EQ(numFmtCN->Format(1234567.89), "1,234,567.89 米/小时");
    std::unique_ptr<NumberFormat> numFmtUS = std::make_unique<NumberFormat>(localesUS, options);
    ASSERT_TRUE(numFmtUS != nullptr);
    EXPECT_EQ(numFmtUS->Format(1), "1 m/h");
    EXPECT_EQ(numFmtUS->Format(1234567.89), "1,234,567.89 m/h");
    std::unique_ptr<NumberFormat> numFmtGB = std::make_unique<NumberFormat>(localesGB, options);
    ASSERT_TRUE(numFmtGB != nullptr);
    EXPECT_EQ(numFmtGB->Format(1), "1 m/h");
    EXPECT_EQ(numFmtGB->Format(1234567.89), "1,234,567.89 m/h");
    std::unique_ptr<NumberFormat> numFmtBO = std::make_unique<NumberFormat>(localesBO, options);
    ASSERT_TRUE(numFmtBO != nullptr);
    EXPECT_EQ(numFmtBO->Format(1234567.89), "1,234,567.89 m/h");
    std::unique_ptr<NumberFormat> numFmtUG = std::make_unique<NumberFormat>(localesUG, options);
    ASSERT_TRUE(numFmtUG != nullptr);
    EXPECT_EQ(numFmtUG->Format(1), "1 m/h");
    EXPECT_EQ(numFmtUG->Format(1234567.89), "1234567.89 m/h");
    std::unique_ptr<NumberFormat> numFmtHK = std::make_unique<NumberFormat>(localesHK, options);
    ASSERT_TRUE(numFmtHK != nullptr);
    EXPECT_EQ(numFmtHK->Format(1234567.89), "1,234,567.89 米/小時");
    std::unique_ptr<NumberFormat> numFmtTW = std::make_unique<NumberFormat>(localesTW, options);
    ASSERT_TRUE(numFmtTW != nullptr);
    EXPECT_EQ(numFmtTW->Format(1234567.89), "1,234,567.89 公尺/小時");
}

/**
 * @tc.name: NumberFormatFuncTest0023
 * @tc.desc: Test Intl NumberFormat.format
 * @tc.type: FUNC
 */
HWTEST_F(NumberFormatTest, NumberFormatFuncTest0023, TestSize.Level1)
{
    string localeCN = "zh-CN";
    vector<string> localesCN{localeCN};
    string localeUS = "en-US";
    vector<string> localesUS{localeUS};
    string localeGB = "en-GB";
    vector<string> localesGB{localeGB};
    string localeBO = "bo";
    vector<string> localesBO{localeBO};
    string localeUG = "ug";
    vector<string> localesUG{localeUG};
    string localeHK = "zh-HK";
    vector<string> localesHK{localeHK};
    string localeTW = "zh-TW";
    vector<string> localesTW{localeTW};
    string style = "unit";
    string unit = "meter-per-hour";
    string unitStyle = "short";
    map<string, string> options = { { "style", style},
                                    { "unit", unit },
                                    { "unitStyle", unitStyle } };
    std::unique_ptr<NumberFormat> numFmtCN = std::make_unique<NumberFormat>(localesCN, options);
    ASSERT_TRUE(numFmtCN != nullptr);
    EXPECT_EQ(numFmtCN->Format(1234567.89), "1,234,567.89 米/小时");
    std::unique_ptr<NumberFormat> numFmtUS = std::make_unique<NumberFormat>(localesUS, options);
    ASSERT_TRUE(numFmtUS != nullptr);
    EXPECT_EQ(numFmtUS->Format(1), "1 m/h");
    EXPECT_EQ(numFmtUS->Format(1234567.89), "1,234,567.89 m/h");
    std::unique_ptr<NumberFormat> numFmtGB = std::make_unique<NumberFormat>(localesGB, options);
    ASSERT_TRUE(numFmtGB != nullptr);
    EXPECT_EQ(numFmtGB->Format(1), "1 m/h");
    EXPECT_EQ(numFmtGB->Format(1234567.89), "1,234,567.89 m/h");
    std::unique_ptr<NumberFormat> numFmtBO = std::make_unique<NumberFormat>(localesBO, options);
    ASSERT_TRUE(numFmtBO != nullptr);
    EXPECT_EQ(numFmtBO->Format(1234567.89), "1,234,567.89 m/h");
    std::unique_ptr<NumberFormat> numFmtUG = std::make_unique<NumberFormat>(localesUG, options);
    ASSERT_TRUE(numFmtUG != nullptr);
    EXPECT_EQ(numFmtUG->Format(1), "1 m/h");
    EXPECT_EQ(numFmtUG->Format(1234567.89), "1234567.89 m/h");
    std::unique_ptr<NumberFormat> numFmtHK = std::make_unique<NumberFormat>(localesHK, options);
    ASSERT_TRUE(numFmtHK != nullptr);
    EXPECT_EQ(numFmtHK->Format(1234567.89), "1,234,567.89 米/小時");
    std::unique_ptr<NumberFormat> numFmtTW = std::make_unique<NumberFormat>(localesTW, options);
    ASSERT_TRUE(numFmtTW != nullptr);
    EXPECT_EQ(numFmtTW->Format(1234567.89), "1,234,567.89 公尺/小時");
}

/**
 * @tc.name: NumberFormatFuncTest0024
 * @tc.desc: Test Intl NumberFormat.format
 * @tc.type: FUNC
 */
HWTEST_F(NumberFormatTest, NumberFormatFuncTest0024, TestSize.Level1)
{
    string localeCN = "zh-CN";
    vector<string> localesCN{localeCN};
    string localeUS = "en-US";
    vector<string> localesUS{localeUS};
    string localeGB = "en-GB";
    vector<string> localesGB{localeGB};
    string localeBO = "bo";
    vector<string> localesBO{localeBO};
    string localeUG = "ug";
    vector<string> localesUG{localeUG};
    string localeHK = "zh-HK";
    vector<string> localesHK{localeHK};
    string localeTW = "zh-TW";
    vector<string> localesTW{localeTW};
    string style = "unit";
    string unit = "meter-per-hour";
    string unitStyle = "narrow";
    map<string, string> options = { { "style", style},
                                    { "unit", unit },
                                    { "unitStyle", unitStyle } };
    std::unique_ptr<NumberFormat> numFmtCN = std::make_unique<NumberFormat>(localesCN, options);
    ASSERT_TRUE(numFmtCN != nullptr);
    EXPECT_EQ(numFmtCN->Format(1234567.89), "1,234,567.89 米/小时");
    std::unique_ptr<NumberFormat> numFmtUS = std::make_unique<NumberFormat>(localesUS, options);
    ASSERT_TRUE(numFmtUS != nullptr);
    EXPECT_EQ(numFmtUS->Format(1), "1 m/h");
    EXPECT_EQ(numFmtUS->Format(1234567.89), "1,234,567.89 m/h");
    std::unique_ptr<NumberFormat> numFmtGB = std::make_unique<NumberFormat>(localesGB, options);
    ASSERT_TRUE(numFmtGB != nullptr);
    EXPECT_EQ(numFmtGB->Format(1), "1 m/h");
    EXPECT_EQ(numFmtGB->Format(1234567.89), "1,234,567.89 m/h");
    std::unique_ptr<NumberFormat> numFmtBO = std::make_unique<NumberFormat>(localesBO, options);
    ASSERT_TRUE(numFmtBO != nullptr);
    EXPECT_EQ(numFmtBO->Format(1234567.89), "1,234,567.89 m/h");
    std::unique_ptr<NumberFormat> numFmtUG = std::make_unique<NumberFormat>(localesUG, options);
    ASSERT_TRUE(numFmtUG != nullptr);
    EXPECT_EQ(numFmtUG->Format(1), "1 m/h");
    EXPECT_EQ(numFmtUG->Format(1234567.89), "1234567.89 m/h");
    std::unique_ptr<NumberFormat> numFmtHK = std::make_unique<NumberFormat>(localesHK, options);
    ASSERT_TRUE(numFmtHK != nullptr);
    EXPECT_EQ(numFmtHK->Format(1234567.89), "1,234,567.89 米/小時");
    std::unique_ptr<NumberFormat> numFmtTW = std::make_unique<NumberFormat>(localesTW, options);
    ASSERT_TRUE(numFmtTW != nullptr);
    EXPECT_EQ(numFmtTW->Format(1234567.89), "1,234,567.89 公尺/小時");
}

/**
 * @tc.name: NumberFormatFuncTest0025
 * @tc.desc: Test Intl NumberFormat.format
 * @tc.type: FUNC
 */
HWTEST_F(NumberFormatTest, NumberFormatFuncTest0025, TestSize.Level1)
{
    string localeCN = "zh-CN";
    vector<string> localesCN{localeCN};
    string localeUS = "en-US";
    vector<string> localesUS{localeUS};
    string localeGB = "en-GB";
    vector<string> localesGB{localeGB};
    string localeBO = "bo";
    vector<string> localesBO{localeBO};
    string localeUG = "ug";
    vector<string> localesUG{localeUG};
    string localeHK = "zh-HK";
    vector<string> localesHK{localeHK};
    string localeTW = "zh-TW";
    vector<string> localesTW{localeTW};
    string style = "unit";
    string unit = "milliliter-per-minute-per-kilogram";
    string unitStyle = "long";
    map<string, string> options = { { "style", style},
                                    { "unit", unit },
                                    { "unitStyle", unitStyle } };
    std::unique_ptr<NumberFormat> numFmtCN = std::make_unique<NumberFormat>(localesCN, options);
    ASSERT_TRUE(numFmtCN != nullptr);
    EXPECT_EQ(numFmtCN->Format(1234567.89), "1,234,567.89 ml/kg/min");
    std::unique_ptr<NumberFormat> numFmtUS = std::make_unique<NumberFormat>(localesUS, options);
    ASSERT_TRUE(numFmtUS != nullptr);
    EXPECT_EQ(numFmtUS->Format(1), "1 ml/kg/min");
    EXPECT_EQ(numFmtUS->Format(1234567.89), "1,234,567.89 ml/kg/min");
    std::unique_ptr<NumberFormat> numFmtGB = std::make_unique<NumberFormat>(localesGB, options);
    ASSERT_TRUE(numFmtGB != nullptr);
    EXPECT_EQ(numFmtGB->Format(1), "1 ml/kg/min");
    EXPECT_EQ(numFmtGB->Format(1234567.89), "1,234,567.89 ml/kg/min");
    std::unique_ptr<NumberFormat> numFmtBO = std::make_unique<NumberFormat>(localesBO, options);
    ASSERT_TRUE(numFmtBO != nullptr);
    EXPECT_EQ(numFmtBO->Format(1234567.89), "1,234,567.89 ml/kg/min");
    std::unique_ptr<NumberFormat> numFmtUG = std::make_unique<NumberFormat>(localesUG, options);
    ASSERT_TRUE(numFmtUG != nullptr);
    EXPECT_EQ(numFmtUG->Format(1), "1 ml/kg/min");
    EXPECT_EQ(numFmtUG->Format(1234567.89), "1234567.89 ml/kg/min");
    std::unique_ptr<NumberFormat> numFmtHK = std::make_unique<NumberFormat>(localesHK, options);
    ASSERT_TRUE(numFmtHK != nullptr);
    EXPECT_EQ(numFmtHK->Format(1234567.89), "1,234,567.89 ml/kg/min");
    std::unique_ptr<NumberFormat> numFmtTW = std::make_unique<NumberFormat>(localesTW, options);
    ASSERT_TRUE(numFmtTW != nullptr);
    EXPECT_EQ(numFmtTW->Format(1234567.89), "1,234,567.89 ml/kg/min");
}

/**
 * @tc.name: NumberFormatFuncTest0026
 * @tc.desc: Test Intl NumberFormat.format
 * @tc.type: FUNC
 */
HWTEST_F(NumberFormatTest, NumberFormatFuncTest0026, TestSize.Level1)
{
    string localeCN = "zh-CN";
    vector<string> localesCN{localeCN};
    string localeUS = "en-US";
    vector<string> localesUS{localeUS};
    string localeGB = "en-GB";
    vector<string> localesGB{localeGB};
    string localeBO = "bo";
    vector<string> localesBO{localeBO};
    string localeUG = "ug";
    vector<string> localesUG{localeUG};
    string localeHK = "zh-HK";
    vector<string> localesHK{localeHK};
    string localeTW = "zh-TW";
    vector<string> localesTW{localeTW};
    string style = "unit";
    string unit = "milliliter-per-minute-per-kilogram";
    string unitStyle = "short";
    map<string, string> options = { { "style", style},
                                    { "unit", unit },
                                    { "unitStyle", unitStyle } };
    std::unique_ptr<NumberFormat> numFmtCN = std::make_unique<NumberFormat>(localesCN, options);
    ASSERT_TRUE(numFmtCN != nullptr);
    EXPECT_EQ(numFmtCN->Format(1234567.89), "1,234,567.89 ml/kg/min");
    std::unique_ptr<NumberFormat> numFmtUS = std::make_unique<NumberFormat>(localesUS, options);
    ASSERT_TRUE(numFmtUS != nullptr);
    EXPECT_EQ(numFmtUS->Format(1), "1 ml/kg/min");
    EXPECT_EQ(numFmtUS->Format(1234567.89), "1,234,567.89 ml/kg/min");
    std::unique_ptr<NumberFormat> numFmtGB = std::make_unique<NumberFormat>(localesGB, options);
    ASSERT_TRUE(numFmtGB != nullptr);
    EXPECT_EQ(numFmtGB->Format(1), "1 ml/kg/min");
    EXPECT_EQ(numFmtGB->Format(1234567.89), "1,234,567.89 ml/kg/min");
    std::unique_ptr<NumberFormat> numFmtBO = std::make_unique<NumberFormat>(localesBO, options);
    ASSERT_TRUE(numFmtBO != nullptr);
    EXPECT_EQ(numFmtBO->Format(1234567.89), "1,234,567.89 ml/kg/min");
    std::unique_ptr<NumberFormat> numFmtUG = std::make_unique<NumberFormat>(localesUG, options);
    ASSERT_TRUE(numFmtUG != nullptr);
    EXPECT_EQ(numFmtUG->Format(1), "1 ml/kg/min");
    EXPECT_EQ(numFmtUG->Format(1234567.89), "1234567.89 ml/kg/min");
    std::unique_ptr<NumberFormat> numFmtHK = std::make_unique<NumberFormat>(localesHK, options);
    ASSERT_TRUE(numFmtHK != nullptr);
    EXPECT_EQ(numFmtHK->Format(1234567.89), "1,234,567.89 ml/kg/min");
    std::unique_ptr<NumberFormat> numFmtTW = std::make_unique<NumberFormat>(localesTW, options);
    ASSERT_TRUE(numFmtTW != nullptr);
    EXPECT_EQ(numFmtTW->Format(1234567.89), "1,234,567.89 ml/kg/min");
}

/**
 * @tc.name: NumberFormatFuncTest0027
 * @tc.desc: Test Intl NumberFormat.format
 * @tc.type: FUNC
 */
HWTEST_F(NumberFormatTest, NumberFormatFuncTest0027, TestSize.Level1)
{
    string localeCN = "zh-CN";
    vector<string> localesCN{localeCN};
    string localeUS = "en-US";
    vector<string> localesUS{localeUS};
    string localeGB = "en-GB";
    vector<string> localesGB{localeGB};
    string localeBO = "bo";
    vector<string> localesBO{localeBO};
    string localeUG = "ug";
    vector<string> localesUG{localeUG};
    string localeHK = "zh-HK";
    vector<string> localesHK{localeHK};
    string localeTW = "zh-TW";
    vector<string> localesTW{localeTW};
    string style = "unit";
    string unit = "milliliter-per-minute-per-kilogram";
    string unitStyle = "narrow";
    map<string, string> options = { { "style", style},
                                    { "unit", unit },
                                    { "unitStyle", unitStyle } };
    std::unique_ptr<NumberFormat> numFmtCN = std::make_unique<NumberFormat>(localesCN, options);
    ASSERT_TRUE(numFmtCN != nullptr);
    EXPECT_EQ(numFmtCN->Format(1234567.89), "1,234,567.89 ml/kg/min");
    std::unique_ptr<NumberFormat> numFmtUS = std::make_unique<NumberFormat>(localesUS, options);
    ASSERT_TRUE(numFmtUS != nullptr);
    EXPECT_EQ(numFmtUS->Format(1), "1 ml/kg/min");
    EXPECT_EQ(numFmtUS->Format(1234567.89), "1,234,567.89 ml/kg/min");
    std::unique_ptr<NumberFormat> numFmtGB = std::make_unique<NumberFormat>(localesGB, options);
    ASSERT_TRUE(numFmtGB != nullptr);
    EXPECT_EQ(numFmtGB->Format(1), "1 ml/kg/min");
    EXPECT_EQ(numFmtGB->Format(1234567.89), "1,234,567.89 ml/kg/min");
    std::unique_ptr<NumberFormat> numFmtBO = std::make_unique<NumberFormat>(localesBO, options);
    ASSERT_TRUE(numFmtBO != nullptr);
    EXPECT_EQ(numFmtBO->Format(1234567.89), "1,234,567.89 ml/kg/min");
    std::unique_ptr<NumberFormat> numFmtUG = std::make_unique<NumberFormat>(localesUG, options);
    ASSERT_TRUE(numFmtUG != nullptr);
    EXPECT_EQ(numFmtUG->Format(1), "1 ml/kg/min");
    EXPECT_EQ(numFmtUG->Format(1234567.89), "1234567.89 ml/kg/min");
    std::unique_ptr<NumberFormat> numFmtHK = std::make_unique<NumberFormat>(localesHK, options);
    ASSERT_TRUE(numFmtHK != nullptr);
    EXPECT_EQ(numFmtHK->Format(1234567.89), "1,234,567.89 ml/kg/min");
    std::unique_ptr<NumberFormat> numFmtTW = std::make_unique<NumberFormat>(localesTW, options);
    ASSERT_TRUE(numFmtTW != nullptr);
    EXPECT_EQ(numFmtTW->Format(1234567.89), "1,234,567.89 ml/kg/min");
}

/**
 * @tc.name: NumberFormatFuncTest0028
 * @tc.desc: Test Intl NumberFormat.format
 * @tc.type: FUNC
 */
HWTEST_F(NumberFormatTest, NumberFormatFuncTest0028, TestSize.Level1)
{
    string localeCN = "zh-CN";
    vector<string> localesCN{localeCN};
    string localeUS = "en-US";
    vector<string> localesUS{localeUS};
    string localeGB = "en-GB";
    vector<string> localesGB{localeGB};
    string localeBO = "bo";
    vector<string> localesBO{localeBO};
    string localeUG = "ug";
    vector<string> localesUG{localeUG};
    string localeHK = "zh-HK";
    vector<string> localesHK{localeHK};
    string localeTW = "zh-TW";
    vector<string> localesTW{localeTW};
    string style = "unit";
    string unit = "rotation-per-minute";
    string unitStyle = "long";
    map<string, string> options = { { "style", style},
                                    { "unit", unit },
                                    { "unitStyle", unitStyle } };
    std::unique_ptr<NumberFormat> numFmtCN = std::make_unique<NumberFormat>(localesCN, options);
    ASSERT_TRUE(numFmtCN != nullptr);
    EXPECT_EQ(numFmtCN->Format(1234567.89), "1,234,567.89 转/分钟");
    std::unique_ptr<NumberFormat> numFmtUS = std::make_unique<NumberFormat>(localesUS, options);
    ASSERT_TRUE(numFmtUS != nullptr);
    EXPECT_EQ(numFmtUS->Format(1), "1 rpm");
    EXPECT_EQ(numFmtUS->Format(1234567.89), "1,234,567.89 rpm");
    std::unique_ptr<NumberFormat> numFmtGB = std::make_unique<NumberFormat>(localesGB, options);
    ASSERT_TRUE(numFmtGB != nullptr);
    EXPECT_EQ(numFmtGB->Format(1), "1 rpm");
    EXPECT_EQ(numFmtGB->Format(1234567.89), "1,234,567.89 rpm");
    std::unique_ptr<NumberFormat> numFmtBO = std::make_unique<NumberFormat>(localesBO, options);
    ASSERT_TRUE(numFmtBO != nullptr);
    EXPECT_EQ(numFmtBO->Format(1234567.89), "སྐར་མ་རེར་སྐོར་བ་ 1,234,567.89");
    std::unique_ptr<NumberFormat> numFmtUG = std::make_unique<NumberFormat>(localesUG, options);
    ASSERT_TRUE(numFmtUG != nullptr);
    EXPECT_EQ(numFmtUG->Format(1), "1 ق/م");
    EXPECT_EQ(numFmtUG->Format(1234567.89), "1234567.89 ق/م");
    std::unique_ptr<NumberFormat> numFmtHK = std::make_unique<NumberFormat>(localesHK, options);
    ASSERT_TRUE(numFmtHK != nullptr);
    EXPECT_EQ(numFmtHK->Format(1234567.89), "1,234,567.89 轉/分鐘");
    std::unique_ptr<NumberFormat> numFmtTW = std::make_unique<NumberFormat>(localesTW, options);
    ASSERT_TRUE(numFmtTW != nullptr);
    EXPECT_EQ(numFmtTW->Format(1234567.89), "1,234,567.89 轉/分鐘");
}

/**
 * @tc.name: NumberFormatFuncTest0029
 * @tc.desc: Test Intl NumberFormat.format
 * @tc.type: FUNC
 */
HWTEST_F(NumberFormatTest, NumberFormatFuncTest0029, TestSize.Level1)
{
    string localeCN = "zh-CN";
    vector<string> localesCN{localeCN};
    string localeUS = "en-US";
    vector<string> localesUS{localeUS};
    string localeGB = "en-GB";
    vector<string> localesGB{localeGB};
    string localeBO = "bo";
    vector<string> localesBO{localeBO};
    string localeUG = "ug";
    vector<string> localesUG{localeUG};
    string localeHK = "zh-HK";
    vector<string> localesHK{localeHK};
    string localeTW = "zh-TW";
    vector<string> localesTW{localeTW};
    string style = "unit";
    string unit = "rotation-per-minute";
    string unitStyle = "short";
    map<string, string> options = { { "style", style},
                                    { "unit", unit },
                                    { "unitStyle", unitStyle } };
    std::unique_ptr<NumberFormat> numFmtCN = std::make_unique<NumberFormat>(localesCN, options);
    ASSERT_TRUE(numFmtCN != nullptr);
    EXPECT_EQ(numFmtCN->Format(1234567.89), "1,234,567.89 转/分钟");
    std::unique_ptr<NumberFormat> numFmtUS = std::make_unique<NumberFormat>(localesUS, options);
    ASSERT_TRUE(numFmtUS != nullptr);
    EXPECT_EQ(numFmtUS->Format(1), "1 rpm");
    EXPECT_EQ(numFmtUS->Format(1234567.89), "1,234,567.89 rpm");
    std::unique_ptr<NumberFormat> numFmtGB = std::make_unique<NumberFormat>(localesGB, options);
    ASSERT_TRUE(numFmtGB != nullptr);
    EXPECT_EQ(numFmtGB->Format(1), "1 rpm");
    EXPECT_EQ(numFmtGB->Format(1234567.89), "1,234,567.89 rpm");
    std::unique_ptr<NumberFormat> numFmtBO = std::make_unique<NumberFormat>(localesBO, options);
    ASSERT_TRUE(numFmtBO != nullptr);
    EXPECT_EQ(numFmtBO->Format(1234567.89), "སྐར་མ་རེར་སྐོར་བ་ 1,234,567.89");
    std::unique_ptr<NumberFormat> numFmtUG = std::make_unique<NumberFormat>(localesUG, options);
    ASSERT_TRUE(numFmtUG != nullptr);
    EXPECT_EQ(numFmtUG->Format(1), "1 ق/م");
    EXPECT_EQ(numFmtUG->Format(1234567.89), "1234567.89 ق/م");
    std::unique_ptr<NumberFormat> numFmtHK = std::make_unique<NumberFormat>(localesHK, options);
    ASSERT_TRUE(numFmtHK != nullptr);
    EXPECT_EQ(numFmtHK->Format(1234567.89), "1,234,567.89 轉/分鐘");
    std::unique_ptr<NumberFormat> numFmtTW = std::make_unique<NumberFormat>(localesTW, options);
    ASSERT_TRUE(numFmtTW != nullptr);
    EXPECT_EQ(numFmtTW->Format(1234567.89), "1,234,567.89 轉/分鐘");
}

/**
 * @tc.name: NumberFormatFuncTest0030
 * @tc.desc: Test Intl NumberFormat.format
 * @tc.type: FUNC
 */
HWTEST_F(NumberFormatTest, NumberFormatFuncTest0030, TestSize.Level1)
{
    string localeCN = "zh-CN";
    vector<string> localesCN{localeCN};
    string localeUS = "en-US";
    vector<string> localesUS{localeUS};
    string localeGB = "en-GB";
    vector<string> localesGB{localeGB};
    string localeBO = "bo";
    vector<string> localesBO{localeBO};
    string localeUG = "ug";
    vector<string> localesUG{localeUG};
    string localeHK = "zh-HK";
    vector<string> localesHK{localeHK};
    string localeTW = "zh-TW";
    vector<string> localesTW{localeTW};
    string style = "unit";
    string unit = "rotation-per-minute";
    string unitStyle = "narrow";
    map<string, string> options = { { "style", style},
                                    { "unit", unit },
                                    { "unitStyle", unitStyle } };
    std::unique_ptr<NumberFormat> numFmtCN = std::make_unique<NumberFormat>(localesCN, options);
    ASSERT_TRUE(numFmtCN != nullptr);
    EXPECT_EQ(numFmtCN->Format(1234567.89), "1,234,567.89 转/分钟");
    std::unique_ptr<NumberFormat> numFmtUS = std::make_unique<NumberFormat>(localesUS, options);
    ASSERT_TRUE(numFmtUS != nullptr);
    EXPECT_EQ(numFmtUS->Format(1), "1 rpm");
    EXPECT_EQ(numFmtUS->Format(1234567.89), "1,234,567.89 rpm");
    std::unique_ptr<NumberFormat> numFmtGB = std::make_unique<NumberFormat>(localesGB, options);
    ASSERT_TRUE(numFmtGB != nullptr);
    EXPECT_EQ(numFmtGB->Format(1), "1 rpm");
    EXPECT_EQ(numFmtGB->Format(1234567.89), "1,234,567.89 rpm");
    std::unique_ptr<NumberFormat> numFmtBO = std::make_unique<NumberFormat>(localesBO, options);
    ASSERT_TRUE(numFmtBO != nullptr);
    EXPECT_EQ(numFmtBO->Format(1234567.89), "སྐར་མ་རེར་སྐོར་བ་ 1,234,567.89");
    std::unique_ptr<NumberFormat> numFmtUG = std::make_unique<NumberFormat>(localesUG, options);
    ASSERT_TRUE(numFmtUG != nullptr);
    EXPECT_EQ(numFmtUG->Format(1), "1 ق/م");
    EXPECT_EQ(numFmtUG->Format(1234567.89), "1234567.89 ق/م");
    std::unique_ptr<NumberFormat> numFmtHK = std::make_unique<NumberFormat>(localesHK, options);
    ASSERT_TRUE(numFmtHK != nullptr);
    EXPECT_EQ(numFmtHK->Format(1234567.89), "1,234,567.89 轉/分鐘");
    std::unique_ptr<NumberFormat> numFmtTW = std::make_unique<NumberFormat>(localesTW, options);
    ASSERT_TRUE(numFmtTW != nullptr);
    EXPECT_EQ(numFmtTW->Format(1234567.89), "1,234,567.89 轉/分鐘");
}

/**
 * @tc.name: NumberFormatFuncTest0031
 * @tc.desc: Test Intl NumberFormat.format
 * @tc.type: FUNC
 */
HWTEST_F(NumberFormatTest, NumberFormatFuncTest0031, TestSize.Level1)
{
    string localeCN = "zh-CN";
    vector<string> localesCN{localeCN};
    string localeUS = "en-US";
    vector<string> localesUS{localeUS};
    string localeGB = "en-GB";
    vector<string> localesGB{localeGB};
    string localeBO = "bo";
    vector<string> localesBO{localeBO};
    string localeUG = "ug";
    vector<string> localesUG{localeUG};
    string localeHK = "zh-HK";
    vector<string> localesHK{localeHK};
    string localeTW = "zh-TW";
    vector<string> localesTW{localeTW};
    string style = "unit";
    string unit = "step-per-minute";
    string unitStyle = "long";
    map<string, string> options = { { "style", style},
                                    { "unit", unit },
                                    { "unitStyle", unitStyle } };
    std::unique_ptr<NumberFormat> numFmtCN = std::make_unique<NumberFormat>(localesCN, options);
    ASSERT_TRUE(numFmtCN != nullptr);
    EXPECT_EQ(numFmtCN->Format(1234567.89), "1,234,567.89 步/分钟");
    std::unique_ptr<NumberFormat> numFmtUS = std::make_unique<NumberFormat>(localesUS, options);
    ASSERT_TRUE(numFmtUS != nullptr);
    EXPECT_EQ(numFmtUS->Format(1), "1 step/min");
    EXPECT_EQ(numFmtUS->Format(1234567.89), "1,234,567.89 steps/min");
    std::unique_ptr<NumberFormat> numFmtGB = std::make_unique<NumberFormat>(localesGB, options);
    ASSERT_TRUE(numFmtGB != nullptr);
    EXPECT_EQ(numFmtGB->Format(1), "1 step/min");
    EXPECT_EQ(numFmtGB->Format(1234567.89), "1,234,567.89 steps/min");
    std::unique_ptr<NumberFormat> numFmtBO = std::make_unique<NumberFormat>(localesBO, options);
    ASSERT_TRUE(numFmtBO != nullptr);
    EXPECT_EQ(numFmtBO->Format(1234567.89), "སྐར་མ་རེར་གོམ་པ་ 1,234,567.89");
    std::unique_ptr<NumberFormat> numFmtUG = std::make_unique<NumberFormat>(localesUG, options);
    ASSERT_TRUE(numFmtUG != nullptr);
    EXPECT_EQ(numFmtUG->Format(1), "%1 ق/م");
    EXPECT_EQ(numFmtUG->Format(1234567.89), "1234567.89 ق/م");
    std::unique_ptr<NumberFormat> numFmtHK = std::make_unique<NumberFormat>(localesHK, options);
    ASSERT_TRUE(numFmtHK != nullptr);
    EXPECT_EQ(numFmtHK->Format(1234567.89), "1,234,567.89 步/分鐘");
    std::unique_ptr<NumberFormat> numFmtTW = std::make_unique<NumberFormat>(localesTW, options);
    ASSERT_TRUE(numFmtTW != nullptr);
    EXPECT_EQ(numFmtTW->Format(1234567.89), "1,234,567.89 步/分鐘");
}

/**
 * @tc.name: NumberFormatFuncTest0032
 * @tc.desc: Test Intl NumberFormat.format
 * @tc.type: FUNC
 */
HWTEST_F(NumberFormatTest, NumberFormatFuncTest0032, TestSize.Level1)
{
    string localeCN = "zh-CN";
    vector<string> localesCN{localeCN};
    string localeUS = "en-US";
    vector<string> localesUS{localeUS};
    string localeGB = "en-GB";
    vector<string> localesGB{localeGB};
    string localeBO = "bo";
    vector<string> localesBO{localeBO};
    string localeUG = "ug";
    vector<string> localesUG{localeUG};
    string localeHK = "zh-HK";
    vector<string> localesHK{localeHK};
    string localeTW = "zh-TW";
    vector<string> localesTW{localeTW};
    string style = "unit";
    string unit = "step-per-minute";
    string unitStyle = "short";
    map<string, string> options = { { "style", style},
                                    { "unit", unit },
                                    { "unitStyle", unitStyle } };
    std::unique_ptr<NumberFormat> numFmtCN = std::make_unique<NumberFormat>(localesCN, options);
    ASSERT_TRUE(numFmtCN != nullptr);
    EXPECT_EQ(numFmtCN->Format(1234567.89), "1,234,567.89 步/分钟");
    std::unique_ptr<NumberFormat> numFmtUS = std::make_unique<NumberFormat>(localesUS, options);
    ASSERT_TRUE(numFmtUS != nullptr);
    EXPECT_EQ(numFmtUS->Format(1), "1 step/min");
    EXPECT_EQ(numFmtUS->Format(1234567.89), "1,234,567.89 steps/min");
    std::unique_ptr<NumberFormat> numFmtGB = std::make_unique<NumberFormat>(localesGB, options);
    ASSERT_TRUE(numFmtGB != nullptr);
    EXPECT_EQ(numFmtGB->Format(1), "1 step/min");
    EXPECT_EQ(numFmtGB->Format(1234567.89), "1,234,567.89 steps/min");
    std::unique_ptr<NumberFormat> numFmtBO = std::make_unique<NumberFormat>(localesBO, options);
    ASSERT_TRUE(numFmtBO != nullptr);
    EXPECT_EQ(numFmtBO->Format(1234567.89), "སྐར་མ་རེར་གོམ་པ་ 1,234,567.89");
    std::unique_ptr<NumberFormat> numFmtUG = std::make_unique<NumberFormat>(localesUG, options);
    ASSERT_TRUE(numFmtUG != nullptr);
    EXPECT_EQ(numFmtUG->Format(1), "%1 ق/م");
    EXPECT_EQ(numFmtUG->Format(1234567.89), "1234567.89 ق/م");
    std::unique_ptr<NumberFormat> numFmtHK = std::make_unique<NumberFormat>(localesHK, options);
    ASSERT_TRUE(numFmtHK != nullptr);
    EXPECT_EQ(numFmtHK->Format(1234567.89), "1,234,567.89 步/分鐘");
    std::unique_ptr<NumberFormat> numFmtTW = std::make_unique<NumberFormat>(localesTW, options);
    ASSERT_TRUE(numFmtTW != nullptr);
    EXPECT_EQ(numFmtTW->Format(1234567.89), "1,234,567.89 步/分鐘");
}

/**
 * @tc.name: NumberFormatFuncTest0033
 * @tc.desc: Test Intl NumberFormat.format
 * @tc.type: FUNC
 */
HWTEST_F(NumberFormatTest, NumberFormatFuncTest0033, TestSize.Level1)
{
    string localeCN = "zh-CN";
    vector<string> localesCN{localeCN};
    string localeUS = "en-US";
    vector<string> localesUS{localeUS};
    string localeGB = "en-GB";
    vector<string> localesGB{localeGB};
    string localeBO = "bo";
    vector<string> localesBO{localeBO};
    string localeUG = "ug";
    vector<string> localesUG{localeUG};
    string localeHK = "zh-HK";
    vector<string> localesHK{localeHK};
    string localeTW = "zh-TW";
    vector<string> localesTW{localeTW};
    string style = "unit";
    string unit = "step-per-minute";
    string unitStyle = "narrow";
    map<string, string> options = { { "style", style},
                                    { "unit", unit },
                                    { "unitStyle", unitStyle } };
    std::unique_ptr<NumberFormat> numFmtCN = std::make_unique<NumberFormat>(localesCN, options);
    ASSERT_TRUE(numFmtCN != nullptr);
    EXPECT_EQ(numFmtCN->Format(1234567.89), "1,234,567.89 步/分钟");
    std::unique_ptr<NumberFormat> numFmtUS = std::make_unique<NumberFormat>(localesUS, options);
    ASSERT_TRUE(numFmtUS != nullptr);
    EXPECT_EQ(numFmtUS->Format(1), "1 step/min");
    EXPECT_EQ(numFmtUS->Format(1234567.89), "1,234,567.89 steps/min");
    std::unique_ptr<NumberFormat> numFmtGB = std::make_unique<NumberFormat>(localesGB, options);
    ASSERT_TRUE(numFmtGB != nullptr);
    EXPECT_EQ(numFmtGB->Format(1), "1 step/min");
    EXPECT_EQ(numFmtGB->Format(1234567.89), "1,234,567.89 steps/min");
    std::unique_ptr<NumberFormat> numFmtBO = std::make_unique<NumberFormat>(localesBO, options);
    ASSERT_TRUE(numFmtBO != nullptr);
    EXPECT_EQ(numFmtBO->Format(1234567.89), "སྐར་མ་རེར་གོམ་པ་ 1,234,567.89");
    std::unique_ptr<NumberFormat> numFmtUG = std::make_unique<NumberFormat>(localesUG, options);
    ASSERT_TRUE(numFmtUG != nullptr);
    EXPECT_EQ(numFmtUG->Format(1), "%1 ق/م");
    EXPECT_EQ(numFmtUG->Format(1234567.89), "1234567.89 ق/م");
    std::unique_ptr<NumberFormat> numFmtHK = std::make_unique<NumberFormat>(localesHK, options);
    ASSERT_TRUE(numFmtHK != nullptr);
    EXPECT_EQ(numFmtHK->Format(1234567.89), "1,234,567.89 步/分鐘");
    std::unique_ptr<NumberFormat> numFmtTW = std::make_unique<NumberFormat>(localesTW, options);
    ASSERT_TRUE(numFmtTW != nullptr);
    EXPECT_EQ(numFmtTW->Format(1234567.89), "1,234,567.89 步/分鐘");
}

/**
 * @tc.name: NumberFormatFuncTest0034
 * @tc.desc: Test Intl NumberFormat.format
 * @tc.type: FUNC
 */
HWTEST_F(NumberFormatTest, NumberFormatFuncTest0034, TestSize.Level1)
{
    string localeCN = "zh-CN";
    vector<string> localesCN{localeCN};
    string localeUS = "en-US";
    vector<string> localesUS{localeUS};
    string localeGB = "en-GB";
    vector<string> localesGB{localeGB};
    string localeBO = "bo";
    vector<string> localesBO{localeBO};
    string localeUG = "ug";
    vector<string> localesUG{localeUG};
    string localeHK = "zh-HK";
    vector<string> localesHK{localeHK};
    string localeTW = "zh-TW";
    vector<string> localesTW{localeTW};
    string style = "unit";
    string unit = "stroke-per-minute";
    string unitStyle = "long";
    map<string, string> options = { { "style", style},
                                    { "unit", unit },
                                    { "unitStyle", unitStyle } };
    std::unique_ptr<NumberFormat> numFmtCN = std::make_unique<NumberFormat>(localesCN, options);
    ASSERT_TRUE(numFmtCN != nullptr);
    EXPECT_EQ(numFmtCN->Format(1234567.89), "1,234,567.89 次/分钟");
    std::unique_ptr<NumberFormat> numFmtUS = std::make_unique<NumberFormat>(localesUS, options);
    ASSERT_TRUE(numFmtUS != nullptr);
    EXPECT_EQ(numFmtUS->Format(1), "1 stroke/min");
    EXPECT_EQ(numFmtUS->Format(1234567.89), "1,234,567.89 strokes/min");
    std::unique_ptr<NumberFormat> numFmtGB = std::make_unique<NumberFormat>(localesGB, options);
    ASSERT_TRUE(numFmtGB != nullptr);
    EXPECT_EQ(numFmtGB->Format(1), "1 stroke/min");
    EXPECT_EQ(numFmtGB->Format(1234567.89), "1,234,567.89 strokes/min");
    std::unique_ptr<NumberFormat> numFmtBO = std::make_unique<NumberFormat>(localesBO, options);
    ASSERT_TRUE(numFmtBO != nullptr);
    EXPECT_EQ(numFmtBO->Format(1234567.89), "སྐར་མ་རེར་ཐེངས་ 1,234,567.89");
    std::unique_ptr<NumberFormat> numFmtUG = std::make_unique<NumberFormat>(localesUG, options);
    ASSERT_TRUE(numFmtUG != nullptr);
    EXPECT_EQ(numFmtUG->Format(1), "1 پالاق/مىنۇت");
    EXPECT_EQ(numFmtUG->Format(1234567.89), "1234567.89 پالاق/مىنۇت");
    std::unique_ptr<NumberFormat> numFmtHK = std::make_unique<NumberFormat>(localesHK, options);
    ASSERT_TRUE(numFmtHK != nullptr);
    EXPECT_EQ(numFmtHK->Format(1234567.89), "1,234,567.89 下/分鐘");
    std::unique_ptr<NumberFormat> numFmtTW = std::make_unique<NumberFormat>(localesTW, options);
    ASSERT_TRUE(numFmtTW != nullptr);
    EXPECT_EQ(numFmtTW->Format(1234567.89), "1,234,567.89 次/分鐘");
}

/**
 * @tc.name: NumberFormatFuncTest0035
 * @tc.desc: Test Intl NumberFormat.format
 * @tc.type: FUNC
 */
HWTEST_F(NumberFormatTest, NumberFormatFuncTest0035, TestSize.Level1)
{
    string localeCN = "zh-CN";
    vector<string> localesCN{localeCN};
    string localeUS = "en-US";
    vector<string> localesUS{localeUS};
    string localeGB = "en-GB";
    vector<string> localesGB{localeGB};
    string localeBO = "bo";
    vector<string> localesBO{localeBO};
    string localeUG = "ug";
    vector<string> localesUG{localeUG};
    string localeHK = "zh-HK";
    vector<string> localesHK{localeHK};
    string localeTW = "zh-TW";
    vector<string> localesTW{localeTW};
    string style = "unit";
    string unit = "stroke-per-minute";
    string unitStyle = "short";
    map<string, string> options = { { "style", style},
                                    { "unit", unit },
                                    { "unitStyle", unitStyle } };
    std::unique_ptr<NumberFormat> numFmtCN = std::make_unique<NumberFormat>(localesCN, options);
    ASSERT_TRUE(numFmtCN != nullptr);
    EXPECT_EQ(numFmtCN->Format(1234567.89), "1,234,567.89 次/分钟");
    std::unique_ptr<NumberFormat> numFmtUS = std::make_unique<NumberFormat>(localesUS, options);
    ASSERT_TRUE(numFmtUS != nullptr);
    EXPECT_EQ(numFmtUS->Format(1), "1 stroke/min");
    EXPECT_EQ(numFmtUS->Format(1234567.89), "1,234,567.89 strokes/min");
    std::unique_ptr<NumberFormat> numFmtGB = std::make_unique<NumberFormat>(localesGB, options);
    ASSERT_TRUE(numFmtGB != nullptr);
    EXPECT_EQ(numFmtGB->Format(1), "1 stroke/min");
    EXPECT_EQ(numFmtGB->Format(1234567.89), "1,234,567.89 strokes/min");
    std::unique_ptr<NumberFormat> numFmtBO = std::make_unique<NumberFormat>(localesBO, options);
    ASSERT_TRUE(numFmtBO != nullptr);
    EXPECT_EQ(numFmtBO->Format(1234567.89), "སྐར་མ་རེར་ཐེངས་ 1,234,567.89");
    std::unique_ptr<NumberFormat> numFmtUG = std::make_unique<NumberFormat>(localesUG, options);
    ASSERT_TRUE(numFmtUG != nullptr);
    EXPECT_EQ(numFmtUG->Format(1), "1 پالاق/مىنۇت");
    EXPECT_EQ(numFmtUG->Format(1234567.89), "1234567.89 پالاق/مىنۇت");
    std::unique_ptr<NumberFormat> numFmtHK = std::make_unique<NumberFormat>(localesHK, options);
    ASSERT_TRUE(numFmtHK != nullptr);
    EXPECT_EQ(numFmtHK->Format(1234567.89), "1,234,567.89 下/分鐘");
    std::unique_ptr<NumberFormat> numFmtTW = std::make_unique<NumberFormat>(localesTW, options);
    ASSERT_TRUE(numFmtTW != nullptr);
    EXPECT_EQ(numFmtTW->Format(1234567.89), "1,234,567.89 次/分鐘");
}

/**
 * @tc.name: NumberFormatFuncTest0036
 * @tc.desc: Test Intl NumberFormat.format
 * @tc.type: FUNC
 */
HWTEST_F(NumberFormatTest, NumberFormatFuncTest0036, TestSize.Level1)
{
    string localeCN = "zh-CN";
    vector<string> localesCN{localeCN};
    string localeUS = "en-US";
    vector<string> localesUS{localeUS};
    string localeGB = "en-GB";
    vector<string> localesGB{localeGB};
    string localeBO = "bo";
    vector<string> localesBO{localeBO};
    string localeUG = "ug";
    vector<string> localesUG{localeUG};
    string localeHK = "zh-HK";
    vector<string> localesHK{localeHK};
    string localeTW = "zh-TW";
    vector<string> localesTW{localeTW};
    string style = "unit";
    string unit = "stroke-per-minute";
    string unitStyle = "narrow";
    map<string, string> options = { { "style", style},
                                    { "unit", unit },
                                    { "unitStyle", unitStyle } };
    std::unique_ptr<NumberFormat> numFmtCN = std::make_unique<NumberFormat>(localesCN, options);
    ASSERT_TRUE(numFmtCN != nullptr);
    EXPECT_EQ(numFmtCN->Format(1234567.89), "1,234,567.89 次/分钟");
    std::unique_ptr<NumberFormat> numFmtUS = std::make_unique<NumberFormat>(localesUS, options);
    ASSERT_TRUE(numFmtUS != nullptr);
    EXPECT_EQ(numFmtUS->Format(1), "1 stroke/min");
    EXPECT_EQ(numFmtUS->Format(1234567.89), "1,234,567.89 strokes/min");
    std::unique_ptr<NumberFormat> numFmtGB = std::make_unique<NumberFormat>(localesGB, options);
    ASSERT_TRUE(numFmtGB != nullptr);
    EXPECT_EQ(numFmtGB->Format(1), "1 stroke/min");
    EXPECT_EQ(numFmtGB->Format(1234567.89), "1,234,567.89 strokes/min");
    std::unique_ptr<NumberFormat> numFmtBO = std::make_unique<NumberFormat>(localesBO, options);
    ASSERT_TRUE(numFmtBO != nullptr);
    EXPECT_EQ(numFmtBO->Format(1234567.89), "སྐར་མ་རེར་ཐེངས་ 1,234,567.89");
    std::unique_ptr<NumberFormat> numFmtUG = std::make_unique<NumberFormat>(localesUG, options);
    ASSERT_TRUE(numFmtUG != nullptr);
    EXPECT_EQ(numFmtUG->Format(1), "1 پالاق/مىنۇت");
    EXPECT_EQ(numFmtUG->Format(1234567.89), "1234567.89 پالاق/مىنۇت");
    std::unique_ptr<NumberFormat> numFmtHK = std::make_unique<NumberFormat>(localesHK, options);
    ASSERT_TRUE(numFmtHK != nullptr);
    EXPECT_EQ(numFmtHK->Format(1234567.89), "1,234,567.89 下/分鐘");
    std::unique_ptr<NumberFormat> numFmtTW = std::make_unique<NumberFormat>(localesTW, options);
    ASSERT_TRUE(numFmtTW != nullptr);
    EXPECT_EQ(numFmtTW->Format(1234567.89), "1,234,567.89 次/分鐘");
}
} // namespace I18n
} // namespace Global
} // namespace OHOS