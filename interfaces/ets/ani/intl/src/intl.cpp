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

#include "intl.h"
#include "ucmndata.h"
#include "unicode/udata.h"
#include "unicode/utypes.h"

using namespace OHOS;
using namespace Global;
using namespace I18n;

extern "C" const DataHeader U_DATA_API U_ICUDATA_ENTRY_POINT;
bool g_icuInit = false;

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

static IntlAniAddon* unwrapAddon(ani_env *env, ani_object object, std::string name)
{
    ani_long ptr;
    if (ANI_OK != env->Object_GetFieldByName_Long(object, name.c_str(), &ptr)) {
        return nullptr;
    }
    return reinterpret_cast<IntlAniAddon*>(ptr);
}

static void setterString(ani_env *env, ani_class cls, std::string name, std::string value, ani_object obj)
{
    if (value.empty()) {
        return;
    }

    ani_method setter;
    if (ANI_OK != env->Class_FindSetter(cls, name.c_str(), &setter)) {
        std::cerr << "Class_FindSetter failed '" << name << "'" << std::endl;
    }

    ani_string value_string{};
    env->String_NewUTF8(value.c_str(), value.size(), &value_string);
    if (ANI_OK != env->Object_CallMethod_Void(obj, setter, value_string)) {
        std::cerr << "Object_CallMethod_Void Fail" << std::endl;
    }
}

static void setterNumber(ani_env *env, ani_class cls, std::string name, std::string value, ani_object obj)
{
    if (value.empty()) {
        return;
    }

    ani_method setter;
    if (ANI_OK != env->Class_FindSetter(cls, name.c_str(), &setter)) {
        std::cerr << "Class_FindSetter failed '" << name << "'" << std::endl;
    }

    ani_double value_double = std::stod(value);
    if (ANI_OK != env->Object_CallMethod_Void(obj, setter, value_double)) {
        std::cerr << "Object_CallMethod_Void Fail" << std::endl;
    }
}

static bool getBooleanMember(ani_env *env, ani_object options,
    std::string cName, std::string mName, bool& member)
{
    ani_class cls;
    const char *className = cName.c_str();
    const char *memberName = mName.c_str();
   
    if (ANI_OK != env->FindClass(className, &cls)) {
        std::cerr << "Not found '" << className << "'" << std::endl;
        return false;
    }
   
    ani_method getter;
    if (ANI_OK != env->Class_FindGetter(cls, memberName, &getter)) {
        std::cerr << "Class_FindGetter failed '" << memberName << "'" << std::endl;
        return false;
    }

    ani_ref ref;
    if (ANI_OK != env->Object_CallMethod_Ref(options, getter, &ref)) {
        std::cerr << "Object_CallMethod_Ref Fail" << std::endl;
        return false;
    }

    ani_boolean isUndefined;
    env->Reference_IsUndefined(ref, &isUndefined);
    if (isUndefined) {
        return false;
    }

    ani_boolean ret;
    env->Object_CallMethodByName_Boolean(static_cast<ani_object>(ref), "unboxed", ":Z", &ret);
    member = ret;
    return true;
}

static bool getDoubleMember(ani_env *env, ani_object options,
    std::string cName, std::string mName, double& member)
{
    ani_class cls;
    const char *className = cName.c_str();
    const char *memberName = mName.c_str();
   
    if (ANI_OK != env->FindClass(className, &cls)) {
        std::cerr << "Not found '" << className << "'" << std::endl;
        return false;
    }
   
    ani_method getter;
    if (ANI_OK != env->Class_FindGetter(cls, memberName, &getter)) {
        std::cerr << "Class_FindGetter failed '" << memberName << "'" << std::endl;
        return false;
    }

    ani_ref ret;
    if (ANI_OK != env->Object_CallMethod_Ref(options, getter, &ret)) {
        std::cerr << "Object_CallMethod_Ref Fail" << std::endl;
        return false;
    }

    ani_boolean isUndefined;
    env->Reference_IsUndefined(ret, &isUndefined);
    if (isUndefined) {
        return false;
    }

    env->Object_CallMethodByName_Double(static_cast<ani_object>(ret), "unboxed", ":D", &member);
    return true;
}

static bool getStringMember(ani_env *env, ani_object options, std::string cName,
    std::string mName, std::string& member)
{
    ani_class cls;
    const char *className = cName.c_str();
    const char *memberName = mName.c_str();
   
    if (ANI_OK != env->FindClass(className, &cls)) {
        std::cerr << "Not found '" << className << "'" << std::endl;
        return false;
    }
   
    ani_method getter;
    if (ANI_OK != env->Class_FindGetter(cls, memberName, &getter)) {
        std::cerr << "Class_FindGetter failed '" << memberName << "'" << std::endl;
        return false;
    }

    ani_ref ret;
    if (ANI_OK != env->Object_CallMethod_Ref(options, getter, &ret)) {
        std::cerr << "Object_CallMethod_Ref Fail" << std::endl;
        return false;
    }

    ani_boolean isUndefined;
    env->Reference_IsUndefined(ret, &isUndefined);
    if (isUndefined) {
        return false;
    }

    member = AniStrToString(env, ret);
    return true;
}

