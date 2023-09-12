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
#include <cctype>
#include <filesystem>
#include "cpp/src/phonenumbers/phonenumber.h"
#include "cpp/src/phonenumbers/shortnumberinfo.h"
#include "cpp/src/phonenumbers/phonenumberutil.h"
#include "hilog/log.h"
#include "libxml/globals.h"
#include "libxml/tree.h"
#include "libxml/xmlstring.h"
#include "matched_number_info.h"
#include "phone_number_rule.h"

namespace OHOS {
namespace Global {
namespace I18n {
using i18n::phonenumbers::PhoneNumber;
using i18n::phonenumbers::PhoneNumberUtil;
using i18n::phonenumbers::ShortNumberInfo;
static constexpr OHOS::HiviewDFX::HiLogLabel LABEL = { LOG_CORE, 0xD001E00, "PhoneNumberRule" };
using namespace OHOS::HiviewDFX;

std::string PhoneNumberRule::xmlCommonPath = "/system/usr/ohos_locale_config/phonenumber/common.xml";

PhoneNumberRule::PhoneNumberRule(std::string& country)
{
    xmlPath = "/system/usr/ohos_locale_config/phonenumber/" + country + ".xml";
    isFixed = true;
    if (!std::filesystem::exists(xmlPath)) {
        isFixed = false;
    }
    this->commonExit = false;
    this->country = country;
}

PhoneNumberRule::~PhoneNumberRule()
{
}

void PhoneNumberRule::Init()
{
    if (isFixed) {
        InitRule(xmlPath);
    }
    if (commonExit) {
        InitRule(xmlCommonPath);
    }
}

// Load the rules of the corresponding country
void PhoneNumberRule::InitRule(std::string& xmlPath)
{
    xmlKeepBlanksDefault(0);
    xmlDocPtr doc = xmlParseFile(xmlPath.c_str());
    xmlNodePtr cur = xmlDocGetRootElement(doc)->xmlChildrenNode;
    while (cur != nullptr) {
        std::string category = reinterpret_cast<const char*>(cur->name);
        xmlNodePtr rule = cur->xmlChildrenNode;
        while (rule != nullptr && !xmlStrcmp(rule->name, reinterpret_cast<const xmlChar*>("rule"))) {
            xmlNodePtr value = rule->xmlChildrenNode;
            std::string insensitive = reinterpret_cast<char*>(xmlNodeGetContent(value));
            value = value->next;
            if (category == "common_exit") {
                commonExit = (insensitive == "True");
                break;
            }
            std::string type = reinterpret_cast<char*>(xmlNodeGetContent(value));
            value = value->next;
            std::string valid = reinterpret_cast<char*>(xmlNodeGetContent(value));
            value = value->next;
            std::string handle = reinterpret_cast<char*>(xmlNodeGetContent(value));
            icu::UnicodeString content = "";
            while (value->next != nullptr && !xmlStrcmp(value->next->name,
                reinterpret_cast<const xmlChar*>("content"))) {
                value = value->next;
                xmlChar* contentPtr = xmlNodeGetContent(value);
                icu::UnicodeString tempContent = reinterpret_cast<char*>(contentPtr);
                content += tempContent;
                xmlFree(contentPtr);
            }
            if (category == "negative" || category == "common") {
                negativeRules.push_back(new RegexRule(content, valid, handle, insensitive, type));
            } else if (category == "positive") {
                positiveRules.push_back(new RegexRule(content, valid, handle, insensitive, type));
            } else if (category == "border") {
                borderRules.push_back(new RegexRule(content, valid, handle, insensitive, type));
            } else if (category == "codes") {
                codesRules.push_back(new RegexRule(content, valid, handle, insensitive, type));
            }
            rule = rule->next;
        }
        cur = cur->next;
    }
    xmlFreeDoc(doc);
}

std::vector<RegexRule*> PhoneNumberRule::GetBorderRules()
{
    return borderRules;
}

std::vector<RegexRule*> PhoneNumberRule::GetCodesRules()
{
    return codesRules;
}

std::vector<RegexRule*> PhoneNumberRule::GetPositiveRules()
{
    return positiveRules;
}

std::vector<RegexRule*> PhoneNumberRule::GetNegativeRules()
{
    return negativeRules;
}

// Short numbers identification
std::vector<MatchedNumberInfo> PhoneNumberRule::HandleShortPhoneNumbers(icu::UnicodeString& message,
    std::string& country)
{
    PhoneNumber phoneNumber;
    PhoneNumberUtil* phoneNumberUtil = PhoneNumberUtil::GetInstance();
    ShortNumberInfo* shortNumberInfo = new ShortNumberInfo();
    std::vector<MatchedNumberInfo> matchedNumberInfoList;
    UErrorCode shortPatternStatus = U_ZERO_ERROR;
    icu::UnicodeString shortRegex = "(?<!(\\d|\\*|-))\\d{2,6}(?!(\\d|\\*|-|%|分钟|版本))";
    icu::RegexMatcher shortMatch = icu::RegexMatcher(shortRegex, 0, shortPatternStatus);
    shortMatch.reset(message);
    PhoneNumberUtil::ErrorType status = PhoneNumberUtil::NO_PARSING_ERROR;
    while (shortMatch.find(shortPatternStatus)) {
        icu::UnicodeString numberToParse = shortMatch.group(shortPatternStatus);
        std::string stringParse;
        numberToParse.toUTF8String(stringParse);
        status = phoneNumberUtil->ParseAndKeepRawInput(stringParse, country, &phoneNumber);
        if (status != PhoneNumberUtil::NO_PARSING_ERROR) {
            HiLog::Error(LABEL, "PhoneNumberRule: failed to call the ParseAndKeepRawInput.");
            continue;
        }
        // Add the valid short number to the result
        if (shortNumberInfo->IsPossibleShortNumberForRegion(phoneNumber, country)) {
            MatchedNumberInfo matcher;
            matcher.SetBegin(shortMatch.start(shortPatternStatus));
            matcher.SetEnd(shortMatch.end(shortPatternStatus));
            icu::UnicodeString stringShort = shortMatch.group(shortPatternStatus);
            matcher.SetContent(stringShort);
            matchedNumberInfoList.push_back(matcher);
        }
    }
    if (shortNumberInfo != nullptr) {
        delete shortNumberInfo;
    }
    return matchedNumberInfoList;
}
} // namespace I18n
} // namespace Global
} // namespace OHOS