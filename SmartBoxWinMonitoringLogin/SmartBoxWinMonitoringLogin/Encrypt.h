#pragma once
#include "pch.h"

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Base64 Encoder / Decoder.
// Notice: 
// 1. 구현된 모든 함수들은 static으로 사용 가능하나 wrapping되어 있으므로
//  encoder/decoder 함수를 제외하고는 개별적으로 사용할 수 없음.
// 2. 변환 결과물은 내부에서 메모리를 생성하여 리턴하므로 반드시 밖에서 삭제(must be deleted by user) 되어야 함.
// 3. Visual Studio MFC in Windows (only) 사용을 전제하여 작성됨. 내부에서 유니코드를 사용하고 있다.
// 4. Referenced original source code (thanks) : https://kldp.org/node/109436
// 


class Encrypt
{
public://Base64 Encoding
    // Encoding (Unicode 문자열 전용)
    static CString Encoding(LPCWSTR lpszSrc);

    // Encoding (Binary data)
    static CString Encoding(const BYTE* pSrcBytes, UINT nLength);

    // Decoding (문자열을 encoding한 경우)
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
#pragma region Packet 암 / 복호화
    CString Encrypt::DecodingPacketString(CString mty, CString str);

    CString Encrypt::GetDencodingCodePacketChar(char** ary, size_t sRow, size_t sCol, CString pvalue);

    /// <summary>
    /// Packet 암호화. string enc = bizEncrypt.EncodingString("890", str);
    /// </summary>
    /// <param name="mty">890 ~ 909</param>
    /// <param name="str">wzms_ko-KR_h00001/WorkZone01/bt77/er00^00^00/tp26.20^ox20.30^ps1018</param>
    /// <returns></returns>
    CString Encrypt::EncodingPacketString(CString mty, CString str);

    CString Encrypt::GetEncodingCodePacketChar(char** ary, size_t sRow, size_t sCol, CString pvalue);
#pragma endregion //Packet 암/복호화

#pragma region Packet 암 / 복호화 10
    /// <summary>
    /// Packet 복호화. string denc = bizEncrypt.DecodingPacketString10("110", str);
    /// </summary>
    /// <param name="mty">110 ~ 119</param>
    /// <param name="str">472337534367287571~</param>
    /// <returns></returns>
    CString Encrypt::DecodingPacketString10(CString mty, CString str);

    CString Encrypt::GetDencodingCodePacketChar10(char** ary, size_t sRow, size_t sCol, CString pvalue);

    /// <summary>
    /// Packet 암호화. CString enc = bizEncrypt.EncodingString10("110", str);
    /// </summary>
    /// <param name="mty">110 ~ 119</param>
    /// <param name="str">wzms_ko-KR_h00001/WorkZone01/bt77/er00^00^00/tp26.20^ox20.30^ps1018</param>
    /// <returns></returns>
    CString Encrypt::EncodingPacketString10(CString mty, CString str);

    CString Encrypt::GetEncodingCodePacketChar10(char** ary, size_t sRow, size_t sCol, CString pvalue);
#pragma endregion //Packet 암/복호화

    CString bizEncode(CString str, int mty);
    CString bizDecode(CString str);



public:// 종합 암/복호화
    CString Encoding100(CString data, CString* ret);
    CString Encoding800(CString data, CString* ret);
    CString Base64Encoding(CString data, CString* ret);

};

