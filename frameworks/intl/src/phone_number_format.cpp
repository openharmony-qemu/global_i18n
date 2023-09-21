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
#include "phone_number_format.h"

#include <dlfcn.h>

#include "localebuilder.h"
#include "locale_config.h"
#include "locid.h"
#include "map"
#include "new"
#include "set"
#include "securec.h"
#include "string"
#include "taboo_utils.h"
#include "utility"

#include "hilog/log.h"


namespace OHOS {
namespace Global {
namespace I18n {
static constexpr OHOS::HiviewDFX::HiLogLabel LABEL = { LOG_CORE, 0xD001E00, "WJTest" };
using namespace OHOS::HiviewDFX;

const int RECV_CHAR_LEN = 128;
using i18n::phonenumbers::PhoneNumberUtil;

PhoneNumberFormat::PhoneNumberFormat(const std::string &countryTag,
                                     const std::map<std::string, std::string> &options)
{
    util = PhoneNumberUtil::GetInstance();
    if (LocaleConfig::IsValidRegion(countryTag)) {
        country = countryTag;
    } else {
        icu::Locale locale = icu::Locale::createFromName(countryTag.c_str());
        country = locale.getCountry();
    }
    std::string type = "";
    auto search = options.find("type");
    if (search != options.end()) {
        type = search->second;
    }

    std::map<std::string, PhoneNumberUtil::PhoneNumberFormat> type2PhoneNumberFormat = {
        {"E164", PhoneNumberUtil::PhoneNumberFormat::E164},
        {"RFC3966", PhoneNumberUtil::PhoneNumberFormat::RFC3966},
        {"INTERNATIONAL", PhoneNumberUtil::PhoneNumberFormat::INTERNATIONAL},
        {"NATIONAL", PhoneNumberUtil::PhoneNumberFormat::NATIONAL}
    };

    std::set<std::string> validType = {"E164", "RFC3966", "INTERNATIONAL", "NATIONAL"};
    if (validType.find(type) != validType.end()) {
        phoneNumberFormat = type2PhoneNumberFormat[type];
    } else {
        phoneNumberFormat = PhoneNumberUtil::PhoneNumberFormat::NATIONAL;
    }
}

PhoneNumberFormat::~PhoneNumberFormat()
{
    if (g_dynamicHandler != NULL) {
        dlclose(g_dynamicHandler);
        g_dynamicHandler = nullptr;
        g_func = nullptr;
    }
}

std::unique_ptr<PhoneNumberFormat> PhoneNumberFormat::CreateInstance(const std::string &countryTag,
    const std::map<std::string, std::string> &options)
{
    std::unique_ptr<PhoneNumberFormat> phoneNumberFormat = std::make_unique<PhoneNumberFormat>(countryTag, options);
    if (phoneNumberFormat->GetPhoneNumberUtil() == nullptr) {
        return nullptr;
    }
    return phoneNumberFormat;
}

PhoneNumberUtil* PhoneNumberFormat::GetPhoneNumberUtil()
{
    return util;
}

bool PhoneNumberFormat::isValidPhoneNumber(const std::string &number) const
{
    i18n::phonenumbers::PhoneNumber phoneNumber;
    PhoneNumberUtil::ErrorType type = util->Parse(number, country, &phoneNumber);
    if (type != PhoneNumberUtil::ErrorType::NO_PARSING_ERROR) {
        return false;
    }
    return util->IsValidNumber(phoneNumber);
}

std::string PhoneNumberFormat::format(const std::string &number) const
{
    i18n::phonenumbers::PhoneNumber phoneNumber;
    PhoneNumberUtil::ErrorType type = util->Parse(number, country, &phoneNumber);
    if (type != PhoneNumberUtil::ErrorType::NO_PARSING_ERROR) {
        return "";
    }
    std::string formatted_number;
    util->Format(phoneNumber, phoneNumberFormat, &formatted_number);
    return formatted_number;
}

std::string PhoneNumberFormat::getLocationName(const std::string &number, const std::string &locale)
{
    // Stitch locale and local country
    UErrorCode status = U_ZERO_ERROR;
    icu::Locale displayLocale = icu::Locale::createFromName(locale.c_str());
    displayLocale.addLikelySubtags(status);
    icu::LocaleBuilder builder = icu::LocaleBuilder().setRegion(country);
    builder.setLanguage(displayLocale.getLanguage());
    builder.setScript(displayLocale.getScript());
    icu::Locale phoneLocale = builder.build(status);
    HiLog::Error(LABEL, "Country is %{public}s, display locale is %{public}s, phone locale is %{public}s",
        country.c_str(), displayLocale.getName(), phoneLocale.getName());

    // Get region code
    i18n::phonenumbers::PhoneNumber phoneNumber;
    PhoneNumberUtil::ErrorType type = util->Parse(number, phoneLocale.getCountry(), &phoneNumber);
    if (type != PhoneNumberUtil::ErrorType::NO_PARSING_ERROR) {
        HiLog::Error(LABEL, "Failed to get phoneNumber.");
        return "";
    }
    std::string regionCode;
    util->GetRegionCodeForNumber(phoneNumber, &regionCode);
    HiLog::Error(LABEL, "Region code is %{public}s.", regionCode.c_str());

    // Get region name by libegocoding and LocaleConfig. (Maybe country name or city name)
    std::string locName = getPhoneLocationName(number, phoneLocale.getName(), locale, regionCode);

    // Get the country name by ICU
    icu::LocaleBuilder regionbuilder = icu::LocaleBuilder().setRegion(regionCode);
    icu::Locale regionLocale = builder.build(status);
    icu::UnicodeString displayRegion;
    regionLocale.getDisplayCountry(displayLocale, displayRegion);
    std::string displayCountryName;
    displayRegion.toUTF8String(displayCountryName);
    HiLog::Error(LABEL, "LocName is %{public}s, displaycountry name is %{public}s.", locName.c_str(), displayCountryName.c_str());
    // Check if the country name
    if (locName.compare(displayCountryName) == 0) {
        HiLog::Error(LABEL, "Get country name.");
        // Check black country code (region code)
        if (getBlockedCountryName(regionCode, displayLocale.getLanguage())) {
            HiLog::Error(LABEL, "Country name blocked.");
            return "";
        }

        // Check replace country code
        TabooUtils taboo;
        std::string replacyCountryName = taboo.ReplaceCountryName(regionCode, displayLocale.getName(), regionCode);
        if (replacyCountryName.compare(regionCode) != 0) {
            HiLog::Error(LABEL, "Country name replaced.");
            return replacyCountryName;
        }

        return displayCountryName;
    }

    std::string formatted_number;
    util->Format(phoneNumber, PhoneNumberUtil::PhoneNumberFormat::E164, &formatted_number);
    HiLog::Error(LABEL, "Formatted number is %{public}s.", formatted_number.c_str());

    // Get the city name by libi18n_util.z.so
    return getCityName(displayLocale.getName(), formatted_number.substr(1), locName);
}

std::string PhoneNumberFormat::getPhoneLocationName(const std::string& number, const std::string& phoneLocale,
    const std::string& displayLocale, const std::string& regionCode)
{
    if (g_dynamicHandler == NULL) {
#ifndef SUPPORT_ASAN
        const char* geocodingSO = "libgeocoding.z.so";
#else
        const char* geocodingSO = "system/asan/lib64/libgeocoding.z.so";
#endif
        g_dynamicHandler = dlopen(geocodingSO, RTLD_NOW);
    }
    if (g_dynamicHandler && !g_func) {
        g_func = reinterpret_cast<ExposeLocationName>(dlsym(g_dynamicHandler, "exposeLocationName"));
    }
    std::string locName;
    if (g_func) {
        // It need to be careful about exposeLocationName function, it use the same locale as phone and display.
        HiLog::Error(LABEL, "Get location name from libgeocoding.");
        char recvArr[RECV_CHAR_LEN];
        g_func(number.c_str(), phoneLocale.c_str(), recvArr);
        locName = recvArr;
    }
    if (!g_func || locName.empty()) {
        HiLog::Error(LABEL, "Get location name from LocaleConfig.");
        locName = LocaleConfig::GetDisplayRegion(regionCode, displayLocale, false);
    }
    return locName;
}

bool PhoneNumberFormat::getBlockedCountryName(const std::string& regionCode, const std::string& language)
{
    void *i18nUtilHandle = dlopen("libi18n_util.z.so", RTLD_NOW);
    if (i18nUtilHandle == nullptr) {
        return false;
    }
    GetBlockedRegionName getBlockedRegionName = (GetBlockedRegionName)dlsym(i18nUtilHandle, "GetBlockedRegionName");
    return getBlockedRegionName(language.c_str(), regionCode.c_str());
}

std::string PhoneNumberFormat::getCityName(const std::string& language, const std::string& phonenumber,
    const std::string& locationName)
{
    char recvArr[RECV_CHAR_LEN];
    int err = strcpy_s(recvArr, RECV_CHAR_LEN, locationName.c_str());
    if (err != 0) {
        return locationName;
    }
    void *i18nUtilHandle = dlopen("libi18n_util.z.so", RTLD_NOW);
    if (i18nUtilHandle == nullptr) {
        HiLog::Error(LABEL, "Find dynamic library failed, phone number is %{public}s, location name is %{public}s, recvArr is %{public}s.",
            phonenumber.c_str(), locationName.c_str(), recvArr);
        return locationName;
    }
    GetBlockedPhoneLocationName getBlockedPhoneLocationName =
        (GetBlockedPhoneLocationName)dlsym(i18nUtilHandle, "GetBlockedPhoneLocationName");

    getBlockedPhoneLocationName(language.c_str(), phonenumber.c_str(), recvArr);
    if (locationName.compare(recvArr) != 0) {
        HiLog::Error(LABEL, "City name blocked, phone number is %{public}s, location name is %{public}s.",
            phonenumber.c_str(), locationName.c_str());
        return "";
    }

    GetReplacedPhoneLocationName getReplacedPhoneLocationName =
        (GetReplacedPhoneLocationName)dlsym(i18nUtilHandle, "GetReplacedPhoneLocationName");
    getReplacedPhoneLocationName(language.c_str(), phonenumber.c_str(), recvArr);
    HiLog::Error(LABEL, "City name returned, phone number is %{public}s, befor replaced location name is %{public}s, now location name is %{public}s.",
        phonenumber.c_str(), locationName.c_str(), recvArr);
    std::string ret(recvArr);
    return ret;
}
} // namespace I18n
} // namespace Global
} // namespace OHOS
