using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace TyrannyStringtableConverter
{
    class AnemonePort
    {

        string TF_Translating(string originalText)
        {
            throw new NotImplementedException();
        }

        string eztrans_proc(string input)
        {
            string szContext = HangulEncode(input);
            // 이지트랜스 오류 잡아주기
            // 「よろしければ今度２人でお話しなどできないでしょうか」
            szContext = szContext.Replace("できないでしょ", "@X@でき@X@ないでしょ");
            szContext = szContext.Replace("きないでしょ", "き@X@ないでしょ");
            szContext = szContext.Replace("でき@X@ないでしょ", "できないでしょ");

            throw new NotImplementedException();
        }

        string HangulEncode(string input)
        {
            string output = "";

            foreach(char it in input)
            {
                if(it == '@' 
                    || it == '\0'
                    || (it >= 0x1100 && it <= 0x11FF)
                    || (it >= 0x3130 && it <= 0x318F)
                    || (it >= 0xA960 && it <= 0xA97F)
                    || (it >= 0xAC00 && it <= 0xD7AF)
                    || (it >= 0xD7B0 && it <= 0xD7FF))
                {
                    output += "+x" + string.Format("{0:X4}", it);
                } else
                {
                    switch (it)
                    {
                        case '↔':
                        case '◁':
                        case '◀':
                        case '▷':
                        case '▶':
                        case '♤':
                        case '♠':
                        case '♡':
                        case '♥':
                        case '♧':
                        case '♣':
                        case '⊙':
                        case '◈':
                        case '▣':
                        case '◐':
                        case '◑':
                        case '▒':
                        case '▤':
                        case '▥':
                        case '▨':
                        case '▧':
                        case '▦':
                        case '▩':
                        case '♨':
                        case '☏':
                        case '☎':
                        case '☜':
                        case '☞':
                        case '↕':
                        case '↗':
                        case '↙':
                        case '↖':
                        case '↘':
                        case '♩':
                        case '♬':
                        case '㉿':
                        case '㈜':
                        case '㏇':
                        case '™':
                        case '㏂':
                        case '㏘':
                        case '＂':
                        case '＇':
                        case '∼':
                        case 'ˇ':
                        case '˘':
                        case '˝':
                        case '¡':
                        case '˚':
                        case '˙':
                        case '˛':
                        case '¿':
                        case 'ː':
                        case '∏':
                        case '￦':
                        case '℉':
                        case '€':
                        case '㎕':
                        case '㎖':
                        case '㎗':
                        case 'ℓ':
                        case '㎘':
                        case '㎣':
                        case '㎤':
                        case '㎥':
                        case '㎦':
                        case '㎙':
                        case '㎚':
                        case '㎛':
                        case '㎟':
                        case '㎠':
                        case '㎢':
                        case '㏊':
                        case '㎍':
                        case '㏏':
                        case '㎈':
                        case '㎉':
                        case '㏈':
                        case '㎧':
                        case '㎨':
                        case '㎰':
                        case '㎱':
                        case '㎲':
                        case '㎳':
                        case '㎴':
                        case '㎵':
                        case '㎶':
                        case '㎷':
                        case '㎸':
                        case '㎀':
                        case '㎁':
                        case '㎂':
                        case '㎃':
                        case '㎄':
                        case '㎺':
                        case '㎻':
                        case '㎼':
                        case '㎽':
                        case '㎾':
                        case '㎿':
                        case '㎐':
                        case '㎑':
                        case '㎒':
                        case '㎓':
                        case '㎔':
                        case 'Ω':
                        case '㏀':
                        case '㏁':
                        case '㎊':
                        case '㎋':
                        case '㎌':
                        case '㏖':
                        case '㏅':
                        case '㎭':
                        case '㎮':
                        case '㎯':
                        case '㏛':
                        case '㎩':
                        case '㎪':
                        case '㎫':
                        case '㎬':
                        case '㏝':
                        case '㏐':
                        case '㏓':
                        case '㏃':
                        case '㏉':
                        case '㏜':
                        case '㏆':
                        case '┒':
                        case '┑':
                        case '┚':
                        case '┙':
                        case '┖':
                        case '┕':
                        case '┎':
                        case '┍':
                        case '┞':
                        case '┟':
                        case '┡':
                        case '┢':
                        case '┦':
                        case '┧':
                        case '┪':
                        case '┭':
                        case '┮':
                        case '┵':
                        case '┶':
                        case '┹':
                        case '┺':
                        case '┽':
                        case '┾':
                        case '╀':
                        case '╁':
                        case '╃':
                        case '╄':
                        case '╅':
                        case '╆':
                        case '╇':
                        case '╈':
                        case '╉':
                        case '╊':
                        case '┱':
                        case '┲':
                        case 'ⅰ':
                        case 'ⅱ':
                        case 'ⅲ':
                        case 'ⅳ':
                        case 'ⅴ':
                        case 'ⅵ':
                        case 'ⅶ':
                        case 'ⅷ':
                        case 'ⅸ':
                        case 'ⅹ':
                        case '½':
                        case '⅓':
                        case '⅔':
                        case '¼':
                        case '¾':
                        case '⅛':
                        case '⅜':
                        case '⅝':
                        case '⅞':
                        case 'ⁿ':
                        case '₁':
                        case '₂':
                        case '₃':
                        case '₄':
                        case 'Ŋ':
                        case 'đ':
                        case 'Ħ':
                        case 'Ĳ':
                        case 'Ŀ':
                        case 'Ł':
                        case 'Œ':
                        case 'Ŧ':
                        case 'ħ':
                        case 'ı':
                        case 'ĳ':
                        case 'ĸ':
                        case 'ŀ':
                        case 'ł':
                        case 'œ':
                        case 'ŧ':
                        case 'ŋ':
                        case 'ŉ':
                        case '㉠':
                        case '㉡':
                        case '㉢':
                        case '㉣':
                        case '㉤':
                        case '㉥':
                        case '㉦':
                        case '㉧':
                        case '㉨':
                        case '㉩':
                        case '㉪':
                        case '㉫':
                        case '㉬':
                        case '㉭':
                        case '㉮':
                        case '㉯':
                        case '㉰':
                        case '㉱':
                        case '㉲':
                        case '㉳':
                        case '㉴':
                        case '㉵':
                        case '㉶':
                        case '㉷':
                        case '㉸':
                        case '㉹':
                        case '㉺':
                        case '㉻':
                        case '㈀':
                        case '㈁':
                        case '㈂':
                        case '㈃':
                        case '㈄':
                        case '㈅':
                        case '㈆':
                        case '㈇':
                        case '㈈':
                        case '㈉':
                        case '㈊':
                        case '㈋':
                        case '㈌':
                        case '㈍':
                        case '㈎':
                        case '㈏':
                        case '㈐':
                        case '㈑':
                        case '㈒':
                        case '㈓':
                        case '㈔':
                        case '㈕':
                        case '㈖':
                        case '㈗':
                        case '㈘':
                        case '㈙':
                        case '㈚':
                        case '㈛':
                        case 'ⓐ':
                        case 'ⓑ':
                        case 'ⓒ':
                        case 'ⓓ':
                        case 'ⓔ':
                        case 'ⓕ':
                        case 'ⓖ':
                        case 'ⓗ':
                        case 'ⓘ':
                        case 'ⓙ':
                        case 'ⓚ':
                        case 'ⓛ':
                        case 'ⓜ':
                        case 'ⓝ':
                        case 'ⓞ':
                        case 'ⓟ':
                        case 'ⓠ':
                        case 'ⓡ':
                        case 'ⓢ':
                        case 'ⓣ':
                        case 'ⓤ':
                        case 'ⓥ':
                        case 'ⓦ':
                        case 'ⓧ':
                        case 'ⓨ':
                        case 'ⓩ':
                        case '①':
                        case '②':
                        case '③':
                        case '④':
                        case '⑤':
                        case '⑥':
                        case '⑦':
                        case '⑧':
                        case '⑨':
                        case '⑩':
                        case '⑪':
                        case '⑫':
                        case '⑬':
                        case '⑭':
                        case '⑮':
                        case '⒜':
                        case '⒝':
                        case '⒞':
                        case '⒟':
                        case '⒠':
                        case '⒡':
                        case '⒢':
                        case '⒣':
                        case '⒤':
                        case '⒥':
                        case '⒦':
                        case '⒧':
                        case '⒨':
                        case '⒩':
                        case '⒪':
                        case '⒫':
                        case '⒬':
                        case '⒭':
                        case '⒮':
                        case '⒯':
                        case '⒰':
                        case '⒱':
                        case '⒲':
                        case '⒳':
                        case '⒴':
                        case '⒵':
                        case '⑴':
                        case '⑵':
                        case '⑶':
                        case '⑷':
                        case '⑸':
                        case '⑹':
                        case '⑺':
                        case '⑻':
                        case '⑼':
                        case '⑽':
                        case '⑾':
                        case '⑿':
                        case '⒀':
                        case '⒁':
                        case '⒂':
                            output += "+X" + string.Format("{0:X4}", it);
                            break;
                        default:
                            output += it;
                            break;
                    }
                }
            }
            return output;
        }

        string HangulDecode(string input)
        {
            string output = "";
            for (int i = 0; i < input.Length; i++)
            {
                // @X = 삭제
                if (i + 2 < input.Length && input[i] == '@' && input[i + 1] == 'X' && input[i + 2] == '@')
                {
                    i += 2;
                    continue;
                }
                else if (i + 5 < input.Length && input[i] == '+' && (input[i + 1] == 'x' || input[i + 1] == 'X')
                        && ((input[i + 2] >= 'A' && input[i + 2] <= 'Z') || (input[i + 2] >= 'a' && input[i + 2] <= 'z') || (input[i + 2] >= '0' && input[i + 2] <= '9'))
                        && ((input[i + 3] >= 'A' && input[i + 3] <= 'Z') || (input[i + 3] >= 'a' && input[i + 3] <= 'z') || (input[i + 3] >= '0' && input[i + 3] <= '9'))
                        && ((input[i + 4] >= 'A' && input[i + 4] <= 'Z') || (input[i + 4] >= 'a' && input[i + 4] <= 'z') || (input[i + 4] >= '0' && input[i + 4] <= '9'))
                        && ((input[i + 5] >= 'A' && input[i + 5] <= 'Z') || (input[i + 5] >= 'a' && input[i + 5] <= 'z') || (input[i + 5] >= '0' && input[i + 5] <= '9')))
                {
                    string hex = input.Substring(i + 2, 4);
                    char converted = (char)int.Parse(hex, System.Globalization.NumberStyles.HexNumber);
                    output += converted;
                } else
                {
                    output += input[i];
                }
            }
            return output;
        }
    }
}
