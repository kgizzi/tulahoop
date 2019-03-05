tulahoop
=============

**The world needs more smart LED hula hoops!**
 This guide is intended for do-it-yourselfers and *not* for profit use. The goal is to create an open source #DIYSMARTHOOP community.
 I hope this guide and code is a good starting place for those interested in making smart LED hula hoops.
 If you use this project and improve the process or code, please contribute back so we can continue to grow this open source smart flow community.
 Putting an LED hoop together is not going to be an easy or straight forward task, but hopefully this guide will contain enough information to help you do it.

  **DISCLAIMER**: This is not an easy, quick, or cheap project. You will likely need a friend to help you during assembly. It takes a certain level of know-how and persistence with trial and error to complete this project.


The capabilities of this hoop are:
* Two modes:
  * Image mode: 70+ bitmap images
  * Pattern mode: Change background color, number of chasers, and chaser color
* Images displayed with persistence of vision (POV)
* IR Remote controls

View a demo video of the hoop here: https://youtu.be/MMnCFZ5w8NA

# Hoop Components

## Electrical Materials
* Adafruit Trinket M0 (https://learn.adafruit.com/adafruit-trinket-m0-circuitpython-arduino)
* APA102 DotStar LEDs (60/m) (https://www.adafruit.com/product/2240)
* IR Sensor (https://www.adafruit.com/product/157)
* Adafruit Remote Control (https://www.adafruit.com/product/389)
* 3.7v 2300mah 14500 li-ion rechargeable batteries (look up GTL brand on ebay)
* 6.3v 1000uF capacitor

## Hoop Materials
* 3/4 inch Polypro tubing
* 5/8 inch polypro tubing (for connector)
* Springs for the battery
* Push button (https://www.hoopologie.com/small-snap-buttons)
* M3x3mm thread insert nuts (https://www.amazon.com/100pcs-Thread-Insert-Threaded-Inserts/dp/B07K1SJN8B/ref=sr_1_8?keywords=M3x3mm+threaded+nut+insert&qid=1551748254&s=gateway&sr=8-8)
* M3x8mm hex socket screws (https://www.amazon.com/gp/product/B01AXVSKUM/ref=ppx_yo_dt_b_asin_title_o00_s00?ie=UTF8&psc=1)

## Assembly Tools
* Soldering Iron & Solder
* 18 Gauge stranded wire
* Helping Hands (optional)
* 2.5mm heat shrink tubing
* Heat gun
* Rivet gun & Rivets
* Hot glue gun
* Drill with bits
* Dremel
* Plyers
* Electrical tape
* Engineering skills and/or ultra craftiness
* A hooper with hoop making experience
* Patience

# Hoop Assembly

Please see [ASSEMBLY.md](https://github.com/kgizzi/tulahoop/blob/master/ASSEMBLY.md)

## Instructions

### Steps with Images
### Soldering
### Assembling

### How to Load Bitmap Images

To convert bitmap image files to header files to program onto the hoop, you can use a python script borrowed from FTOLED and a custom helper script to automate mass image conversion.

#### Convert Images

Run these commands (must be in scripts directory):
```
cd scripts/
./convert.sh /path/to/bitmap/images
```

#### Add Images to Code

The image header files will be copied to the `src/images/` directory and the script output will give you what you need to change the images on the hoop. It will require manual modification to `src/images.h`.

### PlatformIO

This project uses PlatformIO to program the board. Please see https://platformio.org/ for instructions on how to install and use the software. Some basic instructions below:

### Programming

#### Setup:
Install FastLED library:
```
platformio lib install 126
```

#### Compile:
```
platformio run
```

#### Upload:
```
platformio run --target upload
```

#### Debug:
Uncomment ``#define HOOP_DEBUG``.
```
platformio device monitor
```
Some basic troubleshooting is output over USB.

### Using the Hoop / Remote Controls

#### Modes

##### Image Mode

This mode can play bitmap images which are converted from file format to a C Header file and included in the code.

###### Image Mode Controls

| Button(s) | Action |
| --------- |------- |
| Right / Left | Change image pattern |
| Play (Pause) | Toggle auto cycle mode |
| 1 | Auto cycle time of 2 seconds |
| 2 | Auto cycle time of 5 seconds |
| 3 | Auto cycle time of 10 & 30 seconds |

##### Pattern Mode

This mode is a background color with a configurable chaser overlay.

###### Pattern Mode Controls
| Button(s) | Action |
| --------- |------- |
| Right / Left | Change background color |
| Up / Down | Add/remove chasers |
| 1 | Change color of chaser |

##### Brightness Control

| Button(s) | Action |
| --------- |------- |
| Vol Up/Down | Adjust brightness |

##### Switch Modes

| Button(s) | Action |
| --------- |------- |
| Mode + 1 | Use pattern mode |
| Mode + 2 | Use image mode (default) |

# Credits

## FastLED

FastLED is an amazing LED library that makes it very easy to switch out light strips if you have NeoPixels per say, but the quality may not be as good.
Check out the FastLED project here: https://github.com/FastLED/FastLED

## IRLib2

An IR remote library for SAMD21 board such as the Adafruit Trinket M0 used in this project: https://github.com/cyborg5/IRLib2

## FTOLED

I found this library and modified their bitmap reading code to display the bitmap line by line as to create the persistence of vision (POV) effect: https://github.com/freetronics/FTOLED

## Hyperion Hoops

The images which I've chosen to use on this hoop are from the freely available pattern pack by Hyperion Hoops, downloadable from their website: https://www.hyperionhoop.com/shop/mtsp.php?paID=18

If you have read this guide and determined that making an LED hoop isn't for you and you would rather purchase one instead, take a look at what Hyperion has to offer. They are top of the line, with tons of features and a warranty.

## Tula Hula

This hoop and code would not be possible if it weren't for the badass hooper and hoop maker, Tula Hula :purple_heart:.
