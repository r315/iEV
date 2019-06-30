using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Text;
using System.Text.RegularExpressions;
using System.Threading.Tasks;
using CsvHelper;
using Microsoft.VisualBasic.FileIO;


namespace iEV_Host
{
   
    class iEvHost    
    {
        private static readonly Regex trimmer = new Regex(@"\s\s+");
        private static int RPM_INCREMENT = 100;
        private static DateTime lastTime;

        //params order
        // date, time stamp, itf, id, size, data
        //(2019-06-04 13:09:49.115038) can0 601 [8] 00 00 0F 19 00 1D 02 85
        private static CanMessage CreateMessage(params string[] cols)
        {
            CanMessage msg;            

            int id = Convert.ToInt32(cols[3], 16);
            int size = Convert.ToInt32(cols[4].Trim(new char[] { '[', ']' }), 16);
            byte[] payload = new byte[size];

            for(int i = 0; i < size; i++)
            {
                int x = Convert.ToInt32(cols[i + 5], 16);
                payload[i] = (byte)x;
            }

            DateTime ts = DateTime.Parse(cols[1].Trim(')'));

            if (lastTime == DateTime.MinValue)
            {
                lastTime = ts;
            }

            int time = ts.Subtract(lastTime).Milliseconds;

            lastTime = ts;

            switch (id)
            {
                case CanMessage.MESSAGE_ID01:
                    msg = new CanMessage01();
                    break;
                case CanMessage.MESSAGE_ID02:
                    msg = new CanMessage02();
                    break;
                default:
                    msg = new CanMessage();
                    break;
            }
            msg.timeStamp = time;
            msg.SetPayload(payload);            
            return msg;
        }

        static List<CanMessage>  LoadFile(String filename)
        {
            List<CanMessage> messages = new List<CanMessage>(); ;
            StreamReader file = null;

            string delimiter;            

            try
            {
                file = File.OpenText(filename);
                lastTime = DateTime.MinValue;       

                string line = Path.GetExtension(filename);

                switch (line)
                {
                    case ".csv":
                        delimiter = ";";
                        line = file.ReadLine(); // skipt headers
                        break;
                    case ".txt":
                        delimiter = " ";
                        break;
                    default:
                        Console.WriteLine("Extension '{0}' not supported", line);
                        return null;
                }

                while ((line = file.ReadLine()) != null)
                {
                    CanMessage msg;
                    line = trimmer.Replace(line, delimiter);
                    msg = CreateMessage(line.Split(delimiter[0]).Skip(1).ToArray());
                    messages.Add(msg);
                }
            }
            catch (Exception e)
            {
                Console.WriteLine(e.Message);
                messages = null;
            }            
            finally
            {
                if(file != null)
                    file.Dispose();
            }
            return messages;
        }       

        static void ManualMode(MessageSender<CanMessage> sender)
        {
            CanMessage01 msg = new CanMessage01();
            CanMessage02 msg2 = new CanMessage02();

            Console.WriteLine("\n User Up/Down Arrows to change RPM, 'r' to reset 'esc' to exit\n");
            ConsoleKeyInfo cki;

            int rpm = 0;
            msg.MotorTemp = 0;

            do
            {
                Console.Write("Rpm: {0}              \r", rpm);
                cki = Console.ReadKey();
                Console.Write("\r");


                switch (cki.Key.ToString())
                {

                    case "UpArrow":
                        if (rpm < 10000)
                            rpm += RPM_INCREMENT;
                        break;

                    case "DownArrow":
                        rpm -= RPM_INCREMENT;
                        if (rpm < 0)
                            rpm = 0;

                        break;

                    case "R":
                        rpm = 0;
                        break;

                    case "OemComma":
                        if (msg.MotorTemp > 0)
                            msg.MotorTemp -= 25;
                        break;

                    case "OemPeriod":
                        if (msg.MotorTemp < 200)
                            msg.MotorTemp += 25;
                        break;

                    default: break;
                }

                //Console.WriteLine(cki.Key.ToString());

                msg.Rpm = rpm;
                msg.timeStamp = 10;
                msg.repeat = true;

            } while (cki.Key != ConsoleKey.Escape && sender.Send(msg));
        }

        static void FileMode(MessageSender<CanMessage> sender, string filename)
        {            
            Console.WriteLine("Reading messages from file '{0}'", filename);
           
            List<CanMessage> messageList = null;
            
            messageList = LoadFile(filename);


            if(messageList != null)
            {
                Console.WriteLine("Ready to Send {0} messages", messageList.Count);
                Console.WriteLine("Press any to start");
                Console.Read();
                Console.WriteLine("Sending messages...");
                DateTime start = DateTime.Now;
                sender.SendMsgList(messageList);
                sender.Wait();
                TimeSpan time = DateTime.Now.Subtract(start);
                Console.WriteLine("Completed in {0} seconds", time.TotalSeconds);
            }            
        }

        static void Main(string[] args)
        {
            MessageSender<CanMessage> sender = new MessageSender<CanMessage>();

            if (!sender.isActive()) return;

            if(args.Length > 0)
            {
                FileMode(sender, args[0]);
            }
            else
            {
                ManualMode(sender);
            }

            sender.Terminate(); 
        }      
    }
}
