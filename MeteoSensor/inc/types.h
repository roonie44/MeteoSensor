#ifndef _TYPES_H_
#define _TYPES_H_

#define  ASCII(x)	                              (((x) < 10) ? ((x) + 0x30) : ((x) + 0x37))
#define  HEX(x)                                 ((x) - 0x30)

enum Status
{
   OK       = 0,
   BUSY     = 1,
   TIMEOUT  = 2,
   FAILURE  = 3,
};

typedef union
{
   unsigned char     uChar;
   struct
   {
      unsigned char  uNibbleL  : 4;
      unsigned char  uNibbleH  : 4;
   };
}TUCHAR;

typedef union
{
   unsigned short    uShort;
   struct
   {
      unsigned char  uCharL;
      unsigned char  uCharH;
   };
   struct
   {
      unsigned short uNibbleLL  : 4;
      unsigned short uNibbleLH  : 4;
      unsigned short uNibbleHL  : 4;
      unsigned short uNibbleHH  : 4;
   };
}TUSHORT;

typedef union
{
	unsigned int 		uInt;
	struct
	{
		unsigned short uShortL;
		unsigned short uShortH;
	};
	struct
	{
		unsigned char 	uCharLL;
		unsigned char 	uCharLH;
		unsigned char 	uCharHL;
		unsigned char 	uCharHH;
	};
	struct
	{
		unsigned int 	uNibbleLLL	: 4;
		unsigned int 	uNibbleLLH	: 4;
		unsigned int 	uNibbleLHL	: 4;
		unsigned int 	uNibbleLHH	: 4;
		unsigned int 	uNibbleHLL	: 4;
		unsigned int 	uNibbleHLH	: 4;
		unsigned int 	uNibbleHHL	: 4;
		unsigned int 	uNibbleHHH	: 4;
	};
}TUINT;



#endif
