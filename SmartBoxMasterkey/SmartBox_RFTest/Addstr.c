#include "Addstr.h"

#ifdef __cplusplus
extern "C"
{
#endif
	char* Addstr;
	int Addstr_len;
	int Addstr_init(Addstr_Str* str) {
		//Debug("Addstr_Init() start");
		Addstr_destroy(str);
		str->str = (char*)malloc(2);
		//if(!str->str)Debug("14:malloc Error!! Mem leak.");
		str->size = 2;
		memset(str->str, 0, str->size);
		//Debug("str           = %d", str);
		//Debug("str->str      = %d", str->str);
		//Debug("str->size     = %d", str->size);
		//Debug("strlen        = %d", strlen(str->str));
	}
	void Addstr_add(Addstr_Str* str, char* addstr) {
		Addstr_addlen(str, addstr, strlen(addstr));
	}
	void Addstr_addlen(Addstr_Str* str, char* addstr, int addlen) {
		//Debug("Addstr_add() start");
		//Debug("addstr        = %s", addstr);
		//Debug("str           = %d", str);
		//Debug("str->str      = %d", str->str);
		//Debug("str->size     = %d", str->size);
		int s_len = strlen(str->str), pre_len=0;
		//Debug("Addstr_add() 1");
		//Debug("str           = %d = {len:%d,size:%d}", (int)str, strlen(str->str), str->size);
		//Debug("len(addstr)   = %d", strlen(addstr));
		//Debug("addlen        = %d", addlen);
		while ((s_len + addlen + 1) > str->size) {
			pre_len = str->size;
			str->size *= 2;
			//Debug("Addstr_add() 2");
			char* tmp = (char*)malloc(str->size);
			//if (!tmp)Debug("31:malloc Error!! Mem leak.");
			memcpy(tmp, str->str, pre_len);
			memset(tmp + pre_len, 0, pre_len);
			free(str->str);
			str->str = tmp;
			//Debug("Addstr_add() 3");
			//Debug("str           = %d = {len:%d,size:%d}", (int)str, strlen(str->str),str->size);
		}
		memcpy(str->str+s_len, addstr, addlen + 1);
		//Debug("523:Addstr(%d)", strlen(addstr));
		//Debug("str           = %d = {len:%d,size:%d}", (int)str, strlen(str->str),str->size);
		//Debug("len(addstr)   = %d", strlen(addstr));
		//Debug("addlen        = %d", addlen);
	}
	void Addstr_destroy(Addstr_Str* str) {
		if (str->size != 0)
		{
			free(str->str);
			str->str = 0;
			str->size = 0;
		}
	}
	char* Addstr_return(Addstr_Str* str) {
		return str->str;
	}


#ifdef __cplusplus
}
#endif