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

#include "i18n_hilog.h"
#include "intl.h"
#include "utils.h"

using namespace OHOS;
using namespace Global;
using namespace I18n;

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

static void SetBooleanMember(ani_env *env, ani_object obj, const std::string name, const std::string value)
{
    if (value.empty()) {
        return;
    }

    static const char *className = "Lstd/core/Boolean;";
    ani_class cls;
    if (ANI_OK != env->FindClass(className, &cls)) {
        HILOG_ERROR_I18N("Find class '%{public}s' failed", className);
        return;
    }

    ani_method ctor;
    if (ANI_OK != env->Class_FindMethod(cls, "<ctor>", "Z:V", &ctor)) {
        HILOG_ERROR_I18N("Find method '<ctor>' failed");
        return;
    }

    ani_boolean valueBool = value == "true";
    ani_object boolObj;
    if (ANI_OK != env->Object_New(cls, ctor, &boolObj, valueBool)) {
        HILOG_ERROR_I18N("New object '%{public}s' failed", className);
        return;
    }

    if (ANI_OK != env->Object_SetPropertyByName_Ref(obj, name.c_str(), boolObj)) {
        HILOG_ERROR_I18N("Set property '%{public}s' failed", name.c_str());
        return;
    }
}

static void SetNumberMember(ani_env *env, ani_object obj, const std::string name, const std::string value)
{
    if (value.empty()) {
        return;
    }

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

    int32_t status = 0;
    ani_int digits = ConvertString2Int(value, status);
    if (status != 0) {
        return;
    }

    ani_object intObj;
    if (ANI_OK != env->Object_New(cls, ctor, &intObj, digits)) {
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

static bool GetBooleanMember(ani_env *env, ani_object options, const std::string name, bool& value)
{
    ani_ref ref;
    if (ANI_OK != env->Object_GetPropertyByName_Ref(options, name.c_str(), &ref)) {
        HILOG_ERROR_I18N("Get property '%{public}s' failed", name.c_str());
        return false;
    }

    ani_boolean isUndefined;
    env->Reference_IsUndefined(ref, &isUndefined);
    if (isUndefined) {
        return false;
    }

    ani_boolean ret;
    env->Object_CallMethodByName_Boolean(static_cast<ani_object>(ref), "unboxed", ":Z", &ret);
    value = ret;
    return true;
}

static bool GetNumberMember(ani_env *env, ani_object options, const std::string name, int& value)
{
    ani_ref ref;
    if (ANI_OK != env->Object_GetPropertyByName_Ref(options, name.c_str(), &ref)) {
        HILOG_ERROR_I18N("Get property '%{public}s' failed", name.c_str());
        return false;
    }

    ani_boolean isUndefined;
    env->Reference_IsUndefined(ref, &isUndefined);
    if (isUndefined) {
        return false;
    }

    ani_double valueDouble;
    env->Object_CallMethodByName_Double(static_cast<ani_object>(ref), "unboxed", ":D", &valueDouble);
    value = static_cast<int>(valueDouble);
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
    env->Reference_IsUndefined(ref, &isUndefined);
    if (isUndefined) {
        return false;
    }

    value = AniStrToString(env, ref);
    return true;
}

static IntlAniAddon* UnwrapAddon(ani_env *env, ani_object object, const std::string name)
{
    ani_long ptr;
    if (ANI_OK != env->Object_GetFieldByName_Long(object, name.c_str(), &ptr)) {
        HILOG_ERROR_I18N("Get Long '%{public}s' failed", name.c_str());
        return nullptr;
    }
    return reinterpret_cast<IntlAniAddon*>(ptr);
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

template<typename T>
T IntlAniAddon::GetLocaleProperty(ani_env *env, ani_object object,
    const std::string& propertyName, T (LocaleInfo::*getter)() const)
{
    IntlAniAddon* obj = UnwrapAddon(env, object, "nativeLocale");
    if (obj == nullptr || obj->locale_ == nullptr) {
        HILOG_ERROR_I18N("%{public}s: Get Locale object failed", propertyName.c_str());
        return T{};
    }
    return (obj->locale_.get()->*getter)();
}

ani_string IntlAniAddon::Locale_toString(ani_env *env, ani_object object)
{
    std::string value = GetLocaleProperty(env, object, "toString", &LocaleInfo::ToString);
    return StringToAniStr(env, value);
}

ani_object IntlAniAddon::Locale_maximize(ani_env *env, ani_object object)
{
    IntlAniAddon* obj = UnwrapAddon(env, object, "nativeLocale");
    if (obj == nullptr || obj->locale_ == nullptr) {
        HILOG_ERROR_I18N("Maximize: Get Locale object failed");
        return nullptr;
    }

    std::string localeTag = obj->locale_->Maximize();
    return Locale_createWithParam(env, nullptr, StringToAniStr(env, localeTag), nullptr);
}

ani_object IntlAniAddon::Locale_minimize(ani_env *env, ani_object object)
{
    IntlAniAddon* obj = UnwrapAddon(env, object, "nativeLocale");
    if (obj == nullptr || obj->locale_ == nullptr) {
        HILOG_ERROR_I18N("Minimize: Get Locale object failed");
        return nullptr;
    }

    std::string localeTag = obj->locale_->Minimize();
    return Locale_createWithParam(env, nullptr, StringToAniStr(env, localeTag), nullptr);
}

ani_string IntlAniAddon::Locale_getLanguage(ani_env *env, ani_object object)
{
    std::string value = GetLocaleProperty(env, object, "getLanguage", &LocaleInfo::GetLanguage);
    return StringToAniStr(env, value);
}

ani_string IntlAniAddon::Locale_getScript(ani_env *env, ani_object object)
{
    std::string value = GetLocaleProperty(env, object, "getScript", &LocaleInfo::GetScript);
    return StringToAniStr(env, value);
}

ani_string IntlAniAddon::Locale_getRegion(ani_env *env, ani_object object)
{
    std::string value = GetLocaleProperty(env, object, "getRegion", &LocaleInfo::GetRegion);
    return StringToAniStr(env, value);
}

ani_string IntlAniAddon::Locale_getBaseName(ani_env *env, ani_object object)
{
    std::string value = GetLocaleProperty(env, object, "getBaseName", &LocaleInfo::GetBaseName);
    return StringToAniStr(env, value);
}

ani_string IntlAniAddon::Locale_getCaseFirst(ani_env *env, ani_object object)
{
    std::string value = GetLocaleProperty(env, object, "getCaseFirst", &LocaleInfo::GetCaseFirst);
    return StringToAniStr(env, value);
}

ani_string IntlAniAddon::Locale_getCalendar(ani_env *env, ani_object object)
{
    std::string value = GetLocaleProperty(env, object, "getCalendar", &LocaleInfo::GetCalendar);
    return StringToAniStr(env, value);
}

ani_string IntlAniAddon::Locale_getCollation(ani_env *env, ani_object object)
{
    std::string value = GetLocaleProperty(env, object, "getCollation", &LocaleInfo::GetCollation);
    return StringToAniStr(env, value);
}

ani_string IntlAniAddon::Locale_getHourCycle(ani_env *env, ani_object object)
{
    std::string value = GetLocaleProperty(env, object, "getHourCycle", &LocaleInfo::GetHourCycle);
    return StringToAniStr(env, value);
}

ani_string IntlAniAddon::Locale_getNumberingSystem(ani_env *env, ani_object object)
{
    std::string value = GetLocaleProperty(env, object, "getNumberingSystem", &LocaleInfo::GetNumberingSystem);
    return StringToAniStr(env, value);
}

ani_boolean IntlAniAddon::Locale_getNumeric(ani_env *env, ani_object object)
{
    IntlAniAddon* obj = UnwrapAddon(env, object, "nativeLocale");
    if (obj == nullptr || obj->locale_ == nullptr) {
        HILOG_ERROR_I18N("getNumeric: Get Locale object failed");
        return false;
    }
    return (obj->locale_->GetNumeric() == "true");
}


ani_string IntlAniAddon::DateTimeFormat_format(ani_env *env, ani_object object, ani_object date)
{
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

    IntlAniAddon *obj = UnwrapAddon(env, object, "nativeDateTimeFormat");
    if (obj == nullptr || obj->datefmt_ == nullptr) {
        HILOG_ERROR_I18N("FormatDateTime: Get DateTimeFormat object failed");
        return nullptr;
    }

    std::string str = obj->datefmt_->Format(milliseconds);
    return StringToAniStr(env, str);
}

ani_string IntlAniAddon::DateTimeFormat_formatRange(ani_env *env, ani_object object,
    ani_object startDate, ani_object endDate)
{
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

    ani_double firstMilliseconds;
    if (ANI_OK != env->Object_CallMethod_Double(startDate, getms, &firstMilliseconds)) {
        HILOG_ERROR_I18N("Call method 'valueOf' failed");
        return nullptr;
    }

    ani_double secondMilliseconds;
    if (ANI_OK != env->Object_CallMethod_Double(endDate, getms, &secondMilliseconds)) {
        HILOG_ERROR_I18N("Call method 'valueOf' failed");
        return nullptr;
    }

    IntlAniAddon *obj = UnwrapAddon(env, object, "nativeDateTimeFormat");
    if (obj == nullptr || obj->datefmt_ == nullptr) {
        HILOG_ERROR_I18N("FormatDateTimeRange: Get DateTimeFormat object failed");
        return nullptr;
    }

    std::string str = obj->datefmt_->FormatRange(firstMilliseconds, secondMilliseconds);
    return StringToAniStr(env, str);
}

ani_object IntlAniAddon::DateTimeFormat_resolvedOptions(ani_env *env, ani_object object)
{
    IntlAniAddon *obj = UnwrapAddon(env, object, "nativeDateTimeFormat");
    if (obj == nullptr || obj->datefmt_ == nullptr) {
        HILOG_ERROR_I18N("GetDateTimeResolvedOptions: Get DateTimeFormat object failed");
        return nullptr;
    }
    std::map<std::string, std::string> options = {};
    obj->datefmt_->GetResolvedOptions(options);

    static const char *className = "L@ohos/intl/intl/DateTimeOptionsInner;";
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

    ani_object dateTimeOptions;
    if (ANI_OK != env->Object_New(cls, ctor, &dateTimeOptions)) {
        HILOG_ERROR_I18N("New object '%{public}s' failed", className);
        return nullptr;
    }

    std::vector<std::string> keys = {
        "locale", "dateStyle", "timeStyle", "hourCycle", "timeZone", "numberingSystem",
        "weekday", "era", "year", "month", "day", "hour", "second",
        "timeZoneName", "dayPeriod", "localeMatcher", "formatMatcher"
    };

    for (const auto& key : keys) {
        if (options.find(key) != options.end()) {
            SetStringMember(env, dateTimeOptions, key, options[key]);
        }
    }
    SetBooleanMember(env, dateTimeOptions, "hour12", options["hour12"]);
    return dateTimeOptions;
}

ani_string IntlAniAddon::NumberFormat_format(ani_env *env, ani_object object, ani_double number)
{
    IntlAniAddon *obj = UnwrapAddon(env, object, "nativeNumberFormat");
    if (obj == nullptr || obj->numberfmt_ == nullptr) {
        HILOG_ERROR_I18N("FormatNumber: Get NumberFormat object failed");
        return nullptr;
    }

    std::string str = obj->numberfmt_->Format(number);
    return StringToAniStr(env, str);
}

ani_string IntlAniAddon::NumberFormat_formatRange(ani_env *env, ani_object object,
    ani_double startRange, ani_double endRange)
{
    IntlAniAddon *obj = UnwrapAddon(env, object, "nativeNumberFormat");
    if (obj == nullptr || obj->numberfmt_ == nullptr) {
        HILOG_ERROR_I18N("FormatRangeNumber: Get NumberFormat object failed");
        return nullptr;
    }

    std::string str = obj->numberfmt_->FormatRange(startRange, endRange);
    return StringToAniStr(env, str);
}

ani_object IntlAniAddon::NumberFormat_resolvedOptions(ani_env *env, ani_object object)
{
    IntlAniAddon *obj = UnwrapAddon(env, object, "nativeNumberFormat");
    if (obj == nullptr || obj->numberfmt_ == nullptr) {
        HILOG_ERROR_I18N("GetNumberResolvedOptions: Get NumberFormat object failed");
        return nullptr;
    }
    std::map<std::string, std::string> options = {};
    obj->numberfmt_->GetResolvedOptions(options);

    static const char *className = "L@ohos/intl/intl/NumberOptionsInner;";
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

    ani_object numberOptions;
    if (ANI_OK != env->Object_New(cls, ctor, &numberOptions)) {
        HILOG_ERROR_I18N("New object '%{public}s' failed", className);
        return nullptr;
    }

    std::vector<std::string> keys = {
        "locale", "currency", "currencySign", "currencyDisplay", "unit", "unitDisplay", "unitUsage",
        "signDisplay", "compactDisplay", "notation", "localeMatcher", "style", "numberingSystem",
        "roundingPriority", "roundingMode", "minimumIntegerDigits", "minimumFractionDigits", "maximumFractionDigits",
        "minimumSignificantDigits", "maximumSignificantDigits", "roundingIncrement"
    };

    for (const auto& key : keys) {
        if (options.find(key) != options.end()) {
            if (key == "minimumIntegerDigits" || key == "minimumFractionDigits" ||
                key == "maximumFractionDigits" || key == "minimumSignificantDigits" ||
                key == "maximumSignificantDigits" || key == "roundingIncrement") {
                SetNumberMember(env, numberOptions, key, options[key]);
            } else {
                SetStringMember(env, numberOptions, key, options[key]);
            }
        }
    }
    SetBooleanMember(env, numberOptions, "useGrouping", options["useGrouping"]);
    return numberOptions;
}

ani_double IntlAniAddon::Collator_compare(ani_env *env, ani_object object, ani_string first, ani_string second)
{
    IntlAniAddon *obj = UnwrapAddon(env, object, "nativeCollator");
    if (obj == nullptr || obj->collator_ == nullptr) {
        HILOG_ERROR_I18N("CompareString: Get Collator object failed");
        return -1;
    }

    CompareResult compareResult = obj->collator_->Compare(AniStrToString(env, first), AniStrToString(env, second));
    return static_cast<double>(compareResult);
}
ani_object IntlAniAddon::Collator_resolvedOptions(ani_env *env, ani_object object)
{
    IntlAniAddon *obj = UnwrapAddon(env, object, "nativeCollator");
    if (obj == nullptr || obj->collator_ == nullptr) {
        HILOG_ERROR_I18N("GetCollatorResolvedOptions: Get Collator object failed");
        return nullptr;
    }
    std::map<std::string, std::string> options = {};
    obj->collator_->ResolvedOptions(options);

    static const char *className = "L@ohos/intl/intl/CollatorOptionsInner;";
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

    ani_object collatorOptions;
    if (ANI_OK != env->Object_New(cls, ctor, &collatorOptions)) {
        HILOG_ERROR_I18N("New object '%{public}s' failed", className);
        return nullptr;
    }

    std::vector<std::string> keys = {"localeMatcher", "usage", "sensitivity", "collation", "caseFirst"};
    for (const auto& key : keys) {
        if (options.find(key) != options.end()) {
            SetStringMember(env, collatorOptions, key, options[key]);
        }
    }
    SetBooleanMember(env, collatorOptions, "ignorePunctuation", options["ignorePunctuation"]);
    SetBooleanMember(env, collatorOptions, "numeric", options["numeric"]);
    return collatorOptions;
}

ani_string IntlAniAddon::PluralRules_select(ani_env *env, ani_object object, ani_double number)
{
    IntlAniAddon *obj = UnwrapAddon(env, object, "nativePluralRules");
    if (obj == nullptr || obj->pluralrules_ == nullptr) {
        HILOG_ERROR_I18N("Get PluralRules object failed");
        return nullptr;
    }

    std::string str = obj->pluralrules_->Select(number);
    return StringToAniStr(env, str);
}

ani_string IntlAniAddon::RelativeTimeFormat_format(ani_env *env, ani_object object, ani_double value, ani_string unit)
{
    IntlAniAddon *obj = UnwrapAddon(env, object, "nativeRelativeTimeFormat");
    if (obj == nullptr || obj->relativetimefmt_ == nullptr) {
        HILOG_ERROR_I18N("FormatRelativeTime: Get RelativeTimeFormat object failed");
        return nullptr;
    }

    std::string str = obj->relativetimefmt_->Format(value, AniStrToString(env, unit));
    return StringToAniStr(env, str);
}

static void FillInArrayElement(ani_env *env, ani_class arrayCls, ani_object &result,
    const std::vector<std::vector<std::string>> &timeVector)
{
    for (size_t i = 0; i < timeVector.size(); i++) {
        std::string value = timeVector[i][1];
        std::string type = timeVector[i][0];
        std::string unit = "";
        size_t unitIndex = 2;
        if (timeVector[i].size() > unitIndex) {
            unit = timeVector[i][unitIndex];
        }

        static const char *className = "L@ohos/intl/intl/RelativeTimeFormatPartInner;";
        ani_class cls;
        if (ANI_OK != env->FindClass(className, &cls)) {
            HILOG_ERROR_I18N("Find class '%{public}s' failed", className);
            return;
        }

        ani_method ctor;
        if (ANI_OK != env->Class_FindMethod(cls, "<ctor>", nullptr, &ctor)) {
            HILOG_ERROR_I18N("Find method '<ctor>' failed");
            return;
        }

        ani_object obj;
        if (ANI_OK != env->Object_New(cls, ctor, &obj)) {
            HILOG_ERROR_I18N("New object '%{public}s' failed", className);
            return;
        }

        SetStringMember(env, obj, "type", type);
        SetStringMember(env, obj, "unit", unit);
        SetNumberMember(env, obj, "value", value);

        ani_method set;
        if (ANI_OK != env->Class_FindMethod(arrayCls, "$_set", "ILstd/core/Object;:V", &set)) {
            HILOG_ERROR_I18N("Find method '$_set' failed");
            return;
        }

        if (ANI_OK != env->Object_CallMethod_Void(result, set, i, obj)) {
            HILOG_ERROR_I18N("Call method '$_set' failed");
            return;
        }
    }
}

ani_object IntlAniAddon::RelativeTimeFormat_formatToParts(ani_env *env, ani_object object,
    ani_double value, ani_string unit)
{
    IntlAniAddon *obj = UnwrapAddon(env, object, "nativeRelativeTimeFormat");
    if (obj == nullptr || obj->relativetimefmt_ == nullptr) {
        HILOG_ERROR_I18N("FormatToParts: Get RelativeTimeFormat object failed");
        return nullptr;
    }

    std::vector<std::vector<std::string>> timeVector;
    obj->relativetimefmt_->FormatToParts(value, AniStrToString(env, unit), timeVector);

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
    if (ANI_OK != env->Object_New(cls, ctor, &ret, timeVector.size())) {
        HILOG_ERROR_I18N("New object '%{public}s' failed", className);
        return nullptr;
    }

    FillInArrayElement(env, cls, ret, timeVector);
    return ret;
}

ani_object IntlAniAddon::RelativeTimeFormat_resolvedOptions(ani_env *env, ani_object object)
{
    IntlAniAddon *obj = UnwrapAddon(env, object, "nativeRelativeTimeFormat");
    if (obj == nullptr || obj->relativetimefmt_ == nullptr) {
        HILOG_ERROR_I18N("GetRelativeTimeResolvedOptions: Get RelativeTimeFormat object failed");
        return nullptr;
    }
    std::map<std::string, std::string> options = {};
    obj->relativetimefmt_->GetResolvedOptions(options);

    static const char *className = "L@ohos/intl/intl/RelativeTimeFormatResolvedOptionsInner;";
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

    ani_object relativeTimeFormatResolvedOptions;
    if (ANI_OK != env->Object_New(cls, ctor, &relativeTimeFormatResolvedOptions)) {
        HILOG_ERROR_I18N("New object '%{public}s' failed", className);
        return nullptr;
    }

    std::vector<std::string> keys = {"locale", "style", "numeric", "numberingSystem"};
    for (const auto& key : keys) {
        if (options.find(key) != options.end()) {
            SetStringMember(env, relativeTimeFormatResolvedOptions, key, options[key]);
        }
    }
    return relativeTimeFormatResolvedOptions;
}

ani_object IntlAniAddon::Locale_create(ani_env *env, [[maybe_unused]] ani_class clazz)
{
    std::map<std::string, std::string> map = {};
    std::unique_ptr<IntlAniAddon> obj = std::make_unique<IntlAniAddon>();
    obj->locale_ = std::make_unique<LocaleInfo>("", map);

    static const char *className = "L@ohos/intl/intl/Locale;";
    ani_object locale_object = CreateAniObject(env, className, obj.get());
    obj.release();
    return locale_object;
}

ani_object IntlAniAddon::Locale_createWithParam(ani_env *env, [[maybe_unused]] ani_class clazz,
    ani_string locale, ani_object options)
{
    std::map<std::string, std::string> map = {};
    ani_boolean isUndefined;
    env->Reference_IsUndefined(options, &isUndefined);
    if (!isUndefined) {
        const std::vector<std::string> keys = {
            "calendar", "collation", "hourCycle", "numberingSystem", "caseFirst"
        };

        for (const auto& key : keys) {
            std::string value;
            if (GetStringMember(env, options, key, value)) {
                map.insert(std::make_pair(key, value));
            }
        }

        bool numeric;
        if (GetBooleanMember(env, options, "numeric", numeric)) {
            map.insert(std::make_pair("numeric", numeric ? "true" : "false"));
        }
    }

    std::unique_ptr<IntlAniAddon> obj = std::make_unique<IntlAniAddon>();
    obj->locale_ = std::make_unique<LocaleInfo>(AniStrToString(env, locale), map);
    static const char *className = "L@ohos/intl/intl/Locale;";
    ani_object locale_object = CreateAniObject(env, className, obj.get());
    obj.release();
    return locale_object;
}

static std::vector<std::string> getLocaleTags(ani_env *env, ani_object locale)
{
    std::vector<std::string> localeTags;
    ani_class stringClass;
    if (ANI_OK != env->FindClass("Lstd/core/String;", &stringClass)) {
        HILOG_ERROR_I18N("Find class 'Lstd/core/String' failed");
        return localeTags;
    }

    ani_boolean isString;
    env->Object_InstanceOf(locale, stringClass, &isString);
    if (isString) {
        localeTags.push_back(AniStrToString(env, locale));
    } else {
        static const char *className = "Lescompat/Array;";
        ani_class cls;
        if (ANI_OK != env->FindClass(className, &cls)) {
            HILOG_ERROR_I18N("Find class '%{public}s' failed", className);
            return localeTags;
        }

        ani_method getLengthMethod;
        if (ANI_OK != env->Class_FindGetter(cls, "length", &getLengthMethod)) {
            HILOG_ERROR_I18N("Find getter 'length' failed");
            return localeTags;
        }

        ani_double length;
        if (ANI_OK != env->Object_CallMethod_Double(locale, getLengthMethod, &length)) {
            HILOG_ERROR_I18N("Get 'length' failed");
            return localeTags;
        }

        for (int i = 0; i < length; i++) {
            ani_method get;
            if (ANI_OK != env->Class_FindMethod(cls, "$_get", "I:Lstd/core/Object;", &get)) {
                HILOG_ERROR_I18N("Find method '$_get' failed");
                return localeTags;
            }

            ani_ref value;
            if (ANI_OK != env->Object_CallMethod_Ref(locale, get, &value, i)) {
                HILOG_ERROR_I18N("Call method '$_get' failed");
                return localeTags;
            }
            localeTags.push_back(AniStrToString(env, value));
        }
    }
    return localeTags;
}

ani_object IntlAniAddon::DateTimeFormat_create(ani_env *env, [[maybe_unused]] ani_class clazz)
{
    std::unique_ptr<IntlAniAddon> obj = std::make_unique<IntlAniAddon>();
    std::vector<std::string> localeTags;
    std::map<std::string, std::string> map = {};

    obj->datefmt_ = DateTimeFormat::CreateInstance(localeTags, map);
    static const char *className = "L@ohos/intl/intl/DateTimeFormat;";
    ani_object dateTimeFormat_object = CreateAniObject(env, className, obj.get());
    obj.release();
    return dateTimeFormat_object;
}

static std::map<std::string, std::string> getDateTimeFormatOptions(ani_env *env, ani_object options)
{
    std::map<std::string, std::string> map;
    ani_boolean isUndefined;
    env->Reference_IsUndefined(options, &isUndefined);
    if (!isUndefined) {
        std::vector<std::string> stringKeys = {
            "locale", "dateStyle", "timeStyle", "hourCycle", "timeZone", "numberingSystem",
            "weekday", "era", "year", "month", "day", "hour", "minute", "second",
            "timeZoneName", "dayPeriod", "localeMatcher", "formatMatcher"
        };
        for (const auto& key : stringKeys) {
            std::string value;
            if (GetStringMember(env, options, key, value)) {
                map.insert(std::make_pair(key, value));
            }
        }

        bool hour12;
        if (GetBooleanMember(env, options, "hour12", hour12)) {
            map.insert(std::make_pair("hour12", hour12 ? "true" : "false"));
        }
    }
    return map;
}

ani_object IntlAniAddon::DateTimeFormat_createWithParam(ani_env *env, [[maybe_unused]] ani_class clazz,
    ani_object locale, ani_object options)
{
    std::vector<std::string> localeTags = getLocaleTags(env, locale);
    std::map<std::string, std::string> map = getDateTimeFormatOptions(env, options);

    std::unique_ptr<IntlAniAddon> obj = std::make_unique<IntlAniAddon>();
    obj->datefmt_ = DateTimeFormat::CreateInstance(localeTags, map);

    static const char *className = "L@ohos/intl/intl/DateTimeFormat;";
    ani_object dateTimeFormat_object = CreateAniObject(env, className, obj.get());
    obj.release();
    return dateTimeFormat_object;
}

ani_object IntlAniAddon::NumberFormat_create(ani_env *env, [[maybe_unused]] ani_class clazz)
{
    std::vector<std::string> localeTags;
    std::map<std::string, std::string> map = {};
    std::unique_ptr<IntlAniAddon> obj = std::make_unique<IntlAniAddon>();
    obj->numberfmt_ = std::make_unique<NumberFormat>(localeTags, map);

    static const char *className = "L@ohos/intl/intl/NumberFormat;";
    ani_object numberFormat_object = CreateAniObject(env, className, obj.get());
    obj.release();
    return numberFormat_object;
}

static std::map<std::string, std::string> getNumberFormatOptions(ani_env *env, ani_object options)
{
    std::map<std::string, std::string> map;
    ani_boolean isUndefined;
    env->Reference_IsUndefined(options, &isUndefined);
    if (!isUndefined) {
        std::vector<std::string> stringKeys = {
            "locale", "currency", "currencySign", "currencyDisplay", "unit", "unitDisplay", "unitUsage",
            "signDisplay", "compactDisplay", "notation", "localeMatcher", "style", "numberingSystem"
        };
        for (const auto& key : stringKeys) {
            std::string value;
            if (GetStringMember(env, options, key, value)) {
                map.insert(std::make_pair(key, value));
            }
        }

        std::vector<std::pair<std::string, int>> intKeys = {
            {"minimumIntegerDigits", 0}, {"minimumFractionDigits", 0}, {"maximumFractionDigits", 0},
            {"minimumSignificantDigits", 0}, {"maximumSignificantDigits", 0}, {"roundingIncrement", 0}
        };
        for (auto& [key, value] : intKeys) {
            if (GetNumberMember(env, options, key, value)) {
                map.insert(std::make_pair(key, std::to_string(value)));
            }
        }

        bool useGrouping;
        if (GetBooleanMember(env, options, "useGrouping", useGrouping)) {
            map.insert(std::make_pair("useGrouping", useGrouping ? "true" : "false"));
        }
    }
    return map;
}

ani_object IntlAniAddon::NumberFormat_createWithParam(ani_env *env, [[maybe_unused]] ani_class clazz,
    ani_object locale, ani_object options)
{
    std::vector<std::string> localeTags = getLocaleTags(env, locale);
    std::map<std::string, std::string> map = getNumberFormatOptions(env, options);

    std::unique_ptr<IntlAniAddon> obj = std::make_unique<IntlAniAddon>();
    obj->numberfmt_ = std::make_unique<NumberFormat>(localeTags, map);

    static const char *className = "L@ohos/intl/intl/NumberFormat;";
    ani_object numberFormat_object = CreateAniObject(env, className, obj.get());
    obj.release();
    return numberFormat_object;
}

ani_object IntlAniAddon::Collator_create(ani_env *env, [[maybe_unused]] ani_class clazz)
{
    std::vector<std::string> localeTags;
    std::map<std::string, std::string> map = {};
    std::unique_ptr<IntlAniAddon> obj = std::make_unique<IntlAniAddon>();
    obj->collator_ = std::make_unique<Collator>(localeTags, map);

    static const char *className = "L@ohos/intl/intl/Collator;";
    ani_object collator_object = CreateAniObject(env, className, obj.get());
    obj.release();
    return collator_object;
}

static std::map<std::string, std::string> getCollatorOptions(ani_env *env, ani_object options)
{
    std::map<std::string, std::string> map;
    ani_boolean isUndefined;
    env->Reference_IsUndefined(options, &isUndefined);
    if (!isUndefined) {
        std::vector<std::string> stringKeys = {"localeMatcher", "usage", "sensitivity", "collation", "caseFirst"};
        for (const auto& key : stringKeys) {
            std::string value;
            if (GetStringMember(env, options, key, value)) {
                map.insert(std::make_pair(key, value));
            }
        }

        bool ignorePunctuation;
        if (GetBooleanMember(env, options, "ignorePunctuation", ignorePunctuation)) {
            map.insert(std::make_pair("ignorePunctuation", ignorePunctuation ? "true" : "false"));
        }

        bool numeric;
        if (GetBooleanMember(env, options, "numeric", numeric)) {
            map.insert(std::make_pair("numeric", numeric ? "true" : "false"));
        }
    }
    return map;
}

ani_object IntlAniAddon::Collator_createWithParam(ani_env *env, [[maybe_unused]] ani_class clazz,
    ani_object locale, ani_object options)
{
    std::vector<std::string> localeTags = getLocaleTags(env, locale);
    std::map<std::string, std::string> map = getCollatorOptions(env, options);

    std::unique_ptr<IntlAniAddon> obj = std::make_unique<IntlAniAddon>();
    obj->collator_ = std::make_unique<Collator>(localeTags, map);

    static const char *className = "L@ohos/intl/intl/Collator;";
    ani_object collator_object = CreateAniObject(env, className, obj.get());
    obj.release();
    return collator_object;
}

ani_object IntlAniAddon::PluralRules_create(ani_env *env, [[maybe_unused]] ani_class clazz)
{
    std::vector<std::string> localeTags;
    std::map<std::string, std::string> map = {};
    std::unique_ptr<IntlAniAddon> obj = std::make_unique<IntlAniAddon>();
    obj->pluralrules_ = std::make_unique<PluralRules>(localeTags, map);

    static const char *className = "L@ohos/intl/intl/PluralRules;";
    ani_object pluralRules_object = CreateAniObject(env, className, obj.get());
    obj.release();
    return pluralRules_object;
}

static std::map<std::string, std::string> getPluralRulesOptions(ani_env *env, ani_object options)
{
    std::map<std::string, std::string> map;
    ani_boolean isUndefined;
    env->Reference_IsUndefined(options, &isUndefined);
    if (!isUndefined) {
        std::vector<std::string> stringKeys = {"localeMatcher", "type"};
        for (const auto& key : stringKeys) {
            std::string value;
            if (GetStringMember(env, options, key, value)) {
                map.insert(std::make_pair(key, value));
            }
        }

        std::vector<std::pair<std::string, int>> intKeys = {
            {"minimumIntegerDigits", 0}, {"minimumFractionDigits", 0}, {"maximumFractionDigits", 0},
            {"minimumSignificantDigits", 0}, {"maximumSignificantDigits", 0}
        };
        for (auto& [key, value] : intKeys) {
            if (GetNumberMember(env, options, key, value)) {
                map.insert(std::make_pair(key, std::to_string(value)));
            }
        }
    }
    return map;
}

ani_object IntlAniAddon::PluralRules_createWithParam(ani_env *env, [[maybe_unused]] ani_class clazz,
    ani_object locale, ani_object options)
{
    std::vector<std::string> localeTags = getLocaleTags(env, locale);
    std::map<std::string, std::string> map = getPluralRulesOptions(env, options);

    std::unique_ptr<IntlAniAddon> obj = std::make_unique<IntlAniAddon>();
    obj->pluralrules_ = std::make_unique<PluralRules>(localeTags, map);

    static const char *className = "L@ohos/intl/intl/PluralRules;";
    ani_object pluralRules_object = CreateAniObject(env, className, obj.get());
    obj.release();
    return pluralRules_object;
}

ani_object IntlAniAddon::RelativeTimeFormat_create(ani_env *env, [[maybe_unused]] ani_class clazz)
{
    std::vector<std::string> localeTags;
    std::map<std::string, std::string> map = {};
    std::unique_ptr<IntlAniAddon> obj = std::make_unique<IntlAniAddon>();
    obj->relativetimefmt_ = std::make_unique<RelativeTimeFormat>(localeTags, map);

    static const char *className = "L@ohos/intl/intl/RelativeTimeFormat;";
    ani_object relativeTimeFormat_object = CreateAniObject(env, className, obj.get());
    obj.release();
    return relativeTimeFormat_object;
}

static std::map<std::string, std::string> getRelativeTimeFormatOptions(ani_env *env, ani_object options)
{
    std::map<std::string, std::string> map;
    ani_boolean isUndefined;
    env->Reference_IsUndefined(options, &isUndefined);
    if (!isUndefined) {
        std::vector<std::string> stringKeys = {"localeMatcher", "numeric", "style"};
        for (const auto& key : stringKeys) {
            std::string value;
            if (GetStringMember(env, options, key, value)) {
                map.insert(std::make_pair(key, value));
            }
        }
    }
    return map;
}

ani_object IntlAniAddon::RelativeTimeFormat_createWithParam(ani_env *env, [[maybe_unused]] ani_class clazz,
    ani_object locale, ani_object options)
{
    std::vector<std::string> localeTags = getLocaleTags(env, locale);
    std::map<std::string, std::string> map = getRelativeTimeFormatOptions(env, options);

    std::unique_ptr<IntlAniAddon> obj = std::make_unique<IntlAniAddon>();
    obj->relativetimefmt_= std::make_unique<RelativeTimeFormat>(localeTags, map);

    static const char *className = "L@ohos/intl/intl/RelativeTimeFormat;";
    ani_object relativeTimeFormat_object = CreateAniObject(env, className, obj.get());
    obj.release();
    return relativeTimeFormat_object;
}

ani_status IntlAniAddon::BindContext_Locale(ani_env *env)
{
    static const char *className = "L@ohos/intl/intl/Locale;";
    ani_class cls;
    if (ANI_OK != env->FindClass(className, &cls)) {
        HILOG_ERROR_I18N("Find class '%{public}s' failed", className);
        return (ani_status)ANI_ERROR;
    }

    std::array methods = {
        ani_native_function {"create", ":L@ohos/intl/intl/Locale;", reinterpret_cast<void *>(Locale_create) },
        ani_native_function {"create", "Lstd/core/String;L@ohos/intl/intl/LocaleOptions;:L@ohos/intl/intl/Locale;",
            reinterpret_cast<void *>(Locale_createWithParam) },
        ani_native_function {"toString", nullptr, reinterpret_cast<void *>(Locale_toString) },
        ani_native_function {"maximize", nullptr, reinterpret_cast<void *>(Locale_maximize) },
        ani_native_function {"minimize", nullptr, reinterpret_cast<void *>(Locale_minimize) },
        ani_native_function {"getLanguage", nullptr, reinterpret_cast<void *>(Locale_getLanguage) },
        ani_native_function {"getScript", nullptr, reinterpret_cast<void *>(Locale_getScript) },
        ani_native_function {"getRegion", nullptr, reinterpret_cast<void *>(Locale_getRegion) },
        ani_native_function {"getBaseName", nullptr, reinterpret_cast<void *>(Locale_getBaseName) },
        ani_native_function {"getCaseFirst", nullptr, reinterpret_cast<void *>(Locale_getCaseFirst) },
        ani_native_function {"getCalendar", nullptr, reinterpret_cast<void *>(Locale_getCalendar) },
        ani_native_function {"getCollation", nullptr, reinterpret_cast<void *>(Locale_getCollation) },
        ani_native_function {"getHourCycle", nullptr, reinterpret_cast<void *>(Locale_getHourCycle) },
        ani_native_function {"getNumberingSystem", nullptr, reinterpret_cast<void *>(Locale_getNumberingSystem) },
        ani_native_function {"getNumeric", nullptr, reinterpret_cast<void *>(Locale_getNumeric) },
    };

    if (ANI_OK != env->Class_BindNativeMethods(cls, methods.data(), methods.size())) {
        HILOG_ERROR_I18N("Cannot bind native methods to '%{public}s'", className);
        return (ani_status)ANI_ERROR;
    };
    return ANI_OK;
}

ani_status IntlAniAddon::BindContext_DateTimeFormat(ani_env *env)
{
    static const char *className = "L@ohos/intl/intl/DateTimeFormat;";
    ani_class cls;
    if (ANI_OK != env->FindClass(className, &cls)) {
        HILOG_ERROR_I18N("Find class '%{public}s' failed", className);
        return (ani_status)ANI_ERROR;
    }

    std::array methods = {
        ani_native_function {"create", ":L@ohos/intl/intl/DateTimeFormat;",
            reinterpret_cast<void *>(DateTimeFormat_create) },
        ani_native_function {"create",
            "Lstd/core/Object;L@ohos/intl/intl/DateTimeOptions;:L@ohos/intl/intl/DateTimeFormat;",
            reinterpret_cast<void *>(DateTimeFormat_createWithParam) },
        ani_native_function {"format", nullptr, reinterpret_cast<void *>(DateTimeFormat_format) },
        ani_native_function {"formatRange", nullptr, reinterpret_cast<void *>(DateTimeFormat_formatRange) },
        ani_native_function {"resolvedOptions", nullptr, reinterpret_cast<void *>(DateTimeFormat_resolvedOptions) },
    };

    if (ANI_OK != env->Class_BindNativeMethods(cls, methods.data(), methods.size())) {
        HILOG_ERROR_I18N("Cannot bind native methods to '%{public}s'", className);
        return (ani_status)ANI_ERROR;
    };
    return ANI_OK;
}

ani_status IntlAniAddon::BindContext_NumberFormat(ani_env *env)
{
    static const char *className = "L@ohos/intl/intl/NumberFormat;";
    ani_class cls;
    if (ANI_OK != env->FindClass(className, &cls)) {
        HILOG_ERROR_I18N("Find class '%{public}s' failed", className);
        return (ani_status)ANI_ERROR;
    }

    std::array methods = {
        ani_native_function {"create", ":L@ohos/intl/intl/NumberFormat;",
            reinterpret_cast<void *>(NumberFormat_create) },
        ani_native_function {"create",
            "Lstd/core/Object;L@ohos/intl/intl/NumberOptions;:L@ohos/intl/intl/NumberFormat;",
            reinterpret_cast<void *>(NumberFormat_createWithParam) },
        ani_native_function {"format", nullptr, reinterpret_cast<void *>(NumberFormat_format) },
        ani_native_function {"formatRange", nullptr, reinterpret_cast<void *>(NumberFormat_formatRange) },
        ani_native_function {"resolvedOptions", nullptr, reinterpret_cast<void *>(NumberFormat_resolvedOptions) },
    };

    if (ANI_OK != env->Class_BindNativeMethods(cls, methods.data(), methods.size())) {
        HILOG_ERROR_I18N("Cannot bind native methods to '%{public}s'", className);
        return (ani_status)ANI_ERROR;
    };
    return ANI_OK;
}

ani_status IntlAniAddon::BindContext_Collator(ani_env *env)
{
    static const char *className = "L@ohos/intl/intl/Collator;";
    ani_class cls;
    if (ANI_OK != env->FindClass(className, &cls)) {
        HILOG_ERROR_I18N("Find class '%{public}s' failed", className);
        return (ani_status)ANI_ERROR;
    }

    std::array methods = {
        ani_native_function {"create", ":L@ohos/intl/intl/Collator;", reinterpret_cast<void *>(Collator_create) },
        ani_native_function {"create",
            "Lstd/core/Object;L@ohos/intl/intl/CollatorOptions;:L@ohos/intl/intl/Collator;",
            reinterpret_cast<void *>(Collator_createWithParam) },
        ani_native_function {"compare", nullptr, reinterpret_cast<void *>(Collator_compare) },
        ani_native_function {"resolvedOptions", nullptr, reinterpret_cast<void *>(Collator_resolvedOptions) },
    };

    if (ANI_OK != env->Class_BindNativeMethods(cls, methods.data(), methods.size())) {
        HILOG_ERROR_I18N("Cannot bind native methods to '%{public}s'", className);
        return (ani_status)ANI_ERROR;
    };
    return ANI_OK;
}

ani_status IntlAniAddon::BindContext_PluralRules(ani_env *env)
{
    static const char *className = "L@ohos/intl/intl/PluralRules;";
    ani_class cls;
    if (ANI_OK != env->FindClass(className, &cls)) {
        HILOG_ERROR_I18N("Find class '%{public}s' failed", className);
        return (ani_status)ANI_ERROR;
    }

    std::array methods = {
        ani_native_function {"create", ":L@ohos/intl/intl/PluralRules;", reinterpret_cast<void *>(PluralRules_create) },
        ani_native_function {"create",
            "Lstd/core/Object;L@ohos/intl/intl/PluralRulesOptions;:L@ohos/intl/intl/PluralRules;",
            reinterpret_cast<void *>(PluralRules_createWithParam) },
        ani_native_function {"select", nullptr, reinterpret_cast<void *>(PluralRules_select) },
    };

    if (ANI_OK != env->Class_BindNativeMethods(cls, methods.data(), methods.size())) {
        HILOG_ERROR_I18N("Cannot bind native methods to '%{public}s'", className);
        return (ani_status)ANI_ERROR;
    };
    return ANI_OK;
}

ani_status IntlAniAddon::BindContext_RelativeTimeFormat(ani_env *env)
{
    static const char *className = "L@ohos/intl/intl/RelativeTimeFormat;";
    ani_class cls;
    if (ANI_OK != env->FindClass(className, &cls)) {
        HILOG_ERROR_I18N("Find class '%{public}s' failed", className);
        return (ani_status)ANI_ERROR;
    }

    std::array methods = {
        ani_native_function {"create", ":L@ohos/intl/intl/RelativeTimeFormat;",
            reinterpret_cast<void *>(RelativeTimeFormat_create) },
        ani_native_function {"create",
            "Lstd/core/Object;L@ohos/intl/intl/RelativeTimeFormatInputOptions;:L@ohos/intl/intl/RelativeTimeFormat;",
            reinterpret_cast<void *>(RelativeTimeFormat_createWithParam) },
        ani_native_function {"format", nullptr, reinterpret_cast<void *>(RelativeTimeFormat_format) },
        ani_native_function {"formatToParts", nullptr, reinterpret_cast<void *>(RelativeTimeFormat_formatToParts) },
        ani_native_function {"resolvedOptions", nullptr, reinterpret_cast<void *>(RelativeTimeFormat_resolvedOptions) },
    };

    if (ANI_OK != env->Class_BindNativeMethods(cls, methods.data(), methods.size())) {
        HILOG_ERROR_I18N("Cannot bind native methods to '%{public}s'", className);
        return (ani_status)ANI_ERROR;
    };
    return ANI_OK;
}

ANI_EXPORT ani_status ANI_Constructor(ani_vm *vm, uint32_t *result)
{
    ani_env *env;
    if (ANI_OK != vm->GetEnv(ANI_VERSION_1, &env)) {
        HILOG_ERROR_I18N("Unsupported ANI_VERSION_1");
        return (ani_status)ANI_ERROR;
    }

    auto status = IntlAniAddon::BindContext_Locale(env);
    if (status != ANI_OK) {
        return status;
    }

    status = IntlAniAddon::BindContext_DateTimeFormat(env);
    if (status != ANI_OK) {
        return status;
    }

    status = IntlAniAddon::BindContext_NumberFormat(env);
    if (status != ANI_OK) {
        return status;
    }

    status = IntlAniAddon::BindContext_Collator(env);
    if (status != ANI_OK) {
        return status;
    }

    status = IntlAniAddon::BindContext_PluralRules(env);
    if (status != ANI_OK) {
        return status;
    }

    status = IntlAniAddon::BindContext_RelativeTimeFormat(env);
    if (status != ANI_OK) {
        return status;
    }
    *result = ANI_VERSION_1;
    return ANI_OK;
}
