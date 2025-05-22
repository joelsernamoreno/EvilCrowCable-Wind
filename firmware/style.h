const char Style[] PROGMEM = R"=====(
body {
    -webkit-text-size-adjust: 100%;
    -webkit-tap-highlight-color: rgba(0,0,0,0);
    background-color: #333333;
    font-family: -apple-system, BlinkMacSystemFont, "Segoe UI", Roboto, Helvetica, Arial, sans-serif;
    color: white;
    margin: 10px;
    padding: env(safe-area-inset-top) env(safe-area-inset-right) env(safe-area-inset-bottom) env(safe-area-inset-left);
    -webkit-touch-callout: none;
    -webkit-user-select: none;
    user-select: none;
    -webkit-overflow-scrolling: touch;
    overscroll-behavior-y: none;
    -webkit-font-smoothing: antialiased;
}

/* menu */
#menu {
    background: #282829;
    background: linear-gradient(to bottom, #5D5C5E, #000000);
    color: #FFF;
    /* height: 48px; */
    height: 52px;
    padding-left: 18px;
    border-radius: 5px;
    font-family: -apple-system, BlinkMacSystemFont, "Segoe UI", Roboto, Helvetica, Arial, sans-serif;
    font-size: 17px;
    box-shadow: 0 2px 5px rgba(0, 0, 0, 0.2);
    transition: all 0.3s ease;
    -webkit-backdrop-filter: saturate(180%) blur(20px);
    backdrop-filter: saturate(180%) blur(20px);
    position: -webkit-sticky;
    position: sticky;
    top: 0;
    z-index: 100;
}

#menu ul, #menu li {
    margin: 0 auto;
    padding: 0;
    list-style: none;
    -webkit-overflow-scrolling: touch;
}
#menu ul {
    width: 100%;
}
#menu li {
    float: left;
    display: inline;
    position: relative;
}

#menu a {
    display: block;
    line-height: 52px;
    padding: 0 14px;
    text-decoration: none;
    color: #FFFFFF;
    font-size: 17px;
    -webkit-tap-highlight-color: transparent;
}

#menu a.dropdown-arrow:after {
    content: "\25BE";
    margin-left: 5px;
}

#menu li a:hover {
    color: #007AFF;
    background: rgba(242, 242, 242, 0.8);
}

#menu input {
    display: none;
    margin: 0;
    padding: 0;
    height: 52px;
    width: 100%;
    opacity: 0;
    cursor: pointer;
}

#menu label {
    display: none;
    line-height: 20px;
    text-align: center;
    position: absolute;
    left: 35px;
}

#menu label:before {
    font-size: 1.6em;
    content: "\2261"; 
    margin-left: 20px;
}

#menu ul.sub-menus {
    height: auto;
    overflow: hidden;
    width: 170px;
    background: rgba(68, 68, 68, 0.95);
    position: absolute;
    z-index: 99;
    display: none;
    -webkit-backdrop-filter: saturate(180%) blur(20px);
    backdrop-filter: saturate(180%) blur(20px);
}

#menu ul.sub-menus li {
    display: block;
    width: 100%;
}

#menu ul.sub-menus a {
    color: #FFFFFF;
    font-size: 17px;
}

#menu li:hover ul.sub-menus {
    display: block;
}

#menu ul.sub-menus a:hover {
    background: rgba(242, 242, 242, 0.8);
    color: #007AFF;
}

@media screen and (max-width: 800px) {
    #menu {position:relative}
    #menu ul {
        background:rgba(17,17,17,0.95);
        position:absolute;
        top:100%;
        right:0;
        left:0;
        z-index:3;
        height:auto;
        display:none;
        -webkit-backdrop-filter: saturate(180%) blur(20px);
        backdrop-filter: saturate(180%) blur(20px);
    }
    #menu ul.sub-menus {width:100%;position:static;}
    #menu ul.sub-menus a {padding-left:30px;}
    #menu li {display:block;float:none;width:auto;}
    #menu input, #menu label {
        position: absolute;
        top: 0;
        left: 0;
        display: block;
        width: 40px;
        height: 40px;
        cursor: pointer;
        margin-top: 10px;
        margin-bottom: 10px;
    }
    #menu input {z-index:4}
    #menu input:checked + label {color:white}
    #menu input:checked + label:before {content:"\00d7"}
    #menu input:checked ~ ul {display:block}
}

