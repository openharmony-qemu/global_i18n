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
    env->String_GetUTF8Size(ani_str, &strSize);
   
    std::vector<char> buffer(strSize + 1);
    char* utf8Buffer = buffer.data();

    ani_size bytes_written = 0;
    env->String_GetUTF8(ani_str, utf8Buffer, strSize + 1, &bytes_written);
    
    utf8Buffer[bytes_written] = '\0';
    std::string content = std::string(utf8Buffer);
    return content;
}

static ani_string StringToAniStr(ani_env *env, const std::string &str)
{
    ani_string ret;
    if (ANI_OK != env->String_NewUTF8(str.c_str(), str.size(), &ret)) {
        std::cerr << "Create string failed" << std::endl;
        return nullptr;
    }
    return ret;
}

static IntlAniAddon* UnwrapAddon(ani_env *env, ani_object object, std::string name)
{
    ani_long ptr;
    if (ANI_OK != env->Object_GetFieldByName_Long(object, name.c_str(), &ptr)) {
        return nullptr;
    }
    return reinterpret_cast<IntlAniAddon*>(ptr);
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


template<typename T>
T IntlAniAddon::GetLocaleProperty(ani_env *env, ani_object object,
    const std::string& propertyName, T (LocaleInfo::*getter)() const)
{
    IntlAniAddon* obj = UnwrapAddon(env, object, "nativeLocale");
    if (obj == nullptr || obj->locale_ == nullptr) {
        std::cerr << propertyName + ": Get Locale object failed" << std::endl;
        return T{};
    }
    return (obj->locale_.get()->*getter)();
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
        std::cerr << "getNumeric: Get Locale object failed"  << std::endl;
        return false;
    }
    return (obj->locale_->GetNumeric() == "true");
}

ani_string IntlAniAddon::DateTimeFormat_format([[maybe_unused]] ani_env *env, [[maybe_unused]] ani_object object,
    ani_object Date)
{
    ani_class cls;
    if (ANI_OK != env->FindClass("Lescompat/Date;", &cls)) {
        std::cerr << "Not found 'Lescompat/Date'" << std::endl;
        return nullptr;
    }

    ani_method getms;
    if (ANI_OK != env->Class_FindMethod(cls, "valueOf", nullptr, &getms)) {
        std::cerr << "Find Method Fail 'valueOf'" << std::endl;
        return nullptr;
    }

    ani_double milliseconds;
    if (ANI_OK != env->Object_CallMethod_Double(Date, getms, &milliseconds)) {
        std::cerr << "Object_CallMethod_Double Fail" << std::endl;
        return nullptr;
    }

    IntlAniAddon *obj = UnwrapAddon(env, object, "nativeDateTimeFormat");
    if (obj == nullptr || obj->datefmt_ == nullptr) {
        return nullptr;
    }
    std::string str = obj->datefmt_->Format(milliseconds);
    ani_string ret;
    env->String_NewUTF8(str.c_str(), str.size(), &ret);
    return ret;
}

