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
        <form id="uploadForm" method="POST" action="/upload" enctype="multipart/form-data" onsubmit="handleSubmit(event)">
            <input type="file" name="uploadFile" required accept=".txt">
            <input type="submit" value="Upload">
        </form>
    </div>

    <div class="messages-container" id="uploadMessageContainer"></div>

    <script>
        function handleSubmit(event) {
            event.preventDefault();

            const formData = new FormData(document.getElementById('uploadForm'));

            fetch('/upload', {
                method: 'POST',
                body: formData
            })
            .then(response => {
                if (response.ok) {
                    showMessage('success', 'File uploaded successfully');
                } else {
                    showMessage('error', 'Error uploading file');
                }
            })
            .catch(error => {
                showMessage('error', 'Error uploading file');
                console.error('Error:', error);
            });
        }

        function showMessage(type, text) {
            const container = document.querySelector('#uploadMessageContainer');
            const messageDiv = document.createElement('div');
            messageDiv.className = `message ${type}`;
            messageDiv.textContent = text;

            if (type === 'error') {
                messageDiv.style.color = 'red';
            } else if (type === 'success') {
                messageDiv.style.color = 'green';
            }

            container.innerHTML = '';
            container.appendChild(messageDiv);

            setTimeout(() => {
                container.innerHTML = '';
            }, 5000);
        }
    </script>
</body>
</html>
)=====";