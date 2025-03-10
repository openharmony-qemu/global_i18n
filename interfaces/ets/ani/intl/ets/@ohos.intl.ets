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

namespace intl {
    export interface LocaleOptions {
        calendar?: string;
        collation?: string;
        hourCycle?: string;
        numberingSystem?: string;
        numeric?: boolean;
        caseFirst?: string;
    }

    export class LocaleOptionsInner implements LocaleOptions {
        calendar?: string | undefined = "";
        collation?: string | undefined = "";
        hourCycle?: string | undefined = "";
        numberingSystem?: string | undefined = "";
        numeric?: boolean | undefined = false;
        caseFirst?: string | undefined = "";
        locale?: string | undefined = "";
    }

    export interface DateTimeOptions {
        locale?: string;
        dateStyle?: string;
        timeStyle?: string;
        hourCycle?: string;
        timeZone?: string;
        numberingSystem?: string;
        hour12?: boolean;
        weekday?: string;
        era?: string;
        year?: string;
        month?: string;
        day?: string;
        hour?: string;
        minute?: string;
        second?: string;
        timeZoneName?: string;
        dayPeriod?: string;
        localeMatcher?: string;
        formatMatcher?: string;
    }

    export class DateTimeOptionsInner implements DateTimeOptions {
        locale?: string | undefined = "";
        dateStyle?: string | undefined = "";
        timeStyle?: string | undefined = "";
        hourCycle?: string | undefined = "";
        timeZone?: string | undefined = "";
        numberingSystem?: string | undefined = "";
        hour12?: boolean | undefined = false;
        weekday?: string | undefined = "";
        era?: string | undefined = "";
        year?: string | undefined = "";
        month?: string | undefined = "";
        day?: string | undefined = "";
        hour?: string | undefined = "";
        minute?: string | undefined = "";
        second?: string | undefined = "";
        timeZoneName?: string | undefined = "";
        dayPeriod?: string | undefined = "";
        localeMatcher?: string | undefined = "";
        formatMatcher?: string | undefined = "";
    }

    export interface CollatorOptions {
        localeMatcher?: string;
        usage?: string;
        sensitivity?: string;
        ignorePunctuation?: boolean;
        collation?: string;
        numeric?: boolean;
        caseFirst?: string;
    }

    export class CollatorOptionsInner implements CollatorOptions {
        localeMatcher?: string | undefined = "";
        usage?: string | undefined = "";
        sensitivity?: string | undefined = "";
        ignorePunctuation?: boolean | undefined = false;
        collation?: string | undefined = "";
        numeric?: boolean | undefined = false;
        caseFirst?: string | undefined = "";
    }

    export interface PluralRulesOptions {
        localeMatcher?: string;
        type?: string;
        minimumIntegerDigits?: number;
        minimumFractionDigits?: number;
        maximumFractionDigits?: number;
        minimumSignificantDigits?: number;
        maximumSignificantDigits?: number;
    }

    export class PluralRulesOptionsInner implements PluralRulesOptions {
        localeMatcher?: string | undefined = "";
        type?: string | undefined = "";
        minimumIntegerDigits?: number | undefined = 0;
        minimumFractionDigits?: number | undefined = 0;
        maximumFractionDigits?: number | undefined = 0;
        minimumSignificantDigits?: number | undefined = 0;
        maximumSignificantDigits?: number | undefined = 0;
    }

    export interface RelativeTimeFormatResolvedOptions {
        locale: string;
        style: string;
        numeric: string;
        numberingSystem: string;
    }

    export class RelativeTimeFormatResolvedOptionsInner implements RelativeTimeFormatResolvedOptions {
        locale: string = "";
        style: string = "";
        numeric: string = "";
        numberingSystem: string = "";
    }

    export interface RelativeTimeFormatInputOptions {
        localeMatcher?: string;
        numeric?: string;
        style?: string;
    }

    export class RelativeTimeFormatInputOptionsInner implements RelativeTimeFormatInputOptions {
        localeMatcher?: string | undefined = "";
        numeric?: string | undefined = "";
        style?: string | undefined = "";
    }

