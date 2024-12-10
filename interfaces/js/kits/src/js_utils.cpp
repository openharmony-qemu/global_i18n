/*
 * Copyright (c) 2024 Huawei Device Co., Ltd.
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
#include "js_utils.h"

namespace OHOS {
namespace Global {
namespace I18n {
napi_value JSUtils::DefaultConstructor(napi_env env, napi_callback_info info)
{
    return nullptr;
}

napi_value JSUtils::CreateEmptyString(napi_env env)
{
    napi_value result;
    std::string emptyStr = "";
    napi_status status = napi_create_string_utf8(env, emptyStr.c_str(), NAPI_AUTO_LENGTH, &result);
    if (status != napi_ok) {
        HILOG_ERROR_I18N("JSUtils: create string js variable failed.");
    }
    return result;
}
} // namespace I18n
} // namespace Global
} // namespace OHOS