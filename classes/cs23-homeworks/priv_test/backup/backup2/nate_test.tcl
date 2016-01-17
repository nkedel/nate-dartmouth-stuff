#!./TTT
# this is a tester for the ttt cmd
# it will start with every possible soarting position and then
# lets the computer play it's self

#the board that we are going to be using (starts empty)
set theBoard "         "

# prints the board in a manner easier to read
proc print { } {
    global theBoard
    
    for { set i 0 } { $i < 9 } { incr i } {
	if { $i == 0 } {
	    puts -nonewline "           "
	}
	set value [string index $theBoard $i]
	if { $value == " " } {
	    puts -nonewline "  -  "
	} else {
	    puts -nonewline "  "
	    puts -nonewline $value
	    puts -nonewline "  "
	}
	if { $i % 3 == 2 }  {
	    puts " "
	    puts " "
	    puts " "
	    puts -nonewline "           "
	}
    }

}

# function taken from cs23/examples/tcl
proc SetChar { str ndex newchar } {
    return "[string range $str 0 [expr $ndex - 1]]$newchar[string range $str [expr $ndex + 1] end]"}

# interprets the won command
proc winner { value } {
    if { $value == "N" } {
	puts "--------------"
	puts "           play continues"
	puts "           --------------"
    } elseif { $value == "X" } {
	puts "--------------"
	puts "           X is the winner"
	puts "           --------------"
    } elseif { $value == "O" } {
	puts "--------------"
	puts "           O is the winner"
	puts "           --------------"
    } elseif { $value == "T" } {
	puts "--------------"
	puts "           it's a tie"
	puts "           --------------"
       
    }
}

puts " "
puts " "
puts "************************************************************"
puts "* we are going to give ttt every single possible starting  *  "
puts "* borad.  then we are going to let it play itself.  if the *"
puts "* ttt command is working as expected, it should always tie.*"
puts "************************************************************"

for { set i 0 } { $i < 9 } { incr i } {
    set theBoard [SetChar $theBoard $i "X"]
    puts " "
   

    puts "  -------------------------------------"
    puts "  checking with the folowing first move"
    puts "  -------------------------------------"
    puts " "
    puts " "
    print
    
    winner [won $theBoard]
    puts " "
    
    for { set j 0 } { $j < 8 } { incr j } {
	set theBoard [ttt $theBoard]
	print
	winner [won $theBoard]
	puts " "

	after 500
    }
    after 1000
    set theBoard "         "
}

puts "all possible starting positions ckecked."
    

puts " "
puts " "
puts "==========================================================="    
puts "= the above makes adequate test for the won function when ="
puts "= the board is a tie and when there are still moves.  now ="
puts "= we will test to see that it detects winners of X and O  ="
puts "==========================================================="

puts " "
puts " "
#set board "         "
foreach board {"XXXO O   " "O  XXXO  " "  OO  XXX" "XOOX  X  " "OX  X  XO" " OXO X  X" "X  OXO  X" "  X X XOO" "OOOX  XX " "X XOOOX  " "X   XXOOO" "OX OX O X" " O XOXXO " " XO XOX O" "X OXO O X" "OX  OX XO"}  {
    puts " "
    puts "     ----------------------------"
    puts "     checking the following board"
    puts "     ----------------------------"
    puts " "
    set theBoard $board
    print
    winner [won $theBoard]
    after 1000
}


puts " "
puts " "

puts "============================================================="    
puts "= testing for boards that are no good.  illegal characters, ="
puts "= wrong number of arguments, illegal positions ect.  uses   ="
puts "= catch so as not to exit the program.                      ="
puts "============================================================="

foreach board {"hello there" "blah" "xxxxxxxxx" "X  XOO  X" "X  XOp  X"  "X  XOO XX" "XXXXXOOOOO"} {
    puts " "
    puts "     ----------------------------"
    puts "     checking the following board"
    puts "     ----------------------------"
    puts " "
    set theBoard $board
    puts -nonewline "        "
    puts $board
    puts " "
    if { [catch {ttt $theBoard}] }  {
	puts "        not a valid board"
    } else {
	puts "        valid board"
    }
    puts " "
    after 1000
}

puts "     --------------------------------"
puts "     passing illegal \# of arguments"
puts "     to the ttt command           "
puts "     --------------------------------"
puts " " 

if { [catch {ttt $theBoard " "}] }  {
    puts "        ttt command failed"
} else {
    puts "        valid board"
}

after 200

puts " "
puts " "
puts "     --------------------------------"
puts "     passing illegal \# of arguments"
puts "     to the ttt command "
puts "     --------------------------------"
puts " " 


if { [catch {won $theBoard " "}] }  {
    puts "        won command failed"
} else {
    puts "        valid board"
}

puts " "
puts " "
exit






