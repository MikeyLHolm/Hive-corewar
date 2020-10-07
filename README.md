# Hive-corewar
 In this project, you will create a virtual “arena” in which programs will fight against one another (the “Champions”). You will also create an assembler to compile those Champions as well as a Champion to show the world that you can create life from coffee.
 
 This is the final project of the algorithm branch in Hive's Foundation part of studies. Onwards to Internships and Mastery studies...
 
![Visualizer](https://github.com/MikeyLHolm/Hive-corewar/blob/master/cw.gif)


## Objectives
• **The assembler:** this is the program that will compile your champions and trans- late them from the language you will write them in (assembly language) into “Byte- code”.Bytecode is a machine code, which will be directly interpreted by the virtual machine.

• **The virtual machine:** It’s the “arena” in which your champions will be executed. It offers various functions, all of which will be useful for the battle of the champions. Obviously, the virtual machine should allow for numerous simultaneous processes; we are asking you for a gladiator fight, not a one-man show simulator.

• **The champion**: Your current champion will only need to prove to us that you can write bits and pieces of Corewar ASM. This means that the champion you should render for this project only needs to scare the bejesus out of a neurasthenic hedgehog.

## Limitations
This project has to be completed respecting our schools norme. Here us some of the rules:
```
◦ Each function must be maximum 25 lines, not counting the function’s own curly brackets.
◦ Each line must be at most 80 columns wide, comments included.
◦ One instruction per line.
◦ One single variable declaration per line.
◦ No declaration and an initialisation on the same line.
◦ A function can take 4 named parameters maximum.
◦ You can’t declare more than 5 variables per function.
```
Within the mandatory part, you are allowed to use only the following libc functions:
```
◦ close
◦ exit
◦ free
◦ lseek
◦ malloc
◦ open
◦ perror
◦ read
◦ realloc
◦ strerror
◦ write
```

## Usage

**Install**
```
git clone https://github.com/MikeyLHolm/Hive-corewar.git
cd Hive-corewar
make
```
**Assembler**
```
./asm [filename.s]
```
**Virtual Machine**
```
./corewar [-a -f -l -v -z -x (-dump n) (-s n) (-n n)] [filename.cor] ([filename.cor] [filename.cor])

-a:        Show advanced visualizer
-f:        Fix lld instruction bug
-l:        Print lives to standard output
-v:        Show visualizer
-z:        Print aff instructions to standard output
-x:        Show leaks after program terminates
-dump n:   Dump memory at cycle n
-s n:      Start advanced visualizer at cycle n
-n n:      Set player number
```
**Disassembler**
```
./disasm [filename.s]
```

## Grade

**125 / 100**


_Completed: October 1st 2020_
