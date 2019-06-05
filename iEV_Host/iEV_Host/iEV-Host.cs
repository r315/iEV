using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using CsvHelper;
using Microsoft.VisualBasic.FileIO;


namespace iEV_Host
{
   
    class iEvHost    
    {
        
        private static int RPM_INCREMENT = 100;

        static List<CanMessage>  LoadFromFile(String file)
        {             
            List<CanMessage> messages;
            using (CsvReader csv = new CsvReader(File.OpenText(file)))
            {
                messages = new List<CanMessage>();
                csv.Configuration.Delimiter = ";";                
                csv.Read();
                csv.ReadHeader();

                DateTime lastTime = DateTime.MinValue;
                int time;
               

                while (csv.Read())
                {
                    CanMessage msg;                    
                    DateTime ts = DateTime.Parse(csv.GetField("Column3").Trim(')'));
                    //int ts = Convert.ToInt32(dt.ToString("mmssfff"));                   

                    if (lastTime == DateTime.MinValue)
                    {
                        lastTime = ts;                        
                    }                    

                    time = ts.Subtract(lastTime).Milliseconds;
                    //Console.WriteLine(time);
                    lastTime = ts;

                    if(time < 0)
                        Console.WriteLine(time);

                    int id = Convert.ToInt32(csv.GetField("Column5"),16);
                    int size = Convert.ToInt32(csv.GetField("Column6").Trim(new char[] { '[', ']' }), 16);
                    byte[] payload = new byte[CanMessage.PAYLOAD_SIZE];

                    for (int i = 0; i < payload.Length; i++)
                    {
                        int x = Convert.ToInt32(csv.GetField(i + 6), 16);
                        payload[i] = (byte)x;                        
                    }

                    if (id == CanMessage.MESSAGE_ID01)
                    {
                        msg = new CanMessage01();
                    }
                    else
                    {
                        msg = new CanMessage02();
                    }

                    msg.timeStamp = time;
                    msg.SetPayload(payload);
                    messages.Add(msg);
                }
            }            
            return messages;
        }

        static void Main(string[] args)
        {
            MessageSender<CanMessage> sender = new MessageSender<CanMessage>();

            sender.SendMsgList(LoadFromFile(@"..\..\candataeco.csv"));

            CanMessage01 msg = new CanMessage01();
            CanMessage02 msg2 = new CanMessage02();
            
            Console.WriteLine("\n User Up/Down Arrows to change RPM, 'r' to reset 'esc' to exit\n");
            ConsoleKeyInfo cki;

            int rpm = 0;

            do
            {
                Console.Write("Rpm: {0}              \r", rpm);
                cki = Console.ReadKey();
                Console.Write("\r");


                switch (cki.Key.ToString())
                {

                    case "UpArrow":
                        if(rpm < 10000)
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

                    default: break;
                }               
                
                //Console.WriteLine(cki.Key.ToString());

                msg.Rpm = rpm;
                msg.timeStamp = 10;
                msg.repeat = true;
                

            } while (cki.Key != ConsoleKey.Escape && sender.Send(msg));

            sender.Terminate(); 
        }      
    }
}
