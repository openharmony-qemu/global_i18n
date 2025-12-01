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

#include "intl_addon.h"

#include "i18n_hilog.h"
#include "locale_info_addon.h"
#include "number_format_addon.h"
#include "variable_converter.h"

using namespace OHOS;
using namespace Global;
using namespace I18n;

static const int INDEX_ONE_OF_VECTOR = 0;
static const int INDEX_TWO_OF_VECTOR = 1;

static IntlAddon* UnwrapAddon(ani_env *env, ani_object object, const std::string &name)
{
    ani_long ptr;
    if (ANI_OK != env->Object_GetFieldByName_Long(object, name.c_str(), &ptr)) {
        HILOG_ERROR_I18N("Get Long '%{public}s' failed", name.c_str());
        return nullptr;
    }
    return reinterpret_cast<IntlAddon*>(ptr);
}

ani_string IntlAddon::DateTimeFormat_Format(ani_env *env, ani_object object, ani_object date)
{
    static const char *className = "std.core.Date";
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

    ani_long milliseconds;
    if (ANI_OK != env->Object_CallMethod_Long(date, getms, &milliseconds)) {
        HILOG_ERROR_I18N("Call method 'valueOf' failed");
        return nullptr;
    }

    IntlAddon *obj = UnwrapAddon(env, object, "nativeDateTimeFormat");
    if (obj == nullptr || obj->datefmt_ == nullptr) {
        HILOG_ERROR_I18N("FormatDateTime: Get DateTimeFormat object failed");
        return nullptr;
    }

    std::string str = obj->datefmt_->Format(static_cast<double>(milliseconds));
    return VariableConverter::StringToAniStr(env, str);
}

ani_string IntlAddon::DateTimeFormat_FormatRange(ani_env *env, ani_object object,
    ani_object startDate, ani_object endDate)
{
    static const char *className = "std.core.Date";
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

    ani_long firstMilliseconds;
    if (ANI_OK != env->Object_CallMethod_Long(startDate, getms, &firstMilliseconds)) {
        HILOG_ERROR_I18N("Call method 'valueOf' failed");
        return nullptr;
    }

    ani_long secondMilliseconds;
    if (ANI_OK != env->Object_CallMethod_Long(endDate, getms, &secondMilliseconds)) {
        HILOG_ERROR_I18N("Call method 'valueOf' failed");
        return nullptr;
    }

    IntlAddon *obj = UnwrapAddon(env, object, "nativeDateTimeFormat");
    if (obj == nullptr || obj->datefmt_ == nullptr) {
        HILOG_ERROR_I18N("FormatDateTimeRange: Get DateTimeFormat object failed");
        return nullptr;
    }

    std::string str = obj->datefmt_->FormatRange(
        static_cast<double>(firstMilliseconds), static_cast<double>(secondMilliseconds));
    return VariableConverter::StringToAniStr(env, str);
}

ani_object IntlAddon::DateTimeFormat_ResolvedOptions(ani_env *env, ani_object object)
{
    IntlAddon *obj = UnwrapAddon(env, object, "nativeDateTimeFormat");
    if (obj == nullptr || obj->datefmt_ == nullptr) {
        HILOG_ERROR_I18N("GetDateTimeResolvedOptions: Get DateTimeFormat object failed");
        return nullptr;
    }
    std::map<std::string, std::string> options = {};
    obj->datefmt_->GetResolvedOptions(options);

    static const char *className = "@ohos.intl.intl.DateTimeOptionsInner";
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
        "weekday", "era", "year", "month", "day", "hour", "minute", "second",
        "timeZoneName", "dayPeriod", "localeMatcher", "formatMatcher"
    };

    for (const auto& key : keys) {
        if (options.find(key) != options.end()) {
            VariableConverter::SetStringMember(env, dateTimeOptions, key, options[key]);
        }
    }
    VariableConverter::SetBooleanMember(env, dateTimeOptions, "hour12", options["hour12"]);
    return dateTimeOptions;
}


