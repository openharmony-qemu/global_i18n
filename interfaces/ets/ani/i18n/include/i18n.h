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

#ifndef I18N_ANI_H
#define I18N_ANI_H

#include <ani.h>

#include "entity_recognizer.h"
#include "holiday_manager.h"
#include "i18n_break_iterator.h"
#include "i18n_calendar.h"
#include "i18n_normalizer.h"
#include "i18n_timezone.h"
#include "index_util.h"
#include "intl.h"
#include "phone_number_format.h"
#include "simple_date_time_format.h"
#include "simple_number_format.h"
#include "styled_number_format.h"
#include "system_locale_manager.h"
#include "unicode/translit.h"

namespace OHOS {
namespace Global {
namespace I18n {
class I18nPhoneNumberFormatAddon {
public:
    static I18nPhoneNumberFormatAddon* UnwrapAddon(ani_env *env, ani_object object);
    static ani_object Create(ani_env *env, [[maybe_unused]] ani_class clazz, ani_string country, ani_object options);
    static ani_boolean IsValidNumber(ani_env *env, ani_object object, ani_string number);
    static ani_string Format(ani_env *env, ani_object object, ani_string number);
    static ani_string GetLocationName(ani_env *env, ani_object object, ani_string number, ani_string locale);
private:
    std::unique_ptr<PhoneNumberFormat> phonenumberfmt_ = nullptr;
};

class I18nCalendarAddon {
public:
    static I18nCalendarAddon* UnwrapAddon(ani_env *env, ani_object object);
    static ani_object GetCalendar(ani_env *env, ani_string locale, ani_string type);
    static void SetTimeByDate(ani_env *env, ani_object object, ani_object date);
    static void SetTime(ani_env *env, ani_object object, ani_double time);
    static void Set(ani_env *env, ani_object object, ani_double year, ani_double month,
        ani_double date, ani_object hour, ani_object minute, ani_object second);
    static void SetTimeZone(ani_env *env, ani_object object, ani_string timezone);
    static ani_string GetTimeZone(ani_env *env, ani_object object);
    static ani_double GetFirstDayOfWeek(ani_env *env, ani_object object);
    static void SetFirstDayOfWeek(ani_env *env, ani_object object, ani_double value);
    static ani_double GetMinimalDaysInFirstWeek(ani_env *env, ani_object object);
    static void SetMinimalDaysInFirstWeek(ani_env *env, ani_object object, ani_double value);
    static ani_double Get(ani_env *env, ani_object object, ani_string field);
    static ani_string GetDisplayName(ani_env *env, ani_object object, ani_string locale);
    static ani_boolean IsWeekend(ani_env *env, ani_object object, ani_object date);
    static void Add(ani_env *env, ani_object object, ani_string field, ani_double amount);
    static ani_double GetTimeInMillis(ani_env *env, ani_object object);
    static ani_double CompareDays(ani_env *env, ani_object object, ani_object date);
private:
    std::unique_ptr<I18nCalendar> calendar_ = nullptr;
};

class I18nBreakIteratorAddon {
public:
    static I18nBreakIteratorAddon* UnwrapAddon(ani_env *env, ani_object object);
    static ani_object GetLineInstance(ani_env *env, ani_string locale);
    static ani_double Current(ani_env *env, ani_object object);
    static ani_double First(ani_env *env, ani_object object);
    static ani_double Last(ani_env *env, ani_object object);
    static ani_double Next(ani_env *env, ani_object object, ani_object index);
    static ani_double Previous(ani_env *env, ani_object object);
    static void SetLineBreakText(ani_env *env, ani_object object, ani_string text);
    static ani_double Following(ani_env *env, ani_object object, ani_double offset);
    static ani_string GetLineBreakText(ani_env *env, ani_object object);
    static ani_boolean IsBoundary(ani_env *env, ani_object object, ani_double offset);
private:
    std::unique_ptr<I18nBreakIterator> brkiter_ = nullptr;
};

class I18nIndexUtilAddon {
public:
    static I18nIndexUtilAddon* UnwrapAddon(ani_env *env, ani_object object);
    static ani_object GetInstance(ani_env *env, ani_string string);
    static ani_object GetIndexList(ani_env *env, ani_object object);
    static void AddLocale(ani_env *env, ani_object object, ani_string locale);
    static ani_object GetIndex(ani_env *env, ani_object object, ani_string text);
private:
    std::unique_ptr<IndexUtil> indexUtil_ = nullptr;
};

class I18nTimeZoneAddon {
public:
    static I18nTimeZoneAddon* UnwrapAddon(ani_env *env, ani_object object);
    static ani_object GetTimeZone(ani_env *env, ani_string zoneID);
    static ani_string GetID(ani_env *env, ani_object object);
    static ani_string GetDisplayName(ani_env *env, ani_object object, ani_string locale, ani_object isDST);
    static ani_string GetDisplayNameOnlyBool(ani_env *env, ani_object object, ani_object isDST);
    static ani_double GetRawOffset(ani_env *env, ani_object object);
    static ani_double GetOffset(ani_env *env, ani_object object, ani_object date);
    static ani_object GetAvailableIDs(ani_env *env, [[maybe_unused]] ani_object object);
    static ani_object GetAvailableZoneCityIDs(ani_env *env, [[maybe_unused]] ani_object object);
    static ani_object GetCityDisplayName(ani_env *env, [[maybe_unused]] ani_object object,
        ani_string cityID, ani_string locale);
    static ani_object GetTimezoneFromCity(ani_env *env, [[maybe_unused]] ani_object object, ani_string cityID);
    static ani_object GetTimezonesByLocation(ani_env *env, [[maybe_unused]] ani_object object,
        ani_double longitude, ani_double latitude);
private:
    std::unique_ptr<I18nTimeZone> timezone_ = nullptr;
};

class I18nTransliteratorAddon {
public:
    static I18nTransliteratorAddon* UnwrapAddon(ani_env *env, ani_object object);
    static ani_object GetAvailableIDs(ani_env *env, [[maybe_unused]] ani_object object);
    static ani_object GetInstance(ani_env *env, [[maybe_unused]] ani_object object, ani_string id);
    static ani_string Transform(ani_env *env, ani_object object, ani_string text);
private:
    std::unique_ptr<icu::Transliterator> transliterator_ = nullptr;
};

class I18nNormalizerAddon {
public:
    static I18nNormalizerAddon* UnwrapAddon(ani_env *env, ani_object object);
    static ani_object GetInstance(ani_env *env, [[maybe_unused]] ani_object object, ani_enum_item mode);
    static ani_string Normalize(ani_env *env, ani_object object, ani_string text);
private:
    static const int32_t NORMALIZER_MODE_NFC = 1;
    static const int32_t NORMALIZER_MODE_NFD = 2;
    static const int32_t NORMALIZER_MODE_NFKC = 3;
    static const int32_t NORMALIZER_MODE_NFKD = 4;

