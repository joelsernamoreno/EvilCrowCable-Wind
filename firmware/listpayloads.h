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
            <span id="selected-os-label">ALL</span>
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
                        btn.onclick = function () {
                            const selectedOS = this.dataset.os;
                            document.getElementById('selected-os-label').textContent = selectedOS.toUpperCase();
                            document.getElementById('os-filter-modal').style.display = 'none';
                            filterPayloadsByOS(selectedOS);
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