ani_int IntlAddon::Collator_Compare(ani_env *env, ani_object object, ani_string first, ani_string second)
{
    IntlAddon *obj = UnwrapAddon(env, object, "nativeCollator");
    if (obj == nullptr || obj->collator_ == nullptr) {
        HILOG_ERROR_I18N("CompareString: Get Collator object failed");
        return -1;
    }

    CompareResult compareResult = obj->collator_->Compare(
        VariableConverter::AniStrToString(env, first), VariableConverter::AniStrToString(env, second));
    return static_cast<int32_t>(compareResult);
}
ani_object IntlAddon::Collator_ResolvedOptions(ani_env *env, ani_object object)
{
    IntlAddon *obj = UnwrapAddon(env, object, "nativeCollator");
    if (obj == nullptr || obj->collator_ == nullptr) {
        HILOG_ERROR_I18N("GetCollatorResolvedOptions: Get Collator object failed");
        return nullptr;
    }
    std::map<std::string, std::string> options = {};
    obj->collator_->ResolvedOptions(options);

    static const char *className = "@ohos.intl.intl.CollatorOptionsInner";
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
            VariableConverter::SetStringMember(env, collatorOptions, key, options[key]);
        }
    }
    VariableConverter::SetBooleanMember(env, collatorOptions, "ignorePunctuation", options["ignorePunctuation"]);
    VariableConverter::SetBooleanMember(env, collatorOptions, "numeric", options["numeric"]);
    return collatorOptions;
}

ani_string IntlAddon::PluralRules_Select(ani_env *env, ani_object object, ani_double number)
{
    IntlAddon *obj = UnwrapAddon(env, object, "nativePluralRules");
    if (obj == nullptr || obj->pluralrules_ == nullptr) {
        HILOG_ERROR_I18N("Get PluralRules object failed");
        return nullptr;
    }

    std::string str = obj->pluralrules_->Select(number);
    return VariableConverter::StringToAniStr(env, str);
}

ani_string IntlAddon::RelativeTimeFormat_Format(ani_env *env, ani_object object, ani_double value, ani_string unit)
{
    IntlAddon *obj = UnwrapAddon(env, object, "nativeRelativeTimeFormat");
    if (obj == nullptr || obj->relativetimefmt_ == nullptr) {
        HILOG_ERROR_I18N("FormatRelativeTime: Get RelativeTimeFormat object failed");
        return nullptr;
    }

    std::string str = obj->relativetimefmt_->Format(value, VariableConverter::AniStrToString(env, unit));
    return VariableConverter::StringToAniStr(env, str);
}

static void FillInArrayElement(ani_env *env, ani_class arrayCls, ani_object &result,
    const std::vector<std::vector<std::string>> &timeVector)
{
    static const char *className = "@ohos.intl.intl.RelativeTimeFormatPart";
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

    ani_method set;
    if (ANI_OK != env->Class_FindMethod(arrayCls, "$_set", "iY:", &set)) {
        HILOG_ERROR_I18N("Find method '$_set' failed");
        return;
    }

    for (size_t i = 0; i < timeVector.size(); ++i) {
        std::string value = timeVector[i][INDEX_TWO_OF_VECTOR];
        std::string type = timeVector[i][INDEX_ONE_OF_VECTOR];
        std::string unit = "";
        size_t unitIndex = 2;
        if (timeVector[i].size() > unitIndex) {
            unit = timeVector[i][unitIndex];
        }

        ani_object obj;
        if (ANI_OK != env->Object_New(cls, ctor, &obj)) {
            HILOG_ERROR_I18N("New object '%{public}s' failed", className);
            return;
        }

        VariableConverter::SetStringMember(env, obj, "type", type);
        VariableConverter::SetStringMember(env, obj, "value", value);
        VariableConverter::SetStringMember(env, obj, "unit", unit);

        if (ANI_OK != env->Object_CallMethod_Void(result, set, i, obj)) {
            HILOG_ERROR_I18N("Call method '$_set' failed");
            return;
        }
    }
}

