# pipe.tcl
# internal module, include with "source pipe.tcl"
#
# $Header: /usr/tahoe1/nate/prj/pipe/RCS/pipe.old.tcl,v 1.1 1996/05/24 05:26:16 nate Exp $
# by Nate Edel
#
# First limpable version to hand in.
#

set pipes(0) ""
set npipes 1
set buffers(0) ""
set btypes(0) ""
set bpipes(0) ""


proc edit_setPipe { brec pipeno } {
  global bpipes
  set bpipes($brec) $pipeno
  }

proc edit_getPipe { brec } {
  global bpipes
  return $bpipes($brec)
  } 

proc pipe_nodeCreate { prec } {
  global pipes
  set pipes($prec) [ list {nothing} {-1} {-1} {nothing} {0} ]
  }

proc pipe_setData { prec pfld pdat } {
  global pipes
  set pipes($prec) [ lreplace $pipes($prec) $pfld $pfld $pdat ]
  }
  
proc pipe_getData { prec pfld } {
  global pipes
  return [ lindex $pipes($prec) $pfld ]
  }

proc pipe_Open? { pipeNum } {
  global pipes
  return [ lindex pipes($prec) 4 ]
  }

proc edit_Create { bname btype } {
  global buffers
  global btypes
  set buffers($bname) .buf$bname
  text .buf$bname -state normal -width 80 -height 10
  set btypes($bname) $btype
  pack .buf$bname
  }


proc edit_Extract { bufNum st en } {
  global buffers
  set bpath $buffers($bufNum)
  set rvalue [ $bpath get $st $en ]
  puts $rvalue
  return $rvalue 
  }

proc edit_Append { bufNum atext } {
  global buffers
  set bpath $buffers($bufNum)
  $bpath insert end $atext
  }

