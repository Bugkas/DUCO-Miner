#ifndef WEBPAGE_H
#define WEBPAGE_H

#include <pgmspace.h>

const char DASHBOARD_HTML[] PROGMEM = R"rawliteral(
<!DOCTYPE html>
<html lang="en">
<head>
    <meta charset="UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <title>Duino-Coin Solo | Quantum Amber Core Miner</title>
    <link href="https://fonts.googleapis.com/css2?family=Inter:wght@300;400;600&family=Orbitron:wght@400;700;900&family=Fira+Code:wght@400;500&display=swap" rel="stylesheet">
    <style>
        :root {
            --bg-base: #07040d;
            --bg-panel: rgba(255, 255, 255, 0.025);
            --border-glow: rgba(255, 170, 0, 0.18);
            --primary: #ffaa00;
            --primary-glow: rgba(255, 170, 0, 0.5);
            --secondary: #ab00ff;
            --secondary-glow: rgba(171, 0, 255, 0.5);
            --success: #39ff14;
            --success-glow: rgba(57, 255, 20, 0.6);
            --warning: #ff3300;
            --text-main: #fdfbf7;
            --text-muted: #9d94b0;
        }

        * {
            box-sizing: border-box;
            margin: 0;
            padding: 0;
            user-select: none;
        }

        body {
            background-color: var(--bg-base);
            color: var(--text-main);
            font-family: 'Inter', sans-serif;
            min-height: 100vh;
            overflow-x: hidden;
            background-image: 
                radial-gradient(circle at 10% 20%, rgba(255, 170, 0, 0.08) 0%, transparent 40%),
                radial-gradient(circle at 90% 80%, rgba(171, 0, 255, 0.08) 0%, transparent 40%);
            display: flex;
            flex-direction: column;
        }

        .grid-bg {
            position: fixed;
            top: 0;
            left: 0;
            width: 100vw;
            height: 100vh;
            background-image: radial-gradient(rgba(255, 255, 255, 0.01) 1px, transparent 1px);
            background-size: 30px 30px;
            pointer-events: none;
            z-index: 0;
        }

        header {
            position: relative;
            z-index: 10;
            padding: 24px;
            display: flex;
            justify-content: space-between;
            align-items: center;
            border-bottom: 1px solid rgba(255, 255, 255, 0.04);
            background: rgba(7, 4, 13, 0.7);
            backdrop-filter: blur(10px);
        }

        .logo-container {
            display: flex;
            align-items: center;
            gap: 15px;
        }

        .duco-logo {
            width: 38px;
            height: 38px;
            background: linear-gradient(135deg, var(--primary), var(--secondary));
            border-radius: 50%;
            display: flex;
            align-items: center;
            justify-content: center;
            font-family: 'Orbitron', sans-serif;
            font-weight: 900;
            font-size: 1.5rem;
            color: var(--bg-base);
            box-shadow: 0 0 20px var(--primary-glow);
            animation: spin-logo 20s linear infinite;
        }

        @keyframes spin-logo {
            100% { transform: rotate(360deg); }
        }

        .logo-text h1 {
            font-family: 'Orbitron', sans-serif;
            font-size: 1.4rem;
            font-weight: 700;
            letter-spacing: 2px;
            background: linear-gradient(to right, var(--text-main), var(--primary));
            -webkit-background-clip: text;
            -webkit-text-fill-color: transparent;
        }

        .logo-text p {
            font-size: 0.7rem;
            color: var(--text-muted);
            letter-spacing: 4px;
            text-transform: uppercase;
        }

        .badge {
            background: rgba(255, 170, 0, 0.1);
            border: 1px solid var(--primary);
            color: var(--primary);
            padding: 6px 12px;
            border-radius: 20px;
            font-size: 0.75rem;
            font-family: 'Orbitron', sans-serif;
            font-weight: 700;
            letter-spacing: 1px;
            box-shadow: 0 0 10px rgba(255, 170, 0, 0.2);
            display: flex;
            align-items: center;
            gap: 8px;
        }

        .badge.active {
            border-color: var(--success);
            color: var(--success);
            background: rgba(57, 255, 20, 0.05);
            box-shadow: 0 0 10px rgba(57, 255, 20, 0.15);
        }

        .badge-dot {
            width: 8px;
            height: 8px;
            border-radius: 50%;
            background-color: currentColor;
            box-shadow: 0 0 8px currentColor;
            animation: pulse-dot 1.5s infinite;
        }

        @keyframes pulse-dot {
            0%, 100% { opacity: 0.3; }
            50% { opacity: 1; }
        }

        main {
            position: relative;
            z-index: 10;
            flex: 1;
            padding: 30px;
            max-width: 1400px;
            width: 100%;
            margin: 0 auto;
            display: flex;
            flex-direction: column;
            gap: 30px;
        }

        .kpi-container {
            display: grid;
            grid-template-columns: repeat(auto-fit, minmax(240px, 1fr));
            gap: 20px;
        }

        .kpi-card {
            background: var(--bg-panel);
            border: 1px solid rgba(255, 255, 255, 0.03);
            border-radius: 16px;
            padding: 24px;
            backdrop-filter: blur(12px);
            box-shadow: inset 0 0 12px rgba(255, 255, 255, 0.01);
            transition: all 0.3s ease;
            position: relative;
            overflow: hidden;
        }

        .kpi-card::before {
            content: '';
            position: absolute;
            top: 0;
            left: 0;
            width: 100%;
            height: 3px;
            background: linear-gradient(90deg, transparent, var(--primary), transparent);
            transform: translateX(-100%);
            transition: transform 0.5s ease;
        }

        .kpi-card:hover {
            transform: translateY(-4px);
            border-color: var(--border-glow);
            box-shadow: 0 10px 25px rgba(0, 0, 0, 0.4), inset 0 0 12px rgba(255, 170, 0, 0.05);
        }

        .kpi-card:hover::before {
            transform: translateX(100%);
        }

        .kpi-card.sec-glow:hover {
            border-color: rgba(171, 0, 255, 0.2);
            box-shadow: 0 10px 25px rgba(0, 0, 0, 0.4), inset 0 0 12px rgba(171, 0, 255, 0.05);
        }
        .kpi-card.sec-glow::before {
            background: linear-gradient(90deg, transparent, var(--secondary), transparent);
        }

        .kpi-card.success-glow:hover {
            border-color: rgba(57, 255, 20, 0.2);
            box-shadow: 0 10px 25px rgba(0, 0, 0, 0.4), inset 0 0 12px rgba(57, 255, 20, 0.05);
        }
        .kpi-card.success-glow::before {
            background: linear-gradient(90deg, transparent, var(--success), transparent);
        }

        .kpi-label {
            font-size: 0.75rem;
            color: var(--text-muted);
            text-transform: uppercase;
            letter-spacing: 2px;
            font-weight: 600;
            display: flex;
            justify-content: space-between;
            align-items: center;
        }

        .kpi-value {
            font-family: 'Orbitron', sans-serif;
            font-size: 2rem;
            font-weight: 700;
            margin-top: 12px;
            color: var(--text-main);
            letter-spacing: 1px;
            display: flex;
            align-items: baseline;
            gap: 5px;
        }

        .kpi-unit {
            font-size: 0.9rem;
            color: var(--text-muted);
            font-weight: 400;
            font-family: 'Inter', sans-serif;
        }

        .kpi-subtext {
            font-size: 0.7rem;
            color: var(--text-muted);
            margin-top: 8px;
            display: flex;
            align-items: center;
            gap: 5px;
        }

        .kpi-subtext .success { color: var(--success); }
        .kpi-subtext .gold { color: var(--primary); }

        .workspace-grid {
            display: grid;
            grid-template-columns: 1.6fr 1fr;
            gap: 30px;
        }

        @media (max-width: 968px) {
            .workspace-grid {
                grid-template-columns: 1fr;
            }
        }

        .panel {
            background: var(--bg-panel);
            border: 1px solid rgba(255, 255, 255, 0.03);
            border-radius: 16px;
            backdrop-filter: blur(12px);
            box-shadow: inset 0 0 12px rgba(255, 255, 255, 0.01);
            display: flex;
            flex-direction: column;
            overflow: hidden;
        }

        .panel-header {
            padding: 20px 24px;
            border-bottom: 1px solid rgba(255, 255, 255, 0.04);
            display: flex;
            justify-content: space-between;
            align-items: center;
            background: rgba(255, 255, 255, 0.005);
        }

        .panel-title {
            font-family: 'Orbitron', sans-serif;
            font-size: 0.95rem;
            font-weight: 700;
            letter-spacing: 1.5px;
            text-transform: uppercase;
            display: flex;
            align-items: center;
            gap: 10px;
        }

        .panel-title-icon {
            width: 6px;
            height: 14px;
            background: var(--primary);
            border-radius: 2px;
            box-shadow: 0 0 8px var(--primary-glow);
        }

        .panel-title-icon.secondary {
            background: var(--secondary);
            box-shadow: 0 0 8px var(--secondary-glow);
        }

        .panel-content {
            padding: 24px;
            flex: 1;
        }

        .canvas-container {
            position: relative;
            width: 100%;
            height: 200px;
            background: rgba(0, 0, 0, 0.25);
            border: 1px solid rgba(255, 255, 255, 0.02);
            border-radius: 10px;
            margin-bottom: 24px;
            overflow: hidden;
        }

        canvas {
            display: block;
            width: 100%;
            height: 100%;
        }

        .terminal-container {
            background: rgba(4, 2, 8, 0.92);
            border: 1px solid rgba(255, 255, 255, 0.02);
            border-radius: 10px;
            padding: 16px;
            font-family: 'Fira Code', monospace;
            font-size: 0.8rem;
            height: 250px;
            overflow-y: auto;
            box-shadow: inset 0 0 10px rgba(0, 0, 0, 0.85);
            display: flex;
            flex-direction: column;
            gap: 6px;
            scroll-behavior: smooth;
        }

        .terminal-row {
            line-height: 1.4;
            display: flex;
            gap: 8px;
        }

        .term-time {
            color: var(--text-muted);
            flex-shrink: 0;
        }

        .term-tag {
            color: var(--primary);
            font-weight: 600;
            flex-shrink: 0;
        }

        .term-tag.sys { color: var(--secondary); }
        .term-tag.ok { color: var(--success); }
        .term-tag.err { color: var(--warning); }

        .term-msg {
            color: var(--text-main);
            word-break: break-all;
        }

        .settings-form {
            display: flex;
            flex-direction: column;
            gap: 16px;
        }

        .input-row {
            display: grid;
            grid-template-columns: 2fr 1fr;
            gap: 12px;
        }

        .input-group {
            display: flex;
            flex-direction: column;
            gap: 6px;
        }

        .input-label {
            font-size: 0.75rem;
            color: var(--text-muted);
            text-transform: uppercase;
            letter-spacing: 1.5px;
            font-weight: 600;
        }

        .input-wrapper {
            position: relative;
        }

        .form-input {
            width: 100%;
            background: rgba(0, 0, 0, 0.3);
            border: 1px solid rgba(255, 255, 255, 0.06);
            border-radius: 8px;
            padding: 10px 14px;
            color: var(--text-main);
            font-family: 'Inter', sans-serif;
            font-size: 0.9rem;
            transition: all 0.3s ease;
        }

        .form-input:focus {
            outline: none;
            border-color: var(--primary);
            box-shadow: 0 0 10px rgba(255, 170, 0, 0.25), inset 0 0 5px rgba(255, 170, 0, 0.1);
            background: rgba(0, 0, 0, 0.4);
        }

        select.form-input {
            cursor: pointer;
            appearance: none;
        }

        .sys-info-table {
            width: 100%;
            border-collapse: collapse;
            font-size: 0.85rem;
            margin-top: 10px;
        }

        .sys-info-table tr {
            border-bottom: 1px solid rgba(255, 255, 255, 0.02);
        }

        .sys-info-table tr:last-child {
            border-bottom: none;
        }

        .sys-info-table td {
            padding: 10px 0;
            color: var(--text-main);
        }

        .sys-info-table td:first-child {
            color: var(--text-muted);
            font-weight: 500;
        }

        .sys-info-table td:last-child {
            text-align: right;
            font-family: 'Fira Code', monospace;
            font-weight: 600;
        }

        .btn-glow {
            background: linear-gradient(135deg, var(--primary), rgba(255, 170, 0, 0.6));
            border: none;
            color: var(--bg-base);
            font-family: 'Orbitron', sans-serif;
            font-weight: 900;
            font-size: 0.85rem;
            letter-spacing: 2px;
            padding: 14px 20px;
            border-radius: 8px;
            cursor: pointer;
            text-transform: uppercase;
            box-shadow: 0 4px 15px rgba(255, 170, 0, 0.3), 0 0 0 1px var(--primary);
            transition: all 0.3s ease;
            display: flex;
            justify-content: center;
            align-items: center;
            gap: 10px;
            margin-top: 10px;
        }

        .btn-glow:hover {
            transform: translateY(-2px);
            box-shadow: 0 6px 20px rgba(255, 170, 0, 0.5), 0 0 15px var(--primary-glow);
            filter: brightness(1.1);
        }

        .btn-glow:active {
            transform: translateY(0);
        }

        .btn-glow:disabled {
            background: rgba(255, 255, 255, 0.05);
            color: var(--text-muted);
            box-shadow: none;
            cursor: not-allowed;
            transform: none;
            filter: none;
        }

        .btn-reboot {
            background: rgba(255, 255, 255, 0.02);
            border: 1px solid rgba(255, 255, 255, 0.05);
            color: var(--text-main);
            font-family: 'Orbitron', sans-serif;
            font-weight: 700;
            font-size: 0.75rem;
            letter-spacing: 1px;
            padding: 6px 12px;
            border-radius: 6px;
            cursor: pointer;
            transition: all 0.2s ease;
        }

        .btn-reboot:hover {
            background: rgba(255, 51, 0, 0.1);
            border-color: var(--warning);
            color: #ff3300;
            box-shadow: 0 0 8px rgba(255, 51, 0, 0.2);
        }

        ::-webkit-scrollbar {
            width: 6px;
            height: 6px;
        }
        ::-webkit-scrollbar-track {
            background: rgba(0, 0, 0, 0.1);
        }
        ::-webkit-scrollbar-thumb {
            background: rgba(255, 255, 255, 0.1);
            border-radius: 3px;
        }
        ::-webkit-scrollbar-thumb:hover {
            background: var(--primary-glow);
        }

        footer {
            z-index: 10;
            text-align: center;
            padding: 30px;
            font-size: 0.75rem;
            color: var(--text-muted);
            letter-spacing: 1px;
            border-top: 1px solid rgba(255, 255, 255, 0.03);
            margin-top: auto;
        }

        footer span {
            color: var(--primary);
            font-weight: 600;
        }
        
        .toast-notification {
            position: fixed;
            bottom: 30px;
            right: 30px;
            background: rgba(7, 4, 13, 0.95);
            border: 1px solid var(--primary);
            border-radius: 8px;
            padding: 16px 24px;
            box-shadow: 0 10px 30px rgba(0,0,0,0.5), 0 0 15px var(--primary-glow);
            backdrop-filter: blur(10px);
            display: flex;
            align-items: center;
            gap: 15px;
            z-index: 100;
            transform: translateY(150px);
            opacity: 0;
            transition: all 0.4s cubic-bezier(0.175, 0.885, 0.32, 1.275);
        }
        
        .toast-notification.show {
            transform: translateY(0);
            opacity: 1;
        }
        
        .toast-icon {
            color: var(--success);
            font-size: 1.2rem;
            font-family: 'Orbitron', sans-serif;
        }
        
        .toast-text h4 {
            font-size: 0.85rem;
            font-family: 'Orbitron', sans-serif;
            font-weight: 700;
            letter-spacing: 1px;
        }
        
        .toast-text p {
            font-size: 0.75rem;
            color: var(--text-muted);
            margin-top: 2px;
        }
    </style>
