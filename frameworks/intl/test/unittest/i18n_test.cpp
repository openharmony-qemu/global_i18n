/*
 * Copyright (c) 2022 Huawei Device Co., Ltd.
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

#include <gtest/gtest.h>
#include "i18n_break_iterator.h"
#include "i18n_calendar.h"
#include "i18n_normalizer.h"
#include "i18n_timezone.h"
#include "locale_config.h"
#include "measure_data.h"
#include "phone_number_format.h"
#include "preferred_language.h"

using namespace OHOS::Global::I18n;
using testing::ext::TestSize;
using namespace std;

namespace OHOS {
namespace Global {
namespace I18n {
class I18nTest : public testing::Test {
public:
    static void SetUpTestCase(void);
    static void TearDownTestCase(void);
    void SetUp();
    void TearDown();
};

void I18nTest::SetUpTestCase(void)
{}

void I18nTest::TearDownTestCase(void)
{}

void I18nTest::SetUp(void)
{}

void I18nTest::TearDown(void)
{}

/**
 * @tc.name: I18nFuncTest001
 * @tc.desc: Test I18n PreferredLanguage GetPreferredLocale
 * @tc.type: FUNC
 */
HWTEST_F(I18nTest, I18nFuncTest001, TestSize.Level1)
{
    string preferredLocale = PreferredLanguage::GetPreferredLocale();
    EXPECT_TRUE(preferredLocale.size() > 0);
    string systemLocale = "zh-Hans-CN";
    if (LocaleConfig::SetSystemLocale(systemLocale)) {
        I18nErrorCode errorCode = I18nErrorCode::SUCCESS;
        PreferredLanguage::AddPreferredLanguage("en-US", 0, errorCode);
        if (errorCode == I18nErrorCode::SUCCESS) {
            preferredLocale = PreferredLanguage::GetPreferredLocale();
            EXPECT_EQ(preferredLocale, "en-CN");
        }
    }
}

/**
 * @tc.name: I18nFuncTest002
 * @tc.desc: Test I18n Normalizer
 * @tc.type: FUNC
 */
HWTEST_F(I18nTest, I18nFuncTest002, TestSize.Level1)
{
    I18nErrorCode errorCode = I18nErrorCode::SUCCESS;
    I18nNormalizer normalizer(I18nNormalizerMode::NFD, errorCode);
    EXPECT_EQ(errorCode, I18nErrorCode::SUCCESS);

    string text = "\uFB01"; // \uFB01 is ﬁ
    string normalizedText = normalizer.Normalize(text.c_str(), text.length(), errorCode);
    EXPECT_EQ(errorCode, I18nErrorCode::SUCCESS);
    EXPECT_EQ(normalizedText.length(), 3); // 3 is the NFD normalized length of ﬁ.
    EXPECT_EQ(normalizedText, "\uFB01");

    text = "\u0032\u2075";
    normalizedText = normalizer.Normalize(text.c_str(), text.length(), errorCode);
    EXPECT_EQ(errorCode, I18nErrorCode::SUCCESS);
    EXPECT_EQ(normalizedText.length(), 4); // 4 is the expected normalized text length.
    EXPECT_EQ(normalizedText, "\u0032\u2075");

    text = "\u1E9B\u0323";
    normalizedText = normalizer.Normalize(text.c_str(), text.length(), errorCode);
    EXPECT_EQ(errorCode, I18nErrorCode::SUCCESS);
    EXPECT_EQ(normalizedText.length(), 6); // 6 is the expected normalized text length.
    EXPECT_EQ(normalizedText, "\u017F\u0323\u0307");
}

/**
 * @tc.name: I18nFuncTest003
 * @tc.desc: Test I18n Normalizer
 * @tc.type: FUNC
 */
