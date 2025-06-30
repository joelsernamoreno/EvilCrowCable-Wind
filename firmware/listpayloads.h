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
            <h3 style="margin: 0;">Available Payloads</h3>
            <div class="header-buttons">
                <button type="button" name="deleteAllPayloads" onclick="deleteAllPayloads()">Delete 0 payloads</button>
                <button class="select-os-btn" onclick="showOSFilterModal()">Filter OS</button>
            </div>
        </div>
        <hr>
        <div class="payload-list-container">
            <div id="os-filter-modal" class="os-modal" style="display: none;">
                <div class="os-modal-content" onclick="event.stopPropagation()">
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
                function deleteAllPayloads() {
                    if (confirm('WARNING: This will delete ALL payloads and their metadata. Are you sure?')) {
                        fetch('/deleteallpayloads', {
                            method: 'POST'
                        })
                        .then(response => response.json())
                        .then(data => {
                            if (data.success) {
                                // Update button text
                                const deleteButton = document.querySelector('button[name="deleteAllPayloads"]');
                                if (deleteButton) {
                                    deleteButton.textContent = 'Delete 0 payloads';
                                }

                                // Show detailed success message
                                let message = `Deleted ${data.count} payloads successfully:\n`;
                                if (data.deleted_files && data.deleted_files.length > 0) {
                                    message += data.deleted_files.join('\n');
                                }
                                showMessage('success', message);

                                // Refresh the list after a short delay
                                setTimeout(() => window.location.reload(), 1500);
                            } else {
                                showMessage('error', data.message || 'Error deleting payloads');
                            }
                        })
                        .catch(error => {
                            showMessage('error', 'Error deleting payloads');
                            console.error('Error:', error);
                        });
                    }
                }

                function showOSFilterModal() {
                    const modal = document.getElementById('os-filter-modal');
                    modal.style.display = 'flex';
                    modal.onclick = hideOSFilterModal;

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
                });
            </script>


            <!-- Payloads will be inserted here dynamically -->
)=====";