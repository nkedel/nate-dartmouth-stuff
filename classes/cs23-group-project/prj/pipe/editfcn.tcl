#! ./kedit

# $Header: /usr/tahoe1/davidk/KillerS/Integration/RCS/editfcn.tcl,v 3.5 1996/05/28 17:00:06 tim Exp davidk $

# editfcn.tcl

# The base editor function subsystem implementation
# for the Killer Editor
# Killer Software, 1996

# Modification History
#      Created by David Kung on April 18, 1996
#      UNDO structures added by David Kung on April 26, 1996
#      Power functionality added by David Kung on April 27, 1996

# ---- edit_undoStruct ----
# subroutine for the UNDO bindings for a text widget
# binds every single keystroke to an UNDO event
# ONLY USED ON EDITABLE TEXT BUFFERS
# With UNDO instruction sets: W means write
#                             D means delete
#                             P means pasted
#                             K means keyboard selection delete

proc edit_undoStruct {bufferIndex} {
    # undo data structures
    global NumUndo
    global SizeUndo
    global TextUndo
    
    # the delete key
    # binds using it on both regular text and a selection
    bind .d.c.buffer$bufferIndex <Delete> {
	set bufNum [string trimleft %W ".d.c.buffer"] ; # which buffer
	if {[edit_findSelection $bufNum] == ""} {
	    # regular text
	    set IPHere [.d.c.buffer$bufNum index "insert -1 char"]
	    # where is the insertion point
	    if {[edit_getIP $bufNum] != "1.0"} { ; # not at beginning of text
		# increment stuff
		incr NumUndo($bufNum)
		set SizeUndo($bufNum) $NumUndo($bufNum)
		# where is the UNDO array location
		set where $NumUndo($bufNum)
		set letter [%W get "insert -1 char"] ; # the letter deleted
		set TextUndo($bufNum,U$where) $letter ; # stores the letter
		set TextUndo($bufNum,UD$where) W ; # instructions for UNDO
		set TextUndo($bufNum,IP$where) $IPHere; # insertion point here
		set TextUndo($bufNum,SEL$where) "" ; # nothing was selected
		%W delete "insert -1 char" ; # delete the character
	    }
	} else {
	    # regular clearing of selection (will handle UNDO itself)
	    edit_clearSelection $bufNum
	}
	break ; # normal delete should not run
    }

    # Ordinary Letter tracking to the UNDO array
    # binds every single key that can be put in the text widget
    # binds on regular typing and typing over a selection
    bind .d.c.buffer$bufferIndex <KeyPress> {
	set bufNum [string trimleft %W ".d.c.buffer"] ; # which buffer
	# increment UNDO stuff
	incr NumUndo($bufNum)
	set SizeUndo($bufNum) $NumUndo($bufNum)
	set where $NumUndo($bufNum) ; # where is the UNDO array location
	if {[edit_findSelection $bufNum] == ""} {
	    # if NOT selected
	    set IPHere [.d.c.buffer$bufNum index insert] ; # store IP
	    set letter %A ; # find the letter typed
	    set TextUndo($bufNum,U$where) $letter ; # store the letter
	    set TextUndo($bufNum,UD$where) D ; # instructions for UNDO
	    set TextUndo($bufNum,IP$where) $IPHere ; # insertion point here
	    set TextUndo($bufNum,SEL$where) "" ; # nothing was selected
	} else {
	    # if something was selected when letter was typed
	    set range [%W tag ranges sel] ; # find selected region
	    set block [%W get [lindex $range 0] [lindex $range 1]] ;# get words
	    set IPHere [lindex $range 0] ; # get IP
	    set letter %A ; # letter typed
	    set TextUndo($bufNum,IP$where) $IPHere ; # stores insertion point
	    set TextUndo($bufNum,U$where) "$letter $block" ; # stores letter and deleted words
	    set TextUndo($bufNum,SEL$where) $range ; # stores selection region
	    set TextUndo($bufNum,UD$where) K ; # UNDO instructions
	}
    }

    # Keys that should NOT be binded with Letter tracking above

    # Control Key
    bind .d.c.buffer$bufferIndex <Control_L> {
    }

    # KeyPad Enter Key
    bind .d.c.buffer$bufferIndex <KP_Enter> {
    }

    # Caps Lock
    bind .d.c.buffer$bufferIndex <Caps_Lock> {
    }

    # Left Alt Key
    bind .d.c.buffer$bufferIndex <Alt_L> {
    }

    # Right Alt Key
    bind .d.c.buffer$bufferIndex <Alt_R> {
    }

    # Right Shift Key
    bind .d.c.buffer$bufferIndex <Shift_R> {
    }

    # Left Shift Key
    bind .d.c.buffer$bufferIndex <Shift_L> {
    }

    # Weird Compose Character Key
    bind .d.c.buffer$bufferIndex <Multi_key> {
    }

    # Left Key
    bind .d.c.buffer$bufferIndex <Left> {
    }

    # Right Key
    bind .d.c.buffer$bufferIndex <Right> {
    }

    # Up Key
    bind .d.c.buffer$bufferIndex <Up> {
    }

    # Down Key
    bind .d.c.buffer$bufferIndex <Down> {
    }

    # Bind Alt-Z to the UNDO command
    bind .d.c.buffer$bufferIndex <Alt-z> {
	UI_Undo
    }

    # Scheme Parsing binding
    # Bind the Tab key for parsing
    bind .d.c.buffer$bufferIndex <Tab> {
	parse_Line 1.0
	break
    }
}

