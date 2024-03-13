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
#include "date_time_sequence.h"
#include "i18n_hilog.h"
#include "unicode/datefmt.h"
#include "unicode/locid.h"
#include "unicode/smpdtfmt.h"
#include "unicode/dtptngen.h"
#include <regex>

namespace OHOS {
namespace Global {
namespace I18n {

std::string DateTimeSequence::GetDateOrder(const std::string &locale)
{
    UErrorCode icuStatus = U_ZERO_ERROR;
    icu::Locale localeObj = icu::Locale::forLanguageTag(locale.data(), icuStatus);
    if (icuStatus != U_ZERO_ERROR) {
        HILOG_ERROR_I18N("Failed to create locale for GetDateOrder");
        return "";
    }
    icu::SimpleDateFormat* formatter = dynamic_cast<icu::SimpleDateFormat*>
        (icu::DateFormat::createDateInstance(icu::DateFormat::EStyle::kDefault, localeObj));
    if (icuStatus != U_ZERO_ERROR || formatter == nullptr) {
        HILOG_ERROR_I18N("Failed to create SimpleDateFormat");
        return "";
    }
    std::string tempValue;
    icu::UnicodeString unistr;
    formatter->toPattern(unistr);
    delete formatter;
    unistr.toUTF8String<std::string>(tempValue);
    std::string value = ModifyOrder(tempValue);
    std::regex pattern("d+");
    std::regex reg("M+");
    value = regex_replace(value, pattern, "d");
    value = regex_replace(value, reg, "M");
    return value;
}

std::string DateTimeSequence::GetDateTimeOrder(const std::string &locale)
{
    UErrorCode status = U_ZERO_ERROR;
    icu::Locale localeObj = icu::Locale::forLanguageTag(locale.data(), status);
    if (status != U_ZERO_ERROR) {
        HILOG_ERROR_I18N("Failed to create locale with inputs: %{public}s", locale.c_str());
        return "";
    }
    icu::SimpleDateFormat* formatter = dynamic_cast<icu::SimpleDateFormat*>
        (icu::DateFormat::createDateInstance(icu::DateFormat::EStyle::kDateTime, localeObj));
    if (status != U_ZERO_ERROR || formatter == nullptr) {
        HILOG_ERROR_I18N("Failed to create SimpleDateFormat");
        return "";
    }
    icu::UnicodeString pattern;
    formatter->toPattern(pattern);
    
    std::string result;
    pattern.toUTF8String<std::string>(result);
    delete formatter;
    return result;
}

std::string DateTimeSequence::GetAmPmTimeOrder(const std::string &locale)
{
    UErrorCode status = U_ZERO_ERROR;
    icu::Locale localeObj = icu::Locale::forLanguageTag(locale.data(), status);
    if (status != U_ZERO_ERROR) {
        HILOG_ERROR_I18N("Failed to create locale with inputs: %{public}s", locale.c_str());
        return "";
    }
    icu::DateTimePatternGenerator* gen = icu::DateTimePatternGenerator::createInstance(localeObj, status);
    icu::UnicodeString skeleton("h");
    icu::UnicodeString resultStr = gen->getBestPattern(skeleton, status);
    if (U_FAILURE(status) || gen == nullptr) {
        HILOG_ERROR_I18N("Failed to create DateTimePatternGenerator");
        return "";
    }
    std::string result;
    resultStr.toUTF8String<std::string>(result);
    std::regex pattern0("a"); // a represent morning or afternoon
    std::regex pattern1("h"); // h represent time pattern, eg: hh:mm:ss
    std::regex pattern2("[^01\\s]");
    result = regex_replace(result, pattern0, "1");
    result = regex_replace(result, pattern1, "0");
    result = regex_replace(result, pattern2, "");
    delete gen;
    return result;
}

std::string DateTimeSequence::ModifyOrder(std::string &pattern)
{
    int order[3] = { 0 }; // total 3 elements 'y', 'M'/'L', 'd'
    int lengths[4] = { 0 }; // first elements is the currently found elememnts, thus 4 elements totally.
    bool flag = true;
    for (size_t i = 0; i < pattern.length(); ++i) {
        char ch = pattern[i];
        if (flag && std::isalpha(ch)) {
            ProcessNormal(ch, order, 3, lengths, 4); // 3, 4 are lengths of these arrays
        } else if (ch == '\'') {
            flag = !flag;
        }
    }
    std::unordered_map<char, int> pattern2index = {
        { 'y', 1 },
        { 'M', 2 },
        { 'd', 3 },
    };
    std::string ret;
    for (int i = 0; i < 3; ++i) { // 3 is the size of orders
        auto it = pattern2index.find(order[i]);
        if (it == pattern2index.end()) {
            continue;
        }
        int index = it->second;
        if ((lengths[index] > 0) && (lengths[index] <= 6)) { // 6 is the max length of a filed
            ret.append(lengths[index], order[i]);
        }
        if (i < 2) { // 2 is the size of the order minus one
            ret.append(1, '-');
        }
    }
    return ret;
}

void DateTimeSequence::ProcessNormal(char ch, int *order, size_t orderSize, int *lengths, size_t lengsSize)
{
    char adjust;
    int index = -1;
    if (ch == 'd') {
        adjust = 'd';
        index = 3; // 3 is the index of 'd'
    } else if ((ch == 'L') || (ch == 'M')) {
        adjust = 'M';
        index = 2; // 2 is the index of 'M'
    } else if (ch == 'y') {
        adjust = 'y';
        index = 1;
    } else {
        return;
    }
    if ((index < 0) || (index >= static_cast<int>(lengsSize))) {
        return;
    }
    if (lengths[index] == 0) {
        if (lengths[0] >= 3) { // 3 is the index of order
            return;
        }
        order[lengths[0]] = static_cast<int>(adjust);
        ++lengths[0];
        lengths[index] = 1;
    } else {
        ++lengths[index];
    }
}

} // namespace I18n
} // namespace Global
} // namespace OHOS