# Mist Alarm

The mist alarm is the perfect solution for people (like me) who just cannot get up in the morning and want to lie there for hours dozing back to sleep.

## Why I built it

I built this as a solution for me sleeping through alarms and constant snoozing when they do work.

## What it does

The mist alarm is a device which sprays water a good distance (the width of a bed) at the time the user sets on the device. Not only does it spray water, it sounds the buzzer too making the user even more likely to wake up. Both alarm and water jet can are deactivated by a button on the back of the device meaning you must get up and pass through the water to turn it off.

## How it works

The mist alarm uses an RTC to keep time set by the user with a 16*2 lcd display and rotary encoding module. An Elagoo uno reads this and at the time set by the user fires a passive piezo buzzer and a diaphram water pump. The pump takes water from the resovoir and pushes it through the following line that ends with a misting nozzle. The deactivate the pump and buzzer there is a momentary push button.

## Problems hit producing this

Voltage spikes from the motor caused board resets. A flyback diode fixed this issue. Consant brownout from the motor drawing large currents was also causing problems, a capacitor helped here. The lcd screen is also very dim and hard to see at night in the dark and the jet is probably angled a bit too far back limiting the range the water can spray. both of these can be improved in future versions with 10 degrees less angle on the top part and on oled display. 

## Results

I have personally been using this now for a good few months. The system really does wake you up with a bit of a shock and the adrenaline rush helps wake you up for good.