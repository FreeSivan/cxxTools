#ifndef _TAGLIST_H_
#define _TAGLIST_H_
struct VOID;
/************************************************************************
	$ Drived
************************************************************************/
template <unsigned short tag, typename type>
struct Taglist
{
	static const unsigned short value 
		= tag;
	
	typedef type tail;
};
template <unsigned short tag>
struct Taglist <tag, VOID>
{
	static const unsigned short value 
		= tag;
};
/************************************************************************
	$ Drived
************************************************************************/
template <typename tlist> struct TListLen;
template <unsigned short Head, typename Tail>
struct TListLen <Taglist<Head, Tail> >
{
	static const unsigned int value 
		= (unsigned short)TListLen<Tail>::value + 1;
};
template <> struct TListLen <VOID>
{
	static const unsigned int value  
		= 0; 
};
/************************************************************************
	$ Drived
************************************************************************/
template <typename TL, int N> struct TListAt;
template <int N> struct TListAt<VOID, N>
{
	typedef VOID type;
};
template <unsigned short Head, typename Tail>
struct TListAt<Taglist<Head, Tail>, 0>
{
	static const unsigned short value 
		=  Head;
};
template <unsigned short Head, typename Tail, int N>
struct TListAt<Taglist<Head, Tail>, N>
{
	static const unsigned short value 
		=  (unsigned short)TListAt<Tail, N-1>::value;
};
/************************************************************************
	$ Drived
************************************************************************/
template <typename tlist, unsigned short tag> struct TListIdx;
template <unsigned short tag>
struct TListIdx <VOID, tag>
{
	static const unsigned int value 
		= (unsigned int) -1;
}; 
template <typename Tail, unsigned short tag>
struct TListIdx <Taglist<tag, Tail> ,tag>
{	
	static const unsigned int value 
		= 0;
};
template <unsigned short Head, typename Tail, unsigned short tag>
struct TListIdx <Taglist<Head, Tail>, tag>
{
private:
	static const unsigned int tmp 
		= (unsigned int) TListIdx<Tail,tag>::value;
public:
	static const unsigned int value
		= ((unsigned int)-1 == tmp ? (unsigned int)-1:tmp+1); 
};
/************************************************************************
	$ Drived
************************************************************************/
#define TAG_LIST1(V1) Taglist<V1,VOID>
#define TAG_LIST2(V1,V2) Taglist<V1,TAG_LIST1(V2)>
#define TAG_LIST3(V1,V2,V3) Taglist<V1,TAG_LIST2(V2,V3)>
#define TAG_LIST4(V1,V2,V3,V4) Taglist<V1,TAG_LIST3(V2,V3,V4)>
#define TAG_LIST5(V1,V2,V3,V4,V5) Taglist<V1,TAG_LIST4(V2,V3,V4,V5)>
#define TAG_LIST6(V1,V2,V3,V4,V5,V6) Taglist<V1,TAG_LIST5(V2,V3,V4,V5,V6)>
#define TAG_LIST7(V1,V2,V3,V4,V5,V6,V7) Taglist<V1,TAG_LIST6(V2,V3,V4,V5,V6,V7)>
#define TAG_LIST8(V1,V2,V3,V4,V5,V6,V7,V8) Taglist<V1,TAG_LIST7(V2,V3,V4,V5,V6,V7,V8)>
#define TAG_LIST9(V1,V2,V3,V4,V5,V6,V7,V8,V9) Taglist<V1,TAG_LIST8(V2,V3,V4,V5,V6,V7,V8,V9)>
#define TAG_LIST10(V1,V2,V3,V4,V5,V6,V7,V8,V9,V10) Taglist<V1,TAG_LIST9(V2,V3,V4,V5,V6,V7,V8,V9,V10)>

typedef TAG_LIST10(\
		   TAG_NUM_ONE,\
		   TAG_NUM_TWO,\
		   TAG_NUM_THREE,\
		   TAG_NUM_FOUR,\
		   TAG_NUM_FIVE,\
		   TAG_NUM_SIX,\
		   TAG_NUM_SEVEN,\
		   TAG_NUM_EIGHT,\
		   TAG_NUM_NINE,\
		   TAG_NUM_TEN\
		   ) testType;
#endif