# ---- edit_Create ----
# Creates a new buffer (text widget) of type RO or RW
# Information about this created buffer is stored in the
# Buffer Manager which is the ONLY unit that can create buffers

proc edit_create {bufferIndex type} {
    # Buffer Array Variables
    global BufferArray
    global NumBuffers

    # Create a brand new text widget
    text .d.c.buffer$bufferIndex -width 120 -height 60

    # Things to do if type is editable text
    if {$type == "text"} {
	# the UNDO data structures
	global TextUndo
	global NumUndo
	global SizeUndo

	# Savechecker follows the UNDO data structure and from there
	# can see if you have saved or not
	global SaveChecker
	
	# if the buffer is editable create undo
	#undo array created
	# if the buffer is editable text create undo array
	# undo array is globally accessible
	set TextUndo($bufferIndex,U0) "**\0"
	set NumUndo($bufferIndex) 0
	set SizeUndo($bufferIndex) 0
	set SaveChecker($bufferIndex) 0

	edit_undoStruct $bufferIndex
    }
    
    # Pipe binding of the Delete key
    if {$type == "pipe"} {
	bind .d.c.buffer$bufferIndex <Delete> {
	    if {[edit_findSelection 1] == ""} {
		%W delete "insert -1 char"
		break
	    }
	}
    }

    # This prevents a potential error by storing pipe info for the pipe manager
    set BufferArray($bufferIndex,pipenum) -1

    # Store type of new buffer in Buffer Manager Array
    set BufferArray($bufferIndex,type) $type

    # since a buffer has been created, the number of buffers is incremented
    # unless the type was CLIPBOARD since the clipboard does not count
    if {$type != "CLIPBOARD"} {
	if {$bufferIndex > $NumBuffers} {
	    incr NumBuffers
	}
    }   
}

# ---- edit_Load ----
# Load a file into the buffer

proc edit_load {buffernum fn} {
    global BufferArray
    # Get the text from the file
    if [catch {set f [open $fn r]}] {
	return "ERROR OPENING FILE"
    } else {
	set f [open $fn r]
	set x [read $f]
	# insert the new text into the buffer
	.d.c.buffer$buffernum insert insert $x
	# close the file
	close $f
    }
    if {$BufferArray($buffernum,type) == "RO"} {
	.d.c.buffer$buffernum config -state disabled	
    }
    # set the Insertion Point at the beginning of the file
    if {$BufferArray($buffernum,type) != "text"} {
	edit_setIP $buffernum 1.0
    }
}

# ---- edit_Save ----
# Saves the buffer contents to a file

proc edit_save {buffernum filename} {
    # opens the file
    if {[file writable filename] || ![file exists filename]} {
	if [catch {set f [open $filename w]}] {
	    error "Error opening savefile"
	} else {
	    # get text to save
	    set x [.d.c.buffer$buffernum get 1.0 end]
	    
	    # Save it
	    puts $f $x
	    
	    # close the file
	    close $f
	}
    }
}

# ---- edit_Copy ----
# Copies from one buffer to another
# does not differentiate clipboard from regular buffer
# BE SURE TO CHECK if cutting or copying NOTHING that this is NOT called!!!
# or else you get the whole buffer!!!!

proc edit_copy {frombuffernum tobuffernum} {
    if {[.d.c.buffer$frombuffernum tag ranges sel] == ""} {
	set f [.d.c.buffer$frombuffernum get 1.0 end]
    } else {
	set range [.d.c.buffer$frombuffernum tag ranges sel]
	set f [.d.c.buffer$frombuffernum get [lindex $range 0] [lindex $range 1]]
    }
    .d.c.buffer$tobuffernum insert insert $f
}

# ---- edit_clearBuffer ----
# Clears the current buffer
# Deletes everything in the buffer

proc edit_clearBuffer {buffernum} {
    .d.c.buffer$buffernum delete 1.0 end
}

# ---- edit_Close ----
# Destroys a buffer

proc edit_close {buffernum} {
    # Buffer Manager Array must be informed of deletion
    global BufferArray
    set BufferArray($buffernum,pipenum) -1
    set BufferArray($buffernum,type) "DELETED"
    destroy .d.c.buffer$buffernum
}

# ---- edit_clearSelection ----
# clears selected text of a buffer
# should only be called on an editable text buffer
# saves the info in the UNDO array

