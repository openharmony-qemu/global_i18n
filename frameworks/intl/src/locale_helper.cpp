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
#include "locale_helper.h"

namespace OHOS {
namespace Global {
namespace I18n {
constexpr uint8_t INTL_INDEX_TWO = 2;

const std::unordered_map<std::string, std::unordered_set<std::string>> LocaleHelper::KEY_TO_OPTION_NAME = {
    { "localeMatcher", { "best fit", "lookup" } },
    { "usage", { "sort", "search" } },
    { "numeric", {  "false", "true" } },
    { "caseFirst", { "false", "upper", "lower" } },
    { "sensitivity", { "base", "accent", "case", "variant" } },
    { "ignorePunctuation", { "true", "false" } },
};

bool LocaleHelper::IsValidOptionName(const std::string& key, const std::string& option)
{
    auto optionIter = KEY_TO_OPTION_NAME.find(key);
    if (optionIter == KEY_TO_OPTION_NAME.end()) {
        return false;
    }
    return optionIter->second.find(option) != optionIter->second.end();
}

std::string LocaleHelper::BestAvailableLocale(const std::vector<std::string> &availableLocales,
                                              const std::string &locale)
{
    std::string localeCandidate = locale;
    std::string undefined = std::string();
    uint32_t length = availableLocales.size();
    while (true) {
        for (uint32_t i = 0; i < length; ++i) {
            std::string itemStr = availableLocales[i];
            if (itemStr == localeCandidate) {
                return localeCandidate;
            }
        }
        size_t pos = localeCandidate.rfind('-');
        if (pos == std::string::npos) {
            return undefined;
        }
        if (pos >= INTL_INDEX_TWO && localeCandidate[pos - INTL_INDEX_TWO] == '-') {
            pos -= INTL_INDEX_TWO;
        }
        localeCandidate.resize(pos);
    }
}
} // namespace I18n
} // namespace Global
} // namespace OHOS
