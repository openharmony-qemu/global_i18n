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
#ifndef GLOBAL_I18N_DATASHARE_HELPER_H
#define GLOBAL_I18N_DATASHARE_HELPER_H

#include <string>
#include "datashare_helper.h"
#include "datashare_predicates.h"
#include "datashare_result_set.h"
#include "i18n_service_ability_load_manager.h"
#include "system_ability_definition.h"
#include "rdb_errno.h"

namespace OHOS {
namespace Global {
namespace I18n {
extern "C" __attribute__((used)) bool QueryWithProxy(const std::string& key, std::string& value, sptr<IRemoteObject> &proxy);
extern "C" __attribute__((used)) bool Query(const std::string& key, std::string& value);
__attribute__((used)) static std::shared_ptr<DataShare::DataShareHelper> CreateDataShareHelper();
__attribute__((used)) static Uri AssembleUri(const std::string& key);
} // namespace I18n
} // namespace Global
} // namespace OHOS
#endif
