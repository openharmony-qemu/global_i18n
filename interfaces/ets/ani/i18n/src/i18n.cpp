/*
 * Copyright (c) 2021-2025 Huawei Device Co., Ltd.
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

#include <iostream>
#include <array>

#include "character.h"
#include "date_time_sequence.h"
#include "error_util.h"
#include "i18n.h"
#include "i18n_hilog.h"
#include "i18n_service_ability_client.h"
#include "locale_config.h"
#include "locale_matcher.h"
#include "preferred_language.h"

using namespace OHOS;
using namespace Global;
using namespace I18n;

static const std::string NAME_OF_TEMPERATURE = "type";
static const std::string TYPE_OF_TEMPERATURE = "TemperatureType";
static const std::string NAME_OF_WEEKDAY = "type";
static const std::string TYPE_OF_WEEKDAY = "WeekDay";
static const int OFFSET_OF_ENUM = 1;
static const char PATH_SEPARATOR = '/';
static const int SIZE_OF_LIST = 3;
static const int INDEX_ONE_OF_LIST = 0;
static const int INDEX_TWO_OF_LIST = 1;
static const int INDEX_THREE_OF_LIST = 2;

static std::unordered_map<std::string, UCalendarDateFields> g_fieldsMap {
    { "era", UCAL_ERA },
    { "year", UCAL_YEAR },
    { "month", UCAL_MONTH },
    { "week_of_year", UCAL_WEEK_OF_YEAR },
    { "week_of_month", UCAL_WEEK_OF_MONTH },
    { "date", UCAL_DATE },
    { "day_of_year", UCAL_DAY_OF_YEAR },
    { "day_of_week", UCAL_DAY_OF_WEEK },
    { "day_of_week_in_month", UCAL_DAY_OF_WEEK_IN_MONTH },
    { "ap_pm", UCAL_AM_PM },
    { "hour", UCAL_HOUR },
    { "hour_of_day", UCAL_HOUR_OF_DAY },
    { "minute", UCAL_MINUTE },
    { "second", UCAL_SECOND },
    { "millisecond", UCAL_MILLISECOND },
    { "zone_offset", UCAL_ZONE_OFFSET },
    { "dst_offset", UCAL_DST_OFFSET },
    { "year_woy", UCAL_YEAR_WOY },
    { "dow_local", UCAL_DOW_LOCAL },
    { "extended_year", UCAL_EXTENDED_YEAR },
    { "julian_day", UCAL_JULIAN_DAY },
    { "milliseconds_in_day", UCAL_MILLISECONDS_IN_DAY },
    { "is_leap_month", UCAL_IS_LEAP_MONTH },
};

static std::unordered_set<std::string> g_fieldsInFunctionAdd {
    "year", "month", "date", "hour", "minute", "second", "millisecond",
    "week_of_year", "week_of_month", "day_of_year", "day_of_week",
    "day_of_week_in_month", "hour_of_day", "milliseconds_in_day",
};

static std::unordered_map<std::string, CalendarType> g_typeMap {
    { "buddhist", CalendarType::BUDDHIST },
    { "chinese", CalendarType::CHINESE },
    { "coptic", CalendarType::COPTIC },
    { "ethiopic", CalendarType::ETHIOPIC },
    { "hebrew", CalendarType::HEBREW },
    { "gregory", CalendarType::GREGORY },
    { "indian", CalendarType::INDIAN },
    { "islamic_civil", CalendarType::ISLAMIC_CIVIL },
    { "islamic_tbla", CalendarType::ISLAMIC_TBLA },
    { "islamic_umalqura", CalendarType::ISLAMIC_UMALQURA },
    { "japanese", CalendarType::JAPANESE },
    { "persion", CalendarType::PERSIAN },
};

static std::string AniStrToString(ani_env *env, ani_ref aniStr)
{
    ani_string ani_str = static_cast<ani_string>(aniStr);
    ani_size strSize;
    if (ANI_OK != env->String_GetUTF8Size(ani_str, &strSize)) {
        HILOG_ERROR_I18N("Get utf8 size failed");
        return "";
    }

    std::vector<char> buffer(strSize + 1);
    ani_size bytes_written = 0;

    if (ANI_OK != env->String_GetUTF8(ani_str, buffer.data(), buffer.size(), &bytes_written)) {
        HILOG_ERROR_I18N("Create string failed");
        return "";
    }
    return std::string(buffer.data(), bytes_written);
}

static ani_string StringToAniStr(ani_env *env, const std::string &str)
{
    ani_string ret;
    if (ANI_OK != env->String_NewUTF8(str.c_str(), str.size(), &ret)) {
        HILOG_ERROR_I18N("Create ani string failed");
        return nullptr;
    }
    return ret;
}

static ani_object CreateArray(ani_env *env, const std::vector<std::string> strs)
{
    static const char *className = "Lescompat/Array;";
    ani_class cls;
    if (ANI_OK != env->FindClass(className, &cls)) {
        HILOG_ERROR_I18N("Find class '%{public}s' failed", className);
        return nullptr;
    }

    ani_method ctor;
    if (ANI_OK != env->Class_FindMethod(cls, "<ctor>", "I:V", &ctor)) {
        HILOG_ERROR_I18N("Find method '<ctor>' failed");
        return nullptr;
    }

    ani_object ret;
    if (ANI_OK != env->Object_New(cls, ctor, &ret, strs.size())) {
        HILOG_ERROR_I18N("New object '%{public}s' failed", className);
        return nullptr;
    }

    ani_method set;
    if (ANI_OK != env->Class_FindMethod(cls, "$_set", "ILstd/core/Object;:V", &set)) {
        HILOG_ERROR_I18N("Find method '$_set' failed");
        return ret;
    }

    for (size_t i = 0; i < strs.size(); i++) {
        if (ANI_OK != env->Object_CallMethod_Void(ret, set, i, StringToAniStr(env, strs[i]))) {
            HILOG_ERROR_I18N("Call method '$_set' failed");
            return ret;
        }
    }
    return ret;
}

static bool ParseStringArray(ani_env *env, ani_object obj, std::vector<std::string> &strs)
{
    static const char *className = "Lescompat/Array;";
    ani_class cls;
    if (ANI_OK != env->FindClass(className, &cls)) {
        HILOG_ERROR_I18N("Find class '%{public}s' failed", className);
        return false;
    }

    ani_method getLengthMethod;
    if (ANI_OK != env->Class_FindGetter(cls, "length", &getLengthMethod)) {
        HILOG_ERROR_I18N("Find getter 'length' failed");
        return false;
    }

    ani_double length;
    if (ANI_OK != env->Object_CallMethod_Double(obj, getLengthMethod, &length)) {
        HILOG_ERROR_I18N("Get 'length' failed");
        return false;
    }

    ani_method get;
    if (ANI_OK != env->Class_FindMethod(cls, "$_get", "I:Lstd/core/Object;", &get)) {
        HILOG_ERROR_I18N("Find method '$_get' failed");
        return false;
    }

    for (int i = 0; i < (int)length; i++) {
        ani_ref value;
        if (ANI_OK != env->Object_CallMethod_Ref(obj, get, &value, i)) {
            HILOG_ERROR_I18N("Call method '$_get' failed");
            return false;
        }
        strs.push_back(AniStrToString(env, static_cast<ani_string>(value)));
    }
    return true;
}

static bool GetBooleanMember(ani_env *env, ani_object options, const std::string name, bool& value)
{
    ani_ref ref;
    if (ANI_OK != env->Object_GetPropertyByName_Ref(options, name.c_str(), &ref)) {
        HILOG_ERROR_I18N("Get property '%{public}s' failed", name.c_str());
        return false;
    }

    ani_boolean isUndefined;
    if (ANI_OK != env->Reference_IsUndefined(ref, &isUndefined)) {
        HILOG_ERROR_I18N("Reference IsUndefined failed");
        return false;
    }
    if (isUndefined) {
        return false;
    }

    ani_boolean ret;
    if (ANI_OK != env->Object_CallMethodByName_Boolean(static_cast<ani_object>(ref), "unboxed", ":Z", &ret)) {
        HILOG_ERROR_I18N("Call method by name failed");
        return false;
    }
    value = ret;
    return true;
}

static bool GetStringMember(ani_env *env, ani_object options, const std::string name, std::string& value)
{
    ani_ref ref;
    if (ANI_OK != env->Object_GetPropertyByName_Ref(options, name.c_str(), &ref)) {
        HILOG_ERROR_I18N("Get property '%{public}s' failed", name.c_str());
        return false;
    }

    ani_boolean isUndefined;
    if (ANI_OK != env->Reference_IsUndefined(ref, &isUndefined)) {
        HILOG_ERROR_I18N("Reference IsUndefined failed");
        return false;
    }
    if (isUndefined) {
        return false;
    }

    value = AniStrToString(env, ref);
    return true;
}

static void SetNumberMember(ani_env *env, ani_object obj, const std::string name, const ani_int value)
{
    static const char *className = "Lstd/core/Int;";
    ani_class cls;
    if (ANI_OK != env->FindClass(className, &cls)) {
        HILOG_ERROR_I18N("Find class '%{public}s' failed", className);
        return;
    }

    ani_method ctor;
    if (ANI_OK != env->Class_FindMethod(cls, "<ctor>", "I:V", &ctor)) {
        HILOG_ERROR_I18N("Find method '<ctor>' failed");
        return;
    }

    ani_object intObj;
    if (ANI_OK != env->Object_New(cls, ctor, &intObj, value)) {
        HILOG_ERROR_I18N("New object '%{public}s' failed", className);
        return;
    }

    if (ANI_OK != env->Object_SetPropertyByName_Ref(obj, name.c_str(), intObj)) {
        HILOG_ERROR_I18N("Set property '%{public}s' failed", name.c_str());
        return;
    }
}

static void SetStringMember(ani_env *env, ani_object obj, const std::string name, const std::string value)
{
    if (value.empty()) {
        return;
    }

    if (ANI_OK != env->Object_SetPropertyByName_Ref(obj, name.c_str(), StringToAniStr(env, value))) {
        HILOG_ERROR_I18N("Set property '%{public}s' failed", name.c_str());
        return;
    }
}

static ani_enum_item GetEnumItemByIndex(ani_env* env, const char* enumClassName, const int index)
{
    ani_enum aniEnum;
    if (ANI_OK != env->FindEnum(enumClassName, &aniEnum)) {
        HILOG_ERROR_I18N("Find enum '%{public}s' failed", enumClassName);
        return nullptr;
    }

    ani_enum_item enumItem;
    if (ANI_OK != env->Enum_GetEnumItemByIndex(aniEnum, index, &enumItem)) {
        HILOG_ERROR_I18N("Get enumItem '%{public}s' failed", enumClassName);
        return nullptr;
    }
    return enumItem;
}

static ani_double GetDateValue(ani_env *env, ani_object date, const std::string method)
{
    static const char* className = "Lescompat/Date;";
    ani_class cls;
    if (ANI_OK != env->FindClass(className, &cls)) {
        HILOG_ERROR_I18N("Find class '%{public}s' failed", className);
        return -1;
    }

    ani_method get;
    if (ANI_OK != env->Class_FindMethod(cls, method.c_str(), nullptr, &get)) {
        HILOG_ERROR_I18N("Find method '%{public}s' failed", method.c_str());
        return -1;
    }

    ani_double ret;
    if (ANI_OK != env->Object_CallMethod_Double(date, get, &ret)) {
        HILOG_ERROR_I18N("Call method '%{public}s' failed", method.c_str());
        return -1;
    }
    return ret;
}

template<typename T>
static ani_object CreateAniObject(ani_env* env, const char* className, T* ptr)
{
    ani_class cls;
    if (ANI_OK != env->FindClass(className, &cls)) {
        HILOG_ERROR_I18N("Find class '%{public}s' failed", className);
        return nullptr;
    }

    ani_method ctor;
    if (ANI_OK != env->Class_FindMethod(cls, "<ctor>", "J:V", &ctor)) {
        HILOG_ERROR_I18N("Find method '<ctor>' failed");
        return nullptr;
    }

    ani_object obj;
    if (ANI_OK != env->Object_New(cls, ctor, &obj, reinterpret_cast<ani_long>(ptr))) {
        HILOG_ERROR_I18N("New object '%{public}s' failed", className);
        return nullptr;
    }
    return obj;
}

static ani_string GetDisplayCountry(ani_env *env, [[maybe_unused]] ani_object object,
    ani_string country, ani_string locale, ani_object sentenceCase)
{
    std::string countryBuf = AniStrToString(env, country);
    std::string displayLocaleBuf = AniStrToString(env, locale);
    LocaleInfo localeInfo(countryBuf);
    if (!LocaleConfig::IsValidRegion(countryBuf) && localeInfo.GetRegion() == "") {
        ErrorUtil::AniThrow(env, I18N_NOT_VALID, "country", "a valid region");
        return nullptr;
    } else if (!LocaleConfig::IsValidTag(displayLocaleBuf)) {
        ErrorUtil::AniThrow(env, I18N_NOT_VALID, "locale", "a valid locale");
        return nullptr;
    }

    bool sentenceCaseBool = true;
    ani_boolean isUndefined;
    if (ANI_OK != env->Reference_IsUndefined(sentenceCase, &isUndefined)) {
        HILOG_ERROR_I18N("Reference IsUndefined failed");
        return nullptr;
    }
    if (!isUndefined) {
        ani_boolean value;
        if (ANI_OK != env->Object_CallMethodByName_Boolean(sentenceCase, "unboxed", ":Z", &value)) {
            HILOG_ERROR_I18N("Unboxed Boolean failed");
            return nullptr;
        }
        sentenceCaseBool = value;
    }

    std::string str = LocaleConfig::GetDisplayRegion(countryBuf, displayLocaleBuf, sentenceCaseBool);
    return StringToAniStr(env, str);
}

static ani_string GetDisplayLanguage(ani_env *env, [[maybe_unused]] ani_object object,
    ani_string language, ani_string locale, ani_object sentenceCase)
{
    std::string displayLocaleBuf = AniStrToString(env, locale);
    if (!LocaleConfig::IsValidTag(displayLocaleBuf)) {
        ErrorUtil::AniThrow(env, I18N_NOT_VALID, "locale", "a valid locale");
        return nullptr;
    }

    bool sentenceCaseBool = true;
    ani_boolean isUndefined;
    if (ANI_OK != env->Reference_IsUndefined(sentenceCase, &isUndefined)) {
        HILOG_ERROR_I18N("Reference IsUndefined failed");
        return nullptr;
    }
    if (!isUndefined) {
        ani_boolean value;
        if (ANI_OK != env->Object_CallMethodByName_Boolean(sentenceCase, "unboxed", ":Z", &value)) {
            HILOG_ERROR_I18N("Unboxed Boolean failed");
            return nullptr;
        }
        sentenceCaseBool = value;
    }

    std::string str = LocaleConfig::GetDisplayLanguage(AniStrToString(env, language),
        displayLocaleBuf, sentenceCaseBool);
    return StringToAniStr(env, str);
}

static ani_object GetSystemLanguages(ani_env *env, [[maybe_unused]] ani_object object)
{
    std::unordered_set<std::string> systemLanguages = LocaleConfig::GetSystemLanguages();
    std::vector<std::string> vector(systemLanguages.begin(), systemLanguages.end());
    return CreateArray(env, vector);
}

static ani_object GetSystemCountries(ani_env *env, [[maybe_unused]] ani_object object, ani_string language)
{
    std::unordered_set<std::string> systemCountries =
        LocaleConfig::GetSystemCountries(AniStrToString(env, language));
    std::vector<std::string> vector(systemCountries.begin(), systemCountries.end());
    return CreateArray(env, vector);
}

static ani_boolean IsSuggested(ani_env *env, [[maybe_unused]] ani_object object,
    ani_string language, ani_string region)
{
    bool isSuggested = false;
    ani_boolean isUndefined;
    if (ANI_OK != env->Reference_IsUndefined(region, &isUndefined)) {
        HILOG_ERROR_I18N("Reference IsUndefined failed");
        return false;
    }
    if (!isUndefined) {
        isSuggested = LocaleConfig::IsSuggested(AniStrToString(env, language), AniStrToString(env, region));
    } else {
        isSuggested = LocaleConfig::IsSuggested(AniStrToString(env, language));
    }
    return isSuggested;
}

static ani_string GetSystemLanguage(ani_env *env, [[maybe_unused]] ani_object object)
{
    std::string str = LocaleConfig::GetSystemLanguage();
    return StringToAniStr(env, str);
}

static void SetSystemLanguage(ani_env *env, [[maybe_unused]] ani_object object, ani_string language)
{
    I18nErrorCode err = I18nServiceAbilityClient::SetSystemLanguage(AniStrToString(env, language));
    HILOG_INFO_I18N("I18nSystemAddon::SetSystemLanguageImpl with code %{public}d", static_cast<int32_t>(err));
    bool success = err == I18nErrorCode::SUCCESS;
    if (!success) {
        ErrorUtil::AniThrow(env, I18N_NO_PERMISSION, "", "");
    }
}

static ani_string GetSystemRegion(ani_env *env, [[maybe_unused]] ani_object object)
{
    std::string str = LocaleConfig::GetSystemRegion();
    return StringToAniStr(env, str);
}

static void SetSystemRegion(ani_env *env, [[maybe_unused]] ani_object object, ani_string region)
{
    I18nErrorCode err = I18nServiceAbilityClient::SetSystemRegion(AniStrToString(env, region));
    HILOG_INFO_I18N("I18nSystemAddon::SetSystemRegionImpl with code %{public}d", static_cast<int32_t>(err));
    bool success = err == I18nErrorCode::SUCCESS;
    if (!success) {
        ErrorUtil::AniThrow(env, I18N_NO_PERMISSION, "", "");
    }
}

static ani_string GetSystemLocale(ani_env *env, [[maybe_unused]] ani_object object)
{
    std::string str = LocaleConfig::GetSystemLocale();
    return StringToAniStr(env, str);
}

static void SetSystemLocale(ani_env *env, [[maybe_unused]] ani_object object, ani_string locale)
{
    I18nErrorCode err = I18nServiceAbilityClient::SetSystemLocale(AniStrToString(env, locale));
    bool success = err == I18nErrorCode::SUCCESS;
    if (!success) {
        ErrorUtil::AniThrow(env, I18N_NO_PERMISSION, "", "");
    }
}

static ani_boolean Is24HourClock([[maybe_unused]] ani_env *env, [[maybe_unused]] ani_object object)
{
    return LocaleConfig::Is24HourClock();
}

static void Set24HourClock([[maybe_unused]] ani_env *env, [[maybe_unused]] ani_object object, ani_boolean option)
{
    std::string optionStr = option ? "true" : "false";
    I18nErrorCode err = I18nServiceAbilityClient::Set24HourClock(optionStr);
    HILOG_INFO_I18N("I18nSystemAddon::Set24HourClock with code %{public}d", static_cast<int32_t>(err));
    bool success = err == I18nErrorCode::SUCCESS;
    if (!success) {
        ErrorUtil::AniThrow(env, I18N_NO_PERMISSION, "", "");
    }
}

static void AddPreferredLanguage(ani_env *env, [[maybe_unused]] ani_object object,
    ani_string language, ani_object number)
{
    int index = 1000000;
    ani_boolean isUndefined;
    if (ANI_OK != env->Reference_IsUndefined(number, &isUndefined)) {
        HILOG_ERROR_I18N("Reference IsUndefined failed");
        return;
    }
    if (!isUndefined) {
        if (ANI_OK != env->Object_CallMethodByName_Int(number, "unboxed", ":I", &index)) {
            HILOG_ERROR_I18N("Unboxed Boolean failed");
            return;
        }
    }
    I18nErrorCode err = I18nServiceAbilityClient::AddPreferredLanguage(AniStrToString(env, language), index);
    HILOG_INFO_I18N("I18nSystemAddon::AddPreferredLanguageImpl with code %{public}d", static_cast<int32_t>(err));
    if (err == I18nErrorCode::NO_PERMISSION) {
        ErrorUtil::AniThrow(env, I18N_NO_PERMISSION, "", "");
    }
    if (err != I18nErrorCode::SUCCESS) {
        ErrorUtil::AniThrow(env, I18N_NOT_VALID, "language", "a valid language");
    }
}

static void RemovePreferredLanguage([[maybe_unused]] ani_env *env, [[maybe_unused]] ani_object object, ani_double index)
{
    int len = static_cast<int>(PreferredLanguage::GetPreferredLanguageList().size());
    if ((index < 0 || index > len - 1)) {
        ErrorUtil::AniThrow(env, I18N_NOT_VALID, "index", "within the valid range");
        return;
    }

    I18nErrorCode err = I18nServiceAbilityClient::RemovePreferredLanguage(index);
    bool success = err == I18nErrorCode::SUCCESS;
    if (!success) {
        ErrorUtil::AniThrow(env, I18N_NO_PERMISSION, "", "");
    }
}

static ani_object GetPreferredLanguageList(ani_env *env, [[maybe_unused]] ani_object object)
{
    std::vector<std::string> languageList = PreferredLanguage::GetPreferredLanguageList();
    return CreateArray(env, languageList);
}

static ani_string GetFirstPreferredLanguage(ani_env *env, [[maybe_unused]] ani_object object)
{
    std::string language = PreferredLanguage::GetFirstPreferredLanguage();
    return StringToAniStr(env, language);
}

static void SetAppPreferredLanguage(ani_env *env, [[maybe_unused]] ani_object object, ani_string language)
{
    std::string localeTag = AniStrToString(env, language);
    UErrorCode icuStatus = U_ZERO_ERROR;
    icu::Locale locale = icu::Locale::forLanguageTag(localeTag, icuStatus);
    if (U_FAILURE(icuStatus) || !(IsValidLocaleTag(locale) || localeTag.compare("default") == 0)) {
        HILOG_ERROR_I18N("SetAppPreferredLanguage does not support this locale");
        ErrorUtil::AniThrow(env, I18N_NOT_VALID, "language", "a valid language");
        return;
    }

#ifdef SUPPORT_APP_PREFERRED_LANGUAGE
    I18nErrorCode errCode = I18nErrorCode::SUCCESS;
    PreferredLanguage::SetAppPreferredLanguage(localeTag, errCode);
    if (errCode != I18nErrorCode::SUCCESS) {
        HILOG_ERROR_I18N("SetAppPreferredLanguage: set app language to i18n app preferences failed.");
    }
#endif
}

static ani_string GetAppPreferredLanguage(ani_env *env, [[maybe_unused]] ani_object object)
{
#ifdef SUPPORT_APP_PREFERRED_LANGUAGE
    std::string language = PreferredLanguage::GetAppPreferredLanguage();
#else
    std::string language = PreferredLanguage::GetFirstPreferredLanguage();
#endif
    return StringToAniStr(env, language);
}

static void SetUsingLocalDigit([[maybe_unused]] ani_env *env, [[maybe_unused]] ani_object object, ani_boolean flag)
{
    I18nErrorCode err = I18nServiceAbilityClient::SetUsingLocalDigit(flag);
    HILOG_INFO_I18N("I18nSystemAddon::SetUsingLocalDigitAddonImpl with code %{public}d",
        static_cast<int32_t>(err));
    bool res = err == I18nErrorCode::SUCCESS;
    if (!res) {
        ErrorUtil::AniThrow(env, I18N_NO_PERMISSION, "", "");
    }
}

static ani_boolean GetUsingLocalDigit([[maybe_unused]] ani_env *env, [[maybe_unused]] ani_object object)
{
    return LocaleConfig::GetUsingLocalDigit();
}

static ani_string GetSimplifiedLanguage([[maybe_unused]] ani_env *env, [[maybe_unused]] ani_object object,
    ani_string language)
{
    std::string simplifiedLanguage = "";
    ani_boolean isUndefined;
    if (ANI_OK != env->Reference_IsUndefined(language, &isUndefined)) {
        HILOG_ERROR_I18N("Reference IsUndefined failed");
        return nullptr;
    }
    if (isUndefined) {
        simplifiedLanguage = LocaleConfig::GetSimplifiedSystemLanguage();
    } else {
        int code = 0;
        simplifiedLanguage = LocaleConfig::GetSimplifiedLanguage(AniStrToString(env, language), code);
        if (code != 0) {
            HILOG_ERROR_I18N("GetSimplifiedLanguage: language is not valid");
            ErrorUtil::AniThrow(env, I18N_NOT_VALID, "language", "a valid language");
            return nullptr;
        }
    }
    return StringToAniStr(env, simplifiedLanguage);
}

static void SetTemperatureType([[maybe_unused]] ani_env *env, [[maybe_unused]] ani_object object, ani_enum_item type)
{
    TemperatureType typeEnum = TemperatureType::CELSIUS;
    ani_int value;
    if (ANI_OK != env->EnumItem_GetValue_Int(type, &value)) {
        HILOG_ERROR_I18N("I18nSystemAddon::SetTemperatureType: parse temperature type failed.");
        return;
    }
    typeEnum = static_cast<TemperatureType>(value);

    I18nErrorCode err = I18nServiceAbilityClient::SetTemperatureType(typeEnum);
    if (err == I18nErrorCode::SUCCESS) {
        return;
    }
    switch (err) {
        case I18nErrorCode::NOT_SYSTEM_APP:
            ErrorUtil::AniThrow(env, I18N_NOT_SYSTEM_APP, "", "");
            break;
        case I18nErrorCode::NO_PERMISSION:
            ErrorUtil::AniThrow(env, I18N_NO_PERMISSION, "", "");
            break;
        case I18nErrorCode::INVALID_TEMPERATURE_TYPE:
            ErrorUtil::AniThrow(env, I18N_NOT_VALID, NAME_OF_TEMPERATURE, TYPE_OF_TEMPERATURE);
            break;
        default:
            HILOG_ERROR_I18N("I18nSystemAddon::SetTemperatureType: set temperature type failed.");
    }
}

static ani_enum_item GetTemperatureType([[maybe_unused]] ani_env *env, [[maybe_unused]] ani_object object)
{
    TemperatureType type = LocaleConfig::GetTemperatureType();
    int32_t index = static_cast<int32_t>(type) - OFFSET_OF_ENUM;
    static const char *enumName = "L@ohos/i18n/i18n/TemperatureType;";
    return GetEnumItemByIndex(env, enumName, index);
}

static ani_string GetTemperatureName([[maybe_unused]] ani_env *env, [[maybe_unused]] ani_object object,
    ani_enum_item type)
{
    TemperatureType typeEnum = TemperatureType::CELSIUS;
    ani_int value;
    if (ANI_OK != env->EnumItem_GetValue_Int(type, &value)) {
        HILOG_ERROR_I18N("I18nSystemAddon::GetTemperatureName: parse temperature type failed.");
        return nullptr;
    }
    typeEnum = static_cast<TemperatureType>(value);

    std::string temperatureName = LocaleConfig::GetTemperatureName(typeEnum);
    if (temperatureName.empty()) {
        ErrorUtil::AniThrow(env, I18N_NOT_VALID, NAME_OF_TEMPERATURE, TYPE_OF_TEMPERATURE);
        return nullptr;
    }
    return StringToAniStr(env, temperatureName);
}

static void SetFirstDayOfWeek([[maybe_unused]] ani_env *env, [[maybe_unused]] ani_object object, ani_enum_item type)
{
    ani_int weekDay;
    if (ANI_OK != env->EnumItem_GetValue_Int(type, &weekDay)) {
        HILOG_ERROR_I18N("I18nSystemAddon::SetFirstDayOfWeek: get week day failed.");
        return;
    }
    WeekDay typeEnum = static_cast<WeekDay>(weekDay);

    I18nErrorCode err = I18nServiceAbilityClient::SetFirstDayOfWeek(typeEnum);
    if (err == I18nErrorCode::SUCCESS) {
        return;
    }
    switch (err) {
        case I18nErrorCode::NOT_SYSTEM_APP:
            ErrorUtil::AniThrow(env, I18N_NOT_SYSTEM_APP, "", "");
            break;
        case I18nErrorCode::NO_PERMISSION:
            ErrorUtil::AniThrow(env, I18N_NO_PERMISSION, "", "");
            break;
        case I18nErrorCode::INVALID_WEEK_DAY:
            ErrorUtil::AniThrow(env, I18N_NOT_VALID, NAME_OF_WEEKDAY, TYPE_OF_WEEKDAY);
            break;
        default:
            HILOG_ERROR_I18N("I18nSystemAddon::SetFirstDayOfWeek: set first day of week failed.");
    }
}

static ani_enum_item GetFirstDayOfWeek([[maybe_unused]] ani_env *env, [[maybe_unused]] ani_object object)
{
    WeekDay type = LocaleConfig::GetFirstDayOfWeek();
    int32_t index = static_cast<int32_t>(type) - OFFSET_OF_ENUM;
    static const char *enumName = "L@ohos/i18n/i18n/WeekDay;";
    return GetEnumItemByIndex(env, enumName, index);
}

static bool GetUnitInfo(ani_env *env, ani_object obj, std::string &unit, std::string &system)
{
    std::string value;
    if (!GetStringMember(env, obj, "unit", value)) {
        return false;
    }
    unit = value;

    if (!GetStringMember(env, obj, "measureSystem", value)) {
        return false;
    }
    system = value;
    return true;
}

static ani_string UnitConvert(ani_env *env, [[maybe_unused]] ani_object object, ani_object fromUnit,
    ani_object toUnit, ani_double value, ani_string locale, ani_string style)
{
    std::string fromUnitStr;
    std::string fromMeasSys;
    std::string toUnitStr;
    std::string toMeasSys;
    if (!GetUnitInfo(env, fromUnit, fromUnitStr, fromMeasSys) || !GetUnitInfo(env, toUnit, toUnitStr, toMeasSys)) {
        HILOG_ERROR_I18N("Get UnitInfo failed");
        return nullptr;
    }

    int convertStatus = Convert(value, fromUnitStr, fromMeasSys, toUnitStr, toMeasSys);
    std::map<std::string, std::string> map = {};
    map.insert(std::make_pair("style", "unit"));
    if (!convertStatus) {
        map.insert(std::make_pair("unit", fromUnitStr));
    } else {
        map.insert(std::make_pair("unit", toUnitStr));
    }

    ani_boolean isUndefined;
    if (ANI_OK != env->Reference_IsUndefined(style, &isUndefined)) {
        HILOG_ERROR_I18N("Reference IsUndefined failed");
        return nullptr;
    }
    if (!isUndefined) {
        map.insert(std::make_pair("unitDisplay", AniStrToString(env, style)));
    }

    std::vector<std::string> localeTags;
    localeTags.push_back(AniStrToString(env, locale));
    std::unique_ptr<NumberFormat> numberFmt = std::make_unique<NumberFormat>(localeTags, map);
    std::string str = numberFmt->Format(value);
    return StringToAniStr(env, str);
}

static ani_string GetDateOrder(ani_env *env, [[maybe_unused]] ani_object object, ani_string locale)
{
    std::string value = DateTimeSequence::GetDateOrder(AniStrToString(env, locale));
    return StringToAniStr(env, value);
}

static ani_string GetTimePeriodName(ani_env *env, [[maybe_unused]] ani_object object,
    ani_double hour, ani_string locale)
{
    std::string localeTag;
    ani_boolean isUndefined;
    if (ANI_OK != env->Reference_IsUndefined(locale, &isUndefined)) {
        HILOG_ERROR_I18N("Reference IsUndefined failed");
        return nullptr;
    }
    if (!isUndefined) {
        localeTag = AniStrToString(env, locale);
    } else {
        localeTag = LocaleConfig::GetSystemLocale();
    }

    UErrorCode icuStatus = U_ZERO_ERROR;
    icu::Locale icuLocale = icu::Locale::forLanguageTag(localeTag, icuStatus);
    if (U_FAILURE(icuStatus) || !IsValidLocaleTag(icuLocale)) {
        ErrorUtil::AniThrow(env, I18N_NOT_VALID, "locale", "a valid locale");
        return nullptr;
    }
    icu::SimpleDateFormat* formatter = dynamic_cast<icu::SimpleDateFormat*>
        (icu::DateFormat::createDateInstance(icu::DateFormat::EStyle::kDefault, icuLocale));
    if (!formatter) {
        HILOG_ERROR_I18N("GetTimePeriodName Failed to create SimpleDateFormat");
        return nullptr;
    }
    formatter->applyPattern("B");

    icu::UnicodeString name;
    icu::Calendar *calendar = icu::Calendar::createInstance(icuLocale, icuStatus);
    if (calendar == nullptr) {
        delete formatter;
        return nullptr;
    }
    calendar->set(UCalendarDateFields::UCAL_HOUR_OF_DAY, hour);
    formatter->format(calendar->getTime(icuStatus), name);
    std::string result;
    name.toUTF8String(result);
    delete formatter;
    delete calendar;
    return StringToAniStr(env, result);
}

static LocaleInfo* ProcessJsParamLocale(ani_env *env, ani_string locale)
{
    std::string localeTag = AniStrToString(env, locale);
    UErrorCode icuStatus = U_ZERO_ERROR;
    icu::Locale icuLocale = icu::Locale::forLanguageTag(localeTag, icuStatus);
    if (U_FAILURE(icuStatus) || !IsValidLocaleTag(icuLocale)) {
        ErrorUtil::AniThrow(env, I18N_NOT_VALID, "locale", "a valid locale");
        return nullptr;
    }
    return new LocaleInfo(localeTag);
}

static bool ProcessJsParamLocaleList(ani_env *env, ani_object localeList, std::vector<LocaleInfo*> &candidateLocales,
    LocaleInfo *requestLocale)
{
    std::vector<std::string> localeTagList;
    if (!ParseStringArray(env, localeList, localeTagList)) {
        HILOG_ERROR_I18N("ProcessJsParamLocaleList: Failed to obtain the parameter.");
        return false;
    }
    if (localeTagList.size() == 0) {
        return true;
    }

    for (auto it = localeTagList.begin(); it != localeTagList.end(); ++it) {
        UErrorCode icuStatus = U_ZERO_ERROR;
        icu::Locale locale = icu::Locale::forLanguageTag(it->data(), icuStatus);
        if (U_FAILURE(icuStatus) || !IsValidLocaleTag(locale)) {
            HILOG_ERROR_I18N("GetBestMatchLocale param localeList Invalid: %{public}s.", it->data());
            ErrorUtil::AniThrow(env, I18N_NOT_VALID, "locale of localeList", "a valid locale");
            return false;
        }
        LocaleInfo *temp = new LocaleInfo(*it);
        if (LocaleMatcher::Match(requestLocale, temp)) {
            candidateLocales.push_back(temp);
        } else {
            delete temp;
        }
    }
    return true;
}

static void ReleaseParam(LocaleInfo *locale, std::vector<LocaleInfo*> &candidateLocales)
{
    delete locale;
    for (auto it = candidateLocales.begin(); it != candidateLocales.end(); ++it) {
        delete *it;
    }
}

static ani_string GetBestMatchLocale(ani_env *env, [[maybe_unused]] ani_object object,
    ani_string locale, ani_object localeList)
{
    LocaleInfo *requestLocale = ProcessJsParamLocale(env, locale);
    if (requestLocale == nullptr) {
        return nullptr;
    }

    std::vector<LocaleInfo*> candidateLocales;
    bool isValidParam = ProcessJsParamLocaleList(env, localeList, candidateLocales, requestLocale);
    if (!isValidParam) {
        ReleaseParam(requestLocale, candidateLocales);
        return nullptr;
    }

    std::string bestMatchLocaleTag = "";
    if (candidateLocales.size() > 0) {
        LocaleInfo *bestMatch = candidateLocales[0];
        for (size_t i = 1; i < candidateLocales.size(); ++i) {
            if (LocaleMatcher::IsMoreSuitable(bestMatch, candidateLocales[i], requestLocale) < 0) {
                bestMatch = candidateLocales[i];
            }
        }
        bestMatchLocaleTag = bestMatch->ToString();
    }
    ReleaseParam(requestLocale, candidateLocales);
    return StringToAniStr(env, bestMatchLocaleTag);
}

static ani_string GetThreeLetterLanguage(ani_env *env, [[maybe_unused]] ani_object object, ani_string locale)
{
    std::string language = GetISO3Language(AniStrToString(env, locale));
    return StringToAniStr(env, language);
}

static ani_string GetThreeLetterRegion(ani_env *env, [[maybe_unused]] ani_object object, ani_string locale)
{
    std::string country = GetISO3Country(AniStrToString(env, locale));
    return StringToAniStr(env, country);
}

static ani_string GetUnicodeWrappedFilePath(ani_env *env, [[maybe_unused]] ani_object object,
    ani_string path, ani_string delimiter, ani_object locale)
{
    char delimiterChar = PATH_SEPARATOR;
    ani_boolean isUndefined;
    if (ANI_OK != env->Reference_IsUndefined(delimiter, &isUndefined)) {
        HILOG_ERROR_I18N("Reference IsUndefined failed");
        return nullptr;
    }
    if (!isUndefined) {
        std::string result = AniStrToString(env, delimiter);
        if (result.length() != 1) {
            ErrorUtil::AniThrow(env, I18N_NOT_VALID, "delimiter", "a valid delimiter");
            delimiterChar = '\0';
        } else {
            delimiterChar = result.at(0);
        }
    }

    IntlAniAddon* obj = nullptr;
    if (ANI_OK != env->Reference_IsUndefined(locale, &isUndefined)) {
        HILOG_ERROR_I18N("Reference IsUndefined failed");
        return nullptr;
    }
    if (!isUndefined) {
        ani_long ptr;
        if (ANI_OK != env->Object_GetFieldByName_Long(locale, "nativeLocale", &ptr)) {
            HILOG_ERROR_I18N("Get Long nativeLocale failed");
            return nullptr;
        }
        obj = reinterpret_cast<IntlAniAddon*>(ptr);
    }

    std::string errorCode;
    std::shared_ptr<LocaleInfo> localeInfoPtr = obj == nullptr ? nullptr : obj->GetLocaleInfo();
    std::string result = LocaleConfig::GetUnicodeWrappedFilePath(AniStrToString(env, path),
        delimiterChar, localeInfoPtr, errorCode);
    if (!errorCode.empty()) {
        ErrorUtil::AniThrow(env, I18N_NOT_VALID, errorCode, "valid");
        return nullptr;
    }
    return StringToAniStr(env, result);
}

I18nPhoneNumberFormatAddon* I18nPhoneNumberFormatAddon::UnwrapAddon(ani_env *env, ani_object object)
{
    ani_long ptr;
    if (ANI_OK != env->Object_GetFieldByName_Long(object, "nativePhoneNumberFormat", &ptr)) {
        HILOG_ERROR_I18N("Get Long 'nativePhoneNumberFormat' failed");
        return nullptr;
    }
    return reinterpret_cast<I18nPhoneNumberFormatAddon*>(ptr);
}

ani_object I18nPhoneNumberFormatAddon::Create(ani_env *env, [[maybe_unused]] ani_class clazz,
    ani_string country, ani_object options)
{
    std::map<std::string, std::string> optionsStr;
    std::string typeStr;
    ani_boolean isUndefined;
    if (ANI_OK != env->Reference_IsUndefined(options, &isUndefined)) {
        HILOG_ERROR_I18N("Reference IsUndefined failed");
        return nullptr;
    }
    if (!isUndefined) {
        GetStringMember(env, options, "type", typeStr);
    }
    optionsStr.insert(std::make_pair("type", typeStr));

    std::unique_ptr<I18nPhoneNumberFormatAddon> obj = std::make_unique<I18nPhoneNumberFormatAddon>();
    obj->phonenumberfmt_ = PhoneNumberFormat::CreateInstance(AniStrToString(env, country), optionsStr);

    static const char *className = "L@ohos/i18n/i18n/PhoneNumberFormat;";
    ani_object PhoneNumberFormat_object = CreateAniObject(env, className, obj.get());
    obj.release();
    return PhoneNumberFormat_object;
}

ani_boolean I18nPhoneNumberFormatAddon::IsValidNumber(ani_env *env, ani_object object, ani_string number)
{
    auto obj = UnwrapAddon(env, object);
    if (obj == nullptr || obj->phonenumberfmt_ == nullptr) {
        HILOG_ERROR_I18N("IsValidPhoneNumber: GetPhoneNumberFormat object failed");
        return false;
    }
    return obj->phonenumberfmt_->isValidPhoneNumber(AniStrToString(env, number));
}

ani_string I18nPhoneNumberFormatAddon::Format(ani_env *env, ani_object object, ani_string number)
{
    auto obj = UnwrapAddon(env, object);
    if (obj == nullptr || obj->phonenumberfmt_ == nullptr) {
        HILOG_ERROR_I18N("Get PhoneNumberFormat object failed");
        return nullptr;
    }

    std::string formattedPhoneNumber = obj->phonenumberfmt_->format(AniStrToString(env, number));
    return StringToAniStr(env, formattedPhoneNumber);
}

ani_string I18nPhoneNumberFormatAddon::GetLocationName(ani_env *env, ani_object object,
    ani_string number, ani_string locale)
{
    auto obj = UnwrapAddon(env, object);
    if (obj == nullptr || obj->phonenumberfmt_ == nullptr) {
        HILOG_ERROR_I18N("GetLocationName: GetPhoneNumberFormat object failed");
        return nullptr;
    }

    std::string resStr = obj->phonenumberfmt_->getLocationName(AniStrToString(env, number),
        AniStrToString(env, locale));
    return StringToAniStr(env, resStr);
}

I18nCalendarAddon* I18nCalendarAddon::UnwrapAddon(ani_env *env, ani_object object)
{
    ani_long ptr;
    if (ANI_OK != env->Object_GetFieldByName_Long(object, "nativeCalendar", &ptr)) {
        HILOG_ERROR_I18N("Get Long 'nativeCalendar' failed");
        return nullptr;
    }
    return reinterpret_cast<I18nCalendarAddon*>(ptr);
}

ani_object I18nCalendarAddon::GetCalendar(ani_env *env, ani_string locale, ani_string type)
{
    std::unique_ptr<I18nCalendarAddon> obj = std::make_unique<I18nCalendarAddon>();
    ani_boolean isUndefined;
    if (ANI_OK != env->Reference_IsUndefined(type, &isUndefined)) {
        HILOG_ERROR_I18N("Reference IsUndefined failed");
        return nullptr;
    }
    CalendarType calendartype = CalendarType::UNDEFINED;
    if (!isUndefined) {
        std::string typeStr = AniStrToString(env, type);
        if (g_typeMap.find(typeStr) != g_typeMap.end()) {
            calendartype = g_typeMap[typeStr];
        }
    }

    obj->calendar_ = std::make_unique<I18nCalendar>(AniStrToString(env, locale), calendartype);
    static const char* className = "L@ohos/i18n/i18n/Calendar;";
    ani_object calendar_object = CreateAniObject(env, className, obj.get());
    obj.release();
    return calendar_object;
}

void I18nCalendarAddon::SetTimeByDate(ani_env *env, ani_object object, ani_object date)
{
    I18nCalendarAddon* obj = UnwrapAddon(env, object);
    if (obj == nullptr || obj->calendar_ == nullptr) {
        HILOG_ERROR_I18N("SetTime: Get calendar object failed");
        return;
    }
    obj->calendar_->SetTime(GetDateValue(env, date, "valueOf"));
}

void I18nCalendarAddon::SetTime(ani_env *env, ani_object object, ani_double time)
{
    I18nCalendarAddon* obj = UnwrapAddon(env, object);
    if (obj == nullptr || obj->calendar_ == nullptr) {
        HILOG_ERROR_I18N("SetTime: Get calendar object failed");
        return;
    }
    obj->calendar_->SetTime(time);
}

void I18nCalendarAddon::Set(ani_env *env, ani_object object, ani_double year, ani_double month,
    ani_double date, ani_object hour, ani_object minute, ani_object second)
{
    I18nCalendarAddon* obj = UnwrapAddon(env, object);
    if (obj == nullptr || obj->calendar_ == nullptr) {
        HILOG_ERROR_I18N("Set: Get calendar object failed");
        return;
    }
    obj->calendar_->Set(year, month, date);

    ani_boolean isUndefined;
    if (ANI_OK != env->Reference_IsUndefined(hour, &isUndefined)) {
        HILOG_ERROR_I18N("Reference IsUndefined failed");
        return;
    }
    if (!isUndefined) {
        ani_double hourValue;
        if (ANI_OK != env->Object_CallMethodByName_Double(hour, "unboxed", ":D", &hourValue)) {
            HILOG_ERROR_I18N("Unboxed Double failed");
            return;
        }
        obj->calendar_->Set(UCalendarDateFields::UCAL_HOUR_OF_DAY, hourValue);
    }

    if (ANI_OK != env->Reference_IsUndefined(minute, &isUndefined)) {
        HILOG_ERROR_I18N("Reference IsUndefined failed");
        return;
    }
    if (!isUndefined) {
        ani_double minuteValue;
        if (ANI_OK != env->Object_CallMethodByName_Double(minute, "unboxed", ":D", &minuteValue)) {
            HILOG_ERROR_I18N("Unboxed Double failed");
            return;
        }
        obj->calendar_->Set(UCalendarDateFields::UCAL_MINUTE, minuteValue);
    }

    if (ANI_OK != env->Reference_IsUndefined(second, &isUndefined)) {
        HILOG_ERROR_I18N("Reference IsUndefined failed");
        return;
    }
    if (!isUndefined) {
        ani_double secondValue;
        if (ANI_OK != env->Object_CallMethodByName_Double(second, "unboxed", ":D", &secondValue)) {
            HILOG_ERROR_I18N("Unboxed Double failed");
            return;
        }
        obj->calendar_->Set(UCalendarDateFields::UCAL_SECOND, secondValue);
    }
}

void I18nCalendarAddon::SetTimeZone(ani_env *env, ani_object object, ani_string timezone)
{
    I18nCalendarAddon* obj = UnwrapAddon(env, object);
    if (obj == nullptr || obj->calendar_ == nullptr) {
        HILOG_ERROR_I18N("SetTimeZone: Get calendar object failed");
        return;
    }
    obj->calendar_->SetTimeZone(AniStrToString(env, timezone));
}

ani_string I18nCalendarAddon::GetTimeZone(ani_env *env, ani_object object)
{
    I18nCalendarAddon* obj = UnwrapAddon(env, object);
    if (obj == nullptr || obj->calendar_ == nullptr) {
        HILOG_ERROR_I18N("GetTimeZone: Get calendar object failed");
        return nullptr;
    }

    std::string str = obj->calendar_->GetTimeZone();
    return StringToAniStr(env, str);
}

ani_double I18nCalendarAddon::GetFirstDayOfWeek(ani_env *env, ani_object object)
{
    I18nCalendarAddon* obj = UnwrapAddon(env, object);
    if (obj == nullptr || obj->calendar_ == nullptr) {
        HILOG_ERROR_I18N("GetFirstDayOfWeek: Get calendar object failed");
        return -1;
    }
    return obj->calendar_->GetFirstDayOfWeek();
}

void I18nCalendarAddon::SetFirstDayOfWeek(ani_env *env, ani_object object, ani_double value)
{
    I18nCalendarAddon* obj = UnwrapAddon(env, object);
    if (obj == nullptr || obj->calendar_ == nullptr) {
        HILOG_ERROR_I18N("SetFirstDayOfWeek: Get calendar object failed");
        return;
    }
    obj->calendar_->SetFirstDayOfWeek(value);
}

ani_double I18nCalendarAddon::GetMinimalDaysInFirstWeek(ani_env *env, ani_object object)
{
    I18nCalendarAddon* obj = UnwrapAddon(env, object);
    if (obj == nullptr || obj->calendar_ == nullptr) {
        HILOG_ERROR_I18N("GetMinimalDaysInFirstWeek: Get calendar object failed");
        return -1;
    }
    return obj->calendar_->GetMinimalDaysInFirstWeek();
}

void I18nCalendarAddon::SetMinimalDaysInFirstWeek(ani_env *env, ani_object object, ani_double value)
{
    I18nCalendarAddon* obj = UnwrapAddon(env, object);
    if (obj == nullptr || obj->calendar_ == nullptr) {
        HILOG_ERROR_I18N("SetMinimalDaysInFirstWeek: Get calendar object failed");
        return;
    }
    obj->calendar_->SetMinimalDaysInFirstWeek(value);
}

ani_double I18nCalendarAddon::Get(ani_env *env, ani_object object, ani_string field)
{
    std::string str = AniStrToString(env, field);
    if (g_fieldsMap.find(str) == g_fieldsMap.end()) {
        HILOG_ERROR_I18N("Invalid field");
        return -1;
    }

    I18nCalendarAddon* obj = UnwrapAddon(env, object);
    if (obj == nullptr || obj->calendar_ == nullptr) {
        HILOG_ERROR_I18N("Get: Get calendar object failed");
        return -1;
    }
    return obj->calendar_->Get(g_fieldsMap[str]);
}

ani_string I18nCalendarAddon::GetDisplayName(ani_env *env, ani_object object, ani_string locale)
{
    I18nCalendarAddon* obj = UnwrapAddon(env, object);
    if (obj == nullptr || obj->calendar_ == nullptr) {
        HILOG_ERROR_I18N("GetDisplayName: Get calendar object failed");
        return nullptr;
    }

    std::string localeTag = AniStrToString(env, locale);
    std::string name = obj->calendar_->GetDisplayName(localeTag);
    return StringToAniStr(env, name);
}

ani_boolean I18nCalendarAddon::IsWeekend(ani_env *env, ani_object object, ani_object date)
{
    I18nCalendarAddon* obj = UnwrapAddon(env, object);
    if (obj == nullptr || obj->calendar_ == nullptr) {
        HILOG_ERROR_I18N("IsWeekend: Get calendar object failed");
        return false;
    }

    bool isWeekEnd = false;
    ani_boolean isUndefined;
    if (ANI_OK != env->Reference_IsUndefined(date, &isUndefined)) {
        HILOG_ERROR_I18N("Reference IsUndefined failed");
        return false;
    }
    if (isUndefined) {
        isWeekEnd = obj->calendar_->IsWeekend();
    } else {
        UErrorCode error = U_ZERO_ERROR;
        isWeekEnd = obj->calendar_->IsWeekend(GetDateValue(env, date, "valueOf"), error);
    }
    return isWeekEnd;
}

void I18nCalendarAddon::Add(ani_env *env, ani_object object, ani_string field, ani_double amount)
{
    std::string str = AniStrToString(env, field);
    if (g_fieldsInFunctionAdd.find(str) == g_fieldsInFunctionAdd.end()) {
        HILOG_ERROR_I18N("Parameter rangs do not match");
        ErrorUtil::AniThrow(env, I18N_NOT_VALID, "field", "a valid field");
        return;
    }

    I18nCalendarAddon* obj = UnwrapAddon(env, object);
    if (obj == nullptr || obj->calendar_ == nullptr) {
        HILOG_ERROR_I18N("Add: Get calendar object failed");
        return;
    }
    obj->calendar_->Add(g_fieldsMap[str], amount);
}

ani_double I18nCalendarAddon::GetTimeInMillis(ani_env *env, ani_object object)
{
    bool flag = true;
    I18nCalendarAddon* obj = UnwrapAddon(env, object);
    if (obj == nullptr || obj->calendar_ == nullptr) {
        HILOG_ERROR_I18N("GetTimeInMillis: Get calendar object failed");
        flag = false;
    }

    UDate temp = 0;
    if (flag) {
        temp = obj->calendar_->GetTimeInMillis();
    }
    return temp;
}

ani_double I18nCalendarAddon::CompareDays(ani_env *env, ani_object object, ani_object date)
{
    I18nCalendarAddon* obj = UnwrapAddon(env, object);
    if (obj == nullptr || obj->calendar_ == nullptr) {
        HILOG_ERROR_I18N("CompareDays: Get calendar object failed");
        return 0;
    }
    return obj->calendar_->CompareDays(GetDateValue(env, date, "valueOf"));
}

I18nBreakIteratorAddon* I18nBreakIteratorAddon::UnwrapAddon(ani_env *env, ani_object object)
{
    ani_long ptr;
    if (ANI_OK != env->Object_GetFieldByName_Long(object, "nativeBreakIterator", &ptr)) {
        HILOG_ERROR_I18N("Get Long 'nativeBreakIterator' failed");
        return nullptr;
    }
    return reinterpret_cast<I18nBreakIteratorAddon*>(ptr);
}

ani_object I18nBreakIteratorAddon::GetLineInstance(ani_env *env, ani_string locale)
{
    std::unique_ptr<I18nBreakIteratorAddon> obj = std::make_unique<I18nBreakIteratorAddon>();
    obj->brkiter_ = std::make_unique<I18nBreakIterator>(AniStrToString(env, locale));

    static const char* className = "L@ohos/i18n/i18n/BreakIterator;";
    ani_object breakIterator_object = CreateAniObject(env, className, obj.get());
    obj.release();
    return breakIterator_object;
}

ani_double I18nBreakIteratorAddon::Current(ani_env *env, ani_object object)
{
    I18nBreakIteratorAddon* obj = UnwrapAddon(env, object);
    if (obj == nullptr || obj->brkiter_ == nullptr) {
        HILOG_ERROR_I18N("Current: Get BreakIterator object failed");
        return -1;
    }
    return obj->brkiter_->Current();
}

ani_double I18nBreakIteratorAddon::First(ani_env *env, ani_object object)
{
    I18nBreakIteratorAddon* obj = UnwrapAddon(env, object);
    if (obj == nullptr || obj->brkiter_ == nullptr) {
        HILOG_ERROR_I18N("First: Get BreakIterator object failed");
        return -1;
    }
    return obj->brkiter_->First();
}

ani_double I18nBreakIteratorAddon::Last(ani_env *env, ani_object object)
{
    I18nBreakIteratorAddon* obj = UnwrapAddon(env, object);
    if (obj == nullptr || obj->brkiter_ == nullptr) {
        HILOG_ERROR_I18N("Last: Get BreakIterator object failed");
        return -1;
    }
    return obj->brkiter_->Last();
}

ani_double I18nBreakIteratorAddon::Next(ani_env *env, ani_object object, ani_object index)
{
    I18nBreakIteratorAddon* obj = UnwrapAddon(env, object);
    if (obj == nullptr || obj->brkiter_ == nullptr) {
        HILOG_ERROR_I18N("Next: Get BreakIterator object failed");
        return -1;
    }

    int value = 1;
    ani_boolean isUndefined;
    if (ANI_OK != env->Reference_IsUndefined(index, &isUndefined)) {
        HILOG_ERROR_I18N("Reference IsUndefined failed");
        return -1;
    }
    if (!isUndefined) {
        if (ANI_OK != env->Object_CallMethodByName_Int(index, "unboxed", ":I", &value)) {
            HILOG_ERROR_I18N("Unbox Int failed");
            return -1;
        }
    }
    return obj->brkiter_->Next(value);
}

ani_double I18nBreakIteratorAddon::Previous(ani_env *env, ani_object object)
{
    I18nBreakIteratorAddon* obj = UnwrapAddon(env, object);
    if (obj == nullptr || obj->brkiter_ == nullptr) {
        HILOG_ERROR_I18N("Previous: Get BreakIterator object failed");
        return -1;
    }
    return obj->brkiter_->Previous();
}

void I18nBreakIteratorAddon::SetLineBreakText(ani_env *env, ani_object object, ani_string text)
{
    I18nBreakIteratorAddon* obj = UnwrapAddon(env, object);
    if (obj == nullptr || obj->brkiter_ == nullptr) {
        HILOG_ERROR_I18N("SetText: Get BreakIterator object failed");
        return;
    }
    obj->brkiter_->SetText(AniStrToString(env, text).c_str());
}

ani_double I18nBreakIteratorAddon::Following(ani_env *env, ani_object object, ani_double offset)
{
    I18nBreakIteratorAddon* obj = UnwrapAddon(env, object);
    if (obj == nullptr || obj->brkiter_ == nullptr) {
        HILOG_ERROR_I18N("Following: Get BreakIterator object failed");
        return -1;
    }
    return obj->brkiter_->Following(offset);
}

ani_string I18nBreakIteratorAddon::GetLineBreakText(ani_env *env, ani_object object)
{
    I18nBreakIteratorAddon* obj = UnwrapAddon(env, object);
    if (obj == nullptr || obj->brkiter_ == nullptr) {
        HILOG_ERROR_I18N("GetText: Get BreakIterator object failed");
        return nullptr;
    }

    std::string temp;
    obj->brkiter_->GetText(temp);
    return StringToAniStr(env, temp);
}

ani_boolean I18nBreakIteratorAddon::IsBoundary(ani_env *env, ani_object object, ani_double offset)
{
    I18nBreakIteratorAddon* obj = UnwrapAddon(env, object);
    if (obj == nullptr || obj->brkiter_ == nullptr) {
        HILOG_ERROR_I18N("IsBoundary: Get BreakIterator object failed");
        return false;
    }
    return obj->brkiter_->IsBoundary(offset);
}

I18nIndexUtilAddon* I18nIndexUtilAddon::UnwrapAddon(ani_env *env, ani_object object)
{
    ani_long ptr;
    if (ANI_OK != env->Object_GetFieldByName_Long(object, "nativeIndexUtil", &ptr)) {
        HILOG_ERROR_I18N("Get Long 'nativeIndexUtil' failed");
        return nullptr;
    }
    return reinterpret_cast<I18nIndexUtilAddon*>(ptr);
}

ani_object I18nIndexUtilAddon::GetInstance(ani_env *env, ani_string string)
{
    std::string localeTag = "";
    ani_boolean isUndefined;
    if (ANI_OK != env->Reference_IsUndefined(string, &isUndefined)) {
        HILOG_ERROR_I18N("Reference IsUndefined failed");
        return nullptr;
    }
    if (!isUndefined) {
        localeTag = AniStrToString(env, string);
    }

    std::unique_ptr<I18nIndexUtilAddon> obj = std::make_unique<I18nIndexUtilAddon>();
    obj->indexUtil_ = std::make_unique<IndexUtil>(localeTag);

    static const char* className = "L@ohos/i18n/i18n/IndexUtil;";
    ani_object indexUtil_object = CreateAniObject(env, className, obj.get());
    obj.release();
    return indexUtil_object;
}

ani_object I18nIndexUtilAddon::GetIndexList(ani_env *env, ani_object object)
{
    I18nIndexUtilAddon* obj = UnwrapAddon(env, object);
    if (obj == nullptr || obj->indexUtil_ == nullptr) {
        HILOG_ERROR_I18N("GetIndexList: GetPhoneNumberFormat object failed");
        return nullptr;
    }
    std::vector<std::string> indexList = obj->indexUtil_->GetIndexList();
    return CreateArray(env, indexList);
}

void I18nIndexUtilAddon::AddLocale(ani_env *env, ani_object object, ani_string locale)
{
    I18nIndexUtilAddon* obj = UnwrapAddon(env, object);
    if (obj == nullptr || obj->indexUtil_ == nullptr) {
        HILOG_ERROR_I18N("AddLocale: Get IndexUtil object failed");
        return;
    }
    obj->indexUtil_->AddLocale(AniStrToString(env, locale));
}

ani_object I18nIndexUtilAddon::GetIndex(ani_env *env, ani_object object, ani_string text)
{
    I18nIndexUtilAddon* obj = UnwrapAddon(env, object);
    if (obj == nullptr || obj->indexUtil_ == nullptr) {
        HILOG_ERROR_I18N("GetIndex: Get IndexUtil object failed");
        return nullptr;
    }

    std::string index = obj->indexUtil_->GetIndex(AniStrToString(env, text));
    return StringToAniStr(env, index);
}

ani_boolean UnicodeIsDigit(ani_env *env, [[maybe_unused]] ani_object object, ani_string ch)
{
    return IsDigit(AniStrToString(env, ch));
}

ani_boolean UnicodeIsSpaceChar(ani_env *env, [[maybe_unused]] ani_object object, ani_string ch)
{
    return IsSpaceChar(AniStrToString(env, ch));
}

ani_boolean UnicodeIsWhitespace(ani_env *env, [[maybe_unused]] ani_object object, ani_string ch)
{
    return IsWhiteSpace(AniStrToString(env, ch));
}

ani_boolean UnicodeIsRTL(ani_env *env, [[maybe_unused]] ani_object object, ani_string ch)
{
    return IsRTLCharacter(AniStrToString(env, ch));
}

ani_boolean UnicodeIsIdeograph(ani_env *env, [[maybe_unused]] ani_object object, ani_string ch)
{
    return IsIdeoGraphic(AniStrToString(env, ch));
}

ani_boolean UnicodeIsLetter(ani_env *env, [[maybe_unused]] ani_object object, ani_string ch)
{
    return IsLetter(AniStrToString(env, ch));
}

ani_boolean UnicodeIsLowerCase(ani_env *env, [[maybe_unused]] ani_object object, ani_string ch)
{
    return IsLowerCase(AniStrToString(env, ch));
}

ani_boolean UnicodeIsUpperCase(ani_env *env, [[maybe_unused]] ani_object object, ani_string ch)
{
    return IsUpperCase(AniStrToString(env, ch));
}

ani_string UnicodeGetType(ani_env *env, [[maybe_unused]] ani_object object, ani_string ch)
{
    std::string type = GetType(AniStrToString(env, ch));
    return StringToAniStr(env, type);
}

I18nTimeZoneAddon* I18nTimeZoneAddon::UnwrapAddon(ani_env *env, ani_object object)
{
    ani_long ptr;
    if (ANI_OK != env->Object_GetFieldByName_Long(object, "nativeTimeZone", &ptr)) {
        HILOG_ERROR_I18N("Get Long 'nativeTimeZone' failed");
        return nullptr;
    }
    return reinterpret_cast<I18nTimeZoneAddon*>(ptr);
}

ani_object I18nTimeZoneAddon::GetTimeZone(ani_env *env, ani_string zoneID)
{
    std::unique_ptr<I18nTimeZoneAddon> obj = std::make_unique<I18nTimeZoneAddon>();
    ani_boolean isUndefined;
    if (ANI_OK != env->Reference_IsUndefined(zoneID, &isUndefined)) {
        HILOG_ERROR_I18N("Reference IsUndefined failed");
        return nullptr;
    }
    std::string zoneIDStr = "";
    if (!isUndefined) {
        zoneIDStr = AniStrToString(env, zoneID);
    }
    obj->timezone_ = I18nTimeZone::CreateInstance(zoneIDStr, true);

    static const char* className = "L@ohos/i18n/i18n/TimeZone;";
    ani_object timeZone_object = CreateAniObject(env, className, obj.get());
    obj.release();
    return timeZone_object;
}

ani_string I18nTimeZoneAddon::GetID(ani_env *env, ani_object object)
{
    I18nTimeZoneAddon* obj = UnwrapAddon(env, object);
    if (obj == nullptr || obj->timezone_ == nullptr) {
        HILOG_ERROR_I18N("GetID: Get TimeZone object failed");
        return nullptr;
    }

    std::string result = obj->timezone_->GetID();
    return StringToAniStr(env, result);
}

ani_string I18nTimeZoneAddon::GetDisplayName(ani_env *env, ani_object object, ani_string locale, ani_object isDST)
{
    I18nTimeZoneAddon* obj = UnwrapAddon(env, object);
    if (obj == nullptr || obj->timezone_ == nullptr) {
        HILOG_ERROR_I18N("GetTimeZoneDisplayName: Get TimeZone object failed");
        return nullptr;
    }

    bool localeFlag = false;
    ani_boolean isUndefined;
    if (ANI_OK != env->Reference_IsUndefined(locale, &isUndefined)) {
        HILOG_ERROR_I18N("Reference IsUndefined failed");
        return nullptr;
    }
    if (!isUndefined) {
        localeFlag = true;
    }

    bool DSTFlag = false;
    ani_boolean isDSTBool;
    if (ANI_OK != env->Reference_IsUndefined(isDST, &isUndefined)) {
        HILOG_ERROR_I18N("Reference IsUndefined failed");
        return nullptr;
    }
    if (!isUndefined) {
        DSTFlag = true;
        if (ANI_OK != env->Object_CallMethodByName_Boolean(isDST, "unboxed", ":Z", &isDSTBool)) {
            HILOG_ERROR_I18N("Unbox Boolean failed");
            return nullptr;
        }
    }

    std::string result;
    if (localeFlag && DSTFlag) {
        result = obj->timezone_->GetDisplayName(AniStrToString(env, locale), isDSTBool);
    }
    if (localeFlag && !DSTFlag) {
        result = obj->timezone_->GetDisplayName(AniStrToString(env, locale));
    }
    if (!localeFlag && !DSTFlag) {
        result = obj->timezone_->GetDisplayName();
    }
    return StringToAniStr(env, result);
}

ani_double I18nTimeZoneAddon::GetRawOffset(ani_env *env, ani_object object)
{
    I18nTimeZoneAddon* obj = UnwrapAddon(env, object);
    if (obj == nullptr || obj->timezone_ == nullptr) {
        HILOG_ERROR_I18N("GetRawOffset: Get TimeZone object failed");
        return -1;
    }
    return obj->timezone_->GetRawOffset();
}

ani_double I18nTimeZoneAddon::GetOffset(ani_env *env, ani_object object, ani_object date)
{
    I18nTimeZoneAddon* obj = UnwrapAddon(env, object);
    if (obj == nullptr || obj->timezone_ == nullptr) {
        HILOG_ERROR_I18N("GetOffset: Get TimeZone object failed");
        return -1;
    }

    double dateDouble = 0;
    ani_boolean isUndefined;
    if (ANI_OK != env->Reference_IsUndefined(date, &isUndefined)) {
        HILOG_ERROR_I18N("Reference IsUndefined failed");
        return -1;
    }
    if (!isUndefined) {
        if (ANI_OK != env->Object_CallMethodByName_Double(date, "unboxed", ":D", &dateDouble)) {
            HILOG_ERROR_I18N("Unbox Double failed");
            return -1;
        }
    } else {
        auto time = std::chrono::system_clock::now();
        auto since_epoch = time.time_since_epoch();
        auto millis = std::chrono::duration_cast<std::chrono::milliseconds>(since_epoch);
        dateDouble = (double)millis.count();
    }
    return obj->timezone_->GetOffset(dateDouble);
}

ani_object I18nTimeZoneAddon::GetAvailableIDs(ani_env *env, [[maybe_unused]] ani_object object)
{
    I18nErrorCode errorCode = I18nErrorCode::SUCCESS;
    std::set<std::string> timezoneIDs = I18nTimeZone::GetAvailableIDs(errorCode);
    if (errorCode != I18nErrorCode::SUCCESS) {
        return nullptr;
    }

    std::vector<std::string> vector(timezoneIDs.begin(), timezoneIDs.end());
    return CreateArray(env, vector);
}

ani_object I18nTimeZoneAddon::GetAvailableZoneCityIDs(ani_env *env, [[maybe_unused]] ani_object object)
{
    std::unordered_set<std::string> cityIDs = I18nTimeZone::GetAvailableZoneCityIDs();
    std::vector<std::string> vector(cityIDs.begin(), cityIDs.end());
    return CreateArray(env, vector);
}

ani_object I18nTimeZoneAddon::GetCityDisplayName(ani_env *env, [[maybe_unused]] ani_object object,
    ani_string cityID, ani_string locale)
{
    std::string cityIDStr = AniStrToString(env, cityID);
    std::string localeStr = AniStrToString(env, locale);
    std::string name = I18nTimeZone::GetCityDisplayName(cityIDStr, localeStr);
    return StringToAniStr(env, name);
}

ani_object I18nTimeZoneAddon::GetTimezoneFromCity(ani_env *env, [[maybe_unused]] ani_object object, ani_string cityID)
{
    std::string cityIDStr = AniStrToString(env, cityID);
    std::unique_ptr<I18nTimeZoneAddon> obj = std::make_unique<I18nTimeZoneAddon>();
    obj->timezone_ = I18nTimeZone::CreateInstance(cityIDStr, false);

    static const char* className = "L@ohos/i18n/i18n/TimeZone;";
    ani_object timeZone_object = CreateAniObject(env, className, obj.get());
    obj.release();
    return timeZone_object;
}

ani_object I18nTimeZoneAddon::GetTimezonesByLocation(ani_env *env, [[maybe_unused]] ani_object object,
    ani_double longitude, ani_double latitude)
{
    std::vector<std::string> tempList = I18nTimeZone::GetTimezoneIdByLocation(longitude, latitude);
    static const char *className = "Lescompat/Array;";
    ani_class cls;
    if (ANI_OK != env->FindClass(className, &cls)) {
        HILOG_ERROR_I18N("Find class '%{public}s' failed", className);
        return nullptr;
    }

    ani_method ctor;
    if (ANI_OK != env->Class_FindMethod(cls, "<ctor>", "I:V", &ctor)) {
        HILOG_ERROR_I18N("Find method '<ctor>' failed");
        return nullptr;
    }

    ani_object ret;
    if (ANI_OK != env->Object_New(cls, ctor, &ret, tempList.size())) {
        HILOG_ERROR_I18N("New object '%{public}s' failed", className);
        return nullptr;
    }

    ani_method set;
    if (ANI_OK != env->Class_FindMethod(cls, "$_set", "ILstd/core/Object;:V", &set)) {
        HILOG_ERROR_I18N("Find method '$_set' failed");
        return nullptr;
    }

    for (size_t i = 0; i < tempList.size(); i++) {
        ani_object timezone = GetTimezoneFromCity(env, object, StringToAniStr(env, tempList[i]));
        if (ANI_OK != env->Object_CallMethod_Void(ret, set, i, timezone)) {
            HILOG_ERROR_I18N("Call method '$_set' failed");
            return nullptr;
        }
    }
    return ret;
}

I18nTransliteratorAddon* I18nTransliteratorAddon::UnwrapAddon(ani_env *env, ani_object object)
{
    ani_long ptr;
    if (ANI_OK != env->Object_GetFieldByName_Long(object, "nativeTransliterator", &ptr)) {
        HILOG_ERROR_I18N("Get Long 'nativeTransliterator' failed");
        return nullptr;
    }
    return reinterpret_cast<I18nTransliteratorAddon*>(ptr);
}

ani_object I18nTransliteratorAddon::GetAvailableIDs(ani_env *env, [[maybe_unused]] ani_object object)
{
    UErrorCode icuStatus = U_ZERO_ERROR;
    icu::StringEnumeration *strenum = icu::Transliterator::getAvailableIDs(icuStatus);
    if (U_FAILURE(icuStatus)) {
        HILOG_ERROR_I18N("Failed to get available ids");
        if (strenum) {
            delete strenum;
        }
        return nullptr;
    }

    std::vector<std::string> availableIDs;
    uint32_t i = 0;
    const char *temp = nullptr;
    if (strenum == nullptr) {
        return nullptr;
    }
    while ((temp = strenum->next(nullptr, icuStatus)) != nullptr) {
        if (U_FAILURE(icuStatus)) {
            break;
        }
        availableIDs.push_back(temp);
        ++i;
    }
    delete strenum;
    return CreateArray(env, availableIDs);
}

ani_object I18nTransliteratorAddon::GetInstance(ani_env *env, [[maybe_unused]] ani_object object, ani_string id)
{
    std::unique_ptr<I18nTransliteratorAddon> obj = std::make_unique<I18nTransliteratorAddon>();
    UErrorCode status = U_ZERO_ERROR;
    icu::UnicodeString unistr = icu::UnicodeString::fromUTF8(AniStrToString(env, id));
    icu::Transliterator *trans = icu::Transliterator::createInstance(unistr, UTransDirection::UTRANS_FORWARD, status);
    if (U_FAILURE(status) || (trans == nullptr)) {
        return nullptr;
    }

    obj->transliterator_ = std::unique_ptr<icu::Transliterator>(trans);
    static const char* className = "L@ohos/i18n/i18n/Transliterator;";
    ani_object transliterator_object = CreateAniObject(env, className, obj.get());
    obj.release();
    return transliterator_object;
}

ani_string I18nTransliteratorAddon::Transform(ani_env *env, ani_object object, ani_string text)
{
    I18nTransliteratorAddon* obj = UnwrapAddon(env, object);
    if (obj == nullptr || obj->transliterator_ == nullptr) {
        HILOG_ERROR_I18N("Get Transliterator object failed");
        return nullptr;
    }

    icu::UnicodeString unistr = icu::UnicodeString::fromUTF8(AniStrToString(env, text));
    obj->transliterator_->transliterate(unistr);

    std::string temp;
    unistr.toUTF8String(temp);
    return StringToAniStr(env, temp);
}

I18nNormalizerAddon* I18nNormalizerAddon::UnwrapAddon(ani_env *env, ani_object object)
{
    ani_long ptr;
    if (ANI_OK != env->Object_GetFieldByName_Long(object, "nativeNormalizer", &ptr)) {
        HILOG_ERROR_I18N("Get Long 'nativeNormalizer' failed");
        return nullptr;
    }
    return reinterpret_cast<I18nNormalizerAddon*>(ptr);
}

ani_object I18nNormalizerAddon::GetInstance(ani_env *env, [[maybe_unused]] ani_object object, ani_enum_item mode)
{
    int32_t normalizerMode;
    if (ANI_OK != env->EnumItem_GetValue_Int(mode, &normalizerMode)) {
        HILOG_ERROR_I18N("Enum get value 'normalizerMode' failed");
        return nullptr;
    }

    if (normalizerMode != NORMALIZER_MODE_NFC && normalizerMode != NORMALIZER_MODE_NFD &&
        normalizerMode != NORMALIZER_MODE_NFKC && normalizerMode != NORMALIZER_MODE_NFKD) {
        ErrorUtil::AniThrow(env, I18N_NOT_FOUND, "mode", "a valid mode");
        return nullptr;
    }

    std::unique_ptr<I18nNormalizerAddon> obj = std::make_unique<I18nNormalizerAddon>();
    I18nNormalizerMode modeEnum = I18nNormalizerMode(normalizerMode);
    I18nErrorCode errorCode = I18nErrorCode::SUCCESS;
    obj->normalizer_ = std::make_unique<I18nNormalizer>(modeEnum, errorCode);

    static const char* className = "L@ohos/i18n/i18n/Normalizer;";
    ani_object normalizer_object = CreateAniObject(env, className, obj.get());
    obj.release();
    return normalizer_object;
}

ani_string I18nNormalizerAddon::Normalize(ani_env *env, ani_object object, ani_string text)
{
    I18nNormalizerAddon* obj = UnwrapAddon(env, object);
    if (obj == nullptr || obj->normalizer_ == nullptr) {
        HILOG_ERROR_I18N("Get Normalizer object failed");
        return nullptr;
    }

    std::string textStr = AniStrToString(env, text);
    I18nErrorCode errorCode = I18nErrorCode::SUCCESS;
    std::string normalizedText = obj->normalizer_->Normalize(textStr.c_str(), static_cast<int32_t>(textStr.length()),
        errorCode);
    if (errorCode != I18nErrorCode::SUCCESS) {
        return nullptr;
    }
    return StringToAniStr(env, normalizedText);
}

I18nSysLocaleMgrAddon* I18nSysLocaleMgrAddon::UnwrapAddon(ani_env *env, ani_object object)
{
    ani_long ptr;
    if (ANI_OK != env->Object_GetFieldByName_Long(object, "nativeSysLocaleMgr", &ptr)) {
        HILOG_ERROR_I18N("Get Long 'nativeSysLocaleMgr' failed");
        return nullptr;
    }
    return reinterpret_cast<I18nSysLocaleMgrAddon*>(ptr);
}

ani_object I18nSysLocaleMgrAddon::Create(ani_env *env, [[maybe_unused]] ani_class clazz)
{
    std::unique_ptr<I18nSysLocaleMgrAddon> obj = std::make_unique<I18nSysLocaleMgrAddon>();
    obj->systemLocaleManager_ = std::make_unique<SystemLocaleManager>();

    static const char *className = "L@ohos/i18n/i18n/SystemLocaleManager;";
    ani_object SysLocaleMgr_object = CreateAniObject(env, className, obj.get());
    obj.release();
    return SysLocaleMgr_object;
}

static ani_object CreateLocaleItem(ani_env *env, LocaleItem localeItem)
{
    static const char *className = "L@ohos/i18n/i18n/LocaleItemInner;";
    ani_class cls;
    if (ANI_OK != env->FindClass(className, &cls)) {
        HILOG_ERROR_I18N("Find class '%{public}s' failed", className);
        return nullptr;
    }

    ani_method ctor;
    if (ANI_OK != env->Class_FindMethod(cls, "<ctor>", nullptr, &ctor)) {
        HILOG_ERROR_I18N("Find method '<ctor>' failed");
        return nullptr;
    }

    ani_object ret;
    if (ANI_OK != env->Object_New(cls, ctor, &ret)) {
        HILOG_ERROR_I18N("New object '%{public}s' failed", className);
        return nullptr;
    }

    SetStringMember(env, ret, "id", localeItem.id);
    SetStringMember(env, ret, "displayName", localeItem.displayName);
    SetStringMember(env, ret, "localName", localeItem.localName);

    static const char *enumName = "L@ohos/i18n/i18n/SuggestionType;";
    if (ANI_OK != env->Object_SetPropertyByName_Ref(ret, "suggestionType", GetEnumItemByIndex(env,
        enumName, static_cast<int>(localeItem.suggestionType)))) {
        HILOG_ERROR_I18N("Set property 'suggestionType' failed");
    }
    return ret;
}

static ani_object CreateLocaleItemArray(ani_env *env, std::vector<LocaleItem> LocaleItemList)
{
    static const char *className = "Lescompat/Array;";
    ani_class cls;
    if (ANI_OK != env->FindClass(className, &cls)) {
        HILOG_ERROR_I18N("Find class '%{public}s' failed", className);
        return nullptr;
    }

    ani_method ctor;
    if (ANI_OK != env->Class_FindMethod(cls, "<ctor>", "I:V", &ctor)) {
        HILOG_ERROR_I18N("Find method '<ctor>' failed");
        return nullptr;
    }

    ani_object ret;
    if (ANI_OK != env->Object_New(cls, ctor, &ret, LocaleItemList.size())) {
        HILOG_ERROR_I18N("New object '%{public}s' failed", className);
        return nullptr;
    }

    ani_method set;
    if (ANI_OK != env->Class_FindMethod(cls, "$_set", "ILstd/core/Object;:V", &set)) {
        HILOG_ERROR_I18N("Find method '$_set' failed");
        return ret;
    }

    for (size_t i = 0; i < LocaleItemList.size(); i++) {
        if (ANI_OK != env->Object_CallMethod_Void(ret, set, i, CreateLocaleItem(env, LocaleItemList[i]))) {
            HILOG_ERROR_I18N("Call method '$_set' failed");
            return ret;
        }
    }
    return ret;
}

ani_object I18nSysLocaleMgrAddon::GetLanguageInfoArray(ani_env *env, ani_object object,
    ani_object languages, ani_object options)
{
    std::vector<std::string> languageList;
    if (!ParseStringArray(env, languages, languageList)) {
        return nullptr;
    }

    SortOptions sortOptions;
    std::string localeTag;
    bool isUseLocalName = true;
    bool isSuggestedFirst = true;

    ani_boolean isUndefined;
    if (ANI_OK != env->Reference_IsUndefined(options, &isUndefined)) {
        HILOG_ERROR_I18N("Reference IsUndefined failed");
        return nullptr;
    }
    if (isUndefined) {
        localeTag = LocaleConfig::GetSystemLocale();
    } else {
        std::string className = "L@ohos/i18n/i18n/SortOptionsInner;";
        if (!GetStringMember(env, options, "locale", localeTag)) {
            localeTag = LocaleConfig::GetSystemLocale();
        }
        if (!GetBooleanMember(env, options, "isUseLocalName", isUseLocalName)) {
            isUseLocalName = true;
        }
        if (!GetBooleanMember(env, options, "isSuggestedFirst", isSuggestedFirst)) {
            isSuggestedFirst = true;
        }
    }

    sortOptions.localeTag = localeTag;
    sortOptions.isUseLocalName = isUseLocalName;
    sortOptions.isSuggestedFirst = isSuggestedFirst;

    I18nSysLocaleMgrAddon* obj = UnwrapAddon(env, object);
    if (obj == nullptr || obj->systemLocaleManager_ == nullptr) {
        HILOG_ERROR_I18N("GetLanguageInfoArray: Get SystemLocaleManager object failed");
        return nullptr;
    }

    I18nErrorCode err;
    std::vector<LocaleItem> localeItemList =
        obj->systemLocaleManager_->GetLanguageInfoArray(languageList, sortOptions, err);
    if (err == I18nErrorCode::NOT_SYSTEM_APP) {
        ErrorUtil::AniThrow(env, I18N_NOT_SYSTEM_APP, "", "");
        return nullptr;
    }
    return CreateLocaleItemArray(env, localeItemList);
}

ani_object I18nSysLocaleMgrAddon::GetRegionInfoArray(ani_env *env, ani_object object,
    ani_object regions, ani_object options)
{
    std::vector<std::string> countryList;
    if (!ParseStringArray(env, regions, countryList)) {
        return nullptr;
    }

    SortOptions sortOptions;
    std::string localeTag;
    bool isUseLocalName = false;
    bool isSuggestedFirst = true;

    ani_boolean isUndefined;
    if (ANI_OK != env->Reference_IsUndefined(options, &isUndefined)) {
        HILOG_ERROR_I18N("Reference IsUndefined failed");
        return nullptr;
    }
    if (isUndefined) {
        localeTag = LocaleConfig::GetSystemLocale();
    } else {
        std::string className = "L@ohos/i18n/i18n/SortOptionsInner;";
        if (!GetStringMember(env, options, "locale", localeTag)) {
            localeTag = LocaleConfig::GetSystemLocale();
        }
        if (!GetBooleanMember(env, options, "isUseLocalName", isUseLocalName)) {
            isUseLocalName = false;
        }
        if (!GetBooleanMember(env, options, "isSuggestedFirst", isSuggestedFirst)) {
            isSuggestedFirst = true;
        }
    }

    sortOptions.localeTag = localeTag;
    sortOptions.isUseLocalName = isUseLocalName;
    sortOptions.isSuggestedFirst = isSuggestedFirst;

    I18nSysLocaleMgrAddon* obj = UnwrapAddon(env, object);
    if (obj == nullptr || obj->systemLocaleManager_ == nullptr) {
        HILOG_ERROR_I18N("GetCountryInfoArray: Get SystemLocaleManager object failed");
        return nullptr;
    }

    I18nErrorCode err;
    std::vector<LocaleItem> localeItemList =
        obj->systemLocaleManager_->GetCountryInfoArray(countryList, sortOptions, err);
    if (err == I18nErrorCode::NOT_SYSTEM_APP) {
        ErrorUtil::AniThrow(env, I18N_NOT_SYSTEM_APP, "", "");
        return nullptr;
    }
    return CreateLocaleItemArray(env, localeItemList);
}

static ani_object CreateTimeZoneCityItem(ani_env *env, TimeZoneCityItem item)
{
    static const char *className = "L@ohos/i18n/i18n/TimeZoneCityItemInner;";
    ani_class cls;
    if (ANI_OK != env->FindClass(className, &cls)) {
        HILOG_ERROR_I18N("Find class '%{public}s' failed", className);
        return nullptr;
    }

    ani_method ctor;
    if (ANI_OK != env->Class_FindMethod(cls, "<ctor>", nullptr, &ctor)) {
        HILOG_ERROR_I18N("Find method '<ctor>' failed");
        return nullptr;
    }

    ani_object ret;
    if (ANI_OK != env->Object_New(cls, ctor, &ret)) {
        HILOG_ERROR_I18N("New object '%{public}s' failed", className);
        return nullptr;
    }

    SetStringMember(env, ret, "zoneId", item.zoneId);
    SetStringMember(env, ret, "cityId", item.cityId);
    SetStringMember(env, ret, "cityDisplayName", item.cityDisplayName);
    SetStringMember(env, ret, "zoneDisplayName", item.zoneDisplayName);
    SetNumberMember(env, ret, "rawOffset", item.rawOffset);

    if (ANI_OK != env->Object_SetPropertyByName_Double(ret, "offset", item.offset)) {
        HILOG_ERROR_I18N("Set property 'offset' failed");
    }
    return ret;
}

ani_object I18nSysLocaleMgrAddon::GetTimeZoneCityItemArray(ani_env *env, [[maybe_unused]] ani_object object)
{
    I18nErrorCode err;
    std::vector<TimeZoneCityItem> timezoneCityItemList = SystemLocaleManager::GetTimezoneCityInfoArray(err);
    if (err == I18nErrorCode::NOT_SYSTEM_APP) {
        ErrorUtil::AniThrow(env, I18N_NOT_SYSTEM_APP, "", "");
        return nullptr;
    }

    static const char *className = "Lescompat/Array;";
    ani_class cls;
    if (ANI_OK != env->FindClass(className, &cls)) {
        HILOG_ERROR_I18N("Find class '%{public}s' failed", className);
        return nullptr;
    }

    ani_method ctor;
    if (ANI_OK != env->Class_FindMethod(cls, "<ctor>", "I:V", &ctor)) {
        HILOG_ERROR_I18N("Find method '<ctor>' failed");
        return nullptr;
    }

    ani_object ret;
    if (ANI_OK != env->Object_New(cls, ctor, &ret, timezoneCityItemList.size())) {
        HILOG_ERROR_I18N("New object '%{public}s' failed", className);
        return nullptr;
    }

    ani_method set;
    if (ANI_OK != env->Class_FindMethod(cls, "$_set", "ILstd/core/Object;:V", &set)) {
        HILOG_ERROR_I18N("Find method '$_set' failed");
        return ret;
    }

    for (size_t i = 0; i < timezoneCityItemList.size(); i++) {
        if (ANI_OK != env->Object_CallMethod_Void(ret, set, i, CreateTimeZoneCityItem(env, timezoneCityItemList[i]))) {
            HILOG_ERROR_I18N("Call method '$_set' failed");
            return ret;
        }
    }
    return ret;
}

I18nHolidayManagerAddon* I18nHolidayManagerAddon::UnwrapAddon(ani_env *env, ani_object object)
{
    ani_long ptr;
    if (ANI_OK != env->Object_GetFieldByName_Long(object, "nativeHolidayManager", &ptr)) {
        HILOG_ERROR_I18N("Get Long 'nativeHolidayManager' failed");
        return nullptr;
    }
    return reinterpret_cast<I18nHolidayManagerAddon*>(ptr);
}

ani_object I18nHolidayManagerAddon::Create(ani_env *env, [[maybe_unused]] ani_class clazz, ani_string icsPath)
{
    std::unique_ptr<I18nHolidayManagerAddon> obj = std::make_unique<I18nHolidayManagerAddon>();
    obj->holidayManager_ = std::make_unique<HolidayManager>(AniStrToString(env, icsPath).c_str());

    static const char *className = "L@ohos/i18n/i18n/HolidayManager;";
    ani_object HolidayManager_object = CreateAniObject(env, className, obj.get());
    obj.release();
    return HolidayManager_object;
}

ani_boolean I18nHolidayManagerAddon::IsHoliday(ani_env *env, ani_object object, ani_object date)
{
    I18nHolidayManagerAddon* obj = UnwrapAddon(env, object);
    if (obj == nullptr || obj->holidayManager_ == nullptr) {
        HILOG_ERROR_I18N("IsHoliday: Get HolidayManager object failed");
        return false;
    }

    bool res = false;
    ani_boolean isUndefined;
    if (ANI_OK != env->Reference_IsUndefined(date, &isUndefined)) {
        HILOG_ERROR_I18N("Reference IsUndefined failed");
        return false;
    }
    if (isUndefined) {
        res = obj->holidayManager_->IsHoliday();
    } else {
        std::vector<int> list;
        list.push_back(GetDateValue(env, date, "getFullYear"));
        list.push_back(GetDateValue(env, date, "getMonth") + MONTH_INCREASE_ONE);
        list.push_back(GetDateValue(env, date, "getDate"));
        if (list.size() == SIZE_OF_LIST) {
            res = obj->holidayManager_->IsHoliday(list.at(INDEX_ONE_OF_LIST),
                list.at(INDEX_TWO_OF_LIST), list.at(INDEX_THREE_OF_LIST));
        }
    }
    return res;
}

static ani_object CreateHolidayLocalName(ani_env *env, HolidayLocalName localName)
{
    static const char *className = "L@ohos/i18n/i18n/HolidayLocalNameInner;";
    ani_class cls;
    if (ANI_OK != env->FindClass(className, &cls)) {
        HILOG_ERROR_I18N("Find class '%{public}s' failed", className);
        return nullptr;
    }

    ani_method ctor;
    if (ANI_OK != env->Class_FindMethod(cls, "<ctor>", nullptr, &ctor)) {
        HILOG_ERROR_I18N("Find method '<ctor>' failed");
        return nullptr;
    }

    ani_object ret;
    if (ANI_OK != env->Object_New(cls, ctor, &ret)) {
        HILOG_ERROR_I18N("New object '%{public}s' failed", className);
        return nullptr;
    }

    SetStringMember(env, ret, "language", localName.language);
    SetStringMember(env, ret, "name", localName.name);
    return ret;
}

static ani_object CreateHolidayLocalNameArray(ani_env *env, std::vector<HolidayLocalName> localNames)
{
    static const char *className = "Lescompat/Array;";
    ani_class cls;
    if (ANI_OK != env->FindClass(className, &cls)) {
        HILOG_ERROR_I18N("Find class '%{public}s' failed", className);
        return nullptr;
    }

    ani_method ctor;
    if (ANI_OK != env->Class_FindMethod(cls, "<ctor>", "I:V", &ctor)) {
        HILOG_ERROR_I18N("Find method '<ctor>' failed");
        return nullptr;
    }

    ani_object ret;
    if (ANI_OK != env->Object_New(cls, ctor, &ret, localNames.size())) {
        HILOG_ERROR_I18N("New object '%{public}s' failed", className);
        return nullptr;
    }

    ani_method set;
    if (ANI_OK != env->Class_FindMethod(cls, "$_set", "ILstd/core/Object;:V", &set)) {
        HILOG_ERROR_I18N("Find method '$_set' failed");
        return ret;
    }

    for (size_t i = 0; i < localNames.size(); i++) {
        if (ANI_OK != env->Object_CallMethod_Void(ret, set, i, CreateHolidayLocalName(env, localNames[i]))) {
            HILOG_ERROR_I18N("Call method '$_set' failed");
            return ret;
        }
    }
    return ret;
}

static ani_object CreateHolidayInfoItem(ani_env *env, HolidayInfoItem item)
{
    static const char *className = "L@ohos/i18n/i18n/HolidayInfoItemInner;";
    ani_class cls;
    if (ANI_OK != env->FindClass(className, &cls)) {
        HILOG_ERROR_I18N("Find class '%{public}s' failed", className);
        return nullptr;
    }

    ani_method ctor;
    if (ANI_OK != env->Class_FindMethod(cls, "<ctor>", nullptr, &ctor)) {
        HILOG_ERROR_I18N("Find method '<ctor>' failed");
        return nullptr;
    }

    ani_object ret;
    if (ANI_OK != env->Object_New(cls, ctor, &ret)) {
        HILOG_ERROR_I18N("New object '%{public}s' failed", className);
        return nullptr;
    }

    SetStringMember(env, ret, "baseName", item.baseName);
    SetNumberMember(env, ret, "year", item.year);
    SetNumberMember(env, ret, "month", item.month);
    SetNumberMember(env, ret, "day", item.day);

    if (ANI_OK != env->Object_SetPropertyByName_Ref(ret, "localNames",
        CreateHolidayLocalNameArray(env, item.localNames))) {
        HILOG_ERROR_I18N("Set property 'localNames' failed");
    }
    return ret;
}

static ani_object GetHolidayInfoItemResult(ani_env *env, std::vector<HolidayInfoItem> itemList)
{
    static const char *className = "Lescompat/Array;";
    ani_class cls;
    if (ANI_OK != env->FindClass(className, &cls)) {
        HILOG_ERROR_I18N("Find class '%{public}s' failed", className);
        return nullptr;
    }

    ani_method ctor;
    if (ANI_OK != env->Class_FindMethod(cls, "<ctor>", "I:V", &ctor)) {
        HILOG_ERROR_I18N("Find method '<ctor>' failed");
        return nullptr;
    }

    ani_object ret;
    if (ANI_OK != env->Object_New(cls, ctor, &ret, itemList.size())) {
        HILOG_ERROR_I18N("New object '%{public}s' failed", className);
        return nullptr;
    }

    ani_method set;
    if (ANI_OK != env->Class_FindMethod(cls, "$_set", "ILstd/core/Object;:V", &set)) {
        HILOG_ERROR_I18N("Find method '$_set' failed");
        return ret;
    }

    for (size_t i = 0; i < itemList.size(); i++) {
        if (ANI_OK != env->Object_CallMethod_Void(ret, set, i, CreateHolidayInfoItem(env, itemList[i]))) {
            HILOG_ERROR_I18N("Call method '$_set' failed");
            return ret;
        }
    }
    return ret;
}

ani_object I18nHolidayManagerAddon::GetHolidayInfoItemArray(ani_env *env, ani_object object, ani_object year)
{
    I18nHolidayManagerAddon* obj = UnwrapAddon(env, object);
    if (obj == nullptr || obj->holidayManager_ == nullptr) {
        HILOG_ERROR_I18N("GetHolidayInfoItemArray: Get HolidayManager object failed");
        return nullptr;
    }

    std::vector<HolidayInfoItem> itemList;
    ani_boolean isUndefined;
    if (ANI_OK != env->Reference_IsUndefined(year, &isUndefined)) {
        HILOG_ERROR_I18N("Reference IsUndefined failed");
        return nullptr;
    }
    if (isUndefined) {
        itemList = obj->holidayManager_->GetHolidayInfoItemArray();
    } else {
        ani_int yearInt;
        if (ANI_OK != env->Object_CallMethodByName_Int(year, "unboxed", ":I", &yearInt)) {
            HILOG_ERROR_I18N("Unbox Int failed");
            return nullptr;
        }
        if (yearInt < 0) {
            ErrorUtil::AniThrow(env, I18N_NOT_VALID, "year", "a non-negative number");
            itemList = obj->holidayManager_->GetHolidayInfoItemArray(-1);
        } else {
            itemList = obj->holidayManager_->GetHolidayInfoItemArray(yearInt);
        }
    }
    return GetHolidayInfoItemResult(env, itemList);
}

I18nEntityRecognizerAddon* I18nEntityRecognizerAddon::UnwrapAddon(ani_env *env, ani_object object)
{
    ani_long ptr;
    if (ANI_OK != env->Object_GetFieldByName_Long(object, "nativeEntityRecognizer", &ptr)) {
        HILOG_ERROR_I18N("Get Long 'nativeEntityRecognizer' failed");
        return nullptr;
    }
    return reinterpret_cast<I18nEntityRecognizerAddon*>(ptr);
}

ani_object I18nEntityRecognizerAddon::Create(ani_env *env, [[maybe_unused]] ani_class clazz, ani_string locale)
{
    std::string localeStr;
    ani_boolean isUndefined;
    if (ANI_OK != env->Reference_IsUndefined(locale, &isUndefined)) {
        HILOG_ERROR_I18N("Reference IsUndefined failed");
        return nullptr;
    }
    if (isUndefined) {
        localeStr = LocaleConfig::GetSystemLocale();
    } else {
        localeStr = AniStrToString(env, locale);
    }

    UErrorCode localeStatus = U_ZERO_ERROR;
    icu::Locale icuLocale = icu::Locale::forLanguageTag(localeStr, localeStatus);
    if (!IsValidLocaleTag(icuLocale)) {
        ErrorUtil::AniThrow(env, I18N_NOT_VALID, "locale", "a valid locale");
        return nullptr;
    }
    std::unique_ptr<I18nEntityRecognizerAddon> obj = std::make_unique<I18nEntityRecognizerAddon>();
    obj->entityRecognizer_ = std::make_unique<EntityRecognizer>(icuLocale);

    static const char *className = "L@ohos/i18n/i18n/EntityRecognizer;";
    ani_object EntityRecognizer_object = CreateAniObject(env, className, obj.get());
    obj.release();
    return EntityRecognizer_object;
}

static ani_object CreateEntityInfoItem(ani_env *env, const int begin, const int end, const std::string& type)
{
    static const char *className = "L@ohos/i18n/i18n/EntityInfoItemInner;";
    ani_class cls;
    if (ANI_OK != env->FindClass(className, &cls)) {
        HILOG_ERROR_I18N("Find class '%{public}s' failed", className);
        return nullptr;
    }

    ani_method ctor;
    if (ANI_OK != env->Class_FindMethod(cls, "<ctor>", nullptr, &ctor)) {
        HILOG_ERROR_I18N("Find method '<ctor>' failed");
        return nullptr;
    }

    ani_object ret;
    if (ANI_OK != env->Object_New(cls, ctor, &ret)) {
        HILOG_ERROR_I18N("New object '%{public}s' failed", className);
        return nullptr;
    }

    SetStringMember(env, ret, "type", type);
    if (ANI_OK != env->Object_SetPropertyByName_Double(ret, "begin", begin)) {
        HILOG_ERROR_I18N("Set property 'offset' failed");
    }
    if (ANI_OK != env->Object_SetPropertyByName_Double(ret, "end", end)) {
        HILOG_ERROR_I18N("Set property 'offset' failed");
    }
    return ret;
}

static ani_object GetEntityInfoItem(ani_env *env, std::vector<std::vector<int>>& entityInfo)
{
    static const char *className = "Lescompat/Array;";
    ani_class cls;
    if (ANI_OK != env->FindClass(className, &cls)) {
        HILOG_ERROR_I18N("Find class '%{public}s' failed", className);
        return nullptr;
    }

    ani_method ctor;
    if (ANI_OK != env->Class_FindMethod(cls, "<ctor>", "I:V", &ctor)) {
        HILOG_ERROR_I18N("Find method '<ctor>' failed");
        return nullptr;
    }

    ani_object ret;
    if (ANI_OK != env->Object_New(cls, ctor, &ret, (entityInfo[0][0] + entityInfo[1][0]))) {
        HILOG_ERROR_I18N("New object '%{public}s' failed", className);
        return nullptr;
    }

    ani_method set;
    if (ANI_OK != env->Class_FindMethod(cls, "$_set", "ILstd/core/Object;:V", &set)) {
        HILOG_ERROR_I18N("Find method '$_set' failed");
        return ret;
    }

    std::vector<std::string> types = {"phone_number", "date"};
    int index = 0;
    for (std::string::size_type t = 0; t < types.size(); t++) {
        for (int i = 0; i < entityInfo[t][0]; i++) {
            int begin = entityInfo[t][2 * i + 1];
            int end = entityInfo[t][2 * i + 2];
            std::string type = types[t];
            if (ANI_OK != env->Object_CallMethod_Void(ret, set, i, CreateEntityInfoItem(env, begin, end, type))) {
                HILOG_ERROR_I18N("Call method '$_set' failed");
                return ret;
            }
            index++;
        }
    }
    return ret;
}

ani_object I18nEntityRecognizerAddon::FindEntityInfo(ani_env *env, ani_object object, ani_string text)
{
    I18nEntityRecognizerAddon* obj = UnwrapAddon(env, object);
    if (obj == nullptr || obj->entityRecognizer_ == nullptr) {
        HILOG_ERROR_I18N("Get EntityRecognizer object failed");
        return nullptr;
    }

    std::string message = AniStrToString(env, text);
    std::vector<std::vector<int>> entityInfo = obj->entityRecognizer_->FindEntityInfo(message);
    return GetEntityInfoItem(env, entityInfo);
}

I18nSimpleDateTimeFormatAddon* I18nSimpleDateTimeFormatAddon::UnwrapAddon(ani_env *env, ani_object object)
{
    ani_long ptr;
    if (ANI_OK != env->Object_GetFieldByName_Long(object, "nativeSimpleDateTimeFormat", &ptr)) {
        HILOG_ERROR_I18N("Get Long 'nativeSimpleDateTimeFormat' failed");
        return nullptr;
    }
    return reinterpret_cast<I18nSimpleDateTimeFormatAddon*>(ptr);
}

ani_object I18nSimpleDateTimeFormatAddon::GetSimpleDateTimeFormatByPattern(ani_env *env,
    ani_string pattern, ani_object locale)
{
    std::shared_ptr<LocaleInfo> localeInfo = nullptr;
    ani_boolean isUndefined;
    if (ANI_OK != env->Reference_IsUndefined(locale, &isUndefined)) {
        HILOG_ERROR_I18N("Reference IsUndefined failed");
        return nullptr;
    }
    if (!isUndefined) {
        ani_long ptr;
        if (ANI_OK != env->Object_GetFieldByName_Long(locale, "nativeLocale", &ptr)) {
            HILOG_ERROR_I18N("Get Long nativeLocale failed");
            return nullptr;
        }
        localeInfo = reinterpret_cast<IntlAniAddon*>(ptr)->GetLocaleInfo();
    }

    std::unique_ptr<I18nSimpleDateTimeFormatAddon> obj = std::make_unique<I18nSimpleDateTimeFormatAddon>();
    if (obj == nullptr) {
        HILOG_ERROR_I18N("SimpleDateTimeFormatConstructor: Cerate I18nSimpleDateTimeFormatAddon failed.");
        return nullptr;
    }

    I18nErrorCode errCode = I18nErrorCode::SUCCESS;
    obj->simpleDateTimeFormat_ =
        std::make_unique<SimpleDateTimeFormat>(AniStrToString(env, pattern), localeInfo, true, errCode);
    if (errCode == I18nErrorCode::INVALID_DATE_TIME_FORMAT_SKELETON) {
        ErrorUtil::AniThrow(env, I18N_NOT_VALID, "skeleton", "a valid date time format skeleton");
        return nullptr;
    } else if (errCode == I18nErrorCode::INVALID_DATE_TIME_FORMAT_PATTERN) {
        ErrorUtil::AniThrow(env, I18N_NOT_VALID, "pattern", "a valid date time format pattern");
        return nullptr;
    } else if (!obj->simpleDateTimeFormat_ || errCode != I18nErrorCode::SUCCESS) {
        HILOG_ERROR_I18N("SimpleDateTimeFormatConstructor: Construct SimpleDateTimeFormat failed.");
        return nullptr;
    }

    static const char *className = "L@ohos/i18n/i18n/SimpleDateTimeFormat;";
    ani_object SimpleDateTimeFormat_object = CreateAniObject(env, className, obj.get());
    obj.release();
    return SimpleDateTimeFormat_object;
}

ani_object I18nSimpleDateTimeFormatAddon::GetSimpleDateTimeFormatBySkeleton(ani_env *env,
    ani_string skeleton, ani_object locale)
{
    std::shared_ptr<LocaleInfo> localeInfo = nullptr;
    ani_boolean isUndefined;
    if (ANI_OK != env->Reference_IsUndefined(locale, &isUndefined)) {
        HILOG_ERROR_I18N("Reference IsUndefined failed");
        return nullptr;
    }
    if (!isUndefined) {
        ani_long ptr;
        if (ANI_OK != env->Object_GetFieldByName_Long(locale, "nativeLocale", &ptr)) {
            HILOG_ERROR_I18N("Get Long nativeLocale failed");
            return nullptr;
        }
        localeInfo = reinterpret_cast<IntlAniAddon*>(ptr)->GetLocaleInfo();
    }

    std::unique_ptr<I18nSimpleDateTimeFormatAddon> obj = std::make_unique<I18nSimpleDateTimeFormatAddon>();
    if (obj == nullptr) {
        HILOG_ERROR_I18N("SimpleDateTimeFormatConstructor: Cerate I18nSimpleDateTimeFormatAddon failed.");
        return nullptr;
    }

    I18nErrorCode errCode = I18nErrorCode::SUCCESS;
    obj->simpleDateTimeFormat_ =
        std::make_unique<SimpleDateTimeFormat>(AniStrToString(env, skeleton), localeInfo, false, errCode);
    if (errCode == I18nErrorCode::INVALID_DATE_TIME_FORMAT_SKELETON) {
        ErrorUtil::AniThrow(env, I18N_NOT_VALID, "skeleton", "a valid date time format skeleton");
        return nullptr;
    } else if (errCode == I18nErrorCode::INVALID_DATE_TIME_FORMAT_PATTERN) {
        ErrorUtil::AniThrow(env, I18N_NOT_VALID, "pattern", "a valid date time format pattern");
        return nullptr;
    } else if (!obj->simpleDateTimeFormat_ || errCode != I18nErrorCode::SUCCESS) {
        HILOG_ERROR_I18N("SimpleDateTimeFormatConstructor: Construct SimpleDateTimeFormat failed.");
        return nullptr;
    }

    static const char *className = "L@ohos/i18n/i18n/SimpleDateTimeFormat;";
    ani_object SimpleDateTimeFormat_object = CreateAniObject(env, className, obj.get());
    obj.release();
    return SimpleDateTimeFormat_object;
}

ani_string I18nSimpleDateTimeFormatAddon::Format(ani_env *env, ani_object object, ani_object date)
{
    I18nSimpleDateTimeFormatAddon* obj = UnwrapAddon(env, object);
    if (obj == nullptr || obj->simpleDateTimeFormat_ == nullptr) {
        HILOG_ERROR_I18N("SimpleDateTimeFormatConstructor: unwrap SimpleDateTimeFormatAddon failed.");
        return nullptr;
    }

    static const char *className = "Lescompat/Date;";
    ani_class cls;
    if (ANI_OK != env->FindClass(className, &cls)) {
        HILOG_ERROR_I18N("Find class '%{public}s' failed", className);
        return nullptr;
    }

    ani_method getms;
    if (ANI_OK != env->Class_FindMethod(cls, "valueOf", nullptr, &getms)) {
        HILOG_ERROR_I18N("Find method 'valueOf' failed");
        return nullptr;
    }

    ani_double milliseconds;
    if (ANI_OK != env->Object_CallMethod_Double(date, getms, &milliseconds)) {
        HILOG_ERROR_I18N("Call method 'valueOf' failed");
        return nullptr;
    }

    std::string formatResult = obj->simpleDateTimeFormat_->Format(milliseconds);
    if (formatResult.empty()) {
        HILOG_ERROR_I18N("SimpleDateTimeFormatConstructor: format result is empty.");
    }
    return StringToAniStr(env, formatResult);
}

I18nSimpleNumberFormatAddon* I18nSimpleNumberFormatAddon::UnwrapAddon(ani_env *env, ani_object object)
{
    ani_long ptr;
    if (ANI_OK != env->Object_GetFieldByName_Long(object, "nativeSimpleNumberFormat", &ptr)) {
        HILOG_ERROR_I18N("Get Long 'nativeSimpleNumberFormat' failed");
        return nullptr;
    }
    return reinterpret_cast<I18nSimpleNumberFormatAddon*>(ptr);
}

ani_object I18nSimpleNumberFormatAddon::GetSimpleNumberFormatBySkeleton(ani_env *env,
    ani_string skeleton, ani_object locale)
{
    std::shared_ptr<LocaleInfo> localeInfo = nullptr;
    ani_boolean isUndefined;
    if (ANI_OK != env->Reference_IsUndefined(locale, &isUndefined)) {
        HILOG_ERROR_I18N("Reference IsUndefined failed");
        return nullptr;
    }
    if (!isUndefined) {
        ani_long ptr;
        if (ANI_OK != env->Object_GetFieldByName_Long(locale, "nativeLocale", &ptr)) {
            HILOG_ERROR_I18N("Get Long nativeLocale failed");
            return nullptr;
        }
        localeInfo = reinterpret_cast<IntlAniAddon*>(ptr)->GetLocaleInfo();
    }

    std::unique_ptr<I18nSimpleNumberFormatAddon> obj = std::make_unique<I18nSimpleNumberFormatAddon>();
    if (!obj) {
        HILOG_ERROR_I18N("SimpleNumberFormatConstructor: create I18nSimpleNumberFormatAddon failed.");
        return nullptr;
    }

    I18nErrorCode errCode = I18nErrorCode::SUCCESS;
    obj->simpleNumberFormat_ = std::make_shared<SimpleNumberFormat>(AniStrToString(env, skeleton), localeInfo, errCode);
    if (errCode == I18nErrorCode::INVALID_NUMBER_FORMAT_SKELETON) {
        ErrorUtil::AniThrow(env, I18N_NOT_VALID, "skeleton", "a valid number format skeleton");
        return nullptr;
    } else if (!obj->simpleNumberFormat_ || errCode != I18nErrorCode::SUCCESS) {
        HILOG_ERROR_I18N("SimpleNumberFormatConstructor: Construct SimpleNumberFormat failed.");
        return nullptr;
    }

    static const char *className = "L@ohos/i18n/i18n/SimpleNumberFormat;";
    ani_object SimpleNumberFormat_object = CreateAniObject(env, className, obj.get());
    obj.release();
    return SimpleNumberFormat_object;
}

ani_string I18nSimpleNumberFormatAddon::Format(ani_env *env, ani_object object, ani_double value)
{
    I18nSimpleNumberFormatAddon* obj = UnwrapAddon(env, object);
    if (obj == nullptr || obj->simpleNumberFormat_  == nullptr) {
        HILOG_ERROR_I18N("SimpleNumberFormatAddon::Format: Unwrap SimpleNumberFormatAddon failed.");
        return nullptr;
    }

    std::string formatResult = obj->simpleNumberFormat_->Format(value);
    if (formatResult.empty()) {
        HILOG_ERROR_I18N("SimpleNumberFormatAddon::Format: Format result is empty.");
    }
    return StringToAniStr(env, formatResult);
}

I18nStyledNumberFormatAddon* I18nStyledNumberFormatAddon::UnwrapAddon(ani_env *env, ani_object object)
{
    ani_long ptr;
    if (ANI_OK != env->Object_GetFieldByName_Long(object, "nativeStyledNumberFormat", &ptr)) {
        HILOG_ERROR_I18N("Get Long 'nativeStyledNumberFormat' failed");
        return nullptr;
    }
    return reinterpret_cast<I18nStyledNumberFormatAddon*>(ptr);
}

static void GetNumberFmtOrSimpleNumberFmt(ani_env *env, ani_object numberFormat,
    IntlAniAddon **numberFormatAddon, I18nSimpleNumberFormatAddon **simpleNumberFormatAddon)
{
    ani_class simpleNumberFormatClass;
    if (ANI_OK != env->FindClass("L@ohos/i18n/i18n/SimpleNumberFormat;", &simpleNumberFormatClass)) {
        HILOG_ERROR_I18N("Find class 'L@ohos/i18n/i18n/SimpleNumberFormat;' failed");
        return;
    }

    ani_boolean isSimpleNumberFormat;
    if (ANI_OK != env->Object_InstanceOf(numberFormat, simpleNumberFormatClass, &isSimpleNumberFormat)) {
        HILOG_ERROR_I18N("Get Instance failed");
        return;
    }
    if (isSimpleNumberFormat) {
        *simpleNumberFormatAddon = I18nSimpleNumberFormatAddon::UnwrapAddon(env, numberFormat);
    } else {
        ani_long ptr;
        if (ANI_OK != env->Object_GetFieldByName_Long(numberFormat, "nativeNumberFormat", &ptr)) {
            HILOG_ERROR_I18N("Get Long 'nativeNumberFormat' failed");
            *numberFormatAddon = nullptr;
        }
        *numberFormatAddon = reinterpret_cast<IntlAniAddon*>(ptr);
    }
}

void I18nStyledNumberFormatAddon::SetTextStyle(ani_env *env, ani_object options, const std::string &optionName)
{
    ani_ref ref;
    if (ANI_OK != env->Object_GetPropertyByName_Ref(options, optionName.c_str(), &ref)) {
        HILOG_ERROR_I18N("Get property '%{public}s' failed", optionName.c_str());
        return;
    }

    ani_boolean isUndefined;
    if (ANI_OK != env->Reference_IsUndefined(ref, &isUndefined)) {
        HILOG_ERROR_I18N("Reference IsUndefined failed");
        return;
    }
    if (isUndefined) {
        HILOG_ERROR_I18N("setTextStyle: %{public}s is nullptr.", optionName.c_str());
        return;
    }

    styledNumberFormatOptions_[optionName] = ref;
}

bool I18nStyledNumberFormatAddon::InitStyledNumberFormatContent(ani_env *env, IntlAniAddon* numberFormatAddon,
    I18nSimpleNumberFormatAddon* simpleNumberFormatAddon, ani_object options)
{
    if (numberFormatAddon) {
        styledNumberFormat_ =
            std::make_unique<StyledNumberFormat>(true, numberFormatAddon->CopyNumberFormat(), nullptr);
    } else if (simpleNumberFormatAddon) {
        styledNumberFormat_ =
            std::make_unique<StyledNumberFormat>(false, nullptr, simpleNumberFormatAddon->CopySimpleNumberFormat());
    }

    ani_boolean isUndefined;
    if (ANI_OK != env->Reference_IsUndefined(options, &isUndefined)) {
        HILOG_ERROR_I18N("Reference IsUndefined failed");
        return false;
    }
    if (!isUndefined) {
        SetTextStyle(env, options, "integer");
        SetTextStyle(env, options, "decimal");
        SetTextStyle(env, options, "fraction");
        SetTextStyle(env, options, "unit");
    }
    return styledNumberFormat_ != nullptr;
}

ani_object I18nStyledNumberFormatAddon::Create(ani_env *env, [[maybe_unused]] ani_class clazz,
    ani_object numberFormat, ani_object options)
{
    IntlAniAddon *numberFormatAddon = nullptr;
    I18nSimpleNumberFormatAddon *simpleNumberFormatAddon = nullptr;

    GetNumberFmtOrSimpleNumberFmt(env, numberFormat, &numberFormatAddon, &simpleNumberFormatAddon);

    if (!numberFormatAddon && !simpleNumberFormatAddon) {
        ErrorUtil::AniThrow(env, I18N_NOT_FOUND, "numberFormat", "intl.NumberFormat or SimpleNumberFormat");
        return nullptr;
    }

    std::unique_ptr<I18nStyledNumberFormatAddon> obj = std::make_unique<I18nStyledNumberFormatAddon>();
    if (!obj) {
        HILOG_ERROR_I18N("StyledNumberFormatAddon::constructor make unique ptr error");
        return nullptr;
    }

    if (!obj->InitStyledNumberFormatContent(env, numberFormatAddon, simpleNumberFormatAddon, options)) {
        return nullptr;
    }

    static const char *className = "L@ohos/i18n/i18n/StyledNumberFormat;";
    ani_object StyledNumberFormat_object = CreateAniObject(env, className, obj.get());
    obj.release();
    return StyledNumberFormat_object;
}

static ani_object CreateStyleOption(ani_env *env, StyledNumberFormat::NumberPart numberPart, ani_ref textStyleRef)
{
    if (!textStyleRef) {
        HILOG_ERROR_I18N("CreateStyleoption: options do not have %{public}s",
            numberPart.part_name.c_str());
        return nullptr;
    }

    static const char *className = "Larkui/component/styledString/StyleOptionsInner;";
    ani_class cls;
    if (ANI_OK != env->FindClass(className, &cls)) {
        HILOG_ERROR_I18N("Find class '%{public}s' failed", className);
        return nullptr;
    }

    ani_method ctor;
    if (ANI_OK != env->Class_FindMethod(cls, "<ctor>", nullptr, &ctor)) {
        HILOG_ERROR_I18N("Find method '<ctor>' failed");
        return nullptr;
    }

    ani_object styleOptions;
    if (ANI_OK != env->Object_New(cls, ctor, &styleOptions)) {
        HILOG_ERROR_I18N("New object '%{public}s' failed", className);
        return nullptr;
    }

    SetNumberMember(env, styleOptions, "start", numberPart.start);
    SetNumberMember(env, styleOptions, "length", numberPart.length);

    static const char *enumName = "Larkui/component/styledString/StyledStringKey;";
    if (ANI_OK != env->Object_SetPropertyByName_Ref(styleOptions, "styledKey", GetEnumItemByIndex(env, enumName, 0))) {
        HILOG_ERROR_I18N("Set property 'styledKey' failed");
        return styleOptions;
    }

    if (ANI_OK != env->Object_SetPropertyByName_Ref(styleOptions, "styledValue", textStyleRef)) {
        HILOG_ERROR_I18N("Set property 'styledValue' failed");
        return styleOptions;
    }
    return styleOptions;
}

static ani_object CreateStyleOptions(ani_env *env,
    const std::vector<StyledNumberFormat::NumberPart> &numberParts,
    const std::unordered_map<std::string, ani_ref> &styledNumberFormatOptions)
{
    static const char *className = "Lescompat/Array;";
    ani_class cls;
    if (ANI_OK != env->FindClass(className, &cls)) {
        HILOG_ERROR_I18N("Find class '%{public}s' failed", className);
        return nullptr;
    }

    ani_method ctor;
    if (ANI_OK != env->Class_FindMethod(cls, "<ctor>", "I:V", &ctor)) {
        HILOG_ERROR_I18N("Find method '<ctor>' failed");
        return nullptr;
    }

    ani_object ret;
    if (ANI_OK != env->Object_New(cls, ctor, &ret, numberParts.size())) {
        HILOG_ERROR_I18N("New object '%{public}s' failed", className);
        return nullptr;
    }

    ani_method set;
    if (ANI_OK != env->Class_FindMethod(cls, "$_set", "ILstd/core/Object;:V", &set)) {
        HILOG_ERROR_I18N("Find method '$_set' failed");
        return nullptr;
    }

    for (size_t i = 0; i < numberParts.size(); i++) {
        auto iter = styledNumberFormatOptions.find(numberParts[i].part_name);
        if (iter == styledNumberFormatOptions.end()) {
            continue;
        }

        ani_ref textStyleRef = iter->second;
        ani_object option = CreateStyleOption(env, numberParts[i], textStyleRef);
        if (ANI_OK != env->Object_CallMethod_Void(ret, set, i, option)) {
            HILOG_ERROR_I18N("Call method '$_set' failed");
            return nullptr;
        }
    }
    return ret;
}

ani_object I18nStyledNumberFormatAddon::Format(ani_env *env, ani_object object, ani_double value)
{
    I18nStyledNumberFormatAddon* obj = UnwrapAddon(env, object);
    if (obj == nullptr || obj->styledNumberFormat_  == nullptr) {
        HILOG_ERROR_I18N("Format: Get StyledNumberFormat object failed");
        return nullptr;
    }

    std::string formattedNumber = obj->styledNumberFormat_->Format(value);
    std::vector<StyledNumberFormat::NumberPart> numberParts = obj->styledNumberFormat_->ParseToParts(value);
    
    static const char *className = "Larkui/component/styledString/StyledString;";
    ani_class cls;
    if (ANI_OK != env->FindClass(className, &cls)) {
        HILOG_ERROR_I18N("Find class '%{public}s' failed", className);
        return nullptr;
    }

    ani_method ctor;
    if (ANI_OK != env->Class_FindMethod(cls, "<ctor>", "Lstd/core/Object;Lescompat/Array;:V", &ctor)) {
        HILOG_ERROR_I18N("Find method '<ctor>' failed");
        return nullptr;
    }

    ani_object styleOption = CreateStyleOptions(env, numberParts, obj->styledNumberFormatOptions_);
    ani_object styleString;
    if (ANI_OK != env->Object_New(cls, ctor, &styleString, StringToAniStr(env, formattedNumber), styleOption)) {
        HILOG_ERROR_I18N("New object '%{public}s' failed", className);
        return nullptr;
    }
    return styleString;
}

ani_boolean I18nIsRTL(ani_env *env, ani_string locale)
{
    return LocaleConfig::IsRTL(AniStrToString(env, locale));
}

static ani_status BindContextSystem(ani_env *env)
{
    static const char *className = "L@ohos/i18n/i18n/System;";
    ani_class cls;
    if (ANI_OK != env->FindClass(className, &cls)) {
        HILOG_ERROR_I18N("Find class '%{public}s' failed", className);
        return (ani_status)ANI_ERROR;
    }

    std::array methods = {
        ani_native_function { "getDisplayCountry", nullptr, reinterpret_cast<void *>(GetDisplayCountry) },
        ani_native_function { "getDisplayLanguage", nullptr, reinterpret_cast<void *>(GetDisplayLanguage) },
        ani_native_function { "getSystemLanguages", nullptr, reinterpret_cast<void *>(GetSystemLanguages) },
        ani_native_function { "getSystemCountries", nullptr, reinterpret_cast<void *>(GetSystemCountries) },
        ani_native_function { "isSuggested", nullptr, reinterpret_cast<void *>(IsSuggested) },
        ani_native_function { "getSystemLanguage", nullptr, reinterpret_cast<void *>(GetSystemLanguage) },
        ani_native_function { "setSystemLanguage", nullptr, reinterpret_cast<void *>(SetSystemLanguage) },
        ani_native_function { "getSystemRegion", nullptr, reinterpret_cast<void *>(GetSystemRegion) },
        ani_native_function { "setSystemRegion", nullptr, reinterpret_cast<void *>(SetSystemRegion) },
        ani_native_function { "getSystemLocale", nullptr, reinterpret_cast<void *>(GetSystemLocale) },
        ani_native_function { "setSystemLocale", nullptr, reinterpret_cast<void *>(SetSystemLocale) },
        ani_native_function { "is24HourClock", nullptr, reinterpret_cast<void *>(Is24HourClock) },
        ani_native_function { "set24HourClock", nullptr, reinterpret_cast<void *>(Set24HourClock) },
        ani_native_function { "addPreferredLanguage", nullptr, reinterpret_cast<void *>(AddPreferredLanguage) },
        ani_native_function { "removePreferredLanguage", nullptr, reinterpret_cast<void *>(RemovePreferredLanguage) },
        ani_native_function { "getPreferredLanguageList", nullptr, reinterpret_cast<void *>(GetPreferredLanguageList) },
        ani_native_function { "getFirstPreferredLanguage", nullptr,
            reinterpret_cast<void *>(GetFirstPreferredLanguage) },
        ani_native_function { "setAppPreferredLanguage", nullptr, reinterpret_cast<void *>(SetAppPreferredLanguage) },
        ani_native_function { "getAppPreferredLanguage", nullptr, reinterpret_cast<void *>(GetAppPreferredLanguage) },
        ani_native_function { "setUsingLocalDigit", nullptr, reinterpret_cast<void *>(SetUsingLocalDigit) },
        ani_native_function { "getUsingLocalDigit", nullptr, reinterpret_cast<void *>(GetUsingLocalDigit) },
        ani_native_function { "getSimplifiedLanguage", nullptr, reinterpret_cast<void *>(GetSimplifiedLanguage) },
        ani_native_function { "setTemperatureType", nullptr, reinterpret_cast<void *>(SetTemperatureType) },
        ani_native_function { "getTemperatureType", nullptr, reinterpret_cast<void *>(GetTemperatureType) },
        ani_native_function { "getTemperatureName", nullptr, reinterpret_cast<void *>(GetTemperatureName) },
        ani_native_function { "setFirstDayOfWeek", nullptr, reinterpret_cast<void *>(SetFirstDayOfWeek) },
        ani_native_function { "getFirstDayOfWeek", nullptr, reinterpret_cast<void *>(GetFirstDayOfWeek) },
    };

    if (ANI_OK != env->Class_BindNativeMethods(cls, methods.data(), methods.size())) {
        HILOG_ERROR_I18N("Cannot bind native methods to '%{public}s'", className);
        return (ani_status)ANI_ERROR;
    };

    return ANI_OK;
}

static ani_status BindContextI18NUtil(ani_env *env)
{
    static const char *className = "L@ohos/i18n/i18n/I18NUtil;";
    ani_class cls;
    if (ANI_OK != env->FindClass(className, &cls)) {
        HILOG_ERROR_I18N("Find class '%{public}s' failed", className);
        return (ani_status)ANI_ERROR;
    }

    std::array methods = {
        ani_native_function { "unitConvert", nullptr, reinterpret_cast<void *>(UnitConvert) },
        ani_native_function { "getDateOrder", nullptr, reinterpret_cast<void *>(GetDateOrder) },
        ani_native_function { "getTimePeriodName", nullptr, reinterpret_cast<void *>(GetTimePeriodName) },
        ani_native_function { "getBestMatchLocale", nullptr, reinterpret_cast<void *>(GetBestMatchLocale) },
        ani_native_function { "getThreeLetterLanguage", nullptr, reinterpret_cast<void *>(GetThreeLetterLanguage) },
        ani_native_function { "getThreeLetterRegion", nullptr, reinterpret_cast<void *>(GetThreeLetterRegion) },
        ani_native_function { "getUnicodeWrappedFilePath", nullptr,
            reinterpret_cast<void *>(GetUnicodeWrappedFilePath) },
    };

    if (ANI_OK != env->Class_BindNativeMethods(cls, methods.data(), methods.size())) {
        HILOG_ERROR_I18N("Cannot bind native methods to '%{public}s'", className);
        return (ani_status)ANI_ERROR;
    };

    return ANI_OK;
}

static ani_status BindContextPhoneNumberFormat(ani_env *env)
{
    static const char *className = "L@ohos/i18n/i18n/PhoneNumberFormat;";
    ani_class cls;
    if (ANI_OK != env->FindClass(className, &cls)) {
        HILOG_ERROR_I18N("Find class '%{public}s' failed", className);
        return (ani_status)ANI_ERROR;
    }

    std::array methods = {
        ani_native_function { "create", nullptr, reinterpret_cast<void *>(I18nPhoneNumberFormatAddon::Create) },
        ani_native_function { "isValidNumber", nullptr,
            reinterpret_cast<void *>(I18nPhoneNumberFormatAddon::IsValidNumber) },
        ani_native_function { "format", nullptr, reinterpret_cast<void *>(I18nPhoneNumberFormatAddon::Format) },
        ani_native_function { "getLocationName", nullptr,
            reinterpret_cast<void *>(I18nPhoneNumberFormatAddon::GetLocationName) },
    };

    if (ANI_OK != env->Class_BindNativeMethods(cls, methods.data(), methods.size())) {
        HILOG_ERROR_I18N("Cannot bind native methods to '%{public}s'", className);
        return (ani_status)ANI_ERROR;
    };

    return ANI_OK;
}

static ani_status BindContextCalendar(ani_env *env)
{
    static const char *className = "L@ohos/i18n/i18n/Calendar;";
    ani_class cls;
    if (ANI_OK != env->FindClass(className, &cls)) {
        HILOG_ERROR_I18N("Find class '%{public}s' failed", className);
        return (ani_status)ANI_ERROR;
    }

    std::array methods = {
        ani_native_function { "setTime", "Lescompat/Date;:V",
            reinterpret_cast<void *>(I18nCalendarAddon::SetTimeByDate) },
        ani_native_function { "setTime", "D:V", reinterpret_cast<void *>(I18nCalendarAddon::SetTime) },
        ani_native_function { "set", nullptr, reinterpret_cast<void *>(I18nCalendarAddon::Set) },
        ani_native_function { "setTimeZone", nullptr, reinterpret_cast<void *>(I18nCalendarAddon::SetTimeZone) },
        ani_native_function { "getTimeZone", nullptr, reinterpret_cast<void *>(I18nCalendarAddon::GetTimeZone) },
        ani_native_function { "getFirstDayOfWeek", nullptr,
            reinterpret_cast<void *>(I18nCalendarAddon::GetFirstDayOfWeek) },
        ani_native_function { "setFirstDayOfWeek", nullptr,
            reinterpret_cast<void *>(I18nCalendarAddon::SetFirstDayOfWeek) },
        ani_native_function { "getMinimalDaysInFirstWeek", nullptr,
            reinterpret_cast<void *>(I18nCalendarAddon::GetMinimalDaysInFirstWeek) },
        ani_native_function { "setMinimalDaysInFirstWeek", nullptr,
            reinterpret_cast<void *>(I18nCalendarAddon::SetMinimalDaysInFirstWeek) },
        ani_native_function { "get", nullptr, reinterpret_cast<void *>(I18nCalendarAddon::Get) },
        ani_native_function { "getDisplayName", nullptr, reinterpret_cast<void *>(I18nCalendarAddon::GetDisplayName) },
        ani_native_function { "isWeekend", nullptr, reinterpret_cast<void *>(I18nCalendarAddon::IsWeekend) },
        ani_native_function { "add", nullptr, reinterpret_cast<void *>(I18nCalendarAddon::Add) },
        ani_native_function { "getTimeInMillis", nullptr,
            reinterpret_cast<void *>(I18nCalendarAddon::GetTimeInMillis) },
        ani_native_function { "compareDays", nullptr, reinterpret_cast<void *>(I18nCalendarAddon::CompareDays) },
    };

    if (ANI_OK != env->Class_BindNativeMethods(cls, methods.data(), methods.size())) {
        HILOG_ERROR_I18N("Cannot bind native methods to '%{public}s'", className);
        return (ani_status)ANI_ERROR;
    };

    return ANI_OK;
}

static ani_status BindContextBreakIterator(ani_env *env)
{
    static const char *className = "L@ohos/i18n/i18n/BreakIterator;";
    ani_class cls;
    if (ANI_OK != env->FindClass(className, &cls)) {
        HILOG_ERROR_I18N("Find class '%{public}s' failed", className);
        return (ani_status)ANI_ERROR;
    }

    std::array methods = {
        ani_native_function { "current", nullptr, reinterpret_cast<void *>(I18nBreakIteratorAddon::Current) },
        ani_native_function { "first", nullptr, reinterpret_cast<void *>(I18nBreakIteratorAddon::First) },
        ani_native_function { "last", nullptr, reinterpret_cast<void *>(I18nBreakIteratorAddon::Last) },
        ani_native_function { "next", nullptr, reinterpret_cast<void *>(I18nBreakIteratorAddon::Next) },
        ani_native_function { "previous", nullptr, reinterpret_cast<void *>(I18nBreakIteratorAddon::Previous) },
        ani_native_function { "setLineBreakText", nullptr,
            reinterpret_cast<void *>(I18nBreakIteratorAddon::SetLineBreakText) },
        ani_native_function { "following", nullptr, reinterpret_cast<void *>(I18nBreakIteratorAddon::Following) },
        ani_native_function { "getLineBreakText", nullptr,
            reinterpret_cast<void *>(I18nBreakIteratorAddon::GetLineBreakText) },
        ani_native_function { "isBoundary", nullptr, reinterpret_cast<void *>(I18nBreakIteratorAddon::IsBoundary) },
    };

    if (ANI_OK != env->Class_BindNativeMethods(cls, methods.data(), methods.size())) {
        HILOG_ERROR_I18N("Cannot bind native methods to '%{public}s'", className);
        return (ani_status)ANI_ERROR;
    };

    return ANI_OK;
}

static ani_status BindContextIndexUtil(ani_env *env)
{
    static const char *className = "L@ohos/i18n/i18n/IndexUtil;";
    ani_class cls;
    if (ANI_OK != env->FindClass(className, &cls)) {
        HILOG_ERROR_I18N("Find class '%{public}s' failed", className);
        return (ani_status)ANI_ERROR;
    }

    std::array methods = {
        ani_native_function { "getIndexList", nullptr, reinterpret_cast<void *>(I18nIndexUtilAddon::GetIndexList) },
        ani_native_function { "addLocale", nullptr, reinterpret_cast<void *>(I18nIndexUtilAddon::AddLocale) },
        ani_native_function { "getIndex", nullptr, reinterpret_cast<void *>(I18nIndexUtilAddon::GetIndex) },
    };

    if (ANI_OK != env->Class_BindNativeMethods(cls, methods.data(), methods.size())) {
        HILOG_ERROR_I18N("Cannot bind native methods to '%{public}s'", className);
        return (ani_status)ANI_ERROR;
    };

    return ANI_OK;
}

static ani_status BindContextUnicode(ani_env *env)
{
    static const char *className = "L@ohos/i18n/i18n/Unicode;";
    ani_class cls;
    if (ANI_OK != env->FindClass(className, &cls)) {
        HILOG_ERROR_I18N("Find class '%{public}s' failed", className);
        return (ani_status)ANI_ERROR;
    }

    std::array methods = {
        ani_native_function { "isDigit", nullptr, reinterpret_cast<void *>(UnicodeIsDigit) },
        ani_native_function { "isSpaceChar", nullptr, reinterpret_cast<void *>(UnicodeIsSpaceChar) },
        ani_native_function { "isWhitespace", nullptr, reinterpret_cast<void *>(UnicodeIsWhitespace) },
        ani_native_function { "isRTL", nullptr, reinterpret_cast<void *>(UnicodeIsRTL) },
        ani_native_function { "isIdeograph", nullptr, reinterpret_cast<void *>(UnicodeIsIdeograph) },
        ani_native_function { "isLetter", nullptr, reinterpret_cast<void *>(UnicodeIsLetter) },
        ani_native_function { "isLowerCase", nullptr, reinterpret_cast<void *>(UnicodeIsLowerCase) },
        ani_native_function { "isUpperCase", nullptr, reinterpret_cast<void *>(UnicodeIsUpperCase) },
        ani_native_function { "getType", nullptr, reinterpret_cast<void *>(UnicodeGetType) },
    };

    if (ANI_OK != env->Class_BindNativeMethods(cls, methods.data(), methods.size())) {
        HILOG_ERROR_I18N("Cannot bind native methods to '%{public}s'", className);
        return (ani_status)ANI_ERROR;
    };

    return ANI_OK;
}

static ani_status BindContextTimeZone(ani_env *env)
{
    static const char *className = "L@ohos/i18n/i18n/TimeZone;";
    ani_class cls;
    if (ANI_OK != env->FindClass(className, &cls)) {
        HILOG_ERROR_I18N("Find class '%{public}s' failed", className);
        return (ani_status)ANI_ERROR;
    }

    std::array methods = {
        ani_native_function { "getID", nullptr, reinterpret_cast<void *>(I18nTimeZoneAddon::GetID) },
        ani_native_function { "getDisplayName", nullptr, reinterpret_cast<void *>(I18nTimeZoneAddon::GetDisplayName) },
        ani_native_function { "getRawOffset", nullptr, reinterpret_cast<void *>(I18nTimeZoneAddon::GetRawOffset) },
        ani_native_function { "getOffset", nullptr, reinterpret_cast<void *>(I18nTimeZoneAddon::GetOffset) },
        ani_native_function { "getAvailableIDs", nullptr,
            reinterpret_cast<void *>(I18nTimeZoneAddon::GetAvailableIDs) },
        ani_native_function { "getAvailableZoneCityIDs", nullptr,
            reinterpret_cast<void *>(I18nTimeZoneAddon::GetAvailableZoneCityIDs) },
        ani_native_function { "getCityDisplayName", nullptr,
            reinterpret_cast<void *>(I18nTimeZoneAddon::GetCityDisplayName) },
        ani_native_function { "getTimezoneFromCity", nullptr,
            reinterpret_cast<void *>(I18nTimeZoneAddon::GetTimezoneFromCity) },
        ani_native_function { "getTimezonesByLocation", nullptr,
            reinterpret_cast<void *>(I18nTimeZoneAddon::GetTimezonesByLocation) },
    };

    if (ANI_OK != env->Class_BindNativeMethods(cls, methods.data(), methods.size())) {
        HILOG_ERROR_I18N("Cannot bind native methods to '%{public}s'", className);
        return (ani_status)ANI_ERROR;
    };

    return ANI_OK;
}

static ani_status BindContextTransliterator(ani_env *env)
{
    static const char *className = "L@ohos/i18n/i18n/Transliterator;";
    ani_class cls;
    if (ANI_OK != env->FindClass(className, &cls)) {
        HILOG_ERROR_I18N("Find class '%{public}s' failed", className);
        return (ani_status)ANI_ERROR;
    }

    std::array methods = {
        ani_native_function { "getAvailableIDs", nullptr,
            reinterpret_cast<void *>(I18nTransliteratorAddon::GetAvailableIDs) },
        ani_native_function { "getInstance", nullptr, reinterpret_cast<void *>(I18nTransliteratorAddon::GetInstance) },
        ani_native_function { "transform", nullptr, reinterpret_cast<void *>(I18nTransliteratorAddon::Transform) },
    };

    if (ANI_OK != env->Class_BindNativeMethods(cls, methods.data(), methods.size())) {
        HILOG_ERROR_I18N("Cannot bind native methods to '%{public}s'", className);
        return (ani_status)ANI_ERROR;
    };

    return ANI_OK;
}

static ani_status BindContextNormalizer(ani_env *env)
{
    static const char *className = "L@ohos/i18n/i18n/Normalizer;";
    ani_class cls;
    if (ANI_OK != env->FindClass(className, &cls)) {
        HILOG_ERROR_I18N("Find class '%{public}s' failed", className);
        return (ani_status)ANI_ERROR;
    }

    std::array methods = {
        ani_native_function { "getInstance", nullptr, reinterpret_cast<void *>(I18nNormalizerAddon::GetInstance) },
        ani_native_function { "normalize", nullptr, reinterpret_cast<void *>(I18nNormalizerAddon::Normalize) },
    };

    if (ANI_OK != env->Class_BindNativeMethods(cls, methods.data(), methods.size())) {
        HILOG_ERROR_I18N("Cannot bind native methods to '%{public}s'", className);
        return (ani_status)ANI_ERROR;
    };

    return ANI_OK;
}

static ani_status BindContextSysLocaleMgr(ani_env *env)
{
    static const char *className = "L@ohos/i18n/i18n/SystemLocaleManager;";
    ani_class cls;
    if (ANI_OK != env->FindClass(className, &cls)) {
        HILOG_ERROR_I18N("Find class '%{public}s' failed", className);
        return (ani_status)ANI_ERROR;
    }

    std::array methods = {
        ani_native_function { "create", nullptr, reinterpret_cast<void *>(I18nSysLocaleMgrAddon::Create) },
        ani_native_function { "getLanguageInfoArray", nullptr,
            reinterpret_cast<void *>(I18nSysLocaleMgrAddon::GetLanguageInfoArray) },
        ani_native_function { "getRegionInfoArray", nullptr,
            reinterpret_cast<void *>(I18nSysLocaleMgrAddon::GetRegionInfoArray) },
        ani_native_function { "getTimeZoneCityItemArray", nullptr,
            reinterpret_cast<void *>(I18nSysLocaleMgrAddon::GetTimeZoneCityItemArray) },
    };

    if (ANI_OK != env->Class_BindNativeMethods(cls, methods.data(), methods.size())) {
        HILOG_ERROR_I18N("Cannot bind native methods to '%{public}s'", className);
        return (ani_status)ANI_ERROR;
    };

    return ANI_OK;
}

static ani_status BindContextHolidayManager(ani_env *env)
{
    static const char *className = "L@ohos/i18n/i18n/HolidayManager;";
    ani_class cls;
    if (ANI_OK != env->FindClass(className, &cls)) {
        HILOG_ERROR_I18N("Find class '%{public}s' failed", className);
        return (ani_status)ANI_ERROR;
    }

    std::array methods = {
        ani_native_function { "create", nullptr, reinterpret_cast<void *>(I18nHolidayManagerAddon::Create) },
        ani_native_function { "isHoliday", nullptr, reinterpret_cast<void *>(I18nHolidayManagerAddon::IsHoliday) },
        ani_native_function { "getHolidayInfoItemArray", nullptr,
            reinterpret_cast<void *>(I18nHolidayManagerAddon::GetHolidayInfoItemArray) },
    };

    if (ANI_OK != env->Class_BindNativeMethods(cls, methods.data(), methods.size())) {
        HILOG_ERROR_I18N("Cannot bind native methods to '%{public}s'", className);
        return (ani_status)ANI_ERROR;
    };

    return ANI_OK;
}

static ani_status BindContextEntityRecognizer(ani_env *env)
{
    static const char *className = "L@ohos/i18n/i18n/EntityRecognizer;";
    ani_class cls;
    if (ANI_OK != env->FindClass(className, &cls)) {
        HILOG_ERROR_I18N("Find class '%{public}s' failed", className);
        return (ani_status)ANI_ERROR;
    }

    std::array methods = {
        ani_native_function { "create", nullptr, reinterpret_cast<void *>(I18nEntityRecognizerAddon::Create) },
        ani_native_function { "findEntityInfo", nullptr,
            reinterpret_cast<void *>(I18nEntityRecognizerAddon::FindEntityInfo) },
    };

    if (ANI_OK != env->Class_BindNativeMethods(cls, methods.data(), methods.size())) {
        HILOG_ERROR_I18N("Cannot bind native methods to '%{public}s'", className);
        return (ani_status)ANI_ERROR;
    };

    return ANI_OK;
}

static ani_status BindContextSimpleDateTimeFormat(ani_env *env)
{
    static const char *className = "L@ohos/i18n/i18n/SimpleDateTimeFormat;";
    ani_class cls;
    if (ANI_OK != env->FindClass(className, &cls)) {
        HILOG_ERROR_I18N("Find class '%{public}s' failed", className);
        return (ani_status)ANI_ERROR;
    }

    std::array methods = {
        ani_native_function { "format", nullptr, reinterpret_cast<void *>(I18nSimpleDateTimeFormatAddon::Format) },
    };

    if (ANI_OK != env->Class_BindNativeMethods(cls, methods.data(), methods.size())) {
        HILOG_ERROR_I18N("Cannot bind native methods to '%{public}s'", className);
        return (ani_status)ANI_ERROR;
    };

    return ANI_OK;
}

static ani_status BindContextSimpleNumberFormat(ani_env *env)
{
    static const char *className = "L@ohos/i18n/i18n/SimpleNumberFormat;";
    ani_class cls;
    if (ANI_OK != env->FindClass(className, &cls)) {
        HILOG_ERROR_I18N("Find class '%{public}s' failed", className);
        return (ani_status)ANI_ERROR;
    }

    std::array methods = {
        ani_native_function { "format", nullptr, reinterpret_cast<void *>(I18nSimpleNumberFormatAddon::Format) },
    };

    if (ANI_OK != env->Class_BindNativeMethods(cls, methods.data(), methods.size())) {
        HILOG_ERROR_I18N("Cannot bind native methods to '%{public}s'", className);
        return (ani_status)ANI_ERROR;
    };

    return ANI_OK;
}

static ani_status BindContextStyledNumberFormat(ani_env *env)
{
    static const char *className = "L@ohos/i18n/i18n/StyledNumberFormat;";
    ani_class cls;
    if (ANI_OK != env->FindClass(className, &cls)) {
        HILOG_ERROR_I18N("Find class '%{public}s' failed", className);
        return (ani_status)ANI_ERROR;
    }

    std::array methods = {
        ani_native_function { "create", nullptr, reinterpret_cast<void *>(I18nStyledNumberFormatAddon::Create) },
        ani_native_function { "format", nullptr, reinterpret_cast<void *>(I18nStyledNumberFormatAddon::Format) },
    };

    if (ANI_OK != env->Class_BindNativeMethods(cls, methods.data(), methods.size())) {
        HILOG_ERROR_I18N("Cannot bind native methods to '%{public}s'", className);
        return (ani_status)ANI_ERROR;
    };

    return ANI_OK;
}

static ani_status BindContextNameSpace(ani_env *env)
{
    static const char* nameSpaceName = "L@ohos/i18n/i18n;";
    ani_namespace ns;
    if (ANI_OK != env->FindNamespace(nameSpaceName, &ns)) {
        HILOG_ERROR_I18N("Find namespace '%{public}s' failed", nameSpaceName);
        return (ani_status)ANI_ERROR;
    }
    std::array methods = {
        ani_native_function { "getCalendar", nullptr, reinterpret_cast<void *>(I18nCalendarAddon::GetCalendar) },
        ani_native_function { "isRTL", nullptr, reinterpret_cast<void *>(I18nIsRTL) },
        ani_native_function { "getLineInstance", nullptr,
            reinterpret_cast<void *>(I18nBreakIteratorAddon::GetLineInstance) },
        ani_native_function { "getInstance", nullptr, reinterpret_cast<void *>(I18nIndexUtilAddon::GetInstance) },
        ani_native_function { "getTimeZone", nullptr, reinterpret_cast<void *>(I18nTimeZoneAddon::GetTimeZone) },
        ani_native_function { "getSimpleDateTimeFormatByPattern", nullptr,
            reinterpret_cast<void *>(I18nSimpleDateTimeFormatAddon::GetSimpleDateTimeFormatByPattern) },
        ani_native_function { "getSimpleDateTimeFormatBySkeleton", nullptr,
            reinterpret_cast<void *>(I18nSimpleDateTimeFormatAddon::GetSimpleDateTimeFormatBySkeleton) },
        ani_native_function { "getSimpleNumberFormatBySkeleton", nullptr,
            reinterpret_cast<void *>(I18nSimpleNumberFormatAddon::GetSimpleNumberFormatBySkeleton) },
    };
    if (ANI_OK != env->Namespace_BindNativeFunctions(ns, methods.data(), methods.size())) {
        HILOG_ERROR_I18N("Cannot bind native methods to '%{public}s'", nameSpaceName);
        return (ani_status)ANI_ERROR;
    };

    return ANI_OK;
}

ani_status BindContext(ani_env *env)
{
    auto status = BindContextTransliterator(env);
    if (status != ANI_OK) {
        return status;
    }

    status = BindContextNormalizer(env);
    if (status != ANI_OK) {
        return status;
    }

    status = BindContextSysLocaleMgr(env);
    if (status != ANI_OK) {
        return status;
    }

    status = BindContextHolidayManager(env);
    if (status != ANI_OK) {
        return status;
    }

    status = BindContextEntityRecognizer(env);
    if (status != ANI_OK) {
        return status;
    }

    status = BindContextSimpleDateTimeFormat(env);
    if (status != ANI_OK) {
        return status;
    }

    status = BindContextSimpleNumberFormat(env);
    if (status != ANI_OK) {
        return status;
    }

    status = BindContextStyledNumberFormat(env);
    if (status != ANI_OK) {
        return status;
    }

    status = BindContextNameSpace(env);
    if (status != ANI_OK) {
        return status;
    }
    return status;
}

ANI_EXPORT ani_status ANI_Constructor(ani_vm *vm, uint32_t *result)
{
    ani_env *env;
    if (ANI_OK != vm->GetEnv(ANI_VERSION_1, &env)) {
        HILOG_ERROR_I18N("Unsupported ANI_VERSION_1");
        return (ani_status)ANI_ERROR;
    }
    
    auto status = BindContextSystem(env);
    if (status != ANI_OK) {
        return status;
    }

    status = BindContextI18NUtil(env);
    if (status != ANI_OK) {
        return status;
    }

    status = BindContextPhoneNumberFormat(env);
    if (status != ANI_OK) {
        return status;
    }

    status = BindContextCalendar(env);
    if (status != ANI_OK) {
        return status;
    }

    status = BindContextBreakIterator(env);
    if (status != ANI_OK) {
        return status;
    }

    status = BindContextIndexUtil(env);
    if (status != ANI_OK) {
        return status;
    }

    status = BindContextUnicode(env);
    if (status != ANI_OK) {
        return status;
    }

    status = BindContextTimeZone(env);
    if (status != ANI_OK) {
        return status;
    }

    status = BindContext(env);
    *result = ANI_VERSION_1;
    return ANI_OK;
}