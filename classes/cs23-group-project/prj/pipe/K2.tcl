#! ./kedit
#######

# $Header: /usr/tahoe1/davidk/KillerS/Integration/RCS/K2.tcl,v 3.13 1996/05/29 00:38:19 mihai Exp davidk $

# KillerS.tcl

# Modification History:
#   Created by Mihai Banulescu on April 17, 1996
#   Modified by Mihai Banulescu on April 17, 1996 with assistance by Fred Henle
#   Modified by David Kung on April 17, 1996 for integration with UI
#   Modified by Mihai Banulescu on April 18-19, 1996
#   Modified extensively by Mihai Banulescu and Tim Lifshits for integration
#   Modified by David Kung on April 25, 1996 for integration purposes

# We have an WindowArray that uses window numbers as indices and contains the following
#   WindowArray(name,num)  -->  the number of the window, example:  4
#   WindowArray(name,x)  -->  x position, used for moving and resizing
#   WindowArray(name,y)  -->  y position, used for moving and resizing
#   WindowArray(name,id)  -->  the ID of the window canvas item created
#   WindowArray(name,min)  -->  is the window minimized? (part of the "WindowShade" feature)
#   WindowArray(name,color)  -->  color of the window, picked randomly from among 90
#   WindowArray(name,w)  -->  height, used for resizing and display within the frame
#   WindowArray(name,h)  -->  width, used for resizing and display within the frame

# We will also have an array that keeps whether the window is "alive" or not.
# That array will hold a "OPEN" or "DESTROYED" for each window number

exec clear

# Other TCL files necessary

source textfcn.tcl; # text subsystem
source editfcn.tcl; # edit subsystem
source BM.tcl; # the Buffer Manager
source dialog.tcl; # Dialog Box for opening
source parse.tcl; # the parsing functions
source colorpicker2.tcl; # Neil Miller's color choosing utility
source createmenu.tcl; # Moytra and Xiaohong's Tools 
source pipe.tcl; # Nate's Big Pipe functions
source search.tcl; # Gondek's S&R Sado-Relaxation

##########################################################################


###################
###  VARIABLES  ###
###################
set wc 0 ; # wc is the counter for the number of windows
set activeWindow "NOWINDOWS" ; # active window tracker: ESSENTIAL
set absw [expr int([winfo screenwidth .] * 1.0)]  ; # screen width
set absh [expr int([winfo screenheight .] * 0.8)] ; # screen height
set numUntitled 0; # keeps track of untitled windows


# the random color window generator
# creates an array of 100 colors
# a random number generator uses this array to color new windows, as well as old
set i 0
foreach col {black DarkSlateGray SlateGray MidnightBlue DarkSlateBlue DarkSlateBlue darkBlue SteelBlue darkCyan darkCyan CadetBlue DarkGreen DarkOliveGreen DarkSeaGreen SeaGreen MediumSeaGreen LightSeaGreen ForestGreen ForestGreen OliveDrab goldenrod DarkGoldenrod DarkGoldenrod RosyBrown IndianRed SaddleBrown SaddleBrown sienna peru burlywood SandyBrown tan chocolate firebrick firebrick DarkSalmon orange coral LightCoral tomato OrangeRed red darkRed HotPink DeepPink PaleVioletRed maroon MediumVioletRed VioletRed darkMagenta snow4 seashell4 PeachPuff3 NavajoWhite3 NavajoWhite4 azure4 SlateBlue4 RoyalBlue4 RoyalBlue4 blue4 DodgerBlue4 SteelBlue4 DeepSkyBlue4 SkyBlue4 turquoise4 turquoise4 cyan4 DarkSlateGray4 aquamarine4 DarkSeaGreen4 SeaGreen4 PaleGreen4 SpringGreen4 green4 chartreuse4 OliveDrab4 DarkOliveGreen4 yellow4 DarkGoldenrod1 DarkGoldenrod2 DarkGoldenrod3 DarkGoldenrod4 IndianRed3 IndianRed4 firebrick2 firebrick3 firebrick4 brown4 orange3 orange4 DarkOrange1 DeepPink4 magenta4 magenta4 orchid4 plum4 MediumOrchid4 DarkOrchid4 purple4 MediumPurple4} {
    set colors($i) $col
    incr i
}



####################
####  GRAPHICS  ####
####################

#### SETS THE WINDOW ####
wm title . "The Killer Editor"

# starting size
. config -width $absw -height $absh -relief ridge -bg blue -cursor draft_large
#wm minsize . [expr $absw / 2] [expr $absh / 2]
#wm maxsize . $absw $absh
wm geometry . [set absw]x$absh+0+0

pack propagate . false
raise .
update idletasks



####  MENU  ####
# creates the menu from the file createmenu.tcl
# toolbar is created from createmenu.tcl

frame .menu -bg grey -relief ridge -bd 3

UI_Create_Filemenu
UI_Create_Editmenu
UI_Create_Toolsmenu
UI_Create_Buffermenu
UI_Create_Windowmenu
UI_Create_Helpmenu

pack .menu.file .menu.edit .menu.tools .menu.buffers .menu.window -side left
pack .menu.help -side right
pack .menu -fill x -side top


####  TOOLBAR  ####
# created from createmenu.tcl

UI_Create_Toolbar


####  DESKTOP  ####
# Desktop frame .d and canvas frame .d.c. are the basis
# of the desktop that defines the Window Manager

frame .d -bg ivory
canvas .d.c -width $absw -bg darkseagreen2 -closeenough 20.0\
    -cursor pirate -scrollregion {0 0 5000 3000} \
    -xscrollincrement 5 -yscrollincrement 5 \
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
# Small Message label at the bottom of the screen
frame .mw -bg ivory -relief ridge -bd 3
set mes ""
label .mw.message -textvariable mes -width $absw
pack .mw.message -fill x -side left -anchor w
pack .mw -fill x
update idletasks
toplevel .welcome -width $absw -height $absh -bg black -cursor {watch yellow}

