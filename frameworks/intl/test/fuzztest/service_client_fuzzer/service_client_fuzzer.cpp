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
    const static int32_t UID = 3013;
    const static std::vector<TemperatureType> TEMPERATURE_TYPE_VECT = {
        TemperatureType::CELSIUS,
        TemperatureType::FAHRENHEIT,
        TemperatureType::KELVIN};
    const static std::vector<WeekDay> WEEK_DAY_VECT = {
        WeekDay::MON, WeekDay::TUE, WeekDay::WED, WeekDay::THU
        WeekDay::FRI, WeekDay::SAT, WeekDay::SUN};

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
        setuid(UID);
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
        size_t temperatureType = provider.ConsumeIntegralInRange<size_t>(0, 2);

        I18nServiceAbilityClient::SetTemperatureType(TEMPERATURE_TYPE_VECT[temperatureType]);
        size_t weekDay = provider.ConsumeIntegralInRange<size_t>(0, 6);
        I18nServiceAbilityClient::SetFirstDayOfWeek(WEEK_DAY_VECT[weekDay]);
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