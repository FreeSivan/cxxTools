#ifndef _TLVTOOLS_H_
#define _TLVTOOLS_H_
#include "tlvBitarr.h"
template <typename T,int ValLen>
class ValuePool
{
public:
	ValuePool();
	~ValuePool();
public:
	template <int pos> T& Set();
	T& Set(int I_pos);
	template <int pos> const T& Get()const;
	const T& Get(int I_pos)const;
public:
	bool AddResult(int pos);
	bool DelResult(int pos);
	bool GetResult(int pos);
	template <int pos> bool AddResult();
	template <int pos> bool DelResult();
	template <int pos> bool GetResult();
	void ResetResult();
public:
	bool AddJudge(int pos);
	bool DelJudge(int pos);
	bool GetJudge(int pos);
	template <int pos> bool AddJudge();
	template <int pos> bool DelJudge();
	template <int pos> bool GetJudge();
	void ResetJudge();
private:
	T m_Value[ValLen];
	BitArr<ValLen> m_ResultBit;
	BitArr<ValLen> m_JudgeBit;
private:
	ValuePool(const ValuePool<T,ValLen>&);
	ValuePool<T,ValLen>& operator=(const ValuePool<T,ValLen>&);
};


template <typename T,int ValLen>
inline ValuePool<T,ValLen>::ValuePool()
{
}

template <typename T,int ValLen>
inline ValuePool<T,ValLen>::~ValuePool()
{
}

template <typename T,int ValLen> template <int pos>
inline T& ValuePool<T,ValLen>::Set()
{
	m_JudgeBit.template AddBit<pos>();
	return m_Value[pos];
}

template <typename T,int ValLen> template <int pos>
inline const T& ValuePool<T,ValLen>::Get()const
{
	return m_Value[pos];
}

template <typename T,int ValLen>
inline T& ValuePool<T,ValLen>::Set(int I_pos)
{
	m_JudgeBit.addbit(I_pos);
	return m_Value[I_pos];
}

template <typename T,int ValLen>
inline const T& ValuePool<T,ValLen>::Get(int I_pos)const
{
	return m_Value[I_pos];
}

/****************************************************
	$ ResultBit
****************************************************/
template <typename T,int ValLen> template <int pos>
inline bool ValuePool<T,ValLen>::AddResult()
{
	return m_ResultBit.template AddBit<pos>();
}

template <typename T,int ValLen> template <int pos>
inline bool ValuePool<T,ValLen>::DelResult()
{
	return m_ResultBit.template DelBit<pos>();
}

template <typename T,int ValLen> template <int pos>
inline bool ValuePool<T,ValLen>::GetResult()
{
	return m_ResultBit.template GetBit<pos>();
}

template <typename T,int ValLen>
inline bool ValuePool<T,ValLen>::AddResult(int I_pos)
{
	return m_ResultBit.addbit(I_pos);
}

template <typename T, int ValLen>
inline bool ValuePool<T,ValLen>::DelResult(int I_pos)
{
	return m_ResultBit.delbit(I_pos);
}

template <typename T, int ValLen>
inline bool ValuePool<T,ValLen>::GetResult(int I_pos)
{
	return m_ResultBit.getbit(I_pos);
}

template <typename T, int ValLen>
inline void ValuePool<T,ValLen>::ResetResult()
{
	m_ResultBit.reset();
}
/****************************************************
	$ JudgeBit
****************************************************/
template <typename T, int ValLen> template <int pos> 
inline bool ValuePool<T,ValLen>::AddJudge()
{
	return m_JudgeBit.template AddBit<pos>();
}

template <typename T, int ValLen> template <int pos> 
inline bool ValuePool<T,ValLen>::DelJudge()
{
	return m_JudgeBit.template DelBit<pos>();
}

template <typename T, int ValLen> template <int pos> 
inline bool ValuePool<T,ValLen>::GetJudge()
{
	return m_JudgeBit.template GetBit<pos>();
}

template <typename T, int ValLen>
inline bool ValuePool<T,ValLen>::AddJudge(int I_pos)
{
	return m_JudgeBit.addbit(I_pos);
}

template <typename T, int ValLen>
inline bool ValuePool<T,ValLen>::DelJudge(int I_pos)
{
	return m_JudgeBit.delbit(I_pos);
}

template <typename T, int ValLen>
inline bool ValuePool<T,ValLen>::GetJudge(int I_pos)
{
	return m_JudgeBit.getbit(I_pos);
}

template <typename T, int ValLen>
inline void ValuePool<T,ValLen>::ResetJudge()
{
	m_JudgeBit.reset();
}

#endif
