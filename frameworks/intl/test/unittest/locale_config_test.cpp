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

#include "locale_config_test.h"
#include <gtest/gtest.h>
#include "locale_config.h"
#include "locale_config_ext.h"
#include "unicode/locid.h"
#include "parameter.h"

using namespace OHOS::Global::I18n;
using testing::ext::TestSize;
using namespace std;

namespace OHOS {
namespace Global {
namespace I18n {
class LocaleConfigTest : public testing::Test {
public:
    static string originalLanguage;
    static string originalRegion;
    static string originalLocale;
    static void SetUpTestCase(void);
    static void TearDownTestCase(void);
    void SetUp();
    void TearDown();

    static std::vector<std::pair<std::string, std::string>> languageAndRegion;
    static std::vector<std::string> simplifiedLanguage;
};

string LocaleConfigTest::originalLanguage;
string LocaleConfigTest::originalRegion;
string LocaleConfigTest::originalLocale;

std::vector<std::pair<std::string, std::string>> LocaleConfigTest::languageAndRegion {
    { "zh-Hans", "CN" }, { "zh-Hant", "HK" }, { "zh-Hant", "TW" }, { "en-Latn-US", "US" }, { "en-Latn-US", "GB" },
    { "bo", "CN" }, { "ug", "CN" }, { "am", "CN" }, { "ar", "CN" }, { "as", "CN" }, { "az-Latn", "IR" },
    { "be", "CN" }, { "bg", "CN" }, { "bn", "CN" }, { "bs-Latn", "CN" }, { "ca", "CN" }, { "cs", "CN" },
    { "da", "CN" }, { "de", "CN" }, { "el", "CN" }, { "es-ES", "US" }, { "es-US", "ES" }, { "et", "CN" },
    { "eu", "CN" }, { "fa", "IR" }, { "fi", "CN" }, { "tl", "CN" }, { "fr", "CN" },  { "jv-Latn", "CN" },
    { "mai", "CN" }, { "mn-Cyrl", "CN" }, { "uz-Latn", "CN" }
};

std::vector<std::string> LocaleConfigTest::simplifiedLanguage {
    "zh-Hans", "zh-Hant-HK", "zh-Hant", "en", "en-GB", "bo", "ug", "am", "ar", "as", "az-Latn", "be", "bg", "bn",
    "bs-Latn", "ca", "cs", "da", "de", "el", "es", "es-US", "et", "eu", "fa", "fi", "tl", "fr", "jv-Latn", "mai",
    "mn-Cyrl", "uz-Latn"
};

void LocaleConfigTest::SetUpTestCase(void)
{
    originalLanguage = LocaleConfig::GetSystemLanguage();
    originalRegion = LocaleConfig::GetSystemRegion();
    originalLocale = LocaleConfig::GetSystemLocale();
    LocaleConfig::SetSystemLanguage("zh-Hans");
    LocaleConfig::SetSystemRegion("CN");
    LocaleConfig::SetSystemLocale("zh-Hans-CN");
}

void LocaleConfigTest::TearDownTestCase(void)
{
    LocaleConfig::SetSystemLanguage(originalLanguage);
    LocaleConfig::SetSystemRegion(originalRegion);
    LocaleConfig::SetSystemLocale(originalLocale);
}

void LocaleConfigTest::SetUp(void)
{}

void LocaleConfigTest::TearDown(void)
{}

/**
 * @tc.name: LocaleConfigFuncTest001
 * @tc.desc: Test LocaleConfig GetSystemLanguage default
 * @tc.type: FUNC
 */
HWTEST_F(LocaleConfigTest, LocaleConfigFuncTest001, TestSize.Level1)
{
    int ret = SetParameter("hm.sys.language", "");
    if (!ret) {
        EXPECT_EQ(LocaleConfig::GetSystemLanguage(), "zh-Hans");
    }
}

/**
 * @tc.name: LocaleConfigFuncTest002
 * @tc.desc: Test LocaleConfig GetSystemRegion default.
 * @tc.type: FUNC
 */
HWTEST_F(LocaleConfigTest, LocaleConfigFuncTest002, TestSize.Level1)
{
    int ret = SetParameter("hm.sys.locale", "");
    if (!ret) {
        EXPECT_EQ(LocaleConfig::GetSystemRegion(), "CN");
    }
}

/**
 * @tc.name: LocaleConfigFuncTest003
 * @tc.desc: Test LocaleConfig GetSystemLocale default
 * @tc.type: FUNC
 */
HWTEST_F(LocaleConfigTest, LocaleConfigFuncTest003, TestSize.Level1)
{
    int ret = SetParameter("hm.sys.locale", "");
    if (!ret) {
        EXPECT_EQ(LocaleConfig::GetSystemLocale(), "zh-Hans-CN");
    }
}

/**
 * @tc.name: LocaleConfigFuncTest004
 * @tc.desc: Test LocaleConfig SetSystemLanguage
 * @tc.type: FUNC
 */
HWTEST_F(LocaleConfigTest, LocaleConfigFuncTest004, TestSize.Level1)
{
    string language = "pt-PT";
    I18nErrorCode status = LocaleConfig::SetSystemLanguage(language);
    EXPECT_EQ(status, I18nErrorCode::SUCCESS);
    LocaleConfig::SetSystemLanguage("zh-Hans");
    EXPECT_EQ(status, I18nErrorCode::SUCCESS);
    LocaleConfig::SetSystemLanguage("%$=");
}

/**
 * @tc.name: LocaleConfigFuncTest005
 * @tc.desc: Test LocaleConfig SetSystemLocale
 * @tc.type: FUNC
 */
HWTEST_F(LocaleConfigTest, LocaleConfigFuncTest005, TestSize.Level1)
{
    string locale = "zh-Hant-TW";
    I18nErrorCode status = LocaleConfig::SetSystemLocale(locale);
    EXPECT_EQ(status, I18nErrorCode::SUCCESS);
    status = LocaleConfig::SetSystemLocale("zh-Hans-CN");
    EXPECT_EQ(status, I18nErrorCode::SUCCESS);
}

/**
 * @tc.name: LocaleConfigFuncTest006
 * @tc.desc: Test LocaleConfig SetSystemRegion
 * @tc.type: FUNC
 */
HWTEST_F(LocaleConfigTest, LocaleConfigFuncTest006, TestSize.Level1)
{
    string locale = "zh-Hant-TW";
    I18nErrorCode status = LocaleConfig::SetSystemLocale(locale);
    EXPECT_EQ(status, I18nErrorCode::SUCCESS);
    status = LocaleConfig::SetSystemRegion("HK");
    EXPECT_EQ(status, I18nErrorCode::SUCCESS);
    status = LocaleConfig::SetSystemLocale("zh-Hans-CN");
    EXPECT_EQ(status, I18nErrorCode::SUCCESS);
    status = LocaleConfig::SetSystemRegion("@*%");
    EXPECT_EQ(status, I18nErrorCode::INVALID_REGION_TAG);
    status = LocaleConfig::SetSystemLocale("zh-Hans-CN", 2);
    EXPECT_NE(status, I18nErrorCode::SUCCESS);
    status = LocaleConfig::SetSystemLocale("zh-Hans-CN", 2);
    EXPECT_NE(status, I18nErrorCode::SUCCESS);
    status = LocaleConfig::SetSystemLocale("ar-EG");
    EXPECT_EQ(status, I18nErrorCode::SUCCESS);
    EXPECT_FALSE(LocaleConfig::GetUsingLocalDigit());
    status = LocaleConfig::SetSystemLocale("$$@");
    EXPECT_NE(status, I18nErrorCode::SUCCESS);
    status = LocaleConfig::SetSystemLocale("zh-Hans-CN");
    EXPECT_EQ(status, I18nErrorCode::SUCCESS);
}

/**
 * @tc.name: LocaleConfigFuncTest007
 * @tc.desc: Test LocaleConfig GetSystemCountries
 * @tc.type: FUNC
 */
HWTEST_F(LocaleConfigTest, LocaleConfigFuncTest007, TestSize.Level1)
{
    std::unordered_set<std::string> countries = LocaleConfig::GetSystemCountries("zh");
    EXPECT_TRUE(countries.size() > 200);
}

/**
 * @tc.name: LocaleConfigFuncTest008
 * @tc.desc: Test LocaleConfig GetDisplayLanguage
 * @tc.type: FUNC
 */
HWTEST_F(LocaleConfigTest, LocaleConfigFuncTest008, TestSize.Level1)
{
    std::string languageTag = "pt";
    std::string localeTag = "en-US";
    std::string displayName = LocaleConfig::GetDisplayLanguage(languageTag, localeTag, true);
    EXPECT_EQ(displayName, "Portuguese");

    displayName = LocaleConfig::GetDisplayLanguage(languageTag, localeTag, false);
    EXPECT_EQ(displayName, "Portuguese");

    std::string fakeLocaleTag = "FakeLocaleTag";
    displayName = LocaleConfig::GetDisplayLanguage(languageTag, fakeLocaleTag, true);
    EXPECT_EQ(displayName, "");

    std::string fakeLanguageTag = "FakeLanguageTag";
    displayName = LocaleConfig::GetDisplayLanguage(fakeLanguageTag, localeTag, true);
    EXPECT_EQ(displayName, "");

    languageTag = "zh-Hans-CN";
    localeTag = "en-US";
    displayName = LocaleConfig::GetDisplayLanguage(languageTag, localeTag, false);
    EXPECT_EQ(displayName, "Simplified Chinese");

    displayName = LocaleConfig::GetDisplayLanguage(languageTag, localeTag, true);
    EXPECT_EQ(displayName, "Simplified Chinese");

    displayName = LocaleConfig::GetDisplayLanguage(languageTag, fakeLocaleTag, false);
    EXPECT_EQ(displayName, "");

    languageTag = "pt-Latn-BR";
    displayName = LocaleConfig::GetDisplayLanguage(languageTag, localeTag, false);
    EXPECT_EQ(displayName, "Portuguese (Brazil)");

    displayName = LocaleConfig::GetDisplayLanguage(languageTag, fakeLocaleTag, false);
    EXPECT_EQ(displayName, "");

    languageTag = "zh";
    displayName = LocaleConfig::GetDisplayLanguage(languageTag, localeTag, true);
    EXPECT_EQ(displayName, "Chinese");

    localeTag = "en-Hans";
    displayName = LocaleConfig::GetDisplayLanguage(languageTag, localeTag, true);
    EXPECT_EQ(displayName, "Chinese");

    languageTag = "zh-Hans";
    localeTag = "en-US";
    displayName = LocaleConfig::GetDisplayLanguage(languageTag, localeTag, true);
    EXPECT_EQ(displayName, "Simplified Chinese");

    languageTag = "ro-MD";
    displayName = LocaleConfig::GetDisplayLanguage(languageTag, localeTag, true);
    EXPECT_EQ(displayName, "Moldavian");
}

/**
 * @tc.name: LocaleConfigFuncTest009
 * @tc.desc: Test LocaleConfig GetDisplayRegion
 * @tc.type: FUNC
 */
HWTEST_F(LocaleConfigTest, LocaleConfigFuncTest009, TestSize.Level1)
{
    std::string regionTag = "JP";
    std::string localeTag = "zh-Hans-CN";
    std::string displayName = LocaleConfig::GetDisplayRegion(regionTag, localeTag, false);
    EXPECT_EQ(displayName, "日本");

    std::string fakeRegionTag = "XX";
    displayName = LocaleConfig::GetDisplayRegion(fakeRegionTag, localeTag, false);
    EXPECT_EQ(displayName, "XX");

    std::string fakeLocaleTag = "FakeLocaleTag";
    displayName = LocaleConfig::GetDisplayRegion(regionTag, fakeLocaleTag, false);
    EXPECT_EQ(displayName, "");

    localeTag = "en-Latn-US";
    displayName = LocaleConfig::GetDisplayRegion(regionTag, localeTag, false);
    EXPECT_EQ(displayName, "Japan");

    regionTag = "zh-Hans-CN";
    displayName = LocaleConfig::GetDisplayRegion(regionTag, localeTag, false);
    EXPECT_EQ(displayName, "China");

    regionTag = "HK";
    displayName = LocaleConfig::GetDisplayRegion(regionTag, localeTag, false);
    EXPECT_EQ(displayName, "Hong Kong (China)");
}

/**
 * @tc.name: LocaleConfigFuncTest010
 * @tc.desc: Test LocaleConfig GetDisplayLanguage
 * @tc.type: FUNC
 */
HWTEST_F(LocaleConfigTest, LocaleConfigFuncTest010, TestSize.Level1)
{
    std::string languageTag = "zh-Hant-HK";
    std::string localeTag = "zh-Hant";
    std::string displayName = LocaleConfig::GetDisplayLanguage(languageTag, localeTag, true);
    EXPECT_EQ(displayName, "繁體中文（中國香港）");

    languageTag = "zh-Hant-HK";
    localeTag = "zh-Hans";
    displayName = LocaleConfig::GetDisplayLanguage(languageTag, localeTag, true);
    EXPECT_EQ(displayName, "繁体中文（中国香港）");

    languageTag = "zh-Hant-HK";
    localeTag = "en-US";
    displayName = LocaleConfig::GetDisplayLanguage(languageTag, localeTag, true);
    EXPECT_EQ(displayName, "Traditional Chinese(Hong Kong, China)");
}

/**
 * @tc.name: LocaleConfigFuncTest011
 * @tc.desc: Test LocaleConfig GetDisplayRegion
 * @tc.type: FUNC
 */
HWTEST_F(LocaleConfigTest, LocaleConfigFuncTest011, TestSize.Level1)
{
    EXPECT_EQ(LocaleConfig::GetDisplayRegion("zh", "en-US", true), "");
    EXPECT_EQ(LocaleConfig::GetDisplayRegion("zh-Hans-CN", "en-US", true), "China");
}

/**
 * @tc.name: LocaleConfigFuncTest012
 * @tc.desc: Test LocaleConfig GetDisplayRegion
 * @tc.type: FUNC
 */
HWTEST_F(LocaleConfigTest, LocaleConfigFuncTest012, TestSize.Level1)
{
    EXPECT_EQ(LocaleConfig::GetDisplayRegion("zh-Hans", "en-US", true), "");
}

/**
 * @tc.name: LocaleConfigFuncTest013
 * @tc.desc: Test LocaleConfig LocalDigit
 * @tc.type: FUNC
 */
HWTEST_F(LocaleConfigTest, LocaleConfigFuncTest013, TestSize.Level1)
{
    bool current = LocaleConfig::GetUsingLocalDigit();
    I18nErrorCode status = LocaleConfig::SetUsingLocalDigit(true);
    EXPECT_EQ(status, I18nErrorCode::UPDATE_LOCAL_DIGIT_FAILED);
    current = LocaleConfig::GetUsingLocalDigit();
    EXPECT_FALSE(current);
}

/**
 * @tc.name: LocaleConfigFuncTest014
 * @tc.desc: Test LocaleConfig 24 hour clock
 * @tc.type: FUNC
 */
HWTEST_F(LocaleConfigTest, LocaleConfigFuncTest014, TestSize.Level1)
{
    bool current = LocaleConfig::Is24HourClock();
    I18nErrorCode status = LocaleConfig::Set24HourClock("true");
    EXPECT_EQ(status, I18nErrorCode::SUCCESS);
    current = LocaleConfig::Is24HourClock();
    EXPECT_TRUE(current);
    status = LocaleConfig::Set24HourClock("abc");
    EXPECT_EQ(status, I18nErrorCode::INVALID_24_HOUR_CLOCK_TAG);
    LocaleConfig::Set24HourClock("");
    LocaleConfig::Set24HourClock("false");
}

/**
 * @tc.name: LocaleConfigFuncTest015
 * @tc.desc: Test LocaleConfig valid locales
 * @tc.type: FUNC
 */
HWTEST_F(LocaleConfigTest, LocaleConfigFuncTest015, TestSize.Level1)
{
    string localeTag = "zh";
    string validLocaleTag = LocaleConfig::GetValidLocale(localeTag);
    EXPECT_EQ(validLocaleTag, "zh");

    localeTag = "zh-u-hc-h12-nu-latn-ca-chinese-co-pinyin-kf-upper-kn-true";
    validLocaleTag = LocaleConfig::GetValidLocale(localeTag);
    EXPECT_EQ(validLocaleTag, "zh-u-ca-chinese-co-pinyin-kn-true-kf-upper-nu-latn-hc-h12");
}

/**
 * @tc.name: LocaleConfigFuncTest016
 * @tc.desc: Test LocaleConfig isRTL
 * @tc.type: FUNC
 */
HWTEST_F(LocaleConfigTest, LocaleConfigFuncTest016, TestSize.Level1)
{
    string localeTag = "zh";
    bool isRTL = LocaleConfig::IsRTL(localeTag);
    EXPECT_TRUE(!isRTL);
    localeTag = "ar";
    isRTL = LocaleConfig::IsRTL(localeTag);
    EXPECT_TRUE(isRTL);
}

/**
 * @tc.name: LocaleConfigFuncTest017
 * @tc.desc: Test LocaleConfig isSuggested
 * @tc.type: FUNC
 */
HWTEST_F(LocaleConfigTest, LocaleConfigFuncTest017, TestSize.Level1)
{
    string language = "zh";
    bool isSuggested = LocaleConfig::IsSuggested(language);
    EXPECT_TRUE(isSuggested);
    string region = "CN";
    isSuggested = LocaleConfig::IsSuggested(language, region);
    EXPECT_TRUE(isSuggested);
    std::string fakeLanguage = "FakeLanguage";
    isSuggested = LocaleConfig::IsSuggested(fakeLanguage);
    EXPECT_FALSE(isSuggested);
}

/**
 * @tc.name: LocaleConfigFuncTest018
 * @tc.desc: Test LocaleCOnfig systemRegion
 * @tc.type: FUNC
 */
HWTEST_F(LocaleConfigTest, LocaleConfigFuncTest018, TestSize.Level1)
{
    string currentRegion = LocaleConfig::GetSystemRegion();
    EXPECT_TRUE(currentRegion.length() > 0);
    string region = "CN";
    I18nErrorCode status = LocaleConfig::SetSystemRegion(region);
    EXPECT_EQ(status, I18nErrorCode::SUCCESS);
    currentRegion = LocaleConfig::GetSystemRegion();
    EXPECT_EQ(currentRegion, region);
}

/**
 * @tc.name: LocaleConfigFuncTest019
 * @tc.desc: Test LocaleCOnfig systemLocale
 * @tc.type: FUNC
 */
HWTEST_F(LocaleConfigTest, LocaleConfigFuncTest019, TestSize.Level1)
{
    string currentLocale = LocaleConfig::GetSystemLocale();
    EXPECT_TRUE(currentLocale.length() > 0);
    string locale = "zh-Hans-CN";
    I18nErrorCode status = LocaleConfig::SetSystemLocale(locale);
    EXPECT_EQ(status, I18nErrorCode::SUCCESS);
    currentLocale = LocaleConfig::GetSystemLocale();
    EXPECT_EQ(currentLocale, locale);
}

/**
 * @tc.name: LocaleConfigFuncTest020
 * @tc.desc: Test LocaleConfig systemLanguages
 * @tc.type: FUNC
 */
HWTEST_F(LocaleConfigTest, LocaleConfigFuncTest020, TestSize.Level1)
{
    std::unordered_set<std::string> languages = LocaleConfig::GetSystemLanguages();
    EXPECT_TRUE(languages.size() > 2);
    LocaleConfig::SetSystemLocale("ur-Latn-PK");
    LocaleConfig::SetUsingLocalDigit(true);
    LocaleConfig::SetUsingLocalDigit(false);
}

/**
 * @tc.name: LocaleConfigFuncTest021
 * @tc.desc: Test LocaleConfig IsValidLanguage
 * @tc.type: FUNC
 */
HWTEST_F(LocaleConfigTest, LocaleConfigFuncTest021, TestSize.Level1)
{
    const string language = "zh";
    const string fakeLanguage = "010";
    const string tag = "zh-Hans";
    const string fakeTag = "13-Hans";
    bool flag = LocaleConfig::IsValidLanguage(language);
    EXPECT_TRUE(flag);
    flag = LocaleConfig::IsValidLanguage(fakeLanguage);
    EXPECT_TRUE(!flag);
    flag = LocaleConfig::IsValidTag(tag);
    EXPECT_TRUE(flag);
    flag = LocaleConfig::IsValidTag(fakeTag);
    EXPECT_TRUE(!flag);
    EXPECT_FALSE(LocaleConfig::IsValidTag(""));
    icu::Locale locale = LocaleConfigExt::GetIcuLocale(tag);
    EXPECT_EQ(locale.getLanguage(), language);
}

/**
 * @tc.name: LocaleConfigFuncTest022
 * @tc.desc: Test LocaleConfig GetSimplifiedSystemLanguage
 * @tc.type: FUNC
 */
HWTEST_F(LocaleConfigTest, LocaleConfigFuncTest022, TestSize.Level1)
{
    for (auto index = 0; index < LocaleConfigTest::languageAndRegion.size(); ++index) {
        LocaleConfig::SetSystemLanguage(languageAndRegion[index].first);
        LocaleConfig::SetSystemRegion(languageAndRegion[index].second);
        std::string ret = LocaleConfig::GetSimplifiedSystemLanguage();
        EXPECT_EQ(ret, LocaleConfigTest::simplifiedLanguage[index]);
    }
}

/**
 * @tc.name: LocaleConfigFuncTest023
 * @tc.desc: Test LocaleConfig GetEffectiveLanguage and GetEffectiveLocale
 * @tc.type: FUNC
 */
HWTEST_F(LocaleConfigTest, LocaleConfigFuncTest023, TestSize.Level1)
{
    std::string systemLanguage = LocaleConfig::GetSystemLanguage();
    std::string systemLocale = LocaleConfig::GetSystemLocale();
    std::string language = "zh-Hans";
    std::string locale = "zh-Hans-CN-u-mu-celsius";
    LocaleConfig::SetSystemLanguage(language);
    LocaleConfig::SetSystemLocale(locale);
    std::string effectiveLanguage = LocaleConfig::GetEffectiveLanguage();
    std::string effectiveLocale = LocaleConfig::GetEffectiveLocale();
    std::string systemHour = LocaleConfig::GetSystemHour();
    EXPECT_EQ(systemHour, "false");
    EXPECT_EQ(effectiveLanguage, "zh-Hans-CN");
    EXPECT_EQ(effectiveLocale, "zh-Hans-CN-u-mu-celsius");
    LocaleConfig::SetSystemLanguage(systemLanguage);
    LocaleConfig::SetSystemLocale(systemLocale);
}

/**
 * @tc.name: LocaleConfigFuncTest024
 * @tc.desc: Test LocaleConfig Collations
 * @tc.type: FUNC
 */
HWTEST_F(LocaleConfigTest, LocaleConfigFuncTest024, TestSize.Level1)
{
    std::string systemLanguage = LocaleConfig::GetSystemLanguage();
    std::string systemLocale = LocaleConfig::GetSystemLocale();
    LocaleConfig::SetSystemLanguage("zh-Hant");
    LocaleConfig::SetSystemRegion("HK");
    std::unordered_map<std::string, std::string> systemCollations;
    I18nErrorCode errCode = LocaleConfig::GetSystemCollations(systemCollations);
    EXPECT_EQ(errCode, I18nErrorCode::SUCCESS);
    EXPECT_TRUE(systemCollations.find("pinyin") != systemCollations.end());
    EXPECT_TRUE(systemCollations.find("stroke") != systemCollations.end());
    EXPECT_TRUE(systemCollations.find("zhuyin") != systemCollations.end());

    std::string usingCollation;
    errCode = LocaleConfig::GetUsingCollation(usingCollation);
    EXPECT_EQ(errCode, I18nErrorCode::SUCCESS);
    EXPECT_EQ(usingCollation, "stroke");
    std::string systemLocaleInstanceTag = LocaleConfig::GetSystemLocaleInstanceTag();
    EXPECT_EQ(systemLocaleInstanceTag, "zh-Hant-HK");

    LocaleConfig::SetSystemLanguage("en-Latn-US");
    errCode = LocaleConfig::GetSystemCollations(systemCollations);
    EXPECT_EQ(errCode, I18nErrorCode::SUCCESS);
    EXPECT_TRUE(systemCollations.find("lower") != systemCollations.end());
    EXPECT_TRUE(systemCollations.find("upper") != systemCollations.end());
    errCode = LocaleConfig::GetUsingCollation(usingCollation);
    EXPECT_EQ(errCode, I18nErrorCode::SUCCESS);
    EXPECT_EQ(usingCollation, "lower");
    systemLocaleInstanceTag = LocaleConfig::GetSystemLocaleInstanceTag();
    EXPECT_EQ(systemLocaleInstanceTag, "en-Latn-HK");

    LocaleConfig::SetSystemLanguage("zh-Hans");
    errCode = LocaleConfig::GetSystemCollations(systemCollations);
    EXPECT_EQ(errCode, I18nErrorCode::SUCCESS);
    EXPECT_EQ(systemCollations.size(), 0);
    errCode = LocaleConfig::GetUsingCollation(usingCollation);
    EXPECT_EQ(errCode, I18nErrorCode::SUCCESS);
    EXPECT_EQ(usingCollation, "");
    systemLocaleInstanceTag = LocaleConfig::GetSystemLocaleInstanceTag();
    EXPECT_EQ(systemLocaleInstanceTag, "zh-Hans-HK");
    LocaleConfig::SetSystemLanguage(systemLanguage);
    LocaleConfig::SetSystemLocale(systemLocale);
}

/**
 * @tc.name: LocaleConfigFuncTest025
 * @tc.desc: Test LocaleConfig Collations
 * @tc.type: FUNC
 */
HWTEST_F(LocaleConfigTest, LocaleConfigFuncTest025, TestSize.Level1)
{
    std::string systemLanguage = LocaleConfig::GetSystemLanguage();
    std::string systemLocale = LocaleConfig::GetSystemLocale();
    LocaleConfig::SetSystemLanguage("zh-Hans");
    LocaleConfig::SetSystemRegion("HK");
    I18nErrorCode errCode = LocaleConfig::SetSystemCollation("upper");
    EXPECT_EQ(errCode, I18nErrorCode::INVALID_PARAM);
    std::string systemLocaleInstanceTag = LocaleConfig::GetSystemLocaleInstanceTag();
    EXPECT_EQ(systemLocaleInstanceTag, "zh-Hans-HK");

    LocaleConfig::SetSystemLanguage("zh-Hant");
    errCode = LocaleConfig::SetSystemCollation("upper");
    EXPECT_EQ(errCode, I18nErrorCode::INVALID_PARAM);
    systemLocaleInstanceTag = LocaleConfig::GetSystemLocaleInstanceTag();
    EXPECT_EQ(systemLocaleInstanceTag, "zh-Hant-HK");

    errCode = LocaleConfig::SetSystemCollation("pinyin");
    EXPECT_EQ(errCode, I18nErrorCode::SUCCESS);
    systemLocaleInstanceTag = LocaleConfig::GetSystemLocaleInstanceTag();
    EXPECT_EQ(systemLocaleInstanceTag, "zh-Hant-HK-u-co-pinyin");
    LocaleConfig::SetSystemLanguage(systemLanguage);
    LocaleConfig::SetSystemLocale(systemLocale);
}

/**
 * @tc.name: LocaleConfigFuncTest026
 * @tc.desc: Test LocaleConfig numbering system
 * @tc.type: FUNC
 */
HWTEST_F(LocaleConfigTest, LocaleConfigFuncTest026, TestSize.Level1)
{
    std::string systemLanguage = LocaleConfig::GetSystemLanguage();
    std::string systemLocale = LocaleConfig::GetSystemLocale();
    LocaleConfig::SetSystemLanguage("ur");
    LocaleConfig::SetSystemLocale("ur-CN");
    std::unordered_map<std::string, std::string> systemNumberingSystems;
    I18nErrorCode errCode = LocaleConfig::GetSystemNumberingSystems(systemNumberingSystems);
    EXPECT_EQ(errCode, I18nErrorCode::SUCCESS);
    EXPECT_TRUE(systemNumberingSystems.find("arabext") != systemNumberingSystems.end());
    EXPECT_TRUE(systemNumberingSystems.find("latn") != systemNumberingSystems.end());

    std::string usingNumberingSystem;
    errCode = LocaleConfig::GetUsingNumberingSystem(usingNumberingSystem);
    EXPECT_EQ(errCode, I18nErrorCode::SUCCESS);
    EXPECT_EQ(usingNumberingSystem, "latn");
    std::string systemLocaleInstanceTag = LocaleConfig::GetSystemLocaleInstanceTag();
    EXPECT_EQ(systemLocaleInstanceTag, "ur-CN");

    errCode = LocaleConfig::SetSystemNumberingSystem("latn");
    EXPECT_EQ(errCode, I18nErrorCode::SUCCESS);
    errCode = LocaleConfig::GetUsingNumberingSystem(usingNumberingSystem);
    EXPECT_EQ(errCode, I18nErrorCode::SUCCESS);
    EXPECT_EQ(usingNumberingSystem, "latn");
    systemLocaleInstanceTag = LocaleConfig::GetSystemLocaleInstanceTag();
    EXPECT_EQ(systemLocaleInstanceTag, "ur-CN-u-nu-latn");

    errCode = LocaleConfig::SetSystemNumberingSystem("arab");
    EXPECT_EQ(errCode, I18nErrorCode::INVALID_PARAM);

    LocaleConfig::SetSystemLanguage(systemLanguage);
    LocaleConfig::SetSystemLocale(systemLocale);
}

/**
 * @tc.name: LocaleConfigFuncTest027
 * @tc.desc: Test LocaleConfig number pattern
 * @tc.type: FUNC
 */
HWTEST_F(LocaleConfigTest, LocaleConfigFuncTest027, TestSize.Level1)
{
    std::string systemLanguage = LocaleConfig::GetSystemLanguage();
    std::string systemLocale = LocaleConfig::GetSystemLocale();
    LocaleConfig::SetSystemLanguage("zh-Hans");
    LocaleConfig::SetSystemRegion("CN");
    std::unordered_map<std::string, std::string> systemNumberPatterns;
    I18nErrorCode errCode = LocaleConfig::GetSystemNumberPatterns(systemNumberPatterns);
    EXPECT_EQ(errCode, I18nErrorCode::SUCCESS);
    EXPECT_TRUE(systemNumberPatterns.find("002c002e") != systemNumberPatterns.end());
    EXPECT_TRUE(systemNumberPatterns.find("0000002e") != systemNumberPatterns.end());
    EXPECT_TRUE(systemNumberPatterns.find("002e002c") != systemNumberPatterns.end());
    EXPECT_TRUE(systemNumberPatterns.find("0000002c") != systemNumberPatterns.end());

    std::string usingNumberPattern;
    errCode = LocaleConfig::GetUsingNumberPattern(usingNumberPattern);
    EXPECT_EQ(errCode, I18nErrorCode::SUCCESS);
    EXPECT_EQ(usingNumberPattern, "002c002e");

    errCode = LocaleConfig::SetSystemNumberPattern("0000002c");
    EXPECT_EQ(errCode, I18nErrorCode::SUCCESS);
    errCode = LocaleConfig::GetUsingNumberPattern(usingNumberPattern);
    EXPECT_EQ(errCode, I18nErrorCode::SUCCESS);
    EXPECT_EQ(usingNumberPattern, "0000002c");

    errCode = LocaleConfig::SetSystemNumberingSystem("002d002f");
    EXPECT_EQ(errCode, I18nErrorCode::INVALID_PARAM);

    LocaleConfig::SetSystemLanguage(systemLanguage);
    LocaleConfig::SetSystemLocale(systemLocale);
}

/**
 * @tc.name: LocaleConfigFuncTest028
 * @tc.desc: Test LocaleConfig
 * @tc.type: FUNC
 */
HWTEST_F(LocaleConfigTest, LocaleConfigFuncTest028, TestSize.Level1)
{
    std::string localeTag = "zh-Hans-CN-u-nu-arab-x-np-002c002e";
    std::pair<std::string, std::string> numberPattern = LocaleConfig::GetNumberPatternFromLocale(localeTag);
    EXPECT_EQ(numberPattern.first, ",");
    EXPECT_EQ(numberPattern.second, ".");
    std::string removedLocale = LocaleConfig::RemoveCustExtParam(localeTag);
    EXPECT_EQ(removedLocale, "zh-Hans-CN-u-nu-arab");
    removedLocale = LocaleConfig::RemoveAllExtParam(localeTag);
    EXPECT_EQ(removedLocale, "zh-Hans-CN");
    std::string modifiedLocale = LocaleConfig::ModifyExtParam(localeTag, "np", "0000002c", "-x-");
    EXPECT_EQ(modifiedLocale, "zh-Hans-CN-u-nu-arab-x-np-0000002c");
    std::string extParam = LocaleConfig::QueryExtParam(localeTag, "np", "-x-");
    EXPECT_EQ(extParam, "002c002e");
    std::string numberingSystemDigit = LocaleConfig::GetNumberingSystemDigit("latn");
    EXPECT_EQ(numberingSystemDigit, "0123456789");
}

/**
 * @tc.name: LocaleConfigFuncTest029
 * @tc.desc: Test LocaleConfig
 * @tc.type: FUNC
 */
HWTEST_F(LocaleConfigTest, LocaleConfigFuncTest029, TestSize.Level1)
{
    std::string systemLanguage = LocaleConfig::GetSystemLanguage();
    std::string systemLocale = LocaleConfig::GetSystemLocale();

    LocaleConfig::SetSystemLanguage("zh-Hans");
    LocaleConfig::SetSystemLocale("zh-Hans-CN");
    std::unordered_map<std::string, std::string> systemMeasurements;
    I18nErrorCode errCode = LocaleConfig::GetSystemMeasurements(systemMeasurements);
    EXPECT_EQ(errCode, I18nErrorCode::SUCCESS);
    EXPECT_TRUE(systemMeasurements.find("uksystem") != systemMeasurements.end());
    EXPECT_TRUE(systemMeasurements.find("ussystem") != systemMeasurements.end());
    EXPECT_TRUE(systemMeasurements.find("metric") != systemMeasurements.end());

    std::string usingMeasurement;
    errCode = LocaleConfig::GetUsingMeasurement(usingMeasurement);
    EXPECT_EQ(errCode, I18nErrorCode::SUCCESS);
    EXPECT_EQ(usingMeasurement, "metric");

    errCode = LocaleConfig::SetSystemMeasurement("uksystem");
    EXPECT_EQ(errCode, I18nErrorCode::SUCCESS);
    errCode = LocaleConfig::GetUsingMeasurement(usingMeasurement);
    EXPECT_EQ(errCode, I18nErrorCode::SUCCESS);
    EXPECT_EQ(usingMeasurement, "uksystem");

    errCode = LocaleConfig::SetSystemMeasurement("Invalid");
    EXPECT_EQ(errCode, I18nErrorCode::INVALID_PARAM);

    std::string measurement = LocaleConfig::GetMeasurementFromLocale("zh-Hans-CN-u-ms-uksystem");
    EXPECT_EQ(measurement, "uksystem");

    LocaleConfig::SetSystemLanguage(systemLanguage);
    LocaleConfig::SetSystemLocale(systemLocale);
}

/**
 * @tc.name: LocaleConfigFuncTest030
 * @tc.desc: Test LocaleConfig
 * @tc.type: FUNC
 */
HWTEST_F(LocaleConfigTest, LocaleConfigFuncTest030, TestSize.Level1)
{
    std::string systemLanguage = LocaleConfig::GetSystemLanguage();
    std::string systemLocale = LocaleConfig::GetSystemLocale();

    LocaleConfig::SetSystemLanguage("zh-Hans");
    LocaleConfig::SetSystemLocale("zh-Hans-CN");
    std::unordered_map<std::string, std::string> numericalDatePatterns;
    I18nErrorCode errCode = LocaleConfig::GetSystemNumericalDatePatterns(numericalDatePatterns);
    EXPECT_EQ(errCode, I18nErrorCode::SUCCESS);
    EXPECT_TRUE(numericalDatePatterns.find("d/MM/y") != numericalDatePatterns.end());
    EXPECT_TRUE(numericalDatePatterns.find("d/M/yy") != numericalDatePatterns.end());
    EXPECT_TRUE(numericalDatePatterns.find("y-MM-d") != numericalDatePatterns.end());
    EXPECT_TRUE(numericalDatePatterns.find("d/M/y") != numericalDatePatterns.end());
    EXPECT_TRUE(numericalDatePatterns.find("d/MM/yy") != numericalDatePatterns.end());
    EXPECT_TRUE(numericalDatePatterns.find("d.MM.yy") != numericalDatePatterns.end());
    EXPECT_TRUE(numericalDatePatterns.find("yy/MM/d") != numericalDatePatterns.end());
    EXPECT_TRUE(numericalDatePatterns.find("d.MM.y") != numericalDatePatterns.end());
    EXPECT_TRUE(numericalDatePatterns.find("d-MM-y") != numericalDatePatterns.end());
    EXPECT_TRUE(numericalDatePatterns.find("y/M/d") != numericalDatePatterns.end());
    EXPECT_TRUE(numericalDatePatterns.find("d-M-y") != numericalDatePatterns.end());

    std::string identifier;
    errCode = LocaleConfig::GetUsingNumericalDatePattern(identifier);
    EXPECT_EQ(errCode, I18nErrorCode::SUCCESS);
    EXPECT_EQ(identifier, "y/M/d");

    errCode = LocaleConfig::SetSystemNumericalDatePattern("d/MM/yy");
    EXPECT_EQ(errCode, I18nErrorCode::SUCCESS);
    errCode = LocaleConfig::GetUsingNumericalDatePattern(identifier);
    EXPECT_EQ(errCode, I18nErrorCode::SUCCESS);
    EXPECT_EQ(identifier, "d/MM/yy");

    errCode = LocaleConfig::SetSystemNumericalDatePattern("Invalid");
    EXPECT_EQ(errCode, I18nErrorCode::INVALID_PARAM);

    std::string patternNumber = LocaleConfig::GetPatternNumberFromLocale("zh-Hans-CN");
    EXPECT_EQ(patternNumber, "");

    LocaleConfig::SetSystemLanguage(systemLanguage);
    LocaleConfig::SetSystemLocale(systemLocale);
}

/**
 * @tc.name: LocaleConfigFuncTest031
 * @tc.desc: Test IsSuggested
 * @tc.type: FUNC
 */
HWTEST_F(LocaleConfigTest, LocaleConfigFuncTest031, TestSize.Level1)
{
    EXPECT_TRUE(LocaleConfig::IsSuggested("en", "AL"));
    EXPECT_TRUE(LocaleConfig::IsSuggested("en", "AF"));
    EXPECT_TRUE(LocaleConfig::IsSuggested("en", "AE"));
    EXPECT_TRUE(LocaleConfig::IsSuggested("en", "IE"));
    EXPECT_TRUE(LocaleConfig::IsSuggested("en", "AI"));
    EXPECT_TRUE(LocaleConfig::IsSuggested("en", "AG"));
    EXPECT_TRUE(LocaleConfig::IsSuggested("en", "AT"));
    EXPECT_TRUE(LocaleConfig::IsSuggested("en", "AU"));
    EXPECT_TRUE(LocaleConfig::IsSuggested("en", "BB"));
    EXPECT_TRUE(LocaleConfig::IsSuggested("en", "PG"));
    EXPECT_TRUE(LocaleConfig::IsSuggested("en", "BS"));
    EXPECT_TRUE(LocaleConfig::IsSuggested("en", "PK"));
    EXPECT_TRUE(LocaleConfig::IsSuggested("en", "BM"));
    EXPECT_TRUE(LocaleConfig::IsSuggested("en", "MP"));
    EXPECT_TRUE(LocaleConfig::IsSuggested("en", "BE"));
    EXPECT_TRUE(LocaleConfig::IsSuggested("en", "IS"));
    EXPECT_TRUE(LocaleConfig::IsSuggested("en", "PR"));
    EXPECT_TRUE(LocaleConfig::IsSuggested("en", "BZ"));
    EXPECT_TRUE(LocaleConfig::IsSuggested("en", "BW"));
    EXPECT_TRUE(LocaleConfig::IsSuggested("en", "BI"));
    EXPECT_TRUE(LocaleConfig::IsSuggested("en", "DK"));
    EXPECT_TRUE(LocaleConfig::IsSuggested("en", "DE"));
    EXPECT_TRUE(LocaleConfig::IsSuggested("en", "DG"));
    EXPECT_TRUE(LocaleConfig::IsSuggested("en", "DM"));
    EXPECT_TRUE(LocaleConfig::IsSuggested("en", "ER"));
    EXPECT_TRUE(LocaleConfig::IsSuggested("en", "PH"));
    EXPECT_TRUE(LocaleConfig::IsSuggested("en", "FJ"));
    EXPECT_TRUE(LocaleConfig::IsSuggested("en", "FI"));
    EXPECT_TRUE(LocaleConfig::IsSuggested("en", "FK"));
    EXPECT_TRUE(LocaleConfig::IsSuggested("en", "GM"));
    EXPECT_TRUE(LocaleConfig::IsSuggested("en", "SX"));
    EXPECT_TRUE(LocaleConfig::IsSuggested("en", "GD"));
    EXPECT_TRUE(LocaleConfig::IsSuggested("en", "GG"));
    EXPECT_TRUE(LocaleConfig::IsSuggested("en", "GU"));
    EXPECT_TRUE(LocaleConfig::IsSuggested("en", "GY"));
    EXPECT_TRUE(LocaleConfig::IsSuggested("en", "NL"));
    EXPECT_TRUE(LocaleConfig::IsSuggested("en", "KI"));
    EXPECT_TRUE(LocaleConfig::IsSuggested("en", "CA"));
    EXPECT_TRUE(LocaleConfig::IsSuggested("en", "GH"));
    EXPECT_TRUE(LocaleConfig::IsSuggested("en", "ZW"));
    EXPECT_TRUE(LocaleConfig::IsSuggested("en", "CM"));
    EXPECT_TRUE(LocaleConfig::IsSuggested("en", "KY"));
    EXPECT_TRUE(LocaleConfig::IsSuggested("en", "CC"));
    EXPECT_TRUE(LocaleConfig::IsSuggested("en", "KE"));
    EXPECT_TRUE(LocaleConfig::IsSuggested("en", "CK"));
    EXPECT_TRUE(LocaleConfig::IsSuggested("en", "LS"));
    EXPECT_TRUE(LocaleConfig::IsSuggested("en", "LR"));
}

/**
 * @tc.name: LocaleConfigFuncTest032
 * @tc.desc: Test IsSuggested
 * @tc.type: FUNC
 */
HWTEST_F(LocaleConfigTest, LocaleConfigFuncTest032, TestSize.Level1)
{
    EXPECT_TRUE(LocaleConfig::IsSuggested("en", "IM"));
    EXPECT_TRUE(LocaleConfig::IsSuggested("en", "MV"));
    EXPECT_TRUE(LocaleConfig::IsSuggested("en", "MT"));
    EXPECT_TRUE(LocaleConfig::IsSuggested("en", "MW"));
    EXPECT_TRUE(LocaleConfig::IsSuggested("en", "MY"));
    EXPECT_TRUE(LocaleConfig::IsSuggested("en", "MH"));
    EXPECT_TRUE(LocaleConfig::IsSuggested("en", "US"));
    EXPECT_TRUE(LocaleConfig::IsSuggested("en", "UM"));
    EXPECT_TRUE(LocaleConfig::IsSuggested("en", "AS"));
    EXPECT_TRUE(LocaleConfig::IsSuggested("en", "VI"));
    EXPECT_TRUE(LocaleConfig::IsSuggested("en", "MS"));
    EXPECT_TRUE(LocaleConfig::IsSuggested("en", "FM"));
    EXPECT_TRUE(LocaleConfig::IsSuggested("en", "NA"));
    EXPECT_TRUE(LocaleConfig::IsSuggested("en", "ZA"));
    EXPECT_TRUE(LocaleConfig::IsSuggested("en", "SS"));
    EXPECT_TRUE(LocaleConfig::IsSuggested("en", "NR"));
    EXPECT_TRUE(LocaleConfig::IsSuggested("en", "NG"));
    EXPECT_TRUE(LocaleConfig::IsSuggested("en", "NU"));
    EXPECT_TRUE(LocaleConfig::IsSuggested("en", "NF"));
    EXPECT_TRUE(LocaleConfig::IsSuggested("en", "PW"));
    EXPECT_TRUE(LocaleConfig::IsSuggested("en", "PN"));
    EXPECT_TRUE(LocaleConfig::IsSuggested("en", "SE"));
    EXPECT_TRUE(LocaleConfig::IsSuggested("en", "CH"));
    EXPECT_TRUE(LocaleConfig::IsSuggested("en", "WS"));
    EXPECT_TRUE(LocaleConfig::IsSuggested("en", "SL"));
    EXPECT_TRUE(LocaleConfig::IsSuggested("en", "CY"));
    EXPECT_TRUE(LocaleConfig::IsSuggested("en", "SC"));
    EXPECT_TRUE(LocaleConfig::IsSuggested("en", "SH"));
    EXPECT_TRUE(LocaleConfig::IsSuggested("en", "KN"));
    EXPECT_TRUE(LocaleConfig::IsSuggested("en", "LC"));
}

/**
 * @tc.name: LocaleConfigFuncTest033
 * @tc.desc: Test IsSuggested
 * @tc.type: FUNC
 */
HWTEST_F(LocaleConfigTest, LocaleConfigFuncTest033, TestSize.Level1)
{
    EXPECT_TRUE(LocaleConfig::IsSuggested("en", "VC"));
    EXPECT_TRUE(LocaleConfig::IsSuggested("en", "SI"));
    EXPECT_TRUE(LocaleConfig::IsSuggested("en", "SZ"));
    EXPECT_TRUE(LocaleConfig::IsSuggested("en", "SD"));
    EXPECT_TRUE(LocaleConfig::IsSuggested("en", "SB"));
    EXPECT_TRUE(LocaleConfig::IsSuggested("en", "TJ"));
    EXPECT_TRUE(LocaleConfig::IsSuggested("en", "TZ"));
    EXPECT_TRUE(LocaleConfig::IsSuggested("en", "TO"));
    EXPECT_TRUE(LocaleConfig::IsSuggested("en", "TC"));
    EXPECT_TRUE(LocaleConfig::IsSuggested("en", "TT"));
    EXPECT_TRUE(LocaleConfig::IsSuggested("en", "TV"));
    EXPECT_TRUE(LocaleConfig::IsSuggested("en", "TM"));
    EXPECT_TRUE(LocaleConfig::IsSuggested("en", "TK"));
    EXPECT_TRUE(LocaleConfig::IsSuggested("en", "VU"));
    EXPECT_TRUE(LocaleConfig::IsSuggested("en", "UG"));
    EXPECT_TRUE(LocaleConfig::IsSuggested("en", "SG"));
    EXPECT_TRUE(LocaleConfig::IsSuggested("en", "NZ"));
    EXPECT_TRUE(LocaleConfig::IsSuggested("en", "JM"));
    EXPECT_TRUE(LocaleConfig::IsSuggested("en", "AM"));
    EXPECT_TRUE(LocaleConfig::IsSuggested("en", "IR"));
    EXPECT_TRUE(LocaleConfig::IsSuggested("en", "IL"));
    EXPECT_TRUE(LocaleConfig::IsSuggested("en", "IN"));
    EXPECT_TRUE(LocaleConfig::IsSuggested("en", "GB"));
    EXPECT_TRUE(LocaleConfig::IsSuggested("en", "VG"));
    EXPECT_TRUE(LocaleConfig::IsSuggested("en", "IO"));
    EXPECT_TRUE(LocaleConfig::IsSuggested("en", "ZM"));
    EXPECT_TRUE(LocaleConfig::IsSuggested("en", "JE"));
    EXPECT_TRUE(LocaleConfig::IsSuggested("en", "GI"));
    EXPECT_TRUE(LocaleConfig::IsSuggested("en", "MO"));
    EXPECT_TRUE(LocaleConfig::IsSuggested("en", "HK"));
    EXPECT_TRUE(LocaleConfig::IsSuggested("en", "CX"));
    EXPECT_TRUE(LocaleConfig::IsSuggested("zh-Hant", "MO"));
    EXPECT_TRUE(LocaleConfig::IsSuggested("zh-Hant", "TW"));
    EXPECT_TRUE(LocaleConfig::IsSuggested("zh-Hant", "HK"));
}

/**
 * @tc.name: LocaleConfigFuncTest034
 * @tc.desc: Test IsSuggested
 * @tc.type: FUNC
 */
HWTEST_F(LocaleConfigTest, LocaleConfigFuncTest034, TestSize.Level1)
{
    std::unordered_set<std::string> systemLanguages = LocaleConfig::GetSystemLanguages();
    if (systemLanguages.find("fr") != systemLanguages.end()) {
        EXPECT_TRUE(LocaleConfig::IsSuggested("fr", "DZ"));
        EXPECT_TRUE(LocaleConfig::IsSuggested("fr", "BJ"));
        EXPECT_TRUE(LocaleConfig::IsSuggested("fr", "BE"));
        EXPECT_TRUE(LocaleConfig::IsSuggested("fr", "BI"));
        EXPECT_TRUE(LocaleConfig::IsSuggested("fr", "BF"));
        EXPECT_TRUE(LocaleConfig::IsSuggested("fr", "GQ"));
        EXPECT_TRUE(LocaleConfig::IsSuggested("fr", "TG"));
        EXPECT_TRUE(LocaleConfig::IsSuggested("fr", "FR"));
        EXPECT_TRUE(LocaleConfig::IsSuggested("fr", "PF"));
        EXPECT_TRUE(LocaleConfig::IsSuggested("fr", "GF"));
        EXPECT_TRUE(LocaleConfig::IsSuggested("fr", "MF"));
        EXPECT_TRUE(LocaleConfig::IsSuggested("fr", "CG"));
        EXPECT_TRUE(LocaleConfig::IsSuggested("fr", "CD"));
        EXPECT_TRUE(LocaleConfig::IsSuggested("fr", "GP"));
        EXPECT_TRUE(LocaleConfig::IsSuggested("fr", "HT"));
        EXPECT_TRUE(LocaleConfig::IsSuggested("fr", "DJ"));
        EXPECT_TRUE(LocaleConfig::IsSuggested("fr", "GN"));
        EXPECT_TRUE(LocaleConfig::IsSuggested("fr", "CA"));
        EXPECT_TRUE(LocaleConfig::IsSuggested("fr", "GA"));
        EXPECT_TRUE(LocaleConfig::IsSuggested("fr", "CM"));
        EXPECT_TRUE(LocaleConfig::IsSuggested("fr", "KM"));
        EXPECT_TRUE(LocaleConfig::IsSuggested("fr", "CI"));
        EXPECT_TRUE(LocaleConfig::IsSuggested("fr", "RE"));
        EXPECT_TRUE(LocaleConfig::IsSuggested("fr", "LU"));
        EXPECT_TRUE(LocaleConfig::IsSuggested("fr", "RW"));
        EXPECT_TRUE(LocaleConfig::IsSuggested("fr", "MG"));
        EXPECT_TRUE(LocaleConfig::IsSuggested("fr", "ML"));
        EXPECT_TRUE(LocaleConfig::IsSuggested("fr", "MQ"));
        EXPECT_TRUE(LocaleConfig::IsSuggested("fr", "YT"));
        EXPECT_TRUE(LocaleConfig::IsSuggested("fr", "MU"));
        EXPECT_TRUE(LocaleConfig::IsSuggested("fr", "MR"));
        EXPECT_TRUE(LocaleConfig::IsSuggested("fr", "NE"));
        EXPECT_TRUE(LocaleConfig::IsSuggested("fr", "MA"));
        EXPECT_TRUE(LocaleConfig::IsSuggested("fr", "MC"));
        EXPECT_TRUE(LocaleConfig::IsSuggested("fr", "BL"));
        EXPECT_TRUE(LocaleConfig::IsSuggested("fr", "PM"));
        EXPECT_TRUE(LocaleConfig::IsSuggested("fr", "WF"));
        EXPECT_TRUE(LocaleConfig::IsSuggested("fr", "VU"));
        EXPECT_TRUE(LocaleConfig::IsSuggested("fr", "TN"));
        EXPECT_TRUE(LocaleConfig::IsSuggested("fr", "SN"));
        EXPECT_TRUE(LocaleConfig::IsSuggested("fr", "SC"));
        EXPECT_TRUE(LocaleConfig::IsSuggested("fr", "NC"));
        EXPECT_TRUE(LocaleConfig::IsSuggested("fr", "SY"));
        EXPECT_TRUE(LocaleConfig::IsSuggested("fr", "TD"));
        EXPECT_TRUE(LocaleConfig::IsSuggested("fr", "CF"));
    }
}

/**
 * @tc.name: LocaleConfigFuncTest035
 * @tc.desc: Test IsSuggested
 * @tc.type: FUNC
 */
HWTEST_F(LocaleConfigTest, LocaleConfigFuncTest035, TestSize.Level1)
{
    std::unordered_set<std::string> systemLanguages = LocaleConfig::GetSystemLanguages();
    if (systemLanguages.find("de") != systemLanguages.end()) {
        EXPECT_TRUE(LocaleConfig::IsSuggested("de", "AT"));
        EXPECT_TRUE(LocaleConfig::IsSuggested("de", "BE"));
        EXPECT_TRUE(LocaleConfig::IsSuggested("de", "DE"));
        EXPECT_TRUE(LocaleConfig::IsSuggested("de", "LI"));
        EXPECT_TRUE(LocaleConfig::IsSuggested("de", "LU"));
        EXPECT_TRUE(LocaleConfig::IsSuggested("de", "CH"));
        EXPECT_TRUE(LocaleConfig::IsSuggested("de", "IT"));
    }
    if (systemLanguages.find("pt-PT") != systemLanguages.end()) {
        EXPECT_TRUE(LocaleConfig::IsSuggested("pt-PT", "AO"));
        EXPECT_TRUE(LocaleConfig::IsSuggested("pt-PT", "TL"));
        EXPECT_TRUE(LocaleConfig::IsSuggested("pt-PT", "CV"));
        EXPECT_TRUE(LocaleConfig::IsSuggested("pt-PT", "GW"));
        EXPECT_TRUE(LocaleConfig::IsSuggested("pt-PT", "MZ"));
        EXPECT_TRUE(LocaleConfig::IsSuggested("pt-PT", "PT"));
        EXPECT_TRUE(LocaleConfig::IsSuggested("pt-PT", "ST"));
        EXPECT_TRUE(LocaleConfig::IsSuggested("pt-PT", "GQ"));
        EXPECT_TRUE(LocaleConfig::IsSuggested("pt-PT", "MO"));
    }
    if (systemLanguages.find("es") != systemLanguages.end()) {
        EXPECT_TRUE(LocaleConfig::IsSuggested("es", "GQ"));
        EXPECT_TRUE(LocaleConfig::IsSuggested("es", "PH"));
        EXPECT_TRUE(LocaleConfig::IsSuggested("es", "IC"));
        EXPECT_TRUE(LocaleConfig::IsSuggested("es", "EA"));
        EXPECT_TRUE(LocaleConfig::IsSuggested("es", "ES"));
    }
    if (systemLanguages.find("it") != systemLanguages.end()) {
        EXPECT_TRUE(LocaleConfig::IsSuggested("it", "SM"));
        EXPECT_TRUE(LocaleConfig::IsSuggested("it", "VA"));
        EXPECT_TRUE(LocaleConfig::IsSuggested("it", "CH"));
        EXPECT_TRUE(LocaleConfig::IsSuggested("it", "IT"));
    }
    if (systemLanguages.find("ru") != systemLanguages.end()) {
        EXPECT_TRUE(LocaleConfig::IsSuggested("ru", "BY"));
        EXPECT_TRUE(LocaleConfig::IsSuggested("ru", "RU"));
        EXPECT_TRUE(LocaleConfig::IsSuggested("ru", "KZ"));
        EXPECT_TRUE(LocaleConfig::IsSuggested("ru", "KG"));
        EXPECT_TRUE(LocaleConfig::IsSuggested("ru", "UA"));
        EXPECT_TRUE(LocaleConfig::IsSuggested("ru", "MD"));
    }
    if (systemLanguages.find("ko") != systemLanguages.end()) {
        EXPECT_TRUE(LocaleConfig::IsSuggested("ko", "KR"));
    }
}

/**
 * @tc.name: LocaleConfigFuncTest036
 * @tc.desc: Test IsSuggested
 * @tc.type: FUNC
 */
HWTEST_F(LocaleConfigTest, LocaleConfigFuncTest036, TestSize.Level1)
{
    std::unordered_set<std::string> systemLanguages = LocaleConfig::GetSystemLanguages();
    if (systemLanguages.find("ar") != systemLanguages.end()) {
        EXPECT_TRUE(LocaleConfig::IsSuggested("ar", "SA"));
        EXPECT_TRUE(LocaleConfig::IsSuggested("ar", "SD"));
        EXPECT_TRUE(LocaleConfig::IsSuggested("ar", "SO"));
        EXPECT_TRUE(LocaleConfig::IsSuggested("ar", "TN"));
        EXPECT_TRUE(LocaleConfig::IsSuggested("ar", "SY"));
        EXPECT_TRUE(LocaleConfig::IsSuggested("ar", "YE"));
        EXPECT_TRUE(LocaleConfig::IsSuggested("ar", "IQ"));
        EXPECT_TRUE(LocaleConfig::IsSuggested("ar", "LB"));
        EXPECT_TRUE(LocaleConfig::IsSuggested("ar", "IL"));
        EXPECT_TRUE(LocaleConfig::IsSuggested("ar", "JO"));
        EXPECT_TRUE(LocaleConfig::IsSuggested("ar", "TD"));
        EXPECT_TRUE(LocaleConfig::IsSuggested("ar", "SS"));
        EXPECT_TRUE(LocaleConfig::IsSuggested("ar", "MA"));
        EXPECT_TRUE(LocaleConfig::IsSuggested("ar", "MR"));
        EXPECT_TRUE(LocaleConfig::IsSuggested("ar", "DJ"));
        EXPECT_TRUE(LocaleConfig::IsSuggested("ar", "ER"));
        EXPECT_TRUE(LocaleConfig::IsSuggested("ar", "DZ"));
        EXPECT_TRUE(LocaleConfig::IsSuggested("ar", "AE"));
        EXPECT_TRUE(LocaleConfig::IsSuggested("ar", "OM"));
        EXPECT_TRUE(LocaleConfig::IsSuggested("ar", "EG"));
        EXPECT_TRUE(LocaleConfig::IsSuggested("ar", "PS"));
        EXPECT_TRUE(LocaleConfig::IsSuggested("ar", "BH"));
        EXPECT_TRUE(LocaleConfig::IsSuggested("ar", "LY"));
        EXPECT_TRUE(LocaleConfig::IsSuggested("ar", "QA"));
        EXPECT_TRUE(LocaleConfig::IsSuggested("ar", "KW"));
        EXPECT_TRUE(LocaleConfig::IsSuggested("ar", "KM"));
    }
}

/**
 * @tc.name: LocaleConfigFuncTest037
 * @tc.desc: Test IsSuggested
 * @tc.type: FUNC
 */
HWTEST_F(LocaleConfigTest, LocaleConfigFuncTest037, TestSize.Level1)
{
    std::unordered_set<std::string> systemLanguages = LocaleConfig::GetSystemLanguages();
    if (systemLanguages.find("ms") != systemLanguages.end()) {
        EXPECT_TRUE(LocaleConfig::IsSuggested("ms", "MY"));
        EXPECT_TRUE(LocaleConfig::IsSuggested("ms", "SG"));
        EXPECT_TRUE(LocaleConfig::IsSuggested("ms", "BN"));
    }
    if (systemLanguages.find("my") != systemLanguages.end()) {
        EXPECT_TRUE(LocaleConfig::IsSuggested("my", "MM"));
    }
    if (systemLanguages.find("ja") != systemLanguages.end()) {
        EXPECT_TRUE(LocaleConfig::IsSuggested("ja", "JP"));
    }
    if (systemLanguages.find("th") != systemLanguages.end()) {
        EXPECT_TRUE(LocaleConfig::IsSuggested("th", "TH"));
    }
    if (systemLanguages.find("tr") != systemLanguages.end()) {
        EXPECT_TRUE(LocaleConfig::IsSuggested("tr", "TR"));
    }
    if (systemLanguages.find("vi") != systemLanguages.end()) {
        EXPECT_TRUE(LocaleConfig::IsSuggested("vi", "VN"));
    }
    if (systemLanguages.find("id") != systemLanguages.end()) {
        EXPECT_TRUE(LocaleConfig::IsSuggested("id", "ID"));
    }
}

/**
 * @tc.name: LocaleConfigFuncTest038
 * @tc.desc: Test IsSuggested
 * @tc.type: FUNC
 */
HWTEST_F(LocaleConfigTest, LocaleConfigFuncTest038, TestSize.Level1)
{
    std::unordered_set<std::string> systemLanguages = LocaleConfig::GetSystemLanguages();
    if (systemLanguages.find("es-US") != systemLanguages.end()) {
        EXPECT_TRUE(LocaleConfig::IsSuggested("es-US", "PA"));
        EXPECT_TRUE(LocaleConfig::IsSuggested("es-US", "BR"));
        EXPECT_TRUE(LocaleConfig::IsSuggested("es-US", "PE"));
        EXPECT_TRUE(LocaleConfig::IsSuggested("es-US", "PR"));
        EXPECT_TRUE(LocaleConfig::IsSuggested("es-US", "DO"));
        EXPECT_TRUE(LocaleConfig::IsSuggested("es-US", "EC"));
        EXPECT_TRUE(LocaleConfig::IsSuggested("es-US", "CO"));
        EXPECT_TRUE(LocaleConfig::IsSuggested("es-US", "CR"));
        EXPECT_TRUE(LocaleConfig::IsSuggested("es-US", "HN"));
        EXPECT_TRUE(LocaleConfig::IsSuggested("es-US", "CL"));
        EXPECT_TRUE(LocaleConfig::IsSuggested("es-US", "US"));
        EXPECT_TRUE(LocaleConfig::IsSuggested("es-US", "MX"));
        EXPECT_TRUE(LocaleConfig::IsSuggested("es-US", "NI"));
        EXPECT_TRUE(LocaleConfig::IsSuggested("es-US", "SV"));
        EXPECT_TRUE(LocaleConfig::IsSuggested("es-US", "GT"));
        EXPECT_TRUE(LocaleConfig::IsSuggested("es-US", "VE"));
        EXPECT_TRUE(LocaleConfig::IsSuggested("es-US", "BO"));
        EXPECT_TRUE(LocaleConfig::IsSuggested("es-US", "BZ"));
        EXPECT_TRUE(LocaleConfig::IsSuggested("es-US", "AR"));
        EXPECT_TRUE(LocaleConfig::IsSuggested("es-US", "PY"));
        EXPECT_TRUE(LocaleConfig::IsSuggested("es-US", "UY"));
        EXPECT_TRUE(LocaleConfig::IsSuggested("es-US", "CU"));
    }
    if (systemLanguages.find("ca") != systemLanguages.end()) {
        EXPECT_TRUE(LocaleConfig::IsSuggested("ca", "FR"));
        EXPECT_TRUE(LocaleConfig::IsSuggested("ca", "IT"));
        EXPECT_TRUE(LocaleConfig::IsSuggested("ca", "AD"));
        EXPECT_TRUE(LocaleConfig::IsSuggested("ca", "ES"));
    }
    if (systemLanguages.find("nl") != systemLanguages.end()) {
        EXPECT_TRUE(LocaleConfig::IsSuggested("nl", "NL"));
        EXPECT_TRUE(LocaleConfig::IsSuggested("nl", "BQ"));
        EXPECT_TRUE(LocaleConfig::IsSuggested("nl", "SX"));
        EXPECT_TRUE(LocaleConfig::IsSuggested("nl", "BE"));
        EXPECT_TRUE(LocaleConfig::IsSuggested("nl", "CW"));
        EXPECT_TRUE(LocaleConfig::IsSuggested("nl", "SR"));
        EXPECT_TRUE(LocaleConfig::IsSuggested("nl", "AW"));
    }
    if (systemLanguages.find("sr-Latn") != systemLanguages.end()) {
        EXPECT_TRUE(LocaleConfig::IsSuggested("sr-Latn", "BA"));
        EXPECT_TRUE(LocaleConfig::IsSuggested("sr-Latn", "ME"));
        EXPECT_TRUE(LocaleConfig::IsSuggested("sr-Latn", "RS"));
    }
}

/**
 * @tc.name: LocaleConfigFuncTest039
 * @tc.desc: Test IsSuggested
 * @tc.type: FUNC
 */
HWTEST_F(LocaleConfigTest, LocaleConfigFuncTest039, TestSize.Level1)
{
    std::unordered_set<std::string> systemLanguages = LocaleConfig::GetSystemLanguages();
    if (systemLanguages.find("da") != systemLanguages.end()) {
        EXPECT_TRUE(LocaleConfig::IsSuggested("da", "DK"));
        EXPECT_TRUE(LocaleConfig::IsSuggested("da", "GL"));
    }
    if (systemLanguages.find("ur") != systemLanguages.end()) {
        EXPECT_TRUE(LocaleConfig::IsSuggested("ur", "PK"));
        EXPECT_TRUE(LocaleConfig::IsSuggested("ur", "IN"));
    }
    if (systemLanguages.find("sv") != systemLanguages.end()) {
        EXPECT_TRUE(LocaleConfig::IsSuggested("sv", "AX"));
        EXPECT_TRUE(LocaleConfig::IsSuggested("sv", "FI"));
        EXPECT_TRUE(LocaleConfig::IsSuggested("sv", "SE"));
    }
    if (systemLanguages.find("hr") != systemLanguages.end()) {
        EXPECT_TRUE(LocaleConfig::IsSuggested("hr", "BA"));
        EXPECT_TRUE(LocaleConfig::IsSuggested("hr", "HR"));
    }
    if (systemLanguages.find("ro") != systemLanguages.end()) {
        EXPECT_TRUE(LocaleConfig::IsSuggested("ro", "RO"));
        EXPECT_TRUE(LocaleConfig::IsSuggested("ro", "MD"));
    }
    if (systemLanguages.find("nb") != systemLanguages.end()) {
        EXPECT_TRUE(LocaleConfig::IsSuggested("nb", "NO"));
        EXPECT_TRUE(LocaleConfig::IsSuggested("nb", "SJ"));
    }
    if (systemLanguages.find("ne") != systemLanguages.end()) {
        EXPECT_TRUE(LocaleConfig::IsSuggested("ne", "IN"));
        EXPECT_TRUE(LocaleConfig::IsSuggested("ne", "NP"));
    }
    if (systemLanguages.find("bn") != systemLanguages.end()) {
        EXPECT_TRUE(LocaleConfig::IsSuggested("bn", "IN"));
        EXPECT_TRUE(LocaleConfig::IsSuggested("bn", "BD"));
    }
    if (systemLanguages.find("fa") != systemLanguages.end()) {
        EXPECT_TRUE(LocaleConfig::IsSuggested("fa", "AF"));
        EXPECT_TRUE(LocaleConfig::IsSuggested("fa", "IR"));
    }
    if (systemLanguages.find("az-Latn") != systemLanguages.end()) {
        EXPECT_TRUE(LocaleConfig::IsSuggested("az-Latn", "AZ"));
    }
    if (systemLanguages.find("be") != systemLanguages.end()) {
        EXPECT_TRUE(LocaleConfig::IsSuggested("be", "BY"));
    }
    if (systemLanguages.find("bg") != systemLanguages.end()) {
        EXPECT_TRUE(LocaleConfig::IsSuggested("bg", "BG"));
    }
}

/**
 * @tc.name: LocaleConfigFuncTest040
 * @tc.desc: Test IsSuggested
 * @tc.type: FUNC
 */
HWTEST_F(LocaleConfigTest, LocaleConfigFuncTest040, TestSize.Level1)
{
    std::unordered_set<std::string> systemLanguages = LocaleConfig::GetSystemLanguages();
    if (systemLanguages.find("bs-Latn") != systemLanguages.end()) {
        EXPECT_TRUE(LocaleConfig::IsSuggested("bs-Latn", "BA"));
    }
    if (systemLanguages.find("cs") != systemLanguages.end()) {
        EXPECT_TRUE(LocaleConfig::IsSuggested("cs", "CZ"));
    }
    if (systemLanguages.find("et") != systemLanguages.end()) {
        EXPECT_TRUE(LocaleConfig::IsSuggested("et", "EE"));
    }
    if (systemLanguages.find("eu") != systemLanguages.end()) {
        EXPECT_TRUE(LocaleConfig::IsSuggested("eu", "ES"));
    }
    if (systemLanguages.find("el") != systemLanguages.end()) {
        EXPECT_TRUE(LocaleConfig::IsSuggested("el", "GR"));
        EXPECT_TRUE(LocaleConfig::IsSuggested("el", "CY"));
    }
    if (systemLanguages.find("fi") != systemLanguages.end()) {
        EXPECT_TRUE(LocaleConfig::IsSuggested("fi", "FI"));
    }
    if (systemLanguages.find("fil") != systemLanguages.end()) {
        EXPECT_TRUE(LocaleConfig::IsSuggested("fil", "PH"));
    }
    if (systemLanguages.find("gl") != systemLanguages.end()) {
        EXPECT_TRUE(LocaleConfig::IsSuggested("gl", "ES"));
    }
    if (systemLanguages.find("he") != systemLanguages.end()) {
        EXPECT_TRUE(LocaleConfig::IsSuggested("he", "IL"));
    }
    if (systemLanguages.find("hi") != systemLanguages.end()) {
        EXPECT_TRUE(LocaleConfig::IsSuggested("hi", "IN"));
    }
    if (systemLanguages.find("hu") != systemLanguages.end()) {
        EXPECT_TRUE(LocaleConfig::IsSuggested("hu", "HU"));
    }
    if (systemLanguages.find("jv-Latn") != systemLanguages.end()) {
        EXPECT_TRUE(LocaleConfig::IsSuggested("jv-Latn", "ID"));
    }
    if (systemLanguages.find("ka") != systemLanguages.end()) {
        EXPECT_TRUE(LocaleConfig::IsSuggested("ka", "GE"));
    }
    if (systemLanguages.find("km") != systemLanguages.end()) {
        EXPECT_TRUE(LocaleConfig::IsSuggested("km", "KH"));
    }
    if (systemLanguages.find("kk") != systemLanguages.end()) {
        EXPECT_TRUE(LocaleConfig::IsSuggested("kk", "KZ"));
    }
}

/**
 * @tc.name: LocaleConfigFuncTest041
 * @tc.desc: Test IsSuggested
 * @tc.type: FUNC
 */
HWTEST_F(LocaleConfigTest, LocaleConfigFuncTest041, TestSize.Level1)
{
    std::unordered_set<std::string> systemLanguages = LocaleConfig::GetSystemLanguages();
    if (systemLanguages.find("lv") != systemLanguages.end()) {
        EXPECT_TRUE(LocaleConfig::IsSuggested("lv", "LV"));
    }
    if (systemLanguages.find("lo") != systemLanguages.end()) {
        EXPECT_TRUE(LocaleConfig::IsSuggested("lo", "LA"));
    }
    if (systemLanguages.find("lt") != systemLanguages.end()) {
        EXPECT_TRUE(LocaleConfig::IsSuggested("lt", "LT"));
    }
    if (systemLanguages.find("mk") != systemLanguages.end()) {
        EXPECT_TRUE(LocaleConfig::IsSuggested("mk", "MK"));
    }
    if (systemLanguages.find("pl") != systemLanguages.end()) {
        EXPECT_TRUE(LocaleConfig::IsSuggested("pl", "PL"));
    }
    if (systemLanguages.find("pt-BR") != systemLanguages.end()) {
        EXPECT_TRUE(LocaleConfig::IsSuggested("pt-BR", "BR"));
    }
    if (systemLanguages.find("si") != systemLanguages.end()) {
        EXPECT_TRUE(LocaleConfig::IsSuggested("si", "LK"));
    }
    if (systemLanguages.find("sk") != systemLanguages.end()) {
        EXPECT_TRUE(LocaleConfig::IsSuggested("sk", "SK"));
    }
    if (systemLanguages.find("sl") != systemLanguages.end()) {
        EXPECT_TRUE(LocaleConfig::IsSuggested("sl", "SI"));
    }
    if (systemLanguages.find("uk") != systemLanguages.end()) {
        EXPECT_TRUE(LocaleConfig::IsSuggested("uk", "UA"));
    }
    if (systemLanguages.find("uz-Latn") != systemLanguages.end()) {
        EXPECT_TRUE(LocaleConfig::IsSuggested("uz-Latn", "UZ"));
    }
}
} // namespace I18n
} // namespace Global
} // namespace OHOS