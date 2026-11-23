// Build: cc -std=c11 -Wall -Wextra -Wpedantic telemetry.c -o telemetry
// Run: ./telemetry sample-telemetry.txt
// Input: one whitespace-separated row per sample: seconds temperature_C battery_V
// Exit status: 0 for normal, 1 for alerts, 2 for input errors.

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char **argv)
{
    if (argc != 2) {
        fprintf(stderr, "Usage: %s INPUT_FILE\n", argv[0]);
        return 2;
    }

    FILE *input = fopen(argv[1], "r");
    if (input == NULL) {
        perror(argv[1]);
        return 2;
    }

    const double min_temperature = -20.0;
    const double max_temperature = 60.0;
    const double min_voltage = 3.3;
    char line[256];
    size_t samples = 0;
    size_t alerts = 0;
    int status = 0;

    while (fgets(line, sizeof line, input) != NULL) {
        double seconds, temperature, voltage;
        char extra;
        if ((strchr(line, '\n') == NULL && !feof(input)) ||
            sscanf(line, "%lf %lf %lf %c", &seconds, &temperature, &voltage,
                   &extra) != 3 ||
            !isfinite(seconds) || !isfinite(temperature) || !isfinite(voltage) ||
            seconds < 0.0 || voltage < 0.0) {
            fprintf(stderr, "Invalid telemetry on line %zu\n", samples + 1);
            status = 2;
            break;
        }

        ++samples;
        int temperature_alert = temperature < min_temperature ||
                                temperature > max_temperature;
        int battery_alert = voltage < min_voltage;

        if (temperature_alert || battery_alert) {
            ++alerts;
            printf("ALERT at %.1f s\n", seconds);
            if (temperature_alert) {
                printf("  Temperature out of range (%.1f C)\n", temperature);
            }
            if (battery_alert) {
                printf("  Battery voltage low (%.2f V)\n", voltage);
            }
        }
    }

    if (ferror(input)) {
        perror(argv[1]);
        status = 2;
    }
    fclose(input);

    if (status == 2) {
        return status;
    }

    printf("%zu samples checked, %zu with alerts\n", samples, alerts);
    return alerts > 0 ? 1 : 0;
}
