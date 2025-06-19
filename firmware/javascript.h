const char Redirect[] PROGMEM = R"=====(

function checkConnection() {
    if (isNavigating) return;
    // AbortController to prevent hanging
    const controller = new AbortController();
    const timeoutId = setTimeout(() => controller.abort(), 1000); // 1s timeout
    fetch('/stats', { signal: controller.signal })
        .then(response => {
            clearTimeout(timeoutId);
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
            clearTimeout(timeoutId);
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

document.addEventListener("DOMContentLoaded", function () {
    let isNavigating = false;
    // --- Highlight the active navigation link ---
    const links = document.querySelectorAll("#menu a");
    links.forEach(link => {
        link.addEventListener('click', function(e) {
            if (this.classList.contains('active')) {
                e.preventDefault();
                return;
            }
            isNavigating = true;
            document.body.classList.add('page-loading');
            window.location.href = this.href;
        });
    });

    // --- OS-based payload filter ---
    const select = document.getElementById("os-filter");
    if (select) {
        function filterPayloads(os) {
            const payloads = document.querySelectorAll(".payload-item");
            payloads.forEach(el => {
                const itemOS = el.getAttribute("data-os") || "all";
                el.style.display = (os === "all" || itemOS === os) ? "" : "none";
            });
        }
    
        // Defer localStorage access to prevent blocking
        setTimeout(() => {
            // Load saved filter option from localStorage
            const savedOS = localStorage.getItem("selectedOS") || "all";
            select.value = savedOS;
            filterPayloads(savedOS);
        }, 0);
    
        // Save new selection and re-filter
        select.addEventListener("change", function() {
            // Use requestIdleCallback for non-urgent updates
            requestIdleCallback(() => {
                const selected = this.value.toLowerCase();
                localStorage.setItem("selectedOS", selected);
                filterPayloads(selected);
            }, { timeout: 200 }); // Fallback if idle takes too long
        });
    }
});
)=====";