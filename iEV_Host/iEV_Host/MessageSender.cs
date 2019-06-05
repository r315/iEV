using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

using System.IO.Ports;
using System.Threading;


namespace iEV_Host
{
    class MessageSender<T>  where T : class, IMessageSerial, new()
    {
        public const int BAUD_RATE = 115200;

        private SerialPort serialPort;
        private readonly object monitor = new object();        
        private bool active = false;

        private LinkedList<T> ml;
        private T lastMessage;

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

            lastMessage = new T();

            Thread serialPortThread = new Thread(SerialPortSender);
            active = true;
            serialPortThread.Start();
        }
        
        public bool Send(T msg)
        {            
            lock (monitor)
            {
                if (!active)
                    return false;

                ml.AddLast(msg);
                Monitor.Pulse(monitor);
            }
            return true;
        }

        public bool SendMsgList(List<T> msg)
        {
            lock (monitor)
            {
                if (!active)
                    return false;

                foreach (T item in msg)
                {
                    ml.AddLast(item);
                }
                
                Monitor.Pulse(monitor);
            }
            return true;
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
                using (serialPort)
                {
                    while (active)
                    {
                        try
                        {
                            if (ml.Count > 0)
                            {                                
                                LinkedListNode<T> node = ml.First;
                                int sleep;
                                T msg = node.Value;
                                byte[] data = msg.GetBytes();                                
                                serialPort.Write(data, 0, data.Length);
                                //Console.WriteLine("Thread {0} sleeping", Thread.CurrentThread.ManagedThreadId);
                                if (!msg.repeat)
                                {
                                    ml.Remove(node);                                
                                    lastMessage = node.Value;
                                }
                                sleep = node.Value.timeStamp;

                                if (sleep < 0)
                                    Console.WriteLine();
                                    
                                Monitor.Wait(monitor, sleep);
                            }
                            else
                            {
                                Monitor.Wait(monitor);
                            }
                        }
                        catch (ThreadInterruptedException e)
                        {
                            Console.WriteLine("Thread {0} Interrupted, {1}", Thread.CurrentThread.ManagedThreadId, e);
                        }
                        catch (System.IO.IOException e)
                        {
                            Console.WriteLine("Thread {0}: {1}", Thread.CurrentThread.ManagedThreadId, e.Message);
                            active = false;
                            //throw e;
                        }
                    }
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