ani_string IntlAniAddon::DateTimeFormat_format([[maybe_unused]] ani_env *env, [[maybe_unused]] ani_object object,
    ani_object Date)
{
    std::string str = "";
    ani_string ret;
    env->String_NewUTF8(str.c_str(), str.size(), &ret);
    ani_class cls;
    if (ANI_OK != env->FindClass("Lescompat/Date;", &cls)) {
        std::cerr << "Not found 'Lescompat/Date'" << std::endl;
        return ret;
    }

    ani_method getms;
    if (ANI_OK != env->Class_FindMethod(cls, "valueOf", nullptr, &getms)) {
        std::cerr << "Find Method Fail 'valueOf'" << std::endl;
        return ret;
    }

    ani_double milliseconds;
    if (ANI_OK != env->Object_CallMethod_Double(Date, getms, &milliseconds)) {
        std::cerr << "Object_CallMethod_Double Fail" << std::endl;
        return ret;
    }

    IntlAniAddon *obj = unwrapAddon(env, object, "nativeDateTimeFormat");
    str = obj->datefmt_->Format(milliseconds);
    env->String_NewUTF8(str.c_str(), str.size(), &ret);
    return ret;
}

ani_object IntlAniAddon::DateTimeFormat_resolvedOptions([[maybe_unused]] ani_env *env, [[maybe_unused]]
    ani_object object)
{
    IntlAniAddon *obj = unwrapAddon(env, object, "nativeDateTimeFormat");
    std::map<std::string, std::string> options = {};
    obj->datefmt_->GetResolvedOptions(options);
    
    ani_object DateTimeOptions = {};
    static const char *className = "L@ohos/intl/intl/DateTimeOptionsInner;";
    ani_class cls;
    if (ANI_OK != env->FindClass(className, &cls)) {
        std::cerr << "Not found '" << className << "'" << std::endl;
        return DateTimeOptions;
    }

    ani_method ctor;
    if (ANI_OK != env->Class_FindMethod(cls, "<ctor>", nullptr, &ctor)) {
        std::cerr << "get ctor Failed'" << className << "'" << std::endl;
        return DateTimeOptions;
    }

    if (ANI_OK != env->Object_New(cls, ctor, &DateTimeOptions)) {
        std::cerr << "Create Object Failed'" << className << "'" << std::endl;
        return DateTimeOptions;
    }

    ani_method hour12Setter;
    if (ANI_OK != env->Class_FindSetter(cls, "hour12", &hour12Setter)) {
        std::cerr << "Class_FindSetter Fail'" << className << "'" << std::endl;
    }

    ani_boolean hour12 = options["hour12"] == "true";
    if (ANI_OK != env->Object_CallMethod_Void(DateTimeOptions, hour12Setter, hour12)) {
        std::cerr << "Object_CallMethod_Void Fail'" << className << "'" << std::endl;
        return DateTimeOptions;
    }

    std::vector<std::string> keys = {
        "locale", "dateStyle", "timeStyle", "hourCycle", "timeZone", "numberingSystem",
        "weekday", "era", "year", "month", "day", "hour", "second",
        "timeZoneName", "dayPeriod", "localeMatcher", "formatMatcher"
    };

    for (const auto& key : keys) {
        if (options.find(key) != options.end()) {
            setterString(env, cls, key, options[key], DateTimeOptions);
        }
    }

    return DateTimeOptions;
}

ani_string IntlAniAddon::NumberFormat_format([[maybe_unused]] ani_env *env, [[maybe_unused]] ani_object object,
    ani_double number)
{
    IntlAniAddon *obj = unwrapAddon(env, object, "nativeNumberFormat");
    std::string str = obj->numberfmt_->Format(number);
    ani_string ret;
    env->String_NewUTF8(str.c_str(), str.size(), &ret);
    return ret;
}

