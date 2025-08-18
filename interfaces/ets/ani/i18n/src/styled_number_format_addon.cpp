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

#include "styled_number_format_addon.h"

#include "error_util.h"
#include "i18n_hilog.h"
#include "variable_converter.h"

namespace OHOS {
namespace Global {
namespace I18n {
I18nStyledNumberFormatAddon* I18nStyledNumberFormatAddon::UnwrapAddon(ani_env *env, ani_object object)
{
    ani_long ptr;
    if (ANI_OK != env->Object_GetFieldByName_Long(object, "nativeStyledNumberFormat", &ptr)) {
        HILOG_ERROR_I18N("Get Long 'nativeStyledNumberFormat' failed");
        return nullptr;
    }
    return reinterpret_cast<I18nStyledNumberFormatAddon*>(ptr);
}

static void GetNumberFmtOrSimpleNumberFmt(ani_env *env, ani_object numberFormat,
    NumberFormatAddon **numberFormatAddon, I18nSimpleNumberFormatAddon **simpleNumberFormatAddon)
{
    ani_class simpleNumberFormatClass;
    if (ANI_OK != env->FindClass("L@ohos/i18n/i18n/SimpleNumberFormat;", &simpleNumberFormatClass)) {
        HILOG_ERROR_I18N("GetNumberFmtOrSimpleNumberFmt Find class 'L@ohos/i18n/i18n/SimpleNumberFormat;' failed");
        return;
    }

    ani_boolean isSimpleNumberFormat;
    if (ANI_OK != env->Object_InstanceOf(numberFormat, simpleNumberFormatClass, &isSimpleNumberFormat)) {
        HILOG_ERROR_I18N("GetNumberFmtOrSimpleNumberFmt: Get Instance failed");
        return;
    }
    if (isSimpleNumberFormat) {
        *simpleNumberFormatAddon = I18nSimpleNumberFormatAddon::UnwrapAddon(env, numberFormat);
    } else {
        ani_long ptr;
        if (ANI_OK != env->Object_GetFieldByName_Long(numberFormat, "nativeNumberFormat", &ptr)) {
            HILOG_ERROR_I18N("GetNumberFmtOrSimpleNumberFmt: Get Long 'nativeNumberFormat' failed");
            *numberFormatAddon = nullptr;
        }
        *numberFormatAddon = reinterpret_cast<NumberFormatAddon*>(ptr);
    }
}

bool I18nStyledNumberFormatAddon::InitStyledNumberFormatContent(ani_env *env, NumberFormatAddon* numberFormatAddon,
    I18nSimpleNumberFormatAddon* simpleNumberFormatAddon)
{
    if (numberFormatAddon) {
        styledNumberFormat_ =
            std::make_unique<StyledNumberFormat>(true, numberFormatAddon->CopyNumberFormat(), nullptr);
    } else if (simpleNumberFormatAddon) {
        styledNumberFormat_ =
            std::make_unique<StyledNumberFormat>(false, nullptr, simpleNumberFormatAddon->CopySimpleNumberFormat());
    }
    return styledNumberFormat_ != nullptr;
}

ani_object I18nStyledNumberFormatAddon::Create(ani_env *env, [[maybe_unused]] ani_object object,
    ani_object numberFormat)
{
    NumberFormatAddon *numberFormatAddon = nullptr;
    I18nSimpleNumberFormatAddon *simpleNumberFormatAddon = nullptr;

    GetNumberFmtOrSimpleNumberFmt(env, numberFormat, &numberFormatAddon, &simpleNumberFormatAddon);

    if (!numberFormatAddon && !simpleNumberFormatAddon) {
        ErrorUtil::AniThrow(env, I18N_NOT_FOUND, "numberFormat", "intl.NumberFormat or SimpleNumberFormat");
        return nullptr;
    }

    std::unique_ptr<I18nStyledNumberFormatAddon> obj = std::make_unique<I18nStyledNumberFormatAddon>();
    if (!obj) {
        HILOG_ERROR_I18N("StyledNumberFormatAddon::constructor make unique ptr error");
        return nullptr;
    }

    if (!obj->InitStyledNumberFormatContent(env, numberFormatAddon, simpleNumberFormatAddon)) {
        return nullptr;
    }

    static const char *className = "L@ohos/i18n/i18n/StyledNumberFormat;";
    ani_object styledNumberFormatObject = VariableConverter::CreateAniObject(env, className, obj.get());
    obj.release();
    return styledNumberFormatObject;
}

static ani_object CreateStyledPart(ani_env *env, StyledNumberFormat::NumberPart numberPart)
{
    static const char *className = "L@ohos/i18n/i18n/StyledPartInner;";
    ani_class cls;
    if (ANI_OK != env->FindClass(className, &cls)) {
        HILOG_ERROR_I18N("CreateStyledPart: Find class '%{public}s' failed", className);
        return nullptr;
    }

    ani_method ctor;
    if (ANI_OK != env->Class_FindMethod(cls, "<ctor>", nullptr, &ctor)) {
        HILOG_ERROR_I18N("CreateStyledPart: Find method '<ctor>' failed");
        return nullptr;
    }

    ani_object styledPart;
    if (ANI_OK != env->Object_New(cls, ctor, &styledPart)) {
        HILOG_ERROR_I18N("CreateStyledPart: New object '%{public}s' failed", className);
        return nullptr;
    }

    auto status = env->Object_SetPropertyByName_Int(styledPart, "start", static_cast<int32_t>(numberPart.start));
    if (status != ANI_OK) {
        HILOG_ERROR_I18N("Set property 'start' failed, status: %{public}d", status);
        return nullptr;
    }
    status = env->Object_SetPropertyByName_Int(styledPart, "length", static_cast<int32_t>(numberPart.length));
    if (status != ANI_OK) {
        HILOG_ERROR_I18N("Set property 'length' failed, status: %{public}d", status);
        return nullptr;
    }
    VariableConverter::SetStringMember(env, styledPart, "type", numberPart.part_name);
    return styledPart;
}

static ani_object CreateStyledPartArray(ani_env *env, const std::vector<StyledNumberFormat::NumberPart> &numberParts)
{
    static const char *className = "Lescompat/Array;";
    ani_class cls;
    if (ANI_OK != env->FindClass(className, &cls)) {
        HILOG_ERROR_I18N("CreateStyledPartArray: Find class '%{public}s' failed", className);
        return nullptr;
    }

    ani_method ctor;
    if (ANI_OK != env->Class_FindMethod(cls, "<ctor>", "I:V", &ctor)) {
        HILOG_ERROR_I18N("Find method '<ctor>' failed");
        return nullptr;
    }

    ani_object ret;
    if (ANI_OK != env->Object_New(cls, ctor, &ret, numberParts.size())) {
        HILOG_ERROR_I18N("CreateStyledPartArray: New object '%{public}s' failed", className);
        return nullptr;
    }

    ani_method set;
    if (ANI_OK != env->Class_FindMethod(cls, "$_set", "ILstd/core/Object;:V", &set)) {
        HILOG_ERROR_I18N("Find method '$_set' failed");
        return nullptr;
    }

    for (size_t index = 0; index < numberParts.size(); ++index) {
        ani_object styledPart = CreateStyledPart(env, numberParts[index]);
        if (ANI_OK != env->Object_CallMethod_Void(ret, set, index, styledPart)) {
            HILOG_ERROR_I18N("CreateStyledPartArray: Call method '$_set' failed");
            return nullptr;
        }
    }
    return ret;
}

ani_string I18nStyledNumberFormatAddon::GetFormattedNumber(ani_env *env, ani_object object, ani_double value)
{
    I18nStyledNumberFormatAddon* obj = UnwrapAddon(env, object);
    if (obj == nullptr || obj->styledNumberFormat_  == nullptr) {
        HILOG_ERROR_I18N("I18nStyledNumberFormatAddon::GetFormattedNumber: Get StyledNumberFormat object failed");
        return nullptr;
    }
    return VariableConverter::StringToAniStr(env, obj->styledNumberFormat_->Format(value));
}

ani_object I18nStyledNumberFormatAddon::GetStyle(ani_env *env, ani_object object, ani_double value)
{
    I18nStyledNumberFormatAddon* obj = UnwrapAddon(env, object);
    if (obj == nullptr || obj->styledNumberFormat_  == nullptr) {
        HILOG_ERROR_I18N("I18nStyledNumberFormatAddon::GetStyle: Get StyledNumberFormat object failed");
        return nullptr;
    }
    std::vector<StyledNumberFormat::NumberPart> numberParts = obj->styledNumberFormat_->ParseToParts(value);
    return CreateStyledPartArray(env, numberParts);
}

ani_status I18nStyledNumberFormatAddon::BindContextStyledNumberFormat(ani_env *env)
{
    static const char *className = "L@ohos/i18n/i18n/StyledNumberFormat;";
    ani_class cls;
    if (ANI_OK != env->FindClass(className, &cls)) {
        HILOG_ERROR_I18N("BindContextStyledNumberFormat: Find class '%{public}s' failed", className);
        return (ani_status)ANI_ERROR;
    }

    std::array methods = {
        ani_native_function { "create", nullptr, reinterpret_cast<void *>(I18nStyledNumberFormatAddon::Create) },
        ani_native_function { "getFormattedNumber", nullptr,
            reinterpret_cast<void *>(I18nStyledNumberFormatAddon::GetFormattedNumber) },
        ani_native_function { "getStyle", nullptr, reinterpret_cast<void *>(I18nStyledNumberFormatAddon::GetStyle) },
    };

    if (ANI_OK != env->Class_BindNativeMethods(cls, methods.data(), methods.size())) {
        HILOG_ERROR_I18N("BindContextStyledNumberFormat: Cannot bind native methods to '%{public}s'", className);
        return (ani_status)ANI_ERROR;
    };

    return ANI_OK;
}
} // namespace I18n
} // namespace Global
} // namespace OHOS
