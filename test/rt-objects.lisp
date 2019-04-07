;;
;;  ANSI COMMON LISP: 7. Objects
;;
(import 'lisp-system::closp)
(import 'lisp-clos::referenced-class)
(use-package 'lisp-clos)

(load #p"test/rtobjects-reader.lisp")
(load #p"test/rtobjects-class.lisp")


;;
;;  do-tests
;;
(do-tests :test t)