label .welcome.who -text "KILLER SOFTWARE" -bg black -fg red
wm geometry .welcome [set absw]x$absh+0+0
pack .welcome.who -anchor s
update idletasks
after 7000 destroy .welcome



#####################
####  FUNCTIONS  ####
#####################

# Next two functions taken from Welch, page 52
# They intialize and respectively return a random number between 0 and 100


# ---- randomize ----
# seeds the random number generator
proc randomize {seed} {
    global rand

    set rand(ia) 9301;# Multiplier
    set rand(ic) 49297;# Constant
    set rand(im) 233280;# Divisor
    set rand(seed) $seed;# Last result
}

randomize [pid]


# ---- random ----
# returns an integer between 0 and the given range
proc random {range} {
    global rand
   
    set rand(seed) [expr ($rand(seed)*$rand(ia) + $rand(ic)) % $rand(im)]
    return [expr int([expr $rand(seed)/double($rand(im))]*$range)]
}


# ---- NewColor ----
# Gets the next color for the window and its associated bar and buttons
proc NewColor {color} {
    global WindowArray activeWindow rand colors

    if {!($color == "NEWCOLOR")} {
	set WindowArray($activeWindow,color) $color
    } elseif {!($activeWindow == "NOWINDOWS")} {
	set WindowArray($activeWindow,color) $colors([random 100])
    } else {
	return
    }

    $activeWindow configure -bg $WindowArray($activeWindow,color)
    $activeWindow.b configure -bg $WindowArray($activeWindow,color)
    $activeWindow.b.q configure -bg $WindowArray($activeWindow,color)
    $activeWindow.b.x configure -bg $WindowArray($activeWindow,color)
    $activeWindow.e.wl configure -bg $WindowArray($activeWindow,color)
    $activeWindow.e.hl configure -bg $WindowArray($activeWindow,color)
    $activeWindow.e.b configure -bg $WindowArray($activeWindow,color)
    $activeWindow.e.space configure -bg $WindowArray($activeWindow,color)
    $activeWindow.e configure -bg $WindowArray($activeWindow,color)
    if {!($activeWindow == "NOWINDOWS")} {
	.d.xscroll config -troughcolor $WindowArray($activeWindow,color)
	.d.yscroll config -troughcolor $WindowArray($activeWindow,color)
    }
}

