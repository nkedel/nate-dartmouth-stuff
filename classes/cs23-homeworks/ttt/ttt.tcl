#!./ttt
# TicTacToe in Tk
# - Nathan Edel 4/24/96
#
# Should be self explanatory.
#
# Very derivative of John M. Danskin's calc.tcl

wm title . "Tic Tac Toe"

# stolen directly from Danskin's calc.tcl : "Keep track of a new button"
set nbuttons 0

proc NewButton { b } {
  global buttons nbuttons
  set buttons($nbuttons) $b
  incr nbuttons
  }

# set x and y size of board
set xsize 3
set ysize 3
# Define z size of board (not yet used) 
set zsize 1
# set length of array
set asize [expr $xsize * $ysize]

# user starts as X unless they select otherwise
set user X

# we start with a blank board
set board "         "

# main fram as in Danskin
frame .main 
pack .main

# and a rack of frames from each row, also from Danskin
frame .main.row1
frame .main.row2
frame .main.row3
pack .main.row1 .main.row2 .main.row3

# Create top row of our board (largely danskin)
NewButton [button .main.row1.left   -width 7 -text " " -command { select_spot 0 }]
NewButton [button .main.row1.center -width 7 -text " " -command { select_spot 1 }]
NewButton [button .main.row1.right  -width 7 -text " " -command { select_spot 2 }]

# Create middle row (also largely Danskin)
NewButton [button .main.row2.left   -width 7 -text " " -command { select_spot 3 }]
NewButton [button .main.row2.center -width 7 -text " " -command { select_spot 4 }]
NewButton [button .main.row2.right  -width 7 -text " " -command { select_spot 5 }]

# Create bottom row (also largely Danskin)
NewButton [button .main.row3.left   -width 7 -text " "  -command { select_spot 6 }]
NewButton [button .main.row3.center -width 7 -text " "  -command { select_spot 7 }]
NewButton [button .main.row3.right  -width 7 -text " "  -command { select_spot 8 }]

# Our command buttons don't change!
button .main.row1.cmnd -width 15  -text "Start X" -command { start_as "X" }
button .main.row2.cmnd -width 15  -text "Start O" -command { start_as "O" }
button .main.row3.cmnd -width 15  -text "Quit" -command { exit }

pack .main.row1.left .main.row1.center .main.row1.right .main.row1.cmnd -side left
pack .main.row2.left .main.row2.center .main.row2.right .main.row2.cmnd -side left
pack .main.row3.left .main.row3.center .main.row3.right .main.row3.cmnd -side left

# Act upon a user's command
proc select_spot {spot} {
  global board user buttons asize
  puts $board
  if { [string index $board $spot] != " " } return
  set newboard ""
  if { $spot > 0} { append newboard [string range $board 0 [expr $spot - 1]] }
  append newboard $user
  if { $spot < $asize } { append newboard [string range $board [expr $spot + 1] end] }
  if { [won $newboard ] == "N" } { set newboard [ ttt $newboard ] }
  puts $newboard
  set board $newboard
  }
  
#restart the game as a new user
proc start_as {newuser} {
  global board user
  if { [string compare $newuser "X"] == 0 || [ string compare $newuser "O" ] == 0 } {
    puts $newuser
    set user $newuser
    set board "         "
    if { [string compare $user "O" ] ==  0 } { set board [ ttt $board ] }
    puts $board
    }
  }

while {1} { 
  update 
  for { set i 0 } { $i < $asize } { incr i } {
    $buttons($i) config -text [string index $board $i]
    }
  if { [won $board] != "N" } {
    set winner [won $board]
    puts $winner
    if { [string compare $winner "T"] == 0 } { set winner "It's a Tie!" } \
      else { set winner "$winner wins!" }
    toplevel .win
    tk_dialog .win "Winner" $winner "" 0 "Restart as X"
    start_as X
    }
  }
