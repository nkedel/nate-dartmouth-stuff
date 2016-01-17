#! ./kedit

# textfcn.tcl

# Editable text function subsystem implementation
# for the Killer Editor
# Killer Software, 1996

# Modification History
#      Created by Xiaohong Hu on 5/18/96
#      Heavily Modified by David Kung on April 23, 1996
#      Power functionality added by David Kung on April 27, 1996

# ---- text_LoadNew ----
# LoadNew merely loads a file into what should be a new buffer
# will clear the buffer before loading

proc text_LoadNew {buffernum filename}  {
    edit_clearBuffer $buffernum
    edit_load $buffernum $filename
    edit_setIP $buffernum 1.0
}

# ---- text_InsertFile ----
# similar to LoadNew except that the buffer is not cleared and
# the stuff loaded is selected for you

proc text_InsertFile {buffernum filename} {
    set select1 [edit_getIP $buffernum]
    edit_load $buffernum $filename
    set select2 [edit_getIP $buffernum]
    .d.c.buffer$buffernum tag add sel $select1 $select2
    set undolet [.d.c.buffer$buffernum get $select1 $select2]
   
    global BufferArray
    if {$BufferArray($buffernum,type) == "text"} {
	    # if this is editable text, UNDO must be implemeneted
	global NumUndo
	global SizeUndo
	global TextUndo
	incr NumUndo($buffernum)
	set SizeUndo($buffernum) $NumUndo($buffernum)
	set where $NumUndo($buffernum)
	set TextUndo($buffernum,U$where) $undolet
	set TextUndo($buffernum,UD$where) P
	set TextUndo($buffernum,IP$where) $select1
	.d.c.buffer$buffernum tag add sel $select1 $select2
	set TextUndo($buffernum,SEL$where) [edit_findSelection $buffernum]
    }
}

# ---- text_Save ----
# saves the current file for you

proc text_Save {buffernum filename}  {
    global SaveChecker NumUndo
    edit_save $buffernum $filename
    # SaveChecker now knows that changes have/have not been implemented
    set SaveChecker($buffernum) $NumUndo($buffernum)
}

# ---- text_Copy ----
# copies text from ANY buffer to the clipboard

proc text_Copy {buffernum} {
    if {[edit_findSelection $buffernum] != ""} {
	edit_clearBuffer 0
	edit_copy $buffernum 0
    }
}

# ---- text_Clear ----
# clears selected text from the buffer

proc text_Clear {buffernum} {
    edit_clearSelection $buffernum 

}

# ---- text_Cut ----
# cuts the selected text from the buffer
# copies it to both the clipboard and the UNDO data array if editable text
# otherwise just like Copy

proc text_Cut {buffernum} {
    global BufferArray
    if {[edit_findSelection $buffernum] != ""} {
	edit_clearBuffer 0
	edit_copy $buffernum 0
	if {$BufferArray($buffernum,type) == "text"} {
	    edit_clearSelection $buffernum
	}
    }
}

# ---- text_Paste ----
# pastes in most things but only does UNDO for editable text buffers

proc text_Paste {buffernum} {
    set undolet [.d.c.buffer0 get 1.0 end]
    if {$undolet != ""} {
	# pasting over a selection
	if {[edit_findSelection $buffernum] != ""} {
	    edit_clearSelection $buffernum
	}
	# pasteing in from the clipboard
	set IPStart [edit_getIP $buffernum]
	edit_copy 0 $buffernum 
	set IPFinish [edit_getIP $buffernum]
	global BufferArray
	if {$BufferArray($buffernum,type) == "text"} {
	    # if this is editable text, UNDO must be implemeneted
	    global NumUndo
	    global SizeUndo
	    global TextUndo
	    incr NumUndo($buffernum)
	    set SizeUndo($buffernum) $NumUndo($buffernum)
	    set where $NumUndo($buffernum)
	    set TextUndo($buffernum,U$where) $undolet
	    set TextUndo($buffernum,UD$where) P
	    set TextUndo($buffernum,IP$where) $IPStart
	    .d.c.buffer$buffernum tag add sel $IPStart $IPFinish
	    set TextUndo($buffernum,SEL$where) [edit_findSelection $buffernum]
	}
    }
}