ani_object IntlAddon::RelativeTimeFormat_FormatToParts(ani_env *env, ani_object object,
    ani_double value, ani_string unit)
{
    IntlAddon *obj = UnwrapAddon(env, object, "nativeRelativeTimeFormat");
    if (obj == nullptr || obj->relativetimefmt_ == nullptr) {
        HILOG_ERROR_I18N("FormatToParts: Get RelativeTimeFormat object failed");
        return nullptr;
    }

    std::vector<std::vector<std::string>> timeVector;
    obj->relativetimefmt_->FormatToParts(value, VariableConverter::AniStrToString(env, unit), timeVector);

    static const char *className = "std.core.Array";
    ani_class cls;
    if (ANI_OK != env->FindClass(className, &cls)) {
        HILOG_ERROR_I18N("Find class '%{public}s' failed", className);
        return nullptr;
    }

    ani_method ctor;
    if (ANI_OK != env->Class_FindMethod(cls, "<ctor>", "i:", &ctor)) {
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

ani_object IntlAddon::RelativeTimeFormat_ResolvedOptions(ani_env *env, ani_object object)
{
    IntlAddon *obj = UnwrapAddon(env, object, "nativeRelativeTimeFormat");
    if (obj == nullptr || obj->relativetimefmt_ == nullptr) {
        HILOG_ERROR_I18N("GetRelativeTimeResolvedOptions: Get RelativeTimeFormat object failed");
        return nullptr;
    }
    std::map<std::string, std::string> options = {};
    obj->relativetimefmt_->GetResolvedOptions(options);

    static const char *className = "@ohos.intl.intl.RelativeTimeFormatResolvedOptionsInner";
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
            VariableConverter::SetStringMember(env, relativeTimeFormatResolvedOptions, key, options[key]);
        }
    }
    return relativeTimeFormatResolvedOptions;
}

ani_object IntlAddon::DateTimeFormat_Create(ani_env *env, [[maybe_unused]] ani_object object)
{
    std::unique_ptr<IntlAddon> obj = std::make_unique<IntlAddon>();
    std::vector<std::string> localeTags;
    std::map<std::string, std::string> map = {};

    obj->datefmt_ = DateTimeFormat::CreateInstance(localeTags, map);
    static const char *className = "@ohos.intl.intl.DateTimeFormat";
    ani_object dateTimeFormatObject = VariableConverter::CreateAniObject(env, className, obj.get());
    obj.release();
    return dateTimeFormatObject;
}

static std::map<std::string, std::string> GetDateTimeFormatOptions(ani_env *env, ani_object options)
{
    std::map<std::string, std::string> map;
    ani_boolean isUndefined;
    if (ANI_OK != env->Reference_IsUndefined(options, &isUndefined)) {
        HILOG_ERROR_I18N("Reference IsUndefined failed");
        return map;
    }
    if (!isUndefined) {
        std::vector<std::string> stringKeys = {
            "locale", "dateStyle", "timeStyle", "hourCycle", "timeZone", "numberingSystem",
            "weekday", "era", "year", "month", "day", "hour", "minute", "second",
            "timeZoneName", "dayPeriod", "localeMatcher", "formatMatcher"
        };
        for (const auto& key : stringKeys) {
            std::string value;
            if (VariableConverter::GetStringMember(env, options, key, value)) {
                map.insert(std::make_pair(key, value));
            }
        }

        bool hour12;
        if (VariableConverter::GetBooleanMember(env, options, "hour12", hour12)) {
            map.insert(std::make_pair("hour12", hour12 ? "true" : "false"));
        }
    }
    return map;
}

