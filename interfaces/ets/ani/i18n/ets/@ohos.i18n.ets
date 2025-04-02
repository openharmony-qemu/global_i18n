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

namespace i18n {
    loadLibrary("i18n_ani.z");

    export class System {
        static { loadLibrary("i18n_ani.z"); }

        native static getSystemLanguage(): string;
        native static getSystemRegion(): string;
        native static getSystemLocale(): string;
        native static is24HourClock(): boolean;
    }

    export class Calendar {
        static { loadLibrary("i18n_ani.z"); }

        private nativeCalendar: long = 0;

        constructor(context: long) {
            if (this.nativeCalendar == 0) {
                this.nativeCalendar = context;
            }
        }

        native getTimeZone(): string;
        native get(field: string): number;
    }

    export class Unicode {
        static { loadLibrary("i18n_ani.z"); }

        native static isRTL(ch: string): boolean;
    }

    export class TimeZone {
        static { loadLibrary("i18n_ani.z"); }

        private nativeTimeZone: long = 0;

        constructor(context: long) {
            if (this.nativeTimeZone == 0) {
                this.nativeTimeZone = context;
            }
        }
    }
      
    export native function getCalendar(locale: string, type?: string): Calendar;
    export native function isRTL(locale: string): boolean;
    export native function getTimeZone(zoneID?: string): TimeZone;
}
