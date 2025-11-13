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
#ifndef GLOBAL_I18N_ADVANCED_MEASURE_FORMAT_H
#define GLOBAL_I18N_ADVANCED_MEASURE_FORMAT_H

#include <memory>
#include <string>

#include "number_format.h"

namespace OHOS {
namespace Global {
namespace I18n {
class enum UnitUsage {
    AREA_LAND_AGRICULT = 1,
    AREA_LAND_COMMERACL = 2,
    AREA_LAND_RESIDNTL = 3

};

struct AdvancedMeasureOptions {
    std::string unitUsage;
};

class AdvancedMeasureFormat {
public:
    AdvancedMeasureFormat(std::shared_ptr<NumberFormat> numberFormat, const AdvancedMeasureOptions& options);
    ~AdvancedMeasureFormat();

    std::string FormatNumber(double value);

private:
    void InitContext();
    void InitUnitUsage();

    AdvancedMeasureOptions options_;
    std::shared_ptr<NumberFormat> numberFormat_ = nullptr;
};
} // namespace I18n
} // namespace Global
} // namespace OHOS
#endif