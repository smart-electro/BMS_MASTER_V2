#define J1939_MSG_LENGTH	5
#define J1939_DATA_LENGTH	8

union J1939_SID_EID_UNION
{
	unsigned long SE_ID;
	unsigned char Bytes[4];
	struct 
    {
		unsigned char	SourceAddress;
		unsigned char	PDUSpecific;
		unsigned char	PDUFormat;					// CA should use only PDUFormat.
		unsigned char	DataPage			: 1;
		unsigned char	Res					: 1;
		unsigned char	Priority			: 3;
		unsigned char	Dummy       		: 3;	// Samo zasedanje gornjih 3-bit-ov
	};
};
typedef union J1939_SID_EID_UNION J1939_SE_ID;

union J1939_LENGTH_UNION
{
    unsigned char Len;
    struct 
    {
   		unsigned char	DataLength 			: 4;
		unsigned char	RTR					: 4;	// RTR bit, value always 0x00
    };
};
typedef union J1939_LENGTH_UNION J1939_LENGTH;

union J1939_MESSAGE_UNION 
{
	struct 
    {
        J1939_SE_ID     Address;
		J1939_LENGTH    Length;
		unsigned char   Data[J1939_DATA_LENGTH];
	};
	unsigned char Array[J1939_MSG_LENGTH + J1939_DATA_LENGTH];
};
typedef union J1939_MESSAGE_UNION J1939_MESSAGE;