.status-indicator {
    width: 12px;
    height: 12px;
    border-radius: 50%;
    display: inline-block;
    margin-left: 10px;
    transition: background-color 0.3s;
}

.status-online {
    background-color: #34C759;
    box-shadow: 0 0 5px #34C759;
}

.status-offline {
    background-color: #FF3B30;
    box-shadow: 0 0 5px #FF3B30;
}

.stat-container {
    display: -webkit-flex;
    display: flex;
    -webkit-justify-content: center;
    justify-content: center;
    -webkit-align-items: center;
    align-items: center;
    -webkit-flex-wrap: wrap;
    flex-wrap: wrap;
    margin: 5px 0px 15px 0px;
}

.stat-group {
    display: -webkit-flex;
    display: flex;
    -webkit-flex-direction: column;
    flex-direction: column;
    -webkit-align-items: flex-start;
    align-items: flex-start;
    padding: 15px 20px;
    background-color: rgba(51, 51, 51, 0.95);
    border: 1px solid #555;
    border-radius: 13px;
    -webkit-box-shadow: 0 2px 8px rgba(0, 0, 0, 0.12);
    box-shadow: 0 2px 8px rgba(0, 0, 0, 0.12);
    width: 90%;
    max-width: 300px;
    color: white;
    margin: 10px auto 5px auto;
    -webkit-backdrop-filter: saturate(180%) blur(20px);
    backdrop-filter: saturate(180%) blur(20px);
}

.stat-group strong {
    font-size: 17px;
    font-weight: 600;
    display: flex;
    align-items: center;
}

.form-group {
    display: flex;
    flex-direction: column;
    align-items: flex-start;
}

textarea.payload-input, .payload-container pre {
    width: 100%;
    overflow: auto;
    max-height: 80vh;
    height: 23em;
    resize: vertical;
    font-size: 17px;
    border-radius: 13px;
    display: block;
    appearance: none;
    -webkit-appearance: none;
    background-color: rgba(0, 0, 0, 0.95);
    color: white;
    font-family: -apple-system-monospace, monospace;
    border: 1px solid #555; 
    box-sizing: border-box;
    max-width: 100%;
    padding: 10px;
    white-space: pre-wrap;
    word-wrap: break-word;
    -webkit-overflow-scrolling: touch;
}

textarea {
    width: 100%;
    overflow: auto;
    max-height: 80vh;
    height: 23em;
    resize: vertical;
    font-size: 17px;
    border-radius: 13px;
    display: block;
    appearance: none;
    -webkit-appearance: none;
    background-color: rgba(0, 0, 0, 0.95);
    color: white;
    font-family: -apple-system-monospace, monospace;
    border: 1px solid #555; 
    box-sizing: border-box;
    max-width: 100%;
    padding: 10px;
    white-space: pre-wrap;
    word-wrap: break-word;
    -webkit-overflow-scrolling: touch;
}

.switch-container {
    display: -webkit-flex;
    display: flex;
    -webkit-justify-content: space-around;
    justify-content: space-around;
    -webkit-align-items: center;
    align-items: center;
    -webkit-flex-wrap: wrap;
    flex-wrap: wrap;
    padding: 0 10px;
    margin-top: 10px;
    margin-bottom: 5px;
}

.switch-group {
    display: -webkit-flex;
    display: flex;
    -webkit-align-items: center; 
    align-items: center;
    -webkit-justify-content: space-between;
    justify-content: space-between;
    padding: 15px 20px;
    background-color: rgba(51, 51, 51, 0.95);
    border: 1px solid #555;
    border-radius: 13px;
    -webkit-box-shadow: 0 2px 8px rgba(0, 0, 0, 0.12);
    box-shadow: 0 2px 8px rgba(0, 0, 0, 0.12);
    width: 90%;
    max-width: 264px;
    margin: 10px auto;
    text-align: center;
    -webkit-backdrop-filter: saturate(180%) blur(20px);
    backdrop-filter: saturate(180%) blur(20px);
}

.switch {
    position: relative;
    display: inline-block;
    /* width: 51px; */
    width: 60px;
    height: 31px;
    margin-top: 1px;
    margin-bottom: 1px;
} 

