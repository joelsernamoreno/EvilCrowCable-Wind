const char Index[] PROGMEM = R"=====(
<!DOCTYPE HTML>
<html>
<head>
    <title>EvilCrowCable-Wind</title>
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

    <div class="cable-wind-logo">EvilCrowCable-Wind</div>

    <div class="view-container">
        <div class="stat-container">
            <div class="stat-group">
                <strong>Connection Status: <span class="status-indicator"></span></strong>
            </div>
            <div class="stat-group">
                <strong>Firmware:</strong> <span id="firmware">v1.3</span>
            </div>
            <div class="stat-group">
                <strong>Target OS:</strong> <span id="targetos" class="clickable-os" onclick="detectOS()">N/A</span>
            </div>
            <div class="stat-group">
                <strong>Uptime:</strong> <span id="uptime"></span>
            </div>
            <div class="stat-group">
                <strong>CPU Temperature:</strong> <span id="temperature"></span>
            </div>
            <div class="stat-group">
                <strong>CPU Core 0 Frequency:</strong> <span id="cpu0"></span>
            </div>
            <div class="stat-group">
                <strong>CPU Core 1 Frequency:</strong> <span id="cpu1"></span>
            </div>
            <div class="stat-group">
                <strong>Free SPIFFS Memory:</strong> <span id="freespiffs"></span>
            </div>
            <div class="stat-group">
                <strong>Total RAM:</strong> <span id="totalram"></span>
            </div>
            <div class="stat-group">
                <strong>Free RAM:</strong> <span id="freeram"></span>
            </div>
        </div>
    </div>

    <script>
        // JavaScript
        function detectOS() {
            const osElement = document.getElementById('targetos');
            osElement.textContent = 'Detecting...';
            osElement.style.color = '#00f2ff';
            
            fetch('/runlivepayload', {
                method: 'POST',
                headers: {
                    'Content-Type': 'application/x-www-form-urlencoded',
                },
                body: 'livepayload=DetectOS'
            })
            .then(response => {
                if (response.ok) {
                    pollOSDetection();
                } else {
                    throw new Error('Detection failed');
                }
            })
            .catch(error => {
                console.error('Error detecting OS:', error);
                osElement.textContent = 'Error';
                osElement.style.color = '#ff0033';
                setTimeout(() => {
                    osElement.textContent = 'N/A';
                    osElement.style.color = '#00f2ff';
                }, 2000);
            });
        }

        function pollOSDetection() {
            const osElement = document.getElementById('targetos');
            
            fetch('/stats')
            .then(response => response.json())
            .then(data => {
                if (data.os && data.os !== 'OS Unknown' && data.os !== 'N/A') {
                    osElement.textContent = data.os;
                    osElement.style.color = '#00ff88';
                } else {
                    setTimeout(pollOSDetection, 1000);
                }
            })
            .catch(error => {
                console.error('Error polling OS status:', error);
                osElement.textContent = 'Error';
                osElement.style.color = '#ff0033';
                setTimeout(() => {
                    osElement.textContent = 'N/A';
                    osElement.style.color = '#00f2ff';
                }, 2000);
            });
        }

        // Connection check
        function checkConnection() {
            fetch('/stats')
                .then(response => {
                    document.querySelectorAll('.status-indicator').forEach(indicator => {
                        indicator.classList.remove('status-offline');
                        indicator.classList.add('status-online');
                    });
                    return response.json();
                })
                .then(data => {
                    if (document.location.pathname === '/') {
                        document.getElementById('uptime').innerText = data.uptime + ' seconds';
                        document.getElementById('cpu0').innerText = data.cpu0 + ' MHz';
                        document.getElementById('cpu1').innerText = data.cpu1 + ' MHz';
                        document.getElementById('freespiffs').innerText = data.freespiffs + ' bytes';
                        document.getElementById('temperature').innerText = data.temperature + ' °C';
                        document.getElementById('totalram').innerText = data.totalram + ' bytes';
                        document.getElementById('freeram').innerText = data.freeram + ' bytes';
                        document.getElementById("targetos").textContent = data.os || "N/A";
                    }
                })
                .catch(error => {
                    if (error.name !== 'AbortError') { 
                        document.querySelectorAll('.status-indicator').forEach(indicator => {
                            indicator.classList.remove('status-online');
                            indicator.classList.add('status-offline');
                        });
                    }
                });
        }
        setInterval(checkConnection, 5000);
        checkConnection();
    </script>
</body>
</html>
)=====";