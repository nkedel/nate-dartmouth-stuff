#! ./kedit

# $Header: /usr/tahoe1/davidk/KillerS/Integration/RCS/createmenu.tcl,v 3.8 1996/05/29 03:30:38 davidk Exp $

# createmenu.tcl

# This file contains the procedures to create menu items for the Killer Editor

# Modification History
#     Code lifted from K2.tcl by David Kung and Tim Lifshits on May 26, 1996
#     Toolbar added by Xiaohong Hu on May 27, 1996


#### MENU ITEMS ####

# ---- UI_Create_Filemenu ----
# Creates the FILE menu

proc UI_Create_Filemenu {} {
    global activeWindow

    # Create the "File" menubutton and menu 
    menubutton .menu.file -text File -underline 0 -menu .menu.file.menu
    menu .menu.file.menu -tearoff 0

    # New window command
    .menu.file.menu add command -label "New" -underline 0 \
	-accelerator "Alt-N" -command {
	    PrintMessage "Make a new window with untitled buffer"
	    UI_CreateWindow
	}

    # Load command
    .menu.file.menu add command -label "Load" -underline 0 \
	-accelerator "Alt-L" -command {
	    UI_LoadFile
	}

    .menu.file.menu add separator ; # separator

    # Insert File command
    .menu.file.menu add command -label "Insert File" -underline 0 \
	-command {
	    PrintMessage "Insert File"
	    UI_InsertFile
	}

    # Write region command
    .menu.file.menu add command -label "Write Region" -underline 0 \
	-command {
	    PrintMessage "Write Region"
	    UI_WriteRegion
	}

    .menu.file.menu add separator ; # separator

    # Save command
    .menu.file.menu add command -label "Save" -underline 0 \
	-accelerator "Alt-S" -command {
	    PrintMessage "Save"
	    UI_SaveFile
	}

    # Save As command
    .menu.file.menu add command -label "Save As" -underline 5 \
	-command {
	    PrintMessage "Save as" 
	    UI_SaveAsFile
	}

    .menu.file.menu add separator ; # separator

    # Close Window command
    .menu.file.menu add command -label "Close Window" -underline 0 \
	-command {
	    PrintMessage "Close Window"
	    UI_CloseWindow $activeWindow
	}

    # Delete Buffer command
    .menu.file.menu add command -label "Delete Buffer" -underline 0 \
	-command {
	    PrintMessage "Delete Buffer"
	    UI_DeleteBuffer
	}

    .menu.file.menu add separator ; #separator

    # Quit command
    .menu.file.menu add command -label "Quit" -underline 0 \
	-accelerator "Alt-Q" -command {UI_Shutdown}
}

# ---- UI_Create_Editmenu ----
# Creates the EDIT menu

proc UI_Create_Editmenu {} {

    # creates the "Edit" menu and button
    menubutton .menu.edit -text Edit -underline 0 -menu .menu.edit.menu
    menu .menu.edit.menu -tearoff 0

    # UNDO command
    .menu.edit.menu add command -label "Undo" -underline 0 \
	-accelerator "Alt-Z" -command {
	    PrintMessage "Undo"
	    UI_Undo
	}

    # REDO command
    .menu.edit.menu add command -label "Redo" -underline 0 \
	-accelerator "Alt-R" -command {
	    PrintMessage "Redo"
	    UI_Redo
	}

    .menu.edit.menu add separator

    # Cut command
    .menu.edit.menu add command -label "Cut" -underline 2 \
	-accelerator "Alt-X" -command  {
	    PrintMessage "Cut"
	    UI_Cut
	}

    # Copy command
    .menu.edit.menu add command -label "Copy" -underline 0 \
	-accelerator "Alt-C" -command {
	    PrintMessage "Copy"
	    UI_Copy
	}

    # Paste command
    .menu.edit.menu add command -label "Paste" -underline 0 \
	-accelerator "Alt-V" -command {
	    PrintMessage "Paste"
	    UI_Paste
	}

   .menu.edit.menu add separator

    # Clear Selection Command
    .menu.edit.menu add command -label "Clear Selection" -underline 1 \
	-command {
	    PrintMessage "Clear Selected Text"
	    UI_Clear
	}
    
    # Select All command
    .menu.edit.menu add command -label "Select All" -underline 7 \
	-accelerator "Alt-A" -command {
	    PrintMessage "Select All"
	    UI_SelectAll
	}

   .menu.edit.menu add separator

    # REGEXP search
    .menu.edit.menu add command -label "RegExp Search" -underline 7 \
	-command {
	    PrintMessage "Regular Expression Search"
	    UI_Search
	}

    # REGEXP replace
    .menu.edit.menu add command -label "RegExp Search & Replace" -underline 17 \
	-command {
	    PrintMessage "Regular Expression Search and Replace"
	    UI_Replace
	}
}

