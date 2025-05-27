const char Style[] PROGMEM = R"=====(
:root {
  --primary: #00f2ff;
  --secondary: #00ff88;
  --accent: #ff00aa;
  --dark: #0a0a12;
  --darker: #050508;
  --light: #f0f0ff; /* Changed from #e0e0ff */
  --success: #00ff88;
  --error: #ff0033;
  --warning: #ffaa00;
}

body {
  background-color: #000000; /* Pure black */
  color: var(--light);
  font-family: 'Courier New', monospace;
  margin: 0;
  padding: 0;
  overflow-x: hidden;
  line-height: 1.6;
}

/* Header/Navigation - Mobile First */
#menu {
  background: var(--darker);
  border-bottom: 1px solid var(--primary);
  box-shadow: 0 0 15px rgba(0, 242, 255, 0.3);
  padding: 10px 0;
  position: sticky;
  top: 0;
  z-index: 100;
}

#menu ul {
  display: none;
  flex-direction: column;
  padding: 0;
  margin: 0;
  list-style: none;
  width: 100%;
}

#menu li {
  margin: 5px 0;
  position: relative;
}

#menu a {
  color: var(--light);
  text-decoration: none;
  padding: 10px 15px;
  display: block;
  transition: all 0.3s;
  position: relative;
  touch-action: manipulation; /* Disable double-tap zoom */
}

#menu a:hover {
  color: var(--primary);
  background: rgba(0, 242, 255, 0.1);
  will-change: width; 
}

#menu a::after {
  content: '';
  position: absolute;
  bottom: 0;
  left: 50%;
  transform: translateX(-50%);
  width: 0;
  height: 2px;
  background: var(--primary);
  transition: width 0.3s;
}

#menu a:hover::after {
  width: 70%;
}

#menu .usb-icon {
  vertical-align: middle;
  margin-right: 5px;
}

/* Active menu item */
#menu a.active {
  color: var(--primary);
  font-weight: bold;
}

#menu a.active::after {
  width: 70%;
}

/* Hamburger menu - Mobile */
#responsive-menu {
  display: none;
}

#responsive-menu + label {
  display: block;
  cursor: pointer;
  padding: 25px 15px;
  position: absolute;
  top: 0;
  right: 0;
  z-index: 101;
}

#responsive-menu + label::before {
  content: "☰";
  font-size: 1.5em;
  color: var(--primary);
}

#responsive-menu:checked + label::before {
  content: "✕";
}

#responsive-menu:checked ~ ul {
  display: flex;
}

/* Main content */
.view-container {
  background: rgba(10, 10, 18, 0.8);
  border: 1px solid var(--primary);
  border-radius: 5px;
  padding: 15px;
  margin-top: 20px;
  margin: 15px auto;
  max-width: 900px;
  box-shadow: 0 0 20px rgba(0, 242, 255, 0.1);
  position: relative;
  overflow: hidden;
}

.view-container::before {
  content: "";
  position: absolute;
  top: 0;
  left: 0;
  width: 100%;
  height: 3px;
  background: linear-gradient(90deg, var(--primary), var(--secondary));
}

/* Buttons */
button, input[type="submit"], input[type="button"] {
  background: linear-gradient(135deg, var(--primary), var(--secondary));
  color: var(--darker);
  border: none;
  padding: 10px 20px;
  font-weight: bold;
  border-radius: 3px;
  cursor: pointer;
  transition: all 0.3s;
  text-transform: uppercase;
  letter-spacing: 1px;
  position: relative;
  overflow: hidden;
  font-family: 'Courier New', monospace;
  font-size: 14px;
  margin: 5px;
  min-height: 44px;
}

button:hover, input[type="submit"]:hover, input[type="button"]:hover {
  transform: translateY(-2px);
  box-shadow: 0 5px 15px rgba(0, 242, 255, 0.4);
}

