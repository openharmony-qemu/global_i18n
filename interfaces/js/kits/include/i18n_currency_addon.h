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
#ifndef I18N_CURRENCY_ADDON_H
#define I18N_CURRENCY_ADDON_H

#include "i18n_currency.h"
#include "napi/native_api.h"
#include "napi/native_node_api.h"

namespace OHOS {
namespace Global {
namespace I18n {
class I18nCurrencyAddon {
public:
    I18nCurrencyAddon();
    ~I18nCurrencyAddon();
    static void Destructor(napi_env env, void *nativeObject, void *hint);
    static napi_value GetCurrency(napi_env env, napi_callback_info info);
    static napi_value InitI18nCurrency(napi_env env, napi_value exports);

private:
    static napi_value I18nCurrencyConstructor(napi_env env, napi_callback_info info);
    static napi_value GetISO4217Code(napi_env env, napi_callback_info info);
    static napi_value StaticGetCurrency(napi_env env, napi_value localeTag);
    static constexpr napi_type_tag TYPE_TAG = { 0x7cb16f328a4715e9, 0xce1e3a670b8252f4 };
    std::unique_ptr<Currency> currency_ = nullptr;
};
} // namespace I18n
} // namespace Global
} // namespace OHOS
#endif
