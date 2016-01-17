10,11d9
< set toEval ""
< set outEval ""
58a57,60
>   bind . <Alt-q> { 
>     pipe_Shutdown
>     exit
>     }
65a68
>   puts "pipe_Destroy called on buffer #$buffer"
66a70
>      puts "pipe_Close called on buffer #$myPipe"
68a73
>   puts "edit_Close called on buffer #$buffer"
88c93
<   global numPipes 
---
>   global numPipes
167c172
< #    if { $pipenum < 3 } { 
---
>     if { $pipenum < 3 } { 
169c174
< #     pipe_reOpen $pipenum
---
>       pipe_reOpen $pipenum
248a254
>   puts "pipe_Write on #$pipeNum: $toWrite"
273d278
<   gloval toEval outEval
275c280
<   uplevel #0 { catch "\{$toEval\}" outEval }
---
>   catch [ uplevel #0 [ eval "$toEval" ] ] outEval
278a284,340
> proc text_toTCL { inBuffer } {
>   global numPipes
>   if {!edit_isBuffer $inBuffer} {
>     error "text_toTCL called on invalid buffer"
>     }
>   if {$numPipes < 3} {
>     error "text_toTCL called on unititialized pipes"
>     }
>   if [catch {pipe_fromBuffer 0 $inBuffer } cerr ] {
>     error "couldn't sent buffer $inBuffer to TCL ($cerr)"
>     }
>   }
> 
> proc text_toSCHEME { inBuffer } {
>   global numPipes
>   if {!edit_isBuffer $inBuffer} {
>     error "text_toSCHEME called on invalid buffer"
>     }
>   if {$numPipes < 3} {
>     error "text_toSCHEME called on unititialized pipes"
>     }
>   if {!pipe_Open? 1} {
>     if [catch {pipe_reOpen 1}] { 
>       error "Couldn't reopen Scheme pipe."
>       }
>     }  
>   if [catch {pipe_fromBuffer 1 $inBuffer } cerr ] {
>     error "couldn't sent buffer $inBuffer to Scheme ($cerr)"
>     }
>   }
> 
> proc text_toUNIX { inBuffer outBuffer } {
>   global numPipes
>   if {!edit_isBuffer $inBuffer} {
>     error "text_toUNIX called on invalid buffer"
>     }
>   if {$numPipes < 3} {
>     error "text_toUNIXcalled on unititialized pipes"
>     }
>     
>   if {edit_isBuffer $outBuffer} {
>     error "buffer $outBuffer already exists in toUnix"
>     }
> 
>   if [catch {pipe_Create $outBuffer}] {
>     error "couldn't create buffer for new unix window."
>     }
> 
>   if [catch {pipe_Open $outBuffer "csh"} pnum] {
>     error "couldn't create buffer for new unix window."
>     }
>     
>   if [catch {pipe_fromBuffer $pnum $inBuffer } cerr ] {
>     error "couldn't sent buffer $inBuffer to UNIX ($cerr)"
>     }
>   }
>   
294c356
< proc text_StoSCHEME { inBuffer } {
---
> proc text_toSCHEME { inBuffer } {
