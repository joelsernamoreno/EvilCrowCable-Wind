const char StaticListPayloads[] PROGMEM = R"=====(
<!DOCTYPE HTML>
<html>
<head>
    <title>EvilCrowCable-Wind - Payloads</title>
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
        <input type='checkbox' id='responsive-menu'><label for='responsive-menu'></label>
        <ul>
            <li><a href='/'>Home</a></li>
            <li><a href='/livepayload'>Live Payload</a></li>
            <li><a href='/uploadpayload'>Upload Payload</a></li>
            <li><a href='/listpayloads'>List Payloads</a></li>
            <li><a href='/autoexecplanning'>AutoExec Planning</a></li>
            <li><a href='/config'>Config</a></li>
        </ul>
    </nav>

    <div class="cable-wind-logo">PAYLOAD LIST</div>

    <div class="view-container">
        <div class="payload-header">
            <h3 style="margin: 0;">Available Payloads:</h3>
            <select id="os-filter" class="select-os-btn">
                <option value="all">ALL</option>
                <option value="windows">Windows</option>
                <option value="android">Android</option>
                <option value="ios">iOS</option>
                <option value="macos">MacOS</option>
            </select>
        </div>
        <div class="payload-list-container">
            <!-- Payloads will be inserted here dynamically -->
)=====";