const char UploadPayload[] PROGMEM = R"=====(
<!DOCTYPE HTML>
<html>
<head>
    <title>Upload Payload</title>
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
        <form id="uploadForm" method="POST" action="/upload" enctype="multipart/form-data" onsubmit="handleSubmit(event)">
            <div class="form-group">
                <label for="payloadName">Payload Name:</label>
                <input type="text" id="payloadName" name="payloadName" placeholder="Enter a descriptive name" required>
            </div>
            <div class="form-group">
                <label for="payloadDescription">Description:</label>
                <textarea id="payloadDescription" name="payloadDescription" placeholder="Enter a brief description" rows="3"></textarea>
            </div>
            <div class="form-group">
                <label for="uploadFile">Payload File (.txt only):</label>
                <input type="file" id="uploadFile" name="uploadFile" required accept=".txt">
            </div>
            <input type="submit" value="Upload">
        </form>
    </div>

    <script>
        function handleSubmit(event) {
            event.preventDefault();
            const form = document.getElementById('uploadForm');
            const fileInput = form.querySelector('input[type="file"]');
            const nameInput = form.querySelector('#payloadName');
            const descInput = form.querySelector('#payloadDescription');
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
        
            // Proceed with upload
            const formData = new FormData(form);
            const xhr = new XMLHttpRequest();
        
            xhr.upload.onprogress = function(e) {
                if (e.lengthComputable) {
                    const percent = Math.round((e.loaded / e.total) * 100);
                    document.querySelector('.upload-progress').textContent = `Uploading: ${percent}%`;
                }
            };
        
            xhr.onload = function() {
                if (xhr.status === 200) {
                    const response = JSON.parse(xhr.responseText);
                    showMessage('success', response.message);
                    form.reset();
                } else {
                    try {
                        const response = JSON.parse(xhr.responseText);
                        showMessage('error', response.message);
                    } catch {
                        showMessage('error', 'Upload failed');
                    }
                }
            };
        
            xhr.open('POST', '/upload', true);
            xhr.send(formData);
        
            // Add progress element
            const progressDiv = document.createElement('div');
            progressDiv.className = 'upload-progress';
            document.getElementById('uploadMessageContainer').appendChild(progressDiv);
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