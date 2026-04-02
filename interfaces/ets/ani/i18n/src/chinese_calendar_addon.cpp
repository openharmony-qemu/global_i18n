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

#include "chinese_calendar_addon.h"
#include <unordered_map>
#include <unordered_set>
#include "i18n_hilog.h"
#include "variable_converter.h"

using namespace OHOS;
using namespace Global;
using namespace I18n;
ChineseCalendarAddon* ChineseCalendarAddon::UnwrapAddon(ani_env *env, ani_object object)
{
    ani_long ptr;
    if (ANI_OK != env->Object_GetFieldByName_Long(object, "nativeChineseCalendar", &ptr)) {
        HILOG_ERROR_I18N("ChineseCalendarAddon::UnwrapAddon: Get field nativeChineseCalendar failed");
        return nullptr;
    }
    return reinterpret_cast<ChineseCalendarAddon*>(ptr);
}

ani_object ChineseCalendarAddon::GetChineseCalendar(ani_env *env, ani_string locale)
{
    std::unique_ptr<ChineseCalendarAddon> obj = std::make_unique<ChineseCalendarAddon>();
    obj->chineseCalendar_ = std::make_unique<ChineseCalendar>(VariableConverter::AniStrToString(env, locale));
    static const char* className = "@ohos.i18n.i18n.ChineseCalendar";
    ani_object calendarObject = VariableConverter::CreateAniObject(env, className, obj.get());
    obj.release();
    return calendarObject;
}

ani_boolean ChineseCalendarAddon::CheckLeapMonth(ani_env *env, ani_int year, ani_int month)
{
    return ChineseCalendar::CheckLeapMonth(static_cast<int32_t>(year), static_cast<int32_t>(month));
}

void ChineseCalendarAddon::SetChineseCalendarTime(ani_env *env, ani_object object, ani_object chineseCalendarTime)
{
    ChineseCalendarAddon* obj = UnwrapAddon(env, object);
    if (obj == nullptr || obj->chineseCalendar_ == nullptr) {
        HILOG_ERROR_I18N("ChineseCalendarAddon::SetChineseCalendarTime: Get calendar object failed");
        return;
    }

    ChineseCalendarTime time;

    std::unordered_set<std::string> keyOfTypeNumber = { "year", "month", "day", "hour", "minute", "second" };
    std::string valueOfKey;
    for (const auto& key : keyOfTypeNumber) {
        ani_int value = 0;;
        if (ANI_OK != env->Object_GetFieldByName_Int(chineseCalendarTime, key, &value)) {
            continue;
        }
        auto iter = ChineseCalendarTime::MEMBER_NAME_MAP.find(key);
        if (iter != ChineseCalendarTime::MEMBER_NAME_MAP.end()) {
            time.*(iter->second) = value;
        }
    }

    ani_boolean isLeapMonth = false;
    if (ANI_OK != env->Object_GetFieldByName_Boolean(chineseCalendarTime, "isLeapMonth", &time.isLeapMonth)) {
        time.isLeapMonth = false;
    }

    obj->chineseCalendar_->SetChineseCalendarTime(time);
}

void ChineseCalendarAddon::SetTimeByDate(ani_env *env, ani_object object, ani_object date)
{
    ChineseCalendarAddon* obj = UnwrapAddon(env, object);
    if (obj == nullptr || obj->chineseCalendar_ == nullptr) {
        HILOG_ERROR_I18N("ChineseCalendarAddon::SetTimeByDate: Get calendar object failed");
        return;
    }
    double milliseconds;
    if (ANI_OK != env->Object_CallMethodByName_Double(date, "toDouble", ":d", &milliseconds)) {
         std::chrono::milliseconds ms = std::chrono::duration_cast<std::chrono::milliseconds>(
            std::chrono::system_clock::now().time_since_epoch()
        );
        milliseconds = ms.count();
    }
    obj->chineseCalendar_->SetTime(millisecond);
}

void ChineseCalendarAddon::SetTime(ani_env *env, ani_object object, ani_double time)
{
    ChineseCalendarAddon* obj = UnwrapAddon(env, object);
    if (obj == nullptr || obj->chineseCalendar_ == nullptr) {
        HILOG_ERROR_I18N("ChineseCalendarAddon::SetTime: Get calendar object failed");
        return;
    }
    obj->chineseCalendar_->SetTime(time);
}

