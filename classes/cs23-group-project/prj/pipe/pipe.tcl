# pipe.tcl
# internal module, include with "source pipe.tcl"
#
# $Header: /a/tahoe.cs.dartmouth.edu/usr/tahoe1/nate/prj/pipe/RCS/pipe.tcl,v 2.6 1996/05/29 03:37:53 nate Exp nate $
# by Nate Edel
#

set numPipes 1
set pipeArray(0) ""
set toEval ""

# A pipe record is a Tcl list with 5 elements:
# 1st Element (0) : the "type" of the pipe, either the command to be run,
#             "tcl" "scheme" or "csh". Only one "tcl" or "scheme" pipe can
#             exist in the current implementation
# 2nd Element (1) : the file stream returned by open for the pipe. Not used
#             for the tcl pipe. Changes if a pipe is reOpened.
# 3rd Element (2) : the number of the buffer associated with the pipe. Set 
#                   by pipe_Open and never changes.
# 4th Element (3) : full path of the command. The same as (0) unless one of 
#                   the 3 standard commands, in which case the defined path
#                   (hardcoded for this version)
# 5th Element (4) : Open/Closed. 1 if Open, 0 if closed. Changable on pipe
#                   close (as noticed by pipe_Read) or by pipe_Close, and 
#                   on pipe Open or reOpen.
#

#
# pipe_dumpData {pipe record #}
# Diagnostic function (callable by the user in the TCL window) 
# Dumps the contents of a specified pipe record to standard output
#
proc pipe_dumpData { pipeRecord } {
    global pipeArray
    if [ pipe_valid $pipeRecord ] { 
      puts $pipeArray($pipeRecord)
    }
}

#
# pipe_nodeCreate {pipe record #}
# Creates a blank pipe record in the pipe array, if needed.
#
proc pipe_nodeCreate { pipeRecord } {
    global pipeArray
    if [catch { set nothing $pipeArray} ] {
      set pipeArray($pipeRecord) [ list {nothing} {-1} {-1} {nothing} {0} ]
    }
}

#
# pipe_setData
# sets a specified field in the pipe record
#
proc pipe_setData { pipeRecord pipeField pData } {
    global pipeArray
    set pipeArray($pipeRecord) [ lreplace $pipeArray($pipeRecord) $pipeField $pipeField $pData ]
}

#
# pipe_getData
# gets a field from the indicated pipe record and returns it to the user
#
proc pipe_getData { pipeRecord pipeField } {
    global pipeArray
    return [ lindex $pipeArray($pipeRecord) $pipeField ]
}

#
# pipe_isOpen
# returns 1 if the pipe is a valid, open pipe, and 0 otherwise
#
proc pipe_isOpen { pipeNum } {
    global pipeArray
    return [ lindex $pipeArray($pipeNum) 4 ]
}

#
# pipe_valid
# returns 1 if the pipe record is valid and initialized, 0 otherwise 
#
proc pipe_valid { pipeNum } { 
    global numPipes pipeArray
    if { $pipeNum >= $numPipes } { return 0 }
    if [catch {set nothing $pipeArray($pipeNum) }] {return 0}
    if {[string length [pipe_getData $pipeNum 2]] == 0} {
      return 0
      }
    return 1
}

#
# pipe_Init
# Create our default 3 pipes (TCL, Scheme, and Unix [csh]) and 
#   corresponding buffers
#
# * Must be only called once (or returns error)
# * Calls pipe_Open for scheme and csh, creates tcl pipe "by hand" by 
#   calling pipe_Create directly
#
proc pipe_Init { TCLBuffer SchemeBuffer cshBuffer } {
    global numPipes BufferArray
    if {$numPipes != 1} { error "pipe_Init being called twice" }
    
    pipe_nodeCreate 0
    
    set BufferArray($TCLBuffer,name) "*TCL"
    if [ catch { pipe_Create $TCLBuffer } cerr ] { 
	    error "Couldn't create TCL buffer. ($cerr)"
    }
    
    pipe_setData 0 0 "tcl"
    pipe_setData 0 2 $TCLBuffer
    pipe_setData 0 4 1
    edit_setPipe $TCLBuffer 0
    
    set pipeNum [ pipe_Open  scheme $SchemeBuffer ]
    if { $pipeNum != 1 } { 
	error "pipe_init called twice?" 
    }
    set pipeNum [ pipe_Open csh $cshBuffer]
    if { $pipeNum != 2 } { 
	error "pipe_init called twice?" 
    }
}

