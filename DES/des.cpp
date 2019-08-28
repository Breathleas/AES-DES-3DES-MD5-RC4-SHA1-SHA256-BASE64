#include "des.h"

// 初始置换
const unsigned char Table_IP[64] = { 58, 50, 42, 34, 26, 18, 10, 2, 60, 52, 44,
        36, 28, 20, 12, 4, 62, 54, 46, 38, 30, 22, 14, 6, 64, 56, 48, 40, 32,
        24, 16, 8, 57, 49, 41, 33, 25, 17, 9, 1, 59, 51, 43, 35, 27, 19, 11, 3,
        61, 53, 45, 37, 29, 21, 13, 5, 63, 55, 47, 39, 31, 23, 15, 7 };

// 末置换
const unsigned char Table_InverseIP[64] = { 40, 8, 48, 16, 56, 24, 64, 32, 39,
        7, 47, 15, 55, 23, 63, 31, 38, 6, 46, 14, 54, 22, 62, 30, 37, 5, 45, 13,
        53, 21, 61, 29, 36, 4, 44, 12, 52, 20, 60, 28, 35, 3, 43, 11, 51, 19,
        59, 27, 34, 2, 42, 10, 50, 18, 58, 26, 33, 1, 41, 9, 49, 17, 57, 25 };

// 扩展置换
const unsigned char Table_E[48] = { 32, 1, 2, 3, 4, 5, 4, 5, 6, 7, 8, 9, 8, 9,
        10, 11, 12, 13, 12, 13, 14, 15, 16, 17, 16, 17, 18, 19, 20, 21, 20, 21,
        22, 23, 24, 25, 24, 25, 26, 27, 28, 29, 28, 29, 30, 31, 32, 1 };

// 密钥初始置换
const unsigned char Table_PC1[56] = { 57, 49, 41, 33, 25, 17, 9, 1, 58, 50, 42,
        34, 26, 18, 10, 2, 59, 51, 43, 35, 27, 19, 11, 3, 60, 52, 44, 36, 63,
        55, 47, 39, 31, 23, 15, 7, 62, 54, 46, 38, 30, 22, 14, 6, 61, 53, 45,
        37, 29, 21, 13, 5, 28, 20, 12, 4 };

// 左右移运算
const signed char Table_Move[2][16] = {
//加密左移
        { 1, 1, 2, 2, 2, 2, 2, 2, 1, 2, 2, 2, 2, 2, 2, 1 },

        //解密右移
        { 0, -1, -2, -2, -2, -2, -2, -2, -1, -2, -2, -2, -2, -2, -2, -1 } };

// 密钥压缩置换
const unsigned char Table_PC2[48] = { 14, 17, 11, 24, 1, 5, 3, 28, 15, 6, 21,
        10, 23, 19, 12, 4, 26, 8, 16, 7, 27, 20, 13, 2, 41, 52, 31, 37, 47, 55,
        30, 40, 51, 45, 33, 48, 44, 49, 39, 56, 34, 53, 46, 42, 50, 36, 29, 32 };

