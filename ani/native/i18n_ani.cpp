/*
 * Copyright (c) 2021-2025 Huawei Device Co., Ltd.
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

#include <ani.h>
#include <iostream>
#include <array>
#include "locale_config.h"

using namespace OHOS;
using namespace Global;
using namespace I18n;

ani_string getSystemLanguage([[maybe_unused]] ani_env *env, [[maybe_unused]] ani_object object)
{
    std::string str = LocaleConfig::GetSystemLanguage();
    ani_string ret;
    env->String_NewUTF8(str.c_str(), str.size(), &ret);
    return ret;
}

ani_string getSystemLocale([[maybe_unused]] ani_env *env, [[maybe_unused]] ani_object object)
{
    std::string str = LocaleConfig::GetSystemLocale();
    ani_string ret;
    env->String_NewUTF8(str.c_str(), str.size(), &ret);
    return ret;
}

ani_boolean is24HourClock([[maybe_unused]] ani_env *env, [[maybe_unused]] ani_object object)
{
    return LocaleConfig::Is24HourClock();
}

static ani_status BindContext(ani_env *env)
{
    static const char *className = "Li18n/i18n_ani/i18n/System;";
    ani_class cls;
    if (ANI_OK != env->FindClass(className, &cls)) {
        std::cerr << "Not found '" << className << "'" << std::endl;
        return (ani_status)ANI_ERROR;
    }

    std::array methods = {
        ani_native_function {"getSystemLanguage", ":Lstd/core/String;", reinterpret_cast<void *>(getSystemLanguage) },
        ani_native_function {"getSystemLocale", ":Lstd/core/String;", reinterpret_cast<void *>(getSystemLocale) },
        ani_native_function {"is24HourClock", ":Z", reinterpret_cast<void *>(is24HourClock) },
    };

    if (ANI_OK != env->Class_BindNativeMethods(cls, methods.data(), methods.size())) {
        std::cerr << "Cannot bind native methods to '" << className << "'" << std::endl;
        return (ani_status)ANI_ERROR;
    }

    return ANI_OK;
}

ANI_EXPORT ani_status ANI_Constructor(ani_vm *vm, uint32_t *result)
{
    ani_env *env;
    if (ANI_OK != vm->GetEnv(ANI_VERSION_1, &env)) {
        std::cerr << "Unsupported ANI_VERSION_1" << std::endl;
        return (ani_status)ANI_ERROR;
    }
    
    auto status = BindContext(env);
    if (status != ANI_OK) {
        return status;
    }
    *result = ANI_VERSION_1;
    return ANI_OK;
}