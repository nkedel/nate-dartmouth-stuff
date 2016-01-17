#! wish
#######

# $Header: /usr/tahoe1/davidk/KillerS/Global/RCS/canvas.tcl,v 1.10 1996/05/21 20:58:21 mihai Exp mihai $

# Modification History:
#   Created by Mihai Banulescu on April 17, 1996
#   Modified by Mihai Banulescu on April 17, 1996 with assistance by Fred Henle
#   Modified by David Kung on April 17, 1996 for integration with UI
#   Modified by Mihai Banulescu on April 18-19, 1996

# We have an array that uses window numbers as indices and contains the following
#   array(number,name)  -->  the pathname itself, example: .d.c.f4 
#   array(number,num)  -->  the number of the window, example:  4
#   array(number,x)  -->  x position, used for moving and resizing
#   array(number,y)  -->  y position, used for moving and resizing
#   array(number,id)  -->  the ID of the window canvas item created
#   array(number,min)  -->  is the window minimized? (part of the "WindowShade" feature)
#   array(number,color)  -->  color of the window, picked randomly from among 90
#   array(number,w)  -->  height, used for resizing and display within the frame
#   array(number,h)  -->  width, used for resizing and display within the frame



###################
###  VARIABLES  ###
###################
set wc 0
set active none
set absw [winfo screenwidth .]
set absh [winfo screenheight .]

set i 0
foreach col {black DarkSlateGray SlateGray MidnightBlue DarkSlateBlue DarkSlateBlue darkBlue SteelBlue darkCyan darkCyan CadetBlue DarkGreen DarkOliveGreen DarkSeaGreen SeaGreen MediumSeaGreen LightSeaGreen ForestGreen ForestGreen OliveDrab goldenrod DarkGoldenrod DarkGoldenrod RosyBrown IndianRed SaddleBrown SaddleBrown sienna peru burlywood SandyBrown tan chocolate firebrick firebrick DarkSalmon orange coral LightCoral tomato OrangeRed red darkRed HotPink DeepPink PaleVioletRed maroon MediumVioletRed VioletRed darkMagenta snow4 seashell4 PeachPuff3 NavajoWhite3 NavajoWhite4 azure4 SlateBlue4 RoyalBlue4 RoyalBlue4 blue4 DodgerBlue4 SteelBlue4 DeepSkyBlue4 SkyBlue4 turquoise4 turquoise4 cyan4 DarkSlateGray4 aquamarine4 DarkSeaGreen4 SeaGreen4 PaleGreen4 SpringGreen4 green4 chartreuse4 OliveDrab4 DarkOliveGreen4 yellow4 DarkGoldenrod1 DarkGoldenrod2 DarkGoldenrod3 DarkGoldenrod4 IndianRed3 IndianRed4 firebrick2 firebrick3 firebrick4 brown4 orange3 orange4 DarkOrange1 DeepPink4 magenta4 magenta4 orchid4 plum4 MediumOrchid4 DarkOrchid4 purple4 MediumPurple4} {
    set colors($i) $col
    incr i
}

    set catstevens "It's not time to make a change,
Just relax, take it easy,
You're still young, that's your fault,
There's so much you have to go through...

Find a girl, settle down,
If you want you can marry,
Look at me, I am old
But I'm happy.

How can I try to explain?  When I do he turns away again, it's always been the same, same old story..."





####################
####  GRAPHICS  ####
####################

wm title . "Killer Software Editor"
. config -width $absw -height $absh -relief ridge -bg blue
pack propagate . false
raise .


####  MENU  ####
frame .menu -bg grey -relief ridge -bd 3

menubutton .menu.file -text File -underline 0 -menu .menu.file.menu
menubutton .menu.edit -text Edit -underline 0 -menu .menu.edit.menu
menubutton .menu.tools -text Tools -underline 0 -menu .menu.tools.menu
menubutton .menu.buffers -text Buffers -underline 0 -menu .menu.buffers.menu
menubutton .menu.window -text Window -underline 0 -menu .menu.window.menu
menubutton .menu.help -text Help -underline 0 -menu .menu.help.menu

menu .menu.file.menu
menu .menu.edit.menu
menu .menu.tools.menu
menu .menu.buffers.menu
menu .menu.window.menu
menu .menu.help.menu

.menu.file.menu add command -label "Open a new window" -underline 0 \
    -command {PrintMessage "Open a new window"; MakeWindow}
.menu.file.menu add command -label "Load file" -underline 0 \
    -command {PrintMessage "Load file"}
.menu.file.menu add command -label "Insert File" -underline 0 \
    -command {PrintMessage "Insert File"}
.menu.file.menu add command -label "Save" -underline 0 \
    -command {PrintMessage "Save"}
.menu.file.menu add command -label "Save as" -underline 0 \
    -command {PrintMessage "Save as"}
.menu.file.menu add command -label "Delete Buffer" -underline 0 \
    -command {PrintMessage "Delete Buffer"}
.menu.file.menu add command -label "Quit" -underline 0 \
    -command exit

