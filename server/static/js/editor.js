const TEMPLATES = {
    blink: `#include "Led.h"
#include "MCU.h"

int main(void) {
    core::MCU::init();
    component::Led led(io::Pin(0, io::PortB));

    while (1) {
        led.on();
        _delay_ms(500);
        led.off();
        _delay_ms(500);
    }
    return 0;
}`,
    telemetry: `#include "Led.h"
#include "ADC.h"
#include "Telemetry.h"
#include "MCU.h"

int main(void) {
    core::MCU::init();
    io::USART0 &usart = io::USART0::getInstance();
    io::Telemetry::init(usart);

    core::ADConverter &adc = core::ADConverter::getInstance(
        core::referenceVoltage::AVCC,
        core::clockPrescaler::PS_128,
        core::autoTriggerSource::freeRunning,
        io::Pin(0, io::PortC)
    );
    adc.start();

    while (1) {
        uint16_t val = adc.getConversionResult(core::resolution::res_10bit);
        io::Telemetry::sendADC(0, val);
        _delay_ms(100);
    }
    return 0;
}`
};

let compiledHex = null;
let authToken = null;

function log(msg) {
    const term = document.getElementById('terminal');
    const line = document.createElement('div');
    line.textContent = `[${new Date().toLocaleTimeString()}] ${msg}`;
    term.appendChild(line);
    term.scrollTop = term.scrollHeight;
}

async function fetchAuthToken() {
    try {
        const res = await fetch('/api/token');
        if (res.ok) {
            const data = await res.json();
            authToken = data.token;
        }
    } catch (e) {
        console.error('Failed to fetch auth token:', e);
    }
}

function loadTemplate(name) {
    if (TEMPLATES[name]) {
        document.getElementById('code-editor').value = TEMPLATES[name];
        log(`Loaded template: ${name}`);
    }
}

async function fetchPorts() {
    try {
        const res = await fetch('/api/ports');
        const data = await res.json();
        const sel = document.getElementById('port-select');
        sel.textContent = ''; // Safe clear

        const autoOpt = document.createElement('option');
        autoOpt.value = '';
        autoOpt.textContent = 'Auto-Detect';
        sel.appendChild(autoOpt);

        if (Array.isArray(data.ports)) {
            data.ports.forEach(p => {
                const opt = document.createElement('option');
                opt.value = p;
                opt.textContent = p;
                sel.appendChild(opt);
            });
        }
    } catch (e) {
        console.error('Failed to fetch ports:', e);
    }
}

function getAuthHeaders() {
    const headers = { 'Content-Type': 'application/json' };
    if (authToken) {
        headers['X-Auth-Token'] = authToken;
    }
    return headers;
}

async function compileCode() {
    const code = document.getElementById('code-editor').value;
    const mcu = document.getElementById('mcu-select').value;
    document.getElementById('build-status').textContent = 'Compiling...';
    log(`Compiling for ${mcu}...`);

    try {
        const res = await fetch('/api/compile', {
            method: 'POST',
            headers: getAuthHeaders(),
            body: JSON.stringify({ code, mcu })
        });
        const data = await res.json();
        if (data.success) {
            compiledHex = data.hex;
            document.getElementById('build-status').textContent = 'Build Success';
            log('Build succeeded!');
        } else {
            compiledHex = null;
            document.getElementById('build-status').textContent = 'Build Failed';
            log(`Build failed: ${data.stderr || data.error || 'Unknown error'}`);
        }
    } catch (e) {
        log(`Error: ${e.message || e}`);
    }
}

async function flashCode() {
    if (!compiledHex) {
        await compileCode();
        if (!compiledHex) return;
    }
    const mcu = document.getElementById('mcu-select').value;
    const port = document.getElementById('port-select').value;
    log(`Flashing to ${mcu} on port ${port || 'auto'}...`);

    try {
        const res = await fetch('/api/flash', {
            method: 'POST',
            headers: getAuthHeaders(),
            body: JSON.stringify({ hex: compiledHex, mcu, port })
        });
        const data = await res.json();
        if (data.success) {
            log(`Flashed successfully on ${data.port}!`);
        } else {
            log(`Flash failed: ${data.stderr || data.error || 'Unknown error'}`);
        }
    } catch (e) {
        log(`Error: ${e.message || e}`);
    }
}

async function runSimulation() {
    const code = document.getElementById('code-editor').value;
    log('Running host simulation...');
    try {
        const res = await fetch('/api/simulate', {
            method: 'POST',
            headers: getAuthHeaders(),
            body: JSON.stringify({ code })
        });
        const data = await res.json();
        if (data.success) {
            log(`Host Simulation output: ${data.stdout || ''}`);
        } else {
            log(`Simulation error: ${data.error || ''}`);
        }
    } catch (e) {
        log(`Error: ${e.message || e}`);
    }
}

// Initial setup
window.onload = async () => {
    await fetchAuthToken();
    loadTemplate('blink');
    fetchPorts();
};
