#!/usr/local/bin/wish

# editfcn.tcl

# The base editor function subsystem implementation
# for the Killer Editor
# Killer Software, 1996

# Modification History
#      Created by David Kung on April 18, 1996

# ---- edit_Create ----
# Creates a new buffer (text widget) of type RO or RW

proc edit_Create {bufferIndex, type} {
    text .d.c.buffer$bufferIndex
    if {type == "RO"} {
	.d.c.buffer$bufferIndex config -state disabled
    }
}
# ---- edit_Load ----
# Load a file into the buffer

proc edit_Load {buffernum, filename} {
    # Get the text from the file
    set f [open $fn r]
    set x [read $f]
    
    # insert the new text into the buffer
    .d.c.buffer$buffernum insert $x
    
    # close the file
    close $f
}

# ---- edit_Save ----
# Saves the buffer contents to a file

proc edit_Save {buffernum, filename} {
    # opens the file
    set f [open $filename w]
    # get text to save
    set x [.d.c.buffer$buffernum get 1.0 end]
    # Save it
    puts $f $x
    # close the file
    close $f
}

# ---- edit_Copy ----
# Copies from one buffer to another
# does not differentiate clipboard from regular buffer

proc edit_Copy {tobuffernum, frombuffernum} {
    set f [.d.c.buffer$buffernum get sel.begin sel.end]
    .d.c.buffer$buffernum insert $f
}

# ---- edit_clearBuffer ----
# Clears the current buffer

proc edit_ClearBuffer {buffernum} {
    .d.c.buffer$buffernum delete 1.0 end
}

# ---- edit_Close ----
# Destroys a buffer

proc edit_Close {buffernum} {
    destroy .d.c.buffer$buffernum
}

# ---- edit_clearSelection ----
# clears selected text of a buffer

proc edit_clearSelection {buffernum} {
    .d.c.buffer$buffernum delete sel.begin sel.end
}

# ---- edit_findSelection ----
# finds location of the selection

proc edit_findSelection {buffernum} {
    set location [.d.c.buffer$buffernum sel.begin]
    return location
}

# ---- edit_getIP ----
# finds the location of the insertion point

proc edit_getIP {buffernum} {
    set location [.d.c.buffer$buffernum insert]
}

# ---- edit_setIP ----
# sets the insertion point at a location

proc edit_setIP {buffernum, location} {
    .d.c.buffer$buffernum mark set insert location
}

# ---- edit_getline ----
# gets the line at a certain point

proc edit_getline {buffernum, linenumber} {
    set line [.d.c.buffer$buffernum get $linenumber.0 $linenumber.end]
}

# ---- edit_putline ----
# places a string at a point replacing that line

proc edit_putline {buffernum, linenumber, newlineString} {
    .d.c.buffer$buffernum delete $linenumber.0 $linenumber.end
    .d.c.buffer$buffernum insert $linenumber.0 newlineString
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
    return BufferArray($buffernum, type)
}

# ---- edit_getPipe ----
# returns the pipe associated with buffer number

proc edit_getPipe { brec } {
    global bpipes
    return $bpipes($brec)
} 

# ---- edit_setPipe ----
# sets a pipe number to be associated with the buffer number

proc edit_setPipe { brec pipeno } {
    global bpipes
    set bpipes($brec) $pipeno
}

# ---- edit_extractSelection ----
# returns the selected text

proc edit_extractSelection {buffernum} {
    return .d.c.buffer$buffernum get sel.begin sel.end
}

# ---- edit_Extract ----
# returns text from coordinate to coordinate

proc edit_Extract {buffernum, startCoord, endCoord} {
    return .d.c.buffer$buffernum get startCoord endCoord
}

# ---- edit_fileend ----
# returns coordinates of the end of the file

proc edit_fileEnd {buffernum} {
    return .d.c.buffer$buffernum end
}

# ---- edit_isBuffer? ----
# does the buffer exist at the specified location?
# returns false if not and true if yes

proc edit_isBuffer? {buffernum} {
    global BufferArray
    global ArrayNum
    global NumBuffers
    if {buffernum > NumBuffers} {
	return 0
    }
    if {buffernum > ArrayNum} {
	return 0
    }
    if {BufferArray($buffernum, type) == "\0"} {
	return 0
    }
    return 1
}

# ---- edit_Append ----
# appends text at end of file

proc edit_Append { bufNum atext } {
    global buffers
    set bpath $buffers($bufNum)
    $bpath insert end $atext
}

# ---- edit_selectAll ----
# selects all the text

proc edit_selectAll {buffernum} {
    .d.c.buffer$buffernum mark sel.begin 1.0
    .d.c.buffer$buffernum mark sel.end end
}