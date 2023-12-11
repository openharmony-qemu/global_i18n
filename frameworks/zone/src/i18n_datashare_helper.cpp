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
#include "hilog/log.h"
#include "i18n_datashare_helper.h"

namespace OHOS {
namespace Global {
namespace I18n {
static constexpr OHOS::HiviewDFX::HiLogLabel LABEL = { LOG_CORE, 0xD001E00, "I18nDataShareHelper" };
using namespace OHOS::HiviewDFX;

const std::string SETTINGS_DATASHARE_URI =
    "datashare:///com.ohos.settingsdata/entry/settingsdata/SETTINGSDATA?Proxy=true";
const std::string SETTINGS_DATA_EXT_URI = "datashare:///com.ohos.settingsdata.DataAbility";
const std::string SETTINGS_DATA_COLUMN_KEYWORD = "KEYWORD";
const std::string SETTINGS_DATA_COLUMN_VALUE = "VALUE";

bool QueryWithProxy(const std::string& key, std::string& value, sptr<IRemoteObject> &proxy)
{
    std::shared_ptr<DataShare::DataShareHelper> dataShareHelper = DataShare::DataShareHelper::Creator(proxy, SETTINGS_DATASHARE_URI, SETTINGS_DATA_EXT_URI);
    std::vector<std::string> columns = {SETTINGS_DATA_COLUMN_VALUE};
    DataShare::DataSharePredicates predicates;
    predicates.EqualTo(SETTINGS_DATA_COLUMN_KEYWORD, key);
    Uri uri(AssembleUri(key));
    if (dataShareHelper == nullptr) {
        HiLog::Error(LABEL, "QueryWithProxy Query failed, dataShareHelper is nullptr.");
        return false;
    }
    auto result = dataShareHelper->Query(uri, predicates, columns);
    int32_t count;
    if (result == nullptr) {
        HiLog::Error(LABEL, "QueryWithProxy Query failed, result is nullptr.");
        return false;
    }
    result->GetRowCount(count);
    if (count == 0) {
        HiLog::Error(LABEL, "QueryWithProxy Query failed, count is zero.");
        return false;
    }
    const int32_t INDEX = 0;
    result->GoToRow(INDEX);
    int32_t ret = result->GetString(INDEX, value);
    HiLog::Info(LABEL, "QueryWithProxy Query GetString %{public}s.", value.c_str());
    if (ret != NativeRdb::E_OK) {
        HiLog::Error(LABEL, "QueryWithProxy Query failed, GetString Failed.");
        return false;
    }
    result->Close();
    dataShareHelper->Release();
    return true;
}

bool Query(const std::string& key, std::string& value)
{
    std::shared_ptr<DataShare::DataShareHelper> dataShareHelper = CreateDataShareHelper();
    std::vector<std::string> columns = {SETTINGS_DATA_COLUMN_VALUE};
    DataShare::DataSharePredicates predicates;
    predicates.EqualTo(SETTINGS_DATA_COLUMN_KEYWORD, key);
    Uri uri(AssembleUri(key));
    if (dataShareHelper == nullptr) {
        HiLog::Error(LABEL, "Query failed, dataShareHelper is nullptr.");
        return false;
    }
    auto result = dataShareHelper->Query(uri, predicates, columns);
    int32_t count;
    if (result == nullptr) {
        HiLog::Error(LABEL, "Query failed, result is nullptr.");
        return false;
    }
    result->GetRowCount(count);
    if (count == 0) {
        HiLog::Error(LABEL, "Query failed, count is zero.");
        return false;
    }
    const int32_t INDEX = 0;
    result->GoToRow(INDEX);
    int32_t ret = result->GetString(INDEX, value);
    if (ret != NativeRdb::E_OK) {
        HiLog::Error(LABEL, "Query failed, GetString Failed.");
        return false;
    }
    result->Close();
    dataShareHelper->Release();
    return true;
}

Uri AssembleUri(const std::string& key)
{
    Uri uri(SETTINGS_DATASHARE_URI + "&key=" + key);
    return uri;
}

std::shared_ptr<DataShare::DataShareHelper> CreateDataShareHelper()
{
    sptr<IRemoteObject> proxy = I18nServiceAbilityLoadManager::GetInstance()->GetI18nServiceAbility(I18N_SA_ID);
    return DataShare::DataShareHelper::Creator(proxy, SETTINGS_DATASHARE_URI, SETTINGS_DATA_EXT_URI);
}
} // namespace I18n
} // namespace Global
} // namespace OHOS