#
# Create Our default 3 pipes
#
proc pipe_Init { TCLBuffer SchemeBuffer cshBuffer } {
  global pipes npipes
# also the buffer array
  if {$npipes != 1} { error "pipe_Init being called twice" }
  
  pipe_nodeCreate 0
  
  if [ catch { pipe_Create $TCLBuffer  } ] { 
    error "Couldn't create TCL buffer."
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

proc pipe_Create { buffer } {
  global buffers
  if [catch { edit_Create $buffer IN } crerr] { error "$crerr in pipe_Create" }
  set bpath $buffers($buffer)
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
  global npipes pipes

  pipe_nodeCreate $npipes

# we're limitted to a singel TCL pipe for simplicity's sake
  if { [string compare $pipeType "tcl"] == 0 } {
    error "User tried to create additional TCL pipes"
    }

  pipe_setData $npipes 0 $pipeType
  
  if [ catch { pipe_Create $bufferName } ] {
    error "Couldn't create buffer for new pipe."
    }
  pipe_setData $npipes 2 $bufferName
  edit_setPipe $bufferName $npipes
  
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
      pipe_setData $npipes 1 $fileHandle
      }
      
  pipe_setData $npipes 3 $toCreate
  pipe_setData $npipes 4 1
  
  fileevent $fileHandle readable "pipe_Read $npipes"
  set pnum $npipes
  incr npipes
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
  global npipes pipes buffers
  
  if {$pipenum == 0} { error "Trying to read the TCL pipe directly!" }

  set fileHandle [pipe_getData $pipenum 1]  

  #taken from danskin's example, pretty much
  if [eof $fileHandle ] { 
    if [ catch { pipe_Close $pipenum } err ] { error "Couldn't close empty pipe!: $err" }
    if { $pipenum < 3 } { 
#      if [catch {
      pipe_reOpen $pipenum
# } err] {
#        error "Couldn't reOpen standard pipe! ($err)"
#        }
      }
    return

    }
  set readText [read $fileHandle 1]
  set bufNum [pipe_getData $pipenum 2]
  set bpath $buffers($bufNum)
  $bpath insert end $readText
  $bpath mark set p-o-e "end - 1 c"
  $bpath mark set insert "end - 1 c"
  $bpath see insert
  }

#
#
#
proc pipe_Close { pipeNum } {
#  if {!eof [pipe_getData $pipeNum 1]} {
#    [catch pipe_Read $pipeNum]
#    }
  set fileHandle [pipe_getData $pipeNum 1]
  if [catch { close $fileHandle } err ] {
    edit_Append [pipe_getData $pipenum 2] "Pipe Returned: $error"
#   error "Couldn't close pipe #$pipeNum: $err"
    }
  pipe_setData $pipeNum 4 0
  }

  
#
# pipe_Shutdown
#
proc pipe_Shutdown { } {
  global npipes pipes
  
  if [$npipes == 1] { error "No pipes are open to shut down!" }
  
  for { set i 1 } { $i < $npipes } { incr i } {
    pipe_Close $i
    }
  }
  
#
# pipe_Copy -- can wait
# pipe_Paste
#

#
# Submit the selected text from a buffer to its associated pipe
#
proc pipe_submitSelected { bufNum } {
  global pipes
#  if { [edit_isBuffer? $bufName] == 0 } {
#    error "pipe_submitSelected called on an invalid buffer"
#    }
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
  global pipes buffers
#  if { [ edit_isBuffer? $bufName ] == 0 } {
#    error "pipe_Submit called on an invalid buffer"
#    }
  if [ catch { edit_Extract $bufNum p-o-e end } submission ] {
    error "couldn't extract text from buffer: $submission"
    }
  pipe_Write [edit_getPipe $bufNum] $submission
  set bpath $buffers($bufNum)
  $bpath mark set p-o-e insert
  }

#
#
#  
proc pipe_Write { pipeNum toWrite } {
  puts "pipe_Write on #$pipeNum: $toWrite"
  if {[string length $toWrite] <= 0} { error "Invalid text string." }
#  if {!pipe_Valid? $pipeNum} { error "Invalid pipe." }
  if {$pipeNum == 0} {
    set TCLString [ pipe_toTCL $toWrite ] 
    edit_Append [pipe_getData 0 2] $TCLString
    return
    }
#  if {!pipe_Open? $pipeNum} {
#    if {$pipeNum < 3} {
#      if [catch {pipe_reOpen $pipeNum}] {
#        error "Couldn't reopen closed standard pipe!"  
#        }
#      break
#      } else { error "Tried to write to closed pipe!" }
#    }
  if [catch {puts -nonewline [pipe_getData $pipeNum 1] $toWrite}] {
    error "Error writing to pipe #$pipeNum"
    }
  if [catch {flush [pipe_getData $pipeNum 1]}] {
    error "Error writing to pipe #$pipeNum"
    }
  
  }

proc pipe_toTCL { toEval } {
  if {[string length $toEval] < 1} return ""
  catch [ $toEval ] outEval
  return $outEval
  }

proc text_toTCL { inBuffer } {
  global npipes
  if {!edit_isBuffer $inBuffer} {
    error "text_toTCL called on invalid buffer"
    }
  if {$npipes < 3} {
    error "text_toTCL called on unititialized pipes"
    }
  if [catch {pipe_fromBuffer 0 $inBuffer } cerr ] {
    error "couldn't sent buffer $inBuffer to TCL ($cerr)"
    }
  }

proc text_toSCHEME { inBuffer } {
  global npipes
  if {!edit_isBuffer? $inBuffer} {
    error "text_toSCHEME called on invalid buffer"
    }
  if {$npipes < 3} {
    error "text_toSCHEME called on unititialized pipes"
    }
  if {!pipe_Open? 1} {
    if [catch {pipe_reOpen 1}] { 
      error "Couldn't reopen Scheme pipe."
      }
    }  
  if [catch {pipe_fromBuffer 1 $inBuffer } cerr ] {
    error "couldn't sent buffer $inBuffer to Scheme ($cerr)"
    }
  }

proc text_toUNIX { inBuffer outBuffer } {
  global npipes
  if {!edit_isBuffer? $inBuffer} {
    error "text_toUNIX called on invalid buffer"
    }
  if {$npipes < 3} {
    error "text_toUNIXcalled on unititialized pipes"
    }
    
  if {edit_isBuffer? $outBuffer} {
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

pipe_Init 2 3 4
