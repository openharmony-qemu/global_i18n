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
#include "index_util.h"

#include "i18n_hilog.h"
#include "locale_config.h"
#include "unicode/localebuilder.h"
#include "unicode/locid.h"
#include "string"
#include "unicode/unistr.h"
#include "vector"

namespace OHOS {
namespace Global {
namespace I18n {
namespace {
constexpr const char* OVERFLOW_LABEL = "…";
constexpr const char* NO_INDEX_LABEL = "#";
constexpr const char* COMPRESSED_EN_LABEL = "Z";
constexpr const char* ENGLISH_LOCALE = "en";
constexpr int32_t EXT_PARAM_LENGTH = 64;
std::vector<std::string> EN_INDEX_LABELS = {
    "A", "B", "C", "D", "E", "F", "G", "H", "I", "J", "K", "L", "M", "N",
    "O", "P", "Q", "R", "S", "T", "U", "V", "W", "X", "Y", "Z"
};
std::unordered_set<std::string> SUPPORTED_BILINGUA_INDEX_LANGUAGE = {
    "am", "ar", "as", "sr", "be", "bg", "bn", "el", "fa", "gu", "hi", "he", "ja",
    "ka", "kk", "km", "kn", "ko", "lo", "mai", "mk", "ml", "mn", "mr", "my", "ne",
    "or", "pa", "ru", "si", "ta", "te", "th", "ug", "uk", "ur"
};
std::unordered_set<std::string> SUPPORTED_BILINGUA_INDEX_SCRIPT = {
    "zh-Hant"
};
}

IndexUtil::IndexUtil(const std::string &localeTag)
{
    std::string locale = localeTag.empty() ? LocaleConfig::GetEffectiveLocale() : localeTag;
    locale = LocaleConfig::RemoveCustExtParam(locale);
    UErrorCode status = U_ZERO_ERROR;
    icu::Locale icuLocale = icu::Locale::forLanguageTag(locale.c_str(), status);
    if (U_FAILURE(status)) {
        HILOG_ERROR_I18N("IndexUtil: Create icu::Locale failed, code is %{public}s", u_errorName(status));
        return;
    }

    index = std::make_unique<icu::AlphabeticIndex>(icuLocale, status);
    if (U_FAILURE(status)) {
        HILOG_ERROR_I18N("IndexUtil: Create index failed, code is %{public}s", u_errorName(status));
        return;
    }
    icu::Locale extendedLocale = DealWithExtParam(icuLocale);
    bilingualIndex = std::make_unique<icu::AlphabeticIndex>(extendedLocale, status);
    if (U_FAILURE(status)) {
        HILOG_ERROR_I18N("IndexUtil: Create bilingualIndex failed, code is %{public}s", u_errorName(status));
        return;
    }
    if (!InitLocalIndexList()) {
        return;
    }
    isBilingualIndexSupported = IndexUtil::IsBilingualIndexSupported(locale);
    if (isBilingualIndexSupported) {
        icu::Locale enIcuLocale = icu::Locale::forLanguageTag(ENGLISH_LOCALE, status);
        if (U_FAILURE(status)) {
            HILOG_ERROR_I18N("IndexUtil: Create EN icu::Locale failed, code is %{public}s", u_errorName(status));
            return;
        }
        bilingualIndex->addLabels(enIcuLocale, status);
        if (U_FAILURE(status)) {
            HILOG_ERROR_I18N("IndexUtil: addLabels failed, code is %{public}s", u_errorName(status));
            return;
        }
    }
    createSuccess = true;
}

IndexUtil::~IndexUtil()
{
}

icu::Locale IndexUtil::DealWithExtParam(icu::Locale& locale)
{
    UErrorCode status = U_ZERO_ERROR;
    locale.addLikelySubtags(status);
    if (U_FAILURE(status)) {
        HILOG_ERROR_I18N("DealWithExtParam: addLikelySubtags failed");
        return locale;
    }
    std::string language = locale.getLanguage();
    std::string script = locale.getScript();
    std::string region = locale.getCountry();
    language += "-" + script;
    if (language != "zh-Hant" || region == "MO" || region == "HK") {
        return locale;
    }
    char value[EXT_PARAM_LENGTH] = { 0 };
    locale.getKeywordValue("collation", value, sizeof(value) / sizeof(char), status);
    if (U_FAILURE(status)) {
        HILOG_ERROR_I18N("DealWithExtParam: getKeywordValue failed, code is %{public}s", u_errorName(status));
        return locale;
    }
    std::string collationParam(value);
    if (!collationParam.empty()) {
        return locale;
    }
    icu::LocaleBuilder builder;
    builder.setLocale(locale);
    builder.setUnicodeLocaleKeyword("co", "zhuyin");
    icu::Locale extendedLocale = builder.build(status);
    if (U_FAILURE(status)) {
        HILOG_ERROR_I18N("DealWithExtParam: Build extendedLocale failed, code is %{public}s", u_errorName(status));
        return locale;
    }
    return extendedLocale;
}

bool IndexUtil::InitLocalIndexList()
{
    UErrorCode status = U_ZERO_ERROR;
    bilingualIndex->resetBucketIterator(status);
    if (U_FAILURE(status)) {
        HILOG_ERROR_I18N("InitLocalIndex: resetBucketIterator failed, code is %{public}s", u_errorName(status));
        return false;
    }
    while (bilingualIndex->nextBucket(status)) {
        if (U_FAILURE(status)) {
            HILOG_ERROR_I18N("InitLocalIndex: nextBucket failed, code is %{public}s", u_errorName(status));
            return false;
        }
        std::string label;
        bilingualIndex->getBucketLabel().toUTF8String(label);
        localIndexList.emplace_back(label);
    }
    localIndexList.erase(
        std::remove_if(localIndexList.begin(), localIndexList.end(),
            [](const std::string &s) { return s.empty() || s == OVERFLOW_LABEL; }),
        localIndexList.end());
    return true;
}

std::vector<std::string> IndexUtil::GetIndexList()
{
    UErrorCode status = U_ZERO_ERROR;
    std::vector<std::string> indexList;
    std::lock_guard<std::mutex> indexLock(indexMutex);
    if (!IsCreateSuccess() || index == nullptr) {
        return indexList;
    }
    index->resetBucketIterator(status);
    while (index->nextBucket(status)) {
        if (U_SUCCESS(status)) {
            icu::UnicodeString unicodeString = index->getBucketLabel();
            std::string label;
            unicodeString.toUTF8String(label);
            indexList.push_back(label);
        }
    }
    return indexList;
}

void IndexUtil::AddLocale(const std::string &localeTag)
{
    UErrorCode status = U_ZERO_ERROR;
    icu::Locale locale(localeTag.c_str());
    std::lock_guard<std::mutex> indexLock(indexMutex);
    if (createSuccess && index != nullptr) {
        index->addLabels(locale, status);
    }
}

std::string IndexUtil::GetIndex(const std::string &String)
{
    std::lock_guard<std::mutex> indexLock(indexMutex);
    if (!IsCreateSuccess() || index == nullptr) {
        return "";
    }
    UErrorCode status = U_ZERO_ERROR;
    icu::UnicodeString unicodeString(String.c_str());
    int32_t bucketNumber = index->getBucketIndex(unicodeString, status);
    if (!IsStatusSuccess(status)) {
        return "";
    }
    index->resetBucketIterator(status);
    for (int32_t i = 0; i <= bucketNumber; i++) {
        index->nextBucket(status);
        if (!IsStatusSuccess(status)) {
            return "";
        }
    }
    icu::UnicodeString label = index->getBucketLabel();
    std::string result;
    label.toUTF8String(result);
    return result;
}

bool IndexUtil::IsCreateSuccess()
{
    return createSuccess;
}

bool IndexUtil::IsStatusSuccess(UErrorCode status)
{
    return U_SUCCESS(status);
}

bool IndexUtil::IsBilingualIndexSupported(const std::string &localeTag)
{
    UErrorCode status = U_ZERO_ERROR;
    std::string curLocale = LocaleConfig::RemoveCustExtParam(localeTag);
    icu::Locale locale = icu::Locale::forLanguageTag(curLocale, status);
    if (U_FAILURE(status)) {
        HILOG_ERROR_I18N("IsBilingualIndexSupported: forLanguageTag failed");
        return false;
    }
    char value[EXT_PARAM_LENGTH] = { 0 };
    locale.getKeywordValue("collation", value, sizeof(value) / sizeof(char), status);
    if (U_FAILURE(status)) {
        HILOG_ERROR_I18N("IsBilingualIndexSupported: getKeywordValue failed, code is %{public}s", u_errorName(status));
        return false;
    }
    std::string collationParam(value);
    if (collationParam.compare("pinyin") == 0) {
        return false;
    }
    locale.addLikelySubtags(status);
    if (U_FAILURE(status)) {
        HILOG_ERROR_I18N("IsBilingualIndexSupported: addLikelySubtags failed");
        return false;
    }
    std::string language = locale.getLanguage();
    if (SUPPORTED_BILINGUA_INDEX_LANGUAGE.find(language) != SUPPORTED_BILINGUA_INDEX_LANGUAGE.end()) {
        return true;
    }
    std::string script = locale.getScript();
    language += "-" + script;
    return SUPPORTED_BILINGUA_INDEX_SCRIPT.find(language) != SUPPORTED_BILINGUA_INDEX_SCRIPT.end();
}

std::vector<std::string> IndexUtil::GetBilingualIndexList(BilingualIndexMode mode)
{
    if (!IsCreateSuccess()) {
        HILOG_ERROR_I18N("GetBilingualIndexList: bilingualIndex init failed");
        return {};
    }
    std::call_once(bilingualIndexListFlag, [this]() {
            if (localIndexList.empty()) {
                HILOG_ERROR_I18N("GetBilingualIndexList: Get local list failed");
                return;
            }
            std::vector<std::string> fullList = { NO_INDEX_LABEL };
            fullList.insert(fullList.end(), localIndexList.begin(), localIndexList.end());
            if (!isBilingualIndexSupported) {
                bilingualIndexList.insert(std::make_pair(BilingualIndexMode::FULL, fullList));
                bilingualIndexList.insert(std::make_pair(BilingualIndexMode::COMPRESSED_EN, fullList));
                bilingualIndexList.insert(std::make_pair(BilingualIndexMode::COMPRESSED_LOCAL, fullList));
                return;
            }
            std::vector<std::string> compressedEnList = { NO_INDEX_LABEL };
            std::vector<std::string> compressedLocalList { NO_INDEX_LABEL };
            fullList.insert(fullList.end(), EN_INDEX_LABELS.begin(), EN_INDEX_LABELS.end());
            compressedEnList.insert(compressedEnList.end(), localIndexList.begin(), localIndexList.end());
            compressedEnList.emplace_back(COMPRESSED_EN_LABEL);
            compressedLocalList.emplace_back(localIndexList[0]);
            compressedLocalList.insert(compressedLocalList.end(), EN_INDEX_LABELS.begin(), EN_INDEX_LABELS.end());
            bilingualIndexList.insert(std::make_pair(BilingualIndexMode::FULL, fullList));
            bilingualIndexList.insert(std::make_pair(BilingualIndexMode::COMPRESSED_EN, compressedEnList));
            bilingualIndexList.insert(std::make_pair(BilingualIndexMode::COMPRESSED_LOCAL, compressedLocalList));
        });

    auto iter = bilingualIndexList.find(mode);
    if (iter == bilingualIndexList.end()) {
        return {};
    }
    return iter->second;
}

std::string IndexUtil::GetBilingualIndex(const std::string &text)
{
    if (!IsCreateSuccess()) {
        HILOG_ERROR_I18N("GetBilingualIndex: bilingualIndex init failed");
        return NO_INDEX_LABEL;
    }
    if (text.empty()) {
        return NO_INDEX_LABEL;
    }
    std::call_once(bucketNumberToLabelFlag, [this]() {
            UErrorCode status = U_ZERO_ERROR;
            bilingualIndex->resetBucketIterator(status);
            if (U_FAILURE(status)) {
                HILOG_ERROR_I18N("GetBilingualIndex: resetBucketIterator failed, code is %{public}s",
                    u_errorName(status));
                return;
            }
            int32_t bucketCount = 0;
            while (bilingualIndex->nextBucket(status)) {
                if (U_FAILURE(status)) {
                    HILOG_ERROR_I18N("GetBilingualIndex: nextBucket failed, code is %{public}s", u_errorName(status));
                    break;
                }
                std::string label;
                bilingualIndex->getBucketLabel().toUTF8String(label);
                bucketNumberToLabel.insert(std::make_pair(bucketCount++, label));
            }
        });

    UErrorCode status = U_ZERO_ERROR;
    int32_t bucketNumber = bilingualIndex->getBucketIndex(text.c_str(), status);
    if (U_FAILURE(status)) {
        HILOG_ERROR_I18N("GetBilingualIndex: getBucketIndex failed, code is %{public}s", u_errorName(status));
        return NO_INDEX_LABEL;
    }
    auto iter = bucketNumberToLabel.find(bucketNumber);
    if (iter == bucketNumberToLabel.end() || iter->second == OVERFLOW_LABEL) {
        HILOG_ERROR_I18N("GetBilingualIndex: Get label failed, bucketNumber is %{public}d", bucketNumber);
        return NO_INDEX_LABEL;
    }
    return iter->second;
}
} // namespace I18n
} // namespace Global
} // namespace OHOS