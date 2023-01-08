# DIY-Window-Heart

## Goal
Inspired by the glowing hearts that you may see in the window sill as you stroll through the city, I decided to take my own shot at designing a fully customizable window sill heart.

## Materials & Equipment
- Arduino Micro
- Flexible Silicone Neon-like Skinny NeoPixel LED Strip
- 9V 36W Barrel Jack Power Supply
- 5V 3A Step Down Buck Converter
- 1000µF Capacitor
- 470Ω Resistors
- Barrel Jack 2-terminal adaptor
- Jumper wires
- Push buttons
- PLA Filament
- 3D Printer
- Superglue

## Step 1: The Heart
First, I had to create a heart in which to mount the LED strip. Using Fusion 360, I designed a heart with a suitably designed channel cut through it such that you can mount the strip so snugly that glue is not required. I had to be careful to make sure the dip in the heart would not bend the strip so hard that it would break, and I had to make sure that the perimeter of the heart was sized perfectly so that both ends of the LED strip would be invisible in the base of the heart. 

<img width="802" alt="image" src="https://user-images.githubusercontent.com/76706672/211223073-281cbc3d-8e4f-4f19-a0c3-0462646345d9.png">

Next, I had to design the base and its cover. I had hoped that the little clips I designed for the plate to cover up the bottom would hold, but they immediately snapped off after printing -- it's okay though, they double as ventilation for the Arduino! I made it with two holes at the front for buttons, a hole at the back for the barrel jack, and three rectangluar holes at the top -- two for fitting the ends of the LED strip through, and the other for an insert I made to help support the heart and keep it from snapping off the base. The base, the cover for the base, and the insert are all shown below.

<img width="990" alt="Screen Shot 2023-01-08 at 5 58 17 PM" src="https://user-images.githubusercontent.com/76706672/211223278-2c3efcd1-838e-4a81-b100-f38dc5af6a01.png">
<img width="1033" alt="Screen Shot 2023-01-08 at 5 58 39 PM" src="https://user-images.githubusercontent.com/76706672/211223288-df36e450-eabb-4a18-b889-e157ff99b429.png">
<img width="704" alt="Screen Shot 2023-01-08 at 5 59 07 PM" src="https://user-images.githubusercontent.com/76706672/211223304-37b48a14-53c6-42e3-b1f4-94d5091e5b84.png">

Finally, the heart section was too large to fit onto the printbed of my Ender V2 printer, so I sliced it up into four suitable chunks, and then superglued all the pieces together. If I were to do this again, I would slice it so that the glue joints are not on peak areas of stress -- for example, the LED strip wants to rip the pointy bit in the middle of the heart apart, since its flexed there. I would rotate all of the cut points about 45 degrees in a circle on a second attempt. In the photo below, there are two gaps at the side joints -- I fixed that later, by re-printing those pieces. I messed up when I put them in my STL slicer, and cut off a little bit -- the CAD models themselves were fine.

![image](https://user-images.githubusercontent.com/76706672/211223498-a5db3010-8104-45df-9a1f-dd2f33337c25.png)

Then, I superglued the base on, put the LED strip in, and got this!

![image](https://user-images.githubusercontent.com/76706672/211223632-dfb6b8a7-351c-499e-9915-eb4e1ba9b51b.png)

## Step 2: The Electronics

Next, I had to figure out the circuit. I knew I wanted two buttons -- one for changing light modes, another for changing the color within said modes. This is also a gift for my girlfriend, and I want to Rick-Roll her every time she plugs it in. So, I added a piezo electric speaker and I have it play Never Gonna Give You Up every time it boots. A rudimentary diagram of the circuit came out as such:

<img width="925" alt="image" src="https://user-images.githubusercontent.com/76706672/211224327-2edc59fc-713f-476a-b391-cb586a742fe3.png">

I wired this up without the buttons and uploaded some example code from Adafruit. Look, mom, it lights up!

<img width="753" alt="Screen Shot 2023-01-08 at 6 31 49 PM" src="https://user-images.githubusercontent.com/76706672/211224443-090c3a6f-22dd-465b-9d21-23ca99ba6829.png">

I then mounted it into the heart.

![image](https://user-images.githubusercontent.com/76706672/211224785-e703d80a-c0d3-4c7b-ad41-9838d1c597cd.png)

### Step 3: The Code

I knew that I wanted to program this as an FSM. I have 6 main states/modes:

1. Startup (this rickrolls the user)
2. Solid (solid LED colour)
3. Pulse (sends a pulse of light around the heart on either side, starting from the middle at the top)
4. Breathe (fades the light on and off, as if it were breathing)
5. Rainbow Wave (fades a rainbow seamlessly around the led strip)
6. Theater Chase (old style lights chasing around in a circle, but rainbow)

Within states 1-4, the user can press the colour button in order to change the colour used by the effect. Within all the states, the user can press the mode button to change the current mode.

My full code is provided in this repository, and I designed the modes myself, with the exception of the rainbow and theater chase -- Adafruit helped with that -- they were too beautiful to leave out. I did however, have to tweak both effects to work for this project and respond to user input.

Everything works, however I broke some buttons trying to assemble it, so I don't have any videos of me switching modes and colours. New ones are in the mail, and I will try to remember to update this when I get them!




