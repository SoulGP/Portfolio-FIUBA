(ns davici.vm)

(def ^:private ds-cap 8)
(def ^:private ls-cap 8)

(defn- error [msg]
  (throw (ex-info msg {:type :vm/error})))

(defn- push-ds [ds v]
  (if (< (count ds) ds-cap) (conj ds v) (error "Pila de datos llena")))

(defn- compile-code
  "Compila el string a operaciones. Devuelve {:ops [...], :brackets {i j, j i}}.
   Ops incluye keywords (:x :y :t :clamp :pop :dup :swap :rot3 :add :sub :mul :and :or :xor :eq :not :lt :div :mode :loop-begin :loop-end)
   y literales como [:lit n]."
  [code]
  (let [s (or code "")]
    (when (> (count s) 1024)
      (error "Código supera 1024 caracteres"))
    (loop [i 0, ops [], stack []]
      (if (>= i (count s))
        (do
          (when (seq stack)
            (error "Corchetes desbalanceados"))
          {:ops ops :brackets {}})
        (let [ch (.charAt s i)]
          (cond
            (Character/isWhitespace ch)
            (recur (inc i) ops stack)

            (or (= ch \X) (= ch \x))
            (recur (inc i) (conj ops :x) stack)

            (or (= ch \Y) (= ch \y))
            (recur (inc i) (conj ops :y) stack)

            (or (= ch \T) (= ch \t))
            (recur (inc i) (conj ops :t) stack)

            (or (= ch \C) (= ch \c))
            (recur (inc i) (conj ops :clamp) stack)

            (or (= ch \P) (= ch \p))
            (recur (inc i) (conj ops :pop) stack)

            (or (= ch \D) (= ch \d))
            (recur (inc i) (conj ops :dup) stack)

            (or (= ch \S) (= ch \s))
            (recur (inc i) (conj ops :swap) stack)

            (or (= ch \R) (= ch \r))
            (recur (inc i) (conj ops :rot3) stack)

            (or (= ch \N) (= ch \n))
            (let [res (loop [j (inc i), acc 0]
                        (if (and (< j (count s)) (Character/isDigit (.charAt s j)))
                          (let [d (Character/digit (.charAt s j) 10)]
                            (recur (inc j) (+ (* 10 acc) d)))
                          {:j j :val acc}))
                  j (:j res)
                  val (:val res)
                  ops' (conj ops [:lit (int val)])]
              (recur j ops' stack))

            (= ch \+)
            (recur (inc i) (conj ops :add) stack)

            (= ch \!)
            (recur (inc i) (conj ops :not) stack)

            (= ch \|)
            (recur (inc i) (conj ops :or) stack)

            (= ch \^)
            (recur (inc i) (conj ops :xor) stack)

            (= ch \-)
            (recur (inc i) (conj ops :sub) stack)

            (= ch \*)
            (recur (inc i) (conj ops :mul) stack)

            (= ch \&)
            (recur (inc i) (conj ops :and) stack)

            (= ch \=)
            (recur (inc i) (conj ops :eq) stack)

            (= ch \<)
            (recur (inc i) (conj ops :lt) stack)

            (= ch \>)
            (recur (inc i) (conj ops :gt) stack)

            (= ch \/)
            (recur (inc i) (conj ops :div) stack)

            (= ch \%)
            (recur (inc i) (conj ops :modulo) stack)

            (or (= ch \M) (= ch \m))
            (recur (inc i) (conj ops :mode) stack)

            (= ch \[)
            (recur (inc i) (conj ops :loop-begin) (conj stack (count ops)))

            (= ch \])
            (if (seq stack)
              (let [_open-idx (peek stack)
                    stack' (pop stack)
                    ops' (conj ops :loop-end)]
                (recur (inc i) ops' stack'))
              (error "Corchete de cierre sin apertura"))

            (Character/isDigit ch)
            (error "Dígito sin acumulador (use N antes)")

            :else
            (error (str "Comando inválido: " ch))))))))

(defn- exec-ops
  "Ejecuta el programa compilado para x,y,t. Devuelve {:ds [...] } o {:rgb [r g b]} si finaliza temprano por modo M."
  [{:keys [ops]} x y t]
  (let [opcnt (count ops)]
    (loop [idx 0, ds [], ls [], m 0]
      (if (>= idx opcnt)
        {:ds ds}
        (let [op (nth ops idx)]
          (cond
            (= op :x) (recur (inc idx) (push-ds ds x) ls m)
            (= op :y) (recur (inc idx) (push-ds ds y) ls m)
            (= op :t) (recur (inc idx) (push-ds ds t) ls m)

            (= op :clamp)
            (if (seq ds)
              (let [v (peek ds)
                    rest (pop ds)
                    v' (cond (< v 0) 0 (> v 255) 255 :else v)]
                (recur (inc idx) (push-ds rest v') ls m))
              (error "Pila vacía para clamp"))

            (= op :pop)
            (if (seq ds)
              (recur (inc idx) (pop ds) ls m)
              (error "Pila vacía para pop"))

            (= op :dup)
            (if (seq ds)
              (let [v (peek ds)]
                (recur (inc idx) (push-ds ds v) ls m))
              (error "Pila vacía para duplicar"))

            (= op :swap)
            (if (>= (count ds) 2)
              (let [b (peek ds)
                    a (peek (pop ds))
                    rest (pop (pop ds))]
                (recur (inc idx) (-> rest (push-ds b) (push-ds a)) ls m))
              (error "No hay suficientes valores para intercambiar"))

            (= op :rot3)
            (if (>= (count ds) 3)
              (let [c (peek ds)
                    b (peek (pop ds))
                    a (peek (pop (pop ds)))
                    rest (pop (pop (pop ds)))]
                (recur (inc idx) (-> rest (push-ds b) (push-ds c) (push-ds a)) ls m))
              (error "No hay suficientes valores para rotar"))

            (= op :add)
            (if (>= (count ds) 2)
              (let [b (peek ds)
                    a (peek (pop ds))
                    rest (pop (pop ds))]
                (recur (inc idx) (push-ds rest (+ a b)) ls m))
              (error "No hay suficientes valores para sumar"))

            (= op :sub)
            (if (>= (count ds) 2)
              (let [b (peek ds)
                    a (peek (pop ds))
                    rest (pop (pop ds))]
                (recur (inc idx) (push-ds rest (- a b)) ls m))
              (error "No hay suficientes valores para restar"))

            (= op :mul)
            (if (>= (count ds) 2)
              (let [b (peek ds)
                    a (peek (pop ds))
                    rest (pop (pop ds))]
                (recur (inc idx) (push-ds rest (* a b)) ls m))
              (error "No hay suficientes valores para multiplicar"))

            (= op :and)
            (if (>= (count ds) 2)
              (let [b (peek ds)
                    a (peek (pop ds))
                    rest (pop (pop ds))]
                (recur (inc idx) (push-ds rest (bit-and a b)) ls m))
              (error "No hay suficientes valores para AND"))

            (= op :or)
            (if (>= (count ds) 2)
              (let [b (peek ds)
                    a (peek (pop ds))
                    rest (pop (pop ds))]
                (recur (inc idx) (push-ds rest (bit-or a b)) ls m))
              (error "No hay suficientes valores para OR"))

            (= op :xor)
            (if (>= (count ds) 2)
              (let [b (peek ds)
                    a (peek (pop ds))
                    rest (pop (pop ds))]
                (recur (inc idx) (push-ds rest (bit-xor a b)) ls m))
              (error "No hay suficientes valores para XOR"))

            (= op :eq)
            (if (>= (count ds) 2)
              (let [b (peek ds)
                    a (peek (pop ds))
                    rest (pop (pop ds))]
                (recur (inc idx) (push-ds rest (if (= a b) 1 0)) ls m))
              (error "No hay suficientes valores para comparar"))

            (= op :not)
            (if (seq ds)
              (let [a (peek ds)
                    rest (pop ds)]
                (recur (inc idx) (push-ds rest (if (zero? a) 1 0)) ls m))
              (error "Pila vacía para NOT"))

            (= op :lt)
            (if (>= (count ds) 2)
              (let [b (peek ds)
                    a (peek (pop ds))
                    rest (pop (pop ds))]
                (recur (inc idx) (push-ds rest (if (< a b) 1 0)) ls m))
              (error "No hay suficientes valores para comparar (<)"))

            (= op :gt)
            (if (>= (count ds) 2)
              (let [b (peek ds)
                    a (peek (pop ds))
                    rest (pop (pop ds))]
                (recur (inc idx) (push-ds rest (if (> a b) 1 0)) ls m))
              (error "No hay suficientes valores para comparar (>)"))

            (= op :div)
            (if (>= (count ds) 2)
              (let [b (peek ds)
                    a (peek (pop ds))
                    rest (pop (pop ds))]
                (if (zero? b)
                  (case m
                    0 (error "División por cero")
                    1 {:rgb [0 0 0]}
                    2 {:rgb [255 0 0]}
                    (error "Modo M inválido"))
                  (recur (inc idx) (push-ds rest (quot a b)) ls m)))
              (error "No hay suficientes valores para dividir"))

            (= op :modulo)
            (if (>= (count ds) 2)
              (let [b (peek ds)
                    a (peek (pop ds))
                    rest (pop (pop ds))]
                (if (zero? b)
                  ;; mismo comportamiento que / cuando b=0
                  (case m
                    0 (error "División por cero en %")
                    1 {:rgb [0 0 0]}
                    2 {:rgb [255 0 0]}
                    (error "Modo M inválido"))
                  (let [res (mod (rem a b) (Math/abs b))]
                    (recur (inc idx) (push-ds rest res) ls m))))
              (error "No hay suficientes valores para módulo (%)"))

            (= op :mode)
            (recur (inc idx) ds ls (mod (inc m) 3))

            (= op :loop-begin)
            (if (seq ds)
              (let [a (peek ds)
                    ds' (pop ds)]
                (if (pos? a)
                  (let [depth (quot (count ls) 2)]
                    (if (< depth ls-cap)
                      (recur (inc idx) ds' (-> ls (conj (inc idx)) (conj a)) m)
                      (error "Pila de loops llena")))
                  (let [; buscamos el matching ']' en ops
                        match-idx (loop [j (inc idx), open 1]
                                    (when (>= j opcnt)
                                      (error "Corchetes desbalanceados"))
                                    (let [opj (nth ops j)]
                                      (cond
                                        (= opj :loop-begin) (recur (inc j) (inc open))
                                        (= opj :loop-end) (if (= open 1) j (recur (inc j) (dec open)))
                                        :else (recur (inc j) open))))]
                    (recur (inc match-idx) ds' ls m))))
              (error "Pila vacía para iniciar ciclo"))

            (= op :loop-end)
            (if (>= (count ls) 2)
              (let [c (peek ls)
                    ret (peek (pop ls))
                    ls' (pop (pop ls))
                    c' (dec c)]
                (if (pos? c')
                  (recur ret ds (-> ls' (conj ret) (conj c')) m)
                  (recur (inc idx) ds ls' m)))
              (error "] sin ciclo activo"))

            (vector? op)
            (let [[_ n] op]
              (recur (inc idx) (push-ds ds n) ls m))

            :else
            (error "Operación inválida")))))))

(defn- ds->rgb
  "Extrae [r g b] del tope de la pila `ds` devolviendo los valores."
  [ds]
  (let [b (or (peek ds) 0)
        g (or (peek (if (seq ds) (pop ds) [])) 0)
        r (or (peek (if (>= (count ds) 2) (pop (pop ds)) [])) 0)]
    [(int r) (int g) (int b)]))

(defn eval-pixel
  "Evalúa el programa `code` con el lenguaje extendido (incluye <, /, M, [, ]). Devuelve [r g b]."
  [code x y t]
  (when-not (and (integer? x) (integer? y) (integer? t))
    (error "x,y,t deben ser enteros"))
  (when (or (neg? x) (> x 255) (neg? y) (> y 255) (neg? t) (> t 255))
    (error "x,y,t fuera de rango 0-255"))
  (let [compiled (compile-code code)
        res (exec-ops compiled x y t)]
    (if-let [rgb (:rgb res)]
      rgb
      (ds->rgb (:ds res)))))

(defn eval-frame
  "Devuelve un int-array ARGB (256x256) para el tiempo t dado.
  Errores por pixel lanzan ex-info."
  [code t]
  (let [w 256
        h 256
        alpha (int -16777216)
        compiled (compile-code code)
        pixel-argb (fn [x y]
                     (let [res (exec-ops compiled x y t)
                           [r g b] (if-let [rgb (:rgb res)] rgb (ds->rgb (:ds res)))]
                       (bit-or alpha
                               (bit-shift-left (bit-and r 0xFF) 16)
                               (bit-shift-left (bit-and g 0xFF) 8)
                               (bit-and b 0xFF))))
        ;; Paralelizamos por filas para evitar contención en estructuras.
        rows (doall
              (pmap (fn [y]
                      (into [] (map (fn [x] (pixel-argb x y)) (range w))))
                    (range h)))
        flat (apply concat rows)]
    (int-array flat)))