button::after {
  content: "";
  position: absolute;
  top: 0;
  left: -100%;
  width: 100%;
  height: 100%;
  background: linear-gradient(90deg, transparent, rgba(255, 255, 255, 0.2), transparent);
  transition: 0.5s;
}

button:hover::after {
  left: 100%;
}

button[name="deleteWifiButton"], 
button[name="deleteUSBButton"],
button[name="deleteBackupWifiButton"],
button[name="clearCacheButton"] {
  background: linear-gradient(135deg, var(--error), #cc0022);
}

/* Input fields - Improved for mobile */
/* Form inputs - one line height */
input:not([type="checkbox"]):not([type="file"]),
select {
  height: 36px; /* Fixed height for single-line inputs */
  padding: 8px 12px;
  line-height: 1.2;
}

textarea {
  min-height: 100px; /* Keep textareas taller */
  resize: none;
}

* {
  -webkit-tap-highlight-color: transparent;
}

.single-line-input {
  height: 36px !important;
  line-height: 36px !important;
  padding: 0 12px !important;
}

input, textarea, select {
  background: rgba(0, 0, 0, 0.3);
  border: 1px solid var(--primary);
  color: var(--light);
  padding: 8px 12px;
  border-radius: 3px;
  width: 100%;
  margin-bottom: 12px;
  font-family: 'Courier New', monospace;
  transition: all 0.3s;
  box-sizing: border-box;
  font-size: 16px !important;
}

input:focus, textarea:focus, select:focus {
  outline: none;
  border-color: var(--secondary);
  box-shadow: 0 0 10px rgba(0, 255, 136, 0.3);
}*/

input[type="checkbox"] {
  width: auto;
  margin-right: 10px;
}

/* Form groups - Improved spacing */
.form-group {
  margin-bottom: 15px;
}

.form-group label {
  display: block;
  margin-bottom: 6px;
  color: var(--primary);
  font-weight: bold;
}

/* Payload list */
.payload-list-container {
  display: grid;
  grid-template-columns: repeat(auto-fill, minmax(280px, 1fr));
  gap: 12px;
  margin: 15px 0;
}

.payload-item, .payload-item-os {
  border: 1px solid var(--primary);
  border-radius: 3px;
  padding: 12px;
  transition: all 0.3s;
  position: relative;
  background-color: rgba(10, 10, 10, 0.9); /* Darker than before */
}

.payload-item:hover, .payload-item-os:hover {
  transform: translateY(-5px);
  box-shadow: 0 10px 20px rgba(0, 242, 255, 0.2);
  border-color: var(--secondary);
}

.payload-item::before, .payload-item-os::before {
  content: "";
  position: absolute;
  top: 0;
  left: 0;
  width: 3px;
  height: 100%;
  background: linear-gradient(to bottom, var(--primary), var(--secondary));
}

.payload-desc {
  color: #aaa;
  font-size: 13px;
  margin: 6px 0;
}

.payload-filename {
  color: #666;
  font-size: 11px;
  font-family: monospace;
}

/* Terminal style elements */
.terminal-style, .payload-container pre, textarea.payload-input {
  border: 1px solid var(--primary);
  color: var(--secondary);
  padding: 12px;
  border-radius: 3px;
  font-family: 'Courier New', monospace;
  position: relative;
  width: 100%;
  overflow: auto;
  max-height: 80vh;
  height: 20em;
  resize: vertical;
  white-space: pre-wrap;
  word-wrap: break-word;
  box-sizing: border-box;
  background-color: rgba(10, 10, 10, 0.9); /* Darker than before */
}

/* Toast notifications */
.toast-container {
  position: fixed;
  top: 60px; /* Below the menu */
  right: 20px;
  z-index: 1000;
  max-width: 350px;
}

.toast-message {
  position: relative;
  padding: 15px;
  margin-bottom: 10px;
  border-radius: 4px;
  color: white;
  box-shadow: 0 3px 10px rgba(0, 0, 0, 0.2);
  animation: toastSlideIn 0.3s ease-out;
  pointer-events: auto;
  display: flex;
  justify-content: space-between;
  align-items: center;
  background-color: var(--darker);
  border-left: 4px solid;
}

.toast-message.success {
  border-left-color: var(--success);
  color: var(--success);
}

.toast-message.error {
  border-left-color: var(--error);
  color: var(--error);
}

.toast-message.warning {
  border-left-color: var(--warning);
  color: var(--warning);
}

.toast-close {
  cursor: pointer;
  margin-left: 12px;
  font-size: 1.1em;
  color: inherit;
}

@keyframes toastFadeIn {
    from { opacity: 0; transform: translateY(20px); }
    to { opacity: 1; transform: translateY(0); }
}

@keyframes toastFadeOut {
    from { opacity: 1; transform: translateY(0); }
    to { opacity: 0; transform: translateY(20px); }
}

/* OS boxes for autoexec - Mobile optimized */
.os-container {
  display: flex;
  flex-wrap: wrap;
  justify-content: space-between;
  margin: 15px 0;
  gap: 12px;
}

.os-box {
  flex: 1 1 150px;
  min-height: 120px;
  border: 2px dashed var(--primary);
  border-radius: 5px;
  padding: 12px;
  background-color: rgba(51, 51, 51, 0.3);
  text-align: center;
  position: relative;
  transition: all 0.3s;
}

.os-box.highlight {
  border-color: var(--secondary);
  background-color: rgba(0, 122, 255, 0.1);
}

.os-box h3 {
  margin-top: 0;
  color: var(--primary);
  border-bottom: 1px solid var(--primary);
  padding-bottom: 5px;
  font-size: 16px;
}

.payload-preview {
  background-color: rgba(0, 0, 0, 0.5);
  border-radius: 5px;
  padding: 8px;
  margin: 8px 0;
  word-break: break-word;
  font-size: 13px;
}

.payload-item-os {
  position: relative;
  z-index: 1;
  margin-bottom: 5px; /* Ensure spacing between items */
}

/* Command tables */
.command-table {
  width: 100%;
  border-collapse: collapse;
  margin: 15px 0;
  font-size: 0.85em;
  color: var(--light);
}

.command-table th, .command-table td {
  padding: 6px 10px;
  border: 1px solid var(--primary);
  text-align: left;
}

.command-table th {
  background-color: rgba(68, 68, 68, 0.5);
  font-weight: bold;
  color: var(--primary);
}

.command-table tr:nth-child(even) {
  background-color: rgba(51, 51, 51, 0.3);
}

.command-table tr:hover {
  background-color: rgba(85, 85, 85, 0.3);
}

.command-cell {
  cursor: pointer;
  transition: background-color 0.2s;
}

.command-cell:hover {
  background-color: rgba(0, 122, 255, 0.2) !important;
}

/* Special cable wind elements */
.cable-wind-logo {
  text-align: center;
  margin: 25px 0 15px 0; /* Increased top margin */
  font-size: 2em;
  text-transform: uppercase;
  letter-spacing: 2px;
  color: var(--primary);
  text-shadow: 0 0 8px var(--primary);
  position: relative;
  z-index: 1;
}

.clickable-os {
  cursor: pointer;
  color: var(--primary);
  text-decoration: underline;
  transition: color 0.3s;
}

.clickable-os:hover {
  color: var(--secondary);
}

/* Button containers */
.button-container, .button-container-os {
  display: flex;
  flex-wrap: wrap;
  justify-content: center;
  gap: 8px;
  margin: 15px 0;
}

.button-container button, 
.button-container-os button {
  flex: none;
  min-width: 100px;
  background: linear-gradient(135deg, var(--primary), var(--secondary));
  color: var(--darker);
  border: none;
  padding: 10px 20px;
  font-weight: bold;
  border-radius: 3px;
  cursor: pointer;
  transition: all 0.3s;
  min-width: 100px; /* Fixed minimum width */
  width: 100px; /* Fixed width */
  text-transform: uppercase;
  letter-spacing: 1px;
  position: relative;
  overflow: hidden;
  font-family: 'Courier New', monospace;
  font-size: 14px;
  margin: 5px;
  min-height: 44px;
  white-space: nowrap; /* Prevent text wrapping */
  margin-left: auto; /* Push to the right */
  flex-shrink: 0; /* Prevent shrinking */
}

.select-os-btn {
  /* Fixed dimensions */
  width: 120px; /* Slightly wider to accommodate text */
  height: 44px; /* Fixed height */
  min-width: 120px; /* Minimum width */
  max-width: 120px; /* Maximum width to prevent growing */
  /* Visual styling */
  background: linear-gradient(135deg, var(--primary), var(--secondary));
  color: var(--darker);
  border: none;
  padding: 10px 5px; /* Adjusted padding */
  font-weight: bold;
  border-radius: 3px;
  cursor: pointer;
  transition: all 0.3s;
  /* Text styling */
  text-transform: uppercase;
  letter-spacing: 1px;
  font-family: 'Courier New', monospace;
  font-size: 10px;
  white-space: nowrap;
  text-overflow: ellipsis;
  overflow: hidden;
  /* Positioning */
  position: relative;
  margin: 5px;
  flex-shrink: 0; /* Prevent shrinking in flex containers */
  display: inline-flex;
  align-items: center;
  justify-content: center;
}

.button-container button:hover, 
.button-container-os button:hover,
.select-os-btn:hover {
  transform: translateY(-2px);
  box-shadow: 0 5px 15px rgba(0, 242, 255, 0.4);
}


.button-container button::after, 
.button-container-os button::after,
.select-os-btn::after {
  content: "";
  position: absolute;
  top: 0;
  left: -100%;
  width: 100%;
  height: 100%;
  background: linear-gradient(90deg, transparent, rgba(255, 255, 255, 0.2), transparent);
  transition: 0.5s;
}

.button-container button:hover::after, 
.button-container-os button:hover::after,
.select-os-btn:hover::after {
  left: 100%;
}

/* HR styling */
hr {
  border: none;
  height: 1px;
  background: linear-gradient(90deg, transparent, var(--primary), transparent);
  margin: 15px 0;
}

/* Stat containers */
.stat-container {
  display: flex;
  flex-wrap: wrap;
  justify-content: center;
  gap: 12px;
  margin: 15px 0;
}

.stat-group {
  flex: 1 1 180px;
  border: 1px solid var(--primary);
  border-radius: 5px;
  padding: 12px;
  min-width: 0;
  background-color: rgba(10, 10, 10, 0.9); /* Darker than before */
}

/* Form containers */
.config-container {
  background: rgba(51, 51, 51, 0.3);
  border: 1px solid var(--primary);
  border-radius: 5px;
  padding: 15px;
  margin: 15px auto;
  max-width: 600px;
}

/* Metadata form */
#metadataForm {
  background: rgba(51, 51, 51, 0.5);
  border: 1px solid var(--primary);
  border-radius: 5px;
  padding: 15px;
  margin: 15px 0;
  display: none;
}

