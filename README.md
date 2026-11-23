# Satellite Telemetry Checker

A small C command-line tool that reads simulated satellite sensor data from a text file. It prints a warning whenever the temperature falls outside −20 to 60 °C or the battery voltage drops below 3.3 V.

## Run the example

```sh
cc -std=c11 -Wall -Wextra -Wpedantic telemetry.c -o telemetry
./telemetry sample-telemetry.txt
```

Each row contains the elapsed time in seconds, the temperature in Celsius, and the battery voltage. The sample file has six readings, three of which trigger alerts.

The program returns `0` when all readings are in range, `1` when it finds an alert, and `2` when there is an input error.
