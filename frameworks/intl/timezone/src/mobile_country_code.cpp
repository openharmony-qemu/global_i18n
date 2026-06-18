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
#ifdef TEL_CELLULAR_DATA_ENABLE
#include "cellular_data_client.h"
#endif

using namespace OHOS;

extern "C" int GlobalI18nGetCountryCodeFromSimCard(char* res, const int resLength)
{
    std::string resultStr = OHOS::Global::I18n::GetCountryCodeFromSimCardInner();
    return strcpy_s(res, resLength, resultStr.c_str());
}

extern "C" int GlobalI18nGetCountryCodeFromNetwork(char* res, const int resLength)
{
    std::string resultStr = OHOS::Global::I18n::GetCountryCodeFromNetworkInner();
    return strcpy_s(res, resLength, resultStr.c_str());
}

namespace OHOS {
namespace Global {
namespace I18n {
std::string GetCountryCodeFromSimCardInner()
{
    std::string result;
#if defined(TEL_CORE_SERVICE_ENABLE) && defined(TEL_CELLULAR_DATA_ENABLE)
    int32_t slotId = Telephony::CellularDataClient::GetInstance().GetDefaultCellularDataSlotId();
    if (slotId < 0) {
        return "";
    }
    std::u16string countryCode;
    int32_t errCode = DelayedRefSingleton<Telephony::CoreServiceClient>::GetInstance().GetISOCountryCodeForSim(
        slotId, countryCode);
    if (errCode != 0) {
        HILOG_ERROR_I18N("GetCountryCodeFromSimCardInner:: id:%{public}d, errCode:%{public}d.", slotId, errCode);
        return "";
    }
    result = Str16ToStr8(countryCode);
    if (result.empty()) {
        return "";
    }
    for (size_t i = 0; i < result.length(); ++i) {
        result[i] = toupper(result[i]);
    }
#endif
    return result;
}

std::string GetCountryCodeFromNetworkInner()
{
    std::string result;
#if defined(TEL_CORE_SERVICE_ENABLE) && defined(TEL_CELLULAR_DATA_ENABLE)
    int32_t slotId = Telephony::CellularDataClient::GetInstance().GetDefaultCellularDataSlotId();
    if (slotId < 0) {
        return "";
    }
    std::u16string countryCode;
    int32_t errCode = DelayedRefSingleton<Telephony::CoreServiceClient>::GetInstance()
        .GetIsoCountryCodeForNetwork(slotId, countryCode);
    if (errCode != 0) {
        HILOG_ERROR_I18N("GetCountryCodeFromNetworkInner:: id:%{public}d, errCode:%{public}d.", slotId, errCode);
        return "";
    }
    result = Str16ToStr8(countryCode);
    if (result.empty()) {
        return "";
    }
    for (size_t i = 0; i < result.length(); ++i) {
        result[i] = toupper(result[i]);
    }
#endif
    return result;
}
}
}
}