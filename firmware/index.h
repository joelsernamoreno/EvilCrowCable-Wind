const char Index[] PROGMEM = R"=====(
<!DOCTYPE HTML>
<html>
<head>
    <title>Evil Crow Cable Wind</title>
    <meta charset="utf-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0, maximum-scale=1.0, user-scalable=no">
    <meta name="apple-mobile-web-app-capable" content="yes">
    <meta name="apple-mobile-web-app-status-bar-style" content="black">
    <link rel="stylesheet" href="style.css">
    <script src="javascript.js"></script>
</head>
<body>
    <div id="global-toast" class="toast-container"></div>
    <nav id='menu'>
        <input type='checkbox' id='responsive-menu'><label></label>
        <ul>
            <li><a href='/'>Home</a></li>
            <li><a href='/livepayload'>Live Payload</a></li>
            <li><a href='/uploadpayload'>Upload Payload</a></li>
            <li><a href='/listpayloads'>List Payloads</a></li>
            <li><a href='/config'>Config</a></li>
        </ul>
    </nav>

    <div class="stat-container">
        <div class="stat-group">
            <strong>Connection Status: <span class="status-indicator"></span></strong>
        </div>
    </div>

    <!-- <div class="stat-container">
        <div class="stat-group">
            <a href="ish://">Open iSH</a>
        </div>
    </div> -->

    <div class="stat-container">
    	<div class="stat-group">
            <strong>Firmware:</strong>
            <span id="firmware">v1.2</span>
        </div>
        <div class="stat-group">
            <strong>Uptime:</strong>
            <span id="uptime"></span>
        </div>
        <div class="stat-group">
            <strong>CPU Temperature:</strong>
            <span id="temperature"></span>
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
