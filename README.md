# Alarmon
c++ simple alarm clock

Using IrrLicht graphics engine && SDL_Mixer for audio output

The clock displays the current time and the time when the alarm will go off on the screen (24-hour format)
Time for alarm to go off is set in txt file "ressystem/alarm.txt" (24-hour format) (example: "15:00")

When it's time for the alarm to go off (alarm.txt time == local time), the alarm will start to play the alarm ("alarm.wav") slowly at 20% volume. Over time (30-60 seconds) it will get louder and faster until it is at 100% volume and playing at a speed of approximately 200%-300% the rate of its original speed. It will continue indefinitely until [Space] is pressed.


Press [Space] to turn alarm off

Press [Backspace] to force alarm to play
