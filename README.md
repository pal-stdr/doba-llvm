### DOBA language expression format
```
(+ 5 10)            // addition

(set x 15)          // assignment

(if (> x 10)        // if
    (print "ok")
    (print "err")
)


( def foo (bar)                         // Non typed
    (+ bar 10)
)

( def foo ((bar number)) -> number     // Typed
    (+ bar 10)
)

// In LLVM, Types are used to know which operation to apply and how much memory to allocate
```