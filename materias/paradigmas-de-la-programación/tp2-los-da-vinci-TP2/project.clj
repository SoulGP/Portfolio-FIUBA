(defproject davici "0.1.0-SNAPSHOT"
  :description "Animación 256x256 con VM funcional en Clojure y UI JavaFX"
  :url ""
  :license {:name "Unspecified"}
  :min-lein-version "2.9.0"
  :dependencies [[org.clojure/clojure "1.11.1"]
                 ;; JavaFX (Linux). Si usas Windows o macOS, ver README para perfiles.
                 [org.openjfx/javafx-base "21.0.4" :classifier "linux"]
                 [org.openjfx/javafx-graphics "21.0.4" :classifier "linux"]
                 [org.openjfx/javafx-controls "21.0.4" :classifier "linux"]]
  :java-source-paths ["vista"]
  :source-paths ["logica" "vista-clj"]
  :javac-options ["--release" "21"]
  :main davici.main
  :profiles {
    :uberjar {:aot :all}
    ;; Perfiles opcionales para otras plataformas
    :win  {:dependencies [[org.openjfx/javafx-base "21.0.4" :classifier "win"]
                          [org.openjfx/javafx-graphics "21.0.4" :classifier "win"]
                          [org.openjfx/javafx-controls "21.0.4" :classifier "win"]]}
    :mac  {:dependencies [[org.openjfx/javafx-base "21.0.4" :classifier "mac"]
                          [org.openjfx/javafx-graphics "21.0.4" :classifier "mac"]
                          [org.openjfx/javafx-controls "21.0.4" :classifier "mac"]]}
  }
  :clean-targets ^{:protect false} ["target"])
