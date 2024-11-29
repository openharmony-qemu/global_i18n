/*
 * Copyright (c) 2023-2024 Huawei Device Co., Ltd.
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

#include "i18n_hilog.h"
#include "i18n_service_ability_load_manager.h"
#include "ipc_skeleton.h"
#include "locale_config.h"
#include "mem_mgr_client.h"
#include "mem_mgr_proxy.h"
#include "multi_users.h"
#include "os_account_manager.h"
#include "preferred_language.h"
#include "preferences.h"
#include "preferences_helper.h"
#include "system_ability_definition.h"
#include "i18n_service_ability.h"

namespace OHOS {
namespace Global {
namespace I18n {
REGISTER_SYSTEM_ABILITY_BY_ID(I18nServiceAbility, I18N_SA_ID, false);
static const std::string UNLOAD_TASK = "i18n_service_unload";
static const uint32_t DELAY_MILLISECONDS_FOR_UNLOAD_SA = 10000;
static const int32_t UID_TRANSFORM_DIVISOR = 200000;
static const int32_t USER_DEFAULT = 100;

I18nServiceAbility::I18nServiceAbility(int32_t saId, bool runOnCreate) : SystemAbility(saId, runOnCreate)
{
    HILOG_INFO_I18N("I18nServiceAbility object init success.");
}

I18nServiceAbility::~I18nServiceAbility()
{
    HILOG_INFO_I18N("I18nServiceAbility object release.");
}

I18nErrorCode I18nServiceAbility::SetSystemLanguage(const std::string &language)
{
    int32_t userId = GetCallingUserId();
    return LocaleConfig::SetSystemLanguage(language, userId);
}

I18nErrorCode I18nServiceAbility::SetSystemRegion(const std::string &region)
{
    int32_t userId = GetCallingUserId();
    return LocaleConfig::SetSystemRegion(region, userId);
}

I18nErrorCode I18nServiceAbility::SetSystemLocale(const std::string &locale)
{
    int32_t userId = GetCallingUserId();
    return LocaleConfig::SetSystemLocale(locale, userId);
}

I18nErrorCode I18nServiceAbility::Set24HourClock(const std::string &flag)
{
    int32_t userId = GetCallingUserId();
    return LocaleConfig::Set24HourClock(flag, userId);
}

I18nErrorCode I18nServiceAbility::SetUsingLocalDigit(bool flag)
{
    int32_t userId = GetCallingUserId();
    return LocaleConfig::SetUsingLocalDigit(flag, userId);
}

I18nErrorCode I18nServiceAbility::AddPreferredLanguage(const std::string &language, int32_t index)
{
    return PreferredLanguage::AddPreferredLanguage(language, index);
}

I18nErrorCode I18nServiceAbility::RemovePreferredLanguage(int32_t index)
{
    return PreferredLanguage::RemovePreferredLanguage(index);
}

I18nErrorCode I18nServiceAbility::SetTemperatureType(TemperatureType type)
{
    int32_t userId = GetCallingUserId();
    return LocaleConfig::SetTemperatureType(type, userId);
}

I18nErrorCode I18nServiceAbility::SetFirstDayOfWeek(WeekDay type)
{
    int32_t userId = GetCallingUserId();
    return LocaleConfig::SetFirstDayOfWeek(type, userId);
}

void I18nServiceAbility::UnloadI18nServiceAbility()
{
    if (handler != nullptr) {
        handler->RemoveTask(UNLOAD_TASK);
    }
    auto task = [this]() {
        auto i18nSaLoadManager = DelayedSingleton<I18nServiceAbilityLoadManager>::GetInstance();
        if (i18nSaLoadManager != nullptr) {
            HILOG_INFO_I18N("I18nServiceAbility::UnloadI18nServiceAbility start to unload i18n sa.");
            i18nSaLoadManager->UnloadI18nService(I18N_SA_ID);
        }
    };
    if (handler != nullptr) {
        handler->PostTask(task, UNLOAD_TASK, DELAY_MILLISECONDS_FOR_UNLOAD_SA);
    }
}

void I18nServiceAbility::OnStart(const SystemAbilityOnDemandReason& startReason)
{
    HILOG_INFO_I18N("I18nServiceAbility start.");
    i18nServiceEvent = new I18nServiceEvent();
    if (i18nServiceEvent != nullptr) {
        i18nServiceEvent->SubscriberEvent();
        i18nServiceEvent->CheckStartReason(startReason);
    }
#ifdef SUPPORT_MULTI_USER
    MultiUsers::InitMultiUser();
#endif
    bool status = Publish(this);
    if (status) {
        HILOG_INFO_I18N("I18nServiceAbility Publish success.");
    } else {
        HILOG_INFO_I18N("I18nServiceAbility Publish failed.");
    }
    handler = std::make_shared<AppExecFwk::EventHandler>(AppExecFwk::EventRunner::Create(true));
    UnloadI18nServiceAbility();
    int pid = getpid();
    Memory::MemMgrClient::GetInstance().NotifyProcessStatus(pid, 1, 1, I18N_SA_ID);
}

void I18nServiceAbility::OnStop()
{
    int pid = getpid();
    Memory::MemMgrClient::GetInstance().NotifyProcessStatus(pid, 1, 0, I18N_SA_ID);
    if (i18nServiceEvent != nullptr) {
        delete i18nServiceEvent;
        i18nServiceEvent = nullptr;
    }
    HILOG_INFO_I18N("I18nServiceAbility Stop.");
}

int32_t I18nServiceAbility::GetCallingUserId()
{
    int32_t userId = OHOS::IPCSkeleton::GetCallingUid() / UID_TRANSFORM_DIVISOR;
    if (userId == 0) {
        auto err = OHOS::AccountSA::OsAccountManager::GetForegroundOsAccountLocalId(userId);
        if (err != 0) {
            HILOG_ERROR_I18N("I18nServiceAbility::GetCallingUserId: GetForegroundOsAccountLocalId failed.");
            userId = USER_DEFAULT;
        }
    }
    return userId;
}
} // namespace I18n
} // namespace Global
} // namespace OHOS