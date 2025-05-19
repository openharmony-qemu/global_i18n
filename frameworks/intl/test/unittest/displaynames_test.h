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

#ifndef OHOS_GLOBAL_I18N_DISPLAYNAMES_TEST
#define OHOS_GLOBAL_I18N_DISPLAYNAMES_TEST

#include <gtest/gtest.h>
#include <string>

namespace OHOS {
namespace Global {
namespace I18n {
class DispalynamesTest : public testing::Test {
public:
    static void SetUpTestCase(void);
    static void TearDownTestCase(void);
    void SetUp();
    void TearDown();
};

int DispalynamesTest0001(void);
int DispalynamesTest0002(void);
int DispalynamesTest0003(void);
int DispalynamesTest0004(void);
int DispalynamesTest0005(void);
int DispalynamesTest0006(void);
int DispalynamesTest0007(void);
int DispalynamesTest0008(void);
int DispalynamesTest0009(void);
} // namespace I18n
} // namespace Global
} // namespace OHOS
#endif