</head>
<body>
    <div class="grid-bg"></div>

    <header>
        <div class="logo-container">
            <div class="duco-logo">Ð</div>
            <div class="logo-text">
                <h1>Duino-Coin</h1>
                <p>Quantum Amber Core Miner</p>
            </div>
        </div>
        <div class="badge active" id="miner-badge">
            <div class="badge-dot"></div>
            <span id="badge-mode-text">DUCO SYNCING</span>
        </div>
    </header>

    <main>
        <!-- KPI Dashboard Section -->
        <section class="kpi-container">
            <div class="kpi-card">
                <div class="kpi-label">
                    <span>Hash Rate</span>
                    <svg width="12" height="12" viewBox="0 0 24 24" fill="none" stroke="var(--primary)" stroke-width="2"><path d="M13 2L3 14h9l-1 8 10-12h-9l1-8z"/></svg>
                </div>
                <div class="kpi-value" id="stat-hashrate">0.00 <span class="kpi-unit">H/s</span></div>
                <div class="kpi-subtext">
                    <span class="gold" id="subtext-engine">SHA-1 Optimization Core</span>
                </div>
            </div>

            <div class="kpi-card sec-glow">
                <div class="kpi-label">
                    <span>Target Difficulty</span>
                    <svg width="12" height="12" viewBox="0 0 24 24" fill="none" stroke="var(--secondary)" stroke-width="2"><path d="M22 11.08V12a10 10 0 1 1-5.93-9.14"/><polyline points="22 4 12 14.01 9 11.01"/></svg>
                </div>
                <div class="kpi-value" id="stat-difficulty">-</div>
                <div class="kpi-subtext">
                    <span>Allocated by Duco Pool</span>
                </div>
            </div>

            <div class="kpi-card success-glow">
                <div class="kpi-label">
                    <span>Shares (Accepted)</span>
                    <svg width="12" height="12" viewBox="0 0 24 24" fill="none" stroke="var(--success)" stroke-width="2"><rect x="2" y="7" width="20" height="14" rx="2" ry="2"/><path d="M16 21V5a2 2 0 0 0-2-2h-4a2 2 0 0 0-2 2v16"/></svg>
                </div>
                <div class="kpi-value" id="stat-accepted">0 <span class="kpi-unit" id="stat-efficiency">(100% ok)</span></div>
                <div class="kpi-subtext">
                    <span class="success" id="subtext-shares">0 shares accepted by pool</span>
                </div>
            </div>

            <div class="kpi-card success-glow">
                <div class="kpi-label">
                    <span>Wallet & Rewards</span>
                    <svg width="12" height="12" viewBox="0 0 24 24" fill="none" stroke="var(--primary)" stroke-width="2"><circle cx="12" cy="12" r="10"/><path d="M16 8h-6a2 2 0 0 0-2 2v4a2 2 0 0 0 2 2h6"/><rect x="12" y="10" width="8" height="4" rx="1"/></svg>
                </div>
                <div class="kpi-value" id="stat-balance">0.0000 <span class="kpi-unit">DUCO</span></div>
                <div class="kpi-subtext">
                    <span id="subtext-balance">Session Earned: <span class="success">+0.000000 DUCO</span></span>
                </div>
            </div>

            <div class="kpi-card sec-glow">
                <div class="kpi-label">
                    <span>Uptime / Latency</span>
                    <svg width="12" height="12" viewBox="0 0 24 24" fill="none" stroke="var(--text-muted)" stroke-width="2"><circle cx="12" cy="12" r="10"/><polyline points="12 6 12 12 16 14"/></svg>
                </div>
                <div class="kpi-value" id="stat-uptime">00:00:00</div>
                <div class="kpi-subtext">
                    <span>Pool: <span id="stat-ping">Connecting...</span></span>
                </div>
            </div>
        </section>

        <!-- Main Workspace -->
        <section class="workspace-grid">
            
            <!-- Left Side: Live Logs & Graph -->
            <div class="panel">
                <div class="panel-header">
                    <div class="panel-title">
                        <div class="panel-title-icon"></div>
                        <span>Mining Spectrum & Activity Console</span>
                    </div>
                    <button class="btn-reboot" onclick="restartMiner()">Reboot Rig</button>
                </div>
                <div class="panel-content">
                    <div class="canvas-container">
                        <canvas id="liveGraph"></canvas>
                    </div>
                    <div class="terminal-container" id="terminal-console">
                        <div class="terminal-row">
                            <span class="term-time">[00:00:00]</span>
                            <span class="term-tag sys">SYS</span>
                            <span class="term-msg">Duino-Coin Hashing Core initialized successfully.</span>
                        </div>
                    </div>
                </div>
            </div>

            <!-- Right Side: Config & Sys Info -->
            <div class="panel">
                <div class="panel-header">
                    <div class="panel-title">
                        <div class="panel-title-icon secondary"></div>
                        <span>Duino-Coin Miner Settings</span>
                    </div>
                </div>
                <div class="panel-content">
                    <form id="settings-form" class="settings-form" onsubmit="event.preventDefault(); saveSettings();">
                        
                        <div class="input-row">
                            <div class="input-group">
                                <label class="input-label">DUCO Username</label>
                                <input type="text" class="form-input" id="cfg-duco-user" placeholder="bugkash" required>
                            </div>
                            <div class="input-group">
                                <label class="input-label">Difficulty</label>
                                <input type="text" class="form-input" id="cfg-difficulty" placeholder="ESP8266" required>
                            </div>
                        </div>

                        <div class="input-row">
                            <div class="input-group">
                                <label class="input-label">Mining Key</label>
                                <input type="text" class="form-input" id="cfg-miner-key" placeholder="None">
                            </div>
                            <div class="input-group">
                                <label class="input-label">Rig Identifier</label>
                                <input type="text" class="form-input" id="cfg-rig-id" placeholder="ESP8266_Rig" required>
                            </div>
                        </div>

                        <div class="input-row">
                            <div class="input-group">
                                <label class="input-label">Wi-Fi SSID</label>
                                <input type="text" class="form-input" id="cfg-ssid" placeholder="Wi-Fi SSID" required>
                            </div>
                            <div class="input-group">
                                <label class="input-label">Wi-Fi Password</label>
                                <input type="password" class="form-input" id="cfg-pass" placeholder="••••••••">
                            </div>
                        </div>

                        <div class="input-group">
                            <label class="input-label">LED Indicator Control</label>
                            <div class="input-wrapper">
                                <select class="form-input" id="cfg-led">
                                    <option value="0">Off (Stealth Mode)</option>
                                    <option value="1">Solid On</option>
                                    <option value="2">Flash on share update</option>
                                    <option value="3">Pulse slowly (Ambient Fade)</option>
                                    <option value="4">Flash every 5 seconds</option>
                                </select>
                            </div>
                        </div>

                        <button type="submit" class="btn-glow" id="save-btn">
                            <svg width="14" height="14" viewBox="0 0 24 24" fill="none" stroke="currentColor" stroke-width="2.5"><path d="M19 21H5a2 2 0 0 1-2-2V5a2 2 0 0 1 2-2h11l5 5v11a2 2 0 0 1-2 2z"/><polyline points="17 21 17 13 7 13 7 21"/><polyline points="7 3 7 8 15 8"/></svg>
                            Apply Settings
                        </button>
                    </form>

                    <h3 class="input-label" style="margin-top: 24px; margin-bottom: 10px;">System Parameters</h3>
                    <table class="sys-info-table">
                        <tr>
                            <td>Firmware ID</td>
                            <td id="sys-fw">Antigravity-DUCO Core v3.0.0</td>
                        </tr>
                        <tr>
                            <td>Core Chipset</td>
                            <td>ESP8266EX (NodeMCU v2)</td>
                        </tr>
                        <tr>
                            <td>CPU Frequency</td>
                            <td>160 MHz (Overclocked)</td>
                        </tr>
                        <tr>
                            <td>IP Address</td>
                            <td id="sys-ip">0.0.0.0</td>
                        </tr>
                        <tr>
                            <td>Wi-Fi Link</td>
                            <td id="sys-wifi">None</td>
                        </tr>
                        <tr>
                            <td>Free SRAM Heap</td>
                            <td id="sys-heap">0 KB</td>
                        </tr>
                    </table>
                </div>
            </div>
        </section>
    </main>

    <footer>
        Duino-Coin Quantum Core Miner | Powered by <span>Antigravity AI</span>
    </footer>
    
    <!-- Notification -->
    <div class="toast-notification" id="toast">
        <div class="toast-icon">✓</div>
        <div class="toast-text">
            <h4 id="toast-title">Core Synchronized</h4>
            <p id="toast-desc">Configuration applied successfully.</p>
        </div>
    </div>

    <script>
        // Web Audio Context Synthesizer
        let audioCtx = null;
        function playSound(frequency, duration, type = 'sine', volume = 0.05) {
            try {
                if (!audioCtx) {
                    audioCtx = new (window.AudioContext || window.webkitAudioContext)();
                }
                if (audioCtx.state === 'suspended') {
                    audioCtx.resume();
                }
                const osc = audioCtx.createOscillator();
                const gain = audioCtx.createGain();
                
                osc.type = type;
                osc.frequency.setValueAtTime(frequency, audioCtx.currentTime);
                
                gain.gain.setValueAtTime(volume, audioCtx.currentTime);
                gain.gain.exponentialRampToValueAtTime(0.0001, audioCtx.currentTime + duration);
                
                osc.connect(gain);
                gain.connect(audioCtx.destination);
                
                osc.start();
                osc.stop(audioCtx.currentTime + duration);
            } catch (e) {
                console.log("Audio disabled");
            }
        }

        // Play click sound
        function playClick() {
            playSound(1200, 0.05, 'triangle', 0.05);
            playSound(600, 0.08, 'sine', 0.03);
        }
        
        function playSuccessSound() {
            playSound(523.25, 0.1, 'sine', 0.05); // C5
            setTimeout(() => playSound(659.25, 0.1, 'sine', 0.05), 100); // E5
            setTimeout(() => playSound(783.99, 0.15, 'sine', 0.05), 200); // G5
            setTimeout(() => playSound(1046.50, 0.25, 'sine', 0.08), 300); // C6
        }

        // Attach clicks to buttons and inputs
        document.querySelectorAll('button, select, input').forEach(el => {
            el.addEventListener('click', () => playClick());
            el.addEventListener('focus', () => playSound(1000, 0.02, 'sine', 0.01));
        });

        // Graphing System
        const canvas = document.getElementById('liveGraph');
        const ctx = canvas.getContext('2d');
        let hashrateHistory = Array(40).fill(0);
        
        function resizeCanvas() {
            canvas.width = canvas.parentElement.clientWidth;
            canvas.height = canvas.parentElement.clientHeight;
        }
        window.addEventListener('resize', resizeCanvas);
        resizeCanvas();

        function drawGraph() {
            ctx.clearRect(0, 0, canvas.width, canvas.height);
            
            // Draw grid lines
            ctx.strokeStyle = 'rgba(255, 255, 255, 0.02)';
            ctx.lineWidth = 1;
            for(let i = 0; i < canvas.width; i += 40) {
                ctx.beginPath();
                ctx.moveTo(i, 0);
                ctx.lineTo(i, canvas.height);
                ctx.stroke();
            }
            for(let i = 0; i < canvas.height; i += 30) {
                ctx.beginPath();
                ctx.moveTo(0, i);
                ctx.lineTo(canvas.width, i);
                ctx.stroke();
            }
            
            const maxVal = Math.max(...hashrateHistory, 1);
            const step = canvas.width / (hashrateHistory.length - 1);
            
            // Create gradient
            const gradient = ctx.createLinearGradient(0, 0, 0, canvas.height);
            gradient.addColorStop(0, 'rgba(255, 170, 0, 0.4)'); // Amber Gold
            gradient.addColorStop(1, 'rgba(171, 0, 255, 0.0)'); // Purple fade

            // Area path
            ctx.beginPath();
            ctx.moveTo(0, canvas.height);
            for(let i = 0; i < hashrateHistory.length; i++) {
                const x = i * step;
                const y = canvas.height - (hashrateHistory[i] / maxVal) * (canvas.height - 40) - 20;
                ctx.lineTo(x, y);
            }
            ctx.lineTo(canvas.width, canvas.height);
            ctx.closePath();
            ctx.fillStyle = gradient;
            ctx.fill();

            // Line path
            ctx.beginPath();
            for(let i = 0; i < hashrateHistory.length; i++) {
                const x = i * step;
                const y = canvas.height - (hashrateHistory[i] / maxVal) * (canvas.height - 40) - 20;
                if(i === 0) ctx.moveTo(x, y);
                else ctx.lineTo(x, y);
            }
            
            // Draw gold glowing stroke
            ctx.strokeStyle = '#ffaa00';
            ctx.lineWidth = 3;
            ctx.shadowColor = 'rgba(255, 170, 0, 0.5)';
            ctx.shadowBlur = 10;
            ctx.stroke();
            ctx.shadowBlur = 0; // reset
        }

        // Toast Notification
        function showToast(title, desc, isSuccess = true) {
            const toast = document.getElementById('toast');
            document.getElementById('toast-title').innerText = title;
            document.getElementById('toast-desc').innerText = desc;
            
            const icon = toast.querySelector('.toast-icon');
            if (isSuccess) {
                icon.innerHTML = '✓';
                icon.style.color = 'var(--success)';
                toast.style.borderColor = 'var(--primary)';
                playSuccessSound();
            } else {
                icon.innerHTML = '✕';
                icon.style.color = 'var(--warning)';
                toast.style.borderColor = 'var(--warning)';
                playSound(300, 0.4, 'sawtooth', 0.05);
            }
            
            toast.classList.add('show');
            setTimeout(() => {
                toast.classList.remove('show');
            }, 4000);
        }

        // Balance Monitoring System
        let initialBalance = null;
        let currentBalance = 0.0;
        let balanceFetchCounter = 0;
        
        function fetchDucoBalance(username) {
            if (!username) return;
            fetch(`https://server.duinocoin.com/balances/${username}`)
                .then(response => response.json())
                .then(data => {
                    let balance = 0.0;
                    if (data.success && data.result) {
                        balance = parseFloat(data.result.balance);
                    } else if (data.balance) {
                        balance = parseFloat(data.balance);
                    } else if (data.result && typeof data.result === 'number') {
                        balance = parseFloat(data.result);
                    }
                    
                    if (initialBalance === null) {
                        initialBalance = balance;
                    }
                    
                    currentBalance = balance;
                    const earned = currentBalance - initialBalance;
                    
                    document.getElementById('stat-balance').innerHTML = `${currentBalance.toFixed(4)} <span class="kpi-unit">DUCO</span>`;
                    document.getElementById('subtext-balance').innerHTML = `Session Earned: <span class="success">+${earned.toFixed(6)} DUCO</span>`;
                })
                .catch(err => {
                    console.error("Duco balance API error:", err);
                });
        }

        // Fetch Live Stats from ESP8266 Server
        let lastUptimeSecs = 0;
        function updateStats() {
            fetch('/api/stats')
                .then(response => response.json())
                .then(data => {
                    // Query balance every 15 seconds dynamically from user browser
                    if (data.cfg_duco_user && (balanceFetchCounter % 15 === 0)) {
                        fetchDucoBalance(data.cfg_duco_user);
                    }
                    balanceFetchCounter++;

                    // Update KPI numbers
                    let hashrateStr = "0.00";
                    let unit = "H/s";
                    if (data.hashrate > 1000) {
                        hashrateStr = (data.hashrate / 1000).toFixed(2);
                        unit = "kH/s";
                    } else {
                        hashrateStr = data.hashrate.toFixed(2);
                    }
                    document.getElementById('stat-hashrate').innerHTML = `${hashrateStr} <span class="kpi-unit">${unit}</span>`;
                    
                    document.getElementById('stat-difficulty').innerText = data.difficulty || "ESP8266";
                    
                    // Shares and efficiency
                    const total = data.accepted + data.rejected;
                    const pct = total > 0 ? ((data.accepted / total) * 100).toFixed(1) : "100.0";
                    document.getElementById('stat-accepted').innerHTML = `${data.accepted} <span class="kpi-unit">(${pct}% ok)</span>`;
                    document.getElementById('subtext-shares').innerText = `${data.accepted} shares accepted by pool`;
                    
                    document.getElementById('stat-uptime').innerText = data.uptime;
                    document.getElementById('stat-ping').innerText = `${data.pool_name || "Assigned Pool"} (${data.ping} ms)`;
                    
                    const badge = document.getElementById('miner-badge');
                    const badgeText = document.getElementById('badge-mode-text');
                    
                    if (data.connected) {
                        badgeText.innerText = "POOL STRATUM ACTIVE";
                        badge.className = "badge active";
                    } else {
                        badgeText.innerText = data.status.toUpperCase();
                        if (data.status.toLowerCase().includes('connect') || data.status.toLowerCase().includes('wifi')) {
                            badgeText.innerText = "POOL STRATUM SYNCING";
                            badge.className = "badge";
                        } else {
                            badge.className = "badge";
                        }
                    }
                    
                    // Render logs to terminal
                    if (data.logs && Array.isArray(data.logs)) {
                        const consoleEl = document.getElementById('terminal-console');
                        consoleEl.innerHTML = '';
                        
                        data.logs.forEach(log => {
                            // Extract time and msg
                            const match = log.match(/^\[(\d{2}:\d{2}:\d{2})\]\s*(.*)$/);
                            let time = "00:00:00";
                            let msg = log;
                            if (match) {
                                time = match[1];
                                msg = match[2];
                            }
                            
                            let tag = "SYS";
                            let tagClass = "sys";
                            
                            if (msg.includes('accept') || msg.includes('GOOD') || msg.includes('connected') || msg.includes('operational')) {
                                tag = "OK";
                                tagClass = "ok";
                            } else if (msg.includes('reject') || msg.includes('error') || msg.includes('failed') || msg.includes('exhausted') || msg.includes('BAD') || msg.includes('exhaust')) {
                                tag = "ERR";
                                tagClass = "err";
                            } else {
                                tag = "SYS";
                                tagClass = "sys";
                            }

                            const row = document.createElement('div');
                            row.className = 'terminal-row';
                            row.innerHTML = `
                                <span class="term-time">[${time}]</span>
                                <span class="term-tag ${tagClass}">${tag}</span>
                                <span class="term-msg">${msg}</span>
                            `;
                            consoleEl.appendChild(row);
                        });
                        
                        consoleEl.scrollTop = consoleEl.scrollHeight;
                    }
                    
                    // Update system info table
                    document.getElementById('sys-ip').innerText = data.pool_ip || "0.0.0.0"; // using pool_ip fallback or if we expose local IP
                    if (data.wifi_ssid) {
                        document.getElementById('sys-wifi').innerText = `${data.wifi_ssid} (${data.wifi_rssi} dBm)`;
                    }
                    document.getElementById('sys-heap').innerText = `${Math.round(data.free_heap / 1024)} KB`;
                    
                    // Populate configuration inputs on first load
                    if (lastUptimeSecs === 0) {
                        if (data.cfg_duco_user) document.getElementById('cfg-duco-user').value = data.cfg_duco_user;
                        if (data.cfg_difficulty) document.getElementById('cfg-difficulty').value = data.cfg_difficulty;
                        if (data.cfg_miner_key) document.getElementById('cfg-miner-key').value = data.cfg_miner_key;
                        if (data.cfg_rig_id) document.getElementById('cfg-rig-id').value = data.cfg_rig_id;
                        if (data.cfg_ssid) document.getElementById('cfg-ssid').value = data.cfg_ssid;
                        if (data.cfg_led !== undefined) document.getElementById('cfg-led').value = data.cfg_led;
                    }
                    lastUptimeSecs = 1;
                    
                    // Push to graph history
                    hashrateHistory.push(data.hashrate);
                    hashrateHistory.shift();
                    drawGraph();
                })
                .catch(err => {
                    console.error("API error", err);
                });
        }

        // Submit Configurations
        function saveSettings() {
            const ssid = document.getElementById('cfg-ssid').value;
            const pass = document.getElementById('cfg-pass').value;
            const duco_user = document.getElementById('cfg-duco-user').value;
            const miner_key = document.getElementById('cfg-miner-key').value;
            const rig_id = document.getElementById('cfg-rig-id').value;
            const difficulty = document.getElementById('cfg-difficulty').value;
            const led = document.getElementById('cfg-led').value;
            
            const btn = document.getElementById('save-btn');
            btn.disabled = true;
            btn.innerHTML = `
                <svg style="animation: spin-logo 2s linear infinite" width="14" height="14" viewBox="0 0 24 24" fill="none" stroke="currentColor" stroke-width="2.5"><line x1="12" y1="2" x2="12" y2="6"/><line x1="12" y1="18" x2="12" y2="22"/><line x1="4.93" y1="4.93" x2="7.76" y2="7.76"/><line x1="16.24" y1="16.24" x2="19.07" y2="19.07"/><line x1="2" y1="12" x2="6" y2="12"/><line x1="18" y1="12" x2="22" y2="12"/><line x1="4.93" y1="19.07" x2="7.76" y2="16.24"/><line x1="16.24" y1="7.76" x2="19.07" y2="4.93"/></svg>
                Synchronizing...
            `;
            
            const body = new URLSearchParams();
            body.append('ssid', ssid);
            body.append('password', pass);
            body.append('duco_user', duco_user);
            body.append('miner_key', miner_key);
            body.append('rig_id', rig_id);
            body.append('difficulty', difficulty);
            body.append('led', led);
            
            fetch('/api/save', {
                method: 'POST',
                headers: {
                    'Content-Type': 'application/x-www-form-urlencoded'
                },
                body: body.toString()
            })
            .then(res => res.json())
            .then(data => {
                btn.disabled = false;
                btn.innerHTML = `
                    <svg width="14" height="14" viewBox="0 0 24 24" fill="none" stroke="currentColor" stroke-width="2.5"><path d="M19 21H5a2 2 0 0 1-2-2V5a2 2 0 0 1 2-2h11l5 5v11a2 2 0 0 1-2 2z"/><polyline points="17 21 17 13 7 13 7 21"/><polyline points="7 3 7 8 15 8"/></svg>
                    Apply Settings
                `;
                
                if (data.status === 'ok') {
                    showToast('Core Synchronized', 'New configuration loaded. The rig is rebooting to apply changes...', true);
                } else {
                    showToast('Sync Failed', data.message || 'Verification rejected.', false);
                }
            })
            .catch(err => {
                btn.disabled = false;
                btn.innerHTML = `
                    <svg width="14" height="14" viewBox="0 0 24 24" fill="none" stroke="currentColor" stroke-width="2.5"><path d="M19 21H5a2 2 0 0 1-2-2V5a2 2 0 0 1 2-2h11l5 5v11a2 2 0 0 1-2 2z"/><polyline points="17 21 17 13 7 13 7 21"/><polyline points="7 3 7 8 15 8"/></svg>
                    Apply Settings
                `;
                showToast('Sync Failed', 'Network connection timed out.', false);
            });
        }

        async function restartMiner() {
            if (confirm("Are you sure you want to reboot the ESP8266 miner rig?")) {
                try {
                    await fetch('/api/restart', { method: 'POST' });
                    showToast("Rig Rebooting", "Reboot command sent. The board is restarting...", true);
                } catch (err) {
                    showToast("Error", "Error sending reboot command: " + err, false);
                }
            }
        }

        // Initialize and schedule
        setInterval(updateStats, 1000);
        updateStats();
    </script>
</body>
</html>
)rawliteral";

#endif
