/*
 * Copyright (c) 2023 Huawei Device Co., Ltd.
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

#include <climits>
#include <set>
#include "cpp/src/phonenumbers/shortnumberinfo.h"
#include "hilog/log.h"
#include "regex_rule.h"
#include "phone_number_matched.h"

namespace OHOS {
namespace Global {
namespace I18n {
using i18n::phonenumbers::PhoneNumber;
using i18n::phonenumbers::ShortNumberInfo;
static constexpr OHOS::HiviewDFX::HiLogLabel LABEL = { LOG_CORE, 0xD001E00, "PhoneNumberMatched" };
using namespace OHOS::HiviewDFX;

const int PhoneNumberMatched::CONTAIN = 9;
const int PhoneNumberMatched::CONTAIN_OR_INTERSECT = 8;
const UChar32 PhoneNumberMatched::REPLACE_CHAR = 'A';

PhoneNumberMatched::PhoneNumberMatched(std::string& country)
{
    phoneNumberRule = new PhoneNumberRule(country);
    this->country = country;
    if (phoneNumberRule != nullptr) {
        phoneNumberRule->Init();
    }
}

PhoneNumberMatched::~PhoneNumberMatched()
{
    if (phoneNumberRule != nullptr) {
        delete phoneNumberRule;
    }
    phoneNumberRule = nullptr;
}

std::string PhoneNumberMatched::GetCountry()
{
    return country;
}

std::vector<int> PhoneNumberMatched::GetMatchedPhoneNumber(std::string& messageStr)
{
    icu::UnicodeString message = messageStr.c_str();
    PhoneNumberUtil* phoneNumberUtil = PhoneNumberUtil::GetInstance();
    icu::UnicodeString convertQanStr = ConvertQanChar(message);
    if (!phoneNumberRule->isFixed) {
        return DealWithoutFixed(convertQanStr, country, phoneNumberUtil);
    }
    icu::UnicodeString filteredString = HandleNegativeRule(convertQanStr, phoneNumberRule);
    std::vector<MatchedNumberInfo> matchedNumberInfoList = GetPossibleNumberInfos(country,
        phoneNumberUtil, convertQanStr, filteredString);
    // Mark the searched area
    for (size_t i = 0; i < searchedRegion.size(); i++) {
        ReplaceSpecifiedPos(filteredString, searchedRegion[i].first, searchedRegion[i].second);
    }
    std::vector<MatchedNumberInfo> shortList = phoneNumberRule->HandleShortPhoneNumbers(filteredString, country);
    if (shortList.size() != 0) {
        matchedNumberInfoList.insert(matchedNumberInfoList.end(), shortList.begin(), shortList.end());
    }
    matchedNumberInfoList = DeleteRepeatedInfo(matchedNumberInfoList);
    for (auto& matchedNumberInfo : matchedNumberInfoList) {
        DealNumberWithOneBracket(matchedNumberInfo);
    }
    return DealResult(matchedNumberInfoList);
}

// replace full-width symbols with half-width symbols
icu::UnicodeString PhoneNumberMatched::ConvertQanChar(icu::UnicodeString& instr)
{
    icu::UnicodeString result = "";
    icu::UnicodeString fwchstrFirst = "：／．＼∕，！（）？﹡；：﹣—－【】－＋＝｛％｝１２３４５６７８９０ａｂｃｄｅｆｇｈｉ";
    icu::UnicodeString fwchstrSecond = "ｊｋｌｍｎｏｐｑｒｓｔｕｖｗｘｙｚＡＢＣＤＥＦＧＨＩＪＫＬＭＮＯＰＱＲＳＴＵＶＷＸＹＺ";
    icu::UnicodeString fwchstr = fwchstrFirst + fwchstrSecond;
    icu::UnicodeString hwchstrFirst = ":/.\\/,!()?*;:---[]-+={%}1234567890abcdefghi";
    icu::UnicodeString hwchstrSecond = "jklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ";
    icu::UnicodeString hwchstr = hwchstrFirst + hwchstrSecond;
    int len = instr.length();
    for (int i = 0; i < len; i++) {
        int index = fwchstr.indexOf(instr[i]);
        if (index == -1) {
            result += instr[i];
        } else {
            result += hwchstr[index];
        }
    }
    return result;
}

std::vector<int> PhoneNumberMatched::DealWithoutFixed(icu::UnicodeString& message, std::string& country,
    PhoneNumberUtil* util)
{
    std::vector<PhoneNumberMatch*> matchList = FindNumbers(country, util, message);
    std::vector<MatchedNumberInfo> result;
    for (auto& match : matchList) {
        MatchedNumberInfo info;
        icu::UnicodeString content = match->raw_string().c_str();
        PhoneNumber phoneNumber = match->number();
        if (util->IsValidNumber(phoneNumber)) {
            info.SetBegin(match->start());
            info.SetEnd(match->end());
            info.SetContent(content);
            result.push_back(info);
        }
        if (match != nullptr) {
            delete match;
        }
    }
    // Mark the searched area
    for (size_t i = 0; i < searchedRegion.size(); i++) {
        ReplaceSpecifiedPos(message, searchedRegion[i].first, searchedRegion[i].second);
    }
    std::vector<MatchedNumberInfo> shortResult = FindShortNumbers(country, util, message);
    // Merge result
    if (shortResult.size() != 0) {
        result.insert(result.end(), shortResult.begin(), shortResult.end());
    }
    result = DeleteRepeatedInfo(result);
    for (auto& res: result) {
        DealNumberWithOneBracket(res);
    }
    return DealResult(result);
}

// Filtering text using negative rules
icu::UnicodeString PhoneNumberMatched::HandleNegativeRule(icu::UnicodeString& src, PhoneNumberRule* phoneNumberRule)
{
    std::vector<RegexRule*> rules = phoneNumberRule->GetNegativeRules();
    icu::UnicodeString ret = src;
    for (RegexRule* rule : rules) {
        icu::RegexPattern* pattern = rule->GetPattern();
        UErrorCode status = U_ZERO_ERROR;
        icu::RegexMatcher* matcher = pattern->matcher(src, status);
        while (matcher->find()) {
            int start = matcher->start(status);
            int end = matcher->end(status);
            ReplaceSpecifiedPos(ret, start, end);
        }
    }
    return ret;
}

// Replace the characters in the specified area with REPLACE_CHAR
void PhoneNumberMatched::ReplaceSpecifiedPos(icu::UnicodeString& chs, int start, int end)
{
    if (start < end) {
        int len = chs.length();
        for (int i = 0; i < len; i++) {
            if (i >= start && i < end) {
                chs.replace(i, 1, PhoneNumberMatched::REPLACE_CHAR);
            }
        }
    }
}

// Short numbers identification
std::vector<MatchedNumberInfo> PhoneNumberMatched::FindShortNumbers(std::string& country, PhoneNumberUtil* util,
    icu::UnicodeString& message)
{
    std::vector<MatchedNumberInfo> matchedNumberInfoList;
    UErrorCode status = U_ZERO_ERROR;
    icu::UnicodeString regex = "(?<!(\\d|\\*|-))\\d{2,6}(?!(\\d|\\*|-|%))";
    ShortNumberInfo* shortNumberInfo = new ShortNumberInfo();
    if (shortNumberInfo == nullptr) {
        HiLog::Error(LABEL, "shortNumberInfo construct failed.");
        return matchedNumberInfoList;
    }
    icu::RegexPattern* shortPattern = icu::RegexPattern::compile(regex, 0, status);
    if (shortPattern == nullptr) {
        HiLog::Error(LABEL, "shortPattern construct failed.");
        return matchedNumberInfoList;
    }
    icu::RegexMatcher* shortMatch = shortPattern->matcher(message, status);
    if (shortMatch == nullptr) {
        HiLog::Error(LABEL, "shortPattern  matcher failed.");
        return matchedNumberInfoList;
    }
    while (shortMatch->find(status)) {
        icu::UnicodeString numberToParse = shortMatch->group(status);
        std::string stringParse;
        numberToParse.toUTF8String(stringParse);
        PhoneNumber phoneNumber;
        PhoneNumberUtil::ErrorType errorType = util->ParseAndKeepRawInput(stringParse, country, &phoneNumber);
        if (errorType != PhoneNumberUtil::NO_PARSING_ERROR) {
            HiLog::Error(LABEL, "PhoneNumberRule: failed to call the ParseAndKeepRawInput.");
            continue;
        }
        // Add the valid short number to the result
        if (shortNumberInfo->IsPossibleShortNumberForRegion(phoneNumber, country)) {
            MatchedNumberInfo matcher;
            matcher.SetBegin(shortMatch->start(status));
            matcher.SetEnd(shortMatch->end(status));
            icu::UnicodeString stringShort = shortMatch->group(status);
            matcher.SetContent(stringShort);
            matchedNumberInfoList.push_back(matcher);
        }
    }
    delete shortNumberInfo;
    delete shortPattern;
    delete shortMatch;
    return matchedNumberInfoList;
}

// Add the phone number that may be correct, and return true if successful
bool PhoneNumberMatched::AddPhoneNumber(std::string& number, int start, std::vector<PhoneNumberMatch*>& matchList,
    std::string& country, PhoneNumberUtil* util)
{
    PhoneNumber phoneNumber;
    int lenNumber = 7;
    icu::UnicodeString uNumber = number.c_str();
    if (RegexRule::CountDigits(uNumber) < lenNumber) {
        return false;
    }
    PhoneNumberUtil::ErrorType parseStatus = util->Parse(number, country, &phoneNumber);
    if (parseStatus != PhoneNumberUtil::NO_PARSING_ERROR) {
        return false;
    }
    UChar32 space = ' ';
    UChar32 slash = '/';
    // Add to matchList if phone number is not delimited, or valid
    if ((uNumber.indexOf(space) == -1 && uNumber.indexOf(slash) == -1) || util->IsValidNumber(phoneNumber)) {
        PhoneNumberMatch* match = new PhoneNumberMatch(start, number, phoneNumber);
        matchList.push_back(match);
        return true;
    }
    return false;
}

// Add the valid phone number
std::vector<PhoneNumberMatch*> PhoneNumberMatched::FindNumbers(std::string& country, PhoneNumberUtil* util,
    icu::UnicodeString& filteredString)
{
    std::vector<PhoneNumberMatch*> matchList;
    UErrorCode status = U_ZERO_ERROR;
    icu::UnicodeString regex = "[\\+\\(\\d][\\d \\(\\)./-]{5,}[\\d]";
    icu::RegexPattern* numberPattern = icu::RegexPattern::compile(regex, 0, status);
    icu::RegexMatcher* numberMatcher = numberPattern->matcher(filteredString, status);
    while (numberMatcher->find(status)) {
        int32_t start = numberMatcher->start(status);
        int32_t end = numberMatcher->end(status);
        icu::UnicodeString uNumber = filteredString.tempSubString(start, end - start);
        std::string number;
        uNumber.toUTF8String(number);
        // if the entire phone number is invalid, identify each segment that is separated
        if (!AddPhoneNumber(number, start, matchList, country, util)) {
            int searchStart = 0;
            UChar32 space = ' ';
            UChar32 slash = '/';
            if (uNumber.indexOf(space, searchStart) == -1 && uNumber.indexOf(slash, searchStart) == -1) {
                continue;
            }
            while (uNumber.indexOf(space, searchStart) != -1 || uNumber.indexOf(slash, searchStart) != -1) {
                int phoneStart = searchStart;
                int indexSpace = uNumber.indexOf(space, searchStart);
                int indexSlash = uNumber.indexOf(slash, searchStart);
                int phoneEnd =
                    (indexSpace == -1 || (indexSlash != -1 && indexSlash < indexSpace)) ? indexSlash : indexSpace;
                searchStart = phoneEnd + 1;
                std::string tempNumber = number.substr(phoneStart, phoneEnd - phoneStart);
                AddPhoneNumber(tempNumber, phoneStart + start, matchList, country, util);
            }
            // identify the last segment
            std::string lastStr = number.substr(searchStart);
            AddPhoneNumber(lastStr, searchStart + start, matchList, country, util);
        }
    }
    return matchList;
}

// Handing the situation of shortnumber/shortnumber.
bool PhoneNumberMatched::HandleWithShortAndShort(std::vector<MatchedNumberInfo>& result, std::string& country,
    MatchedNumberInfo& info, int contentStart, int contentEnd)
{
    if (country == "CN") {
        UErrorCode status = U_ZERO_ERROR;
        icu::RegexMatcher* pattern =
            new icu::RegexMatcher("(?<![-\\d])\\d{5,6}[/|]\\d{5,6}(?![-\\d])", 0, status);
        icu::UnicodeString str = "";
        if (info.GetContent()[0] == '(' || info.GetContent()[0] == '[') {
            str = info.GetContent().tempSubString(1);
        } else {
            str = info.GetContent();
        }
        pattern->reset(str);
        if (!pattern->find()) {
            result.push_back(info);
            searchedRegion.push_back({contentStart, contentEnd});
            return true;
        }
    } else {
        result.push_back(info);
        searchedRegion.push_back({contentStart, contentEnd});
    }
    return false;
}

// Get possible phone number
std::vector<MatchedNumberInfo> PhoneNumberMatched::GetPossibleNumberInfos(std::string& country, PhoneNumberUtil* util,
    icu::UnicodeString& src, icu::UnicodeString& filteredString)
{
    std::vector<MatchedNumberInfo> result;
    std::vector<PhoneNumberMatch*> matchList = FindNumbers(country, util, filteredString);
    for (auto match : matchList) {
        if (!HandleBorderRule(match, filteredString, phoneNumberRule)) {
            continue;
        }
        PhoneNumberMatch* delMatch = HandleCodesRule(match, src, phoneNumberRule);
        if (delMatch == nullptr) {
            continue;
        }
        icu::UnicodeString content = delMatch->raw_string().c_str();
        int contentStart = delMatch->start();
        int contentEnd = delMatch->end();
        if (util->IsValidNumber(delMatch->number())) {
            MatchedNumberInfo info;
            info.SetBegin(contentStart);
            info.SetEnd(contentEnd);
            info.SetContent(content);
            bool flag = HandleWithShortAndShort(result, country, info, contentStart, contentEnd);
            if (flag) {
                continue;
            }
        }
        std::vector<MatchedNumberInfo> posList = HandlePositiveRule(delMatch, filteredString, phoneNumberRule);
        if (posList.size() != 0) {
            result.insert(result.end(), posList.begin(), posList.end());
        }
    }
    return result;
}

// Remove duplicate results
std::vector<MatchedNumberInfo> PhoneNumberMatched::DeleteRepeatedInfo(std::vector<MatchedNumberInfo>& list)
{
    std::set<MatchedNumberInfo> set;
    std::vector<MatchedNumberInfo> ret;
    for (auto info : list) {
        if (set.find(info) == set.end()) {
            ret.push_back(info);
        }
        set.insert(info);
    }
    return ret;
}

// Process the case where the phone number starts with one bracket
void PhoneNumberMatched::DealNumberWithOneBracket(MatchedNumberInfo& info)
{
    icu::UnicodeString message = info.GetContent();
    if (IsNumberWithOneBracket(message)) {
        info.SetBegin(info.GetBegin() + 1);
        icu::UnicodeString content = info.GetContent().tempSubString(1);
        info.SetContent(content);
    }
}

// check whether the bracket at the start position are redundant
bool PhoneNumberMatched::IsNumberWithOneBracket(icu::UnicodeString& message)
{
    if (message != "") {
        int numLeft = 0;
        int numRight = 0;
        int len = message.length();
        for (int i = 0; i < len; i++) {
            if (message[i] == '(' || message[i] == '[') {
                numLeft++;
            }
            if (message[i] == ')' || message[i] == ']') {
                numRight++;
            }
        }
        if (numLeft > numRight && (message[0] == '(' || message[0] == '[')) {
            return true;
        }
    }
    return false;
}

std::vector<int> PhoneNumberMatched::DealResult(std::vector<MatchedNumberInfo>& matchedNumberInfoList)
{
    std::vector<int> result;
    int length = matchedNumberInfoList.size();
    if (length == 0) {
        result.push_back(0);
    } else {
        int posNumber = 2;
        int posStart = 1;
        int posEnd = 2;
        result.resize(posNumber * length + 1);
        result[0] = length;
        for (int i = 0; i < length; i++) {
            result[posNumber * i + posStart] = matchedNumberInfoList[i].GetBegin();
            result[posNumber * i + posEnd] = matchedNumberInfoList[i].GetEnd();
        }
    }
    return result;
}

// Filter result based on Border rule
bool PhoneNumberMatched::HandleBorderRule(PhoneNumberMatch* match, icu::UnicodeString& message,
    PhoneNumberRule* phoneNumberRule)
{
    std::vector<RegexRule*> rules = phoneNumberRule->GetBorderRules();
    if (rules.size() == 0) {
        return true;
    }
    int begin = match->start();
    int end = match->end();
    // chenk the 10 characters before and after the phone number
    int beginSubTen = begin - 10 < 0 ? 0 : begin - 10;
    int endAddTen = end + 10 > message.length() ? message.length() : end + 10;
    icu::UnicodeString borderStr = message.tempSubString(beginSubTen, endAddTen - beginSubTen);
    for (RegexRule* rule : rules) {
        icu::RegexPattern* pattern = rule->GetPattern();
        UErrorCode status;
        icu::RegexMatcher* mat = pattern->matcher(borderStr, status);
        int type = rule->GetType();
        while (mat->find()) {
            int borderBegin = mat->start(status) + beginSubTen;
            int borderEnd = mat->end(status) + beginSubTen;
            bool isDel = false;
            if (type == PhoneNumberMatched::CONTAIN && borderBegin <= begin && end <= borderEnd) {
                isDel = true;
            } else if (type == PhoneNumberMatched::CONTAIN_OR_INTERSECT && ((borderBegin <= begin &&
                end <= borderEnd) || (borderBegin < begin && begin < borderEnd && borderEnd < end) ||
                (begin < borderBegin && borderBegin < end && end < borderEnd))) {
                isDel = true;
            }
            if (isDel) {
                return false;
            }
        }
    }
    return true;
}

// Filter result based on Codes rule
PhoneNumberMatch* PhoneNumberMatched::HandleCodesRule(PhoneNumberMatch* phoneNumberMatch, icu::UnicodeString& message,
    PhoneNumberRule* phoneNumberRule)
{
    PhoneNumberMatch* match = phoneNumberMatch;
    std::vector<RegexRule*> rules = phoneNumberRule->GetCodesRules();
    bool isVaild = true;
    if (rules.size() != 0) {
        for (RegexRule* rule : rules) {
            PhoneNumberMatch* tempmatch = rule->IsValid(match, message);
            if (tempmatch == nullptr) {
                isVaild = false;
                break;
            } else {
                match = tempmatch;
            }
        }
    }
    if (isVaild) {
        return match;
    } else {
        return nullptr;
    }
}

// Add phone numbers that meet the positive rule to the result
std::vector<MatchedNumberInfo> PhoneNumberMatched::HandlePositiveRule(PhoneNumberMatch* match,
    icu::UnicodeString& message, PhoneNumberRule* phoneNumberRule)
{
    std::vector<RegexRule*> rules = phoneNumberRule->GetPositiveRules();
    if (rules.size() == 0) {
        return {};
    }
    icu::UnicodeString rawString = match->raw_string().c_str();
    icu::UnicodeString str = DealStringWithOneBracket(rawString);
    for (RegexRule* rule : rules) {
        icu::RegexPattern* pattern = rule->GetPattern();
        UErrorCode status = U_ZERO_ERROR;
        icu::RegexMatcher* mat1 = pattern->matcher(str, status);
        if (mat1->find(status)) {
            std::vector<MatchedNumberInfo> infoList = rule->Handle(match, message);
            return infoList;
        }
        icu::RegexMatcher* mat2 = pattern->matcher(message, status);
        mat2->reset(message);
        if (mat2->find()) {
            std::vector<MatchedNumberInfo> infoList = rule->Handle(match, message);
            if (infoList.size() == 0) {
                continue;
            }
            return infoList;
        }
    }
    return {};
}

icu::UnicodeString PhoneNumberMatched::DealStringWithOneBracket(icu::UnicodeString& message)
{
    if (IsNumberWithOneBracket(message)) {
        return message.tempSubString(1);
    }
    return message;
}
} // namespace I18n
} // namespace Global
} // namespace OHOS