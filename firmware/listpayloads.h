const char StaticListPayloads[] PROGMEM = R"=====(
<!DOCTYPE HTML>
<html>
<head>
    <title>EvilCrowCable-Wind - Payloads</title>
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
            <li><a href='/livepayload'>Payload Editor</a></li>
            <li><a href='/uploadpayload'>Upload Payload</a></li>
            <li><a href='/listpayloads'>List Payloads</a></li>
            <li><a href='/autoexecplanning'>AutoExec Planning</a></li>
            <li><a href='/config'>Config</a></li>
        </ul>
    </nav>

    <div class="cable-wind-logo">PAYLOAD LIST</div>

    <div class="view-container">
        <div class="payload-header">
            <h3 style="margin: 0;">Available Payloads:</h3>
            <button class="select-os-btn" onclick="showOSFilterModal()">Filter OS</button>
        </div>
        <div class="payload-list-container">
            <div id="os-filter-modal" class="os-modal" style="display: none;">
                <div class="os-modal-content">
                    <h3>Select OS Filter</h3>
                    <div class="os-options">
                        <button data-os="all">ALL</button>
                        <button data-os="windows">Windows</button>
                        <button data-os="linux">Linux</button>
                        <button data-os="ios">iOS</button>
                        <button data-os="macos">macOS</button>
                        <button data-os="android">Android</button>
                    </div>
                    <button class="close-modal" onclick="hideOSFilterModal()">Cancel</button>
                </div>
            </div>
            <script>
                function showOSFilterModal() {
                    document.getElementById('os-filter-modal').style.display = 'flex';
                
                    document.querySelectorAll('#os-filter-modal .os-options button').forEach(btn => {
                        btn.onclick = function() {
                            const selectedOS = this.dataset.os;
                            hideOSFilterModal();
                            filterPayloadsByOS(selectedOS);
                
                            const filterBtn = document.querySelector('.select-os-btn');
                            if (selectedOS === 'all') {
                                filterBtn.textContent = 'Filter OS';
                            } else {
                                filterBtn.textContent = `OS: ${selectedOS}`;
                            }
                        };
                    });
                }

                function hideOSFilterModal() {
                    document.getElementById('os-filter-modal').style.display = 'none';
                }

                function filterPayloadsByOS(os) {
                    const payloadItems = document.querySelectorAll('.payload-item');
                    payloadItems.forEach(item => {
                        const itemOS = item.dataset.os || 'all';
                        if (os === 'all' || itemOS === os) {
                            item.style.display = '';
                        } else {
                            item.style.display = 'none';
                        }
                    });
                }

                // Optional: Default filter to 'all'
                document.addEventListener('DOMContentLoaded', () => {
                    filterPayloadsByOS('all');
                    updatePayloadCounter();
                });

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

                // delete payloads and metadata, then update file's counter
                function clearPayloads() {
                    if (confirm('Are you sure you want to delete all payloads and .meta files?')) {
                        fetch('/clearpayloads', { method: 'POST' })
                            .then(res => res.json())
                            .then(data => {
                                console.log('[✓] Payloads cleared response:', data);
                                showMessage('success', data.message || 'Payloads cleared');
                                // updatePayloadCounter(); // Actualiza el contador visual

                                // it allow to read the message
                                setTimeout(() => {
                                    location.reload(); // Recarga la página
                                }, 5000);
                            })
                            .catch(err => {
                                console.error('[!] Error clearing payloads:', err);
                                showMessage('error', 'Failed to clear payloads');
                            });
                    }
                }

                function updatePayloadCounter() {
                    fetch('/payloadcount')
                        .then(res => res.json())
                        .then(data => {
                            const countSpan = document.getElementById('payloadCounter');
                            countSpan.textContent = `${data.count}`;
                        })
                        .catch(err => console.error('Error counting payloads:', err));
                }

            </script>


            <!-- Payloads will be inserted here dynamically -->
)=====";