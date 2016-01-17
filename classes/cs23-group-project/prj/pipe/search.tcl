#! ./kedit

# $Header: /usr/tahoe1/davidk/KillerS/Integration/RCS/search.tcl,v 1.6 1996/05/29 03:33:43 mihai Exp gondek $

# search.tcl

# Gondek's search functionality



# ---- UI_Search ----
# Search function

proc UI_Search {} {
    global activeWindow WindowArray BufferArray    
    global laststring
    
    if {$activeWindow != "NOWINDOWS"} {
	set bufNum $WindowArray($activeWindow,buf)
	
	if {[winfo exists .search] == 0} {
	    set srch [toplevel .search -bd 5 -relief raised -class Search]
	    wm geometry .search 500x100+0+0
	    wm title .search "Regexp Search"
	    frame $srch.top
	    frame $srch.bottom
	    pack $srch.top $srch.bottom -side top
	    
	    label $srch.top.mesg -text "Search"
	    pack $srch.top.mesg
	    
	    frame $srch.bottom.entry 
	    frame $srch.bottom.buttons
	    pack $srch.bottom.entry $srch.bottom.buttons -side left
	    label $srch.bottom.entry.l -text Expression:
	    entry $srch.bottom.entry.entry -width 20 -relief sunken \
		-textvariable inp
	    pack $srch.bottom.entry.l $srch.bottom.entry.entry -side left
	    
	    
	    button $srch.bottom.buttons.ok -text Find \
		-width 7 -command {UI_Find}
	    button $srch.bottom.buttons.cancel -text "Cancel" \
		-width 7 -command {destroy .search; return}

	    pack $srch.bottom.buttons.ok $srch.bottom.buttons.cancel -side top
	    
	    bind $srch <Return> "UI_Find" 

	    set laststring ""
	    
	    set currinsert [.d.c.buffer$bufNum index insert] 
	    tkwait variable srchdone 
	    
	    $srch.top.mesg configure -text "Search hit end. "  
	    while {[UI_PromptBox "Wrap search?"] == 1} {
		.d.c.buffer$bufNum mark set insert 1.0 
		UI_Find
		tkwait variable srchdone		
		$srch.top.mesg configure -text "Search hit end."
	    }
 
	    destroy .search



	} else {
	    raise .search
	    focus .search
	}

    } else {
	PrintMessage "No window to search in"
    }


}


# ---- UI_Replace    
# because num is local, but button and inp are global, need
# to use another procedure to implement.

proc UI_Replace {} {
    global activeWindow WindowArray BufferArray    
    global laststring

    if {$activeWindow != "NOWINDOWS"} {   
	if {[winfo exists .searchrep] == 0} {
	    set bufNum $WindowArray($activeWindow,buf)
	    if {$BufferArray($bufNum,type) != "text"} {
		return
	    }
	    set srchrep [toplevel .searchrep -bd 5  -relief raised -class Search]
#	    pack propagate $srchrep false

	    frame $srchrep.entries -height 100
	    frame $srchrep.buttons 
	    pack propagate $srchrep.entries false
	    pack $srchrep.entries -side top -fill x
	    pack $srchrep.buttons -side bottom 


	    frame $srchrep.entries.s 
	    frame $srchrep.entries.r 



	    pack $srchrep.entries.s -side top 
	    pack $srchrep.entries.r -side top

	    label $srchrep.entries.s.l -text "Expression:" -width 15
	    entry $srchrep.entries.s.entry -width 50  -relief sunken \
		-textvariable inp
	    pack  $srchrep.entries.s.l \
		$srchrep.entries.s.entry  -side left 


	    label $srchrep.entries.r.l -text "Replace with:" -width 15
	    entry $srchrep.entries.r.entry -width 50  -relief sunken \
		-textvariable rep
	    pack  $srchrep.entries.r.l \
		$srchrep.entries.r.entry -side left  
	    
	    button $srchrep.buttons.replace -text "Replace" -state disabled \
		-width 13 -command {
		    UI_ReplaceSelection $bufNum
		    .searchrep.buttons.replace configure -state disabled 
		}
	 
	    button $srchrep.buttons.findnext -text "Find Next" \
		-width 13 -command {if [UI_Find] {
		  .searchrep.buttons.replace configure -state normal
		}}
	     #button $srchrep.buttons.repnex -text "Replace Next" \
		\#-width 13 -command "UI_Find; UI_ReplaceSelection $bufNum"
	    button $srchrep.buttons.repall -text "Replace All" \
		-width 13 -command "UI_ReplaceAll $bufNum"
	    button $srchrep.buttons.cancel -text "Cancel" \
		-width 13 -command {destroy .searchrep; return}
	    pack $srchrep.buttons.findnext $srchrep.buttons.replace\
		$srchrep.buttons.repall $srchrep.buttons.cancel -side left


	    set laststring ""
	    set currinsert [.d.c.buffer$bufNum index insert]

	    tkwait variable srchdone

	    PrintMessage "Replace hit end"
	} else {
	    raise .searchrep
	    focus .searchrep
	}
    } else {
	PrintMessage "No Window Selected"
    }
    
}

