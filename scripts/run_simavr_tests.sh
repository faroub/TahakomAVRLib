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
    set +e
    # Run simavr bounded by a 5-second timeout so mainloop execution does not hang CI
    sim_output=$(timeout 5s simavr -m atmega328p -f 16000000 "${elf}" 2>&1)
    sim_rc=$?
    set -e

    if echo "${sim_output}" | grep -iqE "crashed|trap|fatal|illegal instruction|unable to load"; then
        echo "FAILED: ${app} simulation crashed or trapped in simavr:"
        echo "${sim_output}"
        FAILED=1
    elif [ "${sim_rc}" -eq 0 ] || [ "${sim_rc}" -eq 124 ]; then
        echo "PASSED: ${app} executed cleanly in simavr (5s mainloop test passed)"
    else
        echo "FAILED: ${app} simulation exited with error code ${sim_rc}:"
        echo "${sim_output}"
        FAILED=1
    fi
done

if [ "${FAILED}" -ne 0 ]; then
    echo "ERROR: One or more simavr hardware simulations failed!"
    exit 1
fi

echo "OK: All application simulations completed successfully"
exit 0
