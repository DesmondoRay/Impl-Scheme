;;; exercise 2.42

(load "F:/Git/Learning/impl_scheme/test_file/test4/p42-adjoin-position.scm")
(load "F:/Git/Learning/impl_scheme/test_file/test4/p42-safe.scm")
(load "F:/Git/Learning/impl_scheme/test_file/test4/2-2-3.scm")

(define (queens board-size)
  (define (queen-cols k)
    (if (= k 0)
        (list empty-board)
        (filter
         (lambda (positions) (safe? k positions))
         (flatmap
          (lambda (rest-of-queens)
            (map (lambda (new-row)
                   (adjoin-position new-row k rest-of-queens))
                 (enumerate-interval 1 board-size)))
          (queen-cols (- k 1))))))
  (queen-cols board-size))

(define empty-board '())

(for-each (lambda (x)
            (newline)
            (display x))
          (queens 4))
(newline)