#
# pipe_Destroy
# Permanently removes a pipe, and its corresponding buffer.
#
# * Calls pipe_Close to close the pipe if still open.
# * Calls edit_close to close the corresponding buffer. 
#
proc pipe_Destroy { buffer } {
    set myPipe [edit_getPipe $buffer]
    if { $myPipe == -1} { 
	    error "pipe_Destroy called on non-initialized or non-pipe buffer"
    }
    if { $myPipe == 0 } {
	    error "pipe_Destroy called on TCL pipe"
    }
    if {[pipe_isOpen $myPipe] == 1} { 
	    if [ catch { pipe_Close $myPipe } err ] {
	      error "Couldn't run pipe_Close on $myPipe ($err)" 
        }
      }
    edit_close $buffer
    pipe_setData $myPipe 2 ""
}

#
# pipe_Create
# Uses edit_Create to create a buffer for a new pipe, and handles special
#   bindings for pine windows (control-Return and alt-Return)
#
# * Sets bindings to call pipe_Submit and pipe_submitSelected
#
proc pipe_Create { buffer } {
    if [catch { edit_create $buffer pipe } crerr] { 
      error "$crerr in pipe_Create" 
      }
    set bpath .d.c.buffer$buffer
    $bpath mark set p-o-e 1.0
    $bpath mark gravity p-o-e left
    bind $bpath <Control-Return> "pipe_Submit $buffer"
    bind $bpath <Alt-Return> "pipe_submitSelected $buffer"
}

#
# pipe_Open 
# Create a new pipe in a buffer, and returns the new pipe's #
#
# * Contains a few derivations from Danskin's "ReadWritePipe"
# * Calls pipe_Create to create the buffer corresponing to the pipe
# * Standard paths for the csh and Scheme pipes are defined here for
#   the Dec Alphas.
#
proc pipe_Open { pipeType bufferName } {
    global numPipes BufferArray
    
    pipe_nodeCreate $numPipes
    
    set BufferArray($bufferName,name) "*$pipeType"
    
    # we're limitted to a singel TCL pipe for simplicity's sake
    if { [string compare $pipeType "tcl"] == 0 } {
	    error "User tried to create additional TCL pipes"
    }
    
    pipe_setData $numPipes 0 $pipeType
    
    if [ catch { pipe_Create $bufferName } err ] {
	    error "Couldn't create buffer for new pipe. ($err)"
    }
    
    pipe_setData $numPipes 2 $bufferName
    edit_setPipe $bufferName $numPipes
    
    set toCreate $pipeType 
    if { [string compare $toCreate "scheme"] == 0 } {
	    set toCreate "/usr/local/bin/scheme"
    } 
    
    if { [string compare $toCreate "csh"] == 0 } {
	    set toCreate "/usr/ucb/csh -i"
    }
    
    set fileHandle 0
    # Derived from Danskin's example
    if [catch {open "|$toCreate" r+} fileHandle] {
  	  error "Couldn't Create Pipe for $toCreate in pipe_Open. ($fileHandle)"
    } else {
	    pipe_setData $numPipes 1 $fileHandle
    }
    
    pipe_setData $numPipes 3 $toCreate
    pipe_setData $numPipes 4 1
    
    fileevent $fileHandle readable "pipe_Read $numPipes"
    set pnum $numPipes
    incr numPipes
    return $pnum
}



#
# pipe_reOpen
# reOpens closed standard pipe
#
# * pipe must not have been destroyed, but rather should have closed due
#   to a fileevent or pipe_Close call
#
proc pipe_reOpen { pipeNum } {
    if { [pipe_valid $pipeNum] == 0 } {
      error "Called reOpen on an invalid pipe!"
      }
    
    if [ pipe_getData $pipeNum 4 ] {
	    error "Called reOpen on an open pipe!"
    }

    if { $pipeNum > 2 } {
      error "Called reOpen on a user-created pipe!"
      }
    
    # taken from Danskin's example
    set toCreate [pipe_getData $pipeNum 3]
    if [catch {open "|$toCreate" r+} fileHandle] {
    	error "Couldn't Create Pipe for $toCreate in pipe_Open: $fileHandle"
    } else {
	    pipe_setData $pipeNum 1 $fileHandle
    	pipe_setData $pipeNum 4 1
    }
    fileevent $fileHandle readable "pipe_Read $pipeNum"
    bind $bpath <Control-Return> "pipe_Submit $buffer"
    bind $bpath <Alt-Return> "pipe_submitSelected $buffer"
}

