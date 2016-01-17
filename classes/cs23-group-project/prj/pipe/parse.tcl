#! ./kedit

# $Header: /usr/tahoe1/davidk/KillerS/Integration/RCS/parse.tcl,v 3.4 1996/05/28 22:30:59 tim Exp davidk $

# parse.tcl

# Modification History
#        Created by Tim Lifshits


# --- get_next_nonspace
# Returns next non-tab non-space character in string (could be \n)
# Beginning at strIndex

proc get_next_nonspace {str strIndex} {
    while {([string index $str $strIndex] == " ") || ([string index $str $strIndex] == "\t") } {incr strIndex}
    # This will return the empty string if all are spaces 
    return [string index $str $strIndex]    
    
}

# ---- blank_or_comment

proc blank_or_comment {str} {
    set strIndex 0
    set ch [get_next_nonspace $str $strIndex]
    if {($ch == "\n") || ($ch == ";") || ($ch == "")} {
	return 1
    } else {
	return 0
    }
}

# ---- get_from_prev_line ----
# returns all of the text from the beginning of the widget
# to the end of the previous line (the line before the IP)

proc get_from_prev_line {bufNum startIndx} {
    set endIndx [.d.c.buffer$bufNum index "insert linestart -1 char"]
    set tx [.d.c.buffer$bufNum get $startIndx $endIndx]
    return $tx
}

# ---- parse_Line ----
# the big tab man parsing

proc parse_Line {startIndx} {
    global activeWindow WindowArray BufferArray
 
    if {$activeWindow != "NOWINDOWS"} {
	set bufNum $WindowArray($activeWindow,buf)
	
	# Should only work on editable text buffers
	if {$BufferArray($bufNum,type) == "text"} {
	    
	set s [get_from_prev_line $bufNum $startIndx]
	set parseRet [find_paren $s]
	set lineNum [expr [edit_first_of $parseRet] + [edit_first_of $startIndx]]
	set lineNum [expr $lineNum - 1]
	set depth [determine_indent $bufNum $lineNum.[edit_second_of $parseRet]]
	set lineNum [edit_first_of [.d.c.buffer$bufNum index insert]]
	set prevLine [edit_getline $bufNum $lineNum]
	set newLine [indent_string $prevLine $depth]
	edit_putline $bufNum $lineNum $newLine

	# Updates the UNDO data structure
	
	# UNDO variables
	global NumUndo SizeUndo TextUndo
	
	incr NumUndo($bufNum)       
	set where $NumUndo($bufNum); #where is the UNDO array location
	
	set TextUndo($bufNum,U$where) $prevLine ;# the previous line
	set TextUndo($bufNum,UD$where) W; #UNDO instruction set
	set TextUndo($bufNum,IP$where) $lineNum.0
	set TextUndo($bufNum,SEL$where) "$lineNum.0 $lineNum.end"
	
	incr NumUndo($bufNum)
	set SizeUndo($bufNum) $NumUndo($bufNum)
	set where $NumUndo($bufNum)
	
	set TextUndo($bufNum,U$where) $newLine
	set TextUndo($bufNum,UD$where) P
	set TextUndo($bufNum,IP$where) $lineNum.0
	set TextUndo($bufNum,SEL$where) "$lineNum.0 $lineNum.end"

	#####################
	
	return $parseRet
    } else {
	PrintMessage "Can only parse TEXT buffers!!!"
	return 0.0
    }
    } else {
	PrintMessage "No open windows to parse"
    }
}

# ---- determine _indent
# takes the coordinate of the paren to parse by
# and finds the amount to indent by

proc determine_indent {bufNum coord} {
    global bufferArray

    if {$coord == 1.0} {
	return 0
    }
    set l [edit_first_of $coord]
    set c [edit_second_of $coord]
    set first [edit_first_of $coord]
    set last [edit_second_of $coord.end]
    if {[blank_or_comment [edit_getline $bufNum $first]] == 1} {
	return 0
    }
    for {set i [expr $c+1]} {1==1 } {incr i} {	
	set currCh [.d.c.buffer$bufNum get $l.$i]	
	if {$currCh == "(" } {
	    return [expr $i-1]
	}  elseif {($currCh == "\n" ) || ($currCh == ";" )} {
	    return [expr $c+2]
	} elseif {$currCh == " "} {
	    return [expr $i+1]	    
	}
    }
}

# ---- indent_string ----
# takes a string, ensures the number of blank spaces at the beginning is equal to 
#  $depth (parameter)

proc indent_string {s depth} {
    while {([string index $s 0] == " ") || ([string index $s 0] == "\t") } {
	if {[string index $s 0] == " "} {
	    set s [string trimleft $s " "]
	} elseif { [string index $s 0] == "\t" } {
	    set s [string trimleft $s "\t "] 
	} 	
    }
    set temp ""
    for {set i 0} {$i < $depth} {incr i} {
	append temp " "
    }
    return $temp$s
}

# ---- parse_All ----
# Parses the whole text 

proc parse_All {} {
    global WindowArray activeWindow BufferArray
    if {$activeWindow != "NOWINDOWS"} {
	set bufNum $WindowArray($activeWindow,buf)
	set startIndx 1.0
	# Should only work on editable text buffers
	if {$BufferArray($bufNum,type) == "text"} {
	    set lastLine [edit_first_of [.d.c.buffer$bufNum index end]]
	    for {set i 1} {$i <= $lastLine} {incr i} {  
		edit_setIP $bufNum $i.0
		set parseRet [parse_Line $startIndx]	    
		update idletasks
		# This will help parse faster -- only the latest relevant 
		# part of the buffer will now be sent to the lexer
		if {$parseRet == 1.0} {
		    set startIndx $i.0 		
		}
	    }
	} else {
	    PrintMessage "Can only parse TEXT buffers!!!"
	}
    } else {
	PrintMessage "No open windows to parse"
    }
}