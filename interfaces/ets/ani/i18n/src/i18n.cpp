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
#include "i18n.h"
#include "locale_config.h"

using namespace OHOS;
using namespace Global;
using namespace I18n;

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

static std::string ANIStringToString(ani_env *env, ani_string ani_str)
{
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

I18nCalendarAddon* I18nCalendarAddon::unwrapAddon(ani_env *env, ani_object object)
{
    ani_long ptr;
    if (ANI_OK != env->Object_GetFieldByName_Long(object, "nativeCalendar", &ptr)) {
        return nullptr;
    }
    return reinterpret_cast<I18nCalendarAddon*>(ptr);
}

static ani_string getSystemLanguage([[maybe_unused]] ani_env *env, [[maybe_unused]] ani_object object)
{
    std::string str = LocaleConfig::GetSystemLanguage();
    ani_string ret;
    env->String_NewUTF8(str.c_str(), str.size(), &ret);
    return ret;
}

static ani_string getSystemLocale([[maybe_unused]] ani_env *env, [[maybe_unused]] ani_object object)
{
    std::string str = LocaleConfig::GetSystemLocale();
    ani_string ret;
    env->String_NewUTF8(str.c_str(), str.size(), &ret);
    return ret;
}

static ani_boolean is24HourClock([[maybe_unused]] ani_env *env, [[maybe_unused]] ani_object object)
{
    return LocaleConfig::Is24HourClock();
}

static ani_string getSystemRegion([[maybe_unused]] ani_env *env, [[maybe_unused]] ani_object object)
{
    std::string str = LocaleConfig::GetSystemRegion();
    ani_string ret;
    env->String_NewUTF8(str.c_str(), str.size(), &ret);
    return ret;
}

static ani_boolean unicodeIsRTL([[maybe_unused]] ani_env *env, [[maybe_unused]] ani_object object, ani_string ch)
{
    return IsRTLCharacter(ANIStringToString(env, ch));
}

ani_string I18nCalendarAddon::CalendarGetTimeZone([[maybe_unused]] ani_env *env, [[maybe_unused]] ani_object object)
{
    I18nCalendarAddon* obj = unwrapAddon(env, object);
    if (obj == nullptr || obj->calendar_ == nullptr) {
        return nullptr;
    }
    std::string str = obj->calendar_->GetTimeZone();
    ani_string ret;
    env->String_NewUTF8(str.c_str(), str.size(), &ret);
    return ret;
}

ani_double I18nCalendarAddon::Get([[maybe_unused]] ani_env *env, [[maybe_unused]] ani_object object,
    ani_string field)
{
    std::string str = ANIStringToString(env, field);
    if (g_fieldsMap.find(str) == g_fieldsMap.end()) {
        return 0;
    }

    I18nCalendarAddon* obj = unwrapAddon(env, object);
    if (obj == nullptr || obj->calendar_ == nullptr) {
        return -1;
    }
    return obj->calendar_->Get(g_fieldsMap[str]);
}

ani_object I18nCalendarAddon::getCalendar([[maybe_unused]] ani_env *env, ani_string locale, ani_string type)
{
    std::unique_ptr<I18nCalendarAddon> obj = std::make_unique<I18nCalendarAddon>();
    ani_boolean isUndefined;
    env->Reference_IsUndefined(type, &isUndefined);
    CalendarType calendartype = CalendarType::UNDEFINED;
    if (!isUndefined) {
        std::string typeStr = ANIStringToString(env, type);
        if (g_typeMap.find(typeStr) != g_typeMap.end()) {
            calendartype = g_typeMap[typeStr];
        }
    }
    obj->calendar_ = std::make_unique<I18nCalendar>(ANIStringToString(env, locale), calendartype);
    static const char* className = "L@ohos/i18n/i18n/Calendar;";
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
    ani_object calendar_object;
    if (ANI_OK != env->Object_New(cls, ctor, &calendar_object, reinterpret_cast<ani_long>(obj.get()))) {
        std::cerr << "New calendar Fail" << std::endl;
    }
    obj.release();
    return calendar_object;
}

static ani_boolean i18nIsRTL(ani_env *env, ani_string locale)
{
    return LocaleConfig::IsRTL(ANIStringToString(env, locale));
}

ani_object I18nTimeZoneAddon::i18nGetTimeZone([[maybe_unused]] ani_env *env, [[maybe_unused]] ani_object object,
    ani_string zoneID)
{
    std::unique_ptr<I18nTimeZoneAddon> obj = std::make_unique<I18nTimeZoneAddon>();
    ani_boolean isUndefined;
    env->Reference_IsUndefined(zoneID, &isUndefined);
    std::string zoneIDStr = "";
    if (!isUndefined) {
        zoneIDStr = ANIStringToString(env, zoneID);
    }
    obj->timezone_ = I18nTimeZone::CreateInstance(zoneIDStr, isUndefined);

    static const char* className = "L@ohos/i18n/i18n/TimeZone;";
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
    ani_object timeZone_object;
    if (ANI_OK != env->Object_New(cls, ctor, &timeZone_object, reinterpret_cast<ani_long>(obj.get()))) {
        std::cerr << "New timeZone Fail" << std::endl;
    }
    obj.release();
    return timeZone_object;
}

static ani_status BindContextSystem(ani_env *env)
{
    static const char *className = "L@ohos/i18n/i18n/System;";
    ani_class cls;
    if (ANI_OK != env->FindClass(className, &cls)) {
        std::cerr << "Not found '" << className << "'" << std::endl;
        return (ani_status)ANI_ERROR;
    }

    std::array methods = {
        ani_native_function { "getSystemLanguage", ":Lstd/core/String;", reinterpret_cast<void *>(getSystemLanguage) },
        ani_native_function { "getSystemLocale", ":Lstd/core/String;", reinterpret_cast<void *>(getSystemLocale) },
        ani_native_function { "is24HourClock", ":Z", reinterpret_cast<void *>(is24HourClock) },
        ani_native_function { "getSystemRegion", ":Lstd/core/String;", reinterpret_cast<void *>(getSystemRegion) },
    };

    if (ANI_OK != env->Class_BindNativeMethods(cls, methods.data(), methods.size())) {
        std::cerr << "Cannot bind native methods to '" << className << "'" << std::endl;
        return (ani_status)ANI_ERROR;
    };

    return ANI_OK;
}

static ani_status BindContextCalendar(ani_env *env)
{
    static const char *className = "L@ohos/i18n/i18n/Calendar;";
    ani_class cls;
    if (ANI_OK != env->FindClass(className, &cls)) {
        std::cerr << "Not found '" << className << "'" << std::endl;
        return (ani_status)ANI_ERROR;
    }

    std::array methods = {
        ani_native_function { "getTimeZone", ":Lstd/core/String;",
            reinterpret_cast<void *>(I18nCalendarAddon::CalendarGetTimeZone) },
        ani_native_function { "get", "Lstd/core/String;:D",
            reinterpret_cast<void *>(I18nCalendarAddon::Get) },
    };

    if (ANI_OK != env->Class_BindNativeMethods(cls, methods.data(), methods.size())) {
        std::cerr << "Cannot bind native methods to '" << className << "'" << std::endl;
        return (ani_status)ANI_ERROR;
    };

    return ANI_OK;
}

static ani_status BindContextUnicode(ani_env *env)
{
    static const char *className = "L@ohos/i18n/i18n/Unicode;";
    ani_class cls;
    if (ANI_OK != env->FindClass(className, &cls)) {
        std::cerr << "Not found '" << className << "'" << std::endl;
        return (ani_status)ANI_ERROR;
    }

    std::array methods = {
        ani_native_function { "isRTL", "Lstd/core/String;:Z", reinterpret_cast<void *>(unicodeIsRTL) },
    };

    if (ANI_OK != env->Class_BindNativeMethods(cls, methods.data(), methods.size())) {
        std::cerr << "Cannot bind native methods to '" << className << "'" << std::endl;
        return (ani_status)ANI_ERROR;
    };

    return ANI_OK;
}

static ani_status BindContextNameSpace(ani_env *env)
{
    static const char* nameSpaceName = "L@ohos/i18n/i18n;";
    ani_namespace ns;
    if (ANI_OK != env->FindNamespace(nameSpaceName, &ns)) {
        std::cerr << "Not found '" << nameSpaceName << "'" << std::endl;
        return (ani_status)ANI_ERROR;
    }
    std::array methods = {
        ani_native_function { "getCalendar", "Lstd/core/String;Lstd/core/String;:L@ohos/i18n/i18n/Calendar;",
            reinterpret_cast<void *>(I18nCalendarAddon::getCalendar) },
        ani_native_function { "isRTL", "Lstd/core/String;:Z", reinterpret_cast<void *>(i18nIsRTL) },
        ani_native_function { "getTimeZone", "Lstd/core/String;:L@ohos/i18n/i18n/TimeZone;",
            reinterpret_cast<void *>(I18nTimeZoneAddon::i18nGetTimeZone) },
    };
    if (ANI_OK != env->Namespace_BindNativeFunctions(ns, methods.data(), methods.size())) {
        std::cerr << "Cannot bind native methods to '" << nameSpaceName << "'" << std::endl;
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
    
    auto status = BindContextSystem(env);
    if (status != ANI_OK) {
        return status;
    }

    status = BindContextCalendar(env);
    if (status != ANI_OK) {
        return status;
    }

    status = BindContextUnicode(env);
    if (status != ANI_OK) {
        return status;
    }

    status = BindContextNameSpace(env);
    if (status != ANI_OK) {
        return status;
    }
    *result = ANI_VERSION_1;
    return ANI_OK;
}