// S盒
const unsigned char Table_SBOX[8][4][16] = {
        // S1
        14, 4, 13, 1, 2, 15, 11, 8, 3, 10, 6, 12, 5, 9, 0, 7, 0, 15, 7, 4, 14,
        2, 13, 1, 10, 6, 12, 11, 9, 5, 3, 8, 4, 1, 14, 8, 13, 6, 2, 11, 15, 12,
        9, 7, 3, 10, 5, 0, 15, 12, 8, 2, 4, 9, 1, 7, 5, 11, 3, 14, 10, 0, 6, 13,
        // S2
        15, 1, 8, 14, 6, 11, 3, 4, 9, 7, 2, 13, 12, 0, 5, 10, 3, 13, 4, 7, 15,
        2, 8, 14, 12, 0, 1, 10, 6, 9, 11, 5, 0, 14, 7, 11, 10, 4, 13, 1, 5, 8,
        12, 6, 9, 3, 2, 15, 13, 8, 10, 1, 3, 15, 4, 2, 11, 6, 7, 12, 0, 5, 14,
        9,
        // S3
        10, 0, 9, 14, 6, 3, 15, 5, 1, 13, 12, 7, 11, 4, 2, 8, 13, 7, 0, 9, 3, 4,
        6, 10, 2, 8, 5, 14, 12, 11, 15, 1, 13, 6, 4, 9, 8, 15, 3, 0, 11, 1, 2,
        12, 5, 10, 14, 7, 1, 10, 13, 0, 6, 9, 8, 7, 4, 15, 14, 3, 11, 5, 2, 12,
        // S4
        7, 13, 14, 3, 0, 6, 9, 10, 1, 2, 8, 5, 11, 12, 4, 15, 13, 8, 11, 5, 6,
        15, 0, 3, 4, 7, 2, 12, 1, 10, 14, 9, 10, 6, 9, 0, 12, 11, 7, 13, 15, 1,
        3, 14, 5, 2, 8, 4, 3, 15, 0, 6, 10, 1, 13, 8, 9, 4, 5, 11, 12, 7, 2, 14,
        // S5
        2, 12, 4, 1, 7, 10, 11, 6, 8, 5, 3, 15, 13, 0, 14, 9, 14, 11, 2, 12, 4,
        7, 13, 1, 5, 0, 15, 10, 3, 9, 8, 6, 4, 2, 1, 11, 10, 13, 7, 8, 15, 9,
        12, 5, 6, 3, 0, 14, 11, 8, 12, 7, 1, 14, 2, 13, 6, 15, 0, 9, 10, 4, 5,
        3,
        // S6
        12, 1, 10, 15, 9, 2, 6, 8, 0, 13, 3, 4, 14, 7, 5, 11, 10, 15, 4, 2, 7,
        12, 9, 5, 6, 1, 13, 14, 0, 11, 3, 8, 9, 14, 15, 5, 2, 8, 12, 3, 7, 0, 4,
        10, 1, 13, 11, 6, 4, 3, 2, 12, 9, 5, 15, 10, 11, 14, 1, 7, 6, 0, 8, 13,
        // S7
        4, 11, 2, 14, 15, 0, 8, 13, 3, 12, 9, 7, 5, 10, 6, 1, 13, 0, 11, 7, 4,
        9, 1, 10, 14, 3, 5, 12, 2, 15, 8, 6, 1, 4, 11, 13, 12, 3, 7, 14, 10, 15,
        6, 8, 0, 5, 9, 2, 6, 11, 13, 8, 1, 4, 10, 7, 9, 5, 0, 15, 14, 2, 3, 12,
        // S8
        13, 2, 8, 4, 6, 15, 11, 1, 10, 9, 3, 14, 5, 0, 12, 7, 1, 15, 13, 8, 10,
        3, 7, 4, 12, 5, 6, 11, 0, 14, 9, 2, 7, 11, 4, 1, 9, 12, 14, 2, 0, 6, 10,
        13, 15, 3, 5, 8, 2, 1, 14, 7, 4, 10, 8, 13, 15, 12, 9, 0, 3, 5, 6, 11 };

// P盒置换
const unsigned char Table_P[32] = { 16, 7, 20, 21, 29, 12, 28, 17, 1, 15, 23,
        26, 5, 18, 31, 10, 2, 8, 24, 14, 32, 27, 3, 9, 19, 13, 30, 6, 22, 11, 4,
        25 };

//检查key的长度，返回加密模式
KEYMODE Des::checkKey() {
    if (deskey.length() != 0) {
        if (deskey.length() != 32 && deskey.length() != 48) {
            printf("key输入不正确，请输入32位key或者48位key");
            return KEY_ERROR;
        }

        if (deskey.length() % 2 != 0) {
            printf("请检查输入的位数是否是偶数位");
            return KEY_ERROR;
        }

        if (deskey.length() == 32) {
            return KEY_32;
        }

        if (deskey.length() == 48) {
            return KEY_48;
        }
    } else {
        return KEY_ERROR;
    }
    return KEY_ERROR;
}