HWTEST_F(I18nTest, I18nFuncTest003, TestSize.Level1)
{
    I18nErrorCode errorCode = I18nErrorCode::SUCCESS;
    I18nNormalizer normalizer(I18nNormalizerMode::NFC, errorCode);
    EXPECT_EQ(errorCode, I18nErrorCode::SUCCESS);

    string text = "\uFB01"; // \uFB01 is ﬁ
    string normalizedText = normalizer.Normalize(text.c_str(), text.length(), errorCode);
    EXPECT_EQ(errorCode, I18nErrorCode::SUCCESS);
    EXPECT_EQ(normalizedText.length(), 3); // 3 is the NFC normalized length of ﬁ.
    EXPECT_EQ(normalizedText, "\uFB01");

    text = "\u0032\u2075";
    normalizedText = normalizer.Normalize(text.c_str(), text.length(), errorCode);
    EXPECT_EQ(errorCode, I18nErrorCode::SUCCESS);
    EXPECT_EQ(normalizedText.length(), 4); // 4 is the expected normalized text length.
    EXPECT_EQ(normalizedText, "\u0032\u2075");

    text = "\u1E9B\u0323";
    normalizedText = normalizer.Normalize(text.c_str(), text.length(), errorCode);
    EXPECT_EQ(errorCode, I18nErrorCode::SUCCESS);
    EXPECT_EQ(normalizedText.length(), 5); // 5 is the expected normalized text length.
    EXPECT_EQ(normalizedText, "\u1E9B\u0323");
}

/**
 * @tc.name: I18nFuncTest004
 * @tc.desc: Test I18n Normalizer
 * @tc.type: FUNC
 */
HWTEST_F(I18nTest, I18nFuncTest004, TestSize.Level1)
{
    I18nErrorCode errorCode = I18nErrorCode::SUCCESS;
    I18nNormalizer normalizer(I18nNormalizerMode::NFKD, errorCode);
    EXPECT_EQ(errorCode, I18nErrorCode::SUCCESS);

    string text = "\uFB01"; // \uFB01 is ﬁ
    string normalizedText = normalizer.Normalize(text.c_str(), text.length(), errorCode);
    EXPECT_EQ(errorCode, I18nErrorCode::SUCCESS);
    EXPECT_EQ(normalizedText.length(), 2); // 2 is the NFKD normalized length of ﬁ.
    EXPECT_EQ(normalizedText, "\u0066\u0069");

    text = "\u0032\u2075";
    normalizedText = normalizer.Normalize(text.c_str(), text.length(), errorCode);
    EXPECT_EQ(errorCode, I18nErrorCode::SUCCESS);
    EXPECT_EQ(normalizedText.length(), 2); // 2 is the expected normalized text length.
    EXPECT_EQ(normalizedText, "\u0032\u0035");

    text = "\u1E9B\u0323";
    normalizedText = normalizer.Normalize(text.c_str(), text.length(), errorCode);
    EXPECT_EQ(errorCode, I18nErrorCode::SUCCESS);
    EXPECT_EQ(normalizedText.length(), 5); // 5 is the expected normalized text length.
    EXPECT_EQ(normalizedText, "\u0073\u0323\u0307");
}

/**
 * @tc.name: I18nFuncTest005
 * @tc.desc: Test I18n Normalizer
 * @tc.type: FUNC
 */
HWTEST_F(I18nTest, I18nFuncTest005, TestSize.Level1)
{
    I18nErrorCode errorCode = I18nErrorCode::SUCCESS;
    I18nNormalizer normalizer(I18nNormalizerMode::NFKC, errorCode);
    EXPECT_EQ(errorCode, I18nErrorCode::SUCCESS);

    string text = "\uFB01"; // \uFB01 is ﬁ
    string normalizedText = normalizer.Normalize(text.c_str(), text.length(), errorCode);
    EXPECT_EQ(errorCode, I18nErrorCode::SUCCESS);
    EXPECT_EQ(normalizedText.length(), 2); // 2 is the NFKC normalized length of ﬁ.
    EXPECT_EQ(normalizedText, "\u0066\u0069");

    text = "\u0032\u2075";
    normalizedText = normalizer.Normalize(text.c_str(), text.length(), errorCode);
    EXPECT_EQ(errorCode, I18nErrorCode::SUCCESS);
    EXPECT_EQ(normalizedText.length(), 2); // 2 is the expected normalized text length.
    EXPECT_EQ(normalizedText, "\u0032\u0035");

    text = "\u1E9B\u0323";
    normalizedText = normalizer.Normalize(text.c_str(), text.length(), errorCode);
    EXPECT_EQ(errorCode, I18nErrorCode::SUCCESS);
    EXPECT_EQ(normalizedText.length(), 3); // 3 is the expected normalized text length.
    EXPECT_EQ(normalizedText, "\u1E69");
}

