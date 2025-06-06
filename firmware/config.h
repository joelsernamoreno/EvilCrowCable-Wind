const char Configuration[] PROGMEM = R"=====(
<!DOCTYPE HTML>
<html>
<head>
    <title>EvilCrowCable-Wind - Configuration</title>
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
            <li><a href='/'><span class="usb-icon"></span>Home</a></li>
            <li><a href='/livepayload'>Live Payload</a></li>
            <li><a href='/uploadpayload'>Upload Payload</a></li>
            <li><a href='/listpayloads'>List Payloads</a></li>
            <li><a href='/autoexecplanning'>AutoExec Planning</a></li>
            <li><a href='/config'>Config</a></li>
        </ul>
    </nav>

    <div class="cable-wind-logo">CONFIGURATION</div>

    <div class="view-container">
        <form id="layoutForm">
            <div class="form-group">
                <label for="layout">Keyboard Layout:</label>
                <select id="layout" name="layout-select">
                    <option value="EN_US">EN_US</option>
                    <option value="ES_ES">ES_ES</option>
                    <option value="FR_FR">FR_FR</option>
                    <option value="IT_IT">IT_IT</option>
                    <option value="DA_DK">DA_DK</option>
                    <option value="DE_DE">DE_DE</option>
                    <option value="HR_HR">HR_HR</option>
                    <option value="HU_HU">HU_HU</option>
                    <option value="PT_PT">PT_PT</option>
                    <option value="PT_BR">PT_BR</option>
                    <option value="BE_BE">BE_BE</option>
                    <option value="BR_BR">BR_BR</option>
                    <option value="CA_CA">CA_CA</option>
                    <option value="CA_FR">CA_FR</option>
                    <option value="SK_SK">SK_SK</option>
                    <option value="CZ_CZ">CZ_CZ</option>
                    <option value="SV_SE">SV_SE</option>
                    <option value="SI_SI">SI_SI</option>
                </select>
            </div>
            <button type="button" onclick="applyLayout()">Apply Layout</button>
        </form>

        <hr>

        <form id="wifiForm">
            <div class="form-group">
                <label for="ssid">Wi-Fi SSID:</label>
                <input type="text" id="ssid" name="ssid" required class="terminal-style single-line-input">
            </div>
            <div class="form-group">
                <label for="password">Wi-Fi Password:</label>
                <input type="password" id="password" name="password" required class="terminal-style single-line-input">
            </div>
            <button type="button" onclick="applyWiFi()">Apply Wi-Fi</button>
            <button type="button" name="deleteWifiButton" onclick="deleteWiFiConfig()">Delete Wi-Fi Config</button>

            <hr>

            <div class="form-group">
                <label for="backup_ssid">Backup Wi-Fi SSID:</label>
                <input type="text" id="backup_ssid" name="backup_ssid" class="terminal-style single-line-input">
            </div>
            <div class="form-group">
                <label for="backup_password">Backup Wi-Fi Password:</label>
                <input type="password" id="backup_password" name="backup_password" class="terminal-style single-line-input">
            </div>
            <button type="button" onclick="applyBackupWiFi()">Apply Backup Wi-Fi</button>
            <button type="button" name="deleteBackupWifiButton" onclick="deleteBackupWiFiConfig()">Delete Backup Wi-Fi</button>

        </form>

        <hr>

        <form id="usbForm">
            <div class="form-group">
                <label for="vendorID">Vendor ID:</label>
                <input type="text" id="vendorID" name="vendorID" required class="terminal-style">
            </div>
            <div class="form-group">
                <label for="productID">Product ID:</label>
                <input type="text" id="productID" name="productID" required class="terminal-style">
            </div>
            <div class="form-group">
                <label for="productName">Product Name:</label>
                <input type="text" id="productName" name="productName" required class="terminal-style">
            </div>
            <div class="form-group">
                <label for="manufacturerName">Manufacturer Name:</label>
                <input type="text" id="manufacturerName" name="manufacturerName" required class="terminal-style">
            </div>
            <button type="button" onclick="applyUSB()">Apply USB Config</button>
            <button type="button" name="deleteUSBButton" onclick="deleteUSBConfig()">Delete USB Config</button>
        </form>

        <hr>

        <form id="hostnameForm">
            <div class="form-group">
                <label for="hostname">Device Hostname:</label>
                <input type="text" id="hostname" name="hostname" placeholder="cable-wind" required class="terminal-style">
            </div>
            <button type="button" onclick="applyHostname()">Apply Hostname</button>
        </form>
        <hr>
        <button type="button" name="clearCacheButton" onclick="clearCache()">Clear Device Cache</button>
        <p class="payload-desc">Forces reload of CSS/JS files if they were cached.</p>
    </div>

    <script>
        function applyLayout() {
            const layoutSelect = document.getElementById('layout');
            const selectedLayout = layoutSelect.value;
            const formData = new URLSearchParams();
            formData.append('layout', selectedLayout);

            fetch('/layout', {
                method: 'POST',
                headers: {
                    'Content-Type': 'application/x-www-form-urlencoded',
                },
                body: formData.toString(),
            })
            .then(response => {
                if (response.ok) {
                    return response.text();
                } else {
                    throw new Error(`Error: ${response.statusText}`);
                }
            })
            .then(data => {
                showMessage('success', 'Layout successfully applied!');
            })
            .catch(error => {
                showMessage('error', 'Error applying layout.');
                console.error('Error:', error);
            });
        }

        function applyWiFi() {
            const ssid = document.getElementById('ssid').value;
            const password = document.getElementById('password').value;

            if (!ssid || !password) {
                showMessage('error', 'SSID and password are required.');
                return;
            }

            fetch('/updatewifi', {
                method: 'POST',
                headers: {
                    'Content-Type': 'application/x-www-form-urlencoded',
                },
                body: new URLSearchParams({ ssid, password }),
            })
            .then(response => response.json())
            .then(data => {
                if (data.status === 'success') {
                    showMessage('success', data.message);
                } else {
                    showMessage('error', data.message);
                }
            })
            .catch(error => {
                showMessage('error', 'Error applying Wi-Fi settings.');
                console.error('Error:', error);
            });
        }

        function applyBackupWiFi() {
            const ssid = document.getElementById('backup_ssid').value;
            const password = document.getElementById('backup_password').value;
            if (!ssid || !password) {
                showMessage('error', 'Backup SSID and password are required.');
                return;
            }
            fetch('/updatebackupwifi', {
                method: 'POST',
                headers: { 'Content-Type': 'application/x-www-form-urlencoded' },
                body: new URLSearchParams({ ssid, password }),
            })
            .then(response => response.json())
            .then(data => {
                if (data.status === 'success') {
                    showMessage('success', data.message);
                } else {
                    showMessage('error', data.message);
                }
            })
            .catch(error => {
                showMessage('error', 'Error applying backup Wi-Fi settings.');
                console.error('Error:', error);
            });
        }

        function deleteWiFiConfig() {
            fetch('/deletewificonfig', {
                method: 'POST',
            })
            .then(response => response.text())
            .then(data => {
                showMessage('success', 'Wi-Fi Configuration successfully deleted!');
            })
            .catch(error => {
                showMessage('error', 'Error deleting Wi-Fi Configuration.');
                console.error('Error:', error);
            });
        }

        function deleteBackupWiFiConfig() {
            fetch('/deletebackupwificonfig', {
                method: 'POST',
            })
            .then(response => response.text())
            .then(data => {
                showMessage('success', 'Backup Wi-Fi Configuration successfully deleted!');
            })
            .catch(error => {
                showMessage('error', 'Error deleting Backup Wi-Fi Configuration.');
                console.error('Error:', error);
            });
        }


        function applyUSB() {
            const vendorID = document.getElementById('vendorID').value;
            const productID = document.getElementById('productID').value;
            const productName = document.getElementById('productName').value;
            const manufacturerName = document.getElementById('manufacturerName').value;

            if (!vendorID || !productID || !productName || !manufacturerName) {
                showMessage('error', 'All USB fields are required.');
                return;
            }

            showMessage('success', 'USB settings successfully applied!');

            const form = document.getElementById('usbForm');
            const formData = new FormData(form);
            fetch('/updateusb', {
                method: 'POST',
                body: formData,
            })
            .then(response => response.text())
        }

        function deleteUSBConfig() {
            showMessage('success', 'USB Configuration successfully deleted!');

            fetch('/deleteusbconfig', {
                method: 'POST',
            })
            .then(response => response.text())
        }

        function applyHostname() {
            const hostname = document.getElementById('hostname').value;

            if (!hostname) {
                showMessage('error', 'Hostname cannot be empty');
                return;
            }

            fetch('/updatehostname', {
                method: 'POST',
                headers: {
                    'Content-Type': 'application/x-www-form-urlencoded',
                },
                body: new URLSearchParams({ hostname }),
            })
            .then(response => response.json())
            .then(data => {
                if (data.success) {
                    showMessage('success', 'Hostname updated successfully! Device will restart.');
                    setTimeout(() => {
                        window.location.href = `http://${hostname}.local`;
                    }, 3000);
                } else {
                    showMessage('error', data.message || 'Error updating hostname');
                }
            })
            .catch(error => {
                showMessage('error', 'Error updating hostname');
                console.error('Error:', error);
            });
        }

        function clearCache() {
          // Force reload CSS/JS with cache-busting query strings
          const links = document.querySelectorAll('link[rel="stylesheet"], script[src]');
          links.forEach(link => {
            const url = new URL(link.href || link.src);
            url.searchParams.set('nocache', Date.now());
            link.href = url.toString();
          });

          // Show confirmation
          showMessage('success', 'Cache cleared! Refreshing page...');

          // Reload after a delay
          setTimeout(() => location.reload(true), 1000);
        }

        // Function to load the current layout when the config page is loaded
        document.addEventListener('DOMContentLoaded', function() {
            // Fetch the current layout from the server
            fetch('/getcurrentlayout')
            .then(response => response.text())
            .then(currentLayout => {
                // Set the select dropdown to the current layout
                const layoutSelect = document.getElementById('layout');
                if (layoutSelect && currentLayout) {
                    layoutSelect.value = currentLayout;
                }
            })
            .catch(error => {
                console.error('Error fetching current layout:', error);
            });
            // Fetch current hostname
            fetch('/gethostname')
                .then(response => response.text())
                .then(currentHostname => {
                    if (currentHostname) {
                        document.getElementById('hostname').value = currentHostname;
                    }
                });
        });

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