KEYMODE Des::checkKey(std::string key) {
    deskey = key;

    if (deskey.length() != 32 && deskey.length() != 48) {
        printf("key输入不正确，请输入32位key或者48位key");
        return KEY_ERROR;
    }

    if (deskey.length() % 2 != 0) {
        printf("请检查输入的位数是否是偶数位");
        return KEY_ERROR;
    }

    if (deskey.length() == 32) {
        return KEY_32;
    }

    if (deskey.length() == 48) {
        return KEY_48;
    }
    return KEY_ERROR;
}

void Des::print_binary(char * tip, unsigned char * buff, unsigned char bits) {
    unsigned char bit_index = 0;

    printf("\r\n****** start %s ******\r\n", tip);

    for (bit_index = 0; bit_index < bits; bit_index++) {
        printf("%d", (buff[bit_index >> 3] >> (7 - (bit_index % 8))) & 0x01);

        if ((bit_index + 1) % 4 == 0) {
            printf(" ");
        }

        if ((bit_index + 1) % 64 == 0) {
            printf("\r\n");
        }
    }

    printf("\r\n****** end %s ******\r\n", tip);
}

void Des::print_hex(char * tip, unsigned char * buff, unsigned char bytes) {
    unsigned char byte_index = 0;

    printf("\r\n %s", tip);

    for (byte_index = 0; byte_index < bytes; byte_index++) {
        //LOGI("0x%0.2X ", buff[byte_index]);
    }
}

//对两块大小相同的内存区进行异或
//异或结果保存到第一块内存
//unsigned char * p_buf_1       内存区1
//const unsigned char * p_buf_2 内存区2
//unsigned char bytes           内存区大小(单位：字节)
void Des::Xor(unsigned char * p_buf_1, const unsigned char * p_buf_2,
        unsigned char bytes) {
    while (bytes > 0) {
        bytes--;

        p_buf_1[bytes] ^= p_buf_2[bytes];
    }
}

//将缓冲区从第bit_start位到第bit_end进行循环左移
//offset只能是1，2
//本段代码还可以优化。
void Des::move_left(unsigned char * p_input, unsigned char bit_start,
        unsigned char bit_end, unsigned char offset) {
    unsigned char b_val = 0;
    unsigned char b_tmp1 = 0;
    unsigned char b_tmp2 = 0;

    //读取bit_start位
    b_tmp1 = GET_BIT(p_input, bit_start);
    b_tmp2 = GET_BIT(p_input, bit_start + 1);

    //循环左移offset位
    for (; bit_start <= (bit_end - offset); bit_start++) {
        b_val = GET_BIT(p_input, bit_start + offset);
        SET_BIT(p_input, bit_start, b_val);
    }

    //将bit_start开始的offset位移到bit_end后头来
    if (1 == offset) {
        SET_BIT(p_input, bit_end, b_tmp1);
    } else {
        SET_BIT(p_input, bit_end, b_tmp2);
        SET_BIT(p_input, bit_end - 1, b_tmp1);
    }
}

//将缓冲区从第bit_start位到第bit_end进行循环右移
//offset只能是1，2
//本段代码在性能上还可以优化。
void Des::move_right(unsigned char * p_input, unsigned char bit_start,
        unsigned char bit_end, unsigned char offset) {
    unsigned char b_val = 0;
    unsigned char b_tmp1 = 0;
    unsigned char b_tmp2 = 0;

    //读取bit_end位
    b_tmp1 = GET_BIT(p_input, bit_end);
    b_tmp2 = GET_BIT(p_input, bit_end - 1);

    //循环左移offset位
    for (; bit_end >= (bit_start + offset); bit_end--) {
        b_val = GET_BIT(p_input, bit_end - offset);
        SET_BIT(p_input, bit_end, b_val);
    }

    //将bit_end倒数的offset位移到bit_start来
    if (1 == offset) {
        SET_BIT(p_input, bit_start, b_tmp1);
    } else {
        SET_BIT(p_input, bit_start, b_tmp2);
        SET_BIT(p_input, bit_start + 1, b_tmp1);
    }
}

//缓冲区移位
//offset大于0时左移
//offset小于0时右移
void Des::move_bits(unsigned char * p_input, unsigned char bit_start,
        unsigned char bit_end, char offset) {
    if (0 < offset) //左移
            {
        move_left(p_input, bit_start, bit_end, offset);
    } else if (0 > offset) //右移
            {
        move_right(p_input, bit_start, bit_end, -offset);
    }
}

