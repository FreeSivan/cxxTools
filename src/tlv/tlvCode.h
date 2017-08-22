#ifndef _TLVCODE_H_
#define _TLVCODE_H_
#include "tlvValue.h"
#include "tlvList.h"
#include "tlvPatch.h"
#include "tlvTool.h"
#include <map>
#include <assert.h>
using namespace std;

struct MetaValue { int m_int; short m_short; char m_char; };
/************************************************************************
	$ TlvCode
************************************************************************/
template <typename TL> class TlvCode
{
public:
	void Decode(void* pbuffer,int bufferlen);
	void Encode(void* pbuffer,int maxlength, int& currentlen);
public:
	template <unsigned short tag> typename TagType<tag>::type& Set();
	template <unsigned short tag> const typename TagType<tag>::type& Get();
public:
	TlvTool m_tlvCode;
	ValuePool<MetaValue,(int)TListLen<TL>::value> m_value;
};
/************************************************************************
	$ Set
************************************************************************/
template <typename TL> template <unsigned short tag>
inline typename TagType<tag>::type& TlvCode<TL>::Set()
{
	compare_assert<(unsigned int)(-1) != (unsigned int)TListIdx<TL,tag>::value>();
	return TypeDispatch<tag, (int)TListIdx<TL,tag>::value, typename TagType<tag>::type>::set(this);
}
/************************************************************************
	$ Get
************************************************************************/
template <typename TL> template <unsigned short tag>
inline const typename TagType<tag>::type& TlvCode<TL>::Get()
{
	compare_assert<(unsigned int)(-1) != (unsigned int)TListIdx<TL,tag>::value>();
	return TypeDispatch<tag, (int)TListIdx<TL,tag>::value, typename TagType<tag>::type>::get(this);
}
/************************************************************************
	$ TypeDispatch
************************************************************************/
template <typename TL> 
inline void TlvCode<TL>::Decode(void* pbuffer,int bufferlen)
{
	assert(pbuffer && bufferlen > 0);
	m_value.ResetResult();
	m_value.ResetJudge();
	m_tlvCode.decode(pbuffer, bufferlen);
	Drived<TL, (unsigned int)TListLen<TL>::value>
		::DecodeDrived(pbuffer, bufferlen, this);
}
/************************************************************************
	$ TypeDispatch
************************************************************************/
template <typename TL> 
inline void TlvCode<TL>::Encode(void* pbuffer,int maxlength, int& currentlen)
{
	assert(pbuffer && maxlength > 0);
	Drived<TL, (unsigned int)TListLen<TL>::value>
		::EncodeDrived(pbuffer, maxlength, currentlen, this);
}
#endif

