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

#include "i18n_currency.h"
#include "i18n_hilog.h"
#include "unicode/locid.h"

namespace OHOS {
namespace Global {
namespace I18n {
namespace {
constexpr size_t CURRENCY_CACHE_LENGTH = 4;
}

Currency::Currency(const std::string& localeTag)
{
    UErrorCode status = U_ZERO_ERROR;
    icu::Locale locale = icu::Locale::forLanguageTag(localeTag.c_str(), status);
    if (U_FAILURE(status)) {
        HILOG_ERROR_I18N("Currency: Create icu::Locale failed for localeTag is %{public}s, code is %{public}s",
            localeTag.c_str(), u_errorName(status));
        return;
    }
    locale.addLikelySubtags(status);
    if (U_FAILURE(status)) {
        HILOG_ERROR_I18N("Currency: addLikelySubtags failed, code is %{public}s", u_errorName(status));
        return;
    }
    std::string fullLocale = locale.toLanguageTag<std::string>(status);
    if (U_FAILURE(status)) {
        HILOG_ERROR_I18N("Currency: toLanguageTag failed, code is %{public}s", u_errorName(status));
        return;
    }

    UChar currency[CURRENCY_CACHE_LENGTH] = { 0 };
    ucurr_forLocale(fullLocale.c_str(), currency, sizeof(currency) / sizeof(UChar), &status);
    if (U_FAILURE(status)) {
        HILOG_ERROR_I18N("Currency: ucurr_forLocale failed for %{public}s, code is %{public}s",
            fullLocale.c_str(), u_errorName(status));
        return;
    }
    icu::UnicodeString ustr(currency);
    std::string result;
    ustr.toUTF8String(result);
    isoCode_ = result;
}

std::string Currency::GetISO4217Code() const
{
    return isoCode_;
}
} // namespace I18n
} // namespace Global
} // namespace OHOS
