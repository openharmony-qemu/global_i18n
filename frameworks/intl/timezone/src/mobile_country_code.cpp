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

#include "mobile_country_code.h"
#include "i18n_hilog.h"
#include <cctype>
#include <cstdio>
#include <unistd.h>
#include <set>
#include "securec.h"

#ifdef TEL_CORE_SERVICE_ENABLE
#include "core_service_client.h"
#endif

using namespace OHOS;

extern "C" int GlobalI18nGetCountryCodeFromSimCard(char* res, const int resLength)
{
    std::set<std::string> resultSet;
    std::vector<size_t> soltIds = {0, 1};
    for (size_t i = 0; i < soltIds.size(); i++) {
        GlobalI18nGetCountryCodeFromSimCardInner(resultSet, soltIds[i]);
    }
    std::string resultStr = GlobalI18nJoinString(resultSet);
    return strcpy_s(res, resLength, resultStr.c_str());
}

extern "C" int GlobalI18nGetCountryCodeFromNetwork(char* res, const int resLength)
{
    std::set<std::string> resultSet;
    std::vector<size_t> soltIds = {0, 1};
    for (size_t i = 0; i < soltIds.size(); i++) {
        GlobalI18nGetCountryCodeFromNetworkInner(resultSet, soltIds[i]);
    }
    std::string resultStr = GlobalI18nJoinString(resultSet);
    return strcpy_s(res, resLength, resultStr.c_str());
}

std::string GlobalI18nJoinString(std::set<std::string> &resultSet)
{
    std::string resultStr = "";
    for (auto it = resultSet.begin(); it != resultSet.end(); ++it) {
        if (it != resultSet.begin()) {
            resultStr += ",";
        }
        resultStr += *it;
    }
    return resultStr;
}

void GlobalI18nGetCountryCodeFromSimCardInner(std::set<std::string> &resultSet, size_t soltId)
{
#if defined(TEL_CORE_SERVICE_ENABLE)
    std::u16string countryCode;
    int32_t errorCode = DelayedRefSingleton<Telephony::CoreServiceClient>::GetInstance().GetISOCountryCodeForSim(
        soltId, countryCode);
    HILOG_INFO_I18N("GlobalI18nGetCountryCodeFromSimCardInner id:%{public}zu, errorCode:%{public}d.",
        soltId, errorCode);
    std::string result = Str16ToStr8(countryCode);
    if (result.empty()) {
        return;
    }
    for (size_t i = 0; i < result.length(); ++i) {
        result[i] = toupper(result[i]);
    }
    resultSet.insert(result);
#endif
}

void GlobalI18nGetCountryCodeFromNetworkInner(std::set<std::string> &resultSet, size_t soltId)
{
#if defined(TEL_CORE_SERVICE_ENABLE)
    std::u16string countryCode;
    int32_t errorCode = DelayedRefSingleton<Telephony::CoreServiceClient>::GetInstance()
        .GetIsoCountryCodeForNetwork(soltId, countryCode);
    HILOG_INFO_I18N("GlobalI18nGetCountryCodeFromNetworkInner id:%{public}zu, errorCode:%{public}d.",
        soltId, errorCode);
    std::string result = Str16ToStr8(countryCode);
    if (result.empty()) {
        return;
    }
    for (size_t i = 0; i < result.length(); ++i) {
        result[i] = toupper(result[i]);
    }
    resultSet.insert(result);
#endif
}