# ---- MakeWindow ----
# This function creates a new window for a buffer
# Essential creation function for the Window Manager
proc MakeWindow {buffernumber} {
    global wc WindowArray rand colors
    global activeWindow
    global BufferArray
    global statusArray

    set w [frame .d.c.w$wc]; # frame directly on canvas 
    set f [frame .d.c.w$wc.f$wc]; # frame within frame w, needed for resizing

    # Update the buffer array entry for the wndow in which the buffer is displayed
    set BufferArray($buffernumber,window) $f
    set WindowArray($f,w) 700; # original width
    set WindowArray($f,h) 500; # original height
    set WindowArray($f,num) $wc; # number of the window
    set WindowArray($f,min) "EXPANDED"
    set WindowArray($f,color) $colors([random 100])
    if {!($activeWindow == "NOWINDOWS")} {
	.d.xscroll config -troughcolor $WindowArray($activeWindow,color)
	.d.yscroll config -troughcolor $WindowArray($activeWindow,color)
    }
    set WindowArray($f,buf) $buffernumber
    set activeWindow $f
    set statusArray($wc) "OPEN"   

    $f configure -height $WindowArray($f,h) -width $WindowArray($f,w) -bg $WindowArray($f,color) -relief ridge -bd 5
    pack $f
    pack propagate $f false

    after 200
    set where [expr 25 * $wc]
    set WindowArray($f,id) [.d.c create window $where $where -anchor nw -tag movable -window $w]
    set WindowArray($f,x) $where
    set WindowArray($f,y) $where

    # Create the upper (beginning) label and button bar
    label $f.b -text $BufferArray($buffernumber,name) -bg $WindowArray($f,color) -fg ivory -height 2
    button $f.b.q -text O -bg $WindowArray($f,color) -fg ivory -relief raised \
	-cursor {target red} -command {PrintMessage "Bye-bye window"}
    button $f.b.x -text X -bg $WindowArray($f,color) -fg ivory -relief raised \
	-cursor {box_spiral red} -command {PrintMessage "Maximizing"}

    pack $f.b.q -side left
    pack $f.b.x -side right
    pack $f.b -side top -fill x

    # Create the text widget .d.c.w#.f#.m.w.t
    # pack .d.c.buffer# -in 
    frame $f.m -bg black
    frame $f.m.w -relief sunken
    .d.c.buffer$buffernumber configure -relief flat -bg ivory -setgrid true -yscrollcommand "$f.m.w.y set"
    # text $f.m.w.t -relief flat -bg ivory -setgrid true -yscrollcommand "$f.m.w.y set"
    scrollbar $f.m.w.y -orient vertical -command ".d.c.buffer$buffernumber yview"
    
    # Create the lower (end) label and button bar
    frame $f.e -bg $WindowArray($f,color)
    label $f.e.wl -text "Width:" -bg $WindowArray($f,color) -fg ivory
    entry $f.e.we -textvariable WindowArray($f,w) -width 4 -bg ivory -fg $WindowArray($f,color)
    label $f.e.hl -text "Height:" -bg $WindowArray($f,color) -fg ivory
    entry $f.e.he -textvariable WindowArray($f,h) -width 4 -bg ivory -fg $WindowArray($f,color)    
    label $f.e.space -text "  " -bg $WindowArray($f,color)
    label $f.e.b -bg $WindowArray($f,color) -fg ivory -relief sunken -width 2 -bd 4 -cursor sizing

    pack $f.e -side bottom -anchor se
    pack $f.e.b -side right -anchor s
    pack $f.e.space -side right
    pack $f.e.he -side right -padx 2 -pady 4
    pack $f.e.hl -side right
    pack $f.e.we -side right -padx 2 -pady 4
    pack $f.e.wl -side right

    pack $f.m -padx 5
    pack $f.m.w -padx 3 -pady 3
    pack $f.m.w.y -side right -fill y -expand true
    pack .d.c.buffer$buffernumber -in $f.m.w -side top -fill both -expand true    
    raise .d.c.buffer$buffernumber
    Update_Menubars

    ####  BINDINGS  ####
    # This binds a B1 mouse click on the text widget to raise the window
    # in which it is displayed to active status
    bind .d.c.buffer$buffernumber <Button-1> {
	set buffNum [string trimleft %W ".d.c.buffer"]
	BringToTop $BufferArray($buffNum,window)
    }

    # Bind the maximize button to raise the text widget
    # Maximize to be implemented
    bind $f.b.x <Button-1> {
	set activeWindow [string trimright %W ".b.x"]
	BringToTop $activeWindow
	set WindowArray($activeWindow,w) [winfo width .d.c]
	set WindowArray($activeWindow,h) [winfo height .d.c]
	$activeWindow configure -width $WindowArray($activeWindow,w) -height $WindowArray($activeWindow,h)
	puts [winfo x .d.c.w$WindowArray($activeWindow,num)]
	puts [winfo y .d.c.w$WindowArray($activeWindow,num)]
	.d.c coords $WindowArray($activeWindow,id) 0 0
	.d.c xview moveto 0; #[expr [winfo x .d.c.w$WindowArray($activeWindow,num)] / 5000]
	.d.c yview moveto 0; #[expr [winfo y .d.c.w$WindowArray($activeWindow,num)] / 3000]
	focus .d.c.buffer$WindowArray($activeWindow,buf)
    }

    # bind a B1 click on the top frame to raise the window as before
    # also mark the window for movement
    bind $f.b <Button-1> {
	set activeWindow [string trimright %W ".b"]
	MarkThis %X %Y
	BringToTop $activeWindow
	%W config -cursor fleur
    }

    bind $f.b <B1-Motion> {
	DragThis %X %Y
    }

    # Move the window to the new mouse location
    bind $f.b <ButtonRelease-1> {
	MoveThis %X %Y
	%W config -cursor pirate
    }
    
    # Bind the whole frame so user can drag using sides
    #Marks window for motion
    bind $f <Button-1> {
	set activeWindow %W
	MarkThis %X %Y
	BringToTop $activeWindow
	%W config -cursor fleur
     }

    bind $f <B1-Motion> {
	DragThis %X %Y
    }

    bind $f <ButtonRelease-1> {
	MoveThis %X %Y
	%W config -cursor pirate
    }

    # close window.  
    # !!!!Saving needs to ne taken care of here!!!!
    bind $f.b.q <Button-1> {
	set temp [string trimright %W ".b.q"]
	UI_CloseWindow $temp
	break
    }

    # Bind B2 click on the top of the frame to windowshade
    bind $f.b <Button-2> {
	set activeWindow [string trimright %W ".b"]
	UI_Shade
    }


#    bind $f <B1-Motion> {
#	puts "HELLLLLLLLLLLO!"
#	puts [set dxc [expr %x - $xcontour]]
#	puts [set dyc [expr %y - $ycontour]]
#	.d.c move contour [expr %x - $xcontour] [expr %y - $ycontour]
#	set xcontour %x
#	set ycontour %y
#	update idletasks
#    }

    # Bind the return key in the width entry to resize window 
    bind $f.e.we <Return> {
	set activeWindow [string trimright %W ".e.we"] 
	if {$WindowArray($activeWindow,w) < 250} {
	    set WindowArray($activeWindow,w) 250
	} elseif {$WindowArray($activeWindow,w) > 1250} {
	    set WindowArray($activeWindow,w) 1250
	} 
	    $activeWindow configure -width $WindowArray($activeWindow,w)
	
    }

    # Bind the return key in the height entry to resize window
    bind $f.e.he <Return> {
	set activeWindow [string trimright %W ".e.he"]
 	if {$WindowArray($activeWindow,h) < 200} {
	    set WindowArray($activeWindow,h) 200
	} elseif {$WindowArray($activeWindow,h) > 1000} {
	    set WindowArray($activeWindow,h) 1000
	} 
	    $activeWindow configure -height $WindowArray($activeWindow,h)
       
    }

    # Bind the resize button to Mark for resize 
    bind $f.e.b <Button-1> {
	set activeWindow [string trimright %W ".e.b"]
	# This will speed up the resize
	set WindowArray($activeWindow,x) %X
	set WindowArray($activeWindow,y) %Y
	BringToTop $activeWindow
    }

    # Bind to resize
    bind $f.e.b <B1-Motion> {

	set WindowArray($activeWindow,w) \
	    [expr $WindowArray($activeWindow,w) + \
		 (%X - $WindowArray($activeWindow,x))]

	set WindowArray($activeWindow,h) \
	    [expr $WindowArray($activeWindow,h) \
		 + (%Y - $WindowArray($activeWindow,y))]

	update idletasks

	set WindowArray($activeWindow,x) %X
	set WindowArray($activeWindow,y) %Y
    }
    
    # Bind to unforget
    bind $f.e.b <ButtonRelease-1> {

	if {$WindowArray($activeWindow,w) < 250} {
	    set WindowArray($activeWindow,w) 250
	} elseif {$WindowArray($activeWindow,w) > 1250} {
	    set WindowArray($activeWindow,w) 1250
	}
	if {$WindowArray($activeWindow,h) < 200} {
	    set WindowArray($activeWindow,h) 200
	} elseif {$WindowArray($activeWindow,h) > 1000} {
	    set WindowArray($activeWindow,h) 1000
	}
	$activeWindow configure -width $WindowArray($activeWindow,w) \
	    -height $WindowArray($activeWindow,h)
    }

    update idletasks

    incr wc
#}

# ---- MarkThis ----
# Helper function, remembers an (x, y) position to allow moving a window

proc MarkThis {x y} {
    global WindowArray activeWindow flag

    if {$WindowArray($activeWindow,min) == "EXPANDED"} {
	UI_Shade
    }

    set WindowArray($activeWindow,x) $x
    set WindowArray($activeWindow,y) $y
    raise $activeWindow
    raise .d.c
}


# ---- DragThis ----

proc DragThis {x y} {
    global WindowArray activeWindow

#    puts "screen x is $x, screen y is $y"
#    puts "winfo rootx is [winfo rootx .d.c.w$WindowArray($activeWindow,num)]"
#    puts "winfo rooty is [winfo rooty .d.c.w$WindowArray($activeWindow,num)]"
#    puts "canvas width is [winfo width .d.c]"
    
    set oldx $WindowArray($activeWindow,x)
    set oldy $WindowArray($activeWindow,y)

    set WindowArray($activeWindow,x) $x
    set WindowArray($activeWindow,y) $y

    set dx [expr $WindowArray($activeWindow,x) - $oldx]
    set dy [expr $WindowArray($activeWindow,y) - $oldy]

    .d.c move $WindowArray($activeWindow,id) $dx $dy

    set devx [winfo rootx .d.c.w$WindowArray($activeWindow,num)]
#    puts "$devx  >  [winfo rootx .d.c]"
    set devy [winfo rooty .d.c.w$WindowArray($activeWindow,num)]
#    puts "$devy  >  [winfo rooty .d.c]"
    set devxx [expr $devx + $WindowArray($activeWindow,w)]
#    puts "devxx  <  [expr [winfo rootx .d.c] + [winfo width .d.c]]"
    set devyy [expr $devy + $WindowArray($activeWindow,h)]
#    puts "devyy  <  [expr [winfo rooty .d.c] + [winfo height .d.c]]"

    if {$devx < [winfo rootx .d.c]} {
	.d.c xview scroll [expr 1 + $dx / 10] units
	update idletasks
#	puts "SCROLL $dx UNITS LEFT!"
    } elseif {$devxx > [expr [winfo rootx .d.c] + [winfo width .d.c]]} {
	.d.c xview scroll [expr 1 + $dx / 10] units
	update idletasks
#	puts "SCROLL $dx UNITS RIGHT!"
    }
    if {$devy < [winfo rooty .d.c]} {
	.d.c yview scroll [expr 1 + $dy / 10] units
	update idletasks
#	puts "SCROLL $dy UNITS HIGH!"
    } elseif {$devyy > [expr [winfo rooty .d.c] + [winfo height .d.c]]} {
	.d.c yview scroll [expr 1 + $dy / 10] units
	update idletasks
#	puts "SCROLL $dy UNITS LOW!"
    }
}

# ---- MoveThis ----
# Helper function, moves a window by following the mouse

proc MoveThis {x y} {
    global WindowArray activeWindow flag

    if {$WindowArray($activeWindow,min) != "EXPANDED"} {
	UI_Shade
    }

    set mouse [.d.c coords $WindowArray($activeWindow,id)]
    if {[lindex $mouse 0] < 0} {
	puts "TOO FAR LEFT!"
	.d.c move $WindowArray($activeWindow,id) [expr 0 - [lindex $mouse 0]] 0
    }
    if {[lindex $mouse 1] < 0} {
	puts "TOO HIGH!"
	.d.c move $WindowArray($activeWindow,id) 0 [expr 0 - [lindex $mouse 1]]
    }
 }

# ---- UI_Shade ----
# WINDOWSHADING functionality
   
proc UI_Shade {} {
    global activeWindow WindowArray save 

    if {$WindowArray($activeWindow,min) == "EXPANDED"} { ; # if window not shaded
	set save $WindowArray($activeWindow,h)
	set WindowArray($activeWindow,h) [winfo height $activeWindow.b]
	$activeWindow configure -height [winfo height $activeWindow.b]
	pack forget $activeWindow.m
	pack forget $activeWindow.e
	set WindowArray($activeWindow,min) "SHADED"
    } elseif {$WindowArray($activeWindow,min) == "SHADED"} {           ; # if window shaded
	pack $activeWindow.e -side bottom -anchor se
	pack $activeWindow.m -padx 5 -fill both -expand true
	set WindowArray($activeWindow,min) "EXPANDED"
	set WindowArray($activeWindow,h) $save
	$activeWindow configure -height 500
    } else {
	puts "How the hell did you click on a destroyed window??!!"
    }
    update idletasks
}


# ---- PrintMessage ----
# ESSENTIAL Helper function, displays messages on the MAIN message bar
# The one at the bottom of the screen that ALWAYS is active

proc PrintMessage { var } {
    global mes
    set mes $var
}


# ---- BringToTop ----
# This brings the window to the top, and focuses in the text widget

proc BringToTop { windowName } {
    global WindowArray
    global activeWindow
    
    set activeWindow $windowName
    set temp [string trimright $activeWindow $WindowArray($activeWindow,num)]
    set temp [string trimright $temp ".f"]
    raise $temp
    raise .d.c.buffer$WindowArray($activeWindow,buf)
    focus .d.c.buffer$WindowArray($activeWindow,buf)
}

# ---- UI_CreateWindow ----
# Called by New to create a window with an untitled editable text buffer inside
# Creates Untitled buffers
# 

proc UI_CreateWindow {} {
    global wc numUntitled
    incr numUntitled ; #Keeps track of the number of untitled windows
    set bufNum [BM_CreateBuffer text Untitled$numUntitled .d.c.w$wc.f$wc 0]
    MakeWindow $bufNum
    UI_DisplayBuffer $bufNum
}

# ---- UI_OpenHelp ----
# Opens the help files as Read Only Buffers

proc UI_OpenHelp {filename} {
    global wc
    set bufNum [BM_CreateBuffer RO $filename .d.c.w$wc.f$wc 0]
    
    text_LoadNew $bufNum $filename
    MakeWindow $bufNum
    UI_DisplayBuffer $bufNum
}

# ---- UI_CreateBuffer ----
# Creates buffers, acts on buffer manager, ensures no empty windows

proc UI_CreateBuffer {filename} {
    global activeWindow

    if { $filename == "empty" } {
	set number [BM_CreateBuffer text Untitled $activeWindow 0]
    } else {
	set number [BM_CreateBuffer text $filename $activeWindow 0]
	text_LoadNew $number $filename
    }
    return $number
}

# ---- UI_LoadFile ----
# gets new filename, creates a new buffer for it, calls UI_DisplayBuffer to put it in a window

proc UI_LoadFile {} {
    global wc NumBuffers BufferArray
    set duplicate 0
    # Search for duplicates.  Duplicates are Read Only
    if {[set fName [fileselect "Load File" {} 1]] != ""} {
	if {[file readable $fName] == 1} {
	    for {set i 4} {$i <= $NumBuffers} {incr i} {
		if {[edit_isBuffer $i] == 1} {
		    if {$fName == $BufferArray($i,name)} {
			set duplicate 1
		    }
		}
	    }
	    if {[file writable $fName] == 0} {
		set duplicate 2
	    }
	    if {$duplicate > 0} {
		set bufNum [BM_CreateBuffer RO "(RO)$fName" .d.c.w$wc.f$wc 0]
	    } else {
		set bufNum [BM_CreateBuffer text $fName .d.c.w$wc.f$wc 0]
	    }
	    text_LoadNew $bufNum $fName
	    # Need to do this for UI_display_Buffer to be able to check the window field
	    set BufferArray($bufNum,window) "NONE"
	    MakeWindow $bufNum
	    UI_DisplayBuffer $bufNum
	    if {$duplicate == 0} {
		PrintMessage "File Loaded: $fName"
	    } elseif {$duplicate == 1} {
		PrintMessage "READ ONLY FILE LOADED -- Duplicate File: $fName"
	    } elseif {$duplicate == 2} {
		PrintMessage "READ ONLY FILE LOADED: $fName"
	    }
	} else {
	    PrintMessage "You don't have read access to file: $fName"
	}
    } else {
	PrintMessage "Invalid Name - File Load CANCELLED"
    }
}


# ---- UI_DisplayBuffer ----
# If buffer is displayed in another window -- brings that window to top
# Else unpacks current activeWindow buffer, packs given buffer in active window

proc UI_DisplayBuffer {bufNum} {
    global WindowArray activeWindow BufferArray
    
   
    if { $BufferArray($bufNum,window) == "NONE"} {
	if {$activeWindow == "NOWINDOWS"} {
	    MakeWindow $bufNum
	} else {
	    set oldBuf $WindowArray($activeWindow,buf)
	    set BufferArray($oldBuf,window) "NONE"
	    $activeWindow.m.w.y configure -command ".d.c.buffer$bufNum yview"
	    PrintMessage $BufferArray($bufNum,name)
	    $activeWindow.b config -text $BufferArray($bufNum,name) -bg $WindowArray($activeWindow,color) -fg ivory -height 2
	    update idletasks
	    .d.c.buffer$bufNum configure -bg ivory -yscrollcommand "$activeWindow.m.w.y set"
	    bind .d.c.buffer$bufNum <Button-1> {
		set temp [string trimleft %W ".d.c.buffer"]
		BringToTop $BufferArray($temp,window)
	    }
	    pack forget .d.c.buffer$oldBuf
	    pack .d.c.buffer$bufNum -in $activeWindow.m.w -side top -fill both -expand true
	    set WindowArray($activeWindow,buf) $bufNum
	    set BufferArray($bufNum,window) $activeWindow
	}
	BringToTop $activeWindow
    } else { 
	BringToTop $BufferArray($bufNum,window)
    }
    Update_Menubars
}

# ---- UI_InsertFile ----
# The insert file option on the FILE menu
# inserts a file into the current editable text buffer

proc UI_InsertFile {} {
    global WindowArray BufferArray activeWindow
    if {$activeWindow != "NOWINDOWS"} {
	set bufNum $WindowArray($activeWindow,buf)
	if {$BufferArray($bufNum,type) == "text"} {
	    set indices [edit_findSelection $bufNum]
	    if {$BufferArray($bufNum,type) != "RO"} {
		if {[set fName [fileselect "Insert File" {} 1]] != ""} {
		    if {$indices != ""} {
			edit_clearSelection $bufNum
		    }
		    text_InsertFile $bufNum $fName
		}
	    }
	}
    }
}

# ---- UI_WriteRegion ----
# Saves selected region as a file
# Calls up save as dialogue box
# This option is on the FILE menu

proc UI_WriteRegion {} {
    global WindowArray BufferArray activeWindow
    if {$activeWindow != "NOWINDOWS"} {
	set bufNum $WindowArray($activeWindow,buf)
	if {[edit_findSelection $bufNum] != ""} {
	    if {[set filename [fileselect "Save Region As" {} 0]] != ""} {
		if {[file exists $filename] == 1} {
		    set promptReslt [UI_PromptBox "Overwrite $filename?"]
		    if {$promptReslt == 0} {
			PrintMessage "Write Region cancelled"
			return
		    }
		    if [catch {set f [open $filename w]}] {
			error "Error opening savefile"
		    } else {
			set x [edit_extractSelection $bufNum]
			puts $f $x
			close $f
			PrintMessage "Region saved to: $filename"
		    }
		}
	    } else {
		PrintMessage "Write Region cancelled"
	    }   
	}
    }
}

# ---- UI_SaveAsFile ----
# Calls up dialogue box to save as
# Checks for overwrite as well as the type of buffer being saved

proc UI_SaveAsFile {} {
    global WindowArray BufferArray activeWindow
    if {$activeWindow != "NOWINDOWS"} {
	set bufNum $WindowArray($activeWindow,buf)
	if {$BufferArray($bufNum,type) == "text"} {
	    if {[set fName [fileselect "Save File As" {} 0]] != ""} {
		if {[file exists $fName] == 1} {
		    if {[file writable $fName] == 1} {
			set promptReslt [UI_PromptBox "Overwrite $fName with $BufferArray($bufNum,name)?"]
			if { $promptReslt == 0 } {
			    PrintMessage "Save As Cancelled"
			    return
			} 
		    } else {
			PrintMessage "You don't have write access to file: $fName"
			return
		    }
		}
		text_Save $bufNum $fName
		set BufferArray($bufNum,name) $fName 
		$activeWindow.b config -text $BufferArray($bufNum,name)
		PrintMessage "Saved to: $fName"
		Update_Menubars
		return	
	    }
	} elseif {$BufferArray($bufNum,type) == "RO"} {
	    PrintMessage "Save Cancelled - File is READ ONLY!"
	    return
	} else {
	    PrintMessage "Can only save editable text buffers!"
	    return
	}
    }
    PrintMessage "Save As Cancelled"
}

# ---- UI_PromptBox ----
# Creates a generic prompt box for saving and/or other stuff

proc UI_PromptBox { promptText {cancel 0}} {
    global PromptBox

    if {[winfo exists .prompt] == 1} {
	raise .prompt
	focus .prompt
	grab .prompt
	return
    }
    set yn [toplevel .prompt -bd 5 -bg darkseagreen2]
    wm geometry $yn 350x150+0+0
    message $yn.msg -text $promptText -bg darkseagreen2 -anchor c -aspect 500
    frame $yn.buttons -bg darkseagreen2
    button $yn.buttons.yes -text Yes -command {set PromptBox 1} -bg ivory
    button $yn.buttons.no -text No -command {set PromptBox 0} -bg ivory
    pack $yn.msg -fill both -expand true
    pack $yn.buttons -fill x -anchor s
    pack $yn.buttons.yes -side left -ipadx 5 -ipady 5 -padx 5 -fill x -expand true
    pack $yn.buttons.no -side left -ipadx 5 -ipady 5 -padx 5 -fill x -expand true
    if {$cancel == 1} {
	button $yn.buttons.cancel -text Cancel -command {set PromptBox -1} -bg ivory
	pack $yn.buttons.cancel -ipadx 5 -ipady 5 -padx 5 -fill x -expand true
    }

  
    grab $yn
    tkwait variable PromptBox
    grab release $yn
    destroy $yn 
 
    return $PromptBox
    
}

# ---- UI_SaveFile ----
# Saves the current file as the current filename
# Checks that there has been changes and/or that the file is not UNTITLED

proc UI_SaveFile {} {
    global WindowArray BufferArray activeWindow numUntitled NumUndo SaveChecker
    if {$activeWindow != "NOWINDOWS"} {
	set fName $BufferArray($WindowArray($activeWindow,buf),name)
	set bufNum $WindowArray($activeWindow,buf)
	if {$BufferArray($bufNum,type) == "text"} {
	    if {$SaveChecker($bufNum) != $NumUndo($bufNum)} {
		set fileNoName 0
		for {set i 1} {$i <= $numUntitled} {incr i} {
		    if {$BufferArray($bufNum,name) == "Untitled$i"} {
			set fileNoName 1
		    }
		}
		if {$fileNoName == 0} {
		    text_Save $bufNum $fName
		    PrintMessage "Saved to: $fName"
		} else {
		    PrintMessage "Must choose Save As for an untitled window"
		}
	    } else {
		PrintMessage "File has not changed"
	    }
	} elseif {$BufferArray($bufNum,type) == "RO"} {
	    PrintMessage "Save Cancelled - File is READ ONLY!"
	} else {
	    PrintMessage "Can only save editable text buffers!"
	}
    }
}

# ---- UI_CloseWindow ----
# Closes the given window, sets its buffer's window field to "NONE", raises another window and makes it activeWindow

proc UI_CloseWindow {window} {
    global BufferArray WindowArray activeWindow wc statusArray
    if {$activeWindow != "NOWINDOWS"} {
	set BufferArray($WindowArray($window,buf),window) "NONE"
	set statusArray($WindowArray($window,num)) "DESTROYED"
	set temp [string trimright $window $WindowArray($window,num)]
	
	# find the next window to raise
	# only if this is called from the menu, 
	if {$window == $activeWindow} {
	    for {set i [expr $wc - 1] } {$i >= 0 } {incr i -1} {
		if {$statusArray($i) != "DESTROYED"} {
		    set activeWindow .d.c.w$i.f$i
		    BringToTop $activeWindow
		    set i -2
		}
	    }
	    if {$window == $activeWindow} {
		set activeWindow "NOWINDOWS"
	    }
	}
	destroy [string trimright $temp ".f"]
	Update_Menubars
    }
}

# ---- UI_DeleteBuffer ----
# This deletes the buffer in the active window
# Checks if the file has been altered and prompts to save appropriately

proc UI_DeleteBuffer {} {
    global activeWindow WindowArray NumBuffers BufferArray numUntitled SaveChecker NumUndo

    if {$activeWindow != "NOWINDOWS"} {
	set bufNum $WindowArray($activeWindow,buf)
	# buffers 1, 2, 3 are the UNIX, TCL, Scheme pipes initially started.  Can't delete them!!!
	if {($bufNum != 1) && ($bufNum != 2) && ($bufNum !=3)} {
	    # are the buffers saved?
	    if {$BufferArray($bufNum,type) == "text"} {
		if {$SaveChecker($bufNum) != $NumUndo($bufNum)} {
		    set fileNoName 0
		    for {set i 1} {$i <= $numUntitled} {incr i} {
			if {$BufferArray($bufNum,name) == "Untitled$i"} {
			    set fileNoName 1
			}
		    }
		    set answer [UI_PromptBox "$BufferArray($bufNum,name) is an unsaved buffer. Save?" 1]
		    if {$answer == -1} {
			PrintMessage "Delete Buffer Cancelled"
			return "CANCEL"
		    } elseif {$answer == 1} {
			if {$fileNoName == 0} {
			    UI_SaveFile
			} else {
			    if {[set fName [fileselect "Save File As" {} 0]] != ""} {
				if {[file exists $fName] == 1} {
				    set promptReslt [UI_PromptBotx "Overwrite $fName with $BufferArray($bufNum,name)?"]
				    if {$promptReslt == 0} {
					PrintMessage "Delete Buffer Cancelled"
					return "CANCEL"
				    }
				}
				text_Save $bufNum $fName
			    } else {
				PrintMessage "Delete Buffer Cancelled"
			        return "CANCEL"
			    }
			}
		    }
		}
	    }	  
	    set found 0
	    for {set i $NumBuffers} {$i >= 4} {incr i -1} {
		if {($BufferArray($i,type) != "DELETED") && ($BufferArray($i,window) == "NONE")} {
		    set bufToDisplay $i
		    set found 1
		    set i -1 ; #breaks the loop
		}
	    }
	    if {$found == 0} {
		incr numUntitled
		set bufToDisplay [BM_CreateBuffer text Untitled$numUntitled $activeWindow 1]
		set BufferArray($bufToDisplay,window) "NONE"
	    }
	    UI_DisplayBuffer $bufToDisplay
	    BM_DeleteBuffer $bufNum
	    Update_Menubars
	} else {
	    PrintMessage "Permission Denied!  Do not delete the initial pipe buffers!"
	}
    }
}

# ---- UI_Cut ----
# Cut from the current buffer

proc UI_Cut {} {
    global activeWindow WindowArray BufferArray
    if {$activeWindow != "NOWINDOWS"} {
	set bufNum $WindowArray($activeWindow,buf)
	text_Cut $bufNum
    }
}

# ---- UI_Paste ----
# Paste from clipboard into buffer

proc UI_Paste {} {  
    global activeWindow WindowArray BufferArray
    if {$activeWindow != "NOWINDOWS"} {
	set bufNum $WindowArray($activeWindow,buf)
	text_Paste $bufNum
    }
}

# ---- UI_Copy ----
# Copies from buffer into clipboard

proc UI_Copy {} {
    global activeWindow WindowArray BufferArray
    if {$activeWindow != "NOWINDOWS"} {
	set bufNum $WindowArray($activeWindow,buf)
	text_Copy $bufNum
    }
}

# ---- UI_Clear ----
# Clears current selection in buffer

proc UI_Clear {} {
    global activeWindow WindowArray BufferArray
    if {$activeWindow != "NOWINDOWS"} {
	set bufNum $WindowArray($activeWindow,buf)
	text_Clear $bufNum
    }
}

# ---- UI_SelectAll ----
# Selects all text in current buffer

proc UI_SelectAll {} {
    global activeWindow WindowArray BufferArray
    if {$activeWindow != "NOWINDOWS"} {
	set bufNum $WindowArray($activeWindow,buf)
	edit_selectAll $bufNum
    }
}

# ---- UI_Undo ----
# The undo data stucture for editable text buffers

proc UI_Undo {} {
    global activeWindow WindowArray BufferArray
    if {$activeWindow != "NOWINDOWS"} {
	set bufNum $WindowArray($activeWindow,buf)
	if {$BufferArray($bufNum,type) == "text"} {
	    global TextUndo NumUndo SizeUndo
	    set where $NumUndo($bufNum) ; # Delete at $where
	    if {$where > 0} {
		if {$TextUndo($bufNum,SEL$where) == ""} { ; # if no selections
		    if {$TextUndo($bufNum,UD$where) == "W"} {
			.d.c.buffer$bufNum insert $TextUndo($bufNum,IP$where) $TextUndo($bufNum,U$where)
		    } elseif {$TextUndo($bufNum,UD$where) == "D"} {
			.d.c.buffer$bufNum delete $TextUndo($bufNum,IP$where)
		    }
		} else { ; # there were selections
		    set coord1 [lindex $TextUndo($bufNum,SEL$where) 0]
		    set coord2 [lindex $TextUndo($bufNum,SEL$where) 1]
		    .d.c.buffer$bufNum tag delete sel
		    if {$TextUndo($bufNum,UD$where) == "K"} {
			.d.c.buffer$bufNum delete $TextUndo($bufNum,IP$where)
			.d.c.buffer$bufNum insert $TextUndo($bufNum,IP$where) [lrange $TextUndo($bufNum,U$where) 1 end]
			.d.c.buffer$bufNum tag add sel $coord1 $coord2
		    } elseif {$TextUndo($bufNum,UD$where) == "P"} {
			.d.c.buffer$bufNum delete $coord1 $coord2
		    } else {
			.d.c.buffer$bufNum insert $TextUndo($bufNum,IP$where) $TextUndo($bufNum,U$where)
			.d.c.buffer$bufNum tag add sel $coord1 $coord2
		    }
		}
		incr NumUndo($bufNum) -1
	    }
	}
    }
}

# ---- UI_Redo ----
# Redo is a mirror image of Undo with the exception
# that we travel the other way up the Undo Array

proc UI_Redo {} {
    global activeWindow WindowArray BufferArray
    if {$activeWindow != "NOWINDOWS"} {
	set bufNum $WindowArray($activeWindow,buf)
	if {$BufferArray($bufNum,type) == "text"} {
	    global NumUndo TextUndo SizeUndo
	    set where $NumUndo($bufNum) ; # where are you in array?
	    incr where
	    if {$where <= $SizeUndo($bufNum)} {
		# then there is stuff to REDO
		if {$TextUndo($bufNum,SEL$where) == ""} { ;# No selections
		    if {$TextUndo($bufNum,UD$where) == "D"} {
			.d.c.buffer$bufNum insert $TextUndo($bufNum,IP$where) $TextUndo($bufNum,U$where)
		    } elseif {$TextUndo($bufNum,UD$where) == "W"} {
			.d.c.buffer$bufNum delete $TextUndo($bufNum,IP$where)
		    }
		} else { ;# there were selections
		    set coord1 [lindex $TextUndo($bufNum,SEL$where) 0]
		    set coord2 [lindex $TextUndo($bufNum,SEL$where) 1]
		    .d.c.buffer$bufNum tag delete sel
		    if {$TextUndo($bufNum,UD$where) == "K"} {
			.d.c.buffer$bufNum delete $coord1 $coord2
			.d.c.buffer$bufNum insert $TextUndo($bufNum,IP$where) [lindex $TextUndo($bufNum,U$where) 0]
			.d.c.buffer$bufNum tag add sel $TextUndo($bufNum,IP$where)
		    } elseif {$TextUndo($bufNum,UD$where) == "P"} {
			.d.c.buffer$bufNum insert $TextUndo($bufNum,IP$where) $TextUndo($bufNum,U$where)
			.d.c.buffer$bufNum tag add sel $coord1 $coord2
		    } else {
			.d.c.buffer$bufNum delete $coord1 $coord2
			.d.c.buffer$bufNum tag add sel $coord1 $coord2
		    }
		}
		incr NumUndo($bufNum)
	    }
	}
    }
}	    

# ---- Update_Menubars ----
# Update menu bars to appropriately represent the open buffers and windows
# Essential to Buffer Manager and Window Manager tracking

proc Update_Menubars { }  {
global WindowArray wc activeWindow
global BufferArray
global NumBuffers
global statusArray

.menu.window.menu delete 0 last
.menu.buffers.menu delete 0 last
   
    for {set i 0} {$i < $wc} { incr i } {
	if {$statusArray($i) == "OPEN" } {
	    set windowName .d.c.w$i.f$i
	    set buffernum $WindowArray($windowName,buf)
	    set fileName $BufferArray($buffernum,name)
	    .menu.window.menu add command -label "$fileName" -command "BringToTop $windowName; BringToLeft"
	}
    }
    for {set i 1} {$i <= $NumBuffers} {incr i} {
	if {[edit_isBuffer $i] == 1} {
	    set fileName $BufferArray($i,name)
	    .menu.buffers.menu add command -label "$fileName" -command "UI_DisplayBuffer $i"
	}
    }
}

proc BringToLeft {} {
    global WindowArray activeWindow
    
    .d.c coords $WindowArray($activeWindow,id) 0 0
    .d.c xview moveto 0
    .d.c yview moveto 0
}

bind . <Alt-4> {BringToLeft}

# ---- UI_Shutdown ----
# Closes all the open pipes and prompts for saving
# on editable buffers.  Destroys pipes and buffer before exit.

proc UI_Shutdown {} {
    global NumBuffers wc statusArray

    # close windows
    for {set i 0} {$i < $wc} {incr i} {
	if {$statusArray($i) == "OPEN"} {
	    set windowName .d.c.w$i.f$i
	    UI_CloseWindow $windowName
	}
    }

    # check all the open buffers
    for {set i 4} {$i <= $NumBuffers} {incr i} {
	if {[edit_isBuffer $i] == 1} {
	    UI_DisplayBuffer $i
	    set out [UI_DeleteBuffer]
	    if {$out == "CANCEL"} {
		# Cancel quitting
		return
	    }
	}
    }

    # shutdown pipes
    pipe_Shutdown

    # and we're outta here!
    exit
}


# BM_BufferInit is first thing called to initialize files
BM_BufferInit
Update_Menubars

##############################
####  UNIVERSAL BINDINGS  ####
##############################

bind . <Alt-q> {UI_Shutdown; break}
bind . <Alt-n> {UI_CreateWindow; break}
bind . <Alt-l> {UI_LoadFile; break}
bind . <Alt-s> {UI_SaveFile; break}
bind . <Alt-c> {UI_Copy; break}
bind . <Alt-x> {UI_Cut; break}
bind . <Alt-v> {UI_Paste; break}
bind . <Alt-a> {UI_SelectAll; break}
bind . <Help> {UI_OpenHelp General.hlp; break}

bind . <Alt-y> {NewColor NEWCOLOR}
bind . <Alt-7> {pickColor; update idletasks}