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
#ifdef SUPPORT_APP_PREFERRED_LANGUAGE
#include <regex>
#include "application_context.h"
#include "bundle_info.h"
#include "bundle_mgr_interface.h"
#include "hap_resource.h"
#include "iservice_registry.h"
#include "system_ability_definition.h"
#endif
#include "hilog/log.h"
#include "locale_config.h"
#include "locale_info.h"
#include "parameter.h"
#include "preferred_language.h"
#include "vector"

namespace OHOS {
namespace Global {
namespace I18n {
static constexpr OHOS::HiviewDFX::HiLogLabel LABEL = { LOG_CORE, 0xD001E00, "PreferredLanguage" };
using namespace OHOS::HiviewDFX;
const char *PreferredLanguage::RESOURCE_PATH_HEAD = "/data/accounts/account_0/applications/";
const char *PreferredLanguage::RESOURCE_PATH_TAILOR = "/assets/entry/resources.index";
const char *PreferredLanguage::RESOURCE_PATH_SPLITOR = "/";
const char *PreferredLanguage::PREFERRED_LANGUAGES = "persist.sys.preferredLanguages";
const char *PreferredLanguage::APP_LANGUAGE_KEY = "app_language";
const char *PreferredLanguage::I18N_PREFERENCES_FILE_NAME = "/i18n";

bool PreferredLanguage::AddPreferredLanguageExist(std::vector<std::string> &preferredLanguageList, int languageIdx,
    int index, const std::string& language)
{
    if (languageIdx == index) {
        return false;
    }
    if (languageIdx < index) {
        for (int i = languageIdx; i < index; i++) {
            preferredLanguageList[i] = preferredLanguageList[i + 1];
        }
    }
    if (languageIdx > index) {
        for (int i = languageIdx; i > index; i--) {
            preferredLanguageList[i] = preferredLanguageList[i - 1];
        }
    }
    preferredLanguageList[index] = language;
    if (!languageIdx || !index) {
        bool status = LocaleConfig::SetSystemLanguage(preferredLanguageList[0]);
        if (!status) {
            return false;
        }
    }
    return true;
}

bool PreferredLanguage::AddPreferredLanguageNonExist(std::vector<std::string> &preferredLanguageList, int index,
    const std::string& language)
{
    preferredLanguageList.insert(preferredLanguageList.begin() + index, language);
    if (!index) {
        bool status = LocaleConfig::SetSystemLanguage(preferredLanguageList[0]);
        if (!status) {
            return false;
        }
    }
    return true;
}

void PreferredLanguage::AddPreferredLanguage(const std::string &language, int index, I18nErrorCode &errorCode)
{
    if (!LocaleConfig::CheckPermission()) {
        errorCode = I18nErrorCode::NO_PERMISSION;
        return;
    }
    if (!IsValidTag(language)) {
        errorCode = I18nErrorCode::INVALID_PARAMETER;
        return;
    }
    std::vector<std::string> preferredLanguageList = GetPreferredLanguageList();
    int idx = index;
    if (index < 0) {
        idx = 0;
    }
    if (index >= (int)preferredLanguageList.size()) {
        idx = (int)preferredLanguageList.size();
    }
    int languageIdx = -1;
    for (size_t i = 0; i < preferredLanguageList.size(); i++) {
        if (preferredLanguageList[i] == language) {
            languageIdx = (int)i;
            break;
        }
    }
    bool status = true;
    if (languageIdx == -1) {
        status = AddPreferredLanguageNonExist(preferredLanguageList, idx, language);
    } else {
        if (idx == (int)preferredLanguageList.size()) {
            idx -= 1;
        }
        status = AddPreferredLanguageExist(preferredLanguageList, languageIdx, idx, language);
    }
    if (!status) {
        errorCode = I18nErrorCode::FAILED;
        return;
    }
    std::string result = "";
    for (size_t i = 0; i < preferredLanguageList.size(); i++) {
        result += preferredLanguageList[i];
        result += ";";
    }
    result.pop_back();
    if (result.length() > CONFIG_LEN || SetParameter(PREFERRED_LANGUAGES, result.data()) != 0) {
        errorCode = I18nErrorCode::FAILED;
    }
}

bool PreferredLanguage::RemovePreferredLanguage(int index)
{
    if (!LocaleConfig::CheckPermission()) {
        return false;
    }
    std::vector<std::string> preferredLanguageList = GetPreferredLanguageList();
    int idx = index;
    if (index < 0) {
        idx = 0;
    }
    if (index >= (int)preferredLanguageList.size()) {
        idx = (int)preferredLanguageList.size() - 1;
    }
    if (preferredLanguageList.size() == 1) {
        return false;
    }
    std::vector<std::string>::iterator it = preferredLanguageList.begin();
    preferredLanguageList.erase(it + idx);
    bool status = false;
    if (!idx) {
        status = LocaleConfig::SetSystemLanguage(preferredLanguageList[0]);
        if (!status) {
            return false;
        }
    }
    std::string result = "";
    for (it = preferredLanguageList.begin(); it != preferredLanguageList.end(); ++it) {
        result += *it;
        result += ";";
    }
    result.pop_back();
    status = SetParameter(PREFERRED_LANGUAGES, result.data()) == 0;
    return status;
}

std::vector<std::string> PreferredLanguage::GetPreferredLanguageList()
{
    char preferredLanguageValue[CONFIG_LEN];
    GetParameter(PREFERRED_LANGUAGES, "", preferredLanguageValue, CONFIG_LEN);
    std::string systemLanguage = LocaleConfig::GetSystemLanguage();
    std::vector<std::string> list;
    Split(preferredLanguageValue, ";", list);
    if (!list.size()) {
        if (systemLanguage != "") {
            list.push_back(systemLanguage);
        }
        return list;
    }
    if (list[0] == systemLanguage || systemLanguage == "") {
        return list;
    }
    int systemLanguageIdx = -1;
    for (size_t i = 0; i < list.size(); i++) {
        if (list[i] == systemLanguage) {
            systemLanguageIdx = (int)i;
        }
    }
    if (systemLanguageIdx == -1) {
        list.insert(list.begin(), systemLanguage);
    } else {
        for (size_t i = (size_t)systemLanguageIdx; i > 0; i--) {
            list[i] = list[i - 1];
        }
        list[0] = systemLanguage;
    }
    return list;
}

std::string PreferredLanguage::GetFirstPreferredLanguage()
{
    std::vector<std::string> preferredLanguageList = GetPreferredLanguageList();
    return preferredLanguageList[0];
}

#ifdef SUPPORT_APP_PREFERRED_LANGUAGE
std::shared_ptr<NativePreferences::Preferences> PreferredLanguage::GetI18nAppPreferences()
{
    std::shared_ptr<AbilityRuntime::ApplicationContext> appContext = AbilityRuntime::ApplicationContext::GetInstance();
    std::string preferencesDirPath = appContext->GetPreferencesDir();
    std::string i18nPreferencesFilePath = preferencesDirPath + I18N_PREFERENCES_FILE_NAME;
    int status;
    NativePreferences::Options options(i18nPreferencesFilePath);
    std::shared_ptr<NativePreferences::Preferences> preferences =
        NativePreferences::PreferencesHelper::GetPreferences(options, status);
    if (status != 0) {
        HiLog::Error(LABEL, "PreferredLanguage::GetAppPreferredLanguage get i18n app preferences failed.");
        return nullptr;
    }
    return preferences;
}

std::string PreferredLanguage::GetAppPreferredLanguage()
{
    std::shared_ptr<NativePreferences::Preferences> preferences = GetI18nAppPreferences();
    if (preferences == nullptr) {
        HiLog::Error(LABEL, 
            "PreferredLanguage::GetAppPreferredLanguage get i18n preferences failed, return system language.");
        return LocaleConfig::GetSystemLanguage();
    }
    std::string res = preferences->GetString(PreferredLanguage::APP_LANGUAGE_KEY, "");
    if (res.length() == 0) {
        HiLog::Info(LABEL, 
            "PreferredLanguage::GetAppPreferredLanguage app language is empty, return system language.");
        return LocaleConfig::GetSystemLanguage();
    }
    return res;
}

void PreferredLanguage::SetAppPreferredLanguage(const std::string &language, I18nErrorCode &errCode)
{
    std::shared_ptr<AbilityRuntime::ApplicationContext> appContext = AbilityRuntime::ApplicationContext::GetInstance();
    appContext->SetLanguage(language);
    std::shared_ptr<NativePreferences::Preferences> preferences = GetI18nAppPreferences();
    if (preferences == nullptr) {
        errCode = I18nErrorCode::FAILED;
        HiLog::Error(LABEL, "PreferredLanguage::SetAppPreferredLanguage get i18n preferences failed.");
        return;
    }
    int32_t status = preferences->PutString(PreferredLanguage::APP_LANGUAGE_KEY, language);
    if (status != 0) {
        errCode = I18nErrorCode::FAILED;
        HiLog::Error(LABEL,
            "PreferredLanguage::SetAppPreferredLanguage set app language to i18n preferences failed.");
        return;
    }
    preferences->Flush();
}
#endif

std::string PreferredLanguage::GetPreferredLocale()
{
    std::string systemLocale = LocaleConfig::GetSystemLocale();
    LocaleInfo systemLocaleInfo(systemLocale);
    std::string systemRegion = systemLocaleInfo.GetRegion();
    std::string preferredLanguageLocale = GetFirstPreferredLanguage();
    LocaleInfo preferredLanguageLocaleInfo(preferredLanguageLocale);
    std::string preferredLanguage = preferredLanguageLocaleInfo.GetLanguage();
    std::string preferredLocale = preferredLanguage + "-" + systemRegion;
    return preferredLocale;
}

bool PreferredLanguage::IsValidLanguage(const std::string &language)
{
    std::string::size_type size = language.size();
    if ((size != LANGUAGE_LEN) && (size != LANGUAGE_LEN + 1)) {
        return false;
    }
    for (size_t i = 0; i < size; ++i) {
        if ((language[i] > 'z') || (language[i] < 'a')) {
            return false;
        }
    }
    return true;
}

bool PreferredLanguage::IsValidTag(const std::string &tag)
{
    if (!tag.size()) {
        return false;
    }
    std::vector<std::string> splits;
    Split(tag, "-", splits);
    if (!IsValidLanguage(splits[0])) {
        return false;
    }
    return true;
}

void PreferredLanguage::Split(const std::string &src, const std::string &sep, std::vector<std::string> &dest)
{
    std::string::size_type begin = 0;
    std::string::size_type end = src.find(sep);
    while (end != std::string::npos) {
        dest.push_back(src.substr(begin, end - begin));
        begin = end + sep.size();
        end = src.find(sep, begin);
    }
    if (begin != src.size()) {
        dest.push_back(src.substr(begin));
    }
}
} // namespace I18n
} // namespace Global
} // namespace OHOS