const char LivePayload[] PROGMEM = R"=====(
<!DOCTYPE HTML>
<html>
<head>
    <title>EvilCrowCable-Wind - Live Payload</title>
    <meta charset="utf-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0, maximum-scale=1.0, user-scalable=no">
    <meta name="apple-mobile-web-app-capable" content="yes">
    <meta name="apple-mobile-web-app-status-bar-style" content="black">
    <link rel="stylesheet" href="style.css">
    <script src="javascript.js"></script>
    <style>
        .command-reference, .keys-reference {
            background: rgba(10, 10, 18, 0.8);
            border: 1px solid var(--primary);
            border-radius: 5px;
            margin: 20px 0;
            overflow: hidden;
        }
        .command-reference-toggle, .keys-reference-toggle {
            padding: 15px;
            cursor: pointer;
            background: rgba(0, 0, 0, 0.3);
            color: var(--primary);
            font-weight: bold;
            display: flex;
            justify-content: space-between;
            align-items: center;
        }
        .command-reference-toggle:hover, .keys-reference-toggle:hover {
            background: rgba(0, 242, 255, 0.1);
        }
        .command-table-container, .keys-table-container {
            padding: 0 15px 15px;
            max-height: 300px;
            overflow-y: auto;
        }
    </style>
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

    <div class="cable-wind-logo">LIVE PAYLOAD</div>

    <div class="view-container">
        <div class="form-group">
            <label for="livepayload">Payload Editor:</label>
            <textarea id="livePayloadInput" class="terminal-style" name="livepayload" spellcheck="false"></textarea>
        </div>

        <div class="button-container">
            <button type="button" onclick="runPayload()">Run Payload</button>
            <button type="button" onclick="showSavePayloadForm()">Save Payload</button>
        </div>

        <!-- Hidden metadata form that appears when saving -->
        <div id="metadataForm" style="display: none;">
            <div class="form-group">
                <label for="payloadName">Payload Name:</label>
                <input type="text" id="payloadName" name="payloadName" placeholder="Enter a descriptive name" class="terminal-style">
            </div>
            <div class="form-group">
                <label for="payloadDescription">Description:</label>
                <textarea id="payloadDescription" name="payloadDescription" placeholder="Enter a brief description" rows="3" class="terminal-style"></textarea>
            </div>
            <div class="button-container">
                <button type="button" onclick="confirmSavePayload()">Confirm Save</button>
                <button type="button" onclick="cancelSavePayload()" style="background: linear-gradient(135deg, var(--error), #cc0022);">Cancel</button>
            </div>
        </div>

        <div class="command-reference">
            <div class="command-reference-toggle" onclick="toggleSection(this)">
                <span>Syntax Reference</span>
                <span>▼</span>
            </div>
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
                        <tr><td class="command-cell">DetectOS</td><td>DetectOS</td><td>Detect the operating system</td></tr>
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
                        <tr><td class="command-cell">Gui</td><td>Gui</td><td>Press WINDOWS on the victim's computer</td></tr>
                        <tr><td class="command-cell">AltF2</td><td>AltF2</td><td>Press ALT+F2 on the victim's computer</td></tr>
                        <tr><td class="command-cell">GuiSpace</td><td>GuiSpace</td><td>Press WINDOWS+SPACE on the victim's computer</td></tr>
                        <tr><td class="command-cell">Print</td><td>Print Hello World!</td><td>Prints a text on the victim's computer</td></tr>
                        <tr><td class="command-cell">WinPrint</td><td>WinPrint Hello World!</td><td>Print a text on the victim's computer without knowing the layout (Windows only)</td></tr>
                        <tr><td class="command-cell">PrintLine</td><td>PrintLine Hello World!</td><td>Prints a text and presses the ENTER key on the victim's computer</td></tr>
                        <tr><td class="command-cell">WinPrintLine</td><td>WinPrintLine Hello World!</td><td>Prints a text and presses the ENTER key on the victim's computer without knowing the layout (Windows only)</td></tr>
                        <tr><td class="command-cell">Delay</td><td>Delay 5000</td><td>Delay in ms</td></tr>
                        <tr><td class="command-cell">Press</td><td>Press KEY_ENTER</td><td>Press a key or modifier on the victim's computer</td></tr>
                        <tr><td class="command-cell">PressRelease</td><td>PressRelease KEY_ENTER</td><td>Press and Release a key or modifier on the victim's computer</td></tr>
                        <tr><td class="command-cell">Release</td><td>Release</td><td>Release all pressed keys</td></tr>
                    </tbody>
                </table>
            </div>
        </div>

        <div class="keys-reference">
            <div class="keys-reference-toggle" onclick="toggleSection(this)">
                <span>Keys and Modifiers Syntax</span>
                <span>▼</span>
            </div>
            <div class="keys-table-container" style="display: none;">
                <table class="command-table">
                    <thead>
                        <tr>
                            <th>Keys and Modifiers - Press</th>
                            <th>Keys - PressRelease</th>
                        </tr>
                    </thead>
                    <tbody>
                        <tr><td>a - Z, 0 - 9, Special characters</td><td>Example: Press a or PressRelease a</td></tr>
                        <tr><td class="command-cell">Press KEY_LEFT_ALT</td><td>Only press available for modifiers</td></tr>
                        <tr><td class="command-cell">Press KEY_LEFT_GUI</td><td>Only press available for modifiers</td></tr>
                        <tr><td class="command-cell">Press KEY_LEFT_CTRL</td><td>Only press available for modifiers</td></tr>
                        <tr><td class="command-cell">Press KEY_LEFT_SHIFT</td><td>Only press available for modifiers</td></tr>
                        <tr><td class="command-cell">Press KEY_RIGHT_ALT</td><td>Only press available for modifiers</td></tr>
                        <tr><td class="command-cell">Press KEY_RIGHT_GUI</td><td>Only press available for modifiers</td></tr>
                        <tr><td class="command-cell">Press KEY_RIGHT_CTRL</td><td>Only press available for modifiers</td></tr>
                        <tr><td class="command-cell">Press KEY_RIGHT_SHIFT</td><td>Only press available for modifiers</td></tr>
                        <tr><td class="command-cell">Press KEY_ENTER</td><td class="command-cell">PressRelease KEY_ENTER</td></tr>
                        <tr><td class="command-cell">Press KEY_UP_ARROW</td><td class="command-cell">PressRelease KEY_UP_ARROW</td></tr>
                        <tr><td class="command-cell">Press KEY_DOWN_ARROW</td><td class="command-cell">PressRelease KEY_DOWN_ARROW</td></tr>
                        <tr><td class="command-cell">Press KEY_LEFT_ARROW</td><td class="command-cell">PressRelease KEY_LEFT_ARROW</td></tr>
                        <tr><td class="command-cell">Press KEY_RIGHT_ARROW</td><td class="command-cell">PressRelease KEY_RIGHT_ARROW</td></tr>
                        <tr><td class="command-cell">Press KEY_BACKSPACE</td><td class="command-cell">PressRelease KEY_BACKSPACE</td></tr>
                        <tr><td class="command-cell">Press KEY_TAB</td><td class="command-cell">PressRelease KEY_TAB</td></tr>
                        <tr><td class="command-cell">Press KEY_PAUSE</td><td class="command-cell">PressRelease KEY_PAUSE</td></tr>
                        <tr><td class="command-cell">Press KEY_INSERT</td><td class="command-cell">PressRelease KEY_INSERT</td></tr>
                        <tr><td class="command-cell">Press KEY_DELETE</td><td class="command-cell">PressRelease KEY_DELETE</td></tr>
                        <tr><td class="command-cell">Press KEY_PAGE_UP</td><td class="command-cell">PressRelease KEY_PAGE_UP</td></tr>
                        <tr><td class="command-cell">Press KEY_PAGE_DOWN</td><td class="command-cell">PressRelease KEY_PAGE_DOWN</td></tr>
                        <tr><td class="command-cell">Press KEY_ESC</td><td class="command-cell">PressRelease KEY_ESC</td></tr>
                        <tr><td class="command-cell">Press KEY_SPACE</td><td class="command-cell">PressRelease KEY_SPACE</td></tr>
                        <tr><td class="command-cell">Press KEY_HOME</td><td class="command-cell">PressRelease KEY_HOME</td></tr>
                        <tr><td class="command-cell">Press KEY_END</td><td class="command-cell">PressRelease KEY_END</td></tr>
                        <tr><td class="command-cell">Press KEY_CAPS_LOCK</td><td class="command-cell">PressRelease KEY_CAPS_LOCK</td></tr>
                        <tr><td class="command-cell">Press KEY_PRINT_SCREEN</td><td class="command-cell">PressRelease KEY_PRINT_SCREEN</td></tr>
                        <tr><td class="command-cell">Press KEY_SCROLL_LOCK</td><td class="command-cell">PressRelease KEY_SCROLL_LOCK</td></tr>
                        <tr><td class="command-cell">Press KEY_NUM_LOCK</td><td class="command-cell">PressRelease KEY_NUM_LOCK</td></tr>
                        <tr><td class="command-cell">Press KEY_MENU</td><td class="command-cell">PressRelease KEY_MENU</td></tr>
                        <tr><td class="command-cell">Press LED_NUMLOCK</td><td class="command-cell">PressRelease LED_NUMLOCK</td></tr>
                        <tr><td class="command-cell">Press LED_CAPSLOCK</td><td class="command-cell">PressRelease LED_CAPSLOCK</td></tr>
                        <tr><td class="command-cell">Press LED_SCROLLLOCK</td><td class="command-cell">PressRelease LED_SCROLLLOCK</td></tr>
                        <tr><td class="command-cell">Press LED_COMPOSE</td><td class="command-cell">PressRelease LED_COMPOSE</td></tr>
                        <tr><td class="command-cell">Press LED_KANA</td><td class="command-cell">PressRelease LED_KANA</td></tr>
                        <tr><td class="command-cell">Press KEY_F1</td><td class="command-cell">PressRelease KEY_F1</td></tr>
                        <tr><td class="command-cell">Press KEY_F2</td><td class="command-cell">PressRelease KEY_F2</td></tr>
                        <tr><td class="command-cell">Press KEY_F3</td><td class="command-cell">PressRelease KEY_F3</td></tr>
                        <tr><td class="command-cell">Press KEY_F4</td><td class="command-cell">PressRelease KEY_F4</td></tr>
                        <tr><td class="command-cell">Press KEY_F5</td><td class="command-cell">PressRelease KEY_F5</td></tr>
                        <tr><td class="command-cell">Press KEY_F6</td><td class="command-cell">PressRelease KEY_F6</td></tr>
                        <tr><td class="command-cell">Press KEY_F7</td><td class="command-cell">PressRelease KEY_F7</td></tr>
                        <tr><td class="command-cell">Press KEY_F8</td><td class="command-cell">PressRelease KEY_F8</td></tr>
                        <tr><td class="command-cell">Press KEY_F9</td><td class="command-cell">PressRelease KEY_F9</td></tr>
                        <tr><td class="command-cell">Press KEY_F10</td><td class="command-cell">PressRelease KEY_F10</td></tr>
                        <tr><td class="command-cell">Press KEY_F11</td><td class="command-cell">PressRelease KEY_F11</td></tr>
                        <tr><td class="command-cell">Press KEY_F12</td><td class="command-cell">PressRelease KEY_F12</td></tr>
                    </tbody>
                </table>
            </div>
        </div>
    </div>

    <script>
        // Toggle section visibility
        function toggleSection(element) {
            const container = element.parentElement;
            const content = container.querySelector('.command-table-container, .keys-table-container');
            const arrow = element.querySelector('span:last-child');
            
            if (content.style.display === 'none') {
                content.style.display = 'block';
                arrow.textContent = '▲';
            } else {
                content.style.display = 'none';
                arrow.textContent = '▼';
            }
        }

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

        function showSavePayloadForm() {
            const toggle = document.getElementById('toggleSave');
            const payloadContent = document.getElementById('livePayloadInput').value;
            
            if (!payloadContent) {
                showMessage('error', 'Payload content cannot be empty!');
                return;
            }
            
            document.getElementById('metadataForm').style.display = 'block';
            document.getElementById('metadataForm').scrollIntoView({ behavior: 'smooth' });
        }

        function cancelSavePayload() {
            document.getElementById('metadataForm').style.display = 'none';
        }

        function confirmSavePayload() {
            const payloadContent = document.getElementById('livePayloadInput').value;
            const payloadName = document.getElementById('payloadName').value;
            const payloadDesc = document.getElementById('payloadDescription').value;
            
            const formData = new FormData();
            formData.append('livepayload', payloadContent);
            formData.append('payloadName', payloadName || 'Unnamed Payload');
            formData.append('payloadDescription', payloadDesc || 'No description provided');

            fetch('/runlivesave', {
                method: 'POST',
                body: formData
            })
            .then(response => {
                if (response.ok) return response.text();
                else throw new Error('Error saving payload');
            })
            .then(data => {
                showMessage('success', 'Payload saved successfully!');
                document.getElementById('metadataForm').style.display = 'none';
                document.getElementById('payloadName').value = '';
                document.getElementById('payloadDescription').value = '';
            })
            .catch(error => {
                showMessage('error', 'Error saving payload.');
                console.error('Error:', error);
            });
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

            // Auto-remove after 5 seconds
            const timer = setTimeout(() => {
                toast.style.animation = 'toastFadeOut 0.3s ease-out';
                setTimeout(() => toast.remove(), 300);
            }, 5000);

            // Update close handler to also clear the timer
            closeButton.onclick = () => {
                clearTimeout(timer);
                toast.style.animation = 'toastFadeOut 0.3s ease-out';
                setTimeout(() => toast.remove(), 300);
            };
        }

        // Command cell click handlers
        document.querySelectorAll('.command-cell').forEach(cell => {
            cell.addEventListener('click', function() {
                const textarea = document.getElementById('livePayloadInput');
                const command = this.textContent;

                if (textarea.value === '') {
                    textarea.value = command;
                } else {
                    if (textarea.value.slice(-1) !== '\n') { 
                        textarea.value += '\n';
                    }
                    textarea.value += command;
                }

                textarea.focus();
                textarea.scrollTop = textarea.scrollHeight;

                this.style.backgroundColor = 'rgba(0, 242, 255, 0.3)';
                setTimeout(() => {
                    this.style.backgroundColor = '';
                }, 300);
            });

            cell.style.cursor = 'pointer';
        });
    </script>
</body>
</html>
)=====";