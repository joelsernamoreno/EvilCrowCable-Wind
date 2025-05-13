const char Configuration[] PROGMEM = R"=====(
<!DOCTYPE HTML>
<html>
<head>
    <title>Configuration</title>
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

    <div class="view-container">
        <form id="layoutForm">
            <div class="form-group">
                <label for="layout">Keyboard Layout:</label>
                <select id="layout" name="layout-select">
                    <option value="layout1">EN_US</option>
                    <option value="layout2">ES_ES</option>
                    <option value="layout3">FR_FR</option>
                    <option value="layout4">IT_IT</option>
                    <option value="layout5">DA_DK</option>
                    <option value="layout6">DE_DE</option>
                    <option value="layout7">HR_HR</option>
                    <option value="layout8">HU_HU</option>
                    <option value="layout9">PT_PT</option>
                    <option value="layout10">PT_BR</option>
                    <option value="layout11">BE_BE</option>
                    <option value="layout12">BR_BR</option>
                    <option value="layout13">CA_CA</option>
                    <option value="layout14">CA_FR</option>
                    <option value="layout15">SK_SK</option>
                    <option value="layout16">CZ_CZ</option>
                    <option value="layout17">SV_SE</option>
                    <option value="layout18">SI_SI</option>
                </select>
            </div>
            <button type="button" onclick="applyLayout()">Apply</button>
        </form>

        <hr>

        <form id="wifiForm">
            <div class="form-group">
                <label for="ssid">Wi-Fi SSID:</label>
                <input type="text" id="ssid" name="ssid" required>
            </div>

            <div class="form-group">
                <label for="password">Wi-Fi Password:</label>
                <input type="password" id="password" name="password" required>
            </div>
            <button type="button" onclick="applyWiFi()">Apply</button>
            <button type="button" name="deleteWifiButton" onclick="deleteWiFiConfig()">Delete Wi-Fi Configuration</button>
        </form>

        <hr>

        <form id="usbForm">
            <div class="form-group">
                <label for="vendorID">Vendor ID:</label>
                <input type="text" id="vendorID" name="vendorID" required>
            </div>
            <div class="form-group">
                <label for="productID">Product ID:</label>
                <input type="text" id="productID" name="productID" required>
            </div>
            <div class="form-group">
                <label for="productName">Product Name:</label>
                <input type="text" id="productName" name="productName" required>
            </div>
            <div class="form-group">
                <label for="manufacturerName">Manufacturer Name:</label>
                <input type="text" id="manufacturerName" name="manufacturerName" required>
            </div>
            <button type="button" onclick="applyUSB()">Apply USB Configuration</button>
            <button type="button" name="deleteUSBButton" onclick="deleteUSBConfig()">Delete USB Configuration</button>
        </form>

        <hr>
    </div>

    <script>
        function showMessage(type, text) {
            const container = document.getElementById('global-toast');
            const toast = document.createElement('div');
            toast.className = `toast-message ${type}`;
            
            // Message content
            const messageSpan = document.createElement('span');
            messageSpan.textContent = text;
            
            // Close button
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
            
            // Auto-remove after delay
            const timer = setTimeout(() => {
                toast.style.animation = 'toastFadeOut 0.3s ease-out';
                setTimeout(() => toast.remove(), 300);
            }, 5000);
            
            // Clear timer if manually closed
            closeButton.onclick = () => {
                clearTimeout(timer);
                toast.style.animation = 'toastFadeOut 0.3s ease-out';
                setTimeout(() => toast.remove(), 300);
            };
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
        });
    </script>

</body>
</html>
)=====";