.menu.edit.menu add command -label "UNDO" -underline 0 \
    -accelerator "Ctrl+z" -command {PrintMessage "UNDO"}
.menu.edit.menu add command -label "Redo" -underline 0 \
    -accelerator "Ctrl+r" -command {PrintMessage "Redo"}
.menu.edit.menu add command -label "Cut" -underline 0 \
    -command  {PrintMessage "Cut"}
.menu.edit.menu add command -label "Copy" -underline 0 \
    -accelerator "Alt+c" -command {PrintMessage "Copy"}
.menu.edit.menu add command -label "Paste" -underline 0 \
    -accelerator "Alt+v" -command {PrintMessage "Paste"}
.menu.edit.menu add command -label "Clear Selection" -underline 0 \
    -command {PrintMessage "Clear"}
.menu.edit.menu add command -label "Select All" -underline 0 \
    -command {PrintMessage "Select All"}
.menu.edit.menu add command -label "Regular Search" -underline 0 \
    -command {PrintMessage "Regular Search"}
.menu.edit.menu add command -label "Regular Search and Replace" -underline 0 \
    -command {PrintMessage "Regular Search and Replace"}

.menu.tools.menu add cascade -label "TCL" -menu .menu.tools.menu.tcl
.menu.tools.menu add cascade -label "Scheme" -menu .menu.tools.menu.scheme
.menu.tools.menu add command -label "UNIX Shell" \
    -command {PrintMessage "UNIX Shell"}
.menu.tools.menu add command -label "Parse Line(Tab)" \
    -command {PrintMessage "Parse Line"}
.menu.tools.menu add command -label "Parse Entire Text" \
    -command {PrintMessage "Parse Entire Text"}

menu .menu.tools.menu.tcl
menu .menu.tools.menu.scheme

.menu.tools.menu.tcl add command -label "Execute Buffer" \
    -command {PrintMessage ""}
.menu.tools.menu.tcl add command -label "Execute Selection" \
    -command {PrintMessage "Execute Selection" }
.menu.tools.menu.tcl add command -label "Interactive Window" \
    -command {PrintMessage "Run tcl Fil"}


.menu.tools.menu.scheme add command -label "Execute Buffer" \
    -command {PrintMessage "Run scheme File"}
.menu.tools.menu.scheme add command -label "Execute Selection" \
    -command {PrintMessage "Run part of scheme file"}
.menu.tools.menu.scheme add command -label "Interactive Window" \
    -command {PrintMessage "Run scheme Cmd"}

.menu.buffers.menu add command -label "<List of active buffers>" \
    -command {PrintMessage "<List of active buffers>"}
.menu.window.menu add command -label "<List of active windows>" \
    -command {PrintMessage "<List of active windows>" }
.menu.window.menu add command -label "Close window" \
    -command {PrintMessage "Close window"}
         
pack .menu.file .menu.edit .menu.tools .menu.buffers .menu.window -side left
pack .menu.help -side right

pack .menu -fill x -side top


####  TOOLBAR  ####
frame .t -bg grey  -relief ridge -bd 3
button .t.toolbar -text "Hello.  I am a toolbar. But you can't click me yet..." \
    -command exit
pack .t.toolbar -side left
pack .t -fill x

set text2 [set text3 [set text4 666]]
button .t.b1 -textvariable text1
button .t.b2 -textvariable text2
button .t.b3 -textvariable text3
button .t.b4 -textvariable text4
pack .t.b1 .t.b2 .t.b3 .t.b4 -side left -expand true


####  DESKTOP  ####
frame .d -bg ivory
canvas .d.c -width $absw -bg darkseagreen2 -closeenough 20.0\
    -scrollregion {0 0 2000 1500} \
    -xscrollcommand [list .d.xscroll set] \
    -yscrollcommand [list .d.yscroll set]
scrollbar .d.xscroll -orient horizontal \
    -command [list .d.c xview]
scrollbar .d.yscroll -orient vertical \
    -command [list .d.c yview]
pack .d.xscroll -side bottom -fill x
pack .d.yscroll -side right -fill y
pack .d.c -side left -fill both -expand true
pack .d -side top -fill both -expand true


####  MESSAGE WINDOW  ####
frame .mw -bg ivory -relief ridge -bd 3
set mes "And I am a message window...  This is my message to you!"
label .mw.message -textvariable mes -width $absw
pack .mw.message -fill x -side left -anchor w
pack .mw -fill x





#####################
####  FUNCTIONS  ####
#####################
# Next two functions taken from Welch, page 52
# They intialize and respectively return a random number between 0 and 100
proc randomize {seed} {
    global rand

    set rand(ia) 9301;# Multiplier
    set rand(ic) 49297;# Constant
    set rand(im) 233280;# Divisor
    set rand(seed) $seed;# Last result
}


proc random {range} {
    global rand
   
    set rand(seed) [expr ($rand(seed)*$rand(ia) + $rand(ic)) % $rand(im)]
    return [expr int([expr $rand(seed)/double($rand(im))]*$range)]
}

