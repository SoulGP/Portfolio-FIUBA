package com.example.davici;

import javafx.application.Application;

public class Launcher {
    public static void launchWithCode(String code) {
        if (code == null) code = "";
        System.setProperty("davici.initialCode", code);
        Application.launch(App.class);
    }
}

