/**
  * Copyright(C), 2018
  * Name: long_pressed_name
  * Author: Wilson Lan
  * Description:
  *     Your friend is typing his name into a keyboard. Sometimes, when typing
  *     a character c, the key might get long pressed, and the character will
  *     be typed 1 or more times.
  *     You examine the typed characters of the keyboard. Return True if it is
  *     possible that it was your friends name, with some characters (possibly
  *     none) being long pressed.
  */

class Solution {
    public:
        bool isLongPressedName(string name, string typed) {
            int index = 0, lens = name.length();
            for (const auto &c : typed) {
                if (index < lens && c == name[index]) {
                    ++index;
                } else if (index > 0 && c == name[index-1]) {
                    continue;
                } else {
                    return false;
                }
            }
            return index == lens;
        }
};