proc edit_clearSelection {buffernum} {
    global BufferArray
    set range [.d.c.buffer$buffernum tag ranges sel]
    if {$range != ""} { ; # if there actually IS a selection
	if {$BufferArray($buffernum,type) == "text"} { ; # checks that this is editable text
	    # the UNDO data variables
	    global TextUndo NumUndo SizeUndo
	    incr NumUndo($buffernum) ; # incrementing
	    set SizeUndo($buffernum) $NumUndo($buffernum)
	    set where $NumUndo($buffernum) ; # where in the UNDO array are we?
	    # block is set to the selected text
	    set block [.d.c.buffer$buffernum get [lindex $range 0] [lindex $range 1]]
	    set IPHere [lindex $range 0] ; # insertion point
	    set TextUndo($buffernum,IP$where) $IPHere ; # stores insertion point
	    set TextUndo($buffernum,U$where) $block ; # stores selected text 
	    set TextUndo($buffernum,UD$where) W ; # UNDO instructions
	    set TextUndo($buffernum,SEL$where) $range ; # selected region
	}
	# delete the selected region
	.d.c.buffer$buffernum delete [lindex $range 0] [lindex $range 1]
    }
}

# ---- edit_findSelection ----
# finds location of the selection

proc edit_findSelection {buffernum} {
    return [.d.c.buffer$buffernum tag ranges sel]
}

# ---- edit_getIP ----
# finds the location of the insertion point

proc edit_getIP {buffernum} {
    return [.d.c.buffer$buffernum index insert]
}

# ---- edit_setIP ----
# sets the insertion point at a location

proc edit_setIP {buffernum location} {
    .d.c.buffer$buffernum mark set insert $location
}

# ---- edit_getline ----
# gets the line at a certain point
# linenumber is just a number like 3 NOT 3.0 or 4.0 or anything like that

proc edit_getline {buffernum linenumber} {
    return [.d.c.buffer$buffernum get $linenumber.0 $linenumber.end]
}

# ---- edit_putline ----
# places a string at a point replacing that line
# linenumber is JUST A NUMBER like in edit_getline

proc edit_putline {buffernum linenumber newlineString} {
    .d.c.buffer$buffernum delete $linenumber.0 $linenumber.end
    .d.c.buffer$buffernum insert $linenumber.0 $newlineString
}

# ---- edit_movetoTop ----
# moves the insertion point to the top of the buffer

proc edit_movetoTop {buffernum} {
    .d.c.buffer$buffernum mark set insert 1.0
}

# ---- edit_movetoBottom ----
# moves the insertion point to the bottom of the buffer

proc edit_movetoBottom {buffernum} {
    .d.c.buffer$buffernum mark set insert end
}

# ---- edit_bufferType ----
# returns type of the buffer by accessing buffer manager

proc edit_bufferType {buffernum} {
    global BufferArray
    return $BufferArray($buffernum,type)
}

# ---- edit_getPipe ----
# returns the pipe associated with buffer number

proc edit_getPipe {buffernum} {
    global BufferArray
	return $BufferArray($buffernum,pipenum)
} 

# ---- edit_setPipe ----
# sets a pipe number to be associated with the buffer number
# buffernum gets pipenum because pipenum is the pipe number

proc edit_setPipe {buffernum pipenum} {
    global BufferArray
	set BufferArray($buffernum,pipenum) $pipenum
}

# ---- edit_extractSelection ----
# returns the selected text

proc edit_extractSelection {buffernum} {
    set range [.d.c.buffer$buffernum tag ranges sel]
    if {$range != ""} {
	return [.d.c.buffer$buffernum get [lindex $range 0] [lindex $range 1]]
    }
}

# ---- edit_Extract ----
# returns text from coordinate to coordinate
# startCoord and endCoord are in 1.0 2.8 etc format

proc edit_Extract {buffernum startCoord endCoord} {
    return [.d.c.buffer$buffernum get $startCoord $endCoord]
}

# ---- edit_fileend ----
# returns coordinates of the end of the file

proc edit_fileEnd {buffernum} {
    return .d.c.buffer$buffernum end
}

# ---- edit_isBuffer? ----
# does the buffer exist at the specified location?
# returns false if not and true if yes

proc edit_isBuffer {buffernum} {
    global BufferArray
    global NumBuffers
    if {$buffernum > $NumBuffers} {
	return 0
    } elseif {$BufferArray($buffernum,type) == "DELETED"} {
	return 0
    }
    return 1
}

# ---- edit_Append ----
# appends text at end of file

proc edit_Append {buffernum appendtext} {
    .d.c.buffer$buffernum insert end $appendtext
}

# ---- edit_selectAll ----
# selects all the text

proc edit_selectAll {buffernum} {
    .d.c.buffer$buffernum tag delete sel
    .d.c.buffer$buffernum tag add sel 1.0 end
}

# ---- edit_first_of ----
# returns the first coordinate of a coordinate set

proc edit_first_of {coord} {
    return [lindex [split $coord .] 0]
}

# ----edit_second_of ----
# returns the second coordinate of a coordinate set

proc edit_second_of {coord} {
    return [lindex [split $coord .] 1]
}