.switch-label {
    font-size: 17px;
    font-weight: 600;
    margin-left: 5px;
    color: white;
    /* display: inline-block; */
    white-space: nowrap;
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
    background-color: #E9E9EA;
    transition: .4s;
}

.slider:before {
    position: absolute;
    content: "";
    height: 27px;
    width: 27px;
    left: 2px;
    bottom: 2px;
    background-color: white;
    transition: .4s;
}

input:checked + .slider {
    background-color: #34C759;
}

input:checked + .slider:before {
    transform: translateX(29px);
}

.slider.round {
    border-radius: 31px;
}

.slider.round:before {
    border-radius: 50%;
}

/* Toast Notification Styles */
.toast-container {
    position: fixed;
    top: 20px;
    left: 50%;
    transform: translateX(-50%);
    z-index: 10000;
    width: 90%;
    max-width: 500px;
    pointer-events: none;
}

.toast-message {
    padding: 12px 20px;
    margin-bottom: 10px;
    border-radius: 4px;
    font-weight: bold;
    box-shadow: 0 3px 10px rgba(0, 0, 0, 0.2);
    animation: toastSlideIn 0.3s ease-out;
    pointer-events: auto;
    display: flex;
    justify-content: space-between;
    align-items: center;
}

.toast-message.error {
    background-color: #ffebee;
    border-left: 4px solid #f44336;
    color: #d32f2f;
}

.toast-message.success {
    background-color: #e8f5e9;
    border-left: 4px solid #4caf50;
    color: #2e7d32;
}

.toast-close {
    cursor: pointer;
    margin-left: 15px;
    font-size: 1.2em;
    color: inherit;
}

.payload-list-container {
    display: flex;
    flex-direction: column;
    gap: 10px;
    width: 100%;
    max-width: 600px;
    margin: 0 auto;
}

.payload-item {
    background-color: rgba(51, 51, 51, 0.95);
    border: 1px solid #555;
    border-radius: 13px;
    padding: 15px;
    -webkit-backdrop-filter: saturate(180%) blur(20px);
    backdrop-filter: saturate(180%) blur(20px);
}

.payload-desc {
    color: #aaa;
    font-size: 14px;
    margin-top: 5px;
    margin-bottom: 5px;
}

.payload-filename {
    color: #666;
    font-size: 12px;
    font-family: monospace;
}

@keyframes toastSlideIn {
    from { transform: translateY(-100px) translateX(-50%); opacity: 0; }
    to { transform: translateY(0) translateX(-50%); opacity: 1; }
}

@keyframes toastFadeOut {
    from { opacity: 1; }
    to { opacity: 0; }
}

.messages-container {
    margin-top: 10px;
    display: flex;
    flex-direction: column;
    justify-content: center;
    align-items: center;
}

label {
    font-size: 17px;
    margin-top: 10px;
    margin-bottom: 10px;
    margin-left: 5px;
    display: inline-block;
    font-weight: 600;
}

.message {
    /* v produkcii zapni */
    /* display: none; */
    color: #34C759;
    font-weight: 600;
    width: 100%;
    height: 36px;
    /* margin: 5px auto 5px auto; */
    margin: 5px 5px 5px 5px;
    background-color: rgba(52, 199, 89, 0.1);
    border: 1px solid #34C759;
    /* border-radius: 13px; */
    border-radius: 7px;
    -webkit-backdrop-filter: saturate(180%) blur(20px);
    backdrop-filter: saturate(180%) blur(20px);
    display: flex;
    align-items: center;
    justify-content: center;
}

.console {
    width: 99%;
    background-color: rgba(0, 0, 0, 0.95);
    color: white;
    font-family: -apple-system-monospace, monospace;
    padding: 5px;
    border: 0px;
    border-radius: 13px;
    -webkit-overflow-scrolling: touch;
}

.upload-progress {
    color: #007bff;
    margin: 10px 0;
    font-weight: bold;
}

.view-container {
    border: 1px solid #555;
    border-radius: 13px;
    padding: 15px 20px;
    padding: auto;
    max-width: 600px;
    margin: auto;
    background-color: rgba(51, 51, 51, 0.95);
    -webkit-backdrop-filter: saturate(180%) blur(20px);
    backdrop-filter: saturate(180%) blur(20px);
}