# ---- UI_Create_Toolsmenu
# Creates the tools menu

proc UI_Create_Toolsmenu {} {
    global activeWindow

    # creates Tools menu and list
    menubutton .menu.tools -text Tools -underline 0 -menu .menu.tools.menu
    menu .menu.tools.menu -tearoff 0

    # The three pipe processes, TCL, Scheme, UNIX
    .menu.tools.menu add cascade -label "TCL" -menu .menu.tools.menu.tcl -underline 0
    .menu.tools.menu add cascade -label "Scheme" -menu .menu.tools.menu.scheme -underline 0
    .menu.tools.menu add cascade -label "UNIX" -menu .menu.tools.menu.unix -underline 0

   .menu.tools.menu add separator

    # Parse line command (identical to the Tab command)
    .menu.tools.menu add command -label "Parse Line" -underline 0 \
	-accelerator "Tab" -command {
	    PrintMessage "Parse Line"
	    parse_Line 1.0
	}
   
    # Parse entire text command
    .menu.tools.menu add command -label "Parse Entire Text" -underline 6\
	-command {
	    PrintMessage "Parse Entire Text"
	    parse_All
	}
    
    # Submenus for the pipe processes
    menu .menu.tools.menu.tcl -tearoff 0
    menu .menu.tools.menu.scheme -tearoff 0
    menu .menu.tools.menu.unix -tearoff 0   

    ### Pipe menus executing buffers and selections ###

    .menu.tools.menu.tcl add command -label "Execute Buffer" -underline 8\
	-command {
	    PrintMessage "Run TCL File"
	    if {$activeWindow != "NOWINDOWS"} {
		text_BtoTCL $WindowArray($activeWindow,buf)
	    } else {
		PrintMessage "No open windows"
	    }
	}
    .menu.tools.menu.tcl add command -label "Execute Selection" -underline 8\
	-command {
	    PrintMessage "Execute TCL Selection"
	    if {$activeWindow != "NOWINDOWS"} {   
		text_StoTCL $WindowArray($activeWindow,buf)
	    } else {
		PrintMessage "No open windows"
	    }
	}

    .menu.tools.menu.scheme add command -label "Execute Buffer" -underline 8\
	-command {
	    PrintMessage "Run Scheme File"
	    if {$activeWindow != "NOWINDOWS"} {
		text_BtoSCHEME $WindowArray($activeWindow,buf)
	    } else {
		PrintMessage "No open windows"
	    }
	}
    .menu.tools.menu.scheme add command -label "Execute Selection" -underline 8 \
	-command {
	    PrintMessage "Run part of Scheme File"
	    if {$activeWindow != "NOWINDOWS"} {
		text_StoSCHEME $WindowArray($activeWindow,buf)
	    } else {
		PrintMessage "No open windows"
	    }
	}

    .menu.tools.menu.unix add command -label "Execute Buffer" -underline 8 \
	-command {
	    PrintMessage "Run UNIX File"
	    if {$activeWindow != "NOWINDOWS"} {
		set newBuf [BM_Find_Spot]
		text_BtoUNIX $WindowArray($activeWindow,buf) $newBuf
		set BufferArray($newBuf,window) "NONE"
		MakeWindow $newBuf
		UI_DisplayBuffer $newBuf
	    } else {
		PrintMessage "No open windows"
	    }
	}
    .menu.tools.menu.unix add command -label "Execute Selection" -underline 8 \
	-command {
	    PrintMessage "Run part of UNIX File"
	    if {$activeWindow != "NOWINDOWS"} {
		set newBuf [BM_Find_Spot]
		text_StoUNIX $WindowArray($activeWindow,buf) $newBuf
		set BufferArray($newBuf,window) "NONE"
		MakeWindow $newBuf
		UI_DisplayBuffer $newBuf
	    } else {
		PrintMessage "No open windows"
	    }
	}
}

