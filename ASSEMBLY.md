# Hoop Assembly Guide

### Battery Pieces

![image](https://github.com/kgizzi/tulahoop/blob/master/guide/battery_2.jpg)

![image](https://github.com/kgizzi/tulahoop/blob/master/guide/battery_3.jpg)

1. Screw a spring onto the plastic end piece and solder wire to the other side. Make multiples.

![image](https://github.com/kgizzi/tulahoop/blob/master/guide/battery_4.jpg)

![image](https://github.com/kgizzi/tulahoop/blob/master/guide/battery_5.jpg)

2. We got creative and used split material to mold the end piece to fit securely inside the 5/8 tubing. We are open to suggestions with how to improve the battery pieces.


### Prepare the Hoop

1. Determine the size hoop you want to make and cut that length of 3/4 inch polypro tubing.

30 inch diameter hoop * Pi (3.14) = 94.25 inches of tubing.

2. Work the tubing until it is shaped like a circle and the two ends touch.

3. Using the dremel, sand down the ends of the hoop so they fit neatly together.


### IR Receiver

![image](https://github.com/kgizzi/tulahoop/blob/master/guide/ir_1.jpg)

1. Solder the data, ground and positive wires (about 4-5 inches long) onto the ends of the IR sensor.

![image](https://github.com/kgizzi/tulahoop/blob/master/guide/ir_2.jpg)

2. Slip the 2.5mm heat shrink tubing (~1 inch) over the connections, as close to the IR sensor as possible and use the heat gun to shrink them into place.

![image](https://github.com/kgizzi/tulahoop/blob/master/guide/ir_3.jpg)


### LED Strip

![image](https://github.com/kgizzi/tulahoop/blob/master/guide/led_1.jpg)

1. Solder the 1000uF capacitor to the correct side of the APA102 LED strip. The arrows should be pointing away from the capacitor. The negative side of the capacitor has a stripe on it.

![image](https://github.com/kgizzi/tulahoop/blob/master/guide/led_2.jpg)

2. Solder on a wire each to the data and clock pads on the same end of the LED strip. Make sure are long enough to attach to the controller (~6 inches).

![image](https://github.com/kgizzi/tulahoop/blob/master/guide/led_3.jpg)

3. Using plyers, gently bend the capacitor back behind the LED strip. The wires should run under it.


### Controller

![image](https://github.com/kgizzi/tulahoop/blob/master/guide/controller_1.jpg)

1. Solder battery and ground wires (~4-5 inches) facing upwards to the top of the Trinket M0.

![image](https://github.com/kgizzi/tulahoop/blob/master/guide/controller_2.jpg)

2. Choose an appropriate placement for where the controller will rest on the underside of the LED strip. You will want it to be positioned between ~4 to 5 inches from the end of the LED strip. Line that section on the back side with electrical tape, so connections to the board will not touch the pads on the LED strip.

![image](https://github.com/kgizzi/tulahoop/blob/master/guide/controller_3.jpg)

3. Solder the DATA wire to pin 3 and solder the CLK wire to pin 2. I chose to solder the wires coming up from underneath the controller. The micro USB port should be facing away from the end of the LED strip where the capacitor is located.

4. Solder the data wire from the IR sensor to pin 0 on the controller.

5. At this point, all of the connections to the controller have been soldered in place. Now, solder the battery and ground wires of the controller to the LED strip. I chose to solder the battery wire to the underside of the LED strip (pictured below in final assembly), and soldered the ground wire to the front side of the LED strip.

![image](https://github.com/kgizzi/tulahoop/blob/master/guide/controller_4.jpg)

6. Lastly solder the battery and ground wires of the IR sensor to the underside of the LED strip. I chose to use some scotch tape temporarily to hold everything in place.


### Final Assembly (Positive End)

![image](https://github.com/kgizzi/tulahoop/blob/master/guide/final_1.jpg)

1. Go ahead and hot glue the positive and negative wires for the controller to the LED strip to reenforce the connections.

![image](https://github.com/kgizzi/tulahoop/blob/master/guide/final_2.jpg)

2. Slide a 2 inch piece of the 5/8 tubing (inner) over the capacitor. This is an important step!

![image](https://github.com/kgizzi/tulahoop/blob/master/guide/final_3.jpg)

3. Solder on the battery connector as close as possible to the positive side of the capacitor. Try to align the piece as best as possible with the LED strip.

4. Now would be a good time to test the whole thing, if you haven't already. Program the controller and hold the positive end of a battery to the battery piece and using a wire attached to the negative end of the opposte end of the light strip. It should light up! You may also want to test the remote is working too. Once it is in the hoop, it's not very easy to fix.

![image](https://github.com/kgizzi/tulahoop/blob/master/guide/final_4.jpg)

5. Hot glue the battery piece to the end of the LED strip. Feel free to use more hot glue than pictured.

6. Slide the tubing up to the battery piece and rest in place.


**Below is a work in progress**


<image needed here>

7. Carefully drill and dremel a 3mm hole in the inner tubing behind the capacitor and insert the nut. This will help lock the inner tubing in place and can be removed later to help disassemble if the hoop needs repair.

8. Hot glue the remaining soldered connections to the controller and the LED strip to strengthen them. Be sure not to glue the micro USB port shut.

9. Figure out how many LEDs are needed for the size of hoop you are making.

30 inch diameter hoop = 94.25 inches - minus 2.5 inches for the battery = 91.75 inches

Convert (91.75 inches) to meters = 2.33m

The density of the LED strip is 60 LEDs per meter = 140 LEDs (rounding up)

10. Modify the code in src/main.cpp and set `NUM_LEDS` to 140. Upload the program to the controller as explained in README.md. Test the strip again by pressing the ends to a battery and make sure it is working correctly.

**Below is even more of a work in progress**

11. Attach some padding to the back side of the LED strip but be careful not to put too much as it will make it difficult to pull through the tubing.

12. Tie a small heavy object to the end of some string and the other end of the string to the negative end of the LED strip. Drop the object attached to the string through the tube and start to pull through the LED strip.

13. Guide the end with the battery gently into the tube about 4 inches. You can use the 5/8 tubing to gently push from the spring side of the battery connector. Test the length by inserting a battery into the tube. You will need to leave room for the negative side to be inserted with room for the push button.

14. Locate the nut on the inner tubing and drill a hole on the outer tubing to line up with it and thread the M3x8mm thread into place. Congrats, the positive end of the hoop is complete!

### Final Assembly (Negative End)

1. With the light strip sticking out of the negative end, you will want to insert the 5/8 inner tubing most of the way into the outer tubing but do not push it all the way in! Leave room to pull it back out (using plyers if necessary).

2. Solder on the negative battery piece to the GND end of the light strip, as close as possible. Lightly hot glue the connection to strengthen it.

3. Measure about a half an inch from the end and drill a hole on the side of the tube big enough for the head of a push button to easily slide through. Insert the push button underneath the LED strip, fully inside the hoop and line up the head so that it pops out of the hole.

4. Pull the inner tubing back out into place resting against the battery connector.


5. Insert a battery into the hoop and connect the negative end in place. It should light up and work. DO NOT DISCONNECT YET! Carefully drill through the outer and inner tubing and insert a rivet to secure the negative end together.

6. Mark where the push button sits on the other tubing. Disconnect and drill a hole so that the rivet now goes through both tubes.

7. Play with an enjoy your hoop! If you have any suggestions on how to improve this process, please submit back your ideas! If you want to modify the code, please contribute back! Happy hooping!
