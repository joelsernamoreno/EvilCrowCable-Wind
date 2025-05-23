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
            <li><a href='/autoexecplanning'>AutoExec Planning</a></li>
            <li><a href='/config'>Config</a></li>
        </ul>
    </nav>

    <div class="stat-container">
        <div class="stat-group">
            <strong>Connection Status: <span class="status-indicator"></span></strong>
        </div>
    </div>

    <div class="stat-container">
    	<div class="stat-group">
            <strong>Firmware:</strong>
            <span id="firmware">v1.3</span>
        </div>
        <div class="stat-group">
            <strong>Target OS:</strong>
            <span id="targetos" class="clickable-os" onclick="detectOS()">N/A</span>
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

    <script>
        function detectOS() {
            const osElement = document.getElementById('targetos');
            osElement.textContent = 'Detecting...';
            osElement.style.color = '#0056b3';
            
            fetch('/runlivepayload', {
                method: 'POST',
                headers: {
                    'Content-Type': 'application/x-www-form-urlencoded',
                },
                body: 'livepayload=DetectOS'
            })
            .then(response => {
                if (response.ok) {
                    // Start polling for OS detection result
                    pollOSDetection();
                } else {
                    throw new Error('Detection failed');
                }
            })
            .catch(error => {
                console.error('Error detecting OS:', error);
                osElement.textContent = 'Error';
                osElement.style.color = '#FF3B30'; // Red color for error
                setTimeout(() => {
                    osElement.textContent = 'N/A';
                    osElement.style.color = '#007AFF'; // Blue color for normal state
                }, 2000);
            });
        }

        function pollOSDetection() {
            const osElement = document.getElementById('targetos');
            
            fetch('/stats')
            .then(response => response.json())
            .then(data => {
                if (data.os && data.os !== 'OS Unknown' && data.os !== 'N/A') {
                    // Detection successful
                    osElement.textContent = data.os;
                    osElement.style.color = '#0056b3'; // Blue color for success
                } else {
                    // Keep polling until we get a result or timeout
                    setTimeout(pollOSDetection, 1000);
                }
            })
            .catch(error => {
                console.error('Error polling OS status:', error);
                osElement.textContent = 'Error';
                osElement.style.color = '#FF3B30'; // Red color for error
                setTimeout(() => {
                    osElement.textContent = 'N/A';
                    osElement.style.color = '#007AFF'; // Blue color for normal state
                }, 2000);
            });
        }
    </script>
</body>
</html>
)=====";