void ChineseCalendarAddon::Set(ani_env *env, ani_object object, ani_int year, ani_int month,
    ani_int date, ani_object hour, ani_object minute, ani_object second)
{
    ChineseCalendarAddon* obj = UnwrapAddon(env, object);
    if (obj == nullptr || obj->chineseCalendar_ == nullptr) {
        HILOG_ERROR_I18N("ChineseCalendarAddon::Set: Get calendar object failed");
        return;
    }
    obj->chineseCalendar_->Set(year, month, date);

    ani_int hourValue = 0;
    ani_boolean isUndefined;
    if (ANI_OK != env->Reference_IsUndefined(hour, &isUndefined)) {
        HILOG_ERROR_I18N("ChineseCalendarAddon::Set: Reference_IsUndefined failed");
        return;
    }
    if (!isUndefined && ANI_OK != env->Object_CallMethodByName_Int(hour, "toInt", ":i", &hourValue)) {
        HILOG_ERROR_I18N("ChineseCalendarAddon::Set: Get hour failed");
        return;
    }
    obj->chineseCalendar_->Set(UCalendarDateFields::UCAL_HOUR_OF_DAY, hourValue);

    ani_int minuteValue = 0;
    if (ANI_OK != env->Reference_IsUndefined(minute, &isUndefined)) {
        HILOG_ERROR_I18N("ChineseCalendarAddon::Set: Reference_IsUndefined failed");
        return;
    }
    if (!isUndefined && ANI_OK != env->Object_CallMethodByName_Int(minute, "toInt", ":i", &minuteValue)) {
        HILOG_ERROR_I18N("ChineseCalendarAddon::Set: Get minute failed");
        return;
    }
    obj->chineseCalendar_->Set(UCalendarDateFields::UCAL_MINUTE, minuteValue);

    ani_int secondValue = 0;
    if (ANI_OK != env->Reference_IsUndefined(second, &isUndefined)) {
        HILOG_ERROR_I18N("ChineseCalendarAddon::Set: Reference_IsUndefined failed");
        return;
    }
    if (!isUndefined && ANI_OK != env->Object_CallMethodByName_Int(second, "toInt", ":i", &secondValue)) {
        HILOG_ERROR_I18N("ChineseCalendarAddon::Set: Get second failed");
        return;
    }
    obj->chineseCalendar_->Set(UCalendarDateFields::UCAL_SECOND, secondValue);
}

void ChineseCalendarAddon::SetTimeZone(ani_env *env, ani_object object, ani_string timezone)
{
    ChineseCalendarAddon* obj = UnwrapAddon(env, object);
    if (obj == nullptr || obj->chineseCalendar_ == nullptr) {
        HILOG_ERROR_I18N("ChineseCalendarAddon::SetTimeZone: Get calendar object failed");
        return;
    }
    obj->chineseCalendar_->SetTimeZone(VariableConverter::AniStrToString(env, timezone));
}

ani_string ChineseCalendarAddon::GetTimeZone(ani_env *env, ani_object object)
{
    ChineseCalendarAddon* obj = UnwrapAddon(env, object);
    if (obj == nullptr || obj->chineseCalendar_ == nullptr) {
        HILOG_ERROR_I18N("ChineseCalendarAddon::GetTimeZone: Get calendar object failed");
        return nullptr;
    }
    std::string timezone = obj->chineseCalendar_->GetTimeZone();
    return VariableConverter::StringToAniStr(env, timezone);
}

ani_int ChineseCalendarAddon::GetFirstDayOfWeek(ani_env *env, ani_object object)
{
    ChineseCalendarAddon* obj = UnwrapAddon(env, object);
    if (obj == nullptr || obj->chineseCalendar_ == nullptr) {
        HILOG_ERROR_I18N("ChineseCalendarAddon::GetFirstDayOfWeek: Get calendar object failed");
        return -1;
    }
    return obj->chineseCalendar_->GetFirstDayOfWeek();
}

void ChineseCalendarAddon::SetFirstDayOfWeek(ani_env *env, ani_object object, ani_int value)
{
    ChineseCalendarAddon* obj = UnwrapAddon(env, object);
    if (obj == nullptr || obj->chineseCalendar_ == nullptr) {
        HILOG_ERROR_I18N("ChineseCalendarAddon::SetFirstDayOfWeek: Get calendar object failed");
        return;
    }
    obj->chineseCalendar_->SetFirstDayOfWeek(value);
}

