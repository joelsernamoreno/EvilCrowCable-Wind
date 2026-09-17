const char Configuration[] PROGMEM = R"=====(
<!DOCTYPE HTML>
<html>
<head>
    <title>EvilCrowCable-Wind - Configuration</title>
    <meta charset="utf-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0, maximum-scale=1.0, user-scalable=no">
    <meta name="apple-mobile-web-app-capable" content="yes">
    <meta name="apple-mobile-web-app-status-bar-style" content="black">
    <link rel="stylesheet" href="style.css?v=usb-implant-1">
    <script src="javascript.js?v=usb-implant-1"></script>
</head>
<body>
    <div id="global-toast" class="toast-container"></div>
    <nav id='menu'>
        <input type='checkbox' id='responsive-menu'><label for='responsive-menu'></label>
        <ul>
            <li><a href='/'><span class="usb-icon"></span>Home</a></li>
            <li><a href='/livepayload'>Payload Editor</a></li>
            <li><a href='/uploadpayload'>Upload Payload</a></li>
            <li><a href='/listpayloads'>List Payloads</a></li>
            <li><a href='/autoexecplanning'>AutoExec Planning</a></li>
            <li><a href='/config' class="active">Config</a></li>
        </ul>
    </nav>

    <div class="cable-wind-logo">CONFIGURATION</div>

    <div class="view-container">
        <form id="layoutForm">
            <div class="form-group">
                <div class="section-header">Keyboard Layout</div>
                <select id="layout" class="styled-select" name="layout-select">
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

        <form id="wifiForm">
            <div class="section-header">WIFI</div>
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

        <form id="vpnForm">
            <div class="section-header">P2P VPN</div>
            <div class="form-group">
                <label for="joincode">Join Code:</label>
                <input type="text" id="joincode" name="joincode" required class="terminal-style single-line-input">
            </div>
            <button type="button" onclick="applyVPN()">Apply VPN</button>
            <button type="button" name="deleteVPNButton" onclick="deleteVPNConfig()">Delete VPN Config</button>
        </form>

        <form id="usbForm">
            <div class="section-header">USB</div>
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

        <section class="implant-card" id="usbNetworkImplantSection">
            <div class="section-header">USB Network Implant</div>
            <label class="implant-toggle-row" id="usbNetworkImplantRow" for="usbNetworkImplantToggle">
                <span class="implant-switch">
                    <input type="checkbox" id="usbNetworkImplantToggle" aria-describedby="usbNetworkImplantStatus" disabled>
                    <span class="implant-slider"></span>
                </span>
                <span>
                    <strong>USB Network Implant</strong>
                    <small>Keep the keyboard and add a routed USB CDC-NCM adapter</small>
                </span>
            </label>
            <p class="implant-status" id="usbNetworkImplantStatus">Loading status...</p>
            <div class="implant-details" id="usbNetworkImplantDetails" hidden>
                <p class="payload-desc">USB target: <strong>192.168.7.2</strong>. Evil Crow Cable Wind: <strong>192.168.7.1</strong>.</p>
                <p class="payload-desc">Run this on the Linux computer connected to the same Wi-Fi access point:</p>
                <code class="implant-command" id="usbNetworkImplantRoute">Waiting for the Wi-Fi address...</code>
            </div>
        </section>

        <section class="implant-card" id="usbSerialSection">
            <div class="section-header">USB Serial</div>
            <label class="implant-toggle-row" id="usbSerialRow" for="usbSerialToggle">
                <span class="implant-switch">
                    <input type="checkbox" id="usbSerialToggle" aria-describedby="usbSerialStatus" disabled>
                    <span class="implant-slider"></span>
                </span>
                <span>
                    <strong>USB Serial (CDC)</strong>
                    <small>Adds a USB serial console and the serial &lt;-&gt; TCP passthrough</small>
                </span>
            </label>
            <p class="implant-status" id="usbSerialStatus">Loading status...</p>
        </section>

        <section class="implant-card" id="sapsanSection">
            <div class="section-header">AI assistant</div>
            <p class="implant-status" id="sapsanStatus">Loading status...</p>

            <div id="sapsanLoginBox">
                <div class="form-group">
                    <label for="sapsanEmail">Email:</label>
                    <input type="email" id="sapsanEmail" name="sapsanEmail" placeholder="you@example.com" class="terminal-style" autocomplete="username">
                </div>
                <div class="form-group">
                    <label for="sapsanPassword">Password:</label>
                    <input type="password" id="sapsanPassword" name="sapsanPassword" placeholder="Account password" class="terminal-style" autocomplete="current-password">
                </div>
                <button type="button" id="sapsanLoginButton" onclick="sapsanLogin()">Log in</button>
                <p class="payload-desc">Only the session token is stored on the cable, never the password. Once logged in, use "Ask AI for help" in the Payload Editor.</p>
            </div>

            <div id="sapsanLoggedInBox" hidden>
                <button type="button" onclick="sapsanLogout()" style="background: linear-gradient(135deg, var(--error), #cc0022);">Log out</button>
            </div>
        </section>

        <form id="hostnameForm">
            <div class="form-group">
                <div class="section-header">Hostname</div>
                <input type="text" id="hostname" name="hostname" placeholder="cable-wind" required class="terminal-style">
            </div>
            <button type="button" onclick="applyHostname()">Apply Hostname</button>
        </form>

        <hr>
        <div class="config-buttons-container">
            <button type="button" name="clearCacheButton" onclick="clearCache()">Clear Device Cache</button>
            <button type="button" name="rebootDeviceButton" onclick="rebootDevice()">Reboot Device</button>
        </div>
        <p class="payload-desc">Reload of CSS/JS files.</p>
    </div>

    <script>
        let usbNetworkImplantUpdating = false;

        async function refreshUsbNetworkImplantStatus() {
            try {
                const response = await fetch('/usb-network-implant/status', { cache: 'no-store' });
                if (!response.ok) throw new Error('HTTP ' + response.status);
                const data = await response.json();
                const toggle = document.getElementById('usbNetworkImplantToggle');
                const row = document.getElementById('usbNetworkImplantRow');
                const status = document.getElementById('usbNetworkImplantStatus');
                const details = document.getElementById('usbNetworkImplantDetails');

                toggle.checked = data.enabled;
                // A stale enabled setting must remain switchable off even if a
                // different Arduino core no longer provides the NCM driver, and
                // it is never switchable ON while USB Serial holds the endpoints.
                const implantBlocked = data.usbSerialEnabled && !data.enabled;
                toggle.disabled = usbNetworkImplantUpdating ||
                                  (!data.ncmSupported && !data.enabled) || implantBlocked;
                row.classList.toggle('implant-unavailable',
                                     (!data.ncmSupported && !data.enabled) || implantBlocked);
                details.hidden = !data.enabled;
                document.getElementById('usbNetworkImplantRoute').textContent = data.linuxRoute;

                if (!data.ncmSupported) {
                    status.textContent = 'Unavailable: this Arduino-ESP32 build has no CDC-NCM driver.';
                } else if (implantBlocked) {
                    status.textContent = 'Unavailable while USB Serial is enabled: the ESP32-S3 cannot enumerate HID + USB Serial + NCM together. Turn USB Serial off first.';
                } else if (!data.enabled) {
                    status.textContent = 'Disabled. USB exposes the keyboard only.';
                } else if (data.error) {
                    status.textContent = 'Enabled, but not ready: ' + data.error;
                } else if (!data.sourceNatReady) {
                    status.textContent = 'Enabled, but source NAT is not ready.';
                } else if (!data.linkUp) {
                    status.textContent = 'Enabled and routed with source NAT. Waiting for the USB host to bring the NCM link up.';
                } else {
                    status.textContent = 'Ready. Wi-Fi ' + data.wifiIp + ' reaches USB target ' + data.targetIp + ' through source NAT.';
                }
            } catch (error) {
                document.getElementById('usbNetworkImplantStatus').textContent =
                    'Status unavailable: ' + error.message;
            }
        }

        async function setUsbNetworkImplant(enabled) {
            if (usbNetworkImplantUpdating) return;
            const toggle = document.getElementById('usbNetworkImplantToggle');
            const action = enabled ? 'enable' : 'disable';
            if (!confirm('Do you want to ' + action + ' USB Network Implant? The device will restart.')) {
                toggle.checked = !enabled;
                return;
            }

            usbNetworkImplantUpdating = true;
            toggle.disabled = true;
            try {
                const response = await fetch('/usb-network-implant', {
                    method: 'POST',
                    headers: { 'Content-Type': 'application/x-www-form-urlencoded' },
                    body: new URLSearchParams({ enabled: enabled ? '1' : '0' })
                });
                const data = await response.json().catch(() => ({}));
                if (!response.ok || !data.ok) {
                    throw new Error(data.message || ('HTTP ' + response.status));
                }

                if (data.restarting) {
                    showMessage('success', 'USB Network Implant updated. The device is restarting...');
                    document.body.classList.add('page-loading');
                    setTimeout(() => window.location.reload(true), 8000);
                } else {
                    usbNetworkImplantUpdating = false;
                    await refreshUsbNetworkImplantStatus();
                }
            } catch (error) {
                usbNetworkImplantUpdating = false;
                toggle.checked = !enabled;
                toggle.disabled = false;
                showMessage('error', 'Error updating USB Network Implant: ' + error.message);
                refreshUsbNetworkImplantStatus();
            }
        }

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
            if (confirm('Are you sure you want to delete the primary WiFi configuration?')) {
                fetch('/deletewificonfig', {
                    method: 'POST',
                })
                .then(response => {
                    if (response.ok) {
                        return response.json();
                    }
                    throw new Error('Network response was not ok');
                })
                .then(data => {
                    showMessage('success', 'Primary WiFi configuration deleted successfully!');
                })
                .catch(error => {
                    showMessage('error', 'Error deleting WiFi configuration: ' + error.message);
                    console.error('Error:', error);
                });
            }
        }

        function deleteBackupWiFiConfig() {
            if (confirm('Are you sure you want to delete the backup WiFi configuration?')) {
                fetch('/deletebackupwificonfig', {
                    method: 'POST',
                })
                .then(response => {
                    if (response.ok) {
                        return response.json();
                    }
                    throw new Error('Network response was not ok');
                })
                .then(data => {
                    showMessage('success', 'Backup WiFi configuration deleted successfully!');
                })
                .catch(error => {
                    showMessage('error', 'Error deleting backup WiFi configuration: ' + error.message);
                    console.error('Error:', error);
                });
            }
        }

        function applyVPN() {
            const joincode = document.getElementById('joincode').value;
            if (!joincode) {
                showMessage('error', 'Join code is required.');
                return;
            }
            fetch('/updatevpn', {
                method: 'POST',
                headers: {
                    'Content-Type': 'application/x-www-form-urlencoded',
                },
                body: new URLSearchParams({ joincode }),
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
                showMessage('error', 'Error applying VPN settings.');
                console.error('Error:', error);
            });
        }

        function deleteVPNConfig() {
            if (confirm('Are you sure you want to delete the VPN configuration?')) {
                fetch('/deletevpnconfig', {
                    method: 'POST',
                })
                .then(response => {
                    if (response.ok) {
                        return response.json();
                    }
                    throw new Error('Network response was not ok');
                })
                .then(data => {
                    showMessage('success', 'VPN configuration deleted successfully!');
                    document.getElementById('joincode').value = '';
                })
                .catch(error => {
                    showMessage('error', 'Error deleting VPN configuration: ' + error.message);
                    console.error('Error:', error);
                });
            }
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
                    }, 6000);
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

        function rebootDevice() {
            if (confirm('Are you sure you want to reboot the device?')) {
                showMessage('info', 'Device rebooting... Please wait');
                document.body.classList.add('page-loading');
                
                fetch('/reboot', {
                    method: 'POST'
                })
                .then(response => {
                    if (response.ok) {
                        // Try to reload the page after 6 seconds
                        setTimeout(() => {
                            window.location.reload(true);
                        }, 6000);
                    } else {
                        document.body.classList.remove('page-loading');
                        throw new Error('Reboot failed');
                    }
                })
                .catch(error => {
                    document.body.classList.remove('page-loading');
                    showMessage('error', 'Error rebooting device');
                    console.error('Error:', error);
                });
            }
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
        // Function to load the current layout when the config page is loaded
        document.addEventListener('DOMContentLoaded', function() {
            const cachedLayout = localStorage.getItem('currentLayout');
            if (cachedLayout) {
                document.getElementById('layout').value = cachedLayout;
            }

            // Then fetch the actual current layout from the server
            fetch('/getcurrentlayout')
            .then(response => {
                if (!response.ok) throw new Error('Network response was not ok');
                return response.text();
            })
            .then(currentLayout => {
                if (currentLayout) {
                    const layoutSelect = document.getElementById('layout');
                    layoutSelect.value = currentLayout;
                    // Cache the value for future page loads
                    localStorage.setItem('currentLayout', currentLayout);
                }
            })
            .catch(error => {
                console.error('Error fetching current layout:', error);
                // Fall back to localStorage value if fetch fails
                if (cachedLayout) {
                    document.getElementById('layout').value = cachedLayout;
                }
            });

            // Rest of your hostname fetching code...
            fetch('/gethostname')
                .then(response => response.text())
                .then(currentHostname => {
                    if (currentHostname) {
                        document.getElementById('hostname').value = currentHostname;
                    }
                });

            const implantToggle = document.getElementById('usbNetworkImplantToggle');
            implantToggle.addEventListener('change', function() {
                setUsbNetworkImplant(implantToggle.checked);
            });
            refreshUsbNetworkImplantStatus();
            setInterval(refreshUsbNetworkImplantStatus, 5000);

            const usbSerialToggle = document.getElementById('usbSerialToggle');
            usbSerialToggle.addEventListener('change', function() {
                setUsbSerial(usbSerialToggle.checked);
            });
            refreshUsbSerialStatus();
            setInterval(refreshUsbSerialStatus, 5000);

            refreshSapsanStatus();
            const sapsanPwd = document.getElementById('sapsanPassword');
            sapsanPwd.addEventListener('keydown', function(e) {
                if (e.key === 'Enter') { e.preventDefault(); sapsanLogin(); }
            });

        });

        let usbSerialUpdating = false;

        async function refreshUsbSerialStatus() {
            const status = document.getElementById('usbSerialStatus');
            try {
                const response = await fetch('/usb-serial/status', { cache: 'no-store' });
                if (!response.ok) throw new Error('HTTP ' + response.status);
                const data = await response.json();

                const toggle = document.getElementById('usbSerialToggle');
                const row = document.getElementById('usbSerialRow');

                toggle.checked = data.enabled;
                const serialBlocked = data.implantEnabled && !data.enabled;
                toggle.disabled = usbSerialUpdating ||
                                  (!data.supported && !data.enabled) || serialBlocked;
                row.classList.toggle('implant-unavailable',
                                     (!data.supported && !data.enabled) || serialBlocked);

                if (!data.supported) {
                    status.textContent = 'Unavailable: this build has no USB CDC support.';
                } else if (serialBlocked) {
                    status.textContent = 'Unavailable while USB Network Implant is enabled: the ESP32-S3 cannot enumerate HID + USB Serial + NCM together. Turn the implant off first.';
                } else if (data.blocked) {
                    status.textContent = 'Enabled but suppressed because USB Network Implant is also on. Turn one of them off.';
                } else if (!data.enabled) {
                    status.textContent = 'Disabled. USB does not expose a serial port.';
                } else if (!data.started) {
                    status.textContent = 'Enabled, but the CDC interface did not start.';
                } else if (data.hostConnected) {
                    status.textContent = 'Ready. The USB host has the serial port open' +
                        (data.tcpConnected ? ', TCP passthrough connected.' : '.');
                } else {
                    status.textContent = 'Enabled. Waiting for the USB host to open the serial port.';
                }
            } catch (error) {
                status.textContent = 'Status unavailable: ' + error.message;
            }
        }

        async function setUsbSerial(enabled) {
            if (usbSerialUpdating) return;
            const toggle = document.getElementById('usbSerialToggle');
            const action = enabled ? 'enable' : 'disable';
            if (!confirm('Do you want to ' + action + ' USB Serial? The device will restart.')) {
                toggle.checked = !enabled;
                return;
            }

            usbSerialUpdating = true;
            toggle.disabled = true;
            try {
                const response = await fetch('/usb-serial', {
                    method: 'POST',
                    headers: { 'Content-Type': 'application/x-www-form-urlencoded' },
                    body: new URLSearchParams({ enabled: enabled ? '1' : '0' })
                });
                const data = await response.json().catch(() => ({}));
                if (!response.ok || !data.ok) {
                    throw new Error(data.message || ('HTTP ' + response.status));
                }

                if (data.restarting) {
                    showMessage('success', 'USB Serial updated. The device is restarting...');
                    document.body.classList.add('page-loading');
                    setTimeout(() => window.location.reload(true), 8000);
                } else {
                    usbSerialUpdating = false;
                    await refreshUsbSerialStatus();
                }
            } catch (error) {
                usbSerialUpdating = false;
                toggle.checked = !enabled;
                toggle.disabled = false;
                showMessage('error', 'Error updating USB Serial: ' + error.message);
                refreshUsbSerialStatus();
            }
        }

        async function refreshSapsanStatus() {
            const status = document.getElementById('sapsanStatus');
            try {
                const response = await fetch('/sapsan/status', { cache: 'no-store' });
                if (!response.ok) throw new Error('HTTP ' + response.status);
                const data = await response.json();

                document.getElementById('sapsanLoginBox').hidden = data.loggedIn;
                document.getElementById('sapsanLoggedInBox').hidden = !data.loggedIn;

                if (data.loggedIn) {
                    let txt = 'Logged in';
                    if (data.name) txt += ' as ' + data.name;
                    else if (data.email) txt += ' as ' + data.email;
                    if (data.tier) txt += ' (' + data.tier + ')';
                    status.textContent = txt + '.';
                } else if (!data.wifi) {
                    status.textContent = 'Not logged in. Connect the cable to Wi-Fi first.';
                } else {
                    status.textContent = 'Not logged in.';
                }
            } catch (error) {
                status.textContent = 'Status unavailable: ' + error.message;
            }
        }

        async function sapsanLogin() {
            const email = document.getElementById('sapsanEmail').value.trim();
            const password = document.getElementById('sapsanPassword').value;
            const button = document.getElementById('sapsanLoginButton');
            if (!email || !password) {
                showMessage('error', 'Enter your SAPSAN TERMINAL email and password.');
                return;
            }

            button.disabled = true;
            document.getElementById('sapsanStatus').textContent = 'Logging in...';
            try {
                const response = await fetch('/sapsan/login', {
                    method: 'POST',
                    headers: { 'Content-Type': 'application/x-www-form-urlencoded' },
                    body: new URLSearchParams({ email: email, password: password })
                });
                const data = await response.json().catch(() => ({}));
                if (!response.ok || !data.ok) throw new Error(data.message || ('HTTP ' + response.status));
                showMessage('success', 'Logged in to TERMINAL.');
            } catch (error) {
                showMessage('error', 'Login failed: ' + error.message);
            } finally {
                document.getElementById('sapsanPassword').value = '';
                button.disabled = false;
                refreshSapsanStatus();
            }
        }

        async function sapsanLogout() {
            try {
                await fetch('/sapsan/logout', { method: 'POST' });
                showMessage('success', 'Logged out of TERMINAL.');
            } catch (error) {
                showMessage('error', 'Error logging out: ' + error.message);
            }
            refreshSapsanStatus();
        }
    </script>
</body>
</html>
)=====";