ani_object IntlAniAddon::NumberFormat_resolvedOptions([[maybe_unused]] ani_env *env, [[maybe_unused]] ani_object object)
{
    IntlAniAddon *obj = unwrapAddon(env, object, "nativeNumberFormat");
    std::map<std::string, std::string> options = {};
    obj->numberfmt_->GetResolvedOptions(options);
    
    ani_object NumberOptions = {};

    static const char *className = "L@ohos/intl/intl/NumberOptionsInner;";
    ani_class cls;
    if (ANI_OK != env->FindClass(className, &cls)) {
        std::cerr << "Not found '" << className << "'" << std::endl;
        return NumberOptions;
    }

    ani_method ctor;
    if (ANI_OK != env->Class_FindMethod(cls, "<ctor>", nullptr, &ctor)) {
        std::cerr << "get ctor Failed'" << className << "'" << std::endl;
        return NumberOptions;
    }

    if (ANI_OK != env->Object_New(cls, ctor, &NumberOptions)) {
        std::cerr << "Create Object Failed'" << className << "'" << std::endl;
        return NumberOptions;
    }

    ani_method useGroupingSetter;
    if (ANI_OK != env->Class_FindSetter(cls, "useGrouping", &useGroupingSetter)) {
        std::cerr << "Class_FindSetter Fail'" << className << "'" << std::endl;
    }

    ani_boolean useGrouping = options["useGrouping"] == "true";
    if (ANI_OK != env->Object_CallMethod_Void(NumberOptions, useGroupingSetter, useGrouping)) {
        std::cerr << "Object_CallMethod_Void Fail'" << className << "'" << std::endl;
        return NumberOptions;
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
                setterNumber(env, cls, key, options[key], NumberOptions);
            } else {
                setterString(env, cls, key, options[key], NumberOptions);
            }
        }
    }
    
    return NumberOptions;
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

    std::string className = "L@ohos/intl/intl/Locale;";
    ani_boolean isUndefined;
    env->Reference_IsUndefined(options, &isUndefined);
    if (!isUndefined) {
        const std::vector<std::string> keys = {
            "calendar", "collation", "hourCycle", "numberingSystem", "caseFirst"
        };

        for (const auto& key : keys) {
            std::string value;
            if (getStringMember(env, options, className, key, value)) {
                map.insert(std::make_pair(key, value));
            }
        }
    
        bool numeric;
        if (getBooleanMember(env, options, className, "numeric", numeric)) {
            map.insert(std::make_pair("numeric", numeric ? "true" : "false"));
        }
    }

    std::unique_ptr<IntlAniAddon> obj = std::make_unique<IntlAniAddon>();
    obj->locale_ = std::make_unique<LocaleInfo>(localeTag, map);

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

std::vector<std::string> getLocaleTags(ani_env *env, ani_object locale)
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

    if (!g_icuInit) {
        UErrorCode err;
        udata_setCommonDataAfterClean(&U_ICUDATA_ENTRY_POINT, &err);
        g_icuInit = true;
    }

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

std::map<std::string, std::string> getDateTimeFormatOptions(ani_env *env,
    ani_object options, const std::string &className)
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
            if (getStringMember(env, options, className, key, value)) {
                map.insert(std::make_pair(key, value));
            }
        }

        bool hour12;
        if (getBooleanMember(env, options, className, "hour12", hour12)) {
            map.insert(std::make_pair("hour12", hour12 ? "true" : "false"));
        }
    }
    return map;
}

ani_object IntlAniAddon::DateTimeFormat_create1(ani_env *env, ani_class clazz, ani_object locale, ani_object options)
{
    std::vector<std::string> localeTags = getLocaleTags(env, locale);
    std::map<std::string, std::string> map =
        getDateTimeFormatOptions(env, options, "L@ohos/intl/intl/DateTimeOptions;");

    if (!g_icuInit) {
        UErrorCode err;
        udata_setCommonDataAfterClean(&U_ICUDATA_ENTRY_POINT, &err);
        g_icuInit = true;
    }

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

std::map<std::string, std::string> getNumberFormatOptions(ani_env *env,
    ani_object options, const std::string &className)
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
            if (getStringMember(env, options, className, key, value)) {
                map.insert(std::make_pair(key, value));
            }
        }

        std::vector<std::pair<std::string, double>> doubleKeys = {
            {"minimumIntegerDigits", 0}, {"minimumFractionDigits", 0}, {"maximumFractionDigits", 0},
            {"minimumSignificantDigits", 0}, {"maximumSignificantDigits", 0}, {"roundingIncrement", 0}
        };
        for (auto& [key, value] : doubleKeys) {
            if (getDoubleMember(env, options, className, key, value)) {
                map.insert(std::make_pair(key, std::to_string(value)));
            }
        }

        bool useGrouping;
        if (getBooleanMember(env, options, className, "useGrouping", useGrouping)) {
            map.insert(std::make_pair("useGrouping", useGrouping ? "true" : "false"));
        }
    }
    return map;
}

ani_object IntlAniAddon::NumberFormat_create1(ani_env *env, ani_class clazz, ani_object locale, ani_object options)
{
    std::vector<std::string> localeTags = getLocaleTags(env, locale);
    std::map<std::string, std::string> map = getNumberFormatOptions(env, options, "L@ohos/intl/intl/NumberOptions;");

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