//通用置换函数, bits <= 64
//p_input与p_output不能指向同一个地址，否则置换会出错。
void Des::Permutation(unsigned char * p_input, unsigned char * p_output,
        const unsigned char * Table, unsigned char bits) {
    unsigned char b_val = false;
    unsigned char bit_index = 0;

    for (bit_index = 0; bit_index < bits; bit_index++) {
        b_val = GET_BIT(p_input, Table[bit_index] - 1);

        SET_BIT(p_output, bit_index, b_val);
    }
}

//获取从bit_s为起始的第1, 6 位组成行
unsigned char Des::S_GetLine(unsigned char * p_data_ext, unsigned char bit_s) {
    return (GET_BIT(p_data_ext, bit_s + 0) << 1)
            + GET_BIT(p_data_ext, bit_s + 5);
}

//获取从bit_s为起始的第2,3,4,5位组成列
unsigned char Des::S_GetRow(unsigned char * p_data_ext, unsigned char bit_s) {
    unsigned char row;

    //2,3,4,5位组成列
    row = GET_BIT(p_data_ext, bit_s + 1);
    row <<= 1;
    row += GET_BIT(p_data_ext, bit_s + 2);
    row <<= 1;
    row += GET_BIT(p_data_ext, bit_s + 3);
    row <<= 1;
    row += GET_BIT(p_data_ext, bit_s + 4);

    return row;
}

///////////////////////////////////////////////////////////////
//  函 数 名 : des
//  函数功能 : DES加解密
//  处理过程 : 根据标准的DES加密算法用输入的64位密钥对64位密文进行加/解密
//              并将加/解密结果存储到p_output里
//  时    间 : 2006年9月2日
//  返 回 值 :
//  参数说明 :  const char * p_data     输入, 加密时输入明文, 解密时输入密文, 64位(8字节)
//              const char * p_key      输入, 密钥, 64位(8字节)
//              char * p_output         输出, 加密时输出密文, 解密时输入明文, 64位(8字节)
//              unsigned char mode              DES_ENCRYPT 加密  DES_DECRYPT 解密
///////////////////////////////////////////////////////////////
void Des::des(const char * p_data, const char * p_key, const char * p_output,
        DES_MODE mode) {
    unsigned char loop = 0; //16轮运算的循环计数器
    unsigned char key_tmp[8]; //密钥运算时存储中间结果
    unsigned char sub_key[6]; //用于存储子密钥

    unsigned char * p_left;
    const char* p_right;

    unsigned char p_right_ext[8]; //R[i]经过扩展置换生成的48位数据(6字节), 及最终结果的存储
    unsigned char p_right_s[4]; //经过S_BOX置换后的32位数据(4字节)

    unsigned char s_loop = 0; //S_BOX置换的循环计数器

    //密钥第一次缩小换位, 得到一组56位的密钥数据
    Permutation((unsigned char*) p_key, key_tmp, Table_PC1, 56);

    //明文初始化置换
    Permutation((unsigned char*) p_data, (unsigned char*) p_output, Table_IP,
            64);

    p_left = (unsigned char*) p_output; //L0
    p_right = &p_output[4]; //R0

    for (loop = 0; loop < 16; loop++) {
        //把缩进小后的把这56位分为左28位和右28位,
        //对左28位和右28位分别循环左/右移, 得到一组新数据
        //加解密操作时只在移位时有差异
        move_bits(key_tmp, 0, 27, Table_Move[mode][loop]);
        move_bits(key_tmp, 28, 55, Table_Move[mode][loop]);

        //密钥第二次缩小换位，得到一组子48位的子密钥
        Permutation(key_tmp, sub_key, Table_PC2, 48);

        //R0扩展置换
        Permutation((unsigned char*) p_right, p_right_ext, Table_E, 48);

        //将R0扩展置换后得到的48位数据(6字节)与子密钥进行异或
        Xor(p_right_ext, sub_key, 6);

        //S_BOX置换
        for (s_loop = 0; s_loop < 4; s_loop++) {
            unsigned char s_line = 0;
            unsigned char s_row = 0;
            unsigned char s_bit = s_loop * 12;

            s_line = S_GetLine(p_right_ext, s_bit);
            s_row = S_GetRow(p_right_ext, s_bit);

            p_right_s[s_loop] = Table_SBOX[s_loop * 2][s_line][s_row];

            s_bit += 6;

            s_line = S_GetLine(p_right_ext, s_bit);
            s_row = S_GetRow(p_right_ext, s_bit);

            p_right_s[s_loop] <<= 4;
            p_right_s[s_loop] += Table_SBOX[(s_loop * 2) + 1][s_line][s_row];
        }

        //P置换
        Permutation(p_right_s, p_right_ext, Table_P, 32);

        Xor(p_right_ext, p_left, 4);

        memcpy(p_left, (unsigned char*) p_right, 4);
        memcpy((unsigned char*) p_right, p_right_ext, 4);
    }

    memcpy(&p_right_ext[4], p_left, 4);
    memcpy(p_right_ext, p_right, 4);

    //最后再进行一次逆置换, 得到最终加密结果
    Permutation(p_right_ext, (unsigned char*) p_output, Table_InverseIP, 64);
}

