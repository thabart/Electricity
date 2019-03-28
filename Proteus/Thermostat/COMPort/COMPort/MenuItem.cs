using System;

namespace COMPort
{
    public class MenuItem
    {
        public MenuItem( string displayName, Func<bool> callback)
        {
            DisplayName = displayName;
            Callback = callback;
        }

        public string DisplayName { get; set; }
        public Func<bool> Callback { get; set; }
    }
}
