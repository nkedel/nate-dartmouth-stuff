#!/usr/local/bin/tclsh
#
# Silly little game.
#

proc IsOkAnswer {Thing} {
  set ThingLength [ string length $Thing ]
	if [ expr $ThingLength == 0 ] then { 
	  return 0 
	  } 
  for {set ScanCounter 0} {$ScanCounter < $ThingLength} {incr ScanCounter} {
    if { [ string index $Thing $ScanCounter ] == [ string index $Thing [expr $ScanCounter + 1 ] ] } {
         return 1
         }
    }
	return 0
	}

set IsOkFlag 0
puts "Enter a blank line to end."
while {1} {
  puts -nonewline "? "
  gets stdin Item
  if { [ string length $Item ] == 0} break
  set IsOkFlag [ IsOkAnswer [ string toupper $Item ] ]
  if {$IsOkFlag == 1} { puts "yes." } else { puts "no."}
  }

    
