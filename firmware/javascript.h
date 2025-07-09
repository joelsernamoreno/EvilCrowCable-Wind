const char Redirect[] PROGMEM = R"=====(

// Track navigation state and abort controller globally
let isNavigating = false;
let abortControllers = [];
let navigationTimeout = null;
const isHomePage = window.location.pathname === '/' || window.location.pathname === '/index.html';
let lastConnectionCheck = 0;

function checkConnection() {
    // Debounce check - skip if called too frequently
    const now = Date.now();
    if (now - lastConnectionCheck < 2000) return;
    lastConnectionCheck = now;

    if (isNavigating) {
        abortAllRequests();
        return;
    }

    const controller = new AbortController();
    abortControllers.push(controller);

    const timeout = setTimeout(() => controller.abort(), 1500);

    fetch(isHomePage ? '/stats' : '/connectioncheck', { 
        signal: controller.signal 
    })
    .then(response => {
        clearTimeout(timeout);
        if (isNavigating) return;
        if (isHomePage) {
            return response.json().then(data => {
                updateStats(data);
            });
        } else {
            updateConnectionStatus(response.ok);
            return response.json();
        }
    })
    .catch(error => {
        clearTimeout(timeout);
        if (error.name !== 'AbortError' && !isNavigating) {
            updateConnectionStatus(false);
            if (isHomePage) {
                // Set default values when offline
                document.getElementById('uptime').innerText = 'N/A';
                document.getElementById('cpu0').innerText = 'N/A';
                document.getElementById('cpu1').innerText = 'N/A';
                document.getElementById('temperature').innerText = 'N/A';
                document.getElementById('freespiffs').innerText = 'N/A';
                document.getElementById('totalram').innerText = 'N/A';
                document.getElementById('freeram').innerText = 'N/A';
                document.getElementById('targetos').innerText = 'N/A';
                document.getElementById('ssid').innerText = 'N/A';
                document.getElementById('ipaddress').innerText = 'N/A';
            }
        }
    });
}

function abortAllRequests() {
    abortControllers.forEach(controller => controller.abort());
    abortControllers = [];
}

function setupNavigation() {
    const links = document.querySelectorAll("#menu a");

    links.forEach(link => {
        link.addEventListener('click', function(e) {
            if (this.classList.contains('active')) {
                e.preventDefault();
                return;
            }

            // Set navigation flag and abort requests
            isNavigating = true;
            abortAllRequests();

            // Show loading indicator immediately
            document.body.classList.add('page-loading');

            // For iOS, use a more reliable navigation method
            if (/iPad|iPhone|iPod/.test(navigator.userAgent)) {
                e.preventDefault();

                // Use location.replace() for cleaner navigation stack
                setTimeout(() => {
                    window.location.replace(this.href);
                }, 50);
            } else {
                // For desktop, proceed normally with a slight delay
                setTimeout(() => {
                    window.location.href = this.href;
                }, 100);
            }
        });
    });
}

// Reset navigation state when page loads
window.addEventListener('load', () => {
    isNavigating = false;
    document.body.classList.remove('page-loading');
});

function updateConnectionStatus(isOnline) {
    document.querySelectorAll('.status-indicator').forEach(indicator => {
        indicator.classList.toggle('status-online', isOnline);
        indicator.classList.toggle('status-offline', !isOnline);
    });

    if (window.location.pathname !== '/') {
        document.querySelectorAll('.cable-wind-logo').forEach(title => {
            title.classList.toggle('online', isOnline);
            title.classList.toggle('offline', !isOnline);
        });
    }
}

function updateStats(data) {
    // Update all stats that exist in index.h
    if (data.uptime) document.getElementById('uptime').innerText = data.uptime + ' seconds';
    if (data.cpu0) document.getElementById('cpu0').innerText = data.cpu0 + ' MHz';
    if (data.cpu1) document.getElementById('cpu1').innerText = data.cpu1 + ' MHz';
    if (data.temperature) document.getElementById('temperature').innerText = data.temperature.toFixed(1) + ' °C';
    if (data.freespiffs) document.getElementById('freespiffs').innerText = formatBytes(data.freespiffs);
    if (data.totalram) document.getElementById('totalram').innerText = formatBytes(data.totalram);
    if (data.freeram) document.getElementById('freeram').innerText = formatBytes(data.freeram);
    if (data.os) document.getElementById('targetos').innerText = data.os;
    if (data.ssid) document.getElementById('ssid').innerText = data.ssid;
    if (data.ipaddress) document.getElementById('ipaddress').innerText = data.ipaddress;

    // Update connection status indicator
    updateConnectionStatus(true);
}

// Format bytes helper function
function formatBytes(bytes) {
    if (bytes === 0) return '0 Bytes';
    const k = 1024;
    const sizes = ['Bytes', 'KB', 'MB', 'GB'];
    const i = Math.floor(Math.log(bytes) / Math.log(k));
    return parseFloat((bytes / Math.pow(k, i)).toFixed(2)) + ' ' + sizes[i];
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

function countPayloads() {
    fetch('/payloadcounter')
        .then(response => response.json())
        .then(data => {
            if (data.count >= 0) {
                const deleteButton = document.querySelector('button[name="deleteAllPayloads"]');
                if (deleteButton) {
                    deleteButton.textContent = `Delete ${data.count} payloads`;
                }
            }
        })
        .catch(error => {
            console.error('Error counting payloads:', error);
        });
}

function setupOSFilter() {
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
    setupNavigation(); 

    if (window.location.pathname === '/listpayloads') {
        countPayloads();
    }

    // Reset navigation flag when page loads
    isNavigating = false;
    document.body.classList.remove('page-loading');
});
)=====";