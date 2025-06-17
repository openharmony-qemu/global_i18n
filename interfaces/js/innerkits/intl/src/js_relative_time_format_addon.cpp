/*
 * Copyright (c) 2025 Huawei Device Co., Ltd.
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
#include "js_relative_time_format_addon.h"

#include <cmath>
#include "error_util.h"
#include "i18n_hilog.h"
#include "js_utils.h"
#include "locale_helper.h"
#include "utils.h"

namespace OHOS {
namespace Global {
namespace I18n {
static thread_local napi_ref g_JsRelativeTimeFormatConstructor = nullptr;

JSRelativeTimeFormatAddon::JSRelativeTimeFormatAddon()
{
}

JSRelativeTimeFormatAddon::~JSRelativeTimeFormatAddon()
{
}

void JSRelativeTimeFormatAddon::Destructor(napi_env env, void *nativeObject, void *hint)
{
    if (!nativeObject) {
        return;
    }
    delete reinterpret_cast<JSRelativeTimeFormatAddon *>(nativeObject);
    nativeObject = nullptr;
}

napi_value JSRelativeTimeFormatAddon::InitJsRelativeTimeFormat(napi_env env, napi_value exports)
{
    napi_property_descriptor properties[] = {
        DECLARE_NAPI_FUNCTION("format", Format),
        DECLARE_NAPI_FUNCTION("formatToParts", FormatToParts),
        DECLARE_NAPI_FUNCTION("resolvedOptions", GetNumberResolvedOptions),
        DECLARE_NAPI_STATIC_FUNCTION("supportedLocalesOf", SupportedLocalesOf),
    };

    napi_value constructor = nullptr;
    napi_status status = napi_define_class(env, "RelativeTimeFormat", NAPI_AUTO_LENGTH,
        ReltiveTimeFormatConstructor, nullptr,
        sizeof(properties) / sizeof(napi_property_descriptor), properties, &constructor);
    if (status != napi_ok) {
        HILOG_ERROR_I18N("InitJsRelativeTimeFormat: Define class failed when init JsRelativeTimeFormat");
        return nullptr;
    }
    status = napi_set_named_property(env, exports, "RelativeTimeFormat", constructor);
    if (status != napi_ok) {
        HILOG_ERROR_I18N("InitJsRelativeTimeFormat: Set property failed when init JsRelativeTimeFormat");
        return nullptr;
    }
    status = napi_create_reference(env, constructor, 1, &g_JsRelativeTimeFormatConstructor);
    if (status != napi_ok || g_JsRelativeTimeFormatConstructor == nullptr) {
        HILOG_ERROR_I18N("InitJsRelativeTimeFormat: napi_create_reference failed");
        return nullptr;
    }
    return exports;
}

napi_value JSRelativeTimeFormatAddon::ReltiveTimeFormatConstructor(napi_env env, napi_callback_info info)
{
    size_t argc = 2;
    napi_value argv[2] = { nullptr };
    napi_value thisVar = nullptr;
    void *data = nullptr;
    napi_status status = napi_get_cb_info(env, info, &argc, argv, &thisVar, &data);
    if (status != napi_ok) {
        HILOG_ERROR_I18N("ReltiveTimeFormatConstructor: Get parameter info failed");
        return nullptr;
    }
    napi_value new_target;
    status = napi_get_new_target(env, info, &new_target);
    if (status == napi_pending_exception || new_target == nullptr) {
        return CreateRelativeTimeFormatWithoutNew(env, argc, argv);
    }
    std::vector<std::string> localeTags;
    if (argc > 0) {
        int32_t code = 0;
        std::vector<std::string> localeArray = JSUtils::GetLocaleArray(env, argv[0], code);
        if (code != 0) {
            return nullptr;
        }
        localeTags.assign(localeArray.begin(), localeArray.end());
    } else {
        localeTags.push_back("en-US");
    }
    std::map<std::string, std::string> map = {};
    if (argc > 1) {
        GetRelativeTimeOptionValues(env, argv[1], map);
    }
    JSRelativeTimeFormatAddon* obj = new (std::nothrow) JSRelativeTimeFormatAddon();
    if (obj == nullptr) {
        HILOG_ERROR_I18N("RelativeTimeFormatConstructor: Create JSRelativeTimeFormatAddon failed");
        return nullptr;
    }
    status =
        napi_wrap(env, thisVar, reinterpret_cast<void *>(obj), JSRelativeTimeFormatAddon::Destructor,
        nullptr, nullptr);
    if (status != napi_ok) {
        delete obj;
        HILOG_ERROR_I18N("RelativeTimeFormatConstructor: Wrap JSRelativeTimeFormatAddon failed");
        return nullptr;
    }
    if (!obj->InitRelativeTimeFormatContext(env, info, localeTags, map)) {
        delete obj;
        HILOG_ERROR_I18N("Init RelativeTimeFormat failed");
        return nullptr;
    }
    return thisVar;
}

void JSRelativeTimeFormatAddon::GetRelativeTimeOptionValues(napi_env env,
    napi_value options, std::map<std::string, std::string> &map)
{
    JSUtils::GetOptionValue(env, options, "localeMatcher", map);
    JSUtils::GetOptionValue(env, options, "numeric", map);
    JSUtils::GetOptionValue(env, options, "style", map);
    JSUtils::GetOptionValue(env, options, "numberingSystem", map);
}

bool JSRelativeTimeFormatAddon::InitRelativeTimeFormatContext(napi_env env, napi_callback_info info,
    std::vector<std::string> localeTags, std::map<std::string, std::string> &map)
{
    relativetimefmt_ = std::make_unique<RelativeTimeFormat>(localeTags, map);
    return relativetimefmt_ != nullptr;
}

napi_value JSRelativeTimeFormatAddon::CreateRelativeTimeFormatWithoutNew(napi_env env, size_t argc,
    napi_value *argv)
{
    napi_value exception;
    napi_status status = napi_get_and_clear_last_exception(env, &exception);
    if (status != napi_ok) {
        HILOG_ERROR_I18N("CreateRelativeTimeFormatWithoutNew: napi_get_and_clear_last_exception failed");
        return nullptr;
    }
    if (g_JsRelativeTimeFormatConstructor == nullptr) {
        HILOG_ERROR_I18N("CreateRelativeTimeFormatWithoutNew: g_JsRelativeTimeFormatConstructor is nullptr");
        return nullptr;
    }
    napi_value constructor = nullptr;
    status = napi_get_reference_value(env, g_JsRelativeTimeFormatConstructor, &constructor);
    if (status != napi_ok || constructor == nullptr) {
        HILOG_ERROR_I18N("CreateRelativeTimeFormatWithoutNew: napi_get_reference_value failed");
        return nullptr;
    }
    napi_value instance = nullptr;
    status = napi_new_instance(env, constructor, argc, argv, &instance);
    if (status != napi_ok) {
        HILOG_ERROR_I18N("CreateRelativeTimeFormatWithoutNew: napi_new_instance failed");
        return nullptr;
    }
    return instance;
}

napi_value JSRelativeTimeFormatAddon::Format(napi_env env, napi_callback_info info)
{
    size_t argc = 2;
    napi_value argv[2] = { nullptr };
    napi_value thisVar = nullptr;
    void *data = nullptr;
    napi_status status = napi_get_cb_info(env, info, &argc, argv, &thisVar, &data);
    if (status != napi_ok) {
        HILOG_ERROR_I18N("JSRelativeTimeFormatAddon::Format: Get parameter info failed");
        return nullptr;
    }
    double number;
    status = napi_get_value_double(env, argv[0], &number);
    if (status != napi_ok) {
        HILOG_ERROR_I18N("JSRelativeTimeFormatAddon::Format: Get number failed");
        return nullptr;
    }
    std::vector<char> unit;
    if (!JSUtils::GetStringParameter(env, argv[1], unit)) {
        return nullptr;
    }
    JSRelativeTimeFormatAddon *obj = nullptr;
    status = napi_unwrap(env, thisVar, reinterpret_cast<void **>(&obj));
    if (status != napi_ok || !obj || !obj->relativetimefmt_) {
        HILOG_ERROR_I18N("JSRelativeTimeFormatAddon::Format: Get RelativeTimeFormat object failed");
        return nullptr;
    }
    std::string value = obj->relativetimefmt_->Format(number, unit.data());
    return JSUtils::CreateString(env, value);
}

napi_value JSRelativeTimeFormatAddon::GetNumberResolvedOptions(napi_env env, napi_callback_info info)
{
    napi_value thisVar = nullptr;
    void *data = nullptr;
    napi_status status = napi_get_cb_info(env, info, nullptr, nullptr, &thisVar, &data);
    if (status != napi_ok) {
        HILOG_ERROR_I18N("JSRelativeTimeFormatAddon::GetNumberResolvedOptions: Get parameter info failed");
        return nullptr;
    }
    JSRelativeTimeFormatAddon *obj = nullptr;
    status = napi_unwrap(env, thisVar, reinterpret_cast<void **>(&obj));
    if (status != napi_ok || !obj || !obj->relativetimefmt_) {
        HILOG_ERROR_I18N("GetNumberResolvedOptions: Get RelativeTimeFormat object failed");
        return nullptr;
    }
    napi_value result = nullptr;
    status = napi_create_object(env, &result);
    if (status != napi_ok) {
        HILOG_ERROR_I18N("JSRelativeTimeFormatAddon::GetNumberResolvedOptions: napi_create_object failed");
        return nullptr;
    }
    std::map<std::string, std::string> options = {};
    obj->relativetimefmt_->GetResolvedOptions(options);
    JSUtils::SetOptionProperties(env, result, options, "locale");
    JSUtils::SetOptionProperties(env, result, options, "style");
    JSUtils::SetOptionProperties(env, result, options, "numeric");
    JSUtils::SetOptionProperties(env, result, options, "numberingSystem");
    return result;
}

napi_value JSRelativeTimeFormatAddon::FormatToParts(napi_env env, napi_callback_info info)
{
    size_t argc = 2;
    const size_t argsCount = 2; // 2 is required parameter count
    napi_value argv[argsCount] = { nullptr };
    napi_value thisVar = nullptr;
    napi_status status = napi_get_cb_info(env, info, &argc, argv, &thisVar, nullptr);
    if (status != napi_ok) {
        HILOG_ERROR_I18N("JSRelativeTimeFormatAddon::FormatToParts: Get parameter info failed");
        return nullptr;
    }
    if (argc < argsCount) {
        HILOG_ERROR_I18N("JSRelativeTimeFormatAddon::FormatToParts: parameter count less then required.");
        return nullptr;
    }
    napi_valuetype valueType = napi_valuetype::napi_undefined;
    status = napi_typeof(env, argv[0], &valueType);
    if (status != napi_ok) {
        HILOG_ERROR_I18N("JSRelativeTimeFormatAddon::FormatToParts: Get param type failed.");
        return nullptr;
    }
    if (valueType != napi_valuetype::napi_number) {
        HILOG_ERROR_I18N("JSRelativeTimeFormatAddon::FormatToParts: parameter type invalid");
        ErrorUtil::NapiThrowUndefined(env, "parameter type invalid");
        return nullptr;
    }
    int32_t code = 0;
    std::string unit = JSUtils::GetString(env, argv[argsCount - 1], code);
    if (code != 0) {
        HILOG_ERROR_I18N("JSRelativeTimeFormatAddon::FormatToParts: Get second parameter failed");
        return nullptr;
    }
    return GetFormatToPartsInner(env, argv[0], unit, thisVar);
}

napi_value JSRelativeTimeFormatAddon::GetFormatToPartsInner(napi_env env, napi_value param,
    const std::string &unit, napi_value &thisVar)
{
    JSRelativeTimeFormatAddon *obj = nullptr;
    napi_status status = napi_unwrap(env, thisVar, reinterpret_cast<void **>(&obj));
    if (status != napi_ok || !obj || !obj->relativetimefmt_) {
        HILOG_ERROR_I18N("GetFormatToPartsInner: Get NumberFormat object failed");
        return nullptr;
    }
    std::vector<std::vector<std::string>> relativeTimeParts;
    double number = 0;
    status = napi_get_value_double(env, param, &number);
    if (status != napi_ok) {
        HILOG_ERROR_I18N("GetFormatToPartsInner: Get double parameter value failed");
        return nullptr;
    }
    obj->relativetimefmt_->FormatToParts(number, unit, relativeTimeParts);
    return SetNumberFormatParts(env, status, relativeTimeParts);
}

napi_value JSRelativeTimeFormatAddon::SupportedLocalesOf(napi_env env, napi_callback_info info)
{
    size_t argc = 2;
    napi_value argv[2] = { nullptr };
    napi_value thisVar = nullptr;
    napi_status status = napi_get_cb_info(env, info, &argc, argv, &thisVar, nullptr);
    if (status != napi_ok) {
        HILOG_ERROR_I18N("JSRelativeTimeFormatAddon::SupportedLocalesOf: Get parameter info failed");
        return nullptr;
    }
    std::vector<std::string> localeTags;
    if (argc < 1) {
        return JSUtils::CreateArray(env, localeTags);
    }
    int32_t code = 0;
    std::vector<std::string> localeArray = JSUtils::GetLocaleArray(env, argv[0], code);
    if (code != 0) {
        HILOG_ERROR_I18N("JSRelativeTimeFormatAddon::SupportedLocalesOf: Get locale array failed");
        return nullptr;
    }
    localeTags.assign(localeArray.begin(), localeArray.end());
    std::map<std::string, std::string> map = {};
    if (argc > 1) {
        JSUtils::GetOptionValue(env, argv[1], "localeMatcher", map);
    }
    I18nErrorCode i18nStatus = I18nErrorCode::SUCCESS;
    std::vector<std::string> resultLocales = RelativeTimeFormat::SupportedLocalesOf(localeTags, map, i18nStatus);
    if (i18nStatus != I18nErrorCode::SUCCESS) {
        HILOG_ERROR_I18N("JSRelativeTimeFormatAddon::SupportedLocalesOf: SupportedLocalesOf status fail");
        ErrorUtil::NapiThrowUndefined(env, "invalid locale");
        return nullptr;
    }
    return JSUtils::CreateArray(env, resultLocales);
}

napi_value JSRelativeTimeFormatAddon::SetNumberFormatParts(napi_env env, napi_status &status,
    const std::vector<std::vector<std::string>> &numberParts)
{
    napi_value result = nullptr;
    status = napi_create_array_with_length(env, numberParts.size(), &result);
    if (status != napi_ok) {
        HILOG_ERROR_I18N("SetNumberFormatParts: Failed to create array");
        return nullptr;
    }
    for (size_t i = 0; i < numberParts.size(); i++) {
        napi_value value = nullptr;
        status = napi_create_string_utf8(env, numberParts[i][1].c_str(), NAPI_AUTO_LENGTH, &value);
        if (status != napi_ok) {
            HILOG_ERROR_I18N("Failed to create string item numberParts[i][1].");
            return nullptr;
        }
        napi_value type = nullptr;
        status = napi_create_string_utf8(env, numberParts[i][0].c_str(), NAPI_AUTO_LENGTH, &type);
        if (status != napi_ok) {
            HILOG_ERROR_I18N("Failed to create string item numberParts[i][0].");
            return nullptr;
        }
        std::unordered_map<std::string, napi_value> propertys {
            { "type", type },
            { "value", value }
        };
        std::vector<std::string> keyVect = { "type", "value" };
        int32_t code = 0;
        napi_value formatInfo = JSUtils::CreateArrayItem(env, propertys, code, keyVect);
        if (code != 0) {
            return nullptr;
        }
        status = napi_set_element(env, result, i, formatInfo);
        if (status != napi_ok) {
            HILOG_ERROR_I18N("Failed to set array item");
            return nullptr;
        }
    }
    return result;
}

} // namespace I18n
} // namespace Global
} // namespace OHOS