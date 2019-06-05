using System;

namespace iEV_Host
{
    internal interface IMessageSerial
    {
        int timeStamp { get; set; }
        bool repeat{get; set;}
        byte[] GetBytes();
        void SetPayload(byte[] payload);
    } 
}