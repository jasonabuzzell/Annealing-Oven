# Annealing Oven
 
These files are meant to construct an Annealing Oven and replicate the heating cycles using an Arduino microprocessor. The main.ino file (written in C/C++) has been tested specifically for Arduino Uno R3, and the .stl files were 3D printed with the Creality CR-6 SE with ABS filament. There are two diagrams also provided, one detailing the circuitry, and the other showcasing how the parts fit together.

In conjunction with an Arduino device, the user will also need 3 buttons, two 5.1k Ω Resistors, a 16x2 LCD, 4 heating pads, and a temperature sensor (I use a DHT22). There is also the option to use a passive speaker and LED, but is not required for functionality. The temperature sensor also determines how high one can set their peak heating temperature (e.g. DHT22 can measure up to 125°C accurately). The material of the annealing oven also determines the maximum temperature, so ABS is actually not recommended, as it has a glass transition temperature (Tg) of 105°C and will start to deform at higher temperatures (albeit, microscopically). If possible, using heat-resistant material (e.g. epoxy resin, bricks, polycarbonate) would be ideal, as the annealing oven only requires that proper heating occur on both sides of the z-axis (i.e. underneath and above the object).

These files were the result of a thought-experiment on annealing PMMA, a transparent material used for optical purposes, such as contact lenses or aquarium "glass", but most commonly known as "plexiglass". It is often used as a substitute for glass because of its high tensile strength, modular yield and shatter-proof qualities. 

In my case, I was interested in 3D printing lenses for glasses, where PMMA was an option as a 3D printable filament that had a much lower extrusion heating temperature and better adhesion capabilities than polycarbonate, the material used for many glasses lenses. However, upon printing it, I found that it was unclear due to its material layering, and looked into ways to make the PMMA lens optically transparent.

Annealing was one method, which could use the glass transition temperature of PMMA to restructure the object and fix the material layering, thus making it optically transparent. This method is still being tested, but likely will not work for a multitude of reasons. The first is that because the oven is printed in ABS and thus cannot go higher than 105°C, I have to hope that the tacticity of PMMA is either syndio- or iso-tactic and thus has a glass transition temperature of 105°C (pure syndiotactic PMMA has a Tg generally around 85°C). This is unlikely the case. On the other hand, the surface needs to extremely smooth to be truly transparent, which requires meticulously polishing and may only be achieveable by machines. It is likely that the Anneailng Oven won't be able to replicate this.

Molding is another favorable approach, because material layering will not be an issue. In fact, optically clear PMMA typically undergo this procedure through injection molding, which is a machine that melts and distribute PMMA polymers into a mold. I have my own ideas on how to approach this method that is mostly done by machines, but that is unrelated to the Annealing Oven.

For me, the takeaway from this (in the case that it doesn't work) is that it was an interesting experiment that resulted in learning more about C++ programming and electrical engineering, which is also well worth it for someone with zero prior EE experience and looking into coding opportunities!
