/***********************************************************************
	$ 名称：decode
	$ 参数：pbuffer
	$ 功能：
	$ 返回：
***********************************************************************/
inline bool TlvTool::decode(void *pbuffer, int bufferlen)
{
	if(0 == pbuffer || 0 > bufferlen) return false;
	m_tagpos.clear();
	unsigned char* ptlvBegin = (unsigned char*)pbuffer;
	TLV* pTlv = (TLV*)ptlvBegin;
	while (ptlvBegin + 2 * sizeof(unsigned short) + NTOH(pTlv->m_len) <= (unsigned char*)pbuffer + bufferlen)
	{
		m_tagpos.insert(make_pair(NTOH(pTlv->m_tag), (int)(ptlvBegin - (unsigned char*)(pbuffer))));
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
template <typename _Type> _Type TlvTool::NTOH(_Type I_val)
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
inline bool TlvTool::GetTag(void* pbuffer,int bufferlen, unsigned short tag, T& value)
{
	if(0 == pbuffer || 0 > bufferlen) return false;
	unsigned char* ptlvBegin = (unsigned char*)pbuffer;
	map<unsigned short, int>::iterator it = m_tagpos.find(tag);
	if(m_tagpos.end() == it) return false;
	ptlvBegin += it->second;
	TLV* pTlv = (TLV*)ptlvBegin;
	if(sizeof(T) != NTOH(pTlv->m_len)) return false;
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
inline bool TlvTool::AddTag(void* pbuffer, int maxlength, int& currentlen, unsigned short tag, const T& value)
{
	if (0 == pbuffer || 0 > maxlength || currentlen > maxlength) return false;
	if (currentlen + 2 * sizeof(unsigned short) + sizeof(T) > (unsigned int)maxlength) return false;
	TLV* pTlv = (TLV*)((unsigned char*)pbuffer+currentlen);
	pTlv->m_tag = NTOH(tag);
	pTlv->m_len = (unsigned short)NTOH((unsigned short)sizeof(T));
	*(T*)pTlv->m_pval = NTOH(value);
	currentlen += (2*sizeof(unsigned short)+sizeof(T));
	return true;
}
