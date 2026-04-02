/*
 * Copyright (c) 2026 Huawei Device Co., Ltd.
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
#include "chinese_calendar.h"
#include "chnsecal.h"
#include "i18n_hilog.h"

namespace OHOS {
namespace Global {
namespace I18n {
const std::string DEFAULT_LOCALE_TAG = "zh-Hans-CN";

const std::unordered_map<std::string, std::string ChineseCalendarTime::*> ChineseCalendarTime::MEMBER_NAME_MAP = {
    { "year", &ChineseCalendarTime::year },
    { "month", &ChineseCalendarTime::month },
    { "day", &ChineseCalendarTime::day },
    { "hour", &ChineseCalendarTime::hour },
    { "minute", &ChineseCalendarTime::minute },
    { "second", &ChineseCalendarTime::second }
};

ChineseCalendar::ChineseCalendar(std::string localeTag)
    : I18nCalendar(localeTag, CalendarType::CHINESE)
{
}

ChineseCalendar::~ChineseCalendar()
{
}

void ChineseCalendar::InitChineseCalendar()
{
}

void ChineseCalendar::SetChineseCalendarTime(const ChineseCalendarTime& time)
{
    icu::Calendar* icuCalendar = GetIcuCalendar();
    if (icuCalendar == nullptr) {
        HILOG_ERROR_I18N("SetChineseCalendarTime: icuCalendar is nullptr.");
        return; 
    }

    icuCalendar->set(UCAL_EXTENDED_YEAR, time.year);
    icuCalendar->set(UCAL_MONTH, time.month);
    icuCalendar->set(UCAL_DATE, time.day);
    icuCalendar->set(UCAL_HOUR_OF_DAY, time.hour);
    icuCalendar->set(UCAL_MINUTE, time.minute);
    icuCalendar->set(UCAL_SECOND, time.second);
    icuCalendar->set(UCAL_IS_LEAP_MONTH, time.isLeapMonth ? 1 : 0);
}

bool ChineseCalendar::CheckLeapMonth(int32_t year, int32_t month)
{
    UErrorCode status = U_ZERO_ERROR;
    icu::Locale locale = icu::Locale::forLanguageTag(DEFAULT_LOCALE_TAG, status);
    if (U_FAILURE(status)) {
        HILOG_ERROR_I18N("CheckLeapMonth: Create locale failed, code is %{public}s.", u_errorName(status));
        return false;
    }

    std::unique_ptr<icu::ChineseCalendar> chineseCalendar = std::make_unique<icu::ChineseCalendar>(locale, status);
    if (U_FAILURE(status)) {
        HILOG_ERROR_I18N("CheckLeapMonth: Create chineseCalendar failed, code is %{public}s.", u_errorName(status));
        return false;
    }

    chineseCalendar->set(UCAL_EXTENDED_YEAR, year);
    int32_t leapMonth = cal->get(UCAL_LEAP_MONTH, status);
    if (U_FAILURE(status)) {
        HILOG_ERROR_I18N("CheckLeapMonth: Get leap month failed, code is %{public}s.", u_errorName(status));
        return false;
    }

    return leapMonth == month;
}
} // namespace I18n
} // namespace Global
} // namespace OHOS