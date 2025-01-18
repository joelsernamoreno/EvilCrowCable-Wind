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

/* tu je problem */
function handleSubmit(event) {
    event.preventDefault();

    var form = document.getElementById('uploadForm');
    var formData = new FormData(form);

    /* delete in future */
    console.log(Array.from(formData.entries()));

    fetch('/upload', {
        method: 'POST',
        body: formData,
        credentials: 'same-origin',
        mode: 'cors'
    })

    .then(response => response.text())
    .then(data => {
        document.getElementById('uploadMessage').innerText = 'Payload upload correctly';
        form.reset();
    })
    .catch(error => {
        console.error('Error:', error);
        document.getElementById('uploadMessage').innerText = 'Upload failed: ' + error;
    });
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