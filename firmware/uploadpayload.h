const char UploadPayload[] PROGMEM = R"=====(
<!DOCTYPE HTML>
<html>
<head>
    <title>EvilCrowCable-Wind - Upload Payload</title>
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

    <div class="cable-wind-logo">UPLOAD PAYLOAD</div>

    <div class="view-container">
        <form id="uploadForm" method="POST" action="/upload" enctype="multipart/form-data" onsubmit="handleSubmit(event)">
            <div class="form-group">
                <label for="payloadName">Payload Name:</label>
                <input type="text" id="payloadName" name="payloadName" placeholder="Enter a descriptive name" required class="terminal-style">
            </div>
            <div class="form-group">
                <label for="payloadDescription">Description:</label>
                <textarea id="payloadDescription" name="payloadDescription" 
                          placeholder="Write a short description" 
                          rows="5" class="terminal-style"></textarea>
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
            <div class="form-group">
                <label for="uploadFile">Payload File (.txt only):</label>
                <input type="file" id="uploadFile" name="uploadFile" required accept=".txt" class="terminal-style single-line-input">
            </div>
            <input type="submit" value="Upload Payload">
        </form>
    </div>

    <script>
        function handleSubmit(event) {
            event.preventDefault();
            
            const fileInput = document.getElementById('uploadFile');
            const nameInput = document.getElementById('payloadName');
            const file = fileInput.files[0];
            const MAX_SIZE = 200 * 1024; // 200KB in bytes
        
            // Client-side validation
            if (!file) {
                showMessage('error', 'Please select a file');
                return;
            }
        
            if (!file.name.endsWith('.txt')) {
                showMessage('error', 'Only .txt files allowed');
                return;
            }
        
            if (file.size > MAX_SIZE) {
                showMessage('error', 'File too large (max 200KB)');
                return;
            }
        
            if (!nameInput.value.trim()) {
                showMessage('error', 'Payload name is required');
                return;
            }
        
            // Create FormData and submit
            const formData = new FormData(document.getElementById('uploadForm'));
            
            fetch('/upload', {
                method: 'POST',
                body: formData
            })
            .then(response => response.json())
            .then(data => {
                if (data.status === 'success') {
                    showMessage('success', data.message);
                    document.getElementById('uploadForm').reset();
                } else {
                    showMessage('error', data.message);
                }
            })
            .catch(error => {
                showMessage('error', 'Error uploading file');
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
    </script>
</body>
</html>
)=====";