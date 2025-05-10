const char StaticFileList[] PROGMEM = R"=====(
<!DOCTYPE HTML>
<html>
<head>
    <title>Payload</title>
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

    <h3>{{path}}:</h3>
    <div class="payload-container">
        <pre id="payloadContent">{{payloadContent}}</pre>
    </div>

    <div class="button-container">
        <button type="button" onclick="copyPayloadContent()" class="copy-button">Copy Payload</button>
    </div>

    <div class="switch-container">
        <div class="switch-group">
            <span class="switch-label">Run Payload</span>
            <label class="switch">
                <input type="checkbox" id="runPayloadCheckbox">
                <span class="slider round"></span>
            </label>
        </div>

        <div class="switch-group">
            <span class="switch-label">Delete Payload</span>
            <label class="switch">
                <input type="checkbox" id="deletePayloadCheckbox">
                <span class="slider round"></span>
            </label>
        </div>
    </div>

    <script>
        function showMessage(type, text) {
            const container = document.getElementById('global-toast');
            const toast = document.createElement('div');
            toast.className = `toast-message ${type}`;

            const messageSpan = document.createElement('span');
            messageSpan.textContent = text;

            const closeButton = document.createElement('span');
            closeButton.className = 'toast-close';
            closeButton.innerHTML = '&times;';
            closeButton.onclick = () => {
                toast.style.animation = 'toastFadeOut 0.3s ease-out';
                setTimeout(() => toast.remove(), 300);
            };

            toast.appendChild(messageSpan);
            toast.appendChild(closeButton);
            container.appendChild(toast);

            const timer = setTimeout(() => {
                toast.style.animation = 'toastFadeOut 0.3s ease-out';
                setTimeout(() => toast.remove(), 300);
            }, 5000);

            closeButton.onclick = () => {
                clearTimeout(timer);
                toast.style.animation = 'toastFadeOut 0.3s ease-out';
                setTimeout(() => toast.remove(), 300);
            };
        }

        function copyPayloadContent() {
            const payloadText = document.getElementById('payloadContent').textContent;

            // Create a temporary textarea element
            const tempTextArea = document.createElement('textarea');
            tempTextArea.value = payloadText;
            document.body.appendChild(tempTextArea);

            // Select and copy the text
            tempTextArea.select();
            document.execCommand('copy');

            // Remove the temporary element
            document.body.removeChild(tempTextArea);
        
            if (payloadText.length > 0) {
                showMessage('success', 'Payload copied to clipboard!');
            } else {
                showMessage('error', 'No payload to copy!');
            }
        }

        document.getElementById('runPayloadCheckbox').addEventListener('change', function() {
            handleToggle('/dopayload', this.checked);
        });

        document.getElementById('deletePayloadCheckbox').addEventListener('change', function() {
            handleToggle('/deletepayload', this.checked);
        });

        async function handleToggle(url, checked) {
            if (checked) {
                let response = await fetch(url, {
                    method: 'POST',
                    body: new URLSearchParams({configmodule:'{{path}}'})
                });
                let message = await response.text();
                showMessage('success', message);

                setTimeout(function() {
                    document.getElementById('runPayloadCheckbox').checked = false;
                    document.getElementById('deletePayloadCheckbox').checked = false;
                }, 2000); 
            } else {
                showMessage('error', 'No action taken');
            }
        }
    </script>
</body>
</html>
)=====";