/***********************************************************************
	$ 名称：addbit
	$ 参数：I_pos为在数组中的位置
	$ 返回：若映射的下标不在[0,m_maxlengh)内，返回0
	                将映射下标处的bit位置为1
***********************************************************************/
inline bool policy::addbit(char* O_arr,int I_maxlength,int I_pos)
{
	if(0>I_pos || I_pos>=I_maxlength)
	{
		return false;
	}
	O_arr[math_c2b_len(I_pos)] |= math_c2b_pow(math_c2b_pos(I_pos));	
	return true;
}
/***********************************************************************
	$ 名称：delbit
	$ 参数：
	$ 返回：若映射的下标不在[0,m_maxlengh)内，返回false
	                将映射下表位的bit置为0
***********************************************************************/
inline bool policy::delbit(char* O_arr,int I_maxlength,int I_pos)
{
	if(0>I_pos || I_pos>=I_maxlength)
	{
		return false;
	}
	O_arr[math_c2b_len(I_pos)] &= ~math_c2b_pow(math_c2b_pos(I_pos));
	return true;
}
/***********************************************************************
	$ 名称：getbit
	$ 参数：
	$ 返回：若映射的下标不在[0,m_maxlengh)内，返回0
	                如果映射下标bit为1，返回true，否在返回false
***********************************************************************/
inline bool policy::getbit(char* O_arr,int I_maxlength,int I_pos)
{
	if(0>I_pos || I_pos>=I_maxlength)
	{
		return false;
	}
	return 0 != (O_arr[math_c2b_len(I_pos)]&math_c2b_pow(math_c2b_pos(I_pos)));
}
/***********************************************************************
	$ 名称：AddBit
	$ 参数：POS为在数组中的位置
	$ 返回：若映射的下标不在[0,m_maxlengh)内，返回0
	                将映射下标处的bit位置为1
***********************************************************************/
template <int POS>
inline bool policy::AddBit(char* O_arr,int I_maxlength)
{
	if(0>POS || POS>=I_maxlength)
	{
		return false;
	}
	O_arr[(unsigned int)MATH_C2B_LEN<POS>::value] |= 
              (unsigned int)MATH_C2B_POW<(unsigned int)MATH_C2B_POS<POS>::value>::value;
	return true;
}
/***********************************************************************
	$ 名称：DelBit
	$ 参数：
	$ 返回：若映射的下标不在[0,m_maxlengh)内，返回false
	                将映射下表位的bit置为0
***********************************************************************/
template <int POS>
inline bool policy::DelBit(char* O_arr,int I_maxlength)
{
	if(0>POS || POS>=I_maxlength)
	{
		return false;
	}
	O_arr[(unsigned int)MATH_C2B_LEN<POS>::value] &=
              ~(unsigned int)MATH_C2B_POW<(unsigned int)MATH_C2B_POS<POS>::value>::value;
	return true;
}
/***********************************************************************
	$ 名称：getbit
	$ 参数：
	$ 返回：若映射的下标不在[0,m_maxlengh)内，返回0
	                如果映射下标bit为1，返回true，否在返回false
***********************************************************************/
template <int POS>
inline bool policy::GetBit(char* O_arr,int I_maxlength)
{
	if(0>POS || POS>=I_maxlength)
	{
		return false;
	}
	unsigned int len = (unsigned int)MATH_C2B_LEN<POS>::value;
	unsigned int pow = 
			(unsigned int)MATH_C2B_POW<(unsigned int)MATH_C2B_POS<POS>::value>::value;
	return 0 != (O_arr[len]&pow);
}


/***********************************************************************
	$ 名称：bitarr类
	$
***********************************************************************/
template <typename Policy>
inline bitarr<Policy>::bitarr(int I_length)
{
	m_maxlength = I_length;
	m_arr = new char[math_c2b_len(m_maxlength)+1];
	reset();
}
template <typename Policy>
inline bitarr<Policy>::~bitarr()
{
	delete []m_arr;
}
template <typename Policy>
inline bool bitarr<Policy>::addbit(int I_pos)
{
	return Policy::addbit(m_arr,m_maxlength,I_pos);
}
template <typename Policy>
inline bool bitarr<Policy>::delbit(int I_pos)
{
	return Policy::delbit(m_arr,m_maxlength,I_pos);
}
template <typename Policy>
inline bool bitarr<Policy>::getbit(int I_pos)
{
	return Policy::getbit(m_arr,m_maxlength,I_pos);
}
template <typename Policy> template <int POS>
inline bool bitarr<Policy>::AddBit()
{
	return Policy::template AddBit<POS>(m_arr,m_maxlength);
}
template <typename Policy> template <int POS>
inline bool bitarr<Policy>::DelBit()
{
	return Policy::template DelBit<POS>(m_arr,m_maxlength);
}
template <typename Policy> template <int POS>
inline bool bitarr<Policy>::GetBit()
{
	return Policy::template GetBit<POS>(m_arr,m_maxlength);
}
template <typename Policy>
inline void bitarr<Policy>::reset()
{
	for(int i = 0; i<math_c2b_len(m_maxlength)+1;++i)
	{
		m_arr[i] = 0;
	}
}
template <typename Policy>
char* bitarr<Policy>::GetArr()const
{
	return m_arr;
}
template <typename Policy>
int bitarr<Policy>::GetMaxlength()const
{
	return m_maxlength;
}
template <typename Policy>
int bitarr<Policy>::GetArrlen()const
{
	return math_c2b_len(m_maxlength)+1;
}
/***********************************************************************
	$ 名称：BitArr类
	$
***********************************************************************/
template <int Length,typename Policy>
BitArr<Length,Policy>::BitArr()
{
	m_maxlength = Length;
	reset();
}
template <int Length,typename Policy>
BitArr<Length,Policy>::~BitArr()
{
}

template <int Length,typename Policy>
inline bool BitArr<Length,Policy>::addbit(int I_pos)
{
	return Policy::addbit(m_arr,m_maxlength,I_pos);
}
template <int Length,typename Policy>
inline bool BitArr<Length,Policy>::delbit(int I_pos)
{
	return Policy::delbit(m_arr,m_maxlength,I_pos);
}
template <int Length,typename Policy>
inline bool BitArr<Length,Policy>::getbit(int I_pos)
{
	return Policy::getbit(m_arr,m_maxlength,I_pos);
}
template <int Length,typename Policy> template <int POS>
inline bool BitArr<Length,Policy>::AddBit()
{
	return Policy::template AddBit<POS>(m_arr,m_maxlength);
}
template <int Length,typename Policy> template <int POS>
inline bool BitArr<Length,Policy>::DelBit()
{
	return Policy::template DelBit<POS>(m_arr,m_maxlength);
}
template <int Length,typename Policy> template <int POS>
inline bool BitArr<Length,Policy>::GetBit()
{
	return Policy::template GetBit<POS>(m_arr,m_maxlength);
}
template <int Length,typename Policy>
inline void BitArr<Length,Policy>::reset()
{
	for(int i = 0; i<(int)MATH_C2B_LEN<(unsigned int)Length>::value+1; ++i)
	{
		m_arr[i] = 0;
	}
}
template <int Length,typename Policy>
char* BitArr<Length,Policy>::GetArr()const
{
	return (char*)m_arr;
}
template <int Length,typename Policy>
int BitArr<Length,Policy>::GetMaxlength()const
{
	return m_maxlength;
}
template <int Length,typename Policy>
int BitArr<Length,Policy>::GetArrlen()const
{
	return math_c2b_len(m_maxlength)+1;
}
