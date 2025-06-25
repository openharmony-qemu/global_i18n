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

using namespace OHOS;
using namespace Global;
using namespace I18n;

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
        HILOG_ERROR_I18N("Find class 'L@ohos/i18n/i18n/SimpleNumberFormat;' failed");
        return;
    }

    ani_boolean isSimpleNumberFormat;
    if (ANI_OK != env->Object_InstanceOf(numberFormat, simpleNumberFormatClass, &isSimpleNumberFormat)) {
        HILOG_ERROR_I18N("Get Instance failed");
        return;
    }
    if (isSimpleNumberFormat) {
        *simpleNumberFormatAddon = I18nSimpleNumberFormatAddon::UnwrapAddon(env, numberFormat);
    } else {
        ani_long ptr;
        if (ANI_OK != env->Object_GetFieldByName_Long(numberFormat, "nativeNumberFormat", &ptr)) {
            HILOG_ERROR_I18N("Get Long 'nativeNumberFormat' failed");
            *numberFormatAddon = nullptr;
        }
        *numberFormatAddon = reinterpret_cast<NumberFormatAddon*>(ptr);
    }
}

void I18nStyledNumberFormatAddon::SetTextStyle(ani_env *env, ani_object options, const std::string &optionName)
{
    ani_ref ref;
    if (ANI_OK != env->Object_GetPropertyByName_Ref(options, optionName.c_str(), &ref)) {
        HILOG_ERROR_I18N("Get property '%{public}s' failed", optionName.c_str());
        return;
    }

    ani_boolean isUndefined;
    if (ANI_OK != env->Reference_IsUndefined(ref, &isUndefined)) {
        HILOG_ERROR_I18N("Reference IsUndefined failed");
        return;
    }
    if (isUndefined) {
        HILOG_ERROR_I18N("SetTextStyle: %{public}s is nullptr.", optionName.c_str());
        return;
    }

    styledNumberFormatOptions_[optionName] = ref;
}

bool I18nStyledNumberFormatAddon::InitStyledNumberFormatContent(ani_env *env, NumberFormatAddon* numberFormatAddon,
    I18nSimpleNumberFormatAddon* simpleNumberFormatAddon, ani_object options)
{
    if (numberFormatAddon) {
        styledNumberFormat_ =
            std::make_unique<StyledNumberFormat>(true, numberFormatAddon->CopyNumberFormat(), nullptr);
    } else if (simpleNumberFormatAddon) {
        styledNumberFormat_ =
            std::make_unique<StyledNumberFormat>(false, nullptr, simpleNumberFormatAddon->CopySimpleNumberFormat());
    }

    ani_boolean isUndefined;
    if (ANI_OK != env->Reference_IsUndefined(options, &isUndefined)) {
        HILOG_ERROR_I18N("Reference IsUndefined failed");
        return false;
    }
    if (!isUndefined) {
        SetTextStyle(env, options, "integer");
        SetTextStyle(env, options, "decimal");
        SetTextStyle(env, options, "fraction");
        SetTextStyle(env, options, "unit");
    }
    return styledNumberFormat_ != nullptr;
}

ani_object I18nStyledNumberFormatAddon::Create(ani_env *env, [[maybe_unused]] ani_object object,
    ani_object numberFormat, ani_object options)
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

    if (!obj->InitStyledNumberFormatContent(env, numberFormatAddon, simpleNumberFormatAddon, options)) {
        return nullptr;
    }

    static const char *className = "L@ohos/i18n/i18n/StyledNumberFormat;";
    ani_object styledNumberFormatObject = VariableConverter::CreateAniObject(env, className, obj.get());
    obj.release();
    return styledNumberFormatObject;
}

static ani_object CreateStyleOption(ani_env *env, StyledNumberFormat::NumberPart numberPart, ani_ref textStyleRef)
{
    if (!textStyleRef) {
        HILOG_ERROR_I18N("CreateStyleoption: options do not have %{public}s",
            numberPart.part_name.c_str());
        return nullptr;
    }

    static const char *className = "Larkui/component/styledString/StyleOptionsInner;";
    ani_class cls;
    if (ANI_OK != env->FindClass(className, &cls)) {
        HILOG_ERROR_I18N("Find class '%{public}s' failed", className);
        return nullptr;
    }

    ani_method ctor;
    if (ANI_OK != env->Class_FindMethod(cls, "<ctor>", nullptr, &ctor)) {
        HILOG_ERROR_I18N("Find method '<ctor>' failed");
        return nullptr;
    }

    ani_object styleOptions;
    if (ANI_OK != env->Object_New(cls, ctor, &styleOptions)) {
        HILOG_ERROR_I18N("New object '%{public}s' failed", className);
        return nullptr;
    }

    VariableConverter::SetNumberMember(env, styleOptions, "start", numberPart.start);
    VariableConverter::SetNumberMember(env, styleOptions, "length", numberPart.length);

    static const char *enumName = "Larkui/component/styledString/StyledStringKey;";
    if (ANI_OK != env->Object_SetPropertyByName_Ref(styleOptions, "styledKey",
        VariableConverter::GetEnumItemByIndex(env, enumName, 0))) {
        HILOG_ERROR_I18N("Set property 'styledKey' failed");
        return styleOptions;
    }

    if (ANI_OK != env->Object_SetPropertyByName_Ref(styleOptions, "styledValue", textStyleRef)) {
        HILOG_ERROR_I18N("Set property 'styledValue' failed");
        return styleOptions;
    }
    return styleOptions;
}

