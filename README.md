# Welcome to Tetris_CPP

This is a passion project that I started in 2023 to get better at C++. I am a big fan of Tetris and wanted to make a version of the 
game to run on my computer. This could be an easy project if I didn't care about the quality of my game, but I wanted this to be a
perfect Tetris clone. Also, although there're many libraries and frameworks that could help, I wanted to do it with just the stardard
libraries, using the Windows API for rendering. All of this made this project a lot more difficult, but a lot more fun as well.

My final goal with the project is to have it mimic the original game perfectly, with some extra features that I might find useful, and
finally, create my own AI to play the game, preferably without external libraries. This is going to take a while, specially because I
am not fully focused on it at all times. But I will drop an update or a new release every now and then, when they're ready.

The main branch has the latest stable version that I managed to compile. Untested has things that I coded, but haven't tested, it's 
mainly for storing code that I made in computers other than my own, or features that aren't fully implemented.
I use Mingw-w64 to compile this code, the compiler command can be found in the top of the main.cpp file.

## Controls

Left/Right arrow - Move left and right

Down arrow - Soft Drop

Up arrow - Rotate Clockwise

X - Rotate Counterclockwise

Space - Hard Drop

C - Hold

## Remaking the game from scratch

Originally, my goal for this game was to just work on logic, to learn the language. As I became more experienced, I came to realize that most of my original code sucked. Even though I changed a lot of it over time, I feel like trying to fix my early mistakes is pointless.

It is mostly because I didn't think it would become this complicated, but now, any major change takes forever to implement, because I have to redesign a lot of stuff. The source code is not well written, and I think I can do much better now than when I started.

Therefore, I decided to nuke the entire code I have and start fresh. This will first be done in it's own branch (StartOver), and when it is close to what I have now, I'll pull it into main. Hope it will be better. :)
