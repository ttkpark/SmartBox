// Added by Makeit, on 2018/12/18.
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#include "pch.h"
#include "Base64Coder.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
//#include <endian.h>
#include <iostream>



#ifdef _DEBUG
#define new DEBUG_NEW
#endif



///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// macro constants and pre-defines.

static const char MimeBase64[] = {
 'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H',
 'I', 'J', 'K', 'L', 'M', 'N', 'O', 'P',
 'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X',
 'Y', 'Z', 'a', 'b', 'c', 'd', 'e', 'f',
 'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n',
 'o', 'p', 'q', 'r', 's', 't', 'u', 'v',
 'w', 'x', 'y', 'z', '0', '1', '2', '3',
 '4', '5', '6', '7', '8', '9', '+', '/'
};

static int DecodeMimeBase64[256] = {
 -1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
 -1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
 -1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,62,-1,-1,-1,63,
 52,53,54,55,56,57,58,59,60,61,-1,-1,-1,-1,-1,-1,
 -1, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9,10,11,12,13,14,
 15,16,17,18,19,20,21,22,23,24,25,-1,-1,-1,-1,-1,
 -1,26,27,28,29,30,31,32,33,34,35,36,37,38,39,40,
 41,42,43,44,45,46,47,48,49,50,51,-1,-1,-1,-1,-1,
 -1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
 -1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
 -1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
 -1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
 -1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
 -1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
 -1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
 -1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1
};

typedef union {
    struct {
#ifdef LITTLEENDIAN
        unsigned char c1, c2, c3;
#else
        unsigned char c3, c2, c1;
#endif
    };
    struct {
#ifdef LITTLEENDIAN
        unsigned int e1 : 6, e2 : 6, e3 : 6, e4 : 6;
#else
        unsigned int e4 : 6, e3 : 6, e2 : 6, e1 : 6;
#endif
    };
} UST_BUF;




///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// constructor
Base64Coder::Base64Coder()
{
}


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// destructor
Base64Coder::~Base64Coder()
{
}


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// encoder
int Base64Coder::_base64enc(const char* pSrcBuf, UINT nSrcSize, char** ppResult)
{
    ASSERT(pSrcBuf != nullptr);

    UINT nSize = 0;
    UST_BUF tempBuf;

    if (nSrcSize > 0)
    {
        nSize = (4 * (nSrcSize / 3)) + (nSrcSize % 3 ? 4 : 0) + 1;

        (*ppResult) = new char[nSize];

        UINT j = 0;
        for (UINT i = 0; i < nSrcSize; i = i + 3, j = j + 4)
        {
            tempBuf.c3 = pSrcBuf[i];
            if ((i + 1) > nSrcSize)
                tempBuf.c2 = 0;
            else
                tempBuf.c2 = pSrcBuf[i + 1];

            if ((i + 2) > nSrcSize)
                tempBuf.c1 = 0;
            else
                tempBuf.c1 = pSrcBuf[i + 2];

            (*ppResult)[j] = MimeBase64[tempBuf.e4];
            (*ppResult)[j + 1] = MimeBase64[tempBuf.e3];
            (*ppResult)[j + 2] = MimeBase64[tempBuf.e2];
            (*ppResult)[j + 3] = MimeBase64[tempBuf.e1];

            if ((i + 2) > nSrcSize)
                (*ppResult)[j + 2] = '=';
            if ((i + 3) > nSrcSize)
                (*ppResult)[j + 3] = '=';
        }

        (*ppResult)[nSize - 1] = 0;
    }
    else
    {
        ASSERT(0);
    }

    return nSize;
}


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// decoder
void Base64Coder::_base64dec(const char* pSrcBuf, UINT nSrcSize, OUT BYTE** ppResult, OUT UINT* pnLength)
{
    ASSERT(nSrcSize > 0);

    // init
    *ppResult = nullptr;
    *pnLength = 0;

    BYTE* pResult = nullptr;

    int j = 0, nBlank = 0;
    UST_BUF temp;

    // out buffer를 여기서 직접 생성하기로 한다.
    const UINT nOutBufSize = nSrcSize * 3 / 4 + 1;
    if (nOutBufSize > 0)
    {
        pResult = new BYTE[nOutBufSize];
        ASSERT(pResult);

        for (UINT i = 0; i < nSrcSize; i = i + 4, j = j + 3)
        {
            temp.e4 = DecodeMimeBase64[pSrcBuf[i]];
            temp.e3 = DecodeMimeBase64[pSrcBuf[i + 1]];
            if (pSrcBuf[i + 2] == '=')
            {
                temp.e2 = 0;
                nBlank++;
            }
            else
                temp.e2 = DecodeMimeBase64[pSrcBuf[i + 2]];

            if (pSrcBuf[i + 3] == '=')
            {
                temp.e1 = 0;
                nBlank++;
            }
            else
                temp.e1 = DecodeMimeBase64[pSrcBuf[i + 3]];

            pResult[j] = temp.c3;
            pResult[j + 1] = temp.c2;
            pResult[j + 2] = temp.c1;
        }
    }

    // check and set results to return.
    if (j - nBlank > 0)
    {
        *pnLength = j - nBlank;
        ASSERT(*pnLength <= nOutBufSize - 1);

        *ppResult = (BYTE*)pResult;
    }
    else
    {
        // clear
        if (pResult)
        {
            delete pResult;
            pResult = nullptr;
        }
    }
}


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Unicode(Wide char)를 UTF-8로 변환하는 함수. (출처 기억 안남)
void wstrToUtf8(std::string& dest, const std::wstring& src)
{
    dest.clear();

    for (size_t i = 0; i < src.size(); i++)
    {
        wchar_t w = src[i];
        if (w <= 0x7f)
            dest.push_back((char)w);
        else if (w <= 0x7ff)
        {
            dest.push_back(0xc0 | ((w >> 6) & 0x1f));
            dest.push_back(0x80 | (w & 0x3f));
        }
        else if (w <= 0xffff)
        {
            dest.push_back(0xe0 | ((w >> 6 >> 6) & 0x0f));
            dest.push_back(0x80 | ((w >> 6) & 0x3f));
            dest.push_back(0x80 | (w & 0x3f));
        }
        else if (w <= 0x10ffff)
        {
            dest.push_back(0xf0 | ((w >> 6 >> 6 >> 6) & 0x07));
            dest.push_back(0x80 | ((w >> 6 >> 6) & 0x3f));
            dest.push_back(0x80 | ((w >> 6) & 0x3f));
            dest.push_back(0x80 | (w & 0x3f));
        }
        else
            dest.push_back('?');
    }
}