static ani_object CreateStyleOptions(ani_env *env,
    const std::vector<StyledNumberFormat::NumberPart> &numberParts,
    const std::unordered_map<std::string, ani_ref> &styledNumberFormatOptions)
{
    static const char *className = "Lescompat/Array;";
    ani_class cls;
    if (ANI_OK != env->FindClass(className, &cls)) {
        HILOG_ERROR_I18N("Find class '%{public}s' failed", className);
        return nullptr;
    }

    ani_method ctor;
    if (ANI_OK != env->Class_FindMethod(cls, "<ctor>", "I:V", &ctor)) {
        HILOG_ERROR_I18N("Find method '<ctor>' failed");
        return nullptr;
    }

    ani_object ret;
    if (ANI_OK != env->Object_New(cls, ctor, &ret, numberParts.size())) {
        HILOG_ERROR_I18N("New object '%{public}s' failed", className);
        return nullptr;
    }

    ani_method set;
    if (ANI_OK != env->Class_FindMethod(cls, "$_set", "ILstd/core/Object;:V", &set)) {
        HILOG_ERROR_I18N("Find method '$_set' failed");
        return nullptr;
    }

    unsigned int index = 0;
    for (size_t i = 0; i < numberParts.size(); ++i) {
        auto iter = styledNumberFormatOptions.find(numberParts[i].part_name);
        if (iter == styledNumberFormatOptions.end()) {
            continue;
        }

        ani_ref textStyleRef = iter->second;
        ani_object option = CreateStyleOption(env, numberParts[i], textStyleRef);
        if (ANI_OK != env->Object_CallMethod_Void(ret, set, index, option)) {
            HILOG_ERROR_I18N("Call method '$_set' failed");
            return nullptr;
        }
        index += 1;
    }
    return ret;
}

ani_object I18nStyledNumberFormatAddon::Format(ani_env *env, ani_object object, ani_double value)
{
    I18nStyledNumberFormatAddon* obj = UnwrapAddon(env, object);
    if (obj == nullptr || obj->styledNumberFormat_ == nullptr) {
        HILOG_ERROR_I18N("Format: Get StyledNumberFormat object failed");
        return nullptr;
    }

    std::string formattedNumber = obj->styledNumberFormat_->Format(value);
    std::vector<StyledNumberFormat::NumberPart> numberParts = obj->styledNumberFormat_->ParseToParts(value);
    
    static const char *className = "Larkui/component/styledString/StyledString;";
    ani_class cls;
    if (ANI_OK != env->FindClass(className, &cls)) {
        HILOG_ERROR_I18N("Find class '%{public}s' failed", className);
        return nullptr;
    }

    ani_method ctor;
    if (ANI_OK != env->Class_FindMethod(cls, "<ctor>", "Lstd/core/Object;Lescompat/Array;:V", &ctor)) {
        HILOG_ERROR_I18N("Find method '<ctor>' failed");
        return nullptr;
    }

    ani_object styleOption = CreateStyleOptions(env, numberParts, obj->styledNumberFormatOptions_);
    ani_object styleString;
    if (ANI_OK != env->Object_New(cls, ctor, &styleString,
        VariableConverter::StringToAniStr(env, formattedNumber), styleOption)) {
        HILOG_ERROR_I18N("New object '%{public}s' failed", className);
        return nullptr;
    }
    return styleString;
}

ani_status I18nStyledNumberFormatAddon::BindContextStyledNumberFormat(ani_env *env)
{
    static const char *className = "L@ohos/i18n/i18n/StyledNumberFormat;";
    ani_class cls;
    if (ANI_OK != env->FindClass(className, &cls)) {
        HILOG_ERROR_I18N("Find class '%{public}s' failed", className);
        return (ani_status)ANI_ERROR;
    }

    std::array methods = {
        ani_native_function { "create", nullptr, reinterpret_cast<void *>(I18nStyledNumberFormatAddon::Create) },
        ani_native_function { "format", nullptr, reinterpret_cast<void *>(I18nStyledNumberFormatAddon::Format) },
    };

    if (ANI_OK != env->Class_BindNativeMethods(cls, methods.data(), methods.size())) {
        HILOG_ERROR_I18N("Cannot bind native methods to '%{public}s'", className);
        return (ani_status)ANI_ERROR;
    };

    return ANI_OK;
}
