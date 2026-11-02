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
#ifndef GLOBAL_I18N_INDEX_UTIL_H
#define GLOBAL_I18N_INDEX_UTIL_H

#include <cstdint>
#include <mutex>
#include <string>
#include <vector>

#include "memory"
#include "unicode/alphaindex.h"
#include "unicode/utypes.h"

namespace OHOS {
namespace Global {
namespace I18n {
enum class BilingualIndexMode {
    FULL = 0,
    COMPRESSED_EN = 1,
    COMPRESSED_LOCAL = 2,
};

class IndexUtil {
public:
    explicit IndexUtil(const std::string &localeTag);
    virtual ~IndexUtil();
    std::vector<std::string> GetIndexList();
    void AddLocale(const std::string &localeTag);
    std::string GetIndex(const std::string &String);
    static bool IsBilingualIndexSupported(const std::string &localeTag);
    std::vector<std::string> GetBilingualIndexList(BilingualIndexMode mode);
    std::string GetBilingualIndex(const std::string &text);

protected:
    virtual bool IsCreateSuccess();
    virtual bool IsStatusSuccess(UErrorCode status);

private:
    icu::Locale DealWithExtParam(icu::Locale& locale);
    bool InitLocalIndexList();

    std::unique_ptr<icu::AlphabeticIndex> index = nullptr;
    std::unique_ptr<icu::AlphabeticIndex> bilingualIndex = nullptr;
    std::once_flag bucketNumberToLabelFlag;
    std::unordered_map<int32_t, std::string> bucketNumberToLabel;
    std::once_flag bilingualIndexListFlag;
    std::unordered_map<BilingualIndexMode, std::vector<std::string>> bilingualIndexList;
    std::vector<std::string> localIndexList;
    std::mutex indexMutex;
    bool isBilingualIndexSupported = false;
    bool createSuccess = false;
};
} // namespace I18n
} // namespace Global
} // namespace OHOS
#endif