# ---- UI_Create_Buffermenu ----
# Create the Buffer menu that keeps track of active buffers

proc UI_Create_Buffermenu {} {
    menubutton .menu.buffers -text Buffer -underline 0 -menu .menu.buffers.menu
    menu .menu.buffers.menu -tearoff 0
}

# ---- UI_Create_Windowmenu ----
# Creates window menu that keeps track of open windows

proc UI_Create_Windowmenu {} {
    menubutton .menu.window -text Window -underline 0 -menu .menu.window.menu
    menu .menu.window.menu -tearoff 0
}

# --- UI_Create_Helpmenu
# HELP!!!!!!

proc UI_Create_Helpmenu {} {
    
    # Creates the menu item and button
    menubutton .menu.help -text Help -underline 0 -menu .menu.help.menu
    menu .menu.help.menu -tearoff 0

    # General Help file
    .menu.help.menu add command -label "General" -underline 0 \
	-command {UI_OpenHelp General.hlp}
}

#### TOOLBAR  ###
#binding features: When the mouse is entered into the menubar,
#the associated message will be printed on the message window; when the mouse leaves the menubar,
#the associated message will be gone #########

# ---- UI_Create_Toolbar ----
# Creates the toolbar

proc UI_Create_Toolbar { } { 

    frame .toolbar -bg grey -relief ridge -bd 3
    pack .toolbar -fill x

    # Creates first 11 buttons on the left
    for {set i 0} { $i < 11 } { incr i} {	
	button .toolbar.$i -relief raised -justify center -bg skyblue
	pack .toolbar.$i -side left 
	bind .toolbar.$i <Leave> {PrintMessage " "}
    }

    # quit button
    button .toolbar.11 -relief raised -justify center -bg red
    pack .toolbar.11 -side right
    bind .toolbar.11 <Leave> {PrintMessage " "}

    # HELP bindings for mouse motion through these buttons
    bind .toolbar.0 <Enter> {PrintMessage "Opens a new window with an untitled buffer"}
    bind .toolbar.1 <Enter> {PrintMessage "Loads a file into a new window"}
    bind .toolbar.2 <Enter> {PrintMessage "Saves file in current window with current filename"}
    bind .toolbar.3 <Enter> {PrintMessage "Closes the Window"}
    bind .toolbar.4 <Enter> {PrintMessage "Undo last action"}
    bind .toolbar.5 <Enter> {PrintMessage "Redo last undo action"}
    bind .toolbar.6 <Enter> {PrintMessage "Cut current selection to Clipboard"}
    bind .toolbar.7 <Enter> {PrintMessage "Copy current selection to Clipboard"}
    bind .toolbar.8 <Enter> {PrintMessage "Paste from Clipboard"}
    bind .toolbar.9 <Enter> {PrintMessage "Regular Expression Search"}
    bind .toolbar.10 <Enter> {PrintMessage "Regular Expression Search and Replace"}
    bind .toolbar.11 <Enter> {PrintMessage "Quit Editor"}

    # configuring the picture and command of each button
    .toolbar.0 config -bitmap @new.xbm -command {UI_CreateWindow}
    .toolbar.1 config -bitmap @load.xbm -command  {UI_LoadFile}  
    .toolbar.2 config -bitmap @save.xbm -command {UI_SaveFile}
    .toolbar.3 config -bitmap @close.xbm -command {UI_CloseWindow $activeWindow}
    .toolbar.4 config -bitmap @undo.xbm -command {UI_Undo}
    .toolbar.5 config -bitmap @redo.xbm -command {UI_Redo}
    .toolbar.6 config -bitmap @cut.xbm -command {UI_Cut}
    .toolbar.7 config -bitmap @copy.xbm -command {UI_Copy} 
    .toolbar.8 config -bitmap @paste.xbm -command {UI_Paste}
    .toolbar.9 config -bitmap @search.xbm -command {UI_Search}
    .toolbar.10 config -bitmap @replace.xbm -command {UI_Replace}
    .toolbar.11 config -bitmap @quit.xbm -command {UI_Shutdown}
}