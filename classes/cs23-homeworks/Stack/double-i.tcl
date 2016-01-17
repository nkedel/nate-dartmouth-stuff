#!/usr/local/bin/tclsh
#
# Silly little game.
#

proc IsOkAnswer {Thing} {
  set ThingLength [ string length $Thing ]
	if [ expr $ThingLength == 0 ] then { 
	  puts "Bummer dude, but you have to enter something!" 
	  return 0 
	  } 
  for {set ScanCounter 0} {$ScanCounter < $ThingLength} {incr ScanCounter} {
    if { [ string index $Thing $ScanCounter ] == [ string index $Thing [expr $ScanCounter + 1 ] ] } {
      puts "Okay!"
      return 1
      }
    }
  puts "No, but you can bring pizza."
	return 0
	}

set IsOkFlag 0
puts "I'm going to a party." 
while { $IsOkFlag == 0 } {
  puts -nonewline "What do you want to bring? "
  gets stdin Item
  set IsOkFlag [ IsOkAnswer [string toupper $Item ] ]
  }
if { [ string toupper $Item ] == "PIZZA" } { puts "How utterly uncreative." }
