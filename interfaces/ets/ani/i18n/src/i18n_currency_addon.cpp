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

#include "i18n_currency_addon.h"

#include "error_util.h"
#include "i18n_hilog.h"
#include "locale_config.h"
#include "variable_converter.h"

using namespace OHOS;
using namespace Global;
using namespace I18n;

I18nCurrencyAddon* I18nCurrencyAddon::UnwrapAddon(ani_env *env, ani_object object)
{
    ani_long ptr;
    if (ANI_OK != env->Object_GetFieldByName_Long(object, "nativeCurrency", &ptr)) {
        HILOG_ERROR_I18N("I18nCurrencyAddon::UnwrapAddon: Get field nativeCurrency failed");
        return nullptr;
    }
    return reinterpret_cast<I18nCurrencyAddon*>(ptr);
}

ani_object I18nCurrencyAddon::GetCurrency(ani_env *env, ani_object locale)
{
    ani_boolean isUndefined;
    if (ANI_OK != env->Reference_IsUndefined(locale, &isUndefined)) {
        HILOG_ERROR_I18N("I18nCurrencyAddon::GetCurrency: Check locale undefined failed");
        return nullptr;
    }
    if (isUndefined) {
        ErrorUtil::AniThrow(env, I18N_NOT_FOUND, "locale", "");
        return nullptr;
    }
    std::string localeTag = VariableConverter::GetLocaleTagFromBuiltinLocale(env, locale);

    std::unique_ptr<I18nCurrencyAddon> obj = std::make_unique<I18nCurrencyAddon>();
    obj->currency_ = std::make_unique<Currency>(localeTag);

    static const char* className = "@ohos.i18n.i18n.Currency";
    ani_object currencyObject = VariableConverter::CreateAniObject(env, className, obj.get());
    if (currencyObject != nullptr) {
        obj.release();
    }
    return currencyObject;
}

ani_string I18nCurrencyAddon::GetISO4217Code(ani_env *env, ani_object object)
{
    I18nCurrencyAddon* obj = UnwrapAddon(env, object);
    if (obj == nullptr || obj->currency_ == nullptr) {
        HILOG_ERROR_I18N("I18nCurrencyAddon::GetISO4217Code: Get Currency object failed");
        return nullptr;
    }
    std::string result = obj->currency_->GetISO4217Code();
    return VariableConverter::StringToAniStr(env, result);
}

ani_status I18nCurrencyAddon::BindContextCurrency(ani_env *env)
{
    static const char *className = "@ohos.i18n.i18n.Currency";
    ani_class cls;
    if (ANI_OK != env->FindClass(className, &cls)) {
        HILOG_ERROR_I18N("BindContextCurrency: Find class '%{public}s' failed", className);
        return ANI_ERROR;
    }
    std::array methods = {
        ani_native_function { "getISO4217Code", nullptr,
            reinterpret_cast<void *>(I18nCurrencyAddon::GetISO4217Code) },
    };
    if (ANI_OK != env->Class_BindNativeMethods(cls, methods.data(), methods.size())) {
        HILOG_ERROR_I18N("BindContextCurrency: Cannot bind native methods to '%{public}s'",
            className);
        return ANI_ERROR;
    }
    return ANI_OK;
}