ani_int ChineseCalendarAddon::GetMinimalDaysInFirstWeek(ani_env *env, ani_object object)
{
    ChineseCalendarAddon* obj = UnwrapAddon(env, object);
    if (obj == nullptr || obj->chineseCalendar_ == nullptr) {
        HILOG_ERROR_I18N("ChineseCalendarAddon::GetMinimalDaysInFirstWeek: Get calendar object failed");
        return -1;
    }
    return obj->chineseCalendar_->GetMinimalDaysInFirstWeek();
}

void ChineseCalendarAddon::SetMinimalDaysInFirstWeek(ani_env *env, ani_object object, ani_int value)
{
    ChineseCalendarAddon* obj = UnwrapAddon(env, object);
    if (obj == nullptr || obj->chineseCalendar_ == nullptr) {
        HILOG_ERROR_I18N("ChineseCalendarAddon::SetMinimalDaysInFirstWeek: Get calendar object failed");
        return;
    }
    obj->chineseCalendar_->SetMinimalDaysInFirstWeek(value);
}

ani_int ChineseCalendarAddon::Get(ani_env *env, ani_object object, ani_string field)
{
    std::string str = VariableConverter::AniStrToString(env, field);
    if (I18nCalendar::FIELDS_MAP.find(str) == I18nCalendar::FIELDS_MAP.end()) {
        return -1;
    }

    ChineseCalendarAddon* obj = UnwrapAddon(env, object);
    if (obj == nullptr || obj->chineseCalendar_ == nullptr) {
        HILOG_ERROR_I18N("ChineseCalendarAddon::Get: Get calendar object failed");
        return -1;
    }
    return obj->chineseCalendar_->Get(I18nCalendar::FIELDS_MAP[str]);
}

ani_string ChineseCalendarAddon::GetDisplayName(ani_env *env, ani_object object, ani_string locale)
{
    ChineseCalendarAddon* obj = UnwrapAddon(env, object);
    if (obj == nullptr || obj->chineseCalendar_ == nullptr) {
        HILOG_ERROR_I18N("ChineseCalendarAddon::GetDisplayName: Get calendar object failed");
        return VariableConverter::StringToAniStr(env, "");
    }
    std::string localeTag = VariableConverter::AniStrToString(env, locale);
    std::string name = obj->chineseCalendar_->GetDisplayName(localeTag);
    return VariableConverter::StringToAniStr(env, name);
}

ani_boolean ChineseCalendarAddon::IsWeekend(ani_env *env, ani_object object, ani_object date)
{
    ChineseCalendarAddon* obj = UnwrapAddon(env, object);
    if (obj == nullptr || obj->chineseCalendar_ == nullptr) {
        HILOG_ERROR_I18N("ChineseCalendarAddon::IsWeekend: Get calendar object failed");
        return false;
    }

    ani_boolean isUndefined;
    if (ANI_OK != env->Reference_IsUndefined(date, &isUndefined)) {
        return false;
    }
    if (isUndefined) {
        return obj->chineseCalendar_->IsWeekend();
    }
    double milliseconds;
    if (ANI_OK != env->Object_CallMethodByName_Double(date, "toDouble", ":d", &milliseconds)) {
        std::chrono::milliseconds ms = std::chrono::duration_cast<std::chrono::milliseconds>(
            std::chrono::system_clock::now().time_since_epoch()
        );
        milliseconds = ms.count();
    }
    UErrorCode error = U_ZERO_ERROR;
    return obj->chineseCalendar_->IsWeekend(millisecond, error);
}

void ChineseCalendarAddon::Add(ani_env *env, ani_object object, ani_string field, ani_int amount)
{
    std::string str = VariableConverter::AniStrToString(env, field);
    if (I18nCalendar::FIELDS_IN_FUNCTION_ADD.find(str) == I18nCalendar::FIELDS_IN_FUNCTION_ADD.end() ||
        I18nCalendar::FIELDS_MAP.find(str) == I18nCalendar::FIELDS_MAP.end()) {
        return;
    }

    ChineseCalendarAddon* obj = UnwrapAddon(env, object);
    if (obj == nullptr || obj->chineseCalendar_ == nullptr) {
        HILOG_ERROR_I18N("ChineseCalendarAddon::Add: Get calendar object failed");
        return;
    }
    obj->chineseCalendar_->Add(I18nCalendar::FIELDS_MAP[str], amount);
}

ani_long ChineseCalendarAddon::GetTimeInMillis(ani_env *env, ani_object object)
{
    ChineseCalendarAddon* obj = UnwrapAddon(env, object);
    if (obj == nullptr || obj->chineseCalendar_ == nullptr) {
        HILOG_ERROR_I18N("ChineseCalendarAddon::GetTimeInMillis: Get calendar object failed");
        return 0;
    }
    return obj->chineseCalendar_->GetTimeInMillis();
}

