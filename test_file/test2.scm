;;; SICP exercise 1.8

(define (cubic-root-iter guess x)
  (if (good-enough? guess x)
      (improve guess x)
      (cubic-root-iter (improve guess x)
                 x)))

(define (good-enough? guess x)
  (< (/ (abs (- (improve guess x) guess))
        guess)
     0.001))

(define (improve guess x)
  (/ (+ (/ x (* guess guess))
        (* 2 guess))
     3))

(define (cubic-root x)
  (cubic-root-iter 1.0 x))

;; test
(display (cubic-root 8))
(newline)
(display (cubic-root 27))
(newline)
(display (cubic-root 0.001))
(newline)
(display (cubic-root 0.0000004))
(newline)
(display 
 (cubic-root 585310284))
(newline)
