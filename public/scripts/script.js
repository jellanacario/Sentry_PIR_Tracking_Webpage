import { initializeApp } from "https://www.gstatic.com/firebasejs/9.15.0/firebase-app.js";
                    
            //Firebase configuration
            const firebaseConfig = {
                apiKey: "AIzaSyDfxktxDujYy_Xgn0zGqWJNEmSGJf--WhY",
                authDomain: "esp32-firebase-ee4aa.firebaseapp.com",
                databaseURL: "https://esp32-firebase-ee4aa-default-rtdb.asia-southeast1.firebasedatabase.app",
                projectId: "esp32-firebase-ee4aa",
                storageBucket: "esp32-firebase-ee4aa.appspot.com",
                messagingSenderId: "695711591856",
                appId: "1:695711591856:web:7f0f6ddad0fc0d2d48ba75"
            };

            // Initialize Firebase
            const app = initializeApp(firebaseConfig);

            import {getDatabase} from "https://www.gstatic.com/firebasejs/9.15.0/firebase-database.js";
            const db = getDatabase();