/**
 * @tc.name: I18nFuncTest006
 * @tc.desc: Test I18n I18nCalendar
 * @tc.type: FUNC
 */
HWTEST_F(I18nTest, I18nFuncTest006, TestSize.Level1)
{
    I18nCalendar buddhistCalendar("zh-Hans", CalendarType::BUDDHIST);
    buddhistCalendar.Set(UCalendarDateFields::UCAL_YEAR, 2023);
    int32_t year = buddhistCalendar.Get(UCalendarDateFields::UCAL_YEAR);
    EXPECT_EQ(year, 2023);
    I18nCalendar chineseCalendar("zh-Hans", CalendarType::CHINESE);
    chineseCalendar.SetMinimalDaysInFirstWeek(3);
    int32_t minimalDaysInFirstWeek = chineseCalendar.GetMinimalDaysInFirstWeek();
    EXPECT_EQ(minimalDaysInFirstWeek, 3);
    I18nCalendar copticCalendar("zh-Hans", CalendarType::COPTIC);
    copticCalendar.SetFirstDayOfWeek(2);
    int32_t firstDayOfWeek = copticCalendar.GetFirstDayOfWeek();
    EXPECT_EQ(firstDayOfWeek, 2);
    I18nCalendar ethiopicCalendar("zh-Hans", CalendarType::ETHIOPIC);
    int64_t date = 1687244448234;
    UErrorCode status = U_ZERO_ERROR;
    bool isWeekend = ethiopicCalendar.IsWeekend(date, status);
    EXPECT_EQ(isWeekend, false);
    I18nCalendar hebrewCalendar("zh-Hans", CalendarType::HEBREW);
    std::string displayLocaleTag = "en-US";
    std::string displayName = hebrewCalendar.GetDisplayName(displayLocaleTag);
    EXPECT_EQ(displayName, "Hebrew Calendar");
    I18nCalendar gregoryCalendar("zh-Hans", CalendarType::GREGORY);
    gregoryCalendar.Set(UCalendarDateFields::UCAL_MONTH, 2);
    int32_t month = gregoryCalendar.Get(UCalendarDateFields::UCAL_MONTH);
    EXPECT_EQ(month, 2);
    I18nCalendar indianCalendar("zh-Hans", CalendarType::INDIAN);
    indianCalendar.Set(UCalendarDateFields::UCAL_WEEK_OF_YEAR, 10);
    int32_t weekOfYear = indianCalendar.Get(UCalendarDateFields::UCAL_WEEK_OF_YEAR);
    EXPECT_EQ(weekOfYear, 10);
    I18nCalendar islamicCivilCalendar("zh-Hans", CalendarType::ISLAMIC_CIVIL);
    islamicCivilCalendar.Set(UCalendarDateFields::UCAL_WEEK_OF_MONTH, 2);
    int32_t weekOfMonth = islamicCivilCalendar.Get(UCalendarDateFields::UCAL_WEEK_OF_MONTH);
    EXPECT_EQ(weekOfMonth, 2);
    I18nCalendar islamicTblaCalendar("zh-Hans", CalendarType::ISLAMIC_TBLA);
    islamicTblaCalendar.Set(UCalendarDateFields::UCAL_DATE, 3);
    int32_t dateValue = islamicTblaCalendar.Get(UCalendarDateFields::UCAL_DATE);
    EXPECT_EQ(dateValue, 3);
    I18nCalendar islamicUmalquraCalendar("zh-Hans", CalendarType::ISLAMIC_UMALQURA);
    islamicUmalquraCalendar.Set(UCalendarDateFields::UCAL_DAY_OF_YEAR, 123);
    int32_t dayOfYear = islamicUmalquraCalendar.Get(UCalendarDateFields::UCAL_DAY_OF_YEAR);
    EXPECT_EQ(dayOfYear, 123);
    I18nCalendar japaneseCalendar("zh-Hans", CalendarType::JAPANESE);
    japaneseCalendar.Set(UCalendarDateFields::UCAL_DAY_OF_WEEK, 3);
    int32_t dayOfWeek = japaneseCalendar.Get(UCalendarDateFields::UCAL_DAY_OF_WEEK);
    EXPECT_EQ(dayOfWeek, 3);
}

