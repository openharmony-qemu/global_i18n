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


extern "C" int GlobalI18nGetCountryCodeFromSimCard(char* res, const int resLength)
{
    std::set<std::string> resultSet;
    std::vector<size_t> soltIds = {0, 1};
    for (size_t i = 0; i < soltIds.size(); i++) {
        Global_I18N_GetCountryCodeFromSimCardInner(resultSet, soltIds[i]);
    }
    std::string resultStr = "";
    for (size_t i = 0; i < resultSet.size(); i++) {
        if (i > 1) {
            resultStr += ",";
        }
        resultStr += resultSet[i];
    }
    return strcpy_s(res, resLength, resultStr.c_str());
}

void GlobalI18nGetCountryCodeFromSimCardInner(std::set<std::string> &resultSet, size_t soltId)
{
#if defined(TEL_CORE_SERVICE_ENABLE)
    std::u16string countryCode;
    int32_t errorCode = DelayedRefSingleton<Telephony::CoreServiceClient>::GetInstance().GetISOCountryCodeForSim(
        soltId, countryCode);
    HILOG_INFO_I18N("SystemLocaleManager: getCountryCodeForSim id:%{public}d, errorCode:%{public}d.",
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