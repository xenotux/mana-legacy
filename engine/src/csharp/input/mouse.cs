using System.Collections.Generic;

namespace Mana
{
    public class Mouse
    {
        public double positionX;
        public double positionY;

        public Dictionary<MouseKey, KeyState> keys = new Dictionary<MouseKey, KeyState>();
    }
}