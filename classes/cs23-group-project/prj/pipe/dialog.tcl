#! ./kedit

# $Header: /usr/tahoe1/davidk/KillerS/Integration/RCS/dialog.tcl,v 1.15 1996/05/29 02:04:52 gondek Exp gondek $

# dialog.tcl

# Adapted by David Gondek on Apirl 24, 1996 by code
# borrowed from Welch's Practical Programming in TCL and TK
# fixed regexp ../ elimination
# added ~user completion
# added mustExist functionality  (can make up pathnames now)
# window title configurable
# jumps to front if lost

# ---- fileselect ----
# fileselect returns the selected pathname, or {}
# mustExist forces the dialog to find an existing file.

proc fileselect {{why "HI DAVE!!!!!!!"}
		 {default {}} {mustExist 1} } {
    global fileselect
    
    if {[winfo exists .fileselect] == 0} {
	set t [toplevel .fileselect -bd 4 -class Fileselect]
	fileselectResources
	wm title .fileselect $why
	#  message $t.msg -aspect 1000 -text $why
	#  pack $t.msg -side top -fill x

	# Create a read-only entry for the current directory
	set fileselect(dirEnt) [entry $t.dir -width 15 -relief flat -state disabled]
	pack $t.dir -side top -fill x

	# Create an entry for the pathname
	# The value is kept in fileselect(path)
	frame $t.top
	set e [entry $t.top.path -textvariable fileselect(path)]
	pack  $t.top -side top -fill x
	# The label on the entry is defined with an X resource
	label $t.top.l -padx 0
	pack $t.top.l -side left
	pack $t.top.path -side right -fill x -expand true

	# Create a listbox to hold the directory contents
	set lb [listbox $t.list -yscrollcommand [list $t.scroll set]]
	scrollbar $t.scroll -command [list $lb yview]

	# create the OK and Cancel buttons
	# The button text is defined with an X resource
	# The OK button has a rim to indicate it is the default
	frame $t.buttons -bd 10
	frame $t.buttons.ok -bd 2 -relief sunken
	set ok [button $t.buttons.ok.b \
		    -command fileselectOK]
	set can [button $t.buttons.cancel \
		     -command fileselectCancel]

	# Pack the list, scrollbar, and button bo
	# in a horizontal stack below the upper widgets
	pack $t.list -side left -fill both -expand true
	pack $t.scroll -side left -fill y
	pack $t.buttons -side left -fill both
	pack $t.buttons.ok $t.buttons.cancel \
	    -side top -padx 10 -pady 5
	pack $t.buttons.ok.b -padx 4 -pady 4

	fileselectBindings $t $e $lb $ok $can

	# Initialize variables and list the directory
	if {[string length $default] == 0} {
	    set fileselect(path) {}
	    set dir [pwd]
	} else {
	    set fileselect(path) [file tail $default]
	    set dir [file dirname $default]
	}
	set fileselect(dir) {}
	set fileselect(done) 0
	set fileselect(mustExist) $mustExist

	# Wait for the listbox to be visible so
	# that we can provide feedback during the listing

	tkwait visibility .fileselect.list
	fileselectList $dir
	grab $t
	tkwait variable fileselect(done)
	grab release $t
	destroy $t
	puts $fileselect(path)
	return $fileselect(path)
    } else {
	raise .fileselect 
	focus .fileselect
    }
}

# ---- fileselectResources ----
# added functionality

proc fileselectResources {} {
    # The "startup" priority lets users override these
    # path is used to enter the file name
    option add *Fileselect*path.relief sunken startup
    option add *Fileselect*path.background white startup
    option add *Fileselect*path.foreground black startup
    # Text for the label on pathname entry
    option add *Fileselect*1.text File: startup
    # Text for the OK and Cancel buttons
    option add *Fileselect*ok*text OK startup
    option add *Fileselect*ok*underline 0 startup
    option add *Fileselect*cancel.text Cancel startup
    option add *Fileselect*cancel,underline 0 startup
    # Size of the listbox
    option add *Fileselect*list.width 20 startup
    option add *Fileselect*list.height 10 startup
}

