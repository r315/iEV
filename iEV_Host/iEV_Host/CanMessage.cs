using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace iEV_Host
{
    class CanMessage : IMessageSerial
    {
        protected byte[] bytes;

        public byte [] GetBytes()
        {
            return bytes;
        }

        public CanMessage(int size)
        {
            bytes = new byte[size];
        }

        public CanMessage()
        {
            bytes = new byte[8];
            bytes[0] = 255; // Set lowest priority
            bytes[1] = 255; 
        }
    } 
}