.config-container {
    margin: auto;
    border: 1px solid #555;
    width: 80%;
    max-width: 600px;
    padding: 20px;
    box-shadow: 0 2px 8px rgba(0, 0, 0, 0.12);
    border-radius: 13px;
    background-color: rgba(51, 51, 51, 0.95);
    -webkit-backdrop-filter: saturate(180%) blur(20px);
    backdrop-filter: saturate(180%) blur(20px);
}

/* Command table styles */
.command-table {
    width: 100%;
    border-collapse: collapse;
    margin: 20px 0;
    font-size: 0.9em;
    color: white;
}
.command-table th, .command-table td {
    padding: 8px 12px;
    border: 1px solid #555;
    text-align: left;
}
.command-table th {
    background-color: rgba(68, 68, 68, 0.95);
    font-weight: bold;
    color: white;
}
.command-table tr:nth-child(even) {
    background-color: rgba(51, 51, 51, 0.7);
}
.command-table tr:hover {
    background-color: rgba(85, 85, 85, 0.7);
}
.command-reference {
    margin: 20px 0;
    padding: 0;
    background-color: rgba(51, 51, 51, 0.95);
    border: 1px solid #555;
    border-radius: 13px;
    -webkit-backdrop-filter: saturate(180%) blur(20px);
    backdrop-filter: saturate(180%) blur(20px);
}
.command-reference-toggle {
    margin: 0;
    padding: 15px;
    color: white;
    font-size: 17px;
    font-weight: 600;
    cursor: pointer;
    user-select: none;
    -webkit-tap-highlight-color: transparent;
}
.command-reference-toggle:hover {
    color: #007AFF;
}
.command-table-container {
    padding: 0 15px 15px 15px;
}
.command-cell {
    cursor: pointer;
    transition: background-color 0.2s;
}
.command-cell:hover {
    background-color: rgba(0, 122, 255, 0.2) !important;
}
.keys-reference-toggle {
    margin: 0;
    padding: 15px;
    color: white;
    font-size: 17px;
    font-weight: 600;
    cursor: pointer;
    user-select: none;
    -webkit-tap-highlight-color: transparent;
}
.keys-reference-toggle:hover {
    color: #007AFF;
}
.keys-table-container {
    padding: 0 15px 15px 15px;
}
input[type="submit"], button {
    -webkit-appearance: none;
    appearance: none;
    color: white;
    background-color: #007AFF;
    width: 100%;
    height: 44px;
    font-size: 17px;
    border-radius: 13px;
    margin-top: 10px;
    margin-bottom: 10px;
    border: 1px solid white;
    font-weight: 600;
    -webkit-tap-highlight-color: transparent;
    touch-action: manipulation;
}
/* input[type="file"], input[type="submit"] { */
input[type="file"] {
    display: block;
    margin: 10px auto;
    font-size: 17px;
    /* width: 90%; */
    /* text-align: center; */
    width: fit-content;
}
button[name="deleteWifiButton"], button[name="deleteUSBButton"] {
    background-color: #FF3B30;
    color: white;
}
.button-container {
    display: flex;
    justify-content: flex-start;
    margin: 10px 0;
    width: 100%;
    box-sizing: border-box;
}
.copy-button {
    background-color: #73879F;
    color: white;
    border: none;
    padding: 12px 16px;
    border-radius: 13px;
    font-size: 14px;
    cursor: pointer;
    -webkit-tap-highlight-color: transparent;
    touch-action: manipulation;
    white-space: nowrap;
    max-width: 100%;
    box-sizing: border-box;
    overflow: hidden;
    text-overflow: ellipsis;
}
.copy-button:hover {
    background-color: #55616F;
}
#metadataForm {
    background-color: rgba(51, 51, 51, 0.95);
    border: 1px solid #555;
    border-radius: 13px;
    padding: 15px;
    margin: 15px 0;
    -webkit-backdrop-filter: saturate(180%) blur(20px);
    backdrop-filter: saturate(180%) blur(20px);
}
.button-container button {
    flex: 1;
}
.os-container {
    display: flex;
    flex-wrap: wrap;
    justify-content: space-around;
    margin: 20px 0;
}
.os-dropdown {
    position: relative;
    display: inline-block;
    width: 100%;
}
.os-dropdown-content {
    display: none;
    position: absolute;
    background-color: rgba(51, 51, 51, 0.95);
    border: 1px solid #555;
    border-radius: 13px;
    min-width: 160px;
    box-shadow: 0px 8px 16px 0px rgba(0,0,0,0.2);
    z-index: 1000; /* Increased z-index to ensure it's above all other elements */
}
.os-dropdown-content a {
    color: white;
    padding: 12px 16px;
    text-decoration: none;
    display: block;
    cursor: pointer;
}
.os-dropdown-content a:hover {
    background-color: rgba(0, 122, 255, 0.3);
}
.show-dropdown {
    display: block;
}
.os-box {
    width: 25%;
    min-height: 50px;
    border: 2px dashed #555;
    border-radius: 13px;
    margin: 10px 0;
    padding: 10px;
    background-color: rgba(51, 51, 51, 0.5);
    text-align: center;
    position: relative;
}
.os-box h3 {
    margin-top: 0;
    color: #007AFF;
}
.payload-item-os {
    position: relative;
}
.payload-item.dragging {
    opacity: 0.5;
}
.os-box.highlight {
    border-color: #007AFF;
    background-color: rgba(0, 122, 255, 0.1);
}
.payload-preview {
    background-color: rgba(0, 0, 0, 0.5);
    border-radius: 8px;
    padding: 8px;
    margin: 5px 0;
    word-break: break-word;
}
.button-container-os {
    display: flex;
    justify-content: space-between;
    margin: 20px 0;
}
#payloadList {
    max-height: 300px;
    overflow-y: auto;
    margin: 20px 0;
}
.note {
    background-color: rgba(255, 235, 59, 0.1);
    border-left: 4px solid #FFEB3B;
    padding: 10px;
    margin-top: 15px;
    border-radius: 0 4px 4px 0;
}
.clickable-os {
    cursor: pointer;
    text-decoration: underline;
    color: #007AFF; /* Blue color for normal state */
    transition: color 0.3s;
}
.clickable-os:hover {
    color: #0056b3;
}
@media (max-width: 600px) {
    .copy-button {
        font-size: 16px;
        padding: 14px 20px;
    }
}
input[type="text"], textarea {
    width: 95%;
    padding: 12px 15px;
    margin-top: 5px;
    margin-bottom: 15px;
    margin-left: auto;
    border-radius: 13px;
    -webkit-appearance: none;
    appearance: none;
    border: 1px solid white;
    color: black;
    font-size: 17px;
    touch-action: manipulation;
    background-color: #d7d7d7;
    font-family: -apple-system, BlinkMacSystemFont, "Segoe UI", Roboto, Helvetica, Arial, sans-serif;
}
textarea {
    min-height: 100px;
    resize: vertical;
}
select, input[type="text"], input[type="password"] {
    /* width: 89%; */
    width: 95%;
    padding: 12px 15px;
    margin-top: 5px;
    margin-bottom: 15px;
    margin-left: auto;
    border-radius: 13px;
    -webkit-appearance: none;
    appearance: none;
    border: 1px solid white;
    color: black;
    font-size: 17px;
    touch-action: manipulation;
    background-color: #d7d7d7;
}

a[class="pyaloadButton"] {
    background: #007AFF;
    color: white;
    min-width: 89%;
    display: inline-block;
    margin-top: 5px;
    margin-bottom: 10px;
    border-radius: 13px;
    transition: .2s;
    padding: 15px 15px;
    /* border: none; */
    border: 1px solid white;
    font-size: 17px;
    font-weight: 600;
    text-align: left;
    -webkit-tap-highlight-color: transparent;
    touch-action: manipulation;
}

select[name="layout-select"] {
    width: 99%;
    margin-top: 10px;
    margin-bottom: 15px;
    color: black;
}

@media screen and (orientation: landscape) {
    .stat-container {
        -webkit-flex-direction: row;
        flex-direction: row;
    }
}

@media screen and (orientation: portrait) {
    .stat-container {
        -webkit-flex-direction: column;
        flex-direction: column;
    }
}

/* @supports (padding: max(0px)) {
    body {
        padding-left: max(3px, env(safe-area-inset-left));
        padding-right: max(3px, env(safe-area-inset-right));
        padding-bottom: max(3px, env(safe-area-inset-bottom));
    }
} */
)=====";