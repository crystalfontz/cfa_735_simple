//============================================================================
// Copyright 2005, Crystalfontz America, Inc. Written by Brent A. Crosby
// www.crystalfontz.com, brent@crystalfontz.com
//============================================================================
#include <stdio.h>
#include <unistd.h>
#include "typedefs.h"
#include "serial.h"
#include "cf_packet.h"
//------------------------------------------------------------------------------
word get_crc(ubyte *bufptr,word len,word seed)
  {
  //CRC lookup table to avoid bit-shifting loops.
  static const word crcLookupTable[256] =
    {0x00000,0x01189,0x02312,0x0329B,0x04624,0x057AD,0x06536,0x074BF,
     0x08C48,0x09DC1,0x0AF5A,0x0BED3,0x0CA6C,0x0DBE5,0x0E97E,0x0F8F7,
     0x01081,0x00108,0x03393,0x0221A,0x056A5,0x0472C,0x075B7,0x0643E,
     0x09CC9,0x08D40,0x0BFDB,0x0AE52,0x0DAED,0x0CB64,0x0F9FF,0x0E876,
     0x02102,0x0308B,0x00210,0x01399,0x06726,0x076AF,0x04434,0x055BD,
     0x0AD4A,0x0BCC3,0x08E58,0x09FD1,0x0EB6E,0x0FAE7,0x0C87C,0x0D9F5,
     0x03183,0x0200A,0x01291,0x00318,0x077A7,0x0662E,0x054B5,0x0453C,
     0x0BDCB,0x0AC42,0x09ED9,0x08F50,0x0FBEF,0x0EA66,0x0D8FD,0x0C974,
     0x04204,0x0538D,0x06116,0x0709F,0x00420,0x015A9,0x02732,0x036BB,
     0x0CE4C,0x0DFC5,0x0ED5E,0x0FCD7,0x08868,0x099E1,0x0AB7A,0x0BAF3,
     0x05285,0x0430C,0x07197,0x0601E,0x014A1,0x00528,0x037B3,0x0263A,
     0x0DECD,0x0CF44,0x0FDDF,0x0EC56,0x098E9,0x08960,0x0BBFB,0x0AA72,
     0x06306,0x0728F,0x04014,0x0519D,0x02522,0x034AB,0x00630,0x017B9,
     0x0EF4E,0x0FEC7,0x0CC5C,0x0DDD5,0x0A96A,0x0B8E3,0x08A78,0x09BF1,
     0x07387,0x0620E,0x05095,0x0411C,0x035A3,0x0242A,0x016B1,0x00738,
     0x0FFCF,0x0EE46,0x0DCDD,0x0CD54,0x0B9EB,0x0A862,0x09AF9,0x08B70,
     0x08408,0x09581,0x0A71A,0x0B693,0x0C22C,0x0D3A5,0x0E13E,0x0F0B7,
     0x00840,0x019C9,0x02B52,0x03ADB,0x04E64,0x05FED,0x06D76,0x07CFF,
     0x09489,0x08500,0x0B79B,0x0A612,0x0D2AD,0x0C324,0x0F1BF,0x0E036,
     0x018C1,0x00948,0x03BD3,0x02A5A,0x05EE5,0x04F6C,0x07DF7,0x06C7E,
     0x0A50A,0x0B483,0x08618,0x09791,0x0E32E,0x0F2A7,0x0C03C,0x0D1B5,
     0x02942,0x038CB,0x00A50,0x01BD9,0x06F66,0x07EEF,0x04C74,0x05DFD,
     0x0B58B,0x0A402,0x09699,0x08710,0x0F3AF,0x0E226,0x0D0BD,0x0C134,
     0x039C3,0x0284A,0x01AD1,0x00B58,0x07FE7,0x06E6E,0x05CF5,0x04D7C,
     0x0C60C,0x0D785,0x0E51E,0x0F497,0x08028,0x091A1,0x0A33A,0x0B2B3,
     0x04A44,0x05BCD,0x06956,0x078DF,0x00C60,0x01DE9,0x02F72,0x03EFB,
     0x0D68D,0x0C704,0x0F59F,0x0E416,0x090A9,0x08120,0x0B3BB,0x0A232,
     0x05AC5,0x04B4C,0x079D7,0x0685E,0x01CE1,0x00D68,0x03FF3,0x02E7A,
     0x0E70E,0x0F687,0x0C41C,0x0D595,0x0A12A,0x0B0A3,0x08238,0x093B1,
     0x06B46,0x07ACF,0x04854,0x059DD,0x02D62,0x03CEB,0x00E70,0x01FF9,
     0x0F78F,0x0E606,0x0D49D,0x0C514,0x0B1AB,0x0A022,0x092B9,0x08330,
     0x07BC7,0x06A4E,0x058D5,0x0495C,0x03DE3,0x02C6A,0x01EF1,0x00F78};

  //Initial CRC value is 0x0FFFF.
  register word
    newCrc;
  newCrc=seed;
  //This algorithim is based on the IrDA LAP example.
  while(len--)
    newCrc = (newCrc >> 8) ^ crcLookupTable[(newCrc ^ *bufptr++) & 0xff];
  //Make this crc match the one's complement that is sent in the packet.
  return(~newCrc);
  }
