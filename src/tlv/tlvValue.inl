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
