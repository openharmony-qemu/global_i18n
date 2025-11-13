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
#include "advanced_measure_format.h"

#include "i18n_hilog.h"

namespace OHOS {
namespace Global {
namespace I18n {

AdvancedMeasureFormat::AdvancedMeasureFormat(std::shared_ptr<NumberFormat> numberFormat,
    const AdvancedMeasureOptions& options)
{
    numberFormat_ = numberFormat;
    options_ = options;
    InitContext();
}

AdvancedMeasureFormat::~AdvancedMeasureFormat()
{
}

std::string AdvancedMeasureFormat::FormatNumber(double value)
{
    if (numberFormat_ == nullptr) {
        HILOG_ERROR_I18N("AdvancedMeasureFormat::FormatNumber: numberFormat_ is nullptr.");
        return "";
    }
    return numberFormat_->Format(value);
}

void AdvancedMeasureFormat::InitContext()
{
    if (numberFormat_ == nullptr) {
        HILOG_ERROR_I18N("AdvancedMeasureFormat::InitContext: numberFormat_ is nullptr.");
        return;
    }
    InitUnitUsage();
}

void AdvancedMeasureFormat::InitUnitUsage()
{
    if (options_.unitUsage.empty()) {
        return;
    }
    numberFormat_->SetUnitUsage(options_.unitUsage);
}
} // namespace I18n
} // namespace Global
} // namespace OHOS
