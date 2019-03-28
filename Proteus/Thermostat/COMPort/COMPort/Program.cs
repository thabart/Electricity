using System;
using System.Collections.Generic;
using System.IO.Ports;

namespace COMPort
{
    class Program
    {
        public enum CommandTypes
        {
            GET_MIN_TEMPERATURE_RESULT = 3
        }

        private static SerialPort _serialPort = new SerialPort("COM4", 9600);

        private static Dictionary<string, MenuItem> _menu = new Dictionary<string, MenuItem>
        {
            { "1", new MenuItem("Set temperature", SetTemperature) },
            { "2", new MenuItem("Get temperature", GetTemperature) },
            { "Q", new MenuItem("Exit", Exit) }
        };

        private static void DisplayMenu()
        {
            foreach(var kvp in _menu)
            {
                Console.WriteLine($"{kvp.Key} : {kvp.Value.DisplayName}");
            }

            var input = Console.ReadLine();
            if (!_menu.ContainsKey(input))
            {
                Console.WriteLine("the input is invalid");
                DisplayMenu();
                return;
            }
            if (!_menu[input].Callback())
            {
                return;
            }

            DisplayMenu();
        }

        private static bool SetTemperature()
        {
            Console.WriteLine("Enter the temperature");
            var temperature = Console.ReadLine();
            var buffer = new List<byte>
            {
                0x02,
                (byte)int.Parse(temperature)
            };
            _serialPort.Write(buffer.ToArray(), 0, 2);
            return true;
        }

        private static bool GetTemperature()
        {
            var buffer = new List<byte>
            {
                0x01
            };
            _serialPort.Write(buffer.ToArray(), 0, 1);
            return true;
        }

        private static bool Exit()
        {
            Console.WriteLine("Bye !");
            Console.ReadLine();
            return false;
        }

        static void Main(string[] args)
        {
            _serialPort.Parity = Parity.None;
            _serialPort.DataBits = 8;
            _serialPort.StopBits = StopBits.One;
            _serialPort.DataReceived += HandleDataReceived;
            _serialPort.Open();
            DisplayMenu();
        }

        private static void HandleDataReceived(object sender, SerialDataReceivedEventArgs e)
        {
            SerialPort sp = (SerialPort)sender;
            var firstByte = sp.ReadByte();
            var commandType = (CommandTypes)firstByte;
            switch(commandType)
            {
                case CommandTypes.GET_MIN_TEMPERATURE_RESULT:
                    int temperature = sp.ReadByte();
                    Console.WriteLine($"Minimum temperature is {temperature}");
                    break;
            }
        }
    }
}
