(ns vm-test
  (:require [clojure.test :refer :all]
            [davici.vm :refer [eval-pixel]]))

(deftest eval-pixel-tests
  (testing "Casos correctos"
    (are [code x y t expected]
         (= expected (eval-pixel code x y t))

      ;; básicos
      "" 1 2 3 [0 0 0]
      "X" 1 2 3 [0 0 1]
      "Y" 1 2 3 [0 0 2]
      "T" 1 2 3 [0 0 3]
      "XY" 1 2 3 [0 1 2]
      "XYT" 1 2 3 [1 2 3]
      "XYTXYTXY" 1 2 3 [3 1 2]

      ;; literales N
      "N1" 1 2 3 [0 0 1]
      "N2" 1 2 3 [0 0 2]
      "N3" 1 2 3 [0 0 3]
      "N4" 1 2 3 [0 0 4]
      "N5" 1 2 3 [0 0 5]
      "N6" 1 2 3 [0 0 6]
      "N7" 1 2 3 [0 0 7]
      "N8" 1 2 3 [0 0 8]
      "N9" 1 2 3 [0 0 9]
      "N8N9" 1 2 3 [0 8 9]
      "N7N8N9" 1 2 3 [7 8 9]
      "N0N1N2N3N4N5N6N7" 1 2 3 [5 6 7]

      ;; operaciones aritméticas
      "XY+" 1 2 3 [0 0 3]
      "N1N1+" 1 2 3 [0 0 2]
      "XYT++" 1 2 3 [0 0 6]
      "XY-" 1 2 3 [0 0 -1]
      "XY*" 3 2 1 [0 0 6]
      "XY/" 4 2 1 [0 0 2]
      "XY%" 7 5 1 [0 0 2]
      "XY^" 1 3 2 [0 0 2]
      "XY&" 1 3 2 [0 0 1]
      "XY|" 1 3 2 [0 0 3]

      ;; booleanas
      "X!" 0 2 3 [0 0 1]
      "X!" 1 2 3 [0 0 0]
      "X!!" 2 2 3 [0 0 1]
      "XY=" 1 2 3 [0 0 0]
      "XX=" 1 2 3 [0 0 1]
      "XY<" 1 2 3 [0 0 1]
      "XY>" 1 2 3 [0 0 0]

      ;; estructuras
      "XN1[N1+]" 1 2 3 [0 0 2]
      "N2N3[N4+]" 1 2 3 [0 0 14]
      "XTX-[N9+]" 1 2 3 [0 0 19]
      "XTX-[N9+]X" 1 2 3 [0 19 1]
      "N0N2[N3[N1+]]" 1 2 3 [0 0 6]
      "N0N2[N3[N4[N1+]]]" 1 2 3 [0 0 24]))

  (testing "Casos que deben lanzar error"
    (are [code x y t]
         (thrown? clojure.lang.ExceptionInfo (eval-pixel code x y t))

      ;; pila y sobreflujo
      "XYTXYTXYT" 1 2 3
      "NNNNNNNNN" 1 2 3
      "N0N1N2N3N4N5N6N7N8" 1 2 3
      "N0N1N2N3N4N5N6N7ND" 1 2 3
      "P" 1 2 3
      "XYR" 1 2 3
      "XS" 1 2 3
      "X+" 1 2 3
      "TX-[N1+]" 1 2 3
      "N0N1[N1[N1[N1[N1[N1[N1[N1[N1[N1+]]]]]]]]]" 1 2 3
      "XYT+++" 1 2 3

      ;; límites numéricos
      "XY+" 2147483637 11 3
      "XY-" -2147483638 11 3
      "XY*" 1073741823 4 3
      "XY*" -1073741823 -4 3
      "XY*" -1073741824 4 3
      "XY*" 1073741824 -4 3

      ;; divisiones prohibidas
      "XY/" 1 0 3
      "XY%" 1 0 3)))
