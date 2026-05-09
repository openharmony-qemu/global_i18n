/*
 * Copyright (c) 2026 Huawei Device Co., Ltd.
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
#ifndef HISTOGRAM_HELPER_H
#define HISTOGRAM_HELPER_H

#include <chrono>

#ifdef GLOBAL_I18N_METRICS_ENABLE
#include "histogram_plugin_macros.h"
#endif

namespace OHOS {
namespace Global {
namespace I18n {

class ApiMetrics {
public:
    ApiMetrics() : startTime_(std::chrono::steady_clock::now()) {}

    void RecordTime(const char* name)
    {
#ifdef GLOBAL_I18N_METRICS_ENABLE
        auto endTime = std::chrono::steady_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(
            endTime - startTime_).count();
        HISTOGRAM_TIMES(name, static_cast<double>(duration));
#endif
    }

    static void RecordBoolean(const char* name, bool success)
    {
#ifdef GLOBAL_I18N_METRICS_ENABLE
        HISTOGRAM_BOOLEAN(name, success ? 1 : 0);
#endif
    }

private:
    std::chrono::steady_clock::time_point startTime_;
};

} // namespace I18n
} // namespace Global
} // namespace OHOS

#endif
