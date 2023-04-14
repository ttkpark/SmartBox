#include "pch.h"
#include "stdafx.h"

void DataList::initContents(int _NumofContents) {
	focus = 0;
	contents = _NumofContents;
	data = (void**)malloc(_NumofContents * sizeof(void*));
	Size = (int*)malloc(_NumofContents * sizeof(int));
	memset(data, 0, _NumofContents * sizeof(void*));
	memset(Size, 0, _NumofContents * sizeof(int));
}
void DataList::initData(int ContentID, void* _data, int dSize) {
	if (ContentID > contents)return;
	data[ContentID] = malloc(dSize);
	memcpy(data[ContentID], _data, dSize);
	Size[ContentID] = dSize;
}
void DataList::DeleteData(int ContentID) {
	if (data[ContentID] == 0)return; ;
	free(data[ContentID]);
}
void* DataList::WhereisData(int ContentID, int& _len) {
	if (_len != 0)_len = Size[ContentID];
	return data[ContentID];
}
int DataList::AlldataSize() {
	int sum = 0;
	for (int i = 0; i < contents; i++)
		sum += Size[i];
	return sum;
}
void DataList::DeleteContents() {
	for (int i = 0; i < contents; i++) {
		DeleteData(i);
	}
	if (data != 0)free(data);
	if (Size != 0)free(Size);
}
int DataList::getfocusID() {
	return focus;
}
int DataList::setfocusID(int _focus) {
	return focus = _focus;
}
int DataList::getendID() {
	for (int i = 0; i < contents; i++) {
		if (Size[i] == 0) {
			if (i == 0)return -1;
			else return i - 1;
		}
	}
	return -1;
}
DataList::~DataList()
{
	DeleteContents();
}

wchar_t COMPortTestStrW[20];
char COMPortTestStr[20];
wchar_t* COMPort::GetPortNameW() {
	if (!this)return _T("");
	wsprintfW(COMPortTestStrW, L"COM%d", number);
	return COMPortTestStrW;
}
char* COMPort::GetPortName() {
	if (!this)return "";
	sprintf_s(COMPortTestStr, "COM%d", number);
	return COMPortTestStr;
}

//CString 타입에 해당하는 해시 함수가 없으므로 다음과 같이 정의

template <> UINT AFXAPI HashKey<CString>(CString str)
{
	LPCTSTR key = (LPCTSTR)str;
	UINT nHash = 0;
	while (*key)
		nHash = (nHash << 5) + nHash + *key++;
	return nHash;
}