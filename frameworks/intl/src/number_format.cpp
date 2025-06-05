/*
 * Copyright (c) 2021-2022 Huawei Device Co., Ltd.
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
#include "locale_config.h"

#include <cctype>
#include <cmath>
#include <sstream>
#include <tuple>
#include "i18n_hilog.h"
#include "locale_helper.h"
#include "ohos/init_data.h"
#include "parameter.h"
#include "unicode/ucurr.h"
#include "unicode/uenum.h"
#include "unicode/utypes.h"
#include "unicode/unumberformatter.h"
#include "utils.h"
#include "number_format.h"

namespace OHOS {
namespace Global {
namespace I18n {
const char* NumberFormat::DEVICE_TYPE_NAME = "const.product.devicetype";
std::mutex NumberFormat::numToCurrencyMutex;
bool NumberFormat::initISO4217 = false;
std::unordered_map<std::string, std::string> NumberFormat::numToCurrency = {};
bool NumberFormat::icuInitialized = NumberFormat::Init();

std::unordered_map<std::string, UNumberUnitWidth> NumberFormat::unitStyle = {
    { "long", UNumberUnitWidth::UNUM_UNIT_WIDTH_FULL_NAME },
    { "short", UNumberUnitWidth::UNUM_UNIT_WIDTH_SHORT },
    { "narrow", UNumberUnitWidth::UNUM_UNIT_WIDTH_NARROW }
};

std::unordered_map<std::string, UNumberUnitWidth> NumberFormat::currencyStyle = {
    { "symbol", UNumberUnitWidth::UNUM_UNIT_WIDTH_SHORT },
    { "code", UNumberUnitWidth::UNUM_UNIT_WIDTH_ISO_CODE },
    { "name", UNumberUnitWidth::UNUM_UNIT_WIDTH_FULL_NAME },
    { "narrowSymbol", UNumberUnitWidth::UNUM_UNIT_WIDTH_NARROW }
};

std::unordered_map<std::string, UNumberSignDisplay> NumberFormat::signAutoStyle = {
    { "auto", UNumberSignDisplay::UNUM_SIGN_AUTO },
    { "never", UNumberSignDisplay::UNUM_SIGN_NEVER },
    { "always", UNumberSignDisplay::UNUM_SIGN_ALWAYS },
    { "exceptZero", UNumberSignDisplay::UNUM_SIGN_EXCEPT_ZERO }
};

std::unordered_map<std::string, UNumberSignDisplay> NumberFormat::signAccountingStyle = {
    { "auto", UNumberSignDisplay::UNUM_SIGN_ACCOUNTING },
    { "never", UNumberSignDisplay::UNUM_SIGN_NEVER },
    { "always", UNumberSignDisplay::UNUM_SIGN_ACCOUNTING_ALWAYS },
    { "exceptZero", UNumberSignDisplay::UNUM_SIGN_ACCOUNTING_EXCEPT_ZERO }
};

std::unordered_map<UMeasurementSystem, std::string> NumberFormat::measurementSystem = {
    { UMeasurementSystem::UMS_SI, "SI" },
    { UMeasurementSystem::UMS_US, "US" },
    { UMeasurementSystem::UMS_UK, "UK" },
};

std::unordered_map<std::string, UNumberUnitWidth> NumberFormat::defaultUnitStyle = {
    { "tablet", UNumberUnitWidth::UNUM_UNIT_WIDTH_FULL_NAME },
    { "2in1", UNumberUnitWidth::UNUM_UNIT_WIDTH_FULL_NAME },
    { "tv", UNumberUnitWidth::UNUM_UNIT_WIDTH_FULL_NAME },
    { "pc", UNumberUnitWidth::UNUM_UNIT_WIDTH_FULL_NAME },
    { "wearable", UNumberUnitWidth::UNUM_UNIT_WIDTH_NARROW },
    { "liteWearable", UNumberUnitWidth::UNUM_UNIT_WIDTH_NARROW },
    { "watch", UNumberUnitWidth::UNUM_UNIT_WIDTH_NARROW }
};

std::unordered_map<std::string, UNumberUnitWidth> NumberFormat::defaultCurrencyStyle = {
    { "wearable", UNumberUnitWidth::UNUM_UNIT_WIDTH_NARROW },
    { "liteWearable", UNumberUnitWidth::UNUM_UNIT_WIDTH_NARROW },
    { "watch", UNumberUnitWidth::UNUM_UNIT_WIDTH_NARROW }
};

std::unordered_map<std::string, UNumberRoundingPriority> NumberFormat::roundingPriorityStyle = {
    { "auto", UNumberRoundingPriority::UNUM_ROUNDING_PRIORITY_STRICT },
    { "morePrecision", UNumberRoundingPriority::UNUM_ROUNDING_PRIORITY_RELAXED },
    { "lessPrecision", UNumberRoundingPriority::UNUM_ROUNDING_PRIORITY_STRICT }
};

std::unordered_map<std::string, UNumberFormatRoundingMode> NumberFormat::roundingModeStyle = {
    { "ceil", UNumberFormatRoundingMode::UNUM_ROUND_CEILING },
    { "floor", UNumberFormatRoundingMode::UNUM_ROUND_FLOOR },
    { "expand", UNumberFormatRoundingMode::UNUM_ROUND_UP },
    { "trunc", UNumberFormatRoundingMode::UNUM_ROUND_DOWN },
    { "halfCeil", UNumberFormatRoundingMode::UNUM_ROUND_HALF_CEILING },
    { "halfFloor", UNumberFormatRoundingMode::UNUM_ROUND_HALF_FLOOR },
    { "halfExpand", UNumberFormatRoundingMode::UNUM_ROUND_HALFUP },
    { "halfTrunc", UNumberFormatRoundingMode::UNUM_ROUND_HALFDOWN },
    { "halfEven", UNumberFormatRoundingMode::UNUM_ROUND_HALFEVEN },
};

std::vector<size_t> NumberFormat::roundingIncrementList = {
    1, 2, 5, 10, 20, 25, 50, 100, 200, 250, 500, 1000, 2000, 2500, 5000
};

std::map<std::string, std::string> NumberFormat::RelativeTimeFormatConfigs = {
    { "numeric", "auto" }
};

std::unordered_map<UNumberFormatFields, std::string> NumberFormat::numberFormatFieldMap = {
    { UNUM_FRACTION_FIELD, "fraction" },
    { UNUM_DECIMAL_SEPARATOR_FIELD, "decimal" },
    { UNUM_GROUPING_SEPARATOR_FIELD, "group" },
    { UNUM_CURRENCY_FIELD, "currency" },
    { UNUM_PERCENT_FIELD, "percentSign" },
    { UNUM_EXPONENT_SYMBOL_FIELD, "exponentSymbol" },
    { UNUM_EXPONENT_SIGN_FIELD, "exponentSign" },
    { UNUM_EXPONENT_FIELD, "exponent" },
    { UNUM_COMPACT_FIELD, "compact" },
    { UNUM_MEASURE_UNIT_FIELD, "unit" },
    { UNUM_APPROXIMATELY_SIGN_FIELD, "approximatelySign" },
};

std::unordered_map<std::string, int> NumberFormat::defaultCurrencyFractionMap = {
    { "BHD", 3 },
    { "XOF", 0 },
    { "XOF", 0 },
    { "BIF", 0 },
    { "XAF", 0 },
    { "XAF", 0 },
    { "XAF", 0 },
    { "CLP", 0 },
    { "CLF", 4 },
    { "KMF", 0 },
    { "XAF", 0 },
    { "XOF", 0 },
    { "DJF", 0 },
    { "XAF", 0 },
    { "XPF", 0 },
    { "XAF", 0 },
    { "GNF", 0 },
    { "XOF", 0 },
    { "ISK", 0 },
    { "IQD", 3 },
    { "JPY", 0 },
    { "JOD", 3 },
    { "KRW", 0 },
    { "KWD", 3 },
    { "LYD", 3 },
    { "XOF", 0 },
    { "XPF", 0 },
    { "XOF", 0 },
    { "OMR", 3 },
    { "PYG", 0 },
    { "RWF", 0 },
    { "XOF", 0 },
    { "XOF", 0 },
    { "TND", 3 },
    { "UGX", 0 },
    { "UYI", 0 },
    { "UYW", 4 },
    { "VUV", 0 },
    { "VND", 0 },
    { "XPF", 0 },
};

NumberFormat::NumberFormat(const std::vector<std::string> &localeTags, std::map<std::string, std::string> &configs)
{
    SetDefaultStyle();
    UErrorCode status = U_ZERO_ERROR;
    std::unique_ptr<icu::LocaleBuilder> builder = nullptr;
    builder = std::make_unique<icu::LocaleBuilder>();
    ParseConfigs(configs);
    for (size_t i = 0; i < localeTags.size(); i++) {
        std::string curLocale = localeTags[i];
        locale = icu::Locale::forLanguageTag(icu::StringPiece(curLocale), status);
        if (U_FAILURE(status)) {
            status = U_ZERO_ERROR;
            continue;
        }
        if (LocaleInfo::allValidLocales.count(locale.getLanguage()) > 0) {
            localeInfo = std::make_unique<LocaleInfo>(curLocale, configs);
            CreateRelativeTimeFormat(curLocale);
            if (!localeInfo->InitSuccess()) {
                continue;
            }
            locale = localeInfo->GetLocale();
            localeBaseName = localeInfo->GetBaseName();
            icu::MeasureUnit::getAvailable(unitArray, MAX_UNIT_NUM, status);
            if (!U_SUCCESS(status)) {
                status = U_ZERO_ERROR;
                continue;
            }
            createSuccess = true;
            break;
        }
    }
    if (!createSuccess) {
        std::string systemLocale = LocaleConfig::GetEffectiveLocale();
        localeInfo = std::make_unique<LocaleInfo>(systemLocale, configs);
        CreateRelativeTimeFormat(systemLocale);
        if (localeInfo != nullptr && localeInfo->InitSuccess()) {
            locale = localeInfo->GetLocale();
            localeBaseName = localeInfo->GetBaseName();
            icu::MeasureUnit::getAvailable(unitArray, MAX_UNIT_NUM, status);
            if (U_SUCCESS(status)) {
                createSuccess = true;
            }
        }
    }
    if (createSuccess) {
        InitNumberFormat();
    }
}

NumberFormat::~NumberFormat()
{
}

void NumberFormat::InitNumberFormat()
{
    icu::number::UnlocalizedNumberFormatter formatter = icu::number::NumberFormatter::with();
    InitProperties(formatter);
    numberFormat = formatter.locale(locale);
    numberRangeFormat = icu::number::NumberRangeFormatter::with()
        .numberFormatterBoth(formatter)
        .locale(locale);
}

void NumberFormat::CreateRelativeTimeFormat(const std::string& locale)
{
    if (unitUsage == "elapsed-time-second") {
        std::vector<std::string> locales = { locale };
        relativeTimeFormat = std::make_unique<RelativeTimeFormat>(locales,
            RelativeTimeFormatConfigs);
    }
}

void NumberFormat::InitProperties(icu::number::UnlocalizedNumberFormatter &formatter)
{
    InitCurrencyProperties(formatter);
    InitPercentStyleProperties(formatter);
    InitUnitProperties(formatter);
    InitUseGroupingProperties(formatter);
    InitSignProperties(formatter);
    InitNotationProperties(formatter);
    InitDigitsProperties(formatter);
}

void NumberFormat::InitCurrencyProperties(icu::number::UnlocalizedNumberFormatter &formatter)
{
    if (!currency.empty()) {
        UErrorCode status = U_ZERO_ERROR;
        icu::CurrencyUnit currencyUnit(icu::UnicodeString(currency.c_str()).getBuffer(), status);
        if (U_FAILURE(status)) {
            HILOG_ERROR_I18N("NumberFormat::InitCurrencyProperties: Create currency unit failed.");
            return;
        }
        formatter = formatter.unit(currencyUnit);
        if (currencyDisplay != UNumberUnitWidth::UNUM_UNIT_WIDTH_SHORT) {
            formatter = formatter.unitWidth(currencyDisplay);
        }
    }
}

void NumberFormat::InitPercentStyleProperties(icu::number::UnlocalizedNumberFormatter &formatter)
{
    if (!styleString.empty() && styleString == "percent") {
        formatter = formatter.unit(icu::NoUnit::percent())
            .scale(icu::number::Scale::powerOfTen(2)) // 2 is the power of ten
            .precision(icu::number::Precision::fixedFraction(0));
    }
}

void NumberFormat::InitUseGroupingProperties(icu::number::UnlocalizedNumberFormatter &formatter)
{
    if (!useGrouping.empty()) {
        UNumberGroupingStrategy groupingStrategy = (useGrouping == "true") ?
            UNumberGroupingStrategy::UNUM_GROUPING_AUTO : UNumberGroupingStrategy::UNUM_GROUPING_OFF;
        formatter = formatter.grouping(groupingStrategy);
    }
}

void NumberFormat::InitNotationProperties(icu::number::UnlocalizedNumberFormatter &formatter)
{
    if (!notationString.empty()) {
        formatter = formatter.notation(notation);
    }
}

void NumberFormat::InitSignProperties(icu::number::UnlocalizedNumberFormatter &formatter)
{
    if (!currencySign.empty() || !signDisplayString.empty()) {
        formatter = formatter.sign(signDisplay);
    }
}

void NumberFormat::InitUnitProperties(icu::number::UnlocalizedNumberFormatter &formatter)
{
    if (styleString.empty() || styleString.compare("unit") != 0) {
        return;
    }
    for (icu::MeasureUnit curUnit : unitArray) {
        if (!strcmp(curUnit.getSubtype(), unit.c_str())) {
            formatter = formatter.unit(curUnit);
            unitType = curUnit.getType();
        }
    }
    UErrorCode status = U_ZERO_ERROR;
    UMeasurementSystem measSys = ulocdata_getMeasurementSystem(localeBaseName.c_str(), &status);
    if (U_FAILURE(status)) {
        HILOG_ERROR_I18N("NumberFormat::InitUnitProperties: Get measurement system failed.");
        return;
    }
    bool isValidMeaSys = measurementSystem.find(measSys) != measurementSystem.end();
    if (measSys >= 0 && isValidMeaSys) {
        unitMeasSys = measurementSystem[measSys];
    }
    formatter = formatter.unitWidth(unitDisplay);
    formatter = formatter.precision(icu::number::Precision::maxFraction(DEFAULT_FRACTION_DIGITS));
}

void NumberFormat::InitDigitsProperties(icu::number::UnlocalizedNumberFormatter &formatter)
{
    int32_t status = 0;
    if (!maximumSignificantDigits.empty() || !minimumSignificantDigits.empty()) {
        int32_t maxSignificantDigits = ConvertString2Int(maximumSignificantDigits, status);
        if (status == 0) {
            formatter = formatter.precision(icu::number::Precision::maxSignificantDigits(maxSignificantDigits));
        }

        status = 0;
        int32_t minSignificantDigits = ConvertString2Int(minimumSignificantDigits, status);
        if (status == 0) {
            formatter = formatter.precision(icu::number::Precision::minSignificantDigits(minSignificantDigits));
        }
    } else {
        int32_t minIntegerDigits = ConvertString2Int(minimumIntegerDigits, status);
        if (status == 0 && minIntegerDigits > 1) {
            formatter = formatter.integerWidth(icu::number::IntegerWidth::zeroFillTo(minIntegerDigits));
        }

        int32_t minFdStatus = 0;
        int32_t minFractionDigits = ConvertString2Int(minimumFractionDigits, minFdStatus);
        int32_t maxFdStatus = 0;
        int32_t maxFractionDigits = ConvertString2Int(maximumFractionDigits, maxFdStatus);
        if (minFdStatus == 0 || maxFdStatus == 0) {
            isSetFraction = true;
        }
        if (minFdStatus == 0 && maxFdStatus != 0) {
            formatter = formatter.precision(icu::number::Precision::minFraction(minFractionDigits));
        } else if (minFdStatus != 0 && maxFdStatus == 0) {
            formatter = formatter.precision(icu::number::Precision::maxFraction(maxFractionDigits));
        } else if (minFdStatus == 0 && maxFdStatus == 0) {
            formatter =
                formatter.precision(icu::number::Precision::minMaxFraction(minFractionDigits, maxFractionDigits));
        }
    }
    SetRoundingProperties(formatter);
    HandleRoundingPriority(formatter);
}

void NumberFormat::SetRoundingProperties(icu::number::UnlocalizedNumberFormatter &formatter)
{
    if (roundingIncrement != 1) {
        int32_t maxFractionDigits = GetMaximumFractionDigitsValue();
        double increment = pow(POW_BASE_NUMBER, -1.0 * maxFractionDigits) * roundingIncrement;
        formatter = formatter.precision(icu::number::Precision::increment(increment));
    }
    if (!roundingModeStr.empty()) {
        formatter = formatter.roundingMode(roundingMode);
    }
}

void NumberFormat::HandleRoundingPriority(icu::number::UnlocalizedNumberFormatter &formatter)
{
    if (roundingPriorityStr.empty()) {
        return;
    }
    int32_t minFractionDigits = GetMinimumFractionDigitsValue();
    int32_t maxFractionDigits = GetMaximumFractionDigitsValue();
    int32_t maxSigDigits = GetMaximumSignificantDigitsIntValue();
    int32_t minSigDigits = GetMinimumSignificantDigitsIntValue();
    auto precision = icu::number::Precision::minMaxFraction(minFractionDigits, maxFractionDigits)
        .withSignificantDigits(minSigDigits, maxSigDigits, roundingPriority);
    formatter = formatter.precision(precision);
}

int32_t NumberFormat::GetMinimumSignificantDigitsIntValue()
{
    int32_t minSignifDigits = 1;
    if (!minimumSignificantDigits.empty()) {
        int32_t status = 0;
        int32_t minSignDigits = ConvertString2Int(minimumSignificantDigits, status);
        minSignifDigits = (status == 0) ? minSignDigits : minSignifDigits;
    }
    return minSignifDigits;
}

int32_t NumberFormat::GetMaximumSignificantDigitsIntValue()
{
    int32_t maxSign = DEFAULT_MAX_SIGNIFICANT_DIGITS;
    if (!maximumSignificantDigits.empty()) {
        int32_t status = 0;
        int32_t maxSignDigits = ConvertString2Int(maximumSignificantDigits, status);
        maxSign = (status == 0) ? maxSignDigits : maxSign;
    }
    return maxSign;
}

int32_t NumberFormat::GetMaximumFractionDigitsValue()
{
    int32_t maxFraction = DEFAULT_FRACTION_DIGITS;
    if (!maximumFractionDigits.empty()) {
        int32_t status = 0;
        int32_t maxFractionDigits = ConvertString2Int(maximumFractionDigits, status);
        maxFraction = (status == 0) ? maxFractionDigits : maxFraction;
    }
    return maxFraction;
}

int32_t NumberFormat::GetMinimumFractionDigitsValue()
{
    int32_t minFraction = 0;
    if (!minimumFractionDigits.empty()) {
        int32_t status = 0;
        int32_t minFractionDigits = ConvertString2Int(minimumFractionDigits, status);
        minFraction = (status == 0) ? minFractionDigits : minFraction;
    }
    return minFraction;
}

void NumberFormat::ParseConfigs(std::map<std::string, std::string> &configs)
{
    if (configs.count("signDisplay") > 0) {
        signDisplayString = configs["signDisplay"];
    }
    if (signAutoStyle.count(signDisplayString) > 0) {
        signDisplay = signAutoStyle[signDisplayString];
    }
    if (configs.count("style") > 0) {
        styleString = configs["style"];
    }
    if (styleString == "unit" && configs.count("unit") > 0) {
        unit = configs["unit"];
        if (configs.count("unitDisplay") > 0) {
            unitDisplayString = configs["unitDisplay"];
            if (unitStyle.count(unitDisplayString) > 0) {
                unitDisplay = unitStyle[unitDisplayString];
            }
        }
        if (configs.count("unitUsage") > 0) {
            unitUsage = configs["unitUsage"];
        }
    }
    if (styleString == "currency" && configs.count("currency") > 0) {
        currency = GetCurrencyFromConfig(configs["currency"]);
        if (configs.count("currencySign") > 0) {
            currencySign = configs["currencySign"];
        }
        if (currencySign.compare("accounting") == 0 && signAccountingStyle.count(signDisplayString) > 0) {
            signDisplay = signAccountingStyle[signDisplayString];
        }
        if (configs.count("currencyDisplay") > 0 && currencyStyle.count(configs["currencyDisplay"]) > 0) {
            currencyDisplayString = configs["currencyDisplay"];
            currencyDisplay = currencyStyle[currencyDisplayString];
        }
    }
    ParseDigitsConfigs(configs);
    ParseRoundingConfigs(configs);
}

void NumberFormat::ParseDigitsConfigs(std::map<std::string, std::string> &configs)
{
    if (configs.count("notation") > 0) {
        notationString = configs["notation"];
        if (notationString == "scientific") {
            notation = icu::number::Notation::scientific();
        } else if (notationString == "engineering") {
            notation = icu::number::Notation::engineering();
        }
        if (notationString == "compact") {
            if (configs.count("compactDisplay") > 0) {
                compactDisplay = configs["compactDisplay"];
            }
            if (compactDisplay == "long") {
                notation = icu::number::Notation::compactLong();
            } else {
                notation = icu::number::Notation::compactShort();
            }
        }
    }
    if (configs.count("minimumIntegerDigits") > 0) {
        minimumIntegerDigits = configs["minimumIntegerDigits"];
    }
    if (configs.count("minimumFractionDigits") > 0) {
        minimumFractionDigits = configs["minimumFractionDigits"];
    }
    if (configs.count("maximumFractionDigits") > 0) {
        maximumFractionDigits = configs["maximumFractionDigits"];
    }
    if (configs.count("minimumSignificantDigits") > 0) {
        minimumSignificantDigits = configs["minimumSignificantDigits"];
    }
    if (configs.count("maximumSignificantDigits") > 0) {
        maximumSignificantDigits = configs["maximumSignificantDigits"];
    }
    if (configs.count("numberingSystem") > 0) {
        numberingSystem = configs["numberingSystem"];
    }
    if (configs.count("useGrouping") > 0) {
        useGrouping = configs["useGrouping"];
    }
    if (configs.count("localeMatcher") > 0) {
        localeMatcher = configs["localeMatcher"];
    }
}

void NumberFormat::ParseRoundingConfigs(std::map<std::string, std::string> &configs)
{
    if (configs.count("roundingMode") > 0) {
        std::string tempRoundingModeStr = configs["roundingMode"];
        auto iter = roundingModeStyle.find(tempRoundingModeStr);
        if (iter != roundingModeStyle.end()) {
            roundingMode = iter->second;
            roundingModeStr = tempRoundingModeStr;
        }
    }
    if (configs.count("roundingPriority") > 0) {
        std::string tempRoundingPriorityStr = configs["roundingPriority"];
        auto iter = roundingPriorityStyle.find(tempRoundingPriorityStr);
        if (iter != roundingPriorityStyle.end()) {
            roundingPriorityStr = tempRoundingPriorityStr;
            roundingPriority = iter->second;
        }
    }
    if (configs.count("roundingIncrement") > 0) {
        std::string increment = configs["roundingIncrement"];
        int32_t status = 0;
        int32_t incrementNum = ConvertString2Int(increment, status);
        if (status != 0) {
            HILOG_ERROR_I18N("NumberFormat::ParseRoundingConfigs: Convert string to int failed.");
            return;
        }
        auto iter = std::find(roundingIncrementList.begin(), roundingIncrementList.end(),
            static_cast<size_t>(incrementNum));
        if (iter != roundingIncrementList.end()) {
            roundingIncrement = *iter;
        }
    }
}

void NumberFormat::SetUnit(std::string &preferredUnit)
{
    if (preferredUnit.empty()) {
        return;
    }
    for (icu::MeasureUnit curUnit : unitArray) {
        if (!strcmp(curUnit.getSubtype(), preferredUnit.c_str())) {
            numberFormat = numberFormat.unit(curUnit);
        }
    }
}

std::string NumberFormat::Format(double number)
{
    std::string result;
    UErrorCode status = U_ZERO_ERROR;
    icu::UnicodeString formatResult;
    if (IsRelativeTimeFormat(number)) {
        icu::FormattedRelativeDateTime formattedTime = FormatToFormattedRelativeDateTime(number);
        formatResult = formattedTime.toString(status);
    } else {
        icu::number::FormattedNumber formattedNumber = FormatToFormattedNumber(number);
        formatResult = formattedNumber.toString(status);
    }
    if (U_FAILURE(status)) {
        result = "";
    } else {
        formatResult.toUTF8String(result);
    }
    return PseudoLocalizationProcessor(result);
}

std::string NumberFormat::FormatBigInt(int64_t number)
{
    std::string result;
    UErrorCode status = U_ZERO_ERROR;
    std::string numStr = std::to_string(number);
    icu::number::FormattedNumber formattedNumber = FormatDecimalToFormattedNumber(numStr);
    icu::UnicodeString formatResult = formattedNumber.toString(status);
    if (U_SUCCESS(status)) {
        formatResult.toUTF8String(result);
    }
    return PseudoLocalizationProcessor(result);
}

void NumberFormat::FormatToParts(std::vector<std::vector<std::string>> &result)
{
    double number = uprv_getNaN();
    icu::number::FormattedNumber formattedNumber = FormatToFormattedNumber(number);
    FormattedNumberParts(formattedNumber, result, "nan", number);
}

void NumberFormat::FormatBigIntToParts(int64_t number, std::vector<std::vector<std::string>> &result)
{
    std::string numStr = std::to_string(number);
    double numDouble = static_cast<double>(number);
    icu::number::FormattedNumber formattedNumber = FormatDecimalToFormattedNumber(numStr);
    FormattedNumberParts(formattedNumber, result, "bigint", numDouble);
}

void NumberFormat::FormatToParts(double number, std::vector<std::vector<std::string>> &result)
{
    std::string finalUnit = unit;
    if (IsRelativeTimeFormat(number, finalUnit)) {
        RelativeDateTimeFormatToParts(number, finalUnit, result);
        return;
    }
    icu::number::FormattedNumber formattedNumber = FormatToFormattedNumber(number);
    FormattedNumberParts(formattedNumber, result, "number", number);
}

void NumberFormat::FormattedNumberParts(icu::number::FormattedNumber &formattedNumber,
    std::vector<std::vector<std::string>> &result, const std::string &napiType, double number)
{
    UErrorCode status = U_ZERO_ERROR;
    icu::UnicodeString formatResult = formattedNumber.toString(status);
    if (U_FAILURE(status)) {
        HILOG_ERROR_I18N("NumberFormat::FormatToParts: UnicodeString toString failed.");
        return;
    }
    FindAllFormatParts(formattedNumber, result, number, napiType, formatResult);
}

void NumberFormat::FindAllFormatParts(icu::FormattedValue &formattedNumber,
    std::vector<std::vector<std::string>> &result, double number,
    const std::string &napiType, icu::UnicodeString &formatResult)
{
    icu::ConstrainedFieldPosition cfpo;
    int previousLimit = 0;
    bool lastFieldGroup = false;
    int groupLeapLength = 0;
    UErrorCode status = U_ZERO_ERROR;
    struct FormatPartParam param = {previousLimit, lastFieldGroup, groupLeapLength, number, napiType};
    while (formattedNumber.nextPosition(cfpo, status)) {
        if (U_FAILURE(status)) {
            HILOG_ERROR_I18N("NumberFormat::FormatToParts: Get next position failed.");
            return;
        }
        SetEveryFormatPartItem(cfpo, formatResult, result, param);
    }
    if (formatResult.length() > param.previousLimit) {
        std::string typeString = "literal";
        AddFormatParts(typeString, result, formatResult, param.previousLimit, formatResult.length());
    }
}

void NumberFormat::SetEveryFormatPartItem(icu::ConstrainedFieldPosition &cfpo, icu::UnicodeString &formatResult,
    std::vector<std::vector<std::string>> &result, FormatPartParam &param)
{
    int32_t fieldId = cfpo.getField();
    int32_t start = cfpo.getStart();
    int32_t limit = cfpo.getLimit();
    std::string typeString = "literal";
    if (static_cast<UFieldCategory>(cfpo.getCategory()) == UFIELD_CATEGORY_NUMBER_RANGE_SPAN) {
        if (param.previousLimit < start) {
            AddFormatParts(typeString, result, formatResult, param.previousLimit, start);
            param.previousLimit = start;
        }
        return;
    }
    if (static_cast<UNumberFormatFields>(fieldId) == UNUM_GROUPING_SEPARATOR_FIELD) {
        typeString = "integer";
        AddFormatParts(typeString, result, formatResult, param.previousLimit, start);
        {
            typeString = GetNumberFieldType(param.napiType, fieldId, param.number);
            AddFormatParts(typeString, result, formatResult, start, limit);
        }
        param.lastFieldGroup = true;
        param.groupLeapLength = start - param.previousLimit + 1;
        param.previousLimit = limit;
        return;
    } else if (start > param.previousLimit) {
        AddFormatParts(typeString, result, formatResult, param.previousLimit, start);
    }
    if (param.lastFieldGroup) {
        start = start + param.groupLeapLength;
        param.lastFieldGroup = false;
    }
    if (styleString.compare("unit") == 0 && static_cast<UNumberFormatFields>(fieldId) == UNUM_PERCENT_FIELD) {
        typeString = "unit";
    } else {
        typeString = GetNumberFieldType(param.napiType, fieldId, param.number);
    }
    if (start < param.previousLimit && param.previousLimit < limit) {
        AddFormatParts(typeString, result, formatResult, param.previousLimit, limit);
    } else {
        AddFormatParts(typeString, result, formatResult, start, limit);
    }
    param.previousLimit = limit;
}

std::string NumberFormat::GetNumberFieldType(std::string napiType, int32_t fieldId, double number)
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
            return "";
    }
}

void NumberFormat::AddFormatParts(std::string typeString, std::vector<std::vector<std::string>> &result,
    icu::UnicodeString &formatResult, int32_t start, int32_t end)
{
    std::string value = GetSubString(formatResult, start, end);
    std::vector<std::string> info;
    info.push_back(typeString);
    info.push_back(value);
    result.push_back(info);
}

std::string NumberFormat::GetSubString(icu::UnicodeString &formatResult, int32_t start, int32_t end)
{
    icu::UnicodeString substring = formatResult.tempSubStringBetween(start, end);
    std::string val;
    substring.toUTF8String(val);
    return val;
}

void NumberFormat::RelativeDateTimeFormatToParts(double number, std::string &finalUnit,
    std::vector<std::vector<std::string>> &result)
{
    if (!createSuccess) {
        return;
    }
    double finalNumber = number;
    relativeTimeFormat->FormatToParts(finalNumber, finalUnit, result);
}

bool NumberFormat::IsRelativeTimeFormat(double number)
{
    double finalNumber = number;
    std::string finalUnit = unit;
    return IsRelativeTimeFormat(finalNumber, finalUnit);
}

bool NumberFormat::IsRelativeTimeFormat(double number, std::string &unitForConvert)
{
    double finalNumber = number;
    if (unitUsage.compare("elapsed-time-second") == 0 && ConvertDate(finalNumber, unitForConvert) &&
        relativeTimeFormat != nullptr) {
        return true;
    }
    return false;
}

icu::number::FormattedNumber NumberFormat::FormatDecimalToFormattedNumber(std::string number)
{
    if (!createSuccess) {
        return {};
    }
    UErrorCode status = U_ZERO_ERROR;
    icu::number::FormattedNumber formattedNumber =
        numberFormat.formatDecimal(icu::StringPiece(number), status);
    if (U_FAILURE(status)) {
        return {};
    }
    return formattedNumber;
}

icu::number::FormattedNumber NumberFormat::FormatToFormattedNumber(double number)
{
    if (!createSuccess) {
        return {};
    }
    double finalNumber = number;
    std::string finalUnit = unit;
    if ((unitUsage.compare("size-file-byte") == 0 || unitUsage.compare("size-shortfile-byte") == 0) &&
        ConvertByte(finalNumber, finalUnit)) {
        SetUnit(finalUnit);
        SetPrecisionWithByte(finalNumber, finalUnit);
    } else if (!unitUsage.empty()) {
        std::vector<std::string> preferredUnits;
        if (unitUsage.compare("default") == 0) {
            GetDefaultPreferredUnit(localeInfo->GetRegion(), unitType, preferredUnits);
        } else {
            GetPreferredUnit(localeInfo->GetRegion(), unitUsage, preferredUnits);
        }
        std::map<double, std::string> preferredValuesOverOne;
        std::map<double, std::string> preferredValuesUnderOne;
        double num = number;
        for (size_t i = 0; i < preferredUnits.size(); i++) {
            int status = Convert(num, unit, unitMeasSys, preferredUnits[i], unitMeasSys);
            if (!status) {
                continue;
            }
            if (num >= 1) {
                preferredValuesOverOne.insert(std::make_pair(num, preferredUnits[i]));
            } else {
                preferredValuesUnderOne.insert(std::make_pair(num, preferredUnits[i]));
            }
        }
        std::string preferredUnit;
        if (preferredValuesOverOne.size() > 0) {
            finalNumber = preferredValuesOverOne.begin()->first;
            preferredUnit = preferredValuesOverOne.begin()->second;
        } else if (preferredValuesUnderOne.size() > 0) {
            finalNumber = preferredValuesUnderOne.rbegin()->first;
            preferredUnit = preferredValuesUnderOne.rbegin()->second;
        }
        SetUnit(preferredUnit);
    }
    UErrorCode icuStatus = U_ZERO_ERROR;
    icu::number::FormattedNumber formattedNumber = numberFormat.formatDouble(finalNumber, icuStatus);
    if (U_FAILURE(icuStatus)) {
        return {};
    }
    return formattedNumber;
}

icu::FormattedRelativeDateTime NumberFormat::FormatToFormattedRelativeDateTime(double number)
{
    if (!createSuccess) {
        return {};
    }

    double finalNumber = number;
    std::string finalUnit = unit;
    if (unitUsage.compare("elapsed-time-second") == 0 && ConvertDate(finalNumber, finalUnit) &&
        relativeTimeFormat != nullptr) {
        return relativeTimeFormat->FormatToFormattedValue(finalNumber, finalUnit);
    }
    return {};
}

std::string NumberFormat::FormatRange(double start, double end)
{
    if (!createSuccess) {
        HILOG_ERROR_I18N("FormatRange: init numberRangeFormat failed.");
        return PseudoLocalizationProcessor("");
    }
    if (end < start) {
        HILOG_ERROR_I18N("FormatRange: parameter:end less then parameter:start");
        return PseudoLocalizationProcessor("");
    }
    UErrorCode status = U_ZERO_ERROR;
    icu::number::FormattedNumberRange numberRange =
        numberRangeFormat.formatFormattableRange(start, end, status);
    if (U_FAILURE(status)) {
        HILOG_ERROR_I18N("FormatRange: formatFormattableRange failed.");
        return PseudoLocalizationProcessor("");
    }
    icu::UnicodeString res = numberRange.toString(status);
    if (U_FAILURE(status)) {
        HILOG_ERROR_I18N("FormatRange: icu::UnicodeString.toString() failed.");
        return PseudoLocalizationProcessor("");
    }
    std::string result;
    res.toUTF8String(result);
    return PseudoLocalizationProcessor(result);
}

void NumberFormat::FormatRangeToParts(double start, double end,
    std::vector<std::vector<std::string>> &result)
{
    if (!createSuccess) {
        HILOG_ERROR_I18N("NumberFormat::FormatRangeToParts: init numberRangeFormat failed.");
        return;
    }
    if (end < start) {
        HILOG_ERROR_I18N("NumberFormat::FormatRangeToParts: parameter:end less then parameter:start");
        return;
    }
    UErrorCode status = U_ZERO_ERROR;
    icu::number::FormattedNumberRange numberRange =
        numberRangeFormat.formatFormattableRange(start, end, status);
    if (U_FAILURE(status)) {
        HILOG_ERROR_I18N("NumberFormat::FormatRangeToParts: formatFormattableRange failed.");
        return;
    }
    icu::UnicodeString res = numberRange.toString(status);
    if (U_FAILURE(status)) {
        HILOG_ERROR_I18N("NumberFormat::FormatRangeToParts: icu::UnicodeString.toString() failed.");
        return;
    }
    FindAllFormatParts(numberRange, result, start, "number", res);
}

void NumberFormat::GetResolvedOptions(std::map<std::string, std::string> &map, bool setDefault)
{
    map.insert(std::make_pair("locale", localeBaseName));
    if (!styleString.empty()) {
        map.insert(std::make_pair("style", styleString));
    } else {
        if (setDefault) {
            map.insert(std::make_pair("style", "decimal"));
        }
    }
    if (!currency.empty()) {
        map.insert(std::make_pair("currency", currency));
    }
    if (!currencySign.empty()) {
        map.insert(std::make_pair("currencySign", currencySign));
    } else if (setDefault) {
        map.insert(std::make_pair("currencySign", "standard"));
    }
    if (!currencyDisplayString.empty()) {
        map.insert(std::make_pair("currencyDisplay", currencyDisplayString));
    } else if (setDefault) {
        map.insert(std::make_pair("currencyDisplay", "symbol"));
    }
    if (!signDisplayString.empty()) {
        map.insert(std::make_pair("signDisplay", signDisplayString));
    } else if (setDefault) {
        map.insert(std::make_pair("signDisplay", "auto"));
    }
    if (!compactDisplay.empty()) {
        map.insert(std::make_pair("compactDisplay", compactDisplay));
    }
    if (!unitDisplayString.empty()) {
        map.insert(std::make_pair("unitDisplay", unitDisplayString));
    }
    if (!unitUsage.empty()) {
        map.insert(std::make_pair("unitUsage", unitUsage));
    }
    if (!unit.empty()) {
        map.insert(std::make_pair("unit", unit));
    }
    GetDigitsResolvedOptions(map, setDefault);
    GetDigitsResolvedOptionsExt(map, setDefault);
}

std::set<std::string> NumberFormat::GetNumberAvailableLocales()
{
    std::set<std::string> result;
    int32_t count = 0;
    const icu::Locale* locales = icu::NumberFormat::getAvailableLocales(count);
    if (count < 1 || locales == nullptr) {
        HILOG_ERROR_I18N("NumberFormat::GetNumberAvailableLocales: Get available locales failed.");
        return result;
    }
    for (int i = 0; i < count; i++) {
        const char* name = locales[i].getName();
        if (name != nullptr) {
            result.insert(name);
        }
    }
    return result;
}

std::vector<std::string> NumberFormat::SupportedLocalesOf(const std::vector<std::string> &requestLocales,
    const std::map<std::string, std::string> &configs, I18nErrorCode &status)
{
    std::vector<std::string> undefined = {};
    auto requestedLocales = LocaleHelper::CanonicalizeLocaleList(requestLocales, status);
    if (status != I18nErrorCode::SUCCESS) {
        return undefined;
    }
    std::string localeMatcher = LocaleHelper::ParseOption(configs, "localeMatcher", "best fit", true, status);
    if (status != I18nErrorCode::SUCCESS) {
        return undefined;
    }
    std::set<std::string> availableLocales = GetNumberAvailableLocales();
    return LocaleHelper::LookupSupportedLocales(availableLocales, requestedLocales);
}

void NumberFormat::GetDigitsResolvedOptions(std::map<std::string, std::string> &map, bool setDefault)
{
    if (!numberingSystem.empty()) {
        map.insert(std::make_pair("numberingSystem", numberingSystem));
    } else if (!(localeInfo->GetNumberingSystem()).empty()) {
        map.insert(std::make_pair("numberingSystem", localeInfo->GetNumberingSystem()));
    } else {
        UErrorCode status = U_ZERO_ERROR;
        auto numSys = std::unique_ptr<icu::NumberingSystem>(icu::NumberingSystem::createInstance(locale, status));
        if (U_SUCCESS(status)) {
            map.insert(std::make_pair("numberingSystem", numSys->getName()));
        }
    }
    if (!useGrouping.empty()) {
        map.insert(std::make_pair("useGrouping", useGrouping));
    } else if (setDefault) {
        map.insert(std::make_pair("useGrouping", "true"));
    }
    if (!minimumIntegerDigits.empty()) {
        map.insert(std::make_pair("minimumIntegerDigits", minimumIntegerDigits));
    } else if (setDefault) {
        map.insert(std::make_pair("minimumIntegerDigits", "1"));
    }
    if (!minimumFractionDigits.empty()) {
        map.insert(std::make_pair("minimumFractionDigits", minimumFractionDigits));
    } else if (setDefault) {
        GetMinimumFractionDigitsOption(map);
    }
    if (!maximumFractionDigits.empty()) {
        map.insert(std::make_pair("maximumFractionDigits", maximumFractionDigits));
    } else if (setDefault) {
        GetMaximumFractionDigitsOption(map);
    }
    if (!minimumSignificantDigits.empty()) {
        map.insert(std::make_pair("minimumSignificantDigits", minimumSignificantDigits));
    }
    if (!maximumSignificantDigits.empty()) {
        map.insert(std::make_pair("maximumSignificantDigits", maximumSignificantDigits));
    }
}

void NumberFormat::GetDigitsResolvedOptionsExt(std::map<std::string, std::string> &map, bool setDefault)
{
    if (!localeMatcher.empty()) {
        map.insert(std::make_pair("localeMatcher", localeMatcher));
    }
    if (!notationString.empty()) {
        map.insert(std::make_pair("notation", notationString));
    } else if (setDefault) {
        map.insert(std::make_pair("notation", "standard"));
    }
    if (!roundingModeStr.empty()) {
        map.insert(std::make_pair("roundingMode", roundingModeStr));
    }
    if (!roundingPriorityStr.empty()) {
        map.insert(std::make_pair("roundingPriority", roundingPriorityStr));
    }
    if (roundingIncrement != 1) {
        map.insert(std::make_pair("roundingIncrement", std::to_string(roundingIncrement)));
    }
}

void NumberFormat::GetMinimumFractionDigitsOption(std::map<std::string, std::string> &map)
{
    std::string style = "decimal";
    if (map.find("style") != map.end()) {
        style = map["style"];
    }
    if (style == "decimal" || style == "percent") {
        map.insert(std::make_pair("minimumFractionDigits", "0"));
    } else if (style == "currency") {
        int fraction = GetCurrencyFractionDigits(map);
        map.insert(std::make_pair("minimumFractionDigits", std::to_string(fraction)));
    }
}

void NumberFormat::GetMaximumFractionDigitsOption(std::map<std::string, std::string> &map)
{
    std::string style = "decimal";
    if (map.find("style") != map.end()) {
        style = map["style"];
    }
    if (style == "decimal") {
        map.insert(std::make_pair("maximumFractionDigits", "3"));
    } else if (style == "percent") {
        map.insert(std::make_pair("maximumFractionDigits", "0"));
    } else if (style == "currency") {
        int fraction = GetCurrencyFractionDigits(map);
        map.insert(std::make_pair("maximumFractionDigits", std::to_string(fraction)));
    }
}

int NumberFormat::GetCurrencyFractionDigits(std::map<std::string, std::string> &map)
{
    int result = 2; // 2 is default value
    std::string currency;
    if (map.find("currency") != map.end()) {
        currency = map["currency"];
        currency = GetCurrencyFromConfig(currency);
    }
    if (defaultCurrencyFractionMap.find(currency) != defaultCurrencyFractionMap.end()) {
        result = defaultCurrencyFractionMap[currency];
    }
    return result;
}

void NumberFormat::SetPrecisionWithByte(double number, const std::string& finalUnit)
{
    if (isSetFraction) {
        return;
    }
    int32_t FractionDigits = -1;
    // 100 is the threshold between different decimal
    if (finalUnit == "byte" || number >= 100) {
        FractionDigits = 0;
    } else if (number < 1) {
        // 2 is the number of significant digits in the decimal
        FractionDigits = 2;
    // 10 is the threshold between different decimal
    } else if (number < 10) {
        if (unitUsage == "size-shortfile-byte") {
            FractionDigits = 1;
        } else {
            // 2 is the number of significant digits in the decimal
            FractionDigits = 2;
        }
    } else {
        if (unitUsage == "size-shortfile-byte") {
            FractionDigits = 0;
        } else {
            // 2 is the number of significant digits in the decimal
            FractionDigits = 2;
        }
    }
    if (FractionDigits != -1) {
        numberFormat = numberFormat.precision(icu::number::Precision::minMaxFraction(FractionDigits, FractionDigits));
    }
}

std::string NumberFormat::GetCurrency() const
{
    return currency;
}

std::string NumberFormat::GetCurrencySign() const
{
    return currencySign;
}

std::string NumberFormat::GetStyle() const
{
    return styleString;
}

std::string NumberFormat::GetNumberingSystem() const
{
    return numberingSystem;
}

std::string NumberFormat::GetUseGrouping() const
{
    return useGrouping;
}

std::string NumberFormat::GetMinimumIntegerDigits() const
{
    return minimumIntegerDigits;
}

std::string NumberFormat::GetMinimumFractionDigits() const
{
    return minimumFractionDigits;
}

std::string NumberFormat::GetMaximumFractionDigits() const
{
    return maximumFractionDigits;
}

std::string NumberFormat::GetMinimumSignificantDigits() const
{
    return minimumSignificantDigits;
}

std::string NumberFormat::GetMaximumSignificantDigits() const
{
    return maximumSignificantDigits;
}

std::string NumberFormat::GetLocaleMatcher() const
{
    return localeMatcher;
}

bool NumberFormat::Init()
{
    SetHwIcuDirectory();
    return true;
}

void NumberFormat::SetDefaultStyle()
{
    char value[BUFFER_LEN];
    int code = GetParameter(DEVICE_TYPE_NAME, "", value, BUFFER_LEN);
    if (code > 0) {
        std::string deviceType = value;
        if (defaultUnitStyle.find(deviceType) != defaultUnitStyle.end()) {
            unitDisplay = defaultUnitStyle[deviceType];
        }
        if (defaultCurrencyStyle.find(deviceType) != defaultCurrencyStyle.end()) {
            currencyDisplay = defaultCurrencyStyle[deviceType];
        }
    }
}

bool NumberFormat::ReadISO4217Datas()
{
    if (initISO4217) {
        return true;
    }
    std::lock_guard<std::mutex> readDataLock(numToCurrencyMutex);
    if (initISO4217) {
        return true;
    }
    UErrorCode status = U_ZERO_ERROR;
    const char *currentCurrency;
    UEnumeration *currencies = ucurr_openISOCurrencies(UCURR_ALL, &status);
    if (U_FAILURE(status)) {
        return false;
    }

    UChar code[CURRENCY_LEN + 1];  // +1 includes the NUL
    int32_t length = 0;
    while ((currentCurrency = uenum_next(currencies, &length, &status)) != NULL) {
        u_charsToUChars(currentCurrency, code, length + 1);  // +1 includes the NUL
        int32_t numCode = ucurr_getNumericCode(code);
        if (numCode == 0) {
            continue;
        }
        std::stringstream ss;
        ss << std::setw(CURRENCY_LEN) << std::setfill('0') << numCode; // fill with '0'
        numToCurrency.insert(std::make_pair<std::string, std::string>(ss.str(), currentCurrency));
    }
    uenum_close(currencies);
    initISO4217 = true;
    return !numToCurrency.empty();
}

std::string NumberFormat::GetCurrencyFromConfig(const std::string& currency)
{
    if (currency.size() != CURRENCY_LEN) {
        HILOG_ERROR_I18N("Invalid currency code : %{public}s", currency.c_str());
        return "";
    }
    bool isAlpha = true;
    for (auto c : currency) {
        isAlpha = std::isalpha(c);
        if (!isAlpha) {
            break;
        }
    }
    if (isAlpha) {
        return currency;
    }
    if (ReadISO4217Datas() && numToCurrency.find(currency) != numToCurrency.end()) {
        return numToCurrency[currency];
    }
    HILOG_ERROR_I18N("Invalid currency code : %{public}s", currency.c_str());
    return "";
}

std::string NumberFormat::GetIcuDefaultLocale()
{
    return "en-US";
}

std::string NumberFormat::CheckParamLocales(std::vector<std::string> &localeArray)
{
    for (auto iter = localeArray.begin(); iter != localeArray.end(); ++iter) {
        UErrorCode status = U_ZERO_ERROR;
        icu::Locale locale = icu::Locale::forLanguageTag(icu::StringPiece(*iter), status);
        if (U_FAILURE(status)) {
            HILOG_ERROR_I18N("NumberFormat::CheckParamLocales: locale %{public}s invalid", (*iter).c_str());
            return "invalid locale";
        }
        if (LocaleInfo::allValidLocales.count(locale.getLanguage()) < 1) {
            HILOG_ERROR_I18N("NumberFormat::CheckParamLocales: invalid locale: %{public}s ", (*iter).c_str());
            return "invalid locale";
        }
    }
    return "";
}

std::string NumberFormat::CheckNumberFormatOptions(std::map<std::string, std::string> &map, int32_t &code)
{
    if (map.find("style") != map.end()) {
        std::string style = map["style"];
        if (style.compare("currency") == 0 && map.find("currency") == map.end()) {
            code = -1;
            return "style is currency but currency is undefined";
        }
    }
    if (map.find("currency") != map.end()) {
        std::string currency = map["currency"];
        std::string checkCurr = GetCurrencyFromConfig(currency);
        if (checkCurr.empty()) {
            code = -1;
            return "not a wellformed code";
        }
    }
    if (map.find("localeMatcher") != map.end()) {
        I18nErrorCode status = I18nErrorCode::SUCCESS;
        std::string localeMatcher = LocaleHelper::ParseOption(map, "localeMatcher", "best fit", false, status);
        if (status != I18nErrorCode::SUCCESS) {
            code = -1;
            return "getStringOption failed";
        }
    }
    if (map.find("currencySign") != map.end()) {
        std::string currencySign = map["currencySign"];
        if (currencySign.compare("standard") != 0 && currencySign.compare("accounting") != 0) {
            code = -1;
            return "getStringOption failed";
        }
    }
    std::vector<std::tuple<std::string, int, int>> vect = {
        {"minimumIntegerDigits", 1, 21}, // 21 is maximum of minimumIntegerDigits
        {"minimumFractionDigits", 0, 20}, // 20 is maximum of minimumFractionDigits
        {"maximumFractionDigits", 0, 21}, // 21 is maximum of maximumFractionDigits
        {"minimumSignificantDigits", 1, 21}, // 21 is maximum of minimumSignificantDigits
        {"maximumSignificantDigits", 1, 21}, // 21 is maximum of maximumSignificantDigits
    };
    for (auto iter = vect.begin(); iter < vect.end(); ++iter) {
        std::string optionName = std::get<0>(*iter);
        int min = std::get<1>(*iter); // 1 is item index
        int max = std::get<2>(*iter); // 2 is item index
        std::string msg = CheckNumberFormatOptionsExt(map, code, optionName, min, max);
        if (code != 0) {
            return msg;
        }
    }
    return "";
}

std::string NumberFormat::CheckNumberFormatOptionsExt(std::map<std::string, std::string> &map, int32_t &code,
    const std::string &optionName, int min, int max)
{
    if (map.find(optionName) != map.end()) {
        std::string optionNameStr = map[optionName];
        int32_t status = 0;
        int32_t optionValue = ConvertString2Int(optionNameStr, status);
        if (status != 0) {
            code = -1;
            return "";
        }
        if (optionValue < min || optionValue > max) {
            code = -1;
            return "";
        }
    }
    return "";
}
} // namespace I18n
} // namespace Global
} // namespace OHOS