# ---- fileselectBindings

proc fileselectBindings {t e lb ok can} {
    # t - toplevel
    # e - nameentry
    # lb - listbox
    # ok - OK button
    # can - Cancel button

    # Eliminate the wall bonding tag because we do our own focus management
    # change binding order to allow delete precedence (per Neil Miller)
    foreach w [list $e $lb $ok $can] {
	bindtags $w [list $w [winfo class $w] $t]
    }
    # Dialog-global cancel binding
    bind $t <Control-c> fileselectCancel
    bind $t <Button-1> "raise $t"
  
    # Entry bindings
    bind $e <Return> fileselectOK
    bind $e <space> {fileselectComplete;break}

    # A single click, or <space>, puts the name in the entry
    # A double click, or <return>, selects the name
    bind $e <Delete> {
	if {![%W select present]} { 
	    if {![expr [%W index insert] == 0]} {
		%W delete [expr [%W index insert]-1]
	    }
	    break
	} 
	    
    }

    bind $lb <space> "fileselectTake %W; focus $e"
    bind $lb <Button-1> \
	"fileselectClick %W %y; focus $e"
    bind $lb <Return> "fileselectTake %W; fileselectOK"
    bind $lb <Double-Button-1> \
	"fileselectClick %W %y; fileselectOK"

    # Focus management
    bind $e <Tab> "focus $lb; $lb select set 0"
    bind $lb <Tab> "focus $e"
    
    # Button focus.  Extract the underlined letter
    # from the button label to use as the focus key.
#    foreach but [list $ok $can] {
#	set char [string tolower [string index \
#				      [$but cget -text] [$but cget -underline]]]
#	bind $t <Alt-$char> "focus $but ; break"
#    }
    bind $ok <Tab> "focus $can"
    bind $can <Tab> "focus $ok"
    
    # Set up for type in
    focus $e
}

# ---- fileselectList ----

