// Added by Makeit, on 2018/12/18.
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
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
};