ani_object IntlAddon::DateTimeFormat_CreateWithParam(ani_env *env, [[maybe_unused]] ani_object object,
    ani_object locale, ani_object options)
{
    std::vector<std::string> localeTags = VariableConverter::GetLocaleTags(env, locale);
    std::map<std::string, std::string> map = GetDateTimeFormatOptions(env, options);

    std::unique_ptr<IntlAddon> obj = std::make_unique<IntlAddon>();
    obj->datefmt_ = DateTimeFormat::CreateInstance(localeTags, map);

    static const char *className = "@ohos.intl.intl.DateTimeFormat";
    ani_object dateTimeFormatObject = VariableConverter::CreateAniObject(env, className, obj.get());
    obj.release();
    return dateTimeFormatObject;
}

ani_object IntlAddon::Collator_Create(ani_env *env, [[maybe_unused]] ani_object object)
{
    std::vector<std::string> localeTags;
    std::map<std::string, std::string> map = {};
    std::unique_ptr<IntlAddon> obj = std::make_unique<IntlAddon>();
    obj->collator_ = std::make_unique<Collator>(localeTags, map);

    static const char *className = "@ohos.intl.intl.Collator";
    ani_object collatorObject = VariableConverter::CreateAniObject(env, className, obj.get());
    obj.release();
    return collatorObject;
}

static std::map<std::string, std::string> GetCollatorOptions(ani_env *env, ani_object options)
{
    std::map<std::string, std::string> map;
    ani_boolean isUndefined;
    if (ANI_OK != env->Reference_IsUndefined(options, &isUndefined)) {
        HILOG_ERROR_I18N("Reference IsUndefined failed");
        return map;
    }
    if (!isUndefined) {
        std::vector<std::string> stringKeys = {"localeMatcher", "usage", "sensitivity", "collation", "caseFirst"};
        for (const auto& key : stringKeys) {
            std::string value;
            if (VariableConverter::GetStringMember(env, options, key, value)) {
                map.insert(std::make_pair(key, value));
            }
        }

        bool ignorePunctuation;
        if (VariableConverter::GetBooleanMember(env, options, "ignorePunctuation", ignorePunctuation)) {
            map.insert(std::make_pair("ignorePunctuation", ignorePunctuation ? "true" : "false"));
        }

        bool numeric;
        if (VariableConverter::GetBooleanMember(env, options, "numeric", numeric)) {
            map.insert(std::make_pair("numeric", numeric ? "true" : "false"));
        }
    }
    return map;
}

ani_object IntlAddon::Collator_CreateWithParam(ani_env *env, [[maybe_unused]] ani_object object,
    ani_object locale, ani_object options)
{
    std::vector<std::string> localeTags = VariableConverter::GetLocaleTags(env, locale);
    std::map<std::string, std::string> map = GetCollatorOptions(env, options);

    std::unique_ptr<IntlAddon> obj = std::make_unique<IntlAddon>();
    obj->collator_ = std::make_unique<Collator>(localeTags, map);

    static const char *className = "@ohos.intl.intl.Collator";
    ani_object collatorObject = VariableConverter::CreateAniObject(env, className, obj.get());
    obj.release();
    return collatorObject;
}

ani_object IntlAddon::PluralRules_Create(ani_env *env, [[maybe_unused]] ani_object object)
{
    std::vector<std::string> localeTags;
    std::map<std::string, std::string> map = {};
    std::unique_ptr<IntlAddon> obj = std::make_unique<IntlAddon>();
    obj->pluralrules_ = std::make_unique<PluralRules>(localeTags, map);

    static const char *className = "@ohos.intl.intl.PluralRules";
    ani_object pluralRulesObject = VariableConverter::CreateAniObject(env, className, obj.get());
    obj.release();
    return pluralRulesObject;
}

