const char LivePayload[] PROGMEM = R"=====(
<!DOCTYPE HTML>
<html>
<head>
    <title>Live Payload</title>
    <meta charset="utf-8">
    <link rel="stylesheet" href="style.css">
    <style>
        .switch-container {
            display: flex;
            justify-content: space-around;
            align-items: center;
            margin-top: 20px;
            flex-wrap: wrap;
        }

        .switch-group {
            display: flex;
            align-items: center;
            padding: 10px 20px;
            background-color: #333;
            border: 1px solid #555;
            border-radius: 10px;
            box-shadow: 0 0 10px rgba(0, 0, 0, 0.1);
            max-width: 300px;
            margin: 10px;
            text-align: left;
        }

        .switch-label {
            font-size: 16px;
            font-weight: bold;
            margin-right: 10px;
            color: white;
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
            background-color: #ccc;
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
            background-color: #2196F3;
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

        .messages-container {
            display: flex;
            justify-content: center;
            align-items: center;
            margin-top: 20px;
        }

        .message {
            color: green;
            font-weight: bold;
            text-align: center;
            max-width: 300px;
            margin: 10px;
        }
    </style>
    <script>
        function togglePayload(action) {
            var checkbox = document.getElementById('toggle' + action);
            var message = document.getElementById('message' + action);

            if (checkbox.checked) {
                var form = document.getElementById('config');
                var formData = new FormData(form);

                fetch('/runlive' + action.toLowerCase(), {
                    method: 'POST',
                    body: formData
                })
                .then(response => response.text())
                .then(data => {
                    pollPayloadStatus(action);
                })
                .catch(error => {
                    console.error('Error:', error);
                });
            } else {
                message.innerText = '';
            }
        }

        function pollPayloadStatus(action) {
            var checkbox = document.getElementById('toggle' + action);
            var message = document.getElementById('message' + action);

            fetch('/payloadstatus' + action.toLowerCase())
            .then(response => response.json())
            .then(data => {
                console.log('Polling result:', data);

                if (data.status === 'executed') {
                    checkbox.checked = false;
                    message.innerText = 'The action has been executed successfully';
                } else {
                    setTimeout(function() { pollPayloadStatus(action); }, 1000);
                }
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

<form action="/runlivepayload" method="post" id="config" target="iframe">
    <hr>
    <p>Payload: </p>
    <textarea class="css-input-text" form="config" rows="30" cols="50" name="livepayload"></textarea>
    <hr>
    <input type="radio" name="configmodule" value="1" hidden="1" checked="checked">
</form>
<br>
<div class="switch-container">
    <div class="switch-group">
        <span class="switch-label">Run payload</span>
        <label class="switch">
            <input type="checkbox" id="togglePayload" onclick="togglePayload('Payload')">
            <span class="slider round"></span>
        </label>
    </div>

    <div class="switch-group">
        <span class="switch-label">Startup exec payload</span>
        <label class="switch">
            <input type="checkbox" id="toggleStartup" onclick="togglePayload('Startup')">
            <span class="slider round"></span>
        </label>
    </div>

    <div class="switch-group">
        <span class="switch-label">Save payload</span>
        <label class="switch">
            <input type="checkbox" id="toggleSave" onclick="togglePayload('Save')">
            <span class="slider round"></span>
        </label>
    </div>
</div>
<div class="messages-container">
    <div id="messagePayload" class="message"></div>
    <div id="messageStartup" class="message"></div>
    <div id="messageSave" class="message"></div>
</div>
<br>
<hr>
<br>
<iframe style="visibility: hidden;" src="http://" )+local_IPstr+"/runlivepayload" name="iframe"></iframe>
</body>
</html>
)=====";
