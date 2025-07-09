const char LivePayload[] PROGMEM = R"=====(
<!DOCTYPE HTML>
<html>
<head>
    <title>EvilCrowCable-Wind - Payload Editor</title>
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
            <li><a href='/livepayload'>Payload Editor</a></li>
            <li><a href='/uploadpayload'>Upload Payload</a></li>
            <li><a href='/listpayloads'>List Payloads</a></li>
            <li><a href='/autoexecplanning'>AutoExec Planning</a></li>
            <li><a href='/config'>Config</a></li>
        </ul>
    </nav>

    <div class="cable-wind-logo">PAYLOAD EDITOR</div>

    <div class="view-container">
        <div class="form-group">
            <div class="validation-container">
                <span id="validationStatus"></span>
            </div>
            <div class="payload-editor-container">
                <div id="lineNumbers" class="line-numbers"></div>
                <textarea id="livePayloadInput" class="terminal-style" name="livepayload" spellcheck="false"></textarea>
            </div>
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
            <div class="form-group">
                <label for="payloadOS">Target OS:</label>
                <select id="payloadOS" class="styled-select" name="payloadOS" required>
                  <option value="unknown">Select OS</option>
                  <option value="windows">Windows</option>
                  <option value="linux">Linux</option>
                  <option value="android">Android</option>
                  <option value="macos">macOS</option>
                  <option value="ios">iOS</option>
                </select>
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
                        <tr><td class="command-cell"># Layout:</td><td># Layout: EN_US</td><td>Set keyboard layout for the payload. Must be the first line.</td></tr>
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
        // Global variables for autocomplete
        let commandSuggestions = {};
        let currentSuggestion = null;
        let suggestionElement = null;
        
        // Global commands list
        const commands = [
            // Special configuration commands
            '# Layout:',  // Special command for keyboard layout configuration
            // Main commands
            'ServerConnect',
            'DetectOS',
            'RunWin',
            'RunPowershellAdmin',
            'RunCmdAdmin',
            'ShellWin',
            'RunNix',
            'RunLauncher',
            'CtrlAltT',
            'ShellNix',
            'ShellMac',
            'ShellMacCleanup',
            'RunMac',
            'GuiR',
            'Gui',
            'AltF2',
            'GuiSpace',
            'Print',
            'WinPrint',
            'PrintLine',
            'WinPrintLine',
            'Delay',
            'Press',
            'PressRelease',
            'Release',
            // Key constants for Press/PressRelease
            'KEY_LEFT_ALT',
            'KEY_LEFT_GUI',
            'KEY_LEFT_CTRL',
            'KEY_LEFT_SHIFT',
            'KEY_RIGHT_ALT',
            'KEY_RIGHT_GUI',
            'KEY_RIGHT_CTRL',
            'KEY_RIGHT_SHIFT',
            'KEY_ENTER',
            'KEY_UP_ARROW',
            'KEY_DOWN_ARROW',
            'KEY_LEFT_ARROW',
            'KEY_RIGHT_ARROW',
            'KEY_BACKSPACE',
            'KEY_TAB',
            'KEY_PAUSE',
            'KEY_INSERT',
            'KEY_DELETE',
            'KEY_PAGE_UP',
            'KEY_PAGE_DOWN',
            'KEY_ESC',
            'KEY_SPACE',
            'KEY_HOME',
            'KEY_END',
            'KEY_CAPS_LOCK',
            'KEY_PRINT_SCREEN',
            'KEY_SCROLL_LOCK',
            'KEY_NUM_LOCK',
            'KEY_MENU',
            'LED_NUMLOCK',
            'LED_CAPSLOCK',
            'LED_SCROLLLOCK',
            'LED_COMPOSE',
            'LED_KANA',
            'KEY_F1',
            'KEY_F2',
            'KEY_F3',
            'KEY_F4',
            'KEY_F5',
            'KEY_F6',
            'KEY_F7',
            'KEY_F8',
            'KEY_F9',
            'KEY_F10',
            'KEY_F11',
            'KEY_F12'
        ];
        
        // Generate suggestions on load
        commandSuggestions = generateCommandSuggestions(commands);
        
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

        function validatePayload() {
            const payloadInput = document.getElementById('livePayloadInput');
            const validationStatus = document.getElementById('validationStatus');
            const text = payloadInput.value.trim(); // Get trimmed content first

            // Clear previous validation
            validationStatus.className = '';
            validationStatus.textContent = '';
            validationStatus.title = '';

            // Check for empty payload (primary check)
            if (text === '') {
                validationStatus.className = 'validation-empty validation-text';
                validationStatus.textContent = 'Empty payload';
                return; // Exit early if empty
            }

            // Only proceed with line validation if there's content
            const lines = text.split('\n');
            let errors = [];

            lines.forEach((line, index) => {
                line = line.trim();
                if (!line) return; // Skip empty lines

                const parts = line.split(/\s+/);
                const command = parts[0];
                const args = parts.slice(1).join(' ');

                const validationResult = isValidCommand(command, args, index + 1);
                if (!validationResult.valid) {
                    errors.push(`Line ${index + 1}: ${validationResult.message}`);
                }
            });

            // Display validation results
            if (errors.length > 0) {
                validationStatus.className = 'validation-error validation-text';
                validationStatus.textContent = errors.join('\n');
                validationStatus.title = errors.join('\n');
            } else {
                validationStatus.className = 'validation-ok validation-text';
                validationStatus.textContent = 'All commands are valid';
            }
        }

        function isValidCommand(command, args, lineNumber) {
            // Enhanced command validation with specific rules
            const commandRules = {
                '# Layout:': {
                    validate: args => {
                        // Valid layouts from firmware.ino
                        const validLayouts = [
                            'EN_US', 'ES_ES', 'FR_FR', 'IT_IT', 'DA_DK', 
                            'DE_DE', 'HR_HR', 'HU_HU', 'PT_PT', 'PT_BR', 
                            'BE_BE', 'BR_BR', 'CA_CA', 'CA_FR', 'SK_SK', 
                            'CZ_CZ', 'SV_SE', 'SI_SI'
                        ];
                        return validLayouts.includes(args.trim());
                    },
                    message: 'requires valid keyboard layout code (e.g., EN_US, SI_SI)'
                },
                '##': {
                    validate: args => true, // Always valid
                    message: 'Comment line'
                },
                'ServerConnect': {
                    validate: args => args.length > 0,
                    message: 'requires IP address/hostname or ipaddress:port'
                },
                'DetectOS': {
                    validate: args => args.length === 0,
                    message: 'takes no arguments'
                },
                'RunWin': {
                    validate: args => args.length > 0,
                    message: 'requires command to run'
                },
                'RunPowershellAdmin': {
                    validate: args => true, // Can have optional arguments
                    message: 'optional: command to run'
                },
                'RunCmdAdmin': {
                    validate: args => true, // Can have optional arguments
                    message: 'optional: command to run'
                },
                'ShellWin': {
                    validate: args => args.length > 0,
                    message: 'requires IP address or ipaddress:port'
                },
                'RunNix': {
                    validate: args => args.length > 0,
                    message: 'requires command to run'
                },
                'RunLauncher': {
                    validate: args => args.length > 0,
                    message: 'requires application name'
                },
                'CtrlAltT': {
                    validate: args => args.length === 0,
                    message: 'takes no arguments'
                },
                'ShellNix': {
                    validate: args => args.length > 0,
                    message: 'requires IP address or ipaddress:port'
                },
                'ShellMac': {
                    validate: args => args.length > 0,
                    message: 'requires IP address or ipaddress:port'
                },
                'ShellMacCleanup': {
                    validate: args => args.length === 0,
                    message: 'takes no arguments'
                },
                'RunMac': {
                    validate: args => args.length > 0,
                    message: 'requires command or application name'
                },
                'GuiR': {
                    validate: args => args.length === 0,
                    message: 'takes no arguments'
                },
                'Gui': {
                    validate: args => args.length === 0,
                    message: 'takes no arguments'
                },
                'AltF2': {
                    validate: args => args.length === 0,
                    message: 'takes no arguments'
                },
                'GuiSpace': {
                    validate: args => args.length === 0,
                    message: 'takes no arguments'
                },
                'Print': {
                    validate: args => args.length > 0,
                    message: 'requires text to print'
                },
                'WinPrint': {
                    validate: args => args.length > 0,
                    message: 'requires text to print'
                },
                'PrintLine': {
                    validate: args => args.length > 0,
                    message: 'requires text to print'
                },
                'WinPrintLine': {
                    validate: args => args.length > 0,
                    message: 'requires text to print'
                },
                'Delay': {
                    validate: args => {
                        const num = parseInt(args);
                        return args.trim() !== '' && !isNaN(num) && num > 0;
                    },
                    message: 'requires positive number (milliseconds)'
                },
                'Press': {
                    validate: isValidKey,
                    message: 'requires valid key name'
                },
                'PressRelease': {
                    validate: isValidKey,
                    message: 'requires valid key name'
                },
                'Release': {
                    validate: args => args.length === 0,
                    message: 'takes no arguments'
                },
                // Key constants don't need validation as they're used as arguments to Press/PressRelease
                'KEY_LEFT_ALT': { validate: args => false, message: 'not a command but a key parameter' },
                'KEY_LEFT_GUI': { validate: args => false, message: 'not a command but a key parameter' },
                'KEY_LEFT_CTRL': { validate: args => false, message: 'not a command but a key parameter' },
                'KEY_LEFT_SHIFT': { validate: args => false, message: 'not a command but a key parameter' },
                'KEY_RIGHT_ALT': { validate: args => false, message: 'not a command but a key parameter' },
                'KEY_RIGHT_GUI': { validate: args => false, message: 'not a command but a key parameter' },
                'KEY_RIGHT_CTRL': { validate: args => false, message: 'not a command but a key parameter' },
                'KEY_RIGHT_SHIFT': { validate: args => false, message: 'not a command but a key parameter' },
                'KEY_ENTER': { validate: args => false, message: 'not a command but a key parameter' },
                'KEY_UP_ARROW': { validate: args => false, message: 'not a command but a key parameter' },
                'KEY_DOWN_ARROW': { validate: args => false, message: 'not a command but a key parameter' },
                'KEY_LEFT_ARROW': { validate: args => false, message: 'not a command but a key parameter' },
                'KEY_RIGHT_ARROW': { validate: args => false, message: 'not a command but a key parameter' },
                'KEY_BACKSPACE': { validate: args => false, message: 'not a command but a key parameter' },
                'KEY_TAB': { validate: args => false, message: 'not a command but a key parameter' },
                'KEY_PAUSE': { validate: args => false, message: 'not a command but a key parameter' },
                'KEY_INSERT': { validate: args => false, message: 'not a command but a key parameter' },
                'KEY_DELETE': { validate: args => false, message: 'not a command but a key parameter' },
                'KEY_PAGE_UP': { validate: args => false, message: 'not a command but a key parameter' },
                'KEY_PAGE_DOWN': { validate: args => false, message: 'not a command but a key parameter' },
                'KEY_ESC': { validate: args => false, message: 'not a command but a key parameter' },
                'KEY_SPACE': { validate: args => false, message: 'not a command but a key parameter' },
                'KEY_HOME': { validate: args => false, message: 'not a command but a key parameter' },
                'KEY_END': { validate: args => false, message: 'not a command but a key parameter' },
                'KEY_CAPS_LOCK': { validate: args => false, message: 'not a command but a key parameter' },
                'KEY_PRINT_SCREEN': { validate: args => false, message: 'not a command but a key parameter' },
                'KEY_SCROLL_LOCK': { validate: args => false, message: 'not a command but a key parameter' },
                'KEY_NUM_LOCK': { validate: args => false, message: 'not a command but a key parameter' },
                'KEY_MENU': { validate: args => false, message: 'not a command but a key parameter' },
                'LED_NUMLOCK': { validate: args => false, message: 'not a command but a key parameter' },
                'LED_CAPSLOCK': { validate: args => false, message: 'not a command but a key parameter' },
                'LED_SCROLLLOCK': { validate: args => false, message: 'not a command but a key parameter' },
                'LED_COMPOSE': { validate: args => false, message: 'not a command but a key parameter' },
                'LED_KANA': { validate: args => false, message: 'not a command but a key parameter' },
                'KEY_F1': { validate: args => false, message: 'not a command but a key parameter' },
                'KEY_F2': { validate: args => false, message: 'not a command but a key parameter' },
                'KEY_F3': { validate: args => false, message: 'not a command but a key parameter' },
                'KEY_F4': { validate: args => false, message: 'not a command but a key parameter' },
                'KEY_F5': { validate: args => false, message: 'not a command but a key parameter' },
                'KEY_F6': { validate: args => false, message: 'not a command but a key parameter' },
                'KEY_F7': { validate: args => false, message: 'not a command but a key parameter' },
                'KEY_F8': { validate: args => false, message: 'not a command but a key parameter' },
                'KEY_F9': { validate: args => false, message: 'not a command but a key parameter' },
                'KEY_F10': { validate: args => false, message: 'not a command but a key parameter' },
                'KEY_F11': { validate: args => false, message: 'not a command but a key parameter' },
                'KEY_F12': { validate: args => false, message: 'not a command but a key parameter' }
            };

            // Handle empty lines or commands
            if (!command || command === '') {
                return { valid: true }; // Empty lines are valid (allows for comments or spacing)
            }

            // Check if the command exists in our rules
            if (!commandRules[command]) {
                return { valid: false, message: `Unknown command "${command}"` };
            }

            const validation = commandRules[command];
            if (!validation.validate(args)) {
                return { valid: false, message: `${command} ${validation.message}` };
            }

            return { valid: true };
        }

        function isValidKey(key) {
            // Trim whitespace and convert to uppercase for consistent comparison
            key = key.trim().toUpperCase();

            // Check if it's a single printable character (not control characters)
            if (key.length === 1 && key.charCodeAt(0) >= 32 && key.charCodeAt(0) <= 126) {
                return true;
            }

            // Check against our master commands list (already contains all KEY_* constants)
            return commands.some(cmd => 
                cmd.toUpperCase() === key && 
                (cmd.startsWith('KEY_') || cmd.startsWith('LED_'))
            );
        }

        function generateCommandSuggestions(commands) {
            const suggestions = {};
            commands.forEach(cmd => {
                for (let i = 2; i <= cmd.length; i++) {
                    const prefix = cmd.substring(0, i);
                    const remaining = cmd.substring(i);
                    if (!suggestions[prefix] || remaining.length < suggestions[prefix].length) {
                        suggestions[prefix] = remaining;
                    }
                }
            });
            return suggestions;
        }

        function updateLineNumbers() {
            const textarea = document.getElementById('livePayloadInput');
            const lineNumbers = document.getElementById('lineNumbers');
            const lines = textarea.value.split('\n');

            // Get computed styles for accurate measurements
            const textareaStyle = getComputedStyle(textarea);
            const lineHeight = parseFloat(textareaStyle.lineHeight);
            const paddingTop = parseFloat(textareaStyle.paddingTop);
            const paddingBottom = parseFloat(textareaStyle.paddingBottom);

            // Calculate total height needed
            const totalHeight = lines.length * lineHeight + paddingTop + paddingBottom;

            // Generate line numbers HTML
            lineNumbers.innerHTML = Array(lines.length).fill().map((_, i) => 
                `<div style="height: ${lineHeight}px; line-height: ${lineHeight}px">${i + 1}</div>`
            ).join('');

            // Sync scroll positions
            lineNumbers.scrollTop = textarea.scrollTop;
        }

        function validateCurrentLine(e) {
            const textarea = document.getElementById('livePayloadInput');
            const validationStatus = document.getElementById('validationStatus');
            const content = textarea.value.trim();
        
            // Always handle auto-suggestion on any input
            handleAutoSuggestion();
        
            // First check if the entire payload is empty
            if (content === '') {
                validationStatus.className = 'validation-empty validation-text';
                validationStatus.textContent = 'Empty payload';
                validationStatus.title = 'Empty payload';
                return;
            }
        
            const lines = textarea.value.split('\n');
            const currentLineNumber = textarea.value.substr(0, textarea.selectionStart).split('\n').length;
            const currentLine = lines[currentLineNumber - 1].trim();
        
            // Only validate on certain input events
            const shouldValidate = e.inputType === 'insertLineBreak' || 
                                e.inputType === 'insertText' || 
                                e.inputType === 'deleteContentBackward';
        
            if (!shouldValidate) return;
        
            // First validate the current line
            if (currentLine) {
                const validationResult = isValidCommandLine(currentLine);
                if (!validationResult.valid) {
                    const errorMsg = `Error in line ${currentLineNumber}: ${validationResult.message}`;
                    validationStatus.className = 'validation-error validation-text';
                    validationStatus.textContent = errorMsg;
                    validationStatus.title = errorMsg;
                    return;
                }
            }
        
            // If the current line is valid, check for errors in any line of the payload
            for (let i = 0; i < lines.length; i++) {
                const line = lines[i].trim();
                if (!line) continue; // Skip empty lines
        
                const validationResult = isValidCommandLine(line);
                if (!validationResult.valid) {
                    const errorMsg = `Error in line ${i + 1}: ${validationResult.message}`;
                    validationStatus.className = 'validation-error validation-text';
                    validationStatus.textContent = errorMsg;
                    validationStatus.title = errorMsg;
                    return;
                }
            }
        
            // If we get here, the entire payload is valid
            validationStatus.className = 'validation-ok validation-text';
            validationStatus.textContent = 'Valid';
            validationStatus.title = '';
        }

        function isValidCommandLine(line) {
            // Skip comment lines (starting with ##)
            if (line.startsWith('##')) {
                return { valid: true };
            }

            // Special handling for "# Layout:" command
            if (line.startsWith('# Layout:')) {
                const args = line.substring('# Layout:'.length).trim();
                return isValidCommand('# Layout:', args, 0);
            }
            
            // Normal command handling
            const parts = line.split(/\s+/);
            const command = parts[0];
            const args = parts.slice(1).join(' ');

            // Get the line number (for error reporting)
            const textarea = document.getElementById('livePayloadInput');
            const lines = textarea.value.split('\n');
            const currentLineNumber = textarea.value.substr(0, textarea.selectionStart).split('\n').length;
            
            // Pass command, args, and line number to the validator
            return isValidCommand(command, args, currentLineNumber);
        }

        function handleAutoSuggestion() {
            const textarea = document.getElementById('livePayloadInput');
            const cursorPos = textarea.selectionStart;
            const textBeforeCursor = textarea.value.substring(0, cursorPos);
            const currentLine = textBeforeCursor.split('\n').pop().trim();

            // Clear any previous suggestion
            currentSuggestion = null;
            
            // Find matching suggestion without showing visual hint
            for (const [prefix, suggestion] of Object.entries(commandSuggestions)) {
                if (currentLine === prefix) {
                    currentSuggestion = suggestion;
                    return;
                }
            }
        }
        
        function handleTabCompletion(e) {
            if (e.key === 'Tab') {
                // Always prevent default tab behavior in the textarea
                e.preventDefault();
                
                // Only apply autocompletion if we have a suggestion
                if (currentSuggestion) {
                    const textarea = document.getElementById('livePayloadInput');
                    const cursorPos = textarea.selectionStart;
                    const textBeforeCursor = textarea.value.substring(0, cursorPos);
                    const textAfterCursor = textarea.value.substring(cursorPos);
            
                    // Insert the suggestion
                    textarea.value = textBeforeCursor + currentSuggestion + textAfterCursor;
            
                    // Move cursor to end of completed command
                    const newCursorPos = cursorPos + currentSuggestion.length;
                    textarea.setSelectionRange(newCursorPos, newCursorPos);
            
                    // Clean up
                    currentSuggestion = null;
            
                    // Trigger input event
                    const event = new Event('input');
                    textarea.dispatchEvent(event);
                }
                // If no suggestion exists, the cursor simply stays where it is
            }
        }


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
            const validationStatus = document.getElementById('validationStatus');
            const payloadContent = document.getElementById('livePayloadInput').value;

            // Check for empty payload
            if (payloadContent.trim() === '') {
                showMessage('error', 'Payload content cannot be empty!');
                return;
            }

            // Check if there are validation errors
            if (validationStatus.classList.contains('validation-error')) {
                showMessage('error', 'Please fix validation errors before running the payload');
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
            const validationStatus = document.getElementById('validationStatus');
            const payloadContent = document.getElementById('livePayloadInput').value;

            // Check for empty payload
            if (payloadContent.trim() === '') {
                showMessage('error', 'Payload content cannot be empty!');
                return;
            }

            // Check if there are validation errors
            if (validationStatus.classList.contains('validation-error')) {
                showMessage('error', 'Please fix validation errors before saving the payload');
                return;
            }

            document.getElementById('metadataForm').style.display = 'block';
            document.getElementById('metadataForm').scrollIntoView({ behavior: 'smooth' });
        }

        function cancelSavePayload() {
            document.getElementById('metadataForm').style.display = 'none';
        }

        function confirmSavePayload() {
            const validationStatus = document.getElementById('validationStatus');

            // Check if there are validation errors
            if (validationStatus.classList.contains('validation-error')) {
                showMessage('error', 'Please fix validation errors before saving the payload');
                return;
            }

            const payloadContent = document.getElementById('livePayloadInput').value;
            const payloadName = document.getElementById('payloadName').value;
            const payloadDesc = document.getElementById('payloadDescription').value;
            const payloadOS = document.getElementById('payloadOS').value;

            // Additional check for empty payload
            if (!payloadContent) {
                showMessage('error', 'Payload content cannot be empty!');
                return;
            }

            const formData = new FormData();
            formData.append('livepayload', payloadContent);
            formData.append('payloadName', payloadName || 'Unnamed Payload');
            formData.append('payloadDescription', payloadDesc || 'No description provided');
            formData.append('payloadOS', payloadOS || 'unknown');

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
                document.getElementById('payloadOS').value = '';
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

        // Initialize when page loads
        document.addEventListener('DOMContentLoaded', function() {
            updateLineNumbers();

            setTimeout(() => {
                updateLineNumbers();
            }, 100);

            validatePayload();

            // Setup text area event handlers
            const payloadInput = document.getElementById('livePayloadInput');

            // Input event for content changes, validation, and line numbers
            payloadInput.addEventListener('input', function(e) {
                updateLineNumbers();
                validatePayload();
                validateCurrentLine(e);
                handleAutoSuggestion();
            });

            // Key events for tab completion
            payloadInput.addEventListener('keydown', handleTabCompletion);

            // Scroll event for line number sync - improved
            payloadInput.addEventListener('scroll', function() {
                const lineNumbers = document.getElementById('lineNumbers');
                lineNumbers.scrollTop = this.scrollTop;
            });
            // Clear current suggestion on blur
            payloadInput.addEventListener('blur', function() {
                currentSuggestion = null;
            });

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
        });

        if (window.location.search.includes('edit=true')) {
            const editPayloadContent = localStorage.getItem('editPayloadContent');
            const editPayloadPath = localStorage.getItem('editPayloadPath')
            if (editPayloadContent && editPayloadPath) {
                document.getElementById('livePayloadInput').value = editPayloadContent;
                updateLineNumbers();
                validatePayload()
                // Modify the save button to update existing payload
                document.querySelector('button[onclick="showSavePayloadForm()"]').textContent = 'Save Changes';
                document.querySelector('button[onclick="showSavePayloadForm()"]').onclick = function() {
                    updateExistingPayload(editPayloadPath);
                }
                // Clean up localStorage
                localStorage.removeItem('editPayloadContent');
                localStorage.removeItem('editPayloadPath');
            }
        }
        
        function updateExistingPayload(filePath) {
            const validationStatus = document.getElementById('validationStatus');
            const payloadContent = document.getElementById('livePayloadInput').value;

            // Check for empty payload
            if (payloadContent.trim() === '') {
                showMessage('error', 'Payload content cannot be empty!');
                return;
            }

            // Check if there are validation errors
            if (validationStatus.classList.contains('validation-error')) {
                showMessage('error', 'Please fix validation errors before saving changes');
                return;
            }

            fetch('/updatepayload', {
                method: 'POST',
                headers: { 'Content-Type': 'application/json' },
                body: JSON.stringify({ 
                    path: filePath, 
                    content: payloadContent 
                })
            })
            .then(handleUpdateResponse)
            .catch(handleUpdateError);
        }

        function handleUpdateResponse(response) {
            if (!response.ok) {
                return response.text().then(text => { throw new Error(text) });
            }
            return response.json().then(data => {
                if (data.success) {
                    showMessage('success', 'Payload updated successfully!');
                } else {
                    showMessage('error', data.message || 'Update failed');
                }
            });
        }

        function handleUpdateError(error) {
            console.error('Update error:', error);
            showMessage('error', `Update failed: ${error.message}`);
        }
    </script>
</body>
</html>
)=====";