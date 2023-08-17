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
#include <chrono>
#include <unordered_map>
#include <vector>

#include "character.h"
#include "hilog/log.h"
#include "i18n_calendar_addon.h"
#include "i18n_normalizer_addon.h"
#include "system_locale_manager_addon.h"
#include "unicode/locid.h"
#include "unicode/datefmt.h"
#include "unicode/smpdtfmt.h"
#include "unicode/translit.h"
#include "node_api.h"
#include "variable_convertor.h"

#include "error_util.h"
#include "i18n_addon.h"

namespace OHOS {
namespace Global {
namespace I18n {
static constexpr OHOS::HiviewDFX::HiLogLabel LABEL = { LOG_CORE, 0xD001E00, "I18nJs" };
using namespace OHOS::HiviewDFX;

static thread_local napi_ref* g_brkConstructor = nullptr;
static thread_local napi_ref* g_timezoneConstructor = nullptr;
static thread_local napi_ref g_indexUtilConstructor = nullptr;
static thread_local napi_ref* g_transConstructor = nullptr;

static std::unordered_set<std::string> allValidLocalesLanguageTag;

I18nAddon::I18nAddon() : env_(nullptr) {}

I18nAddon::~I18nAddon()
{
}

void I18nAddon::Destructor(napi_env env, void *nativeObject, void *hint)
{
    if (!nativeObject) {
        return;
    }
    delete reinterpret_cast<I18nAddon *>(nativeObject);
    nativeObject = nullptr;
}

napi_value I18nAddon::CreateUnicodeObject(napi_env env, napi_status &initStatus)
{
    napi_status status = napi_ok;
    napi_value character = nullptr;
    status = napi_create_object(env, &character);
    if (status != napi_ok) {
        HiLog::Error(LABEL, "Failed to create character object at init");
        initStatus = napi_generic_failure;
        return nullptr;
    }
    napi_property_descriptor characterProperties[] = {
        DECLARE_NAPI_FUNCTION("isDigit", IsDigitAddon),
        DECLARE_NAPI_FUNCTION("isSpaceChar", IsSpaceCharAddon),
        DECLARE_NAPI_FUNCTION("isWhitespace", IsWhiteSpaceAddon),
        DECLARE_NAPI_FUNCTION("isRTL", IsRTLCharacterAddon),
        DECLARE_NAPI_FUNCTION("isIdeograph", IsIdeoGraphicAddon),
        DECLARE_NAPI_FUNCTION("isLetter", IsLetterAddon),
        DECLARE_NAPI_FUNCTION("isLowerCase", IsLowerCaseAddon),
        DECLARE_NAPI_FUNCTION("isUpperCase", IsUpperCaseAddon),
        DECLARE_NAPI_FUNCTION("getType", GetTypeAddon),
    };
    status = napi_define_properties(env, character,
                                    sizeof(characterProperties) / sizeof(napi_property_descriptor),
                                    characterProperties);
    if (status != napi_ok) {
        HiLog::Error(LABEL, "Failed to set properties of character at init");
        initStatus = napi_generic_failure;
        return nullptr;
    }
    return character;
}

napi_value I18nAddon::CreateI18nUtilObject(napi_env env, napi_status &initStatus)
{
    napi_value i18nUtil = nullptr;
    napi_status status = napi_create_object(env, &i18nUtil);
    if (status != napi_ok) {
        HiLog::Error(LABEL, "Failed to create I18nUtil object at init");
        initStatus = napi_generic_failure;
        return nullptr;
    }
    napi_property_descriptor i18nUtilProperties[] = {
        DECLARE_NAPI_FUNCTION("unitConvert", UnitConvert),
        DECLARE_NAPI_FUNCTION("getDateOrder", GetDateOrder),
        DECLARE_NAPI_FUNCTION("getTimePeriodName", GetTimePeriodName),
    };
    status = napi_define_properties(env, i18nUtil, sizeof(i18nUtilProperties) / sizeof(napi_property_descriptor),
        i18nUtilProperties);
    if (status != napi_ok) {
        HiLog::Error(LABEL, "Failed to set properties of I18nUtil at init");
        initStatus = napi_generic_failure;
        return nullptr;
    }
    return i18nUtil;
}

napi_value I18nAddon::Init(napi_env env, napi_value exports)
{
    napi_status initStatus = napi_ok;
    napi_property_descriptor properties[] = {
        DECLARE_NAPI_FUNCTION("getDisplayLanguage", GetDisplayLanguage),
        DECLARE_NAPI_FUNCTION("getDisplayCountry", GetDisplayCountry),
        DECLARE_NAPI_FUNCTION("getSystemLanguage", GetSystemLanguage),
        DECLARE_NAPI_FUNCTION("getSystemRegion", GetSystemRegion),
        DECLARE_NAPI_FUNCTION("getSystemLocale", GetSystemLocale),
        DECLARE_NAPI_FUNCTION("getCalendar", I18nCalendarAddon::GetCalendar),
        DECLARE_NAPI_FUNCTION("isRTL", IsRTL),
        DECLARE_NAPI_PROPERTY("I18NUtil", CreateI18nUtilObject(env, initStatus)),
        DECLARE_NAPI_FUNCTION("getLineInstance", GetLineInstance),
        DECLARE_NAPI_FUNCTION("getInstance", GetIndexUtil),
        DECLARE_NAPI_PROPERTY("Unicode", CreateUnicodeObject(env, initStatus)),
        DECLARE_NAPI_FUNCTION("addPreferredLanguage", AddPreferredLanguage),
        DECLARE_NAPI_FUNCTION("removePreferredLanguage", RemovePreferredLanguage),
        DECLARE_NAPI_FUNCTION("getPreferredLanguageList", GetPreferredLanguageList),
        DECLARE_NAPI_FUNCTION("getFirstPreferredLanguage", GetFirstPreferredLanguage),
        DECLARE_NAPI_FUNCTION("is24HourClock", Is24HourClock),
        DECLARE_NAPI_FUNCTION("set24HourClock", Set24HourClock),
        DECLARE_NAPI_FUNCTION("getTimeZone", GetI18nTimeZone),
        DECLARE_NAPI_PROPERTY("Transliterator", CreateTransliteratorObject(env, initStatus)),
        DECLARE_NAPI_PROPERTY("TimeZone", CreateTimeZoneObject(env, initStatus)),
        DECLARE_NAPI_PROPERTY("System", CreateSystemObject(env, initStatus)),
        DECLARE_NAPI_PROPERTY("NormalizerMode", I18nNormalizerAddon::CreateI18NNormalizerModeEnum(env, initStatus))
    };
    initStatus = napi_define_properties(env, exports, sizeof(properties) / sizeof(napi_property_descriptor),
        properties);
    if (initStatus != napi_ok) {
        HiLog::Error(LABEL, "Failed to set properties at init");
        return nullptr;
    }
    return exports;
}

void GetOptionValue(napi_env env, napi_value options, const std::string &optionName,
    std::string &value)
{
    napi_value optionValue = nullptr;
    napi_valuetype type = napi_undefined;
    napi_status status = napi_typeof(env, options, &type);
    if (status != napi_ok && type != napi_object) {
        HiLog::Error(LABEL, "Get option failed, option is not an object");
        return;
    }
    bool hasProperty = false;
    napi_status propStatus = napi_has_named_property(env, options, optionName.c_str(), &hasProperty);
    if (propStatus == napi_ok && hasProperty) {
        status = napi_get_named_property(env, options, optionName.c_str(), &optionValue);
        if (status == napi_ok) {
            size_t len;
            napi_get_value_string_utf8(env, optionValue, nullptr, 0, &len);
            std::vector<char> optionBuf(len + 1);
            status = napi_get_value_string_utf8(env, optionValue, optionBuf.data(), len + 1, &len);
            if (status != napi_ok) {
                HiLog::Error(LABEL, "Failed to get string item");
                return;
            }
            value = optionBuf.data();
        }
    }
}

void GetOptionMap(napi_env env, napi_value option, std::map<std::string, std::string> &map)
{
    if (VariableConvertor::CheckNapiValueType(env, option)) {
        size_t len;
        napi_get_value_string_utf8(env, option, nullptr, 0, &len);
        std::vector<char> styleBuf(len + 1);
        napi_status status = napi_get_value_string_utf8(env, option, styleBuf.data(), len + 1, &len);
        if (status != napi_ok) {
            HiLog::Error(LABEL, "Failed to get string item");
            return;
        }
        map.insert(std::make_pair("unitDisplay", styleBuf.data()));
    }
}

napi_value I18nAddon::UnitConvert(napi_env env, napi_callback_info info)
{
    size_t argc = 5;
    napi_value argv[5] = { 0 };
    napi_value thisVar = nullptr;
    void *data = nullptr;
    napi_status status = napi_get_cb_info(env, info, &argc, argv, &thisVar, &data);
    if (status != napi_ok) {
        return nullptr;
    }
    std::string fromUnit;
    GetOptionValue(env, argv[0], "unit", fromUnit);
    std::string fromMeasSys;
    GetOptionValue(env, argv[0], "measureSystem", fromMeasSys);
    std::string toUnit;
    GetOptionValue(env, argv[1], "unit", toUnit);
    std::string toMeasSys;
    GetOptionValue(env, argv[1], "measureSystem", toMeasSys);
    double number = 0;
    napi_get_value_double(env, argv[2], &number); // 2 is the index of value
    int convertStatus = Convert(number, fromUnit, fromMeasSys, toUnit, toMeasSys);
    size_t len;
    napi_get_value_string_utf8(env, argv[3], nullptr, 0, &len); // 3 is the index of value
    std::vector<char> localeBuf(len + 1);
    // 3 is the index of value
    status = napi_get_value_string_utf8(env, argv[3], localeBuf.data(), len + 1, &len);
    if (status != napi_ok) {
        return nullptr;
    }
    std::vector<std::string> localeTags;
    localeTags.push_back(localeBuf.data());
    std::map<std::string, std::string> map = {};
    map.insert(std::make_pair("style", "unit"));
    if (!convertStatus) {
        map.insert(std::make_pair("unit", fromUnit));
    } else {
        map.insert(std::make_pair("unit", toUnit));
    }
    // 4 is the index of value
    GetOptionMap(env, argv[4], map);
    std::unique_ptr<NumberFormat> numberFmt = nullptr;
    numberFmt = std::make_unique<NumberFormat>(localeTags, map);
    std::string value = numberFmt->Format(number);
    napi_value result;
    status = napi_create_string_utf8(env, value.c_str(), NAPI_AUTO_LENGTH, &result);
    if (status != napi_ok) {
        HiLog::Error(LABEL, "Failed to create string item");
        return nullptr;
    }
    return result;
}

napi_value I18nAddon::GetDateOrder(napi_env env, napi_callback_info info)
{
    size_t argc = 1;
    napi_value argv[1] = { 0 };
    napi_value thisVar = nullptr;
    void *data = nullptr;
    napi_status status = napi_get_cb_info(env, info, &argc, argv, &thisVar, &data);
    if (status != napi_ok) {
        return nullptr;
    }
    size_t len = 0;
    napi_get_value_string_utf8(env, argv[0], nullptr, 0, &len);
    std::vector<char> languageBuf(len + 1);
    status = napi_get_value_string_utf8(env, argv[0], languageBuf.data(), len + 1, &len);
    if (status != napi_ok) {
        HiLog::Error(LABEL, "Failed to get locale string for GetDateOrder");
        return nullptr;
    }
    UErrorCode icuStatus = U_ZERO_ERROR;
    icu::Locale locale = icu::Locale::forLanguageTag(languageBuf.data(), icuStatus);
    if (icuStatus != U_ZERO_ERROR) {
        HiLog::Error(LABEL, "Failed to create locale for GetDateOrder");
        return nullptr;
    }
    icu::SimpleDateFormat* formatter = dynamic_cast<icu::SimpleDateFormat*>
        (icu::DateFormat::createDateInstance(icu::DateFormat::EStyle::kDefault, locale));
    if (icuStatus != U_ZERO_ERROR || formatter == nullptr) {
        HiLog::Error(LABEL, "Failed to create SimpleDateFormat");
        return nullptr;
    }
    std::string tempValue;
    icu::UnicodeString unistr;
    formatter->toPattern(unistr);
    delete formatter;
    unistr.toUTF8String<std::string>(tempValue);
    std::string value = ModifyOrder(tempValue);
    napi_value result;
    status = napi_create_string_utf8(env, value.c_str(), NAPI_AUTO_LENGTH, &result);
    if (status != napi_ok) {
        HiLog::Error(LABEL, "Failed to create string item");
        return nullptr;
    }
    return result;
}

std::string I18nAddon::ModifyOrder(std::string &pattern)
{
    int order[3] = { 0 }; // total 3 elements 'y', 'M'/'L', 'd'
    int lengths[4] = { 0 }; // first elements is the currently found elememnts, thus 4 elements totally.
    bool flag = true;
    for (size_t i = 0; i < pattern.length(); ++i) {
        char ch = pattern[i];
        if (flag && std::isalpha(ch)) {
            ProcessNormal(ch, order, 3, lengths, 4); // 3, 4 are lengths of these arrays
        } else if (ch == '\'') {
            flag = !flag;
        }
    }
    std::unordered_map<char, int> pattern2index = {
        { 'y', 1 },
        { 'L', 2 },
        { 'd', 3 },
    };
    std::string ret;
    for (int i = 0; i < 3; ++i) { // 3 is the size of orders
        auto it = pattern2index.find(order[i]);
        if (it == pattern2index.end()) {
            continue;
        }
        int index = it->second;
        if ((lengths[index] > 0) && (lengths[index] <= 6)) { // 6 is the max length of a filed
            ret.append(lengths[index], order[i]);
        }
        if (i < 2) { // 2 is the size of the order minus one
            ret.append(1, '-');
        }
    }
    return ret;
}

napi_value I18nAddon::GetTimePeriodName(napi_env env, napi_callback_info info)
{
    napi_value result;
    int32_t hour;
    std::string localeTag;
    if (GetParamOfGetTimePeriodName(env, info, localeTag, hour) == -1) {
        HiLog::Error(LABEL, "GetTimePeriodName param error");
        napi_create_string_utf8(env, "", NAPI_AUTO_LENGTH, &result);
        return result;
    }

    UErrorCode icuStatus = U_ZERO_ERROR;
    icu::Locale locale = icu::Locale::forLanguageTag(localeTag.data(), icuStatus);
    if (U_FAILURE(icuStatus) || !IsValidLocaleTag(locale)) {
        HiLog::Error(LABEL, "GetTimePeriodName does not support this locale");
        ErrorUtil::NapiThrow(env, I18N_NOT_VALID, true);
    }
    icu::SimpleDateFormat* formatter = dynamic_cast<icu::SimpleDateFormat*>
        (icu::DateFormat::createDateInstance(icu::DateFormat::EStyle::kDefault, locale));
    if (!formatter) {
        HiLog::Error(LABEL, "GetTimePeriodName Failed to create SimpleDateFormat");
        napi_create_string_utf8(env, "", NAPI_AUTO_LENGTH, &result);
        return result;
    }
    formatter->applyPattern("B");

    std::string temp;
    icu::UnicodeString name;
    icu::Calendar *calendar = icu::Calendar::createInstance(locale, icuStatus);
    calendar->set(UCalendarDateFields::UCAL_HOUR_OF_DAY, hour);
    formatter->format(calendar->getTime(icuStatus), name);
    name.toUTF8String(temp);
    napi_create_string_utf8(env, temp.c_str(), NAPI_AUTO_LENGTH, &result);
    delete formatter;
    delete calendar;
    return result;
}

int I18nAddon::GetParamOfGetTimePeriodName(napi_env env, napi_callback_info info, std::string &tag, int32_t &hour)
{
    size_t argc = 2;
    napi_value argv[2] = { 0 };
    napi_value thisVar = nullptr;
    void *data = nullptr;
    napi_status status = napi_get_cb_info(env, info, &argc, argv, &thisVar, &data);
    if (status != napi_ok) {
        HiLog::Error(LABEL, "GetTimePeriodName can't get parameters from getTimePerioudName.");
        return -1;
    }

    napi_valuetype valueType = napi_valuetype::napi_undefined;
    napi_typeof(env, argv[0], &valueType);
    if (valueType != napi_valuetype::napi_number) {
        HiLog::Error(LABEL, "GetTimePeriodName Parameter type does not match");
        ErrorUtil::NapiThrow(env, I18N_NOT_FOUND, true);
    }
    status = napi_get_value_int32(env, argv[0], &hour);
    if (status != napi_ok) {
        HiLog::Error(LABEL, "GetTimePeriodName can't get number from js param");
        return -1;
    }

    valueType = napi_valuetype::napi_undefined;
    napi_typeof(env, argv[1], &valueType);
    if (valueType == napi_valuetype::napi_null || valueType == napi_valuetype::napi_undefined) {
        tag = LocaleConfig::GetSystemLocale();
    } else if (valueType == napi_valuetype::napi_string) {
        int code = 0;
        tag = VariableConvertor::GetString(env, argv[1], code);
        if (code) {
            HiLog::Error(LABEL, "GetTimePeriodName can't get string from js param");
            return -1;
        }
    } else {
        HiLog::Error(LABEL, "GetTimePeriodName Parameter type does not match");
        ErrorUtil::NapiThrow(env, I18N_NOT_FOUND, true);
    }
    return 0;
}

bool I18nAddon::IsValidLocaleTag(icu::Locale &locale)
{
    GetAllValidLocalesTag();
    std::string languageTag = locale.getLanguage();
    if (allValidLocalesLanguageTag.find(languageTag) == allValidLocalesLanguageTag.end()) {
        HiLog::Error(LABEL, "GetTimePeriodName does not support this languageTag: %{public}s", languageTag.c_str());
        return false;
    }
    return true;
}

void I18nAddon::GetAllValidLocalesTag() {
    static bool init = false;
    if (init) {
        return;
    }
    int32_t validCount = 1;
    const icu::Locale *validLocales = icu::Locale::getAvailableLocales(validCount);
    for (int i = 0; i < validCount; i++) {
        allValidLocalesLanguageTag.insert(validLocales[i].getLanguage());
    }
    init = true;
}

void I18nAddon::ProcessNormal(char ch, int *order, size_t orderSize, int *lengths, size_t lengsSize)
{
    char adjust;
    int index = -1;
    if (ch == 'd') {
        adjust = 'd';
        index = 3; // 3 is the index of 'd'
    } else if ((ch == 'L') || (ch == 'M')) {
        adjust = 'L';
        index = 2; // 2 is the index of 'L'
    } else if (ch == 'y') {
        adjust = 'y';
        index = 1;
    } else {
        return;
    }
    if ((index < 0) || (index >= static_cast<int>(lengsSize))) {
        return;
    }
    if (lengths[index] == 0) {
        if (lengths[0] >= 3) { // 3 is the index of order
            return;
        }
        order[lengths[0]] = static_cast<int>(adjust);
        ++lengths[0];
        lengths[index] = 1;
    } else {
        ++lengths[index];
    }
}

napi_value I18nAddon::InitTransliterator(napi_env env, napi_value exports)
{
    napi_status status = napi_ok;
    napi_property_descriptor properties[] = {
        DECLARE_NAPI_FUNCTION("transform", Transform),
    };
    napi_value constructor = nullptr;
    status = napi_define_class(env, "Transliterator", NAPI_AUTO_LENGTH, TransliteratorConstructor, nullptr,
        sizeof(properties) / sizeof(napi_property_descriptor), properties, &constructor);
    if (status != napi_ok) {
        HiLog::Error(LABEL, "Failed to define transliterator class at Init");
        return nullptr;
    }
    g_transConstructor = new (std::nothrow) napi_ref;
    if (!g_transConstructor) {
        HiLog::Error(LABEL, "Failed to create trans ref at init");
        return nullptr;
    }
    status = napi_create_reference(env, constructor, 1, g_transConstructor);
    if (status != napi_ok) {
        HiLog::Error(LABEL, "Failed to create trans reference at init");
        return nullptr;
    }
    return exports;
}

napi_value I18nAddon::TransliteratorConstructor(napi_env env, napi_callback_info info)
{
    size_t argc = 1;
    napi_value argv[1] = { 0 };
    napi_value thisVar = nullptr;
    void *data = nullptr;
    napi_status status = napi_get_cb_info(env, info, &argc, argv, &thisVar, &data);
    if (status != napi_ok) {
        return nullptr;
    }
    napi_valuetype valueType = napi_valuetype::napi_undefined;
    napi_typeof(env, argv[0], &valueType);
    if (valueType != napi_valuetype::napi_string) {
        napi_throw_type_error(env, nullptr, "Parameter type does not match");
        return nullptr;
    }
    int32_t code = 0;
    std::string idTag = VariableConvertor::GetString(env, argv[0], code);
    if (code) {
        return nullptr;
    }
    std::unique_ptr<I18nAddon> obj = nullptr;
    obj = std::make_unique<I18nAddon>();
    status =
        napi_wrap(env, thisVar, reinterpret_cast<void *>(obj.get()), I18nAddon::Destructor, nullptr, nullptr);
    if (status != napi_ok) {
        HiLog::Error(LABEL, "TransliteratorConstructor: Wrap II18nAddon failed");
        return nullptr;
    }
    if (!obj->InitTransliteratorContext(env, info, idTag)) {
        obj.release();
        return nullptr;
    }
    obj.release();
    return thisVar;
}

bool I18nAddon::InitTransliteratorContext(napi_env env, napi_callback_info info, const std::string &idTag)
{
    UErrorCode status = U_ZERO_ERROR;
    icu::UnicodeString unistr = icu::UnicodeString::fromUTF8(idTag);
    icu::Transliterator *trans = icu::Transliterator::createInstance(unistr, UTransDirection::UTRANS_FORWARD, status);
    if ((status != U_ZERO_ERROR) || (trans == nullptr)) {
        return false;
    }
    transliterator_ = std::unique_ptr<icu::Transliterator>(trans);
    return transliterator_ != nullptr;
}

napi_value I18nAddon::CreateTransliteratorObject(napi_env env, napi_status &initStatus)
{
    napi_status status = napi_ok;
    napi_value transliterator = nullptr;
    status = napi_create_object(env, &transliterator);
    if (status != napi_ok) {
        HiLog::Error(LABEL, "Failed to create transliterator object at init");
        initStatus = napi_generic_failure;
        return nullptr;
    }
    napi_property_descriptor transProperties[] = {
        DECLARE_NAPI_FUNCTION("getAvailableIDs", GetAvailableIDs),
        DECLARE_NAPI_FUNCTION("getInstance", GetTransliteratorInstance)
    };
    status = napi_define_properties(env, transliterator,
                                    sizeof(transProperties) / sizeof(napi_property_descriptor),
                                    transProperties);
    if (status != napi_ok) {
        HiLog::Error(LABEL, "Failed to set properties of transliterator at init");
        initStatus = napi_generic_failure;
        return nullptr;
    }
    return transliterator;
}

napi_value I18nAddon::Transform(napi_env env, napi_callback_info info)
{
    size_t argc = 1;
    napi_value argv[1] = { nullptr };
    napi_value thisVar = nullptr;
    void *data = nullptr;
    napi_get_cb_info(env, info, &argc, argv, &thisVar, &data);
    I18nAddon *obj = nullptr;
    napi_status status = napi_unwrap(env, thisVar, reinterpret_cast<void **>(&obj));
    if (status != napi_ok || !obj || !obj->transliterator_) {
        HiLog::Error(LABEL, "Get Transliterator object failed");
        return nullptr;
    }
    if (!argv[0]) {
        return nullptr;
    }
    napi_valuetype valueType = napi_valuetype::napi_undefined;
    napi_typeof(env, argv[0], &valueType);
    if (valueType != napi_valuetype::napi_string) {
        napi_throw_type_error(env, nullptr, "Parameter type does not match");
        return nullptr;
    }
    size_t len = 0;
    status = napi_get_value_string_utf8(env, argv[0], nullptr, 0, &len);
    if (status != napi_ok) {
        HiLog::Error(LABEL, "Get field length failed");
        return nullptr;
    }
    std::vector<char> buf(len + 1);
    status = napi_get_value_string_utf8(env, argv[0], buf.data(), len + 1, &len);
    if (status != napi_ok) {
        HiLog::Error(LABEL, "Get string value failed");
        return nullptr;
    }
    icu::UnicodeString unistr = icu::UnicodeString::fromUTF8(buf.data());
    obj->transliterator_->transliterate(unistr);
    std::string temp;
    unistr.toUTF8String(temp);
    napi_value value;
    status = napi_create_string_utf8(env, temp.c_str(), NAPI_AUTO_LENGTH, &value);
    if (status != napi_ok) {
        HiLog::Error(LABEL, "Get field length failed");
        return nullptr;
    }
    return value;
}

napi_value I18nAddon::GetAvailableIDs(napi_env env, napi_callback_info info)
{
    size_t argc = 0;
    napi_value *argv = nullptr;
    napi_value thisVar = nullptr;
    void *data = nullptr;
    napi_status status = napi_get_cb_info(env, info, &argc, argv, &thisVar, &data);
    if (status != napi_ok) {
        return nullptr;
    }
    UErrorCode icuStatus = U_ZERO_ERROR;
    icu::StringEnumeration *strenum = icu::Transliterator::getAvailableIDs(icuStatus);
    if (icuStatus != U_ZERO_ERROR) {
        HiLog::Error(LABEL, "Failed to get available ids");
        if (strenum) {
            delete strenum;
        }
        return nullptr;
    }

    napi_value result = nullptr;
    napi_create_array(env, &result);
    uint32_t i = 0;
    const char *temp = nullptr;
    if (strenum == nullptr) {
        return nullptr;
    }
    while ((temp = strenum->next(nullptr, icuStatus)) != nullptr) {
        if (icuStatus != U_ZERO_ERROR) {
            break;
        }
        napi_value val = nullptr;
        napi_create_string_utf8(env, temp, strlen(temp), &val);
        napi_set_element(env, result, i, val);
        ++i;
    }
    delete strenum;
    return result;
}

napi_value I18nAddon::GetTransliteratorInstance(napi_env env, napi_callback_info info)
{
    size_t argc = 1; // retrieve 2 arguments
    napi_value argv[1] = { 0 };
    napi_value thisVar = nullptr;
    void *data = nullptr;
    napi_get_cb_info(env, info, &argc, argv, &thisVar, &data);
    napi_value constructor = nullptr;
    napi_status status = napi_get_reference_value(env, *g_transConstructor, &constructor);
    if (status != napi_ok) {
        HiLog::Error(LABEL, "Failed to create reference at GetCalendar");
        return nullptr;
    }
    napi_value result = nullptr;
    status = napi_new_instance(env, constructor, 1, argv, &result); // 2 arguments
    if (status != napi_ok) {
        HiLog::Error(LABEL, "Get Transliterator create instance failed");
        return nullptr;
    }
    return result;
}

napi_value I18nAddon::IsDigitAddon(napi_env env, napi_callback_info info)
{
    size_t argc = 1;
    napi_value argv[1] = { 0 };
    napi_value thisVar = nullptr;
    void *data = nullptr;
    napi_status status = napi_get_cb_info(env, info, &argc, argv, &thisVar, &data);
    if (status != napi_ok) {
        return nullptr;
    }
    napi_valuetype valueType = napi_valuetype::napi_undefined;
    napi_typeof(env, argv[0], &valueType);
    if (valueType != napi_valuetype::napi_string) {
        napi_throw_type_error(env, nullptr, "Parameter type does not match");
        return nullptr;
    }
    int32_t code = 0;
    std::string character = VariableConvertor::GetString(env, argv[0], code);
    if (code) {
        return nullptr;
    }
    bool isDigit = IsDigit(character);
    napi_value result = nullptr;
    status = napi_get_boolean(env, isDigit, &result);
    if (status != napi_ok) {
        HiLog::Error(LABEL, "Create isDigit boolean value failed");
        return nullptr;
    }
    return result;
}

napi_value I18nAddon::IsSpaceCharAddon(napi_env env, napi_callback_info info)
{
    size_t argc = 1;
    napi_value argv[1] = { 0 };
    napi_value thisVar = nullptr;
    void *data = nullptr;
    napi_status status = napi_get_cb_info(env, info, &argc, argv, &thisVar, &data);
    if (status != napi_ok) {
        return nullptr;
    }
    napi_valuetype valueType = napi_valuetype::napi_undefined;
    napi_typeof(env, argv[0], &valueType);
    if (valueType != napi_valuetype::napi_string) {
        napi_throw_type_error(env, nullptr, "Parameter type does not match");
        return nullptr;
    }
    int32_t code = 0;
    std::string character = VariableConvertor::GetString(env, argv[0], code);
    if (code) {
        return nullptr;
    }
    bool isSpaceChar = IsSpaceChar(character);
    napi_value result = nullptr;
    status = napi_get_boolean(env, isSpaceChar, &result);
    if (status != napi_ok) {
        HiLog::Error(LABEL, "Create isSpaceChar boolean value failed");
        return nullptr;
    }
    return result;
}

napi_value I18nAddon::IsWhiteSpaceAddon(napi_env env, napi_callback_info info)
{
    size_t argc = 1;
    napi_value argv[1] = { 0 };
    napi_value thisVar = nullptr;
    void *data = nullptr;
    napi_status status = napi_get_cb_info(env, info, &argc, argv, &thisVar, &data);
    if (status != napi_ok) {
        return nullptr;
    }
    napi_valuetype valueType = napi_valuetype::napi_undefined;
    napi_typeof(env, argv[0], &valueType);
    if (valueType != napi_valuetype::napi_string) {
        napi_throw_type_error(env, nullptr, "Parameter type does not match");
        return nullptr;
    }
    int32_t code = 0;
    std::string character = VariableConvertor::GetString(env, argv[0], code);
    if (code) {
        return nullptr;
    }
    bool isWhiteSpace = IsWhiteSpace(character);
    napi_value result = nullptr;
    status = napi_get_boolean(env, isWhiteSpace, &result);
    if (status != napi_ok) {
        HiLog::Error(LABEL, "Create isWhiteSpace boolean value failed");
        return nullptr;
    }
    return result;
}

napi_value I18nAddon::IsRTLCharacterAddon(napi_env env, napi_callback_info info)
{
    size_t argc = 1;
    napi_value argv[1] = { 0 };
    napi_value thisVar = nullptr;
    void *data = nullptr;
    napi_status status = napi_get_cb_info(env, info, &argc, argv, &thisVar, &data);
    if (status != napi_ok) {
        return nullptr;
    }
    napi_valuetype valueType = napi_valuetype::napi_undefined;
    napi_typeof(env, argv[0], &valueType);
    if (valueType != napi_valuetype::napi_string) {
        napi_throw_type_error(env, nullptr, "Parameter type does not match");
        return nullptr;
    }
    int32_t code = 0;
    std::string character = VariableConvertor::GetString(env, argv[0], code);
    if (code) {
        return nullptr;
    }
    bool isRTLCharacter = IsRTLCharacter(character);
    napi_value result = nullptr;
    status = napi_get_boolean(env, isRTLCharacter, &result);
    if (status != napi_ok) {
        HiLog::Error(LABEL, "Create isRTLCharacter boolean value failed");
        return nullptr;
    }
    return result;
}

napi_value I18nAddon::IsIdeoGraphicAddon(napi_env env, napi_callback_info info)
{
    size_t argc = 1;
    napi_value argv[1] = { 0 };
    napi_value thisVar = nullptr;
    void *data = nullptr;
    napi_status status = napi_get_cb_info(env, info, &argc, argv, &thisVar, &data);
    if (status != napi_ok) {
        return nullptr;
    }
    napi_valuetype valueType = napi_valuetype::napi_undefined;
    napi_typeof(env, argv[0], &valueType);
    if (valueType != napi_valuetype::napi_string) {
        napi_throw_type_error(env, nullptr, "Parameter type does not match");
        return nullptr;
    }
    int32_t code = 0;
    std::string character = VariableConvertor::GetString(env, argv[0], code);
    if (code) {
        return nullptr;
    }
    bool isIdeoGraphic = IsIdeoGraphic(character);
    napi_value result = nullptr;
    status = napi_get_boolean(env, isIdeoGraphic, &result);
    if (status != napi_ok) {
        HiLog::Error(LABEL, "Create isIdeoGraphic boolean value failed");
        return nullptr;
    }
    return result;
}

napi_value I18nAddon::IsLetterAddon(napi_env env, napi_callback_info info)
{
    size_t argc = 1;
    napi_value argv[1] = { 0 };
    napi_value thisVar = nullptr;
    void *data = nullptr;
    napi_status status = napi_get_cb_info(env, info, &argc, argv, &thisVar, &data);
    if (status != napi_ok) {
        return nullptr;
    }
    napi_valuetype valueType = napi_valuetype::napi_undefined;
    napi_typeof(env, argv[0], &valueType);
    if (valueType != napi_valuetype::napi_string) {
        napi_throw_type_error(env, nullptr, "Parameter type does not match");
        return nullptr;
    }
    int32_t code = 0;
    std::string character = VariableConvertor::GetString(env, argv[0], code);
    if (code) {
        return nullptr;
    }
    bool isLetter = IsLetter(character);
    napi_value result = nullptr;
    status = napi_get_boolean(env, isLetter, &result);
    if (status != napi_ok) {
        HiLog::Error(LABEL, "Create isLetter boolean value failed");
        return nullptr;
    }
    return result;
}

napi_value I18nAddon::IsLowerCaseAddon(napi_env env, napi_callback_info info)
{
    size_t argc = 1;
    napi_value argv[1] = { 0 };
    napi_value thisVar = nullptr;
    void *data = nullptr;
    napi_status status = napi_get_cb_info(env, info, &argc, argv, &thisVar, &data);
    if (status != napi_ok) {
        return nullptr;
    }
    napi_valuetype valueType = napi_valuetype::napi_undefined;
    napi_typeof(env, argv[0], &valueType);
    if (valueType != napi_valuetype::napi_string) {
        napi_throw_type_error(env, nullptr, "Parameter type does not match");
        return nullptr;
    }
    int32_t code = 0;
    std::string character = VariableConvertor::GetString(env, argv[0], code);
    if (code) {
        return nullptr;
    }
    bool isLowerCase = IsLowerCase(character);
    napi_value result = nullptr;
    status = napi_get_boolean(env, isLowerCase, &result);
    if (status != napi_ok) {
        HiLog::Error(LABEL, "Create isLowerCase boolean value failed");
        return nullptr;
    }
    return result;
}

napi_value I18nAddon::IsUpperCaseAddon(napi_env env, napi_callback_info info)
{
    size_t argc = 1;
    napi_value argv[1] = { 0 };
    napi_value thisVar = nullptr;
    void *data = nullptr;
    napi_status status = napi_get_cb_info(env, info, &argc, argv, &thisVar, &data);
    if (status != napi_ok) {
        return nullptr;
    }
    napi_valuetype valueType = napi_valuetype::napi_undefined;
    napi_typeof(env, argv[0], &valueType);
    if (valueType != napi_valuetype::napi_string) {
        napi_throw_type_error(env, nullptr, "Parameter type does not match");
        return nullptr;
    }
    int32_t code = 0;
    std::string character = VariableConvertor::GetString(env, argv[0], code);
    if (code) {
        return nullptr;
    }
    bool isUpperCase = IsUpperCase(character);
    napi_value result = nullptr;
    status = napi_get_boolean(env, isUpperCase, &result);
    if (status != napi_ok) {
        HiLog::Error(LABEL, "Create isUpperCase boolean value failed");
        return nullptr;
    }
    return result;
}

napi_value I18nAddon::GetTypeAddon(napi_env env, napi_callback_info info)
{
    size_t argc = 1;
    napi_value argv[1] = { 0 };
    napi_value thisVar = nullptr;
    void *data = nullptr;
    napi_status status = napi_get_cb_info(env, info, &argc, argv, &thisVar, &data);
    if (status != napi_ok) {
        return nullptr;
    }
    napi_valuetype valueType = napi_valuetype::napi_undefined;
    napi_typeof(env, argv[0], &valueType);
    if (valueType != napi_valuetype::napi_string) {
        napi_throw_type_error(env, nullptr, "Parameter type does not match");
        return nullptr;
    }
    int32_t code = 0;
    std::string character = VariableConvertor::GetString(env, argv[0], code);
    if (code) {
        return nullptr;
    }
    std::string type = GetType(character);
    napi_value result = nullptr;
    status = napi_create_string_utf8(env, type.c_str(), NAPI_AUTO_LENGTH, &result);
    if (status != napi_ok) {
        HiLog::Error(LABEL, "Create getType string value failed");
        return nullptr;
    }
    return result;
}

napi_value I18nAddon::GetSystemLanguages(napi_env env, napi_callback_info info)
{
    std::vector<std::string> systemLanguages;
    LocaleConfig::GetSystemLanguages(systemLanguages);
    napi_value result = nullptr;
    napi_status status = napi_create_array_with_length(env, systemLanguages.size(), &result);
    if (status != napi_ok) {
        HiLog::Error(LABEL, "Failed to create array");
        return nullptr;
    }
    for (size_t i = 0; i < systemLanguages.size(); i++) {
        napi_value value = nullptr;
        status = napi_create_string_utf8(env, systemLanguages[i].c_str(), NAPI_AUTO_LENGTH, &value);
        if (status != napi_ok) {
            HiLog::Error(LABEL, "GetSystemLanguages: Failed to create string item");
            return nullptr;
        }
        status = napi_set_element(env, result, i, value);
        if (status != napi_ok) {
            HiLog::Error(LABEL, "GetSystemLanguages: Failed to set array item");
            return nullptr;
        }
    }
    return result;
}

napi_value I18nAddon::GetSystemCountries(napi_env env, napi_callback_info info)
{
    return I18nAddon::GetSystemCountriesImpl(env, info, false);
}

napi_value I18nAddon::GetSystemCountriesWithError(napi_env env, napi_callback_info info)
{
    return I18nAddon::GetSystemCountriesImpl(env, info, true);
}

napi_value I18nAddon::GetSystemCountriesImpl(napi_env env, napi_callback_info info, bool throwError)
{
    size_t argc = 1;
    napi_value argv[1] = { 0 };
    napi_value thisVar = nullptr;
    void *data = nullptr;
    napi_status status = napi_get_cb_info(env, info, &argc, argv, &thisVar, &data);
    if (status != napi_ok) {
        return nullptr;
    }
    if (argv[0] == nullptr) {
        HiLog::Error(LABEL, "Missing parameter");
        ErrorUtil::NapiThrow(env, I18N_NOT_FOUND, throwError);
        return nullptr;
    }
    size_t len = 0;
    napi_get_value_string_utf8(env, argv[0], nullptr, 0, &len);
    std::vector<char> localeBuf(len + 1);
    status = napi_get_value_string_utf8(env, argv[0], localeBuf.data(), len + 1, &len);
    if (status != napi_ok) {
        HiLog::Error(LABEL, "Failed to get string item");
        ErrorUtil::NapiThrow(env, I18N_NOT_VALID, throwError);
        return nullptr;
    }
    std::vector<std::string> systemCountries;
    LocaleConfig::GetSystemCountries(systemCountries);
    napi_value result = nullptr;
    status = napi_create_array_with_length(env, systemCountries.size(), &result);
    if (status != napi_ok) {
        HiLog::Error(LABEL, "Failed to create array");
        return nullptr;
    }
    for (size_t i = 0; i < systemCountries.size(); i++) {
        napi_value value = nullptr;
        status = napi_create_string_utf8(env, systemCountries[i].c_str(), NAPI_AUTO_LENGTH, &value);
        if (status != napi_ok) {
            HiLog::Error(LABEL, "GetSystemCountries: Failed to create string item");
            return nullptr;
        }
        status = napi_set_element(env, result, i, value);
        if (status != napi_ok) {
            HiLog::Error(LABEL, "GetSystemCountries: Failed to set array item");
            return nullptr;
        }
    }
    return result;
}

napi_value I18nAddon::GetSystemLanguage(napi_env env, napi_callback_info info)
{
    std::string value = LocaleConfig::GetSystemLanguage();
    napi_value result = nullptr;
    napi_status status = napi_create_string_utf8(env, value.c_str(), NAPI_AUTO_LENGTH, &result);
    if (status != napi_ok) {
        HiLog::Error(LABEL, "Failed to create string item");
        return nullptr;
    }
    return result;
}

napi_value I18nAddon::GetSystemRegion(napi_env env, napi_callback_info info)
{
    std::string value = LocaleConfig::GetSystemRegion();
    napi_value result = nullptr;
    napi_status status = napi_create_string_utf8(env, value.c_str(), NAPI_AUTO_LENGTH, &result);
    if (status != napi_ok) {
        HiLog::Error(LABEL, "Failed to create string item");
        return nullptr;
    }
    return result;
}

napi_value I18nAddon::GetSystemLocale(napi_env env, napi_callback_info info)
{
    std::string value = LocaleConfig::GetSystemLocale();
    napi_value result = nullptr;
    napi_status status = napi_create_string_utf8(env, value.c_str(), NAPI_AUTO_LENGTH, &result);
    if (status != napi_ok) {
        HiLog::Error(LABEL, "Failed to create string item");
        return nullptr;
    }
    return result;
}

napi_value I18nAddon::GetDisplayLanguage(napi_env env, napi_callback_info info)
{
    return I18nAddon::GetDisplayLanguageImpl(env, info, false);
}

napi_value I18nAddon::GetDisplayLanguageWithError(napi_env env, napi_callback_info info)
{
    return I18nAddon::GetDisplayLanguageImpl(env, info, true);
}

napi_value I18nAddon::GetDisplayLanguageImpl(napi_env env, napi_callback_info info, bool throwError)
{
    // Need to get three parameters to get the display Language.
    size_t argc = 3;
    napi_value argv[3] = { 0 };
    napi_value thisVar = nullptr;
    void *data = nullptr;
    napi_status status = napi_get_cb_info(env, info, &argc, argv, &thisVar, &data);
    if (status != napi_ok) {
        return nullptr;
    }
    if (argv[1] == nullptr) {
        HiLog::Error(LABEL, "Missing parameter");
        ErrorUtil::NapiThrow(env, I18N_NOT_FOUND, throwError);
        return nullptr;
    }
    size_t len = 0;
    napi_get_value_string_utf8(env, argv[0], nullptr, 0, &len);
    std::vector<char> localeBuf(len + 1);
    status = napi_get_value_string_utf8(env, argv[0], localeBuf.data(), len + 1, &len);
    if (status != napi_ok) {
        HiLog::Error(LABEL, "Failed to get string item");
        ErrorUtil::NapiThrow(env, I18N_NOT_VALID, throwError);
        return nullptr;
    }
    napi_get_value_string_utf8(env, argv[1], nullptr, 0, &len);
    std::vector<char> displayLocaleBuf(len + 1);
    status = napi_get_value_string_utf8(env, argv[1], displayLocaleBuf.data(), len + 1, &len);
    if (status != napi_ok) {
        HiLog::Error(LABEL, "Failed to get string item");
        ErrorUtil::NapiThrow(env, I18N_NOT_VALID, throwError);
        return nullptr;
    }
    bool sentenceCase = true;
    int sentenceCaseIndex = 2;
    if (argv[sentenceCaseIndex] != nullptr) {
        napi_get_value_bool(env, argv[sentenceCaseIndex], &sentenceCase);
    }

    std::string value = LocaleConfig::GetDisplayLanguage(localeBuf.data(), displayLocaleBuf.data(), sentenceCase);
    if (value.length() == 0) {
        ErrorUtil::NapiThrow(env, I18N_NOT_VALID, throwError);
        return nullptr;
    }
    napi_value result = nullptr;
    status = napi_create_string_utf8(env, value.c_str(), NAPI_AUTO_LENGTH, &result);
    if (status != napi_ok) {
        HiLog::Error(LABEL, "Failed to create string item");
        return nullptr;
    }
    return result;
}

napi_value I18nAddon::GetDisplayCountry(napi_env env, napi_callback_info info)
{
    return I18nAddon::GetDisplayCountryImpl(env, info, false);
}

napi_value I18nAddon::GetDisplayCountryWithError(napi_env env, napi_callback_info info)
{
    return I18nAddon::GetDisplayCountryImpl(env, info, true);
}

napi_value I18nAddon::GetDisplayCountryImpl(napi_env env, napi_callback_info info, bool throwError)
{
    // Need to get three parameters to get the display country.
    size_t argc = 3;
    napi_value argv[3] = { 0 };
    napi_value thisVar = nullptr;
    void *data = nullptr;
    napi_status status = napi_get_cb_info(env, info, &argc, argv, &thisVar, &data);
    if (status != napi_ok) {
        return nullptr;
    }
    if (argv[1] == nullptr) {
        HiLog::Error(LABEL, "Missing parameter");
        ErrorUtil::NapiThrow(env, I18N_NOT_FOUND, throwError);
        return nullptr;
    }
    size_t len = 0;
    napi_get_value_string_utf8(env, argv[0], nullptr, 0, &len);
    std::vector<char> localeBuf(len + 1);
    status = napi_get_value_string_utf8(env, argv[0], localeBuf.data(), len + 1, &len);
    if (status != napi_ok) {
        HiLog::Error(LABEL, "Failed to get string item");
        ErrorUtil::NapiThrow(env, I18N_NOT_VALID, throwError);
        return nullptr;
    }
    napi_get_value_string_utf8(env, argv[1], nullptr, 0, &len);
    std::vector<char> displayLocaleBuf(len + 1);
    status = napi_get_value_string_utf8(env, argv[1], displayLocaleBuf.data(), len + 1, &len);
    if (status != napi_ok) {
        HiLog::Error(LABEL, "Failed to get string item");
        ErrorUtil::NapiThrow(env, I18N_NOT_VALID, throwError);
        return nullptr;
    }
    bool sentenceCase = true;
    int sentenceCaseIndex = 2;
    if (argv[sentenceCaseIndex] != nullptr) {
        napi_get_value_bool(env, argv[sentenceCaseIndex], &sentenceCase);
    }
    std::string value = LocaleConfig::GetDisplayRegion(localeBuf.data(), displayLocaleBuf.data(), sentenceCase);
    if (value.length() == 0) {
        ErrorUtil::NapiThrow(env, I18N_NOT_VALID, throwError);
    }
    napi_value result = nullptr;
    status = napi_create_string_utf8(env, value.c_str(), NAPI_AUTO_LENGTH, &result);
    if (status != napi_ok) {
        HiLog::Error(LABEL, "Failed to create string item");
        return nullptr;
    }
    return result;
}

napi_value I18nAddon::IsSuggested(napi_env env, napi_callback_info info)
{
    return I18nAddon::IsSuggestedImpl(env, info, false);
}

napi_value I18nAddon::IsSuggestedWithError(napi_env env, napi_callback_info info)
{
    return I18nAddon::IsSuggestedImpl(env, info, true);
}

napi_value I18nAddon::IsSuggestedImpl(napi_env env, napi_callback_info info, bool throwError)
{
    // Need to get two parameters to check is suggested or not.
    size_t argc = 2;
    napi_value argv[2] = { 0 };
    napi_value thisVar = nullptr;
    void *data = nullptr;
    napi_status status = napi_get_cb_info(env, info, &argc, argv, &thisVar, &data);
    if (status != napi_ok) {
        return nullptr;
    }
    if (argv[0] == nullptr) {
        HiLog::Error(LABEL, "Missing parameter");
        ErrorUtil::NapiThrow(env, I18N_NOT_FOUND, throwError);
        return nullptr;
    }
    size_t len = 0;
    napi_get_value_string_utf8(env, argv[0], nullptr, 0, &len);
    std::vector<char> languageBuf(len + 1);
    status = napi_get_value_string_utf8(env, argv[0], languageBuf.data(), len + 1, &len);
    if (status != napi_ok) {
        HiLog::Error(LABEL, "Failed to get string item");
        ErrorUtil::NapiThrow(env, I18N_NOT_VALID, throwError);
        return nullptr;
    }
    bool isSuggested = false;
    if (VariableConvertor::CheckNapiValueType(env, argv[1])) {
        napi_get_value_string_utf8(env, argv[1], nullptr, 0, &len);
        std::vector<char> regionBuf(len + 1);
        status = napi_get_value_string_utf8(env, argv[1], regionBuf.data(), len + 1, &len);
        if (status != napi_ok) {
            HiLog::Error(LABEL, "Failed to get string item");
            ErrorUtil::NapiThrow(env, I18N_NOT_VALID, throwError);
            return nullptr;
        }
        isSuggested = LocaleConfig::IsSuggested(languageBuf.data(), regionBuf.data());
    } else {
        isSuggested = LocaleConfig::IsSuggested(languageBuf.data());
    }
    napi_value result = nullptr;
    status = napi_get_boolean(env, isSuggested, &result);
    if (status != napi_ok) {
        HiLog::Error(LABEL, "Create case first boolean value failed");
        return nullptr;
    }
    return result;
}

napi_value I18nAddon::SetSystemLanguage(napi_env env, napi_callback_info info)
{
    return I18nAddon::SetSystemLanguageImpl(env, info, false);
}

napi_value I18nAddon::SetSystemLanguageWithError(napi_env env, napi_callback_info info)
{
    return I18nAddon::SetSystemLanguageImpl(env, info, true);
}

napi_value I18nAddon::SetSystemLanguageImpl(napi_env env, napi_callback_info info, bool throwError)
{
    size_t argc = 1;
    napi_value argv[1] = { 0 };
    napi_value thisVar = nullptr;
    void *data = nullptr;
    napi_status status = napi_get_cb_info(env, info, &argc, argv, &thisVar, &data);
    if (status != napi_ok) {
        return nullptr;
    }
    if (argv[0] == nullptr) {
        HiLog::Error(LABEL, "Missing parameter");
        ErrorUtil::NapiThrow(env, I18N_NOT_FOUND, throwError);
        return nullptr;
    }
    size_t len = 0;
    napi_get_value_string_utf8(env, argv[0], nullptr, 0, &len);
    std::vector<char> languageBuf(len + 1);
    status = napi_get_value_string_utf8(env, argv[0], languageBuf.data(), len + 1, &len);
    if (status != napi_ok) {
        HiLog::Error(LABEL, "Failed to get string item");
        ErrorUtil::NapiThrow(env, I18N_NOT_VALID, throwError);
        return nullptr;
    }
    bool success = LocaleConfig::SetSystemLanguage(languageBuf.data());
    if (throwError) {
        if (!success) {
            ErrorUtil::NapiThrow(env, I18N_NO_PERMISSION, throwError);
        }
        return nullptr;
    }
    napi_value result = nullptr;
    status = napi_get_boolean(env, success, &result);
    if (status != napi_ok) {
        HiLog::Error(LABEL, "Create set system language boolean value failed");
        return nullptr;
    }
    return result;
}

napi_value I18nAddon::SetSystemRegion(napi_env env, napi_callback_info info)
{
    return I18nAddon::SetSystemRegionImpl(env, info, false);
}

napi_value I18nAddon::SetSystemRegionWithError(napi_env env, napi_callback_info info)
{
    return I18nAddon::SetSystemRegionImpl(env, info, true);
}

napi_value I18nAddon::SetSystemRegionImpl(napi_env env, napi_callback_info info, bool throwError)
{
    size_t argc = 1;
    napi_value argv[1] = { 0 };
    napi_value thisVar = nullptr;
    void *data = nullptr;
    napi_status status = napi_get_cb_info(env, info, &argc, argv, &thisVar, &data);
    if (status != napi_ok) {
        return nullptr;
    }
    if (argv[0] == nullptr) {
        HiLog::Error(LABEL, "Missing parameter");
        ErrorUtil::NapiThrow(env, I18N_NOT_FOUND, throwError);
        return nullptr;
    }
    size_t len = 0;
    napi_get_value_string_utf8(env, argv[0], nullptr, 0, &len);
    std::vector<char> regionBuf(len + 1);
    status = napi_get_value_string_utf8(env, argv[0], regionBuf.data(), len + 1, &len);
    if (status != napi_ok) {
        HiLog::Error(LABEL, "Failed to get string item");
        ErrorUtil::NapiThrow(env, I18N_NOT_VALID, throwError);
        return nullptr;
    }
    bool success = LocaleConfig::SetSystemRegion(regionBuf.data());
    if (throwError) {
        if (!success) {
            ErrorUtil::NapiThrow(env, I18N_NO_PERMISSION, throwError);
        }
        return nullptr;
    }
    napi_value result = nullptr;
    status = napi_get_boolean(env, success, &result);
    if (status != napi_ok) {
        HiLog::Error(LABEL, "Create set system language boolean value failed");
        return nullptr;
    }
    return result;
}

napi_value I18nAddon::SetSystemLocale(napi_env env, napi_callback_info info)
{
    return I18nAddon::SetSystemLocaleImpl(env, info, false);
}

napi_value I18nAddon::SetSystemLocaleWithError(napi_env env, napi_callback_info info)
{
    return I18nAddon::SetSystemLocaleImpl(env, info, true);
}

napi_value I18nAddon::SetSystemLocaleImpl(napi_env env, napi_callback_info info, bool throwError)
{
    size_t argc = 1;
    napi_value argv[1] = { 0 };
    napi_value thisVar = nullptr;
    void *data = nullptr;
    napi_status status = napi_get_cb_info(env, info, &argc, argv, &thisVar, &data);
    if (status != napi_ok) {
        return nullptr;
    }
    if (argv[0] == nullptr) {
        HiLog::Error(LABEL, "Missing parameter");
        ErrorUtil::NapiThrow(env, I18N_NOT_FOUND, throwError);
        return nullptr;
    }
    size_t len = 0;
    napi_get_value_string_utf8(env, argv[0], nullptr, 0, &len);
    std::vector<char> localeBuf(len + 1);
    status = napi_get_value_string_utf8(env, argv[0], localeBuf.data(), len + 1, &len);
    if (status != napi_ok) {
        HiLog::Error(LABEL, "Failed to get string item");
        ErrorUtil::NapiThrow(env, I18N_NOT_VALID, throwError);
        return nullptr;
    }
    bool success = LocaleConfig::SetSystemLocale(localeBuf.data());
    if (throwError) {
        if (!success) {
            ErrorUtil::NapiThrow(env, I18N_NO_PERMISSION, throwError);
        }
        return nullptr;
    }
    napi_value result = nullptr;
    status = napi_get_boolean(env, success, &result);
    if (status != napi_ok) {
        HiLog::Error(LABEL, "Create set system language boolean value failed");
        return nullptr;
    }
    return result;
}

napi_value I18nAddon::IsRTL(napi_env env, napi_callback_info info)
{
    size_t argc = 1;
    napi_value argv[1] = { 0 };
    napi_value thisVar = nullptr;
    void *data = nullptr;
    napi_status status = napi_get_cb_info(env, info, &argc, argv, &thisVar, &data);
    if (status != napi_ok) {
        return nullptr;
    }
    size_t len = 0;
    napi_get_value_string_utf8(env, argv[0], nullptr, 0, &len);
    std::vector<char> localeBuf(len + 1);
    status = napi_get_value_string_utf8(env, argv[0], localeBuf.data(), len + 1, &len);
    if (status != napi_ok) {
        HiLog::Error(LABEL, "Failed to get string item");
        return nullptr;
    }
    bool isRTL = LocaleConfig::IsRTL(localeBuf.data());
    napi_value result = nullptr;
    status = napi_get_boolean(env, isRTL, &result);
    if (status != napi_ok) {
        HiLog::Error(LABEL, "IsRTL failed");
        return nullptr;
    }
    return result;
}

napi_value I18nAddon::InitPhoneNumberFormat(napi_env env, napi_value exports)
{
    napi_status status = napi_ok;
    napi_property_descriptor properties[] = {
        DECLARE_NAPI_FUNCTION("isValidNumber", IsValidPhoneNumber),
        DECLARE_NAPI_FUNCTION("format", FormatPhoneNumber),
        DECLARE_NAPI_FUNCTION("getLocationName", GetLocationName)
    };

    napi_value constructor;
    status = napi_define_class(env, "PhoneNumberFormat", NAPI_AUTO_LENGTH, PhoneNumberFormatConstructor, nullptr,
                               sizeof(properties) / sizeof(napi_property_descriptor), properties, &constructor);
    if (status != napi_ok) {
        HiLog::Error(LABEL, "Define class failed when InitPhoneNumberFormat");
        return nullptr;
    }

    status = napi_set_named_property(env, exports, "PhoneNumberFormat", constructor);
    if (status != napi_ok) {
        HiLog::Error(LABEL, "Set property failed when InitPhoneNumberFormat");
        return nullptr;
    }
    return exports;
}

void GetOptionValue(napi_env env, napi_value options, const std::string &optionName,
                    std::map<std::string, std::string> &map)
{
    napi_value optionValue = nullptr;
    napi_valuetype type = napi_undefined;
    napi_status status = napi_typeof(env, options, &type);
    if (status != napi_ok && type != napi_object) {
        HiLog::Error(LABEL, "Get option failed, option is not an object");
        return;
    }
    bool hasProperty = false;
    napi_status propStatus = napi_has_named_property(env, options, optionName.c_str(), &hasProperty);
    if (propStatus == napi_ok && hasProperty) {
        status = napi_get_named_property(env, options, optionName.c_str(), &optionValue);
        if (status == napi_ok) {
            size_t len = 0;
            napi_get_value_string_utf8(env, optionValue, nullptr, 0, &len);
            std::vector<char> optionBuf(len + 1);
            status = napi_get_value_string_utf8(env, optionValue, optionBuf.data(), len + 1, &len);
            if (status != napi_ok) {
                return;
            }
            map.insert(make_pair(optionName, optionBuf.data()));
        }
    }
}

napi_value I18nAddon::PhoneNumberFormatConstructor(napi_env env, napi_callback_info info)
{
    size_t argc = 2;
    napi_value argv[2] = { 0 };
    napi_value thisVar = nullptr;
    void *data = nullptr;
    napi_status status = napi_get_cb_info(env, info, &argc, argv, &thisVar, &data);
    if (status != napi_ok) {
        return nullptr;
    }
    napi_valuetype valueType = napi_valuetype::napi_undefined;
    napi_typeof(env, argv[0], &valueType);
    if (valueType != napi_valuetype::napi_string) {
        napi_throw_type_error(env, nullptr, "Parameter type does not match");
        return nullptr;
    }
    size_t len = 0;
    status = napi_get_value_string_utf8(env, argv[0], nullptr, 0, &len);
    if (status != napi_ok) {
        HiLog::Error(LABEL, "Get country tag length failed");
        return nullptr;
    }
    std::vector<char> country (len + 1);
    status = napi_get_value_string_utf8(env, argv[0], country.data(), len + 1, &len);
    if (status != napi_ok) {
        HiLog::Error(LABEL, "Get country tag failed");
        return nullptr;
    }
    std::map<std::string, std::string> options;
    GetOptionValue(env, argv[1], "type", options);
    std::unique_ptr<I18nAddon> obj = nullptr;
    obj = std::make_unique<I18nAddon>();
    status = napi_wrap(env, thisVar, reinterpret_cast<void *>(obj.get()),
                       I18nAddon::Destructor, nullptr, nullptr);
    if (status != napi_ok) {
        HiLog::Error(LABEL, "Wrap I18nAddon failed");
        return nullptr;
    }
    if (!obj->InitPhoneNumberFormatContext(env, info, country.data(), options)) {
        return nullptr;
    }
    obj.release();
    return thisVar;
}

bool I18nAddon::InitPhoneNumberFormatContext(napi_env env, napi_callback_info info, const std::string &country,
                                             const std::map<std::string, std::string> &options)
{
    napi_value global = nullptr;
    napi_status status = napi_get_global(env, &global);
    if (status != napi_ok) {
        HiLog::Error(LABEL, "Get global failed");
        return false;
    }
    env_ = env;
    phonenumberfmt_ = PhoneNumberFormat::CreateInstance(country, options);

    return phonenumberfmt_ != nullptr;
}

napi_value I18nAddon::IsValidPhoneNumber(napi_env env, napi_callback_info info)
{
    size_t argc = 1;
    napi_value argv[1] = { 0 };
    napi_value thisVar = nullptr;
    void *data = nullptr;
    napi_get_cb_info(env, info, &argc, argv, &thisVar, &data);
    napi_valuetype valueType = napi_valuetype::napi_undefined;
    napi_typeof(env, argv[0], &valueType);
    if (valueType != napi_valuetype::napi_string) {
        napi_throw_type_error(env, nullptr, "Parameter type does not match");
        return nullptr;
    }

    size_t len = 0;
    napi_status status = napi_get_value_string_utf8(env, argv[0], nullptr, 0, &len);
    if (status != napi_ok) {
        HiLog::Error(LABEL, "Get phone number length failed");
        return nullptr;
    }
    std::vector<char> buf(len + 1);
    status = napi_get_value_string_utf8(env, argv[0], buf.data(), len + 1, &len);
    if (status != napi_ok) {
        HiLog::Error(LABEL, "Get phone number failed");
        return nullptr;
    }

    I18nAddon *obj = nullptr;
    status = napi_unwrap(env, thisVar, reinterpret_cast<void **>(&obj));
    if (status != napi_ok || !obj || !obj->phonenumberfmt_) {
        HiLog::Error(LABEL, "GetPhoneNumberFormat object failed");
        return nullptr;
    }

    bool isValid = obj->phonenumberfmt_->isValidPhoneNumber(buf.data());

    napi_value result = nullptr;
    status = napi_get_boolean(env, isValid, &result);
    if (status != napi_ok) {
        HiLog::Error(LABEL, "Create boolean failed");
        return nullptr;
    }

    return result;
}

napi_value I18nAddon::GetLocationName(napi_env env, napi_callback_info info)
{
    size_t argc = 2;
    napi_value argv[2] = {0, 0};
    napi_value thisVar = nullptr;
    void *data = nullptr;
    napi_get_cb_info(env, info, &argc, argv, &thisVar, &data);

    int32_t code = 0;
    std::string number = VariableConvertor::GetString(env, argv[0], code);
    if (code) {
        return nullptr;
    }
    std::string language = VariableConvertor::GetString(env, argv[1], code);
    if (code) {
        return nullptr;
    }

    I18nAddon *obj = nullptr;
    napi_status status = napi_unwrap(env, thisVar, reinterpret_cast<void **>(&obj));
    if (status != napi_ok || !obj || !obj->phonenumberfmt_) {
        HiLog::Error(LABEL, "GetPhoneNumberFormat object failed");
        return nullptr;
    }

    std::string resStr = obj->phonenumberfmt_->getLocationName(number.data(), language.data());
    napi_value result = nullptr;
    status = napi_create_string_utf8(env, resStr.c_str(), NAPI_AUTO_LENGTH, &result);
    if (status != napi_ok) {
        HiLog::Error(LABEL, "Create result string failed");
        return nullptr;
    }

    return result;
}

napi_value I18nAddon::FormatPhoneNumber(napi_env env, napi_callback_info info)
{
    size_t argc = 1;
    napi_value argv[1] = { 0 };
    napi_value thisVar = nullptr;
    void *data = nullptr;
    napi_get_cb_info(env, info, &argc, argv, &thisVar, &data);
    napi_valuetype valueType = napi_valuetype::napi_undefined;
    napi_typeof(env, argv[0], &valueType);
    if (valueType != napi_valuetype::napi_string) {
        napi_throw_type_error(env, nullptr, "Parameter type does not match");
        return nullptr;
    }

    size_t len = 0;
    napi_status status = napi_get_value_string_utf8(env, argv[0], nullptr, 0, &len);
    if (status != napi_ok) {
        HiLog::Error(LABEL, "Get phone number length failed");
        return nullptr;
    }
    std::vector<char> buf(len + 1);
    status = napi_get_value_string_utf8(env, argv[0], buf.data(), len + 1, &len);
    if (status != napi_ok) {
        HiLog::Error(LABEL, "Get phone number failed");
        return nullptr;
    }

    I18nAddon *obj = nullptr;
    status = napi_unwrap(env, thisVar, reinterpret_cast<void **>(&obj));
    if (status != napi_ok || !obj || !obj->phonenumberfmt_) {
        HiLog::Error(LABEL, "Get PhoneNumberFormat object failed");
        return nullptr;
    }

    std::string formattedPhoneNumber = obj->phonenumberfmt_->format(buf.data());

    napi_value result = nullptr;
    status = napi_create_string_utf8(env, formattedPhoneNumber.c_str(), NAPI_AUTO_LENGTH, &result);
    if (status != napi_ok) {
        HiLog::Error(LABEL, "Create format phone number failed");
        return nullptr;
    }
    return result;
}

napi_value I18nAddon::InitIndexUtil(napi_env env, napi_value exports)
{
    napi_property_descriptor properties[] = {
        DECLARE_NAPI_FUNCTION("getIndexList", GetIndexList),
        DECLARE_NAPI_FUNCTION("addLocale", AddLocale),
        DECLARE_NAPI_FUNCTION("getIndex", GetIndex)
    };

    napi_value constructor = nullptr;
    napi_status status = napi_define_class(env, "IndexUtil", NAPI_AUTO_LENGTH, IndexUtilConstructor, nullptr,
        sizeof(properties) / sizeof(napi_property_descriptor), properties, &constructor);
    if (status != napi_ok) {
        HiLog::Error(LABEL, "Define class failed when InitPhoneNumberFormat");
        return nullptr;
    }

    status = napi_create_reference(env, constructor, 1, &g_indexUtilConstructor);
    if (status != napi_ok) {
        HiLog::Error(LABEL, "Failed to create reference at init");
        return nullptr;
    }
    return exports;
}

napi_value I18nAddon::BreakIteratorConstructor(napi_env env, napi_callback_info info)
{
    size_t argc = 1;
    napi_value argv[1] = { nullptr };
    napi_value thisVar = nullptr;
    void *data = nullptr;
    napi_status status = napi_get_cb_info(env, info, &argc, argv, &thisVar, &data);
    if (status != napi_ok) {
        return nullptr;
    }
    napi_valuetype valueType = napi_valuetype::napi_undefined;
    napi_typeof(env, argv[0], &valueType);
    if (valueType != napi_valuetype::napi_string) {
        napi_throw_type_error(env, nullptr, "Parameter type does not match");
        return nullptr;
    }
    int32_t code = 0;
    std::string localeTag = VariableConvertor::GetString(env, argv[0], code);
    if (code) {
        return nullptr;
    }
    std::unique_ptr<I18nAddon> obj = nullptr;
    obj = std::make_unique<I18nAddon>();
    status =
        napi_wrap(env, thisVar, reinterpret_cast<void *>(obj.get()), I18nAddon::Destructor, nullptr, nullptr);
    if (status != napi_ok) {
        HiLog::Error(LABEL, "BreakIteratorConstructor: Wrap II18nAddon failed");
        return nullptr;
    }
    obj->brkiter_ = std::make_unique<I18nBreakIterator>(localeTag);
    if (!obj->brkiter_) {
        HiLog::Error(LABEL, "Wrap BreakIterator failed");
        return nullptr;
    }
    obj.release();
    return thisVar;
}

napi_value I18nAddon::InitBreakIterator(napi_env env, napi_value exports)
{
    napi_status status = napi_ok;
    napi_property_descriptor properties[] = {
        DECLARE_NAPI_FUNCTION("current", Current),
        DECLARE_NAPI_FUNCTION("first", First),
        DECLARE_NAPI_FUNCTION("last", Last),
        DECLARE_NAPI_FUNCTION("next", Next),
        DECLARE_NAPI_FUNCTION("previous", Previous),
        DECLARE_NAPI_FUNCTION("setLineBreakText", SetText),
        DECLARE_NAPI_FUNCTION("following", Following),
        DECLARE_NAPI_FUNCTION("getLineBreakText", GetText),
        DECLARE_NAPI_FUNCTION("isBoundary", IsBoundary),
    };
    napi_value constructor = nullptr;
    status = napi_define_class(env, "BreakIterator", NAPI_AUTO_LENGTH, BreakIteratorConstructor, nullptr,
        sizeof(properties) / sizeof(napi_property_descriptor), properties, &constructor);
    if (status != napi_ok) {
        HiLog::Error(LABEL, "Failed to define class BreakIterator at Init");
        return nullptr;
    }
    g_brkConstructor = new (std::nothrow) napi_ref;
    if (!g_brkConstructor) {
        HiLog::Error(LABEL, "Failed to create brkiterator ref at init");
        return nullptr;
    }
    status = napi_create_reference(env, constructor, 1, g_brkConstructor);
    if (status != napi_ok) {
        HiLog::Error(LABEL, "Failed to create reference g_brkConstructor at init");
        return nullptr;
    }
    return exports;
}

napi_value I18nAddon::GetLineInstance(napi_env env, napi_callback_info info)
{
    size_t argc = 1;
    napi_value argv[1] = { nullptr };
    napi_value thisVar = nullptr;
    void *data = nullptr;
    napi_get_cb_info(env, info, &argc, argv, &thisVar, &data);
    napi_value constructor = nullptr;
    napi_status status = napi_get_reference_value(env, *g_brkConstructor, &constructor);
    if (status != napi_ok) {
        HiLog::Error(LABEL, "Failed to create reference at GetLineInstance");
        return nullptr;
    }
    if (!argv[0]) {
        return nullptr;
    }
    napi_value result = nullptr;
    status = napi_new_instance(env, constructor, 1, argv, &result); // 1 arguments
    if (status != napi_ok) {
        HiLog::Error(LABEL, "GetLineInstance create instance failed");
        return nullptr;
    }
    return result;
}

napi_value I18nAddon::Current(napi_env env, napi_callback_info info)
{
    size_t argc = 0;
    napi_value *argv = nullptr;
    napi_value thisVar = nullptr;
    void *data = nullptr;
    napi_get_cb_info(env, info, &argc, argv, &thisVar, &data);
    I18nAddon *obj = nullptr;
    napi_status status = napi_unwrap(env, thisVar, reinterpret_cast<void **>(&obj));
    if (status != napi_ok || !obj || !obj->brkiter_) {
        HiLog::Error(LABEL, "Get BreakIterator object failed");
        return nullptr;
    }
    int value = obj->brkiter_->Current();
    napi_value result = nullptr;
    status = napi_create_int32(env, value, &result);
    if (status != napi_ok) {
        HiLog::Error(LABEL, "Create int32_t value failed");
        return nullptr;
    }
    return result;
}

napi_value I18nAddon::First(napi_env env, napi_callback_info info)
{
    size_t argc = 0;
    napi_value *argv = nullptr;
    napi_value thisVar = nullptr;
    void *data = nullptr;
    napi_get_cb_info(env, info, &argc, argv, &thisVar, &data);
    I18nAddon *obj = nullptr;
    napi_status status = napi_unwrap(env, thisVar, reinterpret_cast<void **>(&obj));
    if (status != napi_ok || !obj || !obj->brkiter_) {
        HiLog::Error(LABEL, "Get BreakIterator object failed");
        return nullptr;
    }
    int value = obj->brkiter_->First();
    napi_value result = nullptr;
    status = napi_create_int32(env, value, &result);
    if (status != napi_ok) {
        HiLog::Error(LABEL, "Create int32_t value failed");
        return nullptr;
    }
    return result;
}

napi_value I18nAddon::Last(napi_env env, napi_callback_info info)
{
    size_t argc = 0;
    napi_value *argv = nullptr;
    napi_value thisVar = nullptr;
    void *data = nullptr;
    napi_get_cb_info(env, info, &argc, argv, &thisVar, &data);
    I18nAddon *obj = nullptr;
    napi_status status = napi_unwrap(env, thisVar, reinterpret_cast<void **>(&obj));
    if (status != napi_ok || !obj || !obj->brkiter_) {
        HiLog::Error(LABEL, "Get BreakIterator object failed");
        return nullptr;
    }
    int value = obj->brkiter_->Last();
    napi_value result = nullptr;
    status = napi_create_int32(env, value, &result);
    if (status != napi_ok) {
        HiLog::Error(LABEL, "Create int32_t value failed");
        return nullptr;
    }
    return result;
}

napi_value I18nAddon::Previous(napi_env env, napi_callback_info info)
{
    size_t argc = 0;
    napi_value *argv = nullptr;
    napi_value thisVar = nullptr;
    void *data = nullptr;
    napi_get_cb_info(env, info, &argc, argv, &thisVar, &data);
    I18nAddon *obj = nullptr;
    napi_status status = napi_unwrap(env, thisVar, reinterpret_cast<void **>(&obj));
    if (status != napi_ok || !obj || !obj->brkiter_) {
        HiLog::Error(LABEL, "Get BreakIterator object failed");
        return nullptr;
    }
    int value = obj->brkiter_->Previous();
    napi_value result = nullptr;
    status = napi_create_int32(env, value, &result);
    if (status != napi_ok) {
        HiLog::Error(LABEL, "Create int32_t value failed");
        return nullptr;
    }
    return result;
}

napi_value I18nAddon::Next(napi_env env, napi_callback_info info)
{
    size_t argc = 1;
    napi_value argv[1] = { nullptr };
    napi_value thisVar = nullptr;
    void *data = nullptr;
    napi_get_cb_info(env, info, &argc, argv, &thisVar, &data);
    I18nAddon *obj = nullptr;
    napi_status status = napi_unwrap(env, thisVar, reinterpret_cast<void **>(&obj));
    if (status != napi_ok || !obj || !obj->brkiter_) {
        HiLog::Error(LABEL, "Get BreakIterator object failed");
        return nullptr;
    }
    int value = 1;
    if (VariableConvertor::CheckNapiValueType(env, argv[0])) {
        napi_valuetype valueType = napi_valuetype::napi_undefined;
        napi_typeof(env, argv[0], &valueType);
        if (valueType != napi_valuetype::napi_number) {
            napi_throw_type_error(env, nullptr, "Parameter type does not match");
            return nullptr;
        }
        status = napi_get_value_int32(env, argv[0], &value);
        if (status != napi_ok) {
            HiLog::Error(LABEL, "Retrieve next value failed");
            return nullptr;
        }
    }
    value = obj->brkiter_->Next(value);
    napi_value result = nullptr;
    status = napi_create_int32(env, value, &result);
    if (status != napi_ok) {
        HiLog::Error(LABEL, "Create int32_t value failed");
        return nullptr;
    }
    return result;
}

napi_value I18nAddon::SetText(napi_env env, napi_callback_info info)
{
    size_t argc = 1;
    napi_value argv[1] = { nullptr };
    napi_value thisVar = nullptr;
    void *data = nullptr;
    napi_get_cb_info(env, info, &argc, argv, &thisVar, &data);
    I18nAddon *obj = nullptr;
    napi_status status = napi_unwrap(env, thisVar, reinterpret_cast<void **>(&obj));
    if (status != napi_ok || !obj || !obj->brkiter_) {
        HiLog::Error(LABEL, "Get BreakIterator object failed");
        return nullptr;
    }
    if (!argv[0]) {
        return nullptr;
    }
    napi_valuetype valueType = napi_valuetype::napi_undefined;
    napi_typeof(env, argv[0], &valueType);
    if (valueType != napi_valuetype::napi_string) {
        napi_throw_type_error(env, nullptr, "Parameter type does not match");
        return nullptr;
    }
    size_t len = 0;
    status = napi_get_value_string_utf8(env, argv[0], nullptr, 0, &len);
    if (status != napi_ok) {
        HiLog::Error(LABEL, "Get field length failed");
        return nullptr;
    }
    std::vector<char> buf(len + 1);
    status = napi_get_value_string_utf8(env, argv[0], buf.data(), len + 1, &len);
    if (status != napi_ok) {
        HiLog::Error(LABEL, "Get string value failed");
        return nullptr;
    }
    obj->brkiter_->SetText(buf.data());
    return nullptr;
}

napi_value I18nAddon::GetText(napi_env env, napi_callback_info info)
{
    size_t argc = 0;
    napi_value *argv = nullptr;
    napi_value thisVar = nullptr;
    void *data = nullptr;
    napi_get_cb_info(env, info, &argc, argv, &thisVar, &data);
    I18nAddon *obj = nullptr;
    napi_status status = napi_unwrap(env, thisVar, reinterpret_cast<void **>(&obj));
    if (status != napi_ok || !obj || !obj->brkiter_) {
        HiLog::Error(LABEL, "Get BreakIterator object failed");
        return nullptr;
    }
    napi_value value = nullptr;
    std::string temp;
    obj->brkiter_->GetText(temp);
    status = napi_create_string_utf8(env, temp.c_str(), NAPI_AUTO_LENGTH, &value);
    if (status != napi_ok) {
        HiLog::Error(LABEL, "Get field length failed");
        return nullptr;
    }
    return value;
}

napi_value I18nAddon::Following(napi_env env, napi_callback_info info)
{
    size_t argc = 1;
    napi_value argv[1] = { nullptr };
    napi_value thisVar = nullptr;
    void *data = nullptr;
    napi_get_cb_info(env, info, &argc, argv, &thisVar, &data);
    I18nAddon *obj = nullptr;
    napi_status status = napi_unwrap(env, thisVar, reinterpret_cast<void **>(&obj));
    if (status != napi_ok || !obj || !obj->brkiter_) {
        HiLog::Error(LABEL, "Get BreakIterator object failed");
        return nullptr;
    }
    if (!argv[0]) {
        return nullptr;
    }
    napi_valuetype valueType = napi_valuetype::napi_undefined;
    napi_typeof(env, argv[0], &valueType);
    if (valueType != napi_valuetype::napi_number) {
        napi_throw_type_error(env, nullptr, "Parameter type does not match");
        return nullptr;
    }
    int value;
    status = napi_get_value_int32(env, argv[0], &value);
    if (status != napi_ok) {
        HiLog::Error(LABEL, "Retrieve following value failed");
        return nullptr;
    }
    value = obj->brkiter_->Following(value);
    napi_value result = nullptr;
    status = napi_create_int32(env, value, &result);
    if (status != napi_ok) {
        HiLog::Error(LABEL, "Create int32_t value failed");
        return nullptr;
    }
    return result;
}

napi_value I18nAddon::IsBoundary(napi_env env, napi_callback_info info)
{
    size_t argc = 1;
    napi_value argv[1] = { nullptr };
    napi_value thisVar = nullptr;
    void *data = nullptr;
    napi_get_cb_info(env, info, &argc, argv, &thisVar, &data);
    I18nAddon *obj = nullptr;
    napi_status status = napi_unwrap(env, thisVar, reinterpret_cast<void **>(&obj));
    if (status != napi_ok || !obj || !obj->brkiter_) {
        HiLog::Error(LABEL, "Get BreakIterator object failed");
        return nullptr;
    }
    if (!argv[0]) {
        return nullptr;
    }
    napi_valuetype valueType = napi_valuetype::napi_undefined;
    int value;
    napi_typeof(env, argv[0], &valueType);
    if (valueType != napi_valuetype::napi_number) {
        napi_throw_type_error(env, nullptr, "Parameter type does not match");
        return nullptr;
    }
    status = napi_get_value_int32(env, argv[0], &value);
    if (status != napi_ok) {
        HiLog::Error(LABEL, "Retrieve following value failed");
        return nullptr;
    }
    bool boundary = obj->brkiter_->IsBoundary(value);
    napi_value result = nullptr;
    status = napi_get_boolean(env, boundary, &result);
    if (status != napi_ok) {
        HiLog::Error(LABEL, "Create boolean failed");
        return nullptr;
    }
    return result;
}

napi_value I18nAddon::IndexUtilConstructor(napi_env env, napi_callback_info info)
{
    size_t argc = 1;
    napi_value argv[1] = { 0 };
    napi_value thisVar = nullptr;
    void *data = nullptr;
    napi_status status = napi_get_cb_info(env, info, &argc, argv, &thisVar, &data);
    if (status != napi_ok) {
        return nullptr;
    }
    std::string localeTag = "";
    if (argv[0] != nullptr) {
        napi_valuetype valueType = napi_valuetype::napi_undefined;
        napi_typeof(env, argv[0], &valueType);
        if (valueType != napi_valuetype::napi_string) {
            napi_throw_type_error(env, nullptr, "Parameter type does not match");
            return nullptr;
        }
        size_t len = 0;
        status = napi_get_value_string_utf8(env, argv[0], nullptr, 0, &len);
        if (status != napi_ok) {
            HiLog::Error(LABEL, "Get locale length failed");
            return nullptr;
        }
        std::vector<char> localeBuf(len + 1);
        status = napi_get_value_string_utf8(env, argv[0], localeBuf.data(), len + 1, &len);
        if (status != napi_ok) {
            HiLog::Error(LABEL, "Get locale failed");
            return nullptr;
        }
        localeTag = localeBuf.data();
    }
    std::unique_ptr<I18nAddon> obj = nullptr;
    obj = std::make_unique<I18nAddon>();
    status =
        napi_wrap(env, thisVar, reinterpret_cast<void *>(obj.get()), I18nAddon::Destructor, nullptr, nullptr);
    if (status != napi_ok) {
        HiLog::Error(LABEL, "IndexUtilConstructor: Wrap II18nAddon failed");
        return nullptr;
    }
    if (!obj->InitIndexUtilContext(env, info, localeTag)) {
        return nullptr;
    }
    obj.release();
    return thisVar;
}

bool I18nAddon::InitIndexUtilContext(napi_env env, napi_callback_info info, const std::string &localeTag)
{
    napi_value global = nullptr;
    napi_status status = napi_get_global(env, &global);
    if (status != napi_ok) {
        HiLog::Error(LABEL, "Get global failed");
        return false;
    }
    env_ = env;
    indexUtil_ = std::make_unique<IndexUtil>(localeTag);
    return indexUtil_ != nullptr;
}

napi_value I18nAddon::GetIndexUtil(napi_env env, napi_callback_info info)
{
    size_t argc = 1;
    napi_value argv[1] = { 0 };
    napi_value thisVar = nullptr;
    void *data = nullptr;
    napi_get_cb_info(env, info, &argc, argv, &thisVar, &data);
    napi_value constructor = nullptr;
    napi_status status = napi_get_reference_value(env, g_indexUtilConstructor, &constructor);
    if (status != napi_ok) {
        HiLog::Error(LABEL, "Failed to create reference at GetIndexUtil");
        return nullptr;
    }
    napi_value result = nullptr;
    if (!VariableConvertor::CheckNapiValueType(env, argv[0])) {
        status = napi_new_instance(env, constructor, 0, argv, &result);
    } else {
        status = napi_new_instance(env, constructor, 1, argv, &result);
    }
    if (status != napi_ok) {
        HiLog::Error(LABEL, "Get calendar create instance failed");
        return nullptr;
    }
    return result;
}

napi_value I18nAddon::GetIndexList(napi_env env, napi_callback_info info)
{
    size_t argc = 0;
    napi_value argv[0];
    napi_value thisVar = nullptr;
    void *data = nullptr;
    napi_get_cb_info(env, info, &argc, argv, &thisVar, &data);

    I18nAddon *obj = nullptr;
    napi_status status = napi_unwrap(env, thisVar, reinterpret_cast<void **>(&obj));
    if (status != napi_ok || !obj || !obj->indexUtil_) {
        HiLog::Error(LABEL, "GetPhoneNumberFormat object failed");
        return nullptr;
    }

    std::vector<std::string> indexList = obj->indexUtil_->GetIndexList();
    napi_value result = nullptr;
    status = napi_create_array_with_length(env, indexList.size(), &result);
    if (status != napi_ok) {
        HiLog::Error(LABEL, "Failed to create array");
        return nullptr;
    }
    for (size_t i = 0; i < indexList.size(); i++) {
        napi_value element = nullptr;
        status = napi_create_string_utf8(env, indexList[i].c_str(), NAPI_AUTO_LENGTH, &element);
        if (status != napi_ok) {
            HiLog::Error(LABEL, "Failed to create string item");
            return nullptr;
        }
        status = napi_set_element(env, result, i, element);
        if (status != napi_ok) {
            HiLog::Error(LABEL, "Failed to set array item");
            return nullptr;
        }
    }
    return result;
}

napi_value I18nAddon::AddLocale(napi_env env, napi_callback_info info)
{
    size_t argc = 1;
    napi_value argv[1] = { 0 };
    napi_value thisVar = nullptr;
    void *data = nullptr;
    napi_get_cb_info(env, info, &argc, argv, &thisVar, &data);
    napi_valuetype valueType = napi_valuetype::napi_undefined;
    napi_typeof(env, argv[0], &valueType);
    if (valueType != napi_valuetype::napi_string) {
        napi_throw_type_error(env, nullptr, "Parameter type does not match");
        return nullptr;
    }
    size_t len = 0;
    napi_status status = napi_get_value_string_utf8(env, argv[0], nullptr, 0, &len);
    if (status != napi_ok) {
        HiLog::Error(LABEL, "Get locale length failed");
        return nullptr;
    }
    std::vector<char> buf(len + 1);
    status = napi_get_value_string_utf8(env, argv[0], buf.data(), len + 1, &len);
    if (status != napi_ok) {
        HiLog::Error(LABEL, "Get locale failed");
        return nullptr;
    }
    I18nAddon *obj = nullptr;
    status = napi_unwrap(env, thisVar, reinterpret_cast<void **>(&obj));
    if (status != napi_ok || !obj || !obj->indexUtil_) {
        HiLog::Error(LABEL, "Get IndexUtil object failed");
        return nullptr;
    }
    obj->indexUtil_->AddLocale(buf.data());
    return nullptr;
}

napi_value I18nAddon::GetIndex(napi_env env, napi_callback_info info)
{
    size_t argc = 1;
    napi_value argv[1] = { 0 };
    napi_value thisVar = nullptr;
    void *data = nullptr;
    napi_get_cb_info(env, info, &argc, argv, &thisVar, &data);
    napi_valuetype valueType = napi_valuetype::napi_undefined;
    napi_typeof(env, argv[0], &valueType);
    if (valueType != napi_valuetype::napi_string) {
        napi_throw_type_error(env, nullptr, "Parameter type does not match");
        return nullptr;
    }
    size_t len = 0;
    napi_status status = napi_get_value_string_utf8(env, argv[0], nullptr, 0, &len);
    if (status != napi_ok) {
        HiLog::Error(LABEL, "Get String length failed");
        return nullptr;
    }
    std::vector<char> buf(len + 1);
    status = napi_get_value_string_utf8(env, argv[0], buf.data(), len + 1, &len);
    if (status != napi_ok) {
        HiLog::Error(LABEL, "Get String failed");
        return nullptr;
    }
    I18nAddon *obj = nullptr;
    status = napi_unwrap(env, thisVar, reinterpret_cast<void **>(&obj));
    if (status != napi_ok || !obj || !obj->indexUtil_) {
        HiLog::Error(LABEL, "Get IndexUtil object failed");
        return nullptr;
    }
    std::string index = obj->indexUtil_->GetIndex(buf.data());
    napi_value result = nullptr;
    status = napi_create_string_utf8(env, index.c_str(), NAPI_AUTO_LENGTH, &result);
    if (status != napi_ok) {
        HiLog::Error(LABEL, "GetIndex Failed");
        return nullptr;
    }
    return result;
}

napi_value I18nAddon::Is24HourClock(napi_env env, napi_callback_info info)
{
    bool is24HourClock = LocaleConfig::Is24HourClock();
    napi_value result = nullptr;
    napi_status status = napi_get_boolean(env, is24HourClock, &result);
    if (status != napi_ok) {
        HiLog::Error(LABEL, "Failed to create boolean item");
        return nullptr;
    }
    return result;
}

napi_value I18nAddon::Set24HourClock(napi_env env, napi_callback_info info)
{
    return I18nAddon::Set24HourClockImpl(env, info, false);
}

napi_value I18nAddon::Set24HourClockWithError(napi_env env, napi_callback_info info)
{
    return I18nAddon::Set24HourClockImpl(env, info, true);
}

napi_value I18nAddon::Set24HourClockImpl(napi_env env, napi_callback_info info, bool throwError)
{
    size_t argc = 1;
    napi_value argv[1] = { 0 };
    napi_value thisVar = nullptr;
    void *data = nullptr;
    napi_status status = napi_get_cb_info(env, info, &argc, argv, &thisVar, &data);
    if (status != napi_ok) {
        return nullptr;
    }
    if (argv[0] == nullptr) {
        HiLog::Error(LABEL, "Missing parameter");
        ErrorUtil::NapiThrow(env, I18N_NOT_FOUND, throwError);
        return nullptr;
    }

    bool option = false;
    status = napi_get_value_bool(env, argv[0], &option);
    if (status != napi_ok) {
        HiLog::Error(LABEL, "Failed to get boolean item");
        ErrorUtil::NapiThrow(env, I18N_NOT_VALID, throwError);
        return nullptr;
    }
    bool success = LocaleConfig::Set24HourClock(option);
    if (throwError) {
        if (!success) {
            ErrorUtil::NapiThrow(env, I18N_NO_PERMISSION, throwError);
        }
        return nullptr;
    }
    napi_value result = nullptr;
    status = napi_get_boolean(env, success, &result);
    if (status != napi_ok) {
        HiLog::Error(LABEL, "Create set 24HourClock boolean value failed");
        return nullptr;
    }
    return result;
}

napi_value I18nAddon::AddPreferredLanguage(napi_env env, napi_callback_info info)
{
    return I18nAddon::AddPreferredLanguageImpl(env, info, false);
}

napi_value I18nAddon::AddPreferredLanguageWithError(napi_env env, napi_callback_info info)
{
    return I18nAddon::AddPreferredLanguageImpl(env, info, true);
}

bool I18nAddon::ParseStringParam(napi_env env, napi_value argv, bool throwError, std::string &strParam)
{
    if (argv == nullptr) {
        HiLog::Error(LABEL, "Missing parameter");
        ErrorUtil::NapiThrow(env, I18N_NOT_FOUND, throwError);
        return false;
    }
    napi_valuetype valueType = napi_valuetype::napi_undefined;
    napi_typeof(env, argv, &valueType);
    if (valueType != napi_valuetype::napi_string) {
        ErrorUtil::NapiThrow(env, I18N_NOT_VALID, throwError);
        return false;
    }
    size_t len = 0;
    napi_status status = napi_get_value_string_utf8(env, argv, nullptr, 0, &len);
    if (status != napi_ok) {
        HiLog::Error(LABEL, "get string parameter length failed");
        ErrorUtil::NapiThrow(env, I18N_NOT_VALID, throwError);
        return false;
    }
    std::vector<char> res(len + 1);
    status = napi_get_value_string_utf8(env, argv, res.data(), len + 1, &len);
    if (status != napi_ok) {
        HiLog::Error(LABEL, "get string parameter failed");
        return false;
    }
    strParam = res.data();
    return true;
}

napi_value I18nAddon::AddPreferredLanguageImpl(napi_env env, napi_callback_info info, bool throwError)
{
    size_t argc = 2;
    napi_value argv[2] = { 0 };
    napi_value thisVar = nullptr;
    void *data = nullptr;
    napi_status status = napi_get_cb_info(env, info, &argc, argv, &thisVar, &data);
    if (status != napi_ok) {
        return nullptr;
    }

    std::string language;
    if (!ParseStringParam(env, argv[0], throwError, language)) {
        return nullptr;
    }

    int index = 1000000;
    if (VariableConvertor::CheckNapiValueType(env, argv[1])) {
        status = napi_get_value_int32(env, argv[1], &index);
    }
    if (status != napi_ok) {
        HiLog::Error(LABEL, "addPreferrdLanguage: get index failed");
        ErrorUtil::NapiThrow(env, I18N_NOT_VALID, throwError);
        return nullptr;
    }
    I18nErrorCode errorCode = I18nErrorCode::SUCCESS;
    PreferredLanguage::AddPreferredLanguage(language.data(), index, errorCode);
    if (throwError) {
        if (errorCode == I18nErrorCode::NO_PERMISSION) {
            ErrorUtil::NapiThrow(env, I18N_NO_PERMISSION, throwError);
        }
        if (errorCode == I18nErrorCode::INVALID_PARAMETER || errorCode == I18nErrorCode::FAILED) {
            ErrorUtil::NapiThrow(env, I18N_NOT_VALID, throwError);
        }
        return nullptr;
    }
    bool addResult = true;
    if (errorCode != I18nErrorCode::SUCCESS) {
        addResult = false;
    }
    napi_value result = nullptr;
    status = napi_get_boolean(env, addResult, &result);
    if (status != napi_ok) {
        HiLog::Error(LABEL, "addPreferrdLanguage: create boolean result failed");
        return nullptr;
    }
    return result;
}

napi_value I18nAddon::RemovePreferredLanguage(napi_env env, napi_callback_info info)
{
    return I18nAddon::RemovePreferredLanguageImpl(env, info, false);
}

napi_value I18nAddon::RemovePreferredLanguageWithError(napi_env env, napi_callback_info info)
{
    return I18nAddon::RemovePreferredLanguageImpl(env, info, true);
}

napi_value I18nAddon::RemovePreferredLanguageImpl(napi_env env, napi_callback_info info, bool throwError)
{
    size_t argc = 1;
    napi_value argv[1] = { 0 };
    napi_value thisVar = nullptr;
    void *data = nullptr;
    int len = 0;
    napi_status status = napi_get_cb_info(env, info, &argc, argv, &thisVar, &data);
    if (status != napi_ok) {
        return nullptr;
    }
    if (argv[0] == nullptr) {
        HiLog::Error(LABEL, "Missing parameter");
        ErrorUtil::NapiThrow(env, I18N_NOT_FOUND, throwError);
        return nullptr;
    }

    napi_valuetype valueType = napi_valuetype::napi_undefined;
    napi_typeof(env, argv[0], &valueType);
    if (valueType != napi_valuetype::napi_number) {
        ErrorUtil::NapiThrow(env, I18N_NOT_VALID, throwError);
        return nullptr;
    }
    int index = 1000000;
    status = napi_get_value_int32(env, argv[0], &index);
    if (status != napi_ok) {
        HiLog::Error(LABEL, "removePreferrdLanguage: get index failed");
        ErrorUtil::NapiThrow(env, I18N_NOT_VALID, throwError);
        return nullptr;
    }
    len = static_cast<int>(PreferredLanguage::GetPreferredLanguageList().size());
    if ((index < 0 || index > len - 1) && throwError) {
        ErrorUtil::NapiThrow(env, I18N_NOT_VALID, throwError);
        return nullptr;
    }
    bool success = PreferredLanguage::RemovePreferredLanguage(index);

    if (throwError) {
        if (!success) {
            ErrorUtil::NapiThrow(env, I18N_NO_PERMISSION, throwError);
        }
        return nullptr;
    }
    napi_value result = nullptr;
    status = napi_get_boolean(env, success, &result);
    if (status != napi_ok) {
        HiLog::Error(LABEL, "removePreferrdLanguage: create boolean result failed");
        return nullptr;
    }
    return result;
}

napi_value I18nAddon::GetPreferredLanguageList(napi_env env, napi_callback_info info)
{
    std::vector<std::string> languageList = PreferredLanguage::GetPreferredLanguageList();
    napi_value result = nullptr;
    napi_status status = napi_ok;
    status = napi_create_array_with_length(env, languageList.size(), &result);
    if (status != napi_ok) {
        HiLog::Error(LABEL, "getPreferrdLanguageList: create array failed");
        return nullptr;
    }
    for (size_t i = 0; i < languageList.size(); i++) {
        napi_value value = nullptr;
        status = napi_create_string_utf8(env, languageList[i].c_str(), NAPI_AUTO_LENGTH, &value);
        if (status != napi_ok) {
            HiLog::Error(LABEL, "getPreferrdLanguageList: create string failed");
            return nullptr;
        }
        status = napi_set_element(env, result, i, value);
        if (status != napi_ok) {
            HiLog::Error(LABEL, "GetPreferredLanguageList: set array item failed");
            return nullptr;
        }
    }
    return result;
}

napi_value I18nAddon::GetFirstPreferredLanguage(napi_env env, napi_callback_info info)
{
    std::string language = PreferredLanguage::GetFirstPreferredLanguage();
    napi_value result = nullptr;
    napi_status status = napi_ok;
    status = napi_create_string_utf8(env, language.c_str(), NAPI_AUTO_LENGTH, &result);
    if (status != napi_ok) {
        HiLog::Error(LABEL, "getFirstPreferrdLanguage: create string result failed");
        return nullptr;
    }
    return result;
}

napi_value I18nAddon::GetAppPreferredLanguage(napi_env env, napi_callback_info info)
{
#ifdef SUPPORT_APP_PREFERRED_LANGUAGE
    std::string language = PreferredLanguage::GetAppPreferredLanguage();
#else
    std::string language = PreferredLanguage::GetFirstPreferredLanguage();
#endif
    napi_value result = nullptr;
    napi_status status = napi_ok;
    status = napi_create_string_utf8(env, language.c_str(), NAPI_AUTO_LENGTH, &result);
    if (status != napi_ok) {
        HiLog::Error(LABEL, "getAppPreferrdLanguage: create string result failed");
        return nullptr;
    }
    return result;
}

napi_value I18nAddon::InitI18nTimeZone(napi_env env, napi_value exports)
{
    napi_status status = napi_ok;
    napi_property_descriptor properties[] = {
        DECLARE_NAPI_FUNCTION("getID", GetID),
        DECLARE_NAPI_FUNCTION("getDisplayName", GetTimeZoneDisplayName),
        DECLARE_NAPI_FUNCTION("getRawOffset", GetRawOffset),
        DECLARE_NAPI_FUNCTION("getOffset", GetOffset),
    };
    napi_value constructor = nullptr;
    status = napi_define_class(env, "TimeZone", NAPI_AUTO_LENGTH, I18nTimeZoneConstructor, nullptr,
        sizeof(properties) / sizeof(napi_property_descriptor), properties, &constructor);
    if (status != napi_ok) {
        HiLog::Error(LABEL, "Failed to define class TimeZone at Init");
        return nullptr;
    }
    g_timezoneConstructor = new (std::nothrow) napi_ref;
    if (!g_timezoneConstructor) {
        HiLog::Error(LABEL, "Failed to create TimeZone ref at init");
        return nullptr;
    }
    status = napi_create_reference(env, constructor, 1, g_timezoneConstructor);
    if (status != napi_ok) {
        HiLog::Error(LABEL, "Failed to create reference g_timezoneConstructor at init");
        return nullptr;
    }
    return exports;
}

napi_value I18nAddon::I18nTimeZoneConstructor(napi_env env, napi_callback_info info)
{
    size_t argc = 2;
    napi_value argv[2] = { nullptr };
    napi_value thisVar = nullptr;
    void *data = nullptr;
    napi_status status = napi_get_cb_info(env, info, &argc, argv, &thisVar, &data);
    if (status != napi_ok) {
        return nullptr;
    }
    std::string zoneID = "";
    napi_valuetype valueType = napi_valuetype::napi_undefined;
    if (argv[0] != nullptr) {
        napi_typeof(env, argv[0], &valueType);
        if (valueType != napi_valuetype::napi_string) {
            return nullptr;
        }
        int32_t code = 0;
        zoneID = VariableConvertor::GetString(env, argv[0], code);
        if (code != 0) {
            return nullptr;
        }
    }
    if (argv[1] == nullptr) {
        return nullptr;
    }
    napi_typeof(env, argv[1], &valueType);
    if (valueType != napi_valuetype::napi_boolean) {
        return nullptr;
    }
    bool isZoneID = false;
    status = napi_get_value_bool(env, argv[1], &isZoneID);
    if (status != napi_ok) {
        return nullptr;
    }
    std::unique_ptr<I18nAddon> obj = std::make_unique<I18nAddon>();
    status =
        napi_wrap(env, thisVar, reinterpret_cast<void *>(obj.get()), I18nAddon::Destructor, nullptr, nullptr);
    if (status != napi_ok) {
        return nullptr;
    }
    obj->timezone_ = I18nTimeZone::CreateInstance(zoneID, isZoneID);
    if (!obj->timezone_) {
        return nullptr;
    }
    obj.release();
    return thisVar;
}

napi_value I18nAddon::GetI18nTimeZone(napi_env env, napi_callback_info info)
{
    size_t argc = 1;
    napi_value argv[1] = { nullptr };
    napi_value thisVar = nullptr;
    void *data = nullptr;
    napi_get_cb_info(env, info, &argc, argv, &thisVar, &data);
    if (!VariableConvertor::CheckNapiValueType(env, argv[0])) {
        napi_create_string_utf8(env, "", NAPI_AUTO_LENGTH, &argv[0]);
    }
    return StaticGetTimeZone(env, argv, true);
}

napi_value I18nAddon::GetID(napi_env env, napi_callback_info info)
{
    size_t argc = 0;
    napi_value *argv = nullptr;
    napi_value thisVar = nullptr;
    void *data = nullptr;
    napi_get_cb_info(env, info, &argc, argv, &thisVar, &data);
    I18nAddon *obj = nullptr;
    napi_status status = napi_unwrap(env, thisVar, reinterpret_cast<void **>(&obj));
    if (status != napi_ok || !obj || !obj->timezone_) {
        HiLog::Error(LABEL, "Get TimeZone object failed");
        return nullptr;
    }
    std::string result = obj->timezone_->GetID();
    napi_value value = nullptr;
    status = napi_create_string_utf8(env, result.c_str(), NAPI_AUTO_LENGTH, &value);
    if (status != napi_ok) {
        HiLog::Error(LABEL, "Create result failed");
        return nullptr;
    }
    return value;
}

bool I18nAddon::GetStringFromJS(napi_env env, napi_value argv, std::string &jsString)
{
    size_t len = 0;
    napi_status status = napi_get_value_string_utf8(env, argv, nullptr, 0, &len);
    if (status != napi_ok) {
        HiLog::Error(LABEL, "Failed to get string length");
        return false;
    }
    std::vector<char> argvBuf(len + 1);
    status = napi_get_value_string_utf8(env, argv, argvBuf.data(), len + 1, &len);
    if (status != napi_ok) {
        HiLog::Error(LABEL, "Failed to get string item");
        return false;
    }
    jsString = argvBuf.data();
    return true;
}

int32_t I18nAddon::GetParameter(napi_env env, napi_value *argv, std::string &localeStr, bool &isDST)
{
    napi_status status = napi_ok;
    if (VariableConvertor::CheckNapiValueType(env, argv[1])) {
        napi_valuetype valueType0 = napi_valuetype::napi_undefined;
        napi_valuetype valueType1 = napi_valuetype::napi_undefined;
        napi_typeof(env, argv[0], &valueType0);  // 0 represents first parameter
        napi_typeof(env, argv[1], &valueType1);  // 1 represents second parameter
        bool firstParamFlag = VariableConvertor::CheckNapiValueType(env, argv[0]);
        if (valueType1 == napi_valuetype::napi_boolean) {
            status = napi_get_value_bool(env, argv[1], &isDST);
            if (status != napi_ok) {
                return -1;  // -1 represents Invalid parameter.
            } else if (!firstParamFlag) {
                return 2;  // 2 represents one boolean parameter.
            }
            if (valueType0 == napi_valuetype::napi_string &&
                 GetStringFromJS(env, argv[0], localeStr)) {
                return 3;  // 3 represents one string parameter and one bool parameter.
            }
        }
        return -1;  // -1 represents Invalid parameter.
    }
    return GetFirstParameter(env, argv[0], localeStr, isDST);
}

int32_t I18nAddon::GetFirstParameter(napi_env env, napi_value value, std::string &localeStr, bool &isDST)
{
    if (!VariableConvertor::CheckNapiValueType(env, value)) {
        return 0;  // 0 represents no parameter.
    } else {
        napi_status status = napi_ok;
        napi_valuetype valueType = napi_valuetype::napi_undefined;
        napi_typeof(env, value, &valueType);
        if (valueType == napi_valuetype::napi_string) {
            bool valid = GetStringFromJS(env, value, localeStr);
            // -1 represents Invalid parameter.
            // 1 represents one string parameter.
            return !valid ? -1 : 1;
        } else if (valueType == napi_valuetype::napi_boolean) {
            status = napi_get_value_bool(env, value, &isDST);
            // -1 represents Invalid parameter.
            // 2 represents one boolean parameter.
            return (status != napi_ok) ? -1 : 2;
        }
        return -1;  // -1 represents Invalid parameter.
    }
}

napi_value I18nAddon::GetTimeZoneDisplayName(napi_env env, napi_callback_info info)
{
    size_t argc = 2;
    napi_value argv[2] = { 0 };
    napi_value thisVar = nullptr;
    void *data = nullptr;
    napi_status status = napi_get_cb_info(env, info, &argc, argv, &thisVar, &data);
    if (status != napi_ok) {
        return nullptr;
    }

    I18nAddon *obj = nullptr;
    status = napi_unwrap(env, thisVar, reinterpret_cast<void **>(&obj));
    if (status != napi_ok || !obj || !obj->timezone_) {
        HiLog::Error(LABEL, "Get TimeZone object failed");
        return nullptr;
    }

    std::string locale;
    bool isDST = false;
    int32_t parameterStatus = GetParameter(env, argv, locale, isDST);

    std::string result;
    if (parameterStatus == -1) {  // -1 represents Invalid parameter.
        napi_throw_type_error(env, nullptr, "Parameter type does not match");
        return nullptr;
    } else if (parameterStatus == 0) {
        result = obj->timezone_->GetDisplayName();
    } else if (parameterStatus == 1) {  // 1 represents one string parameter.
        result = obj->timezone_->GetDisplayName(locale);
    } else if (parameterStatus == 2) {  // 2 represents one boolean parameter.
        result = obj->timezone_->GetDisplayName(isDST);
    } else {
        result = obj->timezone_->GetDisplayName(locale, isDST);
    }

    napi_value value = nullptr;
    status = napi_create_string_utf8(env, result.c_str(), NAPI_AUTO_LENGTH, &value);
    if (status != napi_ok) {
        HiLog::Error(LABEL, "Create result failed");
        return nullptr;
    }
    return value;
}

napi_value I18nAddon::GetOffset(napi_env env, napi_callback_info info)
{
    size_t argc = 1;
    napi_value argv[1] = { 0 };
    napi_value thisVar = nullptr;
    void *data = nullptr;
    napi_status status = napi_get_cb_info(env, info, &argc, argv, &thisVar, &data);
    if (status != napi_ok) {
        return nullptr;
    }

    double date = 0;
    if (VariableConvertor::CheckNapiValueType(env, argv[0])) {
        napi_valuetype valueType = napi_valuetype::napi_undefined;
        napi_typeof(env, argv[0], &valueType);
        if (valueType != napi_valuetype::napi_number) {
            HiLog::Error(LABEL, "Invalid parameter type");
            return nullptr;
        }
        status = napi_get_value_double(env, argv[0], &date);
        if (status != napi_ok) {
            HiLog::Error(LABEL, "Get parameter date failed");
            return nullptr;
        }
    } else {
        auto time = std::chrono::system_clock::now();
        auto since_epoch = time.time_since_epoch();
        auto millis = std::chrono::duration_cast<std::chrono::milliseconds>(since_epoch);
        date = (double)millis.count();
    }

    I18nAddon *obj = nullptr;
    status = napi_unwrap(env, thisVar, reinterpret_cast<void **>(&obj));
    if (status != napi_ok || !obj || !obj->timezone_) {
        HiLog::Error(LABEL, "Get TimeZone object failed");
        return nullptr;
    }
    int32_t result = obj->timezone_->GetOffset(date);
    napi_value value = nullptr;
    status = napi_create_int32(env, result, &value);
    if (status != napi_ok) {
        HiLog::Error(LABEL, "Create result failed");
        return nullptr;
    }
    return value;
}

napi_value I18nAddon::GetRawOffset(napi_env env, napi_callback_info info)
{
    size_t argc = 0;
    napi_value *argv = nullptr;
    napi_value thisVar = nullptr;
    void *data = nullptr;
    napi_get_cb_info(env, info, &argc, argv, &thisVar, &data);
    I18nAddon *obj = nullptr;
    napi_status status = napi_unwrap(env, thisVar, reinterpret_cast<void **>(&obj));
    if (status != napi_ok || !obj || !obj->timezone_) {
        HiLog::Error(LABEL, "Get TimeZone object failed");
        return nullptr;
    }
    int32_t result = obj->timezone_->GetRawOffset();
    napi_value value = nullptr;
    status = napi_create_int32(env, result, &value);
    if (status != napi_ok) {
        HiLog::Error(LABEL, "Create result failed");
        return nullptr;
    }
    return value;
}

napi_value I18nAddon::SetUsingLocalDigitAddon(napi_env env, napi_callback_info info)
{
    return I18nAddon::SetUsingLocalDigitAddonImpl(env, info, false);
}

napi_value I18nAddon::SetUsingLocalDigitAddonWithError(napi_env env, napi_callback_info info)
{
    return I18nAddon::SetUsingLocalDigitAddonImpl(env, info, true);
}

napi_value I18nAddon::SetUsingLocalDigitAddonImpl(napi_env env, napi_callback_info info, bool throwError)
{
    size_t argc = 1;
    napi_value argv[1] = { 0 };
    napi_value thisVar = nullptr;
    void *data = nullptr;
    napi_get_cb_info(env, info, &argc, argv, &thisVar, &data);

    if (argv[0] == nullptr) {
        HiLog::Error(LABEL, "Invalid parameter nullptr");
        ErrorUtil::NapiThrow(env, I18N_NOT_FOUND, throwError);
        return nullptr;
    }
    napi_valuetype valueType = napi_valuetype::napi_undefined;
    napi_typeof(env, argv[0], &valueType);
    if (valueType != napi_valuetype::napi_boolean) {
        HiLog::Error(LABEL, "Invalid parameter type");
        ErrorUtil::NapiThrow(env, I18N_NOT_VALID, throwError);
        return nullptr;
    }
    bool flag = false;
    napi_status status = napi_get_value_bool(env, argv[0], &flag);
    if (status != napi_ok) {
        HiLog::Error(LABEL, "Get parameter flag failed");
        return nullptr;
    }

    bool res = LocaleConfig::SetUsingLocalDigit(flag);
    if (throwError) {
        if (!res) {
            ErrorUtil::NapiThrow(env, I18N_NO_PERMISSION, throwError);
        }
        return nullptr;
    }
    napi_value value = nullptr;
    status = napi_get_boolean(env, res, &value);
    if (status != napi_ok) {
        HiLog::Error(LABEL, "Invalid result");
        return nullptr;
    }
    return value;
}

napi_value I18nAddon::GetUsingLocalDigitAddon(napi_env env, napi_callback_info info)
{
    bool res = LocaleConfig::GetUsingLocalDigit();
    napi_value value = nullptr;
    napi_status status = napi_get_boolean(env, res, &value);
    if (status != napi_ok) {
        return nullptr;
    }
    return value;
}

napi_value I18nAddon::CreateTimeZoneObject(napi_env env, napi_status &initStatus)
{
    napi_status status = napi_ok;
    napi_value timezone = nullptr;
    status = napi_create_object(env, &timezone);
    if (status != napi_ok) {
        HiLog::Error(LABEL, "Failed to create timezone object at init");
        initStatus = napi_generic_failure;
        return nullptr;
    }
    napi_property_descriptor timezoneProperties[] = {
        DECLARE_NAPI_FUNCTION("getAvailableIDs", GetAvailableTimezoneIDs),
        DECLARE_NAPI_FUNCTION("getAvailableZoneCityIDs", GetAvailableZoneCityIDs),
        DECLARE_NAPI_FUNCTION("getCityDisplayName", GetCityDisplayName),
        DECLARE_NAPI_FUNCTION("getTimezoneFromCity", GetTimezoneFromCity),
        DECLARE_NAPI_FUNCTION("getTimezonesByLocation", GetTimezonesByLocation)
    };
    status = napi_define_properties(env, timezone,
                                    sizeof(timezoneProperties) / sizeof(napi_property_descriptor),
                                    timezoneProperties);
    if (status != napi_ok) {
        HiLog::Error(LABEL, "Failed to set properties of timezone at init");
        initStatus = napi_generic_failure;
        return nullptr;
    }
    return timezone;
}

napi_value I18nAddon::CreateSystemObject(napi_env env, napi_status &initStatus)
{
    napi_status status = napi_ok;
    napi_value system = nullptr;
    status = napi_create_object(env, &system);
    if (status != napi_ok) {
        HiLog::Error(LABEL, "Failed to create system object at init");
        initStatus = napi_generic_failure;
        return nullptr;
    }
    napi_property_descriptor systemProperties[] = {
        DECLARE_NAPI_FUNCTION("getDisplayCountry", GetDisplayCountryWithError),
        DECLARE_NAPI_FUNCTION("getDisplayLanguage", GetDisplayLanguageWithError),
        DECLARE_NAPI_FUNCTION("getSystemLanguages", GetSystemLanguages),
        DECLARE_NAPI_FUNCTION("getSystemCountries", GetSystemCountriesWithError),
        DECLARE_NAPI_FUNCTION("isSuggested", IsSuggestedWithError),
        DECLARE_NAPI_FUNCTION("getSystemLanguage", GetSystemLanguage),
        DECLARE_NAPI_FUNCTION("setSystemLanguage", SetSystemLanguageWithError),
        DECLARE_NAPI_FUNCTION("getSystemRegion", GetSystemRegion),
        DECLARE_NAPI_FUNCTION("setSystemRegion", SetSystemRegionWithError),
        DECLARE_NAPI_FUNCTION("getSystemLocale", GetSystemLocale),
        DECLARE_NAPI_FUNCTION("setSystemLocale", SetSystemLocaleWithError),
        DECLARE_NAPI_FUNCTION("is24HourClock", Is24HourClock),
        DECLARE_NAPI_FUNCTION("set24HourClock", Set24HourClockWithError),
        DECLARE_NAPI_FUNCTION("addPreferredLanguage", AddPreferredLanguageWithError),
        DECLARE_NAPI_FUNCTION("removePreferredLanguage", RemovePreferredLanguageWithError),
        DECLARE_NAPI_FUNCTION("getPreferredLanguageList", GetPreferredLanguageList),
        DECLARE_NAPI_FUNCTION("getFirstPreferredLanguage", GetFirstPreferredLanguage),
        DECLARE_NAPI_FUNCTION("getAppPreferredLanguage", GetAppPreferredLanguage),
        DECLARE_NAPI_FUNCTION("setUsingLocalDigit", SetUsingLocalDigitAddonWithError),
        DECLARE_NAPI_FUNCTION("getUsingLocalDigit", GetUsingLocalDigitAddon),
    };
    status = napi_define_properties(env, system,
                                    sizeof(systemProperties) / sizeof(napi_property_descriptor),
                                    systemProperties);
    if (status != napi_ok) {
        HiLog::Error(LABEL, "Failed to set properties of system at init");
        initStatus = napi_generic_failure;
        return nullptr;
    }
    return system;
}

napi_value I18nAddon::GetAvailableTimezoneIDs(napi_env env, napi_callback_info info)
{
    I18nErrorCode errorCode = I18nErrorCode::SUCCESS;
    std::set<std::string> timezoneIDs = I18nTimeZone::GetAvailableIDs(errorCode);
    if (errorCode != I18nErrorCode::SUCCESS) {
        return nullptr;
    }
    napi_value result = nullptr;
    napi_status status = napi_create_array_with_length(env, timezoneIDs.size(), &result);
    if (status != napi_ok) {
        HiLog::Error(LABEL, "Failed to create array");
        return nullptr;
    }
    size_t index = 0;
    for (std::set<std::string>::iterator it = timezoneIDs.begin(); it != timezoneIDs.end(); ++it) {
        napi_value value = nullptr;
        status = napi_create_string_utf8(env, (*it).c_str(), NAPI_AUTO_LENGTH, &value);
        if (status != napi_ok) {
            HiLog::Error(LABEL, "Failed to create string item");
            return nullptr;
        }
        status = napi_set_element(env, result, index, value);
        if (status != napi_ok) {
            HiLog::Error(LABEL, "Failed to set array item");
            return nullptr;
        }
        ++index;
    }
    return result;
}

napi_value I18nAddon::GetAvailableZoneCityIDs(napi_env env, napi_callback_info info)
{
    std::set<std::string> cityIDs = I18nTimeZone::GetAvailableZoneCityIDs();
    napi_value result = nullptr;
    napi_status status = napi_create_array_with_length(env, cityIDs.size(), &result);
    if (status != napi_ok) {
        HiLog::Error(LABEL, "Failed to create array");
        return nullptr;
    }
    size_t index = 0;
    for (auto it = cityIDs.begin(); it != cityIDs.end(); ++it) {
        napi_value value = nullptr;
        status = napi_create_string_utf8(env, (*it).c_str(), NAPI_AUTO_LENGTH, &value);
        if (status != napi_ok) {
            HiLog::Error(LABEL, "GetAvailableZoneCityIDs: Failed to create string item");
            return nullptr;
        }
        status = napi_set_element(env, result, index, value);
        if (status != napi_ok) {
            HiLog::Error(LABEL, "GetAvailableZoneCityIDs: Failed to set array item");
            return nullptr;
        }
        ++index;
    }
    return result;
}

napi_value I18nAddon::GetCityDisplayName(napi_env env, napi_callback_info info)
{
    size_t argc = 2;
    napi_value argv[2] = { 0 };
    napi_value thisVar = nullptr;
    void *data = nullptr;
    napi_status status = napi_get_cb_info(env, info, &argc, argv, &thisVar, &data);
    if (status != napi_ok) {
        return nullptr;
    }
    if (argv[0] == nullptr || argv[1] == nullptr) {
        return nullptr;
    }
    napi_valuetype valueType = napi_valuetype::napi_undefined;
    napi_typeof(env, argv[0], &valueType);
    if (valueType != napi_valuetype::napi_string) {
        HiLog::Error(LABEL, "Invalid parameter type");
        return nullptr;
    }
    int32_t code = 0;
    std::string cityID = VariableConvertor::GetString(env, argv[0], code);
    if (code != 0) {
        return nullptr;
    }
    std::string locale = VariableConvertor::GetString(env, argv[1], code);
    if (code != 0) {
        return nullptr;
    }
    std::string name = I18nTimeZone::GetCityDisplayName(cityID, locale);
    napi_value result = nullptr;
    status = napi_create_string_utf8(env, name.c_str(), NAPI_AUTO_LENGTH, &result);
    if (status != napi_ok) {
        return nullptr;
    }
    return result;
}

napi_value I18nAddon::StaticGetTimeZone(napi_env env, napi_value *argv, bool isZoneID)
{
    napi_value constructor = nullptr;
    napi_status status = napi_get_reference_value(env, *g_timezoneConstructor, &constructor);
    if (status != napi_ok) {
        HiLog::Error(LABEL, "Failed to create reference at StaticGetTimeZone");
        return nullptr;
    }
    napi_value newArgv[2] = { 0 };
    newArgv[0] = argv[0];
    status = napi_get_boolean(env, isZoneID, &newArgv[1]);
    if (status != napi_ok) {
        return nullptr;
    }
    napi_value result = nullptr;
    status = napi_new_instance(env, constructor, 2, newArgv, &result); // 2 is parameter num
    if (status != napi_ok) {
        HiLog::Error(LABEL, "StaticGetTimeZone create instance failed");
        return nullptr;
    }
    return result;
}

napi_value I18nAddon::GetTimezoneFromCity(napi_env env, napi_callback_info info)
{
    size_t argc = 1;
    napi_value argv[1] = { nullptr };
    napi_value thisVar = nullptr;
    void *data = nullptr;
    napi_get_cb_info(env, info, &argc, argv, &thisVar, &data);
    return StaticGetTimeZone(env, argv, false);
}

napi_value I18nAddon::InitCharacter(napi_env env, napi_value exports)
{
    napi_status status = napi_ok;
    napi_property_descriptor properties[] = {
        DECLARE_NAPI_FUNCTION("isDigit", IsDigitAddon),
        DECLARE_NAPI_FUNCTION("isSpaceChar", IsSpaceCharAddon),
        DECLARE_NAPI_FUNCTION("isWhitespace", IsWhiteSpaceAddon),
        DECLARE_NAPI_FUNCTION("isRTL", IsRTLCharacterAddon),
        DECLARE_NAPI_FUNCTION("isIdeograph", IsIdeoGraphicAddon),
        DECLARE_NAPI_FUNCTION("isLetter", IsLetterAddon),
        DECLARE_NAPI_FUNCTION("isLowerCase", IsLowerCaseAddon),
        DECLARE_NAPI_FUNCTION("isUpperCase", IsUpperCaseAddon),
        DECLARE_NAPI_FUNCTION("getType", GetTypeAddon)
    };

    napi_value constructor = nullptr;
    status = napi_define_class(env, "Character", NAPI_AUTO_LENGTH, ObjectConstructor, nullptr,
        sizeof(properties) / sizeof(napi_property_descriptor), properties, &constructor);
    if (status != napi_ok) {
        HiLog::Error(LABEL, "Define class failed when InitCharacter");
        return nullptr;
    }

    status = napi_set_named_property(env, exports, "Character", constructor);
    if (status != napi_ok) {
        HiLog::Error(LABEL, "Set property failed when InitCharacter");
        return nullptr;
    }
    return exports;
}

napi_value I18nAddon::ObjectConstructor(napi_env env, napi_callback_info info)
{
    size_t argc = 0;
    napi_value argv[0];
    napi_value thisVar = nullptr;
    void *data = nullptr;
    napi_status status = napi_get_cb_info(env, info, &argc, argv, &thisVar, &data);
    if (status != napi_ok) {
        return nullptr;
    }
    std::unique_ptr<I18nAddon> obj = nullptr;
    obj = std::make_unique<I18nAddon>();
    status =
        napi_wrap(env, thisVar, reinterpret_cast<void *>(obj.get()), I18nAddon::Destructor, nullptr, nullptr);
    if (status != napi_ok) {
        HiLog::Error(LABEL, "Wrap I18nAddon failed");
        return nullptr;
    }
    obj.release();
    return thisVar;
}

napi_value I18nAddon::InitUtil(napi_env env, napi_value exports)
{
    napi_status status = napi_ok;
    napi_property_descriptor properties[] = {
        DECLARE_NAPI_FUNCTION("unitConvert", UnitConvert)
    };

    napi_value constructor = nullptr;
    status = napi_define_class(env, "Util", NAPI_AUTO_LENGTH, ObjectConstructor, nullptr,
        sizeof(properties) / sizeof(napi_property_descriptor), properties, &constructor);
    if (status != napi_ok) {
        HiLog::Error(LABEL, "Define class failed when InitUtil");
        return nullptr;
    }

    status = napi_set_named_property(env, exports, "Util", constructor);
    if (status != napi_ok) {
        HiLog::Error(LABEL, "Set property failed when InitUtil");
        return nullptr;
    }
    return exports;
}

napi_value I18nAddon::GetTimezonesByLocation(napi_env env, napi_callback_info info)
{
    size_t argc = 2;
    napi_value argv[2] = {0, 0};
    napi_value thisVar = nullptr;
    void *data = nullptr;
    napi_status status = napi_get_cb_info(env, info, &argc, argv, &thisVar, &data);
    if (status != napi_ok) {
        return nullptr;
    }
    if (argv[0] == nullptr || argv[1] == nullptr) {
        HiLog::Error(LABEL, "GetTimezonesByLocation: Missing parameter");
        ErrorUtil::NapiThrow(env, I18N_NOT_FOUND, true);
        return nullptr;
    }
    double x, y;
    status = napi_get_value_double(env, argv[0], &x);
    if (status != napi_ok) {
        HiLog::Error(LABEL, "GetTimezonesByLocation: Parse first argument x failed");
        ErrorUtil::NapiThrow(env, I18N_NOT_VALID, true);
        return nullptr;
    }
    status = napi_get_value_double(env, argv[1], &y);
    if (status != napi_ok) {
        HiLog::Error(LABEL, "GetTimezonesByLocation: Parse second argument y failed");
        ErrorUtil::NapiThrow(env, I18N_NOT_VALID, true);
        return nullptr;
    }
    // -180 and 179.9 is the scope of longitude, -90 and 89.9 is the scope of latitude
    if (x < -180 || x > 179.9 || y < -90 || y > 89.9) {
        HiLog::Error(LABEL, "GetTimezonesByLocation: Args x or y exceed it's scope.");
        ErrorUtil::NapiThrow(env, I18N_NOT_VALID, true);
        return nullptr;
    }
    napi_value timezoneList = nullptr;
    napi_create_array(env, &timezoneList);
    std::vector<std::string> tempList = I18nTimeZone::GetTimezoneIdByLocation(x, y);
    for (size_t i = 0; i < tempList.size(); i++) {
        napi_value timezoneId = nullptr;
        status = napi_create_string_utf8(env, tempList[i].c_str(), NAPI_AUTO_LENGTH, &timezoneId);
        if (status != napi_ok) {
            return nullptr;
        }
        napi_value argTimeZoneId[1] = { timezoneId };
        napi_value timezone = StaticGetTimeZone(env, argTimeZoneId, true);
        status = napi_set_element(env, timezoneList, i, timezone);
        if (status != napi_ok) {
            return nullptr;
        }
    }

    return timezoneList;
}

napi_value Init(napi_env env, napi_value exports)
{
    napi_value val = I18nAddon::Init(env, exports);
    val = I18nAddon::InitPhoneNumberFormat(env, val);
    val = I18nAddon::InitBreakIterator(env, val);
    val = I18nCalendarAddon::InitI18nCalendar(env, val);
    val = I18nAddon::InitIndexUtil(env, val);
    val = I18nAddon::InitI18nTimeZone(env, val);
    val = I18nAddon::InitTransliterator(env, val);
    val = I18nAddon::InitCharacter(env, val);
    val = I18nAddon::InitUtil(env, val);
    val = I18nNormalizerAddon::InitI18nNormalizer(env, val);
    val = SystemLocaleManagerAddon::InitSystemLocaleManager(env, val);
    return val;
}

static napi_module g_i18nModule = {
    .nm_version = 1,
    .nm_flags = 0,
    .nm_filename = nullptr,
    .nm_register_func = Init,
    .nm_modname = "i18n",
    .nm_priv = nullptr,
    .reserved = { 0 }
};

extern "C" __attribute__((constructor)) void I18nRegister()
{
    napi_module_register(&g_i18nModule);
}
} // namespace I18n
} // namespace Global
} // namespace OHOS
