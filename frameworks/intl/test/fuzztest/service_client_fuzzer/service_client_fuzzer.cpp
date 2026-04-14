/*
 * Copyright (c) 2026 Huawei Device Co., Ltd.
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

#include <cstddef>
#include <cstdint>
#include <string>
#include <unordered_map>
#include <fuzzer/FuzzedDataProvider.h>
#include "i18n_service_ability_client.h"
#include "i18n_types.h"
#include "holiday_manager.h"
#include "accesstoken_kit.h"
#include "token_setproc.h"
#include "nativetoken_kit.h"
#include "service_client_fuzzer.h"

namespace OHOS {
    void AddChangeNativeTokenTypeAndPermission()
    {
        const char* permissions[] = {
            "ohos.permission.UPDATE_CONFIGURATION"
        };
        NativeTokenInfoParams infoInstance = {
            .dcapsNum = 0,
            .permsNum = sizeof(permissions) / sizeof(permissions[0]),
            .aclsNum = 0,
            .dcaps = nullptr,
            .perms = permissions,
            .acls = nullptr,
            .aplStr = "system_basic",
        };
        infoInstance.processName = "ServiceClientFuzzTest";
        uint64_t tokenId = GetAccessTokenId(&infoInstance);
        printf("Get tokenid is %lu \r\n", tokenId);
        int32_t setSelfErrorCode = SetSelfTokenID(tokenId);
        if (setSelfErrorCode != 0) {
            printf("SetSelfTokenID() Failed, return code is %lu \r\n", setSelfErrorCode);
            return;
        }
        seteuid(0);
        int32_t ret = OHOS::Security::AccessToken::AccessTokenKit::ReloadNativeTokenInfo();
        printf("ReloadNativeTokenInfo result is %d \r\n", ret);
        setuid(3013);
    }

    TemperatureType GetTemperatureTypeByIndex(size_t index)
    {
        switch (index) {
            case 1: return TemperatureType::CELSIUS;
            case 2: return TemperatureType::FAHRENHEIT;
            case 3: return TemperatureType::KELVIN;
            default: throw std::out_of_range("Index out of range");
        }
    }

    WeekDay GetWeekDayByIndex(size_t index)
    {
        switch (index) {
            case 1: return WeekDay::MON;
            case 2: return WeekDay::TUE;
            case 3: return WeekDay::WED;
            case 4: return WeekDay::THU;
            case 5: return WeekDay::FRI;
            case 6: return WeekDay::SAT;
            case 7: return WeekDay::SUN;
            default: throw std::out_of_range("Index out of range");
        }
    }

    void DoSomethingInterestingWithMyAPI(const uint8_t* data, size_t size)
    {
        using namespace Global::I18n;
        FuzzedDataProvider provider(data, size);
        std::string input = provider.ConsumeRandomLengthString();
        size_t newSize = provider.ConsumeIntegral<size_t>();

        I18nServiceAbilityClient::SetSystemLanguage(input);
        I18nServiceAbilityClient::SetSystemRegion(input);
        I18nServiceAbilityClient::SetSystemLocale(input);
        I18nServiceAbilityClient::Set24HourClock(input);
        I18nServiceAbilityClient::SetSystemCollation(input);
        I18nServiceAbilityClient::SetSystemNumberingSystem(input);
        I18nServiceAbilityClient::SetSystemNumberPattern(input);
        I18nServiceAbilityClient::SetSystemMeasurement(input);
        I18nServiceAbilityClient::SetSystemNumericalDatePattern(input);

        I18nServiceAbilityClient::SetUsingLocalDigit(newSize % 2 == 0);
        int32_t intValue = static_cast<int32_t>(newSize);
        I18nServiceAbilityClient::AddPreferredLanguage(input, intValue);
        I18nServiceAbilityClient::RemovePreferredLanguage(intValue);
        I18nServiceAbilityClient::GetLanguageFromUserId(intValue);

        std::unordered_map<std::string, std::string> mapValue;
        I18nServiceAbilityClient::GetSystemCollations(mapValue);
        I18nServiceAbilityClient::GetUsingCollation(input);
        I18nServiceAbilityClient::GetSystemNumberingSystems(mapValue);
        I18nServiceAbilityClient::GetUsingNumberingSystem(input);
        I18nServiceAbilityClient::GetSystemNumberPatterns(mapValue);
        I18nServiceAbilityClient::GetUsingNumberPattern(input);
        I18nServiceAbilityClient::GetSystemMeasurements(mapValue);
        I18nServiceAbilityClient::GetUsingMeasurement(input);
        I18nServiceAbilityClient::GetSystemNumericalDatePatterns(mapValue);
        I18nServiceAbilityClient::GetUsingNumericalDatePattern(input);
        size_t temperatureType = provider.ConsumeIntegralInRange<size_t>(1, 3);
        I18nServiceAbilityClient::SetTemperatureType(GetTemperatureTypeByIndex(temperatureType));
        size_t weekDay = provider.ConsumeIntegralInRange<size_t>(1, 7);
        I18nServiceAbilityClient::SetFirstDayOfWeek(GetWeekDayByIndex(weekDay));
        std::unque_ptr<HolidayManager> holidayManager = std::make_unique<HolidayManager>(input.c_str());
    }
}

/* Fuzzer entry point */
extern "C" int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size)
{
    AddChangeNativeTokenTypeAndPermission();
    OHOS::DoSomethingInterestingWithMyAPI(data, size);
    return 0;
}