    std::unique_ptr<I18nNormalizer> normalizer_ = nullptr;
};

class I18nSysLocaleMgrAddon {
public:
    static I18nSysLocaleMgrAddon* UnwrapAddon(ani_env *env, ani_object object);
    static ani_object Create(ani_env *env, [[maybe_unused]] ani_class clazz);
    static ani_object GetLanguageInfoArray(ani_env *env, ani_object object, ani_object languages, ani_object options);
    static ani_object GetRegionInfoArray(ani_env *env, ani_object object, ani_object regions, ani_object options);
    static ani_object GetTimeZoneCityItemArray(ani_env *env, [[maybe_unused]] ani_object object);
private:
    std::unique_ptr<SystemLocaleManager> systemLocaleManager_ = nullptr;
};

class I18nHolidayManagerAddon {
public:
    static I18nHolidayManagerAddon* UnwrapAddon(ani_env *env, ani_object object);
    static ani_object Create(ani_env *env, [[maybe_unused]] ani_class clazz, ani_string icsPath);
    static ani_boolean IsHoliday(ani_env *env, ani_object object, ani_object date);
    static ani_object GetHolidayInfoItemArray(ani_env *env, ani_object object, ani_object year);
private:
    static const int32_t MONTH_INCREASE_ONE = 1;
    std::unique_ptr<HolidayManager> holidayManager_ = nullptr;
};

class I18nEntityRecognizerAddon {
public:
    static I18nEntityRecognizerAddon* UnwrapAddon(ani_env *env, ani_object object);
    static ani_object Create(ani_env *env, [[maybe_unused]] ani_class clazz, ani_string locale);
    static ani_object FindEntityInfo(ani_env *env, ani_object object, ani_string text);
private:
    std::unique_ptr<EntityRecognizer> entityRecognizer_ = nullptr;
};

class I18nSimpleDateTimeFormatAddon {
public:
    static I18nSimpleDateTimeFormatAddon* UnwrapAddon(ani_env *env, ani_object object);
    static ani_object GetSimpleDateTimeFormatByPattern(ani_env *env, ani_string pattern, ani_object locale);
    static ani_object GetSimpleDateTimeFormatBySkeleton(ani_env *env, ani_string skeleton, ani_object locale);
    static ani_string Format(ani_env *env, ani_object object, ani_object date);
private:
    std::unique_ptr<SimpleDateTimeFormat> simpleDateTimeFormat_ = nullptr;
};

class I18nSimpleNumberFormatAddon {
public:
    static I18nSimpleNumberFormatAddon* UnwrapAddon(ani_env *env, ani_object object);
    static ani_object GetSimpleNumberFormatBySkeleton(ani_env *env, ani_string skeleton, ani_object locale);
    static ani_string Format(ani_env *env, ani_object object, ani_double value);

    std::shared_ptr<SimpleNumberFormat> CopySimpleNumberFormat()
    {
        return simpleNumberFormat_;
    }
private:
    std::shared_ptr<SimpleNumberFormat> simpleNumberFormat_ = nullptr;
};

class I18nStyledNumberFormatAddon {
public:
    static I18nStyledNumberFormatAddon* UnwrapAddon(ani_env *env, ani_object object);
    static ani_object Create(ani_env *env, [[maybe_unused]] ani_class clazz,
        ani_object numberFormat, ani_object options);
    static ani_object Format(ani_env *env, ani_object object, ani_double value);
private:
    void SetTextStyle(ani_env *env, ani_object options, const std::string &optionName);
    bool InitStyledNumberFormatContent(ani_env *env, IntlAniAddon* numberFormatAddon,
        I18nSimpleNumberFormatAddon* simpleNumberFormatAddon, ani_object options);

    std::unordered_map<std::string, ani_ref> styledNumberFormatOptions_ {};
    std::unique_ptr<StyledNumberFormat> styledNumberFormat_ = nullptr;
};
} // namespace I18n
} // namespace Global
} // namespace OHOS
#endif