/************************************************************************
 *功能：字符串转换为16进制表示，每两位为一组
 ************************************************************************/
unsigned char* Des::str_to_hex(const std::string& str) {
    unsigned char *buff = NULL;
    int size = sizeof(unsigned char *) * str.length() / 2;
    buff = (unsigned char*) malloc(size);
    memset((void*) buff, 0, size);

    std::stringstream ss;
    ss << std::hex;
    for (int i = 0; i != str.size() / 2; ++i) {
        int val = 0;
        ss << str.substr(i * 2, 2);
        ss >> val;
        buff[i] = val;
        ss.clear();
    }
    return buff;
}

/************************************************************************
 *功能：获得3DES分组的key
 ************************************************************************/
unsigned char *Des::getKey1(std::string key) {
    unsigned char *key1;
    key1 = str_to_hex(key.substr(0, 16));
    return key1;
}

unsigned char *Des::getKey2(std::string key) {
    unsigned char *key2;
    key2 = str_to_hex(key.substr(16, 32));
    return key2;
}

unsigned char *Des::getKey3(std::string key) {
    unsigned char *key3;
    key3 = str_to_hex(key.substr(32, 48));
    return key3;
}

/************************************************************************
 *功能：数组清空
 ************************************************************************/
void Des::clearBuff(char *buff, int length) {
    for (int i = 0; i < length; i++) {
        buff[i] = '\0';
    }
}

std::string Des::base64Encode(const unsigned char* Data, int DataByte) {
    //编码表
    const char EncodeTable[] =
            "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";
    //返回值
    std::string strEncode;
    unsigned char Tmp[4] = { 0 };
    int LineLength = 0;
    for (int i = 0; i < (int) (DataByte / 3); i++) {
        Tmp[1] = *Data++;
        Tmp[2] = *Data++;
        Tmp[3] = *Data++;
        strEncode += EncodeTable[Tmp[1] >> 2];
        strEncode += EncodeTable[((Tmp[1] << 4) | (Tmp[2] >> 4)) & 0x3F];
        strEncode += EncodeTable[((Tmp[2] << 2) | (Tmp[3] >> 6)) & 0x3F];
        strEncode += EncodeTable[Tmp[3] & 0x3F];
        if (LineLength += 4, LineLength == 76) {
            strEncode += "\r\n";
            LineLength = 0;
        }
    }
    //对剩余数据进行编码
    int Mod = DataByte % 3;
    if (Mod == 1) {
        Tmp[1] = *Data++;
        strEncode += EncodeTable[(Tmp[1] & 0xFC) >> 2];
        strEncode += EncodeTable[((Tmp[1] & 0x03) << 4)];
        strEncode += "==";
    } else if (Mod == 2) {
        Tmp[1] = *Data++;
        Tmp[2] = *Data++;
        strEncode += EncodeTable[(Tmp[1] & 0xFC) >> 2];
        strEncode +=
                EncodeTable[((Tmp[1] & 0x03) << 4) | ((Tmp[2] & 0xF0) >> 4)];
        strEncode += EncodeTable[((Tmp[2] & 0x0F) << 2)];
        strEncode += "=";
    }

    return strEncode;
}