std::string wstrToUtf8(const std::wstring& str)
{
    std::string result;
    wstrToUtf8(result, str);
    return result;
}


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Encoding
// from Unicode character to TCHAR.
CString Base64Coder::Encoding(LPCWSTR lpszSrc)
{
    CString strReturn;

    // UTF8로 먼저 변환.
    std::wstring strWide = lpszSrc;
    std::string strUtf8 = wstrToUtf8(strWide);

    char* pResult = nullptr;
    const UINT nSize = static_cast<UINT>(strUtf8.length());
    const UINT nConvSize = _base64enc(strUtf8.c_str(), nSize, &pResult);

    if (pResult && nConvSize > 0)
    {
        // TCHAR로 변환
        char* pCpy = new char[nConvSize + 1];
        ::ZeroMemory(pCpy, nConvSize + 1);

        memcpy_s(pCpy, nConvSize + 1, pResult, nConvSize);

        strReturn = CA2T((LPSTR)pCpy);

        delete[] pCpy;
        delete[] pResult;
    }

    return strReturn;
}


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Encoding
// from Bytes data to TCHAR
CString Base64Coder::Encoding(const BYTE* pSrcBytes, UINT nLength)
{
    CString strReturn;

    char* pResult = nullptr;
    const UINT nConvSize = _base64enc((char*)pSrcBytes, nLength, &pResult);

    if (pResult && nConvSize > 0)
    {
        // TCHAR로 변환
        char* pCpy = new char[nConvSize + 1];
        ::ZeroMemory(pCpy, nConvSize + 1);

        memcpy_s(pCpy, nConvSize + 1, pResult, nConvSize);

        strReturn = CA2T((LPSTR)pCpy);

        delete[] pCpy;
        delete[] pResult;
    }

    return strReturn;
}


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Decoding
CString Base64Coder::Decoding(LPCWSTR lpszSrc)
{
    CString strReturn;

    // UTF8로 먼저 변환.
    std::wstring strWide = lpszSrc;
    std::string strUtf8 = wstrToUtf8(strWide);

    const UINT nSize = static_cast<UINT>(strUtf8.length());
    BYTE* pResult = nullptr;
    UINT nConvSize = 0;

    // decoding
    _base64dec(strUtf8.c_str(), nSize, &pResult, &nConvSize);

    // TCHAR로 변경.
    if (pResult && nConvSize > 0)
    {
        // 끝에 null 문자열을 붙이기 위해서 복사.
        char* pCpy = new char[nConvSize + 1];
        ::ZeroMemory(pCpy, nConvSize + 1);

        memcpy_s(pCpy, nConvSize + 1, pResult, nConvSize);

        strReturn = CA2T((LPSTR)pCpy);

        delete[] pCpy;
        delete[] pResult;
    }
    else
    {
        ASSERT(0);
    }

    return strReturn;
}


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Decoding
void Base64Coder::Decoding(LPCWSTR lpszSrc, OUT BYTE** ppResult, OUT UINT& nLength)
{
    // UTF8로 먼저 변환.
    std::wstring strWide = lpszSrc;
    std::string strUtf8 = wstrToUtf8(strWide);

    const UINT nSize = static_cast<UINT>(strUtf8.length());
    UINT nConvSize = 0;

    // decoding
    _base64dec(strUtf8.c_str(), nSize, ppResult, &nConvSize);

    if (nConvSize > 0)
    {
        nLength = nConvSize;
        ASSERT(*ppResult);
    }
    else
    {
        ASSERT(*ppResult == nullptr);
    }
}