ani_object IntlAniAddon::DateTimeFormat_resolvedOptions([[maybe_unused]] ani_env *env, [[maybe_unused]]
    ani_object object)
{
    std::map<std::string, std::string> options = {};
    IntlAniAddon *obj = UnwrapAddon(env, object, "nativeDateTimeFormat");
    if (obj == nullptr || obj->datefmt_ == nullptr) {
        return nullptr;
    }
    obj->datefmt_->GetResolvedOptions(options);

    static const char *className = "L@ohos/intl/intl/DateTimeOptionsInner;";
    ani_class cls;
    if (ANI_OK != env->FindClass(className, &cls)) {
        std::cerr << "Not found '" << className << "'" << std::endl;
        return nullptr;
    }

    ani_method ctor;
    if (ANI_OK != env->Class_FindMethod(cls, "<ctor>", nullptr, &ctor)) {
        std::cerr << "get ctor Failed'" << className << "'" << std::endl;
        return nullptr;
    }

    ani_object dateTimeOptions = {};
    if (ANI_OK != env->Object_New(cls, ctor, &dateTimeOptions)) {
        std::cerr << "Create Object Failed'" << className << "'" << std::endl;
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

ani_string IntlAniAddon::NumberFormat_format([[maybe_unused]] ani_env *env, [[maybe_unused]] ani_object object,
    ani_double number)
{
    IntlAniAddon *obj = UnwrapAddon(env, object, "nativeNumberFormat");
    if (obj == nullptr || obj->numberfmt_ == nullptr) {
        return nullptr;
    }
    std::string str = obj->numberfmt_->Format(number);
    ani_string ret;
    env->String_NewUTF8(str.c_str(), str.size(), &ret);
    return ret;
}

ani_object IntlAniAddon::NumberFormat_resolvedOptions([[maybe_unused]] ani_env *env, [[maybe_unused]] ani_object object)
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

ani_object IntlAniAddon::Locale_create0([[maybe_unused]] ani_env *env, [[maybe_unused]] ani_class clazz)
{
    std::map<std::string, std::string> map = {};
    std::unique_ptr<IntlAniAddon> obj = std::make_unique<IntlAniAddon>();
    obj->locale_ = std::make_unique<LocaleInfo>("", map);

    static const char *className = "L@ohos/intl/intl/Locale;";
    ani_class cls;
    if (ANI_OK != env->FindClass(className, &cls)) {
        std::cerr << "Not found '" << className << "'" << std::endl;
        ani_object nullobj = nullptr;
        return nullobj;
    }

    ani_method ctor;
    if (ANI_OK != env->Class_FindMethod(cls, "<ctor>", "J:V", &ctor)) {
        std::cerr << "get ctor Failed'" << className << "'" << std::endl;
        ani_object nullobj = nullptr;
        return nullobj;
    }

    ani_object Locale_object;
    if (ANI_OK != env->Object_New(cls, ctor, &Locale_object, reinterpret_cast<ani_long>(obj.get()))) {
        std::cerr << "New Locale Fail" << std::endl;
    }
    obj.release();
    return Locale_object;
}

ani_object IntlAniAddon::Locale_create1([[maybe_unused]] ani_env *env, [[maybe_unused]] ani_class clazz,
    ani_string locale, ani_object options)
{
    char buffer[256];
    ani_size s;
    env->String_GetUTF8(locale, buffer, sizeof(buffer), &s);
    std::string localeTag = std::string(buffer, s);
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
    obj->locale_ = std::make_unique<LocaleInfo>(localeTag, map);

    std::string className = "L@ohos/intl/intl/Locale;";
    ani_class cls;
    if (ANI_OK != env->FindClass(className.c_str(), &cls)) {
        std::cerr << "Not found '" << className << "'" << std::endl;
        ani_object nullobj = nullptr;
        return nullobj;
    }

    ani_method ctor;
    if (ANI_OK != env->Class_FindMethod(cls, "<ctor>", "J:V", &ctor)) {
        std::cerr << "get ctor Failed'" << className << "'" << std::endl;
        ani_object nullobj = nullptr;
        return nullobj;
    }

    ani_object Locale_object;
    if (ANI_OK != env->Object_New(cls, ctor, &Locale_object, reinterpret_cast<ani_long>(obj.get()))) {
        std::cerr << "New Locale Fail" << std::endl;
    }
    obj.release();
    return Locale_object;
}

static std::vector<std::string> GetLocaleTags(ani_env *env, ani_object locale)
{
    std::vector<std::string> localeTags;
    ani_class stringClass;
    env->FindClass("Lstd/core/String;", &stringClass);
    ani_boolean isString;
    env->Object_InstanceOf(locale, stringClass, &isString);
    if (isString) {
        localeTags.push_back(AniStrToString(env, locale));
    } else {
        static const char *className = "Lescompat/Array;";
        ani_class cls;
        if (ANI_OK != env->FindClass(className, &cls)) {
            return localeTags;
        }

        ani_method getLengthMethod;
        if (ANI_OK != env->Class_FindGetter(cls, "length", &getLengthMethod)) {
            return localeTags;
        }

        ani_double length;
        env->Object_CallMethod_Double(locale, getLengthMethod, &length);
        for (int i = 0; i < length; i++) {
            ani_method get;
            env->Class_FindMethod(cls, "$_get", nullptr, &get);
            ani_ref value;
            env->Object_CallMethod_Ref(locale, get, &value, i);
            localeTags.push_back(AniStrToString(env, value));
        }
    }
    return localeTags;
}

ani_object IntlAniAddon::DateTimeFormat_create0([[maybe_unused]] ani_env *env, [[maybe_unused]] ani_class clazz)
{
    std::unique_ptr<IntlAniAddon> obj = std::make_unique<IntlAniAddon>();
    std::vector<std::string> localeTags;
    std::map<std::string, std::string> map = {};

    obj->datefmt_ = DateTimeFormat::CreateInstance(localeTags, map);

    static const char *className = "L@ohos/intl/intl/DateTimeFormat;";
    ani_class cls;
    if (ANI_OK != env->FindClass(className, &cls)) {
        std::cerr << "Not found '" << className << "'" << std::endl;
        ani_object nullobj = nullptr;
        return nullobj;
    }

    ani_method ctor;
    if (ANI_OK != env->Class_FindMethod(cls, "<ctor>", "J:V", &ctor)) {
        std::cerr << "get ctor Failed'" << className << "'" << std::endl;
        ani_object nullobj = nullptr;
        return nullobj;
    }

    ani_object DateTimeFormat_object;
    if (ANI_OK != env->Object_New(cls, ctor, &DateTimeFormat_object, reinterpret_cast<ani_long>(obj.get()))) {
        std::cerr << "New DateTimeFormat Fail" << std::endl;
    }
    obj.release();
    return DateTimeFormat_object;
}

static std::map<std::string, std::string> GetDateTimeFormatOptions(ani_env *env, ani_object options)
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

ani_object IntlAniAddon::DateTimeFormat_create1(ani_env *env, ani_class clazz, ani_object locale, ani_object options)
{
    std::vector<std::string> localeTags = GetLocaleTags(env, locale);
    std::map<std::string, std::string> map = GetDateTimeFormatOptions(env, options);

    std::unique_ptr<IntlAniAddon> obj = std::make_unique<IntlAniAddon>();
    obj->datefmt_ = DateTimeFormat::CreateInstance(localeTags, map);

    std::string className = "L@ohos/intl/intl/DateTimeFormat;";
    ani_class cls;
    if (ANI_OK != env->FindClass(className.c_str(), &cls)) {
        std::cerr << "Not found '" << className << "'" << std::endl;
        ani_object nullobj = nullptr;
        return nullobj;
    }

    ani_method ctor;
    if (ANI_OK != env->Class_FindMethod(cls, "<ctor>", "J:V", &ctor)) {
        std::cerr << "get ctor Failed'" << className << "'" << std::endl;
        ani_object nullobj = nullptr;
        return nullobj;
    }

    ani_object DateTimeFormat_object;
    if (ANI_OK != env->Object_New(cls, ctor, &DateTimeFormat_object, reinterpret_cast<ani_long>(obj.get()))) {
        std::cerr << "New DateTimeFormat Fail" << std::endl;
    }
    obj.release();
    return DateTimeFormat_object;
}

ani_object IntlAniAddon::NumberFormat_create0([[maybe_unused]] ani_env *env, [[maybe_unused]] ani_class clazz)
{
    std::vector<std::string> localeTags;
    std::map<std::string, std::string> map = {};
    std::unique_ptr<IntlAniAddon> obj = std::make_unique<IntlAniAddon>();
    obj->numberfmt_ = std::make_unique<NumberFormat>(localeTags, map);

    static const char *className = "L@ohos/intl/intl/NumberFormat;";
    ani_class cls;
    if (ANI_OK != env->FindClass(className, &cls)) {
        std::cerr << "Not found '" << className << "'" << std::endl;
        ani_object nullobj = nullptr;
        return nullobj;
    }

    ani_method ctor;
    if (ANI_OK != env->Class_FindMethod(cls, "<ctor>", "J:V", &ctor)) {
        std::cerr << "get ctor Failed'" << className << "'" << std::endl;
        ani_object nullobj = nullptr;
        return nullobj;
    }

    ani_object NumberFormat_object;
    if (ANI_OK != env->Object_New(cls, ctor, &NumberFormat_object, reinterpret_cast<ani_long>(obj.get()))) {
        std::cerr << "New NumberFormat Fail" << std::endl;
    }
    obj.release();
    return NumberFormat_object;
}

static std::map<std::string, std::string> GetNumberFormatOptions(ani_env *env, ani_object options)
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

ani_object IntlAniAddon::NumberFormat_create1(ani_env *env, ani_class clazz, ani_object locale, ani_object options)
{
    std::vector<std::string> localeTags = GetLocaleTags(env, locale);
    std::map<std::string, std::string> map = GetNumberFormatOptions(env, options);

    std::unique_ptr<IntlAniAddon> obj = std::make_unique<IntlAniAddon>();
    obj->numberfmt_ = std::make_unique<NumberFormat>(localeTags, map);

    static const char *className = "L@ohos/intl/intl/NumberFormat;";
    ani_class cls;
    if (ANI_OK != env->FindClass(className, &cls)) {
        std::cerr << "Not found '" << className << "'" << std::endl;
        ani_object nullobj = nullptr;
        return nullobj;
    }

    ani_method ctor;
    if (ANI_OK != env->Class_FindMethod(cls, "<ctor>", "J:V", &ctor)) {
        std::cerr << "get ctor Failed'" << className << "'" << std::endl;
        ani_object nullobj = nullptr;
        return nullobj;
    }

    ani_object NumberFormat_object;
    if (ANI_OK != env->Object_New(cls, ctor, &NumberFormat_object, reinterpret_cast<ani_long>(obj.get()))) {
        std::cerr << "New NumberFormat Fail" << std::endl;
    }
    obj.release();
    return NumberFormat_object;
}

ani_status IntlAniAddon::BindContext_Locale(ani_env *env)
{
    static const char *className = "L@ohos/intl/intl/Locale;";
    ani_class cls;
    if (ANI_OK != env->FindClass(className, &cls)) {
        std::cerr << "Not found '" << className << "'" << std::endl;
        return (ani_status)ANI_ERROR;
    }

    std::array methods = {
        ani_native_function {"create", ":L@ohos/intl/intl/Locale;", reinterpret_cast<void *>(Locale_create0) },
        ani_native_function {"create",
            "Lstd/core/String;L@ohos/intl/intl/LocaleOptions;:L@ohos/intl/intl/Locale;",
            reinterpret_cast<void *>(Locale_create1) },
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
        std::cerr << "Cannot bind native methods to '" << className << "'" << std::endl;
        return (ani_status)ANI_ERROR;
    };
    return ANI_OK;
}

ani_status IntlAniAddon::BindContext_DateTimeFormat(ani_env *env)
{
    static const char *className = "L@ohos/intl/intl/DateTimeFormat;";
    ani_class cls;
    if (ANI_OK != env->FindClass(className, &cls)) {
        std::cerr << "Not found '" << className << "'" << std::endl;
        return (ani_status)ANI_ERROR;
    }

    std::array methods = {
        ani_native_function {"create", ":L@ohos/intl/intl/DateTimeFormat;",
            reinterpret_cast<void *>(DateTimeFormat_create0) },
        ani_native_function {"create",
            "Lstd/core/Object;L@ohos/intl/intl/DateTimeOptions;:L@ohos/intl/intl/DateTimeFormat;",
            reinterpret_cast<void *>(DateTimeFormat_create1) },
        ani_native_function {"format", "Lescompat/Date;:Lstd/core/String;",
            reinterpret_cast<void *>(DateTimeFormat_format) },
        ani_native_function {"resolvedOptions", ":L@ohos/intl/intl/DateTimeOptions;",
            reinterpret_cast<void *>(DateTimeFormat_resolvedOptions) },
    };

    if (ANI_OK != env->Class_BindNativeMethods(cls, methods.data(), methods.size())) {
        std::cerr << "Cannot bind native methods to '" << className << "'" << std::endl;
        return (ani_status)ANI_ERROR;
    };
    return ANI_OK;
}

ani_status IntlAniAddon::BindContext_NumberFormat(ani_env *env)
{
    static const char *className = "L@ohos/intl/intl/NumberFormat;";
    ani_class cls;
    if (ANI_OK != env->FindClass(className, &cls)) {
        std::cerr << "Not found '" << className << "'" << std::endl;
        return (ani_status)ANI_ERROR;
    }

    std::array methods_NumberFormat = {
        ani_native_function {"create", ":L@ohos/intl/intl/NumberFormat;",
            reinterpret_cast<void *>(NumberFormat_create0) },
        ani_native_function {"create",
            "Lstd/core/Object;L@ohos/intl/intl/NumberOptions;:L@ohos/intl/intl/NumberFormat;",
            reinterpret_cast<void *>(NumberFormat_create1) },
        ani_native_function {"format", "D:Lstd/core/String;", reinterpret_cast<void *>(NumberFormat_format) },
        ani_native_function {"resolvedOptions", ":L@ohos/intl/intl/NumberOptions;",
            reinterpret_cast<void *>(NumberFormat_resolvedOptions) },
    };

    if (ANI_OK != env->Class_BindNativeMethods(cls, methods_NumberFormat.data(), methods_NumberFormat.size())) {
        std::cerr << "Cannot bind native methods to '" << className << "'" << std::endl;
        return (ani_status)ANI_ERROR;
    };
    return ANI_OK;
}

ANI_EXPORT ani_status ANI_Constructor(ani_vm *vm, uint32_t *result)
{
    ani_env *env;
    if (ANI_OK != vm->GetEnv(ANI_VERSION_1, &env)) {
        std::cerr << "Unsupported ANI_VERSION_1" << std::endl;
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
    *result = ANI_VERSION_1;
    return ANI_OK;
}