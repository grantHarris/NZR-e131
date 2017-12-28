#define WIFI_LED_GPIO 4
#define E131_DATA_LED_GPIO 4
#define RECPLAY_RED_LED_GPIO 4
#define RECPLAY_GREEN_LED_GPIO 4
#define MODE_RED_LED_GPIO 4
#define MODE_GREEN_LED_GPIO 4

class Indicators {
    public:
        static Indicators * const indicators;
    private:
        Indicators(void) {}
};