std::string Des::base64Decode(const char* Data, int DataByte, int& OutByte) {
    //解码表
    const char DecodeTable[] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
            0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
            0, 0, 0, 0,
            62, // '+'
            0, 0, 0,
            63, // '/'
            52, 53, 54, 55, 56, 57, 58, 59, 60,
            61, // '0'-'9'
            0, 0, 0, 0, 0, 0, 0, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13,
            14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24,
            25, // 'A'-'Z'
            0, 0, 0, 0, 0, 0, 26, 27, 28, 29, 30, 31, 32, 33, 34, 35, 36, 37,
            38, 39, 40, 41, 42, 43, 44, 45, 46, 47, 48, 49, 50, 51, // 'a'-'z'
            };
    //返回值
    std::string strDecode;
    int nValue;
    int i = 0;
    while (i < DataByte) {
        if (*Data != '\r' && *Data != '\n') {
            nValue = DecodeTable[*Data++] << 18;
            nValue += DecodeTable[*Data++] << 12;
            strDecode += (nValue & 0x00FF0000) >> 16;
            OutByte++;
            if (*Data != '=') {
                nValue += DecodeTable[*Data++] << 6;
                strDecode += (nValue & 0x0000FF00) >> 8;
                OutByte++;
                if (*Data != '=') {
                    nValue += DecodeTable[*Data++];
                    strDecode += nValue & 0x000000FF;
                    OutByte++;
                }
            }
            i += 4;
        } else // 回车换行,跳过
        {
            Data++;
            i++;
        }
    }
    return strDecode;
}

/************************************************************************
 *功能：URL字符转换
 ************************************************************************/
unsigned char Des::ToHex(unsigned char x) {
    return x > 9 ? x + 55 : x + 48;
}

unsigned char Des::FromHex(unsigned char x) {
    unsigned char y = NULL;
    if (x >= 'A' && x <= 'Z')
        y = x - 'A' + 10;
    else if (x >= 'a' && x <= 'z')
        y = x - 'a' + 10;
    else if (x >= '0' && x <= '9')
        y = x - '0';
    return y;
}

/************************************************************************
 *功能：URL-ENCODE编码
 ************************************************************************/
std::string Des::UrlEncode(const std::string& str) {
    std::string strTemp = "";
    size_t length = str.length();
    for (size_t i = 0; i < length; i++) {
        if (isalnum((unsigned char) str[i]) || (str[i] == '-')
                || (str[i] == '_') || (str[i] == '.') || (str[i] == '~'))
            strTemp += str[i];
        else if (str[i] == ' ')
            strTemp += "+";
        else {
            strTemp += '%';
            strTemp += ToHex((unsigned char) str[i] >> 4);
            strTemp += ToHex((unsigned char) str[i] % 16);
        }
    }
    return strTemp;
}

/************************************************************************
 *功能：URL-DECODE解码
 ************************************************************************/
std::string Des::UrlDecode(const std::string& str) {
    std::string strTemp = "";
    size_t length = str.length();
    for (size_t i = 0; i < length; i++) {
        if (str[i] == '+')
            strTemp += ' ';
        else if (str[i] == '%') {
            //assert(i + 2 < length);
            unsigned char high = FromHex((unsigned char) str[++i]);
            unsigned char low = FromHex((unsigned char) str[++i]);
            strTemp += high * 16 + low;
        } else
            strTemp += str[i];
    }
    return strTemp;
}

//替换字符串
std::string Des::replaceAllSubStr(string &str, string &oldStr, string &newStr) {
    while (true) {
        string::size_type pos(0);
        if ((pos = str.find(oldStr)) != string::npos) {
            str.replace(pos, oldStr.length(), newStr);
        } else {
            break;
        }
    }
    return str;
}

/************************************************************************
 *功能：3DES-ECB加密
 ************************************************************************/
