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

#include "format_utils.h"

namespace OHOS {
namespace Global {
namespace I18n {
std::unordered_map<UNumberFormatFields, std::string> FormatUtils::numberFormatFieldMap = {
    { UNUM_FRACTION_FIELD, "fraction" },
    { UNUM_DECIMAL_SEPARATOR_FIELD, "decimal" },
    { UNUM_GROUPING_SEPARATOR_FIELD, "group" },
    { UNUM_CURRENCY_FIELD, "currency" },
    { UNUM_PERCENT_FIELD, "percentSign" },
    { UNUM_EXPONENT_SYMBOL_FIELD, "exponentSeparator" },
    { UNUM_EXPONENT_SIGN_FIELD, "exponentMinusSign" },
    { UNUM_EXPONENT_FIELD, "exponentInteger" },
    { UNUM_COMPACT_FIELD, "compact" },
    { UNUM_MEASURE_UNIT_FIELD, "unit" },
    { UNUM_APPROXIMATELY_SIGN_FIELD, "approximatelySign" },
};

std::string FormatUtils::GetNumberFieldType(const std::string &napiType,
    const int32_t &fieldId, const double &number)
{
    UNumberFormatFields formatField = static_cast<UNumberFormatFields>(fieldId);
    if (numberFormatFieldMap.find(formatField) != numberFormatFieldMap.end()) {
        return numberFormatFieldMap[formatField];
    }
    bool isBigint = napiType.compare("bigint") == 0;
    double tempNumber = 0;
    switch (formatField) {
        case UNUM_INTEGER_FIELD:
            tempNumber = isBigint ? tempNumber : number;
            if (isBigint || std::isfinite(tempNumber)) {
                return "integer";
            }
            if (std::isnan(tempNumber)) {
                return "nan";
            }
            return "infinity";
        case UNUM_SIGN_FIELD:
            if (isBigint) {
                return number < 0 ? "minusSign" : "plusSign";
            }
            return std::signbit(number) ? "minusSign" : "plusSign";
        default:
            return "unknown";
    }
}
} // namespace I18n
} // namespace Global
} // namespace OHOS
