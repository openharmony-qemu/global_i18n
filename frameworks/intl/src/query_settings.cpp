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
#include <dlfcn.h>
#include <filesystem>
#include "hilog/log.h"
#include "query_settings.h"

namespace OHOS {
namespace Global {
namespace I18n {
const char* QuerySettings::LIB_PATH = "libzone_util.z.so";
const char* QuerySettings::QUERY_WITH_PROXY = "QueryWithProxy";

static constexpr OHOS::HiviewDFX::HiLogLabel LABEL = { LOG_CORE, 0xD001E00, "QuerySettings" };
using namespace OHOS::HiviewDFX;

#ifdef SUPPORT_GRAPHICS
bool QuerySettings::SettingsQuery(const std::string& key, std::string& value, sptr<IRemoteObject>& proxy)
{
    if (proxy == nullptr) {
        HiLog::Error(LABEL, "QuerySettings::SettingsQuery proxy==nullptr.");
        return false;
    }
    void* handle = dlopen(QuerySettings::LIB_PATH, RTLD_LAZY);
    if (handle == nullptr) {
        HiLog::Error(LABEL, "Query failed, handle is nullptr.");
        return false;
    }
    I18nQuery query = reinterpret_cast<I18nQuery>(dlsym(handle, QuerySettings::QUERY_WITH_PROXY));
    bool result = query(key, value, proxy);
    if (handle != nullptr) {
        dlclose(handle);
    }
    return result;
}
#endif
} // namespace I18n
} // namespace Global
} // namespace OHOS