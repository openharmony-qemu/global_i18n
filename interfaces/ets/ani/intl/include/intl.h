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

#ifndef INTL_ANI_H
#define INTL_ANI_H

#include <ani.h>
#include <iostream>
#include <array>

#include "date_time_format.h"
#include "locale_info.h"
#include "number_format.h"

namespace OHOS {
namespace Global {
namespace I18n {
class IntlAniAddon {
public:
    template<typename T>
    static T GetLocaleProperty(ani_env *env, ani_object object,
        const std::string& propertyName, T (LocaleInfo::*getter)() const);
    static ani_string Locale_getLanguage(ani_env *env, ani_object object);
    static ani_string Locale_getScript(ani_env *env, ani_object object);
    static ani_string Locale_getRegion(ani_env *env, ani_object object);
    static ani_string Locale_getBaseName(ani_env *env, ani_object object);
    static ani_string Locale_getCaseFirst(ani_env *env, ani_object object);
    static ani_string Locale_getCalendar(ani_env *env, ani_object object);
    static ani_string Locale_getCollation(ani_env *env, ani_object object);
    static ani_string Locale_getHourCycle(ani_env *env, ani_object object);
    static ani_string Locale_getNumberingSystem(ani_env *env, ani_object object);
    static ani_boolean Locale_getNumeric(ani_env *env, ani_object object);
    static ani_string DateTimeFormat_format([[maybe_unused]] ani_env *env, [[maybe_unused]] ani_object object,
        ani_object Date);
    static ani_object DateTimeFormat_resolvedOptions([[maybe_unused]] ani_env *env, [[maybe_unused]] ani_object object);
    static ani_string NumberFormat_format([[maybe_unused]] ani_env *env, [[maybe_unused]] ani_object object,
        ani_double number);
    static ani_object NumberFormat_resolvedOptions([[maybe_unused]] ani_env *env, [[maybe_unused]] ani_object object);
    static ani_object Locale_create0([[maybe_unused]] ani_env *env, [[maybe_unused]] ani_class clazz);
    static ani_object Locale_create1([[maybe_unused]] ani_env *env, [[maybe_unused]] ani_class clazz,
        ani_string locale, ani_object options);
    static ani_object DateTimeFormat_create0([[maybe_unused]] ani_env *env, [[maybe_unused]] ani_class clazz);
    static ani_object DateTimeFormat_create1([[maybe_unused]] ani_env *env, [[maybe_unused]] ani_class clazz,
        ani_object locale, ani_object options);
    static ani_object NumberFormat_create0([[maybe_unused]] ani_env *env, [[maybe_unused]] ani_class clazz);
    static ani_object NumberFormat_create1([[maybe_unused]] ani_env *env, [[maybe_unused]] ani_class clazz,
        ani_object locale, ani_object options);
    static ani_status BindContext_Locale(ani_env *env);
    static ani_status BindContext_DateTimeFormat(ani_env *env);
    static ani_status BindContext_NumberFormat(ani_env *env);
private:
    std::unique_ptr<LocaleInfo> locale_ = nullptr;
    std::unique_ptr<DateTimeFormat> datefmt_ = nullptr;
    std::unique_ptr<NumberFormat> numberfmt_ = nullptr;
};
} // namespace I18n
} // namespace Global
} // namespace OHOS
#endif