std::string Des::desEncrypt(std::string str) {
    auto mode = checkKey(deskey);
    if (mode == KEY_ERROR) {
        return nullptr;
    }

    int block = 0;
    int len = str.length();
    std::string encode = "";
    std::string result = "";

    char *key1 = NULL;
    char *key2 = NULL;
    char *key3 = NULL;
    if (mode == KEY_16) {
        key1 = (char*) getKey1(deskey);
        key2 = (char*) getKey1(deskey);
        key3 = (char*) getKey1(deskey);
    }
    if (mode == KEY_32) {
        key1 = (char*) getKey1(deskey);
        key2 = (char*) getKey2(deskey);
        key3 = (char*) getKey1(deskey);
    }
    if (mode == KEY_48) {
        key1 = (char*) getKey1(deskey);
        key2 = (char*) getKey2(deskey);
        key3 = (char*) getKey3(deskey);
    }

    if (len % 8 == 0) {
        block = len / 8;
    } else {
        block = len / 8 + 1;
    }

    //LOGI("block = %d", block);
    unsigned char *result1 = NULL;
    int size = sizeof(unsigned char *) * 8 * block + 1;
    result1 = (unsigned char*) malloc(size);
    memset(result1, 0, size);

    //3DES 加密
    char en_out_put[9] = { 0 };
    char en_data[9] = { 0 };

    for (int i = 0; i < block; i++) {
        clearBuff(en_data, 9);
        if (i == block - 1) {
            if (len % 8 == 0) {
                memcpy(en_data, str.c_str() + (i * 8), 8);
            } else {
                memcpy(en_data, str.c_str() + (i * 8), len % 8);
            }
        } else {
            memcpy(en_data, str.c_str() + (i * 8), 8);
        }
        //LOGI("en_data = %s", en_data);
        des(en_data, key1, en_out_put, DES_ENCRYPT);
        des(en_out_put, key2, en_data, DES_DECRYPT);
        des(en_data, key3, en_out_put, DES_ENCRYPT);

        //result.append(en_out_put);
        for (int n = 0; n < 8; n++) {
            result1[(i * 8) + n] = en_out_put[n];
        }
    }
    encode = base64Encode((unsigned char*) result1, block * 8);
    //encode = base64Encode((unsigned char*) result.c_str(), block * 8);

    free(key1);
    free(key2);
    free(key3);

    free(result1);

    return encode;
}

/************************************************************************
 *功能：3DES-ECB解密
 ************************************************************************/
std::string Des::desDecrypt(std::string str) {
    auto mode = checkKey(deskey);
    if (mode == KEY_ERROR) {
        return nullptr;
    }

    char *key1 = NULL;
    char *key2 = NULL;
    char *key3 = NULL;
    if (mode == KEY_16) {
        key1 = (char*) getKey1(deskey);
        key2 = (char*) getKey1(deskey);
        key3 = (char*) getKey1(deskey);
    }
    if (mode == KEY_32) {
        key1 = (char*) getKey1(deskey);
        key2 = (char*) getKey2(deskey);
        key3 = (char*) getKey1(deskey);
    }
    if (mode == KEY_48) {
        key1 = (char*) getKey1(deskey);
        key2 = (char*) getKey2(deskey);
        key3 = (char*) getKey3(deskey);
    }

    int OutByte = 0;
    std::string decode = "";
    std::string result = "";

    decode = base64Decode(str.c_str(), str.length(), OutByte);

    //如果加密字符串经过解密后长度不为8的倍数，则返回NULL
    if(OutByte % 8 != 0) {
        //LOGI("OutByte = %d", OutByte);
        return "isWrongInput";
    }

    int block = 0;
    block = OutByte / 8;

    //3DES 加密
    char en_out_put[9] = { 0 };
    char en_data[9] = { 0 };

    for (int i = 0; i < block; i++) {
        memcpy(en_out_put, decode.c_str() + (i * 8), 8);
        des(en_out_put, key3, en_data, DES_DECRYPT);
        des(en_data, key2, en_out_put, DES_ENCRYPT);
        des(en_out_put, key1, en_data, DES_DECRYPT);
        result += en_data;
    }

    free(key1);
    free(key2);
    free(key3);

    return result;
}