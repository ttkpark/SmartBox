// Added by Makeit, on 2018/12/18.
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
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
class /*AFX_EXT_CLASS*/ Base64Coder
{
    ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    // public constructor and destructor
public:
    // constructor
    Base64Coder();

    // destructor
    ~Base64Coder();


    ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    // public methods
public:

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
};