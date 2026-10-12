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

#include "error_util.h"
#include "i18n_currency_addon.h"
#include "i18n_hilog.h"
#include "js_lifecycle_managers.h"
#include "js_utils.h"
#include "variable_convertor.h"

namespace OHOS {
namespace Global {
namespace I18n {
static thread_local ThreadReference* g_currencyConstructor = nullptr;
constexpr int32_t CURRENCY_ARGC = 1;

I18nCurrencyAddon::I18nCurrencyAddon() {}

I18nCurrencyAddon::~I18nCurrencyAddon() {}

void I18nCurrencyAddon::Destructor(napi_env env, void *nativeObject, void *hint)
{
    if (!nativeObject) {
        return;
    }
    delete reinterpret_cast<I18nCurrencyAddon *>(nativeObject);
    nativeObject = nullptr;
}

napi_value I18nCurrencyAddon::InitI18nCurrency(napi_env env, napi_value exports)
{
    HandleScope scope(env, "InitI18nCurrency");
    if (!scope.IsOpen()) {
        return nullptr;
    }
    napi_property_descriptor properties[] = {
        DECLARE_NAPI_FUNCTION("getISO4217Code", GetISO4217Code),
    };
    napi_value constructor = nullptr;
    napi_status status = napi_define_class(env, "Currency", NAPI_AUTO_LENGTH,
        I18nCurrencyConstructor, nullptr,
        sizeof(properties) / sizeof(napi_property_descriptor), properties, &constructor);
    if (status != napi_ok) {
        HILOG_ERROR_I18N("InitI18nCurrency: Failed to define class Currency");
        return nullptr;
    }
    g_currencyConstructor = ThreadReference::CreateInstance(env, constructor, CURRENCY_ARGC);
    if (!g_currencyConstructor) {
        HILOG_ERROR_I18N("InitI18nCurrency: Failed to create Currency ref");
        return nullptr;
    }
    status = napi_set_named_property(env, exports, "Currency", constructor);
    if (status != napi_ok) {
        HILOG_ERROR_I18N("InitI18nCurrency: Set property failed");
        return nullptr;
    }
    return exports;
}

napi_value I18nCurrencyAddon::I18nCurrencyConstructor(napi_env env, napi_callback_info info)
{
    size_t argc = CURRENCY_ARGC;
    napi_value argv[CURRENCY_ARGC] = { nullptr };
    napi_value thisVar = nullptr;
    void *data = nullptr;
    napi_status status = napi_get_cb_info(env, info, &argc, argv, &thisVar, &data);
    if (status != napi_ok || argc < CURRENCY_ARGC) {
        HILOG_ERROR_I18N("I18nCurrencyConstructor: Get cb info failed");
        return nullptr;
    }

    napi_valuetype valueType = napi_valuetype::napi_undefined;
    status = napi_typeof(env, argv[0], &valueType);
    if (status != napi_ok || valueType != napi_valuetype::napi_string) {
        return nullptr;
    }
    int32_t code = 0;
    std::string localeTag = VariableConvertor::GetString(env, argv[0], code);
    if (code != 0) {
        return nullptr;
    }
    std::unique_ptr<I18nCurrencyAddon> obj = std::make_unique<I18nCurrencyAddon>();
    status = napi_wrap_s(env, thisVar, reinterpret_cast<void *>(obj.get()),
        I18nCurrencyAddon::Destructor, nullptr, &TYPE_TAG, nullptr);
    if (status != napi_ok) {
        return nullptr;
    }
    obj->currency_ = std::make_unique<Currency>(localeTag);
    if (!obj->currency_) {
        obj.release();
        return nullptr;
    }
    obj.release();
    return thisVar;
}

napi_value I18nCurrencyAddon::GetISO4217Code(napi_env env, napi_callback_info info)
{
    size_t argc = 0;
    napi_value *argv = nullptr;
    napi_value thisVar = nullptr;
    void *data = nullptr;
    napi_status status = napi_get_cb_info(env, info, &argc, argv, &thisVar, &data);
    if (status != napi_ok) {
        return nullptr;
    }
    I18nCurrencyAddon *obj = nullptr;
    status = napi_unwrap_s(env, thisVar, &TYPE_TAG, reinterpret_cast<void **>(&obj));
    if (status != napi_ok || !obj || !obj->currency_) {
        HILOG_ERROR_I18N("GetISO4217Code: Get Currency object failed");
        return nullptr;
    }
    std::string result = obj->currency_->GetISO4217Code();
    return VariableConvertor::CreateString(env, result);
}

napi_value I18nCurrencyAddon::GetCurrency(napi_env env, napi_callback_info info)
{
    size_t argc = CURRENCY_ARGC;
    napi_value argv[CURRENCY_ARGC] = { nullptr };
    napi_value thisVar = nullptr;
    void *data = nullptr;
    napi_status status = napi_get_cb_info(env, info, &argc, argv, &thisVar, &data);
    if (status != napi_ok) {
        HILOG_ERROR_I18N("GetCurrency: Create result failed");
        return nullptr;
    }
    if (argc < CURRENCY_ARGC || VariableConvertor::IsUndefined(env, argv[0])) {
        ErrorUtil::NapiThrowBusinessError(env, I18N_NOT_FOUND, "locale", "", true);
        return nullptr;
    }
    if (VariableConvertor::GetLocaleTypeNoThrow(env, argv[0]) != LocaleType::BUILTINS_LOCALE) {
        ErrorUtil::NapiThrowBusinessError(env, I18N_NOT_FOUND, "locale", "Intl.Locale", true);
        return nullptr;
    }
    std::string localeTag = VariableConvertor::ParseBuiltinsLocale(env, argv[0]);
    napi_value localeTagValue = VariableConvertor::CreateString(env, localeTag);
    return StaticGetCurrency(env, localeTagValue);
}

napi_value I18nCurrencyAddon::StaticGetCurrency(napi_env env, napi_value localeTag)
{
    napi_value constructor = nullptr;
    if (g_currencyConstructor == nullptr) {
        HILOG_ERROR_I18N("StaticGetCurrency: g_currencyConstructor is null");
        return nullptr;
    }
    if (!g_currencyConstructor->GetReferenceValue(env, &constructor) || constructor == nullptr) {
        HILOG_ERROR_I18N("StaticGetCurrency: GetReferenceValue failed");
        return nullptr;
    }
    napi_value result = nullptr;
    napi_status status = napi_new_instance(env, constructor, CURRENCY_ARGC, &localeTag, &result);
    if (status != napi_ok) {
        HILOG_ERROR_I18N("StaticGetCurrency: napi_new_instance failed");
        return nullptr;
    }
    return result;
}
} // namespace I18n
} // namespace Global
} // namespace OHOS
