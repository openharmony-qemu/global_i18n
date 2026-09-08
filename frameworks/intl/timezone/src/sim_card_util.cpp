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
#include "sim_card_util.h"
#include <dlfcn.h>

namespace OHOS {
namespace Global {
namespace I18n {

const char *SimCardUtil::DLOPEN_SO_NAME = "libzone_util.z.so";

SimCardUtil::SimCardUtil()
{
}

SimCardUtil::~SimCardUtil()
{
}

void SimCardUtil::GetSimCardAndNetworkCountries(std::unordered_set<std::string> &resultSet)
{
    void* handler = dlopen(DLOPEN_SO_NAME, RTLD_NOW);
    if (handler == nullptr) {
        HILOG_ERROR_I18N("GetSimAndNetworkCountries: open %{public}s failed", DLOPEN_SO_NAME);
        return;
    }
    std::string simCardCountryCode = GetISOCountryCode("GlobalI18nGetCountryCodeFromSimCard", handler);
    if (!simCardCountryCode.empty()) {
        resultSet.insert(simCardCountryCode);
    }
    std::string networkCountryCode = GetISOCountryCode("GlobalI18nGetCountryCodeFromNetwork", handler);
    if (!networkCountryCode.empty()) {
        resultSet.insert(networkCountryCode);
    }
    dlclose(handler);
}

std::string SimCardUtil::GetCountryCode()
{
    void* handler = dlopen(DLOPEN_SO_NAME, RTLD_NOW);
    if (handler == nullptr) {
        HILOG_ERROR_I18N("SimCardUtil::GetCountryCode: open %{public}s failed", DLOPEN_SO_NAME);
        return "";
    }
    std::string simCardCountryCode = GetISOCountryCode("GlobalI18nGetCountryCodeFromSimCard", handler);
    if (!simCardCountryCode.empty()) {
        dlclose(handler);
        return simCardCountryCode;
    }
    std::string networkCountryCode = GetISOCountryCode("GlobalI18nGetCountryCodeFromNetwork", handler);
    dlclose(handler);
    if (!networkCountryCode.empty()) {
        return networkCountryCode;
    }
    return "";
}

std::string SimCardUtil::GetISOCountryCode(const char* symbol, void* handler)
{
    GetCountryCodeFunc func = reinterpret_cast<GetCountryCodeFunc>(dlsym(handler, symbol));
    if (!func) {
        HILOG_ERROR_I18N("GetISOCountryCode: find symbol %{public}s failed", symbol);
        return "";
    }
    char res[RECV_CHAR_LENGTH] = { 0 };
    int returnCode = func(res, RECV_CHAR_LENGTH);
    if (returnCode != 0) {
        HILOG_ERROR_I18N("GetISOCountryCode: call %{public}s result:%{public}d", symbol, returnCode);
        return "";
    }
    std::string result(res);
    return result;
}
} // namespace I18n
} // namespace Global
} // namespace OHOS