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
    
    <label for="livepayload">Payload:</label><br>
    <form id="payloadForm">
        <textarea id="livePayloadInput" class="payload-input" name="livepayload"></textarea>
    </form>

    <div class="button-container">
        <button type="button" onclick="copyPayload()" class="copy-button">Copy Payload</button>
    </div>

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

    <div class="command-reference">
        <h3 class="command-reference-toggle">Syntax Reference ▼</h3>
        <div class="command-table-container" style="display: none;">
            <table class="command-table">
                <thead>
                    <tr>
                        <th>Command</th>
                        <th>Example</th>
                        <th>Description</th>
                    </tr>
                </thead>
                <tbody>
					<tr><td class="command-cell">ServerConnect</td><td>ServerConnect IP</td><td>Read the "Remote Shell" section for more information</td></tr>
                    <tr><td class="command-cell">RunWin</td><td>RunWin cmd</td><td>Runs a command or a program on the victim's computer</td></tr>
                    <tr><td class="command-cell">RunPowershellAdmin</td><td>RunPowershellAdmin</td><td>Run powershell as administrator</td></tr>
                    <tr><td class="command-cell">RunCmdAdmin</td><td>RunCmdAdmin</td><td>Run cmd as administrator</td></tr>
                    <tr><td class="command-cell">ShellWin</td><td>ShellWin IP</td><td>Read the "Remote Shell" section for more information</td></tr>
                    <tr><td class="command-cell">RunNix</td><td>RunNix getit</td><td>Runs a command or a program on the victim's computer</td></tr>
                    <tr><td class="command-cell">RunLauncher</td><td>RunLauncher Libreoffice</td><td>Runs a command or a program on the victim's computer</td></tr>
                    <tr><td class="command-cell">CtrlAltT</td><td>CtrlAltT</td><td>Run a terminal on the victim's computer</td></tr>
                    <tr><td class="command-cell">ShellNix</td><td>ShellNix IP</td><td>Read the "Remote Shell" section for more information</td></tr>
                    <tr><td class="command-cell">ShellMac</td><td>ShellMac IP</td><td>Read the "Remote Shell" section for more information</td></tr>
                    <tr><td class="command-cell">ShellMacCleanup</td><td>ShellMacCleanup</td><td>Read the "Remote Shell" section for more information</td></tr>
                    <tr><td class="command-cell">RunMac</td><td>RunMac terminal</td><td>Runs a command or a program on the victim's computer</td></tr>
                    <tr><td class="command-cell">GuiR</td><td>GuiR</td><td>Press WINDOWS+R on the victim's computer</td></tr>
                    <tr><td class="command-cell">AltF2</td><td>AltF2</td><td>Press ALT+F2 on the victim's computer</td></tr>
                    <tr><td class="command-cell">GuiSpace</td><td>GuiSpace</td><td>Press WINDOWS+SPACE on the victim's computer</td></tr>
                    <tr><td class="command-cell">Print</td><td>Print Hello World!</td><td>Prints a text on the victim's computer</td></tr>
                    <tr><td class="command-cell">PrintLine</td><td>Printable Hello World!</td><td>Prints a text and presses the ENTER key on the victim's computer</td></tr>
                    <tr><td class="command-cell">Delay</td><td>Delay 5000</td><td>Delay in ms</td></tr>
                    <tr><td class="command-cell">Press</td><td>Press KEY_ENTER</td><td>Press a key or modifier on the victim's computer</td></tr>
                    <tr><td class="command-cell">PressRelease</td><td>PressRelease KEY_ENTER</td><td>Press and Release a key or modifier on the victim's computer</td></tr>
                    <tr><td class="command-cell">Release</td><td>Release</td><td>Release all pressed keys</td></tr>
                </tbody>
            </table>
        </div>
    </div>

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

        function copyPayload() {
            const payloadText = document.getElementById('livePayloadInput');
            payloadText.select();
            document.execCommand('copy');
            
            // Show a brief message to indicate the copy was successful
            const originalText = payloadText.value;
            if (originalText.length > 0) {
                showMessage('success', 'Payload copied to clipboard!');
            } else {
                showMessage('error', 'No payload to copy!');
            }
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

        document.querySelector('.command-reference-toggle').addEventListener('click', function() {
            const container = this.nextElementSibling;
            if (container.style.display === 'none') {
                container.style.display = 'block';
                this.innerHTML = 'Syntax Reference ▲';
            } else {
                container.style.display = 'none';
                this.innerHTML = 'Syntax Reference ▼';
            }
        });

        document.querySelectorAll('.command-cell').forEach(cell => {
            cell.addEventListener('click', function() {
                const textarea = document.getElementById('livePayloadInput');
                const command = this.textContent;
                
                if (textarea.value === '') {
                    textarea.value = command;
                } else {
                    // Add new line if last character isn't already a newline
                    if (textarea.value.slice(-1) !== '\n') {
                        textarea.value += '\n';
                    }
                    textarea.value += command;
                }
                
                // Focus the textarea and scroll to bottom
                textarea.focus();
                textarea.scrollTop = textarea.scrollHeight;
                
                // Optional: Highlight the newly added command briefly
                this.style.backgroundColor = 'rgba(0, 122, 255, 0.3)';
                setTimeout(() => {
                    this.style.backgroundColor = '';
                }, 300);
            });
            
            // Add pointer cursor to command cells
            cell.style.cursor = 'pointer';
        });

    </script>
</body>
</html>
)=====";