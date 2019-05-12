using System;

namespace iEV_Host
{
    internal class CanMessage01 : CanMessage
    {
        private const int MESSAGE_ID01 = 0x601;
        private const int CAN_MSG_SIZE = 10;

        private int Address {
            get { return Address; }
            set
            {             
                base.bytes[0] = (byte)(value >> 8);
                base.bytes[1] = (byte)(value);
            }
        }
        public int Rpm {
            get { return Rpm; }
            set
            {               
                bytes[2] = (byte)(value >> 8);
                bytes[3] = (byte)(value);
            }
        }
        public byte MotorTemp {
            get { return MotorTemp; }
            set
            {        
                bytes[4] = value;
            }
        }
        public byte ControllerTemp {
            get { return ControllerTemp; }
            set
            {               
                bytes[5] = value;
            }
        }
        public int RmsCurrent
        {
            get { return RmsCurrent; }
            set
            {                
                bytes[6] = (byte)(value >> 8);
                bytes[7] = (byte)(value);
            }
        }
        public int CapacitorVoltage {
            get { return CapacitorVoltage; }
            set
            {                
                bytes[8] = (byte)(value >> 8);
                bytes[9] = (byte)(value);
            }
        }

        public CanMessage01() : base (CAN_MSG_SIZE)
        {            
            Address = MESSAGE_ID01;            
        }

        public override string ToString() => String.Format("Address: 0x{0:X}, Rpm: {1}, Motor Temp: {2}", Address, Rpm, MotorTemp);
    }
}