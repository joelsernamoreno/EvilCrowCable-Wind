const char Index[] PROGMEM = R"=====(
<!DOCTYPE HTML>
<html>
<head>
    <title>Evil Crow Cable Wind</title>
    <meta charset="utf-8">
    <link rel="stylesheet" href="style.css">
    <script src="lib.js"></script>
    <style>
        .switch-container {
            display: flex;
            justify-content: space-around;
            align-items: center;
            margin-top: 20px;
            flex-wrap: wrap;
        }

        .switch-group {
            display: flex;
            align-items: center;
            padding: 10px 20px;
            background-color: #333;
            border: 1px solid #555;
            border-radius: 10px;
            box-shadow: 0 0 10px rgba(0, 0, 0, 0.1);
            max-width: 300px;
            margin: 10px;
            text-align: left;
        }

        .switch-label {
            font-size: 16px;
            font-weight: bold;
            margin-right: 10px;
            color: white;
        }

        .switch {
            position: relative;
            display: inline-block;
            width: 60px;
            height: 34px;
        }

        .switch input {
            opacity: 0;
            width: 0;
            height: 0;
        }

        .slider {
            position: absolute;
            cursor: pointer;
            top: 0;
            left: 0;
            right: 0;
            bottom: 0;
            background-color: #ccc;
            transition: .4s;
        }

        .slider:before {
            position: absolute;
            content: "";
            height: 26px;
            width: 26px;
            left: 4px;
            bottom: 4px;
            background-color: white;
            transition: .4s;
        }

        input:checked + .slider {
            background-color: #2196F3;
        }

        input:checked + .slider:before {
            transform: translateX(26px);
        }

        .slider.round {
            border-radius: 34px;
        }

        .slider.round:before {
            border-radius: 50%;
        }

        .messages-container {
            display: flex;
            justify-content: center;
            align-items: center;
            margin-top: 20px;
        }

        .message {
            color: green;
            font-weight: bold;
            text-align: center;
            max-width: 300px;
            margin: 10px;
        }

        .stat-container {
            display: flex;
            justify-content: center;
            align-items: center;
            flex-wrap: wrap;
            margin-top: 20px;
        }

        .stat-group {
            display: flex;
            flex-direction: column;
            align-items: flex-start;
            padding: 10px 20px;
            background-color: #333;
            border: 1px solid #555;
            border-radius: 10px;
            box-shadow: 0 0 10px rgba(0, 0, 0, 0.1);
            max-width: 300px;
            margin: 10px;
            color: white;
        }

        .stat-group strong {
            font-size: 16px;
            font-weight: bold;
            margin-bottom: 5px;
        }
    </style>
    <script>
        function fetchStats() {
            fetch('/stats').then(response => response.json()).then(data => {
                document.getElementById('uptime').innerText = data.uptime + ' seconds';
                document.getElementById('cpu0').innerText = data.cpu0 + ' MHz';
                document.getElementById('cpu1').innerText = data.cpu1 + ' MHz';
                document.getElementById('freespiffs').innerText = data.freespiffs + ' bytes';
                document.getElementById('temperature').innerText = data.temperature + ' °C';
                document.getElementById('totalram').innerText = data.totalram + ' bytes';
                document.getElementById('freeram').innerText = data.freeram + ' bytes';
            });
        }
        setInterval(fetchStats, 5000);
        fetchStats();
    </script>
</head>
<body>
<nav id='menu'>
    <input type='checkbox' id='responsive-menu' onclick='updatemenu()'><label></label>
    <ul>
        <li><a href='/'>Home</a></li>
        <li><a href='/livepayload'>Live Payload</a></li>
        <li><a href='/uploadpayload'>Upload Payload</a></li>
        <li><a href='/listpayloads'>List Payloads</a></li>
        <li><a href='/config'>Config</a></li>
    </ul>
</nav>
<br>
<div class="stat-container">
    <div class="stat-group">
        <strong>Uptime:</strong>
        <span id="uptime"></span>
    </div>
    <div class="stat-group">
        <strong>CPU Core 0 Frequency:</strong>
        <span id="cpu0"></span>
    </div>
    <div class="stat-group">
        <strong>CPU Core 1 Frequency:</strong>
        <span id="cpu1"></span>
    </div>
    <div class="stat-group">
        <strong>Free SPIFFS Memory:</strong>
        <span id="freespiffs"></span>
    </div>
    <div class="stat-group">
        <strong>CPU Temperature:</strong>
        <span id="temperature"></span>
    </div>
    <div class="stat-group">
        <strong>Total RAM:</strong>
        <span id="totalram"></span>
    </div>
    <div class="stat-group">
        <strong>Free RAM:</strong>
        <span id="freeram"></span>
    </div>
</div>
</body>
</html>
)=====";