#ifndef _DES_H_
#define _DES_H_
#include <string>
#include <cstring>
#include <sstream>
#include <iostream>
using namespace std;

//为了提高程序效率，把这两个位操作功能定义在宏。

//读取缓冲区的指定位.
#define GET_BIT(p_array, bit_index)  \
((p_array[(bit_index) >> 3] >> (7 - ((bit_index) & 0x07))) & 0x01)

//设置缓冲区的指定位.
#define SET_BIT(p_array, bit_index, bit_val) \
if (1 == (bit_val)) \
{\
p_array[(bit_index) >> 3] |= 0x01 << (7 - ((bit_index) & 0x07));\
}\
else\
{\
p_array[(bit_index) >> 3] &= ~(0x01 << (7 - ((bit_index) & 0x07)));\
}

//加解密标识，这两个标识涉及到对表的读取位置,
//必须保证DES_ENCRYPT = 0 DES_DECRYPT = 1
typedef enum {
    DES_ENCRYPT = 0, DES_DECRYPT = 1
} DES_MODE;

//key
typedef enum {
    //1key模式 等同于des
    KEY_16,
    //2key模式 key1 ＝ key3
    KEY_32,
    //3key模式
    KEY_48,
    KEY_ERROR
} KEYMODE;

class Des {
private:
    std::string deskey;
private:
    //字符输出处理
    void print_binary(char * tip, unsigned char * buff, unsigned char bits);
    void print_hex(char * tip, unsigned char * buff, unsigned char bytes);
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
    //              unsigned char mode              0 加密  1 解密
    ///////////////////////////////////////////////////////////////
    void des(const char * p_data, const char * p_key, const char * p_output,
            DES_MODE mode);
    //对两块大小相同的内存区进行异或
    //异或结果保存到第一块内存
    //unsigned char * p_buf_1       内存区1
    //const unsigned char * p_buf_2 内存区2
    //unsigned char bytes           内存区大小(单位：字节)
    void Xor(unsigned char * p_buf_1, const unsigned char * p_buf_2,
            unsigned char bytes);
    //将缓冲区从第bit_start位到第bit_end进行循环左移
    //offset只能是1，2
    void move_left(unsigned char * p_input, unsigned char bit_start,
            unsigned char bit_end, unsigned char offset);
    //将缓冲区从第bit_start位到第bit_end进行循环右移
    //offset只能是1，2
    void move_right(unsigned char * p_input, unsigned char bit_start,
            unsigned char bit_end, unsigned char offset);
    //缓冲区移位
    //offset大于0时左移
    //offset小于0时右移
    void move_bits(unsigned char * p_input, unsigned char bit_start,
            unsigned char bit_end, char offset);
    //通用置换函数, bits <= 64
    //p_input与p_output不能指向同一个地址，否则置换会出错。
    void Permutation(unsigned char * p_input, unsigned char * p_output,
            const unsigned char * Table, unsigned char bits);
    //获取从bit_s为起始的第1, 6 位组成行
    unsigned char S_GetLine(unsigned char * p_data_ext, unsigned char bit_s);
    //获取从bit_s为起始的第2,3,4,5位组成列
    unsigned char S_GetRow(unsigned char * p_data_ext, unsigned char bit_s);

    /************************************************************************
     *功能：字符串转换为16进制表示，每两位为一组
     ************************************************************************/
    unsigned char* str_to_hex(const std::string& str);
    /************************************************************************
     *功能：获得3DES分组的key
     ************************************************************************/
    unsigned char *getKey1(std::string key);
    unsigned char *getKey2(std::string key);
    unsigned char *getKey3(std::string key);
    /************************************************************************
     *功能：数组清空
     ************************************************************************/
    void clearBuff(char *buff, int length);
    /*编码
     DataByte
     [in]输入的数据长度,以字节为单位
     */
    std::string base64Encode(const unsigned char* Data, int DataByte);
    /*解码
     DataByte
     [in]输入的数据长度,以字节为单位
     OutByte
     [out]输出的数据长度,以字节为单位,请不要通过返回值计算
     输出数据的长度
     */
    std::string base64Decode(const char* Data, int DataByte, int& OutByte);

    //功能：URL字符转换
    unsigned char ToHex(unsigned char x);
    unsigned char FromHex(unsigned char x);
    //功能：URL-ENCODE编码
    std::string UrlEncode(const std::string& str);
    //功能：URL-DECODE解码
    std::string UrlDecode(const std::string& str);

    //替换字符串
    std::string replaceAllSubStr(string &str, string &oldStr, string &newStr);

    //检查key的长度，返回加密模式
    KEYMODE checkKey();
    KEYMODE checkKey(std::string key);

public:
    Des() {
    }
    ;
    Des(std::string key) {
        deskey = key;
    }
    ;

    ~Des() {
        deskey = nullptr;
    }
    ;
    /************************************************************************
     *功能：3DES-ECB加密
     ************************************************************************/
    std::string desEncrypt(std::string str);

    /************************************************************************
     *功能：3DES-ECB解密
     ************************************************************************/
    std::string desDecrypt(std::string str);

};
#endif //#ifndef _DES_H_