using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace iEV_Host
{
    class CanMessage : IMessageSerial
    {
        int Address { get; set; }
        public int Rpm { get; set; }
        public byte MotorTemp { get; set; }
        public byte ControllerTemp { get; set; }
        public int RmsCurrent { get; set; }
        public int CapacitorVoltage { get; set; }

        public Status status;

        public byte [] GetBytes()
        {
            byte[] msg = new byte[10];
            msg[0] = (byte)(Address >> 8);
            msg[1] = (byte)(Address);

            msg[2] = (byte)(Rpm >> 8);
            msg[3] = (byte)(Rpm);

            msg[4] = MotorTemp;
            msg[5] = ControllerTemp;

            msg[6] = (byte)(RmsCurrent >> 8);
            msg[7] = (byte)(RmsCurrent);

            msg[6] = (byte)(CapacitorVoltage >> 8);
            msg[7] = (byte)(CapacitorVoltage);

            return msg;
        }

        public CanMessage(int addr)
        {
            Address = addr;
            status = Status.Empty;
        }       
        
        public override string ToString() => String.Format("Address: 0x{0:X}, Rpm: {1}, Motor Temp: {2}", Address, Rpm, MotorTemp);
    }
}