# ---- UI_ReplaceSelection ----
# Replaces selection
# Stores info in UNDO data structure

proc UI_ReplaceSelection {bufNum} {
    global rep NumUndo SizeUndo TextUndo
    edit_clearSelection $bufNum
    
    # UNDO stuff
    incr NumUndo($bufNum)
    set SizeUndo($bufNum) $NumUndo($bufNum)
    set where $NumUndo($bufNum)
    set TextUndo($bufNum,U$where) $rep
    set TextUndo($bufNum,UD$where) P
    set IPStart [edit_getIP $bufNum]
    .d.c.buffer$bufNum insert insert $rep
    set IPFinish [edit_getIP $bufNum]
    set TextUndo($bufNum,SEL$where) "$IPStart $IPFinish"
}

proc UI_ReplaceAll {bufNum} {
    global rep inp
    while {[edit_findText $bufNum $inp]} {
	UI_ReplaceSelection $bufNum
    }    
}    
proc UI_Find {} {

    global activeWindow WindowArray BufferArray 
    global inp srchdone
    global laststring currinsert
    if {$inp == ""} {return -1}
    set bufNum $WindowArray($activeWindow,buf)
    
    if {$laststring == $inp} {
	if {$currinsert == [.d.c.buffer$bufNum index insert]} {
	    .d.c.buffer$bufNum mark set insert "insert + 1 chars" 
	}
    }
   
    
    if [catch {set result [edit_findText $bufNum $inp]} errmsg] {
	PrintMessage $errmsg
	return
    }
    if {$result == 0} {
	set srchdone 1
    } 


    set laststring $inp
    set currinsert [.d.c.buffer$bufNum index insert]
}


#selects text if found, if not found returns 0
proc edit_findText { bufNum exp  } {
  
    #get buffer contents into string.    
    set value [.d.c.buffer$bufNum get insert end]   
  
    #call C++ regexp search
    
    #find location 
    if [catch {set offset [reg $value $exp]} result] {
	error $result
	return
    }
    

   
    if {$offset == "0.0"} {
	return 0
    }
    
    
    #parse data
    
    #find location
    set offint [expr int($offset)]
    
    #find length of word
    set offsize [expr $offset - $offint]
    #get decimal portion !! trim?
    while {$offsize > [ expr int($offsize) ]} {	
	set offsize [expr 10 * $offsize ]
    }   
    set offsize [expr int($offsize)]
    
   #update insertion pointer and hilite selection
    .d.c.buffer$bufNum mark set insert "insert + $offint chars"
    .d.c.buffer$bufNum mark set regfoundend "insert + $offsize chars"

    #remove old selection tags.
    .d.c.buffer$bufNum mark set beg 0.0
    .d.c.buffer$bufNum mark set end end
    .d.c.buffer$bufNum tag remove sel beg end
    .d.c.buffer$bufNum tag add sel insert regfoundend
   
    .d.c.buffer$bufNum see insert
    update idletasks
    
    return 1

}
