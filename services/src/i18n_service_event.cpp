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

#include <common_event_data.h>
#include <common_event_manager.h>
#include <common_event_support.h>
#include "common_event_subscriber.h"
#include "i18n_hilog.h"
#include "multi_users.h"

#include "i18n_service_event.h"

namespace OHOS {
namespace Global {
namespace I18n {
const int I18nServiceEvent::RETRY = 3;

I18nServiceEvent::I18nServiceEvent()
{
    userSwitchKey = "usual.event.USER_SWITCHED";
    userAdded = "usual.event.USER_ADDED";
    userRemoved = "usual.event.USER_REMOVED";
    handleEventFunc.emplace(userSwitchKey, &I18nServiceEvent::HandleSwitchUser);
    handleEventFunc.emplace(userAdded, &I18nServiceEvent::HandleAddUser);
    handleEventFunc.emplace(userRemoved, &I18nServiceEvent::HandleRemoveUser);
    for (auto& it : I18nServiceEvent::handleEventFunc) {
        eventHandles.emplace(it.first, std::bind(it.second, this, std::placeholders::_1));
    }
}

I18nServiceEvent::~I18nServiceEvent()
{
    eventHandles.clear();
    handleEventFunc.clear();
    UnSubscriberEvent();
}

void I18nServiceEvent::SubscriberEvent()
{
    if (subscriber) {
        return;
    }
    EventFwk::MatchingSkills matchingSkills;
    for (auto& event : handleEventFunc) {
        matchingSkills.AddEvent(event.first);
    }
    EventFwk::CommonEventSubscribeInfo subscribeInfo(matchingSkills);
    subscriber = std::make_shared<I18nServiceSubscriber>(subscribeInfo, *this);

    int32_t retry = 3;
    do {
        bool status = EventFwk::CommonEventManager::SubscribeCommonEvent(subscriber);
        if (status) {
            HILOG_INFO_I18N("SubscriberEvent: SubscriberEvent success.");
            return;
        }
        HILOG_INFO_I18N("SubscriberEvent: SubscriberEvent failed %{public}d.", retry);
        retry--;
        sleep(1);
    } while (retry);
    HILOG_INFO_I18N("SubscriberEvent: SubscriberEvent failed");
}

void I18nServiceEvent::UnSubscriberEvent()
{
    if (subscriber) {
        bool status = EventFwk::CommonEventManager::UnSubscribeCommonEvent(subscriber);
        if (status != 0) {
            HILOG_INFO_I18N("UnSubscriberEvent: UnSubscriberEvent failed");
        }
        subscriber = nullptr;
    }
}

void I18nServiceEvent::CheckStartReason(const SystemAbilityOnDemandReason& startReason)
{
    std::string action = startReason.GetName();
    HILOG_INFO_I18N("CheckStartReason: start reason is %{public}s", action.c_str());
    if (action == userSwitchKey) {
        std::string localId = std::to_string(startReason.GetExtraData().GetCode());
        MultiUsers::SwitchUser(localId);
    } else if (action == userAdded) {
        std::string localId = std::to_string(startReason.GetExtraData().GetCode());
        MultiUsers::AddUser(localId);
    } else if (action == userRemoved) {
        std::string localId = std::to_string(startReason.GetExtraData().GetCode());
        MultiUsers::RemoveUser(localId);
    }
}

void I18nServiceEvent::OnReceiveEvent(const EventFwk::CommonEventData& data)
{
    AAFwk::Want want = data.GetWant();
    std::string action = want.GetAction();
    auto it = eventHandles.find(action);
    if (it == eventHandles.end()) {
        HILOG_INFO_I18N("OnReceiveEvent: ignore event %{public}s", action.c_str());
        return;
    }
    HILOG_INFO_I18N("OnReceiveEvent: handle event %{public}s", action.c_str());
    it->second(data);
}

void I18nServiceEvent::HandleSwitchUser(const EventFwk::CommonEventData& data) const
{
    AAFwk::Want want = data.GetWant();
    std::string localId = std::to_string(data.GetCode());
    MultiUsers::SwitchUser(localId);
}

void I18nServiceEvent::HandleAddUser(const EventFwk::CommonEventData& data) const
{
    std::string localId = std::to_string(data.GetCode());
    MultiUsers::AddUser(localId);
}

void I18nServiceEvent::HandleRemoveUser(const EventFwk::CommonEventData& data) const
{
    std::string localId = std::to_string(data.GetCode());
    MultiUsers::RemoveUser(localId);
}
} // namespace I18n
} // namespace Global
} // namespace OHOS