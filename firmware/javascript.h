const char Redirect[] PROGMEM = R"=====(

function checkConnection() {
    fetch('/stats')
        .then(response => {
            // Update ALL status indicators (including home page stats)
            document.querySelectorAll('.status-indicator').forEach(indicator => {
                indicator.classList.remove('status-offline');
                indicator.classList.add('status-online');
            });

            // Only update title indicators on NON-home pages
            if (window.location.pathname !== '/') {
                document.querySelectorAll('.cable-wind-logo').forEach(title => {
                    title.classList.remove('offline');
                    title.classList.add('online');
                });
            }
            return response.json();
        })
        .catch(error => {
            if (error.name !== 'AbortError') { 
                document.querySelectorAll('.status-indicator').forEach(indicator => {
                    indicator.classList.remove('status-online');
                    indicator.classList.add('status-offline');
                });

                if (window.location.pathname !== '/') {
                    document.querySelectorAll('.cable-wind-logo').forEach(title => {
                        title.classList.remove('online');
                        title.classList.add('offline');
                    });
                }
            }
        })
        .then(data => {
            if (document.location.pathname === '/') {
                // Update home page stats (connection status remains here)
                document.getElementById('uptime').innerText = data.uptime + ' seconds';
                document.getElementById('cpu0').innerText = data.cpu0 + ' MHz';
                // ... keep all other stat updates ...
            }
        });
}

setInterval(checkConnection, 5000);
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

function debounce(func, wait) {
  let timeout;
  return function() {
    const context = this, args = arguments;
    clearTimeout(timeout);
    timeout = setTimeout(() => func.apply(context, args), wait);
  };
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
        toast.style.animation = 'toastFadeOut 0.3s ease-out';payload
        
        setTimeout(() => toast.remove(), 300);
    };
}

// Prevent double-tap zoom on iOS
document.addEventListener('touchstart', function(event) {
    if (event.touches.length > 1) {
        event.preventDefault();
    }
}, { passive: false });

document.addEventListener('gesturestart', function(e) {
    e.preventDefault();
});

document.addEventListener("DOMContentLoaded", function() {
  const links = document.querySelectorAll("#menu a");
  links.forEach(link => {
    if (link.getAttribute("href") === window.location.pathname) {
      link.classList.add("active");
    }
    // Debounced click handler
    link.addEventListener('click', debounce(function(e) {
      // Prevent default only if not already active
      if (this.classList.contains('active')) {
        e.preventDefault();
        return;
      }

      // Show loading indicator
      document.body.classList.add('page-loading');

      // Force connection initiation
      fetch('/stats', {cache: "no-store"})
        .then(() => {
          window.location.href = this.href;
        });
    }, 300)); // 300ms debounce time
  });
});

// Loading indicator
document.write(`    <style>
        .page-loading::before {
          content: '';
          position: fixed;
          top: 0;
          left: 0;
          width: 100%;
          height: 100%;
          background: rgba(0,0,0,0.7);
          z-index: 9999;
        }
        .page-loading::after {
          content: 'Loading...';
          position: fixed;
          top: 50%;
          left: 50%;
          transform: translate(-50%, -50%);
          color: #00f2ff;
          font-size: 1.5em;
          z-index: 10000;
        }
        .validation-text {
          font-size: 0.75em;
        }
        @media (max-width: 768px) {
          .validation-text {
            font-size: 0.7em;
          }
        }
      </style>
`);
)=====";