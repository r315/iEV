using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

using System.IO.Ports;
using System.Threading;
 
namespace iEV_Host
{
    class MessageSender<T>  where T : IMessageSerial
    {
        public const int BAUD_RATE = 115200;

        private SerialPort serialPort;
        private readonly object monitor = new object();
        private bool active = false;

        private LinkedList<T> ml;

        public MessageSender()
        {
            serialPort = new SerialPort();

            if (SetPortName(serialPort) == false)
            {
                return;
            }

            serialPort.BaudRate = BAUD_RATE;
            serialPort.Parity = Parity.None;
            serialPort.DataBits = 8;
            serialPort.StopBits = StopBits.One;
            serialPort.Handshake = Handshake.None;

            try
            {
                serialPort.Open();
            }
            catch (Exception e)
            {
                Console.Write(e.ToString());
                return;
            }

            ml = new LinkedList<T>();

            Thread serialPortThread = new Thread(SerialPortSender);
            active = true;
            serialPortThread.Start();
        }

        public void Send(T msg)
        {
            
            lock (monitor)
            {
                if (!active)
                    return;

                ml.AddLast(msg);
                Monitor.Pulse(monitor);
            }
        }

        public void Terminate()
        {
            lock (monitor)
            {
                if (!active)
                    return;
                active = false;
                Monitor.Pulse(monitor);
            }
        }

        // Display Port values and prompt user to enter a port.
        public bool SetPortName(SerialPort sp)
        {
            string[] portNames = SerialPort.GetPortNames();

            if (portNames.Length == 0)
            {
                Console.WriteLine("No Serial Ports Available!");
                return false;
            }

            Console.WriteLine("Available Ports:");
            foreach (string s in SerialPort.GetPortNames())
            {
                Console.WriteLine("   {0}", s);
            }

            Console.Write("\nEnter COM port (Default: {0}): ", sp.PortName);
            string portName = Console.ReadLine();

            if (portName != "" && (portName.ToLower()).StartsWith("com"))
            {
                sp.PortName = portName;
            }

            return true;
        }

        private void SerialPortSender()
        {
            lock (monitor)
            {
                try
                {                    
                    while (active)
                    {
                        while (ml.Count > 0)
                        {
                            LinkedListNode<T> node = ml.First;
                            ml.Remove(node);
                            byte[] bytes = node.Value.GetBytes();

                            //Console.WriteLine("Thread {0} sending {1}", Thread.CurrentThread.ManagedThreadId, ToByteString(bytes));
                            serialPort.Write(bytes, 0, bytes.Length);                            
                        }
                        //Console.WriteLine("Thread {0} sleeping", Thread.CurrentThread.ManagedThreadId);
                        Monitor.Wait(monitor);
                    }
                    serialPort.Close();
                }
                catch (ThreadInterruptedException e)
                {
                    throw e;
                }
                
            }
        }

        public string ToByteString(byte [] bytes)
        {           
            StringBuilder sb = new StringBuilder("[ ");

            int i = 0;
            while (true)
            {
                sb.Append(String.Format("{0:X2}", bytes[i++]));
                if (i == bytes.Length)
                {
                    sb.Append(" ]");
                    break;
                }
                sb.Append(", ");
            }

            return sb.ToString();
        }

    }
}