static std::map<std::string, std::string> GetPluralRulesOptions(ani_env *env, ani_object options)
{
    std::map<std::string, std::string> map;
    ani_boolean isUndefined;
    if (ANI_OK != env->Reference_IsUndefined(options, &isUndefined)) {
        HILOG_ERROR_I18N("Reference IsUndefined failed");
        return map;
    }
    if (!isUndefined) {
        std::vector<std::string> stringKeys = {"localeMatcher", "type"};
        for (const auto& key : stringKeys) {
            std::string value;
            if (VariableConverter::GetStringMember(env, options, key, value)) {
                map.insert(std::make_pair(key, value));
            }
        }

        std::vector<std::pair<std::string, int>> intKeys = {
            {"minimumIntegerDigits", 0}, {"minimumFractionDigits", 0}, {"maximumFractionDigits", 0},
            {"minimumSignificantDigits", 0}, {"maximumSignificantDigits", 0}
        };
        for (auto& [key, value] : intKeys) {
            if (VariableConverter::GetNumberMember(env, options, key, value)) {
                map.insert(std::make_pair(key, std::to_string(value)));
            }
        }
    }
    return map;
}

ani_object IntlAddon::PluralRules_CreateWithParam(ani_env *env, [[maybe_unused]] ani_object object,
    ani_object locale, ani_object options)
{
    std::vector<std::string> localeTags = VariableConverter::GetLocaleTags(env, locale);
    std::map<std::string, std::string> map = GetPluralRulesOptions(env, options);

    std::unique_ptr<IntlAddon> obj = std::make_unique<IntlAddon>();
    obj->pluralrules_ = std::make_unique<PluralRules>(localeTags, map);

    static const char *className = "@ohos.intl.intl.PluralRules";
    ani_object pluralRulesObject = VariableConverter::CreateAniObject(env, className, obj.get());
    obj.release();
    return pluralRulesObject;
}

ani_object IntlAddon::RelativeTimeFormat_Create(ani_env *env, [[maybe_unused]] ani_object object)
{
    std::vector<std::string> localeTags;
    std::map<std::string, std::string> map = {};
    std::unique_ptr<IntlAddon> obj = std::make_unique<IntlAddon>();
    obj->relativetimefmt_ = std::make_unique<RelativeTimeFormat>(localeTags, map);

    static const char *className = "@ohos.intl.intl.RelativeTimeFormat";
    ani_object relativeTimeFormatObject = VariableConverter::CreateAniObject(env, className, obj.get());
    obj.release();
    return relativeTimeFormatObject;
}

static std::map<std::string, std::string> GetRelativeTimeFormatOptions(ani_env *env, ani_object options)
{
    std::map<std::string, std::string> map;
    ani_boolean isUndefined;
    if (ANI_OK != env->Reference_IsUndefined(options, &isUndefined)) {
        HILOG_ERROR_I18N("Reference IsUndefined failed");
        return map;
    }
    if (!isUndefined) {
        std::vector<std::string> stringKeys = {"localeMatcher", "numeric", "style"};
        for (const auto& key : stringKeys) {
            std::string value;
            if (VariableConverter::GetStringMember(env, options, key, value)) {
                map.insert(std::make_pair(key, value));
            }
        }
    }
    return map;
}

ani_object IntlAddon::RelativeTimeFormat_CreateWithParam(ani_env *env, [[maybe_unused]] ani_object object,
    ani_object locale, ani_object options)
{
    std::vector<std::string> localeTags = VariableConverter::GetLocaleTags(env, locale);
    std::map<std::string, std::string> map = GetRelativeTimeFormatOptions(env, options);

    std::unique_ptr<IntlAddon> obj = std::make_unique<IntlAddon>();
    obj->relativetimefmt_= std::make_unique<RelativeTimeFormat>(localeTags, map);

    static const char *className = "@ohos.intl.intl.RelativeTimeFormat";
    ani_object relativeTimeFormatObject = VariableConverter::CreateAniObject(env, className, obj.get());
    obj.release();
    return relativeTimeFormatObject;
}