/* Payload buttons */
a.pyaloadButton {
  background: linear-gradient(135deg, var(--primary), var(--secondary));
  color: var(--darker);
  border: none;
  padding: 10px 12px;
  font-weight: bold;
  border-radius: 3px;
  display: block;
  text-align: center;
  text-decoration: none;
  margin: 8px 0;
  transition: all 0.3s;
}

a.pyaloadButton:hover {
  transform: translateY(-2px);
  box-shadow: 0 5px 15px rgba(0, 242, 255, 0.4);
}

/* Responsive design */

@media screen and (-webkit-min-device-pixel-ratio:0) {
  select:focus,
  textarea:focus,
  input:focus {
    font-size: 16px !important;
  }
}

@media (min-width: 768px) {
  /* Show regular menu on desktop */
  #menu ul {
    display: flex;
    flex-direction: row;
    justify-content: center;
    flex-wrap: wrap;
  }
  
  #menu li {
    margin: 0 15px;
  }
  
  /* Hide hamburger menu on desktop */
  #responsive-menu + label {
    display: none;
  }
  
  /* Adjust container sizes for desktop */
  .view-container {
    padding: 20px;
    margin: 20px auto;
  }
  
  .os-box {
    flex: 1 1 200px;
    min-height: 150px;
  }
  
  .terminal-style, textarea.payload-input {
    height: 23em;
  }
  
  .cable-wind-logo {
    font-size: 2.5em;
  }
}

