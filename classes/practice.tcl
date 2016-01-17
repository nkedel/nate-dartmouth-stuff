
set wid 500
set hei 400

canvas .c -width 1000 -height 700 -bg darkseagreen2
pack .c

.c create window 100 100 -anchor nw -window [frame .f]
frame .f.w -height $hei -width $wid -bg blue

pack .f.w
pack propagate .f.w false

frame .f.w.b -height 50 -bg blue
frame .f.w.e -height 35 -bg blue
text .f.w.t
.f.w.t insert insert "pack .f.w.b -fill x
pack .f.w.e.b -side right -anchor se"

pack .f.w.b -fill x
pack propagate .f.w.b false
pack .f.w.e -side bottom -fill x
pack propagate .f.w.e false
pack .f.w.t -fill both

label .f.w.e.wd -textvariable wid
label .f.w.e.ht -textvariable whei
button .f.w.e.b -text "" -bg blue -command {puts "Ya clicked me!!"}
pack .f.w.e.wd .f.w.e.ht -side left -padx 5
pack .f.w.e.b -side right -anchor se

bind .f.w.e.b <Button-2> {
    global xpos ypos

    set xpos %X
    set ypos %Y

    pack forget .f.w.t
}

bind .f.w.e.b <B2-Motion> {
    global xpos ypos hei wid

    puts -nonewline "x: "
    puts -nonewline $xpos
    puts -nonewline " to "
    puts -nonewline %X
    
    puts -nonewline "    y: "
    puts -nonewline $ypos
    puts -nonewline " to "
    puts %Y


    set wid [expr $wid + (%X - $xpos)]
    if {[set temp [expr $hei + (%Y - $ypos)]] <= 85} {
	set hei 85
    } else {
	set hei $temp
    }

    .f.w configure -height $hei -width $wid
    update idletasks
    set xpos %X
    set ypos %Y
}

bind .f.w.e.b <ButtonRelease-2> {
    pack .f.w.t
}
