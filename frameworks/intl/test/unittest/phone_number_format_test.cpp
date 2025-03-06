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

#include <gtest/gtest.h>
#include "phone_number_format_test.h"
#include <sys/stat.h>
#include "taboo_utils.h"

using namespace OHOS::Global::I18n;
using testing::ext::TestSize;
using namespace std;
using namespace testing;

namespace OHOS {
namespace Global {
namespace I18n {

void TranslitTest::SetUpTestCase(void)
{
    SetHwIcuDirectory();
}

void TranslitTest::TearDownTestCase(void)
{
}

void TranslitTest::SetUp(void)
{}

void TranslitTest::TearDown(void)
{}

/**
 * @tc.name: PhoneNumberFormatTest0001
 * @tc.desc: Test icu::Transliterator
 * @tc.type: FUNC
 */
HWTEST_F(TranslitTest, PhoneNumberFormatTest0001, TestSize.Level1)
{
    map<string, string> options;
    std::unique_ptr<PhoneNumberFormat> phoneNumberFormat =
        std::make_unique<PhoneNumberFormat>("CN", options);
    EXPECT_EQ(phoneNumberFormat->getLocationName("+551738491234", "pt"), "São Paulo");
    EXPECT_EQ(phoneNumberFormat->getLocationName("+558435211234", "en"), "Macao - RN");
    EXPECT_EQ(phoneNumberFormat->getLocationName("+551738491234", "en"), "São Paulo");

    EXPECT_EQ(phoneNumberFormat->getLocationName("+903921234567", "tr"), "Türkiye");
    EXPECT_EQ(phoneNumberFormat->getLocationName("+903921234567", "en"), "Türkiye");

    EXPECT_EQ(phoneNumberFormat->getLocationName("+3596139312", "en"), "Bulgaria");
    EXPECT_EQ(phoneNumberFormat->getLocationName("+3599355123", "en"), "Bulgaria");
    EXPECT_EQ(phoneNumberFormat->getLocationName("+3598128123", "en"), "Bulgaria");
    EXPECT_EQ(phoneNumberFormat->getLocationName("+3596139312", "bg"), "България");
    EXPECT_EQ(phoneNumberFormat->getLocationName("+3599355123", "bg"), "България");
    EXPECT_EQ(phoneNumberFormat->getLocationName("+3598128123", "bg"), "България");
    struct stat s;
    if (stat(SYSTEM_TABOO_DATA_PATH.c_str(), &s) == 0) {
        EXPECT_EQ(phoneNumberFormat->getLocationName("+212528912345", "en"), "");
        EXPECT_EQ(phoneNumberFormat->getLocationName("+212528912345", "fr"), "");
        EXPECT_EQ(phoneNumberFormat->getLocationName("+38338123456", "en"), "");
        EXPECT_EQ(phoneNumberFormat->getLocationName("+38338123456", "sr"), "");
    }

    EXPECT_EQ(phoneNumberFormat->getLocationName("+913602123456", "en"), "India");
    EXPECT_EQ(phoneNumberFormat->getLocationName("+913800123456", "en"), "India");

    EXPECT_EQ(phoneNumberFormat->getLocationName("+38138212345", "sr"), "Суринам");

    EXPECT_EQ(phoneNumberFormat->getLocationName("+38138212345", "en"), "Vatican City");
    EXPECT_EQ(phoneNumberFormat->getLocationName("+38138212345", "en"), "Milan");
    EXPECT_EQ(phoneNumberFormat->getLocationName("+38138212345", "en"), "Italy");        

}
} // namespace I18n
} // namespace Global
} // namespace OHOS