ani_status IntlAddon::BindContext_DateTimeFormat(ani_env *env)
{
    static const char *className = "@ohos.intl.intl.DateTimeFormat";
    ani_class cls;
    if (ANI_OK != env->FindClass(className, &cls)) {
        HILOG_ERROR_I18N("Find class '%{public}s' failed", className);
        return (ani_status)ANI_ERROR;
    }

    std::array methods = {
        ani_native_function {"format", nullptr, reinterpret_cast<void *>(DateTimeFormat_Format) },
        ani_native_function {"formatRange", nullptr, reinterpret_cast<void *>(DateTimeFormat_FormatRange) },
        ani_native_function {"resolvedOptions", nullptr, reinterpret_cast<void *>(DateTimeFormat_ResolvedOptions) },
    };
    if (ANI_OK != env->Class_BindNativeMethods(cls, methods.data(), methods.size())) {
        HILOG_ERROR_I18N("Cannot bind native methods to '%{public}s'", className);
        return (ani_status)ANI_ERROR;
    }

    std::array staticMethods = {
        ani_native_function {"create", ":C{@ohos.intl.intl.DateTimeFormat}",
            reinterpret_cast<void *>(DateTimeFormat_Create) },
        ani_native_function {
            "create",
            "X{C{std.core.Array}C{std.core.String}}C{@ohos.intl.intl.DateTimeOptions}:"
            "C{@ohos.intl.intl.DateTimeFormat}",
            reinterpret_cast<void *>(DateTimeFormat_CreateWithParam)
        },
    };
    if (ANI_OK != env->Class_BindStaticNativeMethods(cls, staticMethods.data(), staticMethods.size())) {
        HILOG_ERROR_I18N("Cannot bind static native methods to '%{public}s'", className);
        return (ani_status)ANI_ERROR;
    }
    return ANI_OK;
}

ani_status IntlAddon::BindContext_Collator(ani_env *env)
{
    static const char *className = "@ohos.intl.intl.Collator";
    ani_class cls;
    if (ANI_OK != env->FindClass(className, &cls)) {
        HILOG_ERROR_I18N("Find class '%{public}s' failed", className);
        return (ani_status)ANI_ERROR;
    }

    std::array methods = {
        ani_native_function {"compare", nullptr, reinterpret_cast<void *>(Collator_Compare) },
        ani_native_function {"resolvedOptions", nullptr, reinterpret_cast<void *>(Collator_ResolvedOptions) },
    };
    if (ANI_OK != env->Class_BindNativeMethods(cls, methods.data(), methods.size())) {
        HILOG_ERROR_I18N("Cannot bind native methods to '%{public}s'", className);
        return (ani_status)ANI_ERROR;
    }

    std::array staticMethods = {
        ani_native_function {"create", ":C{@ohos.intl.intl.Collator}", reinterpret_cast<void *>(Collator_Create) },
        ani_native_function {
            "create",
            "X{C{std.core.String}C{std.core.Array}}C{@ohos.intl.intl.CollatorOptions}:C{@ohos.intl.intl.Collator}",
            reinterpret_cast<void *>(Collator_CreateWithParam)
        },
    };
    if (ANI_OK != env->Class_BindStaticNativeMethods(cls, staticMethods.data(), staticMethods.size())) {
        HILOG_ERROR_I18N("Cannot bind static native methods to '%{public}s'", className);
        return (ani_status)ANI_ERROR;
    }
    return ANI_OK;
}

