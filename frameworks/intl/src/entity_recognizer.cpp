/*
 * Copyright (c) 2023 Huawei Device Co., Ltd.
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
#include "entity_recognizer.h"

namespace OHOS {
namespace Global {
namespace I18n {
EntityRecognizer::EntityRecognizer(icu::Locale& locale)
{
    std::string region = locale.getCountry();
    phoneNumberMatched = new PhoneNumberMatched(region);
}

EntityRecognizer::~EntityRecognizer()
{
    if (phoneNumberMatched != nullptr) {
        delete phoneNumberMatched;
    }
    phoneNumberMatched = nullptr;
}

std::vector<std::vector<int>> EntityRecognizer::FindEntityInfo(std::string& message)
{
    std::vector<std::vector<int>> EntityInfo;
    std::vector<int> phoneNumberInfo = phoneNumberMatched->GetMatchedPhoneNumber(message);
    EntityInfo.push_back(phoneNumberInfo);
    std::vector<int> dataInfo{0};
    EntityInfo.push_back(dataInfo);
    return EntityInfo;
}
} // namespace I18n
} // namespace Global
} // namespace OHOS