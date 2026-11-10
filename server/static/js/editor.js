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

function log(msg) {
    const term = document.getElementById('terminal');
    term.innerHTML += `<br>[${new Date().toLocaleTimeString()}] ${msg}`;
    term.scrollTop = term.scrollHeight;
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
        sel.innerHTML = '<option value="">Auto-Detect</option>';
        data.ports.forEach(p => {
            sel.innerHTML += `<option value="${p}">${p}</option>`;
        });
    } catch (e) {
        console.error(e);
    }
}

async function compileCode() {
    const code = document.getElementById('code-editor').value;
    const mcu = document.getElementById('mcu-select').value;
    document.getElementById('build-status').innerText = 'Compiling...';
    log(`Compiling for ${mcu}...`);

    try {
        const res = await fetch('/api/compile', {
            method: 'POST',
            headers: { 'Content-Type': 'application/json' },
            body: JSON.stringify({ code, mcu })
        });
        const data = await res.json();
        if (data.success) {
            compiledHex = data.hex;
            document.getElementById('build-status').innerText = 'Build Success';
            log('✨ Build succeeded!');
        } else {
            compiledHex = null;
            document.getElementById('build-status').innerText = 'Build Failed';
            log(`❌ Build failed:<br><pre>${data.stderr}</pre>`);
        }
    } catch (e) {
        log(`Error: ${e}`);
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
            headers: { 'Content-Type': 'application/json' },
            body: JSON.stringify({ hex: compiledHex, mcu, port })
        });
        const data = await res.json();
        if (data.success) {
            log(`⚡ Flashed successfully on ${data.port}!`);
        } else {
            log(`❌ Flash failed:<br><pre>${data.stderr}</pre>`);
        }
    } catch (e) {
        log(`Error: ${e}`);
    }
}

async function runSimulation() {
    const code = document.getElementById('code-editor').value;
    log('Running host simulation...');
    try {
        const res = await fetch('/api/simulate', {
            method: 'POST',
            headers: { 'Content-Type': 'application/json' },
            body: JSON.stringify({ code })
        });
        const data = await res.json();
        if (data.success) {
            log(`Host Simulation output:<br><pre>${data.stdout}</pre>`);
        } else {
            log(`Simulation error:<br><pre>${data.error}</pre>`);
        }
    } catch (e) {
        log(`Error: ${e}`);
    }
}

// Initial setup
window.onload = () => {
    loadTemplate('blink');
    fetchPorts();
};