ani_int ChineseCalendarAddon::CompareDays(ani_env *env, ani_object object, ani_object date)
{
    ChineseCalendarAddon* obj = UnwrapAddon(env, object);
    if (obj == nullptr || obj->chineseCalendar_ == nullptr) {
        HILOG_ERROR_I18N("ChineseCalendarAddon::CompareDays: Get calendar object failed");
        return 0;
    }
    double milliseconds;
    if (ANI_OK != env->Object_CallMethodByName_Double(date, "toDouble", ":d", &milliseconds)) {
        std::chrono::milliseconds ms = std::chrono::duration_cast<std::chrono::milliseconds>(
            std::chrono::system_clock::now().time_since_epoch()
        );
        milliseconds = ms.count();
    }
    return obj->chineseCalendar_->CompareDays(millisecond);
}

ani_status ChineseCalendarAddon::BindContextChineseCalendar(ani_env *env)
{
    static const char *className = "@ohos.i18n.i18n.ChineseCalendar";
    ani_class cls;
    if (ANI_OK != env->FindClass(className, &cls)) {
        HILOG_ERROR_I18N("BindContextChineseCalendar: Find class '%{public}s' failed", className);
        return ANI_ERROR;
    }

    std::array methods = {
        ani_native_function { "setChineseCalendarTime", nullptr,
            reinterpret_cast<void *>(ChineseCalendarAddon::SetChineseCalendarTime) },
        ani_native_function { "setTime", "C{std.core.Date}:",
            reinterpret_cast<void *>(ChineseCalendarAddon::SetTimeByDate) },
        ani_native_function { "setTime", "d:", reinterpret_cast<void *>(ChineseCalendarAddon::SetTime) },
        ani_native_function { "set", nullptr, reinterpret_cast<void *>(ChineseCalendarAddon::Set) },
        ani_native_function { "setTimeZone", nullptr, reinterpret_cast<void *>(ChineseCalendarAddon::SetTimeZone) },
        ani_native_function { "getTimeZone", nullptr, reinterpret_cast<void *>(ChineseCalendarAddon::GetTimeZone) },
        ani_native_function { "getFirstDayOfWeek", nullptr,
            reinterpret_cast<void *>(ChineseCalendarAddon::GetFirstDayOfWeek) },
        ani_native_function { "setFirstDayOfWeek", nullptr,
            reinterpret_cast<void *>(ChineseCalendarAddon::SetFirstDayOfWeek) },
        ani_native_function { "getMinimalDaysInFirstWeek", nullptr,
            reinterpret_cast<void *>(ChineseCalendarAddon::GetMinimalDaysInFirstWeek) },
        ani_native_function { "setMinimalDaysInFirstWeek", nullptr,
            reinterpret_cast<void *>(ChineseCalendarAddon::SetMinimalDaysInFirstWeek) },
        ani_native_function { "get", nullptr, reinterpret_cast<void *>(ChineseCalendarAddon::Get) },
        ani_native_function { "getDisplayName", nullptr, reinterpret_cast<void *>(ChineseCalendarAddon::GetDisplayName) },
        ani_native_function { "isWeekend", nullptr, reinterpret_cast<void *>(ChineseCalendarAddon::IsWeekend) },
        ani_native_function { "add", nullptr, reinterpret_cast<void *>(ChineseCalendarAddon::Add) },
        ani_native_function { "getTimeInMillis", nullptr,
            reinterpret_cast<void *>(ChineseCalendarAddon::GetTimeInMillis) },
        ani_native_function { "compareDays", nullptr, reinterpret_cast<void *>(ChineseCalendarAddon::CompareDays) },
    };

    if (ANI_OK != env->Class_BindNativeMethods(cls, methods.data(), methods.size())) {
        HILOG_ERROR_I18N("BindContextChineseCalendar: Cannot bind native methods to '%{public}s'", className);
        return ANI_ERROR;
    };

    std::array staticMethods = {
        ani_native_function { "checkLeapMonth", nullptr, reinterpret_cast<void *>(CheckLeapMonth) },
    };

    if (ANI_OK != env->Class_BindStaticNativeMethods(cls, staticMethods.data(), staticMethods.size())) {
        HILOG_ERROR_I18N("BindContextI18NUtil: Cannot bind native static methods to '%{public}s'", className);
        return ANI_ERROR;
    };

    return ANI_OK;
}