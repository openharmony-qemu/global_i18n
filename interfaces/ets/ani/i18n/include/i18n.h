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

#ifndef I18N_ANI_H
#define I18N_ANI_H

#include <ani.h>
#include "i18n_calendar.h"
#include "i18n_timezone.h"

namespace OHOS {
namespace Global {
namespace I18n {
class I18nCalendarAddon {
public:
    static I18nCalendarAddon* unwrapAddon(ani_env *env, ani_object object);
    static ani_object getCalendar([[maybe_unused]] ani_env *env, ani_string locale, ani_string type);
    static ani_string CalendarGetTimeZone([[maybe_unused]] ani_env *env, [[maybe_unused]] ani_object object);
    static ani_double Get([[maybe_unused]] ani_env *env, [[maybe_unused]] ani_object object, ani_string field);
private:
    std::unique_ptr<I18nCalendar> calendar_ = nullptr;
};

class I18nTimeZoneAddon {
public:
    static ani_object i18nGetTimeZone([[maybe_unused]] ani_env *env, [[maybe_unused]] ani_object object,
        ani_string zoneID);
private:
    std::unique_ptr<I18nTimeZone> timezone_ = nullptr;
};
} // namespace I18n
} // namespace Global
} // namespace OHOS
#endif