#ifndef _DISPATCH_H_
#define _DISPATCH_H_
#include "tlvDef.h"
#include "tlvValue.h"
#include "tlvList.h"
/***********************************************************************************
	$ written by xiwen yue : clown 
	$ TagNum
***********************************************************************************/
template <typename T> 
struct TagNum;

#define Tag_Num(_type, _num)                                                      \
template <> struct TagNum<_type>                                                  \
{                                                                                 \
	static const unsigned int value = _num;                                       \
}
Tag_Num(int, TYPE_NUM_INT);
Tag_Num(char, TYPE_NUM_CHAR);
Tag_Num(short, TYPE_NUM_SHORT);
/***********************************************************************************
	$ written by xiwen yue : clown 
	$ TypeDispatch
***********************************************************************************/
template <unsigned short N> 
struct TagType;

#define Tag_Type(_tag, _type)                                                     \
template <> struct TagType<_tag>                                                  \
{                                                                                 \
	typedef _type type;                                                           \
}
Tag_Type(TAG_NUM_ONE,    char);
Tag_Type(TAG_NUM_TWO,    char);
Tag_Type(TAG_NUM_THREE,  char);
Tag_Type(TAG_NUM_FOUR,   int);
Tag_Type(TAG_NUM_FIVE,   int);
Tag_Type(TAG_NUM_SIX,    int);
Tag_Type(TAG_NUM_SEVEN,  short);
Tag_Type(TAG_NUM_EIGHT,  short);
Tag_Type(TAG_NUM_NINE,   short);
Tag_Type(TAG_NUM_TEN,    short);

/***********************************************************************************
	$ TypeDispatch
***********************************************************************************/
template <unsigned short val, 
          int pos, 
          typename T> 
struct TypeDispatch;

#define Tag_Dispatch(_type, _part)                                                \
template <unsigned short val, int pos>                                            \
struct TypeDispatch<val, pos, _type>                                              \
{                                                                                 \
	template <typename pTlvCode>                                                  \
	static _type& set(pTlvCode IO_ptr)                                            \
	{                                                                             \
		return IO_ptr->m_value.template Set<pos>()._part;                         \
	}                                                                             \
	template <typename pTlvCode>                                                  \
	static const _type& get(pTlvCode IO_ptr)                                      \
	{                                                                             \
		return IO_ptr->m_value.template Get<pos>()._part;                         \
	}                                                                             \
}
Tag_Dispatch(int, m_int);
Tag_Dispatch(char, m_char);
Tag_Dispatch(short, m_short);
/***********************************************************************************
	$ EncodeDispatch
***********************************************************************************/
template <unsigned short tag, 
          unsigned int tagPos, 
          unsigned int tagType> 
struct EncodeDispatch;

#define Encode_Dispatch(_tag_type_num, _m_part)                                   \
template <unsigned short tag, unsigned int tagPos>                                \
struct EncodeDispatch<tag, tagPos, _tag_type_num>                                 \
{                                                                                 \
	template <typename pTlvCode>                                                  \
	static bool proc(unsigned char* buffer,                                       \
	                 int maxlength,                                               \
	                 int& currentlen,                                             \
	                 pTlvCode ptr)                                                \
	{                                                                             \
		if (ptr->m_tlvCode.AddTag((void*)buffer,                                  \
			                      maxlength,                                      \
			                      currentlen,                                     \
			                      tag,                                            \
							      ptr->m_value.template Get<tagPos>()._m_part))   \
		{                                                                         \
			ptr->m_value.template AddResult<tag>();                               \
			return true;                                                          \
		}                                                                         \
		return false;                                                             \
	}                                                                             \
}

Encode_Dispatch(TYPE_NUM_INT, m_int);
Encode_Dispatch(TYPE_NUM_CHAR, m_char);
Encode_Dispatch(TYPE_NUM_SHORT, m_short);
/***********************************************************************************
	$ DecodeDispatch
***********************************************************************************/
template <unsigned short tag, 
          unsigned int tagPos, 
          unsigned int tagType> 
