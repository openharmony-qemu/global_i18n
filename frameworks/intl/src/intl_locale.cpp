/*
 * Copyright (c) 2025 Huawei Device Co., Ltd.
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

#include "intl_locale.h"

#include "i18n_hilog.h"
#include "locale_helper.h"
#include "ohos/init_data.h"

namespace OHOS {
namespace Global {
namespace I18n {
const std::string APPLY_OPTION_ERROR_MESSAGE = "apply option to tag failed";
const std::string OUT_OF_RANGE_ERROR_MESSAGE = "Value out of range for locale options property";
const std::string INSERT_OR_BUILD_ERROR_MESSAGE = "insert or build failed";
const std::string INVALID_LOCALE_ERROR_MESSAGE = "invalid locale";
const std::string IntlLocale::languageTag = "language";
const std::string IntlLocale::baseNameTag = "baseName";
const std::string IntlLocale::regionTag = "region";
const std::string IntlLocale::scriptTag = "script";
const std::string IntlLocale::calendarTag = "calendar";
const std::string IntlLocale::collationTag = "collation";
const std::string IntlLocale::hourCycleTag = "hourCycle";
const std::string IntlLocale::numberingSystemTag = "numberingSystem";
const std::string IntlLocale::numericTag = "numeric";
const std::string IntlLocale::caseFirstTag = "caseFirst";
const std::string calendarExtTag = "ca";
const std::string collationExtTag = "co";
const std::string hourCycleExtTag = "hc";
const std::string numberingSystemExtTag = "nu";
const std::string numericExtTag = "kn";
const std::string caseFirstExtTag = "kf";
const std::unordered_set<std::string> HOUR_CYCLE = { "h11", "h12", "h23", "h24" };
const std::unordered_set<std::string> CASE_FIRST = { "upper", "lower", "false" };
constexpr uint8_t INTL_INDEX_FOUR = 4;

IntlLocale::IntlLocale(const std::string& localeTag, const std::unordered_map<std::string, std::string>& configs,
    std::string& errMessage)
{
    if (!ApplyOptionsToTag(localeTag, configs)) {
        errMessage = APPLY_OPTION_ERROR_MESSAGE;
        HILOG_ERROR_I18N("IntlLocale::IntlLocale: ApplyOptionsToTag failed.");
        return;
    }
    icu::LocaleBuilder builder;
    if (!BuildOptionsTags(localeTag, configs, &builder)) {
        errMessage = APPLY_OPTION_ERROR_MESSAGE;
        HILOG_ERROR_I18N("IntlLocale::IntlLocale: BuildOptionsTags failed.");
        return;
    }
    if (!CheckOption(configs)) {
        errMessage = OUT_OF_RANGE_ERROR_MESSAGE;
        HILOG_ERROR_I18N("IntlLocale::IntlLocale: CheckOption failed.");
        return;
    }
    if (!InsertOptions(configs, &builder)) {
        errMessage = INSERT_OR_BUILD_ERROR_MESSAGE;
        HILOG_ERROR_I18N("IntlLocale::IntlLocale: InsertOptions failed.");
        return;
    }
    UErrorCode status = U_ZERO_ERROR;
    icuLocale = builder.build(status);
    if (U_FAILURE(status)) {
        errMessage = INSERT_OR_BUILD_ERROR_MESSAGE;
        HILOG_ERROR_I18N("IntlLocale::IntlLocale: Build icuLocale failed.");
        return;
    }
    icuLocale.canonicalize(status);
    if (U_FAILURE(status)) {
        errMessage = INSERT_OR_BUILD_ERROR_MESSAGE;
        HILOG_ERROR_I18N("IntlLocale::IntlLocale: icuLocale canonicalize failed.");
        return;
    }
    initSuccess = true;
}

IntlLocale::~IntlLocale()
{
}

bool IntlLocale::ApplyOptionsToTag(const std::string& localeTag, const std::unordered_map<std::string, std::string>& configs)
{
    if (localeTag.empty()) {
        HILOG_ERROR_I18N("IntlLocale::ApplyOptionsToTag: localeTag is empty.");
        return false;
    }
    if (!LocaleHelper::IsStructurallyValidLanguageTag(localeTag)) {
        HILOG_ERROR_I18N("IntlLocale::ApplyOptionsToTag: Verify structurally valid languageTag failed.");
        return false;
    }
    if (configs.find(languageTag) != configs.end()) {
        language = configs.at(languageTag);
        if (language.empty() || LocaleHelper::IsAlpha(language, INTL_INDEX_FOUR, INTL_INDEX_FOUR)) {
            HILOG_ERROR_I18N("IntlLocale::ApplyOptionsToTag: language is %{public}s.", language.c_str());
            return false;
        }
    }
    if (configs.find(scriptTag) != configs.end()) {
        script = configs.at(scriptTag);
        if (script.empty()) {
            HILOG_ERROR_I18N("IntlLocale::ApplyOptionsToTag: script is empty.");
            return false;
        }
    }
    if (configs.find(regionTag) != configs.end()) {
        region = configs.at(regionTag);
        if (region.empty()) {
            HILOG_ERROR_I18N("IntlLocale::ApplyOptionsToTag: region is empty.");
            return false;
        }
    }
    return true;
}

bool IntlLocale::BuildOptionsTags(const std::string& localeTag,
    const std::unordered_map<std::string, std::string>& configs, icu::LocaleBuilder* builder)
{
    if (builder == nullptr) {
        HILOG_ERROR_I18N("IntlLocale::BuildOptionsTags: builder is nullptr.");
        return false;
    }
    builder->setLanguageTag(localeTag);
    UErrorCode status = U_ZERO_ERROR;
    icu::Locale locale = builder->build(status);
    if (U_FAILURE(status)) {
        HILOG_ERROR_I18N("IntlLocale::BuildOptionsTags: Build locale failed, localeTag is %{public}s.",
            localeTag.c_str());
        return false;
    }
    locale.canonicalize(status);
    if (U_FAILURE(status)) {
        HILOG_ERROR_I18N("IntlLocale::BuildOptionsTags: locale canonicalize failed.");
        return false;
    }
    builder->setLocale(locale);

    if (configs.find(languageTag) != configs.end()) {
        builder->setLanguage(language);
    }
    if (configs.find(scriptTag) != configs.end()) {
        builder->setScript(script);
    }
    if (configs.find(regionTag) != configs.end()) {
        builder->setRegion(region);
    }

    builder->build(status);
    if (U_FAILURE(status)) {
        HILOG_ERROR_I18N("IntlLocale::BuildOptionsTags: Build locale failed, language is %{public}s, "
            "script is %{public}s, region is %{public}s.", language.c_str(), script.c_str(), region.c_str());
        return false;
    }
    return true;
}

bool IntlLocale::CheckOption(const std::unordered_map<std::string, std::string>& configs)
{
    if (configs.find(calendarTag) != configs.end()) {
        calendar = configs.at(calendarTag);
    }

    if (configs.find(collationTag) != configs.end()) {
        collation = configs.at(collationTag);
    }

    if (configs.find(hourCycleTag) != configs.end()) {
        hourCycle = configs.at(hourCycleTag);
        if (HOUR_CYCLE.find(hourCycle) == HOUR_CYCLE.end()) {
            HILOG_ERROR_I18N("IntlLocale::CheckOption: Check hourCycle failed, hourCycle is %{public}s.",
                hourCycle.c_str());
            return false;
        }
    }

    if (configs.find(numberingSystemTag) != configs.end()) {
        numberingSystem = configs.at(numberingSystemTag);
    }

    if (configs.find(numericTag) != configs.end()) {
        numeric = configs.at(numericTag);
    }

    if (configs.find(caseFirstTag) != configs.end()) {
        caseFirst = configs.at(caseFirstTag);
        if (CASE_FIRST.find(caseFirst) == CASE_FIRST.end()) {
            HILOG_ERROR_I18N("IntlLocale::CheckOption: Check caseFirst failed, caseFirst is %{public}s.",
                caseFirst.c_str());
            return false;
        }
    }
    return true;
}

bool IntlLocale::InsertOptions(const std::unordered_map<std::string, std::string>& configs, icu::LocaleBuilder* builder)
{
    if (configs.find(calendarTag) != configs.end()) {
        if (!uloc_toLegacyType(uloc_toLegacyKey(calendarExtTag.c_str()), calendar.c_str())) {
            HILOG_ERROR_I18N("IntlLocale::InsertOptions: Check calendar failed, calendar is %{public}s.",
                calendar.c_str());
            return false;
        }
        builder->setUnicodeLocaleKeyword(calendarExtTag.c_str(), calendar.c_str());
    }

    if (configs.find(collationTag) != configs.end()) {
        if (!uloc_toLegacyType(uloc_toLegacyKey(collationExtTag.c_str()), collation.c_str())) {
            HILOG_ERROR_I18N("IntlLocale::InsertOptions: Check collation failed, collation is %{public}s.",
                collation.c_str());
            return false;
        }
        builder->setUnicodeLocaleKeyword(collationExtTag.c_str(), collation.c_str());
    }

    if (configs.find(hourCycleTag) != configs.end()) {
        if (!uloc_toLegacyType(uloc_toLegacyKey(hourCycleExtTag.c_str()), hourCycle.c_str())) {
            HILOG_ERROR_I18N("IntlLocale::InsertOptions: Check hourCycle failed, hourCycle is %{public}s.",
                hourCycle.c_str());
            return false;
        }
        builder->setUnicodeLocaleKeyword(hourCycleExtTag.c_str(), hourCycle.c_str());
    }

    if (configs.find(caseFirstTag) != configs.end()) {
        if (!uloc_toLegacyType(uloc_toLegacyKey(caseFirstExtTag.c_str()), caseFirst.c_str())) {
            HILOG_ERROR_I18N("IntlLocale::InsertOptions: Check caseFirst failed, caseFirst is %{public}s.",
                caseFirst.c_str());
            return false;
        }
        builder->setUnicodeLocaleKeyword(caseFirstExtTag.c_str(), caseFirst.c_str());
    }

    if (configs.find(numericTag) != configs.end()) {
        if (!uloc_toLegacyType(uloc_toLegacyKey(numericExtTag.c_str()), numeric.c_str())) {
            HILOG_ERROR_I18N("IntlLocale::InsertOptions: Check numeric failed, numeric is %{public}s.",
                numeric.c_str());
            return false;
        }
        builder->setUnicodeLocaleKeyword(numericExtTag.c_str(), numeric.c_str());
    }

    if (configs.find(numberingSystemTag) != configs.end()) {
        if (!uloc_toLegacyType(uloc_toLegacyKey(numberingSystemExtTag.c_str()), numberingSystem.c_str())) {
            HILOG_ERROR_I18N("IntlLocale::InsertOptions: Check numberingSystem failed, numberingSystem is %{public}s.",
                numberingSystem.c_str());
            return false;
        }
        builder->setUnicodeLocaleKeyword(numberingSystemExtTag.c_str(), numberingSystem.c_str());
    }
    return true;
}

std::string IntlLocale::GetLanguage()
{
    if (!initSuccess) {
        return "";
    }
    return icuLocale.getLanguage();
}

std::string IntlLocale::GetScript()
{
    if (!initSuccess) {
        return "";
    }
    return icuLocale.getScript();
}

std::string IntlLocale::GetRegion()
{
    if (!initSuccess) {
        return "";
    }
    return icuLocale.getCountry();
}

std::string IntlLocale::GetBaseName()
{
    if (!initSuccess) {
        return "";
    }
    std::string result = icuLocale.getBaseName();
    std::replace(result.begin(), result.end(), '_', '-');
    return result;
}

std::string IntlLocale::GetCalendar()
{
    if (!initSuccess) {
        return "";
    }
    UErrorCode status = U_ZERO_ERROR;
    calendar = icuLocale.getUnicodeKeywordValue<std::string>(calendarExtTag, status);
    if (U_FAILURE(status)) {
        return "";
    }
    return calendar;
}

std::string IntlLocale::GetCollation()
{
    if (!initSuccess) {
        return "";
    }
    UErrorCode status = U_ZERO_ERROR;
    collation = icuLocale.getUnicodeKeywordValue<std::string>(collationExtTag, status);
    if (U_FAILURE(status)) {
        return "";
    }
    return collation;
}

std::string IntlLocale::GetHourCycle()
{
    if (!initSuccess) {
        return "";
    }
    UErrorCode status = U_ZERO_ERROR;
    hourCycle = icuLocale.getUnicodeKeywordValue<std::string>(hourCycleExtTag, status);
    if (U_FAILURE(status)) {
        return "";
    }
    return hourCycle;
}

std::string IntlLocale::GetNumberingSystem()
{
    if (!initSuccess) {
        return "";
    }
    UErrorCode status = U_ZERO_ERROR;
    numberingSystem = icuLocale.getUnicodeKeywordValue<std::string>(numberingSystemExtTag, status);
    if (U_FAILURE(status)) {
        return "";
    }
    return numberingSystem;
}

std::string IntlLocale::GetNumeric()
{
    if (!initSuccess) {
        return "";
    }
    UErrorCode status = U_ZERO_ERROR;
    numeric = icuLocale.getUnicodeKeywordValue<std::string>(numericExtTag, status);
    if (U_FAILURE(status)) {
        return "";
    }
    return numeric;
}

std::string IntlLocale::GetCaseFirst()
{
    if (!initSuccess) {
        return "";
    }
    UErrorCode status = U_ZERO_ERROR;
    caseFirst = icuLocale.getUnicodeKeywordValue<std::string>(caseFirstExtTag, status);
    if (U_FAILURE(status)) {
        return "";
    }
    return caseFirst;
}

std::string IntlLocale::ToString(std::string& errMessage)
{
    if (!initSuccess) {
        return "";
    }
    UErrorCode status = U_ZERO_ERROR;
    std::string result = icuLocale.toLanguageTag<std::string>(status);
    if (U_FAILURE(status)) {
        errMessage = INVALID_LOCALE_ERROR_MESSAGE;
        HILOG_ERROR_I18N("IntlLocale::ToString: Get languageTag from icuLocale failed.");
        return "";
    }
    return result;
}

std::string IntlLocale::Maximize()
{
    if (!initSuccess) {
        return "";
    }
    UErrorCode status = U_ZERO_ERROR;
    icu::Locale curLocale = icuLocale;
    curLocale.addLikelySubtags(status);
    if (U_FAILURE(status)) {
        HILOG_ERROR_I18N("IntlLocale::Maximize: Add likely subtags failed.");
        return "";
    }
    std::string result = curLocale.toLanguageTag<std::string>(status);
    if (U_FAILURE(status)) {
        HILOG_ERROR_I18N("IntlLocale::Maximize: Get languageTag from curLocale failed.");
        return "";
    }
    return result;
}

std::string IntlLocale::Minimize()
{
    if (!initSuccess) {
        return "";
    }
    UErrorCode status = U_ZERO_ERROR;
    icu::Locale curLocale = icuLocale;
    curLocale.minimizeSubtags(status);
    if (U_FAILURE(status)) {
        HILOG_ERROR_I18N("IntlLocale::Minimize: Minimize subtags failed.");
        return "";
    }
    std::string result = curLocale.toLanguageTag<std::string>(status);
    if (U_FAILURE(status)) {
        HILOG_ERROR_I18N("IntlLocale::Minimize: Get languageTag from curLocale failed.");
        return "";
    }
    return result;
}

bool IntlLocale::icuInitialized = IntlLocale::Init();

bool IntlLocale::Init()
{
    SetHwIcuDirectory();
    return true;
}
} // namespace I18n
} // namespace Global
} // namespace OHOS
