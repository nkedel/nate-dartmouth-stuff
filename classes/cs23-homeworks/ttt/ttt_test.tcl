#!./ttt
#
# ttt_test.tcl
# - Nate Edel, 4/24/96
#
# Tests ttt and won command implementations, very simply, by
# feeding won a set of known boards and by running ttt on all
# possible start positions for X.
#

proc display {bd} { 
  for {set ti 0} {$ti < 3} {incr ti } {
    for {set tj 0} {$tj < 3} {incr tj} {
      puts -nonewline [string index $bd [expr $tj + [expr $ti * 3]]]
      }
    puts " "
    }
  }

proc build {bn} {
  set outvalue ""
  for {set i 0} {$i < 9} {incr i} {
    if { $i == $bn} { append outvalue "X" } else { append outvalue " "}
    }
  return $outvalue
  }

proc play {bd} {
  while {[won $bd] == "N"} {
    set bd [ttt $bd]
    }
  return $bd
  }

set board(0) "XO  X  OX"
set board(1) "O X X X O"
set board(2) "XXX    OO"
set board(3) "OO    XXX"
set board(4) "O  XXX  O"
set board(5) "X  X OX O"
set board(6) "  XO XO X"
set board(7) "OX  X  XO"
set board(8) "OX XO  XO"
set board(9) "XXO O O X"
set board(10) "OOO  X XX"
set board(11) "XX X  OOO"
set board(12) "XX OOO  X"
set board(13) "O  OXXO X"
set board(14) "X OX O XO"
set board(15) "XOX O  OX"
set board(16) "XOXXOXOXO"
set board(17) "XXOOXXXOO"
set board(18) "OXOOXOXOX"
set board(19) "OOXXOOOXX"
set board(20) "XOXO     "
set board(21) "XX     OO"
set board(22) "XOOXX OXO"
set board(23) "XX  OOXXO"
set nboards 24

puts "Output test of won, should show 8 X's, 8 O's, 4 T's and 4 N's"
for {set i 0} {$i < $nboards} {incr i} {
  puts -nonewline [won $board($i)]
  }
puts " "

puts "Output test of all possible starting first moves, with their endings."
for {set i 0} {$i < 9} {incr i} {
  set gbd [ build $i ]
  puts "Displaying Starting position #$i"
  display $gbd
  set gbd [play $gbd]
  puts " "
  display $gbd
  set win [won $gbd]
  puts "Win/Lose/Tie: $win"
  puts " "
  }
