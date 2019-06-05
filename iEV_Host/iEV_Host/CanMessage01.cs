using System; 

namespace iEV_Host
{
    internal class CanMessage01 : CanMessage
    {       
        public int Rpm {
            get { return bytes[2]<<8 | bytes[3]; }
            set
            {               
                bytes[2] = (byte)(value >> 8);
                bytes[3] = (byte)(value);
            }
        }
        public byte MotorTemp {
            get { return bytes[4]; }
            set
            {        
                bytes[4] = value;
            }
        }
        public byte ControllerTemp {
            get { return bytes[5]; }
            set
            {               
                bytes[5] = value;
            }
        }
        public int RmsCurrent
        {
            get { return bytes[6] << 8 | bytes[7]; }
            set
            {                
                bytes[6] = (byte)(value >> 8);
                bytes[7] = (byte)(value);
            }
        }
        public int CapacitorVoltage {
            get { return bytes[8] << 8 | bytes[9]; }
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

        //public override string ToString() => String.Format("Address: 0x{0:X}, Rpm: {1}, Motor Temp: {2}", Address, Rpm, MotorTemp);
    }
}