struct DecodeDispatch;

#define Decode_Dispatch(_tag_type_num, _m_part)                                   \
template <unsigned short tag, unsigned int tagPos>                                \
struct DecodeDispatch <tag, tagPos, _tag_type_num>                                \
{                                                                                 \
	template <typename pTlvCode>                                                  \
	static bool proc(unsigned char* buffer, int maxlength, pTlvCode ptr)          \
	{                                                                             \
		if (ptr->m_tlvCode.GetTag((void*)buffer,                                  \
			                      maxlength, tag,                                 \
					              ptr->m_value.template Set<tagPos>()._m_part))   \
		{                                                                         \
			ptr->m_value.template AddResult<tag>();                               \
			return true;                                                          \
		}                                                                         \
		return false;                                                             \
	}                                                                             \
}

Decode_Dispatch(TYPE_NUM_INT, m_int);
Decode_Dispatch(TYPE_NUM_CHAR, m_char);
Decode_Dispatch(TYPE_NUM_SHORT, m_short);
/***********************************************************************************
	$ Excute
***********************************************************************************/
template <typename TL, unsigned short tag> struct Execute
{
	template <typename pTlvCode>
	static void Decode(void* pbuffer,
	                   int bufferlen, 
	                   pTlvCode ptr)
	{
		const int tagPos = (int)TListIdx<TL, tag>::value;
		const int typeNum= (int)TagNum<typename TagType<tag>::type>::value;
		
		DecodeDispatch<tag, tagPos, typeNum>
			::proc((unsigned char*)pbuffer, bufferlen, ptr);
	}

	template <typename pTlvCode>
	static void Encode(void* pbuffer,
	                   int maxlength,
	                   int& currentlen, 
	                   pTlvCode ptr)
	{
		const int tagPos = (int)TListIdx<TL, tag>::value;
		const int typeNum = (int)TagNum<typename TagType<tag>::type>::value;
		
		if (!ptr->m_value.template GetJudge<tagPos>())
		{
			return ;
		}
		
		EncodeDispatch<tag, tagPos, typeNum>
			::proc((unsigned char*)pbuffer, maxlength, currentlen, ptr);
	}
};
/***********************************************************************************
	$ Drived
***********************************************************************************/
template <typename TL, unsigned int Len> struct Drived
{

	template <typename pTlvCode>
	static void DecodeDrived(void* pbuffer, 
	                         int bufferlen, 
	                         pTlvCode ptr)
	{
		Execute<TL, (unsigned short)TListAt<TL, Len - 1>::value>
			::Decode(pbuffer, bufferlen, ptr);
		
		Drived<TL,Len - 1>
			::DecodeDrived(pbuffer, bufferlen, ptr);
	}
	
	template <typename pTlvCode>
	static void EncodeDrived(void* pbuffer, 
	                         int maxlength, 
	                         int& currentlen, 
	                         pTlvCode ptr)
	{
		Execute<TL, (unsigned short)TListAt<TL, Len - 1>::value>
			::Encode(pbuffer, maxlength, currentlen, ptr);
		
		Drived<TL,Len - 1>
			::EncodeDrived(pbuffer, maxlength, currentlen, ptr);
	}
	
};
template <typename TL> struct Drived<TL, 1>
{

	template <typename pTlvCode>
	static void DecodeDrived(void* pbuffer, 
	                         int bufferlen, 
	                         pTlvCode ptr)
	{
		Execute<TL, (unsigned short)TListAt<TL, 0>::value>
			::Decode(pbuffer, bufferlen, ptr);
	}
	
	template <typename pTlvCode>
	static void EncodeDrived(void* pbuffer,
	                         int maxlength,
	                         int& currentlen, 
	                         pTlvCode ptr)
	{
		Execute<TL, (unsigned short)TListAt<TL, 0>::value>
			::Encode(pbuffer, maxlength, currentlen, ptr);
	}
	
};
#endif


