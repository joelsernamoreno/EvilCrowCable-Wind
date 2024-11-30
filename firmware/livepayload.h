const char LivePayload[] PROGMEM = R"=====(
<!DOCTYPE HTML>
<html>
<head>
    <title>Live Payload</title>
    <meta charset="utf-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0, maximum-scale=1.0, user-scalable=no">
    <meta name="apple-mobile-web-app-capable" content="yes">
    <meta name="apple-mobile-web-app-status-bar-style" content="black">
    <link rel="stylesheet" href="style.css">
    <script src="javascript.js"></script>
</head>
<body>

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
    
    <label for="livepayload">Payload:</label><br>
    <form id="payloadForm">
        <textarea id="livePayloadInput" class="payload-input" name="livepayload"></textarea>
    </form>

    <div class="switch-container">
        <div class="switch-group">
            <span class="switch-label">Run payload</span>
            <label class="switch">
                <input type="checkbox" id="togglePayload" onclick="runPayload()">
                <span class="slider round"></span>
            </label>
        </div>

        <div class="switch-group">
            <span class="switch-label">Startup exec payload</span>
            <label class="switch">
                <input type="checkbox" id="toggleStartup" onclick="toggleStartupPayload()">
                <span class="slider round"></span>
            </label>
        </div>

        <div class="switch-group">
            <span class="switch-label">Save payload</span>
            <label class="switch">
                <input type="checkbox" id="toggleSave" onclick="savePayload()">
                <span class="slider round"></span>
            </label>
        </div>
    </div>

    <div id="messageContainer" class="messages-container"></div>

    <script>
        let toggleIntervals = {};

        function checkPayloadStatus(endpoint, toggleId) {
            fetch(endpoint)
            .then(response => response.json())
            .then(data => {
                if (data.status === "executed") {
                  document.getElementById(toggleId).checked = false;
                  clearInterval(toggleIntervals[toggleId]);
                }
            })
            .catch(error => console.error("Error checking payload status:", error));
        }

        function showMessage(type, text) {
            const container = document.querySelector('.messages-container');
            const messageDiv = document.createElement('div');
            messageDiv.className = `message ${type}`;
            messageDiv.textContent = text;

            if (type === 'error') {
                messageDiv.style.color = 'red';
            } else if (type === 'success') {
                messageDiv.style.color = 'green';
            }

            container.innerHTML = '';
            container.appendChild(messageDiv);

            setTimeout(() => {
                container.innerHTML = '';
            }, 5000);
        }

        function runPayload() {
            const toggle = document.getElementById('togglePayload');
            const payloadContent = document.getElementById('livePayloadInput').value;

            if (!payloadContent) {
                showMessage('error', 'Payload content cannot be empty!');
                toggle.checked = false;
                return;
            }

            fetch('/runlivepayload', {
                method: 'POST',
                body: new URLSearchParams({ livepayload: payloadContent }),
            })
            .then(response => {
                if (response.ok) return response.text();
                else throw new Error('Error running payload');
            })
            .then(data => {
                showMessage('success', 'Payload running...');
                toggleIntervals['togglePayload'] = setInterval(() => {
                    checkPayloadStatus('/payloadstatuspayload', 'togglePayload');
                }, 1000);
            })
            .catch(error => {
                showMessage('error', 'Error executing payload.');
                console.error('Error:', error);
                toggle.checked = false;
            });
        }

        function savePayload() {
            const toggle = document.getElementById('toggleSave');
            const payloadContent = document.getElementById('livePayloadInput').value;

            if (!payloadContent) {
                showMessage('error', 'Payload content cannot be empty!');
                toggle.checked = false;
                return;
            }

            fetch('/runlivesave', {
                method: 'POST',
                body: new URLSearchParams({ livepayload: payloadContent }),
            })
            .then(response => {
                if (response.ok) return response.text();
                else throw new Error('Error saving payload');
            })
            .then(data => {
                showMessage('success', 'Payload saving...');
                toggleIntervals['toggleSave'] = setInterval(() => {
                    checkPayloadStatus('/payloadstatussave', 'toggleSave');
                }, 1000);
            })
            .catch(error => {
                showMessage('error', 'Error saving payload.');
                console.error('Error:', error);
                toggle.checked = false;
            });
        }

        function toggleStartupPayload() {
            const toggle = document.getElementById('toggleStartup');
            const payloadContent = document.getElementById('livePayloadInput').value;

            if (toggle.checked && !payloadContent) {
                showMessage('error', 'Please provide payload content before enabling Startup Payload.');
                toggle.checked = false;
                return;
            }

            fetch('/runlivestartup', {
                method: 'POST',
                body: new URLSearchParams({ livepayload: payloadContent }),
            })
            .then(response => {
                if (response.ok) return response.text();
                else throw new Error('Error saving startup payload');
            })
            .then(data => {
                showMessage('success', 'Startup payload saving...');
                toggleIntervals['toggleStartup'] = setInterval(() => {
                    checkPayloadStatus('/payloadstatusstartup', 'toggleStartup');
                }, 1000);
            })
            .catch(error => {
                showMessage('error', 'Error saving startup payload.');
                console.error('Error:', error);
                toggle.checked = false;
            });
        }
    </script>
</body>
</html>
)=====";