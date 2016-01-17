

; accept a board, return a new board after one move
(define (ttt bd)
        (if (and (valid-board? bd)
                 (not (game-over? bd)))
      (letrec ((we (whose-move bd))
	       (they (other we)))
	(cond
	 ((string=? bd "         ") "X        ")
	 ((string=? bd "X        ") "X   O    ")
	 ((string=? bd "X   O    ") "X   O   X")
	 ((string=? bd "X   O   X") "X   OO  X")
	 (else  (board (move bd we they)))))
      (string-append "error: bad board " bd)))


(define X #\X)
(define O #\O)

; return X given O, return O given X, undefined otherwise
(define (other c) (if (char=? c X) O X))

(define (make-bd-quality bd quality) (cons bd quality))

(define (board bd-quality) (car bd-quality))

(define (quality bd-quality) (cdr bd-quality))

(define (best-bd-quality a b)
  (if (> (quality a) (quality b)) a b))

; return a move-anal struct with a board and a quality
(define (move bd we they)
  (cond ((won? bd they) (make-bd-quality bd 0))
	((board-full? bd) (make-bd-quality bd 1))
	(else 
	 (letrec ((try-moves 
		   (lambda (sofar i)
		     (cond
		      ((>= i 9) sofar)
		      ((char=? (string-ref bd i) #\space)
		       (let ((nb (new-board bd we i)))
			 (try-moves
			  (best-bd-quality
			   sofar
			   (make-bd-quality nb
					    (- 2 (quality (move nb they we)))))
			  (+ i 1))))
		      (else (try-moves sofar (+ i 1)))))))
	   (try-moves (make-bd-quality bd -1) 0)))))



(define (new-board bd c i)
  (letrec ((ns (string-copy bd))
	   (foo (string-set! ns i c)))
    ns))

(define (won? bd c)
  (or (won-rows? bd c)
      (won-cols? bd c)
      (won-diags? bd c)))

(define (won-rows? bd c)
  (or (won-steps? bd c 0 1)
      (won-steps? bd c 3 1)
      (won-steps? bd c 6 1)))

(define (won-cols? bd c)
  (or (won-steps? bd c 0 3)
      (won-steps? bd c 1 3)
      (won-steps? bd c 2 3)))

(define (won-diags? bd c)
  (or (won-steps? bd c 0 4)
      (won-steps? bd c 2 2)))

(define (won-steps? bd c offset step)
  (and (char=? c (string-ref bd offset))
       (char=? c (string-ref bd (+ offset step)))
       (char=? c (string-ref bd (+ offset step step)))))


(define (whose-move bd)
  (if (= (ct-chars bd X) (ct-chars bd O)) X O))

(define (board-full? bd) (= (ct-chars bd #\space) 0))

(define (ct-chars string c)
  (letrec ((len (string-length string))
	   (cc (lambda (ct i) 
		 (if (>= i len) 
		     ct
		     (cc (+ ct (if (char=? (string-ref string i) c) 1 0))
			 (+ i 1))))))
    (cc 0 0)))


(define (valid-board? bd)
  (and
   (= (string-length bd) 9)
   (= (+ (ct-chars bd X) (ct-chars bd O) (ct-chars bd #\space)) 9)
   (>= (ct-chars bd X) (ct-chars bd O))
   (<= (ct-chars bd X) (+ 1 (ct-chars bd O)))))

(define (game-over? bd)
  (or
   (won? bd X)
   (won? bd O)
   (board-full? bd)))

(define (show-ttt bd)
  (letrec ((loop1 (lambda (i)
		   (if (>= i 3) 
		       (display "\n")
		       (letrec ((loop2 (lambda (j)
					(if (>= j 3)
					    (begin
					      (display "\n")
					      (loop1 (+ i 1)))
					    (let ((foo (display
							(string-ref
							 bd 
							 (+ (* 3 i) j)))))
					      (loop2 (+ j 1)))))))
			 (loop2 0))))))
    (loop1 0)))


(define (play bd)
  (begin
    (show-ttt bd)
    (if (not (game-over? bd))
	(play (ttt bd))
	(if (won? bd X)
	    (display "X won!\n")
	    (if (won? bd O)
		(display "Y won!\n")
		(display "Tie!\n"))))))






