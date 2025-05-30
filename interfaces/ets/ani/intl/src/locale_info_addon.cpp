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

#include "locale_info_addon.h"

#include "i18n_hilog.h"
#include "variable_convertor.h"

using namespace OHOS;
using namespace Global;
using namespace I18n;

LocaleInfoAddon* LocaleInfoAddon::UnwrapAddon(ani_env *env, ani_object object, const std::string name)
{
    ani_long ptr;
    if (ANI_OK != env->Object_GetFieldByName_Long(object, name.c_str(), &ptr)) {
        HILOG_ERROR_I18N("Get Long '%{public}s' failed", name.c_str());
        return nullptr;
    }
    return reinterpret_cast<LocaleInfoAddon*>(ptr);
}

template<typename T>
T LocaleInfoAddon::GetLocaleProperty(ani_env *env, ani_object object,
    const std::string& propertyName, T (LocaleInfo::*getter)() const)
{
    LocaleInfoAddon* obj = UnwrapAddon(env, object, "nativeLocale");
    if (obj == nullptr || obj->locale_ == nullptr) {
        HILOG_ERROR_I18N("%{public}s: Get Locale object failed", propertyName.c_str());
        return T{};
    }
    return (obj->locale_.get()->*getter)();
}

ani_string LocaleInfoAddon::Locale_toString(ani_env *env, ani_object object)
{
    std::string value = GetLocaleProperty(env, object, "toString", &LocaleInfo::ToString);
    return VariableConvertor::StringToAniStr(env, value);
}

ani_object LocaleInfoAddon::Locale_maximize(ani_env *env, ani_object object)
{
    LocaleInfoAddon* obj = UnwrapAddon(env, object, "nativeLocale");
    if (obj == nullptr || obj->locale_ == nullptr) {
        HILOG_ERROR_I18N("Maximize: Get Locale object failed");
        return nullptr;
    }

    std::string localeTag = obj->locale_->Maximize();
    return Locale_createWithParam(env, nullptr, VariableConvertor::StringToAniStr(env, localeTag), nullptr);
}

ani_object LocaleInfoAddon::Locale_minimize(ani_env *env, ani_object object)
{
    LocaleInfoAddon* obj = UnwrapAddon(env, object, "nativeLocale");
    if (obj == nullptr || obj->locale_ == nullptr) {
        HILOG_ERROR_I18N("Minimize: Get Locale object failed");
        return nullptr;
    }

    std::string localeTag = obj->locale_->Minimize();
    return Locale_createWithParam(env, nullptr, VariableConvertor::StringToAniStr(env, localeTag), nullptr);
}

ani_string LocaleInfoAddon::Locale_getLanguage(ani_env *env, ani_object object)
{
    std::string value = GetLocaleProperty(env, object, "getLanguage", &LocaleInfo::GetLanguage);
    return VariableConvertor::StringToAniStr(env, value);
}

ani_string LocaleInfoAddon::Locale_getScript(ani_env *env, ani_object object)
{
    std::string value = GetLocaleProperty(env, object, "getScript", &LocaleInfo::GetScript);
    return VariableConvertor::StringToAniStr(env, value);
}

ani_string LocaleInfoAddon::Locale_getRegion(ani_env *env, ani_object object)
{
    std::string value = GetLocaleProperty(env, object, "getRegion", &LocaleInfo::GetRegion);
    return VariableConvertor::StringToAniStr(env, value);
}

ani_string LocaleInfoAddon::Locale_getBaseName(ani_env *env, ani_object object)
{
    std::string value = GetLocaleProperty(env, object, "getBaseName", &LocaleInfo::GetBaseName);
    return VariableConvertor::StringToAniStr(env, value);
}

ani_string LocaleInfoAddon::Locale_getCaseFirst(ani_env *env, ani_object object)
{
    std::string value = GetLocaleProperty(env, object, "getCaseFirst", &LocaleInfo::GetCaseFirst);
    return VariableConvertor::StringToAniStr(env, value);
}

ani_string LocaleInfoAddon::Locale_getCalendar(ani_env *env, ani_object object)
{
    std::string value = GetLocaleProperty(env, object, "getCalendar", &LocaleInfo::GetCalendar);
    return VariableConvertor::StringToAniStr(env, value);
}

ani_string LocaleInfoAddon::Locale_getCollation(ani_env *env, ani_object object)
{
    std::string value = GetLocaleProperty(env, object, "getCollation", &LocaleInfo::GetCollation);
    return VariableConvertor::StringToAniStr(env, value);
}

ani_string LocaleInfoAddon::Locale_getHourCycle(ani_env *env, ani_object object)
{
    std::string value = GetLocaleProperty(env, object, "getHourCycle", &LocaleInfo::GetHourCycle);
    return VariableConvertor::StringToAniStr(env, value);
}

ani_string LocaleInfoAddon::Locale_getNumberingSystem(ani_env *env, ani_object object)
{
    std::string value = GetLocaleProperty(env, object, "getNumberingSystem", &LocaleInfo::GetNumberingSystem);
    return VariableConvertor::StringToAniStr(env, value);
}

ani_boolean LocaleInfoAddon::Locale_getNumeric(ani_env *env, ani_object object)
{
    LocaleInfoAddon* obj = UnwrapAddon(env, object, "nativeLocale");
    if (obj == nullptr || obj->locale_ == nullptr) {
        HILOG_ERROR_I18N("getNumeric: Get Locale object failed");
        return false;
    }
    return (obj->locale_->GetNumeric() == "true");
}

ani_object LocaleInfoAddon::Locale_create(ani_env *env, [[maybe_unused]] ani_class clazz)
{
    std::map<std::string, std::string> map = {};
    std::unique_ptr<LocaleInfoAddon> obj = std::make_unique<LocaleInfoAddon>();
    obj->locale_ = std::make_unique<LocaleInfo>("", map);

    static const char *className = "L@ohos/intl/intl/Locale;";
    ani_object locale_object = VariableConvertor::CreateAniObject(env, className, obj.get());
    obj.release();
    return locale_object;
}

ani_object LocaleInfoAddon::Locale_createWithParam(ani_env *env, [[maybe_unused]] ani_class clazz,
    ani_string locale, ani_object options)
{
    std::map<std::string, std::string> map = {};
    ani_boolean isUndefined;
    if (ANI_OK != env->Reference_IsUndefined(options, &isUndefined)) {
        HILOG_ERROR_I18N("Reference IsUndefined failed");
        return nullptr;
    }
    if (!isUndefined) {
        const std::vector<std::string> keys = {
            "calendar", "collation", "hourCycle", "numberingSystem", "caseFirst"
        };

        for (const auto& key : keys) {
            std::string value;
            if (VariableConvertor::GetStringMember(env, options, key, value)) {
                map.insert(std::make_pair(key, value));
            }
        }

        bool numeric;
        if (VariableConvertor::GetBooleanMember(env, options, "numeric", numeric)) {
            map.insert(std::make_pair("numeric", numeric ? "true" : "false"));
        }
    }

    std::unique_ptr<LocaleInfoAddon> obj = std::make_unique<LocaleInfoAddon>();
    obj->locale_ = std::make_unique<LocaleInfo>(VariableConvertor::AniStrToString(env, locale), map);
    static const char *className = "L@ohos/intl/intl/Locale;";
    ani_object locale_object = VariableConvertor::CreateAniObject(env, className, obj.get());
    obj.release();
    return locale_object;
}

ani_status LocaleInfoAddon::BindContext_Locale(ani_env *env)
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
