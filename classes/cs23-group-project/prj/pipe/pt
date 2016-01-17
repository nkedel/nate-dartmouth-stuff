#!/usr/local/bin/wish
# internal module, include with "source pipe.tcl"
#
# $Header: /a/tahoe/usr/tahoe1/nate/prj/pipe/RCS/pipe-test.tcl,v 1.1 1996/05/24 23:47:19 nate Exp nate $
# by Nate Edel
#

frame .d
frame .d.c
pack .d .d.c

source "editfcn.tcl"
source "pipe.tcl"
source "BM.tcl"

BM_BufferInit
set NumBuffers 3
pipe_Init 1 2 3
pack .d.c.buffer1
pack .d.c.buffer2
pack .d.c.buffer3
bind . <Alt-q> {
  pipe_Shutdown
  exit
  }
  