#
# pipe_Read
# Called by fileevents to handle reads or shutdowns on pipes.
#
# * Contains a few minor derivations from Danskin's "ReadWritePipe"
# * If a pipe shuts down, and it's a standard pipe, reOpen it.
#   Otherwise, make sure it's closed, and remove the bindinds.
# * If there's something to be read, read it and append it to the text
#   in the corresponding buffer...
# 
proc pipe_Read { pipenum } {
    global numPipes
    if {[pipe_valid $pipenum] == 0} {
      error "Trying to read invalid pipe (#$pipenum)"
      }
    
    if {[pipe_isOpen $pipenum] == 0 } {
      error "Trying to read a closed pipe! (#$pipenum)"
      }
    
    if {$pipenum == 0} { error "Trying to read the TCL pipe directly!" }
    
    set fileHandle [pipe_getData $pipenum 1]  
    
    #taken from danskin's example, pretty much
    if [eof $fileHandle ] { 
	    if [ catch { pipe_Close $pipenum } err ] { 
	      error "Couldn't close empty pipe! ($err)" 
	    }
	    return
    }
    set readText [read $fileHandle 1]
    set bufNum [pipe_getData $pipenum 2]
    set bpath .d.c.buffer$bufNum
    $bpath insert end $readText
    $bpath mark set p-o-e "end - 1 c"
    $bpath mark set insert "end - 1 c"
    $bpath see insert
}

#
# pipe_Close
# Close a pipe, and unbind the two command keystrokes.
#
# * Kills the child process, if any.
# * Closes the pipe pseudo-file.
# * Unbinds the command keystrokes,
# * Marks the pipe as closed.
#
proc pipe_Close { pipeNum } {
    if {$pipeNum == 0} {return}
    set fileHandle [pipe_getData $pipeNum 1]
    set ppid [pid $fileHandle]
    catch {exec "kill" $ppid}
    if [catch { close $fileHandle } err ] {
         catch { edit_Append [pipe_getData $pipeNum 2] "Pipe Returned: $err" }
         }
    set bpath [pipe_getData $pipeNum 2]
    bind $bpath <Control-Return> {}
    bind $bpath <Alt-Return> {}
    pipe_setData $pipeNum 4 0
}


#
# pipe_Shutdown
# Shuts down the pipe subsystem
#
# * Calls pipe_Destroy on all pipes' corresponding buffers.
# * Should only be called once, on shutdown.
# * The TCL pipe, and its corresponding buffer, are never destroyed.
#
proc pipe_Shutdown { } {
    global numPipes
    if {$numPipes == 1} { error "No pipes are open to shut down!" }
    
    for { set i 1 } { $i < $numPipes } { incr i } {
	    if {[pipe_getData $i 2] != ""} {
	      pipe_Destroy [pipe_getData $i 2]
	      }
      }
    set numPipes 1
}

#
# pipe_submitSelected
# Submit the selected text from a buffer to its associated pipe
#
# * Does nothing if operated upon a closed pipe
#
proc pipe_submitSelected { bufNum } {
  if { [edit_isBuffer $bufNum] == 0 } {
    error "pipe_submitSelected called on an invalid buffer #$bufNum"
  }
  set pipeNumber [edit_getPipe $bufNum]
  if { [pipe_valid $pipeNumber ] == 0 } {
    error "pipe_submitSelected called upon an invalid pipe: #$bufNum"
    }
      
  if { [pipe_isOpen $pipeNumber ] == 0 } {
    return
    }
     
  if [ catch { edit_extractSelection $bufNum } submission ] { 
    error "Couldn't extract text from buffer."
  }
    
  if { [string length $submission] > 1 } {
  	pipe_Write $pipeNumber $submission
  }
  .d.c.buffer$bufNum mark set p-o-e "end - 1 c"
  .d.c.buffer$bufNum mark set insert "end - 1 c"
 
}

