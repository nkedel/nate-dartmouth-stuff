#! ./kedit

# $Header: /usr/tahoe1/davidk/KillerS/Integration/RCS/BM.tcl,v 3.4 1996/05/28 22:30:05 tim Exp davidk $

# buffer.tcl

# This subsystem file creates the buffer manager array at startup with BMInit
# and updates it with the commands: CreateBuffer and DeleteBuffer

# Modification History:
#      Created by David Kung on April 17, 1996
#      Modified with extensions by Xiaohong Hu on April 17, 1996
#      Modified with power using by Tim Lifshits and Mihai Banulescu
#      Modified for Nate's Big Pipe by the almighty Pipe and the Mortals

# ---- BM_BufferInit ----
# Initializes for the Buffer Manager Array Structure
# Initializes the counter for the Buffer Array and structure

proc BM_BufferInit {} {
    global BufferArray
    global statusArray
    global NumBuffers

    set NumBuffers 3
    # NumBuffers is the number of arrays currently existing    
    # Created the User Clipboard in index 0 of the buffer array
    # It is of special type "CLIPBOARD" and will be invisible to the user
    set BufferArray(0,name) Clipboard
    # creates the text widget for the Clipboard
    edit_create 0 CLIPBOARD
    set BufferArray(0,window) "NONE"
    # The clipboard is not counted as part of NumBuffers    
#    pipe_Init 1 2 3 ; # this creates the three pipes: TCL, Scheme, UNIX     
    set BufferArray(1,window) NONE
    set BufferArray(2,window) NONE
    set BufferArray(3,window) NONE
    pipe_Init 1 2 3
    PrintMessage "Welcome to the Killer Editor... Tread carefully"
}

# ---- BM_Find_Spot ----
# This procedure returns the first empty spot in the buffer manager array

proc BM_Find_Spot {} {
    global NumBuffers

    for {set i 0} {$i <=  $NumBuffers } {incr i} {
	if {[edit_isBuffer $i] == 0} {
	    return $i
	}
    }
    return $i
}


# ---- BM_CreateBuffer ----
# Creates a new buffer and places its info into the buffer array
# "DELETED" represents a dummy node and/or empty array location
# types are "text", "pipe", or "RO". "CLIPBOARD" is a special buffer type
# returns the number of the location you got!{

proc BM_CreateBuffer { type filename windowname oldWindow } {
    # BufferArray is the global Buffer Manager Data Structure
    # BufferNum is a counter for the number of availiable buffers
    global BufferArray
    global WindowArray
    global NumBuffers
    global statusArray

    set arrayLocation [BM_Find_Spot]
    set statusArray($arrayLocation) "OPEN"
    # the text widget buffer is born!!!!
    # Calls edit_Create {bufferIndex, type}
    # the text widget buffer is created by editCreate
    # the type of the buffer is set within edit_create as well
    edit_create $arrayLocation $type  
    set BufferArray($arrayLocation,name) $filename
    # The buffer that is displayed in the window to which the new buffer is now
    # being open will no longer be displayed anywhere
    if {$oldWindow == 1} {
	set BufferArray($WindowArray($windowname,buf),window) NONE
    }
    set BufferArray($arrayLocation,window) $windowname
    # The windowarray will now remember the name of the buffer 
    # that is being displayed in the current window
    return $arrayLocation
}

# ---- BM_DeleteBuffer ----
# Deletes a buffer from the array and deallocates it

proc BM_DeleteBuffer {number} {
    # Needs the global stuff
    global BufferArray
    global NumBuffers
   
    # if the buffer was editable text, destroy undo array
    if {$BufferArray($number,type) == "text"} {
	global TextUndo
	set TextUndo($number,0) 0
	global NumUndo
	global SizeUndo
	set NumUndo($number) 0
	set SizeUndo($number) 0
    }

    # the buffer is destroyed 
    if {$BufferArray($number,type) == "pipe"} {
	pipe_Destroy $number
    } else {
	edit_close $number
    }

    # empty array location now
    set BufferArray($number,type) "DELETED"
    set BufferArray($number,name) ""
    set BufferArray($number,window) "NONE"

}