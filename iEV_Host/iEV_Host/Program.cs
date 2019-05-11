using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;



namespace iEV_Host
{
    public enum Status {Empty, Ready, Sent };

    class Program
    {
        private static int RPM_INCREMENT = 50;
        private static int MESSAGE_ID = 0x602;

        static void Main(string[] args)
        {            
            CanMessage msg = new CanMessage(MESSAGE_ID);
            MessageSender<CanMessage> sender = new MessageSender<CanMessage>();
            

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
                sender.Send(msg);

            } while (cki.Key != ConsoleKey.Escape);

            sender.Terminate();
        }

      
    }
}