#
# pipe_Submit
# Submit the selected text from a buffer to its associated pipe
#
# * Should do nothing if operated upon a closed pipe.
#
proc pipe_Submit { bufNum } {
  if { [ edit_isBuffer $bufNum ] == 0 } {
	  error "pipe_Submit called on an invalid buffer"
  }
    
  set pipeNumber [edit_getPipe $bufNum]
  if { [pipe_valid $pipeNumber ] == 0 } {
    error "pipe_Submit called upon an invalid pipe: #$bufNum"
  }
      
  if { [pipe_isOpen $pipeNumber ] == 0 } {
    return
  }
    
  if [ catch { .d.c.buffer$bufNum get p-o-e end } submission ] {
  	error "Couldn't extract text from buffer. ($submission)"
  }
  pipe_Write $pipeNumber $submission
  .d.c.buffer$bufNum mark set p-o-e "end - 1 c"
  .d.c.buffer$bufNum mark set insert "end - 1 c"
}

#
# pipe_Write
# Sends text to the program on a pipe. 
#
# * reOpens one of the standard pipes, if closed, and returns error if
#   called on a closed pipe other than one of the standard pipes.
# * Tcl pipe only: Appends the return value or error from the tcl code
#                  being run/submitted...
# * Calls flush to make sure data is actually submitted to a program  
#
proc pipe_Write { pipeNum toWrite } {
    if {[string length $toWrite] <= 0} { error "Invalid text string." }
    
    if {[pipe_valid $pipeNum] == 0} { error "Invalid pipe." }
    
    if {$pipeNum == 0} {
    	set TCLString [ pipe_toTCL $toWrite ] 
	    edit_Append [pipe_getData 0 2] $TCLString
	    return
    }
    
    if {[pipe_isOpen $pipeNum] == 0} {
	    if {$pipeNum < 3} {
	      if [catch {pipe_reOpen $pipeNum}] {
		      error "Couldn't reopen closed standard pipe!"  
	        }
	      } else { 
	        error "Tried to write to closed user-created pipe!" 
	      }
    }
    
    if [catch {puts -nonewline [pipe_getData $pipeNum 1] $toWrite}] {
	    error "Error writing to pipe #$pipeNum"
    }
    
    if [catch {flush [pipe_getData $pipeNum 1]}] {
	    error "Error writing to pipe #$pipeNum"
    }
}

#
# pipe_toTCL
# Sends a command or script to the Tcl interpreter, executes it at the top
#   level and returns the response or error message(s).
#
proc pipe_toTCL { forEval } {
    global toEval outEval
    if {[string length $forEval] < 1} return ""
    set toEval $forEval
    uplevel #0 { catch "$toEval" outEval }
    return $outEval
}

#
# text_StoTCL
# Sends the selected text from a buffer to the Tcl interpreter. 
#
# * Since the Tcl pipe can't close, should always work.
# * Refuses (silently) to submit a blank/empty selection.
#
proc text_StoTCL { inBuffer } {
    global numPipes
    if {[edit_isBuffer $inBuffer] == 0} {
	    error "text_StoTCL called on invalid buffer"
    }
    
    if {$numPipes < 3} {
	    error "text_StoTCL called without initialized pipes"
    }
    
    set selected [ edit_extractSelection $inBuffer ]
    if {[ string length $selected ] < 2 } return
  
    if [catch {pipe_Write 0 $selected } cerr ] {
	    error "Couldn't send buffer $inBuffer to TCL ($cerr)"
    }
}

#
# text_StoSCHEME
# Sends the selected text from a buffer to the Scheme pipe.
#
# * The Scheme pipe should reOpen automatically.
# * Refuses (silently) to submit a blank/empty selection.
#
proc text_StoSCHEME { inBuffer } {
    global numPipes
    
    if {[edit_isBuffer $inBuffer] == 0} {
	    error "text_StoSCHEME called on invalid buffer"
    }
    
    if {$numPipes < 3} {
	    error "text_StoSCHEME called withoug initialized pipes"
    }
    
    if {[pipe_isOpen 1] == 0} {
	    if [catch {pipe_reOpen 1}] { 
	      error "Couldn't reopen Scheme pipe."
	    }
    }
      
    set selected [ edit_extractSelection $inBuffer ]
    if {[ string length $selected ] < 2 } return
    if [catch {pipe_Write 1 $selected } cerr ] {
	    error "Couldn't send buffer $inBuffer to Scheme ($cerr)"
    }
}

