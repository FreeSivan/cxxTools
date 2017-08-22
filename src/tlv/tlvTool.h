#ifndef _TLVTOOL_H_Y00183482_
#define _TLVTOOL_H_Y00183482_
#include <map>
using namespace std;
struct TLV
{
	unsigned short m_tag;
	unsigned short m_len;
	char m_pval[1];
	TLV():m_tag(0),m_len(0){}
};

class TlvTool 
{
public:
	TlvTool(){};
	~TlvTool(){};
public:
	bool decode(void* pbuffer, 
		        int bufferlen);
	
	template <typename _Type> 
	_Type NTOH(_Type I_val);
public:
	template <typename T> 
	bool GetTag(void* pbuffer, 
	            int bufferlen, 
		        unsigned short tag, 
		        T& value);

	
	template <typename T> 
	bool AddTag(void* pbuffer, 
	            int maxlength, 
	            int& currentlen, 
	            unsigned short tag, 
	            const T& value);
private:
	TlvTool(const TlvTool&);
	TlvTool& operator=(const TlvTool&);
private:
	map<unsigned short, int> m_tagpos;
};


template <bool judge> struct compare_assert;
template <> struct compare_assert<true> {};


/***********************************************************************
	$ 名称：decode
	$ 参数：pbuffer
	$ 功能：
	$ 返回：
***********************************************************************/
inline bool TlvTool::decode(void *pbuffer, int bufferlen)
{
	if(0 == pbuffer || 0 > bufferlen) 
	{
		return false;
	}
	m_tagpos.clear();
	
	unsigned char* ptlvBegin = (unsigned char*)pbuffer;
	TLV* pTlv = (TLV*)ptlvBegin;
	
	while (ptlvBegin + 2 * sizeof(unsigned short) + NTOH(pTlv->m_len) 
				<= (unsigned char*)pbuffer + bufferlen)
	{
		int offset = (int)(ptlvBegin - (unsigned char*)(pbuffer));
		m_tagpos.insert(make_pair(NTOH(pTlv->m_tag), offset));
		ptlvBegin += (2*sizeof(unsigned short)+NTOH(pTlv->m_len));
		pTlv = (TLV*)ptlvBegin;
	}
	
	return true;
}
/***********************************************************************
	$ 名称：NTOH
	$ 参数：I_val
	$ 功能：
	$ 返回：
***********************************************************************/
template <typename _Type> 
_Type TlvTool::NTOH(_Type I_val)
{
	int size = sizeof(_Type);
	char* ptr = (char*)&I_val;
	for (int i = 0; i < size/2; ++i)
	{
		char tmp = ptr[i];
		ptr[i] = ptr[size - i - 1];
		ptr[size - i - 1] = tmp;
	}
	return *(_Type*)ptr;
}
/***********************************************************************
	$ 名称：decode
	$ 参数：pbuffer
	$ 功能：
	$ 返回：
***********************************************************************/
template <typename T> 
inline bool TlvTool::GetTag(void* pbuffer,
                            int bufferlen, 
                            unsigned short tag, 
                            T& value)
{
	if(0 == pbuffer || 0 > bufferlen) 
	{
		return false;
	}
	
	unsigned char* ptlvBegin = (unsigned char*)pbuffer;
	map<unsigned short, int>::iterator it = m_tagpos.find(tag);

	if(m_tagpos.end() == it) 
	{
		return false;
	}
	ptlvBegin += it->second;
	TLV* pTlv = (TLV*)ptlvBegin;
	
	if(sizeof(T) != NTOH(pTlv->m_len)) 
	{
		return false;
	}
	value = NTOH(*(T *)pTlv->m_pval);
	return true;
}
/***********************************************************************
	$ 名称：decode
	$ 参数：pbuffer
	$ 功能：
	$ 返回：
***********************************************************************/
template <typename T> 
inline bool TlvTool::AddTag(void* pbuffer, 
                            int maxlength, 
                            int& currentlen, 
                            unsigned short tag, 
                            const T& value)
{
	if (0 == pbuffer || 0 > maxlength || currentlen > maxlength) 
	{
		return false;
	}
	
	if (currentlen + 2 * sizeof(unsigned short) + sizeof(T) 
		> (unsigned int)maxlength) 
	{
		return false;
	}
	
	TLV* pTlv = (TLV*)((unsigned char*)pbuffer+currentlen);
	pTlv->m_tag = NTOH(tag);
	pTlv->m_len = (unsigned short)NTOH((unsigned short)sizeof(T));
	*(T*)pTlv->m_pval = NTOH(value);
	currentlen += (2*sizeof(unsigned short)+sizeof(T));
	return true;
}
#endif 

