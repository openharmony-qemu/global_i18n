/*
 * Copyright (c) 2021-2022 Huawei Device Co., Ltd.
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
#ifndef OHOS_GLOBAL_I18N_PREFERRED_LANGUAGE_H
#define OHOS_GLOBAL_I18N_PREFERRED_LANGUAGE_H

#include <set>
#include <string>
#include <vector>
#include "i18n_types.h"
#ifdef SUPPORT_APP_PREFERRED_LANGUAGE
#include "preferences_helper.h"
#endif

namespace OHOS {
namespace Global {
namespace I18n {
class PreferredLanguage {
public:
    static void AddPreferredLanguage(const std::string& language, int index, I18nErrorCode &errorCode);
    static bool RemovePreferredLanguage(int index);
    static std::vector<std::string> GetPreferredLanguageList();
    static std::string GetFirstPreferredLanguage();
#ifdef SUPPORT_APP_PREFERRED_LANGUAGE
    /**
     * @brief Get App Language.
     * Get the app language from app preferences data which saved by SetAppPreferredLanguage method.
     *
     * @return std::string return current app language.
     */
    static std::string GetAppPreferredLanguage();

    /**
     * @brief Set App Language.
     * The application interface will be refreshed in real time after call this method. And the language will be saved
     * to app preferences data which will be used the next time the application starts.
     *
     * @param language language to be set.
     * @param errCode Indicates whether the setting was successful; errCode == I18nErrorCode::SUCCESS means successful.
     */
    static void SetAppPreferredLanguage(const std::string &language, I18nErrorCode &errCode);
#endif
    static std::string GetPreferredLocale();

private:
    static bool AddPreferredLanguageNonExist(std::vector<std::string> &preferredLangList, int index,
                                             const std::string& language);
    static bool AddPreferredLanguageExist(std::vector<std::string> &preferredLangList, int languageIdx, int index,
                                          const std::string& language);
    static bool IsValidLanguage(const std::string &language);
    static bool IsValidTag(const std::string &tag);
    static void Split(const std::string &src, const std::string &sep, std::vector<std::string> &dest);
#ifdef SUPPORT_APP_PREFERRED_LANGUAGE
    static std::shared_ptr<NativePreferences::Preferences> GetI18nAppPreferences();
#endif
    static const char *RESOURCE_PATH_HEAD;
    static const char *RESOURCE_PATH_TAILOR;
    static const char *RESOURCE_PATH_SPLITOR;
    static const char *PREFERRED_LANGUAGES;
    static const char *APP_LANGUAGE_KEY;
    static const char *I18N_PREFERENCES_FILE_NAME;
    static constexpr int CONFIG_LEN = 128;
    static constexpr uint32_t LANGUAGE_LEN = 2;
};
} // namespace I18n
} // namespace Global
} // namespace OHOS
#endif