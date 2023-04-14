#pragma once

class PacketCoder
{
public:
#pragma region Packet �� / ��ȣȭ
    CString PacketCoder::DecodingPacketString(CString mty, CString str);

    CString PacketCoder::GetDencodingCodePacketChar(char** ary, size_t sRow, size_t sCol, CString pvalue);

    /// <summary>
    /// Packet ��ȣȭ. string enc = bizEncrypt.EncodingString("890", str);
    /// </summary>
    /// <param name="mty">890 ~ 909</param>
    /// <param name="str">wzms_ko-KR_h00001/WorkZone01/bt77/er00^00^00/tp26.20^ox20.30^ps1018</param>
    /// <returns></returns>
    CString PacketCoder::EncodingPacketString(CString mty, CString str);

    CString PacketCoder::GetEncodingCodePacketChar(char** ary, size_t sRow, size_t sCol, CString pvalue);
#pragma endregion //Packet ��/��ȣȭ

#pragma region Packet �� / ��ȣȭ 10
    /// <summary>
    /// Packet ��ȣȭ. string denc = bizEncrypt.DecodingPacketString10("110", str);
    /// </summary>
    /// <param name="mty">110 ~ 119</param>
    /// <param name="str">472337534367287571~</param>
    /// <returns></returns>
    CString PacketCoder::DecodingPacketString10(CString mty, CString str);

    CString PacketCoder::GetDencodingCodePacketChar10(char** ary, size_t sRow, size_t sCol, CString pvalue);

    /// <summary>
    /// Packet ��ȣȭ. CString enc = bizEncrypt.EncodingString10("110", str);
    /// </summary>
    /// <param name="mty">110 ~ 119</param>
    /// <param name="str">wzms_ko-KR_h00001/WorkZone01/bt77/er00^00^00/tp26.20^ox20.30^ps1018</param>
    /// <returns></returns>
    CString PacketCoder::EncodingPacketString10(CString mty, CString str);

    CString PacketCoder::GetEncodingCodePacketChar10(char** ary, size_t sRow, size_t sCol, CString pvalue);
#pragma endregion //Packet ��/��ȣȭ

    CString Encode(CString str, int mty);
    CString Decode(CString str);
};

/*
using System;
using System.Text.RegularExpressions;

public class Program
{

}

*/