ani_status IntlAddon::BindContext_PluralRules(ani_env *env)
{
    static const char *className = "@ohos.intl.intl.PluralRules";
    ani_class cls;
    if (ANI_OK != env->FindClass(className, &cls)) {
        HILOG_ERROR_I18N("Find class '%{public}s' failed", className);
        return (ani_status)ANI_ERROR;
    }

    std::array methods = {
        ani_native_function {"select", nullptr, reinterpret_cast<void *>(PluralRules_Select) },
    };
    if (ANI_OK != env->Class_BindNativeMethods(cls, methods.data(), methods.size())) {
        HILOG_ERROR_I18N("Cannot bind native methods to '%{public}s'", className);
        return (ani_status)ANI_ERROR;
    }

    std::array staticMethods = {
        ani_native_function {"create",
            ":C{@ohos.intl.intl.PluralRules}",
            reinterpret_cast<void *>(PluralRules_Create) },
        ani_native_function {
            "create",
            "X{C{std.core.Array}C{std.core.String}}C{@ohos.intl.intl.PluralRulesOptions}:"
            "C{@ohos.intl.intl.PluralRules}",
            reinterpret_cast<void *>(PluralRules_CreateWithParam)
        },
    };
    if (ANI_OK != env->Class_BindStaticNativeMethods(cls, staticMethods.data(), staticMethods.size())) {
        HILOG_ERROR_I18N("Cannot bind static native methods to '%{public}s'", className);
        return (ani_status)ANI_ERROR;
    }
    return ANI_OK;
}

ani_status IntlAddon::BindContext_RelativeTimeFormat(ani_env *env)
{
    static const char *className = "@ohos.intl.intl.RelativeTimeFormat";
    ani_class cls;
    if (ANI_OK != env->FindClass(className, &cls)) {
        HILOG_ERROR_I18N("Find class '%{public}s' failed", className);
        return (ani_status)ANI_ERROR;
    }

    std::array methods = {
        ani_native_function {"format", nullptr, reinterpret_cast<void *>(RelativeTimeFormat_Format) },
        ani_native_function {"formatToParts", nullptr, reinterpret_cast<void *>(RelativeTimeFormat_FormatToParts) },
        ani_native_function {"resolvedOptions", nullptr, reinterpret_cast<void *>(RelativeTimeFormat_ResolvedOptions) },
    };
    if (ANI_OK != env->Class_BindNativeMethods(cls, methods.data(), methods.size())) {
        HILOG_ERROR_I18N("Cannot bind native methods to '%{public}s'", className);
        return (ani_status)ANI_ERROR;
    }

    std::array staticMethods = {
        ani_native_function {"create", ":C{@ohos.intl.intl.RelativeTimeFormat}",
            reinterpret_cast<void *>(RelativeTimeFormat_Create) },
        ani_native_function {
            "create",
            "X{C{std.core.Array}C{std.core.String}}C{@ohos.intl.intl.RelativeTimeFormatInputOptions}:"
            "C{@ohos.intl.intl.RelativeTimeFormat}",
            reinterpret_cast<void *>(RelativeTimeFormat_CreateWithParam)
        },
    };
    if (ANI_OK != env->Class_BindStaticNativeMethods(cls, staticMethods.data(), staticMethods.size())) {
        HILOG_ERROR_I18N("Cannot bind static native methods to '%{public}s'", className);
        return (ani_status)ANI_ERROR;
    }
    return ANI_OK;
}

ANI_EXPORT ani_status ANI_Constructor(ani_vm *vm, uint32_t *result)
{
    ani_env *env;
    if (ANI_OK != vm->GetEnv(ANI_VERSION_1, &env)) {
        HILOG_ERROR_I18N("Unsupported ANI_VERSION_1");
        return (ani_status)ANI_ERROR;
    }

    auto status = LocaleInfoAddon::BindContext_Locale(env);
    if (status != ANI_OK) {
        return status;
    }

    status = IntlAddon::BindContext_DateTimeFormat(env);
    if (status != ANI_OK) {
        return status;
    }

    status = NumberFormatAddon::BindContext_NumberFormat(env);
    if (status != ANI_OK) {
        return status;
    }

    status = IntlAddon::BindContext_Collator(env);
    if (status != ANI_OK) {
        return status;
    }

    status = IntlAddon::BindContext_PluralRules(env);
    if (status != ANI_OK) {
        return status;
    }

    status = IntlAddon::BindContext_RelativeTimeFormat(env);
    if (status != ANI_OK) {
        return status;
    }
    *result = ANI_VERSION_1;
    return ANI_OK;
}
