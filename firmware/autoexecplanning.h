const char AutoExecPlanning[] PROGMEM = R"=====(
<!DOCTYPE HTML>
<html>
<head>
    <title>Auto Execution Planning</title>
    <meta charset="utf-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0, maximum-scale=1.0, user-scalable=no">
    <meta name="apple-mobile-web-app-capable" content="yes">
    <meta name="apple-mobile-web-app-status-bar-style" content="black">
    <link rel="stylesheet" href="style.css">
    <script src="javascript.js"></script>
    <style>
        .pyaloadButton {
            display: block;
            width: 100%;
            box-sizing: border-box;
        }
    </style>
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

    <div class="view-container">
        <h3>Auto Execution Planning</h3>       
        <div class="os-container">
            <div class="os-box" id="windowsBox" data-os="windows">
                <h3>Windows</h3>
                <div id="windowsPayload"></div>
            </div>
            <div class="os-box" id="linuxBox" data-os="linux">
                <h3>Linux</h3>
                <div id="linuxPayload"></div>
            </div>
            <div class="os-box" id="iosBox" data-os="ios">
                <h3>iOS</h3>
                <div id="iosPayload"></div>
            </div>
            <div class="os-box" id="macosBox" data-os="macos">
                <h3>macOS</h3>
                <div id="macosPayload"></div>
            </div>
            <div class="os-box" id="androidBox" data-os="android">
                <h3>Android</h3>
                <div id="androidPayload"></div>
            </div>
        </div>

        <h4>Available Payloads:</h4>
        <div id="payloadList" class="payload-list-container">
            <!-- Payloads will be loaded here -->
        </div>

        <div class="button-container-os">
            <button type="button" onclick="saveAutoExecPlan()" style="background-color: #34C759;">Save & Enable AutoExec</button>
            <button type="button" onclick="clearAutoExecPlan()" style="background-color: #FF3B30;">Clear All</button>
        </div>
    </div>

    <script>
        let draggedPayload = null;
        let autoExecEnabled = false;
        let activeDropdown = null;

        // Close dropdown when clicking outside
        document.addEventListener('click', function(event) {
            if (activeDropdown && !event.target.closest('.os-dropdown') && !event.target.closest('.payload-item-os')) {
                activeDropdown.classList.remove('show-dropdown');
                activeDropdown = null;
            }
        });

        // Load payloads and current autoexec plan when page loads
        document.addEventListener('DOMContentLoaded', function() {
            loadPayloads();
            loadAutoExecPlan();
        });

        function loadPayloads() {
            fetch('/listpayloadsdata')
                .then(response => response.json())
                .then(payloads => {
                    const payloadList = document.getElementById('payloadList');
                    payloadList.innerHTML = '';
                    
                    payloads.forEach(payload => {
                        const payloadItem = document.createElement('div');
                        payloadItem.className = 'payload-item-os';
                        payloadItem.draggable = true;
                        payloadItem.dataset.path = payload.path;
                        
                        // Create dropdown container
                        const dropdown = document.createElement('div');
                        dropdown.className = 'os-dropdown';
                        
                        // Create dropdown content
                        const dropdownContent = document.createElement('div');
                        dropdownContent.className = 'os-dropdown-content';
                        dropdownContent.innerHTML = `
                            <a data-os="windows">Windows</a>
                            <a data-os="linux">Linux</a>
                            <a data-os="ios">iOS</a>
                            <a data-os="macos">macOS</a>
                            <a data-os="android">Android</a>
                        `;
                        
                        // Add click handlers to dropdown items
                        dropdownContent.querySelectorAll('a').forEach(item => {
                            item.addEventListener('click', function() {
                                assignPayloadToOS(payload, this.dataset.os);
                                dropdownContent.classList.remove('show-dropdown');
                                activeDropdown = null;
                            });
                        });
                        
                        payloadItem.innerHTML = `
                            <a class="pyaloadButton">${payload.name}</a>
                            <div class='payload-desc'>${payload.description}</div>
                            <div class='payload-filename'>${payload.filename}</div>
                        `;
                        
                        // Click handler to show dropdown
                        const payloadButton = payloadItem.querySelector('.pyaloadButton');
                        payloadButton.addEventListener('click', function(e) {
                            if (draggedPayload) return;

                            // Close any open dropdown
                            if (activeDropdown) {
                                activeDropdown.classList.remove('show-dropdown');
                            }

                            // Get position and dimensions
                            const buttonRect = this.getBoundingClientRect();
                            const dropdownHeight = 200; // Approximate height of dropdown

                            // Position dropdown - show above if near bottom of viewport
                            if (buttonRect.bottom + dropdownHeight > window.innerHeight) {
                                dropdownContent.style.top = 'auto';
                                dropdownContent.style.bottom = '100%';
                            } else {
                                dropdownContent.style.top = '100%';
                                dropdownContent.style.bottom = 'auto';
                            }

                            dropdownContent.style.left = '0';
                            dropdownContent.classList.add('show-dropdown');
                            activeDropdown = dropdownContent;

                            e.stopPropagation();
                        });
                        
                        // Drag and drop functionality
                        payloadItem.addEventListener('dragstart', function(e) {
                            draggedPayload = this;
                            this.classList.add('dragging');
                            e.dataTransfer.setData('text/plain', this.dataset.path);
                        });
                        
                        payloadItem.addEventListener('dragend', function() {
                            this.classList.remove('dragging');
                        });
                        
                        dropdown.appendChild(dropdownContent);
                        payloadItem.appendChild(dropdown);
                        payloadList.appendChild(payloadItem);
                    });
                });
        }

        function assignPayloadToOS(payload, os) {
            const payloadDiv = document.getElementById(`${os}Payload`);
            
            payloadDiv.innerHTML = createPayloadPreview({
                path: payload.path,
                name: payload.name,
                description: payload.description,
                filename: payload.filename
            });
            
            payloadDiv.dataset.path = payload.path;
        }

        function loadAutoExecPlan() {
            fetch('/getautoexecplan')
                .then(response => response.json())
                .then(plan => {
                    if (plan) {
                        autoExecEnabled = plan.enabled || false;
                        
                        if (plan.windows) {
                            document.getElementById('windowsPayload').innerHTML = createPayloadPreview(plan.windows);
                            document.getElementById('windowsPayload').dataset.path = plan.windows.path;
                        }
                        if (plan.linux) {
                            document.getElementById('linuxPayload').innerHTML = createPayloadPreview(plan.linux);
                            document.getElementById('linuxPayload').dataset.path = plan.linux.path;
                        }
                        if (plan.ios) {
                            document.getElementById('iosPayload').innerHTML = createPayloadPreview(plan.ios);
                            document.getElementById('iosPayload').dataset.path = plan.ios.path;
                        }
                        if (plan.android) {
                            document.getElementById('androidPayload').innerHTML = createPayloadPreview(plan.android);
                            document.getElementById('androidPayload').dataset.path = plan.android.path;
                        }
                        if (plan.macos) {
                            document.getElementById('macosPayload').innerHTML = createPayloadPreview(plan.macos);
                            document.getElementById('macosPayload').dataset.path = plan.macos.path;
                        }
                    }
                });
        }

        function createPayloadPreview(payload) {
            return `
                <div class="payload-preview">
                    <strong>${payload.name}</strong>
                    <div class="payload-desc">${payload.description}</div>
                    <div class="payload-filename">${payload.filename}</div>
                </div>
            `;
        }

        // Setup drop targets
        document.querySelectorAll('.os-box').forEach(box => {
            box.addEventListener('dragover', function(e) {
                e.preventDefault();
                this.classList.add('highlight');
            });
            
            box.addEventListener('dragleave', function() {
                this.classList.remove('highlight');
            });
            
            box.addEventListener('drop', function(e) {
                e.preventDefault();
                this.classList.remove('highlight');
                
                if (draggedPayload) {
                    const payloadPath = draggedPayload.dataset.path;
                    const payloadName = draggedPayload.querySelector('a').textContent;
                    const payloadDesc = draggedPayload.querySelector('.payload-desc').textContent;
                    const payloadFilename = draggedPayload.querySelector('.payload-filename').textContent;
                    
                    const os = this.dataset.os;
                    const payloadDiv = document.getElementById(`${os}Payload`);
                    
                    payloadDiv.innerHTML = createPayloadPreview({
                        path: payloadPath,
                        name: payloadName,
                        description: payloadDesc,
                        filename: payloadFilename
                    });
                    
                    payloadDiv.dataset.path = payloadPath;
                }
            });
        });

        function saveAutoExecPlan() {
            const plan = {
                enabled: true,
                windows: getPayloadForOS('windows'),
                linux: getPayloadForOS('linux'),
                ios: getPayloadForOS('ios'),
                macos: getPayloadForOS('macos'),
                android: getPayloadForOS('android')
            };
            
            fetch('/saveautoexecplan', {
                method: 'POST',
                headers: {
                    'Content-Type': 'application/json',
                },
                body: JSON.stringify(plan)
            })
            .then(response => response.json())
            .then(data => {
                if (data.success) {
                    showMessage('success', 'Auto execution plan saved and enabled!');
                    autoExecEnabled = true;
                } else {
                    showMessage('error', 'Failed to save auto execution plan.');
                }
            });
        }

        function getPayloadForOS(os) {
            const payloadDiv = document.getElementById(`${os}Payload`);
            if (payloadDiv && payloadDiv.dataset.path) {
                const name = payloadDiv.querySelector('strong')?.textContent || '';
                const description = payloadDiv.querySelector('.payload-desc')?.textContent || '';
                const filename = payloadDiv.querySelector('.payload-filename')?.textContent || '';
                
                return {
                    path: payloadDiv.dataset.path,
                    name: name,
                    description: description,
                    filename: filename
                };
            }
            return null;
        }

        function clearAutoExecPlan() {
            document.querySelectorAll('[id$="Payload"]').forEach(el => {
                el.innerHTML = '';
                el.dataset.path = '';
            });
            
            fetch('/clearautoexecplan', {
                method: 'POST'
            })
            .then(response => response.json())
            .then(data => {
                if (data.success) {
                    showMessage('success', 'Auto execution plan cleared!');
                    autoExecEnabled = false;
                }
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
    </script>
</body>
</html>
)=====";