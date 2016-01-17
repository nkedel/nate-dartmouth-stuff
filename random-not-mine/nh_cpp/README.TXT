This is a very basic skeleton for a C++ implementation of nethack 3.1.3.
I started this as a learning exercise, and quickly ran out of steam. The
infamous DEV TEAM was not interested in my work, nor in pursuing a C++ 
implementation of nethack, even though I think it would help tremendously.

If working on this seems to be a fun idea, let me know and I might help.
If you use any of this source code, be sure to credit me and make the
proper arrangements to send checks. :)

If you don't understand something email me and I might help.

Oh,  the usual "this has no warranty" legal crap applies. Its been a while
since I've even worked on it. I think it compiles at the moment. 

It was written for DJGPP 112m3 (the MSDOS port of GCC 2.63). ANSI.SYS must be
loaded because all the screen displays use it. I never did have time to 
figure out that termcap voodoo crap. Asside from ANSI specific stuff, which I 
tried to put into seperate files, it should compile on any C++ compiler on
any system. 

Some cool things to do that I also never had time for:

    Make a 'message' stream that behaves like 'cout' but pages each line 
    for the top display.

    Implement line-of-sight algorithms.

    Fix the memory leaks.
    
    Finish it.


Yes, I'm in a sour mood.
Bill Foust

Addresses listed in order or preference. I'm a consultant, so I won't be at
Informix forever.

FoustWF@cs.rose-hulman.edu
Ozone@acca.nmsu.edu
Bfoust@Informix.com