    export interface NumberOptions {
        locale?: string
        currency?: string
        currencySign?: string
        currencyDisplay?: string
        unit?: string
        unitDisplay?: string
        unitUsage?: string
        signDisplay?: string
        compactDisplay?: string
        notation?: string
        localeMatcher?: string
        style?: string
        numberingSystem?: string
        useGrouping?: boolean;
        minimumIntegerDigits?: number;
        minimumFractionDigits?: number;
        maximumFractionDigits?: number;
        minimumSignificantDigits?: number;
        maximumSignificantDigits?: number;
        roundingPriority?: string
        roundingIncrement?: number;
        roundingMode?: string
    }

    export class NumberOptionsInner implements NumberOptions {
        locale?: string | undefined = "";
        currency?: string | undefined = "";
        currencySign?: string | undefined = "";
        currencyDisplay?: string | undefined = "";
        unit?: string | undefined = "";
        unitDisplay?: string | undefined = "";
        unitUsage?: string | undefined = "";
        signDisplay?: string | undefined = "";
        compactDisplay?: string | undefined = "";
        notation?: string | undefined = "";
        localeMatcher?: string | undefined = "";
        style?: string | undefined = "";
        numberingSystem?: string | undefined = "";
        useGrouping?: boolean | undefined = false;
        minimumIntegerDigits?: number | undefined = 0;
        minimumFractionDigits?: number | undefined = 0;
        maximumFractionDigits?: number | undefined = 0;
        minimumSignificantDigits?: number | undefined = 0;
        maximumSignificantDigits?: number | undefined = 0;
        roundingPriority?: string | undefined = "";
        roundingIncrement?: number | undefined = 0;
        roundingMode?: string | undefined = "";
    }

    export class Locale {
        static { loadLibrary("intl_ani.z"); }

        private nativeLocale: long = 0;
        private language: string;
        private script: string;
        private region: string;
        private baseName: string;
        private caseFirst: string;
        private calendar: string;
        private collation: string;
        private hourCycle: string;
        private numberingSystem: string;
        private numeric: boolean;

        constructor(context: long) {
            if (this.nativeLocale == 0) {
                this.nativeLocale = context;
            }
        }

        constructor() {
            this.nativeLocale = Locale.create().nativeLocale;
        }

        constructor(locale: string, options?: LocaleOptions) {
            this.nativeLocale = Locale.create(locale, options).nativeLocale;
        }

        native static create(): Locale;
        native static create(locale: string, options?: LocaleOptions): Locale;
    }

    export class DateTimeFormat {
        static { loadLibrary("intl_ani.z"); }

        private nativeDateTimeFormat: long = 0;

        constructor(context: long) {
            if (this.nativeDateTimeFormat == 0) {
                this.nativeDateTimeFormat = context;
            }
        }

        constructor() {
            this.nativeDateTimeFormat = DateTimeFormat.create().nativeDateTimeFormat;
        }

        constructor(locale: string, options?: DateTimeOptions) {
            this.nativeDateTimeFormat = DateTimeFormat.create(locale, options).nativeDateTimeFormat;
        }

        native static create(): DateTimeFormat;
        native static create(locale: string | Array<string>, options?: DateTimeOptions): DateTimeFormat;
        native format(date: Date): string;
        native resolvedOptions(): DateTimeOptions;
    }

    export class NumberFormat {
        static { loadLibrary("intl_ani.z"); }

        private nativeNumberFormat: long = 0;

        constructor(context: long) {
            if (this.nativeNumberFormat == 0) {
                this.nativeNumberFormat = context;
            }
        }

        constructor() {
            this.nativeNumberFormat = NumberFormat.create().nativeNumberFormat;
        }

        constructor(locale: string, options?: NumberOptions) {
            this.nativeNumberFormat = NumberFormat.create(locale, options).nativeNumberFormat;
        }

        native static create(): NumberFormat;
        native static create(locale: string | Array<string>, options?: NumberOptions): NumberFormat;
        native format(number: number): string;
        native resolvedOptions(): NumberOptions;
    }
}
