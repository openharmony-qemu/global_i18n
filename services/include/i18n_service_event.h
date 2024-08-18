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

#ifndef OHOS_GLOBAL_I18N_I18N_SERVICE_EVENT_H
#define OHOS_GLOBAL_I18N_I18N_SERVICE_EVENT_H

#include <functional>
#include <memory>
#include <singleton.h>
#include <string>
#include <unordered_map>
#include "common_event_subscriber.h"
#include "system_ability.h"
#include "want.h"

namespace OHOS {
namespace Global {
namespace I18n {
class I18nServiceEvent {
public:
    I18nServiceEvent();
    ~I18nServiceEvent();

    void CheckStartReason(const SystemAbilityOnDemandReason& startReason);
    void SubscriberEvent();

private:
    class I18nServiceSubscriber : public EventFwk::CommonEventSubscriber {
    public:
        explicit I18nServiceSubscriber(const EventFwk::CommonEventSubscribeInfo& subscribeInfo,
            I18nServiceEvent& registry) : CommonEventSubscriber(subscribeInfo), registry_(registry)
        {}

        ~I18nServiceSubscriber() = default;

        void OnReceiveEvent(const EventFwk::CommonEventData& data) override
        {
            registry_.OnReceiveEvent(data);
        }

    private:
        I18nServiceEvent& registry_;
    };

    void UnSubscriberEvent();
    void OnReceiveEvent(const EventFwk::CommonEventData& data);
    
    void HandleSwitchUser(const EventFwk::CommonEventData& data) const;
    void HandleAddUser(const EventFwk::CommonEventData& data) const;
    void HandleRemoveUser(const EventFwk::CommonEventData& data) const;

    using EventHandle = std::function<void(const EventFwk::CommonEventData&)>;
    typedef void (I18nServiceEvent::*HandleEventFunc)(const EventFwk::CommonEventData&) const;
    std::unordered_map<std::string, EventHandle> eventHandles;
    std::unordered_map<std::string, HandleEventFunc> handleEventFunc;
    std::shared_ptr<I18nServiceSubscriber> subscriber = nullptr;
    std::string userSwitchKey;
    std::string userAdded;
    std::string userRemoved;
    static const int RETRY;
};
} // namespace I18n
} // namespace Global
} // namespace OHOS
#endif // OHOS_GLOBAL_I18N_I18N_SERVICE_EVENT_H