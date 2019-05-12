using System;
namespace iEV_Host
{
    internal class CanMessage02 : CanMessage
    {
        
        private const int MESSAGE_ID01 = 0x602;
        private const int CAN_MSG_SIZE = 10;

        private int Address
        {
            get { return Address; }
            set
            {
                bytes[0] = (byte)(value >> 8);
                bytes[1] = (byte)(value);
            }
        }
        public int Frequency
        {
            get { return Frequency; }
            set
            {
                bytes[2] = (byte)(value >> 8);
                bytes[3] = (byte)(value);
            }
        }
        public byte FaultPrimary
        {
            get { return FaultPrimary; }
            set
            {
                bytes[4] = value;
            }
        }
        public byte FaultSecondary
        {
            get { return FaultSecondary; }
            set
            {
                bytes[5] = value;
            }
        }
        public byte Throttle
        {
            get { return Throttle; }
            set
            {
                bytes[6] = value;                
            }
        }
        public byte Brake
        {
            get { return Brake; }
            set
            {
                bytes[7] = value;
            }
        }

        /**
         * bit  description
         * 0 - Econo
         * 1 - Regen
         * 2 - Reverse
         * 3 - Brake Light
         * */
        public byte System
        {
            get { return Brake; }
            set
            {
                bytes[8] = value;
            }
        }

        public CanMessage02() : base(CAN_MSG_SIZE)
        {
            Address = MESSAGE_ID01;
        }

        public override string ToString() => String.Format(@"
            Address: 0x{0:X}\n
            Frequency: {1}\n
            FaultPrimary: {2}\n
            FaultSecondary: {3}\n
            Throttle: {4}\n
            Brake: {5}\n
            System: {6}\n",
            Address, Frequency, FaultPrimary, FaultSecondary, Throttle, Brake, System);
    }    
}