const char AutoExecPlanning[] PROGMEM = R"=====(
<!DOCTYPE HTML>
<html>
<head>
    <title>EvilCrowCable-Wind - AutoExec Planning</title>
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

    <div class="cable-wind-logo">AUTOEXEC PLANNING</div>

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
            <div class="os-box" id="noDetectionBox" data-os="nodetection">
                <h3>No OS Detection</h3>
                <div id="noDetectionPayload"></div>
            </div>
        </div>

        <h4>Available Payloads:</h4>
        <div id="payloadList" class="payload-list-container">
            <!-- Payloads will be loaded here -->
        </div>

        <div class="button-container-os">
            <button type="button" onclick="saveAutoExecPlan()" style="background: linear-gradient(135deg, var(--success), #00cc66);">Save & Enable AutoExec</button>
            <button type="button" onclick="clearAutoExecPlan()" style="background: linear-gradient(135deg, var(--error), #cc0022);">Clear All</button>
        </div>
    </div>

    <script>
        let draggedPayload = null;
        let autoExecEnabled = false;
        let activeDropdown = null;
        let selectedPayload = null;

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
                        payloadItem.dataset.path = payload.path;

                        payloadItem.innerHTML = `
                            <div class="payload-header">
                                <strong>${payload.name}</strong>
                                <button class="select-os-btn" data-path="${payload.path}">Select OS</button>
                            </div>
                            <div class='payload-desc'>${payload.description}</div>
                            <div class='payload-filename'>${payload.filename}</div>
                        `;

                        // Add click handler for OS selection
                        payloadItem.querySelector('.select-os-btn').addEventListener('click', function(e) {
                            e.stopPropagation();
                            selectedPayload = payload;
                            showOSModal();
                        });

                        payloadList.appendChild(payloadItem);
                    });
                });
        }

        function showOSModal() {
            const modal = document.createElement('div');
            modal.className = 'os-modal';
            modal.innerHTML = `
                <div class="os-modal-content">
                    <h3>Select Target OS for: ${selectedPayload.name}</h3>
                    <div class="os-options">
                        <button data-os="windows">Windows</button>
                        <button data-os="linux">Linux</button>
                        <button data-os="ios">iOS</button>
                        <button data-os="macos">macOS</button>
                        <button data-os="android">Android</button>
                        <button data-os="nodetection">No OS Detection</button>
                    </div>
                    <button class="close-modal">Cancel</button>
                </div>
            `;

            document.body.appendChild(modal);

            // Add click handlers for OS buttons
            modal.querySelectorAll('.os-options button').forEach(btn => {
                btn.addEventListener('click', function(e) {
                    e.stopPropagation();
                    assignPayloadToOS(selectedPayload, this.dataset.os);
                    modal.remove();
                });
            });

            // Add click handler for close button
            modal.querySelector('.close-modal').addEventListener('click', function(e) {
                e.stopPropagation();
                modal.remove();
            });

            // Close modal when clicking outside
            modal.addEventListener('click', function(e) {
                if (e.target === modal) {
                    modal.remove();
                }
            });
        }

        function assignPayloadToOS(payload, os) {
            const osIdMap = {
                windows: 'windowsPayload',
                linux: 'linuxPayload',
                ios: 'iosPayload',
                macos: 'macosPayload',
                android: 'androidPayload',
                nodetection: 'noDetectionPayload'
            };
        
            const payloadDivId = osIdMap[os];
            const payloadDiv = document.getElementById(payloadDivId);
        
            if (!payloadDiv) {
                console.error(`No element found for OS: ${os}`);
                return;
            }
        
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
                        if (plan.nodetection) {
                            document.getElementById('noDetectionPayload').innerHTML = createPayloadPreview(plan.nodetection);
                            document.getElementById('noDetectionPayload').dataset.path = plan.nodetection.path;
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

        function saveAutoExecPlan() {
            const plan = {
                enabled: true,
                windows: getPayloadForOS('windows'),
                linux: getPayloadForOS('linux'),
                ios: getPayloadForOS('ios'),
                macos: getPayloadForOS('macos'),
                android: getPayloadForOS('android'),
                nodetection: getPayloadForOS('nodetection')
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
                    showMessage('success', 'Auto execution plan saved! Will run on next USB insertion.');
                    autoExecEnabled = true;
                } else {
                    showMessage('error', 'Failed to save auto execution plan.');
                }
            });
        }

        function getPayloadForOS(os) {
            const osIdMap = {
                windows: 'windowsPayload',
                linux: 'linuxPayload',
                ios: 'iosPayload',
                macos: 'macosPayload',
                android: 'androidPayload',
                nodetection: 'noDetectionPayload'
            };

            const payloadDivId = osIdMap[os];
            const payloadDiv = document.getElementById(payloadDivId);

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
    </script>
</body>
</html>
)=====";