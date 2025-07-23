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
#include <sys/stat.h>
#include "i18n_types.h"
#include "relative_time_format.h"
#include "format_utils.h"
#include "locale_helper.h"
#include "relative_time_format_test.h"

using namespace OHOS::Global::I18n;
using testing::ext::TestSize;
using namespace std;
using namespace testing;

namespace OHOS {
namespace Global {
namespace I18n {

void RelativeTimeFormatTest::SetUpTestCase(void)
{
}

void RelativeTimeFormatTest::TearDownTestCase(void)
{
}

void RelativeTimeFormatTest::SetUp(void)
{}

void RelativeTimeFormatTest::TearDown(void)
{}

/**
 * @tc.name: RelativeTimeFormatTest0001
 * @tc.desc: Test GetTimezoneIdByLocation
 * @tc.type: FUNC
 */
HWTEST_F(RelativeTimeFormatTest, RelativeTimeFormatTest0001, TestSize.Level1)
{
    ErrorMessage errorMsg;
    const std::vector<std::string> localeTags = {"en"};
    std::vector<std::string> result = RelativeTimeFormat::CanonicalizeLocales(localeTags, errorMsg);
    EXPECT_EQ(localeTags.size(), 1);
    const std::string napiType = "number";
    const int32_t fieldId = UNUM_FRACTION_FIELD;
    std::string type = FormatUtils::GetNumberFieldType(napiType, fieldId, 12.45);
    EXPECT_EQ(type, "fraction");
    const std::vector<std::string> localeArray = {"@1"};
    std::string res = LocaleHelper::CheckParamLocales(localeArray);
    EXPECT_EQ(res, "invalid locale");
    const std::vector<std::string> localeArr = {};
    std::string res2 = LocaleHelper::CheckParamLocales(localeArr);
    EXPECT_EQ(res2, "");
    const std::vector<std::string> localeArr3 = {"default"};
    std::string res3 = LocaleHelper::CheckParamLocales(localeArr3);
    EXPECT_EQ(res3, "");
}
} // namespace I18n
} // namespace Global
} // namespace OHOS