/* OS Selection Modal */
.os-modal {
    position: fixed;
    top: 0;
    left: 0;
    width: 100%;
    height: 100%;
    background-color: rgba(0, 0, 0, 0.8);
    display: flex;
    justify-content: center;
    align-items: center;
    z-index: 10000;
}

.os-modal-content {
    background: var(--darker);
    border: 2px solid var(--primary);
    border-radius: 8px;
    padding: 20px;
    max-width: 90%;
    width: 400px;
    box-shadow: 0 0 20px rgba(0, 242, 255, 0.5);
}

.os-modal h3 {
    color: var(--primary);
    margin-top: 0;
    text-align: center;
}

.os-options {
    display: grid;
    grid-template-columns: 1fr 1fr;
    gap: 10px;
    margin: 20px 0;
}

.os-options button {
    background: linear-gradient(135deg, var(--primary), var(--secondary));
    color: var(--darker);
    border: none;
    padding: 12px;
    border-radius: 4px;
    font-weight: bold;
    cursor: pointer;
    transition: all 0.3s;
}

.os-options button:hover {
    transform: translateY(-2px);
    box-shadow: 0 5px 15px rgba(0, 242, 255, 0.4);
}

.close-modal {
    background: var(--error) !important;
    width: 100%;
    margin-top: 10px;
}