proc fileselectList {dir {files {}} } {
    global fileselect

    # Update the directory display
    set e $fileselect(dirEnt)
    $e config -state normal
    $e delete 0 end
    $e insert 0 $dir
    $e config -state disabled
    # scroll to view the tail end
    $e xview moveto 1
    set fileselect(dir) $dir
    .fileselect.list delete 0 end
    # check for bogus entry
    if ![file isdirectory $dir] {
	.fileselect.list insert 0 "Bad Directory"
	return
    }

    # Give the user some feedback
    .fileselect.list insert 0 Listing...
    update idletasks

    .fileselect.list delete 0
    if {[string length $files] == 0} {
	# List the directory and add an
	# entry for the parent directory
	set files [glob -nocomplain $fileselect(dir)/*]
	.fileselect.list insert end ../
    }

    # Sort the directories to the front
    set dirs {}
    set others {}
    foreach f [lsort $files] {
	if [file isdirectory $f] {
	    lappend dirs [file tail $f] /
	} else {
	    lappend others [file tail $f]
	}
    }
    foreach f [concat $dirs $others] {
	.fileselect.list insert end $f
    }
}

# ---- fileselectOK ----

proc fileselectOK {} {
    global fileselect
puts "Start"
    # Trim ../ (the parent) out of the parthname
    if {[regsub {\.\.\/?} $fileselect(path) {} newpath] !=0} {
	set fileselect(path) $newpath
	set fileselect(dir) [file dirname $fileselect(dir)]
	fileselectOK
	return
    }

    # Assume path is relative to current directory
    set path [string trimright \
		  $fileselect(dir)/$fileselect(path) /]

    if [file isdirectory $path] {
	set fileselect(path) {}
	fileselectList $path
puts "isdir"
	return
    }
    if [file exists $path] {
	set fileselect(path) $path
	set fileselect(done) 1
	return
    }
    # Neither a file or a directory.
    # See if glob will find something
    if [catch {glob $path}  files] {
	# No, perhaps the user typed a new
	#absolute pathname
puts "glob1 $fileselect(dir) :: $fileselect(path)"
	if [catch {glob $fileselect(path)} path] {

	    #Nothing good -attempt completion
	    fileselectComplete
	    if {$fileselect(mustExist) == 0} {
		set fileselect(path) $fileselect(dir)/$fileselect(path)
		set fileselect(done) 1
	    }
	    return
	} else {
	    set st $fileselect(path)
	    #OK, - try again
puts "before $fileselect(dir) :: $fileselect(path)"
	    
	    #~user hack
	    if {[file isdirectory $fileselect(path)]} {
		set fileselect(dir) $fileselect(path)
	        set fileselect(path) ""
	     } else {
       
		set fileselect(dir) \
		    [file dirname $fileselect(path)]
		set fileselect(path) \
		    [file tail $fileselect(path)]
	    }
#	   set fileselect(dir) \
	\#	[file dirname $fileselect(path)]




puts "after $fileselect(dir) :: $fileselect(path)"
	    #handle ~
	    if {0} {	    if {[string index $st 0] == "~"} {
		set stlen [string length $st]
		set st {"~/"[string range $st 1 $stlen]} 	
		puts $st
	    	set fileselect(path) $st
	    }}
	    puts "dir: $fileselect(dir)"
	    puts "path: $fileselect(path)	    "
	    fileselectOK
	    return
	}
    } else {
	# OK, select the file or list them
	if {[llength [split $files]] == 1}{
	    set fileselect(path) $files
	    fileselectOK
	} else {
	    set fileselect(dir) \
		[file dirname [lindex $files 0]]
	    fileselectList $fileselect(dir) $files
	}
    }
}

# ---- fileselectCancel ----

proc fileselectCancel {} {
    global fileselect
    set fileselect(done) 1
    set fileselect(path) {}
    set fileselect(dir) {}
}

# ---- fileselectClick ----

proc fileselectClick {lb y} {
    # Take the item the user click on
    global fileselect
    set fileselect(path) [$lb get [$lb nearest $y]]
}

# ---- fileselectTake ----

proc fileselectTake {lb} {
    # take the currently selected list item
    global fileselect
    set fileselect(path) [$lb get [$lb curselection]]
}

# ---- fileselectComplete

proc fileselectComplete {} {
    global fileselect

    # Do file name completion
    # Nuke the space that triggered this call
    set fileselect(path) [string trim $fileselect(path) \t\ ]

    # Figure out what directory we are looking at:
    # dir is the directory
    # tail is the partial name
    if {[string match /* $fileselect(path)]} {
	set dir [file dirname $fileselect(path)]
	set tail [file tail $fileselect(path)]
    } elseif [string match ~* $fileselect(path)] {
	if [catch {file dirname $fileselect(path)} dir] {
	    return; # bad user
	}
	set tail [file tail $fileselect(path)]
    } else {
	set path $fileselect(dir)/$fileselect(path)
	set dir [file dirname $path]
	set tail [file tail $path]
    }    # See what files are there
    set files [glob -nocomplain $dir/$tail*]
    if {[llength [split $files]] == 1} {
	# Matched a single file
	set fileselect(dir) $dir
	set fileselect(path) [file tail $files]
    } else {
	if {[llength [split $files]] > 1} {
	    #Find longest common prefix
	    set l [expr [string length $tail]-1]
	    set miss 0
	    # Remember that files have absolute paths
	    set file1 [file tail [lindex $files 0]]
	    while {!$miss} {
		incr l
		if {$l == [string length $file1]} {
		    #file1 is a prefix of all others
		    break
		}
		set new [string range $file1 0 $l]
		foreach f $files {
		    if ![string match $new* [file tail $f]] {
			set miss 1
			incr l -1
			break
		    }
		}
	    }
	    set fileselect(path) [string range $file1 0 $l]
	}
	fileselectList $dir $files
    }
}

