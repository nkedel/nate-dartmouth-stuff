#!./nate_ttt3D3d
# TicTacToe in Tk
# - Nathan Edel 4/24/96
#
# Should be self explanatory.
#
# Very derivative of John M. Danskin's calc.tcl

wm title . "3D Tic Tac Toe"

# stolen directly from Danskin's calc.tcl : "Keep track of a new button"
set nbuttons 0

proc NewButton { b } {
  global buttons nbuttons
  set buttons($nbuttons) $b
  incr nbuttons
  }

proc DisplayButtons { board } {
  global buttons asize
  for { set i 0 } { $i < $asize } { incr i } {
    $buttons($i) config -text [string index $board $i]
    }
  }

proc NewRow { wplane wrow } {
  global nbuttons
  frame .main.$wplane.$wrow
  pack  .main.$wplane.$wrow
  NewButton [button .main.$wplane.$wrow.oleft  -relief sunken -width 4 -height 3 -text " " -command "select_spot $nbuttons"]
  NewButton [button .main.$wplane.$wrow.ileft  -relief sunken -width 4 -height 3 -text " " -command "select_spot $nbuttons"]
  NewButton [button .main.$wplane.$wrow.iright -relief sunken -width 4 -height 3 -text " " -command "select_spot $nbuttons"]
  NewButton [button .main.$wplane.$wrow.oright -relief sunken -width 4 -height 3 -text " " -command "select_spot $nbuttons"]
  pack .main.$wplane.$wrow.oleft .main.$wplane.$wrow.ileft .main.$wplane.$wrow.iright .main.$wplane.$wrow.oright -side left
  }
  
proc NewPlane { wplane } {
  frame .main.$wplane -relief raised -bd 2
  frame .main.$wplane.top
  pack .main.$wplane.top
  label .main.$wplane.top.lb -text "* $wplane Board *"
  pack .main.$wplane.top.lb

  NewRow $wplane "row1"
  NewRow $wplane "row2"
  NewRow $wplane "row3"
  NewRow $wplane "row4"
  }
  
  
# Act upon a user's command
proc select_spot {spot} {
  global board user buttons asize
  if { [string index $user 0] == "a" } return
  if { [string index $board $spot] != " " } return
  set newboard ""
  if { $spot > 0} { append newboard [string range $board 0 [expr $spot - 1]] }
  append newboard $user
  if { $spot < $asize } { append newboard [string range $board [expr $spot + 1] end] }
  DisplayButtons $newboard
  update idletasks
  if { [won3D $newboard ] == "n" } { set newboard [ ttt3D $newboard ] }
  set board $newboard
  }
  
#restart the game as a new user
proc start_as {newuser} {
  global board user blankboard
  if { [string compare $newuser "x"] == 0 || [ string compare $newuser "o" ] == 0 || [string compare $newuser "a"] == 0 } {
    set user $newuser
    set board $blankboard
    if { [string compare $user "o" ] ==  0 } { set board [ ttt3D $board ] }
    }
  }

# set x and y size of board
set xsize 4
set ysize 4
# Define z size of board (not yet used) 
set zsize 4
# set length of array
set asize [ expr [expr $xsize * $ysize] * $zsize]

# user starts as X unless they select otherwise
set user x

# we start with a blank board
set blankboard ""
for {set i 0} { $i < 64 } { incr i } { append blankboard " " }
if { [string length $blankboard] != 64 } { puts "Bad Blank Board!"}
set board $blankboard

frame .cmnd
frame .main 
pack .cmnd .main

button .cmnd.startX   -width 10 -height 2 -text "Restart as X" -command { start_as "x" }
button .cmnd.startO   -width 10 -height 2 -text "Restart as O" -command { start_as "o" }
button .cmnd.autoPlay -width 10 -height 2 -text "AutoPlay"     -command { start_as "a" }
button .cmnd.quit     -width 10 -height 2 -text "Quit"         -command { exit }
pack   .cmnd.startX .cmnd.startO .cmnd.autoPlay .cmnd.quit -side left
  
frame .main.upper
frame .main.lower
pack  .main.upper
pack  .main.lower
  
NewPlane "upper.top"
NewPlane "upper.center"
NewPlane "lower.center"
NewPlane "lower.bottom"

pack .main.upper.top .main.upper.center -side left
pack .main.lower.center .main.lower.bottom -side left

while {1} { 
  update 
  if { [string index $user 0] == "a" } { set board [ ttt3D $board ] }
  DisplayButtons $board
  if { [won3D $board] != "n" } {
    set winner [won3D $board]
    if { [string compare $winner "t"] == 0 } { set winner "It's a Tie!" } \
      else {
        if {[string compare $winner $user] == 0 } {
          set winner "You have won as [string toupper $winner], and I bow to your mastery." 
          } else {set winner "I win as [string toupper $winner], you gimp!"} }
    toplevel .win
    switch [ tk_dialog .win "Winner" $winner "" 0 \
      "Start a new game as X" "Start a new game as O" \
      "Watch me play with myself" "Quit this wonderful game." ] \
       0 { start_as "x" } 1 { start_as "o" } 2 { start_as "a" } 3 \
       { exit } 
    }
  }
