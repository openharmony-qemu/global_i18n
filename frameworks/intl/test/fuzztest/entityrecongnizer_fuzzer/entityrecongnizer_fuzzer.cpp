/*
 * Copyright (c) 2022 Huawei Device Co., Ltd.
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except", "in compliance with the License.
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

#include <cstddef>
#include <cstdint>
#include "locale_config.h"
#include "entity_recognizer.h"
#include "entityrecongnizer_fuzzer.h"
#include "date_rule_init.h"
#include "date_time_filter.h"
#include "date_time_matched.h"
#include "date_time_rule.h"
#include "matched_date_time_info.h"

namespace OHOS {
    bool DoSomethingInterestingWithMyAPI(const uint8_t* data, size_t size)
    {
        using namespace Global::I18n;
        if (size < 1) {
            return false;
        }
        std::string input(reinterpret_cast<const char*>(data), size);
        UErrorCode status = U_ZERO_ERROR;
        std::string systemLocale = LocaleConfig::GetSystemLocale();
        icu::Locale locale = icu::Locale::forLanguageTag(systemLocale, status);
        EntityRecognizer* entityRecongnizer = new EntityRecognizer(locale);
        entityRecongnizer->FindEntityInfo(input);

        DateRuleInit* dateRuleInit = new DateRuleInit(input);
        icu::UnicodeString message(input.c_str());
        dateRuleInit->Detect(message);

        DateTimeRule* dateTimeRule = new DateTimeRule(input);
        dateTimeRule->GetUniverseRules();
        dateTimeRule->GetLocalesRules();
        dateTimeRule->GetLocalesRulesBackup();
        dateTimeRule->GetSubRulesMap();
        dateTimeRule->GetSubRules();
        dateTimeRule->GetFilterRules();
        dateTimeRule->GetPastRules();
        dateTimeRule->GetParam();
        dateTimeRule->GetParamBackup();
        dateTimeRule->GetPatternsMap();
        dateTimeRule->CompareLevel(input, input);
        std::unordered_map<std::string, std::string> param =
        {
            {input, input}
        };
        dateTimeRule->Get(param, input);
        dateTimeRule->GetLocale();
        icu::UnicodeString hyphen(input.c_str());
        dateTimeRule->IsRelDates(hyphen, input);
        dateTimeRule->trim(input);
        dateTimeRule->CompareBeginEnd(input, input, false);
        dateTimeRule->SubReplace(input, input, input);
        delete dateTimeRule;
        delete entityRecongnizer;
        delete dateRuleInit;
        return true;
    }

    bool DoSomethingTestWithMyAPI(const uint8_t* data, size_t size)
    {
        using namespace Global::I18n;
        if (size < 1) {
            return false;
        }
        std::string input(reinterpret_cast<const char*>(data), size);
        DateTimeRule* dateTimeRule = new DateTimeRule(input);
        DateTimeFilter* dateTimeFilter = new DateTimeFilter(input, dateTimeRule);
        std::vector<MatchedDateTimeInfo> matches;
        MatchedDateTimeInfo matchedDateTimeInfo = new MatchedDateTimeInfo();
        matches.push_back(matchedDateTimeInfo);
        MatchedDateTimeInfo matchedDateTimeInfo2 = new MatchedDateTimeInfo(size, size, input);
        std::vector<MatchedDateTimeInfo> clearMatches;
        clearMatches.push_back(matchedDateTimeInfo2);
        dateTimeFilter->Filter(hyphen, matches, clearMatches, clearMatches);
        matchedDateTimeInfo.SetIsTimePeriod(size % 2 == 0);
        matchedDateTimeInfo.IsTimePeriod();
        matchedDateTimeInfo.SetBegin(size);
        matchedDateTimeInfo.GetBegin();
        matchedDateTimeInfo.SetEnd(size);
        matchedDateTimeInfo.GetEnd();
        matchedDateTimeInfo.GetRegex();
        matchedDateTimeInfo.GetType();
        matchedDateTimeInfo.SetType(size);
        bool flag = matchedDateTimeInfo < matchedDateTimeInfo2;
        delete dateTimeFilter;
        delete dateTimeRule;
        return flag;
    }

    bool DoDateTimeMatchedTestWithMyAPI(const uint8_t* data, size_t size)
    {
        using namespace Global::I18n;
        if (size < 1) {
            return false;
        }
        std::string input(reinterpret_cast<const char*>(data), size);
        DateTimeMatched* dateTimeMatched = new DateTimeMatched();
        icu::UnicodeString message(input.c_str());
        dateTimeMatched->GetMatchedDateTime(message);
        delete dateTimeMatched;
        return true;
    }
}

/* Fuzzer entry point */
extern "C" int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size)
{
    /* Run your code on data */
    OHOS::DoSomethingInterestingWithMyAPI(data, size);
    OHOS::DoSomethingTestWithMyAPI(data, size);
    OHOS::DoDateTimeMatchedTestWithMyAPI(data, size);
    return 0;
}