randomize [pid]


# This function creates a new window
proc MakeWindow {} {
    global wc active array text1 text2 text3 text4 rand colors catstevens

    set w [frame .d.c.w$wc]
    set f [frame .d.c.w$wc.f$wc]

    set array($f,w) 600
    set array($f,h) 400
    set array($f,num) $wc
    set array($f,name) .d.c.w$wc.f$wc
    set array($f,min) 0
    set array($f,color) $colors([random 100])

    $f configure -height $array($f,h) -width $array($f,w) -bg $array($f,color)
    pack $f
    pack propagate $f false

    set text1 $array($f,color)

    after 200
    set where [expr 25 * $wc]
    set array($f,id) [.d.c create window $where $where -anchor nw -tag movable -window $w]

    # Create the upper (beginning) label and button bar
    label $f.b -textvariable array($f,name) -bg $array($f,color) -fg ivory -height 2
    button $f.b.q -text O -bg $array($f,color) -fg ivory -relief raised \
	-command {PrintMessage "CLOSE THE CURRENT WINDOW"}
    button $f.b.x -text X -bg $array($f,color) -fg ivory -relief raised \
	-command {PrintMessage "I DON'T KNOW WHAT THIS DOES YET"}

    pack $f.b.q -side left
    pack $f.b.x -side right
    pack $f.b -side top -fill x -expand true
    pack propagate $f.b false

    # Create the text widget
    text $f.t -relief flat -bg ivory
    $f.t insert insert $catstevens

    # Create the lower (end) label and button bar
    frame $f.e -bg $array($f,color)
    label $f.e.wl -text "Width:" -bg $array($f,color) -fg ivory
    entry $f.e.we -textvariable array($f,w) -width 4 -bg ivory -fg $array($f,color)
    label $f.e.hl -text "Height:" -bg $array($f,color) -fg ivory
    entry $f.e.he -textvariable array($f,h) -width 4 -bg ivory -fg $array($f,color)
    label $f.e.space -text "  " -bg $array($f,color)
    label $f.e.b -bg $array($f,color) -fg ivory -relief sunken -width 2 -bd 4

    pack $f.e.b -side right -anchor se
    pack $f.e.space -side right
    pack $f.e.he -side right -padx 2 -pady 4
    pack $f.e.hl -side right
    pack $f.e.we -side right -padx 2 -pady 4
    pack $f.e.wl -side right
    pack $f.e -side bottom -fill x -anchor e

    pack $f.t -fill both -expand true -padx 5


    ####  BINDINGS  ####
    bind $f.t <Button-1> {
	set active [string trimright %W ".t"]
	raise $active
    }

    bind $f.b <Button-1> {
	set active [string trimright %W ".b"]

	MarkThis %X %Y $active
	focus [string trimright %W ".b"].t

	set active [string trimright $active $array($active,num)]
	raise [string trimright $active .f]
    }

    bind $f.b.q <Button-1> {
	set active [string trimright %W ".b.q"]
 	set active [string trimright $active $array($active,num)]
	destroy [string trimright $active .f]
	break 
    }

    bind $f.b.x <Button-1> {
	set active [string trimright %W ".b.x"]
	focus [string trimright append %W .b.x].t
	raise $active
    }

    bind $f.b <B1-Motion> {
	set active [string trimright %W ".b"]
	MoveThis %X %Y $active
    }

    bind $f.b <Button-2> {
	set active [string trimright %W ".b"]
	if {$array($active,min) == 0} {
	    pack propagate $active.b false
	    pack forget $active.t
	    pack forget $active.e
	    set array($active,min) 1
	} else {
	    pack propagate $active.b true
	    pack $active.e -fill x -side bottom -expand true
	    pack $active.t -fill both -expand true -padx 5
	    set array($active,min) 0
	}
    }

    bind $f <Button-1> {
	set active [string trimright %W ".b"]
	focus $active.t
	raise $active
	MarkThis %X %Y $active
    }

    bind $f <B1-Motion> {
	set active [string trimright %W ".b"]
	MoveThis %X %Y $active
    }


#    set text1 [winfo reqwidth $f]
    set text2 [winfo reqheight $f]

    tkwait visibility $f
    set text3 [winfo width $f]
    set text4 [winfo height $f]


    update idletasks

    incr wc
}

# Helper function, remembers an (x, y) position to allow moving a window
proc MarkThis {x y w} {
    global array active

    set array($w,x) $x
    set array($w,y) $y
}

# Helper function, moves a window by following the mouse
proc MoveThis {x y w} {
    global array active

    set dx [expr $x - $array($w,x)]
    set dy [expr $y - $array($w,y)]
    set array($w,x) $x
    set array($w,y) $y
    .d.c move $array($w,id) $dx $dy
}

# Helper function, displays messages on the message bar
proc PrintMessage var {
    global mes

    set mes $var
}



for {set i 0} {$i < 5} {incr i} {
    MakeWindow
    update idletasks
}

