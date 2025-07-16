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
        calendar?: string;
        collation?: string;
        hourCycle?: string;
        numberingSystem?: string;
        numeric?: boolean;
        caseFirst?: string;
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
        locale?: string;
        currency?: string;
        currencySign?: string;
        currencyDisplay?: string;
        unit?: string;
        unitDisplay?: string;
        unitUsage?: string;
        signDisplay?: string;
        compactDisplay?: string;
        notation?: string;
        localeMatcher?: string;
        style?: string;
        numberingSystem?: string;
        useGrouping?: boolean;
        minimumIntegerDigits?: number;
        minimumFractionDigits?: number;
        maximumFractionDigits?: number;
        minimumSignificantDigits?: number;
        maximumSignificantDigits?: number;
        roundingPriority?: string;
        roundingIncrement?: number;
        roundingMode?: string;
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
        localeMatcher?: string;
        usage?: string;
        sensitivity?: string;
        ignorePunctuation?: boolean;
        collation?: string;
        numeric?: boolean;
        caseFirst?: string;
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
        localeMatcher?: string;
        type?: string;
        minimumIntegerDigits?: number;
        minimumFractionDigits?: number;
        maximumFractionDigits?: number;
        minimumSignificantDigits?: number;
        maximumSignificantDigits?: number;
    }

    export interface RelativeTimeFormatInputOptions {
        localeMatcher?: string;
        numeric?: string;
        style?: string;
    }

    export class RelativeTimeFormatInputOptionsInner implements RelativeTimeFormatInputOptions {
        localeMatcher?: string;
        numeric?: string;
        style?: string;
    }

    export interface RelativeTimeFormatResolvedOptions {
        locale: string;
        style: string;
        numeric: string;
        numberingSystem: string;
    }

    export class RelativeTimeFormatResolvedOptionsInner implements RelativeTimeFormatResolvedOptions {
        locale: string;
        style: string;
        numeric: string;
        numberingSystem: string;
    }

    class RelativeTimeFormatPart {
        type: string;
        value: string;
        unit?: string;
    }

    export class Locale {
        static { loadLibrary("intl_ani.z"); }

        _language: string;
        _script: string;
        _region: string;
        _baseName: string;
        _caseFirst: string;
        _calendar: string;
        _collation: string;
        _hourCycle: string;
        _numberingSystem: string;
        _numeric: boolean;
        private nativeLocale: long;

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

        get language(): string {
            return this.getLanguage();
        }

        set language(value: string) { }

        get script(): string {
            return this.getScript();
        }

        set script(value: string) { }

        get region(): string {
            return this.getRegion();
        }

        set region(value: string) { }

        get baseName(): string {
            return this.getBaseName();
        }

        set baseName(value: string) { }

        get caseFirst(): string {
            return this.getCaseFirst();
        }

        set caseFirst(value: string) { }

        get calendar(): string {
            return this.getCalendar();
        }

        set calendar(value: string) { }

        get collation(): string {
            return this.getCollation();
        }
        set collation(value: string) { }

        get hourCycle(): string {
            return this.getHourCycle();
        }

        set hourCycle(value: string) { }

        get numberingSystem(): string {
            return this.getNumberingSystem();
        }

        set numberingSystem(value: string) { }

        get numeric(): boolean {
            return this.getNumeric();
        }

        set numeric(value: boolean) { }

        native static create(): Locale;
        native static create(locale: string, options?: LocaleOptions): Locale;
        native toString(): string;
        native maximize(): Locale;
        native minimize(): Locale;
        native getLanguage(): string;
        native getScript(): string;
        native getRegion(): string;
        native getBaseName(): string;
        native getCaseFirst(): string;
        native getCalendar(): string;
        native getCollation(): string;
        native getHourCycle(): string;
        native getNumberingSystem(): string;
        native getNumeric(): boolean;
    }

    export class DateTimeFormat {
        static { loadLibrary("intl_ani.z"); }

        private nativeDateTimeFormat: long;

        constructor(context: long) {
            if (this.nativeDateTimeFormat == 0) {
                this.nativeDateTimeFormat = context;
            }
        }

        constructor() {
            this.nativeDateTimeFormat = DateTimeFormat.create().nativeDateTimeFormat;
        }

        constructor(locale: string | Array<string>, options?: DateTimeOptions) {
            this.nativeDateTimeFormat = DateTimeFormat.create(locale, options).nativeDateTimeFormat;
        }

        native static create(): DateTimeFormat;
        native static create(locale: string | Array<string>, options?: DateTimeOptions): DateTimeFormat;
        native format(date: Date): string;
        native formatRange(startDate: Date, endDate: Date): string;
        native resolvedOptions(): DateTimeOptions;
    }

    export class NumberFormat {
        static { loadLibrary("intl_ani.z"); }

        private nativeNumberFormat: long;

        constructor(context: long) {
            if (this.nativeNumberFormat == 0) {
                this.nativeNumberFormat = context;
            }
        }

        constructor() {
            this.nativeNumberFormat = NumberFormat.create().nativeNumberFormat;
        }

        constructor(locale: string | Array<string>, options?: NumberOptions) {
            this.nativeNumberFormat = NumberFormat.create(locale, options).nativeNumberFormat;
        }

        native static create(): NumberFormat;
        native static create(locale: string | Array<string>, options?: NumberOptions): NumberFormat;
        native format(num: number): string;
        native formatRange(startRange: number, endRange: number): string;
        native resolvedOptions(): NumberOptions;
    }

    export class Collator {
        static { loadLibrary("intl_ani.z"); }

        private nativeCollator: long;

        constructor(context: long) {
            if (this.nativeCollator == 0) {
                this.nativeCollator = context;
            }
        }

        constructor() {
            this.nativeCollator = Collator.create().nativeCollator;
        }

        constructor(locale: string | Array<string>, options?: CollatorOptions) {
            this.nativeCollator = Collator.create(locale, options).nativeCollator;
        }

        native static create(): Collator;
        native static create(locale: string | Array<string>, options?: CollatorOptions): Collator;
        native compare(first: string, second: string): number;
        native resolvedOptions(): CollatorOptions;
    }

    export class PluralRules {
        static { loadLibrary("intl_ani.z"); }

        private nativePluralRules: long;

        constructor(context: long) {
            if (this.nativePluralRules == 0) {
                this.nativePluralRules = context;
            }
        }

        constructor() {
            this.nativePluralRules = PluralRules.create().nativePluralRules;
        }

        constructor(locale: string | Array<string>, options?: PluralRulesOptions) {
            this.nativePluralRules = PluralRules.create(locale, options).nativePluralRules;
        }

        native static create(): PluralRules;
        native static create(locale: string | Array<string>, options?: PluralRulesOptions): PluralRules;
        native select(n: number): string;
    }

    export class RelativeTimeFormat {
        static { loadLibrary("intl_ani.z"); }

        private nativeRelativeTimeFormat: long;

        constructor(context: long) {
            if (this.nativeRelativeTimeFormat == 0) {
                this.nativeRelativeTimeFormat = context;
            }
        }

        constructor() {
            this.nativeRelativeTimeFormat = RelativeTimeFormat.create().nativeRelativeTimeFormat;
        }

        constructor(locale: string | Array<string>, options?: RelativeTimeFormatInputOptions) {
            this.nativeRelativeTimeFormat = RelativeTimeFormat.create(locale, options).nativeRelativeTimeFormat;
        }

        native static create(): RelativeTimeFormat;
        native static create(locale: string | Array<string>,
            options?: RelativeTimeFormatInputOptions): RelativeTimeFormat;
        native format(value: number, unit: string): string;
        native formatToParts(value: number, unit: string): Array<RelativeTimeFormatPart>;
        native resolvedOptions(): RelativeTimeFormatResolvedOptions;
    }
}
