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
#include "i18n_hilog.h"

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

void JSUtils::GetOptionValue(napi_env env, napi_value options, const std::string &optionName,
    std::map<std::string, std::string> &map)
{
    napi_value optionValue = nullptr;
    napi_valuetype type = napi_undefined;
    napi_status status = napi_typeof(env, options, &type);
    if (status != napi_ok && type != napi_object) {
        HILOG_ERROR_I18N("Get option failed, option is not an object");
        return;
    }
    bool hasProperty = false;
    status = napi_has_named_property(env, options, optionName.c_str(), &hasProperty);
    if (status != napi_ok || !hasProperty) {
        HILOG_ERROR_I18N("GetOptionValue: Has not named property");
        return;
    }
    status = napi_get_named_property(env, options, optionName.c_str(), &optionValue);
    if (status != napi_ok) {
        HILOG_ERROR_I18N("GetOptionValue: Get named property for %{public}s failed.", optionName.c_str());
        return;
    }
    size_t len = 0;
    status = napi_get_value_string_utf8(env, optionValue, nullptr, 0, &len);
    if (status != napi_ok) {
        HILOG_ERROR_I18N("GetOptionValue: get string length failed");
        return;
    }
    std::vector<char> optionBuf(len + 1);
    status = napi_get_value_string_utf8(env, optionValue, optionBuf.data(), len + 1, &len);
    if (status != napi_ok) {
        HILOG_ERROR_I18N("GetOptionValue: get string value failed");
        return;
    }
    auto ret = map.insert(make_pair(optionName, optionBuf.data()));
    if (!ret.second) {
        HILOG_ERROR_I18N("GetOptionValue: map insert failed");
    }
}

void JSUtils::GetBoolOptionValue(napi_env env, napi_value options, const std::string &optionName,
    std::map<std::string, std::string> &map)
{
    napi_value optionValue = nullptr;
    napi_valuetype type = napi_undefined;
    napi_status status = napi_typeof(env, options, &type);
    if (status != napi_ok && type != napi_object) {
        HILOG_ERROR_I18N("GetBoolOptionValue: Set option failed, option is not an object");
        return;
    }
    bool hasProperty = false;
    status = napi_has_named_property(env, options, optionName.c_str(), &hasProperty);
    if (status != napi_ok || !hasProperty) {
        HILOG_ERROR_I18N("GetBoolOptionValue: Has not named property.");
        return;
    }
    status = napi_get_named_property(env, options, optionName.c_str(), &optionValue);
    if (status != napi_ok) {
        HILOG_ERROR_I18N("GetBoolOptionValue: Get named property for %{public}s failed.", optionName.c_str());
        return;
    }
    bool boolValue = false;
    status = napi_get_value_bool(env, optionValue, &boolValue);
    if (status != napi_ok) {
        HILOG_ERROR_I18N("GetBoolOptionValue: Get bool value failed.");
        return;
    }
    std::string value = boolValue ? "true" : "false";
    auto ret = map.insert(make_pair(optionName, value));
    if (!ret.second) {
        HILOG_ERROR_I18N("GetBoolOptionValue: map insert failed");
    }
}

std::string JSUtils::GetLocaleTag(napi_env env, napi_value argv)
{
    if (argv == nullptr) {
        return "";
    }
    napi_valuetype valueType = napi_valuetype::napi_undefined;
    napi_typeof(env, argv, &valueType);
    if (valueType != napi_valuetype::napi_string) {
        HILOG_ERROR_I18N("GetLocaleTag: Parameter type does not match");
        return "";
    }
    size_t len = 0;
    napi_status status = napi_get_value_string_utf8(env, argv, nullptr, 0, &len);
    if (status != napi_ok) {
        HILOG_ERROR_I18N("GetLocaleTag -> string: Get locale tag length failed");
        return "";
    }
    std::vector<char> buf;
    buf.resize(len + 1);
    status = napi_get_value_string_utf8(env, argv, buf.data(), len + 1, &len);
    if (status != napi_ok) {
        HILOG_ERROR_I18N("GetLocaleTag: Get locale tag failed");
        return "";
    }
    return buf.data();
}
} // namespace I18n
} // namespace Global
} // namespace OHOS