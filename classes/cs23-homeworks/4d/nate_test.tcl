#!./ttt3D
#
# nate_test.tcl based on
# ttt_test.tcl
# - Nate Edel, 4/24/96
#
# Tests ttt3D and won3D command implementations, very simply, by
# feeding won a set of known boards and by running ttt on all
# possible start positions for X.
#

proc display {bd} { 
  for {set th 0 } {$th < 4} {incr th} {
    for {set ti 0} {$ti < 4} {incr ti } {
      for {set tj 0} {$tj < 4} {incr tj} {
        set x [string index $bd [expr [expr $th * 16] + [expr $tj + [expr $ti * 4]]]]
        if {[string compare $x " "] == 0} {set x "."}
        puts -nonewline $x
        }
      puts " "
      }
    puts " "
    puts " "
    }
  }

proc build {bn} {
  set outvalue ""
  for {set i 0} {$i < 64} {incr i} {
    if { $i == $bn} { append outvalue "x" } else { append outvalue " "}
    }
  return $outvalue
  }

set nboards 26
source wins.tcl

puts "Output test of won, should show 26 characters, alternating X and 0"
for {set i 0} {$i < $nboards} {incr i} {
  puts -nonewline [won3D $board($i)]
  }
puts " "

puts "Tests handling of a short board by ttt3d"
if {[catch {ttt3d "   "}]} {
  puts "Yup... found a short board."
  }

puts "Tests handling of an board containing invalid characters by ttt3d"
if {[catch {ttt3d "                               !                                "}]} {
  puts "Yup... found a bad character."
  }
  
puts "Tests handling of an board containing a win by ttt3d"
if {[catch {ttt3d $board(6)}]} {
  puts "Yup... an uncaught win"
  }  
  
puts "Output test of all possible starting first moves, with their response after one move."
puts "Should also show that won3D will detect non-winning boards as non-winning by showing"
puts "Not done: N for each position... this will take a long time!"
for {set i 0} {$i < 64} {incr i} {
  set gbd [ build $i ]
  puts "Displaying Starting position #$i"
  set gbd [ttt3D $gbd]
  puts " "
  display $gbd
  set win [won3D $gbd]
  puts "Not done: $win"
  puts " "
  }
