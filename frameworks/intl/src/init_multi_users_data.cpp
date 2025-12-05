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
#include <sys/dir.h>
#include <sys/stat.h>
#include <string>
#include <unistd.h>
#include "i18n_hilog.h"
#include "multi_users.h"

namespace OHOS {
namespace Global {
namespace I18n {
const std::string DIR_PATH = "/data/service/el1/public/i18n/global";

void ModifyDirPermission()
{
    mode_t mode = 0777;
    DIR* dir = opendir(DIR_PATH.c_str());
    if (dir == nullptr) {
        HILOG_ERROR_I18N("ModifyDirPermission: dir is nullptr.");
        return;
    }
    struct dirent* entry;
    while ((entry = readdir(dir)) != nullptr) {
        std::string fileName = entry->d_name;
        if (fileName == "." || fileName == "..") {
            continue;
        }

        std::string fullPath = DIR_PATH + "/" + fileName;
        struct stat st;
        if (stat(fullPath.c_str(), &st) == -1) {
            HILOG_ERROR_I18N("ModifyDirPermission: stat %{public}s failed.", fullPath.c_str());
            continue;
        }
        if (chmod(fullPath.c_str(), mode) == -1) {
            HILOG_ERROR_I18N("ModifyDirPermission: chmod %{public}s failed.", fullPath.c_str());
            continue;
        }
    }
    closedir(dir);
}

void Init()
{
    if (!MultiUsers::InitMultiUserData()) {
        return;
    }
    ModifyDirPermission();
}
} // namespace I18n
} // namespace Global
} // namespace OHOS

int main(int argc, char* argv[])
{
#ifdef SUPPORT_MULTI_USER
    HILOG_INFO_I18N("Start init multi users data.");
    OHOS::Global::I18n::Init();
    _exit(0);
#endif
    return 0;
}