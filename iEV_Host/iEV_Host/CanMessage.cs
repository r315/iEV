using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace iEV_Host
{
    class CanMessage : IMessageSerial
    {
        public const int MESSAGE_ID01 = 0x601;
        public const int MESSAGE_ID02 = 0x602;
        public const int CAN_MSG_SIZE = 10;
        public const int PAYLOAD_SIZE = 8;

        protected byte[] bytes;

        public int timeStamp { get; set; }

        public bool repeat { get; set; }

        public int Address
        {
            get { return Address; }
            set
            {
                bytes[0] = (byte)(value >> 8);
                bytes[1] = (byte)(value);
            }
        }

        public byte [] GetBytes()
        {
            return bytes;
        }

        public void SetPayload(byte[] payload)
        {
            for(int i = 0; i < PAYLOAD_SIZE; i++)
            {
                bytes[i+2] = payload[i]; // skip address
            }
        }

        public CanMessage(int size)
        {
            bytes = new byte[size];
        }

        public CanMessage()
        {
            bytes = new byte[8];
            bytes[0] = 255; // dummy message, set lowest priority
            bytes[1] = 255;
            repeat = false;
        }

    } 
}
