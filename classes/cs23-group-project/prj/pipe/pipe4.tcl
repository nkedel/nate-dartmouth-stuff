# pipe.tcl
# internal module, include with "source pipe.tcl"
#
# $Header: /a/tahoe/usr/tahoe1/nate/prj/pipe/RCS/pipe.tcl,v 2.3 1996/05/24 23:47:19 nate Exp nate $
# by Nate Edel
#

set numPipes 1
set pipeArray(0) ""
set toEval ""
set outEval ""

proc pipe_nodeCreate { pipeRecord } {
  global pipeArray
  set pipeArray($pipeRecord) [ list {nothing} {-1} {-1} {nothing} {0} ]
  }

proc pipe_setData { pipeRecord pipeField pData } {
  global pipeArray
  set pipeArray($pipeRecord) [ lreplace $pipeArray($pipeRecord) $pipeField $pipeField $pData ]
  }
  
proc pipe_getData { pipeRecord pipeField } {
  global pipeArray
  return [ lindex $pipeArray($pipeRecord) $pipeField ]
  }

proc pipe_Open? { pipeNum } {
  global pipeArray
  return [ lindex pipeArray($pipeNum) 4 ]
  }

proc pipe_valid { pipeNum } { 
  return 1
  }

#
# Create Our default 3 pipes
#
proc pipe_Init { TCLBuffer SchemeBuffer cshBuffer } {
  global numPipes
  if {$numPipes != 1} { error "pipe_Init being called twice" }
  
  pipe_nodeCreate 0
  
  if [ catch { pipe_Create $TCLBuffer  } cerr ] { 
    error "Couldn't create TCL buffer. ($cerr)"
    }

  pipe_setData 0 0 "tcl"
  pipe_setData 0 2 $TCLBuffer
  pipe_setData 0 4 1
  edit_setPipe $TCLBuffer 0
    
  set pipeNum [ pipe_Open  scheme $SchemeBuffer ]
  if { $pipeNum != 1 } { error "pipe_init called twice?" }
  set pipeNum [ pipe_Open csh $cshBuffer]
  if { $pipeNum != 2 } { error "pipe_init called twice?" }
  }

proc pipe_Destroy { buffer } {\
  set myPipe [edit_getPipe $buffer]
  if { $myPipe == -1} { 
    error "pipe_Destroy called on non-initialized or non-pipe buffer"
    }
  if {[pipe_Open? $myPipe] == 1} { 
     catch { pipe_Close $myPipe }
     }
  edit_close $buffer
  pipe_setData $myPipe 2 ""
  }

proc pipe_Create { buffer } {
  if [catch { edit_create $buffer pipe } crerr] { error "$crerr in pipe_Create" }
  set bpath .d.c.buffer$buffer
  $bpath mark set p-o-e 1.0
  $bpath mark gravity p-o-e left
  bind $bpath <Control-Return> "pipe_Submit $buffer"
  }
  