#
# text_StoUNIX
# Sends the selected text from a buffer to a new Unix pipe, which is
#   created in a buffer provided by the buffer manager.
#
# * Refuses (silently) to submit a blank/empty selection, but finishes
#   creating the new buffer anyway.
#
proc text_StoUNIX { inBuffer outBuffer } {
    global numPipes
    if {[edit_isBuffer $inBuffer] == 0} {
	    error "text_StoUNIX called on invalid buffer"
    }
    
    if {$numPipes < 3} {
	    error "text_StoUNIXcalled on unititialized pipes"
    }
    ~
    if {[edit_isBuffer $outBuffer] == 1} {
    	error "Buffer $outBuffer already exists in toUnix"
    }

    if [catch {pipe_Open "csh" $outBuffer} pnum] {
	    error "Couldn't create pipe for new unix window."
    }

    if [catch {pipe_Open "csh" $outBuffer} pnum] {
	    error "Couldn't create pipe for new unix window."
    }

    set selected [ edit_extractSelection $inBuffer ]
    if {[ string length $selected ] < 2 } return
    if [catch {pipe_Write $pnum $selected } cerr ] {    
	    error "Couldn't sent buffer $inBuffer to UNIX ($cerr)"
    }
}

#
# text_BtoTCL
# Sends the complete text of a buffer to the Tcl interpreter. 
#
# * Since the Tcl pipe can't close, should always work.
# * Refuses (silently) to submit a blank/empty buffer.
#
proc text_BtoTCL { inBuffer } {
    global numPipes
    if {[edit_isBuffer $inBuffer] == 0} {
	error "text_toTCL called on invalid buffer"
    }
    if {$numPipes < 3} {
	error "text_toTCL called on unititialized pipes"
    }
    if [catch {pipe_fromBuffer 0 $inBuffer } cerr ] {
	error "couldn't send buffer $inBuffer to TCL ($cerr)"
    }
}

#
# text_BtoSCHEME
# Sends the complete text of a buffer to the Scheme pipe.
#
# * The Scheme pipe should reOpen automatically.
# * Refuses (silently) to submit a blank/empty selection.
#
proc text_BtoSCHEME { inBuffer } {
  global numPipes
  if {[edit_isBuffer $inBuffer] == 0} {
	    error "text_toSCHEME called on invalid buffer"
  }
  
  if {$numPipes < 3} {
	  error "text_toSCHEME called on unititialized pipes"
  }
  
  if {[pipe_isOpen 1] != 1} {
	  if [catch {pipe_reOpen 1}] { 
      error "Couldn't reopen Scheme pipe."
	  }
  }  
  
  if [catch { pipe_fromBuffer 1 $inBuffer } cerr ] {
    error "Couldn't sent buffer $inBuffer to Scheme ($cerr)"
  }
}

#
# text_BtoUNIX
# Sends the complete text of a buffer to a new Unix pipe, which is
#   created in a buffer provided by the buffer manager.
#
# * Refuses (silently) to submit a blank/empty selection, but finishes
#   creating the new buffer anyway.
#
proc text_BtoUNIX { inBuffer outBuffer } {
    global numPipes
    if {[edit_isBuffer $inBuffer] == 0} {
    	error "text_BtoUNIX called on invalid buffer"
    }
    if {$numPipes < 3} {
	    error "text_BtoUNIX called on unititialized pipes"
    }
    
    if {[edit_isBuffer $outBuffer] == 1} {
	    error "Buffer $outBuffer already exists in toUnix"
    }

    if [catch {pipe_Open "csh" $outBuffer} pnum] {
	    error "Couldn't create buffer for new unix window. ($pnum)"
    }
    
    if [catch {pipe_fromBuffer $pnum $inBuffer } cerr ] {
	    error "Couldn't sent buffer $inBuffer to UNIX ($cerr)"
    }
}

#
# pipe_fromBuffer
# Sends the complete contents of a buffer down a pipe.
#
# * Sends the contents of the buffer as a single string...
# * Won't do anything if buffer is empty. (just returns doing nothing)
#
proc pipe_fromBuffer { pipeNumber bufferNumber } {
    if {[edit_isBuffer $bufferNumber] == 0} {
	    error "pipe_fromBuffer called with invalid buffer #$bufferNumber"
    }
    
    if {[pipe_valid $pipeNumber] == 0} { 
      error "pipe_fromBuffer to bad pipe: #$pipeNumber" 
      }
      
    set extractedText [ .d.c.buffer$bufferNumber get 1.0 end ]
    
    if {[string length $extractedText] < 1} {
      return
    }
      
    pipe_Write $pipeNumber $extractedText
}