//============================================================================
COMMAND_PACKET
  incoming_command;
COMMAND_PACKET
  outgoing_response;
//============================================================================
//                              send_packet()
//
// send_packet() will send set the CRC in outgoing_response and send it to
// the host.
//----------------------------------------------------------------------------
void send_packet(void)
  {
  //In order to send the entire packet in one call to "write()", we
  //need to place the CRC packed against the data. If data_length
  //happens to be MAX_DATA_LENGTH, then the position of the CRC is
  //outgoing_response.CRC (like you would expect), but if data_length
  //is less, the CRC moves into the data area, packed into the first
  //unused data position.
  //
  //Create a pointer to the first unused data positions.
  word
   *packed_CRC_position;
  packed_CRC_position=
    (word *)&outgoing_response.data[outgoing_response.data_length];
  *packed_CRC_position=
    get_crc((ubyte *)&outgoing_response,outgoing_response.data_length+2,0xFFFF);
  //command, length, data[data_length], crc, crc
  SendData((ubyte *)&outgoing_response,outgoing_response.data_length+4);
  }
//============================================================================
//                              check_for_packet()
//
// check_for_packet() will see if there is a valid packet in the input buffer.
// If there is, it will copy it into incoming_command and return 1. If there
// is not it will return 0. incoming_packet may get partially filled with
// garbage if there is not a valid packet available.
//----------------------------------------------------------------------------
ubyte check_for_packet(void)
  {
  Sync_Read_Buffer();
  ubyte
    i;
  //First off, there must be at least 4 bytes available in the input stream
  //for there to be a valid command in it (command, length, no data, CRC).
  if((i=BytesAvail())<4)
  {
	usleep(25);
    return(0);
  }
  PrintHex("Got: ", &incoming_command.command, incoming_command.data_length+2);
  //The "peek" stuff allows us to look into the RS-232 buffer without
  //removing the data.
  Sync_Peek_Pointer();
  //Only commands 0 through MAX_COMMAND are valid.
  if(MAX_COMMAND<(0x3F&(incoming_command.command=PeekByte())))
    {
    //Throw out one byte of garbage. Next pass through should re-sync.
    GetByte();
    return(0);
    }
  //There is a valid command byte. Get the data_length. The data length
  //must be within reason.
  if(MAX_DATA_LENGTH<(incoming_command.data_length=PeekByte()))
    {
    //Throw out one byte of garbage. Next pass through should re-sync.
    GetByte();
    return(0);
    }
  //Now there must be at least incoming_command.data_length+sizeof(CRC) bytes
  //still available for us to continue.
  if((int)PeekBytesAvail()<(incoming_command.data_length+2))
    //It looked like a valid start of a packet, but it does not look
    //like the complete packet has been received yet.
    return(0);
  //There is enough data to make a packet. Transfer over the data.
  for(i=0;i<incoming_command.data_length;i++)
    incoming_command.data[i]=PeekByte();
  //Now move over the CRC.
  incoming_command.CRC.as_bytes[0]=PeekByte();
  incoming_command.CRC.as_bytes[1]=PeekByte();
  //Now check the CRC.
  if(incoming_command.CRC.as_word==
     get_crc((ubyte *)&incoming_command,incoming_command.data_length+2,0xFFFF))
    {
    //This is a good packet. I'll be horn swaggled. Remove the packet
    //from the serial buffer.
    AcceptPeekedData();
    //Let our caller know that incoming_command has good stuff in it.
    return(1);
    }
  //The CRC did not match. Toss out one byte of garbage. Next pass through
  //should re-sync.
  GetByte();
  return(0);
  }
//============================================================================