# 
# Create a new pipe in a buffer
# returns a pipe #
#
# Contains a few minor derivations from Danskin's "ReadWritePipe"
#
proc pipe_Open { pipeType bufferName } {
  global numPipes 

  pipe_nodeCreate $numPipes

# we're limitted to a singel TCL pipe for simplicity's sake
  if { [string compare $pipeType "tcl"] == 0 } {
    error "User tried to create additional TCL pipes"
    }

  pipe_setData $numPipes 0 $pipeType
  
  if [ catch { pipe_Create $bufferName } ] {
    error "Couldn't create buffer for new pipe."
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
  # taken from Danskin's example
  if [catch {open "|$toCreate" r+} fileHandle] {
    error "Couldn't Create Pipe for $toCreate in pipe_Open"
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
# reOpens closed standard pipe
#
proc pipe_reOpen { pipeNum } {
  if [ pipe_getData $pipeNum 4 ] {
    error "Called reOpen on an open pipe!"
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
  }
  
#
# pipe_Read
#
# Contains a few minor derivations from Danskin's "ReadWritePipe"
#
proc pipe_Read { pipenum } {
  global numPipes
  if {$pipenum == 0} { error "Trying to read the TCL pipe directly!" }

  set fileHandle [pipe_getData $pipenum 1]  

  #taken from danskin's example, pretty much
  if [eof $fileHandle ] { 
    if [ catch { pipe_Close $pipenum } err ] { error "Couldn't close empty pipe!: $err" }
#    if { $pipenum < 3 } { 
#      if [catch {
#     pipe_reOpen $pipenum
# } err] {
#        error "Couldn't reOpen standard pipe! ($err)"
#        }
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
#
proc pipe_Close { pipeNum } {
  if {$pipeNum == 0} {return}
  set fileHandle [pipe_getData $pipeNum 1]
  if [catch { close $fileHandle } err ] {
    edit_Append [pipe_getData $pipenum 2] "Pipe Returned: $error"
    }
  pipe_setData $pipeNum 4 0
  }

  
#
# pipe_Shutdown
#
proc pipe_Shutdown { } {
  global numPipes
  
  if {$numPipes == 1} { error "No pipes are open to shut down!" }
  
  for { set i 1 } { $i < $numPipes } { incr i } {
    pipe_Destroy $i
    }
  
  set numPipes 1
  }
  
#
# Submit the selected text from a buffer to its associated pipe
#
proc pipe_submitSelected { bufNum } {
  global pipes
  if { [edit_isBuffer $bufNum] == 0 } {
    error "pipe_submitSelected called on an invalid buffer. ($submission)"
    }
  if [ catch { edit_extractSelected $bufNum } submission ] {
    error "Couldn't extract text from buffer."
    }
  if [ [string length $submission] == 0 ] {
    pipe_Write [ edit_getPipe $bufNum ] $submission
    }
  }
  
#
# Submit the selected text from a buffer to its associated pipe
#
proc pipe_Submit { bufNum } {
  global pipeArray
  if { [ edit_isBuffer $bufNum ] == 0 } {
    error "pipe_Submit called on an invalid buffer"
    }
  if [ catch { .d.c.buffer$bufNum get p-o-e end } submission ] {
    error "couldn't extract text from buffer. ($submission)"
    }
  pipe_Write [edit_getPipe $bufNum] $submission
  .d.c.buffer$bufNum mark set p-o-e insert
  }

#
#
#  
proc pipe_Write { pipeNum toWrite } {
  if {[string length $toWrite] <= 0} { error "Invalid text string." }
  if {[pipe_valid $pipeNum] == 0} { error "Invalid pipe." }
  if {$pipeNum == 0} {
    set TCLString [ pipe_toTCL $toWrite ] 
    edit_Append [pipe_getData 0 2] $TCLString
    return
    }
  if {[pipe_Open? $pipeNum] == 0} {
    if {$pipeNum < 3} {
      if [catch {pipe_reOpen $pipeNum}] {
        error "Couldn't reopen closed standard pipe!"  
        }
      break
      } else { error "Tried to write to closed pipe!" }
    }
  if [catch {puts -nonewline [pipe_getData $pipeNum 1] $toWrite}] {
    error "Error writing to pipe #$pipeNum"
    }
  if [catch {flush [pipe_getData $pipeNum 1]}] {
    error "Error writing to pipe #$pipeNum"
    }
  }

proc pipe_toTCL { forEval } {
  global toEval outEval
  if {[string length $forEval] < 1} return ""
  set toEval $forEval
  uplevel #0 { catch {$toEval} outEval }
  return $outEval
  }

proc text_StoTCL { inBuffer } {
  global numPipes
  if {[edit_isBuffer $inBuffer] == 0} {
    error "text_toTCL called on invalid buffer"
    }
  if {$numPipes < 3} {
    error "text_toTCL called without initialized pipes"
    }
  set selected [ edit_extractSelected $inBuffer ]
  if {[ string length $selected ] < 2 } return
  if [catch {pipe_write 0 $selected } cerr ] {
    error "couldn't send buffer $inBuffer to TCL ($cerr)"
    }
  }

proc text_StoSCHEME { inBuffer } {
  global numPipes
  if {[edit_isBuffer $inBuffer] == 0} {
    error "text_toSCHEME called on invalid buffer"
    }
  if {$numPipes < 3} {
    error "text_toSCHEME called withoug initialized pipes"
    }
  if {[pipe_Open? 1] == 0} {
    if [catch {pipe_reOpen 1}] { 
      error "Couldn't reopen Scheme pipe."
      }
    }  
  set selected [ edit_extractSelected $inBuffer ]
  if {[ string length $selected ] < 2 } return
  if [catch {pipe_write 1 $selected } cerr ] {
    error "couldn't send buffer $inBuffer to Scheme ($cerr)"
    }
  }

proc text_StoUNIX { inBuffer outBuffer } {
  global numPipes
  if {[edit_isBuffer $inBuffer] == 0} {
    error "text_toUNIX called on invalid buffer"
    }
  if {$numPipes < 3} {
    error "text_toUNIXcalled on unititialized pipes"
    }
    
  if {[edit_isBuffer $outBuffer] != 1} {
    error "buffer $outBuffer already exists in toUnix"
    }

  if [catch {pipe_Create $outBuffer}] {
    error "couldn't create buffer for new unix window."
    }

  if [catch {pipe_Open $outBuffer "csh"} pnum] {
    error "couldn't create pipe for new unix window."
    }

  set selected [ edit_extractSelected $inBuffer ]
  if {[ string length $selected ] < 2 } return
  if [catch {pipe_write $pnum $selected } cerr ] {    
    error "couldn't sent buffer $inBuffer to UNIX ($cerr)"
    }
  }

proc text_BtoTCL { inBuffer } {
  global numPipes
  if {!edit_isBuffer $inBuffer} {
    error "text_toTCL called on invalid buffer"
    }
  if {$numPipes < 3} {
    error "text_toTCL called on unititialized pipes"
    }
  if [catch {pipe_fromBuffer 0 $inBuffer } cerr ] {
    error "couldn't send buffer $inBuffer to TCL ($cerr)"
    }
  }

proc text_BtoSCHEME { inBuffer } {
  global numPipes
  if {[edit_isBuffer $inBuffer] == 0} {
    error "text_toSCHEME called on invalid buffer"
    }
  if {$numPipes < 3} {
    error "text_toSCHEME called on unititialized pipes"
    }
  if {[pipe_Open? 1] != 1} {
    if [catch {pipe_reOpen 1}] { 
      error "Couldn't reopen Scheme pipe."
      }
    }  
  if [catch {pipe_fromBuffer 1 $inBuffer } cerr ] {
    error "couldn't sent buffer $inBuffer to Scheme ($cerr)"
    }
  }

proc text_BtoUNIX { inBuffer outBuffer } {
  global numPipes
  if {[edit_isBuffer $inBuffer] == 0} {
    error "text_toUNIX called on invalid buffer"
    }
  if {$numPipes < 3} {
    error "text_toUNIXcalled on unititialized pipes"
    }
    
  if {[edit_isBuffer $outBuffer] == 1} {
    error "buffer $outBuffer already exists in toUnix"
    }

  if [catch {pipe_Create $outBuffer}] {
    error "couldn't create buffer for new unix window."
    }

  if [catch {pipe_Open $outBuffer "csh"} pnum] {
    error "couldn't create buffer for new unix window."
    }
    
  if [catch {pipe_fromBuffer $pnum $inBuffer } cerr ] {
    error "couldn't sent buffer $inBuffer to UNIX ($cerr)"
    }
  }

proc pipe_fromBuffer { pipeNumber bufferNumber } {
  if {[edit_isBuffer $inBuffer] == 0} {
    error "pipe_fromBuffer called with invalid buffer #"
    }
# if {[pipe_valid $pipeNumber] == 0} { error "pipe_fromBuffer to bad pipe#$pipeNumber" }
  set extractedText [ .d.c.buffer$bufferNumber get start end ]
  if {[string length $extractedText] < 1} return
  pipe_Write $pipeNumber
  }