/**
 * @tc.name: I18nFuncTest007
 * @tc.desc: Test I18n I18nCalendar
 * @tc.type: FUNC
 */
HWTEST_F(I18nTest, I18nFuncTest007, TestSize.Level1)
{
    I18nCalendar persianTblaCalendar("zh-Hans", CalendarType::PERSIAN);
    persianTblaCalendar.Set(UCalendarDateFields::UCAL_DAY_OF_WEEK_IN_MONTH, 1);
    int32_t dayOfWeekInMonth = persianTblaCalendar.Get(UCalendarDateFields::UCAL_DAY_OF_WEEK_IN_MONTH);
    EXPECT_EQ(dayOfWeekInMonth, 1);
    I18nCalendar defaultCalendar("zh-Hans", CalendarType::UNDEFINED);
    defaultCalendar.Set(UCalendarDateFields::UCAL_HOUR, 12);
    int32_t hour = defaultCalendar.Get(UCalendarDateFields::UCAL_HOUR);
    EXPECT_EQ(hour, 0);
}

/**
 * @tc.name: I18nFuncTest008
 * @tc.desc: Test I18n I18nBreakIterator
 * @tc.type: FUNC
 */
HWTEST_F(I18nTest, I18nFuncTest008, TestSize.Level1)
{
    std::string fakeLocaleTag = "FakeLocaleTag";
    I18nBreakIterator brkIterator(fakeLocaleTag);
    int32_t res = brkIterator.Current();
    EXPECT_EQ(res, 0);
    res = brkIterator.First();
    EXPECT_EQ(res, 0);
    res = brkIterator.Last();
    EXPECT_EQ(res, 0);
    res = brkIterator.Previous();
    EXPECT_EQ(res, -1);
    int32_t offset = 1;
    res = brkIterator.Next(offset);
    EXPECT_EQ(res, -1);
    res = brkIterator.Next();
    EXPECT_EQ(res, -1);
    res = brkIterator.Following(offset);
    EXPECT_EQ(res, -1);
    bool status = brkIterator.IsBoundary(offset);
    EXPECT_FALSE(status);

    std::string localeTag = "en-Latn-US";
    I18nBreakIterator enBrkIterator(localeTag);
    std::string text = "Test I18nBreakIterator";
    enBrkIterator.SetText(text.c_str());
    std::string resText;
    enBrkIterator.GetText(resText);
    EXPECT_EQ(resText, text);
    res = enBrkIterator.Next();
    EXPECT_EQ(res, 5);
}

/**
 * @tc.name: I18nFuncTest009
 * @tc.desc: Test I18n MeasureData GetDefaultPreferredUnit
 * @tc.type: FUNC
 */
HWTEST_F(I18nTest, I18nFuncTest009, TestSize.Level1)
{
    std::string regionTags[] = {"GB", "US", "CN"};
    std::string usage = "length";
    std::vector<std::string> units;
    for (size_t i = 0; i < sizeof(regionTags) / sizeof(std::string); ++i) {
        GetDefaultPreferredUnit(regionTags[i], usage, units);
        EXPECT_EQ(units.size(), 3);
        units.clear();
    }
}

/**
 * @tc.name: I18nFuncTest010
 * @tc.desc: Test I18n MeasureData GetFallbackPreferredUnit
 * @tc.type: FUNC
 */
HWTEST_F(I18nTest, I18nFuncTest010, TestSize.Level1)
{
    std::string regionTags1[] = {"MX", "NL", "NO", "PL", "RU"};
    std::string usage = "length-person-informal";
    std::vector<std::string> units;
    for (size_t i = 0; i < sizeof(regionTags1) / sizeof(std::string); ++i) {
        GetFallbackPreferredUnit(regionTags1[i], usage, units);
        EXPECT_EQ(units.size(), 2);
        units.clear();
    }

    std::string regionTag2 = "SE";
    usage = "length-person";
    GetFallbackPreferredUnit(regionTag2, usage, units);
    EXPECT_EQ(units.size(), 2);
    units.clear();

    std::string regionTags3[] = {"US", "CN"};
    for (size_t i = 0; i < sizeof(regionTags3) / sizeof(std::string); ++i) {
        GetFallbackPreferredUnit(regionTags3[i], usage, units);
        EXPECT_EQ(units.size(), 1);
        units.clear();
    }
}