.payload-header {
    display: flex;
    justify-content: space-between;
    align-items: center;
    margin-bottom: 8px;
    gap: 10px;
}

.payload-header strong {
    flex-grow: 1; /* Allow the text to take remaining space */
    overflow: hidden;
    text-overflow: ellipsis;
    white-space: nowrap;
}

.select-os-btn:hover {
    background: linear-gradient(135deg, #55616F, #73879F);
}

/* Add this to your style.css */
.status-indicator {
    display: inline-block;
    width: 12px;
    height: 12px;
    border-radius: 50%;
    margin-left: 8px;
    vertical-align: middle;
}

.status-online {
    background-color: var(--success);
    box-shadow: 0 0 8px var(--success);
}

.status-offline {
    background-color: var(--error);
    box-shadow: 0 0 8px var(--error);
}

/* Page title indicators */
.cable-wind-logo::after {
    content: '';
    display: inline-block;
    width: 10px;
    height: 10px;
    border-radius: 50%;
    margin-left: 12px;
    vertical-align: middle;
}

.cable-wind-logo.online::after {
    background-color: var(--success);
    box-shadow: 0 0 6px var(--success);
}

.cable-wind-logo.offline::after {
    background-color: var(--error);
    box-shadow: 0 0 6px var(--error);
}

@media (max-width: 480px) {
    .select-os-btn {
        width: 80px;
        min-width: 80px;
        padding: 6px 8px;
        font-size: 12px;
    }
}
  
  .payload-list-container {
    display: grid;
    grid-template-columns: repeat(auto-fill, minmax(280px, 1fr));
    gap: 15px; /* Increased gap */
    margin: 15px 0;
    position: relative;
    z-index: 1;
}
  
  .button-container button, .button-container-os button {
    width: 100%;
  }
  
  .stat-group {
    flex: 1 1 100%;
  }
}
)=====";
