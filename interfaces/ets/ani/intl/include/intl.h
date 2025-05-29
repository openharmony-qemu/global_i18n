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

#ifndef INTL_ANI_H
#define INTL_ANI_H

#include <ani.h>
#include <array>
#include <iostream>

#include "collator.h"
#include "date_time_format.h"
#include "locale_info.h"
#include "number_format.h"
#include "plural_rules.h"
#include "relative_time_format.h"

namespace OHOS {
namespace Global {
namespace I18n {
class IntlAniAddon {
public:
    template<typename T>
    static T GetLocaleProperty(ani_env *env, ani_object object,
        const std::string& propertyName, T (LocaleInfo::*getter)() const);
    static ani_string Locale_toString(ani_env *env, ani_object object);
    static ani_object Locale_maximize(ani_env *env, ani_object object);
    static ani_object Locale_minimize(ani_env *env, ani_object object);
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
    static ani_string DateTimeFormat_format(ani_env *env, ani_object object, ani_object date);
    static ani_string DateTimeFormat_formatRange(ani_env *env, ani_object object,
        ani_object startDate, ani_object endDate);
    static ani_object DateTimeFormat_resolvedOptions(ani_env *env, ani_object object);
    static ani_string NumberFormat_format(ani_env *env, ani_object object, ani_double number);
    static ani_string NumberFormat_formatRange(ani_env *env, ani_object object,
        ani_double startRange, ani_double endRange);
    static ani_object NumberFormat_resolvedOptions(ani_env *env, ani_object object);
    static ani_double Collator_compare(ani_env *env, ani_object object, ani_string first, ani_string second);
    static ani_object Collator_resolvedOptions(ani_env *env, ani_object object);
    static ani_string PluralRules_select(ani_env *env, ani_object object, ani_double number);
    static ani_string RelativeTimeFormat_format(ani_env *env, ani_object object, ani_double value, ani_string unit);
    static ani_object RelativeTimeFormat_formatToParts(ani_env *env, ani_object object,
        ani_double value, ani_string unit);
    static ani_object RelativeTimeFormat_resolvedOptions(ani_env *env, ani_object object);

    static ani_object Locale_create(ani_env *env, [[maybe_unused]] ani_class clazz);
    static ani_object Locale_createWithParam(ani_env *env, [[maybe_unused]] ani_class clazz,
        ani_string locale, ani_object options);
    static ani_object DateTimeFormat_create(ani_env *env, [[maybe_unused]] ani_class clazz);
    static ani_object DateTimeFormat_createWithParam(ani_env *env, [[maybe_unused]] ani_class clazz,
        ani_object locale, ani_object options);
    static ani_object NumberFormat_create(ani_env *env, [[maybe_unused]] ani_class clazz);
    static ani_object NumberFormat_createWithParam(ani_env *env, [[maybe_unused]] ani_class clazz,
        ani_object locale, ani_object options);
    static ani_object Collator_create(ani_env *env, [[maybe_unused]] ani_class clazz);
    static ani_object Collator_createWithParam(ani_env *env, [[maybe_unused]] ani_class clazz,
        ani_object locale, ani_object options);
    static ani_object PluralRules_create(ani_env *env, [[maybe_unused]] ani_class clazz);
    static ani_object PluralRules_createWithParam(ani_env *env, [[maybe_unused]] ani_class clazz,
        ani_object locale, ani_object options);
    static ani_object RelativeTimeFormat_create(ani_env *env, [[maybe_unused]] ani_class clazz);
    static ani_object RelativeTimeFormat_createWithParam(ani_env *env, [[maybe_unused]] ani_class clazz,
        ani_object locale, ani_object options);

    static ani_status BindContext_Locale(ani_env *env);
    static ani_status BindContext_DateTimeFormat(ani_env *env);
    static ani_status BindContext_NumberFormat(ani_env *env);
    static ani_status BindContext_Collator(ani_env *env);
    static ani_status BindContext_PluralRules(ani_env *env);
    static ani_status BindContext_RelativeTimeFormat(ani_env *env);

    std::shared_ptr<LocaleInfo> GetLocaleInfo()
    {
        if (!this->locale_) {
            return nullptr;
        }
        return this->locale_;
    }

    std::shared_ptr<NumberFormat> CopyNumberFormat()
    {
        return numberfmt_;
    }
private:
    std::unique_ptr<Collator> collator_ = nullptr;
    std::unique_ptr<DateTimeFormat> datefmt_ = nullptr;
    std::shared_ptr<LocaleInfo> locale_ = nullptr;
    std::shared_ptr<NumberFormat> numberfmt_ = nullptr;
    std::unique_ptr<PluralRules> pluralrules_ = nullptr;
    std::unique_ptr<RelativeTimeFormat> relativetimefmt_ = nullptr;
};
} // namespace I18n
} // namespace Global
} // namespace OHOS
#endif