/**
 * @tc.name: I18nFuncTest011
 * @tc.desc: Test I18n MeasureData GetRestPreferredUnit
 * @tc.type: FUNC
 */
HWTEST_F(I18nTest, I18nFuncTest011, TestSize.Level1)
{
    std::string regionTags1[] = {"CA", "IN"};
    std::string usage = "length-person";
    std::vector<std::string> units;
    for (size_t i = 0; i < sizeof(regionTags1) / sizeof(std::string); ++i) {
        GetRestPreferredUnit(regionTags1[i], usage, units);
        EXPECT_EQ(units.size(), 1);
        units.clear();
    }

    std::string regionTags2[] = {"CN", "DK", "PT", "DE", "GB"};
    usage = "length-person-informal";
    for (size_t i = 0; i < sizeof(regionTags2) / sizeof(std::string); ++i) {
        GetRestPreferredUnit(regionTags2[i], usage, units);
        EXPECT_EQ(units.size(), 2);
        units.clear();
    }

    std::string regionTag3 = "KR";
    usage = "speed-wind";
    GetRestPreferredUnit(regionTag3, usage, units);
    EXPECT_EQ(units.size(), 1);
    units.clear();

    std::string regionTag4 = "XX";
    usage = "fake usage";
    GetRestPreferredUnit(regionTag4, usage, units);
    EXPECT_EQ(units.size(), 0);
}

/**
 * @tc.name: I18nFuncTest012
 * @tc.desc: Test I18n MeasureData GetPreferredUnit
 * @tc.type: FUNC
 */
HWTEST_F(I18nTest, I18nFuncTest012, TestSize.Level1)
{
    std::string regionTags1[] = {
        "AT", "BE", "DZ", "EG", "ES", "FR", "HK", "ID", "IL", "IT", "JO", "MY", "SA", "TR", "VN"
    };
    std::string usage = "length-person";
    std::vector<std::string> units;
    for (size_t i = 0; i < sizeof(regionTags1) / sizeof(std::string); ++i) {
        GetPreferredUnit(regionTags1[i], usage, units);
        EXPECT_EQ(units.size(), 2);
        units.clear();
    }

    std::string regionTag2 = "BR";
    usage = "length-person-informal";
    GetPreferredUnit(regionTag2, usage, units);
    EXPECT_EQ(units.size(), 2);
    units.clear();

    std::string regionTags3[] = {"BS", "BZ", "PR", "PW"};
    usage = "temperature-weather";
    for (size_t i = 0; i < sizeof(regionTags3) / sizeof(std::string); i++) {
        GetPreferredUnit(regionTags3[i], usage, units);
        EXPECT_EQ(units.size(), 1);
        units.clear();
    }

    std::string regionTag4 = "XX";
    usage = "fake usage";
    GetPreferredUnit(regionTag4, usage, units);
    EXPECT_EQ(units.size(), 0);
}

/**
 * @tc.name: I18nFuncTest013
 * @tc.desc: Test I18n MeasureData
 * @tc.type: FUNC
 */
