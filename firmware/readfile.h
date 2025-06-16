const char StaticFileList[] PROGMEM = R"=====(
<!DOCTYPE HTML>
<html>
<head>
    <title>EvilCrowCable-Wind - Payload</title>
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

    <div class="cable-wind-logo">PAYLOAD VIEWER</div>

    <div class="view-container">
        <h3>{{path}}:</h3>
        <div class="form-group">
            <textarea id="payloadContent" class="terminal-style" name="payloadContent" spellcheck="false">{{payloadContent}}</textarea>
        </div>

        <div class="button-container">
            <button type="button" onclick="savePayloadChanges()">Save Changes</button>
            <button type="button" onclick="runPayloadFromViewer()">Run Payload</button>
            <button type="button" onclick="deletePayload()" style="background: linear-gradient(135deg, var(--error), #cc0022);">Delete Payload</button>
        </div>
    </div>

    <script>

        function savePayloadChanges() {
            const payloadContent = document.getElementById('payloadContent').value;
            const path = '{{path}}'; // This comes from the server-side template
        
            fetch('/updatepayload', {
                method: 'POST',
                headers: {
                    'Content-Type': 'application/json', // Changed to JSON
                },
                body: JSON.stringify({  // Send as JSON object
                    path: path,
                    content: payloadContent
                })
            })
            .then(response => response.json())
            .then(data => {
                if (data.success) {
                    showMessage('success', 'Payload updated successfully!');
                } else {
                    showMessage('error', data.message || 'Failed to update payload');
                }
            })
            .catch(error => {
                showMessage('error', 'Error updating payload');
                console.error('Error:', error);
            });
        }

        function runPayloadFromViewer() {
            const payloadContent = document.getElementById('payloadContent').value;
            const path = '{{path}}';
            
            fetch('/runlivepayload', {
                method: 'POST',
                headers: {
                    'Content-Type': 'application/x-www-form-urlencoded',
                },
                body: `livepayload=${encodeURIComponent(payloadContent)}&configmodule=${encodeURIComponent(path)}`
            })
            .then(response => response.text())
            .then(data => {
                showMessage('success', 'Payload running...');
            })
            .catch(error => {
                showMessage('error', 'Error executing payload');
                console.error('Error:', error);
            });
        }
    
        function deletePayload() {
            if (confirm('Are you sure you want to delete this payload?')) {
                const path = '{{path}}';
                
                fetch('/deletepayload', {
                    method: 'POST',
                    headers: {
                        'Content-Type': 'application/x-www-form-urlencoded',
                    },
                    body: `configmodule=${encodeURIComponent(path)}`
                })
                .then(response => response.text())
                .then(data => {
                    showMessage('success', 'Payload deleted successfully');
                    setTimeout(function() {
                        window.location.href = '/listpayloads';
                    }, 1000);
                })
                .catch(error => {
                    showMessage('error', 'Error deleting payload');
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

        document.getElementById('runPayloadCheckbox').addEventListener('change', function() {
            handleToggle('/dopayload', this.checked);
        });

        document.getElementById('deletePayloadCheckbox').addEventListener('change', function() {
            if (this.checked) {
                if (confirm('Are you sure you want to delete this payload?')) {
                    handleDelete('/deletepayload', this.checked);
                } else {
                    this.checked = false;
                }
            }
        });

        async function handleToggle(url, checked) {
            if (checked) {
                let response = await fetch(url, {
                    method: 'POST',
                    body: new URLSearchParams({configmodule:'{{path}}'})
                });
                let message = await response.text();
                showMessage('success', message);

                setTimeout(function() {
                    document.getElementById('runPayloadCheckbox').checked = false;
                }, 2000); 
            }
        }

        async function handleDelete(url, checked) {
            if (checked) {
                let response = await fetch(url, {
                    method: 'POST',
                    body: new URLSearchParams({configmodule:'{{path}}'})
                });
                let message = await response.text();
                showMessage('success', message);

                setTimeout(function() {
                    window.location.href = '/listpayloads';
                }, 1000);
            }
        }
    </script>
</body>
</html>
)=====";