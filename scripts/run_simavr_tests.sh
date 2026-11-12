#!/usr/bin/env bash
#
# Hardware Simulation Test Runner for TahakomAVRLib using simavr.
#

set -euo pipefail

cd "$(dirname "$0")/.." || exit 1

if ! command -v simavr &> /dev/null; then
    echo "NOTICE: simavr is not installed on this host. Skipping simavr execution."
    echo "In GitHub Actions CI, simavr will be installed via apt-get."
    exit 0
fi

echo "--- Building all applications for simulation ---"
scripts/check_build.sh

APPS="BlinkLed LightMeter MusicBox Organ WeakIsr"
FAILED=0

for app in ${APPS}; do
    elf="scripts/.build/${app}/${app}.elf"
    if [ ! -f "${elf}" ]; then
        echo "ERROR: ${elf} not found!"
        FAILED=1
        continue
    fi

    echo "--- Simulating ${app} with simavr ---"
    # Run simavr for 1 second (16000000 cycles) in headless mode
    if simavr -m atmega328p -f 16000000 -n 16000000 "${elf}" > /dev/null 2>&1; then
        echo "PASSED: ${app} executed cleanly in simavr"
    else
        echo "FAILED: ${app} simulation failed or trapped in simavr"
        FAILED=1
    fi
done

if [ "${FAILED}" -ne 0 ]; then
    echo "ERROR: One or more simavr hardware simulations failed!"
    exit 1
fi

echo "OK: All application simulations completed successfully"
exit 0