HWTEST_F(I18nTest, I18nFuncTest013, TestSize.Level1)
{
    double value = 10.0;
    std::string fromUnit = "acre";
    std::string fromMeasSys = "US";
    std::string toUnit = "foot";
    std::string toMeasSys = "US";
    int status = Convert(value, fromUnit, fromMeasSys, toUnit, toMeasSys);
    EXPECT_EQ(status, 0);

    std::string fakeUnit = "fake unit";
    status = Convert(value, fakeUnit, fromMeasSys, toUnit, toMeasSys);
    EXPECT_EQ(status, 0);
    status = Convert(value, fromUnit, fromMeasSys, fakeUnit, toMeasSys);
    EXPECT_EQ(status, 0);

    toUnit = "hectare";
    status = Convert(value, fromUnit, fromMeasSys, toUnit, toMeasSys);
    EXPECT_EQ(status, 1);
    EXPECT_TRUE(fabs(value - 4.0468564) < 1e-6);

    std::string unit = "tablespoon";
    std::string measSys = "UK";
    std::vector<double> factors = {0.0, 0.0};
    ComputeFactorValue(unit, measSys, factors);
    EXPECT_EQ(factors.size(), 2);
    EXPECT_TRUE(fabs(factors[0] - 1.77582e-05) < 1e-6);
    unit = "acre";
    factors = {0.0, 0.0};
    ComputeFactorValue(unit, measSys, factors);
    EXPECT_EQ(factors.size(), 2);
    EXPECT_TRUE(fabs(factors[0] - 4046.856422) < 1e-6);

    double res = ComputeSIPrefixValue(unit);
    EXPECT_TRUE(fabs(res - 0) < 1e-6);
    unit = "deci";
    res = ComputeSIPrefixValue(unit);
    EXPECT_TRUE(fabs(res - 0.1) < 1e-6);

    unit = "square-acre";
    factors = {0.0, 0.0};
    ComputePowerValue(unit, measSys, factors);
    EXPECT_EQ(factors.size(), 2);

    unit = "fake-per-hour";
    factors = {0.0, 0.0};
    status = ComputeValue(unit, measSys, factors);
    EXPECT_EQ(status, 0);
    unit = "kilometer-per-fake";
    factors = {0.0, 0.0};
    status = ComputeValue(unit, measSys, factors);
    EXPECT_EQ(status, 0);
}

/**
 * @tc.name: I18nFuncTest014
 * @tc.desc: Test I18n PhoneNumberFormat
 * @tc.type: FUNC
 */
HWTEST_F(I18nTest, I18nFuncTest014, TestSize.Level1)
{
    std::string countryTag = "";
    std::map<std::string, std::string> options = {
        {"type", "INTERNATIONAL"}
    };
    std::unique_ptr<PhoneNumberFormat> formatter = PhoneNumberFormat::CreateInstance(countryTag, options);
    std::string fakePhoneNumber = "93827393872723482";
    bool isValid = formatter->isValidPhoneNumber(fakePhoneNumber);
    EXPECT_FALSE(isValid);
    std::string formattedPhoneNumber = formatter->format(fakePhoneNumber);
    EXPECT_EQ(formattedPhoneNumber, "");
}

/**
 * @tc.name: I18nFuncTest015
 * @tc.desc: Test I18n I18nTimeZone
 * @tc.type: FUNC
 */
HWTEST_F(I18nTest, I18nFuncTest015, TestSize.Level1)
{
    std::string id = "";
    bool isZoneID = true;
    I18nTimeZone timezone1(id, isZoneID);
    std::string timezoneId = timezone1.GetID();
    EXPECT_EQ(timezoneId, "Asia/Shanghai");

    isZoneID = false;
    I18nTimeZone timezone2(id, isZoneID);
    timezoneId = timezone2.GetID();
    EXPECT_EQ(timezoneId, "CST");

    id = "Auckland";
    I18nTimeZone timezone3(id, isZoneID);
    timezoneId = timezone3.GetID();
    EXPECT_EQ(timezoneId, "Pacific/Auckland");
    std::string localeTag = "en-Latn-US";
    std::string displayName = timezone3.GetDisplayName(localeTag);
    EXPECT_EQ(timezoneId, "Pacific/Auckland");

    std::string fakeId = "fake city id";
    std::string cityDisplayName = I18nTimeZone::GetCityDisplayName(fakeId, localeTag);
    EXPECT_EQ(cityDisplayName, "");

    cityDisplayName = I18nTimeZone::GetCityDisplayName(id, localeTag);
    EXPECT_EQ(cityDisplayName, "Auckland (New Zealand)");

    std::string fakeLocale = "fake locale tag";
    cityDisplayName = I18nTimeZone::GetCityDisplayName(id, fakeLocale);
    EXPECT_EQ(cityDisplayName, "");
}
} // namespace I18n
} // namespace Global
} // namespace OHOS