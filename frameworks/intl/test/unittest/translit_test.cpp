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

#include <gtest/gtest.h>
#include <map>
#include <vector>
#include "translit_test.h"
#include <unistd.h>
#include "unicode/utypes.h"
#include "unicode/translit.h"
#include "ohos/init_data.h"
#include "timezone_location.h"

using namespace OHOS::Global::I18n;
using testing::ext::TestSize;
using namespace std;
using namespace testing;

namespace OHOS {
namespace Global {
namespace I18n {

std::map<std::string, std::string> TranslitTest::translitTestCases {
    {"阿胶", "ē jiāo"},
    {"挨打", "ái dǎ"},
    {"扒手", "pá shǒu"},
    {"扒草", "pá cǎo"},
    {"刀把", "dāo bà"},
    {"话把儿", "huà bà er"},
    {"单薄", "dān bó"},
    {"稀薄", "xī bó"},
    {"背包", "bēi bāo"},
    {"背枪", "bēi qiāng"},
    {"剥皮", "bāo pí"},
    {"称心", "chèn xīn"},
    {"对称", "duì chèn"},
    {"冲床", "chòng chuáng"},
    {"冲子", "chòng zi"},
    {"处罚", "chǔ fá"},
    {"处置", "chǔ zhì"},
    {"朝阳", "zhāo yáng"},
    {"朝霞", "zhāo xiá"},
    {"朝气", "zhāo qì"},
    {"重复", "chóng fù"},
    {"重新", "chóng xīn"},
    {"答理", "dā lǐ"},
    {"答应", "dā yìng"},
    {"山大王", "shān dài wáng"},
    {"当天", "dàng tiān"},
    {"颠倒", "diān dǎo"},
    {"倒下", "dǎo xià"},
    {"得喝水了", "děi hē shuǐ le"},
    {"好得很", "hǎo de hěn"},
    {"的确", "dí què"},
    {"目的", "mù dì"},
    {"一语中的", "yī yǔ zhòng dì"},
    {"都市", "dū shì"},
    {"首都", "shǒu dū"},
    {"弹琴", "tán qín"},
    {"弹力", "tán lì"},
    {"理发", "lǐ fà"},
    {"头发", "tóu fà"},
    {"身分", "shēn fèn"},
    {"一分子", "yī fèn zi"},
    {"可汗", "kè hán"},
    {"大汗", "dà hán"},
    {"呼号", "hū háo"},
    {"号叫", "háo jiào"},
    {"应和", "yìng hè"},
    {"和诗", "hè shī"},
    {"和面", "huó miàn"},
    {"和泥", "huó ní"},
    {"搀和", "chān huo"},
    {"搅和", "jiǎo huo"},
    {"喝令", "hè lìng"},
    {"喝彩", "hè cǎi"},
    {"好奇", "hào qí"},
    {"好学", "hào xué"},
    {"还原", "huán yuán"},
    {"还书", "huán shū"},
    {"茶几", "chá jī"},
    {"几乎", "jī hū"},
    {"假期", "jià qī"},
    {"假日", "jià rì"},
    {"间断", "jiàn duàn"},
    {"相间", "xiāng jiàn"},
    {"将士", "jiàng shì"},
    {"虾兵蟹将", "xiā bīng xiè jiàng"},
    {"角色", "jué sè"},
    {"角斗", "jué dòu"},
    {"教书", "jiāo shū"},
    {"教给", "jiāo gěi"},
    {"结果", "jié guǒ"},
    {"结实", "jiē shí"},
    {"睡觉", "shuì jiào"},
    {"午觉", "wǔ jiào"},
    {"关卡", "guān qiǎ"},
    {"卡子", "qiǎ zi"},
    {"看守", "kān shǒu"},
    {"看管", "kān guǎn"},
    {"空白", "kòng bái"},
    {"空地", "kòng dì"},
    {"空闲", "kòng xián"},
    {"露头", "lòu tóu"},
    {"露马脚", "lòu mǎ jiǎo"},
    {"丢三落四", "diū sān là sì"},
    {"落下", "là xià"},
    {"了解", "liǎo jiě"},
    {"不了了之", "bù liǎo liǎo zhī"},
    {"音乐", "yīn yuè"},
    {"乐曲", "yuè qǔ"},
    {"埋怨", "mán yuàn"},
    {"没收", "mò shōu"},
    {"淹没", "yān mò"},
    {"难民", "nàn mín"},
    {"难兄难弟", "nàn xiōng nàn dì"},
    {"宁可", "nìng kě"},
    {"宁折不弯", "nìng zhé bù wān"},
    {"勉强", "miǎn qiǎng"},
    {"强迫", "qiǎng pò"},
    {"强词夺理", "qiǎng cí duó lǐ"},
    {"倔强", "jué jiàng"},
    {"曲调", "qǔ diào"},
    {"曲艺", "qǔ yì"},
    {"歌曲", "gē qǔ"},
    {"数落", "shǔ luò"},
    {"数数", "shǔ shù"},
    {"少年", "shào nián"},
    {"少先队", "shào xiān duì"},
    {"调皮", "tiáo pí"},
    {"调和", "tiáo hé"},
    {"挑战", "tiǎo zhàn"},
    {"挑拨", "tiǎo bō"},
    {"威吓", "wēi hè"},
    {"恐吓", "kǒng hè"},
    {"朝鲜", "cháo xiǎn"},
    {"鲜为人知", "xiǎn wéi rén zhī"},
    {"行市", "háng shì"},
    {"银行", "yín háng"},
    {"道行", "dào héng"},
    {"鸡血", "jī xiě"},
    {"狗血", "gǒu xiě"},
    {"馒头", "mán tou"},
    {"流了点血", "liú le diǎn xiě"},
    {"相声", "xiàng shēng"},
    {"面相", "miàn xiàng"},
    {"应付", "yìng fù"},
    {"应对", "yìng duì"},
    {"要求", "yāo qiú"},
    {"着急", "zháo jí"},
    {"着迷", "zháo mí"},
    {"着凉", "zháo liáng"},
    {"着落", "zhuó luò"},
    {"着重", "zhuó zhòng"},
    {"着手", "zhuó shǒu"},
    {"正月", "zhēng yuè"},
    {"中奖", "zhòng jiǎng"},
    {"中靶", "zhòng bǎ"},
    {"耕种", "gēng zhòng"},
    {"种植", "zhòng zhí"},
    {"转动", "zhuàn dòng"},
    {"转速", "zhuàn sù"},
    {"只身", "zhī shēn"},
    {"一只", "yī zhī"},
    {"西藏", "xī zàng"},
    {"藏民", "zàng mín"},
    {"青藏高原", "qīng zàng gāo yuán"},
    {"散文", "sǎn wén"},
    {"参差", "cēn cī"},
    {"缝纫", "féng rèn"},
    {"劲敌", "jìng dí"},
    {"扇风", "shān fēng"},
    {"畜牧", "xù mù"},
    {"弄堂", "lòng táng"},
    {"伎俩", "jì liǎng"},
    {"草率", "cǎo shuài"},
    {"漂泊", "piāo bó"},
    {"泊车", "bó chē"},
    {"膀胱", "páng guāng"},
    {"复辟", "fù bì"},
    {"单于", "chán yú"},
    {"折本", "shé běn"},
    {"一沓纸", "yì dá zhǐ"},
    {"引吭高歌", "yǐn háng gāo gē"},
    {"出差", "chū chāi"},
    {"差不多", "chà bù duō"},
    {"差错", "chā cuò"},
    {"盛饭", "chéng fàn"},
    {"创伤", "chuāng shāng"},
    {"雪茄", "xuě jiā"},
    {"番茄", "fān qié"},
    {"茄子", "qié zi"},
    {"发疟子", "fā yào zǐ"},
    {"自怨自艾", "zì yuàn zì yì"},
    {"传记", "zhuàn jì"},
    {"荷枪实弹", "hè qiāng shí dàn"},
    {"涨红了脸", "zhàng hóng le liǎn"},
    {"校对", "jiào duì"},
    {"奇数", "jī shù"},
    {"炮制", "páo zhì"},
    {"供给", "gōng jǐ"},
    {"冠冕堂皇", "guān miǎn táng huáng"},
    {"干活", "gàn huó"},
    {"巷道", "hàng dào"},
    {"薄荷", "bò hé"},
    {"可恶", "kě wù"},
    {"大腹便便", "dà fù pián pián"},
    {"星宿", "xīng xiù"},
    {"号啕大哭", "háo táo dà kū"},
    {"倾轧", "qīng yà"},
    {"模样", "mú yàng"},
    {"宿舍", "sù shè"},
    {"殷红", "yān hóng"},
    {"还要", "hái yào"},
    {"系领带", "jì lǐng dài"},
    {"放假", "fàng jià"},
    {"空降", "kōng jiàng"},
    {"塞外", "sài wài"},
    {"屏气凝神", "bǐng qì níng shén"},
    {"考卷", "kǎo juàn"},
    {"乐亭", "lào tíng"},
    {"好为人师", "hào wéi rén shī"},
    {"窥伺", "kuī sì"},
    {"处理", "chǔ lǐ"},
};

void TranslitTest::SetUpTestCase(void)
{
    SetHwIcuDirectory();
}

void TranslitTest::TearDownTestCase(void)
{
}

void TranslitTest::SetUp(void)
{}

void TranslitTest::TearDown(void)
{}

/**
 * @tc.name: TranslitTest0001
 * @tc.desc: Test icu::Transliterator
 * @tc.type: FUNC
 */
HWTEST_F(TranslitTest, TranslitTest0001, TestSize.Level1)
{
    const std::string transId = "Any-Latn";
    UErrorCode status = U_ZERO_ERROR;
    icu::UnicodeString unistr = icu::UnicodeString::fromUTF8(transId);
    icu::Transliterator *trans = icu::Transliterator::createInstance(unistr, UTransDirection::UTRANS_FORWARD, status);
    ASSERT_TRUE(U_SUCCESS(status));
    ASSERT_TRUE(trans != nullptr);
    for (auto iter = translitTestCases.begin(); iter != translitTestCases.end(); ++iter) {
        std::string word = iter->first;
        std::string expect = iter->second;
        icu::UnicodeString uniWord = icu::UnicodeString::fromUTF8(word.data());
        trans->transliterate(uniWord);
        std::string result;
        uniWord.toUTF8String(result);
        EXPECT_EQ(result, expect);
    }
    delete trans;
}

/**
 * @tc.name: TranslitTest0001
 * @tc.desc: Test icu::Transliterator
 * @tc.type: FUNC
 */
HWTEST_F(TranslitTest, TranslitTest0002, TestSize.Level1)
{
    std::unique_ptr<TimeZoneLocation> tzLoc = std::make_unique<TimeZoneLocation>();
    tzLoc->GetTimezoneByLocation(106.58314, 22.46933);
    tzLoc->GetTimezoneByLocation(106.7917876, 22.82248);
    tzLoc->GetTimezoneByLocation(106.726289, 22.85502);
    tzLoc->GetTimezoneByLocation(106.8472477, 22.83867);
    tzLoc->GetTimezoneByLocation(126.897702, 41.798380);
    printf("==================zhongguo yindu=========== \r\n");
    tzLoc->GetTimezoneByLocation(92.36, 27.11);
    tzLoc->GetTimezoneByLocation(93.80, 27.26);
    tzLoc->GetTimezoneByLocation(95.57, 28.90);
    tzLoc->GetTimezoneByLocation(95.91, 28.78);
    tzLoc->GetTimezoneByLocation(97.03, 28.75);
    printf("==================zhongguo budan=========== \r\n");
    tzLoc->GetTimezoneByLocation(89.00, 27.51);
    tzLoc->GetTimezoneByLocation(90.82, 28.05);
    tzLoc->GetTimezoneByLocation(88.90, 27.47);
    printf("==================nanhai=========== \r\n");
    tzLoc->GetTimezoneByLocation(117.74, 15.18);
    tzLoc->GetTimezoneByLocation(116.57, 9.73);
    tzLoc->GetTimezoneByLocation(113.83, 7.38);
    printf("==================nan tong jiao=========== \r\n");
    tzLoc->GetTimezoneByLocation(113.240785, 6.33258);
    printf("==================diaoyudao=========== \r\n");
    tzLoc->GetTimezoneByLocation(123.47, 25.74);
    tzLoc->GetTimezoneByLocation(123.54, 25.72);
    printf("==================Urumqi=========== \r\n");
    tzLoc->GetTimezoneByLocation(87.7, 43.8);
    printf("==================xindeli=========== \r\n");
    tzLoc->GetTimezoneByLocation(77.12, 28.36);
    printf("==================jierjisisitan=========== \r\n");
    tzLoc->GetTimezoneByLocation(72.16912, 41.1918);
    printf("==================yuenan=========== \r\n");
    tzLoc->GetTimezoneByLocation(106.726064, 22.852934);
    tzLoc->GetTimezoneByLocation(106.643796, 22.869609);
    tzLoc->GetTimezoneByLocation(106.747336, 22.671182);
    tzLoc->GetTimezoneByLocation(104.200664, 19.169076);
    tzLoc->GetTimezoneByLocation(105.87491, 11.29094);
}

/**
 * @tc.name: TranslitTest0001
 * @tc.desc: Test icu::Transliterator
 * @tc.type: FUNC
 */
HWTEST_F(TranslitTest, TranslitTest0003, TestSize.Level1)
{
    std::unique_ptr<TimeZoneLocation> tzLoc = std::make_unique<TimeZoneLocation>();
    printf("==================yelushaleng=========== \r\n");
    tzLoc->GetTimezoneByLocation(35.14, 31.96);
    tzLoc->GetTimezoneByLocation(35.20, 31.80);
    tzLoc->GetTimezoneByLocation(35.35, 32.35);
    tzLoc->GetTimezoneByLocation(34.10, 44.94);
    tzLoc->GetTimezoneByLocation(33.52, 44.61);
    tzLoc->GetTimezoneByLocation(-13.19, 27.15);
    tzLoc->GetTimezoneByLocation(21.15, 42.65);
    tzLoc->GetTimezoneByLocation(21.43, 42.00);
    tzLoc->GetTimezoneByLocation(37.80, 48.01);
    tzLoc->GetTimezoneByLocation(39.30, 48.57);
    tzLoc->GetTimezoneByLocation(-59.79, -51.72);
    tzLoc->GetTimezoneByLocation(147.79, 45.08);
    tzLoc->GetTimezoneByLocation(131.86, 37.24);
    tzLoc->GetTimezoneByLocation(33.34, 35.20);
    tzLoc->GetTimezoneByLocation(32.81, 34.77);
    printf("================dongsha qundao=========== \r\n");
    tzLoc->GetTimezoneByLocation(116.81, 20.68);
    tzLoc->GetTimezoneByLocation(116.802329, 20.664739);
    tzLoc->GetTimezoneByLocation(116.897086, 20.662169);
    //边界点
    tzLoc->GetTimezoneByLocation(116.685503, 20.85476);
    printf("================@@@@@@@@=========== \r\n");
    tzLoc->GetTimezoneByLocation(114.236781, 11.525819);
    tzLoc->GetTimezoneByLocation(114.441625, 11.346657);
    tzLoc->GetTimezoneByLocation(114.817913, 11.08117);
    tzLoc->GetTimezoneByLocation(115.598432, 10.734287);
    tzLoc->GetTimezoneByLocation(114.686137, 10.584087);
    tzLoc->GetTimezoneByLocation(114.346505, 10.326766);
    tzLoc->GetTimezoneByLocation(114.061329, 10.087432);
    tzLoc->GetTimezoneByLocation(114.552484, 10.101774);
    tzLoc->GetTimezoneByLocation(114.203021, 8.912868);
    tzLoc->GetTimezoneByLocation(114.453052, 8.856799);
    tzLoc->GetTimezoneByLocation(111.471754, 8.666755);
    tzLoc->GetTimezoneByLocation(113.095773, 8.227688);
    tzLoc->GetTimezoneByLocation(114.247267, 7.535228);
}

/**
 * @tc.name: TranslitTest0004
 * @tc.desc: Test icu::Transliterator
 * @tc.type: FUNC
 */
HWTEST_F(TranslitTest, TranslitTest0004, TestSize.Level1)
{
    std::unique_ptr<TimeZoneLocation> tzLoc = std::make_unique<TimeZoneLocation>();
    printf("================$$$$$$$$$=========== \r\n");
    tzLoc->GetTimezoneByLocation(122.893295, 24.248673);
    tzLoc->GetTimezoneByLocation(124.454892, 24.776871);
    printf("================zhoubian shiqu=========== \r\n");
    tzLoc->GetTimezoneByLocation(127.150348, 40.984974);
    tzLoc->GetTimezoneByLocation(134.274421, 45.171298);
    tzLoc->GetTimezoneByLocation(128.134032, 53.553832);
    tzLoc->GetTimezoneByLocation(117.155382, 51.463265);
    tzLoc->GetTimezoneByLocation(106.099319, 45.34427);
    tzLoc->GetTimezoneByLocation(90.737938, 48.422181);
    tzLoc->GetTimezoneByLocation(87.479194, 50.141346);
    tzLoc->GetTimezoneByLocation(80.892381, 47.443379);
    tzLoc->GetTimezoneByLocation(76.194132, 41.301241);
    tzLoc->GetTimezoneByLocation(74.341692, 38.200328);
    tzLoc->GetTimezoneByLocation(72.765399, 35.498675);
}
} // namespace I18n
} // namespace Global
} // namespace OHOS