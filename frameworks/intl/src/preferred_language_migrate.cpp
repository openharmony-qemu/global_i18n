/*
 * Copyright (c) 2024 Huawei Device Co., Ltd.
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
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <unordered_set>
#include "i18n_hilog.h"
#include "locale_config.h"
#include "locale_matcher.h"
#include "securec.h"
#include "utils.h"

#include "preferred_language_migrate.h"

using OHOS::Global::I18n::IsValidLocaleTag;
using OHOS::Global::I18n::LocaleConfig;
using OHOS::Global::I18n::LocaleInfo;
using OHOS::Global::I18n::LocaleMatcher;
using OHOS::Global::I18n::Merge;
using OHOS::Global::I18n::Split;

std::string GetMatchedLanguage(const std::string& language)
{
    UErrorCode status = U_ZERO_ERROR;
    icu::Locale locale = icu::Locale::forLanguageTag(language.c_str(), status);
    if (U_FAILURE(status) || !IsValidLocaleTag(locale)) {
        HILOG_ERROR_I18N("GetMatchedLanguage: %{public}s is an invalid locale.", language.c_str());
        return "";
    }
    LocaleInfo* requestLocale = new LocaleInfo(language);
    if (requestLocale == nullptr) {
        HILOG_ERROR_I18N("GetMatchedLanguage: %{public}s failed to construct LocaleInfo.",
            language.c_str());
        return "";
    }
    std::vector<LocaleInfo*> candidateLocales;
    std::vector<std::string> supportLanguageList;
    LocaleConfig::GetSystemLanguages(supportLanguageList);
    for (auto& supportLanguage : supportLanguageList) {
        LocaleInfo* supportLocaleInfo = new LocaleInfo(supportLanguage);
        if (supportLocaleInfo == nullptr) {
            HILOG_ERROR_I18N("GetMatchedLanguage: %{public}s failed to construct LocaleInfo.", supportLanguage.c_str());
            continue;
        }
        if (LocaleMatcher::Match(requestLocale, supportLocaleInfo)) {
            candidateLocales.push_back(supportLocaleInfo);
        } else {
            delete supportLocaleInfo;
        }
    }
    std::string matchedLanguage = LocaleMatcher::GetBestMatchedLocale(requestLocale, candidateLocales);
    for (LocaleInfo* supportLocaleInfo : candidateLocales) {
        delete supportLocaleInfo;
    }
    return matchedLanguage;
}

std::vector<std::string> GetMatchedList(const std::vector<std::string>& preferredLanguagesList)
{
    std::vector<std::string> matchedLanguagesList;
    std::unordered_set<std::string> matchedSet;
    for (auto& preferredLanguage : preferredLanguagesList) {
        std::string matchedLanguage = GetMatchedLanguage(preferredLanguage);
        if (matchedLanguage.empty()) {
            HILOG_ERROR_I18N("GetMatchedList: the matching result of %{public}s is empty.",
                preferredLanguage.c_str());
            continue;
        }
        if (matchedSet.find(matchedLanguage) == matchedSet.end()) {
            matchedLanguagesList.push_back(matchedLanguage);
            matchedSet.insert(matchedLanguage);
            HILOG_ERROR_I18N("GetMatchedList: the matching result is %{public}s.",
                matchedLanguage.c_str());
        }
    }
    return matchedLanguagesList;
}

extern "C" char* ConvertPreferredLanguage(const char* preferredLanguages)
{
    HILOG_ERROR_I18N("ConvertPreferredLanguage: start to convert the format of the preferred language.");
    std::vector<std::string> preferredLanguagesList;
    Split(preferredLanguages, ",", preferredLanguagesList);

    std::vector<std::string> matchedLanguagesList = GetMatchedList(preferredLanguagesList);
    std::string mergeResult;
    Merge(matchedLanguagesList, ";", mergeResult);
    size_t bufferSize = sizeof(char) * (mergeResult.length() + 1);
    char* result = new char[bufferSize];
    if (result == nullptr) {
        HILOG_ERROR_I18N("ConvertPreferredLanguage: failed to apply for memory.");
        return result;
    }
    int status = strcpy_s(result, bufferSize, mergeResult.c_str());
    if (status != 0) {
        HILOG_ERROR_I18N("ConvertPreferredLanguage: failed to copy the result.");
        return nullptr;
    }
    return result;
}
