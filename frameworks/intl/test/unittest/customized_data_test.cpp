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

#include <gtest/gtest.h>
#include <map>
#include <vector>
#include <ctime>
#include "customized_data_test.h"
#include <unistd.h>
#include "unicode/utypes.h"
#include "locale_config.h"
#include "ohos/init_data.h"
#include "i18n_types.h"
#include "simple_date_time_format.h"

using namespace OHOS::Global::I18n;
using testing::ext::TestSize;
using namespace std;
using namespace testing;

namespace OHOS {
namespace Global {
namespace I18n {

void CustomizedDataTest::SetUpTestCase(void)
{
    SetHwIcuDirectory();
}

void CustomizedDataTest::TearDownTestCase(void)
{
}

void CustomizedDataTest::SetUp(void)
{}

void CustomizedDataTest::TearDown(void)
{}

/**
 * @tc.name: CustomizedDataTest0001
 * @tc.desc: Test Customized Data
 * @tc.type: FUNC
 */
HWTEST_F(CustomizedDataTest, CustomizedDataTest0001, TestSize.Level1)
{
    EXPECT_EQ(LocaleConfig::GetDisplayLanguage("ast", "am", false), "አውስትሪያን");
    EXPECT_EQ(LocaleConfig::GetDisplayLanguage("bo", "am", false), "ቲቤቲኛ");
    EXPECT_EQ(LocaleConfig::GetDisplayLanguage("bs", "am", false), "ቦስኒያኛ");
    EXPECT_EQ(LocaleConfig::GetDisplayLanguage("da", "am", false), "ዴኒሽኛ");
    EXPECT_EQ(LocaleConfig::GetDisplayLanguage("dv", "am", false), "ዲቬህ");
    EXPECT_EQ(LocaleConfig::GetDisplayLanguage("et", "am", false), "ኢስቶኒያኛ");
    EXPECT_EQ(LocaleConfig::GetDisplayLanguage("fi", "am", false), "ፊኒሽኛ");
    EXPECT_EQ(LocaleConfig::GetDisplayLanguage("gl", "am", false), "ጋሊሺያኛ");
    EXPECT_EQ(LocaleConfig::GetDisplayLanguage("hr", "am", false), "ክሮሽያኛ");
    EXPECT_EQ(LocaleConfig::GetDisplayLanguage("hy", "am", false), "አርመንኛ");
    EXPECT_EQ(LocaleConfig::GetDisplayLanguage("jv", "am", false), "ጃቫኛ");
    EXPECT_EQ(LocaleConfig::GetDisplayLanguage("ka", "am", false), "ጆርጂያኛ");
    EXPECT_EQ(LocaleConfig::GetDisplayLanguage("ksh", "am", false), "ኮሎኝያን");
    EXPECT_EQ(LocaleConfig::GetDisplayLanguage("lt", "am", false), "ሉትንያኛ");
    EXPECT_EQ(LocaleConfig::GetDisplayLanguage("lv", "am", false), "ላትቪያኛ");
    EXPECT_EQ(LocaleConfig::GetDisplayLanguage("mai", "am", false), "ማይተሊኛ");
    EXPECT_EQ(LocaleConfig::GetDisplayLanguage("ms", "am", false), "ማሌይኛ");
    EXPECT_EQ(LocaleConfig::GetDisplayLanguage("nb", "am", false), "የኖርዌይ ቦክማልኛ");
    EXPECT_EQ(LocaleConfig::GetDisplayLanguage("ne", "am", false), "ኔፓልኛ");
    EXPECT_EQ(LocaleConfig::GetDisplayLanguage("new", "am", false), "ነዋሪ");
    EXPECT_EQ(LocaleConfig::GetDisplayLanguage("tzm", "am", false), "መካከለኛ አትላስ ታማዚግት");
    EXPECT_EQ(LocaleConfig::GetDisplayLanguage("ug", "am", false), "ኡግሁርኛ");
    EXPECT_EQ(LocaleConfig::GetDisplayLanguage("zh-Hans", "am", false), "ዘመናዊ ቻይንኛ");
    EXPECT_EQ(LocaleConfig::GetDisplayLanguage("as", "ar", false), "الآسامية");
    EXPECT_EQ(LocaleConfig::GetDisplayLanguage("hu", "ar", false), "المجرية");
    EXPECT_EQ(LocaleConfig::GetDisplayLanguage("kn", "ar", false), "الكانادية");
    EXPECT_EQ(LocaleConfig::GetDisplayLanguage("mai", "ar", false), "المايثيلية");
    EXPECT_EQ(LocaleConfig::GetDisplayLanguage("sl", "ar", false), "السلوفينية");
    EXPECT_EQ(LocaleConfig::GetDisplayLanguage("ur", "ar", false), "الأردية");
    EXPECT_EQ(LocaleConfig::GetDisplayLanguage("jv", "az", false), "yava dili");
    EXPECT_EQ(LocaleConfig::GetDisplayLanguage("ms", "az", false), "malay dili");
    EXPECT_EQ(LocaleConfig::GetDisplayLanguage("ms", "bn", false), "মালয়");
    EXPECT_EQ(LocaleConfig::GetDisplayLanguage("bo", "bo", false), "བོད་སྐད།");
    EXPECT_EQ(LocaleConfig::GetDisplayLanguage("dz", "bo", false), "རྫོང་ཁའི་སྐད།");
    EXPECT_EQ(LocaleConfig::GetDisplayLanguage("en", "bo", false), "དབྱིན་སྐད།");
    EXPECT_EQ(LocaleConfig::GetDisplayLanguage("hi", "bo", false), "ཧིན་རྡུའི་སྐད།");
    EXPECT_EQ(LocaleConfig::GetDisplayLanguage("ja", "bo", false), "ཉི་ཧོང་སྐད།");
    EXPECT_EQ(LocaleConfig::GetDisplayLanguage("ne", "bo", false), "ནེ་པ་ལའི་སྐད།");
    EXPECT_EQ(LocaleConfig::GetDisplayLanguage("ru", "bo", false), "ཨུ་རུ་སུའི་སྐད།");
    EXPECT_EQ(LocaleConfig::GetDisplayLanguage("zh", "bo", false), "རྒྱ་སྐད།");
}

/**
 * @tc.name: CustomizedDataTest0002
 * @tc.desc: Test Customized Data
 * @tc.type: FUNC
 */
HWTEST_F(CustomizedDataTest, CustomizedDataTest0002, TestSize.Level1)
{
    EXPECT_EQ(LocaleConfig::GetDisplayLanguage("es", "ca", false), "castellà");
    EXPECT_EQ(LocaleConfig::GetDisplayLanguage("mk", "ca", false), "macedònic");
    EXPECT_EQ(LocaleConfig::GetDisplayLanguage("kn", "da", false), "kanaresisk");
    EXPECT_EQ(LocaleConfig::GetDisplayLanguage("mi", "el", false), "Μάορι");
    EXPECT_EQ(LocaleConfig::GetDisplayLanguage("or", "el", false), "Οριγικά");
    EXPECT_EQ(LocaleConfig::GetDisplayLanguage("te", "el", false), "Τελουγκουικά");
    EXPECT_EQ(LocaleConfig::GetDisplayLanguage("ms", "et", false), "malai keel");
    EXPECT_EQ(LocaleConfig::GetDisplayLanguage("ba", "eu", false), "bashkirrera");
    EXPECT_EQ(LocaleConfig::GetDisplayLanguage("ce", "eu", false), "txetxeniera");
    EXPECT_EQ(LocaleConfig::GetDisplayLanguage("cu", "eu", false), "Elizako eslaviera");
    EXPECT_EQ(LocaleConfig::GetDisplayLanguage("cv", "eu", false), "chuvashera");
    EXPECT_EQ(LocaleConfig::GetDisplayLanguage("ga", "eu", false), "gaelikoa");
    EXPECT_EQ(LocaleConfig::GetDisplayLanguage("hz", "eu", false), "herera");
    EXPECT_EQ(LocaleConfig::GetDisplayLanguage("nn", "eu", false), "nynorsk norvegiera");
    EXPECT_EQ(LocaleConfig::GetDisplayLanguage("nv", "eu", false), "navahoera");
    EXPECT_EQ(LocaleConfig::GetDisplayLanguage("pa", "eu", false), "panyabí");
    EXPECT_EQ(LocaleConfig::GetDisplayLanguage("ti", "eu", false), "tigriñera");
    EXPECT_EQ(LocaleConfig::GetDisplayLanguage("vo", "eu", false), "volapüka");
    EXPECT_EQ(LocaleConfig::GetDisplayLanguage("mai", "fa", false), "مایتیلی");
    EXPECT_EQ(LocaleConfig::GetDisplayLanguage("ml", "fa", false), "مالایالم");
    EXPECT_EQ(LocaleConfig::GetDisplayLanguage("or", "fa", false), "اوریه");
    EXPECT_EQ(LocaleConfig::GetDisplayLanguage("sw", "fa", false), "سواحلی");
    EXPECT_EQ(LocaleConfig::GetDisplayLanguage("te", "fa", false), "تلوگو");
    EXPECT_EQ(LocaleConfig::GetDisplayLanguage("gu", "fr", false), "gujarati");
    EXPECT_EQ(LocaleConfig::GetDisplayLanguage("km", "gu", false), "ખમેર");
}

/**
 * @tc.name: CustomizedDataTest0003
 * @tc.desc: Test Customized Data
 * @tc.type: FUNC
 */
HWTEST_F(CustomizedDataTest, CustomizedDataTest0003, TestSize.Level1)
{
    EXPECT_EQ(LocaleConfig::GetDisplayLanguage("gu", "he", false), "גוג׳ארטית");
    EXPECT_EQ(LocaleConfig::GetDisplayLanguage("hi", "he", false), "הינדית");
    EXPECT_EQ(LocaleConfig::GetDisplayLanguage("sw", "he", false), "סוואהילית");
    EXPECT_EQ(LocaleConfig::GetDisplayLanguage("am", "hi", false), "अमहरिक");
    EXPECT_EQ(LocaleConfig::GetDisplayLanguage("ca", "hi", false), "कातलान");
    EXPECT_EQ(LocaleConfig::GetDisplayLanguage("es", "hi", false), "स्पैनिश");
    EXPECT_EQ(LocaleConfig::GetDisplayLanguage("ky", "hi", false), "किरगीज़");
    EXPECT_EQ(LocaleConfig::GetDisplayLanguage("ug", "hi", false), "इयोग़र");
    EXPECT_EQ(LocaleConfig::GetDisplayLanguage("nn", "hu", false), "norvég (nynrosk)");
    EXPECT_EQ(LocaleConfig::GetDisplayLanguage("az", "it", false), "azero");
    EXPECT_EQ(LocaleConfig::GetDisplayLanguage("jv", "it", false), "Giavanese");
    EXPECT_EQ(LocaleConfig::GetDisplayLanguage("ms", "it", false), "Malese");
    EXPECT_EQ(LocaleConfig::GetDisplayLanguage("ug", "jv", false), "Uyghur");
    EXPECT_EQ(LocaleConfig::GetDisplayLanguage("uk", "jv", false), "Ukrainian");
    EXPECT_EQ(LocaleConfig::GetDisplayLanguage("vi", "jv", false), "Vietnamese");
    EXPECT_EQ(LocaleConfig::GetDisplayLanguage("vo", "jv", false), "Volapük");
    EXPECT_EQ(LocaleConfig::GetDisplayLanguage("zh", "jv", false), "Chinese");
    EXPECT_EQ(LocaleConfig::GetDisplayLanguage("zh-Hans", "jv", false), "Simplified Chinese");
    EXPECT_EQ(LocaleConfig::GetDisplayLanguage("zh-Hant", "jv", false), "Traditional Chinese");
}

/**
 * @tc.name: CustomizedDataTest0004
 * @tc.desc: Test Customized Data
 * @tc.type: FUNC
 */
HWTEST_F(CustomizedDataTest, CustomizedDataTest0004, TestSize.Level1)
{
    EXPECT_EQ(LocaleConfig::GetDisplayLanguage("pa", "km", false), "ពុនចាពី");
    EXPECT_EQ(LocaleConfig::GetDisplayLanguage("ms", "kn", false), "ಮಲೇ");
    EXPECT_EQ(LocaleConfig::GetDisplayLanguage("am", "lo", false), "ອຳຮາຣາ");
    EXPECT_EQ(LocaleConfig::GetDisplayLanguage("as", "lo", false), "ອັສຊຳ");
    EXPECT_EQ(LocaleConfig::GetDisplayLanguage("be", "lo", false), "ເບລາຣຸ​ສ​ຊຽນ");
    EXPECT_EQ(LocaleConfig::GetDisplayLanguage("bo", "lo", false), "ຕິ​ເບດ​ຕັນ");
    EXPECT_EQ(LocaleConfig::GetDisplayLanguage("gu", "lo", false), "ກູຈາຣາດ");
    EXPECT_EQ(LocaleConfig::GetDisplayLanguage("jv", "lo", false), "ພາ​ສາ​ຊະ​ວາ");
    EXPECT_EQ(LocaleConfig::GetDisplayLanguage("kn", "lo", false), "ກັນນາດາ");
    EXPECT_EQ(LocaleConfig::GetDisplayLanguage("ky", "lo", false), "ເຄີ​ຈິ​ສ");
    EXPECT_EQ(LocaleConfig::GetDisplayLanguage("lo", "lo", false), "ພາສາລາວ");
    EXPECT_EQ(LocaleConfig::GetDisplayLanguage("mk", "lo", false), "ມາ​ເຊ​ໂດ​ນຽນ");
    EXPECT_EQ(LocaleConfig::GetDisplayLanguage("ms", "lo", false), "ພາ​ສາ​ມາ​ເລ");
    EXPECT_EQ(LocaleConfig::GetDisplayLanguage("my", "lo", false), "ພະ​ມ້າ");
    EXPECT_EQ(LocaleConfig::GetDisplayLanguage("pa", "lo", false), "ປັນຈາບ");
    EXPECT_EQ(LocaleConfig::GetDisplayLanguage("ru", "lo", false), "ຣັດ​ເຊຍ");
    EXPECT_EQ(LocaleConfig::GetDisplayLanguage("si", "lo", false), "ຊິນຫາລາ");
    EXPECT_EQ(LocaleConfig::GetDisplayLanguage("sk", "lo", false), "ສະໂລວັກ");
    EXPECT_EQ(LocaleConfig::GetDisplayLanguage("ta", "lo", false), "ຕາມິລ");
    EXPECT_EQ(LocaleConfig::GetDisplayLanguage("tr", "lo", false), "ເຕີກິ​ສ");
    EXPECT_EQ(LocaleConfig::GetDisplayLanguage("ur", "lo", false), "ເອີດູ");
    EXPECT_EQ(LocaleConfig::GetDisplayLanguage("mr", "lt", false), "marathų");
    EXPECT_EQ(LocaleConfig::GetDisplayLanguage("pa", "lt", false), "pandžabų");
    EXPECT_EQ(LocaleConfig::GetDisplayLanguage("kn", "lv", false), "kannaru");
}

/**
 * @tc.name: CustomizedDataTest0005
 * @tc.desc: Test Customized Data
 * @tc.type: FUNC
 */
HWTEST_F(CustomizedDataTest, CustomizedDataTest0005, TestSize.Level1)
{
    EXPECT_EQ(LocaleConfig::GetDisplayLanguage("am", "mi", false), "Amaharika");
    EXPECT_EQ(LocaleConfig::GetDisplayLanguage("ar", "mi", false), "Arapika");
    EXPECT_EQ(LocaleConfig::GetDisplayLanguage("as", "mi", false), "Ahamīhi");
    EXPECT_EQ(LocaleConfig::GetDisplayLanguage("az", "mi", false), "Ahepairani ");
    EXPECT_EQ(LocaleConfig::GetDisplayLanguage("be", "mi", false), "Pēraruhiana");
    EXPECT_EQ(LocaleConfig::GetDisplayLanguage("bn", "mi", false), "Penekari");
    EXPECT_EQ(LocaleConfig::GetDisplayLanguage("bo", "mi", false), "Tipeti");
    EXPECT_EQ(LocaleConfig::GetDisplayLanguage("bs", "mi", false), "Pōngiana ");
    EXPECT_EQ(LocaleConfig::GetDisplayLanguage("da", "mi", false), "Tenemāka");
    EXPECT_EQ(LocaleConfig::GetDisplayLanguage("fa", "mi", false), "Pāhia");
    EXPECT_EQ(LocaleConfig::GetDisplayLanguage("fi", "mi", false), "Whinihi");
    EXPECT_EQ(LocaleConfig::GetDisplayLanguage("fil", "mi", false), "Piripino");
    EXPECT_EQ(LocaleConfig::GetDisplayLanguage("gl", "mi", false), "Karihiana");
    EXPECT_EQ(LocaleConfig::GetDisplayLanguage("gu", "mi", false), "Kuharati");
    EXPECT_EQ(LocaleConfig::GetDisplayLanguage("he", "mi", false), "Hiperu");
    EXPECT_EQ(LocaleConfig::GetDisplayLanguage("hr", "mi", false), "Koroātia");
    EXPECT_EQ(LocaleConfig::GetDisplayLanguage("hu", "mi", false), "Hanekariana");
    EXPECT_EQ(LocaleConfig::GetDisplayLanguage("id", "mi", false), "Initonīhia");
    EXPECT_EQ(LocaleConfig::GetDisplayLanguage("is", "mi", false), "Tiorangiana");
    EXPECT_EQ(LocaleConfig::GetDisplayLanguage("jv", "mi", false), "Hawanihi ");
    EXPECT_EQ(LocaleConfig::GetDisplayLanguage("kk", "mi", false), "Kāhāka");
    EXPECT_EQ(LocaleConfig::GetDisplayLanguage("km", "mi", false), "Kīma");
    EXPECT_EQ(LocaleConfig::GetDisplayLanguage("kn", "mi", false), "Kanata");
    EXPECT_EQ(LocaleConfig::GetDisplayLanguage("ko", "mi", false), "Korea");
    EXPECT_EQ(LocaleConfig::GetDisplayLanguage("ky", "mi", false), "Kerakihi");
    EXPECT_EQ(LocaleConfig::GetDisplayLanguage("lo", "mi", false), "Rāo");
    EXPECT_EQ(LocaleConfig::GetDisplayLanguage("lt", "mi", false), "Rituāniana");
    EXPECT_EQ(LocaleConfig::GetDisplayLanguage("lv", "mi", false), "Ratiwiana");
    EXPECT_EQ(LocaleConfig::GetDisplayLanguage("mai", "mi", false), "Maitahiri ");
    EXPECT_EQ(LocaleConfig::GetDisplayLanguage("mk", "mi", false), "Makeronia");
    EXPECT_EQ(LocaleConfig::GetDisplayLanguage("ml", "mi", false), "Marāiama");
    EXPECT_EQ(LocaleConfig::GetDisplayLanguage("mn", "mi", false), "Mongōriana");
    EXPECT_EQ(LocaleConfig::GetDisplayLanguage("my", "mi", false), "Pemīhia");
    EXPECT_EQ(LocaleConfig::GetDisplayLanguage("nb", "mi", false), "Nowīhiana Pokomāra");
    EXPECT_EQ(LocaleConfig::GetDisplayLanguage("nl", "mi", false), "Tatimana");
    EXPECT_EQ(LocaleConfig::GetDisplayLanguage("or", "mi", false), "Orīa");
    EXPECT_EQ(LocaleConfig::GetDisplayLanguage("pa", "mi", false), "Puniapi");
    EXPECT_EQ(LocaleConfig::GetDisplayLanguage("pl", "mi", false), "Pourihi");
    EXPECT_EQ(LocaleConfig::GetDisplayLanguage("ro", "mi", false), "Rōmaniana");
    EXPECT_EQ(LocaleConfig::GetDisplayLanguage("si", "mi", false), "Hinihara");
    EXPECT_EQ(LocaleConfig::GetDisplayLanguage("sk", "mi", false), "Horowāka");
    EXPECT_EQ(LocaleConfig::GetDisplayLanguage("sl", "mi", false), "Horowīniana");
    EXPECT_EQ(LocaleConfig::GetDisplayLanguage("sq", "mi", false), "Arapainia");
    EXPECT_EQ(LocaleConfig::GetDisplayLanguage("sr", "mi", false), "Hēpiana ");
    EXPECT_EQ(LocaleConfig::GetDisplayLanguage("sw", "mi", false), "Tawahiri");
    EXPECT_EQ(LocaleConfig::GetDisplayLanguage("ta", "mi", false), "Tamiri");
    EXPECT_EQ(LocaleConfig::GetDisplayLanguage("ug", "mi", false), "Ūkere");
    EXPECT_EQ(LocaleConfig::GetDisplayLanguage("uk", "mi", false), "Ūkareiniana");
    EXPECT_EQ(LocaleConfig::GetDisplayLanguage("ur", "mi", false), "Ūrū");
    EXPECT_EQ(LocaleConfig::GetDisplayLanguage("uz", "mi", false), "Hūpeka");
    EXPECT_EQ(LocaleConfig::GetDisplayLanguage("zh-Hant", "mi", false), "Hainamana Taketake");
}

/**
 * @tc.name: CustomizedDataTest0006
 * @tc.desc: Test Customized Data
 * @tc.type: FUNC
 */
HWTEST_F(CustomizedDataTest, CustomizedDataTest0006, TestSize.Level1)
{
    EXPECT_EQ(LocaleConfig::GetDisplayLanguage("ms", "ml", false), "മലായ്");
    EXPECT_EQ(LocaleConfig::GetDisplayLanguage("jv", "mn", false), "Ява");
    EXPECT_EQ(LocaleConfig::GetDisplayLanguage("ms", "mn", false), "Малай");
    EXPECT_EQ(LocaleConfig::GetDisplayLanguage("ms", "ms", false), "Bahasa Melayu");
    EXPECT_EQ(LocaleConfig::GetDisplayLanguage("gu", "my", false), "ဂူဂျာရတ်တီ");
    EXPECT_EQ(LocaleConfig::GetDisplayLanguage("hi", "my", false), "ဟိန္ဒီ");
    EXPECT_EQ(LocaleConfig::GetDisplayLanguage("lt", "my", false), "လစ်သူယေးနီးယား");
    EXPECT_EQ(LocaleConfig::GetDisplayLanguage("my", "my", false), "ဗမာ");
    EXPECT_EQ(LocaleConfig::GetDisplayLanguage("ro", "my", false), "ရိုမေးနီယား");
    EXPECT_EQ(LocaleConfig::GetDisplayLanguage("ru", "my", false), "ရုရှား");
    EXPECT_EQ(LocaleConfig::GetDisplayLanguage("te", "my", false), "တယ်လဂူ");
    EXPECT_EQ(LocaleConfig::GetDisplayLanguage("ur", "my", false), "အူရဒူ");
    EXPECT_EQ(LocaleConfig::GetDisplayLanguage("uz", "my", false), "ဥဇဘက်");
    EXPECT_EQ(LocaleConfig::GetDisplayLanguage("ms", "ne", false), "मलाया");
    EXPECT_EQ(LocaleConfig::GetDisplayLanguage("eu", "or", false), "ବାସ୍କେ");
    EXPECT_EQ(LocaleConfig::GetDisplayLanguage("ms", "or", false), "ମଳୟ");
    EXPECT_EQ(LocaleConfig::GetDisplayLanguage("si", "or", false), "ସିଂହଳା");
    EXPECT_EQ(LocaleConfig::GetDisplayLanguage("mi", "pl", false), "maoryski");
    EXPECT_EQ(LocaleConfig::GetDisplayLanguage("hr", "si", false), "ක්‍රෝසියානු");
    EXPECT_EQ(LocaleConfig::GetDisplayLanguage("hu", "si", false), "හංගේරියානු");
    EXPECT_EQ(LocaleConfig::GetDisplayLanguage("ro", "si", false), "රුමේනියානු");
    EXPECT_EQ(LocaleConfig::GetDisplayLanguage("te", "si", false), "තෙළුඟු");
    EXPECT_EQ(LocaleConfig::GetDisplayLanguage("or", "sr-Latn", false), "orija");
    EXPECT_EQ(LocaleConfig::GetDisplayLanguage("nb", "sv", false), "norska bokmål");
    EXPECT_EQ(LocaleConfig::GetDisplayLanguage("eu", "sw", false), "Kibaske");
    EXPECT_EQ(LocaleConfig::GetDisplayLanguage("kk", "sw", false), "Kikazaki");
    EXPECT_EQ(LocaleConfig::GetDisplayLanguage("my", "sw", false), "Kibarma");
    EXPECT_EQ(LocaleConfig::GetDisplayLanguage("nb", "sw", false), "Kibokmali cha Norwe");
    EXPECT_EQ(LocaleConfig::GetDisplayLanguage("or", "sw", false), "Kiodiya");
    EXPECT_EQ(LocaleConfig::GetDisplayLanguage("ug", "sw", false), "Kiuigu");
    EXPECT_EQ(LocaleConfig::GetDisplayLanguage("uk", "sw", false), "Kiukreni");
    EXPECT_EQ(LocaleConfig::GetDisplayLanguage("as", "ta", false), "அஸ்ஸாமி");
    EXPECT_EQ(LocaleConfig::GetDisplayLanguage("bn", "ta", false), "வங்காளி");
    EXPECT_EQ(LocaleConfig::GetDisplayLanguage("pt", "ta", false), "போர்ச்சுக்கீசியம்");
    EXPECT_EQ(LocaleConfig::GetDisplayLanguage("vi", "ta", false), "வியட்நாமிஸ்");
}

/**
 * @tc.name: CustomizedDataTest0007
 * @tc.desc: Test Customized Data
 * @tc.type: FUNC
 */
HWTEST_F(CustomizedDataTest, CustomizedDataTest0007, TestSize.Level1)
{
    EXPECT_EQ(LocaleConfig::GetDisplayLanguage("arc", "te", false), "అరామేక్");
    EXPECT_EQ(LocaleConfig::GetDisplayLanguage("ba", "te", false), "బష్కిర్");
    EXPECT_EQ(LocaleConfig::GetDisplayLanguage("ceb", "te", false), "సెబుయానో");
    EXPECT_EQ(LocaleConfig::GetDisplayLanguage("fo", "te", false), "ఫారోయీజ్");
    EXPECT_EQ(LocaleConfig::GetDisplayLanguage("hil", "te", false), "హిలిగేయినోన్");
    EXPECT_EQ(LocaleConfig::GetDisplayLanguage("hz", "te", false), "హిరేరో");
    EXPECT_EQ(LocaleConfig::GetDisplayLanguage("kho", "te", false), "ఖటోనీస్");
    EXPECT_EQ(LocaleConfig::GetDisplayLanguage("mg", "te", false), "మాలాగసి");
    EXPECT_EQ(LocaleConfig::GetDisplayLanguage("niu", "te", false), "నాయియన్");
    EXPECT_EQ(LocaleConfig::GetDisplayLanguage("sah", "te", false), "సఖా");
    EXPECT_EQ(LocaleConfig::GetDisplayLanguage("sam", "te", false), "సమారిటన్ అరమేక్");
    EXPECT_EQ(LocaleConfig::GetDisplayLanguage("sat", "te", false), "సంటాలి");
    EXPECT_EQ(LocaleConfig::GetDisplayLanguage("sdh", "te", false), "దక్షిణ కుర్దిష్");
    EXPECT_EQ(LocaleConfig::GetDisplayLanguage("ti", "te", false), "తిగ్రిన్యా");
    EXPECT_EQ(LocaleConfig::GetDisplayLanguage("wo", "te", false), "వొలాఫ్");
    EXPECT_EQ(LocaleConfig::GetDisplayLanguage("th", "th", false), "ภาษาไทย");
    EXPECT_EQ(LocaleConfig::GetDisplayLanguage("as", "tr", false), "Assam dili");
    EXPECT_EQ(LocaleConfig::GetDisplayLanguage("ast", "tr", false), "Asturyasca");
    EXPECT_EQ(LocaleConfig::GetDisplayLanguage("mi", "tr", false), "Maorice");
    EXPECT_EQ(LocaleConfig::GetDisplayLanguage("as", "uk", false), "ассамі");
    EXPECT_EQ(LocaleConfig::GetDisplayLanguage("bg", "ur", false), "بلغاریائی");
    EXPECT_EQ(LocaleConfig::GetDisplayLanguage("et", "ur", false), "اسٹونیائی");
    EXPECT_EQ(LocaleConfig::GetDisplayLanguage("fil", "ur", false), "فلپینو");
    EXPECT_EQ(LocaleConfig::GetDisplayLanguage("hr", "ur", false), "کروشیائی");
    EXPECT_EQ(LocaleConfig::GetDisplayLanguage("hu", "ur", false), "ہنگریائی");
    EXPECT_EQ(LocaleConfig::GetDisplayLanguage("id", "ur", false), "انڈونیشیائی");
    EXPECT_EQ(LocaleConfig::GetDisplayLanguage("kde", "ur", false), "ماكونده");
    EXPECT_EQ(LocaleConfig::GetDisplayLanguage("kea", "ur", false), "كابويرديانو");
    EXPECT_EQ(LocaleConfig::GetDisplayLanguage("khq", "ur", false), "كويرا شيني");
    EXPECT_EQ(LocaleConfig::GetDisplayLanguage("kl", "ur", false), "كالاليست");
    EXPECT_EQ(LocaleConfig::GetDisplayLanguage("kln", "ur", false), "كالينجين");
    EXPECT_EQ(LocaleConfig::GetDisplayLanguage("kn", "ur", false), "کنّڑ");
    EXPECT_EQ(LocaleConfig::GetDisplayLanguage("lt", "ur", false), "لیتھوینیائی");
    EXPECT_EQ(LocaleConfig::GetDisplayLanguage("lu", "ur", false), "لبا-كاتانجا");
    EXPECT_EQ(LocaleConfig::GetDisplayLanguage("ml", "ur", false), "ملیالم");
    EXPECT_EQ(LocaleConfig::GetDisplayLanguage("nmg", "ur", false), "كوايسو");
    EXPECT_EQ(LocaleConfig::GetDisplayLanguage("oc", "ur", false), "آكسیٹان");
    EXPECT_EQ(LocaleConfig::GetDisplayLanguage("quc", "ur", false), "كيشی");
    EXPECT_EQ(LocaleConfig::GetDisplayLanguage("ro", "ur", false), "رومانیائی");
    EXPECT_EQ(LocaleConfig::GetDisplayLanguage("ses", "ur", false), "كويرابورو سينی");
    EXPECT_EQ(LocaleConfig::GetDisplayLanguage("sq", "ur", false), "البانیائی");
    EXPECT_EQ(LocaleConfig::GetDisplayLanguage("sr", "ur", false), "سرب");
    EXPECT_EQ(LocaleConfig::GetDisplayLanguage("zh-Hans", "ur", false), "چینی (آسان)");
}

/**
 * @tc.name: CustomizedDataTest0008
 * @tc.desc: Test Customized Data
 * @tc.type: FUNC
 */
HWTEST_F(CustomizedDataTest, CustomizedDataTest0008, TestSize.Level1)
{
    EXPECT_EQ(LocaleConfig::GetDisplayLanguage("be", "ug", false), "بېلورۇسچە");
    EXPECT_EQ(LocaleConfig::GetDisplayLanguage("bo", "ug", false), "زاڭزۇچە");
    EXPECT_EQ(LocaleConfig::GetDisplayLanguage("ca", "ug", false), "كاتالونچە");
    EXPECT_EQ(LocaleConfig::GetDisplayLanguage("da", "ug", false), "دانىيەچە");
    EXPECT_EQ(LocaleConfig::GetDisplayLanguage("de", "ug", false), "نېمىسچە");
    EXPECT_EQ(LocaleConfig::GetDisplayLanguage("fi", "ug", false), "فىنلاندىيەچە");
    EXPECT_EQ(LocaleConfig::GetDisplayLanguage("gl", "ug", false), "جەلىلىيەچە");
    EXPECT_EQ(LocaleConfig::GetDisplayLanguage("lt", "ug", false), "لىتۋانچە");
    EXPECT_EQ(LocaleConfig::GetDisplayLanguage("lv", "ug", false), "لاتۋىيەچە");
    EXPECT_EQ(LocaleConfig::GetDisplayLanguage("pl", "ug", false), "پولشاچە");
    EXPECT_EQ(LocaleConfig::GetDisplayLanguage("sv", "ug", false), "شۋېتسىيەچە");
    EXPECT_EQ(LocaleConfig::GetDisplayLanguage("zh-Hans", "ug", false), "ئاددىي خەنزۇچە");
    EXPECT_EQ(LocaleConfig::GetDisplayLanguage("zh-Hant", "ug", false), "ئەنئەنىۋىي خەنزۇچە");
    EXPECT_EQ(LocaleConfig::GetDisplayLanguage("am", "uz", false), "amxara");
    EXPECT_EQ(LocaleConfig::GetDisplayLanguage("as", "uz", false), "assom");
    EXPECT_EQ(LocaleConfig::GetDisplayLanguage("bs", "uz", false), "bosniya");
    EXPECT_EQ(LocaleConfig::GetDisplayLanguage("de", "uz", false), "nemis");
    EXPECT_EQ(LocaleConfig::GetDisplayLanguage("en", "uz", false), "ingliz");
    EXPECT_EQ(LocaleConfig::GetDisplayLanguage("es", "uz", false), "ispan");
    EXPECT_EQ(LocaleConfig::GetDisplayLanguage("et", "uz", false), "eston");
    EXPECT_EQ(LocaleConfig::GetDisplayLanguage("fi", "uz", false), "fin");
    EXPECT_EQ(LocaleConfig::GetDisplayLanguage("fil", "uz", false), "filippin");
    EXPECT_EQ(LocaleConfig::GetDisplayLanguage("fr", "uz", false), "fransuz");
    EXPECT_EQ(LocaleConfig::GetDisplayLanguage("jv", "uz", false), "yava");
    EXPECT_EQ(LocaleConfig::GetDisplayLanguage("ka", "uz", false), "gruzin");
    EXPECT_EQ(LocaleConfig::GetDisplayLanguage("kk", "uz", false), "qozoq");
    EXPECT_EQ(LocaleConfig::GetDisplayLanguage("km", "uz", false), "kxmer");
    EXPECT_EQ(LocaleConfig::GetDisplayLanguage("ko", "uz", false), "koreys");
    EXPECT_EQ(LocaleConfig::GetDisplayLanguage("ky", "uz", false), "qirgʻiz");
    EXPECT_EQ(LocaleConfig::GetDisplayLanguage("lv", "uz", false), "latish");
    EXPECT_EQ(LocaleConfig::GetDisplayLanguage("mr", "uz", false), "marathiy");
    EXPECT_EQ(LocaleConfig::GetDisplayLanguage("my", "uz", false), "birma");
    EXPECT_EQ(LocaleConfig::GetDisplayLanguage("nb", "uz", false), "norveg-bukmol");
    EXPECT_EQ(LocaleConfig::GetDisplayLanguage("pa", "uz", false), "panjob");
    EXPECT_EQ(LocaleConfig::GetDisplayLanguage("pl", "uz", false), "polyak");
    EXPECT_EQ(LocaleConfig::GetDisplayLanguage("pt", "uz", false), "portugal");
    EXPECT_EQ(LocaleConfig::GetDisplayLanguage("ro", "uz", false), "rumin");
    EXPECT_EQ(LocaleConfig::GetDisplayLanguage("ru", "uz", false), "rus");
    EXPECT_EQ(LocaleConfig::GetDisplayLanguage("sk", "uz", false), "slovak");
    EXPECT_EQ(LocaleConfig::GetDisplayLanguage("sl", "uz", false), "sloven");
    EXPECT_EQ(LocaleConfig::GetDisplayLanguage("sr", "uz", false), "serb");
}

/**
 * @tc.name: CustomizedDataTest0009
 * @tc.desc: Test Customized Data
 * @tc.type: FUNC
 */
HWTEST_F(CustomizedDataTest, CustomizedDataTest0009, TestSize.Level1)
{
    EXPECT_EQ(LocaleConfig::GetDisplayLanguage("af", "jv", false), "Afrikaans");
    EXPECT_EQ(LocaleConfig::GetDisplayLanguage("alt", "jv", false), "Southern Altai");
    EXPECT_EQ(LocaleConfig::GetDisplayLanguage("am", "jv", false), "Amharic");
    EXPECT_EQ(LocaleConfig::GetDisplayLanguage("ar", "jv", false), "Arabic");
    EXPECT_EQ(LocaleConfig::GetDisplayLanguage("arn", "jv", false), "Mapuche");
    EXPECT_EQ(LocaleConfig::GetDisplayLanguage("as", "jv", false), "Assamese");
    EXPECT_EQ(LocaleConfig::GetDisplayLanguage("ast", "jv", false), "Asturian");
    EXPECT_EQ(LocaleConfig::GetDisplayLanguage("az", "jv", false), "Azerbaijani");
    EXPECT_EQ(LocaleConfig::GetDisplayLanguage("ban", "jv", false), "Balinese");
    EXPECT_EQ(LocaleConfig::GetDisplayLanguage("be", "jv", false), "Belarusian");
    EXPECT_EQ(LocaleConfig::GetDisplayLanguage("bg", "jv", false), "Bulgarian");
    EXPECT_EQ(LocaleConfig::GetDisplayLanguage("bla", "jv", false), "Siksika");
    EXPECT_EQ(LocaleConfig::GetDisplayLanguage("bo", "jv", false), "Tibetan");
    EXPECT_EQ(LocaleConfig::GetDisplayLanguage("bs", "jv", false), "Bosnian");
    EXPECT_EQ(LocaleConfig::GetDisplayLanguage("bug", "jv", false), "Buginese");
    EXPECT_EQ(LocaleConfig::GetDisplayLanguage("ca", "jv", false), "Catalan");
    EXPECT_EQ(LocaleConfig::GetDisplayLanguage("cay", "jv", false), "Cayuga");
    EXPECT_EQ(LocaleConfig::GetDisplayLanguage("ch", "jv", false), "Chamorro");
    EXPECT_EQ(LocaleConfig::GetDisplayLanguage("cs", "jv", false), "Czech");
    EXPECT_EQ(LocaleConfig::GetDisplayLanguage("cu", "jv", false), "Church Slavic");
    EXPECT_EQ(LocaleConfig::GetDisplayLanguage("cv", "jv", false), "Chuvash");
    EXPECT_EQ(LocaleConfig::GetDisplayLanguage("da", "jv", false), "Danish");
    EXPECT_EQ(LocaleConfig::GetDisplayLanguage("de", "jv", false), "German");
    EXPECT_EQ(LocaleConfig::GetDisplayLanguage("de-CH", "jv", false), "German (Switserlan)");
    EXPECT_EQ(LocaleConfig::GetDisplayLanguage("dsb", "jv", false), "Lower Sorbian");
    EXPECT_EQ(LocaleConfig::GetDisplayLanguage("el", "jv", false), "Greek");
    EXPECT_EQ(LocaleConfig::GetDisplayLanguage("en", "jv", false), "English");
    EXPECT_EQ(LocaleConfig::GetDisplayLanguage("en-AU", "jv", false), "English (Ostrali)");
    EXPECT_EQ(LocaleConfig::GetDisplayLanguage("en-CA", "jv", false), "English (Kanada)");
    EXPECT_EQ(LocaleConfig::GetDisplayLanguage("en-GB", "jv", false), "English (Karajan Manunggal)");
    EXPECT_EQ(LocaleConfig::GetDisplayLanguage("en-US", "jv", false), "English (Amérika Sarékat)");
    EXPECT_EQ(LocaleConfig::GetDisplayLanguage("es", "jv", false), "Spanish");
    EXPECT_EQ(LocaleConfig::GetDisplayLanguage("es-419", "jv", false), "Spanish (Amérika Latin)");
    EXPECT_EQ(LocaleConfig::GetDisplayLanguage("es-ES", "jv", false), "Spanish (Sepanyol)");
    EXPECT_EQ(LocaleConfig::GetDisplayLanguage("es-MX", "jv", false), "Spanish (Mèksiko)");
    EXPECT_EQ(LocaleConfig::GetDisplayLanguage("et", "jv", false), "Estonian");
    EXPECT_EQ(LocaleConfig::GetDisplayLanguage("fa", "jv", false), "Persian");
    EXPECT_EQ(LocaleConfig::GetDisplayLanguage("fi", "jv", false), "Finnish");
    EXPECT_EQ(LocaleConfig::GetDisplayLanguage("fil", "jv", false), "Filipino");
    EXPECT_EQ(LocaleConfig::GetDisplayLanguage("fo", "jv", false), "Faroese");
    EXPECT_EQ(LocaleConfig::GetDisplayLanguage("fr", "jv", false), "French");
    EXPECT_EQ(LocaleConfig::GetDisplayLanguage("fr-CA", "jv", false), "French (Kanada)");
    EXPECT_EQ(LocaleConfig::GetDisplayLanguage("fr-CH", "jv", false), "French (Switserlan)");
    EXPECT_EQ(LocaleConfig::GetDisplayLanguage("frc", "jv", false), "Cajun French");
    EXPECT_EQ(LocaleConfig::GetDisplayLanguage("frr", "jv", false), "Northern Frisian");
    EXPECT_EQ(LocaleConfig::GetDisplayLanguage("fy", "jv", false), "Western Frisian");
}

/**
 * @tc.name: CustomizedDataTest0010
 * @tc.desc: Test Customized Data
 * @tc.type: FUNC
 */
HWTEST_F(CustomizedDataTest, CustomizedDataTest0010, TestSize.Level1)
{
    EXPECT_EQ(LocaleConfig::GetDisplayLanguage("ga", "jv", false), "Irish");
    EXPECT_EQ(LocaleConfig::GetDisplayLanguage("gd", "jv", false), "Scottish Gaelic");
    EXPECT_EQ(LocaleConfig::GetDisplayLanguage("gez", "jv", false), "Geez");
    EXPECT_EQ(LocaleConfig::GetDisplayLanguage("gl", "jv", false), "Galician");
    EXPECT_EQ(LocaleConfig::GetDisplayLanguage("gsw", "jv", false), "Swiss German");
    EXPECT_EQ(LocaleConfig::GetDisplayLanguage("gu", "jv", false), "Gujarati");
    EXPECT_EQ(LocaleConfig::GetDisplayLanguage("haw", "jv", false), "Hawaiian");
    EXPECT_EQ(LocaleConfig::GetDisplayLanguage("he", "jv", false), "Hebrew");
    EXPECT_EQ(LocaleConfig::GetDisplayLanguage("hi", "jv", false), "Hindi");
    EXPECT_EQ(LocaleConfig::GetDisplayLanguage("hil", "jv", false), "Hiligaynon");
    EXPECT_EQ(LocaleConfig::GetDisplayLanguage("hr", "jv", false), "Croatian");
    EXPECT_EQ(LocaleConfig::GetDisplayLanguage("hsb", "jv", false), "Upper Sorbian");
    EXPECT_EQ(LocaleConfig::GetDisplayLanguage("ht", "jv", false), "Haitian Creole");
    EXPECT_EQ(LocaleConfig::GetDisplayLanguage("hu", "jv", false), "Hungarian");
    EXPECT_EQ(LocaleConfig::GetDisplayLanguage("hy", "jv", false), "Armenian");
    EXPECT_EQ(LocaleConfig::GetDisplayLanguage("id", "jv", false), "Indonesian");
    EXPECT_EQ(LocaleConfig::GetDisplayLanguage("ig", "jv", false), "Igbo");
    EXPECT_EQ(LocaleConfig::GetDisplayLanguage("inh", "jv", false), "Ingush");
    EXPECT_EQ(LocaleConfig::GetDisplayLanguage("is", "jv", false), "Icelandic");
    EXPECT_EQ(LocaleConfig::GetDisplayLanguage("it", "jv", false), "Italian");
    EXPECT_EQ(LocaleConfig::GetDisplayLanguage("ja", "jv", false), "Japanese");
    EXPECT_EQ(LocaleConfig::GetDisplayLanguage("jv", "jv", false), "Basa Jawa");
    EXPECT_EQ(LocaleConfig::GetDisplayLanguage("ka", "jv", false), "Georgian");
    EXPECT_EQ(LocaleConfig::GetDisplayLanguage("kac", "jv", false), "Kachin");
    EXPECT_EQ(LocaleConfig::GetDisplayLanguage("ko", "jv", false), "Korean");
    EXPECT_EQ(LocaleConfig::GetDisplayLanguage("krc", "jv", false), "Karachay-Balkar");
    EXPECT_EQ(LocaleConfig::GetDisplayLanguage("kru", "jv", false), "Kurukh");
    EXPECT_EQ(LocaleConfig::GetDisplayLanguage("ksh", "jv", false), "Colognian");
    EXPECT_EQ(LocaleConfig::GetDisplayLanguage("ku", "jv", false), "Kurdish");
    EXPECT_EQ(LocaleConfig::GetDisplayLanguage("kum", "jv", false), "Kumyk");
    EXPECT_EQ(LocaleConfig::GetDisplayLanguage("kw", "jv", false), "Cornish");
    EXPECT_EQ(LocaleConfig::GetDisplayLanguage("ky", "jv", false), "Kyrgyz");
    EXPECT_EQ(LocaleConfig::GetDisplayLanguage("lb", "jv", false), "Luxembourgish");
    EXPECT_EQ(LocaleConfig::GetDisplayLanguage("lez", "jv", false), "Lezghian");
    EXPECT_EQ(LocaleConfig::GetDisplayLanguage("lo", "jv", false), "Lao");
    EXPECT_EQ(LocaleConfig::GetDisplayLanguage("loz", "jv", false), "Lozi");
    EXPECT_EQ(LocaleConfig::GetDisplayLanguage("lrc", "jv", false), "Northern Luri");
    EXPECT_EQ(LocaleConfig::GetDisplayLanguage("lt", "jv", false), "Lithuanian");
    EXPECT_EQ(LocaleConfig::GetDisplayLanguage("lus", "jv", false), "Mizo");
    EXPECT_EQ(LocaleConfig::GetDisplayLanguage("lv", "jv", false), "Latvian");
    EXPECT_EQ(LocaleConfig::GetDisplayLanguage("mad", "jv", false), "Madurese");
    EXPECT_EQ(LocaleConfig::GetDisplayLanguage("mg", "jv", false), "Malagasy");
    EXPECT_EQ(LocaleConfig::GetDisplayLanguage("mgh", "jv", false), "Makhuwa-Meetto");
    EXPECT_EQ(LocaleConfig::GetDisplayLanguage("mgo", "jv", false), "Metaʼ");
    EXPECT_EQ(LocaleConfig::GetDisplayLanguage("mic", "jv", false), "Micmac");
    EXPECT_EQ(LocaleConfig::GetDisplayLanguage("mk", "jv", false), "Macedonian");
    EXPECT_EQ(LocaleConfig::GetDisplayLanguage("mn", "jv", false), "Mongolian");
    EXPECT_EQ(LocaleConfig::GetDisplayLanguage("ms", "jv", false), "Basa Melayu");
    EXPECT_EQ(LocaleConfig::GetDisplayLanguage("mt", "jv", false), "Maltese");
    EXPECT_EQ(LocaleConfig::GetDisplayLanguage("mul", "jv", false), "Multiple Languages");
}

/**
 * @tc.name: CustomizedDataTest0011
 * @tc.desc: Test Customized Data
 * @tc.type: FUNC
 */
HWTEST_F(CustomizedDataTest, CustomizedDataTest0011, TestSize.Level1)
{
    EXPECT_EQ(LocaleConfig::GetDisplayLanguage("mus", "jv", false), "Creek");
    EXPECT_EQ(LocaleConfig::GetDisplayLanguage("my", "jv", false), "Burmese");
    EXPECT_EQ(LocaleConfig::GetDisplayLanguage("myv", "jv", false), "Erzya");
    EXPECT_EQ(LocaleConfig::GetDisplayLanguage("nb", "jv", false), "Norwegian Bokmål");
    EXPECT_EQ(LocaleConfig::GetDisplayLanguage("nd", "jv", false), "North Ndebele");
    EXPECT_EQ(LocaleConfig::GetDisplayLanguage("nds", "jv", false), "Low German");
    EXPECT_EQ(LocaleConfig::GetDisplayLanguage("ne", "jv", false), "Nepali");
    EXPECT_EQ(LocaleConfig::GetDisplayLanguage("nl", "jv", false), "Dutch");
    EXPECT_EQ(LocaleConfig::GetDisplayLanguage("nl-BE", "jv", false), "Dutch (Bèlgi)");
    EXPECT_EQ(LocaleConfig::GetDisplayLanguage("nn", "jv", false), "Norwegian Nynorsk");
    EXPECT_EQ(LocaleConfig::GetDisplayLanguage("no", "jv", false), "Norwegian");
    EXPECT_EQ(LocaleConfig::GetDisplayLanguage("nr", "jv", false), "South Ndebele");
    EXPECT_EQ(LocaleConfig::GetDisplayLanguage("nso", "jv", false), "Northern Sotho");
    EXPECT_EQ(LocaleConfig::GetDisplayLanguage("oc", "jv", false), "Occitan");
    EXPECT_EQ(LocaleConfig::GetDisplayLanguage("or", "jv", false), "Oriya");
    EXPECT_EQ(LocaleConfig::GetDisplayLanguage("os", "jv", false), "Ossetic");
    EXPECT_EQ(LocaleConfig::GetDisplayLanguage("pa", "jv", false), "Punjabi");
    EXPECT_EQ(LocaleConfig::GetDisplayLanguage("prg", "jv", false), "Prussian");
    EXPECT_EQ(LocaleConfig::GetDisplayLanguage("pt", "jv", false), "Portuguese");
    EXPECT_EQ(LocaleConfig::GetDisplayLanguage("pt-BR", "jv", false), "Portuguese (Brasil)");
    EXPECT_EQ(LocaleConfig::GetDisplayLanguage("pt-PT", "jv", false), "Portuguese (Portugal)");
    EXPECT_EQ(LocaleConfig::GetDisplayLanguage("rm", "jv", false), "Romansh");
    EXPECT_EQ(LocaleConfig::GetDisplayLanguage("ro", "jv", false), "Romanian");
    EXPECT_EQ(LocaleConfig::GetDisplayLanguage("sa", "jv", false), "Sanskrit");
    EXPECT_EQ(LocaleConfig::GetDisplayLanguage("sba", "jv", false), "Ngambay");
    EXPECT_EQ(LocaleConfig::GetDisplayLanguage("scn", "jv", false), "Sicilian");
    EXPECT_EQ(LocaleConfig::GetDisplayLanguage("sco", "jv", false), "Scots");
    EXPECT_EQ(LocaleConfig::GetDisplayLanguage("se", "jv", false), "Northern Sami");
    EXPECT_EQ(LocaleConfig::GetDisplayLanguage("sk", "jv", false), "Slovak");
    EXPECT_EQ(LocaleConfig::GetDisplayLanguage("sl", "jv", false), "Slovenian");
    EXPECT_EQ(LocaleConfig::GetDisplayLanguage("sm", "jv", false), "Samoan");
    EXPECT_EQ(LocaleConfig::GetDisplayLanguage("so", "jv", false), "Somali");
    EXPECT_EQ(LocaleConfig::GetDisplayLanguage("sq", "jv", false), "Albanian");
    EXPECT_EQ(LocaleConfig::GetDisplayLanguage("sr", "jv", false), "Serbian");
    EXPECT_EQ(LocaleConfig::GetDisplayLanguage("st", "jv", false), "Southern Sotho");
    EXPECT_EQ(LocaleConfig::GetDisplayLanguage("su", "jv", false), "Sundanese");
    EXPECT_EQ(LocaleConfig::GetDisplayLanguage("sv", "jv", false), "Swedish");
    EXPECT_EQ(LocaleConfig::GetDisplayLanguage("swb", "jv", false), "Comorian");
    EXPECT_EQ(LocaleConfig::GetDisplayLanguage("syr", "jv", false), "Syriac");
    EXPECT_EQ(LocaleConfig::GetDisplayLanguage("th", "jv", false), "Thai");
    EXPECT_EQ(LocaleConfig::GetDisplayLanguage("to", "jv", false), "Tongan");
    EXPECT_EQ(LocaleConfig::GetDisplayLanguage("tr", "jv", false), "Turkish");
    EXPECT_EQ(LocaleConfig::GetDisplayLanguage("tvl", "jv", false), "Tuvalu");
    EXPECT_EQ(LocaleConfig::GetDisplayLanguage("ty", "jv", false), "Tahitian");
    EXPECT_EQ(LocaleConfig::GetDisplayLanguage("tyv", "jv", false), "Tuvinian");
}

/**
 * @tc.name: CustomizedDataTest0012
 * @tc.desc: Test Customized Data
 * @tc.type: FUNC
 */
HWTEST_F(CustomizedDataTest, CustomizedDataTest0012, TestSize.Level1)
{
    EXPECT_EQ(LocaleConfig::GetDisplayRegion("BT", "am", false), "ቡታን");
    EXPECT_EQ(LocaleConfig::GetDisplayRegion("EH", "am", false), "ምዕራባዊ ሳሃራ");
    EXPECT_EQ(LocaleConfig::GetDisplayRegion("EZ", "am", false), "የዩሮ ዞን");
    EXPECT_EQ(LocaleConfig::GetDisplayRegion("FO", "am", false), "የፋሮዌ ደሴቶች");
    EXPECT_EQ(LocaleConfig::GetDisplayRegion("HK", "am", false), "ሆንግ ኮንግ (ቻይና)");
    EXPECT_EQ(LocaleConfig::GetDisplayRegion("HM", "am", false), "ኽርድ እና ማክዶናልድ ደሴቶች");
    EXPECT_EQ(LocaleConfig::GetDisplayRegion("MK", "am", false), "ሰሜን መቄዶኒያ");
    EXPECT_EQ(LocaleConfig::GetDisplayRegion("MO", "am", false), "ማካው (ቻይና)");
    EXPECT_EQ(LocaleConfig::GetDisplayRegion("PS", "am", false), "ፍልስጤም");
    EXPECT_EQ(LocaleConfig::GetDisplayRegion("QO", "am", false), "ኦሺኒያ ዳርቻ");
    EXPECT_EQ(LocaleConfig::GetDisplayRegion("TA", "am", false), "ትሪስታን ዲ ኩንያ");
    EXPECT_EQ(LocaleConfig::GetDisplayRegion("TJ", "am", false), "ታጂኪስታን");
    EXPECT_EQ(LocaleConfig::GetDisplayRegion("TW", "am", false), "ታይዋን (ቻይና)");
    EXPECT_EQ(LocaleConfig::GetDisplayRegion("AQ", "ar", false), "القارة القطبية الجنوبية");
    EXPECT_EQ(LocaleConfig::GetDisplayRegion("BV", "ar", false), "جزيرة بوفيت");
    EXPECT_EQ(LocaleConfig::GetDisplayRegion("CP", "ar", false), "جزيرة كليبرتون");
    EXPECT_EQ(LocaleConfig::GetDisplayRegion("HK", "ar", false), "هونغ كونغ (الصين)");
    EXPECT_EQ(LocaleConfig::GetDisplayRegion("IS", "ar", false), "أيسلندا");
    EXPECT_EQ(LocaleConfig::GetDisplayRegion("MO", "ar", false), "ماكاو (الصين)");
    EXPECT_EQ(LocaleConfig::GetDisplayRegion("PS", "ar", false), "فلسطين");
    EXPECT_EQ(LocaleConfig::GetDisplayRegion("TL", "ar", false), "تيمور الشرقية");
    EXPECT_EQ(LocaleConfig::GetDisplayRegion("TW", "ar", false), "تايوان (الصين)");
    EXPECT_EQ(LocaleConfig::GetDisplayRegion("BT", "as", false), "ভূটান");
    EXPECT_EQ(LocaleConfig::GetDisplayRegion("FK", "as", false), "ফকলেণ্ড দ্বীপপুঞ্জ (আইলেছ মালভিনাছ)");
    EXPECT_EQ(LocaleConfig::GetDisplayRegion("HK", "as", false), "হং কং (চীন)");
    EXPECT_EQ(LocaleConfig::GetDisplayRegion("KP", "as", false), "উত্তৰ কোৰিয়া");
    EXPECT_EQ(LocaleConfig::GetDisplayRegion("MK", "as", false), "উত্তৰ মেচিডোনিয়া");
    EXPECT_EQ(LocaleConfig::GetDisplayRegion("MO", "as", false), "মাকাও (চীন)");
    EXPECT_EQ(LocaleConfig::GetDisplayRegion("PS", "as", false), "পেলেষ্টাইন");
    EXPECT_EQ(LocaleConfig::GetDisplayRegion("TW", "as", false), "টাইৱান (চীন)");
    EXPECT_EQ(LocaleConfig::GetDisplayRegion("AQ", "az", false), "Antarktida");
    EXPECT_EQ(LocaleConfig::GetDisplayRegion("HK", "az", false), "Honkonq (Çin)");
    EXPECT_EQ(LocaleConfig::GetDisplayRegion("MO", "az", false), "Makao (Çin)");
    EXPECT_EQ(LocaleConfig::GetDisplayRegion("PS", "az", false), "Fələstin");
    EXPECT_EQ(LocaleConfig::GetDisplayRegion("TA", "az", false), "Tristan-da-Kunya");
    EXPECT_EQ(LocaleConfig::GetDisplayRegion("TW", "az", false), "Tayvan (Çin)");
    EXPECT_EQ(LocaleConfig::GetDisplayRegion("HK", "be", false), "Ганконг (Кітай)");
    EXPECT_EQ(LocaleConfig::GetDisplayRegion("MO", "be", false), "Макаа (Кітай)");
    EXPECT_EQ(LocaleConfig::GetDisplayRegion("PS", "be", false), "Палесціна");
    EXPECT_EQ(LocaleConfig::GetDisplayRegion("TW", "be", false), "Тайвань (Кітай)");
    EXPECT_EQ(LocaleConfig::GetDisplayRegion("HK", "bg", false), "Хонконг (Китай)");
    EXPECT_EQ(LocaleConfig::GetDisplayRegion("MO", "bg", false), "Макао (Китай)");
}

/**
 * @tc.name: CustomizedDataTest0013
 * @tc.desc: Test Customized Data
 * @tc.type: FUNC
 */
HWTEST_F(CustomizedDataTest, CustomizedDataTest0013, TestSize.Level1)
{
    EXPECT_EQ(LocaleConfig::GetDisplayRegion("PS", "bg", false), "Палестина");
    EXPECT_EQ(LocaleConfig::GetDisplayRegion("TW", "bg", false), "Тайван (Китай)");
    EXPECT_EQ(LocaleConfig::GetDisplayRegion("VA", "bg", false), "Ватиканска държава");
    EXPECT_EQ(LocaleConfig::GetDisplayRegion("AC", "bn", false), "অ্যাসসেনশন দ্বীপ");
    EXPECT_EQ(LocaleConfig::GetDisplayRegion("AL", "bn", false), "আলবেনিয়া");
    EXPECT_EQ(LocaleConfig::GetDisplayRegion("AM", "bn", false), "আর্মেনিয়া");
    EXPECT_EQ(LocaleConfig::GetDisplayRegion("BT", "bn", false), "ভূটান");
    EXPECT_EQ(LocaleConfig::GetDisplayRegion("EU", "bn", false), "ইউরোপীয় ইউনিয়ন");
    EXPECT_EQ(LocaleConfig::GetDisplayRegion("GS", "bn", false), "দক্ষিণ জর্জিয়া ও দক্ষিণ স্যান্ডউইচ দ্বীপপুঞ্জ");
    EXPECT_EQ(LocaleConfig::GetDisplayRegion("HK", "bn", false), "হংকং (চীন)");
    EXPECT_EQ(LocaleConfig::GetDisplayRegion("HM", "bn", false), "হার্ড ও ম্যাকডোনাল্ড দ্বীপপুঞ্জ");
    EXPECT_EQ(LocaleConfig::GetDisplayRegion("KP", "bn", false), "উত্তর কোরিয়া");
    EXPECT_EQ(LocaleConfig::GetDisplayRegion("MK", "bn", false), "উত্তর ম্যাসাডোনিয়া");
    EXPECT_EQ(LocaleConfig::GetDisplayRegion("MO", "bn", false), "ম্যাকাও (চীন)");
    EXPECT_EQ(LocaleConfig::GetDisplayRegion("PS", "bn", false), "ফিলিস্তিন");
    EXPECT_EQ(LocaleConfig::GetDisplayRegion("QO", "bn", false), "আউটলাইনিং ওসানিয়া");
    EXPECT_EQ(LocaleConfig::GetDisplayRegion("ST", "bn", false), "সাও টোমে ও প্রিনসিপে");
    EXPECT_EQ(LocaleConfig::GetDisplayRegion("TF", "bn", false), "ফরাসি দক্ষিণাঞ্চল");
    EXPECT_EQ(LocaleConfig::GetDisplayRegion("TL", "bn", false), "পূর্ব তিমুর");
    EXPECT_EQ(LocaleConfig::GetDisplayRegion("TW", "bn", false), "তাইওয়ান (চীন)");
    EXPECT_EQ(LocaleConfig::GetDisplayRegion("CN", "bo", false), "ཀྲུང་གོ།");
    EXPECT_EQ(LocaleConfig::GetDisplayRegion("DE", "bo", false), "འཇར་མན།");
    EXPECT_EQ(LocaleConfig::GetDisplayRegion("GB", "bo", false), "དབྱིན་ཇི།");
    EXPECT_EQ(LocaleConfig::GetDisplayRegion("IN", "bo", false), "ཧིན་རྡུ།");
    EXPECT_EQ(LocaleConfig::GetDisplayRegion("IT", "bo", false), "དབྱི་ཐ་ལི།");
    EXPECT_EQ(LocaleConfig::GetDisplayRegion("JP", "bo", false), "རི་པིན།");
    EXPECT_EQ(LocaleConfig::GetDisplayRegion("KR", "bo", false), "ཁོ་རེ་ཡ་ལྷོ་མ།");
    EXPECT_EQ(LocaleConfig::GetDisplayRegion("NP", "bo", false), "ནེ་པ་ལ།");
    EXPECT_EQ(LocaleConfig::GetDisplayRegion("RU", "bo", false), "ཨུ་རུ་སུ།");
    EXPECT_EQ(LocaleConfig::GetDisplayRegion("US", "bo", false), "ཨ་མེ་རི་ཁ།");
    EXPECT_EQ(LocaleConfig::GetDisplayRegion("ZZ", "bo", false), "རྒྱུས་མེད་ས་ཁུལ།");
    EXPECT_EQ(LocaleConfig::GetDisplayRegion("FK", "br", false), "Inizi Falkland (Inizi Maloù)");
    EXPECT_EQ(LocaleConfig::GetDisplayRegion("HK", "br", false), "Hong Kong");
    EXPECT_EQ(LocaleConfig::GetDisplayRegion("MK", "br", false), "Makedonia (RYKM)");
    EXPECT_EQ(LocaleConfig::GetDisplayRegion("MO", "br", false), "Macau");
    EXPECT_EQ(LocaleConfig::GetDisplayRegion("PS", "br", false), "Palestina");
    EXPECT_EQ(LocaleConfig::GetDisplayRegion("AQ", "bs", false), "Antarktik");
    EXPECT_EQ(LocaleConfig::GetDisplayRegion("BV", "bs", false), "Ostrvo Bouvet");
    EXPECT_EQ(LocaleConfig::GetDisplayRegion("CV", "bs", false), "Zelenortska Republika");
    EXPECT_EQ(LocaleConfig::GetDisplayRegion("FO", "bs", false), "Farska Ostrva");
    EXPECT_EQ(LocaleConfig::GetDisplayRegion("HK", "bs", false), "Hong Kong (Kina)");
    EXPECT_EQ(LocaleConfig::GetDisplayRegion("HM", "bs", false), "Ostrva Herd i McDonald");
    EXPECT_EQ(LocaleConfig::GetDisplayRegion("MO", "bs", false), "Makao (Kina)");
    EXPECT_EQ(LocaleConfig::GetDisplayRegion("PS", "bs", false), "Palestina");
    EXPECT_EQ(LocaleConfig::GetDisplayRegion("TR", "bs", false), "Türkiye");
    EXPECT_EQ(LocaleConfig::GetDisplayRegion("TW", "bs", false), "Tajvan (Kina)");
}

/**
 * @tc.name: CustomizedDataTest0014
 * @tc.desc: Test Customized Data
 * @tc.type: FUNC
 */
HWTEST_F(CustomizedDataTest, CustomizedDataTest0014, TestSize.Level1)
{
    EXPECT_EQ(LocaleConfig::GetDisplayRegion("AC", "ca", false), "Illa de l'Ascensió");
    EXPECT_EQ(LocaleConfig::GetDisplayRegion("HK", "ca", false), "Hong Kong (Xina)");
    EXPECT_EQ(LocaleConfig::GetDisplayRegion("MO", "ca", false), "Macau (Xina)");
    EXPECT_EQ(LocaleConfig::GetDisplayRegion("PS", "ca", false), "Palestina");
    EXPECT_EQ(LocaleConfig::GetDisplayRegion("QO", "ca", false), "Territoris allunyats d'Oceania");
    EXPECT_EQ(LocaleConfig::GetDisplayRegion("TW", "ca", false), "Taiwan (Xina)");
    EXPECT_EQ(LocaleConfig::GetDisplayRegion("VA", "ca", false), "Vaticà");
    EXPECT_EQ(LocaleConfig::GetDisplayRegion("FK", "ce", false), "Фолклендан гӀайренаш (Мальвинаш)");
    EXPECT_EQ(LocaleConfig::GetDisplayRegion("HK", "ce", false), "Гонконг");
    EXPECT_EQ(LocaleConfig::GetDisplayRegion("MO", "ce", false), "Макао");
    EXPECT_EQ(LocaleConfig::GetDisplayRegion("PS", "ce", false), "ПалестӀина");
    EXPECT_EQ(LocaleConfig::GetDisplayRegion("HK", "cs", false), "Hongkong (Čína)");
    EXPECT_EQ(LocaleConfig::GetDisplayRegion("MO", "cs", false), "Macao (Čína)");
    EXPECT_EQ(LocaleConfig::GetDisplayRegion("PS", "cs", false), "Palestina");
    EXPECT_EQ(LocaleConfig::GetDisplayRegion("TW", "cs", false), "Tchaj-wan (Čína)");
    EXPECT_EQ(LocaleConfig::GetDisplayRegion("FK", "cy", false), "Ynysoedd y Falkland (Ynysoedd y Malfinas)");
    EXPECT_EQ(LocaleConfig::GetDisplayRegion("HK", "cy", false), "Hong Kong");
    EXPECT_EQ(LocaleConfig::GetDisplayRegion("MK", "cy", false), "Macedonia (CWIM)");
    EXPECT_EQ(LocaleConfig::GetDisplayRegion("MO", "cy", false), "Macau");
    EXPECT_EQ(LocaleConfig::GetDisplayRegion("PS", "cy", false), "Palesteina");
    EXPECT_EQ(LocaleConfig::GetDisplayRegion("HK", "da", false), "Hongkong (Kina)");
    EXPECT_EQ(LocaleConfig::GetDisplayRegion("MO", "da", false), "Macao (Kina)");
    EXPECT_EQ(LocaleConfig::GetDisplayRegion("PS", "da", false), "Palæstina");
    EXPECT_EQ(LocaleConfig::GetDisplayRegion("TW", "da", false), "Taiwan (Kina)");
    EXPECT_EQ(LocaleConfig::GetDisplayRegion("HK", "de", false), "Hongkong (China)");
    EXPECT_EQ(LocaleConfig::GetDisplayRegion("MO", "de", false), "Macau (China)");
    EXPECT_EQ(LocaleConfig::GetDisplayRegion("PS", "de", false), "Palästina");
    EXPECT_EQ(LocaleConfig::GetDisplayRegion("TW", "de", false), "Taiwan (China)");
    EXPECT_EQ(LocaleConfig::GetDisplayRegion("FK", "dz", false), "ཕལྐ་ལནྜ་གླིང་ཚོམ (ཨིས་ལཱས་མལ་བི་ཎཱས)");
}

/**
 * @tc.name: CustomizedDataTest0015
 * @tc.desc: Test Customized Data
 * @tc.type: FUNC
 */
HWTEST_F(CustomizedDataTest, CustomizedDataTest0015, TestSize.Level1)
{
    EXPECT_EQ(LocaleConfig::GetDisplayRegion("HK", "dz", false), "ཧོང་ཀོང");
    EXPECT_EQ(LocaleConfig::GetDisplayRegion("MO", "dz", false), "མཀ་ཨའུ");
    EXPECT_EQ(LocaleConfig::GetDisplayRegion("FK", "ee", false), "Falkland ƒudomekpowo (Islas Malvinas) nutome");
    EXPECT_EQ(LocaleConfig::GetDisplayRegion("HK", "ee", false), "Hɔng Kɔng nutome");
    EXPECT_EQ(LocaleConfig::GetDisplayRegion("MO", "ee", false), "Macau nutome");
    EXPECT_EQ(LocaleConfig::GetDisplayRegion("HK", "el", false), "Χονγκ Κονγκ (Κίνα)");
    EXPECT_EQ(LocaleConfig::GetDisplayRegion("MO", "el", false), "Μακάο (Κίνα)");
    EXPECT_EQ(LocaleConfig::GetDisplayRegion("PS", "el", false), "Παλαιστίνη");
    EXPECT_EQ(LocaleConfig::GetDisplayRegion("TW", "el", false), "Ταϊβάν (Κίνα)");
    EXPECT_EQ(LocaleConfig::GetDisplayRegion("VA", "el", false), "Πόλη του Βατικανού");
    EXPECT_EQ(LocaleConfig::GetDisplayRegion("FK", "en", false), "Falkland Islands (Islas Malvinas)");
    EXPECT_EQ(LocaleConfig::GetDisplayRegion("HK", "en", false), "Hong Kong (China)");
    EXPECT_EQ(LocaleConfig::GetDisplayRegion("MO", "en", false), "Macao (China)");
    EXPECT_EQ(LocaleConfig::GetDisplayRegion("PS", "en", false), "Palestine");
    EXPECT_EQ(LocaleConfig::GetDisplayRegion("TW", "en", false), "Taiwan (China)");
    EXPECT_EQ(LocaleConfig::GetDisplayRegion("AC", "es", false), "Isla Ascensión");
    EXPECT_EQ(LocaleConfig::GetDisplayRegion("EH", "es", false), "Sahara Occidental");
    EXPECT_EQ(LocaleConfig::GetDisplayRegion("EZ", "es", false), "Zona del euro");
    EXPECT_EQ(LocaleConfig::GetDisplayRegion("GS", "es", false), "Islas Georgias del Sur y Sandwich del Sur");
    EXPECT_EQ(LocaleConfig::GetDisplayRegion("HK", "es", false), "Hong Kong (China)");
    EXPECT_EQ(LocaleConfig::GetDisplayRegion("MO", "es", false), "Macao (China)");
    EXPECT_EQ(LocaleConfig::GetDisplayRegion("PS", "es", false), "Palestina");
    EXPECT_EQ(LocaleConfig::GetDisplayRegion("TL", "es", false), "Timor Oriental");
    EXPECT_EQ(LocaleConfig::GetDisplayRegion("TW", "es", false), "Taiwán (China)");
    EXPECT_EQ(LocaleConfig::GetDisplayRegion("EZ", "es-419", false), "zona del euro");
    EXPECT_EQ(LocaleConfig::GetDisplayRegion("HK", "et", false), "Hongkong (Hiina)");
    EXPECT_EQ(LocaleConfig::GetDisplayRegion("MO", "et", false), "Macau (Hiina)");
    EXPECT_EQ(LocaleConfig::GetDisplayRegion("PS", "et", false), "Palestiina");
    EXPECT_EQ(LocaleConfig::GetDisplayRegion("SZ", "et", false), "eSwatini");
    EXPECT_EQ(LocaleConfig::GetDisplayRegion("TW", "et", false), "Taiwan (Hiina)");
    EXPECT_EQ(LocaleConfig::GetDisplayRegion("HK", "eu", false), "Hong Kong (Txina)");
    EXPECT_EQ(LocaleConfig::GetDisplayRegion("MO", "eu", false), "Macao (Txina)");
    EXPECT_EQ(LocaleConfig::GetDisplayRegion("PS", "eu", false), "Palestina");
    EXPECT_EQ(LocaleConfig::GetDisplayRegion("TJ", "eu", false), "Tadjikistan");
    EXPECT_EQ(LocaleConfig::GetDisplayRegion("TW", "eu", false), "Taiwan (Txina)");
    EXPECT_EQ(LocaleConfig::GetDisplayRegion("UN", "eu", false), "Nazio Batuen Erakundea");
    EXPECT_EQ(LocaleConfig::GetDisplayRegion("AC", "fa", false), "جزیرۀ اسنشن");
    EXPECT_EQ(LocaleConfig::GetDisplayRegion("CP", "fa", false), "جزیرۀ کلیپرتون");
    EXPECT_EQ(LocaleConfig::GetDisplayRegion("EZ", "fa", false), "منطقۀ یورو");
    EXPECT_EQ(LocaleConfig::GetDisplayRegion("HK", "fa", false), "هنگ‌کنگ (چین)");
    EXPECT_EQ(LocaleConfig::GetDisplayRegion("KP", "fa", false), "کرۀ شمالی");
    EXPECT_EQ(LocaleConfig::GetDisplayRegion("MK", "fa", false), "مقدونیۀ شمالی");
    EXPECT_EQ(LocaleConfig::GetDisplayRegion("MO", "fa", false), "ماکائو (چین)");
    EXPECT_EQ(LocaleConfig::GetDisplayRegion("PS", "fa", false), "فلسطین");
}

/**
 * @tc.name: CustomizedDataTest0016
 * @tc.desc: Test Customized Data
 * @tc.type: FUNC
 */
HWTEST_F(CustomizedDataTest, CustomizedDataTest0016, TestSize.Level1)
{
    EXPECT_EQ(LocaleConfig::GetDisplayRegion("TW", "fa", false), "تایوان (چین)");
    EXPECT_EQ(LocaleConfig::GetDisplayRegion("AQ", "fi", false), "Etelämanner");
    EXPECT_EQ(LocaleConfig::GetDisplayRegion("HK", "fi", false), "Hongkong (Kiina)");
    EXPECT_EQ(LocaleConfig::GetDisplayRegion("MO", "fi", false), "Macao (Kiina)");
    EXPECT_EQ(LocaleConfig::GetDisplayRegion("PS", "fi", false), "Palestiina");
    EXPECT_EQ(LocaleConfig::GetDisplayRegion("QO", "fi", false), "Oseanian ulkosaaret");
    EXPECT_EQ(LocaleConfig::GetDisplayRegion("TW", "fi", false), "Taiwan (Kiina)");
    EXPECT_EQ(LocaleConfig::GetDisplayRegion("AC", "fil", false), "Ascension Island");
    EXPECT_EQ(LocaleConfig::GetDisplayRegion("EH", "fil", false), "Western Sahara");
    EXPECT_EQ(LocaleConfig::GetDisplayRegion("HK", "fil", false), "Hong Kong (China)");
    EXPECT_EQ(LocaleConfig::GetDisplayRegion("KP", "fil", false), "North Korea");
    EXPECT_EQ(LocaleConfig::GetDisplayRegion("MO", "fil", false), "Macao (China)");
    EXPECT_EQ(LocaleConfig::GetDisplayRegion("PS", "fil", false), "Palestine");
    EXPECT_EQ(LocaleConfig::GetDisplayRegion("TW", "fil", false), "Taiwan (China)");
    EXPECT_EQ(LocaleConfig::GetDisplayRegion("FK", "fo", false), "Falklandsoyggjar (Islas Malvinas)");
    EXPECT_EQ(LocaleConfig::GetDisplayRegion("HK", "fo", false), "Hong Kong");
    EXPECT_EQ(LocaleConfig::GetDisplayRegion("MK", "fo", false), "Makedónia (FJM)");
    EXPECT_EQ(LocaleConfig::GetDisplayRegion("MO", "fo", false), "Makao");
    EXPECT_EQ(LocaleConfig::GetDisplayRegion("PS", "fo", false), "Palestina");
    EXPECT_EQ(LocaleConfig::GetDisplayRegion("EZ", "fr", false), "Zone euro");
    EXPECT_EQ(LocaleConfig::GetDisplayRegion("HK", "fr", false), "Hong Kong (Chine)");
    EXPECT_EQ(LocaleConfig::GetDisplayRegion("MO", "fr", false), "Macao (Chine)");
    EXPECT_EQ(LocaleConfig::GetDisplayRegion("PS", "fr", false), "Palestine");
    EXPECT_EQ(LocaleConfig::GetDisplayRegion("QO", "fr", false), "Région périphérique de l’Océanie");
    EXPECT_EQ(LocaleConfig::GetDisplayRegion("TW", "fr", false), "Taïwan (Chine)");
    EXPECT_EQ(LocaleConfig::GetDisplayRegion("VA", "fr", false), "Vatican");
    EXPECT_EQ(LocaleConfig::GetDisplayRegion("ZZ", "fr", false), "Région indéterminée");
    EXPECT_EQ(LocaleConfig::GetDisplayRegion("FK", "fr-CA", false), "îles Falkland (Malouines)");
    EXPECT_EQ(LocaleConfig::GetDisplayRegion("FK", "fur", false), "Isulis Falkland (Isulis Malvinas)");
    EXPECT_EQ(LocaleConfig::GetDisplayRegion("HK", "fur", false), "Hong Kong");
    EXPECT_EQ(LocaleConfig::GetDisplayRegion("MO", "fur", false), "Macao");
    EXPECT_EQ(LocaleConfig::GetDisplayRegion("FK", "ga", false), "Oileáin Fháclainne (Islas Malvinas)");
    EXPECT_EQ(LocaleConfig::GetDisplayRegion("HK", "ga", false), "Hong Cong");
    EXPECT_EQ(LocaleConfig::GetDisplayRegion("MK", "ga", false), "an Mhacadóin (PIIM)");
    EXPECT_EQ(LocaleConfig::GetDisplayRegion("MO", "ga", false), "Macao");
    EXPECT_EQ(LocaleConfig::GetDisplayRegion("PS", "ga", false), "an Phalaistín");
    EXPECT_EQ(LocaleConfig::GetDisplayRegion("FK", "gd", false), "Na h-Eileanan Fàclannach (Islas Malvinas)");
    EXPECT_EQ(LocaleConfig::GetDisplayRegion("HK", "gd", false), "Hong Kong");
    EXPECT_EQ(LocaleConfig::GetDisplayRegion("MK", "gd", false), "A’ Mhasadon (FYROM)");
    EXPECT_EQ(LocaleConfig::GetDisplayRegion("MO", "gd", false), "Macàthu");
    EXPECT_EQ(LocaleConfig::GetDisplayRegion("PS", "gd", false), "Palastain");
    EXPECT_EQ(LocaleConfig::GetDisplayRegion("AE", "gl", false), "Emiratos Árabes Unidos");
    EXPECT_EQ(LocaleConfig::GetDisplayRegion("AG", "gl", false), "Antiga e Barbuda");
    EXPECT_EQ(LocaleConfig::GetDisplayRegion("AQ", "gl", false), "Antártida");
    EXPECT_EQ(LocaleConfig::GetDisplayRegion("AR", "gl", false), "Arxentina");
    EXPECT_EQ(LocaleConfig::GetDisplayRegion("AX", "gl", false), "Illas Aland");
}

/**
 * @tc.name: CustomizedDataTest0017
 * @tc.desc: Test Customized Data
 * @tc.type: FUNC
 */
HWTEST_F(CustomizedDataTest, CustomizedDataTest0017, TestSize.Level1)
{
    EXPECT_EQ(LocaleConfig::GetDisplayRegion("BJ", "gl", false), "Benin");
    EXPECT_EQ(LocaleConfig::GetDisplayRegion("BL", "gl", false), "Saint-Barthélemy");
    EXPECT_EQ(LocaleConfig::GetDisplayRegion("BM", "gl", false), "Bermudas");
    EXPECT_EQ(LocaleConfig::GetDisplayRegion("BR", "gl", false), "Brasil");
    EXPECT_EQ(LocaleConfig::GetDisplayRegion("BY", "gl", false), "Bielorrusia");
    EXPECT_EQ(LocaleConfig::GetDisplayRegion("CA", "gl", false), "Canadá");
    EXPECT_EQ(LocaleConfig::GetDisplayRegion("CN", "gl", false), "China");
    EXPECT_EQ(LocaleConfig::GetDisplayRegion("CX", "gl", false), "Illa de Nadal");
    EXPECT_EQ(LocaleConfig::GetDisplayRegion("EH", "gl", false), "Sáhara Occidental");
    EXPECT_EQ(LocaleConfig::GetDisplayRegion("FJ", "gl", false), "Fidxi");
    EXPECT_EQ(LocaleConfig::GetDisplayRegion("FO", "gl", false), "Illas Faroe");
    EXPECT_EQ(LocaleConfig::GetDisplayRegion("GB", "gl", false), "Reino Unido");
    EXPECT_EQ(LocaleConfig::GetDisplayRegion("GW", "gl", false), "Guinea-Bissau");
    EXPECT_EQ(LocaleConfig::GetDisplayRegion("HK", "gl", false), "Hong Kong (China)");
    EXPECT_EQ(LocaleConfig::GetDisplayRegion("IN", "gl", false), "India");
    EXPECT_EQ(LocaleConfig::GetDisplayRegion("JP", "gl", false), "Xapón");
    EXPECT_EQ(LocaleConfig::GetDisplayRegion("KH", "gl", false), "Cambodja");
    EXPECT_EQ(LocaleConfig::GetDisplayRegion("LB", "gl", false), "Líbano");
    EXPECT_EQ(LocaleConfig::GetDisplayRegion("LS", "gl", false), "Lesoto");
    EXPECT_EQ(LocaleConfig::GetDisplayRegion("MF", "gl", false), "Saint-Martin");
    EXPECT_EQ(LocaleConfig::GetDisplayRegion("MO", "gl", false), "Macau (China)");
    EXPECT_EQ(LocaleConfig::GetDisplayRegion("PE", "gl", false), "Perú");
    EXPECT_EQ(LocaleConfig::GetDisplayRegion("PF", "gl", false), "Polinesia Francesa");
    EXPECT_EQ(LocaleConfig::GetDisplayRegion("PM", "gl", false), "Saint-Pierre-et-Miquelon");
    EXPECT_EQ(LocaleConfig::GetDisplayRegion("PS", "gl", false), "Palestina");
    EXPECT_EQ(LocaleConfig::GetDisplayRegion("PY", "gl", false), "Paraguai");
    EXPECT_EQ(LocaleConfig::GetDisplayRegion("SD", "gl", false), "Sudán");
    EXPECT_EQ(LocaleConfig::GetDisplayRegion("SS", "gl", false), "Sudán do Sur");
    EXPECT_EQ(LocaleConfig::GetDisplayRegion("TW", "gl", false), "Taiwán (China)");
    EXPECT_EQ(LocaleConfig::GetDisplayRegion("UM", "gl", false), "Illas Ultramarinas dos EUA");
    EXPECT_EQ(LocaleConfig::GetDisplayRegion("US", "gl", false), "Estados Unidos de América");
    EXPECT_EQ(LocaleConfig::GetDisplayRegion("UY", "gl", false), "Uruguai");
    EXPECT_EQ(LocaleConfig::GetDisplayRegion("YE", "gl", false), "Iemen");
    EXPECT_EQ(LocaleConfig::GetDisplayRegion("BT", "gu", false), "ભુટાન");
    EXPECT_EQ(LocaleConfig::GetDisplayRegion("CP", "gu", false), "ક્લિપર્ટન આઇલેન્ડ");
    EXPECT_EQ(LocaleConfig::GetDisplayRegion("FO", "gu", false), "ફૅરો આઇલેન્ડ");
    EXPECT_EQ(LocaleConfig::GetDisplayRegion("HK", "gu", false), "હોંગ કોંગ (ચાઇના)");
    EXPECT_EQ(LocaleConfig::GetDisplayRegion("IR", "gu", false), "ઇરાન");
    EXPECT_EQ(LocaleConfig::GetDisplayRegion("MO", "gu", false), "મકાઉ (ચાઇના)");
    EXPECT_EQ(LocaleConfig::GetDisplayRegion("PS", "gu", false), "પેલેસ્ટાઇન");
    EXPECT_EQ(LocaleConfig::GetDisplayRegion("TJ", "gu", false), "તજાકિસ્તાન");
    EXPECT_EQ(LocaleConfig::GetDisplayRegion("TW", "gu", false), "તાઇવાન (ચાઇના)");
    EXPECT_EQ(LocaleConfig::GetDisplayRegion("HK", "he", false), "הונג קונג (סין)");
    EXPECT_EQ(LocaleConfig::GetDisplayRegion("MO", "he", false), "מקאו (סין)");
}

/**
 * @tc.name: CustomizedDataTest0018
 * @tc.desc: Test Customized Data
 * @tc.type: FUNC
 */
HWTEST_F(CustomizedDataTest, CustomizedDataTest0018, TestSize.Level1)
{
    EXPECT_EQ(LocaleConfig::GetDisplayRegion("PS", "he", false), "פלשתינה");
    EXPECT_EQ(LocaleConfig::GetDisplayRegion("QO", "he", false), "טריטוריות מרוחקות באוקיאניה");
    EXPECT_EQ(LocaleConfig::GetDisplayRegion("TW", "he", false), "טאיוואן (סין)");
    EXPECT_EQ(LocaleConfig::GetDisplayRegion("VA", "he", false), "קריית הוותיקן");
    EXPECT_EQ(LocaleConfig::GetDisplayRegion("FO", "hi", false), "फैरो आइलैंड्स");
    EXPECT_EQ(LocaleConfig::GetDisplayRegion("HK", "hi", false), "हांग कांग (चीन)");
    EXPECT_EQ(LocaleConfig::GetDisplayRegion("MK", "hi", false), "उत्तर मैसेडोनिया");
    EXPECT_EQ(LocaleConfig::GetDisplayRegion("MO", "hi", false), "मकाओ (चीन)");
    EXPECT_EQ(LocaleConfig::GetDisplayRegion("PS", "hi", false), "फ़िलीस्तीन");
    EXPECT_EQ(LocaleConfig::GetDisplayRegion("SZ", "hi", false), "स्वाज़ीलैंड");
    EXPECT_EQ(LocaleConfig::GetDisplayRegion("TW", "hi", false), "ताइवान (चीन)");
    EXPECT_EQ(LocaleConfig::GetDisplayRegion("VA", "hi", false), "वैटिकन सिटी");
    EXPECT_EQ(LocaleConfig::GetDisplayRegion("GS", "hr", false), "Južna Georgia i Otočje Južni Sendvič");
    EXPECT_EQ(LocaleConfig::GetDisplayRegion("HK", "hr", false), "Hong Kong (Kina)");
    EXPECT_EQ(LocaleConfig::GetDisplayRegion("MO", "hr", false), "Makao (Kina)");
    EXPECT_EQ(LocaleConfig::GetDisplayRegion("PS", "hr", false), "Palestina");
    EXPECT_EQ(LocaleConfig::GetDisplayRegion("ST", "hr", false), "Sveti Toma i Prinsipe");
    EXPECT_EQ(LocaleConfig::GetDisplayRegion("TW", "hr", false), "Tajvan (Kina)");
    EXPECT_EQ(LocaleConfig::GetDisplayRegion("VA", "hr", false), "Vatikan");
    EXPECT_EQ(LocaleConfig::GetDisplayRegion("HK", "hu", false), "Hongkong (Kína)");
    EXPECT_EQ(LocaleConfig::GetDisplayRegion("MO", "hu", false), "Makaó (Kína)");
    EXPECT_EQ(LocaleConfig::GetDisplayRegion("PS", "hu", false), "Palesztina");
    EXPECT_EQ(LocaleConfig::GetDisplayRegion("TW", "hu", false), "Tajvan (Kína)");
    EXPECT_EQ(LocaleConfig::GetDisplayRegion("MK", "ia", false), "Macedonia (ARYM)");
    EXPECT_EQ(LocaleConfig::GetDisplayRegion("HK", "id", false), "Hong Kong (Tiongkok)");
    EXPECT_EQ(LocaleConfig::GetDisplayRegion("MO", "id", false), "Makau (Tiongkok)");
    EXPECT_EQ(LocaleConfig::GetDisplayRegion("PS", "id", false), "Palestina");
    EXPECT_EQ(LocaleConfig::GetDisplayRegion("TW", "id", false), "Taiwan (Tiongkok)");
    EXPECT_EQ(LocaleConfig::GetDisplayRegion("FO", "it", false), "Isole Faroe");
    EXPECT_EQ(LocaleConfig::GetDisplayRegion("HK", "it", false), "Hong Kong (Cina)");
    EXPECT_EQ(LocaleConfig::GetDisplayRegion("MO", "it", false), "Macao (Cina)");
    EXPECT_EQ(LocaleConfig::GetDisplayRegion("PS", "it", false), "Palestina");
    EXPECT_EQ(LocaleConfig::GetDisplayRegion("TW", "it", false), "Taiwan (Cina)");
    EXPECT_EQ(LocaleConfig::GetDisplayRegion("HK", "ja", false), "香港（中国）");
    EXPECT_EQ(LocaleConfig::GetDisplayRegion("KP", "ja", false), "朝鮮民主主義人民共和国");
    EXPECT_EQ(LocaleConfig::GetDisplayRegion("MO", "ja", false), "マカオ（中国）");
    EXPECT_EQ(LocaleConfig::GetDisplayRegion("PS", "ja", false), "パレスチナ");
    EXPECT_EQ(LocaleConfig::GetDisplayRegion("TA", "ja", false), "トリスタンダクーニャ");
    EXPECT_EQ(LocaleConfig::GetDisplayRegion("TW", "ja", false), "台湾（中国）");
    EXPECT_EQ(LocaleConfig::GetDisplayRegion("AL", "jv", false), "Albania");
}

/**
 * @tc.name: CustomizedDataTest0019
 * @tc.desc: Test Customized Data
 * @tc.type: FUNC
 */
HWTEST_F(CustomizedDataTest, CustomizedDataTest0019, TestSize.Level1)
{
    EXPECT_EQ(LocaleConfig::GetDisplayRegion("CN", "jv", false), "Tiongkok");
    EXPECT_EQ(LocaleConfig::GetDisplayRegion("EU", "jv", false), "Uni Éropa");
    EXPECT_EQ(LocaleConfig::GetDisplayRegion("FO", "jv", false), "Kapuloan Faroe");
    EXPECT_EQ(LocaleConfig::GetDisplayRegion("GS", "jv", false), "Georgia Selatan lan Kapuloan Sandwich Selatan");
    EXPECT_EQ(LocaleConfig::GetDisplayRegion("HK", "jv", false), "Hong Kong (Tiongkok)");
    EXPECT_EQ(LocaleConfig::GetDisplayRegion("HM", "jv", false), "Pulo Heard & Kepuloan McDonald");
    EXPECT_EQ(LocaleConfig::GetDisplayRegion("IN", "jv", false), "Indhi");
    EXPECT_EQ(LocaleConfig::GetDisplayRegion("IS", "jv", false), "Islandia");
    EXPECT_EQ(LocaleConfig::GetDisplayRegion("KP", "jv", false), "Korea Utara");
    EXPECT_EQ(LocaleConfig::GetDisplayRegion("MK", "jv", false), "Makedonia Utara");
    EXPECT_EQ(LocaleConfig::GetDisplayRegion("MO", "jv", false), "Macao (Tiongkok)");
    EXPECT_EQ(LocaleConfig::GetDisplayRegion("MZ", "jv", false), "Mozambik");
    EXPECT_EQ(LocaleConfig::GetDisplayRegion("NZ", "jv", false), "Niu Sélan");
    EXPECT_EQ(LocaleConfig::GetDisplayRegion("PS", "jv", false), "Palestina");
    EXPECT_EQ(LocaleConfig::GetDisplayRegion("TF", "jv", false), "Wilayah Prancis Selatan");
    EXPECT_EQ(LocaleConfig::GetDisplayRegion("TW", "jv", false), "Taiwan (Tiongkok)");
    EXPECT_EQ(LocaleConfig::GetDisplayRegion("UN", "jv", false), "Perserikatan Bangsa-Bangsa");
    EXPECT_EQ(LocaleConfig::GetDisplayRegion("VA", "jv", false), "Vatikan");
    EXPECT_EQ(LocaleConfig::GetDisplayRegion("AQ", "ka", false), "ანტარქტიდა");
    EXPECT_EQ(LocaleConfig::GetDisplayRegion("EH", "ka", false), "დასავლეთი საჰარა");
    EXPECT_EQ(LocaleConfig::GetDisplayRegion("GS", "ka", false), "სამხრეთი გეორგია და სამხრეთ სენდვიჩის კუნძულები");
    EXPECT_EQ(LocaleConfig::GetDisplayRegion("HK", "ka", false), "ჰონკონგი (ჩინეთი)");
    EXPECT_EQ(LocaleConfig::GetDisplayRegion("KP", "ka", false), "ჩრდილოეთი კორეა");
    EXPECT_EQ(LocaleConfig::GetDisplayRegion("MK", "ka", false), "ჩრდილოეთი მაკედონია");
    EXPECT_EQ(LocaleConfig::GetDisplayRegion("MO", "ka", false), "მაკაო (ჩინეთი)");
    EXPECT_EQ(LocaleConfig::GetDisplayRegion("MV", "ka", false), "მალდივის რესპუბლიკა");
    EXPECT_EQ(LocaleConfig::GetDisplayRegion("PS", "ka", false), "პალესტინა");
    EXPECT_EQ(LocaleConfig::GetDisplayRegion("SZ", "ka", false), "ესვატინი");
    EXPECT_EQ(LocaleConfig::GetDisplayRegion("TA", "ka", false), "ტრისტანი-და-კუნია");
    EXPECT_EQ(LocaleConfig::GetDisplayRegion("TF", "ka", false), "საფრანგეთის სამხრეთული ტერიტორიები");
    EXPECT_EQ(LocaleConfig::GetDisplayRegion("TL", "ka", false), "აღმოსავლეთი ტიმორი");
    EXPECT_EQ(LocaleConfig::GetDisplayRegion("TW", "ka", false), "ტაივანი (ჩინეთი)");
    EXPECT_EQ(LocaleConfig::GetDisplayRegion("VA", "ka", false), "ვატიკანის ქალაქ-სახელმწიფო");
    EXPECT_EQ(LocaleConfig::GetDisplayRegion("HK", "kk", false), "Гонконг (Қытай)");
    EXPECT_EQ(LocaleConfig::GetDisplayRegion("MO", "kk", false), "Макао (Қытай)");
    EXPECT_EQ(LocaleConfig::GetDisplayRegion("PS", "kk", false), "Палестина");
    EXPECT_EQ(LocaleConfig::GetDisplayRegion("SZ", "kk", false), "Свазиленд");
    EXPECT_EQ(LocaleConfig::GetDisplayRegion("TW", "kk", false), "Тайвань (Қытай)");
    EXPECT_EQ(LocaleConfig::GetDisplayRegion("VA", "kk", false), "Ватикан қаласы");
    EXPECT_EQ(LocaleConfig::GetDisplayRegion("FO", "km", false), "កោះហ្វារ៉ូ");
    EXPECT_EQ(LocaleConfig::GetDisplayRegion("HK", "km", false), "ហុងកុង (ប្រទេសចិន)");
    EXPECT_EQ(LocaleConfig::GetDisplayRegion("KP", "km", false), "កូរ៉េខាងជើង");
}

/**
 * @tc.name: CustomizedDataTest0020
 * @tc.desc: Test Customized Data
 * @tc.type: FUNC
 */
HWTEST_F(CustomizedDataTest, CustomizedDataTest0020, TestSize.Level1)
{
    EXPECT_EQ(LocaleConfig::GetDisplayRegion("MO", "km", false), "ម៉ាកាវ (ប្រទេសចិន)");
    EXPECT_EQ(LocaleConfig::GetDisplayRegion("PS", "km", false), "ប៉ាឡេស្ទីន");
    EXPECT_EQ(LocaleConfig::GetDisplayRegion("SZ", "km", false), "អេស្វាទីនី");
    EXPECT_EQ(LocaleConfig::GetDisplayRegion("TL", "km", false), "ទីម័រខាងកើត");
    EXPECT_EQ(LocaleConfig::GetDisplayRegion("TW", "km", false), "តៃវ៉ាន់ (ប្រទេសចិន)");
    EXPECT_EQ(LocaleConfig::GetDisplayRegion("VA", "km", false), "បុរីវ៉ាទីកង់");
    EXPECT_EQ(LocaleConfig::GetDisplayRegion("HK", "kn", false), "ಹಾಂಗ್ ಕಾಂಗ್ (ಚೀನಾ)");
    EXPECT_EQ(LocaleConfig::GetDisplayRegion("MK", "kn", false), "ಉತ್ತರ ಮ್ಯಾಸೆಡೋನಿಯಾ");
    EXPECT_EQ(LocaleConfig::GetDisplayRegion("MO", "kn", false), "ಮಕಾವ್ (ಚೀನಾ)");
    EXPECT_EQ(LocaleConfig::GetDisplayRegion("PS", "kn", false), "ಪ್ಯಾಲೆಸ್ಟೈನ್");
    EXPECT_EQ(LocaleConfig::GetDisplayRegion("QO", "kn", false), "ಔಟ್‌ಲೈಯಿಂಗ್ ಓಷಿಯಾನಿಯಾ");
    EXPECT_EQ(LocaleConfig::GetDisplayRegion("TJ", "kn", false), "ತಜಾಕಿಸ್ಥಾನ್");
    EXPECT_EQ(LocaleConfig::GetDisplayRegion("TL", "kn", false), "ತಿಮೋರ್-ಲೆಸ್ತೆ");
    EXPECT_EQ(LocaleConfig::GetDisplayRegion("TM", "kn", false), "ತುರ್ಕ್‌ಮೇನಿಸ್ತಾನ");
    EXPECT_EQ(LocaleConfig::GetDisplayRegion("TW", "kn", false), "ತೈವಾನ್ (ಚೀನಾ)");
    EXPECT_EQ(LocaleConfig::GetDisplayRegion("UN", "kn", false), "ಯುನೈಟೆಡ್ ನೇಶನ್ಸ್");
    EXPECT_EQ(LocaleConfig::GetDisplayRegion("HK", "ko", false), "홍콩(중국)");
    EXPECT_EQ(LocaleConfig::GetDisplayRegion("MO", "ko", false), "마카오(중국)");
    EXPECT_EQ(LocaleConfig::GetDisplayRegion("PS", "ko", false), "팔레스타인");
    EXPECT_EQ(LocaleConfig::GetDisplayRegion("TF", "ko", false), "프랑스령 남부 지역");
    EXPECT_EQ(LocaleConfig::GetDisplayRegion("TW", "ko", false), "대만(중국)");
    EXPECT_EQ(LocaleConfig::GetDisplayRegion("UN", "ko", false), "국제 연합");
    EXPECT_EQ(LocaleConfig::GetDisplayRegion("VA", "ko", false), "바티칸");
    EXPECT_EQ(LocaleConfig::GetDisplayRegion("AC", "lo", false), "ເກາະ​ອາ​ສ​ເຊັນ​ຊັນ");
    EXPECT_EQ(LocaleConfig::GetDisplayRegion("AF", "lo", false), "ອັບ​ກາ​ນິດ​ສະ​ຖານ");
    EXPECT_EQ(LocaleConfig::GetDisplayRegion("AL", "lo", false), "ອານ​ບາ​ນີ");
    EXPECT_EQ(LocaleConfig::GetDisplayRegion("AM", "lo", false), "ອາກ​ເມ​ນີ");
    EXPECT_EQ(LocaleConfig::GetDisplayRegion("AO", "lo", false), "ອັງ​ໂກ​ລາ");
    EXPECT_EQ(LocaleConfig::GetDisplayRegion("AQ", "lo", false), "ແອນ​ຕາກ​ຕິ​ກາ");
    EXPECT_EQ(LocaleConfig::GetDisplayRegion("BT", "lo", false), "ບູຕານ");
}

/**
 * @tc.name: CustomizedDataTest0021
 * @tc.desc: Test Customized Data
 * @tc.type: FUNC
 */
HWTEST_F(CustomizedDataTest, CustomizedDataTest0021, TestSize.Level1)
{
    EXPECT_EQ(LocaleConfig::GetDisplayRegion("EU", "lo", false), "ສະ​ຫະ​ພາບ​ເອີ​ຣົບ");
    EXPECT_EQ(LocaleConfig::GetDisplayRegion("FO", "lo", false), "ໝູ່ເກາະຟາ​ໂຣ");
    EXPECT_EQ(LocaleConfig::GetDisplayRegion("HK", "lo", false), "ຮ່ອງ​ກົງ (ຈີນ)");
    EXPECT_EQ(LocaleConfig::GetDisplayRegion("HM", "lo", false), "ໝູ່ເກາະເຮີດ & ແມັກໂດນາ​ລ");
    EXPECT_EQ(LocaleConfig::GetDisplayRegion("IR", "lo", false), "ອີ​ຣ່ານ");
    EXPECT_EQ(LocaleConfig::GetDisplayRegion("KP", "lo", false), "ເກົາ​ຫຼີ​ເໜືອ");
    EXPECT_EQ(LocaleConfig::GetDisplayRegion("MK", "lo", false), "ມາເຊໂດເນຍເໜືອ");
    EXPECT_EQ(LocaleConfig::GetDisplayRegion("MO", "lo", false), "ມາ​ເກົ້າ (ຈີນ)");
    EXPECT_EQ(LocaleConfig::GetDisplayRegion("MV", "lo", false), "ມາ​ລ​ດິ​ບ​");
    EXPECT_EQ(LocaleConfig::GetDisplayRegion("PS", "lo", false), "ປາ​ເລັສ​ຕິນ");
    EXPECT_EQ(LocaleConfig::GetDisplayRegion("ST", "lo", false), "ເຊົາ​ໂຕ​ເມະ ແລະ ປ​ຣິນ​ຊິ​ເປະ");
    EXPECT_EQ(LocaleConfig::GetDisplayRegion("TJ", "lo", false), "ທາ​ຈິ​ກິດ​ສະ​ຖານ");
    EXPECT_EQ(LocaleConfig::GetDisplayRegion("TL", "lo", false), "ຕິ​ມໍ-ເລ​ສ​ເຕ");
    EXPECT_EQ(LocaleConfig::GetDisplayRegion("TM", "lo", false), "ເຕີກ​ມີ​ນິ​ດ​ສະ​ຖານ");
    EXPECT_EQ(LocaleConfig::GetDisplayRegion("TW", "lo", false), "ໄຕ້​ຫວັນ (ຈີນ)");
    EXPECT_EQ(LocaleConfig::GetDisplayRegion("VA", "lo", false), "ເມືອງວາ​ຕິ​ກັ່ງ");
    EXPECT_EQ(LocaleConfig::GetDisplayRegion("HK", "lt", false), "Honkongas (Kinija)");
    EXPECT_EQ(LocaleConfig::GetDisplayRegion("MO", "lt", false), "Makao (Kinija)");
    EXPECT_EQ(LocaleConfig::GetDisplayRegion("PS", "lt", false), "Palestina");
    EXPECT_EQ(LocaleConfig::GetDisplayRegion("TW", "lt", false), "Taivanas (Kinija)");
    EXPECT_EQ(LocaleConfig::GetDisplayRegion("VA", "lt", false), "Vatikanas");
    EXPECT_EQ(LocaleConfig::GetDisplayRegion("HK", "lv", false), "Honkonga (Ķīna)");
    EXPECT_EQ(LocaleConfig::GetDisplayRegion("MO", "lv", false), "Makao (Ķīna)");
    EXPECT_EQ(LocaleConfig::GetDisplayRegion("MV", "lv", false), "Maldīvu salas");
    EXPECT_EQ(LocaleConfig::GetDisplayRegion("PS", "lv", false), "Palestīna");
    EXPECT_EQ(LocaleConfig::GetDisplayRegion("TW", "lv", false), "Taivāna (Ķīna)");
    EXPECT_EQ(LocaleConfig::GetDisplayRegion("HK", "mai", false), "हांगकांग (चीन)");
    EXPECT_EQ(LocaleConfig::GetDisplayRegion("MO", "mai", false), "मकाउ (चीन)");
    EXPECT_EQ(LocaleConfig::GetDisplayRegion("PS", "mai", false), "फिलिस्तीन");
    EXPECT_EQ(LocaleConfig::GetDisplayRegion("TW", "mai", false), "ताइवान (चीन)");
    EXPECT_EQ(LocaleConfig::GetDisplayRegion("AC", "mi", false), "Te Motu Aupikinga");
    EXPECT_EQ(LocaleConfig::GetDisplayRegion("AE", "mi", false), "Te Kotahitanga o Ngā Ariki Arāpia");
    EXPECT_EQ(LocaleConfig::GetDisplayRegion("AG", "mi", false), "Nehe me Papura");
    EXPECT_EQ(LocaleConfig::GetDisplayRegion("AI", "mi", false), "Ānakuira");
    EXPECT_EQ(LocaleConfig::GetDisplayRegion("AL", "mi", false), "Arapaina");
    EXPECT_EQ(LocaleConfig::GetDisplayRegion("AM", "mi", false), "Amēnia");
    EXPECT_EQ(LocaleConfig::GetDisplayRegion("AQ", "mi", false), "Te Tiri o Te Moana");
    EXPECT_EQ(LocaleConfig::GetDisplayRegion("AS", "mi", false), "Amerika Hāmoa");
    EXPECT_EQ(LocaleConfig::GetDisplayRegion("AT", "mi", false), "Atiria");
    EXPECT_EQ(LocaleConfig::GetDisplayRegion("AW", "mi", false), "Arupa");
    EXPECT_EQ(LocaleConfig::GetDisplayRegion("AX", "mi", false), "Ngā Motu Arana");
    EXPECT_EQ(LocaleConfig::GetDisplayRegion("AZ", "mi", false), "Ahapaiāna");
    EXPECT_EQ(LocaleConfig::GetDisplayRegion("BA", "mi", false), "Pōngia me Herekowina");
    EXPECT_EQ(LocaleConfig::GetDisplayRegion("BB", "mi", false), "Pāpatohe");
    EXPECT_EQ(LocaleConfig::GetDisplayRegion("BE", "mi", false), "Pehiamu");
}

/**
 * @tc.name: CustomizedDataTest0022
 * @tc.desc: Test Customized Data
 * @tc.type: FUNC
 */
HWTEST_F(CustomizedDataTest, CustomizedDataTest0022, TestSize.Level1)
{
    EXPECT_EQ(LocaleConfig::GetDisplayRegion("BF", "mi", false), "Pūkino Whaho");
    EXPECT_EQ(LocaleConfig::GetDisplayRegion("BJ", "mi", false), "Pēnina");
    EXPECT_EQ(LocaleConfig::GetDisplayRegion("BN", "mi", false), "Purunei");
    EXPECT_EQ(LocaleConfig::GetDisplayRegion("BQ", "mi", false), "Karapiana Hōrana");
    EXPECT_EQ(LocaleConfig::GetDisplayRegion("BS", "mi", false), "Pāhama");
    EXPECT_EQ(LocaleConfig::GetDisplayRegion("BT", "mi", false), "Putāna");
    EXPECT_EQ(LocaleConfig::GetDisplayRegion("BV", "mi", false), "Te Motu Pauhei");
    EXPECT_EQ(LocaleConfig::GetDisplayRegion("BW", "mi", false), "Potiwana");
    EXPECT_EQ(LocaleConfig::GetDisplayRegion("BY", "mi", false), "Peraruha");
    EXPECT_EQ(LocaleConfig::GetDisplayRegion("BZ", "mi", false), "Pērihi");
    EXPECT_EQ(LocaleConfig::GetDisplayRegion("CC", "mi", false), "Ngā Motu o Koko (Kīrini)");
    EXPECT_EQ(LocaleConfig::GetDisplayRegion("CD", "mi", false), "Te Whenua Manapori o Kongo");
    EXPECT_EQ(LocaleConfig::GetDisplayRegion("CF", "mi", false), "Te Whenua o Awherika Pū");
    EXPECT_EQ(LocaleConfig::GetDisplayRegion("CG", "mi", false), "Te Whenua o Kongo");
    EXPECT_EQ(LocaleConfig::GetDisplayRegion("CI", "mi", false), "Tai Rei");
    EXPECT_EQ(LocaleConfig::GetDisplayRegion("CL", "mi", false), "Hire");
    EXPECT_EQ(LocaleConfig::GetDisplayRegion("CM", "mi", false), "Kamerūna");
    EXPECT_EQ(LocaleConfig::GetDisplayRegion("CO", "mi", false), "Korōmopia");
    EXPECT_EQ(LocaleConfig::GetDisplayRegion("CP", "mi", false), "Te Motu Kipatana");
    EXPECT_EQ(LocaleConfig::GetDisplayRegion("CR", "mi", false), "Koto Rika");
    EXPECT_EQ(LocaleConfig::GetDisplayRegion("CU", "mi", false), "Kupa");
    EXPECT_EQ(LocaleConfig::GetDisplayRegion("CV", "mi", false), "Kāpo Hēro");
    EXPECT_EQ(LocaleConfig::GetDisplayRegion("CX", "mi", false), "Motu Kirhimete");
    EXPECT_EQ(LocaleConfig::GetDisplayRegion("CZ", "mi", false), "Tekia");
    EXPECT_EQ(LocaleConfig::GetDisplayRegion("DG", "mi", false), "Tiēko Karahia");
    EXPECT_EQ(LocaleConfig::GetDisplayRegion("DJ", "mi", false), "Tīpūti");
    EXPECT_EQ(LocaleConfig::GetDisplayRegion("DK", "mi", false), "Tenemaka");
    EXPECT_EQ(LocaleConfig::GetDisplayRegion("DO", "mi", false), "Te Whenua o Tominika");
    EXPECT_EQ(LocaleConfig::GetDisplayRegion("EA", "mi", false), "Heuta me Merira");
    EXPECT_EQ(LocaleConfig::GetDisplayRegion("EG", "mi", false), "Ihipa");
    EXPECT_EQ(LocaleConfig::GetDisplayRegion("EH", "mi", false), "Tahara ki te Uru");
    EXPECT_EQ(LocaleConfig::GetDisplayRegion("ER", "mi", false), "Eritereia");
    EXPECT_EQ(LocaleConfig::GetDisplayRegion("ES", "mi", false), "Pāniora");
    EXPECT_EQ(LocaleConfig::GetDisplayRegion("EU", "mi", false), "Uniana o Ūropi");
    EXPECT_EQ(LocaleConfig::GetDisplayRegion("EZ", "mi", false), "Rohe Uro");
    EXPECT_EQ(LocaleConfig::GetDisplayRegion("FI", "mi", false), "Hinerangi");
    EXPECT_EQ(LocaleConfig::GetDisplayRegion("FK", "mi", false), "Ngā Motu o Whokarangi (Islas Malvinas)");
    EXPECT_EQ(LocaleConfig::GetDisplayRegion("FM", "mi", false), "Ngā Whenua Tōpū o Maikoronihia");
    EXPECT_EQ(LocaleConfig::GetDisplayRegion("FO", "mi", false), "Ngā Motu o Wharo");
    EXPECT_EQ(LocaleConfig::GetDisplayRegion("GA", "mi", false), "Kaponi");
    EXPECT_EQ(LocaleConfig::GetDisplayRegion("GD", "mi", false), "Kerenāra");
    EXPECT_EQ(LocaleConfig::GetDisplayRegion("GF", "mi", false), "Kiana o Wiwi");
    EXPECT_EQ(LocaleConfig::GetDisplayRegion("GL", "mi", false), "Kirīnirangi");
    EXPECT_EQ(LocaleConfig::GetDisplayRegion("GM", "mi", false), "Kamipia");
    EXPECT_EQ(LocaleConfig::GetDisplayRegion("GP", "mi", false), "Kuarerupe");
    EXPECT_EQ(LocaleConfig::GetDisplayRegion("GQ", "mi", false), "Kini ki te Pae o te Ao");
}

/**
 * @tc.name: CustomizedDataTest0023
 * @tc.desc: Test Customized Data
 * @tc.type: FUNC
 */
HWTEST_F(CustomizedDataTest, CustomizedDataTest0023, TestSize.Level1)
{
    EXPECT_EQ(LocaleConfig::GetDisplayRegion("GR", "mi", false), "Karihi");
    EXPECT_EQ(LocaleConfig::GetDisplayRegion("GS", "mi", false), "Ngā Motu o Hōria me Hanawiti ki te Tonga");
    EXPECT_EQ(LocaleConfig::GetDisplayRegion("GT", "mi", false), "Kuatemāra");
    EXPECT_EQ(LocaleConfig::GetDisplayRegion("GU", "mi", false), "Kuamu");
    EXPECT_EQ(LocaleConfig::GetDisplayRegion("GW", "mi", false), "Kini-Pihau");
    EXPECT_EQ(LocaleConfig::GetDisplayRegion("HK", "mi", false), "Hongipua (Haina)");
    EXPECT_EQ(LocaleConfig::GetDisplayRegion("HM", "mi", false), "Ngā Motu o Hāra me Makitanara");
    EXPECT_EQ(LocaleConfig::GetDisplayRegion("IC", "mi", false), "Ngā Motu Pōpokotea");
    EXPECT_EQ(LocaleConfig::GetDisplayRegion("IE", "mi", false), "Airana");
    EXPECT_EQ(LocaleConfig::GetDisplayRegion("IM", "mi", false), "Motu o Tāne");
    EXPECT_EQ(LocaleConfig::GetDisplayRegion("IO", "mi", false), "Rohe Peretania o te Moana Iniana");
    EXPECT_EQ(LocaleConfig::GetDisplayRegion("IQ", "mi", false), "Iraka");
    EXPECT_EQ(LocaleConfig::GetDisplayRegion("IT", "mi", false), "Itari");
    EXPECT_EQ(LocaleConfig::GetDisplayRegion("JE", "mi", false), "Hēhi");
    EXPECT_EQ(LocaleConfig::GetDisplayRegion("JM", "mi", false), "Hamaika");
    EXPECT_EQ(LocaleConfig::GetDisplayRegion("JO", "mi", false), "Horano");
    EXPECT_EQ(LocaleConfig::GetDisplayRegion("JP", "mi", false), "Hapāna");
    EXPECT_EQ(LocaleConfig::GetDisplayRegion("KG", "mi", false), "Kehitāna");
    EXPECT_EQ(LocaleConfig::GetDisplayRegion("KH", "mi", false), "Kamopōtia");
    EXPECT_EQ(LocaleConfig::GetDisplayRegion("KM", "mi", false), "Komorohi");
    EXPECT_EQ(LocaleConfig::GetDisplayRegion("KN", "mi", false), "Hato Kiti me Newihi");
    EXPECT_EQ(LocaleConfig::GetDisplayRegion("KR", "mi", false), "Kōria ki te Tonga");
    EXPECT_EQ(LocaleConfig::GetDisplayRegion("KW", "mi", false), "Kuweiti");
    EXPECT_EQ(LocaleConfig::GetDisplayRegion("KY", "mi", false), "Ngā Moutere Keimana");
    EXPECT_EQ(LocaleConfig::GetDisplayRegion("KZ", "mi", false), "Katatāna");
    EXPECT_EQ(LocaleConfig::GetDisplayRegion("LC", "mi", false), "Hato Ruha");
    EXPECT_EQ(LocaleConfig::GetDisplayRegion("LI", "mi", false), "Rikitaina");
    EXPECT_EQ(LocaleConfig::GetDisplayRegion("LT", "mi", false), "Rehuānia");
    EXPECT_EQ(LocaleConfig::GetDisplayRegion("LY", "mi", false), "Rīpia");
    EXPECT_EQ(LocaleConfig::GetDisplayRegion("MA", "mi", false), "Marako");
    EXPECT_EQ(LocaleConfig::GetDisplayRegion("MC", "mi", false), "Monako");
    EXPECT_EQ(LocaleConfig::GetDisplayRegion("MD", "mi", false), "Morotouwa");
    EXPECT_EQ(LocaleConfig::GetDisplayRegion("ME", "mi", false), "Monotenēro");
    EXPECT_EQ(LocaleConfig::GetDisplayRegion("MG", "mi", false), "Marakāhaka");
    EXPECT_EQ(LocaleConfig::GetDisplayRegion("MH", "mi", false), "Ngā Moutere Māhara");
    EXPECT_EQ(LocaleConfig::GetDisplayRegion("MM", "mi", false), "Maiāma (Pēma)");
    EXPECT_EQ(LocaleConfig::GetDisplayRegion("MN", "mi", false), "Monokōria");
    EXPECT_EQ(LocaleConfig::GetDisplayRegion("MO", "mi", false), "Makao (Haina)");
    EXPECT_EQ(LocaleConfig::GetDisplayRegion("MP", "mi", false), "Ngā Motu Mariana");
    EXPECT_EQ(LocaleConfig::GetDisplayRegion("MS", "mi", false), "Manetera");
    EXPECT_EQ(LocaleConfig::GetDisplayRegion("MV", "mi", false), "Maratīwi");
    EXPECT_EQ(LocaleConfig::GetDisplayRegion("MX", "mi", false), "Mehiko");
    EXPECT_EQ(LocaleConfig::GetDisplayRegion("NC", "mi", false), "Kanaki");
    EXPECT_EQ(LocaleConfig::GetDisplayRegion("NG", "mi", false), "Naitīria");
    EXPECT_EQ(LocaleConfig::GetDisplayRegion("NI", "mi", false), "Nikarakua");
    EXPECT_EQ(LocaleConfig::GetDisplayRegion("NP", "mi", false), "Neporo");
}

/**
 * @tc.name: CustomizedDataTest0024
 * @tc.desc: Test Customized Data
 * @tc.type: FUNC
 */
HWTEST_F(CustomizedDataTest, CustomizedDataTest0024, TestSize.Level1)
{
    EXPECT_EQ(LocaleConfig::GetDisplayRegion("OM", "mi", false), "Omana");
    EXPECT_EQ(LocaleConfig::GetDisplayRegion("PE", "mi", false), "Perū");
    EXPECT_EQ(LocaleConfig::GetDisplayRegion("PF", "mi", false), "Porinihia Wiwi");
    EXPECT_EQ(LocaleConfig::GetDisplayRegion("PG", "mi", false), "Papua Niu Kini");
    EXPECT_EQ(LocaleConfig::GetDisplayRegion("PM", "mi", false), "Hato Piere me Mikirone");
    EXPECT_EQ(LocaleConfig::GetDisplayRegion("PN", "mi", false), "Ngā Motu Pitikēne");
    EXPECT_EQ(LocaleConfig::GetDisplayRegion("PS", "mi", false), "Pirihitia");
    EXPECT_EQ(LocaleConfig::GetDisplayRegion("PW", "mi", false), "Pārao");
    EXPECT_EQ(LocaleConfig::GetDisplayRegion("QO", "mi", false), "Poronihia tawhiti");
    EXPECT_EQ(LocaleConfig::GetDisplayRegion("RE", "mi", false), "Reuniona");
    EXPECT_EQ(LocaleConfig::GetDisplayRegion("RO", "mi", false), "Romānia");
    EXPECT_EQ(LocaleConfig::GetDisplayRegion("RS", "mi", false), "Hēpia");
    EXPECT_EQ(LocaleConfig::GetDisplayRegion("RW", "mi", false), "Rawāna");
    EXPECT_EQ(LocaleConfig::GetDisplayRegion("SA", "mi", false), "Arapia");
    EXPECT_EQ(LocaleConfig::GetDisplayRegion("SD", "mi", false), "Hutāne");
    EXPECT_EQ(LocaleConfig::GetDisplayRegion("SH", "mi", false), "Hato Herena");
    EXPECT_EQ(LocaleConfig::GetDisplayRegion("SJ", "mi", false), "Hapapana me Hano Maheni");
    EXPECT_EQ(LocaleConfig::GetDisplayRegion("SO", "mi", false), "Homāria");
    EXPECT_EQ(LocaleConfig::GetDisplayRegion("SR", "mi", false), "Hurinamo");
    EXPECT_EQ(LocaleConfig::GetDisplayRegion("SS", "mi", false), "Hutāne ki te Tonga");
    EXPECT_EQ(LocaleConfig::GetDisplayRegion("ST", "mi", false), "Hao Tome me Pirihipe");
    EXPECT_EQ(LocaleConfig::GetDisplayRegion("SV", "mi", false), "Etāwatoa");
    EXPECT_EQ(LocaleConfig::GetDisplayRegion("SX", "mi", false), "Hato Mātene");
    EXPECT_EQ(LocaleConfig::GetDisplayRegion("SZ", "mi", false), "Warirangi");
    EXPECT_EQ(LocaleConfig::GetDisplayRegion("TA", "mi", false), "Tiritana ta Kuna");
    EXPECT_EQ(LocaleConfig::GetDisplayRegion("TC", "mi", false), "Ngā Motu Koru-Kākoa");
    EXPECT_EQ(LocaleConfig::GetDisplayRegion("TF", "mi", false), "Ngā Rohe Wīwī ki te Tonga");
    EXPECT_EQ(LocaleConfig::GetDisplayRegion("TH", "mi", false), "Tairana");
    EXPECT_EQ(LocaleConfig::GetDisplayRegion("TJ", "mi", false), "Tahikitāna");
    EXPECT_EQ(LocaleConfig::GetDisplayRegion("TL", "mi", false), "Timorete");
    EXPECT_EQ(LocaleConfig::GetDisplayRegion("TN", "mi", false), "Tunuhia");
    EXPECT_EQ(LocaleConfig::GetDisplayRegion("TR", "mi", false), "Türkiye");
    EXPECT_EQ(LocaleConfig::GetDisplayRegion("TT", "mi", false), "Tirinaki-Tōpako");
    EXPECT_EQ(LocaleConfig::GetDisplayRegion("TW", "mi", false), "Taiwana (Haina)");
    EXPECT_EQ(LocaleConfig::GetDisplayRegion("TZ", "mi", false), "Tanahia");
    EXPECT_EQ(LocaleConfig::GetDisplayRegion("UA", "mi", false), "Ukareina");
    EXPECT_EQ(LocaleConfig::GetDisplayRegion("UM", "mi", false), "Ngā Moutere Iti Raungaiti o Te Hononga o Amerika");
    EXPECT_EQ(LocaleConfig::GetDisplayRegion("UN", "mi", false), "Rūnanga Whakakotahi i ngā Iwi o te Ao");
    EXPECT_EQ(LocaleConfig::GetDisplayRegion("US", "mi", false), "Amerika");
    EXPECT_EQ(LocaleConfig::GetDisplayRegion("UZ", "mi", false), "Uhipeketane");
    EXPECT_EQ(LocaleConfig::GetDisplayRegion("VA", "mi", false), "Tāone nō te Pōpa");
    EXPECT_EQ(LocaleConfig::GetDisplayRegion("VC", "mi", false), "Hato Wēneti me Ngā Kerenarīna");
    EXPECT_EQ(LocaleConfig::GetDisplayRegion("VG", "mi", false), "Ngā Moutere Puhi o Piritene");
    EXPECT_EQ(LocaleConfig::GetDisplayRegion("VI", "mi", false), "Ngā Moutere Puhi o Amerika");
    EXPECT_EQ(LocaleConfig::GetDisplayRegion("VU", "mi", false), "Whenuātū");
    EXPECT_EQ(LocaleConfig::GetDisplayRegion("WF", "mi", false), "Wārihi me Whutuna");
}

/**
 * @tc.name: CustomizedDataTest0025
 * @tc.desc: Test Customized Data
 * @tc.type: FUNC
 */
HWTEST_F(CustomizedDataTest, CustomizedDataTest0025, TestSize.Level1)
{
    EXPECT_EQ(LocaleConfig::GetDisplayRegion("XK", "mi", false), "Kōkoho");
    EXPECT_EQ(LocaleConfig::GetDisplayRegion("YE", "mi", false), "Ēmene");
    EXPECT_EQ(LocaleConfig::GetDisplayRegion("YT", "mi", false), "Maiote");
    EXPECT_EQ(LocaleConfig::GetDisplayRegion("ZA", "mi", false), "Awherika ki te Tonga");
    EXPECT_EQ(LocaleConfig::GetDisplayRegion("FK", "mk", false), "Фолкландски Острови (Малвински Острови)");
    EXPECT_EQ(LocaleConfig::GetDisplayRegion("HK", "mk", false), "Хонгконг (Кина)");
    EXPECT_EQ(LocaleConfig::GetDisplayRegion("MO", "mk", false), "Макао (Кина)");
    EXPECT_EQ(LocaleConfig::GetDisplayRegion("PS", "mk", false), "Палестина");
    EXPECT_EQ(LocaleConfig::GetDisplayRegion("SZ", "mk", false), "Есватини");
    EXPECT_EQ(LocaleConfig::GetDisplayRegion("TW", "mk", false), "Тајван (Кина)");
    EXPECT_EQ(LocaleConfig::GetDisplayRegion("DJ", "ml", false), "ദിജിബൗട്ടി");
    EXPECT_EQ(LocaleConfig::GetDisplayRegion("HK", "ml", false), "ഹോങ്കോങ്ങ് (ചൈന)");
    EXPECT_EQ(LocaleConfig::GetDisplayRegion("HM", "ml", false), "ഹിയേർഡ്, മക്‌ഡൊണാൾഡ് ദ്വീപുകൾ");
    EXPECT_EQ(LocaleConfig::GetDisplayRegion("IS", "ml", false), "ഐസ്‌ലൻഡ്");
    EXPECT_EQ(LocaleConfig::GetDisplayRegion("KP", "ml", false), "ഉത്തര കൊറിയ");
    EXPECT_EQ(LocaleConfig::GetDisplayRegion("MK", "ml", false), "നോർത്ത് മാസിഡോണിയ");
    EXPECT_EQ(LocaleConfig::GetDisplayRegion("MO", "ml", false), "മക്കാവു (ചൈന)");
    EXPECT_EQ(LocaleConfig::GetDisplayRegion("PS", "ml", false), "പലസ്തീൻ");
    EXPECT_EQ(LocaleConfig::GetDisplayRegion("QO", "ml", false), "ഓഷ്യാനിയ ദ്വീപസമൂഹം");
    EXPECT_EQ(LocaleConfig::GetDisplayRegion("TA", "ml", false), "ട്രിസ്റ്റാൻ ഡ കൂന");
    EXPECT_EQ(LocaleConfig::GetDisplayRegion("TJ", "ml", false), "താജികിസ്ഥാൻ");
    EXPECT_EQ(LocaleConfig::GetDisplayRegion("TM", "ml", false), "തുർക്ക്‌മെനിസ്ഥാൻ");
    EXPECT_EQ(LocaleConfig::GetDisplayRegion("TW", "ml", false), "തായ്‌വാൻ (ചൈന)");
    EXPECT_EQ(LocaleConfig::GetDisplayRegion("VA", "ml", false), "വത്തിക്കാൻ സിറ്റി");
    EXPECT_EQ(LocaleConfig::GetDisplayRegion("HK", "mn", false), "Хонконг (Хятад)");
    EXPECT_EQ(LocaleConfig::GetDisplayRegion("HM", "mn", false), "Хийрд ба МакДональд арлууд");
    EXPECT_EQ(LocaleConfig::GetDisplayRegion("MK", "mn", false), "Умард Македон");
    EXPECT_EQ(LocaleConfig::GetDisplayRegion("MO", "mn", false), "Макао (Хятад)");
    EXPECT_EQ(LocaleConfig::GetDisplayRegion("MV", "mn", false), "Малдив");
    EXPECT_EQ(LocaleConfig::GetDisplayRegion("PS", "mn", false), "Палестин");
    EXPECT_EQ(LocaleConfig::GetDisplayRegion("TA", "mn", false), "Тристан да Кунья");
    EXPECT_EQ(LocaleConfig::GetDisplayRegion("TW", "mn", false), "Тайвань (Хятад)");
    EXPECT_EQ(LocaleConfig::GetDisplayRegion("VA", "mn", false), "Ватикан хот");
    EXPECT_EQ(LocaleConfig::GetDisplayRegion("AC", "mr", false), "असेन्शन बेट");
    EXPECT_EQ(LocaleConfig::GetDisplayRegion("CP", "mr", false), "क्लिपरटन बेट");
    EXPECT_EQ(LocaleConfig::GetDisplayRegion("CV", "mr", false), "केप वर्दे");
    EXPECT_EQ(LocaleConfig::GetDisplayRegion("FO", "mr", false), "फॅरो बेटे");
    EXPECT_EQ(LocaleConfig::GetDisplayRegion("HK", "mr", false), "हाँग काँग (चीन)");
    EXPECT_EQ(LocaleConfig::GetDisplayRegion("MO", "mr", false), "मकाओ (चीन)");
    EXPECT_EQ(LocaleConfig::GetDisplayRegion("MV", "mr", false), "मालदिव्ज");
    EXPECT_EQ(LocaleConfig::GetDisplayRegion("MZ", "mr", false), "मोझांबिक");
    EXPECT_EQ(LocaleConfig::GetDisplayRegion("PS", "mr", false), "पॅलेस्टाइन");
    EXPECT_EQ(LocaleConfig::GetDisplayRegion("ST", "mr", false), "साओ टोमे आणि प्रिन्सिप");
    EXPECT_EQ(LocaleConfig::GetDisplayRegion("TA", "mr", false), "त्रिस्तान दा कून्या");
    EXPECT_EQ(LocaleConfig::GetDisplayRegion("TW", "mr", false), "तैवान (चीन)");
    EXPECT_EQ(LocaleConfig::GetDisplayRegion("VA", "mr", false), "वॅटिकन सिटी");
}

/**
 * @tc.name: CustomizedDataTest0026
 * @tc.desc: Test Customized Data
 * @tc.type: FUNC
 */
HWTEST_F(CustomizedDataTest, CustomizedDataTest0026, TestSize.Level1)
{
    EXPECT_EQ(LocaleConfig::GetDisplayRegion("HK", "ms", false), "Hong Kong (China)");
    EXPECT_EQ(LocaleConfig::GetDisplayRegion("MO", "ms", false), "Macao (China)");
    EXPECT_EQ(LocaleConfig::GetDisplayRegion("PS", "ms", false), "Palestin");
    EXPECT_EQ(LocaleConfig::GetDisplayRegion("TW", "ms", false), "Taiwan (China)");
    EXPECT_EQ(LocaleConfig::GetDisplayRegion("VA", "ms", false), "Bandar Vatican");
    EXPECT_EQ(LocaleConfig::GetDisplayRegion("FK", "mt", false), "Il-Gżejjer Falkland (il-Gżejjer Malvinas)");
    EXPECT_EQ(LocaleConfig::GetDisplayRegion("MK", "mt", false), "l-Eks Repubblika Jugoslava tal-Maċedonia");
    EXPECT_EQ(LocaleConfig::GetDisplayRegion("PS", "mt", false), "il-Palestina");
    EXPECT_EQ(LocaleConfig::GetDisplayRegion("TW", "mt", false), "it-Tajwan (iċ-Ċina)");
    EXPECT_EQ(LocaleConfig::GetDisplayRegion("AO", "my", false), "အင်ဂိုလာ");
    EXPECT_EQ(LocaleConfig::GetDisplayRegion("AQ", "my", false), "အန္တာတိက");
    EXPECT_EQ(LocaleConfig::GetDisplayRegion("GS", "my", false), "တောင်ဂျော်ဂျီယာ နှင့် တောင်ဆန်းဒ်ဝစ်ခ်ျ ကျွန်းစုများ");
    EXPECT_EQ(LocaleConfig::GetDisplayRegion("GW", "my", false), "ဂင်းနီဗစ်ဆော");
    EXPECT_EQ(LocaleConfig::GetDisplayRegion("HK", "my", false), "ဟောင်ကောင် (တရုတ်)");
    EXPECT_EQ(LocaleConfig::GetDisplayRegion("HM", "my", false), "ဟတ်ဒ်နှင့် မဂ်ဒေါနယ်ကျွန်းစုများ");
    EXPECT_EQ(LocaleConfig::GetDisplayRegion("MK", "my", false), "မြောက်မက်ဆီဒိုးနီးယား");
    EXPECT_EQ(LocaleConfig::GetDisplayRegion("MO", "my", false), "မကာအို (တရုတ်)");
    EXPECT_EQ(LocaleConfig::GetDisplayRegion("PS", "my", false), "ပါလက်စတိုင်း");
    EXPECT_EQ(LocaleConfig::GetDisplayRegion("QO", "my", false), "သမုဒ္ဒရာတွင်း ကျွန်းနိုင်ငံများ");
    EXPECT_EQ(LocaleConfig::GetDisplayRegion("TF", "my", false), "ပြင်သစ် တောင်ပိုင်း နယ်မြေများ");
    EXPECT_EQ(LocaleConfig::GetDisplayRegion("TJ", "my", false), "တာဂျစ်ကစ္စတန်");
    EXPECT_EQ(LocaleConfig::GetDisplayRegion("TL", "my", false), "တီမောလက်စ်တီ");
    EXPECT_EQ(LocaleConfig::GetDisplayRegion("TW", "my", false), "ထိုင်ဝမ် (တရုတ်)");
    EXPECT_EQ(LocaleConfig::GetDisplayRegion("ZZ", "my", false), "ဒေသ အစိမ်း");
    EXPECT_EQ(LocaleConfig::GetDisplayRegion("AM", "ne", false), "अर्मेनिया");
    EXPECT_EQ(LocaleConfig::GetDisplayRegion("FO", "ne", false), "फारोइ आइल्याण्ड्स");
    EXPECT_EQ(LocaleConfig::GetDisplayRegion("HK", "ne", false), "हङकङ (चीन)");
    EXPECT_EQ(LocaleConfig::GetDisplayRegion("MO", "ne", false), "मकाव (चीन)");
    EXPECT_EQ(LocaleConfig::GetDisplayRegion("PS", "ne", false), "प्यालेस्टाइन");
    EXPECT_EQ(LocaleConfig::GetDisplayRegion("TW", "ne", false), "ताइवान (चीन)");
    EXPECT_EQ(LocaleConfig::GetDisplayRegion("VA", "ne", false), "भ्याटिकन सिटी");
    EXPECT_EQ(LocaleConfig::GetDisplayRegion("HK", "nl", false), "Hongkong (China)");
    EXPECT_EQ(LocaleConfig::GetDisplayRegion("MO", "nl", false), "Macao (China)");
    EXPECT_EQ(LocaleConfig::GetDisplayRegion("PS", "nl", false), "Palestina");
    EXPECT_EQ(LocaleConfig::GetDisplayRegion("TF", "nl", false), "Franse Zuidelijke Gebieden");
    EXPECT_EQ(LocaleConfig::GetDisplayRegion("TW", "nl", false), "Taiwan (China)");
    EXPECT_EQ(LocaleConfig::GetDisplayRegion("FK", "nn", false), "Falklandsøyane (Islas Malvinas)");
    EXPECT_EQ(LocaleConfig::GetDisplayRegion("HK", "no", false), "Hongkong (Kina)");
    EXPECT_EQ(LocaleConfig::GetDisplayRegion("MK", "no", false), "Republikken Nord-Makedonia");
    EXPECT_EQ(LocaleConfig::GetDisplayRegion("MO", "no", false), "Macao (Kina)");
    EXPECT_EQ(LocaleConfig::GetDisplayRegion("PS", "no", false), "Palestina");
    EXPECT_EQ(LocaleConfig::GetDisplayRegion("TW", "no", false), "Taiwan (Kina)");
}

/**
 * @tc.name: CustomizedDataTest0027
 * @tc.desc: Test Customized Data
 * @tc.type: FUNC
 */
HWTEST_F(CustomizedDataTest, CustomizedDataTest0027, TestSize.Level1)
{
    EXPECT_EQ(LocaleConfig::GetDisplayRegion("BT", "or", false), "ଭୁଟାନ୍");
    EXPECT_EQ(LocaleConfig::GetDisplayRegion("CI", "or", false), "କୋଟେ ଡି ଆଇଭୋରି");
    EXPECT_EQ(LocaleConfig::GetDisplayRegion("FO", "or", false), "ଫାରୋଇ ଦ୍ଵୀପପୁଞ୍ଜ");
    EXPECT_EQ(LocaleConfig::GetDisplayRegion("GP", "or", false), "ଗୁଆଡେଲୋପ୍‌");
    EXPECT_EQ(LocaleConfig::GetDisplayRegion("HK", "or", false), "ହଂ କଂ (ଚୀନ୍‌)");
    EXPECT_EQ(LocaleConfig::GetDisplayRegion("KM", "or", false), "କୋମୋରସ୍‌");
    EXPECT_EQ(LocaleConfig::GetDisplayRegion("MD", "or", false), "ମାଲଡୋଭା");
    EXPECT_EQ(LocaleConfig::GetDisplayRegion("MH", "or", false), "ମାର୍ଶାଲ୍‌ ଦ୍ୱୀପପୁଞ୍ଜ");
    EXPECT_EQ(LocaleConfig::GetDisplayRegion("MO", "or", false), "ମକାଓ (ଚୀନ୍‌)");
    EXPECT_EQ(LocaleConfig::GetDisplayRegion("MV", "or", false), "ମାଳଦ୍ୱୀପ");
    EXPECT_EQ(LocaleConfig::GetDisplayRegion("NF", "or", false), "ନର୍ଫକ୍‌ ଦ୍ୱୀପ");
    EXPECT_EQ(LocaleConfig::GetDisplayRegion("PS", "or", false), "ପାଲେଷ୍ଟାଇନ୍");
    EXPECT_EQ(LocaleConfig::GetDisplayRegion("TJ", "or", false), "ତାଜିକିସ୍ଥାନ");
    EXPECT_EQ(LocaleConfig::GetDisplayRegion("TM", "or", false), "ତୁର୍କମେନିସ୍ତାନ୍");
    EXPECT_EQ(LocaleConfig::GetDisplayRegion("TW", "or", false), "ତାଇୱାନ୍‌ (ଚୀନ୍‌)‌");
    EXPECT_EQ(LocaleConfig::GetDisplayRegion("UA", "or", false), "ୟୁକ୍ରେନ୍‌");
    EXPECT_EQ(LocaleConfig::GetDisplayRegion("FO", "pa", false), "ਫ਼ਰੋ ਟਾਪੂ");
    EXPECT_EQ(LocaleConfig::GetDisplayRegion("HK", "pa", false), "ਹਾਂਗ ਕਾਂਗ (ਚੀਨ)");
    EXPECT_EQ(LocaleConfig::GetDisplayRegion("MK", "pa", false), "ਉੱਤਰ ਮੈਸੇਡੋਨੀਆ");
    EXPECT_EQ(LocaleConfig::GetDisplayRegion("MO", "pa", false), "ਮਕਾਓ (ਚੀਨ)");
    EXPECT_EQ(LocaleConfig::GetDisplayRegion("PS", "pa", false), "ਫ਼ਿਲਿਸਤੀਨ");
    EXPECT_EQ(LocaleConfig::GetDisplayRegion("TA", "pa", false), "ਤ੍ਰਿਸਤਾਨ ਦਾ ਕੁੰਹਾ");
    EXPECT_EQ(LocaleConfig::GetDisplayRegion("TW", "pa", false), "ਤਾਈਵਾਨ (ਚੀਨ)");
    EXPECT_EQ(LocaleConfig::GetDisplayRegion("HK", "pl", false), "Hongkong (Chiny)");
    EXPECT_EQ(LocaleConfig::GetDisplayRegion("MO", "pl", false), "Makau (Chiny)");
    EXPECT_EQ(LocaleConfig::GetDisplayRegion("PS", "pl", false), "Palestyna");
    EXPECT_EQ(LocaleConfig::GetDisplayRegion("TW", "pl", false), "Tajwan (Chiny)");
    EXPECT_EQ(LocaleConfig::GetDisplayRegion("FK", "ps", false), "فاکلينډ ټاپوګان (آيزلز مالويناس)");
    EXPECT_EQ(LocaleConfig::GetDisplayRegion("HK", "ps", false), "هانګ کانګ");
    EXPECT_EQ(LocaleConfig::GetDisplayRegion("MK", "ps", false), "مقدونیه (FYROM)");
    EXPECT_EQ(LocaleConfig::GetDisplayRegion("MO", "ps", false), "مکاو");
    EXPECT_EQ(LocaleConfig::GetDisplayRegion("PS", "ps", false), "فلسطين");
    EXPECT_EQ(LocaleConfig::GetDisplayRegion("EZ", "pt", false), "Área do Euro");
    EXPECT_EQ(LocaleConfig::GetDisplayRegion("HK", "pt", false), "Hong Kong (China)");
    EXPECT_EQ(LocaleConfig::GetDisplayRegion("MO", "pt", false), "Macau (China)");
    EXPECT_EQ(LocaleConfig::GetDisplayRegion("PS", "pt", false), "Palestina");
    EXPECT_EQ(LocaleConfig::GetDisplayRegion("SZ", "pt", false), "Eswatini");
    EXPECT_EQ(LocaleConfig::GetDisplayRegion("TJ", "pt", false), "Tajiquistão");
    EXPECT_EQ(LocaleConfig::GetDisplayRegion("TW", "pt", false), "Taiwan (China)");
    EXPECT_EQ(LocaleConfig::GetDisplayRegion("VA", "pt", false), "Vaticano");
    EXPECT_EQ(LocaleConfig::GetDisplayRegion("PS", "pt-PT", false), "Palestina");
    EXPECT_EQ(LocaleConfig::GetDisplayRegion("XA", "pt-PT", false), "pseudoacentos");
    EXPECT_EQ(LocaleConfig::GetDisplayRegion("HK", "ro", false), "Hong Kong (China)");
    EXPECT_EQ(LocaleConfig::GetDisplayRegion("MO", "ro", false), "Macao (China)");
}

/**
 * @tc.name: CustomizedDataTest0028
 * @tc.desc: Test Customized Data
 * @tc.type: FUNC
 */
HWTEST_F(CustomizedDataTest, CustomizedDataTest0028, TestSize.Level1)
{
    EXPECT_EQ(LocaleConfig::GetDisplayRegion("PS", "ro", false), "Palestina");
    EXPECT_EQ(LocaleConfig::GetDisplayRegion("QO", "ro", false), "Oceania Îndepărtată");
    EXPECT_EQ(LocaleConfig::GetDisplayRegion("TW", "ro", false), "Taiwan (China)");
    EXPECT_EQ(LocaleConfig::GetDisplayRegion("VA", "ro", false), "Cetatea Vaticanului");
    EXPECT_EQ(LocaleConfig::GetDisplayRegion("HK", "ru", false), "Гонконг (Китай)");
    EXPECT_EQ(LocaleConfig::GetDisplayRegion("KP", "ru", false), "Северная Корея");
    EXPECT_EQ(LocaleConfig::GetDisplayRegion("MO", "ru", false), "Макао (Китай)");
    EXPECT_EQ(LocaleConfig::GetDisplayRegion("PS", "ru", false), "Палестина");
    EXPECT_EQ(LocaleConfig::GetDisplayRegion("TW", "ru", false), "Тайвань (Китай)");
    EXPECT_EQ(LocaleConfig::GetDisplayRegion("FK", "sd", false), "فلڪ لينڊ ٻيٽ");
    EXPECT_EQ(LocaleConfig::GetDisplayRegion("HK", "sd", false), "هانگ ڪانگ");
    EXPECT_EQ(LocaleConfig::GetDisplayRegion("MK", "sd", false), "ميڪدونيا");
    EXPECT_EQ(LocaleConfig::GetDisplayRegion("MO", "sd", false), "مڪائو");
    EXPECT_EQ(LocaleConfig::GetDisplayRegion("PS", "sd", false), "فلسطين");
    EXPECT_EQ(LocaleConfig::GetDisplayRegion("BV", "si", false), "බුවට් දුපත");
    EXPECT_EQ(LocaleConfig::GetDisplayRegion("CP", "si", false), "ක්ලිපර්ටන් දූපත");
    EXPECT_EQ(LocaleConfig::GetDisplayRegion("HK", "si", false), "හොං කොං (චීනය)");
    EXPECT_EQ(LocaleConfig::GetDisplayRegion("MO", "si", false), "මැකාඕ (චීනය)");
    EXPECT_EQ(LocaleConfig::GetDisplayRegion("PS", "si", false), "පලස්තීනය");
    EXPECT_EQ(LocaleConfig::GetDisplayRegion("ST", "si", false), "සාඕ තෝම් සහ ප්‍රින්සිපේ");
    EXPECT_EQ(LocaleConfig::GetDisplayRegion("TL", "si", false), "ටිමෝර්-ලෙස්ට්");
    EXPECT_EQ(LocaleConfig::GetDisplayRegion("TW", "si", false), "තායිවානය (චීනය)");
    EXPECT_EQ(LocaleConfig::GetDisplayRegion("CP", "sk", false), "Ostrov Clipperton");
    EXPECT_EQ(LocaleConfig::GetDisplayRegion("HK", "sk", false), "Hongkong (Čína)");
    EXPECT_EQ(LocaleConfig::GetDisplayRegion("MO", "sk", false), "Macao (Čína)");
    EXPECT_EQ(LocaleConfig::GetDisplayRegion("PS", "sk", false), "Palestína");
    EXPECT_EQ(LocaleConfig::GetDisplayRegion("TW", "sk", false), "Taiwan (Čína)");
    EXPECT_EQ(LocaleConfig::GetDisplayRegion("HK", "sl", false), "Hongkong (Kitajska)");
    EXPECT_EQ(LocaleConfig::GetDisplayRegion("MO", "sl", false), "Macao (Kitajska)");
    EXPECT_EQ(LocaleConfig::GetDisplayRegion("PS", "sl", false), "Palestina");
    EXPECT_EQ(LocaleConfig::GetDisplayRegion("QO", "sl", false), "Ostala Oceanija");
    EXPECT_EQ(LocaleConfig::GetDisplayRegion("TL", "sl", false), "Vzhodni Timor");
    EXPECT_EQ(LocaleConfig::GetDisplayRegion("TW", "sl", false), "Tajvan (Kitajska)");
    EXPECT_EQ(LocaleConfig::GetDisplayRegion("ZZ", "sl", false), "Neznano območje");
    EXPECT_EQ(LocaleConfig::GetDisplayRegion("FK", "smn", false), "Falklandsuolluuh (Malvinassuolluuh)");
    EXPECT_EQ(LocaleConfig::GetDisplayRegion("HK", "smn", false), "Hong Kong");
    EXPECT_EQ(LocaleConfig::GetDisplayRegion("MO", "smn", false), "Macao");
    EXPECT_EQ(LocaleConfig::GetDisplayRegion("FK", "so", false), "Jasiiradaha Fookland");
    EXPECT_EQ(LocaleConfig::GetDisplayRegion("PS", "so", false), "Falastiin");
    EXPECT_EQ(LocaleConfig::GetDisplayRegion("BL", "sq", false), "Shën Bartolomeu");
    EXPECT_EQ(LocaleConfig::GetDisplayRegion("BM", "sq", false), "Bermudë");
    EXPECT_EQ(LocaleConfig::GetDisplayRegion("BS", "sq", false), "Bahamas");
}

/**
 * @tc.name: CustomizedDataTest0029
 * @tc.desc: Test Customized Data
 * @tc.type: FUNC
 */
HWTEST_F(CustomizedDataTest, CustomizedDataTest0029, TestSize.Level1)
{
    EXPECT_EQ(LocaleConfig::GetDisplayRegion("CW", "sq", false), "Kuraçao");
    EXPECT_EQ(LocaleConfig::GetDisplayRegion("EU", "sq", false), "Bashkimi Europian");
    EXPECT_EQ(LocaleConfig::GetDisplayRegion("EZ", "sq", false), "Eurozonë");
    EXPECT_EQ(LocaleConfig::GetDisplayRegion("GD", "sq", false), "Grenadë");
    EXPECT_EQ(LocaleConfig::GetDisplayRegion("GL", "sq", false), "Grenlandë");
    EXPECT_EQ(LocaleConfig::GetDisplayRegion("GM", "sq", false), "Gambia");
    EXPECT_EQ(LocaleConfig::GetDisplayRegion("GP", "sq", false), "Guadalupe");
    EXPECT_EQ(LocaleConfig::GetDisplayRegion("HK", "sq", false), "Hong-Kong (Kinë)");
    EXPECT_EQ(LocaleConfig::GetDisplayRegion("HM", "sq", false), "Ishulli Hërd dhe Ishujt Mekdonald");
    EXPECT_EQ(LocaleConfig::GetDisplayRegion("KG", "sq", false), "Kirgistan");
    EXPECT_EQ(LocaleConfig::GetDisplayRegion("KP", "sq", false), "Koreja e Veriut");
    EXPECT_EQ(LocaleConfig::GetDisplayRegion("KR", "sq", false), "Koreja e Jugut");
    EXPECT_EQ(LocaleConfig::GetDisplayRegion("MF", "sq", false), "Shën-Martin");
    EXPECT_EQ(LocaleConfig::GetDisplayRegion("MK", "sq", false), "Maqedoni");
    EXPECT_EQ(LocaleConfig::GetDisplayRegion("MM", "sq", false), "Mianmar (Burma)");
    EXPECT_EQ(LocaleConfig::GetDisplayRegion("MO", "sq", false), "Makao (Kinë)");
    EXPECT_EQ(LocaleConfig::GetDisplayRegion("NC", "sq", false), "Kaledonia e Re");
    EXPECT_EQ(LocaleConfig::GetDisplayRegion("PM", "sq", false), "Shën Pier dhe Mikelon");
    EXPECT_EQ(LocaleConfig::GetDisplayRegion("SA", "sq", false), "Arabia Saudite");
    EXPECT_EQ(LocaleConfig::GetDisplayRegion("SH", "sq", false), "Shën-Helenë");
    EXPECT_EQ(LocaleConfig::GetDisplayRegion("SJ", "sq", false), "Svalbard dhe Jan-Majen");
    EXPECT_EQ(LocaleConfig::GetDisplayRegion("SL", "sq", false), "Siera-Leone");
    EXPECT_EQ(LocaleConfig::GetDisplayRegion("ST", "sq", false), "Sao Tome dhe Principe");
    EXPECT_EQ(LocaleConfig::GetDisplayRegion("SZ", "sq", false), "Svazilandë");
    EXPECT_EQ(LocaleConfig::GetDisplayRegion("TW", "sq", false), "Tajvan (Kinë)");
    EXPECT_EQ(LocaleConfig::GetDisplayRegion("UN", "sq", false), "Kombet e Bashkuara");
    EXPECT_EQ(LocaleConfig::GetDisplayRegion("US", "sq", false), "Shtetet e Bashkuara të Amerikës");
    EXPECT_EQ(LocaleConfig::GetDisplayRegion("ZM", "sq", false), "Zambia");
    EXPECT_EQ(LocaleConfig::GetDisplayRegion("HK", "sr", false), "Хонгконг (Кина)");
    EXPECT_EQ(LocaleConfig::GetDisplayRegion("MO", "sr", false), "Макао (Кина)");
    EXPECT_EQ(LocaleConfig::GetDisplayRegion("PS", "sr", false), "Палестина");
    EXPECT_EQ(LocaleConfig::GetDisplayRegion("TW", "sr", false), "Тајван (Кина)");
    EXPECT_EQ(LocaleConfig::GetDisplayRegion("HK", "sr-Latn", false), "Hongkong (Kina)");
    EXPECT_EQ(LocaleConfig::GetDisplayRegion("MO", "sr-Latn", false), "Makao (Kina)");
    EXPECT_EQ(LocaleConfig::GetDisplayRegion("PS", "sr-Latn", false), "Palestina");
    EXPECT_EQ(LocaleConfig::GetDisplayRegion("TL", "sr-Latn", false), "Istočni Timor");
    EXPECT_EQ(LocaleConfig::GetDisplayRegion("TW", "sr-Latn", false), "Tajvan (Kina)");
    EXPECT_EQ(LocaleConfig::GetDisplayRegion("EZ", "sv", false), "Euroområdet");
    EXPECT_EQ(LocaleConfig::GetDisplayRegion("HK", "sv", false), "Hongkong (Kina)");
    EXPECT_EQ(LocaleConfig::GetDisplayRegion("MO", "sv", false), "Macao (Kina)");
    EXPECT_EQ(LocaleConfig::GetDisplayRegion("PS", "sv", false), "Palestina");
    EXPECT_EQ(LocaleConfig::GetDisplayRegion("QO", "sv", false), "Yttre öar i Oceanien");
    EXPECT_EQ(LocaleConfig::GetDisplayRegion("TW", "sv", false), "Taiwan (Kina)");
    EXPECT_EQ(LocaleConfig::GetDisplayRegion("ZZ", "sv", false), "Okänd region");
    EXPECT_EQ(LocaleConfig::GetDisplayRegion("BT", "sw", false), "Butani");
    EXPECT_EQ(LocaleConfig::GetDisplayRegion("HK", "sw", false), "Hong Kong (Uchina)");
    EXPECT_EQ(LocaleConfig::GetDisplayRegion("MO", "sw", false), "Makau (Uchina)");
}

/**
 * @tc.name: CustomizedDataTest0030
 * @tc.desc: Test Customized Data
 * @tc.type: FUNC
 */
HWTEST_F(CustomizedDataTest, CustomizedDataTest0030, TestSize.Level1)
{
    EXPECT_EQ(LocaleConfig::GetDisplayRegion("PS", "sw", false), "Palestina");
    EXPECT_EQ(LocaleConfig::GetDisplayRegion("TW", "sw", false), "Taiwani (Uchina)");
    EXPECT_EQ(LocaleConfig::GetDisplayRegion("FO", "ta", false), "ஃபரோ தீவுகள்");
    EXPECT_EQ(LocaleConfig::GetDisplayRegion("HK", "ta", false), "ஹாங்காங் (சீனா)");
    EXPECT_EQ(LocaleConfig::GetDisplayRegion("MO", "ta", false), "மக்காவு (சீனா)");
    EXPECT_EQ(LocaleConfig::GetDisplayRegion("MV", "ta", false), "மாலத்தீவுகள்");
    EXPECT_EQ(LocaleConfig::GetDisplayRegion("PS", "ta", false), "பாலஸ்தீனம்");
    EXPECT_EQ(LocaleConfig::GetDisplayRegion("QO", "ta", false), "வெளிப்புற ஓசியானியா");
    EXPECT_EQ(LocaleConfig::GetDisplayRegion("TF", "ta", false), "பிரெஞ்சு தெற்குப் பிரதேசங்கள்");
    EXPECT_EQ(LocaleConfig::GetDisplayRegion("TW", "ta", false), "தைவான் (சீனா)");
    EXPECT_EQ(LocaleConfig::GetDisplayRegion("VA", "ta", false), "வாடிகன் சிட்டி");
    EXPECT_EQ(LocaleConfig::GetDisplayRegion("FO", "te", false), "ఫారోయి దీవులు");
    EXPECT_EQ(LocaleConfig::GetDisplayRegion("HK", "te", false), "హాంగ్ కాంగ్ (చైనా)");
    EXPECT_EQ(LocaleConfig::GetDisplayRegion("MK", "te", false), "నార్త్ మెసిడోనియా");
    EXPECT_EQ(LocaleConfig::GetDisplayRegion("MK", "tg", false), "Мақдун (ҶСЮМ)");
    EXPECT_EQ(LocaleConfig::GetDisplayRegion("HK", "th", false), "ฮ่องกง (จีน)");
    EXPECT_EQ(LocaleConfig::GetDisplayRegion("MO", "th", false), "มาเก๊า (จีน)");
    EXPECT_EQ(LocaleConfig::GetDisplayRegion("PS", "th", false), "ปาเลสไตน์");
    EXPECT_EQ(LocaleConfig::GetDisplayRegion("TA", "th", false), "ตริสตันดากูนยา");
    EXPECT_EQ(LocaleConfig::GetDisplayRegion("TW", "th", false), "ไต้หวัน (จีน)");
    EXPECT_EQ(LocaleConfig::GetDisplayRegion("VA", "th", false), "นครรัฐวาติกัน");
    EXPECT_EQ(LocaleConfig::GetDisplayRegion("MK", "ti", false), "መቄዶኒያ");
    EXPECT_EQ(LocaleConfig::GetDisplayRegion("MO", "ti", false), "ማካው");
    EXPECT_EQ(LocaleConfig::GetDisplayRegion("PS", "ti", false), "ፍልስጤም");
    EXPECT_EQ(LocaleConfig::GetDisplayRegion("FK", "tk", false), "Folklend (Malwina) adalary");
    EXPECT_EQ(LocaleConfig::GetDisplayRegion("HK", "tk", false), "Gonkong");
    EXPECT_EQ(LocaleConfig::GetDisplayRegion("MK", "tk", false), "Makedoniýa (ÖÝR)");
    EXPECT_EQ(LocaleConfig::GetDisplayRegion("MO", "tk", false), "Makau");
    EXPECT_EQ(LocaleConfig::GetDisplayRegion("PS", "tk", false), "Palestina");
    EXPECT_EQ(LocaleConfig::GetDisplayRegion("HK", "to", false), "Hongi Kongi");
    EXPECT_EQ(LocaleConfig::GetDisplayRegion("MK", "to", false), "Masetōnia (FYROM)");
    EXPECT_EQ(LocaleConfig::GetDisplayRegion("MO", "to", false), "Makau");
    EXPECT_EQ(LocaleConfig::GetDisplayRegion("PS", "to", false), "Palesitaine");
    EXPECT_EQ(LocaleConfig::GetDisplayRegion("BT", "tr", false), "Bhutan");
    EXPECT_EQ(LocaleConfig::GetDisplayRegion("HK", "tr", false), "Hong Kong (Çin)");
    EXPECT_EQ(LocaleConfig::GetDisplayRegion("MO", "tr", false), "Makao (Çin)");
    EXPECT_EQ(LocaleConfig::GetDisplayRegion("PS", "tr", false), "Filistin");
    EXPECT_EQ(LocaleConfig::GetDisplayRegion("TW", "tr", false), "Tayvan (Çin)");
    EXPECT_EQ(LocaleConfig::GetDisplayRegion("AT", "ug", false), "ئاۋسترىيە");
    EXPECT_EQ(LocaleConfig::GetDisplayRegion("CH", "ug", false), "شىۋىتسارىيە");
    EXPECT_EQ(LocaleConfig::GetDisplayRegion("CP", "ug", false), "كىلىپپېرتون ئارىلى");
    EXPECT_EQ(LocaleConfig::GetDisplayRegion("CZ", "ug", false), "چېخىيە");
    EXPECT_EQ(LocaleConfig::GetDisplayRegion("EH", "ug", false), "غەربىي ساھارا");
    EXPECT_EQ(LocaleConfig::GetDisplayRegion("HK", "ug", false), "جۇڭگو شياڭگاڭ");
}

/**
 * @tc.name: CustomizedDataTest0031
 * @tc.desc: Test Customized Data
 * @tc.type: FUNC
 */
HWTEST_F(CustomizedDataTest, CustomizedDataTest0031, TestSize.Level1)
{
    EXPECT_EQ(LocaleConfig::GetDisplayRegion("HM", "ug", false), "ھېرد ۋە ماكدونالد ئاراللىرى");
    EXPECT_EQ(LocaleConfig::GetDisplayRegion("MM", "ug", false), "ميانمار");
    EXPECT_EQ(LocaleConfig::GetDisplayRegion("MO", "ug", false), "جۇڭگو ئاۋمېن");
    EXPECT_EQ(LocaleConfig::GetDisplayRegion("PS", "ug", false), "پەلەستىن");
    EXPECT_EQ(LocaleConfig::GetDisplayRegion("TW", "ug", false), "جۇڭگو تەيۋەن");
    EXPECT_EQ(LocaleConfig::GetDisplayRegion("ZZ", "ug", false), "ئېنىقسىز رايون");
    EXPECT_EQ(LocaleConfig::GetDisplayRegion("AO", "uk", false), "Анґола");
    EXPECT_EQ(LocaleConfig::GetDisplayRegion("AQ", "uk", false), "Антарктида");
    EXPECT_EQ(LocaleConfig::GetDisplayRegion("GW", "uk", false), "Ґвінея-Бісау");
    EXPECT_EQ(LocaleConfig::GetDisplayRegion("HK", "uk", false), "Гонконг (Китай)");
    EXPECT_EQ(LocaleConfig::GetDisplayRegion("MO", "uk", false), "Макао (Китай)");
    EXPECT_EQ(LocaleConfig::GetDisplayRegion("PS", "uk", false), "Палестина");
    EXPECT_EQ(LocaleConfig::GetDisplayRegion("TW", "uk", false), "Тайвань (Китай)");
    EXPECT_EQ(LocaleConfig::GetDisplayRegion("BV", "ur", false), "جزیرہ بووے");
    EXPECT_EQ(LocaleConfig::GetDisplayRegion("CP", "ur", false), "جزیرہ کلپرٹن");
    EXPECT_EQ(LocaleConfig::GetDisplayRegion("CV", "ur", false), "کیپ ورڈ");
    EXPECT_EQ(LocaleConfig::GetDisplayRegion("EU", "ur", false), "یورپی یونین");
    EXPECT_EQ(LocaleConfig::GetDisplayRegion("FO", "ur", false), "جزیرہ فارو");
    EXPECT_EQ(LocaleConfig::GetDisplayRegion("HK", "ur", false), "ہانگ کانگ (چین)");
    EXPECT_EQ(LocaleConfig::GetDisplayRegion("HM", "ur", false), "جزائر ہرڈ اور مک ڈونلڈ");
    EXPECT_EQ(LocaleConfig::GetDisplayRegion("MO", "ur", false), "مکاؤ (چین)");
    EXPECT_EQ(LocaleConfig::GetDisplayRegion("PS", "ur", false), "فلسطین");
    EXPECT_EQ(LocaleConfig::GetDisplayRegion("ST", "ur", false), "ساؤ ٹوم اور پرنیسپے");
    EXPECT_EQ(LocaleConfig::GetDisplayRegion("TW", "ur", false), "تائیوان (چین)");
    EXPECT_EQ(LocaleConfig::GetDisplayRegion("EZ", "uz", false), "Yevrohudud");
    EXPECT_EQ(LocaleConfig::GetDisplayRegion("HK", "uz", false), "Gonkong (Xitoy)");
    EXPECT_EQ(LocaleConfig::GetDisplayRegion("MO", "uz", false), "Makao (Xitoy)");
    EXPECT_EQ(LocaleConfig::GetDisplayRegion("PS", "uz", false), "Falastin");
    EXPECT_EQ(LocaleConfig::GetDisplayRegion("TF", "uz", false), "Fransiya Janubiy hududlari");
    EXPECT_EQ(LocaleConfig::GetDisplayRegion("TW", "uz", false), "Tayvan (Xitoy)");
    EXPECT_EQ(LocaleConfig::GetDisplayRegion("VA", "uz", false), "Vatikan shahri");
    EXPECT_EQ(LocaleConfig::GetDisplayRegion("EU", "vi", false), "Liên minh Châu Âu");
    EXPECT_EQ(LocaleConfig::GetDisplayRegion("HK", "vi", false), "Hồng Kông (Trung Quốc)");
    EXPECT_EQ(LocaleConfig::GetDisplayRegion("MO", "vi", false), "Ma Cao (Trung Quốc)");
    EXPECT_EQ(LocaleConfig::GetDisplayRegion("PS", "vi", false), "Palestine");
    EXPECT_EQ(LocaleConfig::GetDisplayRegion("TW", "vi", false), "Đài Loan (Trung Quốc)");
    EXPECT_EQ(LocaleConfig::GetDisplayRegion("UN", "vi", false), "Liên Hiệp Quốc");
    EXPECT_EQ(LocaleConfig::GetDisplayRegion("FK", "wae", false), "Falklandinslä (Malwine)");
    EXPECT_EQ(LocaleConfig::GetDisplayRegion("HK", "wae", false), "Hongkong");
    EXPECT_EQ(LocaleConfig::GetDisplayRegion("MO", "wae", false), "Makau");
    EXPECT_EQ(LocaleConfig::GetDisplayRegion("MK", "wo", false), "Maseduwaan (Réewum yugoslawi gu yàgg ga)");
}

/**
 * @tc.name: CustomizedDataTest0032
 * @tc.desc: Test Customized Data
 * @tc.type: FUNC
 */
HWTEST_F(CustomizedDataTest, CustomizedDataTest0032, TestSize.Level1)
{
    EXPECT_EQ(LocaleConfig::GetDisplayRegion("FK", "yue", false), "福克蘭群島 (馬爾維納斯群島)");
    EXPECT_EQ(LocaleConfig::GetDisplayRegion("HK", "yue", false), "中國香港");
    EXPECT_EQ(LocaleConfig::GetDisplayRegion("MO", "yue", false), "中國澳門");
    EXPECT_EQ(LocaleConfig::GetDisplayRegion("PS", "yue", false), "巴勒斯坦");
    EXPECT_EQ(LocaleConfig::GetDisplayRegion("TW", "yue", false), "中國台灣");
    EXPECT_EQ(LocaleConfig::GetDisplayRegion("FK", "yue-Hans", false), "福克兰群岛 (马尔维纳斯群岛)");
    EXPECT_EQ(LocaleConfig::GetDisplayRegion("HK", "yue-Hans", false), "中国香港");
    EXPECT_EQ(LocaleConfig::GetDisplayRegion("MO", "yue-Hans", false), "中国澳门");
    EXPECT_EQ(LocaleConfig::GetDisplayRegion("PS", "yue-Hans", false), "巴勒斯坦");
    EXPECT_EQ(LocaleConfig::GetDisplayRegion("TW", "yue-Hans", false), "中国台湾");
    EXPECT_EQ(LocaleConfig::GetDisplayRegion("HK", "zh", false), "中国香港");
    EXPECT_EQ(LocaleConfig::GetDisplayRegion("MO", "zh", false), "中国澳门");
    EXPECT_EQ(LocaleConfig::GetDisplayRegion("PS", "zh", false), "巴勒斯坦");
    EXPECT_EQ(LocaleConfig::GetDisplayRegion("TW", "zh", false), "中国台湾");
    EXPECT_EQ(LocaleConfig::GetDisplayRegion("HK", "zh-Hant", false), "中國香港");
    EXPECT_EQ(LocaleConfig::GetDisplayRegion("KP", "zh-Hant", false), "朝鮮");
    EXPECT_EQ(LocaleConfig::GetDisplayRegion("MO", "zh-Hant", false), "中國澳門");
    EXPECT_EQ(LocaleConfig::GetDisplayRegion("PS", "zh-Hant", false), "巴勒斯坦");
    EXPECT_EQ(LocaleConfig::GetDisplayRegion("TW", "zh-Hant", false), "中國台灣");
    EXPECT_EQ(LocaleConfig::GetDisplayRegion("PS", "zh-Hant-HK", false), "巴勒斯坦");
}

/**
 * @tc.name: CustomizedDataTest0033
 * @tc.desc: Test Customized Data
 * @tc.type: FUNC
 */
HWTEST_F(CustomizedDataTest, CustomizedDataTest0033, TestSize.Level1)
{
    I18nErrorCode errCode = I18nErrorCode::SUCCESS;
    struct tm tmObj = {.tm_mday = 23, .tm_mon = 3, .tm_year = 124};
    time_t timep = mktime(&tmObj);
    auto sDateFormat0 = std::make_unique<SimpleDateTimeFormat>("MEd", "am", false, errCode);
    EXPECT_EQ(sDateFormat0->Format(timep*1000), "ማክሰኞ፣ 23/04");
    auto sDateFormat1 = std::make_unique<SimpleDateTimeFormat>("yM", "am", false, errCode);
    EXPECT_EQ(sDateFormat1->Format(timep*1000), "04/2024");
    auto sDateFormat2 = std::make_unique<SimpleDateTimeFormat>("yMEd", "am", false, errCode);
    EXPECT_EQ(sDateFormat2->Format(timep*1000), "ማክሰኞ፣ 23/04/2024");
    auto sDateFormat3 = std::make_unique<SimpleDateTimeFormat>("yMMMEd", "am", false, errCode);
    EXPECT_EQ(sDateFormat3->Format(timep*1000), "ማክሰኞ፣ ኤፕሪ 23፣ 2024");
    auto sDateFormat4 = std::make_unique<SimpleDateTimeFormat>("yMd", "am", false, errCode);
    EXPECT_EQ(sDateFormat4->Format(timep*1000), "23/04/2024");
    auto sDateFormat5 = std::make_unique<SimpleDateTimeFormat>("MEd", "ar", false, errCode);
    EXPECT_EQ(sDateFormat5->Format(timep*1000), "الثلاثاء، ٢٣/‏٤");
    auto sDateFormat6 = std::make_unique<SimpleDateTimeFormat>("yMMMEd", "ar", false, errCode);
    EXPECT_EQ(sDateFormat6->Format(timep*1000), "الثلاثاء، ٢٣ أبريل، ٢٠٢٤");
    auto sDateFormat7 = std::make_unique<SimpleDateTimeFormat>("yMMMd", "ar", false, errCode);
    EXPECT_EQ(sDateFormat7->Format(timep*1000), "٢٣ أبريل، ٢٠٢٤");
    auto sDateFormat8 = std::make_unique<SimpleDateTimeFormat>("GyMMM", "bg", false, errCode);
    EXPECT_EQ(sDateFormat8->Format(timep*1000), "апр 2024 г. сл.Хр.");
    auto sDateFormat9 = std::make_unique<SimpleDateTimeFormat>("GyMMMEd", "bg", false, errCode);
    EXPECT_EQ(sDateFormat9->Format(timep*1000), "вт, 23 апр 2024 г. сл.Хр.");
    auto sDateFormat10 = std::make_unique<SimpleDateTimeFormat>("GyMMMd", "bg", false, errCode);
    EXPECT_EQ(sDateFormat10->Format(timep*1000), "23 апр 2024 г. сл.Хр.");
    auto sDateFormat11 = std::make_unique<SimpleDateTimeFormat>("MMM", "bg", false, errCode);
    EXPECT_EQ(sDateFormat11->Format(timep*1000), "апр");
    auto sDateFormat12 = std::make_unique<SimpleDateTimeFormat>("MMMEd", "bg", false, errCode);
    EXPECT_EQ(sDateFormat12->Format(timep*1000), "вт, 23 апр");
    auto sDateFormat13 = std::make_unique<SimpleDateTimeFormat>("MMMd", "bg", false, errCode);
    EXPECT_EQ(sDateFormat13->Format(timep*1000), "23 апр");
    auto sDateFormat14 = std::make_unique<SimpleDateTimeFormat>("yMMM", "bg", false, errCode);
    EXPECT_EQ(sDateFormat14->Format(timep*1000), "апр 2024 г.");
    auto sDateFormat15 = std::make_unique<SimpleDateTimeFormat>("yMMMEd", "bg", false, errCode);
    EXPECT_EQ(sDateFormat15->Format(timep*1000), "вт, 23 апр 2024 г.");
    auto sDateFormat16 = std::make_unique<SimpleDateTimeFormat>("yMMMd", "bg", false, errCode);
    EXPECT_EQ(sDateFormat16->Format(timep*1000), "23 апр 2024 г.");
    auto sDateFormat17 = std::make_unique<SimpleDateTimeFormat>("MEd", "bn", false, errCode);
    EXPECT_EQ(sDateFormat17->Format(timep*1000), "মঙ্গল, ২৩/৪");
    auto sDateFormat18 = std::make_unique<SimpleDateTimeFormat>("MMMEd", "bn", false, errCode);
    EXPECT_EQ(sDateFormat18->Format(timep*1000), "মঙ্গল, ২৩ এপ্রি");
    auto sDateFormat19 = std::make_unique<SimpleDateTimeFormat>("MMMMEd", "bn", false, errCode);
    EXPECT_EQ(sDateFormat19->Format(timep*1000), "মঙ্গল, ২৩ এপ্রিল");
    auto sDateFormat20 = std::make_unique<SimpleDateTimeFormat>("MMdd", "bn", false, errCode);
    EXPECT_EQ(sDateFormat20->Format(timep*1000), "২৩/০৪");
    auto sDateFormat21 = std::make_unique<SimpleDateTimeFormat>("yMM", "bn", false, errCode);
    EXPECT_EQ(sDateFormat21->Format(timep*1000), "০৪/২০২৪");
    auto sDateFormat22 = std::make_unique<SimpleDateTimeFormat>("MMMEd", "bo", false, errCode);
    EXPECT_EQ(sDateFormat22->Format(timep*1000), "ཟླ་༤ ཚེས་ 23 མིག་དམར་");
}

/**
 * @tc.name: CustomizedDataTest0034
 * @tc.desc: Test Customized Data
 * @tc.type: FUNC
 */
HWTEST_F(CustomizedDataTest, CustomizedDataTest0034, TestSize.Level1)
{
    I18nErrorCode errCode = I18nErrorCode::SUCCESS;
    struct tm tmObj = {.tm_mday = 23, .tm_mon = 3, .tm_year = 124};
    time_t timep = mktime(&tmObj);
    auto sDateFormat23 = std::make_unique<SimpleDateTimeFormat>("MMMMd", "bo", false, errCode);
    EXPECT_EQ(sDateFormat23->Format(timep*1000), "ཟླ་བ་བཞི་པ་ འི་ཚེས་ 23");
    auto sDateFormat24 = std::make_unique<SimpleDateTimeFormat>("MMMd", "bo", false, errCode);
    EXPECT_EQ(sDateFormat24->Format(timep*1000), "ཟླ་༤ ཚེས་ 23");
    auto sDateFormat25 = std::make_unique<SimpleDateTimeFormat>("yMMM", "bo", false, errCode);
    EXPECT_EQ(sDateFormat25->Format(timep*1000), "2024 ལོའི ་ཟླ་༤ ཚེས");
    auto sDateFormat26 = std::make_unique<SimpleDateTimeFormat>("yMMMMd", "bo", false, errCode);
    EXPECT_EQ(sDateFormat26->Format(timep*1000), "སྤྱི་ལོ་ 2024 ཟླ་བ་བཞི་པ་ འི་ཚེས ་23");
    auto sDateFormat27 = std::make_unique<SimpleDateTimeFormat>("yMMMd", "bo", false, errCode);
    EXPECT_EQ(sDateFormat27->Format(timep*1000), "2024 ལོའི་ ཟླ་༤ ཚེས་ 23");
    auto sDateFormat28 = std::make_unique<SimpleDateTimeFormat>("yM", "bs", false, errCode);
    EXPECT_EQ(sDateFormat28->Format(timep*1000), "4.2024");
    auto sDateFormat29 = std::make_unique<SimpleDateTimeFormat>("yMM", "bs", false, errCode);
    EXPECT_EQ(sDateFormat29->Format(timep*1000), "4.2024");
    auto sDateFormat30 = std::make_unique<SimpleDateTimeFormat>("EHm", "ca", false, errCode);
    EXPECT_EQ(sDateFormat30->Format(timep*1000), "dt. 0.00");
    auto sDateFormat31 = std::make_unique<SimpleDateTimeFormat>("EHms", "ca", false, errCode);
    EXPECT_EQ(sDateFormat31->Format(timep*1000), "dt. 0.00.00");
    auto sDateFormat32 = std::make_unique<SimpleDateTimeFormat>("Ehm", "ca", false, errCode);
    EXPECT_EQ(sDateFormat32->Format(timep*1000), "dt. 12.00 a. m.");
    auto sDateFormat33 = std::make_unique<SimpleDateTimeFormat>("Ehms", "ca", false, errCode);
    EXPECT_EQ(sDateFormat33->Format(timep*1000), "dt. 12.00.00 a. m.");
    auto sDateFormat34 = std::make_unique<SimpleDateTimeFormat>("GyMMMEd", "ca", false, errCode);
    EXPECT_EQ(sDateFormat34->Format(timep*1000), "dt., 23 abr. 2024 dC");
    auto sDateFormat35 = std::make_unique<SimpleDateTimeFormat>("GyMMMM", "ca", false, errCode);
    EXPECT_EQ(sDateFormat35->Format(timep*1000), "abril de 2024 dC");
    auto sDateFormat36 = std::make_unique<SimpleDateTimeFormat>("GyMMMMEd", "ca", false, errCode);
    EXPECT_EQ(sDateFormat36->Format(timep*1000), "dt., 23 d’abril de 2024 dC");
    auto sDateFormat37 = std::make_unique<SimpleDateTimeFormat>("GyMMMMd", "ca", false, errCode);
    EXPECT_EQ(sDateFormat37->Format(timep*1000), "23 d’abril de 2024 dC");
    auto sDateFormat38 = std::make_unique<SimpleDateTimeFormat>("GyMMMd", "ca", false, errCode);
    EXPECT_EQ(sDateFormat38->Format(timep*1000), "23 abr. 2024 dC");
    auto sDateFormat39 = std::make_unique<SimpleDateTimeFormat>("Hm", "ca", false, errCode);
    EXPECT_EQ(sDateFormat39->Format(timep*1000), "0.00");
    auto sDateFormat40 = std::make_unique<SimpleDateTimeFormat>("Hms", "ca", false, errCode);
    EXPECT_EQ(sDateFormat40->Format(timep*1000), "0.00.00");
    auto sDateFormat41 = std::make_unique<SimpleDateTimeFormat>("Hmsv", "ca", false, errCode);
    EXPECT_EQ(sDateFormat41->Format(timep*1000), "0.00.00 Hora de: Xina");
    auto sDateFormat42 = std::make_unique<SimpleDateTimeFormat>("Hmv", "ca", false, errCode);
    EXPECT_EQ(sDateFormat42->Format(timep*1000), "0.00 Hora de: Xina");
    auto sDateFormat43 = std::make_unique<SimpleDateTimeFormat>("hm", "ca", false, errCode);
    EXPECT_EQ(sDateFormat43->Format(timep*1000), "12.00 a. m.");
    auto sDateFormat44 = std::make_unique<SimpleDateTimeFormat>("hms", "ca", false, errCode);
    EXPECT_EQ(sDateFormat44->Format(timep*1000), "12.00.00 a. m.");
}

/**
 * @tc.name: CustomizedDataTest0035
 * @tc.desc: Test Customized Data
 * @tc.type: FUNC
 */
HWTEST_F(CustomizedDataTest, CustomizedDataTest0035, TestSize.Level1)
{
    I18nErrorCode errCode = I18nErrorCode::SUCCESS;
    struct tm tmObj = {.tm_mday = 23, .tm_mon = 3, .tm_year = 124};
    time_t timep = mktime(&tmObj);
    auto sDateFormat201 = std::make_unique<SimpleDateTimeFormat>("yMMMEd", "mr", false, errCode);
    EXPECT_EQ(sDateFormat201->Format(timep*1000), "मंगळ, २३ एप्रि, २०२४");
    auto sDateFormat202 = std::make_unique<SimpleDateTimeFormat>("MEd", "ms", false, errCode);
    EXPECT_EQ(sDateFormat202->Format(timep*1000), "Sel, 23/04");
    auto sDateFormat203 = std::make_unique<SimpleDateTimeFormat>("Md", "ms", false, errCode);
    EXPECT_EQ(sDateFormat203->Format(timep*1000), "23/04");
    auto sDateFormat204 = std::make_unique<SimpleDateTimeFormat>("yM", "ms", false, errCode);
    EXPECT_EQ(sDateFormat204->Format(timep*1000), "04/2024");
    auto sDateFormat205 = std::make_unique<SimpleDateTimeFormat>("yMEd", "ms", false, errCode);
    EXPECT_EQ(sDateFormat205->Format(timep*1000), "Sel, 23/04/2024");
    auto sDateFormat206 = std::make_unique<SimpleDateTimeFormat>("yMd", "ms", false, errCode);
    EXPECT_EQ(sDateFormat206->Format(timep*1000), "23/04/2024");
    auto sDateFormat207 = std::make_unique<SimpleDateTimeFormat>("Ed", "my", false, errCode);
    EXPECT_EQ(sDateFormat207->Format(timep*1000), "အင်္ဂါ၊ ၂၃");
    auto sDateFormat208 = std::make_unique<SimpleDateTimeFormat>("Ehm", "my", false, errCode);
    EXPECT_EQ(sDateFormat208->Format(timep*1000), "အင်္ဂါ ၁၂:၀၀ နံနက်");
    auto sDateFormat209 = std::make_unique<SimpleDateTimeFormat>("Ehms", "my", false, errCode);
    EXPECT_EQ(sDateFormat209->Format(timep*1000), "အင်္ဂါ ၁၂:၀၀:၀၀ နံနက်");
    auto sDateFormat210 = std::make_unique<SimpleDateTimeFormat>("MEd", "my", false, errCode);
    EXPECT_EQ(sDateFormat210->Format(timep*1000), "အင်္ဂါနေ့၊ ၂၃/ဧပြီ");
    auto sDateFormat211 = std::make_unique<SimpleDateTimeFormat>("MMMEd", "my", false, errCode);
    EXPECT_EQ(sDateFormat211->Format(timep*1000), "အင်္ဂါနေ့၊ ဧပြီလ ၂၃ ရက်");
    auto sDateFormat212 = std::make_unique<SimpleDateTimeFormat>("MMMMEd", "my", false, errCode);
    EXPECT_EQ(sDateFormat212->Format(timep*1000), "အင်္ဂါနေ့၊ ဧပြီလ ၂၃ ရက်");
    auto sDateFormat213 = std::make_unique<SimpleDateTimeFormat>("MMMd", "my", false, errCode);
    EXPECT_EQ(sDateFormat213->Format(timep*1000), "ဧပြီလ ၂၃ ရက်");
    auto sDateFormat214 = std::make_unique<SimpleDateTimeFormat>("Md", "my", false, errCode);
    EXPECT_EQ(sDateFormat214->Format(timep*1000), "၂၃/ဧပြီ");
    auto sDateFormat215 = std::make_unique<SimpleDateTimeFormat>("h", "my", false, errCode);
    EXPECT_EQ(sDateFormat215->Format(timep*1000), "၁၂ နံနက်");
    auto sDateFormat216 = std::make_unique<SimpleDateTimeFormat>("hm", "my", false, errCode);
    EXPECT_EQ(sDateFormat216->Format(timep*1000), "၁၂:၀၀ နံနက်");
    auto sDateFormat217 = std::make_unique<SimpleDateTimeFormat>("hms", "my", false, errCode);
    EXPECT_EQ(sDateFormat217->Format(timep*1000), "၁၂:၀၀:၀၀ နံနက်");
    auto sDateFormat218 = std::make_unique<SimpleDateTimeFormat>("hmsv", "my", false, errCode);
    EXPECT_EQ(sDateFormat218->Format(timep*1000), "၁၂:၀၀:၀၀ နံနက် တရုတ် အချိန်");
    auto sDateFormat219 = std::make_unique<SimpleDateTimeFormat>("hmv", "my", false, errCode);
    EXPECT_EQ(sDateFormat219->Format(timep*1000), "၁၂:၀၀ နံနက် တရုတ် အချိန်");
    auto sDateFormat220 = std::make_unique<SimpleDateTimeFormat>("yM", "my", false, errCode);
    EXPECT_EQ(sDateFormat220->Format(timep*1000), "ဧပြီ/၂၀၂၄");
    auto sDateFormat221 = std::make_unique<SimpleDateTimeFormat>("yMEd", "my", false, errCode);
    EXPECT_EQ(sDateFormat221->Format(timep*1000), "အင်္ဂါနေ့၊ ၂၃/ဧပြီ/၂၀၂၄");
    auto sDateFormat222 = std::make_unique<SimpleDateTimeFormat>("yMMM", "my", false, errCode);
    EXPECT_EQ(sDateFormat222->Format(timep*1000), "၂၀၂၄ ခုနှစ် ဧပြီလ");
}

/**
 * @tc.name: CustomizedDataTest0036
 * @tc.desc: Test Customized Data
 * @tc.type: FUNC
 */
HWTEST_F(CustomizedDataTest, CustomizedDataTest0036, TestSize.Level1)
{
    I18nErrorCode errCode = I18nErrorCode::SUCCESS;
    struct tm tmObj = {.tm_mday = 23, .tm_mon = 3, .tm_year = 124};
    time_t timep = mktime(&tmObj);
    auto sDateFormat45 = std::make_unique<SimpleDateTimeFormat>("hmsv", "ca", false, errCode);
    EXPECT_EQ(sDateFormat45->Format(timep*1000), "12.00.00 a. m. Hora de: Xina");
    auto sDateFormat46 = std::make_unique<SimpleDateTimeFormat>("hmsvvvv", "ca", false, errCode);
    EXPECT_EQ(sDateFormat46->Format(timep*1000), "12:00:00 a. m. (Hora estàndard de la Xina)");
    auto sDateFormat47 = std::make_unique<SimpleDateTimeFormat>("hmv", "ca", false, errCode);
    EXPECT_EQ(sDateFormat47->Format(timep*1000), "12.00 a. m. Hora de: Xina");
    auto sDateFormat48 = std::make_unique<SimpleDateTimeFormat>("yMMM", "ca", false, errCode);
    EXPECT_EQ(sDateFormat48->Format(timep*1000), "abr. 2024");
    auto sDateFormat49 = std::make_unique<SimpleDateTimeFormat>("yMMMd", "ca", false, errCode);
    EXPECT_EQ(sDateFormat49->Format(timep*1000), "23 abr. 2024");
    auto sDateFormat50 = std::make_unique<SimpleDateTimeFormat>("GyMMMEd", "cs", false, errCode);
    EXPECT_EQ(sDateFormat50->Format(timep*1000), "út 23. dub. 2024 n. l.");
    auto sDateFormat51 = std::make_unique<SimpleDateTimeFormat>("GyMMMd", "cs", false, errCode);
    EXPECT_EQ(sDateFormat51->Format(timep*1000), "23. dub 2024 n. l.");
    auto sDateFormat52 = std::make_unique<SimpleDateTimeFormat>("MEd", "cs", false, errCode);
    EXPECT_EQ(sDateFormat52->Format(timep*1000), "út 23.04.");
    auto sDateFormat53 = std::make_unique<SimpleDateTimeFormat>("MMMd", "cs", false, errCode);
    EXPECT_EQ(sDateFormat53->Format(timep*1000), "23. dub");
    auto sDateFormat54 = std::make_unique<SimpleDateTimeFormat>("Md", "cs", false, errCode);
    EXPECT_EQ(sDateFormat54->Format(timep*1000), "23.04.");
    auto sDateFormat55 = std::make_unique<SimpleDateTimeFormat>("yM", "cs", false, errCode);
    EXPECT_EQ(sDateFormat55->Format(timep*1000), "04.2024");
    auto sDateFormat56 = std::make_unique<SimpleDateTimeFormat>("yMEd", "cs", false, errCode);
    EXPECT_EQ(sDateFormat56->Format(timep*1000), "út 23.04.2024");
    auto sDateFormat57 = std::make_unique<SimpleDateTimeFormat>("yMMMEd", "cs", false, errCode);
    EXPECT_EQ(sDateFormat57->Format(timep*1000), "út 23. dub 2024");
    auto sDateFormat58 = std::make_unique<SimpleDateTimeFormat>("yMMMd", "cs", false, errCode);
    EXPECT_EQ(sDateFormat58->Format(timep*1000), "23. dub 2024");
    auto sDateFormat59 = std::make_unique<SimpleDateTimeFormat>("yMd", "cs", false, errCode);
    EXPECT_EQ(sDateFormat59->Format(timep*1000), "23.04.2024");
    auto sDateFormat60 = std::make_unique<SimpleDateTimeFormat>("Ed", "da", false, errCode);
    EXPECT_EQ(sDateFormat60->Format(timep*1000), "tirs. d. 23.");
    auto sDateFormat61 = std::make_unique<SimpleDateTimeFormat>("GyMMMEd", "da", false, errCode);
    EXPECT_EQ(sDateFormat61->Format(timep*1000), "tirs. d. 23. apr. 2024 e.Kr.");
    auto sDateFormat62 = std::make_unique<SimpleDateTimeFormat>("M", "da", false, errCode);
    EXPECT_EQ(sDateFormat62->Format(timep*1000), "04");
    auto sDateFormat63 = std::make_unique<SimpleDateTimeFormat>("MEd", "da", false, errCode);
    EXPECT_EQ(sDateFormat63->Format(timep*1000), "tirs. 23/04");
    auto sDateFormat64 = std::make_unique<SimpleDateTimeFormat>("MMMEd", "da", false, errCode);
    EXPECT_EQ(sDateFormat64->Format(timep*1000), "tirs. d. 23. apr.");
    auto sDateFormat65 = std::make_unique<SimpleDateTimeFormat>("MMMMEd", "da", false, errCode);
    EXPECT_EQ(sDateFormat65->Format(timep*1000), "tirs. d. 23. april");
    auto sDateFormat66 = std::make_unique<SimpleDateTimeFormat>("Md", "da", false, errCode);
    EXPECT_EQ(sDateFormat66->Format(timep*1000), "23/04");
    auto sDateFormat67 = std::make_unique<SimpleDateTimeFormat>("yM", "da", false, errCode);
    EXPECT_EQ(sDateFormat67->Format(timep*1000), "04/2024");
}

/**
 * @tc.name: CustomizedDataTest0037
 * @tc.desc: Test Customized Data
 * @tc.type: FUNC
 */
HWTEST_F(CustomizedDataTest, CustomizedDataTest0037, TestSize.Level1)
{
    I18nErrorCode errCode = I18nErrorCode::SUCCESS;
    struct tm tmObj = {.tm_mday = 23, .tm_mon = 3, .tm_year = 124};
    time_t timep = mktime(&tmObj);
    auto sDateFormat68 = std::make_unique<SimpleDateTimeFormat>("yMEd", "da", false, errCode);
    EXPECT_EQ(sDateFormat68->Format(timep*1000), "tirs. 23/04/2024");
    auto sDateFormat69 = std::make_unique<SimpleDateTimeFormat>("yMMMEd", "da", false, errCode);
    EXPECT_EQ(sDateFormat69->Format(timep*1000), "tirs. d. 23. apr. 2024");
    auto sDateFormat70 = std::make_unique<SimpleDateTimeFormat>("yMd", "da", false, errCode);
    EXPECT_EQ(sDateFormat70->Format(timep*1000), "23/04/2024");
    auto sDateFormat71 = std::make_unique<SimpleDateTimeFormat>("MMM", "el", false, errCode);
    EXPECT_EQ(sDateFormat71->Format(timep*1000), "Απρ");
    auto sDateFormat72 = std::make_unique<SimpleDateTimeFormat>("yMMM", "el", false, errCode);
    EXPECT_EQ(sDateFormat72->Format(timep*1000), "Απρ 2024");
    auto sDateFormat73 = std::make_unique<SimpleDateTimeFormat>("MEd", "en-IE", false, errCode);
    EXPECT_EQ(sDateFormat73->Format(timep*1000), "Tue 23/04");
    auto sDateFormat74 = std::make_unique<SimpleDateTimeFormat>("Md", "en-IE", false, errCode);
    EXPECT_EQ(sDateFormat74->Format(timep*1000), "23/04");
    auto sDateFormat75 = std::make_unique<SimpleDateTimeFormat>("yMEd", "en-IE", false, errCode);
    EXPECT_EQ(sDateFormat75->Format(timep*1000), "Tue 23/04/2024");
    auto sDateFormat76 = std::make_unique<SimpleDateTimeFormat>("yMd", "en-IE", false, errCode);
    EXPECT_EQ(sDateFormat76->Format(timep*1000), "23/04/2024");
    auto sDateFormat77 = std::make_unique<SimpleDateTimeFormat>("Md", "en-NZ", false, errCode);
    EXPECT_EQ(sDateFormat77->Format(timep*1000), "23/04");
    auto sDateFormat78 = std::make_unique<SimpleDateTimeFormat>("yMMMEd", "en-ZW", false, errCode);
    EXPECT_EQ(sDateFormat78->Format(timep*1000), "Tue 23 Apr 2024");
    auto sDateFormat79 = std::make_unique<SimpleDateTimeFormat>("yMMMd", "en-ZW", false, errCode);
    EXPECT_EQ(sDateFormat79->Format(timep*1000), "23 Apr 2024");
    auto sDateFormat80 = std::make_unique<SimpleDateTimeFormat>("MMMdd", "es-419", false, errCode);
    EXPECT_EQ(sDateFormat80->Format(timep*1000), "23 abr");
    auto sDateFormat81 = std::make_unique<SimpleDateTimeFormat>("yMEd", "es-419", false, errCode);
    EXPECT_EQ(sDateFormat81->Format(timep*1000), "mar, 23/4/2024");
    auto sDateFormat82 = std::make_unique<SimpleDateTimeFormat>("yMMMEd", "es-419", false, errCode);
    EXPECT_EQ(sDateFormat82->Format(timep*1000), "mar, 23 abr 2024");
    auto sDateFormat83 = std::make_unique<SimpleDateTimeFormat>("MEd", "es-AR", false, errCode);
    EXPECT_EQ(sDateFormat83->Format(timep*1000), "mar 23/4");
    auto sDateFormat84 = std::make_unique<SimpleDateTimeFormat>("yM", "es-AR", false, errCode);
    EXPECT_EQ(sDateFormat84->Format(timep*1000), "4/2024");
    auto sDateFormat85 = std::make_unique<SimpleDateTimeFormat>("yMMMd", "es-AR", false, errCode);
    EXPECT_EQ(sDateFormat85->Format(timep*1000), "23 abr 2024");
    auto sDateFormat86 = std::make_unique<SimpleDateTimeFormat>("MMMEd", "es-CO", false, errCode);
    EXPECT_EQ(sDateFormat86->Format(timep*1000), "mar, 23 abr");
    auto sDateFormat87 = std::make_unique<SimpleDateTimeFormat>("MMMd", "es-CO", false, errCode);
    EXPECT_EQ(sDateFormat87->Format(timep*1000), "23 abr");
    auto sDateFormat88 = std::make_unique<SimpleDateTimeFormat>("MMMdd", "es-CO", false, errCode);
    EXPECT_EQ(sDateFormat88->Format(timep*1000), "23 abr");
    auto sDateFormat89 = std::make_unique<SimpleDateTimeFormat>("yMMM", "es-CO", false, errCode);
    EXPECT_EQ(sDateFormat89->Format(timep*1000), "abr 2024");
}

/**
 * @tc.name: CustomizedDataTest0038
 * @tc.desc: Test Customized Data
 * @tc.type: FUNC
 */
HWTEST_F(CustomizedDataTest, CustomizedDataTest0038, TestSize.Level1)
{
    I18nErrorCode errCode = I18nErrorCode::SUCCESS;
    struct tm tmObj = {.tm_mday = 23, .tm_mon = 3, .tm_year = 124};
    time_t timep = mktime(&tmObj);
    auto sDateFormat90 = std::make_unique<SimpleDateTimeFormat>("yMMMd", "es-CO", false, errCode);
    EXPECT_EQ(sDateFormat90->Format(timep*1000), "23 abr 2024");
    auto sDateFormat91 = std::make_unique<SimpleDateTimeFormat>("EHm", "es-MX", false, errCode);
    EXPECT_EQ(sDateFormat91->Format(timep*1000), "mar 0:00");
    auto sDateFormat92 = std::make_unique<SimpleDateTimeFormat>("EHms", "es-MX", false, errCode);
    EXPECT_EQ(sDateFormat92->Format(timep*1000), "mar 0:00:00");
    auto sDateFormat93 = std::make_unique<SimpleDateTimeFormat>("yMEd", "es-PA", false, errCode);
    EXPECT_EQ(sDateFormat93->Format(timep*1000), "mar, 23/04/2024");
    auto sDateFormat94 = std::make_unique<SimpleDateTimeFormat>("MMMEd", "eu", false, errCode);
    EXPECT_EQ(sDateFormat94->Format(timep*1000), "api. 23a, ar.");
    auto sDateFormat95 = std::make_unique<SimpleDateTimeFormat>("MMMMd", "eu", false, errCode);
    EXPECT_EQ(sDateFormat95->Format(timep*1000), "apirilaren 23a");
    auto sDateFormat96 = std::make_unique<SimpleDateTimeFormat>("MMMd", "eu", false, errCode);
    EXPECT_EQ(sDateFormat96->Format(timep*1000), "api. 23a");
    auto sDateFormat97 = std::make_unique<SimpleDateTimeFormat>("yMMMEd", "eu", false, errCode);
    EXPECT_EQ(sDateFormat97->Format(timep*1000), "2024 api. 23a, ar.");
    auto sDateFormat98 = std::make_unique<SimpleDateTimeFormat>("yMMMMEd", "eu", false, errCode);
    EXPECT_EQ(sDateFormat98->Format(timep*1000), "2024(e)ko apirilaren 23a, ar.");
    auto sDateFormat99 = std::make_unique<SimpleDateTimeFormat>("yMMMMd", "eu", false, errCode);
    EXPECT_EQ(sDateFormat99->Format(timep*1000), "2024(e)ko apirilaren 23a");
    auto sDateFormat100 = std::make_unique<SimpleDateTimeFormat>("yMMMd", "eu", false, errCode);
    EXPECT_EQ(sDateFormat100->Format(timep*1000), "2024 api. 23a");
    auto sDateFormat101 = std::make_unique<SimpleDateTimeFormat>("EHm", "fr-CA", false, errCode);
    EXPECT_EQ(sDateFormat101->Format(timep*1000), "mar. 00:00");
    auto sDateFormat102 = std::make_unique<SimpleDateTimeFormat>("EHms", "fr-CA", false, errCode);
    EXPECT_EQ(sDateFormat102->Format(timep*1000), "mar. 00:00:00");
    auto sDateFormat103 = std::make_unique<SimpleDateTimeFormat>("Ehm", "fr-CA", false, errCode);
    EXPECT_EQ(sDateFormat103->Format(timep*1000), "mar. 12:00 a.m.");
    auto sDateFormat104 = std::make_unique<SimpleDateTimeFormat>("Ehms", "fr-CA", false, errCode);
    EXPECT_EQ(sDateFormat104->Format(timep*1000), "mar. 12:00:00 a.m.");
    auto sDateFormat105 = std::make_unique<SimpleDateTimeFormat>("Hm", "fr-CA", false, errCode);
    EXPECT_EQ(sDateFormat105->Format(timep*1000), "00:00");
    auto sDateFormat106 = std::make_unique<SimpleDateTimeFormat>("Hms", "fr-CA", false, errCode);
    EXPECT_EQ(sDateFormat106->Format(timep*1000), "00:00:00");
    auto sDateFormat107 = std::make_unique<SimpleDateTimeFormat>("Hmsv", "fr-CA", false, errCode);
    EXPECT_EQ(sDateFormat107->Format(timep*1000), "00:00:00 heure : Chine");
    auto sDateFormat108 = std::make_unique<SimpleDateTimeFormat>("Hmv", "fr-CA", false, errCode);
    EXPECT_EQ(sDateFormat108->Format(timep*1000), "00:00 heure : Chine");
    auto sDateFormat109 = std::make_unique<SimpleDateTimeFormat>("h", "fr-CA", false, errCode);
    EXPECT_EQ(sDateFormat109->Format(timep*1000), "12 a.m.");
    auto sDateFormat110 = std::make_unique<SimpleDateTimeFormat>("hm", "fr-CA", false, errCode);
    EXPECT_EQ(sDateFormat110->Format(timep*1000), "12:00 a.m.");
    auto sDateFormat111 = std::make_unique<SimpleDateTimeFormat>("hms", "fr-CA", false, errCode);
    EXPECT_EQ(sDateFormat111->Format(timep*1000), "12:00:00 a.m.");
}

/**
 * @tc.name: CustomizedDataTest0039
 * @tc.desc: Test Customized Data
 * @tc.type: FUNC
 */
HWTEST_F(CustomizedDataTest, CustomizedDataTest0039, TestSize.Level1)
{
    I18nErrorCode errCode = I18nErrorCode::SUCCESS;
    struct tm tmObj = {.tm_mday = 23, .tm_mon = 3, .tm_year = 124};
    time_t timep = mktime(&tmObj);
    auto sDateFormat112 = std::make_unique<SimpleDateTimeFormat>("hmsv", "fr-CA", false, errCode);
    EXPECT_EQ(sDateFormat112->Format(timep*1000), "12:00:00 a.m. heure : Chine");
    auto sDateFormat113 = std::make_unique<SimpleDateTimeFormat>("hmv", "fr-CA", false, errCode);
    EXPECT_EQ(sDateFormat113->Format(timep*1000), "12:00 a.m. heure : Chine");
    auto sDateFormat114 = std::make_unique<SimpleDateTimeFormat>("ms", "fr-CA", false, errCode);
    EXPECT_EQ(sDateFormat114->Format(timep*1000), "00:00");
    auto sDateFormat115 = std::make_unique<SimpleDateTimeFormat>("GyMMMEd", "gl", false, errCode);
    EXPECT_EQ(sDateFormat115->Format(timep*1000), "mar, 23/04/2024 d.C.");
    auto sDateFormat116 = std::make_unique<SimpleDateTimeFormat>("MMMEd", "gl", false, errCode);
    EXPECT_EQ(sDateFormat116->Format(timep*1000), "mar, 23 abr");
    auto sDateFormat117 = std::make_unique<SimpleDateTimeFormat>("MMMMEd", "gl", false, errCode);
    EXPECT_EQ(sDateFormat117->Format(timep*1000), "mar, 23 de abril");
    auto sDateFormat118 = std::make_unique<SimpleDateTimeFormat>("MMMd", "gl", false, errCode);
    EXPECT_EQ(sDateFormat118->Format(timep*1000), "23 abr");
    auto sDateFormat119 = std::make_unique<SimpleDateTimeFormat>("yMEd", "gl", false, errCode);
    EXPECT_EQ(sDateFormat119->Format(timep*1000), "mar, 23/4/2024");
    auto sDateFormat120 = std::make_unique<SimpleDateTimeFormat>("yMMM", "gl", false, errCode);
    EXPECT_EQ(sDateFormat120->Format(timep*1000), "abr 2024");
    auto sDateFormat121 = std::make_unique<SimpleDateTimeFormat>("yMMMEd", "gl", false, errCode);
    EXPECT_EQ(sDateFormat121->Format(timep*1000), "mar, 23 abr 2024");
    auto sDateFormat122 = std::make_unique<SimpleDateTimeFormat>("yMMMM", "gl", false, errCode);
    EXPECT_EQ(sDateFormat122->Format(timep*1000), "abril de 2024");
    auto sDateFormat123 = std::make_unique<SimpleDateTimeFormat>("yMMMd", "gl", false, errCode);
    EXPECT_EQ(sDateFormat123->Format(timep*1000), "23 abr 2024");
    auto sDateFormat124 = std::make_unique<SimpleDateTimeFormat>("Md", "hu", false, errCode);
    EXPECT_EQ(sDateFormat124->Format(timep*1000), "04. 23.");
    auto sDateFormat125 = std::make_unique<SimpleDateTimeFormat>("yM", "hu", false, errCode);
    EXPECT_EQ(sDateFormat125->Format(timep*1000), "2024. 04.");
    auto sDateFormat126 = std::make_unique<SimpleDateTimeFormat>("MMMEd", "hy", false, errCode);
    EXPECT_EQ(sDateFormat126->Format(timep*1000), "23 ապր, երք");
    auto sDateFormat127 = std::make_unique<SimpleDateTimeFormat>("MMMd", "hy", false, errCode);
    EXPECT_EQ(sDateFormat127->Format(timep*1000), "23 ապր");
    auto sDateFormat128 = std::make_unique<SimpleDateTimeFormat>("yMEd", "hy", false, errCode);
    EXPECT_EQ(sDateFormat128->Format(timep*1000), "23.04.2024, երք");
    auto sDateFormat129 = std::make_unique<SimpleDateTimeFormat>("yMMM", "hy", false, errCode);
    EXPECT_EQ(sDateFormat129->Format(timep*1000), "ապր, 2024 թ.");
    auto sDateFormat130 = std::make_unique<SimpleDateTimeFormat>("yMMMEd", "hy", false, errCode);
    EXPECT_EQ(sDateFormat130->Format(timep*1000), "23 ապր, 2024 թ. երք");
    auto sDateFormat131 = std::make_unique<SimpleDateTimeFormat>("yMMMM", "hy", false, errCode);
    EXPECT_EQ(sDateFormat131->Format(timep*1000), "ապրիլի, 2024 թ.");
    auto sDateFormat132 = std::make_unique<SimpleDateTimeFormat>("yMMMd", "hy", false, errCode);
    EXPECT_EQ(sDateFormat132->Format(timep*1000), "23 ապր, 2024 թ.");
    auto sDateFormat133 = std::make_unique<SimpleDateTimeFormat>("Hmsv", "is", false, errCode);
    EXPECT_EQ(sDateFormat133->Format(timep*1000), "00:00:00 Kína");
}

/**
 * @tc.name: CustomizedDataTest0040
 * @tc.desc: Test Customized Data
 * @tc.type: FUNC
 */
HWTEST_F(CustomizedDataTest, CustomizedDataTest0040, TestSize.Level1)
{
    I18nErrorCode errCode = I18nErrorCode::SUCCESS;
    struct tm tmObj = {.tm_mday = 23, .tm_mon = 3, .tm_year = 124};
    time_t timep = mktime(&tmObj);
    auto sDateFormat134 = std::make_unique<SimpleDateTimeFormat>("Hmv", "is", false, errCode);
    EXPECT_EQ(sDateFormat134->Format(timep*1000), "00:00 Kína");
    auto sDateFormat135 = std::make_unique<SimpleDateTimeFormat>("MEEEEd", "ja", false, errCode);
    EXPECT_EQ(sDateFormat135->Format(timep*1000), "04/23火曜日");
    auto sDateFormat136 = std::make_unique<SimpleDateTimeFormat>("MEd", "ja", false, errCode);
    EXPECT_EQ(sDateFormat136->Format(timep*1000), "04/23(火)");
    auto sDateFormat137 = std::make_unique<SimpleDateTimeFormat>("yMEEEEd", "ja", false, errCode);
    EXPECT_EQ(sDateFormat137->Format(timep*1000), "2024/04/23火曜日");
    auto sDateFormat138 = std::make_unique<SimpleDateTimeFormat>("yMEd", "ja", false, errCode);
    EXPECT_EQ(sDateFormat138->Format(timep*1000), "2024/04/23(火)");
    auto sDateFormat139 = std::make_unique<SimpleDateTimeFormat>("Ed", "jv", false, errCode);
    EXPECT_EQ(sDateFormat139->Format(timep*1000), "23 Sel");
    auto sDateFormat140 = std::make_unique<SimpleDateTimeFormat>("GyMMMEd", "jv", false, errCode);
    EXPECT_EQ(sDateFormat140->Format(timep*1000), "Sel, 23 Apr, 2024 M");
    auto sDateFormat141 = std::make_unique<SimpleDateTimeFormat>("GyMMMd", "jv", false, errCode);
    EXPECT_EQ(sDateFormat141->Format(timep*1000), "23 Apr, 2024 M");
    auto sDateFormat142 = std::make_unique<SimpleDateTimeFormat>("MEd", "jv", false, errCode);
    EXPECT_EQ(sDateFormat142->Format(timep*1000), "Sel, 23/4");
    auto sDateFormat143 = std::make_unique<SimpleDateTimeFormat>("Md", "jv", false, errCode);
    EXPECT_EQ(sDateFormat143->Format(timep*1000), "23/4");
    auto sDateFormat144 = std::make_unique<SimpleDateTimeFormat>("yM", "jv", false, errCode);
    EXPECT_EQ(sDateFormat144->Format(timep*1000), "4/2024");
    auto sDateFormat145 = std::make_unique<SimpleDateTimeFormat>("yMEd", "jv", false, errCode);
    EXPECT_EQ(sDateFormat145->Format(timep*1000), "Sel, 23/4/2024");
    auto sDateFormat146 = std::make_unique<SimpleDateTimeFormat>("yMMMEd", "jv", false, errCode);
    EXPECT_EQ(sDateFormat146->Format(timep*1000), "Sel, 23 Apr, 2024");
    auto sDateFormat147 = std::make_unique<SimpleDateTimeFormat>("yMMMd", "jv", false, errCode);
    EXPECT_EQ(sDateFormat147->Format(timep*1000), "23 Apr, 2024");
    auto sDateFormat148 = std::make_unique<SimpleDateTimeFormat>("yMd", "jv", false, errCode);
    EXPECT_EQ(sDateFormat148->Format(timep*1000), "23/4/2024");
    auto sDateFormat149 = std::make_unique<SimpleDateTimeFormat>("yMMM", "ka", false, errCode);
    EXPECT_EQ(sDateFormat149->Format(timep*1000), "აპრ, 2024");
    auto sDateFormat150 = std::make_unique<SimpleDateTimeFormat>("yMMMEd", "ka", false, errCode);
    EXPECT_EQ(sDateFormat150->Format(timep*1000), "სამ, 23 აპრ, 2024");
    auto sDateFormat151 = std::make_unique<SimpleDateTimeFormat>("yMMMd", "ka", false, errCode);
    EXPECT_EQ(sDateFormat151->Format(timep*1000), "23 აპრ, 2024");
    auto sDateFormat152 = std::make_unique<SimpleDateTimeFormat>("Gy", "km", false, errCode);
    EXPECT_EQ(sDateFormat152->Format(timep*1000), "2024 នៃ គ.ស.");
    auto sDateFormat153 = std::make_unique<SimpleDateTimeFormat>("GyMMM", "km", false, errCode);
    EXPECT_EQ(sDateFormat153->Format(timep*1000), "ខែមេសា 2024 នៃ គ.ស.");
    auto sDateFormat154 = std::make_unique<SimpleDateTimeFormat>("GyMMMEd", "km", false, errCode);
    EXPECT_EQ(sDateFormat154->Format(timep*1000), "អង្គារ 23 ខែមេសា 2024 នៃ គ.ស.");
    auto sDateFormat155 = std::make_unique<SimpleDateTimeFormat>("GyMMMd", "km", false, errCode);
    EXPECT_EQ(sDateFormat155->Format(timep*1000), "23 ខែមេសា 2024 នៃ គ.ស.");
}

/**
 * @tc.name: CustomizedDataTest0041
 * @tc.desc: Test Customized Data
 * @tc.type: FUNC
 */
HWTEST_F(CustomizedDataTest, CustomizedDataTest0041, TestSize.Level1)
{
    I18nErrorCode errCode = I18nErrorCode::SUCCESS;
    struct tm tmObj = {.tm_mday = 23, .tm_mon = 3, .tm_year = 124};
    time_t timep = mktime(&tmObj);
    auto sDateFormat156 = std::make_unique<SimpleDateTimeFormat>("yMMMM", "km", false, errCode);
    EXPECT_EQ(sDateFormat156->Format(timep*1000), "ខែមេសា ឆ្នាំ2024");
    auto sDateFormat157 = std::make_unique<SimpleDateTimeFormat>("MMMEd", "kn", false, errCode);
    EXPECT_EQ(sDateFormat157->Format(timep*1000), "23 ಏಪ್ರಿ, ಮಂಗಳ");
    auto sDateFormat158 = std::make_unique<SimpleDateTimeFormat>("MMdd", "kn", false, errCode);
    EXPECT_EQ(sDateFormat158->Format(timep*1000), "23/04");
    auto sDateFormat159 = std::make_unique<SimpleDateTimeFormat>("yMEd", "kn", false, errCode);
    EXPECT_EQ(sDateFormat159->Format(timep*1000), "23/4/2024, ಮಂಗಳ");
    auto sDateFormat160 = std::make_unique<SimpleDateTimeFormat>("yMM", "kn", false, errCode);
    EXPECT_EQ(sDateFormat160->Format(timep*1000), "4/2024");
    auto sDateFormat161 = std::make_unique<SimpleDateTimeFormat>("yMMMEd", "kn", false, errCode);
    EXPECT_EQ(sDateFormat161->Format(timep*1000), "23 ಏಪ್ರಿ 2024, ಮಂಗಳ");
    auto sDateFormat162 = std::make_unique<SimpleDateTimeFormat>("yMMMd", "kn", false, errCode);
    EXPECT_EQ(sDateFormat162->Format(timep*1000), "23 ಏಪ್ರಿ 2024");
    auto sDateFormat163 = std::make_unique<SimpleDateTimeFormat>("Hms", "ko", false, errCode);
    EXPECT_EQ(sDateFormat163->Format(timep*1000), "00:00:00");
    auto sDateFormat164 = std::make_unique<SimpleDateTimeFormat>("MEd", "lo", false, errCode);
    EXPECT_EQ(sDateFormat164->Format(timep*1000), "ຄານ 23/4");
    auto sDateFormat165 = std::make_unique<SimpleDateTimeFormat>("yMEd", "lo", false, errCode);
    EXPECT_EQ(sDateFormat165->Format(timep*1000), "ຄານ 23/4/2024");
    auto sDateFormat166 = std::make_unique<SimpleDateTimeFormat>("yMMMEd", "lo", false, errCode);
    EXPECT_EQ(sDateFormat166->Format(timep*1000), "ຄານ 23 ມ.ສ. 2024");
    auto sDateFormat167 = std::make_unique<SimpleDateTimeFormat>("MMM", "lt", false, errCode);
    EXPECT_EQ(sDateFormat167->Format(timep*1000), "bal.");
    auto sDateFormat168 = std::make_unique<SimpleDateTimeFormat>("MMMEd", "lt", false, errCode);
    EXPECT_EQ(sDateFormat168->Format(timep*1000), "bal. 23 d., an");
    auto sDateFormat169 = std::make_unique<SimpleDateTimeFormat>("MMMd", "lt", false, errCode);
    EXPECT_EQ(sDateFormat169->Format(timep*1000), "bal. 23 d.");
    auto sDateFormat170 = std::make_unique<SimpleDateTimeFormat>("Md", "lt", false, errCode);
    EXPECT_EQ(sDateFormat170->Format(timep*1000), "04-23");
    auto sDateFormat171 = std::make_unique<SimpleDateTimeFormat>("yMMM", "lt", false, errCode);
    EXPECT_EQ(sDateFormat171->Format(timep*1000), "2024 m. bal.");
    auto sDateFormat172 = std::make_unique<SimpleDateTimeFormat>("yMMMEd", "lt", false, errCode);
    EXPECT_EQ(sDateFormat172->Format(timep*1000), "2024 m. bal. 23 d., an");
    auto sDateFormat173 = std::make_unique<SimpleDateTimeFormat>("yMMMd", "lt", false, errCode);
    EXPECT_EQ(sDateFormat173->Format(timep*1000), "2024 m. bal. 23 d.");
    auto sDateFormat174 = std::make_unique<SimpleDateTimeFormat>("y", "lv", false, errCode);
    EXPECT_EQ(sDateFormat174->Format(timep*1000), "2024. gada");
    auto sDateFormat175 = std::make_unique<SimpleDateTimeFormat>("yMEd", "lv", false, errCode);
    EXPECT_EQ(sDateFormat175->Format(timep*1000), "otrd., 23.04.2024.");
    auto sDateFormat176 = std::make_unique<SimpleDateTimeFormat>("yMMM", "lv", false, errCode);
    EXPECT_EQ(sDateFormat176->Format(timep*1000), "2024. gada apr.");
    auto sDateFormat177 = std::make_unique<SimpleDateTimeFormat>("yMMMEd", "lv", false, errCode);
    EXPECT_EQ(sDateFormat177->Format(timep*1000), "otrd., 2024. gada 23. apr.");
}

/**
 * @tc.name: CustomizedDataTest0042
 * @tc.desc: Test Customized Data
 * @tc.type: FUNC
 */
HWTEST_F(CustomizedDataTest, CustomizedDataTest0042, TestSize.Level1)
{
    I18nErrorCode errCode = I18nErrorCode::SUCCESS;
    struct tm tmObj = {.tm_mday = 23, .tm_mon = 3, .tm_year = 124};
    time_t timep = mktime(&tmObj);
    auto sDateFormat178 = std::make_unique<SimpleDateTimeFormat>("yMMMM", "lv", false, errCode);
    EXPECT_EQ(sDateFormat178->Format(timep*1000), "2024. gada aprīlis");
    auto sDateFormat179 = std::make_unique<SimpleDateTimeFormat>("yMMMd", "lv", false, errCode);
    EXPECT_EQ(sDateFormat179->Format(timep*1000), "2024. gada 23. apr.");
    auto sDateFormat180 = std::make_unique<SimpleDateTimeFormat>("yMd", "lv", false, errCode);
    EXPECT_EQ(sDateFormat180->Format(timep*1000), "23.04.2024.");
    auto sDateFormat181 = std::make_unique<SimpleDateTimeFormat>("Hmsv", "mn", false, errCode);
    EXPECT_EQ(sDateFormat181->Format(timep*1000), "00:00:00 Хятад-н цаг");
    auto sDateFormat182 = std::make_unique<SimpleDateTimeFormat>("Hmv", "mn", false, errCode);
    EXPECT_EQ(sDateFormat182->Format(timep*1000), "00:00 Хятад-н цаг");
    auto sDateFormat183 = std::make_unique<SimpleDateTimeFormat>("M", "mn", false, errCode);
    EXPECT_EQ(sDateFormat183->Format(timep*1000), "4");
    auto sDateFormat184 = std::make_unique<SimpleDateTimeFormat>("MEd", "mn", false, errCode);
    EXPECT_EQ(sDateFormat184->Format(timep*1000), "4-23, Мя");
    auto sDateFormat185 = std::make_unique<SimpleDateTimeFormat>("MMMEd", "mn", false, errCode);
    EXPECT_EQ(sDateFormat185->Format(timep*1000), "4-р сар 23, Мя");
    auto sDateFormat186 = std::make_unique<SimpleDateTimeFormat>("MMMMd", "mn", false, errCode);
    EXPECT_EQ(sDateFormat186->Format(timep*1000), "Дөрөвдүгээр сарын 23 өдөр");
    auto sDateFormat187 = std::make_unique<SimpleDateTimeFormat>("MMMd", "mn", false, errCode);
    EXPECT_EQ(sDateFormat187->Format(timep*1000), "4-р сар 23");
    auto sDateFormat188 = std::make_unique<SimpleDateTimeFormat>("Md", "mn", false, errCode);
    EXPECT_EQ(sDateFormat188->Format(timep*1000), "4/23");
    auto sDateFormat189 = std::make_unique<SimpleDateTimeFormat>("h", "mn", false, errCode);
    EXPECT_EQ(sDateFormat189->Format(timep*1000), "12 ү.ө.");
    auto sDateFormat190 = std::make_unique<SimpleDateTimeFormat>("hmsv", "mn", false, errCode);
    EXPECT_EQ(sDateFormat190->Format(timep*1000), "12:00:00 ү.ө. Хятад-н цаг");
    auto sDateFormat191 = std::make_unique<SimpleDateTimeFormat>("hmv", "mn", false, errCode);
    EXPECT_EQ(sDateFormat191->Format(timep*1000), "12:00 ү.ө. Хятад-н цаг");
    auto sDateFormat192 = std::make_unique<SimpleDateTimeFormat>("yM", "mn", false, errCode);
    EXPECT_EQ(sDateFormat192->Format(timep*1000), "2024-4");
    auto sDateFormat193 = std::make_unique<SimpleDateTimeFormat>("yMEd", "mn", false, errCode);
    EXPECT_EQ(sDateFormat193->Format(timep*1000), "2024-4-23, Мя");
    auto sDateFormat194 = std::make_unique<SimpleDateTimeFormat>("yMMM", "mn", false, errCode);
    EXPECT_EQ(sDateFormat194->Format(timep*1000), "2024 оны 4-р сар");
    auto sDateFormat195 = std::make_unique<SimpleDateTimeFormat>("yMMMEd", "mn", false, errCode);
    EXPECT_EQ(sDateFormat195->Format(timep*1000), "2024 оны 4-р сар 23, Мя");
    auto sDateFormat196 = std::make_unique<SimpleDateTimeFormat>("yMMMM", "mn", false, errCode);
    EXPECT_EQ(sDateFormat196->Format(timep*1000), "2024 оны Дөрөвдүгээр сар");
    auto sDateFormat197 = std::make_unique<SimpleDateTimeFormat>("yMMMd", "mn", false, errCode);
    EXPECT_EQ(sDateFormat197->Format(timep*1000), "2024 оны 4-р сар 23");
    auto sDateFormat198 = std::make_unique<SimpleDateTimeFormat>("yMd", "mn", false, errCode);
    EXPECT_EQ(sDateFormat198->Format(timep*1000), "2024-4-23");
    auto sDateFormat199 = std::make_unique<SimpleDateTimeFormat>("MMdd", "mr", false, errCode);
    EXPECT_EQ(sDateFormat199->Format(timep*1000), "२३/०४");
    auto sDateFormat200 = std::make_unique<SimpleDateTimeFormat>("yMM", "mr", false, errCode);
    EXPECT_EQ(sDateFormat200->Format(timep*1000), "०४/२०२४");
}

/**
 * @tc.name: CustomizedDataTest0043
 * @tc.desc: Test Customized Data
 * @tc.type: FUNC
 */
HWTEST_F(CustomizedDataTest, CustomizedDataTest0043, TestSize.Level1)
{
    I18nErrorCode errCode = I18nErrorCode::SUCCESS;
    struct tm tmObj = {.tm_mday = 23, .tm_mon = 3, .tm_year = 124};
    time_t timep = mktime(&tmObj);
    auto sDateFormat223 = std::make_unique<SimpleDateTimeFormat>("yMMMEd", "my", false, errCode);
    EXPECT_EQ(sDateFormat223->Format(timep*1000), "အင်္ဂါနေ့၊ ၂၀၂၄ ခုနှစ် ဧပြီလ ၂၃ ရက်");
    auto sDateFormat224 = std::make_unique<SimpleDateTimeFormat>("yMMMd", "my", false, errCode);
    EXPECT_EQ(sDateFormat224->Format(timep*1000), "၂၀၂၄ ခုနှစ် ဧပြီလ ၂၃ ရက်");
    auto sDateFormat225 = std::make_unique<SimpleDateTimeFormat>("yMd", "my", false, errCode);
    EXPECT_EQ(sDateFormat225->Format(timep*1000), "၂၃/ဧပြီ/၂၀၂၄");
    auto sDateFormat226 = std::make_unique<SimpleDateTimeFormat>("MEd", "nl", false, errCode);
    EXPECT_EQ(sDateFormat226->Format(timep*1000), "di 23-04");
    auto sDateFormat227 = std::make_unique<SimpleDateTimeFormat>("Md", "nl", false, errCode);
    EXPECT_EQ(sDateFormat227->Format(timep*1000), "23-04");
    auto sDateFormat228 = std::make_unique<SimpleDateTimeFormat>("yM", "nl", false, errCode);
    EXPECT_EQ(sDateFormat228->Format(timep*1000), "04-2024");
    auto sDateFormat229 = std::make_unique<SimpleDateTimeFormat>("yMEd", "nl", false, errCode);
    EXPECT_EQ(sDateFormat229->Format(timep*1000), "di 23-04-2024");
    auto sDateFormat230 = std::make_unique<SimpleDateTimeFormat>("yMd", "nl", false, errCode);
    EXPECT_EQ(sDateFormat230->Format(timep*1000), "23-04-2024");
    auto sDateFormat231 = std::make_unique<SimpleDateTimeFormat>("MEd", "nl-BE", false, errCode);
    EXPECT_EQ(sDateFormat231->Format(timep*1000), "di 23/04");
    auto sDateFormat232 = std::make_unique<SimpleDateTimeFormat>("Md", "nl-BE", false, errCode);
    EXPECT_EQ(sDateFormat232->Format(timep*1000), "23/04");
    auto sDateFormat233 = std::make_unique<SimpleDateTimeFormat>("yM", "nl-BE", false, errCode);
    EXPECT_EQ(sDateFormat233->Format(timep*1000), "04/2024");
    auto sDateFormat234 = std::make_unique<SimpleDateTimeFormat>("yMEd", "nl-BE", false, errCode);
    EXPECT_EQ(sDateFormat234->Format(timep*1000), "di 23-04-24");
    auto sDateFormat235 = std::make_unique<SimpleDateTimeFormat>("yMd", "nl-BE", false, errCode);
    EXPECT_EQ(sDateFormat235->Format(timep*1000), "23/04/24");
    auto sDateFormat236 = std::make_unique<SimpleDateTimeFormat>("MEd", "no", false, errCode);
    EXPECT_EQ(sDateFormat236->Format(timep*1000), "tir. 23.04.");
    auto sDateFormat237 = std::make_unique<SimpleDateTimeFormat>("MMdd", "no", false, errCode);
    EXPECT_EQ(sDateFormat237->Format(timep*1000), "23.04.");
    auto sDateFormat238 = std::make_unique<SimpleDateTimeFormat>("Md", "no", false, errCode);
    EXPECT_EQ(sDateFormat238->Format(timep*1000), "23.04.");
    auto sDateFormat239 = std::make_unique<SimpleDateTimeFormat>("yM", "no", false, errCode);
    EXPECT_EQ(sDateFormat239->Format(timep*1000), "04.2024");
    auto sDateFormat240 = std::make_unique<SimpleDateTimeFormat>("yMEd", "no", false, errCode);
    EXPECT_EQ(sDateFormat240->Format(timep*1000), "tir. 23.04.2024");
    auto sDateFormat241 = std::make_unique<SimpleDateTimeFormat>("yMd", "no", false, errCode);
    EXPECT_EQ(sDateFormat241->Format(timep*1000), "23.04.2024");
    auto sDateFormat242 = std::make_unique<SimpleDateTimeFormat>("MEd", "or", false, errCode);
    EXPECT_EQ(sDateFormat242->Format(timep*1000), "ମଙ୍ଗଳ, 23-4");
    auto sDateFormat243 = std::make_unique<SimpleDateTimeFormat>("MMMEd", "or", false, errCode);
    EXPECT_EQ(sDateFormat243->Format(timep*1000), "ମଙ୍ଗଳ, 23 ଅପ୍ରେଲ");
    auto sDateFormat244 = std::make_unique<SimpleDateTimeFormat>("Md", "or", false, errCode);
    EXPECT_EQ(sDateFormat244->Format(timep*1000), "23-4");
}

/**
 * @tc.name: CustomizedDataTest0044
 * @tc.desc: Test Customized Data
 * @tc.type: FUNC
 */
HWTEST_F(CustomizedDataTest, CustomizedDataTest0044, TestSize.Level1)
{
    I18nErrorCode errCode = I18nErrorCode::SUCCESS;
    struct tm tmObj = {.tm_mday = 23, .tm_mon = 3, .tm_year = 124};
    time_t timep = mktime(&tmObj);
    auto sDateFormat245 = std::make_unique<SimpleDateTimeFormat>("yM", "or", false, errCode);
    EXPECT_EQ(sDateFormat245->Format(timep*1000), "4-2024");
    auto sDateFormat246 = std::make_unique<SimpleDateTimeFormat>("yMEd", "or", false, errCode);
    EXPECT_EQ(sDateFormat246->Format(timep*1000), "ମଙ୍ଗଳ, 23-4-2024");
    auto sDateFormat247 = std::make_unique<SimpleDateTimeFormat>("yMMMEd", "or", false, errCode);
    EXPECT_EQ(sDateFormat247->Format(timep*1000), "ମଙ୍ଗଳ, 23 ଅପ୍ରେଲ 2024");
    auto sDateFormat248 = std::make_unique<SimpleDateTimeFormat>("yMMMd", "or", false, errCode);
    EXPECT_EQ(sDateFormat248->Format(timep*1000), "23 ଅପ୍ରେଲ 2024");
    auto sDateFormat249 = std::make_unique<SimpleDateTimeFormat>("yMd", "or", false, errCode);
    EXPECT_EQ(sDateFormat249->Format(timep*1000), "23-4-2024");
    auto sDateFormat250 = std::make_unique<SimpleDateTimeFormat>("MEd", "pa", false, errCode);
    EXPECT_EQ(sDateFormat250->Format(timep*1000), "ਮੰਗਲ, 23/4");
    auto sDateFormat251 = std::make_unique<SimpleDateTimeFormat>("MMMd", "pt-PT", false, errCode);
    EXPECT_EQ(sDateFormat251->Format(timep*1000), "23 de abr.");
    auto sDateFormat252 = std::make_unique<SimpleDateTimeFormat>("yMMM", "pt-PT", false, errCode);
    EXPECT_EQ(sDateFormat252->Format(timep*1000), "abr. de 2024");
    auto sDateFormat253 = std::make_unique<SimpleDateTimeFormat>("yMMMEEEEd", "pt-PT", false, errCode);
    EXPECT_EQ(sDateFormat253->Format(timep*1000), "terça-feira, 23 de abr. de 2024");
    auto sDateFormat254 = std::make_unique<SimpleDateTimeFormat>("yMMMEd", "pt-PT", false, errCode);
    EXPECT_EQ(sDateFormat254->Format(timep*1000), "ter., 23 de abr. de 2024");
    auto sDateFormat255 = std::make_unique<SimpleDateTimeFormat>("yMMMd", "pt-PT", false, errCode);
    EXPECT_EQ(sDateFormat255->Format(timep*1000), "23 de abr. de 2024");
    auto sDateFormat256 = std::make_unique<SimpleDateTimeFormat>("MEd", "si", false, errCode);
    EXPECT_EQ(sDateFormat256->Format(timep*1000), "04-23, අඟ");
    auto sDateFormat257 = std::make_unique<SimpleDateTimeFormat>("MMMEd", "si", false, errCode);
    EXPECT_EQ(sDateFormat257->Format(timep*1000), "අප්‍රේල් 23, අඟ");
    auto sDateFormat258 = std::make_unique<SimpleDateTimeFormat>("Md", "si", false, errCode);
    EXPECT_EQ(sDateFormat258->Format(timep*1000), "04-23");
    auto sDateFormat259 = std::make_unique<SimpleDateTimeFormat>("yM", "si", false, errCode);
    EXPECT_EQ(sDateFormat259->Format(timep*1000), "2024-04");
    auto sDateFormat260 = std::make_unique<SimpleDateTimeFormat>("yMEd", "si", false, errCode);
    EXPECT_EQ(sDateFormat260->Format(timep*1000), "2024-04-23, අඟ");
    auto sDateFormat261 = std::make_unique<SimpleDateTimeFormat>("yMd", "si", false, errCode);
    EXPECT_EQ(sDateFormat261->Format(timep*1000), "2024-04-23");
    auto sDateFormat262 = std::make_unique<SimpleDateTimeFormat>("GyMMM", "sk", false, errCode);
    EXPECT_EQ(sDateFormat262->Format(timep*1000), "apr 2024 po Kr.");
    auto sDateFormat263 = std::make_unique<SimpleDateTimeFormat>("GyMMMEd", "sk", false, errCode);
    EXPECT_EQ(sDateFormat263->Format(timep*1000), "ut, 23. apr 2024 po Kr.");
    auto sDateFormat264 = std::make_unique<SimpleDateTimeFormat>("GyMMMMd", "sk", false, errCode);
    EXPECT_EQ(sDateFormat264->Format(timep*1000), "23. apríla 2024 po Kr.");
    auto sDateFormat265 = std::make_unique<SimpleDateTimeFormat>("GyMMMd", "sk", false, errCode);
    EXPECT_EQ(sDateFormat265->Format(timep*1000), "23. apr 2024 po Kr.");
    auto sDateFormat266 = std::make_unique<SimpleDateTimeFormat>("MMMEd", "sk", false, errCode);
    EXPECT_EQ(sDateFormat266->Format(timep*1000), "ut, 23. apr");
}

/**
 * @tc.name: CustomizedDataTest0045
 * @tc.desc: Test Customized Data
 * @tc.type: FUNC
 */
HWTEST_F(CustomizedDataTest, CustomizedDataTest0045, TestSize.Level1)
{
    I18nErrorCode errCode = I18nErrorCode::SUCCESS;
    struct tm tmObj = {.tm_mday = 23, .tm_mon = 3, .tm_year = 124};
    time_t timep = mktime(&tmObj);
    auto sDateFormat267 = std::make_unique<SimpleDateTimeFormat>("MMMMEd", "sk", false, errCode);
    EXPECT_EQ(sDateFormat267->Format(timep*1000), "ut, 23. apríla");
    auto sDateFormat268 = std::make_unique<SimpleDateTimeFormat>("MMMd", "sk", false, errCode);
    EXPECT_EQ(sDateFormat268->Format(timep*1000), "23. apr");
    auto sDateFormat269 = std::make_unique<SimpleDateTimeFormat>("yM", "sk", false, errCode);
    EXPECT_EQ(sDateFormat269->Format(timep*1000), "4. 2024");
    auto sDateFormat270 = std::make_unique<SimpleDateTimeFormat>("yMMM", "sk", false, errCode);
    EXPECT_EQ(sDateFormat270->Format(timep*1000), "apr 2024");
    auto sDateFormat271 = std::make_unique<SimpleDateTimeFormat>("yMMMEd", "sk", false, errCode);
    EXPECT_EQ(sDateFormat271->Format(timep*1000), "ut, 23. apr 2024");
    auto sDateFormat272 = std::make_unique<SimpleDateTimeFormat>("yMMMd", "sk", false, errCode);
    EXPECT_EQ(sDateFormat272->Format(timep*1000), "23. apr 2024");
    auto sDateFormat273 = std::make_unique<SimpleDateTimeFormat>("yM", "sl", false, errCode);
    EXPECT_EQ(sDateFormat273->Format(timep*1000), "4. 2024");
    auto sDateFormat274 = std::make_unique<SimpleDateTimeFormat>("EHm", "sq", false, errCode);
    EXPECT_EQ(sDateFormat274->Format(timep*1000), "Mar, 00:00");
    auto sDateFormat275 = std::make_unique<SimpleDateTimeFormat>("EHms", "sq", false, errCode);
    EXPECT_EQ(sDateFormat275->Format(timep*1000), "Mar, 00:00:00");
    auto sDateFormat276 = std::make_unique<SimpleDateTimeFormat>("Ed", "sq", false, errCode);
    EXPECT_EQ(sDateFormat276->Format(timep*1000), "Mar, 23");
    auto sDateFormat277 = std::make_unique<SimpleDateTimeFormat>("Ehm", "sq", false, errCode);
    EXPECT_EQ(sDateFormat277->Format(timep*1000), "Mar, 12:00 e paradites");
    auto sDateFormat278 = std::make_unique<SimpleDateTimeFormat>("Ehms", "sq", false, errCode);
    EXPECT_EQ(sDateFormat278->Format(timep*1000), "Mar, 12:00:00 e paradites");
    auto sDateFormat279 = std::make_unique<SimpleDateTimeFormat>("GyMMM", "sq", false, errCode);
    EXPECT_EQ(sDateFormat279->Format(timep*1000), "Prill 2024 mb.K.");
    auto sDateFormat280 = std::make_unique<SimpleDateTimeFormat>("GyMMMEd", "sq", false, errCode);
    EXPECT_EQ(sDateFormat280->Format(timep*1000), "Mar, 23 Prill 2024 mb.K.");
    auto sDateFormat281 = std::make_unique<SimpleDateTimeFormat>("GyMMMd", "sq", false, errCode);
    EXPECT_EQ(sDateFormat281->Format(timep*1000), "23 Prill 2024 mb.K.");
    auto sDateFormat282 = std::make_unique<SimpleDateTimeFormat>("MEd", "sq", false, errCode);
    EXPECT_EQ(sDateFormat282->Format(timep*1000), "Mar, 23.4");
    auto sDateFormat283 = std::make_unique<SimpleDateTimeFormat>("MMMEd", "sq", false, errCode);
    EXPECT_EQ(sDateFormat283->Format(timep*1000), "Mar, 23 Prill");
    auto sDateFormat284 = std::make_unique<SimpleDateTimeFormat>("MMMMEd", "sq", false, errCode);
    EXPECT_EQ(sDateFormat284->Format(timep*1000), "Mar, 23 Prill");
    auto sDateFormat285 = std::make_unique<SimpleDateTimeFormat>("MMMd", "sq", false, errCode);
    EXPECT_EQ(sDateFormat285->Format(timep*1000), "23 Prill");
    auto sDateFormat286 = std::make_unique<SimpleDateTimeFormat>("yMEd", "sq", false, errCode);
    EXPECT_EQ(sDateFormat286->Format(timep*1000), "Mar, 23.4.2024");
    auto sDateFormat287 = std::make_unique<SimpleDateTimeFormat>("yMMM", "sq", false, errCode);
    EXPECT_EQ(sDateFormat287->Format(timep*1000), "Prill 2024");
    auto sDateFormat288 = std::make_unique<SimpleDateTimeFormat>("yMMMEd", "sq", false, errCode);
    EXPECT_EQ(sDateFormat288->Format(timep*1000), "Mar, 23 Prill 2024");
}

/**
 * @tc.name: CustomizedDataTest0046
 * @tc.desc: Test Customized Data
 * @tc.type: FUNC
 */
HWTEST_F(CustomizedDataTest, CustomizedDataTest0046, TestSize.Level1)
{
    I18nErrorCode errCode = I18nErrorCode::SUCCESS;
    struct tm tmObj = {.tm_mday = 23, .tm_mon = 3, .tm_year = 124};
    time_t timep = mktime(&tmObj);
    auto sDateFormat289 = std::make_unique<SimpleDateTimeFormat>("yMMMd", "sq", false, errCode);
    EXPECT_EQ(sDateFormat289->Format(timep*1000), "23 Prill 2024");
    auto sDateFormat290 = std::make_unique<SimpleDateTimeFormat>("GyMMMEd", "sr-Latn", false, errCode);
    EXPECT_EQ(sDateFormat290->Format(timep*1000), "uto, 23. apr 2024. n. e.");
    auto sDateFormat291 = std::make_unique<SimpleDateTimeFormat>("GyMMMd", "sr-Latn", false, errCode);
    EXPECT_EQ(sDateFormat291->Format(timep*1000), "23. apr 2024. n. e.");
    auto sDateFormat292 = std::make_unique<SimpleDateTimeFormat>("MEd", "sv", false, errCode);
    EXPECT_EQ(sDateFormat292->Format(timep*1000), "tis 04-23");
    auto sDateFormat293 = std::make_unique<SimpleDateTimeFormat>("MMd", "sv", false, errCode);
    EXPECT_EQ(sDateFormat293->Format(timep*1000), "04-23");
    auto sDateFormat294 = std::make_unique<SimpleDateTimeFormat>("MMdd", "sv", false, errCode);
    EXPECT_EQ(sDateFormat294->Format(timep*1000), "04-23");
    auto sDateFormat295 = std::make_unique<SimpleDateTimeFormat>("Md", "sv", false, errCode);
    EXPECT_EQ(sDateFormat295->Format(timep*1000), "04-23");
    auto sDateFormat296 = std::make_unique<SimpleDateTimeFormat>("yMEd", "sv", false, errCode);
    EXPECT_EQ(sDateFormat296->Format(timep*1000), "tis 2024-04-23");
    auto sDateFormat297 = std::make_unique<SimpleDateTimeFormat>("MEd", "ta", false, errCode);
    EXPECT_EQ(sDateFormat297->Format(timep*1000), "செவ்., 23/4");
    auto sDateFormat298 = std::make_unique<SimpleDateTimeFormat>("yMM", "ta", false, errCode);
    EXPECT_EQ(sDateFormat298->Format(timep*1000), "4/2024");
    auto sDateFormat299 = std::make_unique<SimpleDateTimeFormat>("MEd", "te", false, errCode);
    EXPECT_EQ(sDateFormat299->Format(timep*1000), "మంగళ, 23-04");
    auto sDateFormat300 = std::make_unique<SimpleDateTimeFormat>("MMMEd", "te", false, errCode);
    EXPECT_EQ(sDateFormat300->Format(timep*1000), "మంగళ, 23 ఏప్రి");
    auto sDateFormat301 = std::make_unique<SimpleDateTimeFormat>("Md", "te", false, errCode);
    EXPECT_EQ(sDateFormat301->Format(timep*1000), "23-04");
    auto sDateFormat302 = std::make_unique<SimpleDateTimeFormat>("yM", "te", false, errCode);
    EXPECT_EQ(sDateFormat302->Format(timep*1000), "04-2024");
    auto sDateFormat303 = std::make_unique<SimpleDateTimeFormat>("yMEd", "te", false, errCode);
    EXPECT_EQ(sDateFormat303->Format(timep*1000), "మంగళ, 23-04-2024");
    auto sDateFormat304 = std::make_unique<SimpleDateTimeFormat>("yMMMEd", "te", false, errCode);
    EXPECT_EQ(sDateFormat304->Format(timep*1000), "మంగళ, 23 ఏప్రి, 2024");
    auto sDateFormat305 = std::make_unique<SimpleDateTimeFormat>("yMMMd", "te", false, errCode);
    EXPECT_EQ(sDateFormat305->Format(timep*1000), "23 ఏప్రి, 2024");
    auto sDateFormat306 = std::make_unique<SimpleDateTimeFormat>("yMd", "te", false, errCode);
    EXPECT_EQ(sDateFormat306->Format(timep*1000), "23-04-2024");
    auto sDateFormat307 = std::make_unique<SimpleDateTimeFormat>("Hm", "th", false, errCode);
    EXPECT_EQ(sDateFormat307->Format(timep*1000), "00:00");
    auto sDateFormat308 = std::make_unique<SimpleDateTimeFormat>("hmv", "th", false, errCode);
    EXPECT_EQ(sDateFormat308->Format(timep*1000), "12:00 ก่อนเที่ยง เวลาจีน");
    auto sDateFormat309 = std::make_unique<SimpleDateTimeFormat>("GyMd", "tr", false, errCode);
    EXPECT_EQ(sDateFormat309->Format(timep*1000), "23.04.2024 MS");
    auto sDateFormat310 = std::make_unique<SimpleDateTimeFormat>("MEd", "tr", false, errCode);
    EXPECT_EQ(sDateFormat310->Format(timep*1000), "23.04 Sal");
}

/**
 * @tc.name: CustomizedDataTest0047
 * @tc.desc: Test Customized Data
 * @tc.type: FUNC
 */
HWTEST_F(CustomizedDataTest, CustomizedDataTest0047, TestSize.Level1)
{
    I18nErrorCode errCode = I18nErrorCode::SUCCESS;
    struct tm tmObj = {.tm_mday = 23, .tm_mon = 3, .tm_year = 124};
    time_t timep = mktime(&tmObj);
    auto sDateFormat311 = std::make_unique<SimpleDateTimeFormat>("Md", "tr", false, errCode);
    EXPECT_EQ(sDateFormat311->Format(timep*1000), "23.04");
    auto sDateFormat312 = std::make_unique<SimpleDateTimeFormat>("yM", "tr", false, errCode);
    EXPECT_EQ(sDateFormat312->Format(timep*1000), "04.2024");
    auto sDateFormat313 = std::make_unique<SimpleDateTimeFormat>("yMEd", "tr", false, errCode);
    EXPECT_EQ(sDateFormat313->Format(timep*1000), "23.04.2024 Sal");
    auto sDateFormat314 = std::make_unique<SimpleDateTimeFormat>("Gy", "ug", false, errCode);
    EXPECT_EQ(sDateFormat314->Format(timep*1000), "مىلادىيە 2024");
    auto sDateFormat315 = std::make_unique<SimpleDateTimeFormat>("GyMMM", "ug", false, errCode);
    EXPECT_EQ(sDateFormat315->Format(timep*1000), "مىلادىيە 2024-يىلى 4-ئاي");
    auto sDateFormat316 = std::make_unique<SimpleDateTimeFormat>("GyMMMEd", "ug", false, errCode);
    EXPECT_EQ(sDateFormat316->Format(timep*1000), "مىلادىيە 2024-يىلى 4-ئاينىڭ 23-كۈنى سەي");
    auto sDateFormat317 = std::make_unique<SimpleDateTimeFormat>("GyMMMd", "ug", false, errCode);
    EXPECT_EQ(sDateFormat317->Format(timep*1000), "مىلادىيە 2024-يىلى 4-ئاينىڭ 23-كۈنى");
    auto sDateFormat318 = std::make_unique<SimpleDateTimeFormat>("MEd", "ug", false, errCode);
    EXPECT_EQ(sDateFormat318->Format(timep*1000), "23/4 سەي");
    auto sDateFormat319 = std::make_unique<SimpleDateTimeFormat>("MMMEd", "ug", false, errCode);
    EXPECT_EQ(sDateFormat319->Format(timep*1000), "4-ئاينىڭ 23-كۈنى سەي");
    auto sDateFormat322 = std::make_unique<SimpleDateTimeFormat>("MMMMd", "ug", false, errCode);
    EXPECT_EQ(sDateFormat322->Format(timep*1000), "4-ئاينىڭ 23-كۈنى");
    auto sDateFormat323 = std::make_unique<SimpleDateTimeFormat>("MMMd", "ug", false, errCode);
    EXPECT_EQ(sDateFormat323->Format(timep*1000), "4-ئاينىڭ 23-كۈنى");
    auto sDateFormat324 = std::make_unique<SimpleDateTimeFormat>("Md", "ug", false, errCode);
    EXPECT_EQ(sDateFormat324->Format(timep*1000), "23/4");
    auto sDateFormat325 = std::make_unique<SimpleDateTimeFormat>("yM", "ug", false, errCode);
    EXPECT_EQ(sDateFormat325->Format(timep*1000), "4/2024");
    auto sDateFormat326 = std::make_unique<SimpleDateTimeFormat>("yMEd", "ug", false, errCode);
    EXPECT_EQ(sDateFormat326->Format(timep*1000), "23/4/2024 سەي");
    auto sDateFormat327 = std::make_unique<SimpleDateTimeFormat>("yMMMEd", "ug", false, errCode);
    EXPECT_EQ(sDateFormat327->Format(timep*1000), "2024-يىلى 4-ئاينىڭ 23-كۈنى سەي");
    auto sDateFormat328 = std::make_unique<SimpleDateTimeFormat>("yMMMd", "ug", false, errCode);
    EXPECT_EQ(sDateFormat328->Format(timep*1000), "2024-يىلى 4-ئاينىڭ 23-كۈنى");
    auto sDateFormat329 = std::make_unique<SimpleDateTimeFormat>("yMd", "ug", false, errCode);
    EXPECT_EQ(sDateFormat329->Format(timep*1000), "23/4/2024");
    auto sDateFormat332 = std::make_unique<SimpleDateTimeFormat>("Gy", "vi", false, errCode);
    EXPECT_EQ(sDateFormat332->Format(timep*1000), "Năm 2024 SCN");
    auto sDateFormat333 = std::make_unique<SimpleDateTimeFormat>("GyMMM", "vi", false, errCode);
    EXPECT_EQ(sDateFormat333->Format(timep*1000), "tháng 4 năm 2024 SCN");
    auto sDateFormat334 = std::make_unique<SimpleDateTimeFormat>("GyMMMEd", "vi", false, errCode);
    EXPECT_EQ(sDateFormat334->Format(timep*1000), "Th 3, ngày 23 tháng 4 năm 2024 SCN");
    auto sDateFormat335 = std::make_unique<SimpleDateTimeFormat>("GyMMMd", "vi", false, errCode);
    EXPECT_EQ(sDateFormat335->Format(timep*1000), "Ngày 23 tháng 4 năm 2024 SCN");
    auto sDateFormat336 = std::make_unique<SimpleDateTimeFormat>("Hm", "vi", false, errCode);
    EXPECT_EQ(sDateFormat336->Format(timep*1000), "00:00");
}

/**
 * @tc.name: CustomizedDataTest0048
 * @tc.desc: Test Customized Data
 * @tc.type: FUNC
 */
HWTEST_F(CustomizedDataTest, CustomizedDataTest0048, TestSize.Level1)
{
    I18nErrorCode errCode = I18nErrorCode::SUCCESS;
    struct tm tmObj = {.tm_mday = 23, .tm_mon = 3, .tm_year = 124};
    time_t timep = mktime(&tmObj);
    auto sDateFormat337 = std::make_unique<SimpleDateTimeFormat>("MEd", "vi", false, errCode);
    EXPECT_EQ(sDateFormat337->Format(timep*1000), "Th 3, 23/04");
    auto sDateFormat338 = std::make_unique<SimpleDateTimeFormat>("MMMEd", "vi", false, errCode);
    EXPECT_EQ(sDateFormat338->Format(timep*1000), "Th 3, ngày 23 tháng 4");
    auto sDateFormat339 = std::make_unique<SimpleDateTimeFormat>("MMMMEd", "vi", false, errCode);
    EXPECT_EQ(sDateFormat339->Format(timep*1000), "Th 3, ngày 23 tháng 4");
    auto sDateFormat340 = std::make_unique<SimpleDateTimeFormat>("MMMMd", "vi", false, errCode);
    EXPECT_EQ(sDateFormat340->Format(timep*1000), "Ngày 23 tháng 4");
    auto sDateFormat341 = std::make_unique<SimpleDateTimeFormat>("MMMd", "vi", false, errCode);
    EXPECT_EQ(sDateFormat341->Format(timep*1000), "Ngày 23 tháng 4");
    auto sDateFormat342 = std::make_unique<SimpleDateTimeFormat>("MMdd", "vi", false, errCode);
    EXPECT_EQ(sDateFormat342->Format(timep*1000), "23/04");
    auto sDateFormat343 = std::make_unique<SimpleDateTimeFormat>("Md", "vi", false, errCode);
    EXPECT_EQ(sDateFormat343->Format(timep*1000), "23/04");
    auto sDateFormat344 = std::make_unique<SimpleDateTimeFormat>("yM", "vi", false, errCode);
    EXPECT_EQ(sDateFormat344->Format(timep*1000), "04/2024");
    auto sDateFormat345 = std::make_unique<SimpleDateTimeFormat>("yMEd", "vi", false, errCode);
    EXPECT_EQ(sDateFormat345->Format(timep*1000), "Th 3, 23/04/2024");
    auto sDateFormat346 = std::make_unique<SimpleDateTimeFormat>("yMMM", "vi", false, errCode);
    EXPECT_EQ(sDateFormat346->Format(timep*1000), "tháng 4 năm 2024");
    auto sDateFormat347 = std::make_unique<SimpleDateTimeFormat>("yMMMEd", "vi", false, errCode);
    EXPECT_EQ(sDateFormat347->Format(timep*1000), "Th 3, ngày 23 tháng 4 năm 2024");
    auto sDateFormat348 = std::make_unique<SimpleDateTimeFormat>("yMMMd", "vi", false, errCode);
    EXPECT_EQ(sDateFormat348->Format(timep*1000), "Ngày 23 tháng 4 năm 2024");
    auto sDateFormat349 = std::make_unique<SimpleDateTimeFormat>("yMd", "vi", false, errCode);
    EXPECT_EQ(sDateFormat349->Format(timep*1000), "23/04/2024");
    auto sDateFormat350 = std::make_unique<SimpleDateTimeFormat>("MEd", "zh-Hans-SG", false, errCode);
    EXPECT_EQ(sDateFormat350->Format(timep*1000), "23/04 周二");
    auto sDateFormat351 = std::make_unique<SimpleDateTimeFormat>("MMdd", "zh-Hans-SG", false, errCode);
    EXPECT_EQ(sDateFormat351->Format(timep*1000), "23/04");
    auto sDateFormat352 = std::make_unique<SimpleDateTimeFormat>("Md", "zh-Hans-SG", false, errCode);
    EXPECT_EQ(sDateFormat352->Format(timep*1000), "23/04");
    auto sDateFormat353 = std::make_unique<SimpleDateTimeFormat>("yMEd", "zh-Hans-SG", false, errCode);
    EXPECT_EQ(sDateFormat353->Format(timep*1000), "23/04/2024，周二");
    auto sDateFormat354 = std::make_unique<SimpleDateTimeFormat>("yMd", "zh-Hans-SG", false, errCode);
    EXPECT_EQ(sDateFormat354->Format(timep*1000), "23/04/2024");
    auto sDateFormat355 = std::make_unique<SimpleDateTimeFormat>("Bh", "zh-Hant", false, errCode);
    EXPECT_EQ(sDateFormat355->Format(timep*1000), "凌晨 12 時");
    auto sDateFormat356 = std::make_unique<SimpleDateTimeFormat>("Bhm", "zh-Hant", false, errCode);
    EXPECT_EQ(sDateFormat356->Format(timep*1000), "凌晨 12:00");
    auto sDateFormat357 = std::make_unique<SimpleDateTimeFormat>("Bhms", "zh-Hant", false, errCode);
    EXPECT_EQ(sDateFormat357->Format(timep*1000), "凌晨 12:00:00");
    auto sDateFormat358 = std::make_unique<SimpleDateTimeFormat>("EBhm", "zh-Hant", false, errCode);
    EXPECT_EQ(sDateFormat358->Format(timep*1000), "週二凌晨 12:00");
}

/**
 * @tc.name: CustomizedDataTest0049
 * @tc.desc: Test Customized Data
 * @tc.type: FUNC
 */
HWTEST_F(CustomizedDataTest, CustomizedDataTest0049, TestSize.Level1)
{
    I18nErrorCode errCode = I18nErrorCode::SUCCESS;
    struct tm tmObj = {.tm_mday = 23, .tm_mon = 3, .tm_year = 124};
    time_t timep = mktime(&tmObj);
    auto sDateFormat359 = std::make_unique<SimpleDateTimeFormat>("EBhms", "zh-Hant", false, errCode);
    EXPECT_EQ(sDateFormat359->Format(timep*1000), "週二凌晨 12:00:00");
    auto sDateFormat360 = std::make_unique<SimpleDateTimeFormat>("Ed", "zh-Hant", false, errCode);
    EXPECT_EQ(sDateFormat360->Format(timep*1000), "23 日 週二");
    auto sDateFormat361 = std::make_unique<SimpleDateTimeFormat>("Ehm", "zh-Hant", false, errCode);
    EXPECT_EQ(sDateFormat361->Format(timep*1000), "週二上午 12:00");
    auto sDateFormat362 = std::make_unique<SimpleDateTimeFormat>("Ehms", "zh-Hant", false, errCode);
    EXPECT_EQ(sDateFormat362->Format(timep*1000), "週二上午 12:00:00");
    auto sDateFormat363 = std::make_unique<SimpleDateTimeFormat>("Gy", "zh-Hant", false, errCode);
    EXPECT_EQ(sDateFormat363->Format(timep*1000), "西元 2024 年");
    auto sDateFormat364 = std::make_unique<SimpleDateTimeFormat>("GyMMM", "zh-Hant", false, errCode);
    EXPECT_EQ(sDateFormat364->Format(timep*1000), "西元 2024 年 4 月");
    auto sDateFormat365 = std::make_unique<SimpleDateTimeFormat>("GyMMMEd", "zh-Hant", false, errCode);
    EXPECT_EQ(sDateFormat365->Format(timep*1000), "西元 2024 年 4 月 23 日 週二");
    auto sDateFormat366 = std::make_unique<SimpleDateTimeFormat>("GyMMMd", "zh-Hant", false, errCode);
    EXPECT_EQ(sDateFormat366->Format(timep*1000), "西元 2024 年 4 月 23 日 週二");
    auto sDateFormat367 = std::make_unique<SimpleDateTimeFormat>("H", "zh-Hant", false, errCode);
    EXPECT_EQ(sDateFormat367->Format(timep*1000), "0 時");
    auto sDateFormat368 = std::make_unique<SimpleDateTimeFormat>("M", "zh-Hant", false, errCode);
    EXPECT_EQ(sDateFormat368->Format(timep*1000), "4 月");
    auto sDateFormat369 = std::make_unique<SimpleDateTimeFormat>("MMMEd", "zh-Hant", false, errCode);
    EXPECT_EQ(sDateFormat369->Format(timep*1000), "4 月 23 日 週二");
    auto sDateFormat371 = std::make_unique<SimpleDateTimeFormat>("MMMMd", "zh-Hant", false, errCode);
    EXPECT_EQ(sDateFormat371->Format(timep*1000), "4 月 23 日");
    auto sDateFormat372 = std::make_unique<SimpleDateTimeFormat>("MMMd", "zh-Hant", false, errCode);
    EXPECT_EQ(sDateFormat372->Format(timep*1000), "4 月 23 日");
    auto sDateFormat373 = std::make_unique<SimpleDateTimeFormat>("d", "zh-Hant", false, errCode);
    EXPECT_EQ(sDateFormat373->Format(timep*1000), "23 日");
    auto sDateFormat374 = std::make_unique<SimpleDateTimeFormat>("h", "zh-Hant", false, errCode);
    EXPECT_EQ(sDateFormat374->Format(timep*1000), "上午 12 時");
    auto sDateFormat375 = std::make_unique<SimpleDateTimeFormat>("hm", "zh-Hant", false, errCode);
    EXPECT_EQ(sDateFormat375->Format(timep*1000), "上午 12:00");
    auto sDateFormat376 = std::make_unique<SimpleDateTimeFormat>("hms", "zh-Hant", false, errCode);
    EXPECT_EQ(sDateFormat376->Format(timep*1000), "上午 12:00:00");
    auto sDateFormat377 = std::make_unique<SimpleDateTimeFormat>("hmsv", "zh-Hant", false, errCode);
    EXPECT_EQ(sDateFormat377->Format(timep*1000), "上午 12:00:00 [中國時間]");
    auto sDateFormat378 = std::make_unique<SimpleDateTimeFormat>("hmv", "zh-Hant", false, errCode);
    EXPECT_EQ(sDateFormat378->Format(timep*1000), "上午 12:00 [中國時間]");
    auto sDateFormat379 = std::make_unique<SimpleDateTimeFormat>("y", "zh-Hant", false, errCode);
    EXPECT_EQ(sDateFormat379->Format(timep*1000), "2024 年");
    auto sDateFormat380 = std::make_unique<SimpleDateTimeFormat>("yMEEEEd", "zh-Hant", false, errCode);
    EXPECT_EQ(sDateFormat380->Format(timep*1000), "2024 年 4 月 23 日 星期二");
    auto sDateFormat381 = std::make_unique<SimpleDateTimeFormat>("yMMM", "zh-Hant", false, errCode);
    EXPECT_EQ(sDateFormat381->Format(timep*1000), "2024 年 4 月");
}

/**
 * @tc.name: CustomizedDataTest0050
 * @tc.desc: Test Customized Data
 * @tc.type: FUNC
 */
HWTEST_F(CustomizedDataTest, CustomizedDataTest0050, TestSize.Level1)
{
    I18nErrorCode errCode = I18nErrorCode::SUCCESS;
    struct tm tmObj = {.tm_mday = 23, .tm_mon = 3, .tm_year = 124};
    time_t timep = mktime(&tmObj);
    auto sDateFormat382 = std::make_unique<SimpleDateTimeFormat>("yMMMEd", "zh-Hant", false, errCode);
    EXPECT_EQ(sDateFormat382->Format(timep*1000), "2024 年 4 月 23 日 週二");
    auto sDateFormat383 = std::make_unique<SimpleDateTimeFormat>("yMMMM", "zh-Hant", false, errCode);
    EXPECT_EQ(sDateFormat383->Format(timep*1000), "2024 年 4 月");
    auto sDateFormat384 = std::make_unique<SimpleDateTimeFormat>("yMMMd", "zh-Hant", false, errCode);
    EXPECT_EQ(sDateFormat384->Format(timep*1000), "2024 年 4 月 23 日");
    auto sDateFormat388 = std::make_unique<SimpleDateTimeFormat>("Ehm", "zh-Hant-HK", false, errCode);
    EXPECT_EQ(sDateFormat388->Format(timep*1000), "週二 上午 12:00");
    auto sDateFormat389 = std::make_unique<SimpleDateTimeFormat>("Ehms", "zh-Hant-HK", false, errCode);
    EXPECT_EQ(sDateFormat389->Format(timep*1000), "週二 上午 12:00:00");
    auto sDateFormat390 = std::make_unique<SimpleDateTimeFormat>("GyMMMEd", "zh-Hant-HK", false, errCode);
    EXPECT_EQ(sDateFormat390->Format(timep*1000), "公元 2024 年 4 月 23 日 週二");
    auto sDateFormat391 = std::make_unique<SimpleDateTimeFormat>("MMMEd", "zh-Hant-HK", false, errCode);
    EXPECT_EQ(sDateFormat391->Format(timep*1000), "4 月 23 日 週二");
    auto sDateFormat393 = std::make_unique<SimpleDateTimeFormat>("h", "zh-Hant-HK", false, errCode);
    EXPECT_EQ(sDateFormat393->Format(timep*1000), "上午 12 時");
    auto sDateFormat394 = std::make_unique<SimpleDateTimeFormat>("hm", "zh-Hant-HK", false, errCode);
    EXPECT_EQ(sDateFormat394->Format(timep*1000), "上午 12:00");
    auto sDateFormat395 = std::make_unique<SimpleDateTimeFormat>("hms", "zh-Hant-HK", false, errCode);
    EXPECT_EQ(sDateFormat395->Format(timep*1000), "上午 12:00:00");
    auto sDateFormat396 = std::make_unique<SimpleDateTimeFormat>("hmsv", "zh-Hant-HK", false, errCode);
    EXPECT_EQ(sDateFormat396->Format(timep*1000), "上午 12:00:00 [中國時間]");
    auto sDateFormat397 = std::make_unique<SimpleDateTimeFormat>("hmv", "zh-Hant-HK", false, errCode);
    EXPECT_EQ(sDateFormat397->Format(timep*1000), "上午 12:00 [中國時間]");
    auto sDateFormat398 = std::make_unique<SimpleDateTimeFormat>("yMMMEd", "zh-Hant-HK", false, errCode);
    EXPECT_EQ(sDateFormat398->Format(timep*1000), "2024 年 4 月 23 日 週二");
}
} // namespace I18n
} // namespace Global
} // namespace OHOS