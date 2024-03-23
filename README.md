# Snake-Game
##### DESCRIPTION
Just a regular snake game. The users can use buttons to move the snake and monitor the game using four 1088BS 8x8 LED displays (controlled by MAX7219 LED Driver ICs). If the snake’s head touches any of the snake’s body, the game restarts. The food is repopulated at random locations. The body of the snake increases by one node if the snake’s head touches the food piece. The complete version of the snake game would have incorporated four MAX7219 displays and soldered onto a customized PCB. Below is how the project works on a single LED display hardware prototype. 

The final version of the project is not completed in time. The only function that is implemented is the movement of the snake and the user inputs. No death or food mechanism.

##### TIMELINE
3 Months (Jan 1, 2024 -- Mar 31, 2024). Including:
* Learning how to compile AVR code
* Learning pointers in C
* Learning AVR timer
* Learning SPI protocol
* Learning linked list data structure
* Develop MAX7219 driver
* Schematic Design and PCB Design
* Overall Coding

##### CODE EXPLANATION
**Structure**: The snake consists of the head and the rest of the body. For this reason, a double-linked list algorithm is used to implement the snake. Each node of the snake, including the head, is a structure with 4 data: pointer to the previous node, pointer to the next node, row location, and column location. 

**User Control**: Once the user presses the button for one of the 4 directions, the head of the snake changes direction. Once the head changes, function update_node() is called to update the position of the previous nodes to the position of the nodes that come before them. 

![image](https://github.com/JimpanzeeArchived/Image/assets/154708517/be039b9f-21d2-44cf-99ea-ef47094d92f7)

##### HARDWARE
* 4 x MAX7219 LED Driver ICs
* 4 x 1088BS 8x8 LED Displays
* 1 x Atmega328P
* Capacitors and Resistors
* Final PCB

##### FUTURE IMPROVEMENT
**Begin Project With The Hardware:**
* Too much effort is wasted developing a prototype instead of just building the final hardware.
* Much time is spent wiring the prototype using *physical wire* instead of just making a PCB.
* All the software is developed for the prototype, which burns much of the time that could be used for the final hardware.
* The PCB design should have been the first thing to do. Since I now know how to make a mediocre PCB (ish), future projects will begin with PCB design.

**Cleaner Code:**
* Much of the time is wasted due to bugs that are caused by unorganized code.
* The logic of the code could be more well-planned.

**Memory Management:**
* Not a lot of thoughts are given to the memory allocation on the IC (both flash memory and stack memory). Only when the programmer reported that 3K bytes of flash were being loaded was I concerned about memory size.
* I use “INT” when declaring most of my variables. Each “INT” variable takes up 4 bytes of memory, which is overkill for what it is used for.
* In the future, I will use the appropriate declaration such as UINT16_T or UINT8_T to avoid wasting memory space.









