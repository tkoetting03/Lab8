#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

static void trim(char *s) {
    char *p = s, *q = s + strlen(s);
    while (p < q && isspace((unsigned char)*p)) p++;
    while (q > p && isspace((unsigned char)q[-1])) q--;
    size_t n = (size_t)(q - p);
    memmove(s, p, n);
    s[n] = '\0';
}

static int read_line(const char *prompt, char *buf, size_t n) {
    printf("%s", prompt);
    if (!fgets(buf, n, stdin)) return 0;
    trim(buf);
    return 1;
}

static int parse_double(const char *s, double *out) {
    char *end = NULL;
    double v = strtod(s, &end);
    if (end == s) return 0;                      // no number
    while (*end && isspace((unsigned char)*end)) end++;
    if (*end) return 0;                          // leftover junk
    *out = v;
    return 1;
}

static int parse_scale(const char *s, char *out) {
    if (strlen(s) != 1) return 0;
    char c = (char)toupper((unsigned char)s[0]);
    if (c=='C' || c=='F' || c=='K') { *out = c; return 1; }
    return 0;
}

static double to_c(double v, char s) {
    switch (s) {
        case 'C': return v;
        case 'F': return (v - 32.0) * (5.0/9.0);
        case 'K': return v - 273.15;
        default:  return 0.0; // unreachable
    }
}

static double from_c(double c, char t) {
    switch (t) {
        case 'C': return c;
        case 'F': return c * (9.0/5.0) + 32.0;
        case 'K': return c + 273.15;
        default:  return 0.0; // unreachable
    }
}

int main(void) {
    char buf[256], sbuf[256];
    double value;
    char in_scale, out_scale;

    // Read temperature value
    for (;;) {
        if (!read_line("Enter the temperature value: ", buf, sizeof buf)) return 0;
        if (parse_double(buf, &value)) break;
        puts("Invalid temperature. Enter a numeric value (e.g., 21.5).");
    }

    // Read original scale
    for (;;) {
        if (!read_line("Enter the original scale (C, F, or K): ", sbuf, sizeof sbuf)) return 0;
        if (parse_scale(sbuf, &in_scale)) break;
        puts("Invalid scale. Enter exactly one of: C, F, K.");
    }

    // Physical validity (absolute zero)
    if ((in_scale=='C' && value < -273.15) ||
        (in_scale=='F' && value < -459.67) ||
        (in_scale=='K' && value < 0.0)) {
        puts("Invalid input: below absolute zero for the given scale.");
        return 0;
    }

    // Read target scale
    for (;;) {
        if (!read_line("Enter the scale to convert to (C, F, or K): ", sbuf, sizeof sbuf)) return 0;
        if (parse_scale(sbuf, &out_scale)) break;
        puts("Invalid scale. Enter exactly one of: C, F, K.");
    }

    // Convert and categorize based on Celsius
    double c = to_c(value, in_scale);
    double ans = from_c(c, out_scale);

    const char *category, *advisory;
    if (c < 0.0) {
        category = "Freezing";
        advisory = "Stay warm and watch for ice.";
    } else if (c < 10.0) {
        category = "Cold";
        advisory = "Wear a jacket.";
    } else if (c < 25.0) {
        category = "Comfortable";
        advisory = "Nice day—enjoy it!";
    } else if (c < 35.0) {
        category = "Hot";
        advisory = "Drink lots of water!";
    } else {
        category = "Extreme Heat";
        advisory = "Stay indoors if possible.";
    }

    printf("Converted temperature: %.2f %c\n", ans, out_scale);
    printf("Temperature category: %s\n", category);
    printf("Weather advisory: %s\n", advisory);
    return 0;
}