#!/usr/local/bin/wish
#
# a test

#puts [format "#%04x%04x%04x" 1 1 1]

set third 120
set div 8; # always even
set tdiv [expr $div * 3]
set divAngle [expr double($third) / double($div)]
set halfDivAngle [expr double($divAngle / 2)]
set depth 65535
set step [expr 2 * double($depth) / double($div)]
#puts $step
#set halfDivAngle 0

proc GetColor { hue saturation {intensity 1} {internal 0}} {
    global div curColor
    global divAngle
    global step
    global depth

#    set step [expr 2 * double($depth) / double($div)]

    set red $depth
    set green $depth
    set blue $depth
    
    if { $hue < $div } {
	if {[set green [expr ($step * $hue)]] > $depth} {
	    set green $depth
	    set red [expr $depth - ($step * ($hue - ( $div / 2)))]
	}
	set blue 0
    } elseif { $hue < [expr 2 * $div] } {
	if {[set blue [expr ($step * ($hue - $div))]] > $depth} {
	    set blue $depth
	    set green [expr $depth - ($step * ($hue - (3 *$div / 2)))]
	}
#	puts $green
	set red 0
    } elseif { $hue < [expr 3 * $div] } {
	if {[set red [expr ($step * ($hue - (2* $div)))]] > $depth} {
	    set red $depth
	    set blue [expr $depth - ($step * ($hue - (5* $div / 2)))]
	}
	set green 0
    }    
    
#     if {$red > $depth } {
# 	puts "red :$red"
#     }
#     if {$green > $depth } {
# 	puts "green : $green"
#     }
#     if {$blue > $depth } {
# 	puts "bkue : $blue"
#     }
#     if {($hue == 10) && ($saturation == 7)} {
# 	puts "bkue : $blue"
#     }

    if { $saturation > 0 } {
#	puts [expr fmod(($hue * $divAngle + 180),360) ]
	set opphue [GetColor [expr int(fmod((($hue * $divAngle) + 180),360) / $divAngle)] 0 $intensity 1]
	set opphuer [lindex $opphue 0]
	set opphueg [lindex $opphue 1]
	set opphueb [lindex $opphue 2]
	set red [expr $red + (( $opphuer / $div ) * ($saturation ))]
	set green [expr $green + (( $opphueg / $div ) * ($saturation))]
	set blue [expr $blue + (( $opphueb / $div ) * ($saturation))]
    }

    set red [expr $red * $intensity]
    set green [expr $green * $intensity]
    set blue [expr $blue * $intensity]

    set red [expr round($red)]
    set green [expr round($green)]
    set blue [expr round($blue)]

    if {$red > $depth } {
#	puts "red :$red"
#	puts "$hue $saturation"
	set red $depth
    }
    if {$green > $depth } {
#	puts "green : $green"
#	puts "$hue $saturation"
	set green $depth
    }
    if {$blue > $depth } {
#	puts "bkue : $blue"
#	puts "$hue $saturation"
	set blue $depth
#	puts [GetColor [expr int(fmod((($hue * $divAngle) + 180),360) / $divAngle)] 0  ]
#	puts "GetColor [expr int(fmod((($hue * $divAngle) + 180),360) / $divAngle)] 0  "
#	puts $opphue
   }
    

    if { $internal == 0} {
	return [format "#%04x%04x%04x" $red $green  $blue]
    } else {
	return "$red $green  $blue"
    }
}

proc chooseColor {} {
    global div tdiv
    global divAngle halfDivAngle
    global step
    global depth
    global curColor

    toplevel .col

    frame .col.color 
    #-colormap new

    frame .col.color.left -width 4c -height 7c -relief flat
    
    canvas .col.color.colorwheel -height 310 -width 310
    
    pack .col.color.left .col.color.colorwheel -side left -fill y
    
    set ktop 10
    set kbot 300
    set kseg [expr (($kbot - $ktop) / $div) / 2]
    for {set i 0} { $i < $tdiv} { incr i +1 } {
	set start [expr 90 - ($halfDivAngle) + ($divAngle * $i)]
	#    puts $start
	for {set j 0} { $j < $div} { incr j +1 } {
	    set color [GetColor $i $j]
	    set top [expr 10 + ($kseg * $j)]
	    set bottom [expr 300 - ($kseg * $j)]
#	puts $top
	    #	puts $bottom
	    set id [.col.color.colorwheel create arc $top $top $bottom $bottom \
			-start $start -extent $divAngle \
			-style pieslice -width 0 -fill $color -outline $color]
	    .col.color.colorwheel bind $id <Button-1> ".col.color.left configure -bg $color; set curColor $color"
	    #	update idletasks
	    #	for {set k 0} { $k < 5000} { incr k +1 } {set brick 500}
	}
    }

    pack .col.color
}

proc pickColor {} {
    if {[winfo exists .col] == 0} {
	chooseColor
    } else {
	raise .col
	focus .
    }
}

bind all <Alt-6> {
    NewColor $curColor
    raise .
    focus .
}

