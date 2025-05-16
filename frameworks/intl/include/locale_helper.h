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
#ifndef GLOBAL_LOCALE_HELPER_H
#define GLOBAL_LOCALE_HELPER_H

#include <string>
#include <unordered_set>

namespace OHOS {
namespace Global {
namespace I18n {
class LocaleHelper {
public:
    static bool IsValidOptionName(const std::string& key, const std::string& option);

    static std::string BestAvailableLocale(const std::vector<std::string> &availableLocales, 
                                           const std::string &locale);

private:
    static const std::unordered_map<std::string, std::unordered_set<std::string>> KEY_TO_OPTION_NAME;
};
} // namespace I18n
} // namespace Global
} // namespace OHOS
#endif