;;; test3_1.scm

(load "F:/Git/Learning/impl_scheme/test_file/test3_2.scm")

(define (average x y)
  (/ (+ x y) 2))

(define tolerance 0.00001)

(define (sqrt-iter guess x)
  (define (good-enough guess)
    (< (abs (- (square guess) x)) tolerance))
  (define (impove guess)
    (average guess (/ x guess)))

  ((iterative-impove good-enough impove) guess))

(display (sqrt-iter 1.0 2))
(newline)

#| output:
;Value: 1.41422
|#