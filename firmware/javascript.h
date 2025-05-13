const char Redirect[] PROGMEM = R"=====(

function checkConnection() {
    fetch('/stats')
        .then(response => {
            document.querySelectorAll('.status-indicator').forEach(indicator => {
                indicator.classList.remove('status-offline');
                indicator.classList.add('status-online');
            });
            return response.json();
        })
        .catch(error => {
            if (error.name !== 'AbortError') { 
                document.querySelectorAll('.status-indicator').forEach(indicator => {
                    indicator.classList.remove('status-online');
                    indicator.classList.add('status-offline');
                });
            }
        })
        .then(data => {
            if (document.location.pathname === '/') {
                document.getElementById('uptime').innerText = data.uptime + ' seconds';
                document.getElementById('cpu0').innerText = data.cpu0 + ' MHz';
                document.getElementById('cpu1').innerText = data.cpu1 + ' MHz';
                document.getElementById('freespiffs').innerText = data.freespiffs + ' bytes';
                document.getElementById('temperature').innerText = data.temperature + ' °C';
                document.getElementById('totalram').innerText = data.totalram + ' bytes';
                document.getElementById('freeram').innerText = data.freeram + ' bytes';
            }
        });
}
setInterval(checkConnection, 1000);
checkConnection();

function togglePayload(action) {
    var checkbox = document.getElementById('toggle' + action);
    var message = document.getElementById('message' + action);

    if (checkbox.checked) {
        var form = document.getElementById('config');
        var formData = new FormData(form);

        fetch('/runlive' + action.toLowerCase(), {
            method: 'POST',
            body: formData,
            mode: 'cors'
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

function handleSubmit(event) {
    event.preventDefault();

    const formData = new FormData(document.getElementById('uploadForm'));
    const uploadMessage = document.getElementById('uploadMessageContainer');

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
    const container = document.getElementById('global-toast') || document.createElement('div');
    if (!container.id) {
        container.id = 'global-toast';
        container.className = 'toast-container';
        document.body.appendChild(container);
    }
    
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

document.addEventListener('gesturestart', function(e) {
    e.preventDefault();
});

// Prevent double-tap zoom on iOS
document.addEventListener('touchstart', function(event) {
    if (event.touches.length > 1) {
        event.preventDefault();
    }
}, { passive: false });
)=====";