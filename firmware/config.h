const char Configuration[] PROGMEM = R"=====(
<!DOCTYPE HTML>
<html>
<head>
    <title>Configuration</title>
    <meta charset="utf-8">
    <style>
        body {
            font-family: Arial, sans-serif;
            background-color: #f0f0f0;
            margin: 0;
            padding: 20px;
        }
        .container {
            max-width: 600px;
            margin: auto;
            background: white;
            padding: 20px;
            border-radius: 10px;
            box-shadow: 0 0 10px rgba(0, 0, 0, 0.1);
        }
        h2 {
            text-align: center;
            color: #333;
        }
        .form-group {
            margin-bottom: 20px;
        }
        label {
            display: block;
            margin-bottom: 5px;
            font-weight: bold;
        }
        select, input[type="text"], input[type="password"] {
            width: 100%;
            padding: 10px;
            margin-top: 5px;
            border: 1px solid #ccc;
            border-radius: 5px;
        }
        button {
            display: block;
            width: 100%;
            padding: 10px;
            background-color: #4CAF50;
            color: white;
            border: none;
            border-radius: 5px;
            font-size: 16px;
        }
        button:hover {
            background-color: #45a049;
        }
        .message {
            margin-top: 20px;
            padding: 10px;
            border-radius: 5px;
            display: none;
        }
        .success {
            background-color: #d4edda;
            color: #155724;
            border: 1px solid #c3e6cb;
        }
        .error {
            background-color: #f8d7da;
            color: #721c24;
            border: 1px solid #f5c6cb;
        }
    </style>
</head>
<body>
<div class="container">
    <h2>Configuration</h2>
    <form id="layoutForm">
        <div class="form-group">
            <label for="layout">Set layout:</label>
            <select id="layout" name="layout">
                <option value="layout1">EN_US</option>
                <option value="layout2">ES_ES</option>
                <option value="layout3">FR_FR</option>
                <option value="layout4">IT_IT</option>
                <option value="layout5">DA_DK</option>
                <option value="layout6">DE_DE</option>
                <option value="layout7">HR_HR</option>
                <option value="layout8">PT_PT</option>
                <option value="layout9">BE_BE</option>
                <option value="layout10">BR_BR</option>
                <option value="layout11">CA_CA</option>
                <option value="layout12">CA_FR</option>
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
    </form>
    <hr>
    <form id="deleteWiFiForm">
        <button type="button" onclick="deleteWiFiConfig()">Delete Wi-Fi Config</button>
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
        <button type="button" onclick="applyUSB()">Apply USB Config</button>
    </form>
    <hr>
    <form id="deleteUSBForm">
        <button type="button" onclick="deleteUSBConfig()">Delete USB Config</button>
    </form>
    <div id="message" class="message"></div>
</div>

<script>
    function showMessage(type, text) {
        const messageDiv = document.getElementById('message');
        messageDiv.className = `message ${type}`;
        messageDiv.textContent = text;
        messageDiv.style.display = 'block';
    }

    function applyLayout() {
        const form = document.getElementById('layoutForm');
        const formData = new FormData(form);
        fetch('/layout', {
            method: 'POST',
            body: formData
        })
        .then(response => response.text())
        .then(data => {
            showMessage('success', 'Layout applied successfully!');
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
            showMessage('error', 'SSID and Password are required.');
            return;
        }

        const form = document.getElementById('wifiForm');
        const formData = new FormData(form);
        fetch('/updatewifi', {
            method: 'POST',
            body: formData
        })
        .then(response => response.text())
        .then(data => {
            showMessage('success', 'WiFi settings applied. The device will restart.');
        })
        .catch(error => {
            showMessage('error', 'Error applying WiFi settings.');
            console.error('Error:', error);
        });
    }

    function deleteWiFiConfig() {
        fetch('/deletewificonfig', {
            method: 'POST'
        })
        .then(response => response.text())
        .then(data => {
            showMessage('success', 'Wi-Fi configuration deleted successfully!');
        })
        .catch(error => {
            showMessage('error', 'Error deleting Wi-Fi configuration.');
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

        const form = document.getElementById('usbForm');
        const formData = new FormData(form);
        fetch('/updateusb', {
            method: 'POST',
            body: formData
        })
        .then(response => response.text())
        .then(data => {
            showMessage('success', 'USB settings applied successfully!');
        })
        .catch(error => {
            showMessage('error', 'Error applying USB settings.');
            console.error('Error:', error);
        });
    }

    function deleteUSBConfig() {
        fetch('/deleteusbconfig', {
            method: 'POST'
        })
        .then(response => response.text())
        .then(data => {
            showMessage('success', 'USB configuration deleted successfully!');
        })
        .catch(error => {
            showMessage('error', 'Error deleting USB configuration.');
            console.error('Error:', error);
        });
    }
</script>
</body>
</html>
)=====";