const char UploadPayload[] PROGMEM = R"=====(
<!DOCTYPE HTML>
<html>
<head>
    <title>Upload Payload</title>
    <meta charset="utf-8">
    <link rel="stylesheet" href="style.css">
    <style>
        .upload-container {
            display: flex;
            flex-direction: column;
            align-items: center;
            padding: 20px;
            background-color: #333;
            border: 1px solid #444;
            border-radius: 8px;
            box-shadow: 0 0 10px rgba(0, 0, 0, 0.2);
            max-width: 500px;
            margin: auto;
            color: #fff;
        }

        .switch-group {
            display: flex;
            align-items: center;
            margin-bottom: 20px;
            width: 100%;
            justify-content: center;
        }

        .switch-label {
            font-size: 18px;
            font-weight: bold;
            margin-right: 10px;
            color: #fff;
        }

        .switch {
            position: relative;
            display: inline-block;
            width: 60px;
            height: 34px;
        }

        .switch input {
            opacity: 0;
            width: 0;
            height: 0;
        }

        .slider {
            position: absolute;
            cursor: pointer;
            top: 0;
            left: 0;
            right: 0;
            bottom: 0;
            background-color: #888;
            transition: .4s;
        }

        .slider:before {
            position: absolute;
            content: "";
            height: 26px;
            width: 26px;
            left: 4px;
            bottom: 4px;
            background-color: white;
            transition: .4s;
        }

        input:checked + .slider {
            background-color: #4CAF50;
        }

        input:checked + .slider:before {
            transform: translateX(26px);
        }

        .slider.round {
            border-radius: 34px;
        }

        .slider.round:before {
            border-radius: 50%;
        }

        .file-input {
            display: flex;
            flex-direction: column;
            align-items: center;
            margin-top: 20px;
        }

        .file-input input[type="file"] {
            margin-bottom: 10px;
            border: none;
            background-color: #555;
            color: #fff;
            padding: 10px;
            border-radius: 5px;
            cursor: pointer;
        }

        .file-input input[type="submit"] {
            background-color: #4CAF50;
            border: none;
            color: #fff;
            padding: 15px 30px;
            text-align: center;
            text-decoration: none;
            display: inline-block;
            font-size: 16px;
            margin-top: 10px;
            border-radius: 5px;
            cursor: pointer;
            transition: background-color 0.3s;
        }

        .file-input input[type="submit"]:hover {
            background-color: #45a049;
        }

        .message {
            color: #4CAF50;
            font-weight: bold;
            text-align: center;
            margin-top: 20px;
            font-size: 16px;
        }
    </style>
    <script>
        function toggleUpload() {
            var checkbox = document.getElementById('toggleUpload');
            var form = document.getElementById('uploadForm');
            var message = document.getElementById('uploadMessage');

            if (checkbox.checked) {
                form.style.display = 'block';
                message.innerText = '';
            } else {
                form.style.display = 'none';
                message.innerText = '';
            }
        }

        function handleSubmit(event) {
            event.preventDefault();

            var form = document.getElementById('uploadForm');
            var formData = new FormData(form);

            fetch('/upload', {
                method: 'POST',
                body: formData
            })
            .then(response => response.text())
            .then(data => {
                document.getElementById('uploadMessage').innerText = 'Payload upload correctly';
                document.getElementById('toggleUpload').checked = false;
                form.style.display = 'none';
                form.reset();
            })
            .catch(error => {
                console.error('Error:', error);
            });
        }
    </script>
</head>
<body>
    <nav id='menu'>
        <input type='checkbox' id='responsive-menu' onclick='updatemenu()'><label></label>
        <ul>
            <li><a href='/'>Home</a></li>
            <li><a href='/livepayload'>Live Payload</a></li>
            <li><a href='/uploadpayload'>Upload Payload</a></li>
            <li><a href='/listpayloads'>List Payloads</a></li>
            <li><a href='/config'>Config</a></li>
        </ul>
    </nav>
    <br>
    <hr>
    <br>
    <div class="upload-container">
        <div class="switch-group">
            <span class="switch-label">Upload file</span>
            <label class="switch">
                <input type="checkbox" id="toggleUpload" onclick="toggleUpload()">
                <span class="slider round"></span>
            </label>
        </div>
        <form id="uploadForm" method="POST" action="/upload" enctype="multipart/form-data" style="display: none;" onsubmit="handleSubmit(event)">
            <input type="file" name="upload" required>
            <input type="submit" value="Upload">
        </form>
        <div id="uploadMessage" class="message"></div>
    </div>
</body>
</html>
)=====";