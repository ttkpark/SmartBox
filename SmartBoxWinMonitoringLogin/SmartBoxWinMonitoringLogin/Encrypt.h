#pragma once
#include "pch.h"

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Base64 Encoder / Decoder.
// Notice: 
// 1. ������ ��� �Լ����� static���� ��� �����ϳ� wrapping�Ǿ� �����Ƿ�
//  encoder/decoder �Լ��� �����ϰ�� ���������� ����� �� ����.
// 2. ��ȯ ������� ���ο��� �޸𸮸� �����Ͽ� �����ϹǷ� �ݵ�� �ۿ��� ����(must be deleted by user) �Ǿ�� ��.
// 3. Visual Studio MFC in Windows (only) ����� �����Ͽ� �ۼ���. ���ο��� �����ڵ带 ����ϰ� �ִ�.
// 4. Referenced original source code (thanks) : https://kldp.org/node/109436
// 


class Encrypt
{
public://Base64 Encoding
    // Encoding (Unicode ���ڿ� ����)
    static CString Encoding(LPCWSTR lpszSrc);

    // Encoding (Binary data)
    static CString Encoding(const BYTE* pSrcBytes, UINT nLength);

    // Decoding (���ڿ��� encoding�� ���)
    static CString Decoding(LPCWSTR lpszSrc);

    // Decoding (Binary data)
    static void Decoding(LPCWSTR lpszSrc, OUT BYTE** ppResult, OUT UINT& nLength);

    // encoder
    // Out buffer must be deleted by user.
    static int _base64enc(const char* pSrcBuf, UINT nSrcSize, char** ppResult);

    // decoder
    // Out buffer must be deleted by user.
    static void _base64dec(const char* pcSrc, UINT nSrcSize, OUT BYTE** ppResult, OUT UINT* pnLength);


public://bizEncoding Encoding / Decoding
#pragma region Packet �� / ��ȣȭ
    CString Encrypt::DecodingPacketString(CString mty, CString str);

    CString Encrypt::GetDencodingCodePacketChar(char** ary, size_t sRow, size_t sCol, CString pvalue);

    /// <summary>
    /// Packet ��ȣȭ. string enc = bizEncrypt.EncodingString("890", str);
    /// </summary>
    /// <param name="mty">890 ~ 909</param>
    /// <param name="str">wzms_ko-KR_h00001/WorkZone01/bt77/er00^00^00/tp26.20^ox20.30^ps1018</param>
    /// <returns></returns>
    CString Encrypt::EncodingPacketString(CString mty, CString str);

    CString Encrypt::GetEncodingCodePacketChar(char** ary, size_t sRow, size_t sCol, CString pvalue);
#pragma endregion //Packet ��/��ȣȭ

#pragma region Packet �� / ��ȣȭ 10
    /// <summary>
    /// Packet ��ȣȭ. string denc = bizEncrypt.DecodingPacketString10("110", str);
    /// </summary>
    /// <param name="mty">110 ~ 119</param>
    /// <param name="str">472337534367287571~</param>
    /// <returns></returns>
    CString Encrypt::DecodingPacketString10(CString mty, CString str);

    CString Encrypt::GetDencodingCodePacketChar10(char** ary, size_t sRow, size_t sCol, CString pvalue);

    /// <summary>
    /// Packet ��ȣȭ. CString enc = bizEncrypt.EncodingString10("110", str);
    /// </summary>
    /// <param name="mty">110 ~ 119</param>
    /// <param name="str">wzms_ko-KR_h00001/WorkZone01/bt77/er00^00^00/tp26.20^ox20.30^ps1018</param>
    /// <returns></returns>
    CString Encrypt::EncodingPacketString10(CString mty, CString str);

    CString Encrypt::GetEncodingCodePacketChar10(char** ary, size_t sRow, size_t sCol, CString pvalue);
#pragma endregion //Packet ��/��ȣȭ

    CString bizEncode(CString str, int mty);
    CString bizDecode(CString str);



public:// ���� ��/��ȣȭ
    CString Encoding100(CString data, CString* ret);
    CString Encoding800(CString data, CString* ret);
    CString Base64Encoding(CString data, CString* ret);

};

