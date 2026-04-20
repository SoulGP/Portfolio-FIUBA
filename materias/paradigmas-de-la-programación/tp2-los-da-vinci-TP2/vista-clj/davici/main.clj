(ns davici.main
  (:gen-class))

(defn -main
  [& args]
  ;; args: 0 o 1 string con el código (si incluye espacios, debe ir entre comillas)
  (let [code (case (count args)
               0 ""
               1 (first args)
               (clojure.string/join " " args))]
    (when code (System/setProperty "davici.initialCode" code))
    ;; Lanzamos JavaFX desde Java (capa de vista)
    (com.example.davici.Launcher/launchWithCode code)))

